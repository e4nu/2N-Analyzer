//
// Created by Alon Sportes on 16/03/2025.
//

#ifndef RECO_ANALYSIS_FUNCTIONS_H
#define RECO_ANALYSIS_FUNCTIONS_H

#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TString.h>
#include <TStyle.h>
#include <TTree.h>
#include <TVector3.h>

#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

// Include libraries:
#include "analysis_math/analysis_math.h"
#include "constants.h"

// Include CLAS12 libraries:
#include "../../includes/clas12_include.h"

namespace am = analysis_math;

/**
 * @namespace reco_analysis_functions
 *
 * Reconstruction-level helper namespace for CLAS12 particle objects.
 *
 * Purpose:
 *   Collect small, reusable utilities that operate directly on reconstructed
 *   `clas12::region_part_ptr` particles and detector banks.
 *
 * Main responsibilities:
 *   - extract or recompute neutron-candidate path, time-of-flight, beta, and momentum
 *   - determine whether reconstructed particles hit ECAL or FTOF layers
 *   - apply standard detector-quality utilities such as ECAL diagonal cuts and DC edge cuts
 *   - provide legacy and current ECAL charged/neutral veto helpers for FD neutral studies
 *   - provide simple debugging helpers for drift-chamber trajectory plots
 *
 * Design notes:
 *   The functions in this namespace are intentionally lightweight and mostly stateless.
 *   They are written as free functions because they are shared across multiple analysis
 *   applications and do not require persistent class state.
 *
 * Conventions:
 *   - ECAL layer presence is identified through detector-bank checks.
 *   - Recalculated neutral quantities use the electron vertex as the event reference point.
 *   - Several veto helpers are kept both for active use and for historical cross-checks.
 */
