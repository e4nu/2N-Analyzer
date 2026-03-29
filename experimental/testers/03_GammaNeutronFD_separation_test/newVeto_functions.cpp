//
// Obtained from Ron Wagner on 06/11/2025.
// Refactored, extended, and documented by Alon Sportes.
//
// newVeto_functions.cpp
// ---------------------
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

#ifndef NEWVETO_FUNCTIONS
#define NEWVETO_FUNCTIONS

#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TStyle.h>
#include <TTree.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <tuple>
#include <typeinfo>
#include <vector>

// Include libraries:
#include "../../framework/namespaces/general_utilities/utilities.h"
#include "../../framework/namespaces/setup/path_definitions.h"

// Include classes:
#include "../../framework/classes/AMaps/AMaps.cpp"
#include "../../framework/classes/NeutronVetoRon/ChargedHit.h"
#include "../../framework/classes/NeutronVetoRon/NeutralHit.h"
#include "../../framework/classes/ParticleID/ParticleID.cpp"
#include "../../framework/classes/clas12ana/clas12ana.h"

// Include CLAS12 libraries:
#include "../../framework/includes/clas12_include.h"

// Include the FDNeutralVeto class definition:
#include "../../framework/classes/NeutronVetoRon/FDNeutralVeto.cpp"

using namespace utilities;
using std::string;
using std::vector;

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
//     - Within the same tested layer, merge (cluster) multiple matching neutral hits into one representative by
//       averaging HitPath, and mark neighbors as blocking so they are not kept separately.
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

#pragma region /* Veto parameter configuration */

// -----------------------------------------------------------------------------
// Veto parameter configuration (GLOBAL, READ-ONLY)
// -----------------------------------------------------------------------------
// The following constants define the core matching cuts used throughout the FD neutral veto logic.
// These parameters are GLOBAL, READ-ONLY, and intentionally declared as 'const' to preserve
// the logic and physics intent of Ron Wagner’s original implementation.
//
// --- Physical meaning of each cut: ---
//   - dTime_cut [ns]:
//       * Maximum allowed absolute time difference (|Δt|) between candidate hits (in nanoseconds).
//       * Used for temporal matching between neutral and charged hits, or between hits in different layers.
//   - dHitTime_cut [ns]:
//       * Maximum allowed absolute time difference (|Δt|) between hits for opening angle and sector matching (in nanoseconds).
//       * Used in conjunction with opening angle and sector cuts for precise hit association.
//   - Opening_ang_cut_deg [degrees]:
//       * Maximum allowed opening angle (in degrees) between HitPath vectors (directions from electron vertex to hit).
//       * Ensures that only spatially coincident hits (i.e., those likely from the same particle) are considered related.
//   - dSector_cut [integer, sectors]:
//       * Maximum allowed sector difference (Δsector) between hits (on a 6-sector detector, with wrap-around).
//       * For dSector_cut=0, only hits in the same sector are considered matching.
//
// --- Why are all values set to 5? ---
//   - The value '5' (in ns or degrees) is chosen as a conservative, symmetric window that balances efficiency and background rejection.
//   - For time cuts (dTime_cut, dHitTime_cut): 5 ns is comparable to the timing resolution of CLAS12 ECAL/FTOF subdetectors,
//     wide enough to accommodate modest timing jitter but narrow enough to suppress accidental matches.
//   - For opening angle: 5 degrees is tight enough to avoid accidental geometric overlaps, but broad enough to account for
//     reconstruction and vertex uncertainties.
//   - For sector: dSector_cut=0 ensures only direct sector matches are considered (no cross-sector matching).
//
// --- Policy and maintainability notes: ---
//   - These parameters are GLOBAL and CONST to guarantee that all veto logic is applied consistently
//     and to safeguard the original physics intent of Ron Wagner’s code.
//   - DO NOT MODIFY these values or their const-ness lightly; any change may alter the physics performance
//     and would break strict reproducibility with legacy analyses.
//   - If future analyses require cut scans, run-dependent tuning, or dynamic configuration,
//     these should be refactored into a configurable struct or runtime parameters.
//     TODO: Refactor veto parameters into a user-configurable struct or runtime options
//           if/when cut scans or run-dependent tuning are needed for future analyses.
// -----------------------------------------------------------------------------

const double dTime_cut = 5.0;            // [ns]   Maximum allowed |Δt| between hits for matching
const double dHitTime_cut = 5.0;         // [ns]   Maximum allowed |Δt| for opening angle/sector matching
const double Opening_ang_cut_deg = 5.0;  // [deg]  Maximum allowed opening angle between HitPath vectors
const int dSector_cut = 0;               // [sectors] Maximum allowed sector difference (0 = same sector)

#pragma endregion

#pragma region /* FD Neutral Veto Step Control Flags */

// -----------------------------------------------------------------------------
//  FD Neutral Veto Step Control Flags
// -----------------------------------------------------------------------------
//  These flags are COMPILE-TIME step enable/disable switches for the FD neutral veto.
//  -------------------------------------------------------------------------------
//  - Each flag gates an entire veto block (Step 1–4) via 'if (Apply_...)' checks.
//  - The flags themselves perform NO logic; they simply enable or disable the
//    corresponding veto step at compile time.
//
//  Step-by-step behavior:
//    * If a flag is set to 'true', the corresponding veto step is APPLIED.
//    * If set to 'false', the entire step is SKIPPED (its code block is not executed),
//      and no vetoing or bookkeeping from that step occurs.
//
//    - Apply_FTOF_Neut_Veto:      Enables/disables Step 1 (FTOF neutral veto).
//    - Apply_Charged_Veto:        Enables/disables Step 2 (charged veto).
//    - Apply_ECAL_Earliest_Layer_Veto: Enables/disables Step 3 ("earliest ECAL layer wins").
//    - Apply_Same_Layer_Clustering:    Enables/disables Step 4 (same-layer clustering).
//
//  Physics and safety policy:
//    - These flags are declared 'const' to preserve Ron Wagner’s original physics logic
//      and to prevent accidental partial-veto configurations during production analysis.
//    - Disabling any step can significantly change physics behavior and should be done
//      ONLY for debugging or validation studies—not for normal analysis.
//
//  ⚠ WARNING:
//    - Changing any of these to 'false' disables the corresponding veto logic entirely,
//      which may admit backgrounds or alter selection efficiency in nontrivial ways.
//    - Always document and justify any temporary override.
//
//  Future note:
//    - If runtime (rather than compile-time) control is ever needed, these flags could
//      be refactored into a configuration struct or runtime options object.
// -----------------------------------------------------------------------------

