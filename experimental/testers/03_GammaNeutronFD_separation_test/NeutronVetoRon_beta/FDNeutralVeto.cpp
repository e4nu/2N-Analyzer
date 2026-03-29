//
// Obtained from Ron Wagner on 06/11/2025.
// Refactored, extended, and documented by Alon Sportes.
//
// FDNeutralVeto.cpp (After Veto Reformat)
// -----------------
// Implementation file for FDNeutralVeto class.
//

#include "FDNeutralVeto.h"

#include <TCanvas.h>
#include <TMath.h>

#include <cmath>  // std::isfinite

using namespace utilities;
using std::string;
using std::vector;

// =================================================================================================
// FD neutral veto (Ron-style) — debug histogram instrumentation
// =================================================================================================
// This implementation adds ROOT histograms that let us audit the veto at two levels:
//   (1) General hit-level kinematics (neutrals and charged) before veto and after veto.
//   (2) Step-by-step comparison distributions inside the veto (Steps 1–4), including:
//       - differences between "test" and "reference" objects (dp, dtheta, dphi, dtime, dhittime, dvx, dvy, dvz)
//       - opening angle between HitPath vectors
//       - wrap-around aware sector difference on the 6-sector detector
//       - survivor vs vetoed kinematics per step
//
// Matching definition (used throughout Steps 1–4) follows Ron Wagner’s original logic:
//   - Δsector == dSector_cut   (wrap-around aware distance on 6 sectors)
//   - opening angle < Opening_ang_cut_deg between HitPath vectors
//       HitPath is defined as: HitPath = HitPos − electron vertex
//   - |Δt| < dHitTime_cut using HitTime
//
// IMPORTANT:
//   - Neutron momentum / theta / phi are not stored as scalar members on NeutralHit.
//     They are computed from HitPath and TOF using `Calc_nFD_vars(...)` with the per-event start time.
//   - Therefore, all neutral kinematic histogram fills call Calc_nFD_vars(h, m_starttime).
// =================================================================================================

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------

FDNeutralVeto::FDNeutralVeto() = default;

double FDNeutralVeto::WrapPhiDeg(double dphi) const {
    while (dphi >= 180.0) dphi -= 360.0;
    while (dphi < -180.0) dphi += 360.0;
    return dphi;
}

FDNeutralVeto::MultiLayerCandidateType FDNeutralVeto::ClassifyMultiLayerCandidate(const NeutralHit& earliestLayerHit) const {
    // Classification uses TOF-derived beta as a proxy for particle speed.
    // Physics:
    //   - Photons travel at c => beta≈1.
    //   - Neutrons are massive => beta<1 (delayed).
    //   - At high momentum, neutron beta can approach 1, so an "ambiguous" band is kept.

    const auto [Calculated_ToF_nFD, Calculated_Beta_nFD, Calculated_Gamma_nFD, Calculated_reco_P_nFD_v] = nfdveto::Calc_nFD_vars(earliestLayerHit, m_starttime);

    // If beta is not a number (non-physical TOF), treat as ambiguous and let other veto steps dominate.
    if (!std::isfinite(Calculated_Beta_nFD)) { return MultiLayerCandidateType::Ambiguous; }

    if (Calculated_Beta_nFD >= MultiLayerPolicy_beta_photon_like_min) { return MultiLayerCandidateType::PhotonLike; }
    if (Calculated_Beta_nFD <= MultiLayerPolicy_beta_neutron_like_max) { return MultiLayerCandidateType::NeutronLike; }

    return MultiLayerCandidateType::Ambiguous;
}

