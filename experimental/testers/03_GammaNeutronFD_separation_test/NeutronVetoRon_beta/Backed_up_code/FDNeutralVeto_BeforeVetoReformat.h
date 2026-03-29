//
// Obtained from Ron Wagner on 06/11/2025.
// Refactored, extended, and documented by Alon Sportes.
//
// FDNeutralVeto.h (Before Veto Reformat)
// ---------------
// Class wrapper for the Forward Detector (FD) neutral-particle veto framework.
// Implementation of the Forward Detector (FD) neutral-particle veto framework.
//
// This file contains the full hit-level veto logic used to identify clean neutral
// candidates (neutrons / photons) in the CLAS12 Forward Detector ECAL while rejecting
// backgrounds from:
//   - charged-particle contamination,
//   - upstream neutral activity (FTOF),
//   - duplicated responses across ECAL layers,
//   - segmentation-induced multiple hits within the same ECAL layer.
//
// Core concepts:
//   - The veto operates on *hit-level objects* (NeutralHit / ChargedHit), not on
//     reconstructed particles. A single particle may therefore contribute multiple
//     hit objects across different detectors and ECAL layers.
//   - ECAL layers are processed from farthest to closest to the target
//     (ECOUT → ECIN → PCAL) so that the earliest physically meaningful ECAL response
//     is retained for each neutral candidate ("earliest ECAL layer wins").
//   - All matching is performed using sector, opening angle, and hit time criteria,
//     with opening angles defined using HitPath vectors (HitPos − electron vertex).
//
// Main components defined here:
//   - GenerateHits(...)   : builds NeutralHit / ChargedHit objects from reconstructed particles.
//   - neutSelection(...)  : orchestrates the veto passes across ECAL layers.
//   - AddNeutrons(...)    : applies the Ron-style veto logic (Steps 1–4) to a single ECAL layer.
//   - GetNewNeutronFDVetoHits(...): convenience wrapper returning veto-passing NeutralHit objects.
//
// Design philosophy:
//   - Preserve Ron Wagner’s original veto logic and physics intent exactly.
//   - Make all geometry, timing, and ordering assumptions explicit in code and comments.
//   - Favor strict validation and verbose debug output over silent failure.
//

#ifndef FD_NEUTRAL_VETO_H
#define FD_NEUTRAL_VETO_H

#include <TVector3.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// ROOT (debug histograms)
#include <TDirectory.h>
#include <TH1D.h>
#include <TH2D.h>

// Physics constants used in TOF-based neutron reconstruction
#include "../../namespaces/general_utilities/constants.h"

// Include libraries:
#include "../../namespaces/general_utilities/utilities.h"
#include "../../namespaces/setup/path_definitions.h"

// Include classes:
#include "../../classes/AMaps/AMaps.cpp"
#include "../../classes/NeutronVetoRon/ChargedHit.h"
#include "../../classes/NeutronVetoRon/NeutralHit.h"
#include "../../classes/ParticleID/ParticleID.cpp"
#include "../../classes/clas12ana/clas12ana.h"

// Include CLAS12 libraries:
#include "../../includes/clas12_include.h"

namespace bt = basic_tools;
namespace am = analysis_math;
namespace raf = reco_analysis_functions;
namespace hf = histogram_functions;