const bool Apply_FTOF_Neut_Veto = true;            // Step 1
const bool Apply_Charged_Veto = true;              // Step 2
const bool Apply_ECAL_Earliest_Layer_Veto = true;  // Step 3
const bool Apply_Same_Layer_Clustering = true;     // Step 4

#pragma endregion

#pragma region /* AbortWithContext function (GenerateHits helper) */

// -----------------------------------------------------------------------------
// AbortWithContext (GenerateHits helper)
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

[[noreturn]] inline void AbortWithContext_GenerateHits(const std::string& message, const char* file = __FILE__, int line = __LINE__,
#ifdef __GNUC__
                                                       const char* func = "(unknown)"
#else
                                                       const char* func = __FUNCTION__
#endif
) {
    std::cout << "\n";
    std::cout << file << ":" << line << " in " << func << " -> \033[31mERROR!\033[0m " << message << "\n";
    std::cout << "Aborting...\n\n";
    std::exit(1);
}

#pragma endregion

#pragma region /* GenerateHits function */

// -----------------------------------------------------------------------------
// GenerateHits
// -----------------------------------------------------------------------------
// Purpose:
//   Build *hit-level* objects (NeutralHit / ChargedHit) from the event's
//   reconstructed particles. Each FD particle can contribute multiple
//   hit objects: one per fired detector component among
//   {FTOF1A, FTOF1B, FTOF2, PCAL, ECIN, ECOUT}.
//
// Output:
//   - NeutralsFD: NeutralHit objects for FD neutrals (PID 2112 or 22)
//   - ChargedFD : ChargedHit objects for FD charged particles
//
// Notes / conventions used here (must stay consistent with Hit structs):
//   - detector == 12  -> FTOF (bank read via sci())
//   - detector == 7   -> ECAL (bank read via cal())
//   - layer index (Ron-veto layer):
//       0 = FTOF, 1 = PCAL, 2 = ECIN, 3 = ECOUT
//   - hitIndex increments per *created hit object* (not per particle)
//   - pindex is the index of the source particle inside allParticles
//
// IMPORTANT (state fields used later by the veto):
//   - NeutralHit/ChargedHit objects carry per-hit state used by the veto,
//     such as `blocking` and `num_of_ECAL_hits_ahead`.
//   - Therefore `reset()` MUST initialize these fields each time a hit
//     object is created (otherwise counters/flags may leak between events).
//
// HitPath definition:
//   - NeutralHit defines HitPath = HitPos - Vtx_e, where Vtx_e is the
//     electron vertex. All opening-angle comparisons in the veto use
//     HitPath vectors (directions from the vertex to the hit).
// -----------------------------------------------------------------------------

