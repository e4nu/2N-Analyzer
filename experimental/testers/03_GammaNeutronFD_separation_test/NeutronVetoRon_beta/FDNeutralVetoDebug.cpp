//
// Created by Alon Sportes on 16/02/2025.
//
// Obtained from Ron Wagner on 06/11/2025.
// Refactored, extended, and documented by Alon Sportes.
//
// FDNeutralVetoDebug.cpp
// -----------------
// Implementation file for FDNeutralVetoDebug class.
//

#include "FDNeutralVetoDebug.h"

#include <TCanvas.h>
#include <TMath.h>

#include <cmath>  // std::isfinite
#include <map>

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
//   - Shared FD-neutral kinematics utilities (Calc_nFD_vars, SectorDistance, DebugHistConfig) live in `nfdveto`.
// =================================================================================================

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------

FDNeutralVetoDebug::FDNeutralVetoDebug() = default;

double FDNeutralVetoDebug::WrapPhiDeg(double dphi) const {
    while (dphi >= 180.0) dphi -= 360.0;
    while (dphi < -180.0) dphi += 360.0;
    return dphi;
}

// Reformatted step 5 logic and candidate classification REMOVED from debug-only implementation.

// -----------------------------------------------------------------------------
// Debug histogram storage
// -----------------------------------------------------------------------------

#pragma region /* Debug histogram storage */

struct FDNeutralVetoDebug::DebugHists {
    struct VarDef {
        std::string key;
        std::string title;
        int nb;
        double lo;
        double hi;
    };

    DebugHists() = default;

    // General hit vars for neutrals and charged
    std::vector<VarDef> hitVars = {
        {"p", "p [GeV]", 100, 0.0, 10.0},
        {"theta", "theta [deg]", 100, 0.0, 60.0},
        {"phi", "phi [deg]", 100, -180.0, 180.0},
        {"time", "time [ns]", 100, -50.0, 150.0},
        {"hittime", "HitTime [ns]", 100, -50.0, 150.0},
        {"vx", "Vx [cm]", 100, -20.0, 20.0},
        {"vy", "Vy [cm]", 100, -20.0, 20.0},
        {"vz", "Vz [cm]", 100, -40.0, 40.0},
    };

    // Step comparison vars (differences + opening angle + sector difference)
    std::vector<VarDef> stepVars = {
        {"dp", "p(test) - p(ref) [GeV]", 100, -5.0, 5.0},
        {"dtheta", "theta(test) - theta(ref) [deg]", 100, -60.0, 60.0},
        {"dphi", "phi(test) - phi(ref) [deg]", 100, -180.0, 180.0},
        {"dtime", "Time(test) - Time(ref) [ns]", 100, -50.0, 50.0},
        {"dhittime", "HitTime(test) - HitTime(ref) [ns]", 100, -50.0, 50.0},
        {"dvx", "Vx(test) - Vx(ref) [cm]", 100, -20.0, 20.0},
        {"dvy", "Vy(test) - Vy(ref) [cm]", 100, -20.0, 20.0},
        {"dvz", "Vz(test) - Vz(ref) [cm]", 100, -40.0, 40.0},
        {"openang", "opening angle [deg]", 100, 0.0, 60.0},
        {"dsec", "DeltaSector", 13, -0.5, 12.5},
    };

    // Base directory under which ALL FDNeutralVetoDebug debug histograms are created.
    // If the user does not supply a baseDir, we default to the current ROOT gDirectory.
    TDirectory* base = nullptr;

    // General hit histograms:
    // base/general/pre/neutrals/<var>, base/general/pre/charged/<var>, base/general/post/neutrals/<var>
    TH1D* n_p = nullptr;
    TH1D* n_theta = nullptr;
    TH1D* n_phi = nullptr;
    TH1D* n_time = nullptr;
    TH1D* n_hittime = nullptr;
    TH1D* n_vx = nullptr;
    TH1D* n_vy = nullptr;
    TH1D* n_vz = nullptr;

    TH1D* c_p = nullptr;
    TH1D* c_theta = nullptr;
    TH1D* c_phi = nullptr;
    TH1D* c_time = nullptr;
    TH1D* c_hittime = nullptr;
    TH1D* c_vx = nullptr;
    TH1D* c_vy = nullptr;
    TH1D* c_vz = nullptr;

    // Post veto neutrals
    TH1D* n_post_p = nullptr;
    TH1D* n_post_theta = nullptr;
    TH1D* n_post_phi = nullptr;
    TH1D* n_post_time = nullptr;
    TH1D* n_post_hittime = nullptr;
    TH1D* n_post_vx = nullptr;
    TH1D* n_post_vy = nullptr;
    TH1D* n_post_vz = nullptr;

    // Step based 1D and 2D maps keyed by "stage/name"
    std::map<std::string, TH1D*> step1d;
    std::map<std::string, TH2D*> step2d;
    // General 1D histogram cache (non-step)
    std::map<std::string, TH1D*> h1;