// =================================================================================================
// FD neutral veto (Ron-style) — high-level description
// =================================================================================================
// Overview:
//   1) GenerateHits(...) builds *hit objects* (NeutralHit / ChargedHit) for FD particles that fired
//      any of: FTOF (1A/1B/2) and/or ECAL (PCAL/ECIN/ECOUT). A single reconstructed particle can
//      contribute multiple hit objects (e.g., a hit in PCAL and ECIN produces two NeutralHit entries).
//
// Detector order along the beamline (closest to target → farthest downstream):
//   FTOF  →  PCAL  →  ECIN  →  ECOUT
//   (EC  = ECIN+ECOUT,  ECAL = PCAL+EC)
//
// Definition of "related" hits (used throughout Steps 1–4):
//   - Δsector == dSector_cut   (with wrap-around-aware distance on 6 sectors; currently dSector_cut=0)
//   - opening angle < Opening_ang_cut_deg between HitPath vectors
//       * HitPath is defined in NeutralHit as: HitPath = HitPos - Vtx_e
//   - |Δt| < dHitTime_cut using HitTime
//
// Veto algorithm (implemented by neutSelection(...) + AddNeutrons(...)):
//   We process ECAL layers from farthest to closest (ECOUT → ECIN → PCAL). For each tested layer, we apply:
//
//   Step 1: FTOF neutral veto
//     - If a tested ECAL neutral hit matches ANY neutral hit in FTOF, veto the tested ECAL hit.
//
//   Step 2: charged veto
//     - If a tested ECAL neutral hit matches ANY charged hit in the reference set (FTOF through the tested layer),
//       veto the tested ECAL hit.
//
//   Step 3: "earliest ECAL layer wins" (earlier-layer neutral veto + bookkeeping)
//     - If a tested (later/downstream) ECAL hit matches ANY neutral hit in an earlier/closer-to-target ECAL layer,
//       veto the tested hit, so that only the earliest ECAL layer remains for that candidate.
//     - Bookkeeping: increment num_of_ECAL_hits_ahead on the earlier-layer hit (it saw a matching later-layer hit).
//       This counter persists across passes because neutSelection uses pointer views into the same local nHit storage.
//
//   Step 4: same-layer clustering
//     - This is a mutation/deduplication step: within the same tested layer, merge (cluster) multiple matching neutral hits into one representative by
//       averaging HitPath, and mark neighbors as blocking so they are not kept separately.
//     - IMPORTANT: Step 4 MUST only run for a seed hit that is still viable after Steps 1–3 (i.e., not vetoed).
//       Vetoed hits cannot mark others as blocking or become cluster leaders.
//     - IMPORTANT: Step 4 MUST also only run for a seed hit that is not already marked as `blocking`.
//       A hit that is already blocked has already been claimed as a duplicate by an earlier seed, and must not mutate other hits.
//     - Note: HitTime is NOT averaged in clustering; the seed hit's HitTime is kept.
//
// Implicit / indirect behavior (formerly "Step 5"):
//   - Ron Wagner’s original code included an explicit "3-of-3 ECAL layers" veto (PCAL+ECIN+ECOUT).
//     That explicit Step 5 block has been removed in this refactored version.
//
//   - HOWEVER, a closely related rejection is still implemented *indirectly* via the cross-pass
//     bookkeeping counter `num_of_ECAL_hits_ahead` and the final acceptance cut:
//         (Test_Layer_neut_hits[i]->num_of_ECAL_hits_ahead < 2)
//
//     How this works:
//       * In Step 3, when an earlier-layer ECAL hit matches a later-layer test hit, we veto the
//         later-layer hit and increment `num_of_ECAL_hits_ahead` on the earlier-layer hit.
//       * Because `neutSelection()` uses pointer views into the same local `nHit` storage, those
//         increments persist across ECOUT → ECIN → PCAL passes.
//       * For a true 3-layer response where one candidate produces matching hits in PCAL+ECIN+ECOUT,
//         the PCAL hit will accumulate two "ahead" matches (ECIN and ECOUT), i.e.:
//             PCAL.num_of_ECAL_hits_ahead == 2
//         and therefore FAIL the `< 2` cut during the PCAL pass.
//
//   - Therefore, while there is no explicit "3-of-3" veto statement in the code anymore,
//     the current implementation still rejects many (and in particular PCAL-rooted) 3-layer
//     ECAL responses through this ordering + bookkeeping mechanism.
//
//   - IMPORTANT: this indirect behavior is *structural* ("PCAL saw both later layers") rather than
//     a direct particle-level rule ("this particle fired all three layers").
// =================================================================================================

class FDNeutralVeto {
   public:
    // -------------------------------------------------------------------------
    // Constructor / Destructor
    // -------------------------------------------------------------------------

    FDNeutralVeto();  // default constructor

    ~FDNeutralVeto();  // default destructor

    // -------------------------------------------------------------------------
    // Veto parameter configuration (GLOBAL, READ-ONLY)
    // -------------------------------------------------------------------------
    const double dTime_cut = 5.0;            // [ns]
    const double dHitTime_cut = 5.0;         // [ns]
    const double Opening_ang_cut_deg = 5.0;  // [deg]
    const int dSector_cut = 0;               // [sectors]

    // -------------------------------------------------------------------------
    // FD Neutral Veto Step Control Flags
    // -------------------------------------------------------------------------
    const bool Apply_FTOF_Neut_Veto = true;            // Step 1
    const bool Apply_Charged_Veto = true;              // Step 2
    const bool Apply_ECAL_Earliest_Layer_Veto = true;  // Step 3
    const bool Apply_Same_Layer_Clustering = true;     // Step 4