std::tuple<std::vector<NeutralHit>, std::vector<ChargedHit>> GenerateHits(std::vector<region_part_ptr>& allParticles, region_part_ptr& electron) {
    const bool printOutNeutralHit = false;  // If true, print each NeutralHit after creation
    const bool printOutChargedHit = false;  // If true, print each ChargedHit after creation

    //////////////////////////////////////////////////////////////////////////
    // Collect all neutral/charged related hits in the FTOF + ECAL
    //////////////////////////////////////////////////////////////////////////

    std::vector<NeutralHit> NeutralsFD;  // Neutral particles in the FD (hit-objects)
    std::vector<ChargedHit> ChargedFD;   // Charged particles in the FD  (hit-objects)

    int nHitnum = 0;  // NeutralHit hitIndex counter
    int cHitnum = 0;  // ChargedHit hitIndex counter

    // Electron vertex is used by NeutralHit to define HitPath = HitPos - Vtx_e
    const TVector3 electronVtx(electron->par()->getVx(), electron->par()->getVy(), electron->par()->getVz());

    if (printOutNeutralHit || printOutChargedHit) { cout << "\n\n======================================================================================\n"; }

    // Loop over all reconstructed particles in the event
    for (size_t i = 0; i < allParticles.size(); i++) {
        // -----------------------------------------------------------------
        // Basic particle classification / gating
        // -----------------------------------------------------------------

        // Defensive: reject null pointers early
        if (!allParticles[i]) {
            std::ostringstream oss;
            oss << "GenerateHits: allParticles[" << i << "] is null";
            AbortWithContext_GenerateHits(oss.str());
        }
        if (!allParticles[i]->par()) {
            std::ostringstream oss;
            oss << "GenerateHits: allParticles[" << i << "]->par() is null";
            AbortWithContext_GenerateHits(oss.str());
        }

        const int pid = allParticles[i]->par()->getPid();          // PDG code
        const int q = allParticles[i]->par()->getCharge();         // particle charge
        const bool IsNeutral = (q == 0);                           // neutral definition
        const bool IsCharged = (q != 0);                           // charged definition
        const bool IsInFD = (allParticles[i]->getRegion() == FD);  // Forward Detector only
        const bool NonZeroPDG = (pid != 0);                        // require identified PID

        // Skip anything that cannot possibly produce FD veto hits by your criteria
        if (!IsInFD || !NonZeroPDG) { continue; }

        // -----------------------------------------------------------------
        // Detector hit flags (FTOF via sci(), ECAL via cal())
        // -----------------------------------------------------------------

        // FTOF hit flags:
        const bool FTOF1A_hit = (allParticles[i]->sci(clas12::FTOF1A)->getDetector() == 12);  // FTOF1A hit
        const bool FTOF1B_hit = (allParticles[i]->sci(clas12::FTOF1B)->getDetector() == 12);  // FTOF1B hit
        const bool FTOF2_hit = (allParticles[i]->sci(clas12::FTOF2)->getDetector() == 12);    // FTOF2 hit
        const bool FTOF_hit = (FTOF1A_hit || FTOF1B_hit || FTOF2_hit);

        // ECAL hit flags:
        const bool PCAL_hit = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
        const bool ECIN_hit = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
        const bool ECOUT_hit = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
        const bool ECAL_hit = (PCAL_hit || ECIN_hit || ECOUT_hit);

        // If there are no relevant detector hits, there is nothing to build.
        if (!FTOF_hit && !ECAL_hit) { continue; }

        // FD particle is neutral or charged flag (now that we know it has hits)
        const bool IsNeutralFD = (IsNeutral && (FTOF_hit || ECAL_hit));
        const bool IsChargedFD = (IsCharged && (FTOF_hit || ECAL_hit));

        if (IsNeutralFD) {
            // -----------------------------------------------------------------
            // Neutral branch: only neutrons (2112) and photons (22)
            // -----------------------------------------------------------------

            if (printOutNeutralHit) { cout << "IsNeutralFD\n"; }

            const bool FDneutralPDG = (pid == 2112 || pid == 22);
            if (!FDneutralPDG) {
                std::ostringstream oss;
                oss << "GenerateHits: FD neutral has PID=" << pid << " (expected 2112 or 22)";
                AbortWithContext_GenerateHits(oss.str());
            }

            // Count how many ECAL layers fired for this particle (0..3)
            int Num_of_ECAL_hits = 0;
            if (PCAL_hit) { Num_of_ECAL_hits++; }
            if (ECIN_hit) { Num_of_ECAL_hits++; }
            if (ECOUT_hit) { Num_of_ECAL_hits++; }

            // --- FTOF hit objects (layer index 0) ---
            if (FTOF1A_hit) {
                NeutralHit TempNeutralHit;
                TempNeutralHit.reset();
                TempNeutralHit.FillHitData(allParticles[i], (int)i, nHitnum, 12, 0, clas12::FTOF1A, PCAL_hit, ECIN_hit, ECOUT_hit, electronVtx, Num_of_ECAL_hits);
                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
                if (printOutNeutralHit) { TempNeutralHit.PrintOut(); }
            }

            if (FTOF1B_hit) {
                NeutralHit TempNeutralHit;
                TempNeutralHit.reset();
                TempNeutralHit.FillHitData(allParticles[i], (int)i, nHitnum, 12, 0, clas12::FTOF1B, PCAL_hit, ECIN_hit, ECOUT_hit, electronVtx, Num_of_ECAL_hits);
                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
                if (printOutNeutralHit) { TempNeutralHit.PrintOut(); }
            }

            if (FTOF2_hit) {
                NeutralHit TempNeutralHit;
                TempNeutralHit.reset();
                TempNeutralHit.FillHitData(allParticles[i], (int)i, nHitnum, 12, 0, clas12::FTOF2, PCAL_hit, ECIN_hit, ECOUT_hit, electronVtx, Num_of_ECAL_hits);
                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
                if (printOutNeutralHit) { TempNeutralHit.PrintOut(); }
            }

            // --- ECAL hit objects (layer index 1/2/3) ---
            if (PCAL_hit) {
                NeutralHit TempNeutralHit;
                TempNeutralHit.reset();
                TempNeutralHit.FillHitData(allParticles[i], (int)i, nHitnum, 7, 1, clas12::PCAL, PCAL_hit, ECIN_hit, ECOUT_hit, electronVtx, Num_of_ECAL_hits);
                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
                if (printOutNeutralHit) { TempNeutralHit.PrintOut(); }
            }

            if (ECIN_hit) {
                NeutralHit TempNeutralHit;
                TempNeutralHit.reset();
                TempNeutralHit.FillHitData(allParticles[i], (int)i, nHitnum, 7, 2, clas12::ECIN, PCAL_hit, ECIN_hit, ECOUT_hit, electronVtx, Num_of_ECAL_hits);
                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
                if (printOutNeutralHit) { TempNeutralHit.PrintOut(); }
            }

            if (ECOUT_hit) {
                NeutralHit TempNeutralHit;
                TempNeutralHit.reset();
                TempNeutralHit.FillHitData(allParticles[i], (int)i, nHitnum, 7, 3, clas12::ECOUT, PCAL_hit, ECIN_hit, ECOUT_hit, electronVtx, Num_of_ECAL_hits);
                NeutralsFD.push_back(TempNeutralHit);
                nHitnum++;
                if (printOutNeutralHit) { TempNeutralHit.PrintOut(); }
            }
        } else if (IsChargedFD) {
            // -----------------------------------------------------------------
            // Charged branch
            // -----------------------------------------------------------------

            if (printOutChargedHit) { cout << "IsChargedFD\n"; }

            // --- FTOF hit objects (layer index 0) ---
            if (FTOF1A_hit) {
                ChargedHit TempChargedHit;
                TempChargedHit.reset();
                TempChargedHit.FillHitData(allParticles[i], (int)i, cHitnum, 12, 0, clas12::FTOF1A, PCAL_hit, ECIN_hit, ECOUT_hit);
                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
                if (printOutChargedHit) { TempChargedHit.PrintOut(); }
            }

            if (FTOF1B_hit) {
                ChargedHit TempChargedHit;
                TempChargedHit.reset();
                TempChargedHit.FillHitData(allParticles[i], (int)i, cHitnum, 12, 0, clas12::FTOF1B, PCAL_hit, ECIN_hit, ECOUT_hit);
                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
                if (printOutChargedHit) { TempChargedHit.PrintOut(); }
            }

            if (FTOF2_hit) {
                ChargedHit TempChargedHit;
                TempChargedHit.reset();
                TempChargedHit.FillHitData(allParticles[i], (int)i, cHitnum, 12, 0, clas12::FTOF2, PCAL_hit, ECIN_hit, ECOUT_hit);
                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
                if (printOutChargedHit) { TempChargedHit.PrintOut(); }
            }

            // --- ECAL hit objects (layer index 1/2/3) ---
            if (PCAL_hit) {
                ChargedHit TempChargedHit;
                TempChargedHit.reset();
                TempChargedHit.FillHitData(allParticles[i], (int)i, cHitnum, 7, 1, clas12::PCAL, PCAL_hit, ECIN_hit, ECOUT_hit);
                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
                if (printOutChargedHit) { TempChargedHit.PrintOut(); }
            }

            if (ECIN_hit) {
                ChargedHit TempChargedHit;
                TempChargedHit.reset();
                TempChargedHit.FillHitData(allParticles[i], (int)i, cHitnum, 7, 2, clas12::ECIN, PCAL_hit, ECIN_hit, ECOUT_hit);
                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
                if (printOutChargedHit) { TempChargedHit.PrintOut(); }
            }

            if (ECOUT_hit) {
                ChargedHit TempChargedHit;
                TempChargedHit.reset();
                TempChargedHit.FillHitData(allParticles[i], (int)i, cHitnum, 7, 3, clas12::ECOUT, PCAL_hit, ECIN_hit, ECOUT_hit);
                ChargedFD.push_back(TempChargedHit);
                cHitnum++;
                if (printOutChargedHit) { TempChargedHit.PrintOut(); }
            }
        }  // End of neutral/charged branch
    }  // End of loop over allParticles

    // Return the built hit objects (no veto is applied here; veto happens later)
    return {NeutralsFD, ChargedFD};
}  // End of GenerateHits function