namespace reco_analysis_functions {

// ======================================================================================================================================================================
// General utilities for reconstruction-level analysis
// ======================================================================================================================================================================

// GetFDNeutronP function -----------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Return the momentum assigned to an FD neutral candidate.
 *
 * Purpose:
 *   Provide a single entry point for obtaining a momentum value for forward-detector
 *   neutral candidates, while preserving the historical analysis convention that treats
 *   reconstructed neutrons and photon-tagged neutrals differently when nucleon cuts are active.
 *
 * Behavior:
 *   - If `apply_nucleon_cuts` is true and the PID is 2112, the stored reconstructed momentum is used.
 *   - If `apply_nucleon_cuts` is true and the PID is 22, a neutron-mass momentum proxy is computed from beta.
 *   - Otherwise, the stored particle momentum is returned directly.
 *
 * @param Neutron Reconstructed neutral candidate.
 * @param apply_nucleon_cuts Switch controlling whether the historical neutron/photon split is applied.
 * @return Momentum value in GeV/c.
 */
double GetFDNeutronP(clas12::region_part_ptr& Neutron, bool apply_nucleon_cuts) {
    // Hold the momentum value that will be returned at the end of the function.
    double Momentum;

    // When nucleon cuts are enabled, distinguish between true reconstructed neutrons and
    // neutral candidates stored with photon PID, and treat each case separately.
    if (apply_nucleon_cuts) {
        // Read the reconstructed PID so we know which momentum convention to use.
        int ParticlePDG = Neutron->par()->getPid();

        // True neutron: use the momentum already reconstructed and stored in the banks.
        if (ParticlePDG == 2112) {
            Momentum = Neutron->getP();
            // Photon-tagged neutral: reconstruct a neutron-like momentum from beta and gamma.
        } else if (ParticlePDG == 22) {
            // Use the reconstructed path length to the calorimeter hit.
            double Path_ph = Neutron->getPath();
            // Read the stored neutral time; historical note: this quantity can be noisy.
            double Time_ph = Neutron->getTime();  // bad
            // Use the reconstructed beta as the stable timing observable.
            double Beta_ph = Neutron->par()->getBeta();
            // Convert path and beta into an effective time-of-flight.
            double Time_ph_from_Beta_ph = Path_ph / (constants::c * Beta_ph);
            // Convert the effective time-of-flight back into a velocity for consistency.
            double Velocity_ph = Path_ph / Time_ph_from_Beta_ph;
            // Build the relativistic gamma factor from beta.
            double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
            // Convert beta and gamma into a neutron-mass-based momentum proxy.
            Momentum = constants::m_n * Beta_ph * Gamma_ph;
        }
        // Without nucleon cuts, simply trust the stored particle momentum regardless of PID.
    } else {
        Momentum = Neutron->par()->getP();
    }

    // Return the selected or reconstructed neutral momentum.
    return Momentum;
}

// CheckForNeutralFDECALHits function -----------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine which FD ECAL layers were hit by a neutral candidate.
 *
 * @param ParticleInPCAL Output flag set true when PCAL was hit.
 * @param ParticleInECIN Output flag set true when ECIN was hit.
 * @param ParticleInECOUT Output flag set true when ECOUT was hit.
 * @param NeutralFD_ECAL_detlayer Output layer selector corresponding to the earliest available hit.
 * @param NeutralFD Reconstructed neutral candidate.
 */
void CheckForNeutralFDECALHits(bool& ParticleInPCAL, bool& ParticleInECIN, bool& ParticleInECOUT, short& NeutralFD_ECAL_detlayer, clas12::region_part_ptr& NeutralFD) {
    // Detector id 7 corresponds to ECAL; each check marks whether that sampling layer was hit.
    ParticleInPCAL = (NeutralFD->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
    ParticleInECIN = (NeutralFD->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
    ParticleInECOUT = (NeutralFD->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

    // Choose the first available ECAL layer so later code can use a single reference layer.
    NeutralFD_ECAL_detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;
}

// CheckForECALHits functions -------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Convenience wrapper that checks ECAL-hit information for `allParticles[i]`.
 *
 * @param ParticleInPCAL Output flag set true when PCAL was hit.
 * @param ParticleInECIN Output flag set true when ECIN was hit.
 * @param ParticleInECOUT Output flag set true when ECOUT was hit.
 * @param NeutralFD_ECAL_detlayer Output layer selector corresponding to the earliest available hit.
 * @param allParticles Reconstructed event-particle container.
 * @param i Index of the particle to inspect.
 */
void CheckForECALHits(bool& ParticleInPCAL, bool& ParticleInECIN, bool& ParticleInECOUT, short& NeutralFD_ECAL_detlayer, std::vector<clas12::region_part_ptr>& allParticles, const int& i) {
    // Forward the selected particle to the neutral ECAL-hit helper.
    CheckForNeutralFDECALHits(ParticleInPCAL, ParticleInECIN, ParticleInECOUT, NeutralFD_ECAL_detlayer, allParticles[i]);
}

/**
 * @brief Return ECAL-hit flags and the preferred reference layer for a reconstructed particle.
 *
 * @param ParticleFD Reconstructed particle to inspect.
 * @return Tuple `(PCAL hit, ECIN hit, ECOUT hit, preferred layer)`.
 */
std::tuple<bool, bool, bool, short> CheckForECALHits(clas12::region_part_ptr ParticleFD) {
    // Check each ECAL layer independently using the detector-bank id.
    bool ParticleInPCAL = (ParticleFD->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
    bool ParticleInECIN = (ParticleFD->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
    bool ParticleInECOUT = (ParticleFD->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

    // Use the earliest available layer as the default reference layer.
    short ParticleFD_ECAL_detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;

    // Return both the layer-presence flags and the selected reference layer.
    return {ParticleInPCAL, ParticleInECIN, ParticleInECOUT, ParticleFD_ECAL_detlayer};
}

// CheckForFTOFHits functions -------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Return FTOF-hit flags for a reconstructed particle.
 *
 * @param ParticleFD Reconstructed particle to inspect.
 * @return Tuple `(FTOF1A hit, FTOF1B hit, FTOF2 hit)`.
 */
std::tuple<bool, bool, bool> CheckForFTOFHits(clas12::region_part_ptr ParticleFD) {
    // Detector id 12 corresponds to FTOF; test each panel independently.
    bool ParticleInFTOF1A = (ParticleFD->cal(clas12::FTOF1A)->getDetector() == 12);  // FTOF1A hit
    bool ParticleInFTOF1B = (ParticleFD->cal(clas12::FTOF1B)->getDetector() == 12);  // FTOF1B hit
    bool ParticleInFTOF2 = (ParticleFD->cal(clas12::FTOF2)->getDetector() == 12);    // FTOF2 hit

    // Return the panel-hit pattern for downstream selection logic.
    return {ParticleInFTOF1A, ParticleInFTOF1B, ParticleInFTOF2};
}

// CalcPathnFD function -------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Obtain the neutral-candidate path length to ECAL.
 *
 * @param NeutronFD Reconstructed neutral candidate.
 * @param electron Reconstructed electron used as the event-reference vertex when recalculating.
 * @param Recalc_Path_nFD If true, recompute the path geometrically from vertex to hit; otherwise use the stored path.
 * @return Path length in cm.
 */
double CalcPathnFD(clas12::region_part_ptr NeutronFD, clas12::region_part_ptr electron, const bool& Recalc_Path_nFD = true) {
    // Store the path value that will be returned.
    double reco_Path_nFD;

    // Identify the ECAL layer used to define the neutral hit position.
    auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = CheckForECALHits(NeutronFD);

    // Either recompute the path from geometry or use the stored reconstruction value.
    if (Recalc_Path_nFD) {
        // Use the electron vertex as the event-reference production point.
        TVector3 v_nvtx_3v;
        v_nvtx_3v.SetXYZ(electron->par()->getVx(), electron->par()->getVy(), electron->par()->getVz());
        // Build the neutral hit position at the selected ECAL layer.
        TVector3 v_hit_3v;
        v_hit_3v.SetXYZ(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());
        // The geometric path is the straight-line displacement from vertex to hit.
        TVector3 v_path_3v = v_hit_3v - v_nvtx_3v;
        // Convert the path vector into a scalar length.
        reco_Path_nFD = v_path_3v.Mag();
    } else {
        // Fall back to the path stored by the reconstruction software.
        reco_Path_nFD = NeutronFD->getPath();
    }

    // Return the selected path length.
    return reco_Path_nFD;
}

// CalcToFnFD function --------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Obtain the neutral-candidate time-of-flight quantity.
 *
 * @param NeutronFD Reconstructed neutral candidate.
 * @param starttime Event start time used when recalculating.
 * @param Recalc_ToF_nFD If true, compute hit time relative to `starttime`; otherwise use the stored time.
 * @return Time quantity in ns.
 */
double CalcToFnFD(clas12::region_part_ptr NeutronFD, double starttime = 9999, const bool& Recalc_ToF_nFD = true) {
    // Store the time quantity that will be returned.
    double reco_ToF_nFD;

    // Identify the ECAL layer whose timing will be used.
    auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = CheckForECALHits(NeutronFD);

    // Either recompute time-of-flight relative to the event start or use the stored value.
    if (Recalc_ToF_nFD) {
        // Recalculate the ECAL hit time relative to the provided start time.
        reco_ToF_nFD = NeutronFD->cal(detlayer)->getTime() - starttime;
    } else {
        // Use the time value already stored in the particle banks.
        reco_ToF_nFD = NeutronFD->getTime();
    }

    // Return the selected timing quantity.
    return reco_ToF_nFD;
}

// CalcPnFD function ----------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Obtain the neutral-candidate momentum, optionally recomputed from path and timing.
 *
 * @param NeutronFD Reconstructed neutral candidate.
 * @param electron Reconstructed electron used as the reference vertex when recalculating the path.
 * @param starttime Event start time used for time-of-flight recalculation.
 * @param Recalc_P_nFD If true and the neutral carries PID 22, recompute a neutron-mass momentum proxy.
 * @return Momentum in GeV/c.
 */
// TODO: add a function returning TVector3 that uses this function for neutrons. This function does NOT account for the direction of the vector, only it's magnitude!
double CalcPnFD(clas12::region_part_ptr NeutronFD, clas12::region_part_ptr electron, double starttime = 9999, const bool& Recalc_P_nFD = true) {
    // Hold the momentum value that will be returned.
    double Momentum;

    // Recompute momentum only for photon-tagged neutrals when explicitly requested.
    if (Recalc_P_nFD && (NeutronFD->par()->getPid() == 22)) {
        // Recompute the path length from the event vertex to the ECAL hit.
        double reco_Path_nFD = CalcPathnFD(NeutronFD, electron, Recalc_P_nFD);
        // Recompute the time-of-flight using the provided start time.
        double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, Recalc_P_nFD);
        // Convert path and time-of-flight into beta.
        double reco_Beta_nFD = Recalc_P_nFD ? reco_Path_nFD / (reco_ToF_nFD * constants::c) : NeutronFD->par()->getBeta();
        // Build gamma from beta for the relativistic momentum estimate.
        double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));
        // Convert beta and gamma into a neutron-mass momentum proxy.
        Momentum = constants::m_n * reco_Beta_nFD * reco_Gamma_nFD;
    } else {
        // Otherwise trust the momentum stored in the reconstruction banks.
        Momentum = NeutronFD->getP();
    }

    // Return the selected momentum value.
    return Momentum;
}

// checkEcalDiagCuts function -------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Apply the standard ECAL diagonal sampling-fraction cut to an electron candidate.
 *
 * @param electrons Reconstructed particle under test.
 * @return True when the particle passes the diagonal-cut logic, false otherwise.
 */
bool checkEcalDiagCuts(clas12::region_part_ptr electrons) {
    // Analysis threshold for the electron sampling-fraction diagonal cut.
    double ecal_diag_cut = 0.2;
    // Read the reconstructed momentum used in the sampling-fraction comparison.
    double mom = electrons->par()->getP();
    // Apply the cut only to identified electrons; non-electrons automatically pass.
    if (electrons->par()->getPid() == 11) {
        // Above 4.5 GeV, require enough PCAL+ECIN energy relative to the momentum.
        if ((electrons->cal(clas12::PCAL)->getEnergy() + electrons->cal(clas12::ECIN)->getEnergy()) / mom > ecal_diag_cut && mom > 4.5) {
            return true;
        }
        // Below 4.5 GeV, keep the candidate without applying the diagonal cut.
        else if (mom <= 4.5) {
            return true;
        }
        // Any electron failing the sampling-fraction requirement is rejected.
        else {
            return false;
        }
    }
    // Non-electrons are not subject to this electron-specific cut.
    else {
        return true;
    }
}

// DCEdgeCuts function --------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Apply drift-chamber edge cuts to a reconstructed charged particle.
 *
 * @param p Reconstructed particle under test.
 * @return True when the particle is sufficiently far from the DC edge in all three regions.
 */
bool DCEdgeCuts(clas12::region_part_ptr p) {
    // Region-by-region minimum allowed distances from the DC edge for negative and positive tracks.
    std::vector<double> dc_edge_cut_el = {4.5, 3.5, 7.5};  // units cm; {region1, region2, region3} cuts for electrons INBENDING
    std::vector<double> dc_edge_cut_ptr = {2.5, 3, 10.5};  // units cm; {region1, region2, region3} cuts for protons  OUTBENDING

    // Only charged particles have DC trajectories that can be checked against edge cuts.
    if (p->par()->getCharge() != 0) {
        // Read the trajectory-bank indices for the three DC superlayer reference planes.
        auto traj_index_1 = p->traj(DC, 6)->getIndex();   // layer 1
        auto traj_index_2 = p->traj(DC, 18)->getIndex();  // layer 2
        auto traj_index_3 = p->traj(DC, 36)->getIndex();  // layer 3
        // Extract the precomputed distance-to-edge observable at each reference plane.
        auto traj_edge_1 = p->traj(DC, 6)->getFloat("edge", traj_index_1);
        auto traj_edge_2 = p->traj(DC, 18)->getFloat("edge", traj_index_2);
        auto traj_edge_3 = p->traj(DC, 36)->getFloat("edge", traj_index_3);

        // Apply the negative-track thresholds, used here for electron-like tracks.
        if (p->par()->getCharge() < 0 && (dc_edge_cut_el.size() == 3 && traj_edge_1 > dc_edge_cut_el[0] && traj_edge_2 > dc_edge_cut_el[1] && traj_edge_3 > dc_edge_cut_el[2])) {
            return true;
        }
        // Apply the positive-track thresholds, used here for proton-like tracks.
        else if (p->par()->getCharge() > 0 && (dc_edge_cut_ptr.size() == 3 && traj_edge_1 > dc_edge_cut_ptr[0] && traj_edge_2 > dc_edge_cut_ptr[1] && traj_edge_3 > dc_edge_cut_ptr[2])) {
            return true;
        } else {
            return false;
        }
    }
    // Neutral particles do not have relevant DC edge information, so let them pass.
    else {
        return true;
    }
}

// fillDCdebug function -------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Fill drift-chamber debug histograms with projected trajectory coordinates.
 *
 * @param p Reconstructed particle whose trajectory is being visualized.
 * @param h Array of 2D histograms, indexed by DC region.
 * @param weight Histogram fill weight.
 */
void fillDCdebug(clas12::region_part_ptr p, TH2D** h, double weight) {
    // Fill one XY occupancy histogram for each DC reference plane used in the edge cuts.
    h[1]->Fill(p->traj(DC, 6)->getX(), p->traj(DC, 6)->getY(), weight);
    h[2]->Fill(p->traj(DC, 18)->getX(), p->traj(DC, 18)->getY(), weight);
    h[3]->Fill(p->traj(DC, 36)->getX(), p->traj(DC, 36)->getY(), weight);
}

// ======================================================================================================================================================================
// Helper functions for nFD vetoes
// ======================================================================================================================================================================

// CalcSolenoidPhiShift function -----------------------------------------------------------------------------------------------------------------------------------------------

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
 * @param charged_particle Reconstructed charged particle whose direction is being shifted.
 * @param B Effective magnetic-field strength in Tesla.
 * @param L Effective path length through the solenoid in meters.
 * @return Estimated azimuthal rotation Δφ in radians.
 */
double CalcSolenoidPhiShift(const clas12::region_part_ptr& charged_particle, const double& B, const double& L) {
    // Conversion factor between T·m and GeV/c used in the compact bending formula.
    constexpr double kTmmGeV = 0.299792458;  // 1 T·m ≈ 0.299792458 GeV/c
    // Read the signed charge and total reconstructed momentum of the particle.
    const double q = static_cast<double>(charged_particle->par()->getCharge());
    const double P_charged = charged_particle->par()->getP();
    // Guard against non-physical or malformed momentum values before dividing by p.
    if (!(P_charged > 0.0) || !std::isfinite(P_charged)) {
#ifdef DEBUG_VETO
        std::cerr << "[CalcSolenoidPhiShift] non-physical p = " << P_charged << "\n";
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

    // Approximate the field integral with a constant B times an effective path length L.
    double BL_int = B * L;
    // Convert the field integral into an azimuthal rotation angle.
    double SolenoidPhiShift = (kTmmGeV * q * BL_int) / (2.0 * P_charged);
    // Return the estimated phi rotation in radians.
    return SolenoidPhiShift;
}

// GetShiftedUnitMomentum function -------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Build a solenoid-shifted charged-particle reference position at the neutral-hit radius.
 *
 * Purpose:
 *   Starting from a reconstructed charged-particle direction, estimate where that direction
 *   would point at the same radius as a neutral ECAL hit after applying the solenoidal φ shift.
 *
 * @param charged_particle Reconstructed charged particle used to define the reference direction.
 * @param v_nhit_3v Neutral ECAL hit position whose radius is used for the projection.
 * @param B Effective magnetic-field strength in Tesla.
 * @param L Effective path length through the solenoid in meters.
 * @return Shifted 3-vector positioned at `|v_nhit_3v|`.
 */
TVector3 GetShiftedUnitMomentum(const clas12::region_part_ptr& charged_particle, const TVector3& v_nhit_3v, const double& B, const double& L) {
    // A zero or non-finite neutral-hit radius cannot be used to construct a reference point.
    if (v_nhit_3v.Mag() == 0.0 || !std::isfinite(v_nhit_3v.Mag())) {
#ifdef DEBUG_VETO
        std::cerr << "[GetShiftedUnitMomentum] v_nhit_3v has zero/non-finite magnitude, returning (0,0,0)\n";
#endif
        return TVector3(0, 0, 0);
    }
    // This vector will hold the shifted charged-particle reference position.
    TVector3 ShiftedUnitMomentum_3v;
    // Compute the solenoidal azimuthal rotation for the charged particle.
    double Sol_dPhi = CalcSolenoidPhiShift(charged_particle, B, L);  // radians
    // Project the charged direction to the neutral-hit radius after applying the phi shift.
    ShiftedUnitMomentum_3v.SetMagThetaPhi(v_nhit_3v.Mag(), charged_particle->getTheta(), charged_particle->getPhi() + Sol_dPhi);
    // Return the shifted charged-particle reference position.
    return ShiftedUnitMomentum_3v;
}

// ======================================================================================================================================================================
// nFD veto functions
// ======================================================================================================================================================================

// NeutronECAL_Cut_Veto function ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutronECAL_Cut_Veto function */
/**
 * @brief Legacy ECAL neutral selection + charged-proximity veto for an FD neutral candidate.
 *
 * Context:
 *  This is the older ECAL-based veto used in the MSc analysis (copied from the historical
 *  ParticleID implementation). It assumes a *single neutral candidate* at `allParticles[index]`.
 *  In this project it has been superseded by `ChargedECALveto(...)`, but is kept here for
 *  reference and cross-checks.
 *
 * What it does (high-level):
 *  1) Build the beam 3-vector and the reconstructed electron 3-momentum.
 *  2) Compute the 3-momentum transfer q = p_b_3v - p_e_3v (used only for optional, historically-used
 *     angular cuts).
 *  3) Require the candidate to be neutral (charge == 0).
 *  4) Determine which ECAL layer the candidate hit first (PCAL/ECIN/ECOUT), and build the neutral
 *     hit position vector v_nhit_3v at that layer.
 *  5) Apply a set of neutral-quality veto criteria (see below).
 *  6) Loop over all reconstructed charged particles and veto the neutral if any charged particle
 *     has an ECAL hit (or ECAL trajectory projection) within `veto_cut` of v_nhit_3v.
 *
 * Veto criteria (each explained + motivation):
 *  - Neutrality requirement (charge == 0):
 *      This function is intended to select neutrals. A charged candidate is rejected immediately.
 *
 *  - Beta sanity (beta >= 0):
 *      Negative beta is non-physical and typically indicates a malformed time/path association.
 *      Such candidates are rejected.
 *
 *  - Theta sanity (theta_n >= 1 deg):
 *      Reject candidates with theta close to 0. These often arise from bad reconstruction
 *      (e.g. default angles) and can destabilize geometry-based comparisons.
 *
 *  - ECAL-layer requirement (IC || OC):
 *      Require the neutral to have a hit in ECIN or ECOUT. This enforces the historical
 *      'neutron-like' definition used here and removes candidates that only appear in PCAL.
 *
 *  - PCAL veto (PC == false):
 *      If the candidate has a PCAL hit, reject it. Photons commonly deposit in PCAL; in this
 *      legacy selection, PCAL activity is treated as photon-like contamination.
 *
 *  - Charged proximity veto (|v_nhit_3v - v_chit_3v| < veto_cut):
 *      If any charged particle is spatially close to the neutral ECAL hit, veto the neutral.
 *      Motivation: suppress cases where a charged shower seeds or overlaps the neutral cluster.
 *      Charged position is taken from the *same* ECAL layer when available; otherwise the charged
 *      particle's ECAL trajectory projection is used.
 *
 * Notes:
 *  - The historically-present angular cuts on theta_nq and theta_q are currently commented out.
 *    If re-enabled, they would enforce additional kinematic/topology constraints.
 *
 * @param allParticles Reconstructed event particles (neutral candidate at `index`; scanned for charged activity).
 * @param electrons Reconstructed electrons (uses electrons[0]).
 * @param beamE Beam energy (GeV) used to build the beam 3-vector.
 * @param index Index of the neutral candidate in `allParticles`.
 * @param veto_cut Maximum allowed 3D separation between the neutral hit and any charged hit/projection.
 * @return True if the neutral passes all criteria and survives the charged proximity veto; false otherwise.
 */
bool NeutronECAL_Cut_Veto(std::vector<clas12::region_part_ptr>& allParticles, std::vector<clas12::region_part_ptr>& electrons, const double& beamE, const int& index,
                          const double& veto_cut) {
    // Build the incoming beam direction using the nominal beam energy.
    TVector3 p_b_3v(0, 0, beamE); /* Beam 3-vector: (0,0,Ebeam) in GeV/c */

    // Reconstructed electron 3-momentum (magnitude + angles from the reconstruction banks)
    TVector3 p_e_3v;
    p_e_3v.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());

    // 3-momentum transfer q = p_b_3v - p_e_3v (kept for historical angular cuts; currently not used for veto decisions)
    TVector3 p_q_3v = p_b_3v - p_e_3v;

    // Candidate must be neutral for this veto logic to be meaningful
    if (allParticles[index]->par()->getCharge() != 0) { return false; }

    // Build a unit vector that carries the neutral candidate angles (theta, phi)
    TVector3 p_n_Angles_3v;
    p_n_Angles_3v.SetMagThetaPhi(1.0, allParticles[index]->getTheta(), allParticles[index]->getPhi());  // unit vector; angles define the neutral direction

    // Determine which ECAL sampling layers were hit by the neutral candidate.
    // getDetector()==7 indicates an ECAL hit (calorimeter detector id). Using int comparisons avoids fragile double-energy checks.
    const bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
    const bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
    const bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

    // Earliest hit layer (used to define the neutral hit position v_nhit_3v)
    const auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT;

    // Neutral ECAL hit position at the chosen layer
    TVector3 v_nhit_3v(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());

    // Basic kinematics/angles used for quality checks (and for optional historical physics cuts)
    const double beta = allParticles[index]->par()->getBeta();
    const double gamma = 1.0 / sqrt(1.0 - (beta * beta));  // not used in the veto decision; kept for legacy parity
    const double theta_n = analysis_math::RadToDeg(p_n_Angles_3v.Theta());
    const double theta_q = analysis_math::RadToDeg(p_q_3v.Theta());
    const double theta_nq = analysis_math::RadToDeg(p_n_Angles_3v.Angle(p_q_3v));

    // (1) Beta sanity: negative beta is non-physical (bad time/path association)
    if (beta < 0) { return false; }

    // Optional legacy checks (kept commented intentionally):
    //   - beta > 1.1 : strongly unphysical candidates
    //   - theta_nq, theta_q : historical topology/physics cuts (disabled per Larry)
    // if (beta > 1.1) { return false; }
    // if (theta_nq > 25) { return false; }
    // if (theta_q > 40) { return false; }

    // (2) Theta sanity: reject degenerate candidates with theta ~ 0 degrees
    if (theta_n < 1.0) { return false; }  // degrees

    // (3) ECAL-layer requirement: must hit ECIN or ECOUT (legacy neutron-like definition)
    if (!(IC || OC)) { return false; }

    // (4) PCAL veto: reject any candidate with PCAL activity (photon-like contamination in this legacy selection)
    if (PC) { return false; }

    // (5) Charged proximity veto: reject the neutral if any charged particle is too close in ECAL
    // Track whether any charged particle comes too close to the neutral ECAL hit.
    bool Veto = false;
    // Scan all reconstructed particles and compare only charged ones against the neutral candidate.
    for (int j = 0; j < allParticles.size(); j++) {
        // Consider charged particles only
        if (allParticles[j]->par()->getCharge() == 0) { continue; }
        // v_chit_3v is the charged reference position in ECAL (measured hit when available; otherwise trajectory projection)
        TVector3 v_chit_3v;

        if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            // Both neutral and charged have a measured ECIN hit: compare using ECIN measured positions
            v_chit_3v.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist_3v = v_nhit_3v - v_chit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            // Both neutral and charged have a measured ECOUT hit: compare using ECOUT measured positions
            v_chit_3v.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist_3v = v_nhit_3v - v_chit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else {
            // Charged particle does not have a measured hit in the same layer as the neutral.
            // Fall back to the charged particle's ECAL trajectory projection at a reference layer:
            //   ECIN neutral -> trajlayer 4, ECOUT neutral -> trajlayer 7 (legacy mapping).
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit_3v.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                             allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist_3v = v_nhit_3v - v_chit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        }
    }

    // If any charged particle was found within veto_cut, reject the neutral candidate
    // Reject the neutral if any charged-particle proximity condition was triggered.
    if (Veto) { return false; }

    // Survived all criteria: accept this neutral candidate
    // Accept the neutral only if it survived all geometric and quality requirements.
    return true;
}
#pragma endregion

// ChargedECALveto function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ChargedECALveto function */
/**
 * @brief Apply the charged-particle ECAL proximity veto to a forward neutral candidate.
 *
 * Purpose:
 *  Reject a neutral candidate (neutron or photon) if there is nearby charged-particle activity
 *  in ECAL that could indicate the neutral is a mis-associated cluster or contamination from a
 *  charged shower.
 *
 * High level algorithm:
 *  1) Determine the earliest ECAL layer hit by the neutral: PCAL, ECIN, or ECOUT.
 *  2) Define the neutral hit position vector v_nhit at that layer.
 *  3) Apply basic validity and PID related veto criteria (see below).
 *  4) Loop over all reconstructed charged particles and build a charged reference position:
 *     - If the charged particle has a measured hit in the same ECAL layer, use that measured hit.
 *     - Otherwise, use the charged particle trajectory projection at an ECAL reference layer.
 *  5) Veto the neutral if any charged reference position lies within `veto_cut` of v_nhit.
 *
 * Veto criteria and motivation:
 *  - Neutrality requirement (charge == 0):
 *    This function is intended only for neutrals. A charged candidate is rejected immediately.
 *
 *  - Optional PCAL based neutral definition (`apply_PCAL_veto`):
 *    If enabled, require the neutral to have an ECIN or ECOUT hit, and reject any neutral with a
 *    PCAL hit. This implements the analysis policy used to suppress photon-like candidates, since
 *    photons commonly deposit in PCAL while the neutron definition used here excludes PCAL.
 *
 *  - Beta sanity (`0 <= beta <= 1.1`):
 *    Reject non-physical beta (negative) and strongly unphysical beta above 1.1. The upper bound
 *    is intentionally loose to tolerate resolution effects while still rejecting obvious failures.
 *
 *  - Theta sanity (`theta > 1 deg`):
 *    Remove degenerate or malformed candidates with theta near 0 that can lead to undefined
 *    geometry and unstable comparisons.
 *
 *  - Charged proximity veto (`|v_nhit - v_ch| < veto_cut`):
 *    If any charged particle is spatially close to the neutral hit at ECAL, veto the neutral.
 *    This suppresses cases where a charged shower can mimic or seed a neutral cluster.
 *
 * Notes on start time and recalculation:
 *  - If `recalc_P_nFD` is enabled, beta (and momentum) are recomputed from path and time-of-flight
 *    relative to `starttime` rather than using the stored particle values.
 *  - In this implementation, the helper flag `recalc_P_nFD` is derived from `starttime` and
 *    `Recalc_P_nFD`. Ensure the calling code uses a meaningful `starttime` when requesting a
 *    recalculation.
 *
 * @param allParticles Reconstructed event particles to scan for charged activity.
 * @param electron Reconstructed electron used as the vertex reference when recalculating path.
 * @param NeutronFD Neutral candidate under test.
 * @param veto_cut Maximum allowed 3D separation between the neutral hit and a charged reference point.
 * @param starttime Event start time used when recomputing time-of-flight.
 * @param Recalc_P_nFD If true, request recalculation of neutral kinematics from path and timing.
 * @param apply_PCAL_veto If true, require ECIN/ECOUT and reject neutrals with PCAL activity.
 * @return True if the neutral survives the veto; false otherwise.
 */
bool ChargedECALveto(std::vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr& electron, clas12::region_part_ptr NeutronFD, const double& veto_cut,
                     const double& starttime = 9999, const bool& Recalc_P_nFD = false, const bool& apply_PCAL_veto = false) {
    // Veto setup: define how (and whether) we will use reconstructed timing and geometry information.
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // If requested, we can recompute beta (and momentum) using a path and a time-of-flight relative to `starttime`.
    // Here we derive a local switch `recalc_P_nFD` from the user flag and the start-time sentinel.
    // Track whether any charged-particle comparison falls inside the veto radius.
    bool recalc_P_nFD = (starttime == 9999) ? Recalc_P_nFD : false;

    // If `recalc_P_nFD` is enabled, compute the neutral path from the electron vertex to the ECAL hit,
    // and compute ToF from the ECAL hit time relative to `starttime`. These are then converted to beta.
    // If disabled, use the stored particle beta from the reconstruction banks.
    const double reco_Path_nFD = CalcPathnFD(NeutronFD, electron, recalc_P_nFD);
    const double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, recalc_P_nFD);
    const double reco_Beta_nFD = recalc_P_nFD ? reco_Path_nFD / (reco_ToF_nFD * constants::c) : NeutronFD->par()->getBeta();

    // Gamma is the usual relativistic factor. It is computed here for completeness and debugging.
    // It is not used by the veto decision itself.
    const double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));

    // Determine which ECAL layers were hit by the neutral and choose the earliest hit layer.
    // detlayer is used consistently to define the neutral hit location and the charged comparison strategy.
    const auto [PC, IC, OC, detlayer] = CheckForECALHits(NeutronFD);

    // Neutral ECAL hit position in the chosen layer.
    const TVector3 reco_nFD_hit_3v(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());

    // Build a momentum direction vector for the neutral (angles from the reconstructed particle).
    // The magnitude is taken from CalcPnFD if recalculation is enabled; otherwise it uses the stored momentum.
    // This is only used for a theta sanity cut.
    TVector3 reco_P_nFD_3v;
    reco_P_nFD_3v.SetMagThetaPhi(CalcPnFD(NeutronFD, electron, starttime, recalc_P_nFD), NeutronFD->getTheta(), NeutronFD->getPhi());

    // Veto application: apply fast rejection criteria first, then scan charged particles for proximity.
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // (A) Optional PCAL based neutral definition:
    //     - Require ECIN or ECOUT (neutral must have an ECAL hit beyond PCAL)
    //     - Veto any PCAL hit (photon suppression policy in this analysis)
    if (apply_PCAL_veto && (!(IC || OC) || PC)) { return false; }

    // (B) Neutrality requirement: this veto is defined only for neutrals.
    if (NeutronFD->par()->getCharge() != 0) { return false; }

    // (C) Beta sanity: reject non-physical beta and strongly unphysical values.
    // if (reco_Beta_nFD < 0.0 || reco_Beta_nFD > 1.1) { return false; }
    if (reco_Beta_nFD < 0.0) { return false; }

    // (D) Theta sanity: avoid degenerate theta close to 0 degrees.
    if (am::RadToDeg(reco_P_nFD_3v.Theta()) < 1.0) { return false; }

    // Now let's put a charge particle veto:
    bool Veto = false;

    // Compare the neutral candidate against every reconstructed charged particle in the event.
    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; }  // Only consider charged particles

