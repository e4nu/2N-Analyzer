//
// Created by Alon Sportes on 16/02/2025.
//
// FDNeutralVetoDebug.h
// ----------------
// ROOT debug histogram instrumentation for the FD neutral veto.
//
// This class is intentionally NOT the veto itself.
// It only:
//   - owns/creates ROOT directories and histograms,
//   - caches histograms by stage/group/name,
//   - fills diagnostics for "pairs", "survivors", and "vetoed" per stage,
//   - fills general pre/post-veto kinematics.
//
// The veto physics and matching logic must live elsewhere (e.g. FDNeutralVeto).
//

#ifndef FD_NEUTRAL_VETO_DEBUG_H
#define FD_NEUTRAL_VETO_DEBUG_H

#include <TDirectory.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TVector3.h>

#include <cmath>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// Include libraries:
#include "../../namespaces/general_utilities/constants.h"
#include "nfdveto.h"

// Other includes:
#include "ChargedHit.h"
#include "NeutralHit.h"

class FDNeutralVetoDebug {
   public:
    // -------------------------------------------------------------------------
    // Debug histograms configuration
    // -------------------------------------------------------------------------
    struct DebugHists;

    FDNeutralVetoDebug();
    ~FDNeutralVetoDebug();
    void EnableDebugHists(const nfdveto::DebugHistConfig& cfg);
    void FinalizeDebugHists();
    void SetStartTime(double starttime);
    void FillGeneralHits_PreVeto(const std::vector<NeutralHit>& nHits, const std::vector<ChargedHit>& cHits);
    void FillGeneralHits_PostVeto(const std::vector<NeutralHit>& nSurvivors);
    void FillStepPair_NeutNeut(const std::string& stage, const NeutralHit& test, const NeutralHit& ref);
    void FillStepPair_NeutCh(const std::string& stage, const NeutralHit& test, const ChargedHit& ref);
    void FillStepSurvivor(const std::string& stage, const NeutralHit& h);
    void FillStepVetoed(const std::string& stage, const NeutralHit& h);

   private:
    double WrapPhiDeg(double dphi) const;

    std::tuple<double, double, double, TVector3> Calc_nFD_vars(const NeutralHit& h, double starttime) const { return nfdveto::Calc_nFD_vars(h, starttime); }

    nfdveto::DebugHistConfig m_dbgCfg{};

    DebugHists* m_dbg = nullptr;

    double m_starttime = 0.0;

    void BookIfNeeded();
};

#endif  // FD_NEUTRAL_VETO_DEBUG_H