#pragma endregion

#pragma region /* AddNeutrons function */

// -----------------------------------------------------------------------------
// AddNeutrons
// -----------------------------------------------------------------------------
// Apply the Ron-style FD neutral veto to a *single* ECAL test layer of neutral hits.
//
// Inputs (POINTER VIEWS):
//   - FTOF_neut_hits      : pointers to NeutralHit objects in FTOF (Ron layer 0)
//   - Mid_neut_hits       : pointers to NeutralHit objects in ECAL layers *before* the test layer (exclude FTOF)
//   - Ch_hits             : pointers to ChargedHit objects from FTOF through the test layer (inclusive)
//   - Test_Layer_neut_hits: pointers to NeutralHit objects in the tested ECAL layer (PCAL/ECIN/ECOUT)
//   - Neut_hits_after_veto: output (by-value) NeutralHit objects appended when a test hit survives all steps
//
// Matching definition (used in Steps 1–4):
//   - Δsector == dSector_cut
//   - opening angle between HitPath vectors < Opening_ang_cut_deg
//   - |Δt| between HitTime values < dHitTime_cut
//
// Step summary (what each step does):
//   Step 1 (FTOF neutral veto):
//     If the test-layer neutral matches ANY FTOF neutral, veto the test-layer hit.
//   Step 2 (charged veto):
//     If the test-layer neutral matches ANY charged hit in Ch_hits, veto the test-layer hit.
//   Step 3 (earlier-ECAL neutral veto + bookkeeping):
//     If the test-layer neutral matches ANY earlier-layer ECAL neutral in Mid_neut_hits, veto the test-layer hit.
//     Additionally, increment `num_of_ECAL_hits_ahead` on the *earlier-layer* hit.
//   Step 4 (same-layer clustering):
//     Merge (cluster) multiple test-layer neutral hits that match each other by averaging HitPath components,
//     and mark neighbor hits as `blocking=true` so only one representative of that cluster survives.
//
// Bookkeeping: num_of_ECAL_hits_ahead
//   - This counter lives on a NeutralHit and counts how many matching hits were found in *later* ECAL layers.
//   - It is incremented in Step 3 on the *earlier-layer* hit when that hit matches the current later-layer test hit.
//   - Because `neutSelection()` builds pointer vectors into the same local `nHit` objects, these increments persist
//     across passes (ECOUT → ECIN → PCAL). Therefore, the final cut `num_of_ECAL_hits_ahead < 2` is meaningful.
//   - Concrete interpretation for PCAL (the earliest ECAL layer):
//       * PCAL matched 0 or 1 later ECAL layer (ECIN or ECOUT)  -> passes (<2)
//       * PCAL matched both ECIN and ECOUT (two later layers)   -> fails (>=2)
//
// IMPORTANT: When updating xHitPath/yHitPath/zHitPath we MUST call UpdateHitPath() to keep HitPath consistent.
// -----------------------------------------------------------------------------

