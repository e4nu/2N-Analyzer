#pragma once

// Put this somewhere shared (e.g. header or top of your .cpp)
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"

// Include libraries:
#include "../../../../framework/classes/NeutronVetoRon/NeutralHit.h"
#include "../../../../framework/classes/hPlots/hsPlots.cpp"
#include "../../ECAL_vetoes.cpp"
#include "../../variables/NeutronSelection_variables.h"
#include "HistContext.h"

// =====================================================================================================
// Resolution histogram group (TL vs reco neutron momentum resolution)
//
// Notes:
// - The two TH2D histograms are owned by this struct (allocated with `new`).
// - The two slice-by-momentum objects are held as pointers so you can pass in your existing objects.
//   This keeps the struct usable even if the slice type is your custom class (e.g. hsPlots).
// =====================================================================================================

struct NeutronSelectionResolution_histograms {
    HistContext Context;

    // =================================================================================================
    // Resolution / TL-reco matching variables (set via SetUpResolutionVariables; NOT in the constructor)
    // =================================================================================================

    int Neutron_match_counter = 0;

    // Reco neutron debug kinematics (cached from the chosen reco neutron)
    double RecoNeutronP_Debug = 0.0;
    double RecoNeutronTheta_Debug = 0.0;
    double RecoNeutronPhi_Debug = 0.0;

    // Per-entry TL neutron kinematics (to be filled by your resolution loop)
    double TLNeutronP = 0.0;
    double TLNeutronTheta = 0.0;
    double TLNeutronPhi = 0.0;

    // Per-entry reco neutron kinematics (to be filled by your resolution loop)
    double RecoNeutronP = 0.0;
    double RecoNeutronTheta = 0.0;
    double RecoNeutronPhi = 0.0;

    // Per-entry TL-reco differences (to be filled by your resolution loop)
    double dNeutronTheta = 0.0;
    double dNeutronPhi = 0.0;

    // Per-entry PID (to be filled by your resolution loop)
    int pid_nRes = 0;

    // Matching cuts (defaults as in your snippet; you can override later)
    double dPhiCut = 5.0;
    double dThetaCut = 2.0;

    // Per-entry cut booleans (to be filled by your resolution loop)
    bool nRes_TL_Pass_PIDCut = false;
    bool Reco_InFD = true;
    bool TL_InFD = true;
    bool nRes_Pass_FiducialCuts = true;

    bool Reco_Theta_kinCut = true;
    bool TL_Theta_kinCuts = true;
    bool nRes_Pass_ThetaKinCut = true;

    bool nRes_Reco_Pass_Neutron_MomKinCut = true;
    bool nRes_TL_Pass_Neutron_MomKinCut = true;

    bool nRes_Pass_dThetaCut = false;
    bool nRes_Pass_dPhiCut = false;

    double nResolution = 0.0;

    // TH2D histograms (owned)
    TH2D* h_P_nFD_Res_VS_truth_P_nFD;
    TH2D* h_P_nFD_Res_VS_reco_P_nFD;

    // Slice-by-momentum objects (not owned)
    std::vector<TObject*> Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList;
    hsPlots h_P_nFD_Res_BysliceOf_truth_P_nFD;
    std::vector<TObject*> Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList;
    hsPlots h_P_nFD_Res_BysliceOf_reco_P_nFD;