    // GetOrBookH1
    // Purpose:
    //   Lazy-book a general (non-step) 1D histogram in the given directory.
    //   This is used for the "general/pre" and "general/post" distributions.
    // Implementation notes:
    //   - We cache by full directory path + histogram name so repeated calls return the same TH1D.
    //   - Sumw2() is enabled for correct statistical uncertainties.
    TH1D* GetOrBookH1(TDirectory* dir, const std::string& name, const std::string& title, int nb, double lo, double hi) {
        if (!dir) return nullptr;
        const std::string key = std::string(dir->GetPath()) + "/" + name;
        auto it = h1.find(key);
        if (it != h1.end()) return it->second;

        dir->cd();
        TH1D* h = new TH1D(name.c_str(), title.c_str(), nb, lo, hi);
        h->Sumw2();
        h1[key] = h;
        return h;
    }

    TH1D* GetOrBookStep1D(TDirectory* dir, const std::string& name, const std::string& title, int nb, double lo, double hi) {
        if (!dir) return nullptr;
        const std::string key = std::string(dir->GetPath()) + "/" + name;
        auto it = step1d.find(key);
        if (it != step1d.end()) return it->second;

        dir->cd();
        TH1D* h = new TH1D(name.c_str(), title.c_str(), nb, lo, hi);
        h->Sumw2();
        step1d[key] = h;
        return h;
    }

    TH2D* GetOrBookStep2D(TDirectory* dir, const std::string& name, const std::string& title, int nbx, double lox, double hix, int nby, double loy, double hiy) {
        if (!dir) return nullptr;
        const std::string key = std::string(dir->GetPath()) + "/" + name;
        auto it = step2d.find(key);
        if (it != step2d.end()) return it->second;

        dir->cd();
        TH2D* h = new TH2D(name.c_str(), title.c_str(), nbx, lox, hix, nby, loy, hiy);
        h->Sumw2();
        step2d[key] = h;
        return h;
    }

    // MkdirPath
    // Purpose:
    //   Create (if needed) and return a nested ROOT directory structure given a relative path.
    // Notes:
    //   - ROOT directories behave like a filesystem tree.
    //   - This helper ensures histograms are grouped by purpose (general vs steps vs steps2d).
    TDirectory* MkdirPath(TDirectory* baseDir, const std::string& rel) {
        if (!baseDir) return nullptr;
        TDirectory* cur = baseDir;
        size_t start = 0;
        while (start < rel.size()) {
            size_t slash = rel.find('/', start);
            std::string token = (slash == std::string::npos) ? rel.substr(start) : rel.substr(start, slash - start);
            if (!token.empty()) {
                TDirectory* next = dynamic_cast<TDirectory*>(cur->Get(token.c_str()));
                if (!next) next = cur->mkdir(token.c_str());
                cur = next;
            }
            if (slash == std::string::npos) break;
            start = slash + 1;
        }
        return cur;
    }

    // GetStep1D (grouped)
    // Purpose:
    //   Return (and lazily book if needed) a step-level 1D histogram under:
    //     <base>/steps/<stage>/<group>/<name>
    // Notes:
    //   - We cache by full directory path + histogram name.
    //   - This MUST stay consistent with BookStepStage(), which books under:
    //       steps/<stage>/{pairs, survivors, vetoed}
    TH1D* GetStep1D(const std::string& stage, const std::string& group, const std::string& name, const std::string& title, int nb, double lo, double hi) {
        // If no ROOT base directory is defined, we cannot attach histograms anywhere.
        if (!base) return nullptr;

        // Construct the relative directory path:
        //   steps/<stage>/<group>
        // This ensures histograms are grouped first by veto stage, then by category.
        const std::string rel = std::string("steps/") + stage + "/" + group;

        // Create the directory tree if it does not yet exist.
        // MkdirPath walks token-by-token and creates missing subdirectories.
        TDirectory* d = MkdirPath(base, rel);

        // Defensive check: if directory creation failed for any reason, do not dereference.
        // Returning nullptr keeps the debug layer non-invasive (it must never crash physics code).
        if (!d) return nullptr;

        // Build a unique cache key using the full ROOT directory path plus histogram name.
        // This guarantees uniqueness even if the same histogram name appears in different stages.
        const std::string key = d->GetPath() + std::string("/") + name;

        // Check whether we already booked this histogram.
        auto it = step1d.find(key);
        if (it != step1d.end()) {
            // Histogram already exists → return existing instance.
            return it->second;
        }

        // Change current ROOT directory so the histogram is owned by that directory.
        d->cd();

        // Create the 1D histogram with the requested binning.
        TH1D* h = new TH1D(name.c_str(), title.c_str(), nb, lo, hi);

        // Enable storage of sum of weights squared (required for correct statistical errors).
        h->Sumw2();

        // Store in internal cache so future calls reuse the same object.
        step1d[key] = h;

        // Return the newly created histogram.
        return h;
    }