bool FDNeutralVeto::ShouldApplyIndirectMultiLayerRejection(const NeutralHit& earliestLayerHit) const {
    // Indirect "3-layer" rejection corresponds to the bookkeeping-based cut:
    //   num_of_ECAL_hits_ahead < 2
    // which tends to reject cases where the earliest ECAL layer (PCAL) has matches in BOTH later layers.
    //
    // Multi-layer policy:
    //   - PhotonLike: keep strict topology rejection (apply the indirect cut).
    //   - NeutronLike: relax the indirect rejection to preserve high-p neutron efficiency.
    //   - Ambiguous: default to neutron-like behavior (efficiency-preserving). Adjust if analysis prefers purity.

    if (!Apply_TopologyAwareMultiLayerRejection) { return true; }

    const MultiLayerCandidateType type = ClassifyMultiLayerCandidate(earliestLayerHit);

    if (type == MultiLayerCandidateType::PhotonLike) { return true; }

    // NeutronLike or Ambiguous => do NOT apply the indirect topology rejection.
    return false;
}

// -----------------------------------------------------------------------------
// Destructor (defaulted)
// -----------------------------------------------------------------------------

FDNeutralVeto::~FDNeutralVeto() = default;

// -----------------------------------------------------------------------------
// Debug histogram public controls (pass-through to FDNeutralVetoDebug)
// -----------------------------------------------------------------------------

void FDNeutralVeto::EnableDebugHists(const DebugHistConfig& cfg) {
    // Store config locally so veto code can cheaply check the enable flag.
    m_dbgCfg = cfg;

    // If disabled, drop the debug helper.
    if (!m_dbgCfg.enable) {
        m_dbg.reset();
        return;
    }

    // Lazily create the debug helper and forward the configuration.
    if (!m_dbg) { m_dbg = std::make_unique<FDNeutralVetoDebug>(); }
    m_dbg->EnableDebugHists(cfg);

    // Keep debug helper in sync with the current event start time (if user sets it early).
    m_dbg->SetStartTime(m_starttime);
}

void FDNeutralVeto::FinalizeDebugHists() {
    if (!m_dbgCfg.enable) return;
    if (!m_dbg) return;
    m_dbg->FinalizeDebugHists();
}

// -----------------------------------------------------------------------------
// SetStartTime (forward to debug helper)
// -----------------------------------------------------------------------------

void FDNeutralVeto::SetStartTime(double starttime) {
    m_starttime = starttime;
    if (m_dbg) { m_dbg->SetStartTime(starttime); }
}

// -----------------------------------------------------------------------------
// Debug histogram fill API (thin forwarding wrappers)
// -----------------------------------------------------------------------------

void FDNeutralVeto::FillGeneralHits_PreVeto(const std::vector<NeutralHit>& nHits, const std::vector<ChargedHit>& cHits) {
    if (!m_dbgCfg.enable) return;
    if (!m_dbg) return;
    m_dbg->FillGeneralHits_PreVeto(nHits, cHits);
}

void FDNeutralVeto::FillGeneralHits_PostVeto(const std::vector<NeutralHit>& nSurvivors) {
    if (!m_dbgCfg.enable) return;
    if (!m_dbg) return;
    m_dbg->FillGeneralHits_PostVeto(nSurvivors);
}

void FDNeutralVeto::FillStepPair_NeutNeut(const std::string& stage, const NeutralHit& test, const NeutralHit& ref) {
    if (!m_dbgCfg.enable) return;
    if (!m_dbg) return;
    m_dbg->FillStepPair_NeutNeut(stage, test, ref);
}

void FDNeutralVeto::FillStepPair_NeutCh(const std::string& stage, const NeutralHit& test, const ChargedHit& ref) {
    if (!m_dbgCfg.enable) return;
    if (!m_dbg) return;
    m_dbg->FillStepPair_NeutCh(stage, test, ref);
}

void FDNeutralVeto::FillStepSurvivor(const std::string& stage, const NeutralHit& h) {
    if (!m_dbgCfg.enable) return;
    if (!m_dbg) return;
    m_dbg->FillStepSurvivor(stage, h);
}

void FDNeutralVeto::FillStepVetoed(const std::string& stage, const NeutralHit& h) {
    if (!m_dbgCfg.enable) return;
    if (!m_dbg) return;
    m_dbg->FillStepVetoed(stage, h);
}

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

#pragma region /* GenerateHits */