    // -------------------------------------------------------------------------
    // Debug histograms configuration
    // -------------------------------------------------------------------------
    struct DebugHistConfig {
        bool enable = false;

        // If non-null, histograms are booked under this directory.
        // If null, EnableDebugHists will attempt to use gDirectory.
        TDirectory* baseDir = nullptr;

        // Book 2D histograms for the step variables (all i<j pairs).
        bool book2DStepHists = true;

        // If true, also write canvases for every histogram at FinalizeDebugHists.
        // This can generate many objects. Default false.
        bool writeCanvases = false;
    };

    // Opaque internal debug-histogram manager.
    // Forward-declared here so the implementation file can name the type when
    // defining  members, while keeping the full definition in the .cpp.
    struct DebugHists;

    // Enable/disable debugging histograms.
    // Call once before event loop. You can pass a TFile or any TDirectory.
    void EnableDebugHists(const DebugHistConfig& cfg);

    // Write all histograms (and optional canvases) to the configured directory.
    // Call once after event loop.
    void FinalizeDebugHists();

    // -------------------------------------------------------------------------
    // Public API
    // -------------------------------------------------------------------------

    // Per event start time for TOF based neutron momentum reconstruction
    // You must call this once per event before calling GetNewNeutronFDVetoHits if debug hists are enabled
    void SetStartTime(double starttime);

    std::tuple<std::vector<NeutralHit>, std::vector<ChargedHit>> GenerateHits(std::vector<region_part_ptr>& allParticles, region_part_ptr& electron);

    std::vector<NeutralHit> neutSelection(std::vector<NeutralHit> nHit, std::vector<ChargedHit> cHit);

    void AddNeutrons(const std::vector<NeutralHit*>& FTOF_neut_hits, const std::vector<NeutralHit*>& Mid_neut_hits, const std::vector<ChargedHit*>& Ch_hits,
                     std::vector<NeutralHit*>& Test_Layer_neut_hits, std::vector<NeutralHit>& Neut_hits_after_veto);

    std::vector<NeutralHit> GetNewNeutronFDVetoHits(std::vector<region_part_ptr>& allParticles, region_part_ptr& electron);

    // -------------------------------------------------------------------------
    // Calc_nFD_vars
    // -------------------------------------------------------------------------
    // Purpose:
    //   Compute TOF-based neutral (neutron) kinematics from a NeutralHit.
    //   This is the class-level version used by the debug histogram instrumentation.
    //
    //   NOTE: This is NOT identical to the original lambda you provided earlier.
    //   The original lambda performs the raw calculation without guarding against
    //   non-physical inputs. This implementation adds minimal safety guards:
    //     - If ToF <= 0, returns beta and gamma as NAN and a zero-magnitude momentum vector.
    //     - If beta is not in (0,1), returns beta and gamma as NAN and a zero-magnitude momentum vector.
    //
    // Inputs:
    //   - h         : NeutralHit (must already contain HitPath and HitTime)
    //   - starttime : event start time [ns]
    //
    // Returned tuple:
    //   (ToF [ns], beta, gamma, Pvec [GeV])
    //
    // Physics details (matches the original intent):
    //   - ToF   = HitTime - starttime
    //   - beta  = |HitPath| / (ToF * c)
    //   - gamma = 1 / sqrt(1 - beta^2)
    //   - p     = m_n * beta * gamma
    //   - Pvec direction is taken from HitPath (theta/phi)
    std::tuple<double, double, double, TVector3> Calc_nFD_vars(const NeutralHit& h, double starttime) const {
        // 1) Copy the path from the electron vertex to the neutral hit position.
        //    This is the same vector you use throughout the veto for angular matching.
        const TVector3 hitPath = h.HitPath;

        // 2) Compute time-of-flight relative to the provided event start time.
        //    Units are assumed to be [ns] consistently with HitTime and starttime.
        const double tof = h.HitTime - starttime;

        // 3) Guard against non-physical or undefined TOF.
        //    Negative/zero TOF would cause beta and momentum to blow up.
        if (tof <= 0.0) {
            TVector3 pvec;
            pvec.SetMagThetaPhi(0.0, hitPath.Theta(), hitPath.Phi());
            return {tof, std::nan(""), std::nan(""), pvec};
        }

        // 4) Compute beta = L / (c * ToF).
        //    - hitPath.Mag() is the flight path length [cm].
        //    - constants::c is speed of light [cm/ns].
        const double beta = hitPath.Mag() / (tof * constants::c);

        // 5) Guard against non-physical beta.
        //    beta must be strictly between 0 and 1 for a massive particle.
        if (!(beta > 0.0) || !(beta < 1.0)) {
            TVector3 pvec;
            pvec.SetMagThetaPhi(0.0, hitPath.Theta(), hitPath.Phi());
            return {tof, std::nan(""), std::nan(""), pvec};
        }

        // 6) Compute gamma = 1 / sqrt(1 - beta^2).
        const double gamma = 1.0 / std::sqrt(1.0 - beta * beta);

        // 7) Compute neutron momentum magnitude p = m_n * beta * gamma.
        //    constants::m_n is neutron mass in [GeV].
        const double p = constants::m_n * beta * gamma;

        // 8) Build a 3-vector with magnitude p and direction along HitPath.
        TVector3 pvec;
        pvec.SetMagThetaPhi(p, hitPath.Theta(), hitPath.Phi());

        // 9) Return the TOF-derived kinematics.
        return {tof, beta, gamma, pvec};
    }