    // GetStep1D (default group)
    // Purpose:
    //   Backwards-compatible wrapper that defaults the group to "pairs".
    TH1D* GetStep1D(const std::string& stage, const std::string& name, const std::string& title, int nb, double lo, double hi) { return GetStep1D(stage, "pairs", name, title, nb, lo, hi); }

    // GetStep2D (grouped)
    // Purpose:
    //   Return (and lazily book if needed) a step-level 2D histogram under:
    //     <base>/steps/<stage>/<group>/<name>
    // Notes:
    //   - We cache by full directory path + histogram name.
    //   - This MUST stay consistent with BookStepStage(), which books 2D under:
    //       steps/<stage>/pairs2d
    TH2D* GetStep2D(const std::string& stage, const std::string& group, const std::string& name, const std::string& title, int nbx, double lox, double hix, int nby, double loy, double hiy) {
        // If no base directory exists, booking is impossible.
        if (!base) return nullptr;

        // Directory structure:
        //   steps/<stage>/<group>
        const std::string rel = std::string("steps/") + stage + "/" + group;

        // Ensure directory exists.
        TDirectory* d = MkdirPath(base, rel);

        // Defensive check: if directory creation failed for any reason, do not dereference.
        // Returning nullptr keeps the debug layer non-invasive (it must never crash physics code).
        if (!d) return nullptr;

        // Unique key for caching (full directory path + histogram name).
        const std::string key = d->GetPath() + std::string("/") + name;

        // Return cached histogram if already booked.
        auto it = step2d.find(key);
        if (it != step2d.end()) { return it->second; }

        // Switch ROOT context to correct directory.
        d->cd();

        // Create 2D histogram with requested X and Y binning.
        TH2D* h = new TH2D(name.c_str(), title.c_str(), nbx, lox, hix, nby, loy, hiy);

        // Enable proper error handling.
        h->Sumw2();

        // Cache pointer.
        step2d[key] = h;

        // Return newly created histogram.
        return h;
    }

    // GetStep2D (default group)
    // Purpose:
    //   Backwards-compatible wrapper that defaults the group to "pairs2d".
    TH2D* GetStep2D(const std::string& stage, const std::string& name, const std::string& title, int nbx, double lox, double hix, int nby, double loy, double hiy) {
        return GetStep2D(stage, "pairs2d", name, title, nbx, lox, hix, nby, loy, hiy);
    }

    // BookGeneral
    // Purpose:
    //   Pre-book the "general" kinematic histograms for:
    //     - neutrals before veto
    //     - charged before veto
    //     - neutrals after veto
    // Notes:
    //   - Booking is optional because we also lazy-book, but this establishes a stable directory layout.
    void BookGeneral() {
        // general/pre/neutrals and general/pre/charged, general/post/neutrals
        TDirectory* dPreN = MkdirPath(base, "general/pre/neutrals");
        TDirectory* dPreC = MkdirPath(base, "general/pre/charged");
        TDirectory* dPostN = MkdirPath(base, "general/post/neutrals");

        for (const auto& v : hitVars) {
            GetOrBookH1(dPreN, ("n_" + v.key).c_str(), ("Neutral " + v.title).c_str(), v.nb, v.lo, v.hi);
            GetOrBookH1(dPreC, ("c_" + v.key).c_str(), ("Charged " + v.title).c_str(), v.nb, v.lo, v.hi);
            GetOrBookH1(dPostN, ("n_" + v.key).c_str(), ("Neutral " + v.title).c_str(), v.nb, v.lo, v.hi);
        }
    }

    // BookStepStage
    // Purpose:
    //   Pre-book all step-level histograms for a named stage such as:
    //     "step1_before", "step1_after", ..., "after_all_veto".
    // What is booked:
    //   - Pair-comparison 1D distributions (dp, dtheta, dphi, dtime, dhittime, dvx, dvy, dvz, openang, dsec).
    //   - Survivor and vetoed kinematics for the tested (neutral) hit.
    //   - Optionally, all unique 2D combinations of the pair variables.
    // Notes:
    //   - In production, you may prefer lazy-booking only; the pre-booking is convenient for completeness.
    void BookStepStage(const std::string& stage, bool book2D) {
        TDirectory* dPairs = MkdirPath(base, "steps/" + stage + "/pairs");
        TDirectory* dPairs2D = MkdirPath(base, "steps/" + stage + "/pairs2d");
        TDirectory* dSurv = MkdirPath(base, "steps/" + stage + "/survivors");
        TDirectory* dVeto = MkdirPath(base, "steps/" + stage + "/vetoed");

        for (const auto& v : stepVars) { GetOrBookStep1D(dPairs, v.key.c_str(), v.title.c_str(), v.nb, v.lo, v.hi); }

        for (const auto& v : hitVars) {
            GetOrBookStep1D(dSurv, ("surv_" + v.key).c_str(), ("Survivor " + v.title).c_str(), v.nb, v.lo, v.hi);
            GetOrBookStep1D(dVeto, ("veto_" + v.key).c_str(), ("Vetoed " + v.title).c_str(), v.nb, v.lo, v.hi);
        }

        if (book2D) {
            for (size_t i = 0; i < stepVars.size(); i++) {
                for (size_t j = i + 1; j < stepVars.size(); j++) {
                    const auto& xi = stepVars[i];
                    const auto& yj = stepVars[j];
                    const std::string name = xi.key + "_vs_" + yj.key;
                    const std::string title = xi.title + " vs " + yj.title;
                    GetOrBookStep2D(dPairs2D, name, title, xi.nb, xi.lo, xi.hi, yj.nb, yj.lo, yj.hi);
                }
            }
        }
    }