std::tuple<std::vector<NeutralHit>, std::vector<ChargedHit>> FDNeutralVeto::GenerateHits(std::vector<region_part_ptr>& allParticles, region_part_ptr& electron) {
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
            AbortWithContext_GenerateHits(oss.str(), __FILE__, __LINE__,
#ifdef __GNUC__
                                          "(unknown)"
#else
                                          __FUNCTION__
#endif
            );
        }
        if (!allParticles[i]->par()) {
            std::ostringstream oss;
            oss << "GenerateHits: allParticles[" << i << "]->par() is null";
            AbortWithContext_GenerateHits(oss.str(), __FILE__, __LINE__,
#ifdef __GNUC__
                                          "(unknown)"
#else
                                          __FUNCTION__
#endif
            );
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
                AbortWithContext_GenerateHits(oss.str(), __FILE__, __LINE__,
#ifdef __GNUC__
                                              "(unknown)"
#else
                                              __FUNCTION__
#endif
                );
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

    // Debug: fill general hists before veto
    FillGeneralHits_PreVeto(NeutralsFD, ChargedFD);

    // Return the built hit objects (no veto is applied here; veto happens later)
    return {NeutralsFD, ChargedFD};
}  // End of GenerateHits function

#pragma endregion

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

#pragma region /* AddNeutrons */

