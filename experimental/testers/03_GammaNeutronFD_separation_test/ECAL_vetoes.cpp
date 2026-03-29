// source gammaNeutronFD_separation_test_run.sh

#ifndef ECAL_VETOES
#define ECAL_VETOES

// #include "newVeto_functions_original.cpp"
#include "newVeto_functions.cpp"

// #include <TCanvas.h>
// #include <TChain.h>
// #include <TFile.h>
// #include <TH1.h>
// #include <TH2.h>
// #include <TLatex.h>
// #include <TLorentzVector.h>
// #include <TStyle.h>
// #include <TTree.h>

// #include <chrono>
// #include <cmath>
// #include <cstdlib>
// #include <iostream>
// #include <string>
// #include <tuple>
// #include <typeinfo>
// #include <vector>

// // Include libraries:
// #include "../../framework/namespaces/general_utilities/utilities.h"
// #include "../../framework/namespaces/setup/path_definitions.h"

// // Include classes:
// #include "../../framework/classes/AMaps/AMaps.cpp"
// #include "../../framework/classes/NeutronVetoRon/ChargedHit.h"
// #include "../../framework/classes/NeutronVetoRon/NeutralHit.h"
// #include "../../framework/classes/ParticleID/ParticleID.cpp"
// #include "../../framework/classes/clas12ana/clas12ana.h"

// // Include CLAS12 libraries:
// #include "../../framework/includes/clas12_include.h"

// using namespace utilities;
// using std::string;
// using std::vector;

#pragma region /* Set_dr_n_Slices function */

vector<vector<double>> Set_dr_n_Slices(const double& Delta, const double& LowerLim = 0., const double& CutOff = 300.) {
    bool PrintOut = false;

    vector<vector<double>> dr_n_SliceLimits;

    double SliceLowerLim = LowerLim;
    const double SliceUpperLim = 9999.;

    while (SliceLowerLim < CutOff) {
        dr_n_SliceLimits.push_back({SliceLowerLim, SliceUpperLim});

        SliceLowerLim += Delta;
    }

    return dr_n_SliceLimits;
}
#pragma endregion

#pragma region /* Set_Mom_Res_Slices function */

vector<vector<double>> Set_Mom_Res_Slices(const double& beamE, const double& ParticleMomTh, const double& Delta) {
    bool PrintOut = false;

    vector<vector<double>> MomResSliceLimits;

    double SliceLowerLim = ParticleMomTh;
    const double SliceUpperMomLim = beamE;

    while (SliceLowerLim < SliceUpperMomLim) {
        double SliceUpperLim = std::min(SliceLowerLim + Delta, SliceUpperMomLim);
        MomResSliceLimits.push_back({SliceLowerLim, SliceUpperLim});

        if (PrintOut) { std::cout << "SetMomResSlices: Adding slice: [" << SliceLowerLim << ", " << SliceUpperLim << "]\n"; }

        SliceLowerLim = SliceUpperLim;
    }

    return MomResSliceLimits;
}
#pragma endregion

#pragma region /* NeutronECAL_Cut_Veto_original_custom function */
/* NOTE: this code is valid for a single neutral only. */

/* In the input, we have:
 * const std::unique_ptr<clas12::clas12reader> &c12 - the event
 * beamE
 * index - of the particle in question (the neutral) */