    void WriteAll(bool writeCanvases) {
        if (!base) return;
        base->cd();

        if (!writeCanvases) return;

        // Optional: create canvases for every histogram (can be large output).
        // Histograms remain owned by ROOT directories.
        for (const auto& kv : h1) {
            if (!kv.second) continue;
            TCanvas* c = new TCanvas();
            kv.second->Draw();
            c->Write((std::string("cv_") + kv.second->GetName()).c_str());
        }
        for (const auto& kv : step1d) {
            if (!kv.second) continue;
            TCanvas* c = new TCanvas();
            kv.second->Draw();
            c->Write((std::string("cv_") + kv.second->GetName()).c_str());
        }
        for (const auto& kv : step2d) {
            if (!kv.second) continue;
            TCanvas* c = new TCanvas();
            kv.second->Draw("COLZ");
            c->Write((std::string("cv_") + kv.second->GetName()).c_str());
        }
    }
};

#pragma endregion

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------

FDNeutralVetoDebug::~FDNeutralVetoDebug() {
    if (m_dbg) {
        delete m_dbg;
        m_dbg = nullptr;
    }
}

// -----------------------------------------------------------------------------
// Debug histogram public controls
// -----------------------------------------------------------------------------
// EnableDebugHists(cfg):
//   - turns instrumentation on/off
//   - stores configuration (base directory, 2D booking policy, optional canvas writing)
// FinalizeDebugHists():
//   - writes histograms (and optionally canvases) into the ROOT output
// SetStartTime(t0):
//   - sets per-event start time used by Calc_nFD_vars(...) for TOF-based neutron kinematics
// -----------------------------------------------------------------------------

#pragma region /* Debug histogram public controls */

void FDNeutralVetoDebug::EnableDebugHists(const nfdveto::DebugHistConfig& cfg) {
    // Store the new debug configuration (enable flag, directory, 2D policy, etc.).
    m_dbgCfg = cfg;

    // If the user disabled debug, release any existing histogram manager and stop.
    if (!m_dbgCfg.enable) {
        if (m_dbg) {
            delete m_dbg;
            m_dbg = nullptr;
        }
        return;
    }

    // If enabled, ensure the histogram manager exists and directories/hists are booked.
    BookIfNeeded();
}

void FDNeutralVetoDebug::FinalizeDebugHists() {
    // If debug is not enabled or we never booked, nothing to do.
    if (!m_dbgCfg.enable || !m_dbg) return;

    // Write all histograms into the configured ROOT directory.
    // Optionally also write canvases (can be very large output).
    m_dbg->WriteAll(m_dbgCfg.writeCanvases);

    // Do not delete m_dbg here by default.
    // ROOT owns histogram objects under directories, and the caller may want multiple writes.
}

#pragma endregion

// -----------------------------------------------------------------------------
// SetStartTime
// Purpose:
//   Provide the event start time used for TOF-based neutron kinematics.
// Notes:
//   Must be called once per event before any TOF-based histogram filling.
// -----------------------------------------------------------------------------

#pragma region /* SetStartTime */

void FDNeutralVetoDebug::SetStartTime(double starttime) {
    // This value must be set once per event before filling any TOF-derived neutral kinematics.
    // Calc_nFD_vars(h, m_starttime) uses:
    //   ToF = HitTime - starttime
    // and then computes beta, gamma, and neutron momentum from the flight path length.
    m_starttime = starttime;
}

#pragma endregion

// -----------------------------------------------------------------------------
// Debug histogram helpers
// -----------------------------------------------------------------------------

#pragma region /* Debug histogram helpers */