void AddNeutrons(const std::vector<NeutralHit*>& FTOF_neut_hits, const std::vector<NeutralHit*>& Mid_neut_hits, const std::vector<ChargedHit*>& Ch_hits,
                 std::vector<NeutralHit*>& Test_Layer_neut_hits, std::vector<NeutralHit>& Neut_hits_after_veto) {
    // Apply the neutral veto to `Test_Layer_neut_hits` using earlier neutral hits (FTOF + intermediate layers)
    // and all charged hits up to the tested layer. Passing hits are appended to `Neut_hits_after_veto`.

    const bool printOutNeutralHit = false;  // Set true for verbose debugging of neutral veto decisions
    const bool printOutChargedHit = false;  // Set true for verbose debugging of charged hit comparisons

    if ((printOutNeutralHit || printOutChargedHit) && Test_Layer_neut_hits.size() > 0) {
        cout << "\n======================================================================================\n";
        cout << "======================================================================================\n";
        cout << "======================================================================================\n";
    }

    bool Bad_neutral_hit;

    // Loop over each candidate neutral hit in the test layer.
    for (size_t i = 0; i < Test_Layer_neut_hits.size(); i++) {
        if (printOutNeutralHit || printOutChargedHit) { cout << "\n\n======================================================================================\n"; }

        // `Bad_neutral_hit` is the veto decision flag for THIS test-layer hit.
        // If any of Steps 1–3 find a matching hit (neutral FTOF, charged, or earlier ECAL neutral),
        // we set this to true and do NOT accept this test-layer hit.
        Bad_neutral_hit = false;  // This flag is set to true if *any* veto condition matches.

        // TODO: HitPath vs hit-position matching
        //       All angular matching in the veto currently uses HitPath vectors
        //       (HitPos - Vtx_e), i.e. directions defined with respect to the
        //       reconstructed electron vertex. This choice suppresses sensitivity
        //       to the absolute detector geometry but implicitly assumes that the
        //       vertex is well reconstructed.
        //
        //       It may be worth cross-checking (or optionally supporting) angular
        //       matching using raw hit positions (x,y,z) instead of HitPath, to
        //       quantify sensitivity to vertex smearing or misreconstruction.
        TVector3 Test_vector_HitPath = Test_Layer_neut_hits[i]->HitPath;  // Direction (from electron vertex) to the test hit (used for opening-angle matching).
        double Test_Layer_HitTime = Test_Layer_neut_hits[i]->HitTime;     // Hit time of the test hit (used for |Δt| matching).
        int Test_Layer_sector = Test_Layer_neut_hits[i]->sector;          // Sector of the test hit (used for Δsector matching).

        // Helper: minimal sector distance on a 6-sector detector (1..6), accounting for wrap-around.
        // Example: sectors (1,6) are distance 1 rather than 5.
        auto SectorDistance = [](int s1, int s2) {
            const int ds = std::abs(s1 - s2);
            return std::min(ds, 6 - ds);
        };

#pragma region /* Step 1 */

        // ---------------------------------------------------------------------
        // Step 1: FTOF neutral veto
        // ---------------------------------------------------------------------
        // Veto this test-layer neutral if it matches ANY neutral hit in FTOF
        // under the standard matching cuts (Δsector, opening angle, |Δt|).

        if (Apply_FTOF_Neut_Veto) {
            for (size_t j = 0; j < FTOF_neut_hits.size(); j++) {
                // Compare the test-layer neutral to each FTOF neutral:
                //   - Δsector must equal dSector_cut
                //   - opening angle between HitPath vectors must be < Opening_ang_cut_deg
                //   - |Δt| must be < dHitTime_cut
                // If a match exists, this test-layer hit is vetoed (Step 1).
                TVector3 FTOF_vector_HitPath = FTOF_neut_hits[j]->HitPath;
                int FTOF_sector = FTOF_neut_hits[j]->sector;
                double FTOF_HitTime = FTOF_neut_hits[j]->HitTime;

                const int FTOF_dSec = SectorDistance(Test_Layer_sector, FTOF_sector);
                const double FTOF_opening_ang = FTOF_vector_HitPath.Angle(Test_vector_HitPath) * TMath::RadToDeg();
                const double FTOF_dT = TMath::Abs(Test_Layer_HitTime - FTOF_HitTime);
                const bool Step1_match = (FTOF_dSec == dSector_cut) && (FTOF_opening_ang < Opening_ang_cut_deg) && (FTOF_dT < dHitTime_cut);

                if (Step1_match) {
                    // A matching FTOF neutral suggests this ECAL neutral is not a clean candidate -> veto it.
                    Bad_neutral_hit = true;
                }

                if (Step1_match) {
                    if (printOutNeutralHit) {
                        Test_Layer_neut_hits[i]->DebugPrintOut({{"DeltaSector (step 1)", Test_Layer_neut_hits[i]->ToStr(FTOF_dSec, "")},
                                                                {"opening_ang (step 1)", Test_Layer_neut_hits[i]->ToStr(FTOF_opening_ang, "[deg]")},
                                                                {"DeltaTime (step 1)", Test_Layer_neut_hits[i]->ToStr(FTOF_dT, "[ns]")}},
                                                               "\033[31m");
                        FTOF_neut_hits[j]->PrintOut();
                    }
                } else {
                    if (printOutNeutralHit) {
                        Test_Layer_neut_hits[i]->DebugPrintOut({{"DeltaSector (step 1)", Test_Layer_neut_hits[i]->ToStr(FTOF_dSec, "")},
                                                                {"opening_ang (step 1)", Test_Layer_neut_hits[i]->ToStr(FTOF_opening_ang, "[deg]")},
                                                                {"DeltaTime (step 1)", Test_Layer_neut_hits[i]->ToStr(FTOF_dT, "[ns]")}},
                                                               "\033[32m");
                        FTOF_neut_hits[j]->PrintOut();
                    }
                }
            }  // End of FTOF_neut_hits loop
        }  // End of Step 1

#pragma endregion

#pragma region /* Step 2 */

        // ---------------------------------------------------------------------
        // Step 2: charged veto
        // ---------------------------------------------------------------------
        // Veto this test-layer neutral if it matches ANY charged hit in the
        // reference set (FTOF through the test layer) under the same cuts.

        if (Apply_Charged_Veto) {
            for (size_t j = 0; j < Ch_hits.size(); j++) {
                // Compare the test-layer neutral to each charged hit in the reference set (FTOF..test-layer):
                // A match implies the neutral candidate is likely contaminated by charged activity -> veto (Step 2).
                TVector3 Charged_vector_Hit = Ch_hits[j]->HitPath;
                int Charged_sector = Ch_hits[j]->sector;
                double Charged_HitTime = Ch_hits[j]->HitTime;

                const int Charged_dSec = SectorDistance(Test_Layer_sector, Charged_sector);
                const double Charged_opening_ang = Charged_vector_Hit.Angle(Test_vector_HitPath) * TMath::RadToDeg();
                const double Charged_dT = TMath::Abs(Test_Layer_HitTime - Charged_HitTime);
                const bool Step2_match = (Charged_dSec == dSector_cut) && (Charged_opening_ang < Opening_ang_cut_deg) && (Charged_dT < dHitTime_cut);

                if (Step2_match) { Bad_neutral_hit = true; }

                if (Step2_match) {
                    if (printOutNeutralHit) {
                        Test_Layer_neut_hits[i]->DebugPrintOut({{"DeltaSector (step 2)", Test_Layer_neut_hits[i]->ToStr(Charged_dSec, "")},
                                                                {"opening_ang (step 2)", Test_Layer_neut_hits[i]->ToStr(Charged_opening_ang, "[deg]")},
                                                                {"DeltaTime (step 2)", Test_Layer_neut_hits[i]->ToStr(Charged_dT, "[ns]")}},
                                                               "\033[31m");
                    }

                    if (printOutChargedHit) { Ch_hits[j]->PrintOut(); }
                } else {
                    if (printOutNeutralHit) {
                        Test_Layer_neut_hits[i]->DebugPrintOut({{"DeltaSector (step 2)", Test_Layer_neut_hits[i]->ToStr(Charged_dSec, "")},
                                                                {"opening_ang (step 2)", Test_Layer_neut_hits[i]->ToStr(Charged_opening_ang, "[deg]")},
                                                                {"DeltaTime (step 2)", Test_Layer_neut_hits[i]->ToStr(Charged_dT, "[ns]")}},
                                                               "\033[32m");
                    }

                    if (printOutChargedHit) { Ch_hits[j]->PrintOut(); }
                }
            }  // End of Ch_hits loop
        }  // End of Step 2

#pragma endregion

#pragma region /* Step 3 */

        // ---------------------------------------------------------------------
        // Step 3: earlier-ECAL neutral veto + bookkeeping
        // ---------------------------------------------------------------------
        // Enforce "earliest ECAL layer wins": veto this test-layer neutral if it
        // matches ANY earlier-layer ECAL neutral (Mid_neut_hits) under the same cuts.
        // Bookkeeping: increment num_of_ECAL_hits_ahead on the EARLIER-LAYER hit
        // (it has one more matching hit in a later ECAL layer).

        if (Apply_ECAL_Earliest_Layer_Veto) {
            for (size_t j = 0; j < Mid_neut_hits.size(); j++) {
                // Mid_neut_hits contains ONLY ECAL neutrals in layers BEFORE the test layer (PCAL/ECIN as appropriate).
                // This step enforces "earliest ECAL layer wins": later-layer hits are vetoed if an earlier-layer match exists.
                const TVector3 Neutral_vector_HitPath = Mid_neut_hits[j]->HitPath;
                const double Neutral_HitTime = Mid_neut_hits[j]->HitTime;
                const int Neutral_sector = Mid_neut_hits[j]->sector;

                const int Neutral_dSec = SectorDistance(Test_Layer_sector, Neutral_sector);
                const double Neutral_opening_ang = Neutral_vector_HitPath.Angle(Test_vector_HitPath) * TMath::RadToDeg();
                const double Neutral_dT = TMath::Abs(Test_Layer_HitTime - Neutral_HitTime);
                const bool Step3_match = (Neutral_dSec == dSector_cut) && (Neutral_opening_ang < Opening_ang_cut_deg) && (Neutral_dT < dHitTime_cut);

                if (Step3_match) {
                    // Veto the later-layer test hit (earlier-layer hit exists and matches).
                    // Bookkeeping: increment on the EARLIER-LAYER hit:
                    //   Mid hit sees one more matching hit in a later ECAL layer ("ahead" of it).
                    // Because we are using pointers into the same nHit storage, this counter persists across passes.
                    Bad_neutral_hit = true;
                    Mid_neut_hits[j]->num_of_ECAL_hits_ahead++;
                }

                if (Step3_match) {
                    if (printOutNeutralHit) {
                        Test_Layer_neut_hits[i]->DebugPrintOut({{"DeltaSector (step 3)", Test_Layer_neut_hits[i]->ToStr(Neutral_dSec, "")},
                                                                {"opening_ang (step 3)", Test_Layer_neut_hits[i]->ToStr(Neutral_opening_ang, "[deg]")},
                                                                {"DeltaTime (step 3)", Test_Layer_neut_hits[i]->ToStr(Neutral_dT, "[ns]")}},
                                                               "\033[31m");
                        Mid_neut_hits[j]->PrintOut();
                    }
                } else {
                    if (printOutNeutralHit) {
                        Test_Layer_neut_hits[i]->DebugPrintOut({{"DeltaSector (step 3)", Test_Layer_neut_hits[i]->ToStr(Neutral_dSec, "")},
                                                                {"opening_ang (step 3)", Test_Layer_neut_hits[i]->ToStr(Neutral_opening_ang, "[deg]")},
                                                                {"DeltaTime (step 3)", Test_Layer_neut_hits[i]->ToStr(Neutral_dT, "[ns]")}},
                                                               "\033[32m");
                        Mid_neut_hits[j]->PrintOut();
                    }
                }
            }  // End of Mid_neut_hits loop
        }  // End of Step 3

#pragma endregion

#pragma region /* Step 4 */

        // ---------------------------------------------------------------------
        // Step 4: same-layer clustering
        // ---------------------------------------------------------------------
        // Merge (cluster) multiple test-layer neutral hits that match each other
        // under the same cuts by averaging HitPath, and mark neighbors as blocking.

        if (Apply_Same_Layer_Clustering) {
            // Cluster accumulators (seeded by the current hit):
            double xAvg = Test_Layer_neut_hits[i]->HitPath.X();
            double yAvg = Test_Layer_neut_hits[i]->HitPath.Y();
            double zAvg = Test_Layer_neut_hits[i]->HitPath.Z();
            double Num_of_near_neighbors = 1;

            if (!Test_Layer_neut_hits[i]->blocking) {
                for (size_t j = i + 1; j < Test_Layer_neut_hits.size(); j++) {
                    TVector3 Neutral_vector_HitPath = Test_Layer_neut_hits[j]->HitPath;
                    int Neutral_sector = Test_Layer_neut_hits[j]->sector;
                    double Neutral_HitTime = Test_Layer_neut_hits[j]->HitTime;

                    const int Neutral_dSec = SectorDistance(Test_Layer_sector, Neutral_sector);
                    const double Neutral_opening_ang = Neutral_vector_HitPath.Angle(Test_vector_HitPath) * TMath::RadToDeg();
                    const double Neutral_dT = TMath::Abs(Test_Layer_HitTime - Neutral_HitTime);
                    const bool Step4_match =
                        (Neutral_dSec == dSector_cut) && (Neutral_opening_ang < Opening_ang_cut_deg) && (Neutral_dT < dHitTime_cut) && (Test_Layer_neut_hits[j]->blocking == false);

                    if (Step4_match) {
                        Num_of_near_neighbors++;
                        xAvg += Test_Layer_neut_hits[j]->HitPath.X();
                        yAvg += Test_Layer_neut_hits[j]->HitPath.Y();
                        zAvg += Test_Layer_neut_hits[j]->HitPath.Z();
                        Test_Layer_neut_hits[j]->blocking = true;
                    }

                    if (Step4_match) {
                        if (printOutNeutralHit) {
                            Test_Layer_neut_hits[i]->DebugPrintOut({{"DeltaSector (step 4)", Test_Layer_neut_hits[i]->ToStr(Neutral_dSec, "")},
                                                                    {"opening_ang (step 4)", Test_Layer_neut_hits[i]->ToStr(Neutral_opening_ang, "[deg]")},
                                                                    {"DeltaTime (step 4)", Test_Layer_neut_hits[i]->ToStr(Neutral_dT, "[ns]")}},
                                                                   "\033[31m");
                            Test_Layer_neut_hits[j]->PrintOut();
                        }
                    } else {
                        if (printOutNeutralHit) {
                            Test_Layer_neut_hits[i]->DebugPrintOut({{"DeltaSector (step 4)", Test_Layer_neut_hits[i]->ToStr(Neutral_dSec, "")},
                                                                    {"opening_ang (step 4)", Test_Layer_neut_hits[i]->ToStr(Neutral_opening_ang, "[deg]")},
                                                                    {"DeltaTime (step 4)", Test_Layer_neut_hits[i]->ToStr(Neutral_dT, "[ns]")}},
                                                                   "\033[32m");
                            Test_Layer_neut_hits[j]->PrintOut();
                        }
                    }
                }
            }  // End of blocking check

            // If we found additional near neighbors (cluster size > 1), replace this hit's HitPath by the cluster average.
            // Only cluster-average if the hit is not vetoed by Steps 1–3 and is the cluster leader (not blocked).
            // We average position (HitPath components) but keep the seed HitTime unchanged.
            if ((Bad_neutral_hit == false) && (Num_of_near_neighbors > 1) && !Test_Layer_neut_hits[i]->blocking) {
                // Update the scalar components first, then call UpdateHitPath() to keep the cached TVector3 consistent.
                // TODO: Cluster timing policy (???): we average positions but keep the seed hit's HitTime.
                //       If timing matters for representative selection, consider defining how HitTime should be chosen for same-layer clusters.
                Test_Layer_neut_hits[i]->xHitPath = xAvg / Num_of_near_neighbors;
                Test_Layer_neut_hits[i]->yHitPath = yAvg / Num_of_near_neighbors;
                Test_Layer_neut_hits[i]->zHitPath = zAvg / Num_of_near_neighbors;
                Test_Layer_neut_hits[i]->UpdateHitPath();
            }
        }  // End of Step 4

#pragma endregion

        // TODO: Clarify policy consistency between Step 3 and the `num_of_ECAL_hits_ahead < 2` cut.
        //       Step 3 vetoes LATER-layer test hits when an earlier-layer match exists ("earliest ECAL layer wins").
        //       The `num_of_ECAL_hits_ahead` cut, however, is applied to the CURRENT test hit and is most relevant
        //       for PCAL (where Step 3 has no earlier ECAL layer to veto it). In that case it can reject PCAL hits
        //       that matched BOTH ECIN and ECOUT (num_of_ECAL_hits_ahead >= 2).

        // Final acceptance requires ALL of the following:
        //   (1) !Bad_neutral_hit : Steps 1–3 found no matching veto condition
        //   (2) !blocking        : this hit is the cluster representative for its same-layer group (Step 4)
        //   (3) num_of_ECAL_hits_ahead < 2 :
        //       This is a cross-pass bookkeeping cut.
        //       The counter is incremented in Step 3 on earlier-layer hits when they match later-layer test hits.
        //       Because neutSelection uses pointer views into the same nHit objects, the counter persists across
        //       ECOUT → ECIN → PCAL passes.
        //       Concrete meaning for PCAL:
        //         - matched 0 or 1 later ECAL layer -> accept
        //         - matched both ECIN and ECOUT     -> reject (>=2)
        // Indirect "3-of-3 ECAL" rejection (former Step 5):
        //   If a candidate truly produced matching hits in all three ECAL layers, then during earlier passes
        //   Step 3 will have incremented the PCAL hit's `num_of_ECAL_hits_ahead` twice (once for ECOUT and once
        //   for ECIN). The PCAL pass then rejects it via the `< 2` requirement. This reproduces much of the
        //   Step-5 physics effect without an explicit standalone Step 5 block.
        // TODO: Consider refactoring the indirect "3-of-3 ECAL" rejection (formerly Step 5) into an explicit, standalone veto block for clarity and maintainability.
        if (!Bad_neutral_hit && !Test_Layer_neut_hits[i]->blocking && (Test_Layer_neut_hits[i]->num_of_ECAL_hits_ahead < 2)) { Neut_hits_after_veto.push_back(*Test_Layer_neut_hits[i]); }
    }  // End of Test_Layer_neut_hits loop
}  // End of AddNeutrons function