   private:
    // -----------------------------------------------------------------------------
    // AbortWithContext (GenerateHits helper)
    // -----------------------------------------------------------------------------
    [[noreturn]] void AbortWithContext_GenerateHits(const std::string& message, const char* file, int line, const char* func);

    int SectorDistance(int s1, int s2) const {
        const int ds = std::abs(s1 - s2);
        return std::min(ds, 6 - ds);
    }

    double WrapPhiDeg(double dphi) const;

    // -------------------------------------------------------------------------
    // Debug histogram internals
    // -------------------------------------------------------------------------
    DebugHistConfig m_dbgCfg{};
    DebugHists* m_dbg = nullptr;  // owned, allocated when enabled
    double m_starttime = 0.0;

    void BookIfNeeded();

    // ---------------------------------------------------------------------
    // Debug histogram fill API
    // ---------------------------------------------------------------------
    // These functions do NOT change veto logic. They only record diagnostics.
    //
    // General histograms:
    //   - "PreVeto"  : filled after GenerateHits builds the raw hit objects.
    //   - "PostVeto" : filled after neutSelection returns survivors.
    //
    // Step histograms (inside AddNeutrons):
    //   - "stepX_before" is filled for every test/ref comparison attempted
    //     in that step (so you can see the distribution entering the cut).
    //   - "stepX_after" is filled once per tested hit, categorized as
    //     survivor or vetoed at that step.
    //
    // 2D histograms:
    //   - When enabled, all unique variable pairs (i<j) for the step variables
    //     are booked/fill per stage. This can be very large output.
    void FillGeneralHits_PreVeto(const std::vector<NeutralHit>& nHits, const std::vector<ChargedHit>& cHits);
    void FillGeneralHits_PostVeto(const std::vector<NeutralHit>& nSurvivors);

    // ---------------------------------------------------------------------
    // Step hist filling helpers
    // ---------------------------------------------------------------------
    // These helpers record, per veto step and per stage, the diagnostic
    // variables you requested:
    //   - Differences: dp, dtheta, dphi, dtime, dhittime, dvx, dvy, dvz
    //   - Opening angle between HitPath vectors
    //   - Sector distance (wrap-around aware)
    //
    // Stage naming convention:
    //   - "all_before_veto" : raw hits exist, before any AddNeutrons decisions
    //   - "step1_before"    : right before the Step 1 cut is evaluated
    //   - "step1_after"     : after Step 1 decision for the current test hit
    //   - ...
    //   - "after_all_veto"  : final survivors after all steps
    //
    // Fill per-comparison pair diagnostics for neutral-vs-neutral comparisons.
    void FillStepPair_NeutNeut(const std::string& stage, const NeutralHit& test, const NeutralHit& ref);

    // Fill per-comparison pair diagnostics for neutral-vs-charged comparisons.
    void FillStepPair_NeutCh(const std::string& stage, const NeutralHit& test, const ChargedHit& ref);

    // Fill tested-hit kinematics as a survivor at the given stage.
    void FillStepSurvivor(const std::string& stage, const NeutralHit& h);

    // Fill tested-hit kinematics as vetoed at the given stage.
    void FillStepVetoed(const std::string& stage, const NeutralHit& h);
};

#endif  // FD_NEUTRAL_VETO_H