// BookIfNeeded
// Purpose:
//   One-time creation of the DebugHists manager and the ROOT histogram directory layout.
// Notes:
//   - This is called from multiple fill sites, but only the first call will allocate/define histograms.
//   - Histograms are owned by ROOT directories, not by the DebugHists object.
void FDNeutralVetoDebug::BookIfNeeded() {
    if (!m_dbgCfg.enable) return;
    if (m_dbg) return;

    m_dbg = new DebugHists();

    TDirectory* base = m_dbgCfg.baseDir ? m_dbgCfg.baseDir : gDirectory;
    if (!base) {
        // If no ROOT directory context exists, we cannot book histograms.
        // Clean up so a later call can retry when a directory becomes available.
        delete m_dbg;
        m_dbg = nullptr;
        return;
    }

    m_dbg->base = m_dbg->MkdirPath(base, "FDNeutralVetoDebug");
    if (!m_dbg->base) return;

    // General pre veto directory
    TDirectory* dGen = m_dbg->MkdirPath(m_dbg->base, "general");
    dGen->cd();

    m_dbg->n_p = new TH1D("n_p", "Neutral p(TOF) [GeV]", 100, 0.0, 10.0);
    m_dbg->n_theta = new TH1D("n_theta", "Neutral theta(TOF) [deg]", 100, 0.0, 60.0);
    m_dbg->n_phi = new TH1D("n_phi", "Neutral phi(TOF) [deg]", 100, -180.0, 180.0);
    m_dbg->n_time = new TH1D("n_time", "Neutral Time [ns]", 100, -50.0, 150.0);
    m_dbg->n_hittime = new TH1D("n_hittime", "Neutral HitTime [ns]", 100, -50.0, 150.0);
    m_dbg->n_vx = new TH1D("n_vx", "Neutral Vx [cm]", 100, -20.0, 20.0);
    m_dbg->n_vy = new TH1D("n_vy", "Neutral Vy [cm]", 100, -20.0, 20.0);
    m_dbg->n_vz = new TH1D("n_vz", "Neutral Vz [cm]", 100, -40.0, 40.0);

    m_dbg->c_p = new TH1D("c_p", "Charged p [GeV]", 100, 0.0, 10.0);
    m_dbg->c_theta = new TH1D("c_theta", "Charged theta [deg]", 100, 0.0, 60.0);
    m_dbg->c_phi = new TH1D("c_phi", "Charged phi [deg]", 100, -180.0, 180.0);
    m_dbg->c_time = new TH1D("c_time", "Charged Time [ns]", 100, -50.0, 150.0);
    m_dbg->c_hittime = new TH1D("c_hittime", "Charged HitTime [ns]", 50, -50.0, 150.0);
    m_dbg->c_vx = new TH1D("c_vx", "Charged Vx [cm]", 100, -20.0, 20.0);
    m_dbg->c_vy = new TH1D("c_vy", "Charged Vy [cm]", 100, -20.0, 20.0);
    m_dbg->c_vz = new TH1D("c_vz", "Charged Vz [cm]", 100, -40.0, 40.0);

    // Post veto neutrals
    m_dbg->n_post_p = new TH1D("n_post_p", "Neutral post veto p(TOF) [GeV]", 100, 0.0, 10.0);
    m_dbg->n_post_theta = new TH1D("n_post_theta", "Neutral post veto theta(TOF) [deg]", 100, 0.0, 60.0);
    m_dbg->n_post_phi = new TH1D("n_post_phi", "Neutral post veto phi(TOF) [deg]", 100, -180.0, 180.0);
    m_dbg->n_post_time = new TH1D("n_post_time", "Neutral post veto Time [ns]", 100, -50.0, 150.0);
    m_dbg->n_post_hittime = new TH1D("n_post_hittime", "Neutral post veto HitTime [ns]", 100, -50.0, 150.0);
    m_dbg->n_post_vx = new TH1D("n_post_vx", "Neutral post veto Vx [cm]", 100, -20.0, 20.0);
    m_dbg->n_post_vy = new TH1D("n_post_vy", "Neutral post veto Vy [cm]", 100, -20.0, 20.0);
    m_dbg->n_post_vz = new TH1D("n_post_vz", "Neutral post veto Vz [cm]", 100, -40.0, 40.0);

    // Sumw2
    for (TH1D* h : {m_dbg->n_p,      m_dbg->n_theta,      m_dbg->n_phi,      m_dbg->n_time,      m_dbg->n_hittime,      m_dbg->n_vx,      m_dbg->n_vy,      m_dbg->n_vz,
                    m_dbg->c_p,      m_dbg->c_theta,      m_dbg->c_phi,      m_dbg->c_time,      m_dbg->c_hittime,      m_dbg->c_vx,      m_dbg->c_vy,      m_dbg->c_vz,
                    m_dbg->n_post_p, m_dbg->n_post_theta, m_dbg->n_post_phi, m_dbg->n_post_time, m_dbg->n_post_hittime, m_dbg->n_post_vx, m_dbg->n_post_vy, m_dbg->n_post_vz}) {
        if (h) h->Sumw2();
    }
}