#pragma endregion

#pragma region /* neutSelection function */

// -----------------------------------------------------------------------------
// neutSelection
// -----------------------------------------------------------------------------
// Purpose:
//   Apply the FD neutral veto across ECAL layers in a fixed order:
//     ECOUT (layer 3) → ECIN (layer 2) → PCAL (layer 1)
//
// Key design choice (pointer views for persistent bookkeeping):
//   - `nHit` and `cHit` are passed BY VALUE, so neutSelection owns stable local storage
//     for the duration of the function call.
//   - We then build layer-specific vectors as POINTERS into these local vectors:
//       vLay0n/vLay1n/vLay2n/vLay3n and vLay0c/.../vLay3c
//   - This is critical for Step 3 bookkeeping: `num_of_ECAL_hits_ahead` is incremented on earlier-layer hits when
//     they match later-layer test hits. Using pointers makes those increments persist across passes.
//
// Pass structure:
//   - ECOUT test pass:
//       Mid neutrals = PCAL + ECIN, charged refs = FTOF + PCAL + ECIN + ECOUT
//   - ECIN test pass:
//       Mid neutrals = PCAL,        charged refs = FTOF + PCAL + ECIN
//   - PCAL test pass:
//       Mid neutrals = empty,       charged refs = FTOF + PCAL
//
// Output:
//   - Returns the list of NeutralHit objects (by value) that survive all veto steps.
// -----------------------------------------------------------------------------

