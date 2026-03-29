#pragma once

#include <cmath>
#include <string>
#include <vector>

// Include histogram context:
#include "HistContext.h"

// Include histogram libraries:
#include "NeutronSelectionAngular_histograms.h"
#include "NeutronSelectionBase_histograms.h"
#include "NeutronSelectionMisc_histograms.h"
#include "NeutronSelectionResolution_histograms.h"
#include "NeutronSelectionSameSector_histograms.h"

// Helper: clone the base context and override only the neutron-tag ("nFD", "LnFD", ...)
static inline HistContext WithNTag(const HistContext& base, const std::string& nTag) {
    HistContext out = base;
    out.nTag = nTag;
    return out;
}

struct NeutronSelection_histograms {
    NeutronSelectionBase_histograms NSB_nFD_histograms;
    NeutronSelectionBase_histograms NSB_LnFD_histograms;
    NeutronSelectionAngular_histograms NSA_nFD_histograms;
    NeutronSelectionAngular_histograms NSA_LnFD_histograms;
    NeutronSelectionSameSector_histograms NSSS_nFD_histograms;
    NeutronSelectionSameSector_histograms NSSS_LnFD_histograms;
    NeutronSelectionMisc_histograms MSM_nFD_histograms;
    NeutronSelectionMisc_histograms MSM_LnFD_histograms;
    NeutronSelectionResolution_histograms NSR_nFD_histograms;

    // =======================
    // Constructor
    // =======================
    NeutronSelection_histograms(const HistContext& ctx,  // for naming consistency; e.g. "ECALveto" or "matched"
                                std::vector<std::string>& HistSubjects, std::vector<std::string>& HistSubjects2, std::vector<bool>& FirstPrint, std::vector<TH1*>& HistoList,
                                double P_upperLim, double Ebeam, const std::vector<std::vector<double>>& dr_n_SliceLimits, DSCuts dTheta_cuts,
                                std::vector<TObject*>& HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n, const std::vector<std::vector<double>>& nFD_Res_SliceLimits,
                                std::vector<TObject*>& HistoList_nRes_BysliceOf_truth_P_nFD, std::vector<TObject*>& HistoList_nRes_BysliceOf_reco_P_nFD)
        : NSB_nFD_histograms(HistoList, P_upperLim, Ebeam, WithNTag(ctx, "nFD")),
          NSB_LnFD_histograms(HistoList, P_upperLim, Ebeam, WithNTag(ctx, "LnFD")),
          NSA_nFD_histograms(HistoList, dr_n_SliceLimits, HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n, Ebeam, WithNTag(ctx, "nFD")),
          NSA_LnFD_histograms(HistoList, dr_n_SliceLimits, HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n, Ebeam, WithNTag(ctx, "LnFD")),
          NSSS_nFD_histograms(HistoList, dTheta_cuts, Ebeam, WithNTag(ctx, "nFD")),
          NSSS_LnFD_histograms(HistoList, dTheta_cuts, Ebeam, WithNTag(ctx, "LnFD")),
          MSM_nFD_histograms(HistoList, Ebeam, WithNTag(ctx, "nFD")),
          MSM_LnFD_histograms(HistoList, Ebeam, WithNTag(ctx, "LnFD")),
          NSR_nFD_histograms(HistoList, nFD_Res_SliceLimits, HistoList_nRes_BysliceOf_truth_P_nFD, HistoList_nRes_BysliceOf_reco_P_nFD, Ebeam, WithNTag(ctx, "nFD")) {
        HistSubjects.push_back(ctx.selection);
        HistSubjects2.push_back(ctx.selectionDescription);
        FirstPrint.push_back(ctx.selectionFirstPrint);
    }

    void CompareHistograms(const HistContext& ctx, const std::string& saveDirectory, const std::string& saveDirectoryName = "Histogram_Comparisons") {
        HistContext nFDctxPrototype(ctx.selection, ctx.selectionDescription, "nFD");
        CompareHistogramsPrototype(nFDctxPrototype, saveDirectory, saveDirectoryName);

        HistContext LnFDctxPrototype(ctx.selection, ctx.selectionDescription, "LnFD");
        CompareHistogramsPrototype(LnFDctxPrototype, saveDirectory, saveDirectoryName);
    }

    void CompareHistogramsPrototype(const HistContext& ctx, const std::string& saveDirectory, const std::string& saveDirectoryName = "Histogram_Comparisons") {
        if (ctx.nTag == "nFD") {
            hf::CompareHistograms({NSB_nFD_histograms.h_reco_P_e_VS_P_nFD, NSB_nFD_histograms.h_Edep_ECAL_nFD_VS_beta_nFD, NSA_nFD_histograms.h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e},
                                  saveDirectory, saveDirectoryName, "Reco_nFD_" + ctx.selection + "_1e_cut");
        } else if (ctx.nTag == "LnFD") {
            hf::CompareHistograms({NSB_LnFD_histograms.h_reco_P_e_VS_P_nFD, NSB_LnFD_histograms.h_Edep_ECAL_nFD_VS_beta_nFD, NSA_LnFD_histograms.h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e},
                                  saveDirectory, saveDirectoryName, "Reco_nFD_" + ctx.selection + "_1e_cut");
        }

        if (ctx.nTag == "nFD") {
            hf::CompareHistograms({NSB_nFD_histograms.h_reco_nFD_multi_AllN, NSB_nFD_histograms.h_reco_nFD_multi_OnlyGood, NSB_nFD_histograms.h_reco_nFD_multi_OnlyBad}, saveDirectory,
                                  saveDirectoryName, "reco_nFD_multi_AllN_" + ctx.selection + "_1e_cut");
        }
    }
};