void FDNeutralVetoDebug::FillGeneralHits_PreVeto(const std::vector<NeutralHit>& nHits, const std::vector<ChargedHit>& cHits) {
    if (!m_dbgCfg.enable) return;
    BookIfNeeded();
    if (!m_dbg) return;

    for (const auto& h : nHits) {
        auto [Calculated_ToF_nFD, Calculated_Beta_nFD, Calculated_Gamma_nFD, Calculated_reco_P_nFD_v] = this->Calc_nFD_vars(h, m_starttime);
        m_dbg->n_p->Fill(Calculated_reco_P_nFD_v.Mag());
        m_dbg->n_theta->Fill(Calculated_reco_P_nFD_v.Theta() * TMath::RadToDeg());
        m_dbg->n_phi->Fill(Calculated_reco_P_nFD_v.Phi() * TMath::RadToDeg());
        m_dbg->n_time->Fill(h.Time);
        m_dbg->n_hittime->Fill(h.HitTime);
        m_dbg->n_vx->Fill(h.Vtx.X());
        m_dbg->n_vy->Fill(h.Vtx.Y());
        m_dbg->n_vz->Fill(h.Vtx.Z());
    }

    for (const auto& h : cHits) {
        // Replace these if your ChargedHit uses different names
        m_dbg->c_p->Fill(h.P.Mag());
        m_dbg->c_theta->Fill(h.P.Theta() * TMath::RadToDeg());
        m_dbg->c_phi->Fill(h.P.Phi() * TMath::RadToDeg());
        m_dbg->c_time->Fill(h.Time);
        m_dbg->c_hittime->Fill(h.HitTime);
        m_dbg->c_vx->Fill(h.Vtx.X());
        m_dbg->c_vy->Fill(h.Vtx.Y());
        m_dbg->c_vz->Fill(h.Vtx.Z());
    }
}

void FDNeutralVetoDebug::FillGeneralHits_PostVeto(const std::vector<NeutralHit>& nSurvivors) {
    if (!m_dbgCfg.enable) return;
    BookIfNeeded();
    if (!m_dbg) return;

    for (const auto& h : nSurvivors) {
        auto [Calculated_ToF_nFD, Calculated_Beta_nFD, Calculated_Gamma_nFD, Calculated_reco_P_nFD_v] = this->Calc_nFD_vars(h, m_starttime);
        m_dbg->n_post_p->Fill(Calculated_reco_P_nFD_v.Mag());
        m_dbg->n_post_theta->Fill(Calculated_reco_P_nFD_v.Theta() * TMath::RadToDeg());
        m_dbg->n_post_phi->Fill(Calculated_reco_P_nFD_v.Phi() * TMath::RadToDeg());
        m_dbg->n_post_time->Fill(h.Time);
        m_dbg->n_post_hittime->Fill(h.HitTime);
        m_dbg->n_post_vx->Fill(h.Vtx.X());
        m_dbg->n_post_vy->Fill(h.Vtx.Y());
        m_dbg->n_post_vz->Fill(h.Vtx.Z());
    }
}