    // Constructor:
    // - `list` may be nullptr; if non-null, TH2Ds are added to it.
    // - `nameTag` lets you keep your naming convention consistent (e.g. "ECALveto" or "matched").
    // - `truthSlice` and `recoSlice` are pointers to already-constructed slice objects.
    NeutronSelectionResolution_histograms(std::vector<TH1*>& HistoList, const vector<vector<double>>& nFD_Res_SliceLimits, vector<TObject*> HistoList_nRes_BysliceOf_truth_P_nFD,
                                          vector<TObject*> HistoList_nRes_BysliceOf_reco_P_nFD, const double& Ebeam, const HistContext& ctx)
        : Context(ctx),
          h_P_nFD_Res_BysliceOf_truth_P_nFD(nFD_Res_SliceLimits, hsPlots::TH1D_TYPE, HistoList_nRes_BysliceOf_truth_P_nFD, &Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList,
                                            ("P_" + ctx.nTag + "_Res_BysliceOf_truth_P_" + ctx.nTag + "_" + ctx.selection + "_1e_cut").c_str(),
                                            ("P_{" + ctx.nTag + "} resolution in slices of P^{truth}_{" + ctx.nTag + "} in 1e cut (" + ctx.selection + ");Resolution = (P^{truth}_{" +
                                             ctx.nTag + "} - P^{reco}_{" + ctx.nTag + "})/P^{truth}_{" + ctx.nTag + "};Counts")
                                                .c_str(),
                                            50, -1.1, 1.1, 50, -1.1, 1.1, "P^{truth}_{" + ctx.nTag + "} [GeV/c]"),
          h_P_nFD_Res_BysliceOf_reco_P_nFD(nFD_Res_SliceLimits, hsPlots::TH1D_TYPE, HistoList_nRes_BysliceOf_reco_P_nFD, &Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList,
                                           ("P_" + ctx.nTag + "_Res_BysliceOf_reco_P_" + ctx.nTag + "_" + ctx.selection + "_1e_cut").c_str(),
                                           ("P_{" + ctx.nTag + "} resolution in slices of P^{reco}_{" + ctx.nTag + "} in 1e cut (" + ctx.selection + ");Resolution = (P^{truth}_{" +
                                            ctx.nTag + "} - P^{reco}_{" + ctx.nTag + "})/P^{truth}_{" + ctx.nTag + "};Counts")
                                               .c_str(),
                                           50, -1.1, 1.1, 50, -1.1, 1.1, "P^{reco}_{" + ctx.nTag + "} [GeV/c]") {
        h_P_nFD_Res_VS_truth_P_nFD = new TH2D(("P_" + ctx.nTag + "_Res_VS_truth_P_" + ctx.nTag + "_" + ctx.selection + "_1e_cut").c_str(),
                                              ("R_{" + ctx.nTag + "} vs. P^{truth}_{" + ctx.nTag + "} in 1e cut (" + ctx.selection + ");P^{truth}_{" + ctx.nTag +
                                               "} [GeV/c];Resolution = (P^{truth}_{" + ctx.nTag + "} - P^{reco}_{" + ctx.nTag + "})/P^{truth}_{" + ctx.nTag + "}")
                                                  .c_str(),
                                              100, 0, Ebeam * 1.1, 100, -1.1, 1.1);
        HistoList.push_back(h_P_nFD_Res_VS_truth_P_nFD);
        h_P_nFD_Res_VS_reco_P_nFD = new TH2D(("P_" + ctx.nTag + "_Res_VS_reco_P_" + ctx.nTag + "_" + ctx.selection + "_1e_cut").c_str(),
                                             ("R_{" + ctx.nTag + "} vs. P^{reco}_{" + ctx.nTag + "} in 1e cut (" + ctx.selection + ");P^{reco}_{" + ctx.nTag +
                                              "} [GeV/c];Resolution = (P^{truth}_{" + ctx.nTag + "} - P^{reco}_{" + ctx.nTag + "})/P^{truth}_{" + ctx.nTag + "}")
                                                 .c_str(),
                                             100, 0, Ebeam * 1.1, 100, -1.1, 1.1);
        HistoList.push_back(h_P_nFD_Res_VS_reco_P_nFD);
    }

    void Fill(double TLNeutronP, double RecoNeutronP, double nResolution, const double& weight) {
        h_P_nFD_Res_VS_truth_P_nFD->Fill(TLNeutronP, nResolution, weight);
        h_P_nFD_Res_VS_reco_P_nFD->Fill(RecoNeutronP, nResolution, weight);
        h_P_nFD_Res_BysliceOf_truth_P_nFD.Fill(TLNeutronP, nResolution, nResolution, weight);
        h_P_nFD_Res_BysliceOf_reco_P_nFD.Fill(RecoNeutronP, nResolution, nResolution, weight);
    }
};