        // Charged reference position in ECAL.
        // Prefer a measured hit in the same layer as the neutral; otherwise fall back to the trajectory
        // projection at a reference ECAL layer to estimate where the charged particle would intersect ECAL.
        TVector3 reco_ChargedFD_hit_3v;

        if ((detlayer == clas12::PCAL) && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
            // If both particles hit the pre-shower calorimeter, use the pre-shower calorimeter to determine reco_ChargedFD_hit_3v
            reco_ChargedFD_hit_3v.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
            TVector3 v_dist_3v = reco_nFD_hit_3v - reco_ChargedFD_hit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            // If both particles hit the inner calorimeter, use the inner calorimeter to determine reco_ChargedFD_hit_3v
            reco_ChargedFD_hit_3v.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist_3v = reco_nFD_hit_3v - reco_ChargedFD_hit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            // If both particles hit the outer calorimeter, use the outer calorimeter to determine reco_ChargedFD_hit_3v
            reco_ChargedFD_hit_3v.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist_3v = reco_nFD_hit_3v - reco_ChargedFD_hit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else {
            // Trajectory layer mapping used in this project:
            //   PCAL -> 1, ECIN -> 4, ECOUT -> 7
            const int trajlayer = (detlayer == clas12::PCAL) ? 1 : (detlayer == clas12::ECIN) ? 4 : 7;
            reco_ChargedFD_hit_3v.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                                         allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist_3v = reco_nFD_hit_3v - reco_ChargedFD_hit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        }
    }