void FDNeutralVetoDebug::FillStepPair_NeutNeut(const std::string& stage, const NeutralHit& test, const NeutralHit& ref) {
    // Skip immediately if debug instrumentation is disabled.
    if (!m_dbgCfg.enable) return;

    // Ensure histogram manager exists (lazy allocation).
    BookIfNeeded();

    // If booking failed (e.g. no baseDir), exit safely.
    if (!m_dbg) return;

    // Compute TOF-based kinematics for both test and reference neutral hits.
    // Each call returns:
    //   (ToF, beta, gamma, reconstructed 3-momentum vector).
    auto [Calculated_ToF_nFD_t, Calculated_Beta_nFD_t, Calculated_Gamma_nFD_t, Calculated_reco_P_nFD_v_t] = this->Calc_nFD_vars(test, m_starttime);
    auto [Calculated_ToF_nFD_r, Calculated_Beta_nFD_r, Calculated_Gamma_nFD_r, Calculated_reco_P_nFD_v_r] = this->Calc_nFD_vars(ref, m_starttime);

    // Momentum magnitude difference.
    const double dp = Calculated_reco_P_nFD_v_t.Mag() - Calculated_reco_P_nFD_v_r.Mag();

    // Polar angle difference (converted to degrees).
    const double dtheta = (Calculated_reco_P_nFD_v_t.Theta() - Calculated_reco_P_nFD_v_r.Theta()) * TMath::RadToDeg();

    // Azimuthal angle difference wrapped into [-180°, 180°].
    const double dphi = WrapPhiDeg((Calculated_reco_P_nFD_v_t.Phi() - Calculated_reco_P_nFD_v_r.Phi()) * TMath::RadToDeg());

    // Raw timing differences.
    const double dtime = test.Time - ref.Time;
    const double dhittime = test.HitTime - ref.HitTime;

    // Vertex coordinate differences.
    const double dvx = test.Vtx.X() - ref.Vtx.X();
    const double dvy = test.Vtx.Y() - ref.Vtx.Y();
    const double dvz = test.Vtx.Z() - ref.Vtx.Z();

    // Opening angle between hit-path vectors (degrees).
    const double openang = ref.HitPath.Angle(test.HitPath) * TMath::RadToDeg();

    // Sector difference using wrap-aware metric.
    const double dsec = static_cast<double>(nfdveto::SectorDistance(test.sector, ref.sector));

    // Fill 1D diagnostic histograms under:
    //   steps/<stage>/pairs/
    m_dbg->GetStep1D(stage, "pairs", "dp", "Momentum difference [GeV]", 100, -5.0, 5.0)->Fill(dp);
    m_dbg->GetStep1D(stage, "pairs", "dtheta", "Theta difference [deg]", 100, -60.0, 60.0)->Fill(dtheta);
    m_dbg->GetStep1D(stage, "pairs", "dphi", "Phi difference [deg]", 100, -180.0, 180.0)->Fill(dphi);
    m_dbg->GetStep1D(stage, "pairs", "dtime", "Time difference [ns]", 100, -50.0, 50.0)->Fill(dtime);
    m_dbg->GetStep1D(stage, "pairs", "dhittime", "HitTime difference [ns]", 100, -50.0, 50.0)->Fill(dhittime);
    m_dbg->GetStep1D(stage, "pairs", "dvx", "Vx difference [cm]", 100, -20.0, 20.0)->Fill(dvx);
    m_dbg->GetStep1D(stage, "pairs", "dvy", "Vy difference [cm]", 100, -20.0, 20.0)->Fill(dvy);
    m_dbg->GetStep1D(stage, "pairs", "dvz", "Vz difference [cm]", 100, -40.0, 40.0)->Fill(dvz);
    m_dbg->GetStep1D(stage, "pairs", "openang", "Opening angle [deg]", 100, 0.0, 60.0)->Fill(openang);
    m_dbg->GetStep1D(stage, "pairs", "dsec", "Sector difference", 13, -0.5, 12.5)->Fill(dsec);
}

void FDNeutralVetoDebug::FillStepPair_NeutCh(const std::string& stage, const NeutralHit& test, const ChargedHit& ref) {
    // If debug is disabled, skip.
    if (!m_dbgCfg.enable) return;

    // Ensure histogram manager is ready.
    BookIfNeeded();
    if (!m_dbg) return;

    // Compute TOF-based kinematics for the neutral hit.
    // Charged hit momentum is taken directly from tracking.
    auto [Calculated_ToF_nFD_t, Calculated_Beta_nFD_t, Calculated_Gamma_nFD_t, Calculated_reco_P_nFD_v_t] = this->Calc_nFD_vars(test, m_starttime);
    const TVector3& Calculated_reco_P_nFD_v_r = ref.P;

    // Calculate kinematic and geometric differences.
    const double dp = Calculated_reco_P_nFD_v_t.Mag() - Calculated_reco_P_nFD_v_r.Mag();
    const double dtheta = (Calculated_reco_P_nFD_v_t.Theta() - Calculated_reco_P_nFD_v_r.Theta()) * TMath::RadToDeg();
    const double dphi = WrapPhiDeg((Calculated_reco_P_nFD_v_t.Phi() - Calculated_reco_P_nFD_v_r.Phi()) * TMath::RadToDeg());
    const double dtime = test.Time - ref.Time;
    const double dhittime = test.HitTime - ref.HitTime;
    const double dvx = test.Vtx.X() - ref.Vtx.X();
    const double dvy = test.Vtx.Y() - ref.Vtx.Y();
    const double dvz = test.Vtx.Z() - ref.Vtx.Z();
    const double openang = ref.HitPath.Angle(test.HitPath) * TMath::RadToDeg();
    const double dsec = static_cast<double>(nfdveto::SectorDistance(test.sector, ref.sector));

    // Fill 1D diagnostic histograms under:
    //   steps/<stage>/pairs/
    m_dbg->GetStep1D(stage, "pairs", "dp", "Momentum difference [GeV]", 100, -5.0, 5.0)->Fill(dp);
    m_dbg->GetStep1D(stage, "pairs", "dtheta", "Theta difference [deg]", 100, -60.0, 60.0)->Fill(dtheta);
    m_dbg->GetStep1D(stage, "pairs", "dphi", "Phi difference [deg]", 100, -180.0, 180.0)->Fill(dphi);
    m_dbg->GetStep1D(stage, "pairs", "dtime", "Time difference [ns]", 100, -50.0, 50.0)->Fill(dtime);
    m_dbg->GetStep1D(stage, "pairs", "dhittime", "HitTime difference [ns]", 50, -50.0, 50.0)->Fill(dhittime);
    m_dbg->GetStep1D(stage, "pairs", "dvx", "Vx difference [cm]", 100, -20.0, 20.0)->Fill(dvx);
    m_dbg->GetStep1D(stage, "pairs", "dvy", "Vy difference [cm]", 100, -20.0, 20.0)->Fill(dvy);
    m_dbg->GetStep1D(stage, "pairs", "dvz", "Vz difference [cm]", 100, -40.0, 40.0)->Fill(dvz);
    m_dbg->GetStep1D(stage, "pairs", "openang", "Opening angle [deg]", 100, 0.0, 60.0)->Fill(openang);
    m_dbg->GetStep1D(stage, "pairs", "dsec", "Sector difference", 13, -0.5, 12.5)->Fill(dsec);
}