void FDNeutralVeto::AddNeutrons(const std::vector<NeutralHit*>& FTOF_neut_hits, const std::vector<NeutralHit*>& Mid_neut_hits, const std::vector<ChargedHit*>& Ch_hits,
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

        // Pointer to the current test-layer neutral hit.
        NeutralHit* test = Test_Layer_neut_hits[i];

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

        // Cache frequently used test-hit values for readability (physics logic unchanged).
        // HitPath: direction from electron vertex to hit position.
        const TVector3 Test_vector_HitPath = test->HitPath;

        // HitTime: detector-level time used for matching between hits.
        const double Test_Layer_HitTime = test->HitTime;

        // Sector: CLAS12 sector index (1..6) used for the wrap-around aware sector matching.
        const int Test_Layer_sector = test->sector;

        // Before any veto steps for this test hit
        // Fill as "all_before_veto" pairs only when we actually compare, so do nothing here.

#pragma region /* Step 1: FTOF neutral veto */

        // ---------------------------------------------------------------------
        // Step 1: FTOF neutral veto
        // ---------------------------------------------------------------------
        // Veto this test-layer neutral if it matches ANY neutral hit in FTOF
        // under the standard matching cuts (Δsector, opening angle, |Δt|).
        bool Step1_veto = false;

        if (Apply_FTOF_Neut_Veto) {
            for (size_t j = 0; j < FTOF_neut_hits.size(); j++) {
                NeutralHit* ref = FTOF_neut_hits[j];

                // Debug: fill pair-comparison distributions for Step 1 (neutral test vs neutral FTOF reference).
                FillStepPair_NeutNeut("step1_before", *test, *ref);

                // Compare the test-layer neutral to each FTOF neutral:
                //   - Δsector must equal dSector_cut
                //   - opening angle between HitPath vectors must be < Opening_ang_cut_deg
                //   - |Δt| must be < dHitTime_cut
                // If a match exists, this test-layer hit is vetoed (Step 1).
                const int FTOF_dSec = nfdveto::SectorDistance(Test_Layer_sector, ref->sector);
                const double FTOF_opening_ang = ref->HitPath.Angle(Test_vector_HitPath) * TMath::RadToDeg();
                const double FTOF_dT = TMath::Abs(Test_Layer_HitTime - ref->HitTime);
                const bool Step1_match = (FTOF_dSec == dSector_cut) && (FTOF_opening_ang < Opening_ang_cut_deg) && (FTOF_dT < dHitTime_cut);

                if (Step1_match) {
                    // A matching FTOF neutral suggests this ECAL neutral is not a clean candidate -> veto it.
                    Bad_neutral_hit = true;
                    Step1_veto = true;
                }

                if (Step1_match) {
                    if (printOutNeutralHit) {
                        test->DebugPrintOut({{"DeltaSector (step 1)", test->ToStr(FTOF_dSec, "")},
                                             {"opening_ang (step 1)", test->ToStr(FTOF_opening_ang, "[deg]")},
                                             {"DeltaTime (step 1)", test->ToStr(FTOF_dT, "[ns]")}},
                                            "\033[31m");
                        ref->PrintOut();
                    }
                } else {
                    if (printOutNeutralHit) {
                        test->DebugPrintOut({{"DeltaSector (step 1)", test->ToStr(FTOF_dSec, "")},
                                             {"opening_ang (step 1)", test->ToStr(FTOF_opening_ang, "[deg]")},
                                             {"DeltaTime (step 1)", test->ToStr(FTOF_dT, "[ns]")}},
                                            "\033[32m");
                        ref->PrintOut();
                    }
                }
            }  // End of FTOF_neut_hits loop
        }  // End of Step 1

        // Debug: record the post-Step-1 outcome for this test hit.
        if (!Step1_veto) {
            FillStepSurvivor("step1_after", *test);
        } else {
            FillStepVetoed("step1_after", *test);
        }

#pragma endregion

#pragma region /* Step 2: charged veto */

        // ---------------------------------------------------------------------
        // Step 2: charged veto
        // ---------------------------------------------------------------------
        // Veto this test-layer neutral if it matches ANY charged hit in the
        // reference set (FTOF through the test layer) under the same cuts.
        bool Step2_veto = false;

        if (Apply_Charged_Veto) {
            for (size_t j = 0; j < Ch_hits.size(); j++) {
                ChargedHit* ref = Ch_hits[j];

                // Debug: fill pair-comparison distributions for Step 2 (neutral test vs charged reference).
                FillStepPair_NeutCh("step2_before", *test, *ref);

                const int Charged_dSec = nfdveto::SectorDistance(Test_Layer_sector, ref->sector);
                const double Charged_opening_ang = ref->HitPath.Angle(Test_vector_HitPath) * TMath::RadToDeg();
                const double Charged_dT = TMath::Abs(Test_Layer_HitTime - ref->HitTime);
                const bool Step2_match = (Charged_dSec == dSector_cut) && (Charged_opening_ang < Opening_ang_cut_deg) && (Charged_dT < dHitTime_cut);

                if (Step2_match) {
                    Bad_neutral_hit = true;
                    Step2_veto = true;
                }

                if (Step2_match) {
                    if (printOutNeutralHit) {
                        test->DebugPrintOut({{"DeltaSector (step 2)", test->ToStr(Charged_dSec, "")},
                                             {"opening_ang (step 2)", test->ToStr(Charged_opening_ang, "[deg]")},
                                             {"DeltaTime (step 2)", test->ToStr(Charged_dT, "[ns]")}},
                                            "\033[31m");
                    }
                    if (printOutChargedHit) { ref->PrintOut(); }
                } else {
                    if (printOutNeutralHit) {
                        test->DebugPrintOut({{"DeltaSector (step 2)", test->ToStr(Charged_dSec, "")},
                                             {"opening_ang (step 2)", test->ToStr(Charged_opening_ang, "[deg]")},
                                             {"DeltaTime (step 2)", test->ToStr(Charged_dT, "[ns]")}},
                                            "\033[32m");
                    }
                    if (printOutChargedHit) { ref->PrintOut(); }
                }
            }  // End of Ch_hits loop
        }  // End of Step 2

        // Debug: record the cumulative post-Step-2 outcome for this test hit.
        // At this point, `Bad_neutral_hit` means "the hit is not viable" (vetoed by Step 1 and/or Step 2).
        // We therefore fill survivors if and only if the hit remains viable after Step 2.
        if (!Step2_veto) {
            FillStepSurvivor("step2_after", *test);
        } else {
            FillStepVetoed("step2_after", *test);
        }

#pragma endregion

#pragma region /* Step 3: earlier-ECAL neutral veto + bookkeeping */

        // ---------------------------------------------------------------------
        // Step 3: earlier-ECAL neutral veto + bookkeeping
        // ---------------------------------------------------------------------
        // Enforce "earliest ECAL layer wins": veto this test-layer neutral if it
        // matches ANY earlier-layer ECAL neutral (Mid_neut_hits) under the same cuts.
        // Bookkeeping: increment num_of_ECAL_hits_ahead on the EARLIER-LAYER hit
        // (it has one more matching hit in a later ECAL layer).
        bool Step3_veto = false;

        if (Apply_ECAL_Earliest_Layer_Veto) {
            for (size_t j = 0; j < Mid_neut_hits.size(); j++) {
                // Mid_neut_hits contains ONLY ECAL neutrals in layers BEFORE the test layer (PCAL/ECIN as appropriate).
                // This step enforces "earliest ECAL layer wins": later-layer hits are vetoed if an earlier-layer match exists.
                NeutralHit* ref = Mid_neut_hits[j];

                // Debug: fill pair-comparison distributions for Step 3 (neutral test vs earlier-layer neutral reference).
                FillStepPair_NeutNeut("step3_before", *test, *ref);

                const int Neutral_dSec = nfdveto::SectorDistance(Test_Layer_sector, ref->sector);
                const double Neutral_opening_ang = ref->HitPath.Angle(Test_vector_HitPath) * TMath::RadToDeg();
                const double Neutral_dT = TMath::Abs(Test_Layer_HitTime - ref->HitTime);
                const bool Step3_match = (Neutral_dSec == dSector_cut) && (Neutral_opening_ang < Opening_ang_cut_deg) && (Neutral_dT < dHitTime_cut);

                if (Step3_match) {
                    // Veto the later-layer test hit (earlier-layer hit exists and matches).
                    // Bookkeeping: increment on the EARLIER-LAYER hit:
                    //   Mid hit sees one more matching hit in a later ECAL layer ("ahead" of it).
                    // Because we are using pointers into the same nHit storage, this counter persists across passes.
                    Bad_neutral_hit = true;
                    Step3_veto = true;
                    ref->num_of_ECAL_hits_ahead++;
                }

                if (Step3_match) {
                    if (printOutNeutralHit) {
                        test->DebugPrintOut({{"DeltaSector (step 3)", test->ToStr(Neutral_dSec, "")},
                                             {"opening_ang (step 3)", test->ToStr(Neutral_opening_ang, "[deg]")},
                                             {"DeltaTime (step 3)", test->ToStr(Neutral_dT, "[ns]")}},
                                            "\033[31m");
                        ref->PrintOut();
                    }
                } else {
                    if (printOutNeutralHit) {
                        test->DebugPrintOut({{"DeltaSector (step 3)", test->ToStr(Neutral_dSec, "")},
                                             {"opening_ang (step 3)", test->ToStr(Neutral_opening_ang, "[deg]")},
                                             {"DeltaTime (step 3)", test->ToStr(Neutral_dT, "[ns]")}},
                                            "\033[32m");
                        ref->PrintOut();
                    }
                }
            }  // End of Mid_neut_hits loop
        }  // End of Step 3

        // Debug: record the cumulative post-Step-3 outcome for this test hit.
        // After Step 3, `Bad_neutral_hit` encodes whether the hit is still viable after Steps 1–3.
        if (!Step3_veto) {
            FillStepSurvivor("step3_after", *test);
        } else {
            FillStepVetoed("step3_after", *test);
        }

#pragma endregion

#pragma region /* Step 4: same-layer clustering */

        // ---------------------------------------------------------------------
        // Step 4: same-layer clustering (mutation/deduplication step)
        //
        // Step 4 is NOT a veto test; it mutates state (sets `blocking` on neighbors and can move the seed via averaging).
        // Therefore it must only run when:
        //   Apply_Same_Layer_Clustering && !Bad_neutral_hit && !test->blocking
        // ---------------------------------------------------------------------
        if (Apply_Same_Layer_Clustering
            // Guard 1: the seed hit must still be a viable neutral candidate.
            // Steps 1–3 answer the question: "should this hit be kept at all?"
            // If Bad_neutral_hit == true, this hit is already vetoed and will never survive,
            // so it must NOT be allowed to drive same-layer clustering.
            // Allowing a vetoed hit to cluster would let it mark good hits as `blocking`,
            // poisoning the cluster for reasons unrelated to the physics veto logic.
            && !Bad_neutral_hit

            // Guard 2: the seed hit must not already be marked as a duplicate.
            // `blocking == true` means this hit has already lost the representative
            // role to an earlier seed in the same layer.
            // A blocked hit must never become a cluster leader or mutate other hits,
            // otherwise vector order would allow duplicate hits to re-cluster and
            // produce non-deterministic or order-dependent results.
            && !test->blocking) {
            // Cluster accumulators (seeded by the current hit):
            double xAvg = test->HitPath.X();
            double yAvg = test->HitPath.Y();
            double zAvg = test->HitPath.Z();
            double Num_of_near_neighbors = 1;

            for (size_t j = i + 1; j < Test_Layer_neut_hits.size(); j++) {
                NeutralHit* ref = Test_Layer_neut_hits[j];

                // Debug: fill pair-comparison distributions for Step 4 (same-layer neutral clustering comparison).
                FillStepPair_NeutNeut("step4_before", *test, *ref);

                const int Neutral_dSec = nfdveto::SectorDistance(Test_Layer_sector, ref->sector);
                const double Neutral_opening_ang = ref->HitPath.Angle(Test_vector_HitPath) * TMath::RadToDeg();
                const double Neutral_dT = TMath::Abs(Test_Layer_HitTime - ref->HitTime);
                // Only unblocked neighbors can be claimed as duplicates.
                const bool Step4_match = (Neutral_dSec == dSector_cut) && (Neutral_opening_ang < Opening_ang_cut_deg) && (Neutral_dT < dHitTime_cut) && (ref->blocking == false);

                if (Step4_match) {
                    Num_of_near_neighbors++;
                    xAvg += ref->HitPath.X();
                    yAvg += ref->HitPath.Y();
                    zAvg += ref->HitPath.Z();
                    ref->blocking = true;
                }

                if (Step4_match) {
                    if (printOutNeutralHit) {
                        test->DebugPrintOut({{"DeltaSector (step 4)", test->ToStr(Neutral_dSec, "")},
                                             {"opening_ang (step 4)", test->ToStr(Neutral_opening_ang, "[deg]")},
                                             {"DeltaTime (step 4)", test->ToStr(Neutral_dT, "[ns]")}},
                                            "\033[31m");
                        ref->PrintOut();
                    }
                } else {
                    if (printOutNeutralHit) {
                        test->DebugPrintOut({{"DeltaSector (step 4)", test->ToStr(Neutral_dSec, "")},
                                             {"opening_ang (step 4)", test->ToStr(Neutral_opening_ang, "[deg]")},
                                             {"DeltaTime (step 4)", test->ToStr(Neutral_dT, "[ns]")}},
                                            "\033[32m");
                        ref->PrintOut();
                    }
                }
            }

            // If cluster size > 1, average the seed hit path and UpdateHitPath().
            if (Num_of_near_neighbors > 1) {
                // Update the scalar components first, then call UpdateHitPath() to keep the cached TVector3 consistent.
                // TODO: Cluster timing policy (???): we average positions but keep the seed hit's HitTime.
                //       If timing matters for representative selection, consider defining how HitTime should be chosen for same-layer clusters.
                test->xHitPath = xAvg / Num_of_near_neighbors;
                test->yHitPath = yAvg / Num_of_near_neighbors;
                test->zHitPath = zAvg / Num_of_near_neighbors;
                test->UpdateHitPath();
            }
        }  // End of Step 4

        // Debug: record the post-Step-4 (clustering) outcome for this test hit.
        // Note: Step 4 may be skipped entirely for vetoed seeds, so only non-vetoed, non-blocked hits are survivors here.
        // TODO: note that this is does not fill what we think it does for step 4 (survived/vetoed). Find another way to plot this.
        if (!Bad_neutral_hit && !test->blocking) {
            FillStepSurvivor("step4_after", *test);
        } else if (Bad_neutral_hit) {
            FillStepVetoed("step4_after", *test);
        }

#pragma endregion

        if (!Apply_TopologyAwareMultiLayerPolicy) {
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
            if (!Bad_neutral_hit && !test->blocking /* && (test->num_of_ECAL_hits_ahead < 2) */) { Neut_hits_after_veto.push_back(*test); }
            // if (!Bad_neutral_hit && !test->blocking && (test->num_of_ECAL_hits_ahead < 2)) { Neut_hits_after_veto.push_back(*test); }
        } else {
            // -----------------------------------------------------------------
            // Option C: conditional handling of the indirect "3-layer" topology rejection
            // -----------------------------------------------------------------
            // Physics intent:
            //   - The bookkeeping cut `num_of_ECAL_hits_ahead < 2` acts like a topology-based rejection for cases
            //     where the earliest ECAL layer (PCAL) has matching hits in BOTH later layers.
            //   - This is effective against charged contamination, but can also reject real high-p neutrons that
            //     produce multi-layer ECAL responses.
            //   - With Option C enabled, we classify the earliest-layer (PCAL) candidate using TOF-derived beta.
            //     Photon-like (beta≈1) keeps the strict rejection; neutron-like relaxes it.
            bool PassesIndirectTopologyCut = true;

            if (test->layer == 1 && test->num_of_ECAL_hits_ahead >= 2) { PassesIndirectTopologyCut = !ShouldApplyIndirectMultiLayerRejection(*test); }

            if (!Bad_neutral_hit && !test->blocking && PassesIndirectTopologyCut) { Neut_hits_after_veto.push_back(*test); }
        }
    }  // End of Test_Layer_neut_hits loop
}  // End of AddNeutrons function