std::vector<NeutralHit> neutSelection(std::vector<NeutralHit> nHit, std::vector<ChargedHit> cHit) {
    std::vector<NeutralHit*> vLay0n;  // FTOF all parts (we want to throw any neutral related to those hits)
    std::vector<NeutralHit*> vLay1n;  // PCAL
    std::vector<NeutralHit*> vLay2n;  // ECIN
    std::vector<NeutralHit*> vLay3n;  // ECOUT

    std::vector<ChargedHit*> vLay0c;  // FTOF all
    std::vector<ChargedHit*> vLay1c;  // PCAL
    std::vector<ChargedHit*> vLay2c;  // ECIN
    std::vector<ChargedHit*> vLay3c;  // ECOUT

    // Temporary vectors that combine ALL preceding planes to be used as reference (veto) layers.
    // This is done to avoid cases when we have hit in layer 1 and 3, without hit in layer 2.
    std::vector<NeutralHit*> vTempLayersNeutral;
    std::vector<ChargedHit*> vTempLayersCharged;

    std::vector<NeutralHit> vNeut;
    vNeut.clear();

    // Neutral hits:
    for (size_t i = 0; i < nHit.size(); i++) {
        int d = nHit.at(i).detector;
        int l = nHit.at(i).layer;

        // FTOF detector num in clas12 is 12 and ECAL && PCAL num is 7
        if (d == 12) { vLay0n.push_back(&nHit.at(i)); }
        if (d == 7 && l == 1) { vLay1n.push_back(&nHit.at(i)); }
        if (d == 7 && l == 2) { vLay2n.push_back(&nHit.at(i)); }
        if (d == 7 && l == 3) { vLay3n.push_back(&nHit.at(i)); }
    }

    // Charged hits:
    for (size_t i = 0; i < cHit.size(); i++) {
        int d = cHit.at(i).detector;
        int l = cHit.at(i).layer;

        // FTOF detector num in clas12 is 12 and ECAL && PCAL num is 7
        if (d == 12) { vLay0c.push_back(&cHit.at(i)); }
        if (d == 7 && l == 1) { vLay1c.push_back(&cHit.at(i)); }
        if (d == 7 && l == 2) { vLay2c.push_back(&cHit.at(i)); }
        if (d == 7 && l == 3) { vLay3c.push_back(&cHit.at(i)); }
    }

    vNeut.clear();

    // We start from ECOUT neutrals and proceed down to PCAL (ECOUT → ECIN → PCAL).

    // ECOUT hits:
    // Charged hits:
    // vTempLayersCharged is now all the charged hits from the FTOF to the ECOUT:
    vTempLayersCharged.clear();
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay2c.begin(), vLay2c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay3c.begin(), vLay3c.end());

    // Neutral hits:
    vTempLayersNeutral.clear();
    vTempLayersNeutral.insert(vTempLayersNeutral.end(), vLay1n.begin(), vLay1n.end());
    vTempLayersNeutral.insert(vTempLayersNeutral.end(), vLay2n.begin(), vLay2n.end());

    // Pass 1: Test ECOUT neutrals (layer 3).
    //   - Step 3 compares against earlier ECAL neutrals (PCAL+ECIN) and vetoes ECOUT when a match exists.
    //   - Also increments num_of_ECAL_hits_ahead on those earlier-layer hits when they match ECOUT.
    AddNeutrons(vLay0n,              // Neutral hits in the FTOF
                vTempLayersNeutral,  // Neutral hits in the layers before the test layer and not the FTOF layer (first layer)
                vTempLayersCharged,  // All the charged hits from the FTOF to the test layer included
                vLay3n,              // The test layer of neutron hits (here - ECOUT)
                vNeut                // Neutral hits that passed the veto
    );

    // ECIN hits:
    // Charged hits:
    // vTempLayersCharged is now all the charged hits from the FTOF to the ECIN:
    vTempLayersCharged.clear();
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay2c.begin(), vLay2c.end());

    // Neutral hits:
    vTempLayersNeutral.clear();
    vTempLayersNeutral.insert(vTempLayersNeutral.end(), vLay1n.begin(), vLay1n.end());

    // Pass 2: Test ECIN neutrals (layer 2).
    //   - Step 3 compares against earlier ECAL neutrals (PCAL) and vetoes ECIN when a match exists.
    //   - Also increments num_of_ECAL_hits_ahead on PCAL hits when they match ECIN.
    AddNeutrons(vLay0n,              // Neutral hits in the FTOF
                vTempLayersNeutral,  // Neutral hits in the layers before the test layer and not the FTOF layer (first layer)
                vTempLayersCharged,  // All the charged hits from the FTOF to the test layer included
                vLay2n,              // The test layer of neutron hits (here - ECIN)
                vNeut                // Neutral hits that passed the veto
    );

    // PCAL hits:
    // Charged hits:
    // vTempLayersCharged is now all the charged hits from the FTOF to the PCAL:
    vTempLayersCharged.clear();
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());

    // Neutral hits:
    vTempLayersNeutral.clear();

    // Pass 3: Test PCAL neutrals (layer 1).
    //   - No earlier ECAL layer exists, so Step 3 has an empty Mid list (cannot veto PCAL by earlier ECAL neutrals).
    //   - However, PCAL hits already accumulated num_of_ECAL_hits_ahead during the ECOUT/ECIN passes.
    //     The final cut (num_of_ECAL_hits_ahead < 2) can therefore reject PCAL hits
    //   that matched BOTH ECIN and ECOUT.
    AddNeutrons(vLay0n,              // Neutral hits in the FTOF
                vTempLayersNeutral,  // Neutral hits in the layers before the test layer and not the FTOF layer (first layer)
                vTempLayersCharged,  // All the charged hits from the FTOF to the test layer included
                vLay1n,              // The test layer of neutron hits (here - PCAL)
                vNeut                // Neutral hits that passed the veto
    );

    return vNeut;
}  // End of neutSelection function