void FDNeutralVetoDebug::FillStepSurvivor(const std::string& stage, const NeutralHit& h) {
    // Guard: skip if debugging is disabled.
    if (!m_dbgCfg.enable) return;

    // Ensure histogram booking is complete.
    BookIfNeeded();
    if (!m_dbg) return;

    // Compute TOF-based kinematics for this neutral hit.
    auto [ToF, Beta, Gamma, P] = this->Calc_nFD_vars(h, m_starttime);

    // Fill survivor histograms for all relevant variables under:
    //   steps/<stage>/survivors/
    m_dbg->GetStep1D(stage, "survivors", "surv_p", "Survivor p(TOF) [GeV]", 100, 0.0, 10.0)->Fill(P.Mag());
    m_dbg->GetStep1D(stage, "survivors", "surv_theta", "Survivor theta(TOF) [deg]", 100, 0.0, 60.0)->Fill(P.Theta() * TMath::RadToDeg());
    m_dbg->GetStep1D(stage, "survivors", "surv_phi", "Survivor phi(TOF) [deg]", 100, -180.0, 180.0)->Fill(P.Phi() * TMath::RadToDeg());
    m_dbg->GetStep1D(stage, "survivors", "surv_time", "Survivor Time [ns]", 100, -50.0, 150.0)->Fill(h.Time);
    m_dbg->GetStep1D(stage, "survivors", "surv_hittime", "Survivor HitTime [ns]", 100, -50.0, 150.0)->Fill(h.HitTime);
    m_dbg->GetStep1D(stage, "survivors", "surv_vx", "Survivor Vx [cm]", 100, -20.0, 20.0)->Fill(h.Vtx.X());
    m_dbg->GetStep1D(stage, "survivors", "surv_vy", "Survivor Vy [cm]", 100, -20.0, 20.0)->Fill(h.Vtx.Y());
    m_dbg->GetStep1D(stage, "survivors", "surv_vz", "Survivor Vz [cm]", 100, -40.0, 40.0)->Fill(h.Vtx.Z());
}

void FDNeutralVetoDebug::FillStepVetoed(const std::string& stage, const NeutralHit& h) {
    // Guard: skip if debugging is disabled.
    if (!m_dbgCfg.enable) return;

    // Ensure histogram booking is complete.
    BookIfNeeded();
    if (!m_dbg) return;

    // Compute TOF-based kinematics for this neutral hit.
    auto [ToF, Beta, Gamma, P] = this->Calc_nFD_vars(h, m_starttime);

    // Fill vetoed histograms for all relevant variables under:
    //   steps/<stage>/vetoed/
    m_dbg->GetStep1D(stage, "vetoed", "veto_p", "Vetoed p(TOF) [GeV]", 100, 0.0, 10.0)->Fill(P.Mag());
    m_dbg->GetStep1D(stage, "vetoed", "veto_theta", "Vetoed theta(TOF) [deg]", 100, 0.0, 60.0)->Fill(P.Theta() * TMath::RadToDeg());
    m_dbg->GetStep1D(stage, "vetoed", "veto_phi", "Vetoed phi(TOF) [deg]", 100, -180.0, 180.0)->Fill(P.Phi() * TMath::RadToDeg());
    m_dbg->GetStep1D(stage, "vetoed", "veto_time", "Vetoed Time [ns]", 100, -50.0, 150.0)->Fill(h.Time);
    m_dbg->GetStep1D(stage, "vetoed", "veto_hittime", "Vetoed HitTime [ns]", 100, -50.0, 150.0)->Fill(h.HitTime);
    m_dbg->GetStep1D(stage, "vetoed", "veto_vx", "Vetoed Vx [cm]", 100, -20.0, 20.0)->Fill(h.Vtx.X());
    m_dbg->GetStep1D(stage, "vetoed", "veto_vy", "Vetoed Vy [cm]", 100, -20.0, 20.0)->Fill(h.Vtx.Y());
    m_dbg->GetStep1D(stage, "vetoed", "veto_vz", "Vetoed Vz [cm]", 100, -40.0, 40.0)->Fill(h.Vtx.Z());
}

#pragma endregion