bool NeutronECAL_Cut_Veto_original_custom(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, region_part_ptr neut, const double& veto_cut) {
    TVector3 p_b(0, 0, beamE); /* beam energy */

    TVector3 p_e; /* our electron */
    p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 p_q = p_b - p_e; /* 3-momentum transfer */

    if (neut->par()->getCharge() != 0) { return false; } /* determine if the particle is neutral or not */

    // Check which layers of the ECAL have been hit
    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1, neut->getTheta(), neut->getPhi()); /* calculate the angles of the neutral particle */

    /* check where did the particle hit.
     * no hit - we'll get 0
     * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
       Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison) */
    bool PC = (neut->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (neut->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (neut->cal(clas12::ECOUT)->getDetector() == 7);
    auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

    /* v_nhit = location of neutral particle hit */
    TVector3 v_nhit(neut->cal(detlayer)->getX(), neut->cal(detlayer)->getY(), neut->cal(detlayer)->getZ());
    double beta = neut->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double theta_n = am::RadToDeg(p_n_Angles.Theta());
    double theta_q = am::RadToDeg(p_q.Theta());
    double theta_nq = am::RadToDeg(p_n_Angles.Angle(p_q));

    if (beta < 0) { return false; }
    //    if (beta > 1.1) { return false; }
    //    // physics cuts, to be ignored according to Larry.
    //    if (theta_nq > 25) { return false; }
    //    if (theta_q > 40) { return false; }
    if (theta_n < 1) { return false; } /* to avoid events with theta_n = 0 (the "1" is in deg) */
    // if (!(IC || OC)) { return false; } /* hit only one of these layers */
    // if (PC) { return false; }          /* to veto out the gammas (photons) */

    // Now let's put a charge particle veto
    bool Veto = false;
    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; } /* looking on charged particles only */
        TVector3 v_chit;                                            /* v_chit = location of charged particle hit */

        if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else {
            /* the neutral has to hit either the ECIN or ECOUT.
               If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        }
    }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
#pragma endregion

#pragma region /* NeutronECAL_Cut_Veto_original_custom_2 function */
/* NOTE: this code is valid for a single neutral only. */

/* In the input, we have:
 * const std::unique_ptr<clas12::clas12reader> &c12 - the event
 * beamE
 * index - of the particle in question (the neutral) */
bool NeutronECAL_Cut_Veto_original_custom_2(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, NeutralHit neut, const double& veto_cut) {
    TVector3 p_b(0, 0, beamE); /* beam energy */

    TVector3 p_e; /* our electron */
    p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 p_q = p_b - p_e; /* 3-momentum transfer */

    // Check which layers of the ECAL have been hit
    double Theta_rad = am::CalcTheta_rad(neut.xHit, neut.yHit, neut.zHit);
    double Phi_rad = am::CalcPhi_rad(neut.xHit, neut.yHit);

    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1, Theta_rad, Phi_rad); /* calculate the angles of the neutral particle */

    /* check where did the particle hit.
     * no hit - we'll get 0
     * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
       Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison) */
    bool PC = neut.clas12particle_hit_in_PCAL;
    bool IC = neut.clas12particle_hit_in_ECIN;
    bool OC = neut.clas12particle_hit_in_ECOUT;
    auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

    /* v_nhit = location of neutral particle hit */
    TVector3 v_nhit(neut.xHit, neut.yHit, neut.zHit);
    // double beta = neut->par()->getBeta();
    // double gamma = 1 / sqrt(1 - (beta * beta));
    double theta_n = am::RadToDeg(p_n_Angles.Theta());
    double theta_q = am::RadToDeg(p_q.Theta());
    double theta_nq = am::RadToDeg(p_n_Angles.Angle(p_q));

    // if (beta < 0) { return false; }
    //    if (beta > 1.1) { return false; }
    //    // physics cuts, to be ignored according to Larry.
    //    if (theta_nq > 25) { return false; }
    //    if (theta_q > 40) { return false; }
    if (theta_n < 1) { return false; } /* to avoid events with theta_n = 0 (the "1" is in deg) */
    // if (!(IC || OC)) { return false; } /* hit only one of these layers */
    // if (PC) { return false; }          /* to veto out the gammas (photons) */

    // Now let's put a charge particle veto
    bool Veto = false;
    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; } /* looking on charged particles only */
        TVector3 v_chit;                                            /* v_chit = location of charged particle hit */

        if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else {
            /* the neutral has to hit either the ECIN or ECOUT.
               If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        }
    }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
#pragma endregion

#pragma region /* NeutronECAL_Cut_Veto_original function */
/* NOTE: this code is valid for a single neutral only. */

/* In the input, we have:
 * const std::unique_ptr<clas12::clas12reader> &c12 - the event
 * beamE
 * index - of the particle in question (the neutral) */
bool NeutronECAL_Cut_Veto_original(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, const int& index, const double& veto_cut) {
    TVector3 p_b(0, 0, beamE); /* beam energy */

    TVector3 p_e; /* our electron */
    p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 p_q = p_b - p_e; /* 3-momentum transfer */

    if (allParticles[index]->par()->getCharge() != 0) { return false; } /* determine if the particle is neutral or not */

    // Check which layers of the ECAL have been hit
    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1, allParticles[index]->getTheta(), allParticles[index]->getPhi()); /* calculate the angles of the neutral particle */

    /* check where did the particle hit.
     * no hit - we'll get 0
     * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
       Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison) */
    bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);
    auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

    /* v_nhit = location of neutral particle hit */
    TVector3 v_nhit(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());
    double beta = allParticles[index]->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double theta_n = am::RadToDeg(p_n_Angles.Theta());
    double theta_q = am::RadToDeg(p_q.Theta());
    double theta_nq = am::RadToDeg(p_n_Angles.Angle(p_q));

    if (beta < 0) { return false; }
    //    if (beta > 1.1) { return false; }
    //    // physics cuts, to be ignored according to Larry.
    //    if (theta_nq > 25) { return false; }
    //    if (theta_q > 40) { return false; }
    if (theta_n < 1) { return false; } /* to avoid events with theta_n = 0 (the "1" is in deg) */
    if (!(IC || OC)) { return false; } /* hit only one of these layers */
    if (PC) { return false; }          /* to veto out the gammas (photons) */

    // Now let's put a charge particle veto
    bool Veto = false;
    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; } /* looking on charged particles only */
        TVector3 v_chit;                                            /* v_chit = location of charged particle hit */

        if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else {
            /* the neutral has to hit either the ECIN or ECOUT.
               If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        }
    }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
#pragma endregion

#pragma region /* NeutronECAL_Cut_NeutVeto_custom function */
#include <cmath>
#include <iostream>

/**
 * @brief Compute the solenoidal Larmor rotation angle Δφ for the electron track.
 *
 * Given an electron track, the magnetic field B (Tesla), and effective path length L (meters),
 * this function calculates the azimuthal deflection angle due to the solenoidal field.
 *
 * Formula:
 *   Δφ = (0.299792458 * q * B * L) / (2 * p)
 * where q is the track charge in units of e, p is the momentum magnitude in GeV/c.
 *
 * The factor 0.299792458 converts (T·m) into GeV/c, and the 1/2 factor is the solenoid coupling.
 * If B(z) is non-uniform, replace B*L with the integral ∫Bz dz.
 *
 * @param electrons Vector of region_part_ptr, first element should be the electron track.
 * @param B Magnetic field strength in Tesla.
 * @param L Effective solenoid length in meters.
 * @return Computed Δφ in radians. Returns 0.0 if inputs are invalid.
 */
double Calc_Sol_dPhi(const vector<region_part_ptr>& electrons, const double& B, const double& L) {
    // Sanity checks to avoid UB / division by zero
    if (electrons.empty() || !electrons[0]) {
#ifdef DEBUG_VETO
        std::cerr << "[Calc_Sol_dPhi] electrons empty or null ptr at [0]\n";
#endif
        return 0.0;
    }
    const auto* ep = electrons[0]->par();
    if (!ep) {
#ifdef DEBUG_VETO
        std::cerr << "[Calc_Sol_dPhi] electrons[0]->par() is null\n";
#endif
        return 0.0;
    }
    /* Charged particle offset in phi due to a solenoidal field.
     *
     * Rotation angle (Larmor rotation) over length L is:
     *   Δφ = (0.299792458 * q * ∫B·dz) / (2 * p)    [radians]
     * with p in GeV/c, B in Tesla, L in meters, q in units of |e|.
     * The 0.299792458 is the GeV/c↔(T·m) conversion; the 1/2 is the solenoid coupling.
     *
     * Notes:
     * - Use the particle momentum magnitude p, not Pz (paraxial p≈pz, but p is safer).
     * - Sign comes from q * B (right-hand rule). If your coord system twists the other way,
     *   flip the sign when adding to phi.
     * - If B(z) isn’t uniform, replace B*L with the path integral ∫Bz dz.
     */

    constexpr double kTmmGeV = 0.299792458;  // 1 T·m ≈ 0.299792458 GeV/c

    const double q = static_cast<double>(ep->getCharge());  // e.g., electron: -1
    const double p = ep->getP();                            // GeV/c (total)
    if (!(p > 0.0) || !std::isfinite(p)) {
#ifdef DEBUG_VETO
        std::cerr << "[Calc_Sol_dPhi] non-physical p=" << p << "\n";
#endif
        return 0.0;
    }

    // rotation angle in radians (includes sign via q*B)
    double BL_int = B * L;                                 // replace with integral if you have B(z)
    double Sol_dPhi = (kTmmGeV * q * BL_int) / (2.0 * p);  // radians

    return Sol_dPhi;
}

/**
 * @brief Build the expected charged hit position for the neutral‑veto check.
 *
 * This function estimates where the associated charged particle (typically the
 * electron) would appear in the ECAL, after accounting for solenoidal field
 * rotation Δφ. The rotated hit position (shifted_e_unit_v) is placed on the same radius
 * as the neutral hit (v_nhit) but with the electron’s θ and φ+Δφ.
 *
 * Workflow:
 *  - Verify that v_nhit has a valid, non‑zero magnitude; otherwise return (0,0,0).
 *  - Compute Δφ from B, L, and the electron momentum using Calc_Sol_dPhi().
 *  - Use SetMagThetaPhi() with |v_nhit|, electron θ, and φ+Δφ to build shifted_e_unit_v.
 *
 * Robustness:
 *  - Returns a zero vector on invalid v_nhit input.
 *  - Emits debug messages if compiled with -DDEBUG_VETO.
 *
 * @param electrons Vector of region_part_ptr, with the first element assumed to be the electron.
 * @param v_nhit Neutral hit position in ECAL coordinates.
 * @param B Magnetic field strength in Tesla.
 * @param L The effective distance the particle travels along the solenoid axis (the z-direction)
 * @return Rotated charged‑hit vector shifted_e_unit_v.
 */
TVector3 Get_shifted_e_unit_v_NeutVeto(const vector<region_part_ptr>& electrons, const TVector3& v_nhit, const double& B, const double& L) {
    if (v_nhit.Mag() == 0.0 || !std::isfinite(v_nhit.Mag())) {
#ifdef DEBUG_VETO
        std::cerr << "[Get_shifted_e_unit_v_NeutVeto] v_nhit has zero/non-finite magnitude, returning (0,0,0)\n";
#endif
        return TVector3(0, 0, 0);
    }
    TVector3 shifted_e_unit_v;

    double Sol_dPhi = Calc_Sol_dPhi(electrons, B, L);  // radians

    shifted_e_unit_v.SetMagThetaPhi(
        v_nhit.Mag(), electrons[0]->getTheta(),
        electrons[0]->getPhi() + Sol_dPhi  // add sign-consistent rotation
                                           // shifted_e_unit_v.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(), electrons[0]->getPhi() - Sol_dPhi  // add sign-consistent rotation
                                           // shifted_e_unit_v.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(), electrons[0]->getPhi() + 3 * Sol_dPhi  // add sign-consistent rotation
                                           // shifted_e_unit_v.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(), electrons[0]->getPhi() + 0.  // add sign-consistent rotation
    );

    return shifted_e_unit_v;
}

bool NeutronECAL_Cut_NeutVeto_custom(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, region_part_ptr neut, const double& veto_cut) {
    /* The veto flag */
    bool Veto = false;

    /* Solenoid field parameters */
    double B = 5.0;  // Tesla
    double L = 0.5;  // meters

    /* 3-momentum transfer */
    TVector3 P_b(0, 0, beamE);  // Beam energy
    TVector3 P_e;               // Reco electron momentum vector
    P_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 q = P_b - P_e;  // 3-momentum transfer

    /* Safety check - determine if the particle is neutral or not */
    if (neut->par()->getCharge() != 0) { return false; }

    bool PC = (neut->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (neut->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (neut->cal(clas12::ECOUT)->getDetector() == 7);
    // if (!(IC || OC)) { return false; }                 /* must hit ECIN or ECOUT */
    // if (PC) { return false; }                          /* veto photons */
    auto detlayer = IC ? clas12::ECIN : clas12::ECOUT; /* earliest valid calorimeter layer among ECIN/ECOUT */

    // calculate the angles of the neutral particle
    TVector3 P_n_Angles;
    P_n_Angles.SetMagThetaPhi(1, neut->getTheta(), neut->getPhi());

    // v_nhit = location of neutral particle hit
    TVector3 v_nhit(neut->cal(detlayer)->getX(), neut->cal(detlayer)->getY(), neut->cal(detlayer)->getZ());
    if (v_nhit.Mag() == 0.0 || !std::isfinite(v_nhit.Mag())) { return false; }
    double Beta = neut->par()->getBeta();  // Beta of the neutral particle
    double Theta_n = am::RadToDeg(P_n_Angles.Theta());

    if (Beta < 0) { return false; }
    if (Theta_n < 1) { return false; } /* to avoid events with Theta_n = 0 (the "1" is in deg) */
    if (!std::isfinite(Theta_n)) { return false; }

#ifdef DEBUG_VETO
    {
        const double dphi = Calc_Sol_dPhi(electrons, B, L);
        std::cerr << "[NeutronECAL_Cut_NeutVeto] dphi(rad)=" << dphi << ", |v_nhit|=" << v_nhit.Mag() << ", veto_cut=" << veto_cut << "\n";
    }
#endif

    TVector3 shifted_e_unit_v = Get_shifted_e_unit_v_NeutVeto(electrons, v_nhit, B, L);

    TVector3 v_dist = v_nhit - shifted_e_unit_v;

    if (v_dist.Mag() < veto_cut) { Veto = true; }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
#pragma endregion

#pragma region /* NeutronECAL_Cut_NeutVeto_custom_2 function */
bool NeutronECAL_Cut_NeutVeto_custom_2(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, NeutralHit neut, const double& veto_cut) {
    /* The veto flag */
    bool Veto = false;

    /* Solenoid field parameters */
    double B = 5.0;  // Tesla
    double L = 0.5;  // meters

    /* 3-momentum transfer */
    TVector3 P_b(0, 0, beamE);  // Beam energy
    TVector3 P_e;               // Reco electron momentum vector
    P_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 q = P_b - P_e;  // 3-momentum transfer

    /* Safety check - determine if the particle is neutral or not */
    // if (neut->par()->getCharge() != 0) { return false; }

    bool PC = neut.clas12particle_hit_in_PCAL;
    bool IC = neut.clas12particle_hit_in_ECIN;
    bool OC = neut.clas12particle_hit_in_ECOUT;
    // if (!(IC || OC)) { return false; }                 /* must hit ECIN or ECOUT */
    // if (PC) { return false; }                          /* veto photons */
    auto detlayer = IC ? clas12::ECIN : clas12::ECOUT; /* earliest valid calorimeter layer among ECIN/ECOUT */

    // calculate the angles of the neutral particle
    double Theta_rad = am::CalcTheta_rad(neut.xHit, neut.yHit, neut.zHit);
    double Phi_rad = am::CalcPhi_rad(neut.xHit, neut.yHit);

    TVector3 P_n_Angles;
    P_n_Angles.SetMagThetaPhi(1, Theta_rad, Phi_rad);

    // v_nhit = location of neutral particle hit
    TVector3 v_nhit(neut.xHit, neut.yHit, neut.zHit);
    if (v_nhit.Mag() == 0.0 || !std::isfinite(v_nhit.Mag())) { return false; }
    // double Beta = neut->par()->getBeta();  // Beta of the neutral particle
    double Theta_n = am::RadToDeg(P_n_Angles.Theta());

    // if (Beta < 0) { return false; }
    if (Theta_n < 1) { return false; } /* to avoid events with Theta_n = 0 (the "1" is in deg) */
    if (!std::isfinite(Theta_n)) { return false; }

#ifdef DEBUG_VETO
    {
        const double dphi = Calc_Sol_dPhi(electrons, B, L);
        std::cerr << "[NeutronECAL_Cut_NeutVeto] dphi(rad)=" << dphi << ", |v_nhit|=" << v_nhit.Mag() << ", veto_cut=" << veto_cut << "\n";
    }
#endif

    TVector3 shifted_e_unit_v = Get_shifted_e_unit_v_NeutVeto(electrons, v_nhit, B, L);

    TVector3 v_dist = v_nhit - shifted_e_unit_v;

    if (v_dist.Mag() < veto_cut) { Veto = true; }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
#pragma endregion

#pragma region /* NeutronECAL_Cut_NeutVeto function */
#include <cmath>
#include <iostream>

// /**
//  * @brief Compute the solenoidal Larmor rotation angle Δφ for the electron track.
//  *
//  * Given an electron track, the magnetic field B (Tesla), and effective path length L (meters),
//  * this function calculates the azimuthal deflection angle due to the solenoidal field.
//  *
//  * Formula:
//  *   Δφ = (0.299792458 * q * B * L) / (2 * p)
//  * where q is the track charge in units of e, p is the momentum magnitude in GeV/c.
//  *
//  * The factor 0.299792458 converts (T·m) into GeV/c, and the 1/2 factor is the solenoid coupling.
//  * If B(z) is non-uniform, replace B*L with the integral ∫Bz dz.
//  *
//  * @param electrons Vector of region_part_ptr, first element should be the electron track.
//  * @param B Magnetic field strength in Tesla.
//  * @param L Effective solenoid length in meters.
//  * @return Computed Δφ in radians. Returns 0.0 if inputs are invalid.
//  */
// double Calc_Sol_dPhi(const vector<region_part_ptr>& electrons, const double& B, const double& L) {
//     // Sanity checks to avoid UB / division by zero
//     if (electrons.empty() || !electrons[0]) {
// #ifdef DEBUG_VETO
//         std::cerr << "[Calc_Sol_dPhi] electrons empty or null ptr at [0]\n";
// #endif
//         return 0.0;
//     }
//     const auto* ep = electrons[0]->par();
//     if (!ep) {
// #ifdef DEBUG_VETO
//         std::cerr << "[Calc_Sol_dPhi] electrons[0]->par() is null\n";
// #endif
//         return 0.0;
//     }
//     /* Charged particle offset in phi due to a solenoidal field.
//      *
//      * Rotation angle (Larmor rotation) over length L is:
//      *   Δφ = (0.299792458 * q * ∫B·dz) / (2 * p)    [radians]
//      * with p in GeV/c, B in Tesla, L in meters, q in units of |e|.
//      * The 0.299792458 is the GeV/c↔(T·m) conversion; the 1/2 is the solenoid coupling.
//      *
//      * Notes:
//      * - Use the particle momentum magnitude p, not Pz (paraxial p≈pz, but p is safer).
//      * - Sign comes from q * B (right-hand rule). If your coord system twists the other way,
//      *   flip the sign when adding to phi.
//      * - If B(z) isn’t uniform, replace B*L with the path integral ∫Bz dz.
//      */

//     constexpr double kTmmGeV = 0.299792458;  // 1 T·m ≈ 0.299792458 GeV/c

//     const double q = static_cast<double>(ep->getCharge());  // e.g., electron: -1
//     const double p = ep->getP();                            // GeV/c (total)
//     if (!(p > 0.0) || !std::isfinite(p)) {
// #ifdef DEBUG_VETO
//         std::cerr << "[Calc_Sol_dPhi] non-physical p=" << p << "\n";
// #endif
//         return 0.0;
//     }

//     // rotation angle in radians (includes sign via q*B)
//     double BL_int = B * L;                                 // replace with integral if you have B(z)
//     double Sol_dPhi = (kTmmGeV * q * BL_int) / (2.0 * p);  // radians

//     return Sol_dPhi;
// }

// /**
//  * @brief Build the expected charged hit position for the neutral‑veto check.
//  *
//  * This function estimates where the associated charged particle (typically the
//  * electron) would appear in the ECAL, after accounting for solenoidal field
//  * rotation Δφ. The rotated hit position (shifted_e_unit_v) is placed on the same radius
//  * as the neutral hit (v_nhit) but with the electron’s θ and φ+Δφ.
//  *
//  * Workflow:
//  *  - Verify that v_nhit has a valid, non‑zero magnitude; otherwise return (0,0,0).
//  *  - Compute Δφ from B, L, and the electron momentum using Calc_Sol_dPhi().
//  *  - Use SetMagThetaPhi() with |v_nhit|, electron θ, and φ+Δφ to build shifted_e_unit_v.
//  *
//  * Robustness:
//  *  - Returns a zero vector on invalid v_nhit input.
//  *  - Emits debug messages if compiled with -DDEBUG_VETO.
//  *
//  * @param electrons Vector of region_part_ptr, with the first element assumed to be the electron.
//  * @param v_nhit Neutral hit position in ECAL coordinates.
//  * @param B Magnetic field strength in Tesla.
//  * @param L The effective distance the particle travels along the solenoid axis (the z-direction)
//  * @return Rotated charged‑hit vector shifted_e_unit_v.
//  */
// TVector3 Get_shifted_e_unit_v_NeutVeto(const vector<region_part_ptr>& electrons, const TVector3& v_nhit, const double& B, const double& L) {
//     if (v_nhit.Mag() == 0.0 || !std::isfinite(v_nhit.Mag())) {
// #ifdef DEBUG_VETO
//         std::cerr << "[Get_shifted_e_unit_v_NeutVeto] v_nhit has zero/non-finite magnitude, returning (0,0,0)\n";
// #endif
//         return TVector3(0, 0, 0);
//     }
//     TVector3 shifted_e_unit_v;

//     double Sol_dPhi = Calc_Sol_dPhi(electrons, B, L);  // radians

//     shifted_e_unit_v.SetMagThetaPhi(
//         v_nhit.Mag(), electrons[0]->getTheta(),
//         electrons[0]->getPhi() + Sol_dPhi  // add sign-consistent rotation
//                                            // shifted_e_unit_v.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(), electrons[0]->getPhi() - Sol_dPhi  // add sign-consistent rotation
//                                            // shifted_e_unit_v.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(), electrons[0]->getPhi() + 3 * Sol_dPhi  // add sign-consistent
//                                            rotation
//                                            // shifted_e_unit_v.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(), electrons[0]->getPhi() + 0.  // add sign-consistent rotation
//     );

//     return shifted_e_unit_v;
// }

/**
 * @brief ECAL neutral-candidate cut with a charged-track (electron) solenoidal veto.
 *
 * Validates a neutral (neutron) candidate by requiring an ECIN/ECOUT hit and then
 * vetoes it if a charged-track hit is *expected* nearby once the solenoidal rotation
 * of the reconstructed electron is accounted for. The expected charged hit (shifted_e_unit_v)
 * is built at the same radius as the neutral hit (v_nhit), using the electron’s θ
 * and φ shifted by the Larmor angle Δφ computed via Calc_Sol_dPhi(B, L).
 *
 * Processing steps:
 *  1) Guardrails: index bounds, non-null pointers, electron present.
 *  2) Neutral must hit ECIN/ECOUT; PCAL is vetoed (photon suppression).
 *  3) Build v_nhit from the earliest available EC layer among {ECIN, ECOUT}.
 *  4) Compute Δφ from B, L, and the electron momentum; sign follows q·B.
 *  5) Build shifted_e_unit_v with |v_nhit|, electron θ, and φ+Δφ.
 *  6) Form v_dist = v_nhit − shifted_e_unit_v and apply the spatial veto: |v_dist| < veto_cut ⇒ reject.
 *
 * Notes & assumptions:
 *  - Single neutral candidate per call.
 *  - Uses total momentum magnitude p for Δφ (safer than pz).
 *  - If B(z) is non-uniform, replace B·L with ∫Bz dz in Calc_Sol_dPhi.
 *  - Basic kinematic hygiene enforced (θ_n > 1°, finite magnitudes). Optional physics
 *    cuts (θ_nq, θ_q, β upper limits) can be re-enabled where indicated.
 *  - With -DDEBUG_VETO, concise diagnostics are printed (Δφ, |v_nhit|, veto_cut).
 *
 * @param allParticles  All reconstructed particles; the neutral is at @p index.
 * @param electrons     Reconstructed electrons; the leading electron is electrons[0].
 * @param beamE         Beam energy in GeV (used to form q = p_b − p_e if needed).
 * @param index         Index of the neutral candidate within @p allParticles.
 * @param veto_cut      Spatial veto threshold (same units as ECAL coordinates).
 * @return              true if the neutral passes all checks and the veto; false otherwise.
 */
bool NeutronECAL_Cut_NeutVeto(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, const int& index, const double& veto_cut) {
    // Precondition checks
    if (index < 0 || index >= static_cast<int>(allParticles.size())) {
#ifdef DEBUG_VETO
        std::cerr << "[NeutronECAL_Cut_NeutVeto] BAD index=" << index << " size(allParticles)=" << allParticles.size() << "\n";
#endif
        return false;
    }
    if (electrons.empty() || !electrons[0]) {
#ifdef DEBUG_VETO
        std::cerr << "[NeutronECAL_Cut_NeutVeto] electrons empty or null ptr at [0]\n";
#endif
        return false;
    }
    if (!allParticles[index]) {
#ifdef DEBUG_VETO
        std::cerr << "[NeutronECAL_Cut_NeutVeto] allParticles[index] is null ptr\n";
#endif
        return false;
    }
    /* The veto flag */
    bool Veto = false;

    /* Solenoid field parameters */
    double B = 5.0;  // Tesla
    double L = 0.5;  // meters

    /* 3-momentum transfer */
    TVector3 P_b(0, 0, beamE);  // Beam energy
    TVector3 P_e;               // Reco electron momentum vector
    P_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 q = P_b - P_e;  // 3-momentum transfer

    /* Safety check - determine if the particle is neutral or not */
    if (allParticles[index]->par()->getCharge() != 0) { return false; }

    bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);
    if (!(IC || OC)) { return false; }                 /* must hit ECIN or ECOUT */
    if (PC) { return false; }                          /* veto photons */
    auto detlayer = IC ? clas12::ECIN : clas12::ECOUT; /* earliest valid calorimeter layer among ECIN/ECOUT */

    // calculate the angles of the neutral particle
    TVector3 P_n_Angles;
    P_n_Angles.SetMagThetaPhi(1, allParticles[index]->getTheta(), allParticles[index]->getPhi());

    // v_nhit = location of neutral particle hit
    TVector3 v_nhit(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());
    if (v_nhit.Mag() == 0.0 || !std::isfinite(v_nhit.Mag())) { return false; }
    double Beta = allParticles[index]->par()->getBeta();  // Beta of the neutral particle
    double Theta_n = am::RadToDeg(P_n_Angles.Theta());

    if (Beta < 0) { return false; }
    if (Theta_n < 1) { return false; } /* to avoid events with Theta_n = 0 (the "1" is in deg) */
    if (!std::isfinite(Theta_n)) { return false; }

#ifdef DEBUG_VETO
    {
        const double dphi = Calc_Sol_dPhi(electrons, B, L);
        std::cerr << "[NeutronECAL_Cut_NeutVeto] dphi(rad)=" << dphi << ", |v_nhit|=" << v_nhit.Mag() << ", veto_cut=" << veto_cut << "\n";
    }
#endif

    TVector3 shifted_e_unit_v = Get_shifted_e_unit_v_NeutVeto(electrons, v_nhit, B, L);

    TVector3 v_dist = v_nhit - shifted_e_unit_v;

    if (v_dist.Mag() < veto_cut) { Veto = true; }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
#pragma endregion

#pragma region /* NeutronECAL_Cut_NeutVeto function (original) */
/* NOTE: this code is valid for a single neutral only. */

/* In the input, we have:
 * const std::unique_ptr<clas12::clas12reader> &c12 - the event
 * beamE
 * index - of the particle in question (the neutral) */
bool NeutronECAL_Cut_NeutVeto_original(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, const int& index, const double& veto_cut) {
    TVector3 p_b(0, 0, beamE); /* beam energy */

    /* Charged particle offset in phi due to a solenoidal field.
     *
     * Rotation angle (Larmor rotation) over length L is:
     *   Δφ = (0.299792458 * q * ∫B·dz) / (2 * p)    [radians]
     * with p in GeV/c, B in Tesla, L in meters, q in units of |e|.
     * The 0.299792458 is the GeV/c↔(T·m) conversion; the 1/2 is the solenoid coupling.
     *
     * Notes:
     * - Use the particle momentum magnitude p, not Pz (paraxial p≈pz, but p is safer).
     * - Sign comes from q * B (right-hand rule). If your coord system twists the other way,
     *   flip the sign when adding to phi.
     * - If B(z) isn’t uniform, replace B*L with the path integral ∫Bz dz.
     */

    double kTmmGeV = 0.299792458;  // 1 T·m ≈ 0.299792458 GeV/c

    double B = 5.0;                               // Tesla
    double L = 0.5;                               // meters
    double q = electrons[0]->par()->getCharge();  // e.g., electron: -1
    double p = electrons[0]->par()->getP();       // GeV/c  (use total momentum, not just Pz)

    // rotation angle in radians (includes sign via q*B)
    double BL_int = B * L;                                 // replace with integral if you have B(z)
    double Sol_dPhi = (kTmmGeV * q * BL_int) / (2.0 * p);  // radians

    // Optional: print in degrees for quick checks
    // std::cout << "Sol_dPhi = " << Sol_dPhi * (180.0 / TMath::Pi()) << " deg\n";

    TVector3 p_e;  // rotated electron momentum vector
    p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());

    TVector3 p_q = p_b - p_e;  // 3-momentum transfer

    if (allParticles[index]->par()->getCharge() != 0) { return false; } /* determine if the particle is neutral or not */

    // Check which layers of the ECAL have been hit
    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1, allParticles[index]->getTheta(), allParticles[index]->getPhi()); /* calculate the angles of the neutral particle */

    /* check where did the particle hit.
     * no hit - we'll get 0
     * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
       Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison) */
    bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);
    auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

    /* v_nhit = location of neutral particle hit */
    TVector3 v_nhit(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());
    double beta = allParticles[index]->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double theta_n = am::RadToDeg(p_n_Angles.Theta());
    double theta_q = am::RadToDeg(p_q.Theta());
    double theta_nq = am::RadToDeg(p_n_Angles.Angle(p_q));

    if (beta < 0) { return false; }
    //    if (beta > 1.1) { return false; }
    //    // physics cuts, to be ignored according to Larry.
    //    if (theta_nq > 25) { return false; }
    //    if (theta_q > 40) { return false; }
    if (theta_n < 1) { return false; } /* to avoid events with theta_n = 0 (the "1" is in deg) */
    if (!(IC || OC)) { return false; } /* hit only one of these layers */
    if (PC) { return false; }          /* to veto out the gammas (photons) */

    // Now let's put a charge particle veto
    bool Veto = false;

    //
    //
    //

    TVector3 v_chit;
    v_chit.SetMagThetaPhi(
        v_nhit.Mag(), electrons[0]->getTheta(),
        electrons[0]->getPhi() + Sol_dPhi  // add sign-consistent rotation
                                           // v_chit.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(), electrons[0]->getPhi() - Sol_dPhi  // add sign-consistent rotation
                                           // v_chit.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(), electrons[0]->getPhi() + 3 * Sol_dPhi  // add sign-consistent rotation
                                           // v_chit.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(), electrons[0]->getPhi() + 0.  // add sign-consistent rotation
    );

    TVector3 v_dist = v_nhit - v_chit;
    if (v_dist.Mag() < veto_cut) { Veto = true; }

    //
    //
    //

    // for (int j = 0; j < allParticles.size(); j++) {
    //     if (allParticles[j]->par()->getCharge() == 0) { continue; } /* looking on charged particles only */
    //     TVector3 v_chit;                                            /* v_chit = location of charged particle hit */

    //     if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
    //         /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_chit */
    //         v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
    //         TVector3 v_dist = v_nhit - v_chit;

    //         if (v_dist.Mag() < veto_cut) { Veto = true; }
    //     } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
    //         /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_chit */
    //         v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
    //         TVector3 v_dist = v_nhit - v_chit;

    //         if (v_dist.Mag() < veto_cut) { Veto = true; }
    //     } else {
    //         /* the neutral has to hit either the ECIN or ECOUT.
    //            If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
    //         int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
    //         v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
    //                       allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
    //         TVector3 v_dist = v_nhit - v_chit;

    //         if (v_dist.Mag() < veto_cut) { Veto = true; }
    //     }
    // }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
#pragma endregion

#pragma region /* NeutronECAL_Cut_RadGammaVeto function */

bool NeutronECAL_Cut_RadGammaVeto(region_part_ptr& Redef_nFD, vector<region_part_ptr>& Redef_gammaFD, const double& beamE, const double& veto_cut) {
    // TVector3 p_b(0, 0, beamE); /* beam energy */

    // TVector3 p_e; /* our electron */
    // p_e.SetMagThetaPhi(Redef_gammaFD[0]->getP(), Redef_gammaFD[0]->getTheta(), Redef_gammaFD[0]->getPhi());
    // TVector3 p_q = p_b - p_e; /* 3-momentum transfer */

    if (Redef_nFD->par()->getCharge() != 0) { return false; } /* determine if the particle is neutral or not */

    // Check which layers of the ECAL have been hit
    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1, Redef_nFD->getTheta(), Redef_nFD->getPhi()); /* calculate the angles of the neutral particle */

    /* check where did the particle hit.
     * no hit - we'll get 0
     * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
       Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison) */
    bool PC = (Redef_nFD->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (Redef_nFD->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (Redef_nFD->cal(clas12::ECOUT)->getDetector() == 7);
    auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

    /* v_nFD_hit = location of neutral particle hit */
    TVector3 v_nFD_hit(Redef_nFD->cal(detlayer)->getX(), Redef_nFD->cal(detlayer)->getY(), Redef_nFD->cal(detlayer)->getZ());
    double beta = Redef_nFD->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double theta_n = am::RadToDeg(p_n_Angles.Theta());
    // double theta_q = am::RadToDeg(p_q.Theta());
    // double theta_nq = am::RadToDeg(p_n_Angles.Angle(p_q));

    // if (beta < 0) { return false; }
    // //    if (beta > 1.1) { return false; }
    // //    // physics cuts, to be ignored according to Larry.
    // //    if (theta_nq > 25) { return false; }
    // //    if (theta_q > 40) { return false; }
    // if (theta_n < 1) { return false; } /* to avoid events with theta_n = 0 (the "1" is in deg) */
    if (!(IC || OC)) { return false; } /* hit only one of these layers */
    if (PC) { return false; }          /* to veto out the gammas (photons) */

    // Now let's put a charge particle veto
    bool Veto = false;
    for (int j = 0; j < Redef_gammaFD.size(); j++) {
        if (Redef_gammaFD[j]->par()->getCharge() != 0) { continue; } /* looking on charged particles only */
        TVector3 v_gammaFD_hit;                                      /* v_gammaFD_hit = location of charged particle hit */

        if ((detlayer == clas12::ECIN) && (Redef_gammaFD[j]->cal(clas12::ECIN)->getZ() != 0)) {
            /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_gammaFD_hit */
            v_gammaFD_hit.SetXYZ(Redef_gammaFD[j]->cal(clas12::ECIN)->getX(), Redef_gammaFD[j]->cal(clas12::ECIN)->getY(), Redef_gammaFD[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = v_nFD_hit - v_gammaFD_hit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (Redef_gammaFD[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_gammaFD_hit */
            v_gammaFD_hit.SetXYZ(Redef_gammaFD[j]->cal(clas12::ECOUT)->getX(), Redef_gammaFD[j]->cal(clas12::ECOUT)->getY(), Redef_gammaFD[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = v_nFD_hit - v_gammaFD_hit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else {
            /* the neutral has to hit either the ECIN or ECOUT.
               If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_gammaFD_hit.SetXYZ(Redef_gammaFD[j]->traj(clas12::ECAL, trajlayer)->getX(), Redef_gammaFD[j]->traj(clas12::ECAL, trajlayer)->getY(),
                                 Redef_gammaFD[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = v_nFD_hit - v_gammaFD_hit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        }
    }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
#pragma endregion

#endif  // ECAL_VETOES