#pragma endregion

#pragma region /* NewNeutronFDVeto function (unused) */

// Get FD neutron as std::vector<region_part_ptr>:
std::vector<region_part_ptr> NewNeutronFDVeto(std::vector<region_part_ptr>& allParticles, region_part_ptr& electron, std::vector<int>& outputParticles_ind) {
    /*
    This function applies the new veto to neutrons in the FD

    allParticles - all particles in the event

    returns - allParticles with neutrons that passed the veto
    */

    auto [NeutralsFD, ChargedFD] = GenerateHits(allParticles, electron);

    std::vector<NeutralHit> NeutronsAfterVeto = neutSelection(NeutralsFD, ChargedFD);

    std::vector<region_part_ptr> outputParticles;
    outputParticles.clear();

    for (size_t i = 0; i < NeutronsAfterVeto.size(); i++) {
        int pindex = NeutronsAfterVeto.at(i).pindex;

        outputParticles.push_back(allParticles[pindex]);
        outputParticles_ind.push_back(pindex);
    }

    return outputParticles;
}  // End of NewNeutronFDVeto function

#pragma endregion

#pragma region /* GetNewNeutronFDVetoHits function */

// -----------------------------------------------------------------------------
// GetNewNeutronFDVetoHits
// -----------------------------------------------------------------------------
// Purpose:
//   Convenience wrapper that returns the *hit-level* neutrals (NeutralHit)
//   that survive the FD neutral veto.
//
// Inputs:
//   - allParticles      : reconstructed particles for the current event
//   - electron          : the reconstructed electron (used to define Vtx_e)
//   - outputParticles_ind: (currently unused in this function) kept for API
//                          symmetry with NewNeutronFDVeto(...)
//
// Output:
//   - Vector of NeutralHit objects that passed the veto logic in neutSelection(...)
//
// Notes:
//   - This function does NOT reduce allParticles; it only returns the surviving
//     NeutralHit entries.
//   - The veto itself is implemented in neutSelection(...) / AddNeutrons(...).
// -----------------------------------------------------------------------------

std::vector<NeutralHit> GetNewNeutronFDVetoHits(std::vector<region_part_ptr>& allParticles, region_part_ptr& electron) {
    // Step 0: build FD hit objects for this event (no veto yet)
    //   - NeutralsFD: all FD neutral hit-objects in FTOF/PCAL/ECIN/ECOUT
    //   - ChargedFD : all FD charged hit-objects in FTOF/PCAL/ECIN/ECOUT
    auto [NeutralsFD, ChargedFD] = GenerateHits(allParticles, electron);

    // Step 1: apply the FD neutral veto across ECAL layers
    //   - runs ECOUT → ECIN → PCAL passes
    //   - applies Steps 1–4 (FTOF neutral veto, charged veto, earliest-ECAL selection, same-layer clustering)
    std::vector<NeutralHit> NeutronsAfterVeto = neutSelection(NeutralsFD, ChargedFD);

    // Step 2: return surviving neutral hit objects
    return NeutronsAfterVeto;
}  // End of GetNewNeutronFDVetoHits function

#pragma endregion

#endif  // NEWVETO_FUNCTIONS