#pragma endregion

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

#pragma region /* neutSelection */

std::vector<NeutralHit> FDNeutralVeto::neutSelection(std::vector<NeutralHit> nHit, std::vector<ChargedHit> cHit) {
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

    // We start from ECOUT neutrals and proceed down to PCAL (ECOUT → ECIN → PCAL).

    // Stage: before all veto steps
    // We only fill pair hists inside AddNeutrons during comparisons.
    // Here we also tag a stage directory with survivor and vetoed hists if needed later.
    // For now, the general pre-veto hit hists are filled in GenerateHits.

    // Pass 1: ECOUT test
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

    // Pass 2: ECIN test
    // Charged hits:
    // vTempLayersCharged is now all the charged hits from the FTOF to the ECIN:
    vTempLayersCharged.clear();
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay0c.begin(), vLay0c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay1c.begin(), vLay1c.end());
    vTempLayersCharged.insert(vTempLayersCharged.end(), vLay2c.begin(), vLay2c.end());

    // Neutral hits:
    vTempLayersNeutral.clear();
    vTempLayersNeutral.insert(vTempLayersNeutral.end(), vLay1n.begin(), vLay1n.end());

    //   - Step 3 compares against earlier ECAL neutrals (PCAL) and vetoes ECIN when a match exists.
    //   - Also increments num_of_ECAL_hits_ahead on PCAL hits when they match ECIN.
    AddNeutrons(vLay0n,              // Neutral hits in the FTOF
                vTempLayersNeutral,  // Neutral hits in the layers before the test layer and not the FTOF layer (first layer)
                vTempLayersCharged,  // All the charged hits from the FTOF to the test layer included
                vLay2n,              // The test layer of neutron hits (here - ECIN)
                vNeut                // Neutral hits that passed the veto
    );

    // Pass 3: PCAL test
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

    // Stage: after all veto steps
    FillGeneralHits_PostVeto(vNeut);

    // Also fill survivor kinematics into "after_all_veto"
    // Debug: fill survivor kinematics after all veto steps.
    for (const auto& h : vNeut) { FillStepSurvivor("after_all_veto", h); }

    return vNeut;
}  // End of neutSelection function

#pragma endregion

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

#pragma region /* GetNewNeutronFDVetoHits */

std::vector<NeutralHit> FDNeutralVeto::GetNewNeutronFDVetoHits(std::vector<region_part_ptr>& allParticles, region_part_ptr& electron) {
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