    // Reject the neutral as soon as any charged reference point overlaps within the cut.
    if (Veto) { return false; }
    // Accept the neutral only if no charged-particle proximity veto was triggered.
    return true;
}
#pragma endregion

// NeutralECALveto function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutralECALveto function */
/**
 * @brief Apply the solenoid-corrected neutral ECAL proximity veto.
 *
 * Purpose:
 *   Test whether a forward neutral ECAL hit lies too close to the solenoid-shifted direction
 *   of reconstructed charged particles, motivated by cases where charged tracks, especially the
 *   electron, radiate photons that continue approximately along the local charged-track direction.
 *
 * Main steps:
 *   - determine the neutral reference ECAL layer and hit position
 *   - optionally recompute neutral path, time-of-flight, beta, and momentum
 *   - apply basic neutral-quality and optional PCAL-based selection requirements
 *   - for each charged reference particle, estimate a solenoid-shifted ECAL position at the neutral radius
 *   - veto the neutral when the shifted charged reference lies within `veto_cut` of the neutral hit
 *
 * Important note:
 *   This helper uses a compact solenoid φ-shift model rather than full detector transport.
 *   It is therefore best treated as an analysis-level geometric veto approximation.
 *
 * @param allParticles Reconstructed event particles scanned for charged reference tracks.
 * @param electron Reconstructed electron used as the vertex reference when recalculating path.
 * @param NeutronFD Neutral candidate under test.
 * @param veto_cut Maximum allowed 3D separation between the neutral hit and the shifted charged reference.
 * @param starttime Event start time used when recomputing time-of-flight.
 * @param Recalc_P_nFD If true, request recalculation of neutral kinematics from path and timing.
 * @param apply_PCAL_veto If true, require ECIN/ECOUT and reject neutrals with PCAL activity.
 * @param B Effective magnetic-field strength used in the solenoid shift model.
 * @param L Effective path length used in the solenoid shift model.
 * @return True if the neutral survives the solenoid-corrected veto; false otherwise.
 */
