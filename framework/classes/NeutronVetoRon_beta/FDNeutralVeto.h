//
// Obtained from Ron Wagner on 06/11/2025.
// Refactored, extended, and documented by Alon Sportes.
//
// FDNeutralVeto.h (After Veto Reformat)
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
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// Physics constants used in TOF-based neutron reconstruction
#include "../../namespaces/general_utilities/constants.h"
#include "nfdveto.h"

// Include libraries:
#include "../../namespaces/general_utilities/utilities.h"
#include "../../namespaces/setup/path_definitions.h"

// Include classes:
#include "../../classes/ParticleID/ParticleID.cpp"
#include "../../classes/clas12ana/clas12ana.h"
#include "ChargedHit.h"
#include "FDNeutralVetoDebug.cpp"
#include "NeutralHit.h"

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
    // Topology-aware multi-layer policy: handling of indirect 3-layer ECAL rejection
    // -------------------------------------------------------------------------
    // Motivation (physics):
    //   - Photons (beta≈1) can naturally light up multiple ECAL layers.
    //   - Neutrons (beta<1) can also create multi-layer responses, especially at high momentum.
    //   - The original Ron-style logic rejected many 3-layer responses indirectly via:
    //       num_of_ECAL_hits_ahead < 2
    //   - This policy keeps Steps 1–4 identical and makes that indirect rejection conditional,
    //     based on TOF-derived beta classification of the earliest-layer ECAL hit.
    const bool Apply_TopologyAwareMultiLayerPolicy = false;

    const bool Apply_TopologyAwareMultiLayerRejection = (Apply_TopologyAwareMultiLayerPolicy && true);
    // const bool Apply_TopologyAwareMultiLayerRejection = true;

    // Candidate classification thresholds (used only when Apply_TopologyAwareMultiLayerRejection==true):
    //   - beta is computed from (HitPath, HitTime, starttime) via Calc_nFD_vars.
    //   - These are tunable analysis parameters; defaults are conservative.
    const double MultiLayerPolicy_beta_photon_like_min = 0.95;
    const double MultiLayerPolicy_beta_neutron_like_max = 0.90;

    // -------------------------------------------------------------------------
    // Debug histograms configuration
    // -------------------------------------------------------------------------
    using DebugHistConfig = nfdveto::DebugHistConfig;

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

   private:
    // -----------------------------------------------------------------------------
    // AbortWithContext_GenerateHits (private helper)
    // -----------------------------------------------------------------------------
    // Purpose:
    //   Provide a context-rich abort mechanism consistent with the hit-struct
    //   AbortWithContext style (file/line/function + message), but for free
    //   functions like GenerateHits where we do not yet have a hit object to dump.
    //
    // Notes:
    //   - Intended for strict debugging/validation: abort on unexpected/invalid
    //     event content (null particle pointers, missing particle kinematics, etc.).
    //   - Uses default arguments so the call-site file/line/function are captured.
    // -----------------------------------------------------------------------------
    [[noreturn]] inline void AbortWithContext_GenerateHits(const std::string& message, const char* file, int line,
#ifdef __GNUC__
                                                           const char* func
#else
                                                           const char* func
#endif
    ) {
        std::cout << "\n";
        std::cout << file << ":" << line << " in " << func << " -> \033[31mERROR!\033[0m " << message << "\n";
        std::cout << "Aborting...\n\n";
        std::exit(1);
    }

    double WrapPhiDeg(double dphi) const;

    enum class MultiLayerCandidateType { PhotonLike, NeutronLike, Ambiguous };

    // Multi-layer candidate classifier.
    // Physics intent:
    //   - Use TOF-derived beta as a proxy to separate beta≈1 (photon-like) from delayed (neutron-like).
    //   - Ambiguous is used when beta is in-between; by default we treat ambiguous as neutron-like
    //     for efficiency unless analysis policy changes.
    MultiLayerCandidateType ClassifyMultiLayerCandidate(const NeutralHit& earliestLayerHit) const;

    // Multi-layer policy decision: whether to apply the indirect "3-layer" rejection (num_of_ECAL_hits_ahead < 2)
    // for the current earliest-layer ECAL survivor.
    bool ShouldApplyIndirectMultiLayerRejection(const NeutralHit& earliestLayerHit) const;

    // -------------------------------------------------------------------------
    // Debug histogram internals
    // -------------------------------------------------------------------------
    DebugHistConfig m_dbgCfg{};
    std::unique_ptr<FDNeutralVetoDebug> m_dbg;
    double m_starttime = 0.0;

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
