//
// Created by Alon Sportes on 16/02/2025.
//

#ifndef FD_NEUTRAL_KINEMATICS_H
#define FD_NEUTRAL_KINEMATICS_H

#include <TDirectory.h>
#include <TVector3.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <tuple>

// Include libraries:
#include "../../namespaces/general_utilities/constants.h"

// Other includes:
#include "ChargedHit.h"
#include "NeutralHit.h"

namespace nfdveto {

#pragma region /* Veto structs */

/**
 * @brief Hold runtime configuration for FD neutral-veto debug histogram booking/filling.
 *
 * This struct is a plain configuration holder used by the FD-neutral-veto debug layer
 * (histogram booking, caching, filling, and optional canvas writing). It intentionally
 * contains no physics logic and performs no ROOT I/O by itself.
 *
 * Workflow:
 *  - Set `enable` as the master on/off switch.
 *  - Provide a ROOT owner directory via `baseDir` (typically a TFile* or TDirectory*).
 *  - Optionally enable extra histogram categories via `book2DStepHists`.
 *  - Optionally allow writing canvases via `writeCanvases` (usually keep false for speed).
 *
 * Robustness:
 *  - If `enable == false`, debug code should be a no-op (no booking, no filling, no writing).
 *  - If `baseDir == nullptr`, booking/filling should be skipped even when enabled.
 *
 * Fields:
 *  - enable: Master on/off switch for all debug histogram activity.
 *  - baseDir: ROOT directory that owns the debug output (TFile* or TDirectory*).
 *  - book2DStepHists: If true, 2D per-step histograms may be booked/filled in addition to 1D.
 *  - writeCanvases: If true, code may create/write TCanvas objects (slower; use sparingly).
 */
struct DebugHistConfig {
    bool enable = false;
    TDirectory* baseDir = nullptr;
    bool book2DStepHists = true;
    bool writeCanvases = false;
};

#pragma endregion

#pragma region /* Veto functions */

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

#pragma region /* Calc_Sol_dPhi function */
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

#pragma endregion

#pragma region /* Get_shifted_e_unit_v_NeutVeto */

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

#pragma endregion

#pragma region /* ChargedECALveto function */
/* NOTE: this code is valid for a single neutral only. */

/**
 * @brief Apply a charged-particle proximity veto in ECAL around a given neutral hit.
 *
 * This function rejects a neutral candidate if any reconstructed charged particle has an ECAL
 * hit (or an ECAL trajectory projection when the matching ECAL layer is not directly hit) that
 * lies within a configurable 3D distance `veto_cut` from the neutral hit position.
 *
 * Workflow:
 *  - Determine the neutral’s earliest ECAL layer (PCAL, ECIN, or ECOUT) using the per-hit flags
 *    stored on `NeutralHit` (`clas12particle_hit_in_PCAL/ECIN/ECOUT`).
 *  - Build the neutral-hit position vector `v_nhit` from (xHit,yHit,zHit).
 *  - Loop over all reconstructed particles and consider only charged ones (charge != 0).
 *  - For each charged particle:
 *      * If the charged particle has a measured hit in the same ECAL layer as the neutral (ECIN or ECOUT),
 *        compare neutral-hit position to that measured charged-hit position.
 *      * Otherwise, use the charged particle’s ECAL trajectory projection at the corresponding layer
 *        (traj layer 4 for ECIN, 7 for ECOUT) as the expected charged-hit position and compare to `v_nhit`.
 *  - If any charged particle lies within `veto_cut`, veto the neutral (return false); otherwise return true.
 *
 * Robustness:
 *  - Returns false for trivially non-physical neutral directions (theta < 1 deg) to avoid degenerate cases.
 *  - This function performs no explicit null checks on the input particle pointers; it assumes the calling
 *    analysis provides valid `allParticles`/`electrons` content consistent with the rest of the analysis.
 *
 * @param allParticles Reconstructed event particles (FD/others); used to scan for charged ECAL activity.
 * @param electrons Reconstructed electron vector (used here for consistency with the surrounding API).
 * @param beamE Beam energy (present for API symmetry; not currently used by this function).
 * @param neut The NeutralHit candidate being tested.
 * @param veto_cut Maximum allowed 3D distance (same length units as hit positions) between `v_nhit`
 *        and a charged hit/projection before vetoing.
 * @return True if the neutral survives the charged proximity veto; false if vetoed.
 */
bool ChargedECALveto(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, NeutralHit neut, const double& veto_cut) {
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

    double theta_n = am::RadToDeg(p_n_Angles.Theta());

    if (theta_n < 1) { return false; } /* to avoid events with theta_n = 0 (the "1" is in deg) */

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

#pragma region /* NeutralECALveto function */

/**
 * @brief Apply a neutral-particle proximity veto in ECAL using a solenoid-shifted electron reference.
 *
 * This function rejects a neutral candidate if its ECAL hit position lies within a configurable 3D
 * distance `veto_cut` from the *expected* charged-particle (typically electron) hit position, after
 * applying an approximate solenoidal-field azimuthal rotation Δφ.
 *
 * The key idea is to compare the neutral hit location `v_nhit` to a shifted electron reference
 * direction `shifted_e_unit_v`, where the electron’s φ is rotated by Δφ and the resulting vector is
 * placed on the same radius |v_nhit|.
 *
 * Workflow:
 *  - Define solenoid parameters (B, L) used for the Δφ estimate.
 *  - Build the beam vector P_b and reconstructed electron momentum vector P_e.
 *  - Form the 3-momentum transfer q = P_b - P_e (currently computed for context; not used in the veto).
 *  - Build the neutral direction angles (θ, φ) from the neutral hit position.
 *  - Construct the neutral hit position vector `v_nhit` from (xHit, yHit, zHit).
 *  - Reject trivially non-physical cases (|v_nhit| == 0, non-finite θ, or θ < 1 deg).
 *  - Compute the shifted electron reference vector using Get_shifted_e_unit_v_NeutVeto(...).
 *  - Compute the spatial separation v_dist = v_nhit - shifted_e_unit_v.
 *  - If |v_dist| < veto_cut, veto the neutral (return false); otherwise return true.
 *
 * Robustness:
 *  - Returns false on non-physical neutral geometry (zero magnitude, non-finite θ, θ < 1 deg).
 *  - Emits optional debug prints when compiled with -DDEBUG_VETO.
 *  - The solenoid parameters (B, L) are fixed constants here; if a run-dependent or field-map-based
 *    correction is needed, refactor to pass them in or compute Δφ from a proper ∫Bz dz.
 *
 * @param allParticles Reconstructed event particles (present for API symmetry; not currently used).
 * @param electrons Reconstructed electron vector (electrons[0] is assumed to be valid).
 * @param beamE Beam energy used to build the beam 3-vector P_b.
 * @param neut The NeutralHit candidate being tested.
 * @param veto_cut Maximum allowed 3D distance (same length units as hit positions) between `v_nhit`
 *        and the shifted electron reference before vetoing.
 * @return True if the neutral survives the neutral proximity veto; false if vetoed.
 */
bool NeutralECALveto(vector<region_part_ptr>& allParticles, vector<region_part_ptr>& electrons, const double& beamE, NeutralHit neut, const double& veto_cut) {
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

    // calculate the angles of the neutral particle
    double Theta_rad = am::CalcTheta_rad(neut.xHit, neut.yHit, neut.zHit);
    double Phi_rad = am::CalcPhi_rad(neut.xHit, neut.yHit);

    TVector3 P_n_Angles;
    P_n_Angles.SetMagThetaPhi(1, Theta_rad, Phi_rad);

    // v_nhit = location of neutral particle hit
    TVector3 v_nhit(neut.xHit, neut.yHit, neut.zHit);
    if (v_nhit.Mag() == 0.0 || !std::isfinite(v_nhit.Mag())) { return false; }
    double Theta_n = am::RadToDeg(P_n_Angles.Theta());

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

#pragma region /* SectorDistance function */

/**
 * @brief Compute the wrap-around-aware distance between two CLAS12 sectors on a 6-sector detector.
 *
 * This helper returns the minimal sector-step distance on a circular 6-sector indexing scheme,
 * i.e. it treats (1,6) as distance 1 rather than 5.
 *
 * Workflow:
 *  - Compute the absolute sector difference |s1 - s2|.
 *  - Return the smaller of (ds) and (6 - ds) to account for wrap-around.
 *
 * Robustness:
 *  - Works for typical CLAS12 sector indices (1..6).
 *  - If inputs are outside 1..6, the arithmetic still runs, but the interpretation may be invalid.
 *
 * @param s1 First sector index (typically 1..6).
 * @param s2 Second sector index (typically 1..6).
 * @return Minimal wrap-around-aware sector distance in integer steps.
 */
inline int SectorDistance(int s1, int s2) {
    const int ds = std::abs(s1 - s2);
    return std::min(ds, 6 - ds);
}

#pragma endregion

#pragma region /* Calc_nFD_vars function */

/**
 * @brief Compute basic FD-neutron ToF kinematics from a NeutralHit and an event start time.
 *
 * This is the single shared header-only definition used across the codebase for converting a
 * hit-level path length and hit time into (ToF, beta, gamma, and a reconstructed momentum vector).
 *
 * Workflow:
 *  - Read the hit path (HitPath) from the electron vertex to the neutral hit position.
 *  - Compute time-of-flight ToF = HitTime - starttime.
 *  - Compute beta = |HitPath| / (ToF * c).
 *  - Compute gamma = 1 / sqrt(1 - beta^2).
 *  - Compute p = m_n * beta * gamma and build a 3-vector along HitPath.
 *
 * Robustness:
 *  - Emits warnings for non-physical ToF or beta, but continues the calculation.
 *  - If beta is outside (0,1), gamma may become NaN; downstream code must handle this.
 *  - No guards are enforced here by design; callers should decide how to treat non-physical hits.
 *
 * @param h NeutralHit carrying HitPath and HitTime.
 * @param starttime Event start time used to form ToF (same time units as HitTime).
 * @return Tuple: (ToF [ns], beta, gamma, reco_P_nFD [GeV/c]).
 */
inline std::tuple<double, double, double, TVector3> Calc_nFD_vars(const NeutralHit& h, double starttime) {
    // 1) Copy the path from the electron vertex to the neutral hit position.
    const TVector3 hitPath = h.HitPath;

    // 2) Compute time-of-flight relative to the provided event start time.
    //    Negative Calculated_ToF_nFD -> taken care of with beta guards
    const double Calculated_ToF_nFD = h.HitTime - starttime;

    // 3) Compute beta = L / (c * ToF).
    const double Calculated_Beta_nFD = hitPath.Mag() / (Calculated_ToF_nFD * constants::c);
    
    // 4) Safeguard: beta must be positive.
    //    If beta is outside that range, treat this neutral hit as bad and return a zero momentum vector.
    //    COATJAVA convention note: when beta is negative, the stored reco momentum magnitude is often 0.
    //    If both the TOF-derived beta and the stored reco momentum are non-physical, assume the hit is bad.
    if (Calculated_Beta_nFD < 0.0 && h.P.Mag() == 0) { return {Calculated_ToF_nFD, Calculated_Beta_nFD, 0., TVector3(0., 0., 0.)}; }

    // 5) Compute gamma = 1 / sqrt(1 - beta^2).
    const double Calculated_Gamma_nFD = 1.0 / std::sqrt(1.0 - Calculated_Beta_nFD * Calculated_Beta_nFD);

    // 6) Compute neutron momentum magnitude p = m_n * beta * gamma.
    const double p = constants::m_n * Calculated_Beta_nFD * Calculated_Gamma_nFD;

    // 7) Build a 3-vector with magnitude p and direction along HitPath.
    TVector3 Calculated_reco_P_nFD_v;
    Calculated_reco_P_nFD_v.SetMagThetaPhi(p, hitPath.Theta(), hitPath.Phi());

    // std::cerr << "\033[35mP.Mag():\033[0m " << h.P.Mag() << " [GeV/c]" << std::endl;
    // std::cerr << "\033[35mP.calc:\033[0m " << Calculated_reco_P_nFD_v.Mag() << " [GeV/c]" << std::endl;

    // std::cout << "\n";
    // std::cout << "\n---------------------------";
    // std::cout << "\n";

    // 9) Return the ToF-derived kinematics.
    return {Calculated_ToF_nFD, Calculated_Beta_nFD, Calculated_Gamma_nFD, Calculated_reco_P_nFD_v};
}

#pragma endregion

#pragma endregion

}  // namespace nfdveto

#endif  // FD_NEUTRAL_KINEMATICS_H