// TODO: review and debug this implementation of the neutral ECAL veto
// TODO: check if this should be implemented to other particles beside the electron
bool NeutralECALveto(std::vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr& electron, clas12::region_part_ptr NeutronFD, const double& veto_cut,
                     const double& starttime = 9999, const bool& Recalc_P_nFD = false, const bool& apply_PCAL_veto = false, const double B = constants::B, const double L = constants::L) {
    // Veto setup: define how (and whether) we will use reconstructed timing and geometry information.
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // If requested, we can recompute beta (and momentum) using a path and a time-of-flight relative to `starttime`.
    // Here we derive a local switch `recalc_P_nFD` from the user flag and the start-time sentinel.
    // Derive a local switch controlling whether neutral kinematics are recomputed.
    bool recalc_P_nFD = (starttime == 9999) ? Recalc_P_nFD : false;

    // If `recalc_P_nFD` is enabled, compute the neutral path from the electron vertex to the ECAL hit,
    // and compute ToF from the ECAL hit time relative to `starttime`. These are then converted to beta.
    // If disabled, use the stored particle beta from the reconstruction banks.
    // Recompute or read the neutral path, timing, and beta needed for basic sanity checks.
    const double reco_Path_nFD = CalcPathnFD(NeutronFD, electron, recalc_P_nFD);
    const double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, recalc_P_nFD);
    const double reco_Beta_nFD = recalc_P_nFD ? reco_Path_nFD / (reco_ToF_nFD * constants::c) : NeutronFD->par()->getBeta();

    // Gamma is the usual relativistic factor. It is computed here for completeness and debugging.
    // It is not used by the veto decision itself.
    const double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));

    // Determine which ECAL layers were hit by the neutral and choose the earliest hit layer.
    // detlayer is used consistently to define the neutral hit location and the charged comparison strategy.
    // Identify the ECAL layer used as the neutral reference surface.
    const auto [PC, IC, OC, detlayer] = CheckForECALHits(NeutronFD);

    // Neutral ECAL hit position in the chosen layer.
    // Build the measured neutral ECAL hit position at that reference layer.
    const TVector3 reco_nFD_hit_3v(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());

    // Build a momentum direction vector for the neutral (angles from the reconstructed particle).
    // The magnitude is taken from CalcPnFD if recalculation is enabled; otherwise it uses the stored momentum.
    // This is only used for a theta sanity cut.
    // Build the neutral momentum-direction vector used only for the theta sanity cut.
    TVector3 reco_P_nFD_3v;
    reco_P_nFD_3v.SetMagThetaPhi(CalcPnFD(NeutronFD, electron, starttime, recalc_P_nFD), NeutronFD->getTheta(), NeutronFD->getPhi());

    // Veto application: apply fast rejection criteria first, then scan charged particles for proximity.
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // (A) Optional PCAL based neutral definition:
    //     - Require ECIN or ECOUT (neutral must have an ECAL hit beyond PCAL)
    //     - Veto any PCAL hit (photon suppression policy in this analysis)
    // Optionally enforce the same PCAL-based neutral definition used elsewhere in the analysis.
    if (apply_PCAL_veto && (!(IC || OC) || PC)) { return false; }

    // (B) Neutrality requirement: this veto is defined only for neutrals.
    // This veto is defined only for neutral candidates.
    if (NeutronFD->par()->getCharge() != 0) { return false; }

    // (C) Beta sanity: reject non-physical beta and strongly unphysical values.
    // if (reco_Beta_nFD < 0.0 || reco_Beta_nFD > 1.1) { return false; }
    // Reject clearly non-physical timing solutions.
    if (reco_Beta_nFD < 0.0) { return false; }

    // (D) Theta sanity: avoid degenerate theta close to 0 degrees.
    // Reject degenerate candidates with theta too close to the beam axis.
    if (am::RadToDeg(reco_P_nFD_3v.Theta()) < 1.0) { return false; }

    // Now let's put a charge particle veto:
    // Track whether any shifted charged-particle reference lands too close to the neutral hit.
    bool Veto = false;

    // Compare the neutral hit to each charged-particle solenoid-shifted reference direction.
    for (int j = 0; j < allParticles.size(); j++) {
        // Only use charged particles as reference tracks in the solenoid-shifted comparison.
        if (allParticles[j]->par()->getCharge() == 0) { continue; }
        // Estimate the shifted charged-particle reference position at the neutral-hit radius.
        TVector3 ShiftedUnitMomentum_3v = GetShiftedUnitMomentum(allParticles[j], reco_nFD_hit_3v, B, L);
        // Measure the 3D separation between the neutral hit and the shifted charged reference.
        TVector3 v_dist_3v = reco_nFD_hit_3v - ShiftedUnitMomentum_3v;
        // Mark the neutral for rejection when the shifted charged reference lies inside the veto radius.
        if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
    }

    // Reject the neutral if any shifted charged reference overlaps within the cut.
    if (Veto) { return false; }
    // Accept the neutral only if it survived all sanity checks and shifted-reference comparisons.
    return true;
}
#pragma endregion

};  // namespace reco_analysis_functions

#endif  // RECO_ANALYSIS_FUNCTIONS_H
