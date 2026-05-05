//
// Created by Alon Sportes on 29/03/2026.
//

#include "FinalState/FinalStateHistograms.h"

// ======================================================================================================================================================================
// Constructors
// ======================================================================================================================================================================

// Main constructor: initializes the beta-curve helpers, then builds every histogram category from the provided settings.
FinalStateHistograms::FinalStateHistograms(const ExperimentParameters& Experiment, const FinalStateConfiguration::EventVariant& eventVariant,
                                           const FinalStateConfiguration::FinalStateSignature& finalStateSignature, const HistogramDefinitionSettings& hDefSettings)
    : finalStateConfig(Experiment, eventVariant, finalStateSignature), betaCurves(hDefSettings.Momentum_ulim) {
    Init1eCutHistograms(hDefSettings);
    InitReactionMonitoringHistograms(hDefSettings);
    InitWHistograms(hDefSettings);
    InitEeHistograms(hDefSettings);
    InitOmegaHistograms(hDefSettings);
    InitEcalHistograms(hDefSettings);
    InitTKIHistograms(hDefSettings);
    InitMomentumTransferHistograms(hDefSettings);
    InitBetaHistograms(hDefSettings);
    InitAngleHistograms(hDefSettings);
    InitAcceptanceMapHistograms(hDefSettings);
    InitResolutionHistograms(hDefSettings);
}

// ======================================================================================================================================================================
// Destructor
// ======================================================================================================================================================================

// Destructor: releases the ROOT histogram objects owned by this class.
FinalStateHistograms::~FinalStateHistograms() {
    for (TH1D* histogram : owned_th1d_histograms_) {
        delete histogram;
    }
    for (TH2D* histogram : owned_th2d_histograms_) {
        delete histogram;
    }
    for (THStack* histogram : owned_thstack_histograms_) {
        delete histogram;
    }
}

TH1D* FinalStateHistograms::Make1D(const std::string& finalState, const std::string& statTitle, const std::string& histTitle, const std::string& xAxisTitle, double lowerX, double upperX,
                                   const int bins) {
    const std::string displayTitle = histTitle + " (" + finalState + ", );" + xAxisTitle + ";";
    const std::string fullStatTitle = statTitle + (finalState.empty() ? "" : " (" + finalState + ")");
    return OwnHistogram(new TH1D(fullStatTitle.c_str(), displayTitle.c_str(), bins, lowerX, upperX));
}

TH2D* FinalStateHistograms::Make2D(const std::string& finalState, const std::string& statTitle, const std::string& histTitle, const std::string& xAxisTitle, const std::string& yAxisTitle,
                                   const double lowerX, const double upperX, const double lowerY, const double upperY, const int binsX, const int binsY) {
    const std::string context = histogram_functions::JoinHistogram2DContext("", finalState, "");
    const std::string fullStatTitle = statTitle + (context.empty() ? "" : " (" + context + ")");
    const std::string displayTitle = histTitle + (context.empty() ? "" : " (" + context + ")") + ";" + xAxisTitle + ";" + yAxisTitle;
    return OwnHistogram(new TH2D(fullStatTitle.c_str(), displayTitle.c_str(), binsX, lowerX, upperX, binsY, lowerY, upperY));
}

TH1D* FinalStateHistograms::OwnHistogram(TH1D* histogram) {
    owned_th1d_histograms_.push_back(histogram);
    return histogram;
}

TH2D* FinalStateHistograms::OwnHistogram(TH2D* histogram) {
    owned_th2d_histograms_.push_back(histogram);
    return histogram;
}

THStack* FinalStateHistograms::OwnHistogram(THStack* histogram) {
    owned_thstack_histograms_.push_back(histogram);
    return histogram;
}

void FinalStateHistograms::DeleteHistogram(TH1D*& histogram) {
    delete histogram;
    histogram = nullptr;
}

void FinalStateHistograms::DeleteHistogram(TH2D*& histogram) {
    delete histogram;
    histogram = nullptr;
}

void FinalStateHistograms::DeleteHistogram(THStack*& histogram) {
    delete histogram;
    histogram = nullptr;
}

void FinalStateHistograms::BindReactionHistograms(HistogramsByReaction<TH1D>& group, THStack* stack, TH1D* allIntHistogram, TH1D* qeHistogram, TH1D* mecHistogram, TH1D* resHistogram,
                                                  TH1D* disHistogram) {
    group.Reset();
    group.SetStack(stack);
    group.SetHistograms(allIntHistogram, qeHistogram, mecHistogram, resHistogram, disHistogram);
    group.AddHistogramsToStack();
}

void FinalStateHistograms::BindReactionHistograms(HistogramsByReaction<TH2D>& group, TH2D* allIntHistogram, TH2D* qeHistogram, TH2D* mecHistogram, TH2D* resHistogram, TH2D* disHistogram) {
    group.Reset();
    group.SetHistograms(allIntHistogram, qeHistogram, mecHistogram, resHistogram, disHistogram);
}

void FinalStateHistograms::AttachBetaCurvesToHistogram(TH2D* histogram) {
    if (histogram == nullptr) { return; }

    const std::initializer_list<BetaCurveLegendEntry> entries = {
        {betaCurves.beta_neutron, "n", kOrange + 7},     {betaCurves.beta_proton, "p", kRed + 1},         {betaCurves.beta_Kplus, "K^{+}", kGreen + 2},
        {betaCurves.beta_Kminus, "K^{-}", kGreen + 3},   {betaCurves.beta_Kzero, "K^{0}", kTeal + 2},     {betaCurves.beta_piplus, "#pi^{+}", kBlue + 1},
        {betaCurves.beta_piminus, "#pi^{-}", kBlue + 2}, {betaCurves.beta_pizero, "#pi^{0}", kAzure + 1}, {betaCurves.beta_electron, "e^{-}", kMagenta + 2},
        {betaCurves.beta_photon, "#gamma", kBlack},      {betaCurves.beta_deuteron, "d", kViolet + 1},
    };

    auto* legend = new TLegend(0.16, 0.62, 0.48, 0.89);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->SetNColumns(2);
    legend->SetTextSize(0.026);

    auto* functions = histogram->GetListOfFunctions();
    for (const auto& entry : entries) {
        auto* curveClone = static_cast<TF1*>(entry.curve->Clone(Form("%s_%s", histogram->GetName(), entry.curve->GetName())));
        curveClone->SetLineColor(entry.color);
        curveClone->SetLineWidth(2);
        curveClone->SetBit(TF1::kNotDraw, false);
        functions->Add(curveClone);
        legend->AddEntry(curveClone, entry.label, "l");
    }

    functions->Add(legend);
}

void FinalStateHistograms::AttachBetaCurvesToHistogram(hPlot2D& plot) { AttachBetaCurvesToHistogram(plot.GetHistogram()); }

void FinalStateHistograms::Init1eCutHistograms(const HistogramDefinitionSettings& hDefSettings) {
    const std::string finalState = "1e cut";

    const int numTH1Dbins = hDefSettings.numTH1Dbins;
    const int numTH2Dbins = hDefSettings.numTH2Dbins;
    const int numTH1Dbins_Mom_eff_Plots = hDefSettings.numTH1Dbins_Mom_eff_Plots;
    const double beamEMax = hDefSettings.Momentum_ulim;

    h_Vx_e_1e_cut = Make1D(finalState, "V_{x}^{e}", "V_{x}^{e} histogram", "V_{x}^{e} [cm]", hDefSettings.Vx_FD_llim, hDefSettings.Vx_FD_ulim, numTH1Dbins);
    h_Vy_e_1e_cut = Make1D(finalState, "V_{y}^{e}", "V_{y}^{e} histogram", "V_{y}^{e} [cm]", hDefSettings.Vy_FD_llim, hDefSettings.Vy_FD_ulim, numTH1Dbins);
    h_Vz_e_1e_cut = Make1D(finalState, "V_{z}^{e}", "V_{z}^{e} histogram", "V_{z}^{e} [cm]", hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH1Dbins);
    h_Vz_e_vs_P_e_1e_cut = Make2D(finalState, "V_{z}^{e} vs. P_{e}", "V_{z}^{e} vs. P_{e}", "P_{e} [GeV/c]", "V_{z}^{e} [cm]", hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim,
                                  hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_Vz_e_vs_Theta_e_1e_cut = Make2D(finalState, "V_{z}^{e} vs. #theta_{e}", "V_{z}^{e} vs. #theta_{e}", "#theta_{e} [#circ]", "V_{z}^{e} [cm]", hDefSettings.Theta_llim_FD,
                                      hDefSettings.Theta_ulim_FD, hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_Vz_e_vs_Phi_e_1e_cut = Make2D(finalState, "V_{z}^{e} vs. #phi_{e}", "V_{z}^{e} vs. #theta_{e}", "#phi_{e} [#circ]", "V_{z}^{e} [cm]", hDefSettings.Phi_llim, hDefSettings.Phi_ulim,
                                    hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);

    h_Vx_pCD_1e_cut = Make1D(finalState, "V_{x}^{pCD}", "V_{x}^{pCD} histogram", "V_{x}^{pCD} [cm]", hDefSettings.Vx_CD_llim, hDefSettings.Vx_CD_ulim, numTH1Dbins);
    h_Vx_pFD_1e_cut = Make1D(finalState, "V_{x}^{pFD}", "V_{x}^{pFD} histogram", "V_{x}^{pFD} [cm]", hDefSettings.Vx_FD_llim, hDefSettings.Vx_FD_ulim, numTH1Dbins);
    h_Vy_pCD_1e_cut = Make1D(finalState, "V_{y}^{pCD}", "V_{y}^{pCD} histogram", "V_{y}^{pCD} [cm]", hDefSettings.Vy_CD_llim, hDefSettings.Vy_CD_ulim, numTH1Dbins);
    h_Vy_pFD_1e_cut = Make1D(finalState, "V_{y}^{pFD}", "V_{y}^{pFD} histogram", "V_{y}^{pFD} [cm]", hDefSettings.Vy_FD_llim, hDefSettings.Vy_FD_ulim, numTH1Dbins);
    h_Vz_pCD_1e_cut = Make1D(finalState, "V_{z}^{pCD}", "V_{z}^{pCD} histogram", "V_{z}^{pCD} [cm]", hDefSettings.Vz_CD_llim, hDefSettings.Vz_CD_ulim, numTH1Dbins);
    h_Vz_pFD_1e_cut = Make1D(finalState, "V_{z}^{pFD}", "V_{z}^{pFD} histogram", "V_{z}^{pFD} [cm]", hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH1Dbins);
    h_Vz_pFD_vs_P_pFD_1e_cut = Make2D(finalState, "V_{z}^{pFD} vs. P_{pFD}", "V_{z}^{pFD} vs. P_{pFD}", "P_{pFD} [GeV/c]", "V_{z}^{pFD} [cm]", hDefSettings.Momentum_llim,
                                      hDefSettings.Momentum_ulim, hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_Vz_pFD_vs_Theta_pFD_1e_cut = Make2D(finalState, "V_{z}^{pFD} vs. #theta_{pFD}", "V_{z}^{pFD} vs. #theta_{pFD}", "#theta_{pFD} [#circ]", "V_{z}^{pFD} [cm]", hDefSettings.Theta_llim_FD,
                                          hDefSettings.Theta_ulim_FD, hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_Vz_pFD_vs_Phi_pFD_1e_cut = Make2D(finalState, "V_{z}^{pFD} vs. #phi_{pFD}", "V_{z}^{pFD} vs. #theta_{pFD}", "#phi_{pFD} [#circ]", "V_{z}^{pFD} [cm]", hDefSettings.Phi_llim,
                                        hDefSettings.Phi_ulim, hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_Vz_pCD_vs_P_pCD_1e_cut = Make2D(finalState, "V_{z}^{pCD} vs. P_{pCD}", "V_{z}^{pCD} vs. P_{pCD}", "P_{pCD} [GeV/c]", "V_{z}^{pCD} [cm]", hDefSettings.Momentum_llim,
                                      hDefSettings.Momentum_ulim, hDefSettings.Vz_CD_llim, hDefSettings.Vz_CD_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_Vz_pCD_vs_Theta_pCD_1e_cut = Make2D(finalState, "V_{z}^{pCD} vs. #theta_{pCD}", "V_{z}^{pCD} vs. #theta_{pCD}", "#theta_{pCD} [#circ]", "V_{z}^{pCD} [cm]", hDefSettings.Theta_llim_CD,
                                          hDefSettings.Theta_ulim_CD, hDefSettings.Vz_CD_llim, hDefSettings.Vz_CD_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_Vz_pCD_vs_Phi_pCD_1e_cut = Make2D(finalState, "V_{z}^{pCD} vs. #phi_{pCD}", "V_{z}^{pCD} vs. #theta_{pCD}", "#phi_{pCD} [#circ]", "V_{z}^{pCD} [cm]", hDefSettings.Phi_llim,
                                        hDefSettings.Phi_ulim, hDefSettings.Vz_CD_llim, hDefSettings.Vz_CD_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);

    h_Vx_KpCD_1e_cut = Make1D(finalState, "V_{x}^{K^{+}CD}", "V_{x}^{K^{+}CD} histogram", "V_{x}^{K^{+}CD} [cm]", hDefSettings.Vx_CD_llim, hDefSettings.Vx_CD_ulim, numTH1Dbins);
    h_Vx_KpFD_1e_cut = Make1D(finalState, "V_{x}^{K^{+}FD}", "V_{x}^{K^{+}FD} histogram", "V_{x}^{K^{+}FD} [cm]", hDefSettings.Vx_FD_llim, hDefSettings.Vx_FD_ulim, numTH1Dbins);
    h_Vy_KpCD_1e_cut = Make1D(finalState, "V_{y}^{K^{+}CD}", "V_{y}^{K^{+}CD} histogram", "V_{y}^{K^{+}CD} [cm]", hDefSettings.Vy_CD_llim, hDefSettings.Vy_CD_ulim, numTH1Dbins);
    h_Vy_KpFD_1e_cut = Make1D(finalState, "V_{y}^{K^{+}FD}", "V_{y}^{K^{+}FD} histogram", "V_{y}^{K^{+}FD} [cm]", hDefSettings.Vy_FD_llim, hDefSettings.Vy_FD_ulim, numTH1Dbins);
    h_Vz_KpCD_1e_cut = Make1D(finalState, "V_{z}^{K^{+}CD}", "V_{z}^{K^{+}CD} histogram", "V_{z}^{K^{+}CD} [cm]", hDefSettings.Vz_CD_llim, hDefSettings.Vz_CD_ulim, numTH1Dbins);
    h_Vz_KpFD_1e_cut = Make1D(finalState, "V_{z}^{K^{+}FD}", "V_{z}^{K^{+}FD} histogram", "V_{z}^{K^{+}FD} [cm]", hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH1Dbins);

    h_Vx_KmCD_1e_cut = Make1D(finalState, "V_{x}^{K^{-}CD}", "V_{x}^{K^{-}CD} histogram", "V_{x}^{K^{-}CD} [cm]", hDefSettings.Vx_CD_llim, hDefSettings.Vx_CD_ulim, numTH1Dbins);
    h_Vx_KmFD_1e_cut = Make1D(finalState, "V_{x}^{K^{-}FD}", "V_{x}^{K^{-}FD} histogram", "V_{x}^{K^{-}FD} [cm]", hDefSettings.Vx_FD_llim, hDefSettings.Vx_FD_ulim, numTH1Dbins);
    h_Vy_KmCD_1e_cut = Make1D(finalState, "V_{y}^{K^{-}CD}", "V_{y}^{K^{-}CD} histogram", "V_{y}^{K^{-}CD} [cm]", hDefSettings.Vy_CD_llim, hDefSettings.Vy_CD_ulim, numTH1Dbins);
    h_Vy_KmFD_1e_cut = Make1D(finalState, "V_{y}^{K^{-}FD}", "V_{y}^{K^{-}FD} histogram", "V_{y}^{K^{-}FD} [cm]", hDefSettings.Vy_FD_llim, hDefSettings.Vy_FD_ulim, numTH1Dbins);
    h_Vz_KmCD_1e_cut = Make1D(finalState, "V_{z}^{K^{-}CD}", "V_{z}^{K^{-}CD} histogram", "V_{z}^{K^{-}CD} [cm]", hDefSettings.Vz_CD_llim, hDefSettings.Vz_CD_ulim, numTH1Dbins);
    h_Vz_KmFD_1e_cut = Make1D(finalState, "V_{z}^{K^{-}FD}", "V_{z}^{K^{-}FD} histogram", "V_{z}^{K^{-}FD} [cm]", hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH1Dbins);

    h_Vx_pipCD_1e_cut = Make1D(finalState, "V_{x}^{#pi^{+}CD}", "V_{x}^{#pi^{+}CD} histogram", "V_{x}^{#pi^{+}CD} [cm]", hDefSettings.Vx_CD_llim, hDefSettings.Vx_CD_ulim, numTH1Dbins);
    h_Vx_pipFD_1e_cut = Make1D(finalState, "V_{x}^{#pi^{+}FD}", "V_{x}^{#pi^{+}FD} histogram", "V_{x}^{#pi^{+}FD} [cm]", hDefSettings.Vx_FD_llim, hDefSettings.Vx_FD_ulim, numTH1Dbins);
    h_Vy_pipCD_1e_cut = Make1D(finalState, "V_{y}^{#pi^{+}CD}", "V_{y}^{#pi^{+}CD} histogram", "V_{y}^{#pi^{+}CD} [cm]", hDefSettings.Vy_CD_llim, hDefSettings.Vy_CD_ulim, numTH1Dbins);
    h_Vy_pipFD_1e_cut = Make1D(finalState, "V_{y}^{#pi^{+}FD}", "V_{y}^{#pi^{+}FD} histogram", "V_{y}^{#pi^{+}FD} [cm]", hDefSettings.Vy_FD_llim, hDefSettings.Vy_FD_ulim, numTH1Dbins);
    h_Vz_pipCD_1e_cut = Make1D(finalState, "V_{z}^{#pi^{+}CD}", "V_{z}^{#pi^{+}CD} histogram", "V_{z}^{#pi^{+}CD} [cm]", hDefSettings.Vz_CD_llim, hDefSettings.Vz_CD_ulim, numTH1Dbins);
    h_Vz_pipFD_1e_cut = Make1D(finalState, "V_{z}^{#pi^{+}FD}", "V_{z}^{#pi^{+}FD} histogram", "V_{z}^{#pi^{+}FD} [cm]", hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH1Dbins);

    h_Vx_pimCD_1e_cut = Make1D(finalState, "V_{x}^{#pi^{-}CD}", "V_{x}^{#pi^{-}CD} histogram", "V_{x}^{#pi^{-}CD} [cm]", hDefSettings.Vx_CD_llim, hDefSettings.Vx_CD_ulim, numTH1Dbins);
    h_Vx_pimFD_1e_cut = Make1D(finalState, "V_{x}^{#pi^{-}FD}", "V_{x}^{#pi^{-}FD} histogram", "V_{x}^{#pi^{-}FD} [cm]", hDefSettings.Vx_FD_llim, hDefSettings.Vx_FD_ulim, numTH1Dbins);
    h_Vy_pimCD_1e_cut = Make1D(finalState, "V_{y}^{#pi^{-}CD}", "V_{y}^{#pi^{-}CD} histogram", "V_{y}^{#pi^{-}CD} [cm]", hDefSettings.Vy_CD_llim, hDefSettings.Vy_CD_ulim, numTH1Dbins);
    h_Vy_pimFD_1e_cut = Make1D(finalState, "V_{y}^{#pi^{-}FD}", "V_{y}^{#pi^{-}FD} histogram", "V_{y}^{#pi^{-}FD} [cm]", hDefSettings.Vy_FD_llim, hDefSettings.Vy_FD_ulim, numTH1Dbins);
    h_Vz_pimCD_1e_cut = Make1D(finalState, "V_{z}^{#pi^{-}CD}", "V_{z}^{#pi^{-}CD} histogram", "V_{z}^{#pi^{-}CD} [cm]", hDefSettings.Vz_CD_llim, hDefSettings.Vz_CD_ulim, numTH1Dbins);
    h_Vz_pimFD_1e_cut = Make1D(finalState, "V_{z}^{#pi^{-}FD}", "V_{z}^{#pi^{-}FD} histogram", "V_{z}^{#pi^{-}FD} [cm]", hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH1Dbins);

    h_Vx_d_CD_1e_cut = Make1D(finalState, "V_{x}^{D,CD}", "V_{x}^{D,CD} histogram", "V_{x}^{D,CD} [cm]", hDefSettings.Vx_CD_llim, hDefSettings.Vx_CD_ulim, numTH1Dbins);
    h_Vx_d_FD_1e_cut = Make1D(finalState, "V_{x}^{D,FD}", "V_{x}^{D,FD} histogram", "V_{x}^{D,FD} [cm]", hDefSettings.Vx_FD_llim, hDefSettings.Vx_FD_ulim, numTH1Dbins);
    h_Vy_d_CD_1e_cut = Make1D(finalState, "V_{y}^{D,CD}", "V_{y}^{D,CD} histogram", "V_{y}^{D,CD} [cm]", hDefSettings.Vy_CD_llim, hDefSettings.Vy_CD_ulim, numTH1Dbins);
    h_Vy_d_FD_1e_cut = Make1D(finalState, "V_{y}^{D,FD}", "V_{y}^{D,FD} histogram", "V_{y}^{D,FD} [cm]", hDefSettings.Vy_FD_llim, hDefSettings.Vy_FD_ulim, numTH1Dbins);
    h_Vz_d_CD_1e_cut = Make1D(finalState, "V_{z}^{D,CD}", "V_{z}^{D,CD} histogram", "V_{z}^{D,CD} [cm]", hDefSettings.Vz_CD_llim, hDefSettings.Vz_CD_ulim, numTH1Dbins);
    h_Vz_d_FD_1e_cut = Make1D(finalState, "V_{z}^{D,FD}", "V_{z}^{D,FD} histogram", "V_{z}^{D,FD} [cm]", hDefSettings.Vz_FD_llim, hDefSettings.Vz_FD_ulim, numTH1Dbins);

    h_dVx_pFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{pFD}_{x}", "pFD vertex correlation dV^{pFD}_{x} histogram", "dV^{pFD}_{x} = V^{pFD}_{x} - V^{e}_{x} [cm]",
                              hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVy_pFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{pFD}_{y}", "pFD vertex correlation dV^{pFD}_{y} histogram", "dV^{pFD}_{y} = V^{pFD}_{y} - V^{e}_{y} [cm]",
                              hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVz_pFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{pFD}_{z}", "pFD vertex correlation dV^{pFD}_{z} histogram", "dV^{pFD}_{z} = V^{pFD}_{z} - V^{e}_{z} [cm]",
                              hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVz_pFD_vs_P_pFD_1e_cut = Make2D(finalState, "dV^{pFD}_{z} vs. P_{pFD}", "dV^{pFD}_{z} vs. P_{pFD}", "P_{pFD} [GeV/c]", "dV^{pFD}_{z} [cm]", hDefSettings.Momentum_llim,
                                       hDefSettings.Momentum_ulim, hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_dVz_pFD_vs_Theta_pFD_1e_cut = Make2D(finalState, "dV^{pFD}_{z} vs. #theta_{pFD}", "dV^{pFD}_{z} vs. #theta_{pFD}", "#theta_{pFD} [#circ]", "dV^{pFD}_{z} [cm]",
                                           hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_dVz_pFD_vs_Phi_pFD_1e_cut = Make2D(finalState, "dV^{pFD}_{z} vs. #phi_{pFD}", "dV^{pFD}_{z} vs. #theta_{pFD}", "#phi_{pFD} [#circ]", "dV^{pFD}_{z} [cm]", hDefSettings.Phi_llim,
                                         hDefSettings.Phi_ulim, hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH2Dbins * 2., numTH2Dbins * 2.);

    h_dVx_pCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{pCD}_{x}", "pCD vertex correlation dV^{pCD}_{x} histogram", "dV^{pCD}_{x} = V^{pCD}_{x} - V^{e}_{x} [cm]",
                              hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVy_pCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{pCD}_{y}", "pCD vertex correlation dV^{pCD}_{y} histogram", "dV^{pCD}_{y} = V^{pCD}_{y} - V^{e}_{y} [cm]",
                              hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVz_pCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{pCD}_{z}", "pCD vertex correlation dV^{pCD}_{z} histogram", "dV^{pCD}_{z} = V^{pCD}_{z} - V^{e}_{z} [cm]",
                              hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVz_pCD_vs_P_pCD_1e_cut = Make2D(finalState, "dV^{pCD}_{z} vs. P_{pCD}", "dV^{pCD}_{z} vs. P_{pCD}", "P_{pCD} [GeV/c]", "dV^{pCD}_{z} [cm]", hDefSettings.Momentum_llim,
                                       hDefSettings.Momentum_ulim, hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_dVz_pCD_vs_Theta_pCD_1e_cut = Make2D(finalState, "dV^{pCD}_{z} vs. #theta_{pCD}", "dV^{pCD}_{z} vs. #theta_{pCD}", "#theta_{pCD} [#circ]", "dV^{pCD}_{z} [cm]",
                                           hDefSettings.Theta_llim_CD, hDefSettings.Theta_ulim_CD, hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH2Dbins * 2., numTH2Dbins * 2.);
    h_dVz_pCD_vs_Phi_pCD_1e_cut = Make2D(finalState, "dV^{pCD}_{z} vs. #phi_{pCD}", "dV^{pCD}_{z} vs. #theta_{pCD}", "#phi_{pCD} [#circ]", "dV^{pCD}_{z} [cm]", hDefSettings.Phi_llim,
                                         hDefSettings.Phi_ulim, hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH2Dbins * 2., numTH2Dbins * 2.);

    h_dVx_KpFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{+}FD}_{x}", "FD K^{+} vertex correlation dV^{K^{+}FD}_{x} histogram", "dV^{K^{+}FD}_{x} = V^{K^{+}FD}_{x} - V^{e}_{x} [cm]",
                               hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVy_KpFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{+}FD}_{y}", "FD K^{+} vertex correlation dV^{K^{+}FD}_{y} histogram", "dV^{K^{+}FD}_{y} = V^{K^{+}FD}_{y} - V^{e}_{y} [cm]",
                               hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVz_KpFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{+}FD}_{z}", "FD K^{+} vertex correlation dV^{K^{+}FD}_{z} histogram", "dV^{K^{+}FD}_{z} = V^{K^{+}FD}_{z} - V^{e}_{z} [cm]",
                               hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVx_KpCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{+}CD}_{x}", "CD K^{+} vertex correlation dV^{K^{+}CD}_{x} histogram", "dV^{K^{+}CD}_{x} = V^{K^{+}CD}_{x} - V^{e}_{x} [cm]",
                               hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVy_KpCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{+}CD}_{y}", "CD K^{+} vertex correlation dV^{K^{+}CD}_{y} histogram", "dV^{K^{+}CD}_{y} = V^{K^{+}CD}_{y} - V^{e}_{y} [cm]",
                               hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVz_KpCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{+}CD}_{z}", "CD K^{+} vertex correlation dV^{K^{+}CD}_{z} histogram", "dV^{K^{+}CD}_{z} = V^{K^{+}CD}_{z} - V^{e}_{z} [cm]",
                               hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);

    h_dVx_KmFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{-}FD}_{x}", "FD K^{-} vertex correlation dV^{K^{-}FD}_{x} histogram", "dV^{K^{-}FD}_{x} = V^{K^{-}FD}_{x} - V^{e}_{x} [cm]",
                               hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVy_KmFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{-}FD}_{y}", "FD K^{-} vertex correlation dV^{K^{-}FD}_{y} histogram", "dV^{K^{-}FD}_{y} = V^{K^{-}FD}_{y} - V^{e}_{y} [cm]",
                               hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVz_KmFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{-}FD}_{z}", "FD K^{-} vertex correlation dV^{K^{-}FD}_{z} histogram", "dV^{K^{-}FD}_{z} = V^{K^{-}FD}_{z} - V^{e}_{z} [cm]",
                               hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVx_KmCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{-}CD}_{x}", "CD K^{-} vertex correlation dV^{K^{-}CD}_{x} histogram", "dV^{K^{-}CD}_{x} = V^{K^{-}CD}_{x} - V^{e}_{x} [cm]",
                               hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVy_KmCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{-}CD}_{y}", "CD K^{-} vertex correlation dV^{K^{-}CD}_{y} histogram", "dV^{K^{-}CD}_{y} = V^{K^{-}CD}_{y} - V^{e}_{y} [cm]",
                               hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVz_KmCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{K^{-}CD}_{z}", "CD K^{-} vertex correlation dV^{K^{-}CD}_{z} histogram", "dV^{K^{-}CD}_{z} = V^{K^{-}CD}_{z} - V^{e}_{z} [cm]",
                               hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);

    h_dVx_pipFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{+}FD}_{x}", "FD #pi^{+} vertex correlation dV^{#pi^{+}FD}_{x} histogram",
                                "dV^{#pi^{+}FD}_{x} = V^{#pi^{+}FD}_{x} - V^{e}_{x} [cm]", hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVy_pipFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{+}FD}_{y}", "FD #pi^{+} vertex correlation dV^{#pi^{+}FD}_{y} histogram",
                                "dV^{#pi^{+}FD}_{y} = V^{#pi^{+}FD}_{y} - V^{e}_{y} [cm]", hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVz_pipFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{+}FD}_{z}", "FD #pi^{+} vertex correlation dV^{#pi^{+}FD}_{z} histogram",
                                "dV^{#pi^{+}FD}_{z} = V^{#pi^{+}FD}_{z} - V^{e}_{z} [cm]", hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVx_pipCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{+}CD}_{x}", "CD #pi^{+} vertex correlation dV^{#pi^{+}CD}_{x} histogram",
                                "dV^{#pi^{+}CD}_{x} = V^{#pi^{+}CD}_{x} - V^{e}_{x} [cm]", hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVy_pipCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{+}CD}_{y}", "CD #pi^{+} vertex correlation dV^{#pi^{+}CD}_{y} histogram",
                                "dV^{#pi^{+}CD}_{y} = V^{#pi^{+}CD}_{y} - V^{e}_{y} [cm]", hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVz_pipCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{+}CD}_{z}", "CD #pi^{+} vertex correlation dV^{#pi^{+}CD}_{z} histogram",
                                "dV^{#pi^{+}CD}_{z} = V^{#pi^{+}CD}_{z} - V^{e}_{z} [cm]", hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);

    h_dVx_pimFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{-}FD}_{x}", "FD #pi^{-} vertex correlation dV^{#pi^{-}FD}_{x} histogram",
                                "dV^{#pi^{-}FD}_{x} = V^{#pi^{-}FD}_{x} - V^{e}_{x} [cm]", hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVy_pimFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{-}FD}_{y}", "FD #pi^{-} vertex correlation dV^{#pi^{-}FD}_{y} histogram",
                                "dV^{#pi^{-}FD}_{y} = V^{#pi^{-}FD}_{y} - V^{e}_{y} [cm]", hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVz_pimFD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{-}FD}_{z}", "FD #pi^{-} vertex correlation dV^{#pi^{-}FD}_{z} histogram",
                                "dV^{#pi^{-}FD}_{z} = V^{#pi^{-}FD}_{z} - V^{e}_{z} [cm]", hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVx_pimCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{-}CD}_{x}", "CD #pi^{-} vertex correlation dV^{#pi^{-}CD}_{x} histogram",
                                "dV^{#pi^{-}CD}_{x} = V^{#pi^{-}CD}_{x} - V^{e}_{x} [cm]", hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVy_pimCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{-}CD}_{y}", "CD #pi^{-} vertex correlation dV^{#pi^{-}CD}_{y} histogram",
                                "dV^{#pi^{-}CD}_{y} = V^{#pi^{-}CD}_{y} - V^{e}_{y} [cm]", hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVz_pimCD_1e_cut = Make1D(finalState, "Vertex corr. dV^{#pi^{-}CD}_{z}", "CD #pi^{-} vertex correlation dV^{#pi^{-}CD}_{z} histogram",
                                "dV^{#pi^{-}CD}_{z} = V^{#pi^{-}CD}_{z} - V^{e}_{z} [cm]", hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);

    h_dVx_d_FD_1e_cutt = Make1D(finalState, "Vertex corr. dV^{DFD}_{x}", "FD D vertex correlation dV^{DFD}_{x} histogram", "dV^{DFD}_{x} = V^{DFD}_{x} - V^{e}_{x} [cm]",
                                hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVy_d_FD_1e_cutt = Make1D(finalState, "Vertex corr. dV^{DFD}_{y}", "FD D vertex correlation dV^{DFD}_{y} histogram", "dV^{DFD}_{y} = V^{DFD}_{y} - V^{e}_{y} [cm]",
                                hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVz_d_FD_1e_cutt = Make1D(finalState, "Vertex corr. dV^{DFD}_{z}", "FD D vertex correlation dV^{DFD}_{z} histogram", "dV^{DFD}_{z} = V^{DFD}_{z} - V^{e}_{z} [cm]",
                                hDefSettings.dV_llim_FD, hDefSettings.dV_ulim_FD, numTH1Dbins);
    h_dVx_d_CD_1e_cutt = Make1D(finalState, "Vertex corr. dV^{DCD}_{x}", "CD D vertex correlation dV^{DCD}_{x} histogram", "dV^{DCD}_{x} = V^{DCD}_{x} - V^{e}_{x} [cm]",
                                hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVy_d_CD_1e_cutt = Make1D(finalState, "Vertex corr. dV^{DCD}_{y}", "CD D vertex correlation dV^{DCD}_{y} histogram", "dV^{DCD}_{y} = V^{DCD}_{y} - V^{e}_{y} [cm]",
                                hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);
    h_dVz_d_CD_1e_cutt = Make1D(finalState, "Vertex corr. dV^{DCD}_{z}", "CD D vertex correlation dV^{DCD}_{z} histogram", "dV^{DCD}_{z} = V^{DCD}_{z} - V^{e}_{z} [cm]",
                                hDefSettings.dV_llim_CD, hDefSettings.dV_ulim_CD, numTH1Dbins);

    for (int i = 1; i <= 3; ++i) {
        h_dc_e_hitmap_AC_1e_cut[i] = OwnHistogram(new TH2D(Form("dc_e_hitmap_region_%d_AC", i), Form("DC electron hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300));
        h_dc_pFD_hit_map_AC_1e_cut[i] = OwnHistogram(new TH2D(Form("dc_pFD_hitmap_region_%d_AC", i), Form("DC pFD hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300));
        h_dc_KpFD_hit_map_AC_1e_cut[i] =
            OwnHistogram(new TH2D(Form("dc_KpFD_hitmap_region_%d_AC", i), Form("DC K^{+}FD hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300));
        h_dc_KmFD_hit_map_AC_1e_cut[i] =
            OwnHistogram(new TH2D(Form("dc_KmFD_hitmap_region_%d_AC", i), Form("DC K^{-}FD hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300));
        h_dc_pipFD_hit_map_AC_1e_cut[i] =
            OwnHistogram(new TH2D(Form("dc_pipFD_hitmap_region_%d_AC", i), Form("DC #pi^{+}FD hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300));
        h_dc_pimFD_hit_map_AC_1e_cut[i] =
            OwnHistogram(new TH2D(Form("dc_pimFD_hitmap_region_%d_AC", i), Form("DC #pi^{-}FD hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300));
        h_dc_d_FD_hit_map_AC_1e_cut[i] =
            OwnHistogram(new TH2D(Form("dc_d_FD_hitmap_region_%d_AC", i), Form("DC FD deuteron hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300));
    }

    h_Nphe_1e_cut_BC = Make1D(finalState, "N_{phe} in HTCC", "N_{phe} in HTCC", "N_{phe}", 0.0, hDefSettings.Nphe_lim, numTH1Dbins);
    h_Nphe_1e_cut_AC = Make1D(finalState, "N_{phe} in HTCC AC", "N_{phe} in HTCC - AC", "N_{phe}", 0.0, hDefSettings.Nphe_lim, numTH1Dbins);
    h_Electron_Edep_PCAL_VS_Edep_EC_1e_cut =
        OwnHistogram(new TH2D("Electron_Edep_PCAL_VS_Edep_EC_1e_cut", "Electron E_{dep}^{PCAL} vs. E_{dep}^{EC} (1e cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]",
                 numTH2Dbins * 2., 0, 0.6, numTH2Dbins * 2., 0, 0.6));

    hSF_1e_cut_BC = Make1D(finalState, "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", hDefSettings.SF_llim, hDefSettings.SF_ulim, numTH1Dbins);
    hSF_1e_cut_AC =
        Make1D(finalState, "SF AC", "Sampling fraction f_{e} - after cuts", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", hDefSettings.SF_llim, hDefSettings.SF_ulim, numTH1Dbins);
    hSF_VS_P_e_1e_cut_BC = Make2D(finalState, "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 0.0, beamEMax,
                                  hDefSettings.SF_llim, hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    hSF_VS_P_e_1e_cut_AC = Make2D(finalState, "SF vs. P_{e} AC", "Sampling fraction f_{e} vs. P_{e} - after cuts", "P_{e} [GeV/c]", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 0.0,
                                  beamEMax, hDefSettings.SF_llim, hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    hSF_VS_Edep_PCAL_1e_cut_BC = Make2D(finalState, "SF vs. E_{dep}^{PCAL}", "Sampling fraction f_{e} vs. E_{dep}^{PCAL}", "E_{dep}^{PCAL} [GeV]",
                                        "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 0.0, 1.25, hDefSettings.SF_llim, hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    hSF_VS_Edep_PCAL_1e_cut_AC = Make2D(finalState, "SF vs. E_{dep}^{PCAL} AC", "Sampling fraction f_{e} vs. E_{dep}^{PCAL} - after cuts", "E_{dep}^{PCAL} [GeV]",
                                        "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 0.0, 1.25, hDefSettings.SF_llim, hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    for (int i = 1; i <= 6; ++i) {
        h_SF_VS_P_e_BC_BySector_1e_cut[i] =
            OwnHistogram(new TH2D(Form("SF_VS_P_e_BC_Sector%d_1e_cut", i), Form("Electron sampling fraction vs. P_{e} - sector %d - before cuts (1e cut);P_{e} [GeV/c];Electron sampling fraction", i),
                     numTH2Dbins * 2., 0, beamEMax, numTH2Dbins * 2., hDefSettings.SF_llim, hDefSettings.SF_ulim));
        h_SF_VS_P_e_AC_BySector_1e_cut[i] =
            OwnHistogram(new TH2D(Form("SF_VS_P_e_AC_Sector%d_1e_cut", i), Form("Electron sampling fraction vs. P_{e} - sector %d - after cuts (1e cut);P_{e} [GeV/c];Electron sampling fraction", i),
                     numTH2Dbins * 2., 0, beamEMax, numTH2Dbins * 2., hDefSettings.SF_llim, hDefSettings.SF_ulim));
        h_SF_VS_Edep_PCAL_BC_BySector_1e_cut[i] =
            OwnHistogram(new TH2D(Form("SF_VS_Edep_PCAL_BC_Sector%d_1e_cut", i),
                     Form("Electron sampling fraction vs. E_{dep}^{PCAL} - sector %d - before cuts (1e cut);E_{dep}^{PCAL} [GeV];Electron sampling fraction", i), numTH2Dbins * 2., 0, 1.25,
                     numTH2Dbins * 2., hDefSettings.SF_llim, hDefSettings.SF_ulim));
        h_SF_VS_Edep_PCAL_AC_BySector_1e_cut[i] =
            OwnHistogram(new TH2D(Form("SF_VS_Edep_PCAL_AC_Sector%d_1e_cut", i),
                     Form("Electron sampling fraction vs. E_{dep}^{PCAL} - sector %d - after cuts (1e cut);E_{dep}^{PCAL} [GeV];Electron sampling fraction", i), numTH2Dbins * 2., 0, 1.25,
                     numTH2Dbins * 2., hDefSettings.SF_llim, hDefSettings.SF_ulim));
    }

    hVcal_VS_EoP_1e_cut_BC_PCAL = Make2D(finalState, "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]", "Sampling Fraction (SF)", 0.0, 50, hDefSettings.SF_llim,
                                         hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    hVcal_VS_EoP_1e_cut_AC_PCAL = Make2D(finalState, "Vcal vs. SF AC", "ECAL V coordinate vs. SF - after cuts", "ECAL V coordinate [cm]", "Sampling Fraction (SF)", 0.0, 50,
                                         hDefSettings.SF_llim, hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    hWcal_VS_EoP_1e_cut_BC_PCAL = Make2D(finalState, "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]", "Sampling Fraction (SF)", 0.0, 50, hDefSettings.SF_llim,
                                         hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    hWcal_VS_EoP_1e_cut_AC_PCAL = Make2D(finalState, "Wcal vs. SF AC", "ECAL W coordinate vs. SF - after cuts", "ECAL W coordinate [cm]", "Sampling Fraction (SF)", 0.0, 50,
                                         hDefSettings.SF_llim, hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    hUcal_VS_EoP_1e_cut_BC_PCAL = Make2D(finalState, "Ucal vs. SF", "ECAL U coordinate vs. SF", "ECAL U coordinate [cm]", "Sampling Fraction (SF)", 0.0, 50, hDefSettings.SF_llim,
                                         hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    hUcal_VS_EoP_1e_cut_AC_PCAL = Make2D(finalState, "Ucal vs. SF AC", "ECAL U coordinate vs. SF - after cuts", "ECAL U coordinate [cm]", "Sampling Fraction (SF)", 0.0, 50,
                                         hDefSettings.SF_llim, hDefSettings.SF_ulim, numTH2Dbins * 2., numTH2Dbins * 2.);
    hPCAL_acceptance_1e_cut = Make2D(finalState, "Electron PCAL hitmap", "Electron PCAL hitmap", "x [cm]", "y [cm]", -400, 400, -400, 400, numTH2Dbins * 2., numTH2Dbins * 2.);

    hChi2_e_1e_cut = Make1D(finalState, "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_pCD_1e_cut = Make1D(finalState, "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_pFD_1e_cut = Make1D(finalState, "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_KpCD_1e_cut = Make1D(finalState, "#chi^{2}_{K^{+}}", "#chi^{2}_{K^{+}} histogram", "#chi^{2}_{K^{+}}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_KpFD_1e_cut = Make1D(finalState, "#chi^{2}_{K^{+}}", "#chi^{2}_{K^{+}} histogram", "#chi^{2}_{K^{+}}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_KmCD_1e_cut = Make1D(finalState, "#chi^{2}_{K^{-}}", "#chi^{2}_{K^{-}} histogram", "#chi^{2}_{K^{-}}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_KmFD_1e_cut = Make1D(finalState, "#chi^{2}_{K^{-}}", "#chi^{2}_{K^{-}} histogram", "#chi^{2}_{K^{-}}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_pipCD_1e_cut = Make1D(finalState, "#chi^{2}_{#pi^{+}}", "#chi^{2}_{#pi^{+}} histogram", "#chi^{2}_{#pi^{+}}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_pipFD_1e_cut = Make1D(finalState, "#chi^{2}_{#pi^{+}}", "#chi^{2}_{#pi^{+}} histogram", "#chi^{2}_{#pi^{+}}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_pimCD_1e_cut = Make1D(finalState, "#chi^{2}_{#pi^{-}}", "#chi^{2}_{#pi^{-}} histogram", "#chi^{2}_{#pi^{-}}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_pimFD_1e_cut = Make1D(finalState, "#chi^{2}_{#pi^{-}}", "#chi^{2}_{#pi^{-}} histogram", "#chi^{2}_{#pi^{-}}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_d_CD_1e_cut = Make1D(finalState, "#chi^{2}_{D}", "#chi^{2}_{D} histogram", "#chi^{2}_{D}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);
    hChi2_d_FD_1e_cut = Make1D(finalState, "#chi^{2}_{D}", "#chi^{2}_{D} histogram", "#chi^{2}_{D}", -hDefSettings.Chi2_lim, hDefSettings.Chi2_lim, numTH1Dbins);

    h_DeltaToF_pCD_VS_P_pCD_1e_cut =
        OwnHistogram(new TH2D("DeltaToF_pCD_VS_P_pCD_1e_cut", "#DeltaToF vs. P_{pCD} (1e cut);P_{pCD} [GeV/c];#DeltaToF [ns]", numTH2Dbins * 2., 0., 3., numTH2Dbins * 2., -2, 2));
    h_pCD_dist_to_first_MVT_edge_1e_cut = OwnHistogram(new TH1D("pCD_dist_to_first_MVT_edge_1e_cut", "Distance of pCD to first MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_pCD_dist_to_last_MVT_edge_1e_cut = OwnHistogram(new TH1D("pCD_dist_to_last_MVT_edge_1e_cut", "Distance of pCD to last MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_phi_pCD_VS_P_pCD_T_1e_cut = OwnHistogram(new TH2D("phi_pCD_VS_P_pCD_T_1e_cut", "#phi_{pCD} vs. P^{T}_{pCD} (1e cut);#phi_{pCD} [#circ];P^{T}_{pCD} [GeV/c]", numTH2Dbins * 2., hDefSettings.Phi_llim,
                                           hDefSettings.Phi_ulim, numTH2Dbins * 2., 0., 2.));
    h_KpCD_dist_to_first_MVT_edge_1e_cut = OwnHistogram(new TH1D("KpCD_dist_to_first_MVT_edge_1e_cut", "Distance of K^{+}CD to first MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_KpCD_dist_to_last_MVT_edge_1e_cut = OwnHistogram(new TH1D("KpCD_dist_to_last_MVT_edge_1e_cut", "Distance of K^{+}CD to last MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_phi_KpCD_VS_P_KpCD_T_1e_cut = OwnHistogram(new TH2D("phi_KpCD_VS_P_KpCD_T_1e_cut", "#phi_{K^{+}CD} vs. P^{T}_{K^{+}CD} (1e cut);#phi_{K^{+}CD} [#circ];P^{T}_{K^{+}CD} [GeV/c]", numTH2Dbins * 2.,
                                             hDefSettings.Phi_llim, hDefSettings.Phi_ulim, numTH2Dbins * 2., 0., 2.));
    h_KmCD_dist_to_first_MVT_edge_1e_cut = OwnHistogram(new TH1D("KmCD_dist_to_first_MVT_edge_1e_cut", "Distance of K^{-}CD to first MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_KmCD_dist_to_last_MVT_edge_1e_cut = OwnHistogram(new TH1D("KmCD_dist_to_last_MVT_edge_1e_cut", "Distance of K^{-}CD to last MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_phi_KmCD_VS_P_KmCD_T_1e_cut = OwnHistogram(new TH2D("phi_KmCD_VS_P_KmCD_T_1e_cut", "#phi_{K^{-}CD} vs. P^{T}_{K^{-}CD} (1e cut);#phi_{K^{-}CD} [#circ];P^{T}_{K^{-}CD} [GeV/c]", numTH2Dbins * 2.,
                                             hDefSettings.Phi_llim, hDefSettings.Phi_ulim, numTH2Dbins * 2., 0., 2.));
    h_pipCD_dist_to_first_MVT_edge_1e_cut = OwnHistogram(new TH1D("pipCD_dist_to_first_MVT_edge_1e_cut", "Distance of #pi^{+}CD to first MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_pipCD_dist_to_last_MVT_edge_1e_cut = OwnHistogram(new TH1D("pipCD_dist_to_last_MVT_edge_1e_cut", "Distance of #pi^{+}CD to last MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_phi_pipCD_VS_P_pipCD_T_1e_cut = OwnHistogram(new TH2D("phi_pipCD_VS_P_pipCD_T_1e_cut", "#phi_{#pi^{+}CD} vs. P^{T}_{#pi^{+}CD} (1e cut);#phi_{#pi^{+}CD} [#circ];P^{T}_{#pi^{+}CD} [GeV/c]",
                                               numTH2Dbins * 2., hDefSettings.Phi_llim, hDefSettings.Phi_ulim, numTH2Dbins * 2., 0., 2.));
    h_pimCD_dist_to_first_MVT_edge_1e_cut = OwnHistogram(new TH1D("pimCD_dist_to_first_MVT_edge_1e_cut", "Distance of #pi^{-}CD to first MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_pimCD_dist_to_last_MVT_edge_1e_cut = OwnHistogram(new TH1D("pimCD_dist_to_last_MVT_edge_1e_cut", "Distance of #pi^{-}CD to last MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_phi_pimCD_VS_P_pimCD_T_1e_cut = OwnHistogram(new TH2D("phi_pimCD_VS_P_pimCD_T_1e_cut", "#phi_{#pi^{-}CD} vs. P^{T}_{#pi^{-}CD} (1e cut);#phi_{#pi^{-}CD} [#circ];P^{T}_{#pi^{-}CD} [GeV/c]",
                                               numTH2Dbins * 2., hDefSettings.Phi_llim, hDefSettings.Phi_ulim, numTH2Dbins * 2., 0., 2.));
    h_d_CD_dist_to_first_MVT_edge_1e_cut = OwnHistogram(new TH1D("d_CD_dist_to_first_MVT_edge_1e_cut", "Distance of D CD to first MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_d_CD_dist_to_last_MVT_edge_1e_cut = OwnHistogram(new TH1D("d_CD_dist_to_last_MVT_edge_1e_cut", "Distance of D CD to last MTV edge (1e cut);Distance [cm]", numTH1Dbins * 2., -5., 25.));
    h_phi_d_CD_VS_P_d_CD_T_1e_cut = OwnHistogram(new TH2D("phi_d_CD_VS_P_d_CD_T_1e_cut", "#phi_{D CD} vs. P^{T}_{D CD} (1e cut);#phi_{D CD} [#circ];P^{T}_{D CD} [GeV/c]", numTH2Dbins * 2.,
                                             hDefSettings.Phi_llim, hDefSettings.Phi_ulim, numTH2Dbins * 2., 0., 2.));

    hP_e_1e_cut_FD = Make1D(finalState, "Electron momentum", "Electron momentum P_{e}", "P_{e} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_p_1e_cut_CD = Make1D(finalState, "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_p_1e_cut_FD = Make1D(finalState, "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_pipCD_1e_cut = Make1D(finalState, "#pi^{+} momentum", "#pi^{+} momentum P_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_pipFD_1e_cut = Make1D(finalState, "#pi^{+} momentum", "#pi^{+} momentum P_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_pimCD_1e_cut = Make1D(finalState, "#pi^{-} momentum", "#pi^{-} momentum P_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_pimFD_1e_cut = Make1D(finalState, "#pi^{-} momentum", "#pi^{-} momentum P_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_KpCD_1e_cut = Make1D(finalState, "K^{+} momentum", "K^{+} momentum P_{K^{+}}", "P_{K^{+}} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_KpFD_1e_cut = Make1D(finalState, "K^{+} momentum", "K^{+} momentum P_{K^{+}}", "P_{K^{+}} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_KmCD_1e_cut = Make1D(finalState, "K^{-} momentum", "K^{-} momentum P_{K^{-}}", "P_{K^{-}} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_KmFD_1e_cut = Make1D(finalState, "K^{-} momentum", "K^{-} momentum P_{K^{-}}", "P_{K^{-}} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_d_CD_1e_cut = Make1D(finalState, "Deuterons momentum", "Deuterons momentum P_{D}", "P_{D} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_d_FD_1e_cut = Make1D(finalState, "Deuterons momentum", "Deuterons momentum P_{D}", "P_{D} [GeV/c]", 0.0, beamEMax, numTH1Dbins);
    hP_LnFD_APID_1e_cut_FD =
        Make1D("1e_cut", "Leading FD neutron momentum APID", "Leading FD neutron momentum P_{n} APID", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim, numTH1Dbins);
    hP_LnFD_APID_1e_cut_ZOOMOUT_FD =
        Make1D("1e_cut", "Leading FD neutron momentum APID - ZOOMOUT", "Leading FD neutron momentum P_{n} APID - ZOOMOUT", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, 15., numTH1Dbins);
    hP_LnFD_BPID_1e_cut_FD =
        Make1D("1e_cut", "Leading FD neutron momentum BPID", "Leading FD neutron momentum P_{n} BPID", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim, numTH1Dbins);
    hP_LnFD_BPID_1e_cut_ZOOMOUT_FD =
        Make1D("1e_cut", "Leading FD neutron momentum BPID - ZOOMOUT", "Leading FD neutron momentum P_{n} BPID - ZOOMOUT", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, 15., numTH1Dbins);
    hP_LnFD_APIDandNS_1e_cut_FD = Make1D("1e_cut", "Leading FD neutron momentum APID&NC", "Leading FD neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]", hDefSettings.Momentum_llim,
                                         hDefSettings.Momentum_ulim, numTH1Dbins);
    hP_LnFD_APIDandNS_1e_cut_ZOOMOUT_FD = Make1D("1e_cut", "Leading FD neutron momentum APID&NC - ZOOMOUT", "Leading FD neutron momentum P_{n} APID&NC - ZOOMOUT", "P_{n} [GeV/c]",
                                                 hDefSettings.Momentum_llim, 15., numTH1Dbins);
    hP_nFD_APID_1e_cut_FD =
        Make1D("1e_cut", "FD neutron momentum APID", "FD neutron momentum P_{n} APID", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim, numTH1Dbins);
    hP_nFD_APID_1e_cut_ZOOMOUT_FD =
        Make1D("1e_cut", "FD neutron momentum APID - ZOOMOUT", "FD neutron momentum P_{n} APID - ZOOMOUT", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, 15., numTH1Dbins);
    hP_nFD_BPID_1e_cut_FD =
        Make1D("1e_cut", "FD neutron momentum BPID", "FD neutron momentum P_{n} BPID", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim, numTH1Dbins);
    hP_nFD_BPID_1e_cut_ZOOMOUT_FD =
        Make1D("1e_cut", "FD neutron momentum BPID - ZOOMOUT", "FD neutron momentum P_{n} BPID - ZOOMOUT", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, 15., numTH1Dbins);
    hP_nFD_APIDandNS_1e_cut_FD =
        Make1D("1e_cut", "FD neutron momentum APID&NC", "FD neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim, numTH1Dbins);
    hP_nFD_APIDandNS_1e_cut_ZOOMOUT_FD =
        Make1D("1e_cut", "FD neutron momentum APID&NC - ZOOMOUT", "FD neutron momentum P_{n} APID&NC - ZOOMOUT", "P_{n} [GeV/c]", hDefSettings.Momentum_llim, 15., numTH1Dbins);

    hP_e_reco_1e_cut_FD = Make1D(finalState, "Reco FD Electron momentum", "FD Electron momentum P^{reco}_{e}", "P^{reco}_{e} [GeV/c]", hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim,
                                 numTH1Dbins_Mom_eff_Plots);
    hP_e_reco_1e_cut_FD_ZOOMIN =
        Make1D(finalState, "Reco FD Electron momentum - ZOOMIN", "FD Electron momentum P^{reco}_{e} - ZOOMIN", "P^{reco}_{e} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_e_vs_Theta_e_reco_1e_cut_FD = Make2D(finalState, "P_{e} vs. #theta_{e}", "P_{e} vs. #theta_{e}", "P_{e} [GeV/c]", "#theta_{e} [#circ]", hDefSettings.Momentum_llim,
                                            hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_p_reco_1e_cut_FD = Make1D(finalState, "Reco FD Proton momentum", "FD Proton momentum P^{reco}_{p}", "P^{reco}_{p} [GeV/c]", hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim,
                                 numTH1Dbins_Mom_eff_Plots);
    hP_p_reco_1e_cut_CD = Make1D(finalState, "Reco CD Proton momentum", "CD Proton momentum P^{reco}_{p}", "P^{reco}_{p} [GeV/c]", hDefSettings.CDMomentum_llim, hDefSettings.CDMomentum_ulim,
                                 numTH1Dbins_Mom_eff_Plots);
    hP_p_reco_1e_cut_FD_ZOOMIN =
        Make1D(finalState, "Reco FD Proton momentum - ZOOMIN", "FD Proton momentum P^{reco}_{p} - ZOOMIN", "P^{reco}_{p} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_p_reco_1e_cut_CD_ZOOMIN =
        Make1D(finalState, "Reco CD Proton momentum - ZOOMIN", "CD Proton momentum P^{reco}_{p} - ZOOMIN", "P^{reco}_{p} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_p_vs_Theta_p_reco_1e_cut_FD = Make2D(finalState, "P_{p} vs. #theta_{p}", "P_{p} vs. #theta_{p}", "P_{p} [GeV/c]", "#theta_{p} [#circ]", hDefSettings.Momentum_llim,
                                            hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    hP_p_vs_Theta_p_reco_1e_cut_CD = Make2D(finalState, "P_{p} vs. #theta_{p}", "P_{p} vs. #theta_{p}", "P_{p} [GeV/c]", "#theta_{p} [#circ]", hDefSettings.CDMomentum_llim,
                                            hDefSettings.CDMomentum_ulim, hDefSettings.Theta_llim_CD, hDefSettings.Theta_ulim_CD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_LnFD_reco_APID_1e_cut_FD = Make1D(finalState, "Reco leading FD neutron momentum APID", "Leading FD neutron momentum P^{reco}_{n} APID", "P^{reco}_{n} [GeV/c]",
                                         hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_reco_APID_1e_cut_FD_ZOOMIN = Make1D(finalState, "Reco leading FD neutron momentum APID - ZOOMIN", "Leading FD neutron momentum P^{reco}_{n} APID - ZOOMIN",
                                                "P^{reco}_{n} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_reco_APID_1e_cut_FD_ZOOMOUT = Make1D(finalState, "Reco leading FD neutron momentum APID - ZOOMOUT", "Leading FD neutron momentum P^{reco}_{n} APID - ZOOMOUT",
                                                 "P^{reco}_{n} [GeV/c]", 0.0, 15., numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_vs_Theta_LnFD_reco_APID_1e_cut_FD =
        Make2D(finalState, "P_{LnFD} vs. #theta_{LnFD} APID", "P_{LnFD} vs. #theta_{LnFD} APID", "P_{LnFD} [GeV/c]", "#theta_{LnFD} [#circ]", hDefSettings.Momentum_llim,
               hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_LnFD_reco_BPID_1e_cut_FD = Make1D(finalState, "Reco leading FD neutron momentum BPID", "Leading FD neutron momentum P^{reco}_{n} BPID", "P^{reco}_{n} [GeV/c]",
                                         hDefSettings.Momentum_llim, hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_reco_BPID_1e_cut_FD_ZOOMIN = Make1D(finalState, "Reco leading FD neutron momentum BPID - ZOOMIN", "Leading FD neutron momentum P^{reco}_{n} BPID - ZOOMIN",
                                                "P^{reco}_{n} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_reco_BPID_1e_cut_FD_ZOOMOUT = Make1D(finalState, "Reco leading FD neutron momentum BPID - ZOOMOUT", "Leading FD neutron momentum P^{reco}_{n} BPID - ZOOMOUT",
                                                 "P^{reco}_{n} [GeV/c]", 0.0, 15., numTH1Dbins_Mom_eff_Plots);
    hP_LnFD_vs_Theta_LnFD_reco_BPID_1e_cut_FD =
        Make2D(finalState, "P_{LnFD} vs. #theta_{LnFD} BPID", "P_{LnFD} vs. #theta_{LnFD} BPID", "P_{LnFD} [GeV/c]", "#theta_{LnFD} [#circ]", hDefSettings.Momentum_llim,
               hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_nFD_reco_APID_1e_cut_FD = Make1D(finalState, "Reco FD neutrons momentum APID", "FD neutrons momentum P^{reco}_{n} APID", "P^{reco}_{n} [GeV/c]", hDefSettings.Momentum_llim,
                                        hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_nFD_reco_APID_1e_cut_FD_ZOOMIN =
        Make1D(finalState, "Reco FD neutrons momentum APID - ZOOMIN", "FD neutrons momentum P^{reco}_{n} APID - ZOOMIN", "P^{reco}_{n} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_nFD_reco_APID_1e_cut_FD_ZOOMOUT =
        Make1D(finalState, "Reco FD neutron momentum APID - ZOOMOUT", "FD neutrons momentum P^{reco}_{n} APID - ZOOMOUT", "P^{reco}_{n} [GeV/c]", 0.0, 15., numTH1Dbins_Mom_eff_Plots);
    hP_nFD_vs_Theta_nFD_reco_APID_1e_cut_FD =
        Make2D(finalState, "P_{nFD} vs. #theta_{nFD} APID", "P_{nFD} vs. #theta_{nFD} APID", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]", hDefSettings.Momentum_llim,
               hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_nFD_reco_BPID_1e_cut_FD = Make1D(finalState, "Reco FD neutrons momentum BPID", "FD neutrons momentum P^{reco}_{n} BPID", "P^{reco}_{n} [GeV/c]", hDefSettings.Momentum_llim,
                                        hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_nFD_reco_BPID_1e_cut_FD_ZOOMIN =
        Make1D(finalState, "Reco FD neutrons momentum BPID - ZOOMIN", "FD neutrons momentum P^{reco}_{n} BPID - ZOOMIN", "P^{reco}_{n} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_nFD_reco_BPID_1e_cut_FD_ZOOMOUT =
        Make1D(finalState, "Reco FD neutron momentum BPID - ZOOMOUT", "FD neutrons momentum P^{reco}_{n} BPID - ZOOMOUT", "P^{reco}_{n} [GeV/c]", 0.0, 15., numTH1Dbins_Mom_eff_Plots);
    hP_nFD_vs_Theta_nFD_reco_BPID_1e_cut_FD =
        Make2D(finalState, "P_{nFD} vs. #theta_{nFD} BPID", "P_{nFD} vs. #theta_{nFD} BPID", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]", hDefSettings.Momentum_llim,
               hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_piplus_reco_1e_cut = Make1D(finalState, "Reco #pi^{+} momentum", "#pi^{+} momentum P^{reco}_{#pi^{+}}", "P^{reco}_{#pi^{+}} [GeV/c]", hDefSettings.Momentum_llim,
                                   hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_ZOOMIN =
        Make1D(finalState, "Reco #pi^{+} momentum - ZOOMIN", "#pi^{+} momentum P^{reco}_{#pi^{+}} - ZOOMIN", "P^{reco}_{#pi^{+}} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_FD = Make1D(finalState, "Reco FD #pi^{+} momentum", "FD #pi^{+} momentum P^{reco}_{#pi^{+}}", "P^{reco}_{#pi^{+}} [GeV/c]", hDefSettings.Momentum_llim,
                                      hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_CD = Make1D(finalState, "Reco CD #pi^{+} momentum", "CD #pi^{+} momentum P^{reco}_{#pi^{+}}", "P^{reco}_{#pi^{+}} [GeV/c]", hDefSettings.CDMomentum_llim,
                                      hDefSettings.CDMomentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_FD_ZOOMIN =
        Make1D(finalState, "Reco FD #pi^{+} momentum - ZOOMIN", "FD #pi^{+} momentum P^{reco}_{#pi^{+}} - ZOOMIN", "P^{reco}_{#pi^{+}} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_reco_1e_cut_CD_ZOOMIN =
        Make1D(finalState, "Reco CD #pi^{+} momentum - ZOOMIN", "CD #pi^{+} momentum P^{reco}_{#pi^{+}} - ZOOMIN", "P^{reco}_{#pi^{+}} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piplus_vs_Theta_piplus_reco_1e_cut_FD =
        Make2D(finalState, "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]", "#theta_{#pi^{+}} [#circ]", hDefSettings.Momentum_llim,
               hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    hP_piplus_vs_Theta_piplus_reco_1e_cut_CD =
        Make2D(finalState, "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]", "#theta_{#pi^{+}} [#circ]", hDefSettings.CDMomentum_llim,
               hDefSettings.CDMomentum_ulim, hDefSettings.Theta_llim_CD, hDefSettings.Theta_ulim_CD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_piminus_reco_1e_cut = Make1D(finalState, "Reco #pi^{-} momentum", "#pi^{-} momentum P^{reco}_{#pi^{-}}", "P^{reco}_{#pi^{-}} [GeV/c]", hDefSettings.Momentum_llim,
                                    hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_ZOOMIN =
        Make1D(finalState, "Reco #pi^{-} momentum - ZOOMIN", "#pi^{-} momentum P^{reco}_{#pi^{-}} - ZOOMIN", "P^{reco}_{#pi^{-}} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_FD = Make1D(finalState, "Reco FD #pi^{-} momentum", "FD #pi^{-} momentum P^{reco}_{#pi^{-}}", "P^{reco}_{#pi^{-}} [GeV/c]", hDefSettings.Momentum_llim,
                                       hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_CD = Make1D(finalState, "Reco CD #pi^{-} momentum", "CD #pi^{-} momentum P^{reco}_{#pi^{-}}", "P^{reco}_{#pi^{-}} [GeV/c]", hDefSettings.CDMomentum_llim,
                                       hDefSettings.CDMomentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_FD_ZOOMIN =
        Make1D(finalState, "Reco FD #pi^{-} momentum - ZOOMIN", "FD #pi^{-} momentum P^{reco}_{#pi^{-}} - ZOOMIN", "P^{reco}_{#pi^{-}} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_reco_1e_cut_CD_ZOOMIN =
        Make1D(finalState, "Reco CD #pi^{-} momentum - ZOOMIN", "CD #pi^{-} momentum P^{reco}_{#pi^{-}} - ZOOMIN", "P^{reco}_{#pi^{-}} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_piminus_vs_Theta_piminus_reco_1e_cut_FD =
        Make2D(finalState, "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]", "#theta_{#pi^{-}} [#circ]", hDefSettings.Momentum_llim,
               hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    hP_piminus_vs_Theta_piminus_reco_1e_cut_CD =
        Make2D(finalState, "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]", "#theta_{#pi^{-}} [#circ]", hDefSettings.CDMomentum_llim,
               hDefSettings.CDMomentum_ulim, hDefSettings.Theta_llim_CD, hDefSettings.Theta_ulim_CD, numTH2Dbins * 2, numTH2Dbins * 2);

    hP_ph_reco_APID_1e_cut_FD = Make1D(finalState, "Reco FD #gamma momentum APID", "FD #gamma momentum P^{reco}_{#gamma} APID", "P^{reco}_{#gamma} [GeV/c]", hDefSettings.Momentum_llim,
                                       hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_ph_reco_APID_1e_cut_FD_ZOOMIN =
        Make1D(finalState, "Reco FD #gamma momentum APID - ZOOMIN", "FD #gamma momentum P^{reco}_{#gamma} APID - ZOOMIN", "P^{reco}_{#gamma} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_ph_reco_BPID_1e_cut_FD = Make1D(finalState, "Reco FD #gamma momentum BPID", "FD #gamma momentum P^{reco}_{#gamma} BPID", "P^{reco}_{#gamma} [GeV/c]", hDefSettings.Momentum_llim,
                                       hDefSettings.Momentum_ulim, numTH1Dbins_Mom_eff_Plots);
    hP_ph_reco_BPID_1e_cut_FD_ZOOMIN =
        Make1D(finalState, "Reco FD #gamma momentum BPID - ZOOMIN", "FD #gamma momentum P^{reco}_{#gamma} BPID - ZOOMIN", "P^{reco}_{#gamma} [GeV/c]", 0.0, 1, numTH1Dbins_Mom_eff_Plots);
    hP_ph_vs_Theta_ph_reco_APID_1e_cut_FD =
        Make2D(finalState, "P_{#gamma} vs. #theta_{#gamma} APID", "P_{#gamma} vs. #theta_{#gamma} APID", "P_{#gamma} [GeV/c]", "#theta_{#gamma} [#circ]", hDefSettings.Momentum_llim,
               hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    hP_ph_vs_Theta_ph_reco_BPID_1e_cut_FD =
        Make2D(finalState, "P_{#gamma} vs. #theta_{#gamma} BPID", "P_{#gamma} vs. #theta_{#gamma} BPID", "P_{#gamma} [GeV/c]", "#theta_{#gamma} [#circ]", hDefSettings.Momentum_llim,
               hDefSettings.Momentum_ulim, hDefSettings.Theta_llim_FD, hDefSettings.Theta_ulim_FD, numTH2Dbins * 2, numTH2Dbins * 2);
}

void FinalStateHistograms::InitReactionMonitoringHistograms(const HistogramDefinitionSettings& hDefSettings) {
    const int numTH1Dbins_ReacMon_plots = hDefSettings.numTH1Dbins_ReacMon_plots;
    const int numTH2Dbins_ReacMon_plots = hDefSettings.numTH2Dbins_ReacMon_plots;
    const double P_miss_llim = hDefSettings.P_miss_llim;
    const double P_miss_ulim = hDefSettings.P_miss_ulim;
    const double E_miss_llim = hDefSettings.E_miss_llim;
    const double E_miss_ulim = hDefSettings.E_miss_ulim;
    const double Q2_llim_FD = hDefSettings.Q2_llim_FD;
    const double Q2_ulim_FD = hDefSettings.Q2_ulim_FD;
    const double xB_llim = hDefSettings.xB_llim;
    const double xB_ulim = hDefSettings.xB_ulim;
    const double theta_q_llim = hDefSettings.theta_q_llim;
    const double theta_q_ulim = hDefSettings.theta_q_ulim;
    sP_miss_1N_pFDpCD = OwnHistogram(new THStack("P^{1N}_{miss} distribution (All Int., pFDpCD)", "P^{1N}_{miss} distribution (pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];"));
    hP_miss_1N_All_Int_pFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (All Int., pFDpCD)", "P^{1N}_{miss} distribution (All Int., pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_QEL_pFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (QEL only, pFDpCD)", "P^{1N}_{miss} distribution (QEL only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_MEC_pFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (MEC only, pFDpCD)", "P^{1N}_{miss} distribution (MEC only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_RES_pFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (RES only, pFDpCD)", "P^{1N}_{miss} distribution (RES only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_DIS_pFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (DIS only, pFDpCD)", "P^{1N}_{miss} distribution (DIS only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_All_Int_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (All Int., pFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (All Int., pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_QEL_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (QEL only, pFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (QEL only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_MEC_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (MEC only, pFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (MEC only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_RES_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (RES only, pFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (RES only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_DIS_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (DIS only, pFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (DIS only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_All_Int_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (All Int., pFDpCD)",
                                                      "P^{1N}_{miss} vs. P^{2N}_{miss} (All Int., pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                      "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_QEL_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (QEL only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. P^{2N}_{miss} (QEL only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_MEC_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (MEC only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. P^{2N}_{miss} (MEC only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_RES_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (RES only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. P^{2N}_{miss} (RES only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_DIS_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (DIS only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. P^{2N}_{miss} (DIS only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_All_Int_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (All Int., pFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (All Int., pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_QEL_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (QEL only, pFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (QEL only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_MEC_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (MEC only, pFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (MEC only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_RES_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (RES only, pFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (RES only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_DIS_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (DIS only, pFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (DIS only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_Q2_All_Int_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (All Int., pFDpCD)", "P^{1N}_{miss} vs. Q^{2} (All Int., pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_Q2_QEL_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (QEL only, pFDpCD)", "P^{1N}_{miss} vs. Q^{2} (QEL only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_Q2_MEC_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (MEC only, pFDpCD)", "P^{1N}_{miss} vs. Q^{2} (MEC only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_Q2_RES_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (RES only, pFDpCD)", "P^{1N}_{miss} vs. Q^{2} (RES only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_Q2_DIS_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (DIS only, pFDpCD)", "P^{1N}_{miss} vs. Q^{2} (DIS only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_xB_All_Int_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (All Int., pFDpCD)", "P^{1N}_{miss} vs. x_{B} (All Int., pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_xB_QEL_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (QEL only, pFDpCD)", "P^{1N}_{miss} vs. x_{B} (QEL only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_xB_MEC_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (MEC only, pFDpCD)", "P^{1N}_{miss} vs. x_{B} (MEC only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_xB_RES_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (RES only, pFDpCD)", "P^{1N}_{miss} vs. x_{B} (RES only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_xB_DIS_pFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (DIS only, pFDpCD)", "P^{1N}_{miss} vs. x_{B} (DIS only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_theta_q_All_Int_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (All Int., pFDpCD)",
                                                    "P^{1N}_{miss} vs. #theta_{q} (All Int., pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                    numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_QEL_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (QEL only, pFDpCD)",
                                                "P^{1N}_{miss} vs. #theta_{q} (QEL only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_MEC_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (MEC only, pFDpCD)",
                                                "P^{1N}_{miss} vs. #theta_{q} (MEC only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_rES_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (RES only, pFDpCD)",
                                                "P^{1N}_{miss} vs. #theta_{q} (RES only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_DIS_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (DIS only, pFDpCD)",
                                                "P^{1N}_{miss} vs. #theta_{q} (DIS only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_All_Int_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD)",
                                                      "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{pFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_QEL_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_MEC_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_RES_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_DIS_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_All_Int_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD)",
                                                      "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{pFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_QEL_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_MEC_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_RES_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_DIS_pFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    sE_miss_1N_pFDpCD = OwnHistogram(new THStack("E^{1N}_{miss} distribution (All Int., pFDpCD)", "E^{1N}_{miss} distribution (pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];"));
    hE_miss_1N_All_Int_pFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (All Int., pFDpCD)", "E^{1N}_{miss} distribution (All Int., pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                         numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_QEL_pFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (QEL only, pFDpCD)", "E^{1N}_{miss} distribution (QEL only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_MEC_pFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (MEC only, pFDpCD)", "E^{1N}_{miss} distribution (MEC only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_RES_pFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (RES only, pFDpCD)", "E^{1N}_{miss} distribution (RES only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_DIS_pFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (DIS only, pFDpCD)", "E^{1N}_{miss} distribution (DIS only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_All_Int_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (All Int., pFDpCD)",
                                                      "E^{1N}_{miss} vs. P^{2N}_{miss} (All Int., pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                      "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_QEL_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (QEL only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. P^{2N}_{miss} (QEL only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_MEC_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (MEC only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. P^{2N}_{miss} (MEC only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_RES_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (RES only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. P^{2N}_{miss} (RES only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_DIS_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (DIS only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. P^{2N}_{miss} (DIS only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_All_Int_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (All Int., pFDpCD)",
                                                      "E^{1N}_{miss} vs. E^{2N}_{miss} (All Int., pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_QEL_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (QEL only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. E^{2N}_{miss} (QEL only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_MEC_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (MEC only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. E^{2N}_{miss} (MEC only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_RES_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (RES only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. E^{2N}_{miss} (RES only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_DIS_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (DIS only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. E^{2N}_{miss} (DIS only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_Q2_All_Int_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (All Int., pFDpCD)", "E^{1N}_{miss} vs. Q^{2} (All Int., pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_Q2_QEL_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (QEL only, pFDpCD)", "E^{1N}_{miss} vs. Q^{2} (QEL only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_Q2_MEC_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (MEC only, pFDpCD)", "E^{1N}_{miss} vs. Q^{2} (MEC only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_Q2_RES_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (RES only, pFDpCD)", "E^{1N}_{miss} vs. Q^{2} (RES only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_Q2_DIS_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (DIS only, pFDpCD)", "E^{1N}_{miss} vs. Q^{2} (DIS only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_xB_All_Int_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (All Int., pFDpCD)", "E^{1N}_{miss} vs. x_{B} (All Int., pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                               numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_xB_QEL_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (QEL only, pFDpCD)", "E^{1N}_{miss} vs. x_{B} (QEL only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_xB_MEC_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (MEC only, pFDpCD)", "E^{1N}_{miss} vs. x_{B} (MEC only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_xB_RES_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (RES only, pFDpCD)", "E^{1N}_{miss} vs. x_{B} (RES only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_xB_DIS_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (DIS only, pFDpCD)", "E^{1N}_{miss} vs. x_{B} (DIS only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_theta_q_All_Int_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (All Int., pFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (All Int., pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_QEL_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (QEL only, pFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (QEL only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_MEC_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (MEC only, pFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (MEC only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_rES_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (RES only, pFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (RES only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_DIS_pFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (DIS only, pFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (DIS only, pFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_All_Int_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD)",
                                                      "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_QEL_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_MEC_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_RES_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_DIS_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_All_Int_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD)",
                                                      "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_QEL_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_MEC_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_RES_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_DIS_pFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD);E^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    sP_miss_2N_pFDpCD =
        OwnHistogram(new THStack("P^{2N}_{miss} distribution (All Int., pFDpCD)", "P^{2N}_{miss} distribution (pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];"));
    hP_miss_2N_All_Int_pFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (All Int., pFDpCD)",
                                         "P^{2N}_{miss} distribution (All Int., pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];",
                                         numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_2N_QEL_pFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (QEL only, pFDpCD)",
                                     "P^{2N}_{miss} distribution (QEL only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];", numTH1Dbins_ReacMon_plots,
                                     P_miss_llim, P_miss_ulim));
    hP_miss_2N_MEC_pFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (MEC only, pFDpCD)",
                                     "P^{2N}_{miss} distribution (MEC only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];", numTH1Dbins_ReacMon_plots,
                                     P_miss_llim, P_miss_ulim));
    hP_miss_2N_RES_pFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (RES only, pFDpCD)",
                                     "P^{2N}_{miss} distribution (RES only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];", numTH1Dbins_ReacMon_plots,
                                     P_miss_llim, P_miss_ulim));
    hP_miss_2N_DIS_pFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (DIS only, pFDpCD)",
                                     "P^{2N}_{miss} distribution (DIS only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];", numTH1Dbins_ReacMon_plots,
                                     P_miss_llim, P_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_All_Int_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (All Int., pFDpCD)",
                                                      "P^{2N}_{miss} vs. E^{2N}_{miss} (All Int., pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                      "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_QEL_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (QEL only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. E^{2N}_{miss} (QEL only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                  "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_MEC_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (MEC only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. E^{2N}_{miss} (MEC only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                  "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_RES_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (RES only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. E^{2N}_{miss} (RES only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                  "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_DIS_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (DIS only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. E^{2N}_{miss} (DIS only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                  "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_Q2_All_Int_pFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (All Int., pFDpCD)",
                 "P^{2N}_{miss} vs. Q^{2} (All Int., pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_Q2_QEL_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (QEL only, pFDpCD)",
                                           "P^{2N}_{miss} vs. Q^{2} (QEL only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_Q2_MEC_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (MEC only, pFDpCD)",
                                           "P^{2N}_{miss} vs. Q^{2} (MEC only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_Q2_RES_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (RES only, pFDpCD)",
                                           "P^{2N}_{miss} vs. Q^{2} (RES only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_Q2_DIS_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (DIS only, pFDpCD)",
                                           "P^{2N}_{miss} vs. Q^{2} (DIS only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_xB_All_Int_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (All Int., pFDpCD)",
                                               "P^{2N}_{miss} vs. x_{B} (All Int., pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                               numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_xB_QEL_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (QEL only, pFDpCD)",
                                           "P^{2N}_{miss} vs. x_{B} (QEL only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_xB_MEC_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (MEC only, pFDpCD)",
                                           "P^{2N}_{miss} vs. x_{B} (MEC only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_xB_RES_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (RES only, pFDpCD)",
                                           "P^{2N}_{miss} vs. x_{B} (RES only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_xB_DIS_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (DIS only, pFDpCD)",
                                           "P^{2N}_{miss} vs. x_{B} (DIS only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_theta_q_All_Int_pFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (All Int., pFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (All Int., pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_QEL_pFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (QEL only, pFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (QEL only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_MEC_pFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (MEC only, pFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (MEC only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_rES_pFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (RES only, pFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (RES only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_DIS_pFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (DIS only, pFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (DIS only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_All_Int_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD)",
                                                      "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_QEL_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_MEC_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_RES_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_DIS_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_All_Int_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD)",
                                                      "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_QEL_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_MEC_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_RES_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_DIS_pFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD);P^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    sE_miss_2N_pFDpCD = OwnHistogram(new THStack("E^{2N}_{miss} distribution (All Int., pFDpCD)", "E^{2N}_{miss} distribution (pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];"));
    hE_miss_2N_All_Int_pFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (All Int., pFDpCD)", "E^{2N}_{miss} distribution (All Int., pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                         numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_QEL_pFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (QEL only, pFDpCD)", "E^{2N}_{miss} distribution (QEL only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_MEC_pFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (MEC only, pFDpCD)", "E^{2N}_{miss} distribution (MEC only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_RES_pFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (RES only, pFDpCD)", "E^{2N}_{miss} distribution (RES only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_DIS_pFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (DIS only, pFDpCD)", "E^{2N}_{miss} distribution (DIS only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_VS_Q2_All_Int_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (All Int., pFDpCD)", "E^{2N}_{miss} vs. Q^{2} (All Int., pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_Q2_QEL_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (QEL only, pFDpCD)", "E^{2N}_{miss} vs. Q^{2} (QEL only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_Q2_MEC_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (MEC only, pFDpCD)", "E^{2N}_{miss} vs. Q^{2} (MEC only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_Q2_RES_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (RES only, pFDpCD)", "E^{2N}_{miss} vs. Q^{2} (RES only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_Q2_DIS_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (DIS only, pFDpCD)", "E^{2N}_{miss} vs. Q^{2} (DIS only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_xB_All_Int_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (All Int., pFDpCD)", "E^{2N}_{miss} vs. x_{B} (All Int., pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                               numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_xB_QEL_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (QEL only, pFDpCD)", "E^{2N}_{miss} vs. x_{B} (QEL only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_xB_MEC_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (MEC only, pFDpCD)", "E^{2N}_{miss} vs. x_{B} (MEC only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_xB_RES_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (RES only, pFDpCD)", "E^{2N}_{miss} vs. x_{B} (RES only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_xB_DIS_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (DIS only, pFDpCD)", "E^{2N}_{miss} vs. x_{B} (DIS only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_theta_q_All_Int_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (All Int., pFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (All Int., pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_QEL_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (QEL only, pFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (QEL only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_MEC_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (MEC only, pFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (MEC only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_rES_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (RES only, pFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (RES only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_DIS_pFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (DIS only, pFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (DIS only, pFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_All_Int_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD)",
                                                      "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_QEL_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_MEC_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_RES_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_DIS_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_All_Int_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD)",
                                                      "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_QEL_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_MEC_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_RES_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_DIS_pFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD);E^{2N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_xB_All_Int_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (All Int., pFDpCD)", "Q^{2} vs. x_{B} (All Int., pFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD,
                                        Q2_ulim_FD, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_xB_QEL_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (QEL only, pFDpCD)", "Q^{2} vs. x_{B} (QEL only, pFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD,
                                    numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_xB_MEC_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (MEC only, pFDpCD)", "Q^{2} vs. x_{B} (MEC only, pFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD,
                                    numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_xB_RES_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (RES only, pFDpCD)", "Q^{2} vs. x_{B} (RES only, pFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD,
                                    numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_xB_DIS_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (DIS only, pFDpCD)", "Q^{2} vs. x_{B} (DIS only, pFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD,
                                    numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_theta_q_All_Int_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (All Int., pFDpCD)", "Q^{2} vs. #theta_{q} (All Int., pFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                             numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_QEL_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (QEL only, pFDpCD)", "Q^{2} vs. #theta_{q} (QEL only, pFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                         numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_MEC_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (MEC only, pFDpCD)", "Q^{2} vs. #theta_{q} (MEC only, pFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                         numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_rES_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (RES only, pFDpCD)", "Q^{2} vs. #theta_{q} (RES only, pFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                         numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_DIS_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (DIS only, pFDpCD)", "Q^{2} vs. #theta_{q} (DIS only, pFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                         numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_All_Int_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD)",
                                               "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - "
                                               "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                               numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_QEL_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_MEC_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_RES_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_DIS_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_All_Int_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD)",
                                               "Q^{2} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - "
                                               "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                               numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_QEL_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_MEC_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_RES_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_DIS_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD);Q^{2} = |#font[62]{P}_{pFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    sxB_pFDpCD = OwnHistogram(new THStack("x_{B} distribution (All Int., pFDpCD)", "x_{B} distribution (pFDpCD);x_{B};"));
    hxB_All_Int_pFDpCD = OwnHistogram(new TH1D("x_{B} distribution (All Int., pFDpCD)", "x_{B} distribution (All Int., pFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_QEL_pFDpCD = OwnHistogram(new TH1D("x_{B} distribution (QEL only, pFDpCD)", "x_{B} distribution (QEL only, pFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_MEC_pFDpCD = OwnHistogram(new TH1D("x_{B} distribution (MEC only, pFDpCD)", "x_{B} distribution (MEC only, pFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_RES_pFDpCD = OwnHistogram(new TH1D("x_{B} distribution (RES only, pFDpCD)", "x_{B} distribution (RES only, pFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_DIS_pFDpCD = OwnHistogram(new TH1D("x_{B} distribution (DIS only, pFDpCD)", "x_{B} distribution (DIS only, pFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_VS_theta_q_All_Int_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (All Int., pFDpCD)", "x_{B} vs. #theta_{q} (All Int., pFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                                             xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_QEL_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (QEL only, pFDpCD)", "x_{B} vs. #theta_{q} (QEL only, pFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots, xB_llim,
                                         xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_MEC_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (MEC only, pFDpCD)", "x_{B} vs. #theta_{q} (MEC only, pFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots, xB_llim,
                                         xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_rES_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (RES only, pFDpCD)", "x_{B} vs. #theta_{q} (RES only, pFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots, xB_llim,
                                         xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_DIS_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (DIS only, pFDpCD)", "x_{B} vs. #theta_{q} (DIS only, pFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots, xB_llim,
                                         xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_All_Int_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD)",
                                               "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                               numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_QEL_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_MEC_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_RES_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_DIS_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_All_Int_pFDpCD =
        OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD)",
                 "x_{B} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]", numTH2Dbins_ReacMon_plots,
                 xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_QEL_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_MEC_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_RES_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_DIS_pFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    stheta_q_pFDpCD = OwnHistogram(new THStack("#theta_{q} distribution (All Int., pFDpCD)", "#theta_{q} distribution (pFDpCD);#theta_{q} [#circ];"));
    htheta_q_All_Int_pFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (All Int., pFDpCD)", "#theta_{q} distribution (All Int., pFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_QEL_pFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (QEL only, pFDpCD)", "#theta_{q} distribution (QEL only, pFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_MEC_pFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (MEC only, pFDpCD)", "#theta_{q} distribution (MEC only, pFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_rES_pFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (RES only, pFDpCD)", "#theta_{q} distribution (RES only, pFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_DIS_pFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (DIS only, pFDpCD)", "#theta_{q} distribution (DIS only, pFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_All_Int_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_QEL_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (QEL only, pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_MEC_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (MEC only, pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_RES_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (RES only, pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_DIS_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{pFD}} (DIS only, pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_All_Int_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (All Int., pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_QEL_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_MEC_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_RES_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_DIS_pFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    stheta_q_N_pFDpCD = OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (All Int., pFDpCD)",
                                    "#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];"));
    htheta_q_N_All_Int_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (All Int., pFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (All Int., pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];",
                                         numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));  // NOTE: the same plot is being filled in the angles section!
    htheta_q_N_QEL_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (QEL only, pFDpCD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (QEL only, pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];", numTH1Dbins_ReacMon_plots,
                                     theta_q_llim, theta_q_ulim));
    htheta_q_N_MEC_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (MEC only, pFDpCD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (MEC only, pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];", numTH1Dbins_ReacMon_plots,
                                     theta_q_llim, theta_q_ulim));
    htheta_q_N_RES_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (RES only, pFDpCD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (RES only, pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];", numTH1Dbins_ReacMon_plots,
                                     theta_q_llim, theta_q_ulim));
    htheta_q_N_DIS_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (DIS only, pFDpCD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{pFD}} distribution (DIS only, pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];", numTH1Dbins_ReacMon_plots,
                                     theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_All_Int_pFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} ReacMon (All Int., pFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} (All Int., "
                 "pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_QEL_pFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} (QEL only, pFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} (QEL only, "
                 "pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_MEC_pFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} (MEC only, pFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} (MEC only, "
                 "pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_RES_pFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} (RES only, pFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} (RES only, "
                 "pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_DIS_pFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} (DIS only, pFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},r} (DIS only, "
                 "pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    stheta_q_r_pFDpCD = OwnHistogram(new THStack("#theta_{#font[62]{q},r} distribution (All Int., pFDpCD)",
                                    "#theta_{#font[62]{q},r} distribution (pFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];"));
    htheta_q_r_All_Int_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (All Int., pFDpCD)",
                                         "#theta_{#font[62]{q},r} distribution (All Int., pFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                         numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));  // NOTE: the same plot is being filled in the angles section!
    htheta_q_r_QEL_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (QEL only, pFDpCD)",
                                     "#theta_{#font[62]{q},r} distribution (QEL only, pFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                     numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_r_MEC_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (MEC only, pFDpCD)",
                                     "#theta_{#font[62]{q},r} distribution (MEC only, pFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                     numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_r_RES_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (RES only, pFDpCD)",
                                     "#theta_{#font[62]{q},r} distribution (RES only, pFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                     numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_r_DIS_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (DIS only, pFDpCD)",
                                     "#theta_{#font[62]{q},r} distribution (DIS only, pFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                     numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    sP_miss_1N_nFDpCD = OwnHistogram(new THStack("P^{1N}_{miss} distribution (All Int., nFDpCD)", "P^{1N}_{miss} distribution (nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];"));
    hP_miss_1N_All_Int_nFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (All Int., nFDpCD)", "P^{1N}_{miss} distribution (All Int., nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_QEL_nFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (QEL only, nFDpCD)", "P^{1N}_{miss} distribution (QEL only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_MEC_nFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (MEC only, nFDpCD)", "P^{1N}_{miss} distribution (MEC only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_RES_nFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (RES only, nFDpCD)", "P^{1N}_{miss} distribution (RES only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_DIS_nFDpCD =
        OwnHistogram(new TH1D("P^{1N}_{miss} distribution (DIS only, nFDpCD)", "P^{1N}_{miss} distribution (DIS only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];",
                 numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_All_Int_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (All Int., nFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (All Int., nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_QEL_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (QEL only, nFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (QEL only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_MEC_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (MEC only, nFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (MEC only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_RES_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (RES only, nFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (RES only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_1N_DIS_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{1N}_{miss} (DIS only, nFDpCD)",
                 "P^{1N}_{miss} vs. E^{1N}_{miss} (DIS only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{1N}_{miss} = #omega - T_{L} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_All_Int_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (All Int., nFDpCD)",
                                                      "P^{1N}_{miss} vs. P^{2N}_{miss} (All Int., nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                      "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_QEL_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (QEL only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. P^{2N}_{miss} (QEL only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_MEC_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (MEC only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. P^{2N}_{miss} (MEC only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_RES_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (RES only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. P^{2N}_{miss} (RES only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_P_miss_2N_DIS_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. P^{2N}_{miss} (DIS only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. P^{2N}_{miss} (DIS only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_All_Int_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (All Int., nFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (All Int., nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_QEL_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (QEL only, nFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (QEL only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_MEC_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (MEC only, nFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (MEC only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_RES_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (RES only, nFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (RES only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_E_miss_2N_DIS_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. E^{2N}_{miss} (DIS only, nFDpCD)",
                 "P^{1N}_{miss} vs. E^{2N}_{miss} (DIS only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_1N_VS_Q2_All_Int_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (All Int., nFDpCD)", "P^{1N}_{miss} vs. Q^{2} (All Int., nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_Q2_QEL_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (QEL only, nFDpCD)", "P^{1N}_{miss} vs. Q^{2} (QEL only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_Q2_MEC_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (MEC only, nFDpCD)", "P^{1N}_{miss} vs. Q^{2} (MEC only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_Q2_RES_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (RES only, nFDpCD)", "P^{1N}_{miss} vs. Q^{2} (RES only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_Q2_DIS_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. Q^{2} (DIS only, nFDpCD)", "P^{1N}_{miss} vs. Q^{2} (DIS only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_1N_VS_xB_All_Int_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (All Int., nFDpCD)", "P^{1N}_{miss} vs. x_{B} (All Int., nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_xB_QEL_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (QEL only, nFDpCD)", "P^{1N}_{miss} vs. x_{B} (QEL only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_xB_MEC_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (MEC only, nFDpCD)", "P^{1N}_{miss} vs. x_{B} (MEC only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_xB_RES_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (RES only, nFDpCD)", "P^{1N}_{miss} vs. x_{B} (RES only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_xB_DIS_nFDpCD =
        OwnHistogram(new TH2D("P^{1N}_{miss} vs. x_{B} (DIS only, nFDpCD)", "P^{1N}_{miss} vs. x_{B} (DIS only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];x_{B}",
                 numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_1N_VS_theta_q_All_Int_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (All Int., nFDpCD)",
                                                    "P^{1N}_{miss} vs. #theta_{q} (All Int., nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                    numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_QEL_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (QEL only, nFDpCD)",
                                                "P^{1N}_{miss} vs. #theta_{q} (QEL only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_MEC_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (MEC only, nFDpCD)",
                                                "P^{1N}_{miss} vs. #theta_{q} (MEC only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_rES_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (RES only, nFDpCD)",
                                                "P^{1N}_{miss} vs. #theta_{q} (RES only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_DIS_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{q} (DIS only, nFDpCD)",
                                                "P^{1N}_{miss} vs. #theta_{q} (DIS only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]",
                                                numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_All_Int_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD)",
                                                      "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{nFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_QEL_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_MEC_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_RES_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_N_DIS_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_All_Int_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD)",
                                                      "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{nFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_QEL_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_MEC_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_RES_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_1N_VS_theta_q_r_DIS_nFDpCD = OwnHistogram(new TH2D("P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD)",
                                                  "P^{1N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD);P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    sE_miss_1N_nFDpCD = OwnHistogram(new THStack("E^{1N}_{miss} distribution (All Int., nFDpCD)", "E^{1N}_{miss} distribution (nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];"));
    hE_miss_1N_All_Int_nFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (All Int., nFDpCD)", "E^{1N}_{miss} distribution (All Int., nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                         numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_QEL_nFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (QEL only, nFDpCD)", "E^{1N}_{miss} distribution (QEL only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_MEC_nFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (MEC only, nFDpCD)", "E^{1N}_{miss} distribution (MEC only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_RES_nFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (RES only, nFDpCD)", "E^{1N}_{miss} distribution (RES only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_DIS_nFDpCD = OwnHistogram(new TH1D("E^{1N}_{miss} distribution (DIS only, nFDpCD)", "E^{1N}_{miss} distribution (DIS only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_All_Int_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (All Int., nFDpCD)",
                                                      "E^{1N}_{miss} vs. P^{2N}_{miss} (All Int., nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                      "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_QEL_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (QEL only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. P^{2N}_{miss} (QEL only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_MEC_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (MEC only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. P^{2N}_{miss} (MEC only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_RES_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (RES only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. P^{2N}_{miss} (RES only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_P_miss_2N_DIS_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. P^{2N}_{miss} (DIS only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. P^{2N}_{miss} (DIS only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];P^{2N}_{miss} = "
                                                  "|#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_All_Int_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (All Int., nFDpCD)",
                                                      "E^{1N}_{miss} vs. E^{2N}_{miss} (All Int., nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_QEL_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (QEL only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. E^{2N}_{miss} (QEL only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_MEC_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (MEC only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. E^{2N}_{miss} (MEC only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_RES_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (RES only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. E^{2N}_{miss} (RES only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_E_miss_2N_DIS_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. E^{2N}_{miss} (DIS only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. E^{2N}_{miss} (DIS only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_1N_VS_Q2_All_Int_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (All Int., nFDpCD)", "E^{1N}_{miss} vs. Q^{2} (All Int., nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_Q2_QEL_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (QEL only, nFDpCD)", "E^{1N}_{miss} vs. Q^{2} (QEL only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_Q2_MEC_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (MEC only, nFDpCD)", "E^{1N}_{miss} vs. Q^{2} (MEC only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_Q2_RES_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (RES only, nFDpCD)", "E^{1N}_{miss} vs. Q^{2} (RES only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_Q2_DIS_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. Q^{2} (DIS only, nFDpCD)", "E^{1N}_{miss} vs. Q^{2} (DIS only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_1N_VS_xB_All_Int_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (All Int., nFDpCD)", "E^{1N}_{miss} vs. x_{B} (All Int., nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                               numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_xB_QEL_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (QEL only, nFDpCD)", "E^{1N}_{miss} vs. x_{B} (QEL only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_xB_MEC_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (MEC only, nFDpCD)", "E^{1N}_{miss} vs. x_{B} (MEC only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_xB_RES_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (RES only, nFDpCD)", "E^{1N}_{miss} vs. x_{B} (RES only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_xB_DIS_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. x_{B} (DIS only, nFDpCD)", "E^{1N}_{miss} vs. x_{B} (DIS only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_1N_VS_theta_q_All_Int_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (All Int., nFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (All Int., nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_QEL_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (QEL only, nFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (QEL only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_MEC_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (MEC only, nFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (MEC only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_rES_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (RES only, nFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (RES only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_DIS_nFDpCD =
        OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{q} (DIS only, nFDpCD)", "E^{1N}_{miss} vs. #theta_{q} (DIS only, nFDpCD);E^{1N}_{miss} = #omega - T_{L} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_All_Int_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD)",
                                                      "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_QEL_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_MEC_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_RES_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_N_DIS_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_All_Int_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD)",
                                                      "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_QEL_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_MEC_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_RES_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_1N_VS_theta_q_r_DIS_nFDpCD = OwnHistogram(new TH2D("E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD)",
                                                  "E^{1N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD);E^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    sP_miss_2N_nFDpCD =
        OwnHistogram(new THStack("P^{2N}_{miss} distribution (All Int., nFDpCD)", "P^{2N}_{miss} distribution (nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];"));
    hP_miss_2N_All_Int_nFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (All Int., nFDpCD)",
                                         "P^{2N}_{miss} distribution (All Int., nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];",
                                         numTH1Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim));
    hP_miss_2N_QEL_nFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (QEL only, nFDpCD)",
                                     "P^{2N}_{miss} distribution (QEL only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];", numTH1Dbins_ReacMon_plots,
                                     P_miss_llim, P_miss_ulim));
    hP_miss_2N_MEC_nFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (MEC only, nFDpCD)",
                                     "P^{2N}_{miss} distribution (MEC only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];", numTH1Dbins_ReacMon_plots,
                                     P_miss_llim, P_miss_ulim));
    hP_miss_2N_RES_nFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (RES only, nFDpCD)",
                                     "P^{2N}_{miss} distribution (RES only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];", numTH1Dbins_ReacMon_plots,
                                     P_miss_llim, P_miss_ulim));
    hP_miss_2N_DIS_nFDpCD = OwnHistogram(new TH1D("P^{2N}_{miss} distribution (DIS only, nFDpCD)",
                                     "P^{2N}_{miss} distribution (DIS only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];", numTH1Dbins_ReacMon_plots,
                                     P_miss_llim, P_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_All_Int_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (All Int., nFDpCD)",
                                                      "P^{2N}_{miss} vs. E^{2N}_{miss} (All Int., nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                      "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_QEL_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (QEL only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. E^{2N}_{miss} (QEL only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                  "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_MEC_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (MEC only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. E^{2N}_{miss} (MEC only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                  "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_RES_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (RES only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. E^{2N}_{miss} (RES only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                  "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_E_miss_2N_DIS_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. E^{2N}_{miss} (DIS only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. E^{2N}_{miss} (DIS only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| "
                                                  "[GeV/c];E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hP_miss_2N_VS_Q2_All_Int_nFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (All Int., nFDpCD)",
                 "P^{2N}_{miss} vs. Q^{2} (All Int., nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_Q2_QEL_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (QEL only, nFDpCD)",
                                           "P^{2N}_{miss} vs. Q^{2} (QEL only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_Q2_MEC_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (MEC only, nFDpCD)",
                                           "P^{2N}_{miss} vs. Q^{2} (MEC only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_Q2_RES_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (RES only, nFDpCD)",
                                           "P^{2N}_{miss} vs. Q^{2} (RES only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_Q2_DIS_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. Q^{2} (DIS only, nFDpCD)",
                                           "P^{2N}_{miss} vs. Q^{2} (DIS only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hP_miss_2N_VS_xB_All_Int_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (All Int., nFDpCD)",
                                               "P^{2N}_{miss} vs. x_{B} (All Int., nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                               numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_xB_QEL_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (QEL only, nFDpCD)",
                                           "P^{2N}_{miss} vs. x_{B} (QEL only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_xB_MEC_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (MEC only, nFDpCD)",
                                           "P^{2N}_{miss} vs. x_{B} (MEC only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_xB_RES_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (RES only, nFDpCD)",
                                           "P^{2N}_{miss} vs. x_{B} (RES only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_xB_DIS_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. x_{B} (DIS only, nFDpCD)",
                                           "P^{2N}_{miss} vs. x_{B} (DIS only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];x_{B}",
                                           numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hP_miss_2N_VS_theta_q_All_Int_nFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (All Int., nFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (All Int., nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_QEL_nFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (QEL only, nFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (QEL only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_MEC_nFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (MEC only, nFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (MEC only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_rES_nFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (RES only, nFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (RES only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_DIS_nFDpCD =
        OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{q} (DIS only, nFDpCD)",
                 "P^{2N}_{miss} vs. #theta_{q} (DIS only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{L} + #font[62]{P}_{R} - #font[62]{q}| [GeV/c];#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                 P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_All_Int_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD)",
                                                      "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_QEL_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_MEC_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_RES_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_N_DIS_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_All_Int_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD)",
                                                      "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_QEL_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_MEC_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_RES_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hP_miss_2N_VS_theta_q_r_DIS_nFDpCD = OwnHistogram(new TH2D("P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD)",
                                                  "P^{2N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD);P^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, P_miss_llim, P_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    sE_miss_2N_nFDpCD = OwnHistogram(new THStack("E^{2N}_{miss} distribution (All Int., nFDpCD)", "E^{2N}_{miss} distribution (nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];"));
    hE_miss_2N_All_Int_nFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (All Int., nFDpCD)", "E^{2N}_{miss} distribution (All Int., nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                         numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_QEL_nFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (QEL only, nFDpCD)", "E^{2N}_{miss} distribution (QEL only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_MEC_nFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (MEC only, nFDpCD)", "E^{2N}_{miss} distribution (MEC only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_RES_nFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (RES only, nFDpCD)", "E^{2N}_{miss} distribution (RES only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_DIS_nFDpCD = OwnHistogram(new TH1D("E^{2N}_{miss} distribution (DIS only, nFDpCD)", "E^{2N}_{miss} distribution (DIS only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];",
                                     numTH1Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim));
    hE_miss_2N_VS_Q2_All_Int_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (All Int., nFDpCD)", "E^{2N}_{miss} vs. Q^{2} (All Int., nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_Q2_QEL_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (QEL only, nFDpCD)", "E^{2N}_{miss} vs. Q^{2} (QEL only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_Q2_MEC_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (MEC only, nFDpCD)", "E^{2N}_{miss} vs. Q^{2} (MEC only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_Q2_RES_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (RES only, nFDpCD)", "E^{2N}_{miss} vs. Q^{2} (RES only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_Q2_DIS_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. Q^{2} (DIS only, nFDpCD)", "E^{2N}_{miss} vs. Q^{2} (DIS only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];Q^{2} [GeV^{2}/c^{2}]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD));
    hE_miss_2N_VS_xB_All_Int_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (All Int., nFDpCD)", "E^{2N}_{miss} vs. x_{B} (All Int., nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                               numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_xB_QEL_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (QEL only, nFDpCD)", "E^{2N}_{miss} vs. x_{B} (QEL only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_xB_MEC_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (MEC only, nFDpCD)", "E^{2N}_{miss} vs. x_{B} (MEC only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_xB_RES_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (RES only, nFDpCD)", "E^{2N}_{miss} vs. x_{B} (RES only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_xB_DIS_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. x_{B} (DIS only, nFDpCD)", "E^{2N}_{miss} vs. x_{B} (DIS only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];x_{B}",
                                           numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hE_miss_2N_VS_theta_q_All_Int_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (All Int., nFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (All Int., nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_QEL_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (QEL only, nFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (QEL only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_MEC_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (MEC only, nFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (MEC only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_rES_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (RES only, nFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (RES only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_DIS_nFDpCD =
        OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{q} (DIS only, nFDpCD)", "E^{2N}_{miss} vs. #theta_{q} (DIS only, nFDpCD);E^{2N}_{miss} = #omega - T_{L} - T_{R} [GeV];#theta_{q} [#circ]",
                 numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_All_Int_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD)",
                                                      "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_QEL_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_MEC_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_RES_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_N_DIS_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_All_Int_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD)",
                                                      "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - "
                                                      "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                      numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_QEL_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_MEC_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_RES_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hE_miss_2N_VS_theta_q_r_DIS_nFDpCD = OwnHistogram(new TH2D("E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD)",
                                                  "E^{2N}_{miss} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD);E^{2N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                                  "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                                  numTH2Dbins_ReacMon_plots, E_miss_llim, E_miss_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_xB_All_Int_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (All Int., nFDpCD)", "Q^{2} vs. x_{B} (All Int., nFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD,
                                        Q2_ulim_FD, numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_xB_QEL_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (QEL only, nFDpCD)", "Q^{2} vs. x_{B} (QEL only, nFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD,
                                    numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_xB_MEC_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (MEC only, nFDpCD)", "Q^{2} vs. x_{B} (MEC only, nFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD,
                                    numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_xB_RES_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (RES only, nFDpCD)", "Q^{2} vs. x_{B} (RES only, nFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD,
                                    numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_xB_DIS_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. x_{B} (DIS only, nFDpCD)", "Q^{2} vs. x_{B} (DIS only, nFDpCD);Q^{2} [GeV^{2}/c^{2}];x_{B}", numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD,
                                    numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hQ2_VS_theta_q_All_Int_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (All Int., nFDpCD)", "Q^{2} vs. #theta_{q} (All Int., nFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                             numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_QEL_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (QEL only, nFDpCD)", "Q^{2} vs. #theta_{q} (QEL only, nFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                         numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_MEC_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (MEC only, nFDpCD)", "Q^{2} vs. #theta_{q} (MEC only, nFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                         numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_rES_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (RES only, nFDpCD)", "Q^{2} vs. #theta_{q} (RES only, nFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                         numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_DIS_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{q} (DIS only, nFDpCD)", "Q^{2} vs. #theta_{q} (DIS only, nFDpCD);Q^{2} [GeV^{2}/c^{2}];#theta_{q} [#circ]",
                                         numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_All_Int_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD)",
                                               "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - "
                                               "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                               numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_QEL_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_MEC_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_RES_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_N_DIS_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_All_Int_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD)",
                                               "Q^{2} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - "
                                               "#font[62]{q}| [GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                               numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_QEL_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_MEC_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_RES_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hQ2_VS_theta_q_r_DIS_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD)",
                                           "Q^{2} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD);Q^{2} = |#font[62]{P}_{nFD} - #font[62]{q}| "
                                           "[GeV/c];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    sxB_nFDpCD = OwnHistogram(new THStack("x_{B} distribution (All Int., nFDpCD)", "x_{B} distribution (nFDpCD);x_{B};"));
    hxB_All_Int_nFDpCD = OwnHistogram(new TH1D("x_{B} distribution (All Int., nFDpCD)", "x_{B} distribution (All Int., nFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_QEL_nFDpCD = OwnHistogram(new TH1D("x_{B} distribution (QEL only, nFDpCD)", "x_{B} distribution (QEL only, nFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_MEC_nFDpCD = OwnHistogram(new TH1D("x_{B} distribution (MEC only, nFDpCD)", "x_{B} distribution (MEC only, nFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_RES_nFDpCD = OwnHistogram(new TH1D("x_{B} distribution (RES only, nFDpCD)", "x_{B} distribution (RES only, nFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_DIS_nFDpCD = OwnHistogram(new TH1D("x_{B} distribution (DIS only, nFDpCD)", "x_{B} distribution (DIS only, nFDpCD);x_{B};", numTH1Dbins_ReacMon_plots, xB_llim, xB_ulim));
    hxB_VS_theta_q_All_Int_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (All Int., nFDpCD)", "x_{B} vs. #theta_{q} (All Int., nFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots,
                                             xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_QEL_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (QEL only, nFDpCD)", "x_{B} vs. #theta_{q} (QEL only, nFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots, xB_llim,
                                         xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_MEC_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (MEC only, nFDpCD)", "x_{B} vs. #theta_{q} (MEC only, nFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots, xB_llim,
                                         xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_rES_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (RES only, nFDpCD)", "x_{B} vs. #theta_{q} (RES only, nFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots, xB_llim,
                                         xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_DIS_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{q} (DIS only, nFDpCD)", "x_{B} vs. #theta_{q} (DIS only, nFDpCD);x_{B};#theta_{q} [#circ]", numTH2Dbins_ReacMon_plots, xB_llim,
                                         xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_All_Int_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD)",
                                               "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                               numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_QEL_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_MEC_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_RES_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_N_DIS_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD);x_{B};#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_All_Int_nFDpCD =
        OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD)",
                 "x_{B} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]", numTH2Dbins_ReacMon_plots,
                 xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_QEL_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_MEC_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_RES_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    hxB_VS_theta_q_r_DIS_nFDpCD = OwnHistogram(new TH2D("x_{B} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD)",
                                           "x_{B} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD);x_{B};#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                                           numTH2Dbins_ReacMon_plots, xB_llim, xB_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    stheta_q_nFDpCD = OwnHistogram(new THStack("#theta_{q} distribution (All Int., nFDpCD)", "#theta_{q} distribution (nFDpCD);#theta_{q} [#circ];"));
    htheta_q_All_Int_nFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (All Int., nFDpCD)", "#theta_{q} distribution (All Int., nFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_QEL_nFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (QEL only, nFDpCD)", "#theta_{q} distribution (QEL only, nFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_MEC_nFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (MEC only, nFDpCD)", "#theta_{q} distribution (MEC only, nFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_rES_nFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (RES only, nFDpCD)", "#theta_{q} distribution (RES only, nFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_DIS_nFDpCD =
        OwnHistogram(new TH1D("#theta_{q} distribution (DIS only, nFDpCD)", "#theta_{q} distribution (DIS only, nFDpCD);#theta_{q} [#circ];", numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_All_Int_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_QEL_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (QEL only, nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_MEC_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (MEC only, nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_RES_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (RES only, nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_N_DIS_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},#font[62]{P}_{nFD}} (DIS only, nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ]", numTH2Dbins_ReacMon_plots,
                 theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_All_Int_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (All Int., nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_QEL_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_MEC_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_RES_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_VS_theta_q_r_DIS_nFDpCD =
        OwnHistogram(new TH2D("#theta_{q} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD)",
                 "#theta_{q} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD);#theta_{q} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    stheta_q_N_nFDpCD = OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (All Int., nFDpCD)",
                                    "#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];"));
    htheta_q_N_All_Int_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (All Int., nFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (All Int., nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];",
                                         numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));  // NOTE: the same plot is being filled in the angles section!
    htheta_q_N_QEL_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (QEL only, nFDpCD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (QEL only, nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];", numTH1Dbins_ReacMon_plots,
                                     theta_q_llim, theta_q_ulim));
    htheta_q_N_MEC_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (MEC only, nFDpCD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (MEC only, nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];", numTH1Dbins_ReacMon_plots,
                                     theta_q_llim, theta_q_ulim));
    htheta_q_N_RES_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (RES only, nFDpCD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (RES only, nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];", numTH1Dbins_ReacMon_plots,
                                     theta_q_llim, theta_q_ulim));
    htheta_q_N_DIS_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (DIS only, nFDpCD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{nFD}} distribution (DIS only, nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];", numTH1Dbins_ReacMon_plots,
                                     theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_All_Int_nFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} ReacMon (All Int., nFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} (All Int., "
                 "nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_QEL_nFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} (QEL only, nFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} (QEL only, "
                 "nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_MEC_nFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} (MEC only, nFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} (MEC only, "
                 "nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_RES_nFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} (RES only, nFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} (RES only, "
                 "nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_N_VS_theta_q_r_DIS_nFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} (DIS only, nFDpCD)",
                 "#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},r} (DIS only, "
                 "nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{L}} [#circ];#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ]",
                 numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim, numTH2Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    stheta_q_r_nFDpCD = OwnHistogram(new THStack("#theta_{#font[62]{q},r} distribution (All Int., nFDpCD)",
                                    "#theta_{#font[62]{q},r} distribution (nFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];"));
    htheta_q_r_All_Int_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (All Int., nFDpCD)",
                                         "#theta_{#font[62]{q},r} distribution (All Int., nFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                         numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));  // NOTE: the same plot is being filled in the angles section!
    htheta_q_r_QEL_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (QEL only, nFDpCD)",
                                     "#theta_{#font[62]{q},r} distribution (QEL only, nFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                     numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_r_MEC_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (MEC only, nFDpCD)",
                                     "#theta_{#font[62]{q},r} distribution (MEC only, nFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                     numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_r_RES_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (RES only, nFDpCD)",
                                     "#theta_{#font[62]{q},r} distribution (RES only, nFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                     numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));
    htheta_q_r_DIS_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},r} distribution (DIS only, nFDpCD)",
                                     "#theta_{#font[62]{q},r} distribution (DIS only, nFDpCD);#theta_{#font[62]{q},r} = #theta_{#font[62]{q},(- #font[62]{P}^{1N}_{miss})} [#circ];",
                                     numTH1Dbins_ReacMon_plots, theta_q_llim, theta_q_ulim));

    BindReactionHistograms(hP_miss_1N_pFDpCD_by_reaction, sP_miss_1N_pFDpCD, hP_miss_1N_All_Int_pFDpCD, hP_miss_1N_QEL_pFDpCD, hP_miss_1N_MEC_pFDpCD, hP_miss_1N_RES_pFDpCD,
                           hP_miss_1N_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_1N_pFDpCD_by_reaction, sE_miss_1N_pFDpCD, hE_miss_1N_All_Int_pFDpCD, hE_miss_1N_QEL_pFDpCD, hE_miss_1N_MEC_pFDpCD, hE_miss_1N_RES_pFDpCD,
                           hE_miss_1N_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_2N_pFDpCD_by_reaction, sP_miss_2N_pFDpCD, hP_miss_2N_All_Int_pFDpCD, hP_miss_2N_QEL_pFDpCD, hP_miss_2N_MEC_pFDpCD, hP_miss_2N_RES_pFDpCD,
                           hP_miss_2N_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_2N_pFDpCD_by_reaction, sE_miss_2N_pFDpCD, hE_miss_2N_All_Int_pFDpCD, hE_miss_2N_QEL_pFDpCD, hE_miss_2N_MEC_pFDpCD, hE_miss_2N_RES_pFDpCD,
                           hE_miss_2N_DIS_pFDpCD);
    BindReactionHistograms(hxB_pFDpCD_by_reaction, sxB_pFDpCD, hxB_All_Int_pFDpCD, hxB_QEL_pFDpCD, hxB_MEC_pFDpCD, hxB_RES_pFDpCD, hxB_DIS_pFDpCD);
    BindReactionHistograms(htheta_q_pFDpCD_by_reaction, stheta_q_pFDpCD, htheta_q_All_Int_pFDpCD, htheta_q_QEL_pFDpCD, htheta_q_MEC_pFDpCD, htheta_q_rES_pFDpCD, htheta_q_DIS_pFDpCD);
    BindReactionHistograms(htheta_q_N_pFDpCD_by_reaction, stheta_q_N_pFDpCD, htheta_q_N_All_Int_pFDpCD, htheta_q_N_QEL_pFDpCD, htheta_q_N_MEC_pFDpCD, htheta_q_N_RES_pFDpCD,
                           htheta_q_N_DIS_pFDpCD);
    BindReactionHistograms(htheta_q_r_pFDpCD_by_reaction, stheta_q_r_pFDpCD, htheta_q_r_All_Int_pFDpCD, htheta_q_r_QEL_pFDpCD, htheta_q_r_MEC_pFDpCD, htheta_q_r_RES_pFDpCD,
                           htheta_q_r_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_1N_nFDpCD_by_reaction, sP_miss_1N_nFDpCD, hP_miss_1N_All_Int_nFDpCD, hP_miss_1N_QEL_nFDpCD, hP_miss_1N_MEC_nFDpCD, hP_miss_1N_RES_nFDpCD,
                           hP_miss_1N_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_1N_nFDpCD_by_reaction, sE_miss_1N_nFDpCD, hE_miss_1N_All_Int_nFDpCD, hE_miss_1N_QEL_nFDpCD, hE_miss_1N_MEC_nFDpCD, hE_miss_1N_RES_nFDpCD,
                           hE_miss_1N_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_2N_nFDpCD_by_reaction, sP_miss_2N_nFDpCD, hP_miss_2N_All_Int_nFDpCD, hP_miss_2N_QEL_nFDpCD, hP_miss_2N_MEC_nFDpCD, hP_miss_2N_RES_nFDpCD,
                           hP_miss_2N_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_2N_nFDpCD_by_reaction, sE_miss_2N_nFDpCD, hE_miss_2N_All_Int_nFDpCD, hE_miss_2N_QEL_nFDpCD, hE_miss_2N_MEC_nFDpCD, hE_miss_2N_RES_nFDpCD,
                           hE_miss_2N_DIS_nFDpCD);
    BindReactionHistograms(hxB_nFDpCD_by_reaction, sxB_nFDpCD, hxB_All_Int_nFDpCD, hxB_QEL_nFDpCD, hxB_MEC_nFDpCD, hxB_RES_nFDpCD, hxB_DIS_nFDpCD);
    BindReactionHistograms(htheta_q_nFDpCD_by_reaction, stheta_q_nFDpCD, htheta_q_All_Int_nFDpCD, htheta_q_QEL_nFDpCD, htheta_q_MEC_nFDpCD, htheta_q_rES_nFDpCD, htheta_q_DIS_nFDpCD);
    BindReactionHistograms(htheta_q_N_nFDpCD_by_reaction, stheta_q_N_nFDpCD, htheta_q_N_All_Int_nFDpCD, htheta_q_N_QEL_nFDpCD, htheta_q_N_MEC_nFDpCD, htheta_q_N_RES_nFDpCD,
                           htheta_q_N_DIS_nFDpCD);
    BindReactionHistograms(htheta_q_r_nFDpCD_by_reaction, stheta_q_r_nFDpCD, htheta_q_r_All_Int_nFDpCD, htheta_q_r_QEL_nFDpCD, htheta_q_r_MEC_nFDpCD, htheta_q_r_RES_nFDpCD,
                           htheta_q_r_DIS_nFDpCD);

    BindReactionHistograms(hP_miss_1N_VS_E_miss_1N_pFDpCD_by_reaction, hP_miss_1N_VS_E_miss_1N_All_Int_pFDpCD, hP_miss_1N_VS_E_miss_1N_QEL_pFDpCD, hP_miss_1N_VS_E_miss_1N_MEC_pFDpCD,
                           hP_miss_1N_VS_E_miss_1N_RES_pFDpCD, hP_miss_1N_VS_E_miss_1N_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_P_miss_2N_pFDpCD_by_reaction, hP_miss_1N_VS_P_miss_2N_All_Int_pFDpCD, hP_miss_1N_VS_P_miss_2N_QEL_pFDpCD, hP_miss_1N_VS_P_miss_2N_MEC_pFDpCD,
                           hP_miss_1N_VS_P_miss_2N_RES_pFDpCD, hP_miss_1N_VS_P_miss_2N_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_E_miss_2N_pFDpCD_by_reaction, hP_miss_1N_VS_E_miss_2N_All_Int_pFDpCD, hP_miss_1N_VS_E_miss_2N_QEL_pFDpCD, hP_miss_1N_VS_E_miss_2N_MEC_pFDpCD,
                           hP_miss_1N_VS_E_miss_2N_RES_pFDpCD, hP_miss_1N_VS_E_miss_2N_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_Q2_pFDpCD_by_reaction, hP_miss_1N_VS_Q2_All_Int_pFDpCD, hP_miss_1N_VS_Q2_QEL_pFDpCD, hP_miss_1N_VS_Q2_MEC_pFDpCD, hP_miss_1N_VS_Q2_RES_pFDpCD,
                           hP_miss_1N_VS_Q2_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_xB_pFDpCD_by_reaction, hP_miss_1N_VS_xB_All_Int_pFDpCD, hP_miss_1N_VS_xB_QEL_pFDpCD, hP_miss_1N_VS_xB_MEC_pFDpCD, hP_miss_1N_VS_xB_RES_pFDpCD,
                           hP_miss_1N_VS_xB_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_theta_q_pFDpCD_by_reaction, hP_miss_1N_VS_theta_q_All_Int_pFDpCD, hP_miss_1N_VS_theta_q_QEL_pFDpCD, hP_miss_1N_VS_theta_q_MEC_pFDpCD,
                           hP_miss_1N_VS_theta_q_rES_pFDpCD, hP_miss_1N_VS_theta_q_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_theta_q_N_pFDpCD_by_reaction, hP_miss_1N_VS_theta_q_N_All_Int_pFDpCD, hP_miss_1N_VS_theta_q_N_QEL_pFDpCD, hP_miss_1N_VS_theta_q_N_MEC_pFDpCD,
                           hP_miss_1N_VS_theta_q_N_RES_pFDpCD, hP_miss_1N_VS_theta_q_N_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_theta_q_r_pFDpCD_by_reaction, hP_miss_1N_VS_theta_q_r_All_Int_pFDpCD, hP_miss_1N_VS_theta_q_r_QEL_pFDpCD, hP_miss_1N_VS_theta_q_r_MEC_pFDpCD,
                           hP_miss_1N_VS_theta_q_r_RES_pFDpCD, hP_miss_1N_VS_theta_q_r_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_P_miss_2N_pFDpCD_by_reaction, hE_miss_1N_VS_P_miss_2N_All_Int_pFDpCD, hE_miss_1N_VS_P_miss_2N_QEL_pFDpCD, hE_miss_1N_VS_P_miss_2N_MEC_pFDpCD,
                           hE_miss_1N_VS_P_miss_2N_RES_pFDpCD, hE_miss_1N_VS_P_miss_2N_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_E_miss_2N_pFDpCD_by_reaction, hE_miss_1N_VS_E_miss_2N_All_Int_pFDpCD, hE_miss_1N_VS_E_miss_2N_QEL_pFDpCD, hE_miss_1N_VS_E_miss_2N_MEC_pFDpCD,
                           hE_miss_1N_VS_E_miss_2N_RES_pFDpCD, hE_miss_1N_VS_E_miss_2N_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_Q2_pFDpCD_by_reaction, hE_miss_1N_VS_Q2_All_Int_pFDpCD, hE_miss_1N_VS_Q2_QEL_pFDpCD, hE_miss_1N_VS_Q2_MEC_pFDpCD, hE_miss_1N_VS_Q2_RES_pFDpCD,
                           hE_miss_1N_VS_Q2_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_xB_pFDpCD_by_reaction, hE_miss_1N_VS_xB_All_Int_pFDpCD, hE_miss_1N_VS_xB_QEL_pFDpCD, hE_miss_1N_VS_xB_MEC_pFDpCD, hE_miss_1N_VS_xB_RES_pFDpCD,
                           hE_miss_1N_VS_xB_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_theta_q_pFDpCD_by_reaction, hE_miss_1N_VS_theta_q_All_Int_pFDpCD, hE_miss_1N_VS_theta_q_QEL_pFDpCD, hE_miss_1N_VS_theta_q_MEC_pFDpCD,
                           hE_miss_1N_VS_theta_q_rES_pFDpCD, hE_miss_1N_VS_theta_q_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_theta_q_N_pFDpCD_by_reaction, hE_miss_1N_VS_theta_q_N_All_Int_pFDpCD, hE_miss_1N_VS_theta_q_N_QEL_pFDpCD, hE_miss_1N_VS_theta_q_N_MEC_pFDpCD,
                           hE_miss_1N_VS_theta_q_N_RES_pFDpCD, hE_miss_1N_VS_theta_q_N_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_theta_q_r_pFDpCD_by_reaction, hE_miss_1N_VS_theta_q_r_All_Int_pFDpCD, hE_miss_1N_VS_theta_q_r_QEL_pFDpCD, hE_miss_1N_VS_theta_q_r_MEC_pFDpCD,
                           hE_miss_1N_VS_theta_q_r_RES_pFDpCD, hE_miss_1N_VS_theta_q_r_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_E_miss_2N_pFDpCD_by_reaction, hP_miss_2N_VS_E_miss_2N_All_Int_pFDpCD, hP_miss_2N_VS_E_miss_2N_QEL_pFDpCD, hP_miss_2N_VS_E_miss_2N_MEC_pFDpCD,
                           hP_miss_2N_VS_E_miss_2N_RES_pFDpCD, hP_miss_2N_VS_E_miss_2N_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_Q2_pFDpCD_by_reaction, hP_miss_2N_VS_Q2_All_Int_pFDpCD, hP_miss_2N_VS_Q2_QEL_pFDpCD, hP_miss_2N_VS_Q2_MEC_pFDpCD, hP_miss_2N_VS_Q2_RES_pFDpCD,
                           hP_miss_2N_VS_Q2_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_xB_pFDpCD_by_reaction, hP_miss_2N_VS_xB_All_Int_pFDpCD, hP_miss_2N_VS_xB_QEL_pFDpCD, hP_miss_2N_VS_xB_MEC_pFDpCD, hP_miss_2N_VS_xB_RES_pFDpCD,
                           hP_miss_2N_VS_xB_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_theta_q_pFDpCD_by_reaction, hP_miss_2N_VS_theta_q_All_Int_pFDpCD, hP_miss_2N_VS_theta_q_QEL_pFDpCD, hP_miss_2N_VS_theta_q_MEC_pFDpCD,
                           hP_miss_2N_VS_theta_q_rES_pFDpCD, hP_miss_2N_VS_theta_q_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_theta_q_N_pFDpCD_by_reaction, hP_miss_2N_VS_theta_q_N_All_Int_pFDpCD, hP_miss_2N_VS_theta_q_N_QEL_pFDpCD, hP_miss_2N_VS_theta_q_N_MEC_pFDpCD,
                           hP_miss_2N_VS_theta_q_N_RES_pFDpCD, hP_miss_2N_VS_theta_q_N_DIS_pFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_theta_q_r_pFDpCD_by_reaction, hP_miss_2N_VS_theta_q_r_All_Int_pFDpCD, hP_miss_2N_VS_theta_q_r_QEL_pFDpCD, hP_miss_2N_VS_theta_q_r_MEC_pFDpCD,
                           hP_miss_2N_VS_theta_q_r_RES_pFDpCD, hP_miss_2N_VS_theta_q_r_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_Q2_pFDpCD_by_reaction, hE_miss_2N_VS_Q2_All_Int_pFDpCD, hE_miss_2N_VS_Q2_QEL_pFDpCD, hE_miss_2N_VS_Q2_MEC_pFDpCD, hE_miss_2N_VS_Q2_RES_pFDpCD,
                           hE_miss_2N_VS_Q2_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_xB_pFDpCD_by_reaction, hE_miss_2N_VS_xB_All_Int_pFDpCD, hE_miss_2N_VS_xB_QEL_pFDpCD, hE_miss_2N_VS_xB_MEC_pFDpCD, hE_miss_2N_VS_xB_RES_pFDpCD,
                           hE_miss_2N_VS_xB_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_theta_q_pFDpCD_by_reaction, hE_miss_2N_VS_theta_q_All_Int_pFDpCD, hE_miss_2N_VS_theta_q_QEL_pFDpCD, hE_miss_2N_VS_theta_q_MEC_pFDpCD,
                           hE_miss_2N_VS_theta_q_rES_pFDpCD, hE_miss_2N_VS_theta_q_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_theta_q_N_pFDpCD_by_reaction, hE_miss_2N_VS_theta_q_N_All_Int_pFDpCD, hE_miss_2N_VS_theta_q_N_QEL_pFDpCD, hE_miss_2N_VS_theta_q_N_MEC_pFDpCD,
                           hE_miss_2N_VS_theta_q_N_RES_pFDpCD, hE_miss_2N_VS_theta_q_N_DIS_pFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_theta_q_r_pFDpCD_by_reaction, hE_miss_2N_VS_theta_q_r_All_Int_pFDpCD, hE_miss_2N_VS_theta_q_r_QEL_pFDpCD, hE_miss_2N_VS_theta_q_r_MEC_pFDpCD,
                           hE_miss_2N_VS_theta_q_r_RES_pFDpCD, hE_miss_2N_VS_theta_q_r_DIS_pFDpCD);
    BindReactionHistograms(hQ2_VS_xB_pFDpCD_by_reaction, hQ2_VS_xB_All_Int_pFDpCD, hQ2_VS_xB_QEL_pFDpCD, hQ2_VS_xB_MEC_pFDpCD, hQ2_VS_xB_RES_pFDpCD, hQ2_VS_xB_DIS_pFDpCD);
    BindReactionHistograms(hQ2_VS_theta_q_pFDpCD_by_reaction, hQ2_VS_theta_q_All_Int_pFDpCD, hQ2_VS_theta_q_QEL_pFDpCD, hQ2_VS_theta_q_MEC_pFDpCD, hQ2_VS_theta_q_rES_pFDpCD,
                           hQ2_VS_theta_q_DIS_pFDpCD);
    BindReactionHistograms(hQ2_VS_theta_q_N_pFDpCD_by_reaction, hQ2_VS_theta_q_N_All_Int_pFDpCD, hQ2_VS_theta_q_N_QEL_pFDpCD, hQ2_VS_theta_q_N_MEC_pFDpCD, hQ2_VS_theta_q_N_RES_pFDpCD,
                           hQ2_VS_theta_q_N_DIS_pFDpCD);
    BindReactionHistograms(hQ2_VS_theta_q_r_pFDpCD_by_reaction, hQ2_VS_theta_q_r_All_Int_pFDpCD, hQ2_VS_theta_q_r_QEL_pFDpCD, hQ2_VS_theta_q_r_MEC_pFDpCD, hQ2_VS_theta_q_r_RES_pFDpCD,
                           hQ2_VS_theta_q_r_DIS_pFDpCD);
    BindReactionHistograms(hxB_VS_theta_q_pFDpCD_by_reaction, hxB_VS_theta_q_All_Int_pFDpCD, hxB_VS_theta_q_QEL_pFDpCD, hxB_VS_theta_q_MEC_pFDpCD, hxB_VS_theta_q_rES_pFDpCD,
                           hxB_VS_theta_q_DIS_pFDpCD);
    BindReactionHistograms(hxB_VS_theta_q_N_pFDpCD_by_reaction, hxB_VS_theta_q_N_All_Int_pFDpCD, hxB_VS_theta_q_N_QEL_pFDpCD, hxB_VS_theta_q_N_MEC_pFDpCD, hxB_VS_theta_q_N_RES_pFDpCD,
                           hxB_VS_theta_q_N_DIS_pFDpCD);
    BindReactionHistograms(hxB_VS_theta_q_r_pFDpCD_by_reaction, hxB_VS_theta_q_r_All_Int_pFDpCD, hxB_VS_theta_q_r_QEL_pFDpCD, hxB_VS_theta_q_r_MEC_pFDpCD, hxB_VS_theta_q_r_RES_pFDpCD,
                           hxB_VS_theta_q_r_DIS_pFDpCD);
    BindReactionHistograms(htheta_q_VS_theta_q_N_pFDpCD_by_reaction, htheta_q_VS_theta_q_N_All_Int_pFDpCD, htheta_q_VS_theta_q_N_QEL_pFDpCD, htheta_q_VS_theta_q_N_MEC_pFDpCD,
                           htheta_q_VS_theta_q_N_RES_pFDpCD, htheta_q_VS_theta_q_N_DIS_pFDpCD);
    BindReactionHistograms(htheta_q_VS_theta_q_r_pFDpCD_by_reaction, htheta_q_VS_theta_q_r_All_Int_pFDpCD, htheta_q_VS_theta_q_r_QEL_pFDpCD, htheta_q_VS_theta_q_r_MEC_pFDpCD,
                           htheta_q_VS_theta_q_r_RES_pFDpCD, htheta_q_VS_theta_q_r_DIS_pFDpCD);
    BindReactionHistograms(htheta_q_N_VS_theta_q_r_pFDpCD_by_reaction, htheta_q_N_VS_theta_q_r_All_Int_pFDpCD, htheta_q_N_VS_theta_q_r_QEL_pFDpCD, htheta_q_N_VS_theta_q_r_MEC_pFDpCD,
                           htheta_q_N_VS_theta_q_r_RES_pFDpCD, htheta_q_N_VS_theta_q_r_DIS_pFDpCD);

    BindReactionHistograms(hP_miss_1N_VS_E_miss_1N_nFDpCD_by_reaction, hP_miss_1N_VS_E_miss_1N_All_Int_nFDpCD, hP_miss_1N_VS_E_miss_1N_QEL_nFDpCD, hP_miss_1N_VS_E_miss_1N_MEC_nFDpCD,
                           hP_miss_1N_VS_E_miss_1N_RES_nFDpCD, hP_miss_1N_VS_E_miss_1N_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_P_miss_2N_nFDpCD_by_reaction, hP_miss_1N_VS_P_miss_2N_All_Int_nFDpCD, hP_miss_1N_VS_P_miss_2N_QEL_nFDpCD, hP_miss_1N_VS_P_miss_2N_MEC_nFDpCD,
                           hP_miss_1N_VS_P_miss_2N_RES_nFDpCD, hP_miss_1N_VS_P_miss_2N_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_E_miss_2N_nFDpCD_by_reaction, hP_miss_1N_VS_E_miss_2N_All_Int_nFDpCD, hP_miss_1N_VS_E_miss_2N_QEL_nFDpCD, hP_miss_1N_VS_E_miss_2N_MEC_nFDpCD,
                           hP_miss_1N_VS_E_miss_2N_RES_nFDpCD, hP_miss_1N_VS_E_miss_2N_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_Q2_nFDpCD_by_reaction, hP_miss_1N_VS_Q2_All_Int_nFDpCD, hP_miss_1N_VS_Q2_QEL_nFDpCD, hP_miss_1N_VS_Q2_MEC_nFDpCD, hP_miss_1N_VS_Q2_RES_nFDpCD,
                           hP_miss_1N_VS_Q2_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_xB_nFDpCD_by_reaction, hP_miss_1N_VS_xB_All_Int_nFDpCD, hP_miss_1N_VS_xB_QEL_nFDpCD, hP_miss_1N_VS_xB_MEC_nFDpCD, hP_miss_1N_VS_xB_RES_nFDpCD,
                           hP_miss_1N_VS_xB_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_theta_q_nFDpCD_by_reaction, hP_miss_1N_VS_theta_q_All_Int_nFDpCD, hP_miss_1N_VS_theta_q_QEL_nFDpCD, hP_miss_1N_VS_theta_q_MEC_nFDpCD,
                           hP_miss_1N_VS_theta_q_rES_nFDpCD, hP_miss_1N_VS_theta_q_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_theta_q_N_nFDpCD_by_reaction, hP_miss_1N_VS_theta_q_N_All_Int_nFDpCD, hP_miss_1N_VS_theta_q_N_QEL_nFDpCD, hP_miss_1N_VS_theta_q_N_MEC_nFDpCD,
                           hP_miss_1N_VS_theta_q_N_RES_nFDpCD, hP_miss_1N_VS_theta_q_N_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_1N_VS_theta_q_r_nFDpCD_by_reaction, hP_miss_1N_VS_theta_q_r_All_Int_nFDpCD, hP_miss_1N_VS_theta_q_r_QEL_nFDpCD, hP_miss_1N_VS_theta_q_r_MEC_nFDpCD,
                           hP_miss_1N_VS_theta_q_r_RES_nFDpCD, hP_miss_1N_VS_theta_q_r_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_P_miss_2N_nFDpCD_by_reaction, hE_miss_1N_VS_P_miss_2N_All_Int_nFDpCD, hE_miss_1N_VS_P_miss_2N_QEL_nFDpCD, hE_miss_1N_VS_P_miss_2N_MEC_nFDpCD,
                           hE_miss_1N_VS_P_miss_2N_RES_nFDpCD, hE_miss_1N_VS_P_miss_2N_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_E_miss_2N_nFDpCD_by_reaction, hE_miss_1N_VS_E_miss_2N_All_Int_nFDpCD, hE_miss_1N_VS_E_miss_2N_QEL_nFDpCD, hE_miss_1N_VS_E_miss_2N_MEC_nFDpCD,
                           hE_miss_1N_VS_E_miss_2N_RES_nFDpCD, hE_miss_1N_VS_E_miss_2N_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_Q2_nFDpCD_by_reaction, hE_miss_1N_VS_Q2_All_Int_nFDpCD, hE_miss_1N_VS_Q2_QEL_nFDpCD, hE_miss_1N_VS_Q2_MEC_nFDpCD, hE_miss_1N_VS_Q2_RES_nFDpCD,
                           hE_miss_1N_VS_Q2_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_xB_nFDpCD_by_reaction, hE_miss_1N_VS_xB_All_Int_nFDpCD, hE_miss_1N_VS_xB_QEL_nFDpCD, hE_miss_1N_VS_xB_MEC_nFDpCD, hE_miss_1N_VS_xB_RES_nFDpCD,
                           hE_miss_1N_VS_xB_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_theta_q_nFDpCD_by_reaction, hE_miss_1N_VS_theta_q_All_Int_nFDpCD, hE_miss_1N_VS_theta_q_QEL_nFDpCD, hE_miss_1N_VS_theta_q_MEC_nFDpCD,
                           hE_miss_1N_VS_theta_q_rES_nFDpCD, hE_miss_1N_VS_theta_q_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_theta_q_N_nFDpCD_by_reaction, hE_miss_1N_VS_theta_q_N_All_Int_nFDpCD, hE_miss_1N_VS_theta_q_N_QEL_nFDpCD, hE_miss_1N_VS_theta_q_N_MEC_nFDpCD,
                           hE_miss_1N_VS_theta_q_N_RES_nFDpCD, hE_miss_1N_VS_theta_q_N_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_1N_VS_theta_q_r_nFDpCD_by_reaction, hE_miss_1N_VS_theta_q_r_All_Int_nFDpCD, hE_miss_1N_VS_theta_q_r_QEL_nFDpCD, hE_miss_1N_VS_theta_q_r_MEC_nFDpCD,
                           hE_miss_1N_VS_theta_q_r_RES_nFDpCD, hE_miss_1N_VS_theta_q_r_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_E_miss_2N_nFDpCD_by_reaction, hP_miss_2N_VS_E_miss_2N_All_Int_nFDpCD, hP_miss_2N_VS_E_miss_2N_QEL_nFDpCD, hP_miss_2N_VS_E_miss_2N_MEC_nFDpCD,
                           hP_miss_2N_VS_E_miss_2N_RES_nFDpCD, hP_miss_2N_VS_E_miss_2N_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_Q2_nFDpCD_by_reaction, hP_miss_2N_VS_Q2_All_Int_nFDpCD, hP_miss_2N_VS_Q2_QEL_nFDpCD, hP_miss_2N_VS_Q2_MEC_nFDpCD, hP_miss_2N_VS_Q2_RES_nFDpCD,
                           hP_miss_2N_VS_Q2_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_xB_nFDpCD_by_reaction, hP_miss_2N_VS_xB_All_Int_nFDpCD, hP_miss_2N_VS_xB_QEL_nFDpCD, hP_miss_2N_VS_xB_MEC_nFDpCD, hP_miss_2N_VS_xB_RES_nFDpCD,
                           hP_miss_2N_VS_xB_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_theta_q_nFDpCD_by_reaction, hP_miss_2N_VS_theta_q_All_Int_nFDpCD, hP_miss_2N_VS_theta_q_QEL_nFDpCD, hP_miss_2N_VS_theta_q_MEC_nFDpCD,
                           hP_miss_2N_VS_theta_q_rES_nFDpCD, hP_miss_2N_VS_theta_q_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_theta_q_N_nFDpCD_by_reaction, hP_miss_2N_VS_theta_q_N_All_Int_nFDpCD, hP_miss_2N_VS_theta_q_N_QEL_nFDpCD, hP_miss_2N_VS_theta_q_N_MEC_nFDpCD,
                           hP_miss_2N_VS_theta_q_N_RES_nFDpCD, hP_miss_2N_VS_theta_q_N_DIS_nFDpCD);
    BindReactionHistograms(hP_miss_2N_VS_theta_q_r_nFDpCD_by_reaction, hP_miss_2N_VS_theta_q_r_All_Int_nFDpCD, hP_miss_2N_VS_theta_q_r_QEL_nFDpCD, hP_miss_2N_VS_theta_q_r_MEC_nFDpCD,
                           hP_miss_2N_VS_theta_q_r_RES_nFDpCD, hP_miss_2N_VS_theta_q_r_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_Q2_nFDpCD_by_reaction, hE_miss_2N_VS_Q2_All_Int_nFDpCD, hE_miss_2N_VS_Q2_QEL_nFDpCD, hE_miss_2N_VS_Q2_MEC_nFDpCD, hE_miss_2N_VS_Q2_RES_nFDpCD,
                           hE_miss_2N_VS_Q2_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_xB_nFDpCD_by_reaction, hE_miss_2N_VS_xB_All_Int_nFDpCD, hE_miss_2N_VS_xB_QEL_nFDpCD, hE_miss_2N_VS_xB_MEC_nFDpCD, hE_miss_2N_VS_xB_RES_nFDpCD,
                           hE_miss_2N_VS_xB_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_theta_q_nFDpCD_by_reaction, hE_miss_2N_VS_theta_q_All_Int_nFDpCD, hE_miss_2N_VS_theta_q_QEL_nFDpCD, hE_miss_2N_VS_theta_q_MEC_nFDpCD,
                           hE_miss_2N_VS_theta_q_rES_nFDpCD, hE_miss_2N_VS_theta_q_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_theta_q_N_nFDpCD_by_reaction, hE_miss_2N_VS_theta_q_N_All_Int_nFDpCD, hE_miss_2N_VS_theta_q_N_QEL_nFDpCD, hE_miss_2N_VS_theta_q_N_MEC_nFDpCD,
                           hE_miss_2N_VS_theta_q_N_RES_nFDpCD, hE_miss_2N_VS_theta_q_N_DIS_nFDpCD);
    BindReactionHistograms(hE_miss_2N_VS_theta_q_r_nFDpCD_by_reaction, hE_miss_2N_VS_theta_q_r_All_Int_nFDpCD, hE_miss_2N_VS_theta_q_r_QEL_nFDpCD, hE_miss_2N_VS_theta_q_r_MEC_nFDpCD,
                           hE_miss_2N_VS_theta_q_r_RES_nFDpCD, hE_miss_2N_VS_theta_q_r_DIS_nFDpCD);
    BindReactionHistograms(hQ2_VS_xB_nFDpCD_by_reaction, hQ2_VS_xB_All_Int_nFDpCD, hQ2_VS_xB_QEL_nFDpCD, hQ2_VS_xB_MEC_nFDpCD, hQ2_VS_xB_RES_nFDpCD, hQ2_VS_xB_DIS_nFDpCD);
    BindReactionHistograms(hQ2_VS_theta_q_nFDpCD_by_reaction, hQ2_VS_theta_q_All_Int_nFDpCD, hQ2_VS_theta_q_QEL_nFDpCD, hQ2_VS_theta_q_MEC_nFDpCD, hQ2_VS_theta_q_rES_nFDpCD,
                           hQ2_VS_theta_q_DIS_nFDpCD);
    BindReactionHistograms(hQ2_VS_theta_q_N_nFDpCD_by_reaction, hQ2_VS_theta_q_N_All_Int_nFDpCD, hQ2_VS_theta_q_N_QEL_nFDpCD, hQ2_VS_theta_q_N_MEC_nFDpCD, hQ2_VS_theta_q_N_RES_nFDpCD,
                           hQ2_VS_theta_q_N_DIS_nFDpCD);
    BindReactionHistograms(hQ2_VS_theta_q_r_nFDpCD_by_reaction, hQ2_VS_theta_q_r_All_Int_nFDpCD, hQ2_VS_theta_q_r_QEL_nFDpCD, hQ2_VS_theta_q_r_MEC_nFDpCD, hQ2_VS_theta_q_r_RES_nFDpCD,
                           hQ2_VS_theta_q_r_DIS_nFDpCD);
    BindReactionHistograms(hxB_VS_theta_q_nFDpCD_by_reaction, hxB_VS_theta_q_All_Int_nFDpCD, hxB_VS_theta_q_QEL_nFDpCD, hxB_VS_theta_q_MEC_nFDpCD, hxB_VS_theta_q_rES_nFDpCD,
                           hxB_VS_theta_q_DIS_nFDpCD);
    BindReactionHistograms(hxB_VS_theta_q_N_nFDpCD_by_reaction, hxB_VS_theta_q_N_All_Int_nFDpCD, hxB_VS_theta_q_N_QEL_nFDpCD, hxB_VS_theta_q_N_MEC_nFDpCD, hxB_VS_theta_q_N_RES_nFDpCD,
                           hxB_VS_theta_q_N_DIS_nFDpCD);
    BindReactionHistograms(hxB_VS_theta_q_r_nFDpCD_by_reaction, hxB_VS_theta_q_r_All_Int_nFDpCD, hxB_VS_theta_q_r_QEL_nFDpCD, hxB_VS_theta_q_r_MEC_nFDpCD, hxB_VS_theta_q_r_RES_nFDpCD,
                           hxB_VS_theta_q_r_DIS_nFDpCD);
    BindReactionHistograms(htheta_q_VS_theta_q_N_nFDpCD_by_reaction, htheta_q_VS_theta_q_N_All_Int_nFDpCD, htheta_q_VS_theta_q_N_QEL_nFDpCD, htheta_q_VS_theta_q_N_MEC_nFDpCD,
                           htheta_q_VS_theta_q_N_RES_nFDpCD, htheta_q_VS_theta_q_N_DIS_nFDpCD);
    BindReactionHistograms(htheta_q_VS_theta_q_r_nFDpCD_by_reaction, htheta_q_VS_theta_q_r_All_Int_nFDpCD, htheta_q_VS_theta_q_r_QEL_nFDpCD, htheta_q_VS_theta_q_r_MEC_nFDpCD,
                           htheta_q_VS_theta_q_r_RES_nFDpCD, htheta_q_VS_theta_q_r_DIS_nFDpCD);
    BindReactionHistograms(htheta_q_N_VS_theta_q_r_nFDpCD_by_reaction, htheta_q_N_VS_theta_q_r_All_Int_nFDpCD, htheta_q_N_VS_theta_q_r_QEL_nFDpCD, htheta_q_N_VS_theta_q_r_MEC_nFDpCD,
                           htheta_q_N_VS_theta_q_r_RES_nFDpCD, htheta_q_N_VS_theta_q_r_DIS_nFDpCD);
}

void FinalStateHistograms::InitWHistograms(const HistogramDefinitionSettings& hDefSettings) {
    const int numTH1Dbins_W_Plots = hDefSettings.numTH1Dbins_W_Plots;
    const int numTH2Dbins_Momentum_transfer_plots = hDefSettings.numTH2Dbins_Momentum_transfer_plots;
    const double W_llim = hDefSettings.W_llim;
    const double W_ulim = hDefSettings.W_ulim;
    const double beamEMax = hDefSettings.Momentum_ulim;
    sW_1e_cut = OwnHistogram(new THStack("W distribution (All Int., 1e cut)", "W distribution (1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"));
    hW_All_Int_1e_cut = OwnHistogram(new TH1D("W distribution (All Int., 1e cut)", "W distribution (All Int., 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];",
                                 numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_QEL_1e_cut = OwnHistogram(new TH1D("W distribution (QEL only, 1e cut)", "W distribution (QEL only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_MEC_1e_cut = OwnHistogram(new TH1D("W distribution (MEC only, 1e cut)", "W distribution (MEC only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_RES_1e_cut = OwnHistogram(new TH1D("W distribution (RES only, 1e cut)", "W distribution (RES only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_DIS_1e_cut = OwnHistogram(new TH1D("W distribution (DIS only, 1e cut)", "W distribution (DIS only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_VS_q_3v_1e_cut =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (All Int., 1e cut)", "W vs. |#font[62]{q}| (All Int., 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_QEL_1e_cut =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (QEL only, 1e cut)", "W vs. |#font[62]{q}| (QEL only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_MEC_1e_cut =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (MEC only, 1e cut)", "W vs. |#font[62]{q}| (MEC only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_RES_1e_cut =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (RES only, 1e cut)", "W vs. |#font[62]{q}| (RES only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_DIS_1e_cut =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (DIS only, 1e cut)", "W vs. |#font[62]{q}| (DIS only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_1e_cut = OwnHistogram(new TH2D("W vs. #omega (All Int., 1e cut)", "W vs. #omega (All Int., 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_QEL_1e_cut = OwnHistogram(new TH2D("W vs. #omega (QEL only, 1e cut)", "W vs. #omega (QEL only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_MEC_1e_cut = OwnHistogram(new TH2D("W vs. #omega (MEC only, 1e cut)", "W vs. #omega (MEC only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_RES_1e_cut = OwnHistogram(new TH2D("W vs. #omega (RES only, 1e cut)", "W vs. #omega (RES only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_DIS_1e_cut = OwnHistogram(new TH2D("W vs. #omega (DIS only, 1e cut)", "W vs. #omega (DIS only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    sW_1p = OwnHistogram(new THStack("W distribution (All Int., 1p)", "W distribution (1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"));
    hW_All_Int_1p =
        OwnHistogram(new TH1D("W distribution (All Int., 1p)", "W distribution (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_QEL_1p =
        OwnHistogram(new TH1D("W distribution (QEL only, 1p)", "W distribution (QEL only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_MEC_1p =
        OwnHistogram(new TH1D("W distribution (MEC only, 1p)", "W distribution (MEC only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_RES_1p =
        OwnHistogram(new TH1D("W distribution (RES only, 1p)", "W distribution (RES only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_DIS_1p =
        OwnHistogram(new TH1D("W distribution (DIS only, 1p)", "W distribution (DIS only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_VS_q_3v_1p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (All Int., 1p)", "W vs. |#font[62]{q}| (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                             numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_QEL_1p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (QEL only, 1p)", "W vs. |#font[62]{q}| (QEL only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_MEC_1p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (MEC only, 1p)", "W vs. |#font[62]{q}| (MEC only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_RES_1p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (RES only, 1p)", "W vs. |#font[62]{q}| (RES only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_DIS_1p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (DIS only, 1p)", "W vs. |#font[62]{q}| (DIS only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_1p = OwnHistogram(new TH2D("W vs. #omega (All Int., 1p)", "W vs. #omega (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                              numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_QEL_1p = OwnHistogram(new TH2D("W vs. #omega (QEL only, 1p)", "W vs. #omega (QEL only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_MEC_1p = OwnHistogram(new TH2D("W vs. #omega (MEC only, 1p)", "W vs. #omega (MEC only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_RES_1p = OwnHistogram(new TH2D("W vs. #omega (RES only, 1p)", "W vs. #omega (RES only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_DIS_1p = OwnHistogram(new TH2D("W vs. #omega (DIS only, 1p)", "W vs. #omega (DIS only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    sW_1n = OwnHistogram(new THStack("W distribution (All Int., 1n)", "W distribution (1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"));
    hW_All_Int_1n =
        OwnHistogram(new TH1D("W distribution (All Int., 1n)", "W distribution (All Int., 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_QEL_1n =
        OwnHistogram(new TH1D("W distribution (QEL only, 1n)", "W distribution (QEL only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_MEC_1n =
        OwnHistogram(new TH1D("W distribution (MEC only, 1n)", "W distribution (MEC only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_RES_1n =
        OwnHistogram(new TH1D("W distribution (RES only, 1n)", "W distribution (RES only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_DIS_1n =
        OwnHistogram(new TH1D("W distribution (DIS only, 1n)", "W distribution (DIS only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_VS_q_3v_1n = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (All Int., 1n)", "W vs. |#font[62]{q}| (All Int., 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                             numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_QEL_1n = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (QEL only, 1n)", "W vs. |#font[62]{q}| (QEL only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_MEC_1n = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (MEC only, 1n)", "W vs. |#font[62]{q}| (MEC only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_RES_1n = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (RES only, 1n)", "W vs. |#font[62]{q}| (RES only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_DIS_1n = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (DIS only, 1n)", "W vs. |#font[62]{q}| (DIS only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_1n = OwnHistogram(new TH2D("W vs. #omega (All Int., 1n)", "W vs. #omega (All Int., 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                              numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_QEL_1n = OwnHistogram(new TH2D("W vs. #omega (QEL only, 1n)", "W vs. #omega (QEL only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_MEC_1n = OwnHistogram(new TH2D("W vs. #omega (MEC only, 1n)", "W vs. #omega (MEC only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_RES_1n = OwnHistogram(new TH2D("W vs. #omega (RES only, 1n)", "W vs. #omega (RES only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_DIS_1n = OwnHistogram(new TH2D("W vs. #omega (DIS only, 1n)", "W vs. #omega (DIS only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    sW_2p = OwnHistogram(new THStack("W distribution (All Int., 2p)", "W distribution (2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"));
    hW_All_Int_2p =
        OwnHistogram(new TH1D("W distribution (All Int., 2p)", "W distribution (All Int., 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_QEL_2p =
        OwnHistogram(new TH1D("W distribution (QEL only, 2p)", "W distribution (QEL only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_MEC_2p =
        OwnHistogram(new TH1D("W distribution (MEC only, 2p)", "W distribution (MEC only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_RES_2p =
        OwnHistogram(new TH1D("W distribution (RES only, 2p)", "W distribution (RES only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_DIS_2p =
        OwnHistogram(new TH1D("W distribution (DIS only, 2p)", "W distribution (DIS only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_VS_q_3v_2p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (All Int., 2p)", "W vs. |#font[62]{q}| (All Int., 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                             numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_QEL_2p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (QEL only, 2p)", "W vs. |#font[62]{q}| (QEL only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_MEC_2p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (MEC only, 2p)", "W vs. |#font[62]{q}| (MEC only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_RES_2p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (RES only, 2p)", "W vs. |#font[62]{q}| (RES only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_DIS_2p = OwnHistogram(new TH2D("W vs. |#font[62]{q}| (DIS only, 2p)", "W vs. |#font[62]{q}| (DIS only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_2p = OwnHistogram(new TH2D("W vs. #omega (All Int., 2p)", "W vs. #omega (All Int., 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                              numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_QEL_2p = OwnHistogram(new TH2D("W vs. #omega (QEL only, 2p)", "W vs. #omega (QEL only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_MEC_2p = OwnHistogram(new TH2D("W vs. #omega (MEC only, 2p)", "W vs. #omega (MEC only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_RES_2p = OwnHistogram(new TH2D("W vs. #omega (RES only, 2p)", "W vs. #omega (RES only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_DIS_2p = OwnHistogram(new TH2D("W vs. #omega (DIS only, 2p)", "W vs. #omega (DIS only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    sW_pFDpCD = OwnHistogram(new THStack("W distribution (All Int., pFDpCD)", "W distribution (pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"));
    hW_All_Int_pFDpCD = OwnHistogram(new TH1D("W distribution (All Int., pFDpCD)", "W distribution (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];",
                                 numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_QEL_pFDpCD = OwnHistogram(new TH1D("W distribution (QEL only, pFDpCD)", "W distribution (QEL only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_MEC_pFDpCD = OwnHistogram(new TH1D("W distribution (MEC only, pFDpCD)", "W distribution (MEC only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_RES_pFDpCD = OwnHistogram(new TH1D("W distribution (RES only, pFDpCD)", "W distribution (RES only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_DIS_pFDpCD = OwnHistogram(new TH1D("W distribution (DIS only, pFDpCD)", "W distribution (DIS only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_VS_q_3v_pFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (All Int., pFDpCD)", "W vs. |#font[62]{q}| (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_QEL_pFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (QEL only, pFDpCD)", "W vs. |#font[62]{q}| (QEL only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_MEC_pFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (MEC only, pFDpCD)", "W vs. |#font[62]{q}| (MEC only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_RES_pFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (RES only, pFDpCD)", "W vs. |#font[62]{q}| (RES only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_DIS_pFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (DIS only, pFDpCD)", "W vs. |#font[62]{q}| (DIS only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_pFDpCD = OwnHistogram(new TH2D("W vs. #omega (All Int., pFDpCD)", "W vs. #omega (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_QEL_pFDpCD = OwnHistogram(new TH2D("W vs. #omega (QEL only, pFDpCD)", "W vs. #omega (QEL only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_MEC_pFDpCD = OwnHistogram(new TH2D("W vs. #omega (MEC only, pFDpCD)", "W vs. #omega (MEC only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_RES_pFDpCD = OwnHistogram(new TH2D("W vs. #omega (RES only, pFDpCD)", "W vs. #omega (RES only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_DIS_pFDpCD = OwnHistogram(new TH2D("W vs. #omega (DIS only, pFDpCD)", "W vs. #omega (DIS only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    sW_nFDpCD = OwnHistogram(new THStack("W distribution (All Int., nFDpCD)", "W distribution (nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"));
    hW_All_Int_nFDpCD = OwnHistogram(new TH1D("W distribution (All Int., nFDpCD)", "W distribution (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];",
                                 numTH1Dbins_W_Plots, W_llim, W_ulim));
    hW_QEL_nFDpCD = OwnHistogram(new TH1D("W distribution (QEL only, nFDpCD)", "W distribution (QEL only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_MEC_nFDpCD = OwnHistogram(new TH1D("W distribution (MEC only, nFDpCD)", "W distribution (MEC only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_RES_nFDpCD = OwnHistogram(new TH1D("W distribution (RES only, nFDpCD)", "W distribution (RES only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_DIS_nFDpCD = OwnHistogram(new TH1D("W distribution (DIS only, nFDpCD)", "W distribution (DIS only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];", numTH1Dbins_W_Plots,
                             W_llim, W_ulim));
    hW_VS_q_3v_nFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (All Int., nFDpCD)", "W vs. |#font[62]{q}| (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_QEL_nFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (QEL only, nFDpCD)", "W vs. |#font[62]{q}| (QEL only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_MEC_nFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (MEC only, nFDpCD)", "W vs. |#font[62]{q}| (MEC only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_RES_nFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (RES only, nFDpCD)", "W vs. |#font[62]{q}| (RES only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_q_3v_DIS_nFDpCD =
        OwnHistogram(new TH2D("W vs. |#font[62]{q}| (DIS only, nFDpCD)", "W vs. |#font[62]{q}| (DIS only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];q [GeV/c]",
                 numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_nFDpCD = OwnHistogram(new TH2D("W vs. #omega (All Int., nFDpCD)", "W vs. #omega (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                  numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_QEL_nFDpCD = OwnHistogram(new TH2D("W vs. #omega (QEL only, nFDpCD)", "W vs. #omega (QEL only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_MEC_nFDpCD = OwnHistogram(new TH2D("W vs. #omega (MEC only, nFDpCD)", "W vs. #omega (MEC only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_RES_nFDpCD = OwnHistogram(new TH2D("W vs. #omega (RES only, nFDpCD)", "W vs. #omega (RES only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));
    hW_VS_omega_DIS_nFDpCD = OwnHistogram(new TH2D("W vs. #omega (DIS only, nFDpCD)", "W vs. #omega (DIS only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#omega [GeV]",
                                      numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, 0, beamEMax));

    BindReactionHistograms(hW_1e_cut_by_reaction, sW_1e_cut, hW_All_Int_1e_cut, hW_QEL_1e_cut, hW_MEC_1e_cut, hW_RES_1e_cut, hW_DIS_1e_cut);
    BindReactionHistograms(hW_1p_by_reaction, sW_1p, hW_All_Int_1p, hW_QEL_1p, hW_MEC_1p, hW_RES_1p, hW_DIS_1p);
    BindReactionHistograms(hW_1n_by_reaction, sW_1n, hW_All_Int_1n, hW_QEL_1n, hW_MEC_1n, hW_RES_1n, hW_DIS_1n);
    BindReactionHistograms(hW_2p_by_reaction, sW_2p, hW_All_Int_2p, hW_QEL_2p, hW_MEC_2p, hW_RES_2p, hW_DIS_2p);
    BindReactionHistograms(hW_pFDpCD_by_reaction, sW_pFDpCD, hW_All_Int_pFDpCD, hW_QEL_pFDpCD, hW_MEC_pFDpCD, hW_RES_pFDpCD, hW_DIS_pFDpCD);
    BindReactionHistograms(hW_nFDpCD_by_reaction, sW_nFDpCD, hW_All_Int_nFDpCD, hW_QEL_nFDpCD, hW_MEC_nFDpCD, hW_RES_nFDpCD, hW_DIS_nFDpCD);

    BindReactionHistograms(hW_VS_q_3v_1e_cut_by_reaction, hW_VS_q_3v_1e_cut, hW_VS_q_3v_QEL_1e_cut, hW_VS_q_3v_MEC_1e_cut, hW_VS_q_3v_RES_1e_cut, hW_VS_q_3v_DIS_1e_cut);
    BindReactionHistograms(hW_VS_omega_1e_cut_by_reaction, hW_VS_omega_1e_cut, hW_VS_omega_QEL_1e_cut, hW_VS_omega_MEC_1e_cut, hW_VS_omega_RES_1e_cut, hW_VS_omega_DIS_1e_cut);
    BindReactionHistograms(hW_VS_q_3v_1p_by_reaction, hW_VS_q_3v_1p, hW_VS_q_3v_QEL_1p, hW_VS_q_3v_MEC_1p, hW_VS_q_3v_RES_1p, hW_VS_q_3v_DIS_1p);
    BindReactionHistograms(hW_VS_omega_1p_by_reaction, hW_VS_omega_1p, hW_VS_omega_QEL_1p, hW_VS_omega_MEC_1p, hW_VS_omega_RES_1p, hW_VS_omega_DIS_1p);
    BindReactionHistograms(hW_VS_q_3v_1n_by_reaction, hW_VS_q_3v_1n, hW_VS_q_3v_QEL_1n, hW_VS_q_3v_MEC_1n, hW_VS_q_3v_RES_1n, hW_VS_q_3v_DIS_1n);
    BindReactionHistograms(hW_VS_omega_1n_by_reaction, hW_VS_omega_1n, hW_VS_omega_QEL_1n, hW_VS_omega_MEC_1n, hW_VS_omega_RES_1n, hW_VS_omega_DIS_1n);
    BindReactionHistograms(hW_VS_q_3v_2p_by_reaction, hW_VS_q_3v_2p, hW_VS_q_3v_QEL_2p, hW_VS_q_3v_MEC_2p, hW_VS_q_3v_RES_2p, hW_VS_q_3v_DIS_2p);
    BindReactionHistograms(hW_VS_omega_2p_by_reaction, hW_VS_omega_2p, hW_VS_omega_QEL_2p, hW_VS_omega_MEC_2p, hW_VS_omega_RES_2p, hW_VS_omega_DIS_2p);
    BindReactionHistograms(hW_VS_q_3v_pFDpCD_by_reaction, hW_VS_q_3v_pFDpCD, hW_VS_q_3v_QEL_pFDpCD, hW_VS_q_3v_MEC_pFDpCD, hW_VS_q_3v_RES_pFDpCD, hW_VS_q_3v_DIS_pFDpCD);
    BindReactionHistograms(hW_VS_omega_pFDpCD_by_reaction, hW_VS_omega_pFDpCD, hW_VS_omega_QEL_pFDpCD, hW_VS_omega_MEC_pFDpCD, hW_VS_omega_RES_pFDpCD, hW_VS_omega_DIS_pFDpCD);
    BindReactionHistograms(hW_VS_q_3v_nFDpCD_by_reaction, hW_VS_q_3v_nFDpCD, hW_VS_q_3v_QEL_nFDpCD, hW_VS_q_3v_MEC_nFDpCD, hW_VS_q_3v_RES_nFDpCD, hW_VS_q_3v_DIS_nFDpCD);
    BindReactionHistograms(hW_VS_omega_nFDpCD_by_reaction, hW_VS_omega_nFDpCD, hW_VS_omega_QEL_nFDpCD, hW_VS_omega_MEC_nFDpCD, hW_VS_omega_RES_nFDpCD, hW_VS_omega_DIS_nFDpCD);
}

void FinalStateHistograms::InitMomentumTransferHistograms(const HistogramDefinitionSettings& hDefSettings) {
    auto& directories = momentum_transfer_directories_;
    const int numTH1Dbins_Momentum_transfer_plots = hDefSettings.numTH1Dbins_Momentum_transfer_plots;
    const int numTH2Dbins_Momentum_transfer_plots = hDefSettings.numTH2Dbins_Momentum_transfer_plots;
    const double Q2_llim_FD = hDefSettings.Q2_llim_FD;
    const double Q2_ulim_FD = hDefSettings.Q2_ulim_FD;
    const double W_llim = hDefSettings.W_llim;
    const double W_ulim = hDefSettings.W_ulim;

    // Momentum transfer histograms (no #(e) cut)
    sQ2_All_e = OwnHistogram(new THStack("Q^{2} (no #(e) cut, CD & FD)", "Q^{2} Histogram (no #(e) cut, CD & FD);Q^{2} [GeV^{2}/c^{2}];"));
    hQ2_All_e = OwnHistogram(new TH1D("Q^{2} (no #(e) cut, FD)", "Q^{2} (no #(e) cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_QEL_All_e = OwnHistogram(new TH1D("Q^{2} (QEL only, no #(e) cut, FD)", "Q^{2} (QEL only, no #(e) cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_MEC_All_e = OwnHistogram(new TH1D("Q^{2} (MEC only, no #(e) cut, FD)", "Q^{2} (MEC only, no #(e) cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_RES_All_e = OwnHistogram(new TH1D("Q^{2} (RES only, no #(e) cut, FD)", "Q^{2} (RES only, no #(e) cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_DIS_All_e = OwnHistogram(new TH1D("Q^{2} (DIS only, no #(e) cut, FD)", "Q^{2} (DIS only, no #(e) cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_All_e_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_All_e"];

    sq_3v_All_e = OwnHistogram(new THStack("q = |#font[62]{q}| (no #(e) cut, CD & FD)", "q = |#font[62]{q}| Histogram (no #(e) cut, CD & FD);q [GeV/c];"));
    hq_3v_All_e = OwnHistogram(new TH1D("q = |#font[62]{q}| (no #(e) cut, FD)", "q = |#font[62]{q}| (no #(e) cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_QEL_All_e =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (QEL only, no #(e) cut, FD)", "q = |#font[62]{q}| (QEL only, no #(e) cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_MEC_All_e =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (MEC only, no #(e) cut, FD)", "q = |#font[62]{q}| (MEC only, no #(e) cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_RES_All_e =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (RES only, no #(e) cut, FD)", "q = |#font[62]{q}| (RES only, no #(e) cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_DIS_All_e =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (DIS only, no #(e) cut, FD)", "q = |#font[62]{q}| (DIS only, no #(e) cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_All_e_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_All_e"];

    // Momentum transfer histograms (1e cut)
    sQ2_1e_cut = OwnHistogram(new THStack("Q^{2} (1e cut ,CD & FD)", "Q^{2} Histogram (1e cut, CD & FD);Q^{2} [GeV^{2}/c^{2}];"));
    hQ2_1e_cut = OwnHistogram(new TH1D("Q^{2} (1e cut, FD)", "Q^{2} (1e cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_QEL_1e_cut = OwnHistogram(new TH1D("Q^{2} (QEL only, 1e cut, FD)", "Q^{2} (QEL only, 1e cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_MEC_1e_cut = OwnHistogram(new TH1D("Q^{2} (MEC only, 1e cut, FD)", "Q^{2} (MEC only, 1e cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_RES_1e_cut = OwnHistogram(new TH1D("Q^{2} (RES only, 1e cut, FD)", "Q^{2} (RES only, 1e cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_DIS_1e_cut = OwnHistogram(new TH1D("Q^{2} (DIS only, 1e cut, FD)", "Q^{2} (DIS only, 1e cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_1e_cut_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1e_cut"];

    sq_3v_1e_cut = OwnHistogram(new THStack("q = |#font[62]{q}| (1e cut, CD & FD)", "q = |#font[62]{q}| Histogram (1e cut, CD & FD);q [GeV/c];"));
    hq_3v_1e_cut = OwnHistogram(new TH1D("q = |#font[62]{q}| (1e cut, FD)", "q = |#font[62]{q}| (1e cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_QEL_1e_cut =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (QEL only, 1e cut, FD)", "q = |#font[62]{q}| (QEL only, 1e cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_MEC_1e_cut =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (MEC only, 1e cut, FD)", "q = |#font[62]{q}| (MEC only, 1e cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_RES_1e_cut =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (RES only, 1e cut, FD)", "q = |#font[62]{q}| (RES only, 1e cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_DIS_1e_cut =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (DIS only, 1e cut, FD)", "q = |#font[62]{q}| (DIS only, 1e cut, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_1e_cut_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1e_cut"];

    hQ2_VS_W_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. W (All Int., 1e cut)", "Q^{2} vs. W (All Int., 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_QEL_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. W (QEL only, 1e cut)", "Q^{2} vs. W (QEL only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_MEC_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. W (MEC only, 1e cut)", "Q^{2} vs. W (MEC only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_RES_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. W (RES only, 1e cut)", "Q^{2} vs. W (RES only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_DIS_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. W (DIS only, 1e cut)", "Q^{2} vs. W (DIS only, 1e cut);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_W_1e_cut_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1e_cut"];

    hQ2_VS_q_3v_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (All Int., 1e cut)", "Q^{2} vs. |#font[62]{q}| (All Int., 1e cut);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                  numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_QEL_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (QEL only, 1e cut)", "Q^{2} vs. |#font[62]{q}| (QEL only, 1e cut);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_MEC_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (MEC only, 1e cut)", "Q^{2} vs. |#font[62]{q}| (MEC only, 1e cut);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_RES_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (RES only, 1e cut)", "Q^{2} vs. |#font[62]{q}| (RES only, 1e cut);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_DIS_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (DIS only, 1e cut)", "Q^{2} vs. |#font[62]{q}| (DIS only, 1e cut);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_q_3v_1e_cut_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1e_cut"];

    hQ2_VS_omega_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. #omega (All Int., 1e cut)", "Q^{2} vs. #omega (All Int., 1e cut);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                   Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_QEL_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. #omega (QEL only, 1e cut)", "Q^{2} vs. #omega (QEL only, 1e cut);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_MEC_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. #omega (MEC only, 1e cut)", "Q^{2} vs. #omega (MEC only, 1e cut);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_RES_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. #omega (RES only, 1e cut)", "Q^{2} vs. #omega (RES only, 1e cut);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_DIS_1e_cut = OwnHistogram(new TH2D("Q^{2} vs. #omega (DIS only, 1e cut)", "Q^{2} vs. #omega (DIS only, 1e cut);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_omega_1e_cut_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1e_cut"];

    hq_3v_VS_omega_1e_cut = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (All Int., 1e cut)", "|#font[62]{q}| vs. #omega (All Int., 1e cut);#omega [GeV];q [GeV/c]",
                                     numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_QEL_1e_cut = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (QEL only, 1e cut)", "|#font[62]{q}| vs. #omega (QEL only, 1e cut);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_MEC_1e_cut = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (MEC only, 1e cut)", "|#font[62]{q}| vs. #omega (MEC only, 1e cut);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_RES_1e_cut = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (RES only, 1e cut)", "|#font[62]{q}| vs. #omega (RES only, 1e cut);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_DIS_1e_cut = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (DIS only, 1e cut)", "|#font[62]{q}| vs. #omega (DIS only, 1e cut);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_VS_omega_1e_cut_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1e_cut"];

    // Momentum transfer histograms (1p)
    sQ2_1p = OwnHistogram(new THStack("Q^{2} (1p, CD & FD)", "Q^{2} (1p, CD & FD);Q^{2} [GeV^{2}/c^{2}];"));
    hQ2_1p = OwnHistogram(new TH1D("Q^{2} (1p, FD)", "Q^{2} (1p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_QEL_1p = OwnHistogram(new TH1D("Q^{2} (QEL only, 1p, FD)", "Q^{2} (QEL only, 1p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_MEC_1p = OwnHistogram(new TH1D("Q^{2} (MEC only, 1p, FD)", "Q^{2} (MEC only, 1p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_RES_1p = OwnHistogram(new TH1D("Q^{2} (RES only, 1p, FD)", "Q^{2} (RES only, 1p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_DIS_1p = OwnHistogram(new TH1D("Q^{2} (DIS only, 1p, FD)", "Q^{2} (DIS only, 1p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_1p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1p"];
    std::string hQ2_1p_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_1p"];

    sq_3v_1p = OwnHistogram(new THStack("q = |#font[62]{q}| (1p, CD & FD)", "q = |#font[62]{q}| Histogram (1p, CD & FD);q [GeV/c];"));
    hq_3v_1p = OwnHistogram(new TH1D("q = |#font[62]{q}| (1p, FD)", "q = |#font[62]{q}| (1p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_QEL_1p = OwnHistogram(new TH1D("q = |#font[62]{q}| (QEL only, 1p, FD)", "q = |#font[62]{q}| (QEL only, 1p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_MEC_1p = OwnHistogram(new TH1D("q = |#font[62]{q}| (MEC only, 1p, FD)", "q = |#font[62]{q}| (MEC only, 1p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_RES_1p = OwnHistogram(new TH1D("q = |#font[62]{q}| (RES only, 1p, FD)", "q = |#font[62]{q}| (RES only, 1p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_DIS_1p = OwnHistogram(new TH1D("q = |#font[62]{q}| (DIS only, 1p, FD)", "q = |#font[62]{q}| (DIS only, 1p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_1p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1p"];

    hQ2_VS_W_1p = OwnHistogram(new TH2D("Q^{2} vs. W (All Int., 1p)", "Q^{2} vs. W (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                           numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_QEL_1p = OwnHistogram(new TH2D("Q^{2} vs. W (QEL only, 1p)", "Q^{2} vs. W (QEL only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_MEC_1p = OwnHistogram(new TH2D("Q^{2} vs. W (MEC only, 1p)", "Q^{2} vs. W (MEC only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_RES_1p = OwnHistogram(new TH2D("Q^{2} vs. W (RES only, 1p)", "Q^{2} vs. W (RES only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_DIS_1p = OwnHistogram(new TH2D("Q^{2} vs. W (DIS only, 1p)", "Q^{2} vs. W (DIS only, 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_W_1p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1p"];

    hQ2_VS_q_3v_1p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (All Int., 1p)", "Q^{2} vs. |#font[62]{q}| (All Int., 1p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                              Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_QEL_1p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (QEL only, 1p)", "Q^{2} vs. |#font[62]{q}| (QEL only, 1p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_MEC_1p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (MEC only, 1p)", "Q^{2} vs. |#font[62]{q}| (MEC only, 1p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_RES_1p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (RES only, 1p)", "Q^{2} vs. |#font[62]{q}| (RES only, 1p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_DIS_1p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (DIS only, 1p)", "Q^{2} vs. |#font[62]{q}| (DIS only, 1p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_q_3v_1p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1p"];

    hQ2_VS_omega_1p = OwnHistogram(new TH2D("Q^{2} vs. #omega (All Int., 1p)", "Q^{2} vs. #omega (All Int., 1p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                               Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_QEL_1p = OwnHistogram(new TH2D("Q^{2} vs. #omega (QEL only, 1p)", "Q^{2} vs. #omega (QEL only, 1p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_MEC_1p = OwnHistogram(new TH2D("Q^{2} vs. #omega (MEC only, 1p)", "Q^{2} vs. #omega (MEC only, 1p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_RES_1p = OwnHistogram(new TH2D("Q^{2} vs. #omega (RES only, 1p)", "Q^{2} vs. #omega (RES only, 1p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_DIS_1p = OwnHistogram(new TH2D("Q^{2} vs. #omega (DIS only, 1p)", "Q^{2} vs. #omega (DIS only, 1p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_omega_1p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1p"];

    hq_3v_VS_omega_1p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (All Int., 1p)", "|#font[62]{q}| vs. #omega (All Int., 1p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                 Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_QEL_1p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (QEL only, 1p)", "|#font[62]{q}| vs. #omega (QEL only, 1p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_MEC_1p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (MEC only, 1p)", "|#font[62]{q}| vs. #omega (MEC only, 1p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_RES_1p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (RES only, 1p)", "|#font[62]{q}| vs. #omega (RES only, 1p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_DIS_1p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (DIS only, 1p)", "|#font[62]{q}| vs. #omega (DIS only, 1p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_VS_omega_1p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1p"];

    // Momentum transfer histograms (1n)
    sQ2_1n = OwnHistogram(new THStack("Q^{2} (1n, CD & FD)", "Q^{2} (1n, CD & FD);Q^{2} [GeV^{2}/c^{2}];"));
    hQ2_1n = OwnHistogram(new TH1D("Q^{2} (1n, FD)", "Q^{2} (1n, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_QEL_1n = OwnHistogram(new TH1D("Q^{2} (QEL only, 1n, FD)", "Q^{2} (QEL only, 1n, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_MEC_1n = OwnHistogram(new TH1D("Q^{2} (MEC only, 1n, FD)", "Q^{2} (MEC only, 1n, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_RES_1n = OwnHistogram(new TH1D("Q^{2} (RES only, 1n, FD)", "Q^{2} (RES only, 1n, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_DIS_1n = OwnHistogram(new TH1D("Q^{2} (DIS only, 1n, FD)", "Q^{2} (DIS only, 1n, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_QEL_1n = OwnHistogram(new TH1D("q = |#font[62]{q}| (QEL only, 1n, FD)", "q = |#font[62]{q}| (QEL only, 1n, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_MEC_1n = OwnHistogram(new TH1D("q = |#font[62]{q}| (MEC only, 1n, FD)", "q = |#font[62]{q}| (MEC only, 1n, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_RES_1n = OwnHistogram(new TH1D("q = |#font[62]{q}| (RES only, 1n, FD)", "q = |#font[62]{q}| (RES only, 1n, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_DIS_1n = OwnHistogram(new TH1D("q = |#font[62]{q}| (DIS only, 1n, FD)", "q = |#font[62]{q}| (DIS only, 1n, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_1n_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1n"];
    std::string hQ2_1n_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_1n"];

    sq_3v_1n = OwnHistogram(new THStack("q = |#font[62]{q}| (1n, CD & FD)", "q = |#font[62]{q}| Histogram (1n, CD & FD);q [GeV/c];"));
    hq_3v_1n = OwnHistogram(new TH1D("q = |#font[62]{q}| (1n, FD)", "q = |#font[62]{q}| (1n, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_1n_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1n"];

    hQ2_VS_W_1n = OwnHistogram(new TH2D("Q^{2} vs. W (All Int., 1n)", "Q^{2} vs. W (All Int., 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                           numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_QEL_1n = OwnHistogram(new TH2D("Q^{2} vs. W (QEL only, 1n)", "Q^{2} vs. W (QEL only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_MEC_1n = OwnHistogram(new TH2D("Q^{2} vs. W (MEC only, 1n)", "Q^{2} vs. W (MEC only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_RES_1n = OwnHistogram(new TH2D("Q^{2} vs. W (RES only, 1n)", "Q^{2} vs. W (RES only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_DIS_1n = OwnHistogram(new TH2D("Q^{2} vs. W (DIS only, 1n)", "Q^{2} vs. W (DIS only, 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_W_1n_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1n"];

    hQ2_VS_q_3v_1n = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (All Int., 1n)", "Q^{2} vs. |#font[62]{q}| (All Int., 1n);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                              Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_QEL_1n = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (QEL only, 1n)", "Q^{2} vs. |#font[62]{q}| (QEL only, 1n);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_MEC_1n = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (MEC only, 1n)", "Q^{2} vs. |#font[62]{q}| (MEC only, 1n);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_RES_1n = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (RES only, 1n)", "Q^{2} vs. |#font[62]{q}| (RES only, 1n);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_DIS_1n = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (DIS only, 1n)", "Q^{2} vs. |#font[62]{q}| (DIS only, 1n);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_q_3v_1n_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1n"];

    hQ2_VS_omega_1n = OwnHistogram(new TH2D("Q^{2} vs. #omega (All Int., 1n)", "Q^{2} vs. #omega (All Int., 1n);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                               Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_QEL_1n = OwnHistogram(new TH2D("Q^{2} vs. #omega (QEL only, 1n)", "Q^{2} vs. #omega (QEL only, 1n);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_MEC_1n = OwnHistogram(new TH2D("Q^{2} vs. #omega (MEC only, 1n)", "Q^{2} vs. #omega (MEC only, 1n);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_RES_1n = OwnHistogram(new TH2D("Q^{2} vs. #omega (RES only, 1n)", "Q^{2} vs. #omega (RES only, 1n);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_DIS_1n = OwnHistogram(new TH2D("Q^{2} vs. #omega (DIS only, 1n)", "Q^{2} vs. #omega (DIS only, 1n);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_omega_1n_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1n"];

    hq_3v_VS_omega_1n = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (All Int., 1n)", "|#font[62]{q}| vs. #omega (All Int., 1n);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                 Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_QEL_1n = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (QEL only, 1n)", "|#font[62]{q}| vs. #omega (QEL only, 1n);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_MEC_1n = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (MEC only, 1n)", "|#font[62]{q}| vs. #omega (MEC only, 1n);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_RES_1n = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (RES only, 1n)", "|#font[62]{q}| vs. #omega (RES only, 1n);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_DIS_1n = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (DIS only, 1n)", "|#font[62]{q}| vs. #omega (DIS only, 1n);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_VS_omega_1n_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_1n"];

    // Momentum transfer histograms (2p)
    sQ2_2p = OwnHistogram(new THStack("Q^{2} (2p, CD & FD)", "Q^{2} (2p, CD & FD);Q^{2} [GeV^{2}/c^{2}];"));
    hQ2_2p = OwnHistogram(new TH1D("Q^{2} (2p, FD)", "Q^{2} (2p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_QEL_2p = OwnHistogram(new TH1D("Q^{2} (QEL only, 2p, FD)", "Q^{2} (QEL only, 2p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_MEC_2p = OwnHistogram(new TH1D("Q^{2} (MEC only, 2p, FD)", "Q^{2} (MEC only, 2p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_RES_2p = OwnHistogram(new TH1D("Q^{2} (RES only, 2p, FD)", "Q^{2} (RES only, 2p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_DIS_2p = OwnHistogram(new TH1D("Q^{2} (DIS only, 2p, FD)", "Q^{2} (DIS only, 2p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_2p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_2p"];

    sq_3v_2p = OwnHistogram(new THStack("q = |#font[62]{q}| (2p, CD & FD)", "q = |#font[62]{q}| Histogram (2p, CD & FD);q [GeV/c];"));
    hq_3v_2p = OwnHistogram(new TH1D("q = |#font[62]{q}| (2p, FD)", "q = |#font[62]{q}| (2p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_QEL_2p = OwnHistogram(new TH1D("q = |#font[62]{q}| (QEL only, 2p, FD)", "q = |#font[62]{q}| (QEL only, 2p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_MEC_2p = OwnHistogram(new TH1D("q = |#font[62]{q}| (MEC only, 2p, FD)", "q = |#font[62]{q}| (MEC only, 2p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_RES_2p = OwnHistogram(new TH1D("q = |#font[62]{q}| (RES only, 2p, FD)", "q = |#font[62]{q}| (RES only, 2p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_DIS_2p = OwnHistogram(new TH1D("q = |#font[62]{q}| (DIS only, 2p, FD)", "q = |#font[62]{q}| (DIS only, 2p, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_2p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_2p"];

    hQ2_VS_W_2p = OwnHistogram(new TH2D("Q^{2} vs. W (All Int., 2p)", "Q^{2} vs. W (All Int., 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                           numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_QEL_2p = OwnHistogram(new TH2D("Q^{2} vs. W (QEL only, 2p)", "Q^{2} vs. W (QEL only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_MEC_2p = OwnHistogram(new TH2D("Q^{2} vs. W (MEC only, 2p)", "Q^{2} vs. W (MEC only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_RES_2p = OwnHistogram(new TH2D("Q^{2} vs. W (RES only, 2p)", "Q^{2} vs. W (RES only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_DIS_2p = OwnHistogram(new TH2D("Q^{2} vs. W (DIS only, 2p)", "Q^{2} vs. W (DIS only, 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_W_2p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_2p"];

    hQ2_VS_q_3v_2p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (All Int., 2p)", "Q^{2} vs. |#font[62]{q}| (All Int., 2p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                              Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_QEL_2p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (QEL only, 2p)", "Q^{2} vs. |#font[62]{q}| (QEL only, 2p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_MEC_2p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (MEC only, 2p)", "Q^{2} vs. |#font[62]{q}| (MEC only, 2p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_RES_2p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (RES only, 2p)", "Q^{2} vs. |#font[62]{q}| (RES only, 2p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_DIS_2p = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (DIS only, 2p)", "Q^{2} vs. |#font[62]{q}| (DIS only, 2p);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                  Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_q_3v_2p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_2p"];

    hQ2_VS_omega_2p = OwnHistogram(new TH2D("Q^{2} vs. #omega (All Int., 2p)", "Q^{2} vs. #omega (All Int., 2p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                               Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_QEL_2p = OwnHistogram(new TH2D("Q^{2} vs. #omega (QEL only, 2p)", "Q^{2} vs. #omega (QEL only, 2p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_MEC_2p = OwnHistogram(new TH2D("Q^{2} vs. #omega (MEC only, 2p)", "Q^{2} vs. #omega (MEC only, 2p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_RES_2p = OwnHistogram(new TH2D("Q^{2} vs. #omega (RES only, 2p)", "Q^{2} vs. #omega (RES only, 2p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_DIS_2p = OwnHistogram(new TH2D("Q^{2} vs. #omega (DIS only, 2p)", "Q^{2} vs. #omega (DIS only, 2p);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD,
                                   Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_omega_2p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_2p"];

    hq_3v_VS_omega_2p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (All Int., 2p)", "|#font[62]{q}| vs. #omega (All Int., 2p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                 Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_QEL_2p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (QEL only, 2p)", "|#font[62]{q}| vs. #omega (QEL only, 2p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_MEC_2p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (MEC only, 2p)", "|#font[62]{q}| vs. #omega (MEC only, 2p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_RES_2p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (RES only, 2p)", "|#font[62]{q}| vs. #omega (RES only, 2p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_DIS_2p = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (DIS only, 2p)", "|#font[62]{q}| vs. #omega (DIS only, 2p);#omega [GeV];q [GeV/c]", numTH2Dbins_Momentum_transfer_plots,
                                     Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_VS_omega_2p_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_2p"];

    // Momentum transfer histograms (pFDpCD)
    sQ2_pFDpCD = OwnHistogram(new THStack("Q^{2} (pFDpCD, CD & FD)", "Q^{2} (pFDpCD, CD & FD);Q^{2} [GeV^{2}/c^{2}];"));
    hQ2_pFDpCD = OwnHistogram(new TH1D("Q^{2} (pFDpCD, FD)", "Q^{2} (pFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_QEL_pFDpCD = OwnHistogram(new TH1D("Q^{2} (QEL only, pFDpCD, FD)", "Q^{2} (QEL only, pFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_MEC_pFDpCD = OwnHistogram(new TH1D("Q^{2} (MEC only, pFDpCD, FD)", "Q^{2} (MEC only, pFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_RES_pFDpCD = OwnHistogram(new TH1D("Q^{2} (RES only, pFDpCD, FD)", "Q^{2} (RES only, pFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_DIS_pFDpCD = OwnHistogram(new TH1D("Q^{2} (DIS only, pFDpCD, FD)", "Q^{2} (DIS only, pFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_pFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_pFDpCD"];
    std::string hQ2_pFDpCD_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_pFDpCD_Q2"];

    sq_3v_pFDpCD = OwnHistogram(new THStack("q = |#font[62]{q}| (pFDpCD, CD & FD)", "q = |#font[62]{q}| Histogram (pFDpCD, CD & FD);q [GeV/c];"));
    hq_3v_pFDpCD = OwnHistogram(new TH1D("q = |#font[62]{q}| (pFDpCD, FD)", "q = |#font[62]{q}| (pFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_QEL_pFDpCD =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (QEL only, pFDpCD, FD)", "q = |#font[62]{q}| (QEL only, pFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_MEC_pFDpCD =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (MEC only, pFDpCD, FD)", "q = |#font[62]{q}| (MEC only, pFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_RES_pFDpCD =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (RES only, pFDpCD, FD)", "q = |#font[62]{q}| (RES only, pFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_DIS_pFDpCD =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (DIS only, pFDpCD, FD)", "q = |#font[62]{q}| (DIS only, pFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_pFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_pFDpCD"];

    hQ2_VS_W_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (All Int., pFDpCD)", "Q^{2} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_QEL_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (QEL only, pFDpCD)", "Q^{2} vs. W (QEL only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_MEC_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (MEC only, pFDpCD)", "Q^{2} vs. W (MEC only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_RES_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (RES only, pFDpCD)", "Q^{2} vs. W (RES only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_DIS_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (DIS only, pFDpCD)", "Q^{2} vs. W (DIS only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_W_pFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_pFDpCD"];

    hQ2_VS_q_3v_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (All Int., pFDpCD)", "Q^{2} vs. |#font[62]{q}| (All Int., pFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                  numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_QEL_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (QEL only, pFDpCD)", "Q^{2} vs. |#font[62]{q}| (QEL only, pFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_MEC_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (MEC only, pFDpCD)", "Q^{2} vs. |#font[62]{q}| (MEC only, pFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_RES_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (RES only, pFDpCD)", "Q^{2} vs. |#font[62]{q}| (RES only, pFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_DIS_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (DIS only, pFDpCD)", "Q^{2} vs. |#font[62]{q}| (DIS only, pFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_q_3v_pFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_pFDpCD"];

    hQ2_VS_omega_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (All Int., pFDpCD)", "Q^{2} vs. #omega (All Int., pFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                   Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_QEL_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (QEL only, pFDpCD)", "Q^{2} vs. #omega (QEL only, pFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_MEC_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (MEC only, pFDpCD)", "Q^{2} vs. #omega (MEC only, pFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_RES_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (RES only, pFDpCD)", "Q^{2} vs. #omega (RES only, pFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_DIS_pFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (DIS only, pFDpCD)", "Q^{2} vs. #omega (DIS only, pFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_omega_pFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_pFDpCD"];

    hq_3v_VS_omega_pFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (All Int., pFDpCD)", "|#font[62]{q}| vs. #omega (All Int., pFDpCD);#omega [GeV];q [GeV/c]",
                                     numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_QEL_pFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (QEL only, pFDpCD)", "|#font[62]{q}| vs. #omega (QEL only, pFDpCD);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_MEC_pFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (MEC only, pFDpCD)", "|#font[62]{q}| vs. #omega (MEC only, pFDpCD);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_RES_pFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (RES only, pFDpCD)", "|#font[62]{q}| vs. #omega (RES only, pFDpCD);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_DIS_pFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (DIS only, pFDpCD)", "|#font[62]{q}| vs. #omega (DIS only, pFDpCD);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_VS_omega_pFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_pFDpCD"];

    // Momentum transfer histograms (nFDpCD)
    sQ2_nFDpCD = OwnHistogram(new THStack("Q^{2} (nFDpCD, CD & FD)", "Q^{2} (nFDpCD, CD & FD);Q^{2} [GeV^{2}/c^{2}];"));
    hQ2_nFDpCD = OwnHistogram(new TH1D("Q^{2} (nFDpCD, FD)", "Q^{2} (nFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_QEL_nFDpCD = OwnHistogram(new TH1D("Q^{2} (QEL only, nFDpCD, FD)", "Q^{2} (QEL only, nFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_MEC_nFDpCD = OwnHistogram(new TH1D("Q^{2} (MEC only, nFDpCD, FD)", "Q^{2} (MEC only, nFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_RES_nFDpCD = OwnHistogram(new TH1D("Q^{2} (RES only, nFDpCD, FD)", "Q^{2} (RES only, nFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_DIS_nFDpCD = OwnHistogram(new TH1D("Q^{2} (DIS only, nFDpCD, FD)", "Q^{2} (DIS only, nFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_nFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_nFDpCD"];
    std::string hQ2_nFDpCD_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_nFDpCD_Q2"];

    sq_3v_nFDpCD = OwnHistogram(new THStack("q = |#font[62]{q}| (nFDpCD, CD & FD)", "q = |#font[62]{q}| Histogram (nFDpCD, CD & FD);q [GeV/c];"));
    hq_3v_nFDpCD = OwnHistogram(new TH1D("q = |#font[62]{q}| (nFDpCD, FD)", "q = |#font[62]{q}| (nFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_QEL_nFDpCD =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (QEL only, nFDpCD, FD)", "q = |#font[62]{q}| (QEL only, nFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_MEC_nFDpCD =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (MEC only, nFDpCD, FD)", "q = |#font[62]{q}| (MEC only, nFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_RES_nFDpCD =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (RES only, nFDpCD, FD)", "q = |#font[62]{q}| (RES only, nFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_DIS_nFDpCD =
        OwnHistogram(new TH1D("q = |#font[62]{q}| (DIS only, nFDpCD, FD)", "q = |#font[62]{q}| (DIS only, nFDpCD, FD);q [GeV/c];", numTH1Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_nFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_nFDpCD"];

    hQ2_VS_W_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (All Int., nFDpCD)", "Q^{2} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                               numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_QEL_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (QEL only, nFDpCD)", "Q^{2} vs. W (QEL only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_MEC_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (MEC only, nFDpCD)", "Q^{2} vs. W (MEC only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_RES_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (RES only, nFDpCD)", "Q^{2} vs. W (RES only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_W_DIS_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. W (DIS only, nFDpCD)", "Q^{2} vs. W (DIS only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];Q^{2} [GeV^{2}/c^{2}]",
                                   numTH2Dbins_Momentum_transfer_plots, W_llim, W_ulim, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_W_nFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_nFDpCD"];

    hQ2_VS_q_3v_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (All Int., nFDpCD)", "Q^{2} vs. |#font[62]{q}| (All Int., nFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                  numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_QEL_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (QEL only, nFDpCD)", "Q^{2} vs. |#font[62]{q}| (QEL only, nFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_MEC_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (MEC only, nFDpCD)", "Q^{2} vs. |#font[62]{q}| (MEC only, nFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_RES_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (RES only, nFDpCD)", "Q^{2} vs. |#font[62]{q}| (RES only, nFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_q_3v_DIS_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. |#font[62]{q}| (DIS only, nFDpCD)", "Q^{2} vs. |#font[62]{q}| (DIS only, nFDpCD);q [GeV/c];Q^{2} [GeV^{2}/c^{2}]",
                                      numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_q_3v_nFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_nFDpCD"];

    hQ2_VS_omega_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (All Int., nFDpCD)", "Q^{2} vs. #omega (All Int., nFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                   Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_QEL_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (QEL only, nFDpCD)", "Q^{2} vs. #omega (QEL only, nFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_MEC_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (MEC only, nFDpCD)", "Q^{2} vs. #omega (MEC only, nFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_RES_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (RES only, nFDpCD)", "Q^{2} vs. #omega (RES only, nFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hQ2_VS_omega_DIS_nFDpCD = OwnHistogram(new TH2D("Q^{2} vs. #omega (DIS only, nFDpCD)", "Q^{2} vs. #omega (DIS only, nFDpCD);#omega [GeV];Q^{2} [GeV^{2}/c^{2}]", numTH2Dbins_Momentum_transfer_plots,
                                       Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hQ2_VS_omega_nFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_nFDpCD"];

    hq_3v_VS_omega_nFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (All Int., nFDpCD)", "|#font[62]{q}| vs. #omega (All Int., nFDpCD);#omega [GeV];q [GeV/c]",
                                     numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_QEL_nFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (QEL only, nFDpCD)", "|#font[62]{q}| vs. #omega (QEL only, nFDpCD);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_MEC_nFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (MEC only, nFDpCD)", "|#font[62]{q}| vs. #omega (MEC only, nFDpCD);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_RES_nFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (RES only, nFDpCD)", "|#font[62]{q}| vs. #omega (RES only, nFDpCD);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    hq_3v_VS_omega_DIS_nFDpCD = OwnHistogram(new TH2D("|#font[62]{q}| vs. #omega (DIS only, nFDpCD)", "|#font[62]{q}| vs. #omega (DIS only, nFDpCD);#omega [GeV];q [GeV/c]",
                                         numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD, numTH2Dbins_Momentum_transfer_plots, Q2_llim_FD, Q2_ulim_FD));
    std::string hq_3v_VS_omega_nFDpCD_Dir = directories.Momentum_transfer_dir_map["Momentum_transfer_nFDpCD"];

    BindReactionHistograms(hQ2_All_e_by_reaction, sQ2_All_e, hQ2_All_e, hQ2_QEL_All_e, hQ2_MEC_All_e, hQ2_RES_All_e, hQ2_DIS_All_e);
    BindReactionHistograms(hq_3v_All_e_by_reaction, sq_3v_All_e, hq_3v_All_e, hq_3v_QEL_All_e, hq_3v_MEC_All_e, hq_3v_RES_All_e, hq_3v_DIS_All_e);
    BindReactionHistograms(hQ2_1e_cut_by_reaction, sQ2_1e_cut, hQ2_1e_cut, hQ2_QEL_1e_cut, hQ2_MEC_1e_cut, hQ2_RES_1e_cut, hQ2_DIS_1e_cut);
    BindReactionHistograms(hq_3v_1e_cut_by_reaction, sq_3v_1e_cut, hq_3v_1e_cut, hq_3v_QEL_1e_cut, hq_3v_MEC_1e_cut, hq_3v_RES_1e_cut, hq_3v_DIS_1e_cut);
    BindReactionHistograms(hQ2_1p_by_reaction, sQ2_1p, hQ2_1p, hQ2_QEL_1p, hQ2_MEC_1p, hQ2_RES_1p, hQ2_DIS_1p);
    BindReactionHistograms(hq_3v_1p_by_reaction, sq_3v_1p, hq_3v_1p, hq_3v_QEL_1p, hq_3v_MEC_1p, hq_3v_RES_1p, hq_3v_DIS_1p);
    BindReactionHistograms(hQ2_1n_by_reaction, sQ2_1n, hQ2_1n, hQ2_QEL_1n, hQ2_MEC_1n, hQ2_RES_1n, hQ2_DIS_1n);
    BindReactionHistograms(hq_3v_1n_by_reaction, sq_3v_1n, hq_3v_1n, hq_3v_QEL_1n, hq_3v_MEC_1n, hq_3v_RES_1n, hq_3v_DIS_1n);
    BindReactionHistograms(hQ2_2p_by_reaction, sQ2_2p, hQ2_2p, hQ2_QEL_2p, hQ2_MEC_2p, hQ2_RES_2p, hQ2_DIS_2p);
    BindReactionHistograms(hq_3v_2p_by_reaction, sq_3v_2p, hq_3v_2p, hq_3v_QEL_2p, hq_3v_MEC_2p, hq_3v_RES_2p, hq_3v_DIS_2p);
    BindReactionHistograms(hQ2_pFDpCD_by_reaction, sQ2_pFDpCD, hQ2_pFDpCD, hQ2_QEL_pFDpCD, hQ2_MEC_pFDpCD, hQ2_RES_pFDpCD, hQ2_DIS_pFDpCD);
    BindReactionHistograms(hq_3v_pFDpCD_by_reaction, sq_3v_pFDpCD, hq_3v_pFDpCD, hq_3v_QEL_pFDpCD, hq_3v_MEC_pFDpCD, hq_3v_RES_pFDpCD, hq_3v_DIS_pFDpCD);
    BindReactionHistograms(hQ2_nFDpCD_by_reaction, sQ2_nFDpCD, hQ2_nFDpCD, hQ2_QEL_nFDpCD, hQ2_MEC_nFDpCD, hQ2_RES_nFDpCD, hQ2_DIS_nFDpCD);
    BindReactionHistograms(hq_3v_nFDpCD_by_reaction, sq_3v_nFDpCD, hq_3v_nFDpCD, hq_3v_QEL_nFDpCD, hq_3v_MEC_nFDpCD, hq_3v_RES_nFDpCD, hq_3v_DIS_nFDpCD);

    BindReactionHistograms(hQ2_VS_W_1e_cut_by_reaction, hQ2_VS_W_1e_cut, hQ2_VS_W_QEL_1e_cut, hQ2_VS_W_MEC_1e_cut, hQ2_VS_W_RES_1e_cut, hQ2_VS_W_DIS_1e_cut);
    BindReactionHistograms(hQ2_VS_q_3v_1e_cut_by_reaction, hQ2_VS_q_3v_1e_cut, hQ2_VS_q_3v_QEL_1e_cut, hQ2_VS_q_3v_MEC_1e_cut, hQ2_VS_q_3v_RES_1e_cut, hQ2_VS_q_3v_DIS_1e_cut);
    BindReactionHistograms(hQ2_VS_omega_1e_cut_by_reaction, hQ2_VS_omega_1e_cut, hQ2_VS_omega_QEL_1e_cut, hQ2_VS_omega_MEC_1e_cut, hQ2_VS_omega_RES_1e_cut, hQ2_VS_omega_DIS_1e_cut);
    BindReactionHistograms(hq_3v_VS_omega_1e_cut_by_reaction, hq_3v_VS_omega_1e_cut, hq_3v_VS_omega_QEL_1e_cut, hq_3v_VS_omega_MEC_1e_cut, hq_3v_VS_omega_RES_1e_cut,
                           hq_3v_VS_omega_DIS_1e_cut);
    BindReactionHistograms(hQ2_VS_W_1p_by_reaction, hQ2_VS_W_1p, hQ2_VS_W_QEL_1p, hQ2_VS_W_MEC_1p, hQ2_VS_W_RES_1p, hQ2_VS_W_DIS_1p);
    BindReactionHistograms(hQ2_VS_q_3v_1p_by_reaction, hQ2_VS_q_3v_1p, hQ2_VS_q_3v_QEL_1p, hQ2_VS_q_3v_MEC_1p, hQ2_VS_q_3v_RES_1p, hQ2_VS_q_3v_DIS_1p);
    BindReactionHistograms(hQ2_VS_omega_1p_by_reaction, hQ2_VS_omega_1p, hQ2_VS_omega_QEL_1p, hQ2_VS_omega_MEC_1p, hQ2_VS_omega_RES_1p, hQ2_VS_omega_DIS_1p);
    BindReactionHistograms(hq_3v_VS_omega_1p_by_reaction, hq_3v_VS_omega_1p, hq_3v_VS_omega_QEL_1p, hq_3v_VS_omega_MEC_1p, hq_3v_VS_omega_RES_1p, hq_3v_VS_omega_DIS_1p);
    BindReactionHistograms(hQ2_VS_W_1n_by_reaction, hQ2_VS_W_1n, hQ2_VS_W_QEL_1n, hQ2_VS_W_MEC_1n, hQ2_VS_W_RES_1n, hQ2_VS_W_DIS_1n);
    BindReactionHistograms(hQ2_VS_q_3v_1n_by_reaction, hQ2_VS_q_3v_1n, hQ2_VS_q_3v_QEL_1n, hQ2_VS_q_3v_MEC_1n, hQ2_VS_q_3v_RES_1n, hQ2_VS_q_3v_DIS_1n);
    BindReactionHistograms(hQ2_VS_omega_1n_by_reaction, hQ2_VS_omega_1n, hQ2_VS_omega_QEL_1n, hQ2_VS_omega_MEC_1n, hQ2_VS_omega_RES_1n, hQ2_VS_omega_DIS_1n);
    BindReactionHistograms(hq_3v_VS_omega_1n_by_reaction, hq_3v_VS_omega_1n, hq_3v_VS_omega_QEL_1n, hq_3v_VS_omega_MEC_1n, hq_3v_VS_omega_RES_1n, hq_3v_VS_omega_DIS_1n);
    BindReactionHistograms(hQ2_VS_W_2p_by_reaction, hQ2_VS_W_2p, hQ2_VS_W_QEL_2p, hQ2_VS_W_MEC_2p, hQ2_VS_W_RES_2p, hQ2_VS_W_DIS_2p);
    BindReactionHistograms(hQ2_VS_q_3v_2p_by_reaction, hQ2_VS_q_3v_2p, hQ2_VS_q_3v_QEL_2p, hQ2_VS_q_3v_MEC_2p, hQ2_VS_q_3v_RES_2p, hQ2_VS_q_3v_DIS_2p);
    BindReactionHistograms(hQ2_VS_omega_2p_by_reaction, hQ2_VS_omega_2p, hQ2_VS_omega_QEL_2p, hQ2_VS_omega_MEC_2p, hQ2_VS_omega_RES_2p, hQ2_VS_omega_DIS_2p);
    BindReactionHistograms(hq_3v_VS_omega_2p_by_reaction, hq_3v_VS_omega_2p, hq_3v_VS_omega_QEL_2p, hq_3v_VS_omega_MEC_2p, hq_3v_VS_omega_RES_2p, hq_3v_VS_omega_DIS_2p);
    BindReactionHistograms(hQ2_VS_W_pFDpCD_by_reaction, hQ2_VS_W_pFDpCD, hQ2_VS_W_QEL_pFDpCD, hQ2_VS_W_MEC_pFDpCD, hQ2_VS_W_RES_pFDpCD, hQ2_VS_W_DIS_pFDpCD);
    BindReactionHistograms(hQ2_VS_q_3v_pFDpCD_by_reaction, hQ2_VS_q_3v_pFDpCD, hQ2_VS_q_3v_QEL_pFDpCD, hQ2_VS_q_3v_MEC_pFDpCD, hQ2_VS_q_3v_RES_pFDpCD, hQ2_VS_q_3v_DIS_pFDpCD);
    BindReactionHistograms(hQ2_VS_omega_pFDpCD_by_reaction, hQ2_VS_omega_pFDpCD, hQ2_VS_omega_QEL_pFDpCD, hQ2_VS_omega_MEC_pFDpCD, hQ2_VS_omega_RES_pFDpCD, hQ2_VS_omega_DIS_pFDpCD);
    BindReactionHistograms(hq_3v_VS_omega_pFDpCD_by_reaction, hq_3v_VS_omega_pFDpCD, hq_3v_VS_omega_QEL_pFDpCD, hq_3v_VS_omega_MEC_pFDpCD, hq_3v_VS_omega_RES_pFDpCD,
                           hq_3v_VS_omega_DIS_pFDpCD);
    BindReactionHistograms(hQ2_VS_W_nFDpCD_by_reaction, hQ2_VS_W_nFDpCD, hQ2_VS_W_QEL_nFDpCD, hQ2_VS_W_MEC_nFDpCD, hQ2_VS_W_RES_nFDpCD, hQ2_VS_W_DIS_nFDpCD);
    BindReactionHistograms(hQ2_VS_q_3v_nFDpCD_by_reaction, hQ2_VS_q_3v_nFDpCD, hQ2_VS_q_3v_QEL_nFDpCD, hQ2_VS_q_3v_MEC_nFDpCD, hQ2_VS_q_3v_RES_nFDpCD, hQ2_VS_q_3v_DIS_nFDpCD);
    BindReactionHistograms(hQ2_VS_omega_nFDpCD_by_reaction, hQ2_VS_omega_nFDpCD, hQ2_VS_omega_QEL_nFDpCD, hQ2_VS_omega_MEC_nFDpCD, hQ2_VS_omega_RES_nFDpCD, hQ2_VS_omega_DIS_nFDpCD);
    BindReactionHistograms(hq_3v_VS_omega_nFDpCD_by_reaction, hq_3v_VS_omega_nFDpCD, hq_3v_VS_omega_QEL_nFDpCD, hq_3v_VS_omega_MEC_nFDpCD, hq_3v_VS_omega_RES_nFDpCD,
                           hq_3v_VS_omega_DIS_nFDpCD);
}

void FinalStateHistograms::InitEeHistograms(const HistogramDefinitionSettings& hDefSettings) {
    auto& directories = e_e_directories_;
    const int numTH1Dbins_E_e_Plots = hDefSettings.numTH1Dbins_E_e_Plots;
    const int numTH2Dbins_E_e_Plots = hDefSettings.numTH2Dbins_E_e_Plots;
    const double beamEMax = hDefSettings.Momentum_ulim;

#pragma region /* Energy (E_e) histograms */

    // E_e plots (1e cut)
    /* Energy (E_e) histograms (1e cut, CD & FD) */
    sE_e_1e_cut = OwnHistogram(new THStack("E_{e} (1e cut)", "E_{e} Histogram (1e cut);E_{e} [GeV]"));
    std::string sE_e_1e_cut_Dir = directories.E_e_dir_map["E_e_All_Int_1e_cut_stack"];

    hE_e_All_Int_1e_cut_FD = OwnHistogram(new TH1D("E_{e} (1e cut)", "E_{e} Histogram (All Int., 1e cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_QEL_1e_cut_FD = OwnHistogram(new TH1D("E_{e} (QEL Only, 1e cut)", "E_{e} Histogram (QEL Only, 1e cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_MEC_1e_cut_FD = OwnHistogram(new TH1D("E_{e} (MEC only, 1e cut)", "E_{e} Histogram (MEC only, 1e cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_RES_1e_cut_FD = OwnHistogram(new TH1D("E_{e} (RES only, 1e cut)", "E_{e} Histogram (RES only, 1e cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_DIS_1e_cut_FD = OwnHistogram(new TH1D("E_{e} (DIS only, 1e cut)", "E_{e} Histogram (DIS only, 1e cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_All_Int_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1e_cut"];
    std::string hE_e_QEL_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1e_cut"];
    std::string hE_e_MEC_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1e_cut"];
    std::string hE_e_RES_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1e_cut"];
    std::string hE_e_DIS_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1e_cut"];

    hE_e_VS_Theta_e_All_Int_1e_cut_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (All Int., 1e cut)", "E_{e} vs. #theta_{e} (All Int., 1e cut);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots,
                                                 0, 50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_QEL_1e_cut_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (QEL Only, 1e cut)", "E_{e} vs. #theta_{e} (QEL Only, 1e cut);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_MEC_1e_cut_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (MEC only, 1e cut)", "E_{e} vs. #theta_{e} (MEC only, 1e cut);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_RES_1e_cut_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (RES only, 1e cut)", "E_{e} vs. #theta_{e} (RES only, 1e cut);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_DIS_1e_cut_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (DIS only, 1e cut)", "E_{e} vs. #theta_{e} (DIS only, 1e cut);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_VS_Theta_e_All_Int_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1e_cut"];
    std::string hE_e_VS_Theta_e_QEL_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1e_cut"];
    std::string hE_e_VS_Theta_e_MEC_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1e_cut"];
    std::string hE_e_VS_Theta_e_RES_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1e_cut"];
    std::string hE_e_VS_Theta_e_DIS_1e_cut_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1e_cut"];

    // E_e plots (1p)
    /* Energy (E_e) histograms (1p, CD & FD) */
    sE_e_1p_FD = OwnHistogram(new THStack("E_{e} (1p)", "E_{e} Histogram (1p);E_{e} [GeV]"));
    std::string sE_e_1p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1p_stack"];

    hE_e_All_Int_1p_FD = OwnHistogram(new TH1D("E_{e} (All Int., 1p)", "E_{e} Histogram (All Int., 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_QEL_1p_FD = OwnHistogram(new TH1D("E_{e} (QEL Only, 1p)", "E_{e} Histogram (QEL Only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_MEC_1p_FD = OwnHistogram(new TH1D("E_{e} (MEC only, 1p)", "E_{e} Histogram (MEC only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_RES_1p_FD = OwnHistogram(new TH1D("E_{e} (RES only, 1p)", "E_{e} Histogram (RES only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_DIS_1p_FD = OwnHistogram(new TH1D("E_{e} (DIS only, 1p)", "E_{e} Histogram (DIS only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_All_Int_1p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1p"];
    std::string hE_e_QEL_1p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1p"];
    std::string hE_e_MEC_1p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1p"];
    std::string hE_e_RES_1p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1p"];
    std::string hE_e_DIS_1p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1p"];

    /* E_e vs. Theta_e (1p, CD & FD) */
    hE_e_VS_Theta_e_All_Int_1p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (All Int., 1p)", "E_{e} vs. #theta_{e} (All Int., 1p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                             numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_QEL_1p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (QEL Only, 1p)", "E_{e} vs. #theta_{e} (QEL Only, 1p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_MEC_1p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (MEC only, 1p)", "E_{e} vs. #theta_{e} (MEC only, 1p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_RES_1p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (RES only, 1p)", "E_{e} vs. #theta_{e} (RES only, 1p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_DIS_1p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (DIS only, 1p)", "E_{e} vs. #theta_{e} (DIS only, 1p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_VS_Theta_e_All_Int_1p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1p"];
    std::string hE_e_VS_Theta_e_QEL_1p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1p"];
    std::string hE_e_VS_Theta_e_MEC_1p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1p"];
    std::string hE_e_VS_Theta_e_RES_1p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1p"];
    std::string hE_e_VS_Theta_e_DIS_1p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1p"];

    /* Energy (E_e) histograms around theta_e = 15 (1p, CD & FD) */
    sE_e_15_1p_FD = OwnHistogram(new THStack("E_{e} (1p)", "E_{e} Histogram (1p);E_{e} [GeV]"));
    std::string sE_e_15_1p_CD_Dir = directories.E_e_dir_map["E_e_All_Int_1p_stack"];
    std::string sE_e_15_1p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1p_stack"];

    hE_e_15_All_Int_1p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., 1p)", "E_{e} around #theta_{e} = 15#circ (All Int., 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_QEL_1p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, 1p)", "E_{e} around #theta_{e} = 15#circ (QEL Only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_MEC_1p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (MEC only, 1p)", "E_{e} around #theta_{e} = 15#circ (MEC only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_RES_1p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (RES only, 1p)", "E_{e} around #theta_{e} = 15#circ (RES only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_DIS_1p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (DIS only, 1p)", "E_{e} around #theta_{e} = 15#circ (DIS only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_15_All_Int_1p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1p"];
    std::string hE_e_15_QEL_1p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1p"];
    std::string hE_e_15_MEC_1p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1p"];
    std::string hE_e_15_RES_1p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1p"];
    std::string hE_e_15_DIS_1p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1p"];

    // E_e plots (1n)
    /* Energy (E_e) histograms (1n, CD & FD) */
    sE_e_1n_FD = OwnHistogram(new THStack("E_{e} (1n)", "E_{e} Histogram (1n);E_{e} [GeV]"));
    std::string sE_e_1n_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1n_stack"];

    hE_e_All_Int_1n_FD = OwnHistogram(new TH1D("E_{e} (All Int., 1n)", "E_{e} Histogram (All Int., 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_QEL_1n_FD = OwnHistogram(new TH1D("E_{e} (QEL Only, 1n)", "E_{e} Histogram (QEL Only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_MEC_1n_FD = OwnHistogram(new TH1D("E_{e} (MEC only, 1n)", "E_{e} Histogram (MEC only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_RES_1n_FD = OwnHistogram(new TH1D("E_{e} (RES only, 1n)", "E_{e} Histogram (RES only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_DIS_1n_FD = OwnHistogram(new TH1D("E_{e} (DIS only, 1n)", "E_{e} Histogram (DIS only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_All_Int_1n_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1n"];
    std::string hE_e_QEL_1n_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1n"];
    std::string hE_e_MEC_1n_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1n"];
    std::string hE_e_RES_1n_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1n"];
    std::string hE_e_DIS_1n_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1n"];

    /* E_e vs. Theta_e (1n, CD & FD) */
    hE_e_VS_Theta_e_All_Int_1n_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (All Int., 1n)", "E_{e} vs. #theta_{e} (All Int., 1n);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                             numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_QEL_1n_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (QEL Only, 1n)", "E_{e} vs. #theta_{e} (QEL Only, 1n);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_MEC_1n_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (MEC only, 1n)", "E_{e} vs. #theta_{e} (MEC only, 1n);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_RES_1n_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (RES only, 1n)", "E_{e} vs. #theta_{e} (RES only, 1n);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_DIS_1n_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (DIS only, 1n)", "E_{e} vs. #theta_{e} (DIS only, 1n);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_VS_Theta_e_All_Int_1n_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1n"];
    std::string hE_e_VS_Theta_e_QEL_1n_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1n"];
    std::string hE_e_VS_Theta_e_MEC_1n_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1n"];
    std::string hE_e_VS_Theta_e_RES_1n_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1n"];
    std::string hE_e_VS_Theta_e_DIS_1n_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_1n"];

    /* Energy (E_e) histograms around theta_e = 15 (1n, CD & FD) */
    sE_e_15_1n_FD = OwnHistogram(new THStack("E_{e} (1n)", "E_{e} Histogram (1n);E_{e} [GeV]"));
    std::string sE_e_15_1n_CD_Dir = directories.E_e_dir_map["E_e_All_Int_1n_stack"];
    std::string sE_e_15_1n_FD_Dir = directories.E_e_dir_map["E_e_All_Int_1n_stack"];

    hE_e_15_All_Int_1n_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., 1n)", "E_{e} around #theta_{e} = 15#circ (All Int., 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_QEL_1n_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, 1n)", "E_{e} around #theta_{e} = 15#circ (QEL Only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_MEC_1n_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (MEC only, 1n)", "E_{e} around #theta_{e} = 15#circ (MEC only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_RES_1n_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (RES only, 1n)", "E_{e} around #theta_{e} = 15#circ (RES only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_DIS_1n_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (DIS only, 1n)", "E_{e} around #theta_{e} = 15#circ (DIS only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_15_All_Int_1n_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1n"];
    std::string hE_e_15_QEL_1n_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1n"];
    std::string hE_e_15_MEC_1n_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1n"];
    std::string hE_e_15_RES_1n_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1n"];
    std::string hE_e_15_DIS_1n_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_1n"];

    // E_e plots (2p)
    /* Energy (E_e) histograms (2p, CD & FD) */
    sE_e_2p_FD = OwnHistogram(new THStack("E_{e} (2p)", "E_{e} Histogram (2p);E_{e} [GeV]"));
    std::string sE_e_2p_CD_Dir = directories.E_e_dir_map["E_e_All_Int_2p_stack"];
    std::string sE_e_2p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_2p_stack"];

    hE_e_All_Int_2p_FD = OwnHistogram(new TH1D("E_{e} (All Int., 2p)", "E_{e} Histogram (All Int., 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_QEL_2p_FD = OwnHistogram(new TH1D("E_{e} (QEL Only, 2p)", "E_{e} Histogram (QEL Only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_MEC_2p_FD = OwnHistogram(new TH1D("E_{e} (MEC only, 2p)", "E_{e} Histogram (MEC only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_RES_2p_FD = OwnHistogram(new TH1D("E_{e} (RES only, 2p)", "E_{e} Histogram (RES only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_DIS_2p_FD = OwnHistogram(new TH1D("E_{e} (DIS only, 2p)", "E_{e} Histogram (DIS only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_All_Int_2p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_2p"];
    std::string hE_e_QEL_2p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_2p"];
    std::string hE_e_MEC_2p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_2p"];
    std::string hE_e_RES_2p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_2p"];
    std::string hE_e_DIS_2p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_2p"];

    /* E_e vs. Theta_e (2p, CD & FD) */
    hE_e_VS_Theta_e_All_Int_2p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (All Int., 2p)", "E_{e} vs. #theta_{e} (All Int., 2p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                             numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_QEL_2p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (QEL Only, 2p)", "E_{e} vs. #theta_{e} (QEL Only, 2p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_MEC_2p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (MEC only, 2p)", "E_{e} vs. #theta_{e} (MEC only, 2p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_RES_2p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (RES only, 2p)", "E_{e} vs. #theta_{e} (RES only, 2p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_DIS_2p_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (DIS only, 2p)", "E_{e} vs. #theta_{e} (DIS only, 2p);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0, 50,
                                         numTH2Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_VS_Theta_e_All_Int_2p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_2p"];
    std::string hE_e_VS_Theta_e_QEL_2p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_2p"];
    std::string hE_e_VS_Theta_e_MEC_2p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_2p"];
    std::string hE_e_VS_Theta_e_RES_2p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_2p"];
    std::string hE_e_VS_Theta_e_DIS_2p_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_2p"];

    /* Energy (E_e) histograms around theta_e = 15 (2p, CD & FD) */
    sE_e_15_2p_FD = OwnHistogram(new THStack("E_{e} (2p)", "E_{e} Histogram (2p);E_{e} [GeV]"));
    std::string sE_e_15_2p_CD_Dir = directories.E_e_dir_map["E_e_All_Int_2p_stack"];
    std::string sE_e_15_2p_FD_Dir = directories.E_e_dir_map["E_e_All_Int_2p_stack"];

    hE_e_15_All_Int_2p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., 2p)", "E_{e} around #theta_{e} = 15#circ (All Int., 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_QEL_2p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, 2p)", "E_{e} around #theta_{e} = 15#circ (QEL Only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_MEC_2p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (MEC only, 2p)", "E_{e} around #theta_{e} = 15#circ (MEC only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_RES_2p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (RES only, 2p)", "E_{e} around #theta_{e} = 15#circ (RES only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_DIS_2p_FD = OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (DIS only, 2p)", "E_{e} around #theta_{e} = 15#circ (DIS only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_15_All_Int_2p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_2p"];
    std::string hE_e_15_QEL_2p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_2p"];
    std::string hE_e_15_MEC_2p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_2p"];
    std::string hE_e_15_RES_2p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_2p"];
    std::string hE_e_15_DIS_2p_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_2p"];

    // E_e plots (pFDpCD)
    /* Energy (E_e) histograms (pFDpCD, CD & FD) */
    sE_e_pFDpCD_FD = OwnHistogram(new THStack("E_{e} (pFDpCD)", "E_{e} Histogram (pFDpCD);E_{e} [GeV]"));
    std::string sE_e_pFDpCD_CD_Dir = directories.E_e_dir_map["E_e_All_Int_pFDpCD_stack"];
    std::string sE_e_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_pFDpCD_stack"];

    hE_e_All_Int_pFDpCD_FD = OwnHistogram(new TH1D("E_{e} (All Int., pFDpCD)", "E_{e} Histogram (All Int., pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_QEL_pFDpCD_FD = OwnHistogram(new TH1D("E_{e} (QEL Only, pFDpCD)", "E_{e} Histogram (QEL Only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_MEC_pFDpCD_FD = OwnHistogram(new TH1D("E_{e} (MEC only, pFDpCD)", "E_{e} Histogram (MEC only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_RES_pFDpCD_FD = OwnHistogram(new TH1D("E_{e} (RES only, pFDpCD)", "E_{e} Histogram (RES only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_DIS_pFDpCD_FD = OwnHistogram(new TH1D("E_{e} (DIS only, pFDpCD)", "E_{e} Histogram (DIS only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_All_Int_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_pFDpCD"];
    std::string hE_e_QEL_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_pFDpCD"];
    std::string hE_e_MEC_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_pFDpCD"];
    std::string hE_e_RES_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_pFDpCD"];
    std::string hE_e_DIS_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_pFDpCD"];

    /* E_e vs. Theta_e (pFDpCD, CD & FD) */
    hE_e_VS_Theta_e_All_Int_pFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (All Int., pFDpCD)", "E_{e} vs. #theta_{e} (All Int., pFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots,
                                                 0, 50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_QEL_pFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (QEL Only, pFDpCD)", "E_{e} vs. #theta_{e} (QEL Only, pFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_MEC_pFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (MEC only, pFDpCD)", "E_{e} vs. #theta_{e} (MEC only, pFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_RES_pFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (RES only, pFDpCD)", "E_{e} vs. #theta_{e} (RES only, pFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_DIS_pFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (DIS only, pFDpCD)", "E_{e} vs. #theta_{e} (DIS only, pFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_VS_Theta_e_All_Int_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_pFDpCD"];
    std::string hE_e_VS_Theta_e_QEL_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_pFDpCD"];
    std::string hE_e_VS_Theta_e_MEC_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_pFDpCD"];
    std::string hE_e_VS_Theta_e_RES_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_pFDpCD"];
    std::string hE_e_VS_Theta_e_DIS_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_pFDpCD"];

    /* Energy (E_e) histograms around theta_e = 15 (pFDpCD, CD & FD) */
    sE_e_15_pFDpCD_FD = OwnHistogram(new THStack("E_{e} (pFDpCD)", "E_{e} Histogram (pFDpCD);E_{e} [GeV]"));
    std::string sE_e_15_pFDpCD_CD_Dir = directories.E_e_dir_map["E_e_All_Int_pFDpCD_stack"];
    std::string sE_e_15_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_pFDpCD_stack"];

    hE_e_15_All_Int_pFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., pFDpCD)", "E_{e} around #theta_{e} = 15#circ (All Int., pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_QEL_pFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, pFDpCD)", "E_{e} around #theta_{e} = 15#circ (QEL Only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_MEC_pFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (MEC only, pFDpCD)", "E_{e} around #theta_{e} = 15#circ (MEC only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_RES_pFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (RES only, pFDpCD)", "E_{e} around #theta_{e} = 15#circ (RES only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_DIS_pFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (DIS only, pFDpCD)", "E_{e} around #theta_{e} = 15#circ (DIS only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_15_All_Int_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_pFDpCD"];
    std::string hE_e_15_QEL_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_pFDpCD"];
    std::string hE_e_15_MEC_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_pFDpCD"];
    std::string hE_e_15_RES_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_pFDpCD"];
    std::string hE_e_15_DIS_pFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_pFDpCD"];

    // E_e plots (nFDpCD)
    /* Energy (E_e) histograms (nFDpCD, CD & FD) */
    sE_e_nFDpCD_FD = OwnHistogram(new THStack("E_{e} (nFDpCD)", "E_{e} Histogram (nFDpCD);E_{e} [GeV]"));
    std::string sE_e_nFDpCD_CD_Dir = directories.E_e_dir_map["E_e_All_Int_nFDpCD_stack"];
    std::string sE_e_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_nFDpCD_stack"];

    hE_e_All_Int_nFDpCD_FD = OwnHistogram(new TH1D("E_{e} (All Int., nFDpCD)", "E_{e} Histogram (All Int., nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_QEL_nFDpCD_FD = OwnHistogram(new TH1D("E_{e} (QEL Only, nFDpCD)", "E_{e} Histogram (QEL Only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_MEC_nFDpCD_FD = OwnHistogram(new TH1D("E_{e} (MEC only, nFDpCD)", "E_{e} Histogram (MEC only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_RES_nFDpCD_FD = OwnHistogram(new TH1D("E_{e} (RES only, nFDpCD)", "E_{e} Histogram (RES only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_DIS_nFDpCD_FD = OwnHistogram(new TH1D("E_{e} (DIS only, nFDpCD)", "E_{e} Histogram (DIS only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_All_Int_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_nFDpCD"];
    std::string hE_e_QEL_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_nFDpCD"];
    std::string hE_e_MEC_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_nFDpCD"];
    std::string hE_e_RES_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_nFDpCD"];
    std::string hE_e_DIS_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_nFDpCD"];

    /* E_e vs. Theta_e (nFDpCD, CD & FD) */
    hE_e_VS_Theta_e_All_Int_nFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (All Int., nFDpCD)", "E_{e} vs. #theta_{e} (All Int., nFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots,
                                                 0, 50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_QEL_nFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (QEL Only, nFDpCD)", "E_{e} vs. #theta_{e} (QEL Only, nFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_MEC_nFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (MEC only, nFDpCD)", "E_{e} vs. #theta_{e} (MEC only, nFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_RES_nFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (RES only, nFDpCD)", "E_{e} vs. #theta_{e} (RES only, nFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    hE_e_VS_Theta_e_DIS_nFDpCD_FD = OwnHistogram(new TH2D("E_{e} vs. #theta_{e} (DIS only, nFDpCD)", "E_{e} vs. #theta_{e} (DIS only, nFDpCD);#theta_{e} [#circ];E_{e} [GeV]", numTH2Dbins_E_e_Plots, 0,
                                             50, numTH2Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_VS_Theta_e_All_Int_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_nFDpCD"];
    std::string hE_e_VS_Theta_e_QEL_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_nFDpCD"];
    std::string hE_e_VS_Theta_e_MEC_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_nFDpCD"];
    std::string hE_e_VS_Theta_e_RES_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_nFDpCD"];
    std::string hE_e_VS_Theta_e_DIS_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_VS_Theta_e_All_Int_nFDpCD"];

    /* Energy (E_e) histograms around theta_e = 15 (nFDpCD, CD & FD) */
    sE_e_15_nFDpCD_FD = OwnHistogram(new THStack("E_{e} (nFDpCD)", "E_{e} Histogram (nFDpCD);E_{e} [GeV]"));
    std::string sE_e_15_nFDpCD_CD_Dir = directories.E_e_dir_map["E_e_All_Int_nFDpCD_stack"];
    std::string sE_e_15_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_All_Int_nFDpCD_stack"];

    hE_e_15_All_Int_nFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., nFDpCD)", "E_{e} around #theta_{e} = 15#circ (All Int., nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_QEL_nFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, nFDpCD)", "E_{e} around #theta_{e} = 15#circ (QEL Only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_MEC_nFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (MEC only, nFDpCD)", "E_{e} around #theta_{e} = 15#circ (MEC only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_RES_nFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (RES only, nFDpCD)", "E_{e} around #theta_{e} = 15#circ (RES only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    hE_e_15_DIS_nFDpCD_FD =
        OwnHistogram(new TH1D("E_{e} around #theta_{e} = 15#circ (DIS only, nFDpCD)", "E_{e} around #theta_{e} = 15#circ (DIS only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamEMax));
    std::string hE_e_15_All_Int_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_nFDpCD"];
    std::string hE_e_15_QEL_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_nFDpCD"];
    std::string hE_e_15_MEC_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_nFDpCD"];
    std::string hE_e_15_RES_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_nFDpCD"];
    std::string hE_e_15_DIS_nFDpCD_FD_Dir = directories.E_e_dir_map["E_e_15_All_Int_nFDpCD"];

    BindReactionHistograms(hE_e_1e_cut_FD_by_reaction, sE_e_1e_cut, hE_e_All_Int_1e_cut_FD, hE_e_QEL_1e_cut_FD, hE_e_MEC_1e_cut_FD, hE_e_RES_1e_cut_FD, hE_e_DIS_1e_cut_FD);
    BindReactionHistograms(hE_e_1p_FD_by_reaction, sE_e_1p_FD, hE_e_All_Int_1p_FD, hE_e_QEL_1p_FD, hE_e_MEC_1p_FD, hE_e_RES_1p_FD, hE_e_DIS_1p_FD);
    BindReactionHistograms(hE_e_15_1p_FD_by_reaction, sE_e_15_1p_FD, hE_e_15_All_Int_1p_FD, hE_e_15_QEL_1p_FD, hE_e_15_MEC_1p_FD, hE_e_15_RES_1p_FD, hE_e_15_DIS_1p_FD);
    BindReactionHistograms(hE_e_1n_FD_by_reaction, sE_e_1n_FD, hE_e_All_Int_1n_FD, hE_e_QEL_1n_FD, hE_e_MEC_1n_FD, hE_e_RES_1n_FD, hE_e_DIS_1n_FD);
    BindReactionHistograms(hE_e_15_1n_FD_by_reaction, sE_e_15_1n_FD, hE_e_15_All_Int_1n_FD, hE_e_15_QEL_1n_FD, hE_e_15_MEC_1n_FD, hE_e_15_RES_1n_FD, hE_e_15_DIS_1n_FD);
    BindReactionHistograms(hE_e_2p_FD_by_reaction, sE_e_2p_FD, hE_e_All_Int_2p_FD, hE_e_QEL_2p_FD, hE_e_MEC_2p_FD, hE_e_RES_2p_FD, hE_e_DIS_2p_FD);
    BindReactionHistograms(hE_e_15_2p_FD_by_reaction, sE_e_15_2p_FD, hE_e_15_All_Int_2p_FD, hE_e_15_QEL_2p_FD, hE_e_15_MEC_2p_FD, hE_e_15_RES_2p_FD, hE_e_15_DIS_2p_FD);
    BindReactionHistograms(hE_e_pFDpCD_FD_by_reaction, sE_e_pFDpCD_FD, hE_e_All_Int_pFDpCD_FD, hE_e_QEL_pFDpCD_FD, hE_e_MEC_pFDpCD_FD, hE_e_RES_pFDpCD_FD, hE_e_DIS_pFDpCD_FD);
    BindReactionHistograms(hE_e_15_pFDpCD_FD_by_reaction, sE_e_15_pFDpCD_FD, hE_e_15_All_Int_pFDpCD_FD, hE_e_15_QEL_pFDpCD_FD, hE_e_15_MEC_pFDpCD_FD, hE_e_15_RES_pFDpCD_FD,
                           hE_e_15_DIS_pFDpCD_FD);
    BindReactionHistograms(hE_e_nFDpCD_FD_by_reaction, sE_e_nFDpCD_FD, hE_e_All_Int_nFDpCD_FD, hE_e_QEL_nFDpCD_FD, hE_e_MEC_nFDpCD_FD, hE_e_RES_nFDpCD_FD, hE_e_DIS_nFDpCD_FD);
    BindReactionHistograms(hE_e_15_nFDpCD_FD_by_reaction, sE_e_15_nFDpCD_FD, hE_e_15_All_Int_nFDpCD_FD, hE_e_15_QEL_nFDpCD_FD, hE_e_15_MEC_nFDpCD_FD, hE_e_15_RES_nFDpCD_FD,
                           hE_e_15_DIS_nFDpCD_FD);

    BindReactionHistograms(hE_e_VS_Theta_e_1e_cut_FD_by_reaction, hE_e_VS_Theta_e_All_Int_1e_cut_FD, hE_e_VS_Theta_e_QEL_1e_cut_FD, hE_e_VS_Theta_e_MEC_1e_cut_FD,
                           hE_e_VS_Theta_e_RES_1e_cut_FD, hE_e_VS_Theta_e_DIS_1e_cut_FD);
    BindReactionHistograms(hE_e_VS_Theta_e_1p_FD_by_reaction, hE_e_VS_Theta_e_All_Int_1p_FD, hE_e_VS_Theta_e_QEL_1p_FD, hE_e_VS_Theta_e_MEC_1p_FD, hE_e_VS_Theta_e_RES_1p_FD,
                           hE_e_VS_Theta_e_DIS_1p_FD);
    BindReactionHistograms(hE_e_VS_Theta_e_1n_FD_by_reaction, hE_e_VS_Theta_e_All_Int_1n_FD, hE_e_VS_Theta_e_QEL_1n_FD, hE_e_VS_Theta_e_MEC_1n_FD, hE_e_VS_Theta_e_RES_1n_FD,
                           hE_e_VS_Theta_e_DIS_1n_FD);
    BindReactionHistograms(hE_e_VS_Theta_e_2p_FD_by_reaction, hE_e_VS_Theta_e_All_Int_2p_FD, hE_e_VS_Theta_e_QEL_2p_FD, hE_e_VS_Theta_e_MEC_2p_FD, hE_e_VS_Theta_e_RES_2p_FD,
                           hE_e_VS_Theta_e_DIS_2p_FD);
    BindReactionHistograms(hE_e_VS_Theta_e_pFDpCD_FD_by_reaction, hE_e_VS_Theta_e_All_Int_pFDpCD_FD, hE_e_VS_Theta_e_QEL_pFDpCD_FD, hE_e_VS_Theta_e_MEC_pFDpCD_FD,
                           hE_e_VS_Theta_e_RES_pFDpCD_FD, hE_e_VS_Theta_e_DIS_pFDpCD_FD);
    BindReactionHistograms(hE_e_VS_Theta_e_nFDpCD_FD_by_reaction, hE_e_VS_Theta_e_All_Int_nFDpCD_FD, hE_e_VS_Theta_e_QEL_nFDpCD_FD, hE_e_VS_Theta_e_MEC_nFDpCD_FD,
                           hE_e_VS_Theta_e_RES_nFDpCD_FD, hE_e_VS_Theta_e_DIS_nFDpCD_FD);

#pragma endregion
}

void FinalStateHistograms::InitResolutionHistograms(const HistogramDefinitionSettings& hDefSettings) {
    auto& directories = resolution_directories_;
    const int numTH1Dbins_nRes_Plots = hDefSettings.numTH1Dbins_nRes_Plots;
    const int numTH2Dbins_nRes_Plots = hDefSettings.numTH2Dbins_nRes_Plots;
    const int numTH1Dbins_ReacMon_plots = hDefSettings.numTH1Dbins_ReacMon_plots;
    const int numTH2Dbins = hDefSettings.numTH2Dbins;
    const double beamE110 = hDefSettings.Momentum_ulim;
    const double Momentum_llim = hDefSettings.Momentum_llim;
    const double Momentum_ulim = hDefSettings.Momentum_ulim;
    const double Theta_llim_FD = hDefSettings.Theta_llim_FD;
    const double Theta_ulim_FD = hDefSettings.Theta_ulim_FD;
    const double Phi_llim = hDefSettings.Phi_llim;
    const double Phi_ulim = hDefSettings.Phi_ulim;
    const double Vz_lim = hDefSettings.Vz_lim;
    const double FD_nucleon_momentum_cut_llim = hDefSettings.FD_nucleon_momentum_cut_llim;
    const double FD_nucleon_momentum_cut_ulim = hDefSettings.FD_nucleon_momentum_cut_ulim;

#pragma region /* Resolution histograms */

    // Resolution histograms (1p)

    // Match monitoring plots (1p)
    hdTheta_pFD_TL_BC_1p = hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} BC", "#Delta#theta_{pFD} of FD proton BC", "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [#circ]",
                                   directories.Resolution_dir_map["Monitoring_1p"], "00a_DeltaTheta_pFD_BC_TL_1p", -Theta_ulim_FD, Theta_ulim_FD, numTH1Dbins_nRes_Plots);
    hdTheta_pFD_TL_ZOOMIN_BC_1p =
        hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} - ZOOMIN BC", "#Delta#theta_{pFD} of FD proton - ZOOMIN BC", "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_1p"], "00b_DeltaTheta_pFD_BC_TL_ZOOMIN_1p", -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hdPhi_pFD_TL_BC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} BC", "#Delta#phi_{pFD} of FD proton BC", "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [#circ]",
                                 directories.Resolution_dir_map["Monitoring_1p"], "00c_DeltaPhi_pFD_BC_TL_1p", Phi_llim, Phi_ulim, numTH1Dbins_nRes_Plots);
    hdPhi_pFD_TL_ZOOMIN_BC_1p =
        hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} - ZOOMIN BC", "#Delta#phi_{pFD} of FD proton - ZOOMIN BC", "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_1p"], "00d_DeltaPhi_pFD_BC_TL_ZOOMIN_1p", -20, 20, numTH1Dbins_nRes_Plots);

    hdTheta_pFD_TL_AdPC_1p =
        hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} AdPC", "#Delta#theta_{pFD} of FD proton AdPC", "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_1p"], "00e_DeltaTheta_pFD_AdPC_TL_1p", -Theta_ulim_FD, Theta_ulim_FD, numTH1Dbins_nRes_Plots);
    hdTheta_pFD_TL_ZOOMIN_AdPC_1p =
        hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} - ZOOMIN AdPC", "#Delta#theta_{pFD} of FD proton - ZOOMIN AdPC", "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_1p"], "00f_DeltaTheta_pFD_AdPC_TL_ZOOMIN_1p", -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hdPhi_pFD_TL_AdPC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} AdPC", "#Delta#phi_{pFD} of FD proton AdPC", "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [#circ]",
                                   directories.Resolution_dir_map["Monitoring_1p"], "00g_DeltaPhi_pFD_AdPC_TL_1p", Phi_llim, Phi_ulim, numTH1Dbins_nRes_Plots);
    hdPhi_pFD_TL_ZOOMIN_AdPC_1p =
        hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} - ZOOMIN AdPC", "#Delta#phi_{pFD} of FD proton - ZOOMIN AdPC", "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_1p"], "00h_DeltaPhi_pFD_AdPC_TL_ZOOMIN_1p", -20, 20, numTH1Dbins_nRes_Plots);

    hTheta_pFD_TL_ApResC_1p = hPlot1D("1p", "", "TL #theta^{truth}_{pFD} ApResC", "#theta^{truth}_{pFD} of FD proton ApResC", "#theta^{truth}_{pFD} [#circ]",
                                      directories.Resolution_dir_map["Monitoring_1p"], "01a_Theta_pFD_ApResC_TL_1p", Theta_llim_FD, Theta_ulim_FD, numTH1Dbins_nRes_Plots);
    hPhi_pFD_TL_ApResC_1p = hPlot1D("1p", "", "TL #phi^{truth}_{pFD} ApResC", "#phi^{truth}_{pFD} of FD proton ApResC", "#phi^{truth}_{pFD} [#circ]",
                                    directories.Resolution_dir_map["Monitoring_1p"], "02a_Phi_pFD_ApResC_TL_1p", Phi_llim, Phi_ulim, numTH1Dbins_nRes_Plots);
    hTheta_pFD_TL_VS_Phi_pFD_TL_ApResC_1p = OwnHistogram(new TH2D("#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} ApResC (1p, FD)",
                                                     "#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} ApResC (1p, FD);#phi^{truth}_{pFD} [#circ];#theta^{truth}_{pFD} [#circ]",
                                                     numTH2Dbins_nRes_Plots, Phi_llim, Phi_ulim, numTH2Dbins_nRes_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_pFD_TL_VS_Phi_pFD_TL_ApResC_1p_Dir = directories.Resolution_dir_map["Monitoring_1p"];

    hTheta_pFD_TL_MatchedP_1p = hPlot1D("1p", "", "TL #theta^{truth}_{pFD} of matched TL p", "#theta^{truth}_{pFD} of matched TL FD proton", "#theta^{truth}_{pFD} [#circ]",
                                        directories.Resolution_dir_map["Monitoring_1p"], "01b_Theta_pFD_MatchedN_TL_1p", Theta_llim_FD, Theta_ulim_FD, numTH1Dbins_nRes_Plots);
    hPhi_pFD_TL_MatchedP_1p = hPlot1D("1p", "", "TL #phi^{truth}_{pFD} of matched TL p", "#phi^{truth}_{pFD} of matched TL FD proton", "#phi^{truth}_{pFD} [#circ]",
                                      directories.Resolution_dir_map["Monitoring_1p"], "02b_Phi_pFD_MatchedN_TL_1p", Phi_llim, Phi_ulim, numTH1Dbins_nRes_Plots);
    hTheta_pFD_TL_VS_Phi_pFD_TL_MatchedP_1p = OwnHistogram(new TH2D("#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} of matched TL p (1p, FD)",
                                                       "#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} of matched TL FD proton (1p, FD);#phi^{truth}_{pFD} [#circ];#theta^{truth}_{pFD} [#circ]",
                                                       numTH2Dbins_nRes_Plots, Phi_llim, Phi_ulim, numTH2Dbins_nRes_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_pFD_TL_VS_Phi_pFD_TL_MatchedP_1p_Dir = directories.Resolution_dir_map["Monitoring_1p"];

    hP_pFD_Res_1p = hPlot1D("1p", "", "R_{pFD}", "FD neutron R_{pFD}", "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}", directories.Resolution_dir_map["Resolution_1p"],
                            "04_P_pFD_Res_1p", -2, 2, numTH1Dbins_nRes_Plots);
    hP_pFD_Res_VS_TL_P_pFD_1p = OwnHistogram(new TH2D("R_{pFD} vs. P^{truth}_{pFD} (1p, FD)",
                                         "R_{pFD} vs. P^{truth}_{pFD} (1p, FD);P^{truth}_{pFD} [GeV/c];"
                                         "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                         numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_pFD_Res_VS_TL_P_pFD_ZOOMIN_1p = OwnHistogram(new TH2D("R_{pFD} vs. P^{truth}_{pFD} - ZOOMIN (1p, FD)",
                                                "R_{pFD} vs. P^{truth}_{pFD} - ZOOMIN (1p, FD);P^{truth}_{pFD} [GeV/c];"
                                                "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                                numTH2Dbins_nRes_Plots, FD_nucleon_momentum_cut_llim, FD_nucleon_momentum_cut_ulim, numTH2Dbins_nRes_Plots, -0.75, 0.75));
    hP_pFD_Res_VS_TL_P_pFD_noKC_1p = OwnHistogram(new TH2D("R_{pFD} vs. P^{truth}_{pFD} no mom. KC (1p, FD)",
                                              "R_{pFD} vs. P^{truth}_{pFD} no mom. KC (1p, FD);P^{truth}_{pFD} [GeV/c];"
                                              "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                              numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_pFD_Res_VS_Reco_P_pFD_1p = OwnHistogram(new TH2D("R_{pFD} vs. P^{reco}_{pFD} (1p, FD)",
                                           "R_{pFD} vs. P^{reco}_{pFD} (1p, FD);P^{reco}_{pFD} [GeV/c];"
                                           "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                           numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_pFD_Res_VS_Reco_P_pFD_ZOOMIN_1p = OwnHistogram(new TH2D("R_{pFD} vs. P^{reco}_{pFD} - ZOOMIN (1p, FD)",
                                                  "R_{pFD} vs. P^{reco}_{pFD} - ZOOMIN (1p, FD);P^{reco}_{pFD} [GeV/c];"
                                                  "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                                  numTH2Dbins_nRes_Plots, FD_nucleon_momentum_cut_llim, FD_nucleon_momentum_cut_ulim, numTH2Dbins_nRes_Plots, -0.75, 0.75));
    hP_pFD_Res_VS_Reco_P_pFD_noKC_1p = OwnHistogram(new TH2D("R_{pFD} vs. P^{reco}_{pFD} no mom. KC (1p, FD)",
                                                "R_{pFD} vs. P^{reco}_{pFD} no mom. KC (1p, FD);P^{reco}_{pFD} [GeV/c];"
                                                "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                                numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_pFD_Res_VS_Smear_Reco_P_pFD_1p = OwnHistogram(new TH2D("R_{pFD} vs. smeared P^{reco}_{pFD} (1p, FD)",
                                                 "R_{pFD} vs. smeared P^{reco}_{pFD} (1p, FD);Smeared P^{reco}_{pFD} [GeV/c];"
                                                 "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                                 numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_pFD_Res_VS_Smear_Reco_P_pFD_ZOOMIN_1p = OwnHistogram(new TH2D("R_{pFD} vs. smeared P^{reco}_{pFD} - ZOOMIN (1p, FD)",
                                                        "R_{pFD} vs. smeared P^{reco}_{pFD} - ZOOMIN (1p, FD);Smeared P^{reco}_{pFD} [GeV/c];"
                                                        "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                                        numTH2Dbins_nRes_Plots, FD_nucleon_momentum_cut_llim, FD_nucleon_momentum_cut_ulim, numTH2Dbins_nRes_Plots, -0.75, 0.75));
    hP_pFD_Res_VS_Smear_Reco_P_pFD_noKC_1p = OwnHistogram(new TH2D("R_{pFD} vs. smeared P^{reco}_{pFD} no mom. KC (1p, FD)",
                                                      "R_{pFD} vs. smeared P^{reco}_{pFD} no mom. KC (1p, FD);Smeared P^{reco}_{pFD} [GeV/c];"
                                                      "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                                      numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    std::string hP_pFD_Res_VS_TL_P_pFD_1p_Dir = directories.Resolution_dir_map["Resolution_1p"];
    std::string hP_pFD_Res_VS_Reco_P_pFD_1p_Dir = directories.Resolution_dir_map["Resolution_1p"];

    // Match multiplicity (1p)
    hpRes_Match_Multi_1p = hPlot1D("1p", "FD", "Proton resolution match multiplicity", "Proton resolution match multiplicity", "Match multiplicity",
                                   directories.Resolution_dir_map["Match_multi_1p"], "01_pRes_Match_Multi_1p", 0.0, 10., 10);
    hpRes_Match_Multi_vs_Reco_P_pFD_1p = hPlot2D("1p", "FD", "Match multiplicity vs. P^{reco}_{pFD}", "Match multiplicity vs. P^{reco}_{pFD}", "Match multiplicity", "P^{reco}_{pFD} [GeV/c]",
                                                 directories.Resolution_dir_map["Match_multi_1p"], "02_pRes_Match_Multi_vs_Reco_P_pFD_1p", 0.0, 10., Momentum_llim, Momentum_ulim, 10, 50);
    hpRes_Match_Multi_vs_Reco_Theta_pFD_1p =
        hPlot2D("1p", "FD", "Match multiplicity vs. #theta^{reco}_{pFD}", "Match multiplicity vs. #theta^{reco}_{pFD}", "Match multiplicity", "#theta^{reco}_{pFD} [#circ]",
                directories.Resolution_dir_map["Match_multi_1p"], "03_pRes_Match_Multi_vs_Reco_Theta_pFD_1p", 0.0, 10., Theta_llim_FD, Theta_ulim_FD, 10, 50);
    hpRes_Match_Multi_vs_Reco_Phi_pFD_1p =
        hPlot2D("1p", "FD", "Match multiplicity vs. #phi^{reco}_{pFD}", "Match multiplicity vs. #phi^{reco}_{pFD}", "Match multiplicity", "#phi^{reco}_{pFD} [#circ]",
                directories.Resolution_dir_map["Match_multi_1p"], "04_pRes_Match_Multi_vs_Reco_Phi_pFD_1p", 0.0, 10., Phi_llim, Phi_ulim, 10, 50);

    hReco_P_pFD_pRes_1p = hPlot1D("1p", "FD", "Reco P_{pFD} used in pRes", "Reco P_{pFD} used in pRes", "P_{pFD} [GeV/c]", directories.Resolution_dir_map["Resolution_1p"],
                                  "06a_Reco_P_pFD_used_in_pRes_1p", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hTL_P_pFD_pRes_1p = hPlot1D("1p", "FD", "TL P_{pFD} used in pRes", "TL P_{pFD} used in pRes", "P_{pFD} [GeV/c]", directories.Resolution_dir_map["Resolution_1p"],
                                "06b_TL_P_pFD_used_in_pRes_1p", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);

    hTL_P_pFD_vs_Reco_P_pFD_1p = hPlot2D("1p", "FD", "P^{truth}_{pFD} vs. P^{reco}_{pFD}", "P^{truth}_{pFD} vs. P^{reco}_{pFD}", "P^{truth}_{pFD} [GeV/c]", "P^{reco}_{pFD} [GeV/c]",
                                         directories.Resolution_dir_map["Resolution_1p"], "00XX_TL_P_pFD_vs_Reco_P_pFD_1p", Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim,
                                         numTH2Dbins_nRes_Plots, numTH2Dbins_nRes_Plots);
    hTL_P_pFD_vs_Smear_Reco_P_pFD_1p = hPlot2D("1p", "FD", "P^{truth}_{pFD} vs. smeared P^{reco}_{pFD}", "P^{truth}_{pFD} vs. smeared P^{reco}_{pFD}", "P^{truth}_{pFD} [GeV/c]",
                                               "Smeared P^{reco}_{pFD} [GeV/c]", directories.Resolution_dir_map["Resolution_1p"], "00XX_TL_P_pFD_vs_Reco_P_pFD_1p", Momentum_llim,
                                               Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_nRes_Plots, numTH2Dbins_nRes_Plots);

    // Resolution histograms (1n)
#pragma region /* Resolution histograms (1n) */

#pragma region /* Match monitoring plots (1n) */

    /* Resolution histograms of neutrons passing nRes cuts, but before matching cuts */

    hdTheta_nFD_TL_BC_1n = hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} BC", "#Delta#theta_{nFD} of FD neutron BC", "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [#circ]",
                                   directories.Resolution_dir_map["Monitoring_Before_matching_cuts_1n"], "01_DeltaTheta_nFD_BC_TL_1n", -Theta_ulim_FD, Theta_ulim_FD, numTH1Dbins_nRes_Plots);
    hdTheta_nFD_TL_ZOOMIN_BC_1n =
        hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} - ZOOMIN BC", "#Delta#theta_{nFD} of FD neutron - ZOOMIN BC", "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_Before_matching_cuts_1n"], "02_DeltaTheta_nFD_BC_TL_ZOOMIN_1n", -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hdPhi_nFD_TL_BC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} BC", "#Delta#phi_{nFD} of FD neutron BC", "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [#circ]",
                                 directories.Resolution_dir_map["Monitoring_Before_matching_cuts_1n"], "03_DeltaPhi_nFD_BC_TL_1n", Phi_llim, Phi_ulim, numTH1Dbins_nRes_Plots);
    hdPhi_nFD_TL_ZOOMIN_BC_1n =
        hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} - ZOOMIN BC", "#Delta#phi_{nFD} of FD neutron - ZOOMIN BC", "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_Before_matching_cuts_1n"], "04_DeltaPhi_nFD_BC_TL_ZOOMIN_1n", -20, 20, numTH1Dbins_nRes_Plots);

    hTheta_nFD_TL_AnResC_1n =
        hPlot1D("1n", "", "TL #theta^{truth}_{nFD} AnResC", "#theta^{truth}_{nFD} of FD neutron AnResC", "#theta^{truth}_{nFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_Before_matching_cuts_1n"], "05a_Theta_nFD_AnResC_TL_1n", Theta_llim_FD, Theta_ulim_FD, numTH1Dbins_nRes_Plots);
    hPhi_nFD_TL_AnResC_1n = hPlot1D("1n", "", "TL #phi^{truth}_{nFD} AnResC", "#phi^{truth}_{nFD} of FD neutron AnResC", "#phi^{truth}_{nFD} [#circ]",
                                    directories.Resolution_dir_map["Monitoring_Before_matching_cuts_1n"], "05b_Phi_nFD_AnResC_TL_1n", Phi_llim, Phi_ulim, numTH1Dbins_nRes_Plots);
    hTheta_nFD_TL_VS_Phi_nFD_TL_AnResC_1n = OwnHistogram(new TH2D("#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} AnResC (1n, FD)",
                                                     "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} AnResC (1n, FD);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]",
                                                     numTH2Dbins_nRes_Plots, Phi_llim, Phi_ulim, numTH2Dbins_nRes_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_nFD_TL_VS_Phi_nFD_TL_AnResC_1n_Dir = directories.Resolution_dir_map["Monitoring_Before_matching_cuts_1n"];

    /* Resolution histograms of neutrons passing nRes cuts and nRes_Pass_dPhiCut */

    hdTheta_nFD_TL_AdPC_1n =
        hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} AdPC", "#Delta#theta_{nFD} of FD neutron AdPC", "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_After_only_dPhi_cuts_1n"], "01_DeltaTheta_nFD_AdPC_TL_1n", -Theta_ulim_FD, Theta_ulim_FD, numTH1Dbins_nRes_Plots);
    hdTheta_nFD_TL_ZOOMIN_AdPC_1n =
        hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} - ZOOMIN AdPC", "#Delta#theta_{nFD} of FD neutron - ZOOMIN AdPC", "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_After_only_dPhi_cuts_1n"], "02_DeltaTheta_nFD_AdPC_TL_ZOOMIN_1n", -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hdPhi_nFD_TL_AdPC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} AdPC", "#Delta#phi_{nFD} of FD neutron AdPC", "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [#circ]",
                                   directories.Resolution_dir_map["Monitoring_After_only_dPhi_cuts_1n"], "03_DeltaPhi_nFD_AdPC_TL_1n", Phi_llim, Phi_ulim, numTH1Dbins_nRes_Plots);
    hdPhi_nFD_TL_ZOOMIN_AdPC_1n =
        hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} - ZOOMIN AdPC", "#Delta#phi_{nFD} of FD neutron - ZOOMIN AdPC", "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_After_only_dPhi_cuts_1n"], "04_DeltaPhi_nFD_AdPC_TL_ZOOMIN_1n", -20, 20, numTH1Dbins_nRes_Plots);

    /* Resolution histograms of neutrons passing nRes cuts and matching cuts */

    hTheta_nFD_TL_MatchedN_1n =
        hPlot1D("1n", "", "TL #theta^{truth}_{nFD} of matched TL n", "#theta^{truth}_{nFD} of matched TL FD neutron", "#theta^{truth}_{nFD} [#circ]",
                directories.Resolution_dir_map["Monitoring_After_matching_cuts_1n"], "01_Theta_nFD_MatchedN_TL_1n", Theta_llim_FD, Theta_ulim_FD, numTH1Dbins_nRes_Plots);
    hPhi_nFD_TL_MatchedN_1n = hPlot1D("1n", "", "TL #phi^{truth}_{nFD} of matched TL n", "#phi^{truth}_{nFD} of matched TL FD neutron", "#phi^{truth}_{nFD} [#circ]",
                                      directories.Resolution_dir_map["Monitoring_After_matching_cuts_1n"], "02_Phi_nFD_MatchedN_TL_1n", Phi_llim, Phi_ulim, numTH1Dbins_nRes_Plots);
    hTheta_nFD_TL_VS_Phi_nFD_TL_MatchedN_1n =
        OwnHistogram(new TH2D("#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} of matched TL n (1n, FD)",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} of matched TL FD neutron (1n, FD);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", numTH2Dbins_nRes_Plots, Phi_llim,
                 Phi_ulim, numTH2Dbins_nRes_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_nFD_TL_VS_Phi_nFD_TL_MatchedN_1n_Dir = directories.Resolution_dir_map["Monitoring_After_matching_cuts_1n"];
#pragma endregion

#pragma region /* Resolution momentum plots */

    hP_nFD_Res_VS_TL_P_nFD_1n = OwnHistogram(new TH2D("R_{nFD} vs. P^{truth}_{nFD} (1n, FD)",
                                         "R_{nFD} vs. P^{truth}_{nFD} (1n, FD);P^{truth}_{nFD} [GeV/c];"
                                         "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                         numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_TL_P_nFD_ZOOMIN_1n = OwnHistogram(new TH2D("R_{nFD} vs. P^{truth}_{nFD} - ZOOMIN (1n, FD)",
                                                "R_{nFD} vs. P^{truth}_{nFD} - ZOOMIN (1n, FD);P^{truth}_{nFD} [GeV/c];"
                                                "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                                numTH2Dbins_nRes_Plots, FD_nucleon_momentum_cut_llim, FD_nucleon_momentum_cut_ulim, numTH2Dbins_nRes_Plots, -0.75, 0.75));
    hP_nFD_Res_VS_TL_P_nFD_noKC_1n = OwnHistogram(new TH2D("R_{nFD} vs. P^{truth}_{nFD} no mom. KC (1n, FD)",
                                              "R_{nFD} vs. P^{truth}_{nFD} no mom. KC (1n, FD);P^{truth}_{nFD} [GeV/c];"
                                              "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                              numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Reco_P_nFD_1n = OwnHistogram(new TH2D("R_{nFD} vs. P^{reco}_{nFD} (1n, FD)",
                                           "R_{nFD} vs. P^{reco}_{nFD} (1n, FD);P^{reco}_{nFD} [GeV/c];"
                                           "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                           numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Reco_P_nFD_ZOOMIN_1n = OwnHistogram(new TH2D("R_{nFD} vs. P^{reco}_{nFD} - ZOOMIN (1n, FD)",
                                                  "R_{nFD} vs. P^{reco}_{nFD} - ZOOMIN (1n, FD);P^{reco}_{nFD} [GeV/c];"
                                                  "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                                  numTH2Dbins_nRes_Plots, FD_nucleon_momentum_cut_llim, FD_nucleon_momentum_cut_ulim, numTH2Dbins_nRes_Plots, -0.75, 0.75));
    hP_nFD_Res_VS_Reco_P_nFD_noKC_1n = OwnHistogram(new TH2D("R_{nFD} vs. P^{reco}_{nFD} no mom. KC (1n, FD)",
                                                "R_{nFD} vs. P^{reco}_{nFD} no mom. KC (1n, FD);P^{reco}_{nFD} [GeV/c];"
                                                "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                                numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Corr_Reco_P_nFD_1n = OwnHistogram(new TH2D("R_{nFD} vs. corrected P^{reco}_{nFD} (1n, FD)",
                                                "R_{nFD} vs. corrected P^{reco}_{nFD} (1n, FD);Corrected P^{reco}_{nFD} [GeV/c];"
                                                "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                                numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Corr_Reco_P_nFD_ZOOMIN_1n = OwnHistogram(new TH2D("R_{nFD} vs. corrected P^{reco}_{nFD} - ZOOMIN (1n, FD)",
                                                       "R_{nFD} vs. corrected P^{reco}_{nFD} - ZOOMIN (1n, FD);Corrected P^{reco}_{nFD} [GeV/c];"
                                                       "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                                       numTH2Dbins_nRes_Plots, FD_nucleon_momentum_cut_llim, FD_nucleon_momentum_cut_ulim, numTH2Dbins_nRes_Plots, -0.75, 0.75));
    hP_nFD_Res_VS_Corr_Reco_P_nFD_noKC_1n = OwnHistogram(new TH2D("R_{nFD} vs. corrected P^{reco}_{nFD} no mom. KC (1n, FD)",
                                                     "R_{nFD} vs. corrected P^{reco}_{nFD} no mom. KC (1n, FD);Corrected P^{reco}_{nFD} [GeV/c];"
                                                     "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                                     numTH2Dbins_nRes_Plots, 0, beamE110, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    std::string hP_nFD_Res_VS_TL_P_nFD_1n_Dir = directories.Resolution_dir_map["Resolution_mom_plots_1n"];
    std::string hP_nFD_Res_VS_Reco_P_nFD_1n_Dir = directories.Resolution_dir_map["Resolution_mom_plots_1n"];

    hReco_P_nFD_nRes_1n = hPlot1D("1n", "FD", "Reco P_{nFD} used in nRes", "Reco P_{nFD} used in nRes", "P_{nFD} [GeV/c]", directories.Resolution_dir_map["Resolution_mom_plots_1n"],
                                  "04a_Reco_P_nFD_used_in_nRes_1n", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hTL_P_nFD_nRes_1n = hPlot1D("1n", "FD", "TL P_{nFD} used in nRes", "TL P_{nFD} used in nRes", "P_{nFD} [GeV/c]", directories.Resolution_dir_map["Resolution_mom_plots_1n"],
                                "04b_TL_P_nFD_used_in_nRes_1n", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);

    hTL_P_nFD_vs_Reco_P_nFD_1n = hPlot2D("1n", "FD", "P^{truth}_{nFD} vs. P^{reco}_{nFD}", "P^{truth}_{nFD} vs. P^{reco}_{nFD}", "P^{truth}_{nFD} [GeV/c]", "P^{reco}_{nFD} [GeV/c]",
                                         directories.Resolution_dir_map["Resolution_mom_plots_1n"], "05a_TL_P_nFD_vs_Reco_P_nFD_1n", Momentum_llim, Momentum_ulim, Momentum_llim,
                                         Momentum_ulim, numTH2Dbins_nRes_Plots, numTH2Dbins_nRes_Plots);
    hTL_P_nFD_vs_Corr_Reco_P_nFD_1n = hPlot2D("1n", "FD", "P^{truth}_{nFD} vs. corrected P^{reco}_{nFD}", "P^{truth}_{nFD} vs. corrected P^{reco}_{nFD}", "P^{truth}_{nFD} [GeV/c]",
                                              "Corrected P^{reco}_{nFD} [GeV/c]", directories.Resolution_dir_map["Resolution_mom_plots_1n"], "05b_TL_P_nFD_vs_Reco_P_nFD_1n", Momentum_llim,
                                              Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_nRes_Plots, numTH2Dbins_nRes_Plots);
#pragma endregion

#pragma region /* Resolution status plots */

    hP_nFD_Res_VS_e_status_1n = OwnHistogram(new TH2D("R_{nFD} vs. electron status (1n, FD)",
                                         "R_{nFD} vs. electron status (1n, FD);Electron status;"
                                         "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                         numTH2Dbins_nRes_Plots, -2300., -2000., numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_nFD_status_1n = OwnHistogram(new TH2D("R_{nFD} vs. neutron status (1n, FD)",
                                           "R_{nFD} vs. neutron status (1n, FD);Neutron status;"
                                           "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                           numTH2Dbins_nRes_Plots, 1900., 2300., numTH2Dbins_nRes_Plots, -1.1, 1.1));
    std::string hP_nFD_Res_VS_status_1n_Dir = directories.Resolution_dir_map["Resolution_status_plots_1n"];
#pragma endregion

#pragma region /* Resolution vertex plots */

    hP_nFD_Res_VS_Vx_e_1n = OwnHistogram(new TH2D("R_{nFD} vs. V_{x,e} (1n, FD)",
                                     "R_{nFD} vs. V_{x,e} (1n, FD);V_{x,e} [cm];"
                                     "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                     numTH2Dbins_nRes_Plots, -Vz_lim, Vz_lim, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Vy_e_1n = OwnHistogram(new TH2D("R_{nFD} vs. V_{y,e} (1n, FD)",
                                     "R_{nFD} vs. V_{y,e} (1n, FD);V_{y,e} [cm];"
                                     "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                     numTH2Dbins_nRes_Plots, -Vz_lim, Vz_lim, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Vz_e_1n = OwnHistogram(new TH2D("R_{nFD} vs. V_{z,e} (1n, FD)",
                                     "R_{nFD} vs. V_{z,e} (1n, FD);V_{z,e} [cm];"
                                     "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                     numTH2Dbins_nRes_Plots, -Vz_lim, Vz_lim, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    std::string hP_nFD_Res_VS_Vtx_e_1n_Dir = directories.Resolution_dir_map["Resolution_vtx_plots_1n"];
#pragma endregion

#pragma region /* Resolution hit coordinates plots */

    hP_nFD_Res_VS_Hitx_1n = OwnHistogram(new TH2D("R_{nFD} vs. Hit_{x} (1n, FD)",
                                     "R_{nFD} vs. Hit_{x} (1n, FD);Hit_{x} [cm];"
                                     "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                     numTH2Dbins_nRes_Plots, -500, 500, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Hity_1n = OwnHistogram(new TH2D("R_{nFD} vs. Hit_{y} (1n, FD)",
                                     "R_{nFD} vs. Hit_{y} (1n, FD);Hit_{y} [cm];"
                                     "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                     numTH2Dbins_nRes_Plots, -500, 500, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Hitz_1n = OwnHistogram(new TH2D("R_{nFD} vs. Hit_{z} (1n, FD)",
                                     "R_{nFD} vs. Hit_{z} (1n, FD);Hit_{z} [cm];"
                                     "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                     numTH2Dbins_nRes_Plots, -500, 500, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    std::string hP_nFD_Res_VS_Hcorr_1n_Dir = directories.Resolution_dir_map["Resolution_Hcorr_plots_1n"];

#pragma region /* Resolution matched hit coordinates plots */

    hP_nFD_Res_VS_Hx_1n = OwnHistogram(new TH2D("R_{nFD} vs. H_{x} (1n, FD)",
                                   "R_{nFD} vs. H_{x} (1n, FD);H_{x} [cm];"
                                   "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                   numTH2Dbins_nRes_Plots, -20, 20, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Hy_1n = OwnHistogram(new TH2D("R_{nFD} vs. H_{y} (1n, FD)",
                                   "R_{nFD} vs. H_{y} (1n, FD);H_{y} [cm];"
                                   "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                   numTH2Dbins_nRes_Plots, -20, 20, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Hz_1n = OwnHistogram(new TH2D("R_{nFD} vs. H_{z} (1n, FD)",
                                   "R_{nFD} vs. H_{z} (1n, FD);H_{z} [cm];"
                                   "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                   numTH2Dbins_nRes_Plots, -20, 20, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    std::string hP_nFD_Res_VS_matchedHcorr_1n_Dir = directories.Resolution_dir_map["Resolution_matchedHcorr_plots_1n"];
#pragma endregion

#pragma region /* Resolution ECAL coordinates plots */

    hP_nFD_Res_VS_Lu_1n = OwnHistogram(new TH2D("R_{nFD} vs. L_{u} (1n, FD)",
                                   "R_{nFD} vs. L_{u} (1n, FD);L_{u} [cm];"
                                   "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                   numTH2Dbins_nRes_Plots, 0, 500, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Lv_1n = OwnHistogram(new TH2D("R_{nFD} vs. L_{v} (1n, FD)",
                                   "R_{nFD} vs. L_{v} (1n, FD);L_{v} [cm];"
                                   "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                   numTH2Dbins_nRes_Plots, 0, 500, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Lw_1n = OwnHistogram(new TH2D("R_{nFD} vs. L_{w} (1n, FD)",
                                   "R_{nFD} vs. L_{w} (1n, FD);L_{w} [cm];"
                                   "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                   numTH2Dbins_nRes_Plots, 0, 500, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    std::string hP_nFD_Res_VS_ECALcorr_1n_Dir = directories.Resolution_dir_map["Resolution_ECALcorr_plots_1n"];
#pragma endregion

#pragma region /* Resolution shower width plots */

    hP_nFD_Res_VS_Du_1n = OwnHistogram(new TH2D("R_{nFD} vs. D_{u} (1n, FD)",
                                   "R_{nFD} vs. D_{u} (1n, FD);D_{u} [cm];"
                                   "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                   numTH2Dbins_nRes_Plots, 0, 25, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Dv_1n = OwnHistogram(new TH2D("R_{nFD} vs. D_{v} (1n, FD)",
                                   "R_{nFD} vs. D_{v} (1n, FD);D_{v} [cm];"
                                   "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                   numTH2Dbins_nRes_Plots, 0, 25, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_Dw_1n = OwnHistogram(new TH2D("R_{nFD} vs. D_{w} (1n, FD)",
                                   "R_{nFD} vs. D_{w} (1n, FD);D_{w} [cm];"
                                   "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                   numTH2Dbins_nRes_Plots, 0, 25, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    std::string hP_nFD_Res_VS_shower_width_1n_Dir = directories.Resolution_dir_map["Resolution_shower_width_plots_1n"];
#pragma endregion

#pragma region /* Resolution shower width plots */

    hP_nFD_Res_VS_m2u_1n = OwnHistogram(new TH2D("R_{nFD} vs. m2_{u} (1n, FD)",
                                    "R_{nFD} vs. m2_{u} (1n, FD);m2_{u} [cm];"
                                    "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                    numTH2Dbins_nRes_Plots, 0, 250, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_m2v_1n = OwnHistogram(new TH2D("R_{nFD} vs. m2_{v} (1n, FD)",
                                    "R_{nFD} vs. m2_{v} (1n, FD);m2_{v} [cm];"
                                    "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                    numTH2Dbins_nRes_Plots, 0, 250, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_m2w_1n = OwnHistogram(new TH2D("R_{nFD} vs. m2_{w} (1n, FD)",
                                    "R_{nFD} vs. m2_{w} (1n, FD);m2_{w} [cm];"
                                    "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                    numTH2Dbins_nRes_Plots, 0, 250, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_m3u_1n = OwnHistogram(new TH2D("R_{nFD} vs. m3_{u} (1n, FD)",
                                    "R_{nFD} vs. m3_{u} (1n, FD);m3_{u} [cm];"
                                    "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                    numTH2Dbins_nRes_Plots, 0, 250, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_m3v_1n = OwnHistogram(new TH2D("R_{nFD} vs. m3_{v} (1n, FD)",
                                    "R_{nFD} vs. m3_{v} (1n, FD);m3_{v} [cm];"
                                    "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                    numTH2Dbins_nRes_Plots, 0, 250, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    hP_nFD_Res_VS_m3w_1n = OwnHistogram(new TH2D("R_{nFD} vs. m3_{w} (1n, FD)",
                                    "R_{nFD} vs. m3_{w} (1n, FD);m3_{w} [cm];"
                                    "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                    numTH2Dbins_nRes_Plots, 0, 250, numTH2Dbins_nRes_Plots, -1.1, 1.1));
    std::string hP_nFD_Res_VS_shower_moment_e_1n_Dir = directories.Resolution_dir_map["Resolution_shower_moment_plots_1n"];
#pragma endregion

    hP_nFD_Res_1n = hPlot1D("1n", "", "R_{nFD}", "FD neutron R_{nFD}", "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}", directories.Resolution_dir_map["Resolution_1n"],
                            "04_P_nFD_Res_1n", -2, 2, numTH1Dbins_nRes_Plots);

    // Basic neutron variables (1n)
    hReco_L_1n = hPlot1D("1n", "FD", "Reco neutron path #font[12]{L_{reco}}", "Reco neutron path #font[12]{L_{reco}}", "#font[12]{L_{reco}} [cm]",
                         directories.Resolution_dir_map["Basic_var_1n"], "01a_Reco_L_1n", 700, 950, numTH1Dbins_ReacMon_plots);
    hReco_L_VS_reco_P_nFD_1n = hPlot2D("1n", "FD", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{reco}_{nFD}}",
                                       "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{reco}_{nFD}}", "#font[12]{L_{reco}} [cm]", "#font[12]{P^{reco}_{nFD}} [GeV/c]",
                                       directories.Resolution_dir_map["Basic_var_1n"], "01b_Reco_L_VS_reco_P_nFD_1n", 700, 950, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_L_VS_truth_P_nFD_1n = hPlot2D("1n", "FD", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{truth}_{nFD}}",
                                        "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{truth}_{nFD}}", "#font[12]{L_{reco}} [cm]", "#font[12]{P^{truth}_{nFD}} [GeV/c]",
                                        directories.Resolution_dir_map["Basic_var_1n"], "01c_Reco_L_VS_truth_P_nFD_1n", 700, 950, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_L_VS_R_nFD_1n = hPlot2D("1n", "FD", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{R_{nFD}}", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{R_{nFD}}",
                                  "#font[12]{L_{reco}} [cm]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}", directories.Resolution_dir_map["Basic_var_1n"],
                                  "01d_Reco_L_VS_R_nFD_1n", 700, 950, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_L_VS_reco_theta_nFD_1n =
        hPlot2D("1n", "FD", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{#theta_{nFD}}", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{#theta_{nFD}}",
                "#font[12]{L_{reco}} [cm]", "#font[12]{#theta_{nFD}} [#circ]", directories.Resolution_dir_map["Basic_var_1n"], "01e_Reco_L_VS_reco_theta_nFD_1n", 700, 950, Theta_llim_FD,
                Theta_ulim_FD, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_L_VS_reco_phi_nFD_1n = hPlot2D("1n", "FD", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{#phi_{nFD}}", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{#phi_{nFD}}",
                                         "#font[12]{L_{reco}} [cm]", "#font[12]{#phi_{nFD}} [#circ]", directories.Resolution_dir_map["Basic_var_1n"], "01f_Reco_L_VS_reco_phi_nFD_1n", 700,
                                         950, Phi_llim, Phi_ulim, numTH2Dbins * 3, numTH2Dbins * 3);

    hReco_L_ECIN_1n = hPlot1D("1n", "ECIN Only", "Reco neutron path #font[12]{L_{reco}}", "Reco neutron path #font[12]{L_{reco}}", "#font[12]{L_{reco}} [cm]",
                              directories.Resolution_dir_map["Basic_var_1n"], "02a_Reco_L_ECIN_1n", 700, 950, numTH1Dbins_ReacMon_plots);
    hReco_L_VS_reco_P_nFD_ECIN_1n =
        hPlot2D("1n", "ECIN Only", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{reco}_{nFD}}", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{reco}_{nFD}}",
                "#font[12]{L_{reco}} [cm]", "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Basic_var_1n"], "02b_Reco_L_VS_reco_P_nFD_ECIN_1n", 700, 950, 0.4 * 0.95,
                beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_L_VS_truth_P_nFD_ECIN_1n =
        hPlot2D("1n", "ECIN Only", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{truth}_{nFD}}", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{truth}_{nFD}}",
                "#font[12]{L_{reco}} [cm]", "#font[12]{P^{truth}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Basic_var_1n"], "02c_Reco_L_VS_truth_P_nFD_ECIN_1n", 700, 950, 0.4 * 0.95,
                beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_L_VS_R_nFD_ECIN_1n = hPlot2D("1n", "ECIN Only", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{R_{nFD}}", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{R_{nFD}}",
                                       "#font[12]{L_{reco}} [cm]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}", directories.Resolution_dir_map["Basic_var_1n"],
                                       "02d_Reco_L_VS_R_nFD_ECIN_1n", 700, 950, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    hReco_L_ECOUT_1n = hPlot1D("1n", "ECOUT Only", "Reco neutron path #font[12]{L_{reco}}", "Reco neutron path #font[12]{L_{reco}}", "#font[12]{L_{reco}} [cm]",
                               directories.Resolution_dir_map["Basic_var_1n"], "03a_Reco_L_ECOUT_1n", 700, 950, numTH1Dbins_ReacMon_plots);
    hReco_L_VS_reco_P_nFD_ECOUT_1n =
        hPlot2D("1n", "ECOUT Only", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{reco}_{nFD}}", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{reco}_{nFD}}",
                "#font[12]{L_{reco}} [cm]", "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Basic_var_1n"], "03b_Reco_L_VS_reco_P_nFD_ECOUT_1n", 700, 950, 0.4 * 0.95,
                beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_L_VS_truth_P_nFD_ECOUT_1n =
        hPlot2D("1n", "ECOUT Only", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{truth}_{nFD}}", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{P^{truth}_{nFD}}",
                "#font[12]{L_{reco}} [cm]", "#font[12]{P^{truth}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Basic_var_1n"], "03c_Reco_L_VS_truth_P_nFD_ECOUT_1n", 700, 950, 0.4 * 0.95,
                beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_L_VS_R_nFD_ECOUT_1n = hPlot2D("1n", "ECOUT Only", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{R_{nFD}}", "Reco neutron path #font[12]{L_{reco}} vs. #font[12]{R_{nFD}}",
                                        "#font[12]{L_{reco}} [cm]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}", directories.Resolution_dir_map["Basic_var_1n"],
                                        "03d_Reco_L_VS_R_nFD_ECOUT_1n", 700, 950, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    hReco_t_ToF_1n = hPlot1D("1n", "FD", "Reco neutron #font[12]{t_{ToF}}", "Reco neutron #font[12]{t_{ToF}}", "#font[12]{t_{ToF}} [ns]", directories.Resolution_dir_map["Basic_var_1n"],
                             "04a_Reco_t_ToF_1n", 135, 220, numTH1Dbins_ReacMon_plots);
    hReco_t_ToF_VS_reco_P_nFD_1n = hPlot2D("1n", "FD", "Reco neutron #font[12]{t_{ToF}} vs. #font[12]{P^{reco}_{nFD}}", "Reco neutron #font[12]{t_{ToF}} vs. #font[12]{P^{reco}_{nFD}}",
                                           "#font[12]{t_{ToF}} [ns]", "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Basic_var_1n"], "04b_Reco_t_ToF_VS_reco_P_nFD_1n",
                                           135, 220, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_t_ToF_VS_truth_P_nFD_1n = hPlot2D("1n", "FD", "Reco neutron #font[12]{t_{ToF}} vs. #font[12]{P^{truth}_{nFD}}", "Reco neutron #font[12]{t_{ToF}} vs. #font[12]{P^{truth}_{nFD}}",
                                            "#font[12]{t_{ToF}} [ns]", "#font[12]{P^{truth}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Basic_var_1n"],
                                            "04c_Reco_t_ToF_VS_truth_P_nFD_1n", 135, 220, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_t_ToF_VS_R_nFD_1n = hPlot2D("1n", "FD", "Reco neutron #font[12]{t_{ToF}} vs. #font[12]{R_{nFD}}", "Reco neutron #font[12]{t_{ToF}} vs. #font[12]{R_{nFD}}",
                                      "#font[12]{t_{ToF}} [ns]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}", directories.Resolution_dir_map["Basic_var_1n"],
                                      "04d_Reco_beta_VS_R_nFD_1n", 135, 220, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    hReco_beta_1n = hPlot1D("1n", "FD", "Reco neutron #font[12]{#beta^{reco}_{nFD}}", "Reco neutron #font[12]{#beta^{reco}_{nFD}}", "#font[12]{#beta^{reco}_{nFD}}",
                            directories.Resolution_dir_map["Basic_var_1n"], "05a_Reco_beta_1n", 0.35, 1.05, numTH1Dbins_ReacMon_plots);
    hReco_beta_VS_reco_P_nFD_1n =
        hPlot2D("1n", "FD", "Reco neutron #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{P^{reco}_{nFD}}", "Reco neutron #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{P^{reco}_{nFD}}",
                "#font[12]{P^{reco}_{nFD}} [GeV/c]", "#font[12]{#beta^{reco}_{nFD}}", directories.Resolution_dir_map["Basic_var_1n"], "05b_Reco_beta_VS_reco_P_nFD_1n", 0.4 * 0.95, beamE110,
                0.35, 1.05, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_beta_VS_truth_P_nFD_1n =
        hPlot2D("1n", "FD", "Reco neutron #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{P^{truth}_{nFD}}", "Reco neutron #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{P^{truth}_{nFD}}",
                "#font[12]{P^{truth}_{nFD}} [GeV/c]", "#font[12]{#beta^{reco}_{nFD}}", directories.Resolution_dir_map["Basic_var_1n"], "05c_Reco_beta_VS_truth_P_nFD_1n", 0.4 * 0.95,
                beamE110, 0.35, 1.05, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_beta_VS_R_nFD_1n = hPlot2D("1n", "FD", "Reco neutron #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{R_{nFD}}", "Reco neutron #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{R_{nFD}}",
                                     "#font[12]{#beta^{reco}_{nFD}}", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}",
                                     directories.Resolution_dir_map["Basic_var_1n"], "05d_Reco_beta_VS_R_nFD_1n", 0.35, 1.05, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    hReco_ToF_from_beta_1n = hPlot1D("1n", "FD", "Reco neutron ToF from #font[12]{#beta^{reco}_{nFD}} - #font[12]{t_{ToF}^{#beta^{reco}_{nFD}}}",
                                     "Reco neutron ToF from #font[12]{#beta^{reco}_{nFD}} - #font[12]{t_{ToF}^{#beta^{reco}_{nFD}}}",
                                     "#font[12]{t_{ToF}^{#beta^{reco}_{nFD}} = L_{reco}/#left(c#times#beta^{reco}_{nFD}#right)} [ns]", directories.Resolution_dir_map["Basic_var_1n"],
                                     "06a_Reco_ToF_from_beta_1n", 20, 75, numTH1Dbins_ReacMon_plots);
    hReco_ToF_from_beta_VS_reco_P_nFD_1n = hPlot2D(
        "1n", "FD", "Reco neutron ToF from #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{P^{reco}_{nFD}}", "Reco neutron ToF from #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{P^{reco}_{nFD}}",
        "#font[12]{t_{ToF}^{#beta^{reco}_{nFD}} = L_{reco}/#left(c#times#beta^{reco}_{nFD}#right)} [ns]", "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Basic_var_1n"],
        "06b_Reco_ToF_from_beta_VS_reco_P_nFD_1n", 20, 75, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_ToF_from_beta_VS_truth_P_nFD_1n =
        hPlot2D("1n", "FD", "Reco neutron ToF from #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{P^{truth}_{nFD}}",
                "Reco neutron ToF from #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{P^{truth}_{nFD}}",
                "#font[12]{t_{ToF}^{#beta^{reco}_{nFD}} = L_{reco}/#left(c#times#beta^{reco}_{nFD}#right)} [ns]", "#font[12]{P^{truth}_{nFD}} [GeV/c]",
                directories.Resolution_dir_map["Basic_var_1n"], "06c_Reco_ToF_from_beta_VS_truth_P_nFD_1n", 20, 75, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_ToF_from_beta_VS_R_nFD_1n =
        hPlot2D("1n", "FD", "Reco neutron ToF from #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{R_{nFD}}", "Reco neutron ToF from #font[12]{#beta^{reco}_{nFD}} vs. #font[12]{R_{nFD}}",
                "#font[12]{t_{ToF}^{#beta^{reco}_{nFD}} = L_{reco}/#left(c#times#beta^{reco}_{nFD}#right)} [ns]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}",
                directories.Resolution_dir_map["Basic_var_1n"], "06d_Reco_ToF_from_beta_VS_R_nFD_1n", 20, 75, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    hReco_calc_ToF_1n =
        hPlot1D("1n", "FD", "Reco neutron ToF from calculation - #font[12]{t_{ToF}^{calc}}", "Reco neutron ToF from calculation - #font[12]{t_{ToF}^{calc}}",
                "#font[12]{t_{ToF}^{calc} = t_{ECAL} - t_{start}} [ns]", directories.Resolution_dir_map["Basic_var_1n"], "07a_Reco_calc_ToF_1n", 20, 75, numTH1Dbins_ReacMon_plots);
    hReco_calc_ToF_VS_reco_P_nFD_1n = hPlot2D("1n", "FD", "Reco neutron calculated #font[12]{t_{ToF}^{calc}} vs. #font[12]{P^{reco}_{nFD}}",
                                              "Reco neutron calculated #font[12]{t_{ToF}^{calc}} vs. #font[12]{P^{reco}_{nFD}}", "#font[12]{t_{ToF}^{calc} = t_{ECAL} - t_{start}} [ns]",
                                              "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Basic_var_1n"], "07b_Reco_calc_ToF_VS_reco_P_nFD_1n", 20, 75, 0.4 * 0.95,
                                              beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_calc_ToF_VS_truth_P_nFD_1n = hPlot2D("1n", "FD", "Reco neutron calculated #font[12]{t_{ToF}^{calc}} vs. #font[12]{P^{truth}_{nFD}}",
                                               "Reco neutron calculated #font[12]{t_{ToF}^{calc}} vs. #font[12]{P^{truth}_{nFD}}", "#font[12]{t_{ToF}^{calc} = t_{ECAL} - t_{start}} [ns]",
                                               "#font[12]{P^{truth}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Basic_var_1n"], "07c_Reco_calc_ToF_VS_truth_P_nFD_1n", 20, 75,
                                               0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hReco_calc_ToF_VS_R_nFD_1n =
        hPlot2D("1n", "FD", "Reco neutron calculated #font[12]{t_{ToF}^{calc}} vs. #font[12]{R_{nFD}}", "Reco neutron calculated #font[12]{t_{ToF}^{calc}} vs. #font[12]{R_{nFD}}",
                "#font[12]{t_{ToF}^{calc} = t_{ECAL} - t_{start}} [ns]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}",
                directories.Resolution_dir_map["Basic_var_1n"], "07d_Reco_calc_ToF_VS_R_nFD_1n", 20, 75, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    // Correction fit justification (1n)
    hEff_dist_TL_1n = hPlot1D("1n", "FD", "Effective distance #font[12]{L^{truth}_{eff}} from #font[12]{#beta^{truth}_{nFD}}",
                              "Effective distance #font[12]{L^{truth}_{eff}} from #font[12]{#beta^{truth}_{nFD}}",
                              "#font[12]{L^{truth}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{#beta^{reco}_{nFD}}} [cm]", directories.Resolution_dir_map["Corr_just_1n"],
                              "08a_Eff_dist_TL_1n", 400, 1600, numTH1Dbins_ReacMon_plots);
    hEff_dist_TL_VS_reco_P_nFD_1n =
        hPlot2D("1n", "FD", "Effective distance #font[12]{L^{truth}_{eff}} vs. #font[12]{P^{reco}_{nFD}}", "Effective distance #font[12]{L^{truth}_{eff}} vs. #font[12]{P^{reco}_{nFD}}",
                "#font[12]{L^{truth}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{#beta^{reco}_{nFD}}} [cm]", "#font[12]{P^{reco}_{nFD}} [GeV/c]",
                directories.Resolution_dir_map["Corr_just_1n"], "08b_Eff_dist_TL_VS_reco_P_nFD_1n", 400, 1600, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hEff_dist_TL_VS_truth_P_nFD_1n =
        hPlot2D("1n", "FD", "Effective distance #font[12]{L^{truth}_{eff}} vs. #font[12]{P^{truth}_{nFD}}", "Effective distance #font[12]{L^{truth}_{eff}} vs. #font[12]{P^{truth}_{nFD}}",
                "#font[12]{L^{truth}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{#beta^{reco}_{nFD}}} [cm]", "#font[12]{P^{truth}_{nFD}} [GeV/c]",
                directories.Resolution_dir_map["Corr_just_1n"], "08c_Eff_dist_TL_VS_truth_P_nFD_1n", 400, 1600, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hEff_dist_TL_VS_R_nFD_1n =
        hPlot2D("1n", "FD", "Effective distance #font[12]{L^{truth}_{eff}} vs. #font[12]{R_{nFD}}", "Effective distance #font[12]{L^{truth}_{eff}} vs. #font[12]{R_{nFD}}",
                "#font[12]{L^{truth}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{#beta^{reco}_{nFD}}} [cm]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}",
                directories.Resolution_dir_map["Corr_just_1n"], "08d_Eff_dist_TL_VS_R_nFD_1n", 400, 1600, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    hEff_dist_calc_1n =
        hPlot1D("1n", "FD", "Effective distance #font[12]{L^{calc}_{eff}} from #font[12]{t_{ToF}^{calc}}", "Effective distance #font[12]{L^{calc}_{eff}} from #font[12]{t_{ToF}^{calc}}",
                "#font[12]{L^{calc}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{calc}} [cm]", directories.Resolution_dir_map["Corr_just_1n"], "09a_Eff_dist_calc_1n", 400, 1600,
                numTH1Dbins_ReacMon_plots);
    hEff_dist_calc_VS_reco_P_nFD_1n =
        hPlot2D("1n", "FD", "Effective distance #font[12]{L^{calc}_{eff}} vs. #font[12]{P^{reco}_{nFD}}", "Effective distance #font[12]{L^{calc}_{eff}} vs. #font[12]{P^{reco}_{nFD}}",
                "#font[12]{L^{calc}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{calc}} [cm]", "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"],
                "09b_Eff_dist_calc_VS_reco_P_nFD_1n", 400, 1600, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hEff_dist_calc_VS_truth_P_nFD_1n =
        hPlot2D("1n", "FD", "Effective distance #font[12]{L^{calc}_{eff}} vs. #font[12]{P^{truth}_{nFD}}", "Effective distance #font[12]{L^{calc}_{eff}} vs. #font[12]{P^{truth}_{nFD}}",
                "#font[12]{L^{calc}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{calc}} [cm]", "#font[12]{P^{truth}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"],
                "09c_Eff_dist_calc_VS_truth_P_nFD_1n", 400, 1600, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hEff_dist_calc_VS_R_nFD_1n =
        hPlot2D("1n", "FD", "Effective distance #font[12]{L^{calc}_{eff}} vs. #font[12]{R_{nFD}}", "Effective distance #font[12]{L^{calc}_{eff}} vs. #font[12]{R_{nFD}}",
                "#font[12]{L^{calc}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{calc}} [cm]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}",
                directories.Resolution_dir_map["Corr_just_1n"], "09d_Eff_dist_calc_VS_R_nFD_1n", 400, 1600, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    hDeltaL_TL_1n = hPlot1D("1n", "FD", "Distance difference between #font[12]{L^{truth}_{eff}} and #font[12]{L_{reco}}",
                            "Distance difference between #font[12]{L^{truth}_{eff}} and #font[12]{L_{reco}}", "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]",
                            directories.Resolution_dir_map["Corr_just_1n"], "10a_DeltaL_TL_1n", -100, 400, numTH1Dbins_ReacMon_plots);
    hDeltaL_TL_VS_reco_P_nFD_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{P^{reco}_{nFD}}", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{P^{reco}_{nFD}}",
                "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"],
                "10b_DeltaL_TL_VS_reco_P_nFD_1n", -100, 400, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_TL_VS_truth_P_nFD_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{P^{truth}_{nFD}}", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{P^{truth}_{nFD}}",
                "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{P^{truth}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"],
                "10c_DeltaL_TL_VS_truth_P_nFD_1n", -100, 400, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_TL_VS_R_nFD_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{R_{nFD}}", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{R_{nFD}}",
                "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}",
                directories.Resolution_dir_map["Corr_just_1n"], "10d_DeltaL_TL_VS_R_nFD_1n", -100, 400, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_TL_VS_Eff_dist_TL_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{L^{truth}_{eff}}", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{L^{truth}_{eff}}",
                "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{L^{truth}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{#beta^{reco}_{nFD}}} [cm]",
                directories.Resolution_dir_map["Corr_just_1n"], "10e_DeltaL_TL_VS_Eff_dist_TL_1n", -100, 400, 400, 1600, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_TL_VS_Reco_L_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{L_{reco}}", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{L_{reco}}",
                "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{L_{reco}} [cm]", directories.Resolution_dir_map["Corr_just_1n"],
                "10f_DeltaL_TL_VS_Reco_L_1n", -100, 400, 700, 950, numTH2Dbins * 3, numTH2Dbins * 3);

    hDeltaL_calc_1n = hPlot1D("1n", "FD", "Distance difference between #font[12]{L^{calc}_{eff}} and #font[12]{L_{reco}}",
                              "Distance difference between #font[12]{L^{calc}_{eff}} and #font[12]{L_{reco}}", "#font[12]{#DeltaL^{calc} = L^{calc}_{eff} - #font[12]{L_{reco}}} [cm]",
                              directories.Resolution_dir_map["Corr_just_1n"], "11a_DeltaL_calc_1n", -100, 400, numTH1Dbins_ReacMon_plots);
    hDeltaL_calc_VS_reco_P_nFD_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{P^{reco}_{nFD}}", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{P^{reco}_{nFD}}",
                "#font[12]{#DeltaL^{calc} = L^{calc}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"],
                "11b_DeltaL_calc_VS_reco_P_nFD_1n", -100, 400, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_calc_VS_truth_P_nFD_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{P^{truth}_{nFD}}", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{P^{truth}_{nFD}}",
                "#font[12]{#DeltaL^{calc} = L^{calc}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{P^{truth}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"],
                "11c_DeltaL_calc_VS_truth_P_nFD_1n", -100, 400, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_calc_VS_R_nFD_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{R_{nFD}}", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{R_{nFD}}",
                "#font[12]{#DeltaL^{calc} = L^{calc}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}",
                directories.Resolution_dir_map["Corr_just_1n"], "11d_DeltaL_calc_VS_R_nFD_1n", -100, 400, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_calc_VS_Eff_dist_TL_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{L^{calc}_{eff}}", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{L^{calc}_{eff}}",
                "#font[12]{#DeltaL^{calc} = L^{calc}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{L^{calc}_{eff} = c#times#beta^{truth}_{nFD}#timest_{ToF}^{#beta^{reco}_{nFD}}} [cm]",
                directories.Resolution_dir_map["Corr_just_1n"], "11e_DeltaL_calc_VS_Eff_dist_TL_1n", -100, 400, 400, 1600, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_calc_VS_Reco_L_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{L_{reco}}", "Distance difference #font[12]{#DeltaL^{calc}} vs. #font[12]{L_{reco}}",
                "#font[12]{#DeltaL^{calc} = L^{calc}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{L_{reco}} [cm]", directories.Resolution_dir_map["Corr_just_1n"],
                "11f_DeltaL_calc_VS_Reco_L_1n", -100, 400, 700, 950, numTH2Dbins * 3, numTH2Dbins * 3);

    hDeltaL_TL_below_0_2_1n = hPlot1D("1n", "FD", "Distance difference between #font[12]{L^{truth}_{eff}} and #font[12]{L_{reco}} for #font[12]{R_{nFD}<0.2}",
                                      "Distance difference between #font[12]{L^{truth}_{eff}} and #font[12]{L_{reco}} for #font[12]{R_{nFD}<0.2}",
                                      "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", directories.Resolution_dir_map["Corr_just_1n"],
                                      "12a_DeltaL_TL_for_R_nFD_below_0_2_1n", -100, 400, numTH1Dbins_ReacMon_plots);
    hDeltaL_TL_VS_reco_P_nFD_below_0_2_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{P^{reco}_{nFD}} for #font[12]{R_{nFD}<0.2}",
                "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{P^{reco}_{nFD}} for #font[12]{R_{nFD}<0.2}",
                "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"],
                "12b_DeltaL_TL_VS_reco_P_nFD_below_0_2_1n", -100, 400, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_TL_VS_truth_P_nFD_below_0_2_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{P^{truth}_{nFD}} for #font[12]{R_{nFD}<0.2}",
                "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{P^{truth}_{nFD}} for #font[12]{R_{nFD}<0.2}",
                "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{P^{truth}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"],
                "12c_DeltaL_TL_VS_truth_P_nFD_below_0_2_1n", -100, 400, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_TL_VS_R_nFD_below_0_2_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{R_{nFD}} for #font[12]{R_{nFD}<0.2}",
                "Distance difference #font[12]{#DeltaL^{truth}} vs. #font[12]{R_{nFD}} for #font[12]{R_{nFD}<0.2}", "#font[12]{#DeltaL^{truth} = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]",
                "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}", directories.Resolution_dir_map["Corr_just_1n"], "12d_DeltaL_TL_VS_R_nFD_below_0_2_1n", -100, 400,
                -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    hDeltaL_TL_above_0_4_1n = hPlot1D("1n", "FD", "Distance difference between #font[12]{L^{truth}_{eff}} and #font[12]{L_{reco}} for #font[12]{R_{nFD}>0.4}",
                                      "Distance difference between #font[12]{L^{truth}_{eff}} and #font[12]{L_{reco}} for #font[12]{R_{nFD}>0.4}",
                                      "#font[12]{#DeltaL = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", directories.Resolution_dir_map["Corr_just_1n"],
                                      "13a_DeltaL_TL_for_R_nFD_above_0_4_1n", -100, 400, numTH1Dbins_ReacMon_plots);
    hDeltaL_TL_VS_reco_P_nFD_above_0_4_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL} vs. #font[12]{P^{reco}_{nFD}} for #font[12]{R_{nFD}>0.4}",
                "Distance difference #font[12]{#DeltaL} vs. #font[12]{P^{reco}_{nFD}} for #font[12]{R_{nFD}>0.4}", "#font[12]{#DeltaL = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]",
                "#font[12]{P^{reco}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"], "13b_DeltaL_TL_VS_reco_P_nFD_above_0_4_1n", -100, 400, 0.4 * 0.95, beamE110,
                numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_TL_VS_truth_P_nFD_above_0_4_1n =
        hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL} vs. #font[12]{P^{truth}_{nFD}} for #font[12]{R_{nFD}>0.4}",
                "Distance difference #font[12]{#DeltaL} vs. #font[12]{P^{truth}_{nFD}} for #font[12]{R_{nFD}>0.4}", "#font[12]{#DeltaL = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]",
                "#font[12]{P^{truth}_{nFD}} [GeV/c]", directories.Resolution_dir_map["Corr_just_1n"], "13c_DeltaL_TL_VS_truth_P_nFD_above_0_4_1n", -100, 400, 0.4 * 0.95, beamE110,
                numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltaL_TL_VS_R_nFD_above_0_4_1n = hPlot2D("1n", "FD", "Distance difference #font[12]{#DeltaL} vs. #font[12]{R_{nFD}} for #font[12]{R_{nFD}>0.4}",
                                               "Distance difference #font[12]{#DeltaL} vs. #font[12]{R_{nFD}} for #font[12]{R_{nFD}>0.4}",
                                               "#font[12]{#DeltaL = L^{truth}_{eff} - #font[12]{L_{reco}}} [cm]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}",
                                               directories.Resolution_dir_map["Corr_just_1n"], "13d_DeltaL_TL_VS_R_nFD_above_0_4_1n", -100, 400, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    // Smearing fit justification (1n)
    hDeltat_ToF_reco_1n = hPlot1D("1n", "FD", "Reco neutron ToF error #font[12]{#Deltat^{err}_{ToF}}", "Reco neutron ToF error #font[12]{#Deltat^{err}_{ToF}}",
                                  "#font[12]{#Deltat^{err}_{ToF} = - t_{ToF}#times#left(1 - #beta^{2}#right)#timesR_{nFD}} [ns]", directories.Resolution_dir_map["Smear_just_1n"],
                                  "13a_Deltat_ToF_reco_for_R_nFD_1n", -20, 5, numTH1Dbins_ReacMon_plots);
    hDeltat_ToF_reco_VS_reco_P_nFD_1n =
        hPlot2D("1n", "FD", "Reco neutron ToF error #font[12]{#Deltat^{err}_{ToF}} vs. #font[12]{P^{reco}_{nFD}}",
                "Reco neutron ToF error #font[12]{#Deltat^{err}_{ToF}} vs. #font[12]{P^{reco}_{nFD}}",
                "#font[12]{#Deltat^{err}_{ToF} = - t_{ToF}#times#left(1 - #beta^{2}#right)#timesR_{nFD}} [ns]", "#font[12]{P^{reco}_{nFD}} [GeV/c]",
                directories.Resolution_dir_map["Smear_just_1n"], "13b_Deltat_ToF_reco_VS_reco_P_nFD_1n", -20, 5, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltat_ToF_reco_VS_truth_P_nFD_1n =
        hPlot2D("1n", "FD", "Reco neutron ToF error #font[12]{#Deltat^{err}_{ToF}} vs. #font[12]{P^{truth}_{nFD}}",
                "Reco neutron ToF error #font[12]{#Deltat^{err}_{ToF}} vs. #font[12]{P^{truth}_{nFD}}",
                "#font[12]{#Deltat^{err}_{ToF} = - t_{ToF}#times#left(1 - #beta^{2}#right)#timesR_{nFD}} [ns]", "#font[12]{P^{truth}_{nFD}} [GeV/c]",
                directories.Resolution_dir_map["Smear_just_1n"], "13c_Deltat_ToF_reco_VS_truth_P_nFD_1n", -20, 5, 0.4 * 0.95, beamE110, numTH2Dbins * 3, numTH2Dbins * 3);
    hDeltat_ToF_reco_VS_R_nFD_1n =
        hPlot2D("1n", "FD", "Reco neutron ToF error #font[12]{#Deltat^{err}_{ToF}} vs. #font[12]{R_{nFD}}", "Reco neutron ToF error #font[12]{#Deltat^{err}_{ToF}} vs. #font[12]{R_{nFD}}",
                "#font[12]{#Deltat^{err}_{ToF} = - t_{ToF}#times#left(1 - #beta^{2}#right)#timesR_{nFD}} [ns]", "#font[12]{R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}}",
                directories.Resolution_dir_map["Smear_just_1n"], "13d_Deltat_ToF_reco_VS_R_nFD_1n", -20, 5, -1, 1, numTH2Dbins * 3, numTH2Dbins * 3);

    // Match multiplicity (1n)
    hnRes_Match_Multi_1n = hPlot1D("1n", "FD", "Neutron resolution match multiplicity", "Neutron resolution match multiplicity", "Match multiplicity",
                                   directories.Resolution_dir_map["Match_multi_1n"], "01_nRes_Match_Multi_1n", 0.0, 10., 10);
    hnRes_Match_Multi_vs_Reco_P_nFD_1n = hPlot2D("1n", "FD", "Match multiplicity vs. P^{reco}_{nFD}", "Match multiplicity vs. P^{reco}_{nFD}", "Match multiplicity", "P^{reco}_{nFD} [GeV/c]",
                                                 directories.Resolution_dir_map["Match_multi_1n"], "02_nRes_Match_Multi_vs_Reco_P_nFD_1n", 0.0, 10., Momentum_llim, Momentum_ulim, 10, 50);
    hnRes_Match_Multi_vs_Reco_Theta_nFD_1n =
        hPlot2D("1n", "FD", "Match multiplicity vs. #theta^{reco}_{nFD}", "Match multiplicity vs. #theta^{reco}_{nFD}", "Match multiplicity", "#theta^{reco}_{nFD} [#circ]",
                directories.Resolution_dir_map["Match_multi_1n"], "03_nRes_Match_Multi_vs_Reco_Theta_nFD_1n", 0.0, 10., Theta_llim_FD, Theta_ulim_FD, 10, 50);
    hnRes_Match_Multi_vs_Reco_Phi_nFD_1n =
        hPlot2D("1n", "FD", "Match multiplicity vs. #phi^{reco}_{nFD}", "Match multiplicity vs. #phi^{reco}_{nFD}", "Match multiplicity", "#phi^{reco}_{nFD} [#circ]",
                directories.Resolution_dir_map["Match_multi_1n"], "04_nRes_Match_Multi_vs_Reco_Phi_nFD_1n", 0.0, 10., Phi_llim, Phi_ulim, 10, 50);
#pragma endregion
#pragma endregion
}

void FinalStateHistograms::InitEcalHistograms(const HistogramDefinitionSettings& hDefSettings) {
    auto& directories = ecal_directories_;
    const int numTH1Dbins_E_cal_Plots = hDefSettings.numTH1Dbins_E_cal_Plots;
    const int numTH2Dbins_E_cal_Plots = hDefSettings.numTH2Dbins_E_cal_Plots;
    const double Momentum_llim = hDefSettings.Momentum_llim;
    const double Momentum_ulim = hDefSettings.Momentum_ulim;
    const double P_nucFD_llim = hDefSettings.P_nucFD_llim;
    const double P_nucFD_ulim = hDefSettings.P_nucFD_ulim;
    const double P_nucCD_llim = hDefSettings.P_nucCD_llim;
    const double P_nucCD_ulim = hDefSettings.P_nucCD_ulim;
    const double Theta_llim_FD = hDefSettings.Theta_llim_FD;
    const double Theta_ulim_FD = hDefSettings.Theta_ulim_FD;
    const double Theta_llim_CD = hDefSettings.Theta_llim_CD;
    const double Theta_ulim_CD = hDefSettings.Theta_ulim_CD;
    const double Phi_llim = hDefSettings.Phi_llim;
    const double Phi_ulim = hDefSettings.Phi_ulim;
    const double dP_T_lim = hDefSettings.dP_T_lim;
    const double W_llim = hDefSettings.W_llim;
    const double W_ulim = hDefSettings.W_ulim;
    const double beamE135 = hDefSettings.Momentum_ulim * (1.35 / 1.1);

#pragma region /* Ecal Reconstruction Histograms */

    // Ecal reconstruction histograms (1p)

    // Ecal plots (1p)
    sEcal_1p = OwnHistogram(new THStack("E_{cal} Reconstruction (1p)", "E_{cal} Reconstruction (1p);E_{cal} = E_{e} + T_{pFD} [GeV]"));
    std::string sEcal_1p_Dir = directories.Ecal_dir_map["Ecal_stack_1p"];

    hEcal_All_Int_1p = OwnHistogram(new TH1D("E_{cal} reco. (All Int., 1p)", "E_{cal} Reconstruction (All Int., 1p);E_{cal} = E_{e} + T_{pFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_QEL_1p = OwnHistogram(new TH1D("E_{cal} reco. (QEL only, 1p)", "E_{cal} Reconstruction (QEL only, 1p);E_{cal} = E_{e} + T_{pFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_MEC_1p = OwnHistogram(new TH1D("E_{cal} reco. (MEC only, 1p)", "E_{cal} Reconstruction (MEC only, 1p);E_{cal} = E_{e} + T_{pFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_RES_1p = OwnHistogram(new TH1D("E_{cal} reco. (RES only, 1p)", "E_{cal} Reconstruction (RES only, 1p);E_{cal} = E_{e} + T_{pFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_DIS_1p = OwnHistogram(new TH1D("E_{cal} reco. (DIS only, 1p)", "E_{cal} Reconstruction (DIS only, 1p);E_{cal} = E_{e} + T_{pFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_All_Int_1p_Dir = directories.Ecal_dir_map["Ecal_All_Int_1p"];
    std::string hEcal_QEL_1p_Dir = directories.Ecal_dir_map["Ecal_QEL_1p"];
    std::string hEcal_MEC_1p_Dir = directories.Ecal_dir_map["Ecal_MEC_1p"];
    std::string hEcal_RES_1p_Dir = directories.Ecal_dir_map["Ecal_RES_1p"];
    std::string hEcal_DIS_1p_Dir = directories.Ecal_dir_map["Ecal_DIS_1p"];

    // Ecal vs. momentum (1p)
    hEcal_vs_P_e_1p = OwnHistogram(new TH2D("E_{cal} vs. P_{e} (All Int., 1p)", "E_{cal} vs. P_{e} (All Int., 1p);P_{e} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Momentum_llim, Momentum_ulim,
                               numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_e_test_1p = OwnHistogram(new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1p)", "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1p);P_{e} [GeV/c];E_{cal} [GeV];",
                                    numTH2Dbins_E_cal_Plots, Momentum_llim, Momentum_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_P_e_1p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_1p"];

    hEcal_vs_P_p_1p = OwnHistogram(new TH2D("E_{cal} vs. P_{p} (All Int., 1p)", "E_{cal} vs. P_{p} (All Int., 1p);P_{p} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, P_nucFD_llim, P_nucFD_ulim,
                               numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_p_test_1p = OwnHistogram(new TH2D("E_{cal} vs. P_{p} for E_{cal}>E_{beam} (All Int., 1p)", "E_{cal} vs. P_{p} for E_{cal}>E_{beam} (All Int., 1p);P_{p} [GeV/c];E_{cal} [GeV];",
                                    numTH2Dbins_E_cal_Plots, P_nucFD_llim, P_nucFD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_P_p_1p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_1p"];

    // Ecal vs. angles (1p)
    hEcal_vs_Theta_e_1p = OwnHistogram(new TH2D("E_{cal} vs. #theta_{e} (All Int., 1p)", "E_{cal} vs. #theta_{e} (All Int., 1p);#theta_{e} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Theta_llim_FD,
                                   Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_e_1p = OwnHistogram(new TH2D("E_{cal} vs. #phi_{e} (All Int., 1p)", "E_{cal} vs. #phi_{e} (All Int., 1p);#phi_{e} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim,
                                 numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_e_test_1p =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1p)", "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1p);#theta_{e} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_e_test_1p = OwnHistogram(new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1p)", "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1p);#phi_{e} [#circ];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_e_1p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_1p"];
    std::string hEcal_vs_Phi_e_1p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_1p"];

    hEcal_vs_Theta_p_1p = OwnHistogram(new TH2D("E_{cal} vs. #theta_{p} (All Int., 1p)", "E_{cal} vs. #theta_{p} (All Int., 1p);#theta_{p} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Theta_llim_FD,
                                   Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_p_1p = OwnHistogram(new TH2D("E_{cal} vs. #phi_{p} (All Int., 1p)", "E_{cal} vs. #phi_{p} (All Int., 1p);#phi_{p} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim,
                                 numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_p_test_1p =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{p} for E_{cal}>E_{beam} (All Int., 1p)", "E_{cal} vs. #theta_{p} for E_{cal}>E_{beam} (All Int., 1p);#theta_{p} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_p_test_1p = OwnHistogram(new TH2D("E_{cal} vs. #phi_{p} for E_{cal}>E_{beam} (All Int., 1p)", "E_{cal} vs. #phi_{p} for E_{cal}>E_{beam} (All Int., 1p);#phi_{p} [#circ];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_p_1p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_1p"];
    std::string hEcal_vs_Phi_p_1p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_1p"];

    // Ecal vs. dAlpha_T (1p)
    hEcal_vs_dAlpha_T_1p = OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T} (All Int., 1p)", "E_{cal} vs. #delta#alpha_{T} (All Int., 1p);#delta#alpha_{T} [#circ];E_{cal} [GeV];",
                                    numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dAlpha_T_1p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_1p"];

    // Ecal vs. dP_T (1p)
    hEcal_vs_dP_T_1p = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T} (All Int., 1p)", "E_{cal} vs. #deltaP_{T} (All Int., 1p);#deltaP_{T} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, 0, dP_T_lim,
                                numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dP_T_1p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_1p"];

    // Ecal vs. W (1p)
    hEcal_vs_W_1p = OwnHistogram(new TH2D("E_{cal} vs. W (All Int., 1p)", "E_{cal} vs. W (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];E_{cal} = E_{e} + T_{pFD} [GeV];",
                             numTH2Dbins_E_cal_Plots, W_llim, W_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_W_1p_Dir = directories.Ecal_dir_map["Ecal_stack_1p"];

    // Ecal reconstruction histograms (1n)

    // Ecal plots (1n)
    sEcal_1n = OwnHistogram(new THStack("E_{cal} Reconstruction (1n)", "E_{cal} Reconstruction (1n);E_{cal} = E_{e} + T_{nFD} [GeV]"));
    std::string sEcal_1n_Dir = directories.Ecal_dir_map["Ecal_stack_1n"];

    hEcal_All_Int_1n = OwnHistogram(new TH1D("E_{cal} reco. (All Int., 1n)", "E_{cal} Reconstruction (All Int., 1n);E_{cal} = E_{e} + T_{nFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_QEL_1n = OwnHistogram(new TH1D("E_{cal} reco. (QEL only, 1n)", "E_{cal} Reconstruction (QEL only, 1n);E_{cal} = E_{e} + T_{nFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_MEC_1n = OwnHistogram(new TH1D("E_{cal} reco. (MEC only, 1n)", "E_{cal} Reconstruction (MEC only, 1n);E_{cal} = E_{e} + T_{nFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_RES_1n = OwnHistogram(new TH1D("E_{cal} reco. (RES only, 1n)", "E_{cal} Reconstruction (RES only, 1n);E_{cal} = E_{e} + T_{nFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_DIS_1n = OwnHistogram(new TH1D("E_{cal} reco. (DIS only, 1n)", "E_{cal} Reconstruction (DIS only, 1n);E_{cal} = E_{e} + T_{nFD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_All_Int_1n_Dir = directories.Ecal_dir_map["Ecal_All_Int_1n"];
    std::string hEcal_QEL_1n_Dir = directories.Ecal_dir_map["Ecal_QEL_1n"];
    std::string hEcal_MEC_1n_Dir = directories.Ecal_dir_map["Ecal_MEC_1n"];
    std::string hEcal_RES_1n_Dir = directories.Ecal_dir_map["Ecal_RES_1n"];
    std::string hEcal_DIS_1n_Dir = directories.Ecal_dir_map["Ecal_DIS_1n"];

    // Ecal vs. momentum (1n)
    hEcal_vs_P_e_1n = OwnHistogram(new TH2D("E_{cal} vs. P_{e} (All Int., 1n)", "E_{cal} vs. P_{e} (All Int., 1n);P_{e} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Momentum_llim, Momentum_ulim,
                               numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_e_test_1n = OwnHistogram(new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1n)", "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1n);P_{e} [GeV/c];E_{cal} [GeV];",
                                    numTH2Dbins_E_cal_Plots, Momentum_llim, Momentum_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_P_e_1n_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_1n"];

    hEcal_vs_P_n_1n = OwnHistogram(new TH2D("E_{cal} vs. P_{n} (All Int., 1n)", "E_{cal} vs. P_{n} (All Int., 1n);P_{n} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, P_nucFD_llim, P_nucFD_ulim,
                               numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_n_test_1n = OwnHistogram(new TH2D("E_{cal} vs. P_{n} for E_{cal}>E_{beam} (All Int., 1n)", "E_{cal} vs. P_{n} for E_{cal}>E_{beam} (All Int., 1n);P_{n} [GeV/c];E_{cal} [GeV];",
                                    numTH2Dbins_E_cal_Plots, P_nucFD_llim, P_nucFD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_P_n_1n_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_1n"];

    // Ecal vs. angles (1n)
    hEcal_vs_Theta_e_1n = OwnHistogram(new TH2D("E_{cal} vs. #theta_{e} (All Int., 1n)", "E_{cal} vs. #theta_{e} (All Int., 1n);#theta_{e} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Theta_llim_FD,
                                   Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_e_1n = OwnHistogram(new TH2D("E_{cal} vs. #phi_{e} (All Int., 1n)", "E_{cal} vs. #phi_{e} (All Int., 1n);#phi_{e} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim,
                                 numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_e_test_1n =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1n)", "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1n);#theta_{e} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_e_test_1n = OwnHistogram(new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1n)", "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1n);#phi_{e} [#circ];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_e_1n_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_1n"];
    std::string hEcal_vs_Phi_e_1n_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_1n"];

    hEcal_vs_Theta_n_1n = OwnHistogram(new TH2D("E_{cal} vs. #theta_{n} (All Int., 1n)", "E_{cal} vs. #theta_{n} (All Int., 1n);#theta_{n} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Theta_llim_FD,
                                   Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_n_1n = OwnHistogram(new TH2D("E_{cal} vs. #phi_{n} (All Int., 1n)", "E_{cal} vs. #phi_{n} (All Int., 1n);#phi_{n} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim,
                                 numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_n_test_1n =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{n} for E_{cal}>E_{beam} (All Int., 1n)", "E_{cal} vs. #theta_{n} for E_{cal}>E_{beam} (All Int., 1n);#theta_{n} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_n_test_1n = OwnHistogram(new TH2D("E_{cal} vs. #phi_{n} for E_{cal}>E_{beam} (All Int., 1n)", "E_{cal} vs. #phi_{n} for E_{cal}>E_{beam} (All Int., 1n);#phi_{n} [#circ];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_n_1n_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_1n"];
    std::string hEcal_vs_Phi_n_1n_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_1n"];

    // Ecal vs. dAlpha_T (1n)
    hEcal_vs_dAlpha_T_1n = OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T} (All Int., 1n)", "E_{cal} vs. #delta#alpha_{T} (All Int., 1n);#delta#alpha_{T} [#circ];E_{cal} [GeV];",
                                    numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dAlpha_T_1n_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_1n"];

    // Ecal vs. dP_T (1n)
    hEcal_vs_dP_T_1n = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T} (All Int., 1n)", "E_{cal} vs. #deltaP_{T} (All Int., 1n);#deltaP_{T} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, 0, dP_T_lim,
                                numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dP_T_1n_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_1n"];

    // Ecal vs. W (1n)
    hEcal_vs_W_1n = OwnHistogram(new TH2D("E_{cal} vs. W (All Int., 1n)", "E_{cal} vs. W (All Int., 1n);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];E_{cal} = E_{e} + T_{nFD} [GeV];",
                             numTH2Dbins_E_cal_Plots, W_llim, W_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_W_1n_Dir = directories.Ecal_dir_map["Ecal_stack_1n"];

    // Ecal reconstruction histograms (2p)

    // Ecal plots (2p)
    sEcal_2p = OwnHistogram(new THStack("E_{cal} Reconstruction (2p)", "E_{cal} Reconstruction (2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]"));
    std::string sEcal_2p_Dir = directories.Ecal_dir_map["Ecal_stack_2p"];

    hEcal_All_Int_2p = OwnHistogram(new TH1D("E_{cal} reco. (All Int., 2p)", "E_{cal} Reconstruction (All Int., 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_QEL_2p = OwnHistogram(new TH1D("E_{cal} reco. (QEL only, 2p)", "E_{cal} Reconstruction (QEL only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_MEC_2p = OwnHistogram(new TH1D("E_{cal} reco. (MEC only, 2p)", "E_{cal} Reconstruction (MEC only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_RES_2p = OwnHistogram(new TH1D("E_{cal} reco. (RES only, 2p)", "E_{cal} Reconstruction (RES only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_DIS_2p = OwnHistogram(new TH1D("E_{cal} reco. (DIS only, 2p)", "E_{cal} Reconstruction (DIS only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_All_Int_2p_Dir = directories.Ecal_dir_map["Ecal_All_Int_2p"];
    std::string hEcal_QEL_2p_Dir = directories.Ecal_dir_map["Ecal_QEL_2p"];
    std::string hEcal_MEC_2p_Dir = directories.Ecal_dir_map["Ecal_MEC_2p"];
    std::string hEcal_RES_2p_Dir = directories.Ecal_dir_map["Ecal_RES_2p"];
    std::string hEcal_DIS_2p_Dir = directories.Ecal_dir_map["Ecal_DIS_2p"];

    // Ecal vs. dAlpha_T (2p)
    hEcal_vs_dAlpha_T_L_2p = OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p)", "E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [#circ];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_2p = OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p)", "E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                                        numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dAlpha_T_L_2p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_2p"];
    std::string hEcal_vs_dAlpha_T_tot_2p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_2p"];

    // Ecal vs. dP_T (2p)
    hEcal_vs_dP_T_L_2p = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., 2p)", "E_{cal} vs. #deltaP_{T,L} (All Int., 2p);#deltaP_{T,L} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, 0,
                                  dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_2p = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., 2p)", "E_{cal} vs. #deltaP_{T,tot} (All Int., 2p);#deltaP_{T,tot} [GeV/c];E_{cal} [GeV];",
                                    numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dP_T_L_2p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_2p"];
    std::string hEcal_vs_dP_T_tot_2p_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_2p"];

    // Ecal reconstruction histograms (pFDpCD)

    // Ecal plots (pFDpCD)
    sEcal_pFDpCD = OwnHistogram(new THStack("E_{cal} Reconstruction (pFDpCD)", "E_{cal} Reconstruction (pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]"));
    std::string sEcal_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_stack_pFDpCD"];

    hEcal_All_Int_pFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (All Int., pFDpCD)", "E_{cal} Reconstruction (All Int., pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_QEL_pFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (QEL only, pFDpCD)", "E_{cal} Reconstruction (QEL only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_MEC_pFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (MEC only, pFDpCD)", "E_{cal} Reconstruction (MEC only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_RES_pFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (RES only, pFDpCD)", "E_{cal} Reconstruction (RES only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_DIS_pFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (DIS only, pFDpCD)", "E_{cal} Reconstruction (DIS only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_All_Int_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_All_Int_pFDpCD"];
    std::string hEcal_QEL_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_QEL_pFDpCD"];
    std::string hEcal_MEC_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_MEC_pFDpCD"];
    std::string hEcal_RES_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_RES_pFDpCD"];
    std::string hEcal_DIS_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_DIS_pFDpCD"];

    // Ecal vs. momentum (pFDpCD)
    hEcal_vs_P_e_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. P_{e} (All Int., pFDpCD)", "E_{cal} vs. P_{e} (All Int., pFDpCD);P_{e} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Momentum_llim,
                                   Momentum_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_e_test_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., pFDpCD)", "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., pFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                        numTH2Dbins_E_cal_Plots, Momentum_llim, Momentum_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_P_e_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_pFDpCD"];

    hEcal_vs_P_pFD_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. P_{pFD} (All Int., pFDpCD)", "E_{cal} vs. P_{pFD} (All Int., pFDpCD);P_{pFD} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, P_nucFD_llim,
                                     P_nucFD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_pCD_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. P_{pCD} (All Int., pFDpCD)", "E_{cal} vs. P_{pCD} (All Int., pFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, P_nucCD_llim,
                                     P_nucCD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_pFD_test_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. P_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD)", "E_{cal} vs. P_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD);P_{pFD} [GeV/c];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, P_nucFD_llim, P_nucFD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_pCD_test_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD)", "E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, P_nucCD_llim, P_nucCD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_P_pFD_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_pFDpCD"];
    std::string hEcal_vs_P_pCD_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_pFDpCD"];

    // Ecal vs. angles (pFDpCD)
    hEcal_vs_Theta_e_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #theta_{e} (All Int., pFDpCD)", "E_{cal} vs. #theta_{e} (All Int., pFDpCD);#theta_{e} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots,
                                       Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_e_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #phi_{e} (All Int., pFDpCD)", "E_{cal} vs. #phi_{e} (All Int., pFDpCD);#phi_{e} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Phi_llim,
                                     Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_e_test_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., pFDpCD)", "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., pFDpCD);#theta_{e} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_e_test_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., pFDpCD)", "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., pFDpCD);#phi_{e} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_e_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_pFDpCD"];
    std::string hEcal_vs_Phi_e_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_pFDpCD"];

    hEcal_vs_Theta_pFD_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #theta_{pFD} (All Int., pFDpCD)", "E_{cal} vs. #theta_{pFD} (All Int., pFDpCD);#theta_{pFD} [#circ];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_pFD_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #phi_{pFD} (All Int., pFDpCD)", "E_{cal} vs. #phi_{pFD} (All Int., pFDpCD);#phi_{pFD} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots,
                                       Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_pFD_test_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD)", "E_{cal} vs. #theta_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD);#theta_{pFD} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_pFD_test_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #phi_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD)", "E_{cal} vs. #phi_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD);#phi_{pFD} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_pFD_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_pFDpCD"];
    std::string hEcal_vs_Phi_pFD_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_pFDpCD"];

    hEcal_vs_Theta_pCD_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #theta_{pCD} (All Int., pFDpCD)", "E_{cal} vs. #theta_{pCD} (All Int., pFDpCD);#theta_{pCD} [#circ];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_llim_CD, Theta_ulim_CD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_pCD_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #phi_{pCD} (All Int., pFDpCD)", "E_{cal} vs. #phi_{pCD} (All Int., pFDpCD);#phi_{pCD} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots,
                                       Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_pCD_test_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD)", "E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD);#theta_{pCD} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_CD, Theta_ulim_CD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_pCD_test_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD)", "E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD);#phi_{pCD} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_pCD_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_pFDpCD"];
    std::string hEcal_vs_Phi_pCD_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_pFDpCD"];

    // Ecal vs. dAlpha_T (pFDpCD)
    hEcal_vs_dAlpha_T_L_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., pFDpCD)", "E_{cal} vs. #delta#alpha_{T,L} (All Int., pFDpCD);#delta#alpha_{T,L} [#circ];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (All Int., pFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_QEL_Only_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (QEL only, pFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (QEL only, pFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_MEC_Only_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (MEC only, pFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (MEC only, pFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_RES_Only_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (RES only, pFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (RES only, pFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_DIS_Only_pFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (DIS only, pFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (DIS only, pFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dAlpha_T_L_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_pFDpCD"];
    std::string hEcal_vs_dAlpha_T_tot_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_pFDpCD"];

    // Ecal vs. dP_T (pFDpCD)
    hEcal_vs_dP_T_L_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., pFDpCD)", "E_{cal} vs. #deltaP_{T,L} (All Int., pFDpCD);#deltaP_{T,L} [GeV];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., pFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (All Int., pFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                        numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_QEL_Only_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (QEL only, pFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (QEL only, pFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                                 numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_MEC_Only_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (MEC only, pFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (MEC only, pFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                                 numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_RES_Only_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (RES only, pFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (RES only, pFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                                 numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_DIS_Only_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (DIS only, pFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (DIS only, pFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                                 numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dP_T_L_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_pFDpCD"];
    std::string hEcal_vs_dP_T_tot_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_pFDpCD"];

    // Ecal vs. W (pFDpCD)
    hEcal_vs_W_pFDpCD = OwnHistogram(new TH2D("E_{cal} vs. W (All Int., pFDpCD)",
                                 "E_{cal} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV];",
                                 numTH2Dbins_E_cal_Plots, W_llim, W_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_W_pFDpCD_Dir = directories.Ecal_dir_map["Ecal_stack_pFDpCD"];

    // Ecal reconstruction histograms (nFDpCD)

    // Ecal plots (nFDpCD)
    sEcal_nFDpCD = OwnHistogram(new THStack("E_{cal} Reconstruction (nFDpCD)", "E_{cal} Reconstruction (nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]"));
    std::string sEcal_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_stack_nFDpCD"];

    hEcal_All_Int_nFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (All Int., nFDpCD)", "E_{cal} Reconstruction (All Int., nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_QEL_nFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (QEL only, nFDpCD)", "E_{cal} Reconstruction (QEL only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_MEC_nFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (MEC only, nFDpCD)", "E_{cal} Reconstruction (MEC only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_RES_nFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (RES only, nFDpCD)", "E_{cal} Reconstruction (RES only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    hEcal_DIS_nFDpCD =
        OwnHistogram(new TH1D("E_{cal} reco. (DIS only, nFDpCD)", "E_{cal} Reconstruction (DIS only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]", numTH1Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_All_Int_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_All_Int_nFDpCD"];
    std::string hEcal_QEL_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_QEL_nFDpCD"];
    std::string hEcal_MEC_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_MEC_nFDpCD"];
    std::string hEcal_RES_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_RES_nFDpCD"];
    std::string hEcal_DIS_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_DIS_nFDpCD"];

    // Ecal vs. momentum (nFDpCD)
    hEcal_vs_P_e_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. P_{e} (All Int., nFDpCD)", "E_{cal} vs. P_{e} (All Int., nFDpCD);P_{e} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Momentum_llim,
                                   Momentum_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_e_test_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., nFDpCD)", "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., nFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                        numTH2Dbins_E_cal_Plots, Momentum_llim, Momentum_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_P_e_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_nFDpCD"];

    hEcal_vs_P_nFD_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. P_{nFD} (All Int., nFDpCD)", "E_{cal} vs. P_{nFD} (All Int., nFDpCD);P_{nFD} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, P_nucFD_llim,
                                     P_nucFD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_pCD_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. P_{pCD} (All Int., nFDpCD)", "E_{cal} vs. P_{pCD} (All Int., nFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, P_nucCD_llim,
                                     P_nucCD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_nFD_test_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. P_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD)", "E_{cal} vs. P_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD);P_{nFD} [GeV/c];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, P_nucFD_llim, P_nucFD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_P_pCD_test_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD)", "E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, P_nucCD_llim, P_nucCD_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_P_nFD_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_nFDpCD"];
    std::string hEcal_vs_P_pCD_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Mom_nFDpCD"];

    // Ecal vs. angles (nFDpCD)
    hEcal_vs_Theta_e_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #theta_{e} (All Int., nFDpCD)", "E_{cal} vs. #theta_{e} (All Int., nFDpCD);#theta_{e} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots,
                                       Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_e_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #phi_{e} (All Int., nFDpCD)", "E_{cal} vs. #phi_{e} (All Int., nFDpCD);#phi_{e} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots, Phi_llim,
                                     Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_e_test_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., nFDpCD)", "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., nFDpCD);#theta_{e} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_e_test_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., nFDpCD)", "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., nFDpCD);#phi_{e} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_e_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_nFDpCD"];
    std::string hEcal_vs_Phi_e_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_nFDpCD"];

    hEcal_vs_Theta_nFD_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #theta_{nFD} (All Int., nFDpCD)", "E_{cal} vs. #theta_{nFD} (All Int., nFDpCD);#theta_{nFD} [#circ];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_nFD_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #phi_{nFD} (All Int., nFDpCD)", "E_{cal} vs. #phi_{nFD} (All Int., nFDpCD);#phi_{nFD} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots,
                                       Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_nFD_test_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD)", "E_{cal} vs. #theta_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD);#theta_{nFD} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_nFD_test_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #phi_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD)", "E_{cal} vs. #phi_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD);#phi_{nFD} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_nFD_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_nFDpCD"];
    std::string hEcal_vs_Phi_nFD_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_nFDpCD"];

    hEcal_vs_Theta_pCD_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #theta_{pCD} (All Int., nFDpCD)", "E_{cal} vs. #theta_{pCD} (All Int., nFDpCD);#theta_{pCD} [#circ];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_llim_CD, Theta_ulim_CD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_pCD_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #phi_{pCD} (All Int., nFDpCD)", "E_{cal} vs. #phi_{pCD} (All Int., nFDpCD);#phi_{pCD} [#circ];E_{cal} [GeV];", numTH2Dbins_E_cal_Plots,
                                       Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Theta_pCD_test_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD)", "E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD);#theta_{pCD} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Theta_llim_CD, Theta_ulim_CD, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_Phi_pCD_test_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD)", "E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD);#phi_{pCD} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, Phi_llim, Phi_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_Theta_pCD_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_nFDpCD"];
    std::string hEcal_vs_Phi_pCD_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_Ang_nFDpCD"];

    // Ecal vs. dAlpha_T (nFDpCD)
    hEcal_vs_dAlpha_T_L_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., nFDpCD)", "E_{cal} vs. #delta#alpha_{T,L} (All Int., nFDpCD);#delta#alpha_{T,L} [#circ];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_QEL_Only_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (QEL only, nFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (QEL only, nFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_MEC_Only_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (MEC only, nFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (MEC only, nFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_RES_Only_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (RES only, nFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (RES only, nFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dAlpha_T_tot_DIS_Only_nFDpCD =
        OwnHistogram(new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (DIS only, nFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (DIS only, nFDpCD);#delta#alpha_{T,tot} [#circ];E_{cal} [GeV];",
                 numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dAlpha_T_L_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_nFDpCD"];
    std::string hEcal_vs_dAlpha_T_tot_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_nFDpCD"];

    // Ecal vs. dP_T (nFDpCD)
    hEcal_vs_dP_T_L_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., nFDpCD)", "E_{cal} vs. #deltaP_{T,L} (All Int., nFDpCD);#deltaP_{T,L} [GeV];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                        numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_QEL_Only_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (QEL only, nFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (QEL only, nFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                                 numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_MEC_Only_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (MEC only, nFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (MEC only, nFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                                 numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_RES_Only_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (RES only, nFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (RES only, nFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                                 numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    hEcal_vs_dP_T_tot_DIS_Only_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. #deltaP_{T,tot} (DIS only, nFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (DIS only, nFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                                 numTH2Dbins_E_cal_Plots, 0, dP_T_lim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_dP_T_L_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_nFDpCD"];
    std::string hEcal_vs_dP_T_tot_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_rec_vs_TKI_nFDpCD"];

    // Ecal vs. W (nFDpCD)
    hEcal_vs_W_nFDpCD = OwnHistogram(new TH2D("E_{cal} vs. W (All Int., nFDpCD)",
                                 "E_{cal} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV];",
                                 numTH2Dbins_E_cal_Plots, W_llim, W_ulim, numTH2Dbins_E_cal_Plots, 0, beamE135));
    std::string hEcal_vs_W_nFDpCD_Dir = directories.Ecal_dir_map["Ecal_stack_nFDpCD"];
#pragma endregion
}

void FinalStateHistograms::InitTKIHistograms(const HistogramDefinitionSettings& hDefSettings) {
    auto& directories = tki_directories_;
    const int numTH1Dbins_TKI_dP_T_Plots = hDefSettings.numTH1Dbins_TKI_dP_T_Plots;
    const int numTH1Dbins_TKI_dAlpha_T_Plots = hDefSettings.numTH1Dbins_TKI_dAlpha_T_Plots;
    const int numTH2Dbins_TKI_Plots = hDefSettings.numTH2Dbins_TKI_Plots;
    const double dP_T_lim = hDefSettings.dP_T_lim;
    const double W_llim = hDefSettings.W_llim;
    const double W_ulim = hDefSettings.W_ulim;

#pragma region /* TKI histograms */

    // TKI histograms (1p)
    sdP_T_1p = OwnHistogram(new THStack("#deltaP_{T} vs. #deltaP_{T,tot} (1p)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (1p);#deltaP_{T} [GeV/c]"));
    hdP_T_1p = OwnHistogram(new TH1D("#deltaP_{T} (1p)", "#deltaP_{T} histogram (1p);#deltaP_{T} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,p}| [GeV/c]", numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    std::string hdP_T_1p_Dir = directories.TKI_dir_map["dP_T_1p"];

    sdAlpha_T_1p = OwnHistogram(new THStack("#delta#alpha_{T} & #delta#alpha_{T,tot} (1p)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (1p);#delta#alpha_{T} [#circ]"));
    hdAlpha_T_1p = OwnHistogram(new TH1D("#delta#alpha_{T} (1p)", "#delta#alpha_{T} histogram (1p);#delta#alpha_{T} [#circ]", numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180));
    std::string hdAlpha_T_1p_Dir = directories.TKI_dir_map["dAlpha_T_1p"];

    sdPhi_T_1p = OwnHistogram(new THStack("#delta#phi_{T} & #delta#phi_{T,tot} (1p)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (1p);#delta#phi_{T} [#circ]"));
    hdPhi_T_1p = OwnHistogram(new TH1D("#delta#phi_{T} (1p)", "#delta#phi_{T} histogram (1p);#delta#phi_{T} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    std::string hdPhi_T_1p_Dir = directories.TKI_dir_map["dPhi_T_1p"];

    hdP_T_vs_dAlpha_T_1p = OwnHistogram(new TH2D("#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1p)", "#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1p);#delta#alpha_{T} [#circ];#deltaP_{T} [GeV/c];",
                                    numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    std::string hdP_T_vs_dAlpha_T_1p_Dir = directories.TKI_dir_map["dP_T_vs_dAlpha_T_1p"];

    // TKI histograms (1n)
    sdP_T_1n = OwnHistogram(new THStack("#deltaP_{T} vs. #deltaP_{T,tot} (1n)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (1n);#deltaP_{T} [GeV/c]"));
    hdP_T_1n = OwnHistogram(new TH1D("#deltaP_{T} (1n)", "#deltaP_{T} histogram (1n);#deltaP_{T} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,n}| [GeV/c]", numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    std::string hdP_T_1n_Dir = directories.TKI_dir_map["dP_T_1n"];

    sdAlpha_T_1n = OwnHistogram(new THStack("#delta#alpha_{T} & #delta#alpha_{T,tot} (1n)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (1n);#delta#alpha_{T} [#circ]"));
    hdAlpha_T_1n = OwnHistogram(new TH1D("#delta#alpha_{T} (1n)", "#delta#alpha_{T} histogram (1n);#delta#alpha_{T} [#circ]", numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180));
    std::string hdAlpha_T_1n_Dir = directories.TKI_dir_map["dAlpha_T_1n"];

    sdPhi_T_1n = OwnHistogram(new THStack("#delta#phi_{T} & #delta#phi_{T,tot} (1n)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (1n);#delta#phi_{T} [#circ]"));
    hdPhi_T_1n = OwnHistogram(new TH1D("#delta#phi_{T} (1n)", "#delta#phi_{T} histogram (1n);#delta#phi_{T} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    std::string hdPhi_T_1n_Dir = directories.TKI_dir_map["dPhi_T_1n"];

    hdP_T_vs_dAlpha_T_1n = OwnHistogram(new TH2D("#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1n)", "#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1n);#delta#alpha_{T} [#circ];#deltaP_{T} [GeV/c];",
                                    numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    std::string hdP_T_vs_dAlpha_T_1n_Dir = directories.TKI_dir_map["dP_T_vs_dAlpha_T_1n"];

    // TKI histograms (2p)
    sdP_T_2p = OwnHistogram(new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (2p)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (2p);#deltaP_{T} [GeV/c]"));
    hdP_T_L_2p =
        OwnHistogram(new TH1D("#deltaP_{T,L} (2p)", "#deltaP_{T,L} by Leading Proton (2p);#deltaP_{T,L} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,1}| [GeV/c]", numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_2p = OwnHistogram(new TH1D("#deltaP_{T,tot} (2p)", "#deltaP_{T,tot} by Momentum Sum (2p);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,1} + #font[62]{p}_{T,2}| [GeV/c];",
                            numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    std::string hdP_T_L_2p_Dir = directories.TKI_dir_map["dP_T_2p"];
    std::string hdP_T_tot_2p_Dir = directories.TKI_dir_map["dP_T_2p"];

    sdAlpha_T_2p = OwnHistogram(new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (2p)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (2p);#delta#alpha_{T} [#circ]"));
    hdAlpha_T_L_2p = OwnHistogram(new TH1D("#delta#alpha_{T,L} (2p)", "#delta#alpha_{T,L} by Leading Proton (2p);#delta#alpha_{T,L} [#circ]", numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180));
    hdAlpha_T_tot_2p = OwnHistogram(new TH1D("#delta#alpha_{T,tot} (2p)", "#delta#alpha_{T,tot} by Momentum Sum (2p);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180));
    std::string hdAlpha_T_L_2p_Dir = directories.TKI_dir_map["dAlpha_T_2p"];
    std::string hdAlpha_T_tot_2p_Dir = directories.TKI_dir_map["dAlpha_T_2p"];

    sdPhi_T_2p = OwnHistogram(new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (2p)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (2p);#delta#phi_{T} [#circ]"));
    hdPhi_T_L_2p = OwnHistogram(new TH1D("#delta#phi_{T,L} (2p)", "#delta#phi_{T,L} by Leading Proton (2p);#delta#phi_{T,L} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    hdPhi_T_tot_2p = OwnHistogram(new TH1D("#delta#phi_{T,tot} (2p)", "#delta#phi_{T,tot} by Momentum Sum (2p);#delta#phi_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    std::string hdPhi_T_L_2p_Dir = directories.TKI_dir_map["dPhi_T_2p"];
    std::string hdPhi_T_tot_2p_Dir = directories.TKI_dir_map["dPhi_T_2p"];

    hdP_T_L_vs_dAlpha_T_L_2p =
        OwnHistogram(new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p)", "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [#circ];#deltaP_{T,L} [GeV/c];",
                 numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    hdP_T_tot_vs_dAlpha_T_tot_2p =
        OwnHistogram(new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p)", "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [#circ];#deltaP_{T,tot} [GeV/c];",
                 numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    std::string hdP_T_L_vs_dAlpha_T_L_2p_Dir = directories.TKI_dir_map["dP_T_vs_dAlpha_T_2p"];
    std::string hdP_T_tot_vs_dAlpha_T_tot_2p_Dir = directories.TKI_dir_map["dP_T_vs_dAlpha_T_2p"];

    // TKI histograms (pFDpCD)

    // TKI plots (pFDpCD)
    sdP_T_pFDpCD = OwnHistogram(new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (pFDpCD)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (pFDpCD);#deltaP_{T} [GeV/c]"));
    sdP_T_tot_pFDpCD = OwnHistogram(new THStack("#deltaP_{T,tot} stack (pFDpCD)", "#deltaP_{T,tot} stack (pFDpCD);#deltaP_{T,tot} [GeV/c]"));
    hdP_T_L_pFDpCD = OwnHistogram(new TH1D("#deltaP_{T,L} (pFDpCD)", "#deltaP_{T,L} by leading proton (pFDpCD);#deltaP_{T,L} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pL}| [GeV/c]",
                              numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_pFDpCD =
        OwnHistogram(new TH1D("#deltaP_{T,tot} (pFDpCD)", "#deltaP_{T,tot} by Momentum Sum (pFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                 numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_QEL_Only_pFDpCD = OwnHistogram(new TH1D("#deltaP_{T,tot} (QEL only, pFDpCD)",
                                         "#deltaP_{T,tot} by Momentum Sum (QEL only, pFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                                         numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_MEC_Only_pFDpCD = OwnHistogram(new TH1D("#deltaP_{T,tot} (MEC only, pFDpCD)",
                                         "#deltaP_{T,tot} by Momentum Sum (MEC only, pFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                                         numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_RES_Only_pFDpCD = OwnHistogram(new TH1D("#deltaP_{T,tot} (RES only, pFDpCD)",
                                         "#deltaP_{T,tot} by Momentum Sum (RES only, pFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                                         numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_DIS_Only_pFDpCD = OwnHistogram(new TH1D("#deltaP_{T,tot} (DIS only, pFDpCD)",
                                         "#deltaP_{T,tot} by Momentum Sum (DIS only, pFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                                         numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    std::string hdP_T_L_pFDpCD_Dir = directories.TKI_dir_map["dP_T_pFDpCD"];
    std::string hdP_T_tot_pFDpCD_Dir = directories.TKI_dir_map["dP_T_pFDpCD"];

    sdAlpha_T_pFDpCD = OwnHistogram(new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (pFDpCD)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (pFDpCD);#delta#alpha_{T} [#circ]"));
    sdAlpha_T_tot_pFDpCD = OwnHistogram(new THStack("#delta#alpha_{T,tot} stack (pFDpCD)", "#delta#alpha_{T,tot} stack (pFDpCD);#delta#alpha_{T,tot} [#circ]"));
    hdAlpha_T_L_pFDpCD = OwnHistogram(new TH1D("#delta#alpha_{T,L} (pFDpCD)", "#delta#alpha_{T,L} by leading proton (pFDpCD);#delta#alpha_{T,L} [#circ]", numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180));
    hdAlpha_T_tot_pFDpCD = OwnHistogram(new TH1D("#delta#alpha_{T,tot} (pFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (pFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180));
    hdAlpha_T_tot_QEL_Only_pFDpCD =
        OwnHistogram(new TH1D("#delta#alpha_{T,tot} (QEL only, pFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (QEL only, pFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    hdAlpha_T_tot_MEC_Only_pFDpCD =
        OwnHistogram(new TH1D("#delta#alpha_{T,tot} (MEC only, pFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (MEC only, pFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    hdAlpha_T_tot_RES_Only_pFDpCD =
        OwnHistogram(new TH1D("#delta#alpha_{T,tot} (RES only, pFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (RES only, pFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    hdAlpha_T_tot_DIS_Only_pFDpCD =
        OwnHistogram(new TH1D("#delta#alpha_{T,tot} (DIS only, pFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (DIS only, pFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    std::string hdAlpha_T_L_pFDpCD_Dir = directories.TKI_dir_map["dAlpha_T_pFDpCD"];
    std::string hdAlpha_T_tot_pFDpCD_Dir = directories.TKI_dir_map["dAlpha_T_pFDpCD"];

    sdPhi_T_pFDpCD = OwnHistogram(new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (pFDpCD)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (pFDpCD);#delta#phi_{T} [#circ]"));
    hdPhi_T_L_pFDpCD = OwnHistogram(new TH1D("#delta#phi_{T,L} (pFDpCD)", "#delta#phi_{T,L} by leading proton (pFDpCD);#delta#phi_{T,L} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    hdPhi_T_tot_pFDpCD = OwnHistogram(new TH1D("#delta#phi_{T,tot} (pFDpCD)", "#delta#phi_{T,tot} by Momentum Sum (pFDpCD);#delta#phi_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    std::string hdPhi_T_L_pFDpCD_Dir = directories.TKI_dir_map["dPhi_T_pFDpCD"];
    std::string hdPhi_T_tot_pFDpCD_Dir = directories.TKI_dir_map["dPhi_T_pFDpCD"];

    hdP_T_L_vs_dAlpha_T_L_pFDpCD =
        OwnHistogram(new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., pFDpCD)", "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., pFDpCD);#delta#alpha_{T,L} [#circ];#deltaP_{T,L} [GeV/c];",
                 numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    hdP_T_tot_vs_dAlpha_T_tot_pFDpCD = OwnHistogram(new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)",
                                                "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., pFDpCD);#delta#alpha_{T,tot} [#circ];#deltaP_{T,tot} [GeV/c];", numTH2Dbins_TKI_Plots, 0,
                                                180, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    std::string hdP_T_L_vs_dAlpha_T_L_pFDpCD_Dir = directories.TKI_dir_map["dP_T_vs_dAlpha_T_pFDpCD"];
    std::string hdP_T_tot_vs_dAlpha_T_tot_pFDpCD_Dir = directories.TKI_dir_map["dP_T_vs_dAlpha_T_pFDpCD"];

    // TKI vs. W (pFDpCD)
    hdP_T_L_vs_W_pFDpCD = OwnHistogram(new TH2D("#deltaP_{T,L} vs. W (All Int., pFDpCD)",
                                   "#deltaP_{T,L} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                   "#deltaP_{T,L} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pL}| [GeV/c];",
                                   numTH2Dbins_TKI_Plots, W_llim, W_ulim, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    hdP_T_tot_vs_W_pFDpCD = OwnHistogram(new TH2D("#deltaP_{T,tot} vs. W (All Int., pFDpCD)",
                                     "#deltaP_{T,tot} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                     "#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c];",
                                     numTH2Dbins_TKI_Plots, W_llim, W_ulim, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    std::string hdP_T_L_vs_W_pFDpCD_Dir = directories.TKI_dir_map["dP_T_pFDpCD"];
    std::string hdP_T_tot_vs_W_pFDpCD_Dir = directories.TKI_dir_map["dP_T_pFDpCD"];

    hdAlpha_T_L_vs_W_pFDpCD = OwnHistogram(new TH2D("#delta#alpha_{T,L} vs. W (All Int., pFDpCD)",
                                       "#delta#alpha_{T,L} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#delta#alpha_{T,L} [#circ];",
                                       numTH2Dbins_TKI_Plots, W_llim, W_ulim, numTH2Dbins_TKI_Plots, 0, 180));
    hdAlpha_T_tot_vs_W_pFDpCD = OwnHistogram(new TH2D("#delta#alpha_{T,tot} vs. W (All Int., pFDpCD)",
                                         "#delta#alpha_{T,tot} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#delta#alpha_{T,tot} [#circ];",
                                         numTH2Dbins_TKI_Plots, W_llim, W_ulim, numTH2Dbins_TKI_Plots, 0, 180));
    std::string hdAlpha_T_L_vs_W_pFDpCD_Dir = directories.TKI_dir_map["dAlpha_T_pFDpCD"];
    std::string hdAlpha_T_tot_vs_W_pFDpCD_Dir = directories.TKI_dir_map["dAlpha_T_pFDpCD"];

    // TKI histograms (nFDpCD)

    // TKI plots (nFDpCD)
    sdP_T_nFDpCD = OwnHistogram(new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (nFDpCD)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (nFDpCD);#deltaP_{T} [GeV/c]"));
    sdP_T_tot_nFDpCD = OwnHistogram(new THStack("#deltaP_{T,tot} stack (nFDpCD)", "#deltaP_{T,tot} stack (nFDpCD);#deltaP_{T,tot} [GeV/c]"));
    hdP_T_L_nFDpCD = OwnHistogram(new TH1D("#deltaP_{T,L} (nFDpCD)", "#deltaP_{T,L} by leading nucleon (nFDpCD);#deltaP_{T,L} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,nL}| [GeV/c]",
                              numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_nFDpCD =
        OwnHistogram(new TH1D("#deltaP_{T,tot} (nFDpCD)", "#deltaP_{T,tot} by Momentum Sum (nFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,nFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                 numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_QEL_Only_nFDpCD = OwnHistogram(new TH1D("#deltaP_{T,tot} (QEL only, nFDpCD)",
                                         "#deltaP_{T,tot} by Momentum Sum (QEL only, nFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                                         numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_MEC_Only_nFDpCD = OwnHistogram(new TH1D("#deltaP_{T,tot} (MEC only, nFDpCD)",
                                         "#deltaP_{T,tot} by Momentum Sum (MEC only, nFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                                         numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_RES_Only_nFDpCD = OwnHistogram(new TH1D("#deltaP_{T,tot} (RES only, nFDpCD)",
                                         "#deltaP_{T,tot} by Momentum Sum (RES only, nFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                                         numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    hdP_T_tot_DIS_Only_nFDpCD = OwnHistogram(new TH1D("#deltaP_{T,tot} (DIS only, nFDpCD)",
                                         "#deltaP_{T,tot} by Momentum Sum (DIS only, nFDpCD);#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,pFD} + #font[62]{p}_{T,pCD}| [GeV/c]",
                                         numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_lim));
    std::string hdP_T_L_nFDpCD_Dir = directories.TKI_dir_map["dP_T_nFDpCD"];
    std::string hdP_T_tot_nFDpCD_Dir = directories.TKI_dir_map["dP_T_nFDpCD"];

    sdAlpha_T_nFDpCD = OwnHistogram(new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (nFDpCD)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (nFDpCD);#delta#alpha_{T} [#circ]"));
    sdAlpha_T_tot_nFDpCD = OwnHistogram(new THStack("#delta#alpha_{T,tot} stack (nFDpCD)", "#delta#alpha_{T,tot} stack (nFDpCD);#delta#alpha_{T,tot} [#circ]"));
    hdAlpha_T_L_nFDpCD = OwnHistogram(new TH1D("#delta#alpha_{T,L} (nFDpCD)", "#delta#alpha_{T,L} by leading nucleon (nFDpCD);#delta#alpha_{T,L} [#circ]", numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180));
    hdAlpha_T_tot_nFDpCD = OwnHistogram(new TH1D("#delta#alpha_{T,tot} (nFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (nFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180));
    hdAlpha_T_tot_QEL_Only_nFDpCD =
        OwnHistogram(new TH1D("#delta#alpha_{T,tot} (QEL only, nFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (QEL only, nFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    hdAlpha_T_tot_MEC_Only_nFDpCD =
        OwnHistogram(new TH1D("#delta#alpha_{T,tot} (MEC only, nFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (MEC only, nFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    hdAlpha_T_tot_RES_Only_nFDpCD =
        OwnHistogram(new TH1D("#delta#alpha_{T,tot} (RES only, nFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (RES only, nFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    hdAlpha_T_tot_DIS_Only_nFDpCD =
        OwnHistogram(new TH1D("#delta#alpha_{T,tot} (DIS only, nFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (DIS only, nFDpCD);#delta#alpha_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    std::string hdAlpha_T_L_nFDpCD_Dir = directories.TKI_dir_map["dAlpha_T_nFDpCD"];
    std::string hdAlpha_T_tot_nFDpCD_Dir = directories.TKI_dir_map["dAlpha_T_nFDpCD"];

    sdPhi_T_nFDpCD = OwnHistogram(new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (nFDpCD)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (nFDpCD);#delta#phi_{T} [#circ]"));
    hdPhi_T_L_nFDpCD = OwnHistogram(new TH1D("#delta#phi_{T,L} (nFDpCD)", "#delta#phi_{T,L} by leading nucleon (nFDpCD);#delta#phi_{T,L} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    hdPhi_T_tot_nFDpCD = OwnHistogram(new TH1D("#delta#phi_{T,tot} (nFDpCD)", "#delta#phi_{T,tot} by Momentum Sum (nFDpCD);#delta#phi_{T,tot} [#circ]", numTH1Dbins_TKI_dP_T_Plots, 0, 180));
    std::string hdPhi_T_L_nFDpCD_Dir = directories.TKI_dir_map["dPhi_T_nFDpCD"];
    std::string hdPhi_T_tot_nFDpCD_Dir = directories.TKI_dir_map["dPhi_T_nFDpCD"];

    hdP_T_L_vs_dAlpha_T_L_nFDpCD =
        OwnHistogram(new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., nFDpCD)", "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., nFDpCD);#delta#alpha_{T,L} [#circ];#deltaP_{T,L} [GeV/c];",
                 numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    hdP_T_tot_vs_dAlpha_T_tot_nFDpCD = OwnHistogram(new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)",
                                                "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD);#delta#alpha_{T,tot} [#circ];#deltaP_{T,tot} [GeV/c];", numTH2Dbins_TKI_Plots, 0,
                                                180, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    std::string hdP_T_L_vs_dAlpha_T_L_nFDpCD_Dir = directories.TKI_dir_map["dP_T_vs_dAlpha_T_nFDpCD"];
    std::string hdP_T_tot_vs_dAlpha_T_tot_nFDpCD_Dir = directories.TKI_dir_map["dP_T_vs_dAlpha_T_nFDpCD"];

    // TKI vs. W (nFDpCD)
    hdP_T_L_vs_W_nFDpCD = OwnHistogram(new TH2D("#deltaP_{T,L} vs. W (All Int., nFDpCD)",
                                   "#deltaP_{T,L} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                   "#deltaP_{T,L} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,nL}| [GeV/c];",
                                   numTH2Dbins_TKI_Plots, W_llim, W_ulim, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    hdP_T_tot_vs_W_nFDpCD = OwnHistogram(new TH2D("#deltaP_{T,tot} vs. W (All Int., nFDpCD)",
                                     "#deltaP_{T,tot} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                     "#deltaP_{T,tot} = |#font[62]{p}_{T,e} + #font[62]{p}_{T,nFD} + #font[62]{p}_{T,pCD}| [GeV/c];",
                                     numTH2Dbins_TKI_Plots, W_llim, W_ulim, numTH2Dbins_TKI_Plots, 0, dP_T_lim));
    std::string hdP_T_L_vs_W_nFDpCD_Dir = directories.TKI_dir_map["dP_T_nFDpCD"];
    std::string hdP_T_tot_vs_W_nFDpCD_Dir = directories.TKI_dir_map["dP_T_nFDpCD"];

    hdAlpha_T_L_vs_W_nFDpCD = OwnHistogram(new TH2D("#delta#alpha_{T,L} vs. W (All Int., nFDpCD)",
                                       "#delta#alpha_{T,L} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#delta#alpha_{T,L} [#circ];",
                                       numTH2Dbins_TKI_Plots, W_llim, W_ulim, numTH2Dbins_TKI_Plots, 0, 180));
    hdAlpha_T_tot_vs_W_nFDpCD = OwnHistogram(new TH2D("#delta#alpha_{T,tot} vs. W (All Int., nFDpCD)",
                                         "#delta#alpha_{T,tot} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#delta#alpha_{T,tot} [#circ];",
                                         numTH2Dbins_TKI_Plots, W_llim, W_ulim, numTH2Dbins_TKI_Plots, 0, 180));
    std::string hdAlpha_T_L_vs_W_nFDpCD_Dir = directories.TKI_dir_map["dAlpha_T_nFDpCD"];
    std::string hdAlpha_T_tot_vs_W_nFDpCD_Dir = directories.TKI_dir_map["dAlpha_T_nFDpCD"];
#pragma endregion
}

void FinalStateHistograms::InitOmegaHistograms(const HistogramDefinitionSettings& hDefSettings) {
    auto& directories = omega_directories_;
    const int numTH1Dbins_ET_Plots = hDefSettings.numTH1Dbins_ET_Plots;
    const double beamEMax = hDefSettings.Momentum_ulim;

#pragma region /* Energy Transfer (ET or omega) histograms */

    // Energy Transfer (ET) histograms
    std::string tET_All_Ang = "Energy transfer #omega for every #theta_{e}", tET15 = "Energy transfer #omega Around #theta_{e} = 15#circ";

    // ET for every theta_e (1p)
    sET_All_Ang_All_Int_1p_FD = OwnHistogram(new THStack("#omega for all #theta_{e} (1p)", "Energy transfer #omega for all #theta_{e} (1p);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET_All_Ang_All_Int_1p_FD_Dir = directories.Omega_dir_map["ETrans_All_Ang_stack_1p"];

    hET_All_Ang_All_Int_1p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (All Int., 1p)", "Energy transfer #omega for all #theta_{e} (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_QEL_1p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (QEL Only, 1p, FD)", "Energy transfer #omega for all #theta_{e} (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_MEC_1p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (MEC only, 1p, FD)", "Energy transfer #omega for all #theta_{e} (MEC only, 1p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_RES_1p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (RES only, 1p, FD)", "Energy transfer #omega for all #theta_{e} (RES only, 1p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_DIS_1p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (DIS only, 1p, FD)", "Energy transfer #omega for all #theta_{e} (DIS only, 1p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET_All_Ang_All_Int_1p_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_All_Ang_1p"];
    std::string hET_All_Ang_QEL_1p_FD_Dir = directories.Omega_dir_map["ETrans_QEL_All_Ang_1p"];
    std::string hET_All_Ang_MEC_1p_FD_Dir = directories.Omega_dir_map["ETrans_MEC_All_Ang_1p"];
    std::string hET_All_Ang_RES_1p_FD_Dir = directories.Omega_dir_map["ETrans_RES_All_Ang_1p"];
    std::string hET_All_Ang_DIS_1p_FD_Dir = directories.Omega_dir_map["ETrans_DIS_All_Ang_1p"];

    // ET for every theta_e (1n)
    sET_All_Ang_All_Int_1n_FD = OwnHistogram(new THStack("#omega for all #theta_{e} (1n)", "Energy transfer #omega for all #theta_{e} (1n);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET_All_Ang_All_Int_1n_FD_Dir = directories.Omega_dir_map["ETrans_All_Ang_stack_1n"];

    hET_All_Ang_All_Int_1n_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (All Int., 1n)", "Energy transfer #omega for all #theta_{e} (All Int., 1n);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_QEL_1n_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (QEL Only, 1n, FD)", "Energy transfer #omega for all #theta_{e} (QEL Only, 1n);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_MEC_1n_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (MEC only, 1n, FD)", "Energy transfer #omega for all #theta_{e} (MEC only, 1n);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_RES_1n_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (RES only, 1n, FD)", "Energy transfer #omega for all #theta_{e} (RES only, 1n);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_DIS_1n_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (DIS only, 1n, FD)", "Energy transfer #omega for all #theta_{e} (DIS only, 1n);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET_All_Ang_All_Int_1n_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_All_Ang_1n"];
    std::string hET_All_Ang_QEL_1n_FD_Dir = directories.Omega_dir_map["ETrans_QEL_All_Ang_1n"];
    std::string hET_All_Ang_MEC_1n_FD_Dir = directories.Omega_dir_map["ETrans_MEC_All_Ang_1n"];
    std::string hET_All_Ang_RES_1n_FD_Dir = directories.Omega_dir_map["ETrans_RES_All_Ang_1n"];
    std::string hET_All_Ang_DIS_1n_FD_Dir = directories.Omega_dir_map["ETrans_DIS_All_Ang_1n"];

    // ET for every theta_e (2p)
    sET_All_Ang_All_Int_2p_FD = OwnHistogram(new THStack("#omega for all #theta_{e} (2p)", "Energy transfer #omega for all #theta_{e} (2p);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET_All_Ang_All_Int_2p_FD_Dir = directories.Omega_dir_map["ETrans_All_Ang_stack_2p"];

    hET_All_Ang_All_Int_2p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (All Int., 2p)", "Energy transfer #omega for all #theta_{e} (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_QEL_2p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (QEL Only, 2p, FD)", "Energy transfer #omega for all #theta_{e} (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_MEC_2p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (MEC only, 2p, FD)", "Energy transfer #omega for all #theta_{e} (MEC only, 2p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_RES_2p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (RES only, 2p, FD)", "Energy transfer #omega for all #theta_{e} (RES only, 2p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_DIS_2p_FD =
        OwnHistogram(new TH1D("#omega for all #theta_{e} (DIS only, 2p, FD)", "Energy transfer #omega for all #theta_{e} (DIS only, 2p);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET_All_Ang_All_Int_2p_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_All_Ang_2p"];
    std::string hET_All_Ang_QEL_2p_FD_Dir = directories.Omega_dir_map["ETrans_QEL_All_Ang_2p"];
    std::string hET_All_Ang_MEC_2p_FD_Dir = directories.Omega_dir_map["ETrans_MEC_All_Ang_2p"];
    std::string hET_All_Ang_RES_2p_FD_Dir = directories.Omega_dir_map["ETrans_RES_All_Ang_2p"];
    std::string hET_All_Ang_DIS_2p_FD_Dir = directories.Omega_dir_map["ETrans_DIS_All_Ang_2p"];

    // ET for every theta_e (pFDpCD)
    sET_All_Ang_All_Int_pFDpCD_FD = OwnHistogram(new THStack("#omega for all #theta_{e} (pFDpCD)", "Energy transfer #omega for all #theta_{e} (pFDpCD);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET_All_Ang_All_Int_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_All_Ang_stack_pFDpCD"];

    hET_All_Ang_All_Int_pFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (All Int., pFDpCD)", "Energy transfer #omega for all #theta_{e} (All Int., pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                             numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_QEL_pFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (QEL Only, pFDpCD, FD)", "Energy transfer #omega for all #theta_{e} (QEL Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_MEC_pFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (MEC only, pFDpCD, FD)", "Energy transfer #omega for all #theta_{e} (MEC only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_RES_pFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (RES only, pFDpCD, FD)", "Energy transfer #omega for all #theta_{e} (RES only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_DIS_pFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (DIS only, pFDpCD, FD)", "Energy transfer #omega for all #theta_{e} (DIS only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET_All_Ang_All_Int_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_All_Ang_pFDpCD"];
    std::string hET_All_Ang_QEL_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_QEL_All_Ang_pFDpCD"];
    std::string hET_All_Ang_MEC_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_MEC_All_Ang_pFDpCD"];
    std::string hET_All_Ang_RES_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_RES_All_Ang_pFDpCD"];
    std::string hET_All_Ang_DIS_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_DIS_All_Ang_pFDpCD"];

    // ET for every theta_e (nFDpCD)
    sET_All_Ang_All_Int_nFDpCD_FD = OwnHistogram(new THStack("#omega for all #theta_{e} (nFDpCD)", "Energy transfer #omega for all #theta_{e} (nFDpCD);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET_All_Ang_All_Int_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_All_Ang_stack_nFDpCD"];

    hET_All_Ang_All_Int_nFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (All Int., nFDpCD)", "Energy transfer #omega for all #theta_{e} (All Int., nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                             numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_QEL_nFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (QEL Only, nFDpCD, FD)", "Energy transfer #omega for all #theta_{e} (QEL Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_MEC_nFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (MEC only, nFDpCD, FD)", "Energy transfer #omega for all #theta_{e} (MEC only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_RES_nFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (RES only, nFDpCD, FD)", "Energy transfer #omega for all #theta_{e} (RES only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamEMax));
    hET_All_Ang_DIS_nFDpCD_FD = OwnHistogram(new TH1D("#omega for all #theta_{e} (DIS only, nFDpCD, FD)", "Energy transfer #omega for all #theta_{e} (DIS only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET_All_Ang_All_Int_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_All_Ang_nFDpCD"];
    std::string hET_All_Ang_QEL_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_QEL_All_Ang_nFDpCD"];
    std::string hET_All_Ang_MEC_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_MEC_All_Ang_nFDpCD"];
    std::string hET_All_Ang_RES_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_RES_All_Ang_nFDpCD"];
    std::string hET_All_Ang_DIS_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_DIS_All_Ang_nFDpCD"];

    // ET around 15 Deg (1p)
    sET15_All_Int_1p_FD = OwnHistogram(new THStack("ET around #theta_{e} = 15#circ (1p)", "Energy transfer #omega Around #theta_{e} = 15#circ (1p);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET15_All_Int_1p_FD_Dir = directories.Omega_dir_map["ETrans_15_stack_1p"];

    hET15_All_Int_1p_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (All Int., 1p)", "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]",
                                   numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_QEL_1p_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (QEL Only, 1p, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_MEC_1p_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (MEC only, 1p, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (MEC only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_RES_1p_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (RES only, 1p, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (RES only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_DIS_1p_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (DIS only, 1p, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (DIS only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET15_All_Int_1p_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_15_1p"];
    std::string hET15_QEL_1p_FD_Dir = directories.Omega_dir_map["ETrans_QEL_15_1p"];
    std::string hET15_MEC_1p_FD_Dir = directories.Omega_dir_map["ETrans_MEC_15_1p"];
    std::string hET15_RES_1p_FD_Dir = directories.Omega_dir_map["ETrans_RES_15_1p"];
    std::string hET15_DIS_1p_FD_Dir = directories.Omega_dir_map["ETrans_DIS_15_1p"];

    // ET around 15 Deg (1n)
    sET15_All_Int_1n_FD = OwnHistogram(new THStack("ET around #theta_{e} = 15#circ (1n)", "Energy transfer #omega Around #theta_{e} = 15#circ (1n);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET15_All_Int_1n_FD_Dir = directories.Omega_dir_map["ETrans_15_stack_1n"];

    hET15_All_Int_1n_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (All Int., 1n)", "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 1n);#omega = E_{beam}-E_{e} [GeV]",
                                   numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_QEL_1n_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (QEL Only, 1n, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_MEC_1n_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (MEC only, 1n, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (MEC only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_RES_1n_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (RES only, 1n, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (RES only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_DIS_1n_FD = OwnHistogram(new TH1D("ET around #theta_{e} = 15#circ (DIS only, 1n, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (DIS only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET15_All_Int_1n_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_15_1n"];
    std::string hET15_QEL_1n_FD_Dir = directories.Omega_dir_map["ETrans_QEL_15_1n"];
    std::string hET15_MEC_1n_FD_Dir = directories.Omega_dir_map["ETrans_MEC_15_1n"];
    std::string hET15_RES_1n_FD_Dir = directories.Omega_dir_map["ETrans_RES_15_1n"];
    std::string hET15_DIS_1n_FD_Dir = directories.Omega_dir_map["ETrans_DIS_15_1n"];

    // ET around 15 Deg (2p)
    sET15_All_Int_2p_FD = OwnHistogram(new THStack("#omega around #theta_{e} = 15#circ (2p)", "Energy transfer #omega Around #theta_{e} = 15#circ (2p);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET15_All_Int_2p_FD_Dir = directories.Omega_dir_map["ETrans_15_stack_2p"];

    hET15_All_Int_2p_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (All Int., 2p)", "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]",
                                   numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_QEL_2p_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, 2p, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_MEC_2p_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (MEC only, 2p, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (MEC only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_RES_2p_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (RES only, 2p, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (RES only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_DIS_2p_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (DIS only, 2p, FD)", "Energy transfer #omega Around #theta_{e} = 15#circ (DIS only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                               numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET15_All_Int_2p_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_15_2p"];
    std::string hET15_QEL_2p_FD_Dir = directories.Omega_dir_map["ETrans_QEL_15_2p"];
    std::string hET15_MEC_2p_FD_Dir = directories.Omega_dir_map["ETrans_MEC_15_2p"];
    std::string hET15_RES_2p_FD_Dir = directories.Omega_dir_map["ETrans_RES_15_2p"];
    std::string hET15_DIS_2p_FD_Dir = directories.Omega_dir_map["ETrans_DIS_15_2p"];

    // ET around 15 Deg (pFDpCD)
    sET15_All_Int_pFDpCD_FD = OwnHistogram(new THStack("#omega around #theta_{e} = 15#circ (pFDpCD)", "Energy transfer #omega Around #theta_{e} = 15#circ (pFDpCD);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET15_All_Int_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_15_stack_pFDpCD"];

    hET15_All_Int_pFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (All Int., pFDpCD)",
                                       "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., pFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_QEL_pFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, pFDpCD, FD)",
                                   "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_MEC_pFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (MEC only, pFDpCD, FD)",
                                   "Energy transfer #omega Around #theta_{e} = 15#circ (MEC only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_RES_pFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (RES only, pFDpCD, FD)",
                                   "Energy transfer #omega Around #theta_{e} = 15#circ (RES only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_DIS_pFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (DIS only, pFDpCD, FD)",
                                   "Energy transfer #omega Around #theta_{e} = 15#circ (DIS only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET15_All_Int_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_15_pFDpCD"];
    std::string hET15_QEL_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_QEL_15_pFDpCD"];
    std::string hET15_MEC_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_MEC_15_pFDpCD"];
    std::string hET15_RES_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_RES_15_pFDpCD"];
    std::string hET15_DIS_pFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_DIS_15_pFDpCD"];

    // ET around 15 Deg (nFDpCD)
    sET15_All_Int_nFDpCD_FD = OwnHistogram(new THStack("#omega around #theta_{e} = 15#circ (nFDpCD)", "Energy transfer #omega Around #theta_{e} = 15#circ (nFDpCD);#omega = E_{beam}-E_{e} [GeV]"));
    std::string sET15_All_Int_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_15_stack_nFDpCD"];

    hET15_All_Int_nFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (All Int., nFDpCD)",
                                       "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., nFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_QEL_nFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, nFDpCD, FD)",
                                   "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_MEC_nFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (MEC only, nFDpCD, FD)",
                                   "Energy transfer #omega Around #theta_{e} = 15#circ (MEC only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_RES_nFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (RES only, nFDpCD, FD)",
                                   "Energy transfer #omega Around #theta_{e} = 15#circ (RES only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    hET15_DIS_nFDpCD_FD = OwnHistogram(new TH1D("#omega around #theta_{e} = 15#circ (DIS only, nFDpCD, FD)",
                                   "Energy transfer #omega Around #theta_{e} = 15#circ (DIS only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]", numTH1Dbins_ET_Plots, 0, beamEMax));
    std::string hET15_All_Int_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_All_Int_15_nFDpCD"];
    std::string hET15_QEL_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_QEL_15_nFDpCD"];
    std::string hET15_MEC_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_MEC_15_nFDpCD"];
    std::string hET15_RES_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_RES_15_nFDpCD"];
    std::string hET15_DIS_nFDpCD_FD_Dir = directories.Omega_dir_map["ETrans_DIS_15_nFDpCD"];

#pragma endregion
}

void FinalStateHistograms::InitBetaHistograms(const HistogramDefinitionSettings& hDefSettings) {
    auto& directories = beta_directories_;
    const int numTH1Dbins_Beta_Plots = hDefSettings.numTH1Dbins_Beta_Plots;
    const int numTH2Dbins_Beta_Plots = hDefSettings.numTH2Dbins_Beta_Plots;
    const double Beta_dist_ulim = hDefSettings.Beta_dist_ulim;
    const double Beta_dist_llim = hDefSettings.Beta_dist_llim;
    const double Beta_dist_ZOOMOUT_ulim = hDefSettings.Beta_dist_ZOOMOUT_ulim;
    const double Beta_dist_ZOOMOUT_llim = hDefSettings.Beta_dist_ZOOMOUT_llim;
    const double Beta_lim_const = hDefSettings.Beta_lim_const;
    const double Beta_lim = hDefSettings.Beta_lim;
    const double P_lim = hDefSettings.P_lim;
    const double beamE11 = hDefSettings.Momentum_ulim;
    const double beamE1425 = hDefSettings.Momentum_ulim * (1.425 / 1.1);

#pragma region /* Beta histograms */

    // Beta plots (1n)

    // Beta of neutrons from 'photons' (1n, FD)
    hBeta_n_from_ph_01_1n_FD = hPlot1D("1n", "FD", "#beta of n from '#gamma'", "Neutron #beta from 'photons'", "#beta", directories.Beta_dir_map["Neutron_beta_from_ph_01_1n"],
                                       "01_Beta_Neutron_from_photons_1n", 0.98, Beta_dist_ulim, numTH1Dbins_Beta_Plots);
    hBeta_n_from_ph_01_1n_ZOOMOUT_FD =
        hPlot1D("1n", "FD", "#beta of n from '#gamma' - ZOOMOUT", "Neutron #beta from 'photons' - ZOOMOUT", "#beta", directories.Beta_dir_map["Neutron_beta_from_ph_01_1n"],
                "01_Beta_Neutron_from_photons_1n_ZOOMOUT", Beta_dist_ZOOMOUT_llim, Beta_dist_ZOOMOUT_ulim, numTH1Dbins_Beta_Plots);

    hBeta_n_from_ph_02_1n_FD =
        hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL", "Neutron #beta from 'photons' w/o PCAL hit", "#beta", directories.Beta_dir_map["Neutron_beta_from_ph_02_1n"],
                "01_Beta_Neutron_from_photons_noPCAL_1n", Beta_dist_llim, Beta_dist_ulim, numTH1Dbins_Beta_Plots);
    hBeta_n_from_ph_02_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL - ZOOMOUT", "Neutron #beta from 'photons' w/o PCAL hit - ZOOMOUT", "#beta",
                                               directories.Beta_dir_map["Neutron_beta_from_ph_02_1n"], "01_Beta_Neutron_from_photons_noPCAL_1n_ZOOMOUT", Beta_dist_ZOOMOUT_llim,
                                               Beta_dist_ZOOMOUT_ulim, numTH1Dbins_Beta_Plots);

    hBeta_n_from_ph_03_1n_FD =
        hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & ECIN", "Neutron #beta from 'photons' w/o PCAL & w/ECIN", "#beta", directories.Beta_dir_map["Neutron_beta_from_ph_03_1n"],
                "01_Beta_Neutron_from_photons_noPCAL_yesECIN_1n", Beta_dist_llim, Beta_dist_ulim, numTH1Dbins_Beta_Plots);
    hBeta_n_from_ph_03_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & ECIN - ZOOMOUT", "Neutron #beta from 'photons' w/o PCAL & w/ECIN - ZOOMOUT", "#beta",
                                               directories.Beta_dir_map["Neutron_beta_from_ph_03_1n"], "01_Beta_Neutron_from_photons_noPCAL_yesECIN_1n_ZOOMOUT", Beta_dist_ZOOMOUT_llim,
                                               Beta_dist_ZOOMOUT_ulim, numTH1Dbins_Beta_Plots);

    hBeta_n_from_ph_04_1n_FD =
        hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & !ECIN & ECOUT", "Neutron #beta from 'photons' w/o PCAL/ECIN & w/ECOUT", "#beta",
                directories.Beta_dir_map["Neutron_beta_from_ph_04_1n"], "01_Beta_Neutron_from_photons_noPCAL_noECIN_yesECOUT_1n", Beta_dist_llim, Beta_dist_ulim, numTH1Dbins_Beta_Plots);
    hBeta_n_from_ph_04_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & !ECIN & ECOUT - ZOOMOUT", "Neutron #beta from 'photons' w/o PCAL/ECIN & w/ECOUT - ZOOMOUT",
                                               "#beta", directories.Beta_dir_map["Neutron_beta_from_ph_04_1n"], "01_Beta_Neutron_from_photons_noPCAL_noECIN_yesECOUT_1n_ZOOMOUT",
                                               Beta_dist_ZOOMOUT_llim, Beta_dist_ZOOMOUT_ulim, numTH1Dbins_Beta_Plots);

    // Beta vs. P plots

    // Beta vs. P (no #(e) cut)

    // Beta vs. P (no #(e) cut)
    hBeta_vs_P_CD = hPlot2D("all particles", "no #(e) cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_All_e"],
                            "01_Beta_vs_P_All_Particles_CD", 0.0, beamE1425, 0, Beta_lim_const, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_FD = hPlot2D("all particles", "no #(e) cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_All_e"],
                            "01_Beta_vs_P_All_Particles_FD", 0.0, beamE1425, 0, Beta_lim_const, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_Electrons_Only_FD = hPlot2D("electrons only", "no #(e) cut", "", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_All_e"],
                                           "02_Beta_vs_P_Electrons_Only_FD", 0.0, beamE1425, 0.5, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_Protons_Only_CD = hPlot2D("protons only", "no #(e) cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_All_e"],
                                         "03_Beta_vs_P_Protons_Only_CD", 0.0, beamE11, 0, 1.1, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_Protons_Only_FD = hPlot2D("protons only", "no #(e) cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_All_e"],
                                         "03_Beta_vs_P_Protons_Only_FD", 0.0, beamE11, 0, 1.1, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_Neutrons_Only_CD = hPlot2D("neutrons only", "no #(e) cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_All_e"],
                                          "04_Beta_vs_P_Neutrons_Only_CD", 0.0, beamE11, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_Neutrons_Only_FD = hPlot2D("neutrons only", "no #(e) cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_All_e"],
                                          "04_Beta_vs_P_Neutrons_Only_FD", 0.0, beamE11, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P by charge (no #(e) cut)
    hBeta_vs_P_positive_part_All_e_CD =
        hPlot2D("", "no #(e) cut", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_All_e"],
                "01_Beta_vs_P_q_p1_All_e_CD", 0.0, beamE1425, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_positive_part_All_e_FD =
        hPlot2D("", "no #(e) cut", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_All_e"],
                "01_Beta_vs_P_q_p1_All_e_FD", 0.0, beamE1425, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_All_e_CD =
        hPlot2D("", "no #(e) cut", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_All_e"],
                "02_Beta_vs_P_q_0_All_e_CD", 0.0, beamE1425, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_All_e_FD =
        hPlot2D("", "no #(e) cut", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_All_e"],
                "02_Beta_vs_P_q_0_All_e_FD", 0.0, beamE1425, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_All_e_CD =
        hPlot2D("", "no #(e) cut", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_All_e"],
                "03_Beta_vs_P_q_m1_All_e_CD", 0.0, beamE1425, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_All_e_FD =
        hPlot2D("", "no #(e) cut", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_All_e"],
                "03_Beta_vs_P_q_m1_All_e_FD", 0.0, beamE1425, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P (1e cut)

    // Beta vs. P for all particles (1e cut)
    hBeta_vs_P_1e_cut_CD = hPlot2D("all particles", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                   "01_Beta_vs_P_1e_cut_All_Particles_CD", 0.0, beamE11, 0, Beta_lim_const, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1e_cut_FD = hPlot2D("all particles", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                   "01_Beta_vs_P_1e_cut_All_Particles_FD", 0.0, beamE11, 0, Beta_lim_const, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1e_cut_Electrons_Only_FD = hPlot2D("electrons only", "1e cut", "", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                                  "02_Beta_vs_P_1e_cut_Electrons_Only_FD", 0.0, beamE1425, 0.5, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1e_cut_Protons_Only_CD = hPlot2D("protons only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                                "03_Beta_vs_P_1e_cut_Protons_Only_CD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1e_cut_Protons_Only_FD = hPlot2D("protons only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                                "03_Beta_vs_P_1e_cut_Protons_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1e_cut_Neutrons_Only_CD_CLAS12 =
        hPlot2D("neutrons only clas12", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                "04a_Beta_vs_P_1e_cut_Neutrons_Only_CD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1e_cut_Neutrons_Only_FD_CLAS12 =
        hPlot2D("neutrons only clas12", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                "04a_Beta_vs_P_1e_cut_Neutrons_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1e_cut_Neutrons_Only_FD_ByDef = hPlot2D("neutrons only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                                       "04b_Beta_vs_P_1e_cut_Neutrons_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1e_cut_Kplus_Only_CD = hPlot2D("K^{+} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                              "05_Beta_vs_P_1e_cut_Kplus_Only_CD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1e_cut_Kplus_Only_FD = hPlot2D("K^{+} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                              "05_Beta_vs_P_1e_cut_Kplus_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1e_cut_Kminus_Only_CD = hPlot2D("K^{-} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                               "06_Beta_vs_P_1e_cut_Kminus_Only_CD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1e_cut_Kminus_Only_FD = hPlot2D("K^{-} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                               "06_Beta_vs_P_1e_cut_Kminus_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1e_cut_piplus_Only_CD = hPlot2D("#pi^{+} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                               "07_Beta_vs_P_1e_cut_piplus_Only_CD", 0.0, P_lim, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1e_cut_piplus_Only_FD = hPlot2D("#pi^{+} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                               "07_Beta_vs_P_1e_cut_piplus_Only_FD", 0.0, P_lim, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1e_cut_piminus_Only_CD = hPlot2D("#pi^{-} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                                "08_Beta_vs_P_1e_cut_piminus_Only_CD", 0.0, P_lim, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1e_cut_piminus_Only_FD = hPlot2D("#pi^{-} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1e_cut"],
                                                "08_Beta_vs_P_1e_cut_piminus_Only_FD", 0.0, P_lim, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P by charge (1e cut)
    hBeta_vs_P_positive_part_1e_cut_CD =
        hPlot2D("all particles", "1e cut", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_1e_cut"], "01_Beta_vs_P_q_p1_1e_cut_CD", 0.0, beamE11, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_positive_part_1e_cut_FD =
        hPlot2D("all particles", "1e cut", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_1e_cut"], "01_Beta_vs_P_q_p1_1e_cut_FD", 0.0, beamE11, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_1e_cut_CD =
        hPlot2D("all particles", "1e cut", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_1e_cut"], "02_Beta_vs_P_q_0_1e_cut_CD", 0.0, beamE11, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_1e_cut_FD =
        hPlot2D("all particles", "1e cut", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_1e_cut"], "02_Beta_vs_P_q_0_1e_cut_FD", 0.0, beamE11, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_1e_cut_CD =
        hPlot2D("all particles", "1e cut", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_1e_cut"], "03_Beta_vs_P_q_m1_1e_cut_CD", 0.0, beamE11, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_1e_cut_FD =
        hPlot2D("all particles", "1e cut", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_1e_cut"], "03_Beta_vs_P_q_m1_1e_cut_FD", 0.0, beamE11, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P (1p)

    // Beta vs. P for all particles (1p)
    hBeta_vs_P_1p_CD = hPlot2D("all particles", "1p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1p"], "01_Beta_vs_P_1p_All_Particles_CD",
                               0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1p_FD = hPlot2D("all particles", "1p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1p"], "01_Beta_vs_P_1p_All_Particles_FD",
                               0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1p_Electrons_Only_FD = hPlot2D("electrons only", "1p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1p"],
                                              "02_Beta_vs_P_1p_Electrons_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1p_Protons_Only_CD = hPlot2D("protons only", "1p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1p"],
                                            "03_Beta_vs_P_1p_Protons_Only_CD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1p_Protons_Only_FD = hPlot2D("protons only", "1p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1p"],
                                            "03_Beta_vs_P_1p_Protons_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P by charge (1p)
    hBeta_vs_P_positive_part_1p_CD =
        hPlot2D("all particles", "1p", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_1p"],
                "01_Beta_vs_P_q_p1_1p_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_positive_part_1p_FD =
        hPlot2D("all particles", "1p", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_1p"],
                "01_Beta_vs_P_q_p1_1p_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_1p_CD =
        hPlot2D("all particles", "1p", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_1p"],
                "02_Beta_vs_P_q_0_1p_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_1p_FD =
        hPlot2D("all particles", "1p", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_1p"],
                "02_Beta_vs_P_q_0_1p_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_1p_CD =
        hPlot2D("all particles", "1p", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_1p"],
                "03_Beta_vs_P_q_m1_1p_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_1p_FD =
        hPlot2D("all particles", "1p", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_1p"],
                "03_Beta_vs_P_q_m1_1p_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P (1n)

    // Beta vs. P for all particles (1n)
    hBeta_vs_P_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"], "00_Beta_vs_P_All_Particles_CD_1n",
                               0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"], "00_Beta_vs_P_All_Particles_FD_1n",
                               0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1n_Electrons_Only_FD = hPlot2D("electrons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                              "01_Beta_vs_P_Electrons_Only_FD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1n_Neutrons_Only_CD = hPlot2D("id. neutrons only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                             "02_Beta_vs_P_id_Neutrons_Only_CD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1n_Neutrons_Only_FD = hPlot2D("id. neutrons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                             "02_Beta_vs_P_id_Neutrons_Only_FD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1n_Neutrons_Only_ZOOMOUT_FD = hPlot2D("id. neutrons only", "1n", "FD", "#beta vs. P - ZOOMOUT", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                                     "02_Beta_vs_P_id_Neutrons_Only_ZOOMOUT_FD_1n", 0.0, P_lim, 0, 2, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1n_Neutrons_Only_from_photons_FD =
        hPlot2D("id. neutrons", "1n", "FD", "#beta vs. P", " Cond. to id. neutron's #beta vs. P from 'photons'", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                "02a_Beta_vs_P_id_Neutrons_Only_from_potons_FD_1n", 0.0, P_lim, 0.9, 1.15, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1n_Photons_Only_CD = hPlot2D("id. photons only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                            "03_Beta_vs_P_id_Photons_Only_CD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1n_Photons_Only_FD = hPlot2D("id. photons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                            "03_Beta_vs_P_id_Photons_Only_FD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1n_Protons_Only_CD = hPlot2D("id. protons only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                            "04_Beta_vs_P_id_Protons_Only_CD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1n_Protons_Only_FD = hPlot2D("id. protons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                            "04_Beta_vs_P_id_Protons_Only_FD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1n_Piplus_Only_CD = hPlot2D("id. #pi^{+} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                           "05_Beta_vs_P_id_piplus_Only_CD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1n_Piplus_Only_FD = hPlot2D("id. #pi^{+} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                           "05_Beta_vs_P_id_piplus_Only_FD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1n_Piminus_Only_CD = hPlot2D("id. #pi^{-} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                            "06_Beta_vs_P_id_piminus_Only_CD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1n_Piminus_Only_FD = hPlot2D("id. #pi^{-} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                            "06_Beta_vs_P_id_piminus_Only_FD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1n_Kplus_Only_CD = hPlot2D("id. K^{+} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                          "05_Beta_vs_P_id_Kplus_Only_CD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1n_Kplus_Only_FD = hPlot2D("id. K^{+} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                          "05_Beta_vs_P_id_Kplus_Only_FD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_1n_Kminus_Only_CD = hPlot2D("id. K^{-} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                           "06_Beta_vs_P_id_Kminus_Only_CD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_1n_Kminus_Only_FD = hPlot2D("id. K^{-} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_1n"],
                                           "06_Beta_vs_P_id_Kminus_Only_FD_1n", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P by charge (1n)
    hBeta_vs_P_pos_part_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta",
                                        directories.Beta_dir_map["Beta_VS_P_by_charge_1n"], "01_Beta_vs_P_q_p1_1n_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_pos_part_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta",
                                        directories.Beta_dir_map["Beta_VS_P_by_charge_1n"], "01_Beta_vs_P_q_p1_1n_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neut_part_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta",
                                         directories.Beta_dir_map["Beta_VS_P_by_charge_1n"], "02_Beta_vs_P_q_0_1n_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neut_part_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta",
                                         directories.Beta_dir_map["Beta_VS_P_by_charge_1n"], "02_Beta_vs_P_q_0_1n_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neg_part_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta",
                                        directories.Beta_dir_map["Beta_VS_P_by_charge_1n"], "03_Beta_vs_P_q_m1_1n_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neg_part_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta",
                                        directories.Beta_dir_map["Beta_VS_P_by_charge_1n"], "03_Beta_vs_P_q_m1_1n_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P (2p)

    // Beta vs. P for all particles (2p)
    hBeta_vs_P_2p_CD = hPlot2D("all particles", "2p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_2p"], "01_Beta_vs_P_2p_All_Particles_CD",
                               0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_2p_FD = hPlot2D("all particles", "2p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_2p"], "01_Beta_vs_P_2p_All_Particles_FD",
                               0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_2p_Electrons_Only_FD = hPlot2D("electrons only", "2p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_2p"],
                                              "02_Beta_vs_P_2p_Electrons_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_2p_Protons_Only_CD = hPlot2D("protons only", "2p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_2p"],
                                            "03_Beta_vs_P_2p_Protons_Only_CD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_2p_Protons_Only_FD = hPlot2D("protons only", "2p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_2p"],
                                            "03_Beta_vs_P_2p_Protons_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P by charge (2p)
    hBeta_vs_P_positive_part_2p_CD =
        hPlot2D("all particles", "2p", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_2p"],
                "01_Beta_vs_P_q_p1_2p_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_positive_part_2p_FD =
        hPlot2D("all particles", "2p", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_2p"],
                "01_Beta_vs_P_q_p1_2p_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_2p_CD =
        hPlot2D("all particles", "2p", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_2p"],
                "02_Beta_vs_P_q_0_2p_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_2p_FD =
        hPlot2D("all particles", "2p", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_2p"],
                "02_Beta_vs_P_q_0_2p_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_2p_CD =
        hPlot2D("all particles", "2p", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_2p"],
                "03_Beta_vs_P_q_m1_2p_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_2p_FD =
        hPlot2D("all particles", "2p", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_by_charge_2p"],
                "03_Beta_vs_P_q_m1_2p_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P (pFDpCD)

    // Beta vs. P for all particles (pFDpCD)
    hBeta_vs_P_pFDpCD_CD = hPlot2D("all particles", "pFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_pFDpCD"],
                                   "01_Beta_vs_P_pFDpCD_All_Particles_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_pFDpCD_FD = hPlot2D("all particles", "pFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_pFDpCD"],
                                   "01_Beta_vs_P_pFDpCD_All_Particles_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_pFDpCD_Electrons_Only_FD = hPlot2D("electrons only", "pFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_pFDpCD"],
                                                  "02_Beta_vs_P_pFDpCD_Electrons_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_pFDpCD_Protons_Only_CD = hPlot2D("protons only", "pFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_pFDpCD"],
                                                "03_Beta_vs_P_pFDpCD_Protons_Only_CD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_pFDpCD_Protons_Only_FD = hPlot2D("protons only", "pFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_pFDpCD"],
                                                "03_Beta_vs_P_pFDpCD_Protons_Only_FD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_pFD_pFDpCD_Protons_Only_FD =
        hPlot2D("", "pFDpCD", "FD", "#beta vs. FD proton momentum", "#beta vs. FD proton momentum", "P_{pFD} [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_pFDpCD"],
                "04a_Beta_vs_P_pFD_pFDpCD_Protons_Only_CD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_pCD_pFDpCD_Protons_Only_CD =
        hPlot2D("", "pFDpCD", "CD", "#beta vs. CD proton momentum", "#beta vs. CD proton momentum", "P_{pCD} [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_pFDpCD"],
                "04b_Beta_vs_P_pCD_pFDpCD_Protons_Only_CD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P by charge (pFDpCD)
    hBeta_vs_P_positive_part_pFDpCD_CD =
        hPlot2D("all particles", "pFDpCD", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_pFDpCD"], "01_Beta_vs_P_q_p1_pFDpCD_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_positive_part_pFDpCD_FD =
        hPlot2D("all particles", "pFDpCD", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_pFDpCD"], "01_Beta_vs_P_q_p1_pFDpCD_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_pFDpCD_CD =
        hPlot2D("all particles", "pFDpCD", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_pFDpCD"], "02_Beta_vs_P_q_0_pFDpCD_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neutral_part_pFDpCD_FD =
        hPlot2D("all particles", "pFDpCD", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_pFDpCD"], "02_Beta_vs_P_q_0_pFDpCD_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_pFDpCD_CD =
        hPlot2D("all particles", "pFDpCD", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_pFDpCD"], "03_Beta_vs_P_q_m1_pFDpCD_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_negative_part_pFDpCD_FD =
        hPlot2D("all particles", "pFDpCD", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_pFDpCD"], "03_Beta_vs_P_q_m1_pFDpCD_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P (nFDpCD)

    // Beta vs. P for all particles (nFDpCD)
    hBeta_vs_P_nFDpCD_CD = hPlot2D("all particles", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                   "00_Beta_vs_P_All_Particles_CD_nFDpCD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_nFDpCD_FD = hPlot2D("all particles", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                   "00_Beta_vs_P_All_Particles_FD_nFDpCD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_nFDpCD_Electrons_Only_FD = hPlot2D("electrons only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                                  "01_Beta_vs_P_Electrons_Only_FD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_nFDpCD_Neutrons_Only_CD = hPlot2D("id. neutrons only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                                 "02_Beta_vs_P_id_Neutrons_Only_CD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_nFDpCD_Neutrons_Only_FD = hPlot2D("id. neutrons only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                                 "02_Beta_vs_P_id_Neutrons_Only_FD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_nFDpCD_Neutrons_Only_ZOOMOUT_FD =
        hPlot2D("id. neutrons only", "nFDpCD", "FD", "#beta vs. P - ZOOMOUT", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                "02_Beta_vs_P_id_Neutrons_Only_ZOOMOUT_FD_nFDpCD", 0.0, P_lim, 0, 2, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_nFDpCD_Neutrons_Only_from_photons_FD =
        hPlot2D("id. neutrons", "nFDpCD", "FD", "#beta vs. P", " Cond. to id. neutron's #beta vs. P from 'photons'", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                "02a_Beta_vs_P_id_Neutrons_Only_from_potons_FD_nFDpCD", 0.0, P_lim, 0.9, 1.15, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_nFDpCD_Photons_Only_CD = hPlot2D("id. photons only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                                "03_Beta_vs_P_id_Photons_Only_CD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_nFDpCD_Photons_Only_FD = hPlot2D("id. photons only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                                "03_Beta_vs_P_id_Photons_Only_FD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_nFDpCD_Protons_Only_CD = hPlot2D("id. protons only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                                "04_Beta_vs_P_id_Protons_Only_CD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_nFDpCD_Protons_Only_FD = hPlot2D("id. protons only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                                "04_Beta_vs_P_id_Protons_Only_FD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_nFDpCD_Piplus_Only_CD = hPlot2D("id. #pi^{+} only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                               "05_Beta_vs_P_id_piplus_Only_CD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_nFDpCD_Piplus_Only_FD = hPlot2D("id. #pi^{+} only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                               "05_Beta_vs_P_id_piplus_Only_FD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_nFDpCD_Piminus_Only_CD = hPlot2D("id. #pi^{-} only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                                "06_Beta_vs_P_id_piminus_Only_CD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_nFDpCD_Piminus_Only_FD = hPlot2D("id. #pi^{-} only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                                "06_Beta_vs_P_id_piminus_Only_FD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_nFDpCD_Kplus_Only_CD = hPlot2D("id. K^{+} only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                              "05_Beta_vs_P_id_Kplus_Only_CD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_nFDpCD_Kplus_Only_FD = hPlot2D("id. K^{+} only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                              "05_Beta_vs_P_id_Kplus_Only_FD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hBeta_vs_P_nFDpCD_Kminus_Only_CD = hPlot2D("id. K^{-} only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                               "06_Beta_vs_P_id_Kminus_Only_CD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_nFDpCD_Kminus_Only_FD = hPlot2D("id. K^{-} only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta", directories.Beta_dir_map["Beta_VS_P_nFDpCD"],
                                               "06_Beta_vs_P_id_Kminus_Only_FD_nFDpCD", 0.0, P_lim, 0, Beta_lim, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    // Beta vs. P by charge (nFDpCD)
    hBeta_vs_P_pos_part_nFDpCD_CD =
        hPlot2D("all particles", "nFDpCD", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_nFDpCD"], "01_Beta_vs_P_q_p1_nFDpCD_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_pos_part_nFDpCD_FD =
        hPlot2D("all particles", "nFDpCD", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_nFDpCD"], "01_Beta_vs_P_q_p1_nFDpCD_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neut_part_nFDpCD_CD =
        hPlot2D("all particles", "nFDpCD", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_nFDpCD"], "02_Beta_vs_P_q_0_nFDpCD_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neut_part_nFDpCD_FD =
        hPlot2D("all particles", "nFDpCD", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_nFDpCD"], "02_Beta_vs_P_q_0_nFDpCD_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neg_part_nFDpCD_CD =
        hPlot2D("all particles", "nFDpCD", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_nFDpCD"], "03_Beta_vs_P_q_m1_nFDpCD_CD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hBeta_vs_P_neg_part_nFDpCD_FD =
        hPlot2D("all particles", "nFDpCD", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1", "P [GeV/c]", "#beta",
                directories.Beta_dir_map["Beta_VS_P_by_charge_nFDpCD"], "03_Beta_vs_P_q_m1_nFDpCD_FD", 0.0, beamE11, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    for (hPlot2D* betaPlot : {
             &hBeta_vs_P_CD,
             &hBeta_vs_P_FD,
             &hBeta_vs_P_Electrons_Only_FD,
             &hBeta_vs_P_Protons_Only_CD,
             &hBeta_vs_P_Protons_Only_FD,
             &hBeta_vs_P_Neutrons_Only_CD,
             &hBeta_vs_P_Neutrons_Only_FD,
             &hBeta_vs_P_positive_part_All_e_CD,
             &hBeta_vs_P_positive_part_All_e_FD,
             &hBeta_vs_P_neutral_part_All_e_CD,
             &hBeta_vs_P_neutral_part_All_e_FD,
             &hBeta_vs_P_negative_part_All_e_CD,
             &hBeta_vs_P_negative_part_All_e_FD,
             &hBeta_vs_P_1e_cut_CD,
             &hBeta_vs_P_1e_cut_FD,
             &hBeta_vs_P_1e_cut_Electrons_Only_FD,
             &hBeta_vs_P_1e_cut_Protons_Only_CD,
             &hBeta_vs_P_1e_cut_Protons_Only_FD,
             &hBeta_vs_P_1e_cut_Neutrons_Only_CD_CLAS12,
             &hBeta_vs_P_1e_cut_Neutrons_Only_FD_CLAS12,
             &hBeta_vs_P_1e_cut_Neutrons_Only_FD_ByDef,
             &hBeta_vs_P_1e_cut_Kplus_Only_CD,
             &hBeta_vs_P_1e_cut_Kplus_Only_FD,
             &hBeta_vs_P_1e_cut_Kminus_Only_CD,
             &hBeta_vs_P_1e_cut_Kminus_Only_FD,
             &hBeta_vs_P_1e_cut_piplus_Only_CD,
             &hBeta_vs_P_1e_cut_piplus_Only_FD,
             &hBeta_vs_P_1e_cut_piminus_Only_CD,
             &hBeta_vs_P_1e_cut_piminus_Only_FD,
             &hBeta_vs_P_positive_part_1e_cut_CD,
             &hBeta_vs_P_positive_part_1e_cut_FD,
             &hBeta_vs_P_neutral_part_1e_cut_CD,
             &hBeta_vs_P_neutral_part_1e_cut_FD,
             &hBeta_vs_P_negative_part_1e_cut_CD,
             &hBeta_vs_P_negative_part_1e_cut_FD,
             &hBeta_vs_P_1p_CD,
             &hBeta_vs_P_1p_FD,
             &hBeta_vs_P_1p_Electrons_Only_FD,
             &hBeta_vs_P_1p_Protons_Only_CD,
             &hBeta_vs_P_1p_Protons_Only_FD,
             &hBeta_vs_P_positive_part_1p_CD,
             &hBeta_vs_P_positive_part_1p_FD,
             &hBeta_vs_P_neutral_part_1p_CD,
             &hBeta_vs_P_neutral_part_1p_FD,
             &hBeta_vs_P_negative_part_1p_CD,
             &hBeta_vs_P_negative_part_1p_FD,
             &hBeta_vs_P_1n_CD,
             &hBeta_vs_P_1n_FD,
             &hBeta_vs_P_1n_Electrons_Only_FD,
             &hBeta_vs_P_1n_Neutrons_Only_CD,
             &hBeta_vs_P_1n_Neutrons_Only_FD,
             &hBeta_vs_P_1n_Neutrons_Only_ZOOMOUT_FD,
             &hBeta_vs_P_1n_Neutrons_Only_from_photons_FD,
             &hBeta_vs_P_1n_Photons_Only_CD,
             &hBeta_vs_P_1n_Photons_Only_FD,
             &hBeta_vs_P_1n_Protons_Only_CD,
             &hBeta_vs_P_1n_Protons_Only_FD,
             &hBeta_vs_P_1n_Piplus_Only_CD,
             &hBeta_vs_P_1n_Piplus_Only_FD,
             &hBeta_vs_P_1n_Piminus_Only_CD,
             &hBeta_vs_P_1n_Piminus_Only_FD,
             &hBeta_vs_P_1n_Kplus_Only_CD,
             &hBeta_vs_P_1n_Kplus_Only_FD,
             &hBeta_vs_P_1n_Kminus_Only_CD,
             &hBeta_vs_P_1n_Kminus_Only_FD,
             &hBeta_vs_P_pos_part_1n_CD,
             &hBeta_vs_P_pos_part_1n_FD,
             &hBeta_vs_P_neut_part_1n_CD,
             &hBeta_vs_P_neut_part_1n_FD,
             &hBeta_vs_P_neg_part_1n_CD,
             &hBeta_vs_P_neg_part_1n_FD,
             &hBeta_vs_P_2p_CD,
             &hBeta_vs_P_2p_FD,
             &hBeta_vs_P_2p_Electrons_Only_FD,
             &hBeta_vs_P_2p_Protons_Only_CD,
             &hBeta_vs_P_2p_Protons_Only_FD,
             &hBeta_vs_P_positive_part_2p_CD,
             &hBeta_vs_P_positive_part_2p_FD,
             &hBeta_vs_P_neutral_part_2p_CD,
             &hBeta_vs_P_neutral_part_2p_FD,
             &hBeta_vs_P_negative_part_2p_CD,
             &hBeta_vs_P_negative_part_2p_FD,
             &hBeta_vs_P_pFDpCD_CD,
             &hBeta_vs_P_pFDpCD_FD,
             &hBeta_vs_P_pFDpCD_Electrons_Only_FD,
             &hBeta_vs_P_pFDpCD_Protons_Only_CD,
             &hBeta_vs_P_pFDpCD_Protons_Only_FD,
             &hBeta_vs_P_pFD_pFDpCD_Protons_Only_FD,
             &hBeta_vs_P_pCD_pFDpCD_Protons_Only_CD,
             &hBeta_vs_P_positive_part_pFDpCD_CD,
             &hBeta_vs_P_positive_part_pFDpCD_FD,
             &hBeta_vs_P_neutral_part_pFDpCD_CD,
             &hBeta_vs_P_neutral_part_pFDpCD_FD,
             &hBeta_vs_P_negative_part_pFDpCD_CD,
             &hBeta_vs_P_negative_part_pFDpCD_FD,
             &hBeta_vs_P_nFDpCD_CD,
             &hBeta_vs_P_nFDpCD_FD,
             &hBeta_vs_P_nFDpCD_Electrons_Only_FD,
             &hBeta_vs_P_nFDpCD_Neutrons_Only_CD,
             &hBeta_vs_P_nFDpCD_Neutrons_Only_FD,
             &hBeta_vs_P_nFDpCD_Neutrons_Only_ZOOMOUT_FD,
             &hBeta_vs_P_nFDpCD_Neutrons_Only_from_photons_FD,
             &hBeta_vs_P_nFDpCD_Photons_Only_CD,
             &hBeta_vs_P_nFDpCD_Photons_Only_FD,
             &hBeta_vs_P_nFDpCD_Protons_Only_CD,
             &hBeta_vs_P_nFDpCD_Protons_Only_FD,
             &hBeta_vs_P_nFDpCD_Piplus_Only_CD,
             &hBeta_vs_P_nFDpCD_Piplus_Only_FD,
             &hBeta_vs_P_nFDpCD_Piminus_Only_CD,
             &hBeta_vs_P_nFDpCD_Piminus_Only_FD,
             &hBeta_vs_P_nFDpCD_Kplus_Only_CD,
             &hBeta_vs_P_nFDpCD_Kplus_Only_FD,
             &hBeta_vs_P_nFDpCD_Kminus_Only_CD,
             &hBeta_vs_P_nFDpCD_Kminus_Only_FD,
             &hBeta_vs_P_pos_part_nFDpCD_CD,
             &hBeta_vs_P_pos_part_nFDpCD_FD,
             &hBeta_vs_P_neut_part_nFDpCD_CD,
             &hBeta_vs_P_neut_part_nFDpCD_FD,
             &hBeta_vs_P_neg_part_nFDpCD_CD,
             &hBeta_vs_P_neg_part_nFDpCD_FD,
         }) {
        AttachBetaCurvesToHistogram(*betaPlot);
    }
#pragma endregion
}

void FinalStateHistograms::InitAcceptanceMapHistograms(const HistogramDefinitionSettings& hDefSettings) {
    auto& directories = acceptance_map_directories_;
    const int numTH1Dbins_ReacMon_plots = hDefSettings.numTH1Dbins_ReacMon_plots;
    const int numTH2Dbins_Electron_AMaps_Plots = hDefSettings.numTH2Dbins_Electron_AMaps_Plots;
    const int numTH2Dbins_Nucleon_AMaps_Plots = hDefSettings.numTH2Dbins_Nucleon_AMaps_Plots;
    const double Momentum_llim = hDefSettings.Momentum_llim;
    const double Momentum_ulim = hDefSettings.Momentum_ulim;
    const double Theta_llim_FD = hDefSettings.Theta_llim_FD;
    const double Theta_ulim_FD = hDefSettings.Theta_ulim_FD;
    const double Phi_llim = hDefSettings.Phi_llim;
    const double Phi_ulim = hDefSettings.Phi_ulim;
    const double ThetaFD_AMaps_llim = hDefSettings.ThetaFD_AMaps_llim;
    const double ThetaFD_AMaps_ulim = hDefSettings.ThetaFD_AMaps_ulim;

#pragma region /* Acceptance maps histograms */

    /* Acceptance maps are handled completely by the AMaps class */
    hElectronAMapBC = hPlot2D("", "", "Electron_AMap_BC", "Electron AMap BC", "#phi_{e} [#circ]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_BC_1e_cut"], "01_Electron_AMap_BC",
                              Phi_llim, Phi_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Electron_AMaps_Plots, numTH2Dbins_Electron_AMaps_Plots);
    hProtonAMapBC = hPlot2D("", "", "Proton_AMap_BC", "Proton AMap BC", "#phi_{p} [#circ]", "#theta_{p} [#circ]", directories.AMaps_dir_map["AMaps_BC_1e_cut"], "02_Proton_AMap_BC", Phi_llim,
                            Phi_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hNeutronAMapBC = hPlot2D("", "", "Neutron_AMap_BC", "Neutron AMap BC", "#phi_{n} [#circ]", "#theta_{n} [#circ]", directories.AMaps_dir_map["AMaps_BC_1e_cut"], "03_Neutron_AMap_BC",
                             Phi_llim, Phi_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hNucleonAMapBC = hPlot2D("", "", "Nucleon_AMap_BC", "Nucleon AMap BC", "#phi_{nuc} [#circ]", "#theta_{nuc} [#circ]", directories.AMaps_dir_map["AMaps_BC_1e_cut"], "04_Nucleon_AMap_BC",
                             Phi_llim, Phi_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hReco_P_e_AMaps = hPlot1D("1e cut", "FD", "Reco P_{e} used in AMaps", "Reco P_{e} used in AMaps", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                              "01a_Reco_P_e_used_in_AMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hTL_P_e_AMaps = hPlot1D("1e cut", "FD", "TL P_{e} used in AMaps", "TL P_{e} used in AMaps", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"], "01b_TL_P_e_used_in_AMaps",
                            Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Theta_e_AMaps = hPlot1D("1e cut", "FD", "Reco #theta_{e} used in AMaps", "Reco #theta_{e} used in AMaps", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                  "01c_Reco_Theta_e_used_in_AMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hTL_Theta_e_AMaps = hPlot1D("1e cut", "FD", "TL #theta_{e} used in AMaps", "TL #theta_{e} used in AMaps", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                "01d_TL_Theta_e_used_in_AMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Phi_e_AMaps = hPlot1D("1e cut", "FD", "Reco #phi_{e} used in AMaps", "Reco #phi_{e} used in AMaps", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                "01e_Reco_Phi_e_used_in_AMaps", Phi_llim, Phi_ulim, numTH1Dbins_ReacMon_plots);
    hTL_Phi_e_AMaps = hPlot1D("1e cut", "FD", "TL #phi_{e} used in AMaps", "TL #phi_{e} used in AMaps", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                              "01f_TL_Phi_e_used_in_AMaps", Phi_llim, Phi_ulim, numTH1Dbins_ReacMon_plots);
    hReco_P_e_vs_Reco_Theta_e_AMap =
        hPlot2D("", "", "Reco_P_e_vs_Reco_Theta_e_AMap", "Reco P_{e} vs. Reco #theta_{e} used in AMaps", "P_{e} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "01aa_Reco_P_e_vs_Reco_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_e_vs_Reco_Phi_e_AMap =
        hPlot2D("", "", "Reco_P_e_vs_Reco_Phi_e_AMap", "Reco P_{e} vs. Reco #phi_{e} used in AMaps", "P_{e} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "01ab_Reco_P_e_vs_Reco_Phi_e_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_e_vs_TL_Theta_e_AMap =
        hPlot2D("", "", "Reco_P_e_vs_TL_Theta_e_AMap", "Reco P_{e} vs. TL #theta_{e} used in AMaps", "P_{e} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "01ba_Reco_P_e_vs_TL_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_e_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "Reco_P_e_vs_TL_Phi_e_AMap", "Reco P_{e} vs. TL #phi_{e} used in AMaps", "P_{e} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "01bb_Reco_P_e_vs_TL_Phi_e_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_e_vs_TL_Theta_e_AMap =
        hPlot2D("", "", "TL_P_e_vs_TL_Theta_e_AMap", "TL P_{e} vs. TL #theta_{e} used in AMaps", "P_{e} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "01ba_TL_P_e_vs_TL_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_e_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "TL_P_e_vs_TL_Phi_e_AMap", "TL P_{e} vs. TL #phi_{e} used in AMaps", "P_{e} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "01ab_TL_P_e_vs_TL_Phi_e_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hReco_P_pFD_AMaps = hPlot1D("1e cut", "FD", "Reco P_{pFD} used in AMaps", "Reco P_{pFD} used in AMaps", "P_{pFD} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                "02a_Reco_P_pFD_used_in_AMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hTL_P_pFD_AMaps = hPlot1D("1e cut", "FD", "TL P_{pFD} used in AMaps", "TL P_{pFD} used in AMaps", "P_{pFD} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                              "02b_TL_P_pFD_used_in_AMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Theta_pFD_AMaps = hPlot1D("1e cut", "FD", "Reco #theta_{pFD} used in AMaps", "Reco #theta_{pFD} used in AMaps", "#theta_{pFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                    "02c_Reco_Theta_pFD_used_in_AMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hTL_Theta_pFD_AMaps = hPlot1D("1e cut", "FD", "TL #theta_{pFD} used in AMaps", "TL #theta_{pFD} used in AMaps", "#theta_{pFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                  "02d_TL_Theta_pFD_used_in_AMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Phi_pFD_AMaps = hPlot1D("1e cut", "FD", "Reco #phi_{pFD} used in AMaps", "Reco #phi_{pFD} used in AMaps", "#phi_{pFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                  "01e_Reco_Phi_pFD_used_in_AMaps", -180, 180, numTH1Dbins_ReacMon_plots);
    hTL_Phi_pFD_AMaps = hPlot1D("1e cut", "FD", "TL #phi_{pFD} used in AMaps", "TL #phi_{pFD} used in AMaps", "#phi_{pFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                "01f_TL_Phi_pFD_used_in_AMaps", -180, 180, numTH1Dbins_ReacMon_plots);
    hReco_P_pFD_vs_Reco_Theta_pFD_AMap = hPlot2D("", "", "Reco_P_pFD_vs_Reco_Theta_pFD_AMap", "Reco P_{pFD} vs. Reco #theta_{pFD} used in AMaps", "P_{pFD} [GeV/c]", "#theta_{pFD} [#circ]",
                                                 directories.AMaps_dir_map["AMaps_1e_cut"], "02aa_Reco_P_pFD_vs_Reco_Theta_pFD_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_Reco_Phi_pFD_AMap = hPlot2D("", "", "Reco_P_pFD_vs_Reco_Phi_pFD_AMap", "Reco P_{pFD} vs. Reco #phi_{pFD} used in AMaps", "P_{pFD} [GeV/c]", "#phi_{pFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ab_Reco_P_pFD_vs_Reco_Phi_pFD_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_Theta_pFD_AMap = hPlot2D("", "", "Reco_P_pFD_vs_TL_Theta_pFD_AMap", "Reco P_{pFD} vs. TL #theta_{pFD} used in AMaps", "P_{pFD} [GeV/c]", "#theta_{pFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ac_Reco_P_pFD_vs_TL_Theta_pFD_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_Phi_pFD_AMap =
        hPlot2D("", "", "Reco_P_pFD_vs_TL_Phi_pFD_AMap", "Reco P_{pFD} vs. TL #phi_{pFD} used in AMaps", "P_{pFD} [GeV/c]", "#phi_{pFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ad_Reco_P_pFD_vs_TL_Phi_pFD_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_Reco_P_e_AMap =
        hPlot2D("", "", "Reco_P_pFD_vs_Reco_P_e_AMap", "Reco P_{pFD} vs. Reco P_{e} used in AMaps", "P_{pFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ae_Reco_P_pFD_vs_Reco_P_e_AMap", Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "Reco_P_pFD_vs_TL_P_e_AMap", "Reco P_{pFD} vs. TL P_{e} used in AMaps", "P_{pFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02af_Reco_P_pFD_vs_TL_P_e_AMap", Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "Reco_P_pFD_vs_Reco_Theta_e_AMap", "Reco P_{pFD} vs. Reco #theta_{e} used in AMaps", "P_{pFD} [GeV/c]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ag_Reco_P_pFD_vs_Reco_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_Reco_Phi_e_AMap =
        hPlot2D("", "", "Reco_P_pFD_vs_Reco_Phi_e_AMap", "Reco P_{pFD} vs. Reco #phi_{e} used in AMaps", "P_{pFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ah_Reco_P_pFD_vs_Reco_Phi_e_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_Theta_e_AMap =
        hPlot2D("", "", "Reco_P_pFD_vs_TL_Theta_e_AMap", "Reco P_{pFD} vs. TL #theta_{e} used in AMaps", "P_{pFD} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ai_Reco_P_pFD_vs_TL_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "Reco_P_pFD_vs_TL_Phi_e_AMap", "Reco P_{pFD} vs. TL #phi_{e} used in AMaps", "P_{pFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02aj_Reco_P_pFD_vs_TL_Phi_e_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_Reco_Theta_pFD_AMap = hPlot2D("", "", "TL_P_pFD_vs_Reco_Theta_pFD_AMap", "TL P_{pFD} vs. Reco #theta_{pFD} used in AMaps", "P_{pFD} [GeV/c]", "#theta_{pFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ba_TL_P_pFD_vs_Reco_Theta_pFD_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_Theta_pFD_AMap =
        hPlot2D("", "", "TL_P_pFD_vs_TL_Theta_pFD_AMap", "TL P_{pFD} vs. TL #theta_{pFD} used in AMaps", "P_{pFD} [GeV/c]", "#theta_{pFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bb_TL_P_pFD_vs_TL_Theta_pFD_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_Phi_pFD_AMap =
        hPlot2D("", "", "TL_P_pFD_vs_TL_Phi_pFD_AMap", "TL P_{pFD} vs. TL #phi_{pFD} used in AMaps", "P_{pFD} [GeV/c]", "#phi_{pFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bc_TL_P_pFD_vs_TL_Phi_pFD_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "TL_P_pFD_vs_TL_P_e_AMap", "TL P_{pFD} vs. Reco P_{e} used in AMaps", "P_{pFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bd_TL_P_pFD_vs_TL_P_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_Reco_Theta_e_AMap =
        hPlot2D("", "", "TL_P_pFD_vs_Reco_Theta_e_AMap", "TL P_{pFD} vs. Reco #theta_{e} used in AMaps", "P_{pFD} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02be_TL_P_pFD_vs_Reco_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_Theta_e_AMap =
        hPlot2D("", "", "TL_P_pFD_vs_TL_Theta_e_AMap", "TL P_{pFD} vs. TL #theta_{e} used in AMaps", "P_{pFD} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bf_TL_P_pFD_vs_TL_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "TL_P_pFD_vs_TL_Phi_e_AMap", "TL P_{pFD} vs. TL #phi_{e} used in AMaps", "P_{pFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bg_TL_P_pFD_vs_TL_Phi_e_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_Reco_Phi_pFD_AMap = hPlot2D("", "", "Reco_Theta_pFD_vs_Reco_Phi_pFD_AMap", "Reco #theta_{pFD} vs. Reco #phi_{pFD} used in AMaps", "#theta_{pFD} [#circ]",
                                                   "#phi_{pFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02ca_Reco_Theta_pFD_vs_Reco_Phi_pFD_AMap", ThetaFD_AMaps_llim,
                                                   ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_TL_Phi_pFD_AMap = hPlot2D("", "", "Reco_Theta_pFD_vs_TL_Phi_pFD_AMap", "Reco #theta_{pFD} vs. TL #phi_{pFD} used in AMaps", "#theta_{pFD} [#circ]",
                                                 "#phi_{pFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02cb_Reco_Theta_pFD_vs_TL_Phi_pFD_AMap", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_Reco_P_e_AMap = hPlot2D("", "", "Reco_Theta_pFD_vs_Reco_P_e_AMap", "Reco #theta_{pFD} vs. Reco P_{e} used in AMaps", "#theta_{pFD} [#circ]", "P_{e} [GeV/c]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02cc_Reco_Theta_pFD_vs_Reco_P_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Momentum_llim,
                                               Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "Reco_Theta_pFD_vs_TL_P_e_AMap", "Reco #theta_{pFD} vs. TL P_{e} used in AMaps", "#theta_{pFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02cd_Reco_Theta_pFD_vs_TL_P_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "Reco_Theta_pFD_vs_Reco_Theta_e_AMap", "Reco #theta_{pFD} vs. Reco #theta_{e} used in AMaps", "#theta_{pFD} [#circ]",
                                                   "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02ce_Reco_Theta_pFD_vs_Reco_Theta_e_AMap", ThetaFD_AMaps_llim,
                                                   ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_Reco_Phi_e_AMap = hPlot2D("", "", "Reco_Theta_pFD_vs_Reco_Phi_e_AMap", "Reco #theta_{pFD} vs. Reco #phi_{e} used in AMaps", "#theta_{pFD} [#circ]", "#phi_{e} [#circ]",
                                                 directories.AMaps_dir_map["AMaps_1e_cut"], "02cf_Reco_Theta_pFD_vs_Reco_Phi_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim,
                                                 Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_TL_Theta_e_AMap = hPlot2D("", "", "Reco_Theta_pFD_vs_TL_Theta_e_AMap", "Reco #theta_{pFD} vs. TL #theta_{e} used in AMaps", "#theta_{pFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02cg_Reco_Theta_pFD_vs_TL_Theta_e_AMap", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_TL_Phi_e_AMap = hPlot2D("", "", "Reco_Theta_pFD_vs_TL_Phi_e_AMap", "Reco #theta_{pFD} vs. TL #phi_{e} used in AMaps", "#theta_{pFD} [#circ]", "#phi_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ch_Reco_Theta_pFD_vs_TL_Phi_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_TL_Phi_pFD_AMap = hPlot2D("", "", "TL_Theta_pFD_vs_TL_Phi_pFD_AMap", "TL #theta_{pFD} vs. Reco #phi_{pFD} used in AMaps", "#theta_{pFD} [#circ]", "#phi_{pFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02da_TL_Theta_pFD_vs_TL_Phi_pFD_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_TL_P_e_AMap = hPlot2D("", "", "TL_Theta_pFD_vs_TL_P_e_AMap", "TL #theta_{pFD} vs. Reco P_{e} used in AMaps", "#theta_{pFD} [#circ]", "P_{e} [GeV/c]",
                                           directories.AMaps_dir_map["AMaps_1e_cut"], "02db_TL_Theta_pFD_vs_TL_P_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim,
                                           ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "TL_Theta_pFD_vs_Reco_Theta_e_AMap", "TL #theta_{pFD} vs. Reco #theta_{e} used in AMaps", "#theta_{pFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02dc_TL_Theta_pFD_vs_Reco_Theta_e_AMap", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_TL_Phi_e_AMap = hPlot2D("", "", "TL_Theta_pFD_vs_TL_Phi_e_AMap", "TL #theta_{pFD} vs. Reco #phi_{e} used in AMaps", "#theta_{pFD} [#circ]", "#phi_{e} [#circ]",
                                             directories.AMaps_dir_map["AMaps_1e_cut"], "02de_TL_Theta_pFD_vs_TL_Phi_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_TL_Theta_e_AMap = hPlot2D("", "", "TL_Theta_pFD_vs_TL_Theta_e_AMap", "TL #theta_{pFD} vs. TL #theta_{e} used in AMaps", "#theta_{pFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02df_TL_Theta_pFD_vs_TL_Theta_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_Reco_Phi_pFD_AMap = hPlot2D("", "", "Reco_Phi_pFD_vs_Reco_Phi_pFD_AMap", "Reco #phi_{pFD} vs. Reco #phi_{pFD} used in AMaps", "#phi_{pFD} [#circ]", "#phi_{pFD} [#circ]",
                                                 directories.AMaps_dir_map["AMaps_1e_cut"], "02ea_Reco_Phi_pFD_vs_Reco_Phi_pFD_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                                 numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_TL_Phi_pFD_AMap = hPlot2D("", "", "Reco_Phi_pFD_vs_TL_Phi_pFD_AMap", "Reco #phi_{pFD} vs. TL #phi_{pFD} used in AMaps", "#phi_{pFD} [#circ]", "#phi_{pFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02eb_Reco_Phi_pFD_vs_TL_Phi_pFD_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_Reco_P_e_AMap =
        hPlot2D("", "", "Reco_Phi_pFD_vs_Reco_P_e_AMap", "Reco #phi_{pFD} vs. Reco P_{e} used in AMaps", "#phi_{pFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ec_Reco_Phi_pFD_vs_Reco_P_e_AMap", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "Reco_Phi_pFD_vs_TL_P_e_AMap", "Reco #phi_{pFD} vs. TL P_{e} used in AMaps", "#phi_{pFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ed_Reco_Phi_pFD_vs_TL_P_e_AMap", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "Reco_Phi_pFD_vs_Reco_Theta_e_AMap", "Reco #phi_{pFD} vs. Reco #theta_{e} used in AMaps", "#phi_{pFD} [#circ]", "#theta_{e} [#circ]",
                                                 directories.AMaps_dir_map["AMaps_1e_cut"], "02ee_Reco_Phi_pFD_vs_Reco_Theta_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_Reco_Phi_e_AMap = hPlot2D("", "", "Reco_Phi_pFD_vs_Reco_Phi_e_AMap", "Reco #phi_{pFD} vs. Reco #phi_{e} used in AMaps", "#phi_{pFD} [#circ]", "#phi_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ef_Reco_Phi_pFD_vs_Reco_Phi_e_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_TL_Theta_e_AMap = hPlot2D("", "", "Reco_Phi_pFD_vs_TL_Theta_e_AMap", "Reco #phi_{pFD} vs. TL #theta_{e} used in AMaps", "#phi_{pFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02eg_Reco_Phi_pFD_vs_TL_Theta_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "Reco_Phi_pFD_vs_TL_Phi_e_AMap", "Reco #phi_{pFD} vs. TL #phi_{e} used in AMaps", "#phi_{pFD} [#circ]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ef_Reco_Phi_pFD_vs_TL_Phi_e_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_TL_Phi_pFD_AMap = hPlot2D("", "", "TL_Phi_pFD_vs_TL_Phi_pFD_AMap", "TL #phi_{pFD} vs. Reco #phi_{pFD} used in AMaps", "#phi_{pFD} [#circ]", "#phi_{pFD} [#circ]",
                                             directories.AMaps_dir_map["AMaps_1e_cut"], "02fa_TL_Phi_pFD_vs_TL_Phi_pFD_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "TL_Phi_pFD_vs_TL_P_e_AMap", "TL #phi_{pFD} vs. Reco P_{e} used in AMaps", "#phi_{pFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02fb_TL_Phi_pFD_vs_TL_P_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "TL_Phi_pFD_vs_Reco_Theta_e_AMap", "TL #phi_{pFD} vs. Reco #theta_{e} used in AMaps", "#phi_{pFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02fc_TL_Phi_pFD_vs_Reco_Theta_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_TL_Theta_e_AMap = hPlot2D("", "", "TL_Phi_pFD_vs_TL_Theta_e_AMap", "TL #phi_{pFD} vs. TL #theta_{e} used in AMaps", "#phi_{pFD} [#circ]", "#theta_{e} [#circ]",
                                             directories.AMaps_dir_map["AMaps_1e_cut"], "02fd_TL_Phi_pFD_vs_TL_Theta_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "TL_Phi_pFD_vs_TL_Phi_e_AMap", "TL #phi_{pFD} vs. TL #phi_{e} used in AMaps", "#phi_{pFD} [#circ]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02fh_TL_Phi_pFD_vs_TL_Phi_e_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hReco_P_nFD_AMaps = hPlot1D("1e cut", "FD", "Reco P_{nFD} used in AMaps", "Reco P_{nFD} used in AMaps", "P_{nFD} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                "03a_Reco_P_nFD_used_in_AMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hTL_P_nFD_AMaps = hPlot1D("1e cut", "FD", "TL P_{nFD} used in AMaps", "TL P_{nFD} used in AMaps", "P_{nFD} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                              "03b_TL_P_nFD_used_in_AMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Theta_nFD_AMaps = hPlot1D("1e cut", "FD", "Reco #theta_{nFD} used in AMaps", "Reco #theta_{nFD} used in AMaps", "#theta_{nFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                    "03c_Reco_Theta_nFD_used_in_AMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hTL_Theta_nFD_AMaps = hPlot1D("1e cut", "FD", "TL #theta_{nFD} used in AMaps", "TL #theta_{nFD} used in AMaps", "#theta_{nFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                  "03d_TL_Theta_nFD_used_in_AMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Phi_nFD_AMaps = hPlot1D("1e cut", "FD", "Reco #phi_{nFD} used in AMaps", "Reco #phi_{nFD} used in AMaps", "#phi_{nFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                  "01e_Reco_Phi_nFD_used_in_AMaps", -180, 180, numTH1Dbins_ReacMon_plots);
    hTL_Phi_nFD_AMaps = hPlot1D("1e cut", "FD", "TL #phi_{nFD} used in AMaps", "TL #phi_{nFD} used in AMaps", "#phi_{nFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                                "01f_TL_Phi_nFD_used_in_AMaps", -180, 180, numTH1Dbins_ReacMon_plots);
    hReco_P_nFD_vs_Reco_Theta_nFD_AMap = hPlot2D("", "", "Reco_P_nFD_vs_Reco_Theta_nFD_AMap", "Reco P_{nFD} vs. Reco #theta_{nFD} used in AMaps", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]",
                                                 directories.AMaps_dir_map["AMaps_1e_cut"], "02aa_Reco_P_nFD_vs_Reco_Theta_nFD_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_Reco_Phi_nFD_AMap = hPlot2D("", "", "Reco_P_nFD_vs_Reco_Phi_nFD_AMap", "Reco P_{nFD} vs. Reco #phi_{nFD} used in AMaps", "P_{nFD} [GeV/c]", "#phi_{nFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ab_Reco_P_nFD_vs_Reco_Phi_nFD_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_Theta_nFD_AMap = hPlot2D("", "", "Reco_P_nFD_vs_TL_Theta_nFD_AMap", "Reco P_{nFD} vs. TL #theta_{nFD} used in AMaps", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ac_Reco_P_nFD_vs_TL_Theta_nFD_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_Phi_nFD_AMap =
        hPlot2D("", "", "Reco_P_nFD_vs_TL_Phi_nFD_AMap", "Reco P_{nFD} vs. TL #phi_{nFD} used in AMaps", "P_{nFD} [GeV/c]", "#phi_{nFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ad_Reco_P_nFD_vs_TL_Phi_nFD_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_Reco_P_e_AMap =
        hPlot2D("", "", "Reco_P_nFD_vs_Reco_P_e_AMap", "Reco P_{nFD} vs. Reco P_{e} used in AMaps", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ae_Reco_P_nFD_vs_Reco_P_e_AMap", Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "Reco_P_nFD_vs_TL_P_e_AMap", "Reco P_{nFD} vs. TL P_{e} used in AMaps", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02af_Reco_P_nFD_vs_TL_P_e_AMap", Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "Reco_P_nFD_vs_Reco_Theta_e_AMap", "Reco P_{nFD} vs. Reco #theta_{e} used in AMaps", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ag_Reco_P_nFD_vs_Reco_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_Reco_Phi_e_AMap =
        hPlot2D("", "", "Reco_P_nFD_vs_Reco_Phi_e_AMap", "Reco P_{nFD} vs. Reco #phi_{e} used in AMaps", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ah_Reco_P_nFD_vs_Reco_Phi_e_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_Theta_e_AMap =
        hPlot2D("", "", "Reco_P_nFD_vs_TL_Theta_e_AMap", "Reco P_{nFD} vs. TL #theta_{e} used in AMaps", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ai_Reco_P_nFD_vs_TL_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "Reco_P_nFD_vs_TL_Phi_e_AMap", "Reco P_{nFD} vs. TL #phi_{e} used in AMaps", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02aj_Reco_P_nFD_vs_TL_Phi_e_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_Reco_Theta_nFD_AMap = hPlot2D("", "", "TL_P_nFD_vs_Reco_Theta_nFD_AMap", "TL P_{nFD} vs. Reco #theta_{nFD} used in AMaps", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ba_TL_P_nFD_vs_Reco_Theta_nFD_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_Theta_nFD_AMap =
        hPlot2D("", "", "TL_P_nFD_vs_TL_Theta_nFD_AMap", "TL P_{nFD} vs. TL #theta_{nFD} used in AMaps", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bb_TL_P_nFD_vs_TL_Theta_nFD_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_Phi_nFD_AMap =
        hPlot2D("", "", "TL_P_nFD_vs_TL_Phi_nFD_AMap", "TL P_{nFD} vs. TL #phi_{nFD} used in AMaps", "P_{nFD} [GeV/c]", "#phi_{nFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bc_TL_P_nFD_vs_TL_Phi_nFD_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "TL_P_nFD_vs_TL_P_e_AMap", "TL P_{nFD} vs. Reco P_{e} used in AMaps", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bd_TL_P_nFD_vs_TL_P_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_Reco_Theta_e_AMap =
        hPlot2D("", "", "TL_P_nFD_vs_Reco_Theta_e_AMap", "TL P_{nFD} vs. Reco #theta_{e} used in AMaps", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02be_TL_P_nFD_vs_Reco_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_Theta_e_AMap =
        hPlot2D("", "", "TL_P_nFD_vs_TL_Theta_e_AMap", "TL P_{nFD} vs. TL #theta_{e} used in AMaps", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bf_TL_P_nFD_vs_TL_Theta_e_AMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "TL_P_nFD_vs_TL_Phi_e_AMap", "TL P_{nFD} vs. TL #phi_{e} used in AMaps", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02bg_TL_P_nFD_vs_TL_Phi_e_AMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_Reco_Phi_nFD_AMap = hPlot2D("", "", "Reco_Theta_nFD_vs_Reco_Phi_nFD_AMap", "Reco #theta_{nFD} vs. Reco #phi_{nFD} used in AMaps", "#theta_{nFD} [#circ]",
                                                   "#phi_{nFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02ca_Reco_Theta_nFD_vs_Reco_Phi_nFD_AMap", ThetaFD_AMaps_llim,
                                                   ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_TL_Phi_nFD_AMap = hPlot2D("", "", "Reco_Theta_nFD_vs_TL_Phi_nFD_AMap", "Reco #theta_{nFD} vs. TL #phi_{nFD} used in AMaps", "#theta_{nFD} [#circ]",
                                                 "#phi_{nFD} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02cb_Reco_Theta_nFD_vs_TL_Phi_nFD_AMap", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_Reco_P_e_AMap = hPlot2D("", "", "Reco_Theta_nFD_vs_Reco_P_e_AMap", "Reco #theta_{nFD} vs. Reco P_{e} used in AMaps", "#theta_{nFD} [#circ]", "P_{e} [GeV/c]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02cc_Reco_Theta_nFD_vs_Reco_P_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Momentum_llim,
                                               Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "Reco_Theta_nFD_vs_TL_P_e_AMap", "Reco #theta_{nFD} vs. TL P_{e} used in AMaps", "#theta_{nFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02cd_Reco_Theta_nFD_vs_TL_P_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "Reco_Theta_nFD_vs_Reco_Theta_e_AMap", "Reco #theta_{nFD} vs. Reco #theta_{e} used in AMaps", "#theta_{nFD} [#circ]",
                                                   "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02ce_Reco_Theta_nFD_vs_Reco_Theta_e_AMap", ThetaFD_AMaps_llim,
                                                   ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_Reco_Phi_e_AMap = hPlot2D("", "", "Reco_Theta_nFD_vs_Reco_Phi_e_AMap", "Reco #theta_{nFD} vs. Reco #phi_{e} used in AMaps", "#theta_{nFD} [#circ]", "#phi_{e} [#circ]",
                                                 directories.AMaps_dir_map["AMaps_1e_cut"], "02cf_Reco_Theta_nFD_vs_Reco_Phi_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim,
                                                 Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_TL_Theta_e_AMap = hPlot2D("", "", "Reco_Theta_nFD_vs_TL_Theta_e_AMap", "Reco #theta_{nFD} vs. TL #theta_{e} used in AMaps", "#theta_{nFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02cg_Reco_Theta_nFD_vs_TL_Theta_e_AMap", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_TL_Phi_e_AMap = hPlot2D("", "", "Reco_Theta_nFD_vs_TL_Phi_e_AMap", "Reco #theta_{nFD} vs. TL #phi_{e} used in AMaps", "#theta_{nFD} [#circ]", "#phi_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ch_Reco_Theta_nFD_vs_TL_Phi_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_TL_Phi_nFD_AMap = hPlot2D("", "", "TL_Theta_nFD_vs_TL_Phi_nFD_AMap", "TL #theta_{nFD} vs. Reco #phi_{nFD} used in AMaps", "#theta_{nFD} [#circ]", "#phi_{nFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02da_TL_Theta_nFD_vs_TL_Phi_nFD_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_TL_P_e_AMap = hPlot2D("", "", "TL_Theta_nFD_vs_TL_P_e_AMap", "TL #theta_{nFD} vs. Reco P_{e} used in AMaps", "#theta_{nFD} [#circ]", "P_{e} [GeV/c]",
                                           directories.AMaps_dir_map["AMaps_1e_cut"], "02db_TL_Theta_nFD_vs_TL_P_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim,
                                           ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "TL_Theta_nFD_vs_Reco_Theta_e_AMap", "TL #theta_{nFD} vs. Reco #theta_{e} used in AMaps", "#theta_{nFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"], "02dc_TL_Theta_nFD_vs_Reco_Theta_e_AMap", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_TL_Phi_e_AMap = hPlot2D("", "", "TL_Theta_nFD_vs_TL_Phi_e_AMap", "TL #theta_{nFD} vs. Reco #phi_{e} used in AMaps", "#theta_{nFD} [#circ]", "#phi_{e} [#circ]",
                                             directories.AMaps_dir_map["AMaps_1e_cut"], "02de_TL_Theta_nFD_vs_TL_Phi_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_TL_Theta_e_AMap = hPlot2D("", "", "TL_Theta_nFD_vs_TL_Theta_e_AMap", "TL #theta_{nFD} vs. TL #theta_{e} used in AMaps", "#theta_{nFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02df_TL_Theta_nFD_vs_TL_Theta_e_AMap", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_Reco_Phi_nFD_AMap = hPlot2D("", "", "Reco_Phi_nFD_vs_Reco_Phi_nFD_AMap", "Reco #phi_{nFD} vs. Reco #phi_{nFD} used in AMaps", "#phi_{nFD} [#circ]", "#phi_{nFD} [#circ]",
                                                 directories.AMaps_dir_map["AMaps_1e_cut"], "02ea_Reco_Phi_nFD_vs_Reco_Phi_nFD_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                                 numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_TL_Phi_nFD_AMap = hPlot2D("", "", "Reco_Phi_nFD_vs_TL_Phi_nFD_AMap", "Reco #phi_{nFD} vs. TL #phi_{nFD} used in AMaps", "#phi_{nFD} [#circ]", "#phi_{nFD} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02eb_Reco_Phi_nFD_vs_TL_Phi_nFD_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_Reco_P_e_AMap =
        hPlot2D("", "", "Reco_Phi_nFD_vs_Reco_P_e_AMap", "Reco #phi_{nFD} vs. Reco P_{e} used in AMaps", "#phi_{nFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ec_Reco_Phi_nFD_vs_Reco_P_e_AMap", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "Reco_Phi_nFD_vs_TL_P_e_AMap", "Reco #phi_{nFD} vs. TL P_{e} used in AMaps", "#phi_{nFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ed_Reco_Phi_nFD_vs_TL_P_e_AMap", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "Reco_Phi_nFD_vs_Reco_Theta_e_AMap", "Reco #phi_{nFD} vs. Reco #theta_{e} used in AMaps", "#phi_{nFD} [#circ]", "#theta_{e} [#circ]",
                                                 directories.AMaps_dir_map["AMaps_1e_cut"], "02ee_Reco_Phi_nFD_vs_Reco_Theta_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_Reco_Phi_e_AMap = hPlot2D("", "", "Reco_Phi_nFD_vs_Reco_Phi_e_AMap", "Reco #phi_{nFD} vs. Reco #phi_{e} used in AMaps", "#phi_{nFD} [#circ]", "#phi_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02ef_Reco_Phi_nFD_vs_Reco_Phi_e_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_TL_Theta_e_AMap = hPlot2D("", "", "Reco_Phi_nFD_vs_TL_Theta_e_AMap", "Reco #phi_{nFD} vs. TL #theta_{e} used in AMaps", "#phi_{nFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02eg_Reco_Phi_nFD_vs_TL_Theta_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "Reco_Phi_nFD_vs_TL_Phi_e_AMap", "Reco #phi_{nFD} vs. TL #phi_{e} used in AMaps", "#phi_{nFD} [#circ]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02ef_Reco_Phi_nFD_vs_TL_Phi_e_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_TL_Phi_nFD_AMap = hPlot2D("", "", "TL_Phi_nFD_vs_TL_Phi_nFD_AMap", "TL #phi_{nFD} vs. Reco #phi_{nFD} used in AMaps", "#phi_{nFD} [#circ]", "#phi_{nFD} [#circ]",
                                             directories.AMaps_dir_map["AMaps_1e_cut"], "02fa_TL_Phi_nFD_vs_TL_Phi_nFD_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_TL_P_e_AMap =
        hPlot2D("", "", "TL_Phi_nFD_vs_TL_P_e_AMap", "TL #phi_{nFD} vs. Reco P_{e} used in AMaps", "#phi_{nFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02fb_TL_Phi_nFD_vs_TL_P_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_Reco_Theta_e_AMap = hPlot2D("", "", "TL_Phi_nFD_vs_Reco_Theta_e_AMap", "TL #phi_{nFD} vs. Reco #theta_{e} used in AMaps", "#phi_{nFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["AMaps_1e_cut"], "02fc_TL_Phi_nFD_vs_Reco_Theta_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_TL_Theta_e_AMap = hPlot2D("", "", "TL_Phi_nFD_vs_TL_Theta_e_AMap", "TL #phi_{nFD} vs. TL #theta_{e} used in AMaps", "#phi_{nFD} [#circ]", "#theta_{e} [#circ]",
                                             directories.AMaps_dir_map["AMaps_1e_cut"], "02fd_TL_Phi_nFD_vs_TL_Theta_e_AMap", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_TL_Phi_e_AMap =
        hPlot2D("", "", "TL_Phi_nFD_vs_TL_Phi_e_AMap", "TL #phi_{nFD} vs. TL #phi_{e} used in AMaps", "#phi_{nFD} [#circ]", "#phi_{e} [#circ]", directories.AMaps_dir_map["AMaps_1e_cut"],
                "02fh_TL_Phi_nFD_vs_TL_Phi_e_AMap", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hElectronAMapBCwKC = hPlot2D("", "", "Electron_AMap_BCwKC", "Electron AMap BC wKC", "#phi_{e} [#circ]", "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_BC_1e_cut"],
                                 "01_Electron_AMap_BCwKC", Phi_llim, Phi_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Electron_AMaps_Plots, numTH2Dbins_Electron_AMaps_Plots);
    hProtonAMapBCwKC = hPlot2D("", "", "Proton_AMap_BCwKC", "Proton AMap BC wKC", "#phi_{p} [#circ]", "#theta_{p} [#circ]", directories.AMaps_dir_map["WMaps_BC_1e_cut"],
                               "02_Proton_AMap_BCwKC", Phi_llim, Phi_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hNeutronAMapBCwKC = hPlot2D("", "", "Neutron_AMap_BCwKC", "Neutron AMap BC wKC", "#phi_{n} [#circ]", "#theta_{n} [#circ]", directories.AMaps_dir_map["WMaps_BC_1e_cut"],
                                "03_Neutron_AMap_BCwKC", Phi_llim, Phi_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hNucleonAMapBCwKC = hPlot2D("", "", "Nucleon_AMap_BCwKC", "Nucleon AMap BC wKC", "#phi_{nuc} [#circ]", "#theta_{nuc} [#circ]", directories.AMaps_dir_map["WMaps_BC_1e_cut"],
                                "04_Nucleon_AMap_BCwKC", Phi_llim, Phi_ulim, Theta_llim_FD, Theta_ulim_FD, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hReco_P_e_WMaps = hPlot1D("1e cut", "FD", "Reco P_{e} used in WMaps", "Reco P_{e} used in WMaps", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                              "01a_Reco_P_e_used_in_WMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hTL_P_e_WMaps = hPlot1D("1e cut", "FD", "TL P_{e} used in WMaps", "TL P_{e} used in WMaps", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"], "01b_TL_P_e_used_in_WMaps",
                            Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Theta_e_WMaps = hPlot1D("1e cut", "FD", "Reco #theta_{e} used in WMaps", "Reco #theta_{e} used in WMaps", "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                  "01c_Reco_Theta_e_used_in_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hTL_Theta_e_WMaps = hPlot1D("1e cut", "FD", "TL #theta_{e} used in WMaps", "TL #theta_{e} used in WMaps", "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                "01d_TL_Theta_e_used_in_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Phi_e_WMaps = hPlot1D("1e cut", "FD", "Reco #phi_{e} used in WMaps", "Reco #phi_{e} used in WMaps", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                "01e_Reco_Phi_e_used_in_WMaps", Phi_llim, Phi_ulim, numTH1Dbins_ReacMon_plots);
    hTL_Phi_e_WMaps = hPlot1D("1e cut", "FD", "TL #phi_{e} used in WMaps", "TL #phi_{e} used in WMaps", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                              "01f_TL_Phi_e_used_in_WMaps", Phi_llim, Phi_ulim, numTH1Dbins_ReacMon_plots);
    hReco_P_e_vs_Reco_Theta_e_WMap = hPlot2D("", "", "Reco_P_e_vs_Reco_Theta_e_WMaps", "Reco P_{e} vs. Reco #theta_{e} used in WMaps", "P_{e} [GeV/c]", "#theta_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "01aa_Reco_P_e_vs_Reco_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                             ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_e_vs_Reco_Phi_e_WMap =
        hPlot2D("", "", "Reco_P_e_vs_Reco_Phi_e_WMaps", "Reco P_{e} vs. Reco #phi_{e} used in WMaps", "P_{e} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "01ab_Reco_P_e_vs_Reco_Phi_e_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_e_vs_TL_Theta_e_WMap =
        hPlot2D("", "", "Reco_P_e_vs_TL_Theta_e_WMaps", "Reco P_{e} vs. TL #theta_{e} used in WMaps", "P_{e} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "01ba_Reco_P_e_vs_TL_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_e_vs_TL_Phi_e_WMap =
        hPlot2D("", "", "Reco_P_e_vs_TL_Phi_e_WMaps", "Reco P_{e} vs. TL #phi_{e} used in WMaps", "P_{e} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "01bb_Reco_P_e_vs_TL_Phi_e_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_e_vs_TL_Theta_e_WMap =
        hPlot2D("", "", "TL_P_e_vs_TL_Theta_e_WMap", "TL P_{e} vs. TL #theta_{e} used in WMaps", "P_{e} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "01ba_TL_P_e_vs_TL_Theta_e_WMap", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_e_vs_TL_Phi_e_WMap =
        hPlot2D("", "", "TL_P_e_vs_TL_Phi_e_WMap", "TL P_{e} vs. TL #phi_{e} used in WMaps", "P_{e} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "01ab_TL_P_e_vs_TL_Phi_e_WMap", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hReco_P_pFD_WMaps = hPlot1D("1e cut", "FD", "Reco P_{pFD} used in WMaps", "Reco P_{pFD} used in WMaps", "P_{pFD} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                "02a_Reco_P_pFD_used_in_WMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hTL_P_pFD_WMaps = hPlot1D("1e cut", "FD", "TL P_{pFD} used in WMaps", "TL P_{pFD} used in WMaps", "P_{pFD} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                              "02b_TL_P_pFD_used_in_WMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Theta_pFD_WMaps = hPlot1D("1e cut", "FD", "Reco #theta_{pFD} used in WMaps", "Reco #theta_{pFD} used in WMaps", "#theta_{pFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                    "02c_Reco_Theta_pFD_used_in_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hTL_Theta_pFD_WMaps = hPlot1D("1e cut", "FD", "TL #theta_{pFD} used in WMaps", "TL #theta_{pFD} used in WMaps", "#theta_{pFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                  "02d_TL_Theta_pFD_used_in_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Phi_pFD_WMaps = hPlot1D("1e cut", "FD", "Reco #phi_{pFD} used in WMaps", "Reco #phi_{pFD} used in WMaps", "#phi_{pFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                  "01e_Reco_Phi_pFD_used_in_WMaps", -180, 180, numTH1Dbins_ReacMon_plots);
    hTL_Phi_pFD_WMaps = hPlot1D("1e cut", "FD", "TL #phi_{pFD} used in WMaps", "TL #phi_{pFD} used in WMaps", "#phi_{pFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                "01f_TL_Phi_pFD_used_in_WMaps", -180, 180, numTH1Dbins_ReacMon_plots);
    hReco_P_pFD_vs_Reco_Theta_pFD_WMap = hPlot2D("", "", "Reco_P_pFD_vs_Reco_Theta_pFD_WMaps", "Reco P_{pFD} vs. Reco #theta_{pFD} used in WMaps", "P_{pFD} [GeV/c]", "#theta_{pFD} [#circ]",
                                                 directories.AMaps_dir_map["WMaps_1e_cut"], "02aa_Reco_P_pFD_vs_Reco_Theta_pFD_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_Reco_Phi_pFD_WMap = hPlot2D("", "", "Reco_P_pFD_vs_Reco_Phi_pFD_WMaps", "Reco P_{pFD} vs. Reco #phi_{pFD} used in WMaps", "P_{pFD} [GeV/c]", "#phi_{pFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ab_Reco_P_pFD_vs_Reco_Phi_pFD_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_Theta_pFD_WMap = hPlot2D("", "", "Reco_P_pFD_vs_TL_Theta_pFD_WMaps", "Reco P_{pFD} vs. TL #theta_{pFD} used in WMaps", "P_{pFD} [GeV/c]", "#theta_{pFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ac_Reco_P_pFD_vs_TL_Theta_pFD_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_Phi_pFD_WMap =
        hPlot2D("", "", "Reco_P_pFD_vs_TL_Phi_pFD_WMaps", "Reco P_{pFD} vs. TL #phi_{pFD} used in WMaps", "P_{pFD} [GeV/c]", "#phi_{pFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ad_Reco_P_pFD_vs_TL_Phi_pFD_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_Reco_P_e_WMap =
        hPlot2D("", "", "Reco_P_pFD_vs_Reco_P_e_WMaps", "Reco P_{pFD} vs. Reco P_{e} used in WMaps", "P_{pFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ae_Reco_P_pFD_vs_Reco_P_e_WMaps", Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_P_e_WMap =
        hPlot2D("", "", "Reco_P_pFD_vs_TL_P_e_WMaps", "Reco P_{pFD} vs. TL P_{e} used in WMaps", "P_{pFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02af_Reco_P_pFD_vs_TL_P_e_WMaps", Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "Reco_P_pFD_vs_Reco_Theta_e_WMaps", "Reco P_{pFD} vs. Reco #theta_{e} used in WMaps", "P_{pFD} [GeV/c]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ag_Reco_P_pFD_vs_Reco_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_Reco_Phi_e_WMap =
        hPlot2D("", "", "Reco_P_pFD_vs_Reco_Phi_e_WMaps", "Reco P_{pFD} vs. Reco #phi_{e} used in WMaps", "P_{pFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ah_Reco_P_pFD_vs_Reco_Phi_e_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "Reco_P_pFD_vs_TL_Theta_e_WMaps", "Reco P_{pFD} vs. TL #theta_{e} used in WMaps", "P_{pFD} [GeV/c]", "#theta_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02ai_Reco_P_pFD_vs_TL_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                             ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_pFD_vs_TL_Phi_e_WMap =
        hPlot2D("", "", "Reco_P_pFD_vs_TL_Phi_e_WMaps", "Reco P_{pFD} vs. TL #phi_{e} used in WMaps", "P_{pFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02aj_Reco_P_pFD_vs_TL_Phi_e_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_Reco_Theta_pFD_WMap = hPlot2D("", "", "TL_P_pFD_vs_Reco_Theta_pFD_WMaps", "TL P_{pFD} vs. Reco #theta_{pFD} used in WMaps", "P_{pFD} [GeV/c]", "#theta_{pFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ba_TL_P_pFD_vs_Reco_Theta_pFD_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_Theta_pFD_WMap = hPlot2D("", "", "TL_P_pFD_vs_TL_Theta_pFD_WMaps", "TL P_{pFD} vs. TL #theta_{pFD} used in WMaps", "P_{pFD} [GeV/c]", "#theta_{pFD} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02bb_TL_P_pFD_vs_TL_Theta_pFD_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                             ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_Phi_pFD_WMap =
        hPlot2D("", "", "TL_P_pFD_vs_TL_Phi_pFD_WMaps", "TL P_{pFD} vs. TL #phi_{pFD} used in WMaps", "P_{pFD} [GeV/c]", "#phi_{pFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02bc_TL_P_pFD_vs_TL_Phi_pFD_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_P_e_WMap =
        hPlot2D("", "", "TL_P_pFD_vs_TL_P_e_WMaps", "TL P_{pFD} vs. Reco P_{e} used in WMaps", "P_{pFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02bd_TL_P_pFD_vs_TL_P_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "TL_P_pFD_vs_Reco_Theta_e_WMaps", "TL P_{pFD} vs. Reco #theta_{e} used in WMaps", "P_{pFD} [GeV/c]", "#theta_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02be_TL_P_pFD_vs_Reco_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                             ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_Theta_e_WMap =
        hPlot2D("", "", "TL_P_pFD_vs_TL_Theta_e_WMaps", "TL P_{pFD} vs. TL #theta_{e} used in WMaps", "P_{pFD} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02bf_TL_P_pFD_vs_TL_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_pFD_vs_TL_Phi_e_WMap =
        hPlot2D("", "", "TL_P_pFD_vs_TL_Phi_e_WMaps", "TL P_{pFD} vs. TL #phi_{e} used in WMaps", "P_{pFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02bg_TL_P_pFD_vs_TL_Phi_e_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_Reco_Phi_pFD_WMap = hPlot2D("", "", "Reco_Theta_pFD_vs_Reco_Phi_pFD_WMaps", "Reco #theta_{pFD} vs. Reco #phi_{pFD} used in WMaps", "#theta_{pFD} [#circ]",
                                                   "#phi_{pFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02ca_Reco_Theta_pFD_vs_Reco_Phi_pFD_WMaps", ThetaFD_AMaps_llim,
                                                   ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_TL_Phi_pFD_WMap = hPlot2D("", "", "Reco_Theta_pFD_vs_TL_Phi_pFD_WMaps", "Reco #theta_{pFD} vs. TL #phi_{pFD} used in WMaps", "#theta_{pFD} [#circ]",
                                                 "#phi_{pFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02cb_Reco_Theta_pFD_vs_TL_Phi_pFD_WMaps", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_Reco_P_e_WMap = hPlot2D("", "", "Reco_Theta_pFD_vs_Reco_P_e_WMaps", "Reco #theta_{pFD} vs. Reco P_{e} used in WMaps", "#theta_{pFD} [#circ]", "P_{e} [GeV/c]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02cc_Reco_Theta_pFD_vs_Reco_P_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Momentum_llim,
                                               Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_TL_P_e_WMap = hPlot2D("", "", "Reco_Theta_pFD_vs_TL_P_e_WMaps", "Reco #theta_{pFD} vs. TL P_{e} used in WMaps", "#theta_{pFD} [#circ]", "P_{e} [GeV/c]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02cd_Reco_Theta_pFD_vs_TL_P_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Momentum_llim,
                                             Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "Reco_Theta_pFD_vs_Reco_Theta_e_WMaps", "Reco #theta_{pFD} vs. Reco #theta_{e} used in WMaps", "#theta_{pFD} [#circ]",
                                                   "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02ce_Reco_Theta_pFD_vs_Reco_Theta_e_WMaps", ThetaFD_AMaps_llim,
                                                   ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_Reco_Phi_e_WMap = hPlot2D("", "", "Reco_Theta_pFD_vs_Reco_Phi_e_WMaps", "Reco #theta_{pFD} vs. Reco #phi_{e} used in WMaps", "#theta_{pFD} [#circ]",
                                                 "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02cf_Reco_Theta_pFD_vs_Reco_Phi_e_WMaps", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "Reco_Theta_pFD_vs_TL_Theta_e_WMaps", "Reco #theta_{pFD} vs. TL #theta_{e} used in WMaps", "#theta_{pFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02cg_Reco_Theta_pFD_vs_TL_Theta_e_WMaps", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_pFD_vs_TL_Phi_e_WMap = hPlot2D("", "", "Reco_Theta_pFD_vs_TL_Phi_e_WMaps", "Reco #theta_{pFD} vs. TL #phi_{e} used in WMaps", "#theta_{pFD} [#circ]", "#phi_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ch_Reco_Theta_pFD_vs_TL_Phi_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_TL_Phi_pFD_WMap = hPlot2D("", "", "TL_Theta_pFD_vs_TL_Phi_pFD_WMaps", "TL #theta_{pFD} vs. Reco #phi_{pFD} used in WMaps", "#theta_{pFD} [#circ]", "#phi_{pFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02da_TL_Theta_pFD_vs_TL_Phi_pFD_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_TL_P_e_WMap = hPlot2D("", "", "TL_Theta_pFD_vs_TL_P_e_WMaps", "TL #theta_{pFD} vs. Reco P_{e} used in WMaps", "#theta_{pFD} [#circ]", "P_{e} [GeV/c]",
                                           directories.AMaps_dir_map["WMaps_1e_cut"], "02db_TL_Theta_pFD_vs_TL_P_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim,
                                           ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "TL_Theta_pFD_vs_Reco_Theta_e_WMaps", "TL #theta_{pFD} vs. Reco #theta_{e} used in WMaps", "#theta_{pFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02dc_TL_Theta_pFD_vs_Reco_Theta_e_WMaps", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_TL_Phi_e_WMap = hPlot2D("", "", "TL_Theta_pFD_vs_TL_Phi_e_WMaps", "TL #theta_{pFD} vs. Reco #phi_{e} used in WMaps", "#theta_{pFD} [#circ]", "#phi_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02de_TL_Theta_pFD_vs_TL_Phi_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_pFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "TL_Theta_pFD_vs_TL_Theta_e_WMaps", "TL #theta_{pFD} vs. TL #theta_{e} used in WMaps", "#theta_{pFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02df_TL_Theta_pFD_vs_TL_Theta_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_Reco_Phi_pFD_WMap = hPlot2D("", "", "Reco_Phi_pFD_vs_Reco_Phi_pFD_WMaps", "Reco #phi_{pFD} vs. Reco #phi_{pFD} used in WMaps", "#phi_{pFD} [#circ]",
                                                 "#phi_{pFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02ea_Reco_Phi_pFD_vs_Reco_Phi_pFD_WMaps", Phi_llim, Phi_ulim, Phi_llim,
                                                 Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_TL_Phi_pFD_WMap = hPlot2D("", "", "Reco_Phi_pFD_vs_TL_Phi_pFD_WMaps", "Reco #phi_{pFD} vs. TL #phi_{pFD} used in WMaps", "#phi_{pFD} [#circ]", "#phi_{pFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02eb_Reco_Phi_pFD_vs_TL_Phi_pFD_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_Reco_P_e_WMap =
        hPlot2D("", "", "Reco_Phi_pFD_vs_Reco_P_e_WMaps", "Reco #phi_{pFD} vs. Reco P_{e} used in WMaps", "#phi_{pFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ec_Reco_Phi_pFD_vs_Reco_P_e_WMaps", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_TL_P_e_WMap =
        hPlot2D("", "", "Reco_Phi_pFD_vs_TL_P_e_WMaps", "Reco #phi_{pFD} vs. TL P_{e} used in WMaps", "#phi_{pFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ed_Reco_Phi_pFD_vs_TL_P_e_WMaps", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "Reco_Phi_pFD_vs_Reco_Theta_e_WMaps", "Reco #phi_{pFD} vs. Reco #theta_{e} used in WMaps", "#phi_{pFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02ee_Reco_Phi_pFD_vs_Reco_Theta_e_WMaps", Phi_llim, Phi_ulim,
                                                 ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_Reco_Phi_e_WMap = hPlot2D("", "", "Reco_Phi_pFD_vs_Reco_Phi_e_WMaps", "Reco #phi_{pFD} vs. Reco #phi_{e} used in WMaps", "#phi_{pFD} [#circ]", "#phi_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ef_Reco_Phi_pFD_vs_Reco_Phi_e_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "Reco_Phi_pFD_vs_TL_Theta_e_WMaps", "Reco #phi_{pFD} vs. TL #theta_{e} used in WMaps", "#phi_{pFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02eg_Reco_Phi_pFD_vs_TL_Theta_e_WMaps", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_pFD_vs_TL_Phi_e_WMap = hPlot2D("", "", "Reco_Phi_pFD_vs_TL_Phi_e_WMaps", "Reco #phi_{pFD} vs. TL #phi_{e} used in WMaps", "#phi_{pFD} [#circ]", "#phi_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02ef_Reco_Phi_pFD_vs_TL_Phi_e_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_TL_Phi_pFD_WMap = hPlot2D("", "", "TL_Phi_pFD_vs_TL_Phi_pFD_WMaps", "TL #phi_{pFD} vs. Reco #phi_{pFD} used in WMaps", "#phi_{pFD} [#circ]", "#phi_{pFD} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02fa_TL_Phi_pFD_vs_TL_Phi_pFD_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_TL_P_e_WMap =
        hPlot2D("", "", "TL_Phi_pFD_vs_TL_P_e_WMaps", "TL #phi_{pFD} vs. Reco P_{e} used in WMaps", "#phi_{pFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02fb_TL_Phi_pFD_vs_TL_P_e_WMaps", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "TL_Phi_pFD_vs_Reco_Theta_e_WMaps", "TL #phi_{pFD} vs. Reco #theta_{e} used in WMaps", "#phi_{pFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02fc_TL_Phi_pFD_vs_Reco_Theta_e_WMaps", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "TL_Phi_pFD_vs_TL_Theta_e_WMaps", "TL #phi_{pFD} vs. TL #theta_{e} used in WMaps", "#phi_{pFD} [#circ]", "#theta_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02fd_TL_Phi_pFD_vs_TL_Theta_e_WMaps", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_pFD_vs_TL_Phi_e_WMap =
        hPlot2D("", "", "TL_Phi_pFD_vs_TL_Phi_e_WMaps", "TL #phi_{pFD} vs. TL #phi_{e} used in WMaps", "#phi_{pFD} [#circ]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02fh_TL_Phi_pFD_vs_TL_Phi_e_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hReco_P_nFD_WMaps = hPlot1D("1e cut", "FD", "Reco P_{nFD} used in WMaps", "Reco P_{nFD} used in WMaps", "P_{nFD} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                "03a_Reco_P_nFD_used_in_WMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hTL_P_nFD_WMaps = hPlot1D("1e cut", "FD", "TL P_{nFD} used in WMaps", "TL P_{nFD} used in WMaps", "P_{nFD} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                              "03b_TL_P_nFD_used_in_WMaps", Momentum_llim, Momentum_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Theta_nFD_WMaps = hPlot1D("1e cut", "FD", "Reco #theta_{nFD} used in WMaps", "Reco #theta_{nFD} used in WMaps", "#theta_{nFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                    "03c_Reco_Theta_nFD_used_in_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hTL_Theta_nFD_WMaps = hPlot1D("1e cut", "FD", "TL #theta_{nFD} used in WMaps", "TL #theta_{nFD} used in WMaps", "#theta_{nFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                  "03d_TL_Theta_nFD_used_in_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH1Dbins_ReacMon_plots);
    hReco_Phi_nFD_WMaps = hPlot1D("1e cut", "FD", "Reco #phi_{nFD} used in WMaps", "Reco #phi_{nFD} used in WMaps", "#phi_{nFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                  "01e_Reco_Phi_nFD_used_in_WMaps", -180, 180, numTH1Dbins_ReacMon_plots);
    hTL_Phi_nFD_WMaps = hPlot1D("1e cut", "FD", "TL #phi_{nFD} used in WMaps", "TL #phi_{nFD} used in WMaps", "#phi_{nFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                                "01f_TL_Phi_nFD_used_in_WMaps", -180, 180, numTH1Dbins_ReacMon_plots);
    hReco_P_nFD_vs_Reco_Theta_nFD_WMap = hPlot2D("", "", "Reco_P_nFD_vs_Reco_Theta_nFD_WMaps", "Reco P_{nFD} vs. Reco #theta_{nFD} used in WMaps", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]",
                                                 directories.AMaps_dir_map["WMaps_1e_cut"], "02aa_Reco_P_nFD_vs_Reco_Theta_nFD_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_Reco_Phi_nFD_WMap = hPlot2D("", "", "Reco_P_nFD_vs_Reco_Phi_nFD_WMaps", "Reco P_{nFD} vs. Reco #phi_{nFD} used in WMaps", "P_{nFD} [GeV/c]", "#phi_{nFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ab_Reco_P_nFD_vs_Reco_Phi_nFD_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_Theta_nFD_WMap = hPlot2D("", "", "Reco_P_nFD_vs_TL_Theta_nFD_WMaps", "Reco P_{nFD} vs. TL #theta_{nFD} used in WMaps", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ac_Reco_P_nFD_vs_TL_Theta_nFD_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_Phi_nFD_WMap =
        hPlot2D("", "", "Reco_P_nFD_vs_TL_Phi_nFD_WMaps", "Reco P_{nFD} vs. TL #phi_{nFD} used in WMaps", "P_{nFD} [GeV/c]", "#phi_{nFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ad_Reco_P_nFD_vs_TL_Phi_nFD_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_Reco_P_e_WMap =
        hPlot2D("", "", "Reco_P_nFD_vs_Reco_P_e_WMaps", "Reco P_{nFD} vs. Reco P_{e} used in WMaps", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ae_Reco_P_nFD_vs_Reco_P_e_WMaps", Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_P_e_WMap =
        hPlot2D("", "", "Reco_P_nFD_vs_TL_P_e_WMaps", "Reco P_{nFD} vs. TL P_{e} used in WMaps", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02af_Reco_P_nFD_vs_TL_P_e_WMaps", Momentum_llim, Momentum_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "Reco_P_nFD_vs_Reco_Theta_e_WMaps", "Reco P_{nFD} vs. Reco #theta_{e} used in WMaps", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ag_Reco_P_nFD_vs_Reco_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_Reco_Phi_e_WMap =
        hPlot2D("", "", "Reco_P_nFD_vs_Reco_Phi_e_WMaps", "Reco P_{nFD} vs. Reco #phi_{e} used in WMaps", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ah_Reco_P_nFD_vs_Reco_Phi_e_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "Reco_P_nFD_vs_TL_Theta_e_WMaps", "Reco P_{nFD} vs. TL #theta_{e} used in WMaps", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02ai_Reco_P_nFD_vs_TL_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                             ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_P_nFD_vs_TL_Phi_e_WMap =
        hPlot2D("", "", "Reco_P_nFD_vs_TL_Phi_e_WMaps", "Reco P_{nFD} vs. TL #phi_{e} used in WMaps", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02aj_Reco_P_nFD_vs_TL_Phi_e_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_Reco_Theta_nFD_WMap = hPlot2D("", "", "TL_P_nFD_vs_Reco_Theta_nFD_WMaps", "TL P_{nFD} vs. Reco #theta_{nFD} used in WMaps", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ba_TL_P_nFD_vs_Reco_Theta_nFD_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_Theta_nFD_WMap = hPlot2D("", "", "TL_P_nFD_vs_TL_Theta_nFD_WMaps", "TL P_{nFD} vs. TL #theta_{nFD} used in WMaps", "P_{nFD} [GeV/c]", "#theta_{nFD} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02bb_TL_P_nFD_vs_TL_Theta_nFD_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                             ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_Phi_nFD_WMap =
        hPlot2D("", "", "TL_P_nFD_vs_TL_Phi_nFD_WMaps", "TL P_{nFD} vs. TL #phi_{nFD} used in WMaps", "P_{nFD} [GeV/c]", "#phi_{nFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02bc_TL_P_nFD_vs_TL_Phi_nFD_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_P_e_WMap =
        hPlot2D("", "", "TL_P_nFD_vs_TL_P_e_WMaps", "TL P_{nFD} vs. Reco P_{e} used in WMaps", "P_{nFD} [GeV/c]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02bd_TL_P_nFD_vs_TL_P_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "TL_P_nFD_vs_Reco_Theta_e_WMaps", "TL P_{nFD} vs. Reco #theta_{e} used in WMaps", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02be_TL_P_nFD_vs_Reco_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim,
                                             ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_Theta_e_WMap =
        hPlot2D("", "", "TL_P_nFD_vs_TL_Theta_e_WMaps", "TL P_{nFD} vs. TL #theta_{e} used in WMaps", "P_{nFD} [GeV/c]", "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02bf_TL_P_nFD_vs_TL_Theta_e_WMaps", Momentum_llim, Momentum_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_P_nFD_vs_TL_Phi_e_WMap =
        hPlot2D("", "", "TL_P_nFD_vs_TL_Phi_e_WMaps", "TL P_{nFD} vs. TL #phi_{e} used in WMaps", "P_{nFD} [GeV/c]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02bg_TL_P_nFD_vs_TL_Phi_e_WMaps", Momentum_llim, Momentum_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_Reco_Phi_nFD_WMap = hPlot2D("", "", "Reco_Theta_nFD_vs_Reco_Phi_nFD_WMaps", "Reco #theta_{nFD} vs. Reco #phi_{nFD} used in WMaps", "#theta_{nFD} [#circ]",
                                                   "#phi_{nFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02ca_Reco_Theta_nFD_vs_Reco_Phi_nFD_WMaps", ThetaFD_AMaps_llim,
                                                   ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_TL_Phi_nFD_WMap = hPlot2D("", "", "Reco_Theta_nFD_vs_TL_Phi_nFD_WMaps", "Reco #theta_{nFD} vs. TL #phi_{nFD} used in WMaps", "#theta_{nFD} [#circ]",
                                                 "#phi_{nFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02cb_Reco_Theta_nFD_vs_TL_Phi_nFD_WMaps", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_Reco_P_e_WMap = hPlot2D("", "", "Reco_Theta_nFD_vs_Reco_P_e_WMaps", "Reco #theta_{nFD} vs. Reco P_{e} used in WMaps", "#theta_{nFD} [#circ]", "P_{e} [GeV/c]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02cc_Reco_Theta_nFD_vs_Reco_P_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Momentum_llim,
                                               Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_TL_P_e_WMap = hPlot2D("", "", "Reco_Theta_nFD_vs_TL_P_e_WMaps", "Reco #theta_{nFD} vs. TL P_{e} used in WMaps", "#theta_{nFD} [#circ]", "P_{e} [GeV/c]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02cd_Reco_Theta_nFD_vs_TL_P_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Momentum_llim,
                                             Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "Reco_Theta_nFD_vs_Reco_Theta_e_WMaps", "Reco #theta_{nFD} vs. Reco #theta_{e} used in WMaps", "#theta_{nFD} [#circ]",
                                                   "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02ce_Reco_Theta_nFD_vs_Reco_Theta_e_WMaps", ThetaFD_AMaps_llim,
                                                   ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_Reco_Phi_e_WMap = hPlot2D("", "", "Reco_Theta_nFD_vs_Reco_Phi_e_WMaps", "Reco #theta_{nFD} vs. Reco #phi_{e} used in WMaps", "#theta_{nFD} [#circ]",
                                                 "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02cf_Reco_Theta_nFD_vs_Reco_Phi_e_WMaps", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "Reco_Theta_nFD_vs_TL_Theta_e_WMaps", "Reco #theta_{nFD} vs. TL #theta_{e} used in WMaps", "#theta_{nFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02cg_Reco_Theta_nFD_vs_TL_Theta_e_WMaps", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Theta_nFD_vs_TL_Phi_e_WMap = hPlot2D("", "", "Reco_Theta_nFD_vs_TL_Phi_e_WMaps", "Reco #theta_{nFD} vs. TL #phi_{e} used in WMaps", "#theta_{nFD} [#circ]", "#phi_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ch_Reco_Theta_nFD_vs_TL_Phi_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_TL_Phi_nFD_WMap = hPlot2D("", "", "TL_Theta_nFD_vs_TL_Phi_nFD_WMaps", "TL #theta_{nFD} vs. Reco #phi_{nFD} used in WMaps", "#theta_{nFD} [#circ]", "#phi_{nFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02da_TL_Theta_nFD_vs_TL_Phi_nFD_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_TL_P_e_WMap = hPlot2D("", "", "TL_Theta_nFD_vs_TL_P_e_WMaps", "TL #theta_{nFD} vs. Reco P_{e} used in WMaps", "#theta_{nFD} [#circ]", "P_{e} [GeV/c]",
                                           directories.AMaps_dir_map["WMaps_1e_cut"], "02db_TL_Theta_nFD_vs_TL_P_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim,
                                           ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "TL_Theta_nFD_vs_Reco_Theta_e_WMaps", "TL #theta_{nFD} vs. Reco #theta_{e} used in WMaps", "#theta_{nFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02dc_TL_Theta_nFD_vs_Reco_Theta_e_WMaps", ThetaFD_AMaps_llim,
                                                 ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_TL_Phi_e_WMap = hPlot2D("", "", "TL_Theta_nFD_vs_TL_Phi_e_WMaps", "TL #theta_{nFD} vs. Reco #phi_{e} used in WMaps", "#theta_{nFD} [#circ]", "#phi_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02de_TL_Theta_nFD_vs_TL_Phi_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Theta_nFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "TL_Theta_nFD_vs_TL_Theta_e_WMaps", "TL #theta_{nFD} vs. TL #theta_{e} used in WMaps", "#theta_{nFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02df_TL_Theta_nFD_vs_TL_Theta_e_WMaps", ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, ThetaFD_AMaps_llim,
                                               ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_Reco_Phi_nFD_WMap = hPlot2D("", "", "Reco_Phi_nFD_vs_Reco_Phi_nFD_WMaps", "Reco #phi_{nFD} vs. Reco #phi_{nFD} used in WMaps", "#phi_{nFD} [#circ]",
                                                 "#phi_{nFD} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02ea_Reco_Phi_nFD_vs_Reco_Phi_nFD_WMaps", Phi_llim, Phi_ulim, Phi_llim,
                                                 Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_TL_Phi_nFD_WMap = hPlot2D("", "", "Reco_Phi_nFD_vs_TL_Phi_nFD_WMaps", "Reco #phi_{nFD} vs. TL #phi_{nFD} used in WMaps", "#phi_{nFD} [#circ]", "#phi_{nFD} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02eb_Reco_Phi_nFD_vs_TL_Phi_nFD_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_Reco_P_e_WMap =
        hPlot2D("", "", "Reco_Phi_nFD_vs_Reco_P_e_WMaps", "Reco #phi_{nFD} vs. Reco P_{e} used in WMaps", "#phi_{nFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ec_Reco_Phi_nFD_vs_Reco_P_e_WMaps", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_TL_P_e_WMap =
        hPlot2D("", "", "Reco_Phi_nFD_vs_TL_P_e_WMaps", "Reco #phi_{nFD} vs. TL P_{e} used in WMaps", "#phi_{nFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02ed_Reco_Phi_nFD_vs_TL_P_e_WMaps", Phi_llim, Phi_ulim, Momentum_llim, Momentum_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "Reco_Phi_nFD_vs_Reco_Theta_e_WMaps", "Reco #phi_{nFD} vs. Reco #theta_{e} used in WMaps", "#phi_{nFD} [#circ]",
                                                 "#theta_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"], "02ee_Reco_Phi_nFD_vs_Reco_Theta_e_WMaps", Phi_llim, Phi_ulim,
                                                 ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_Reco_Phi_e_WMap = hPlot2D("", "", "Reco_Phi_nFD_vs_Reco_Phi_e_WMaps", "Reco #phi_{nFD} vs. Reco #phi_{e} used in WMaps", "#phi_{nFD} [#circ]", "#phi_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02ef_Reco_Phi_nFD_vs_Reco_Phi_e_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "Reco_Phi_nFD_vs_TL_Theta_e_WMaps", "Reco #phi_{nFD} vs. TL #theta_{e} used in WMaps", "#phi_{nFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02eg_Reco_Phi_nFD_vs_TL_Theta_e_WMaps", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hReco_Phi_nFD_vs_TL_Phi_e_WMap = hPlot2D("", "", "Reco_Phi_nFD_vs_TL_Phi_e_WMaps", "Reco #phi_{nFD} vs. TL #phi_{e} used in WMaps", "#phi_{nFD} [#circ]", "#phi_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02ef_Reco_Phi_nFD_vs_TL_Phi_e_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_TL_Phi_nFD_WMap = hPlot2D("", "", "TL_Phi_nFD_vs_TL_Phi_nFD_WMaps", "TL #phi_{nFD} vs. Reco #phi_{nFD} used in WMaps", "#phi_{nFD} [#circ]", "#phi_{nFD} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02fa_TL_Phi_nFD_vs_TL_Phi_nFD_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_TL_P_e_WMap =
        hPlot2D("", "", "TL_Phi_nFD_vs_TL_P_e_WMaps", "TL #phi_{nFD} vs. Reco P_{e} used in WMaps", "#phi_{nFD} [#circ]", "P_{e} [GeV/c]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02fb_TL_Phi_nFD_vs_TL_P_e_WMaps", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_Reco_Theta_e_WMap = hPlot2D("", "", "TL_Phi_nFD_vs_Reco_Theta_e_WMaps", "TL #phi_{nFD} vs. Reco #theta_{e} used in WMaps", "#phi_{nFD} [#circ]", "#theta_{e} [#circ]",
                                               directories.AMaps_dir_map["WMaps_1e_cut"], "02fc_TL_Phi_nFD_vs_Reco_Theta_e_WMaps", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                               numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_TL_Theta_e_WMap = hPlot2D("", "", "TL_Phi_nFD_vs_TL_Theta_e_WMaps", "TL #phi_{nFD} vs. TL #theta_{e} used in WMaps", "#phi_{nFD} [#circ]", "#theta_{e} [#circ]",
                                             directories.AMaps_dir_map["WMaps_1e_cut"], "02fd_TL_Phi_nFD_vs_TL_Theta_e_WMaps", Phi_llim, Phi_ulim, ThetaFD_AMaps_llim, ThetaFD_AMaps_ulim,
                                             numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hTL_Phi_nFD_vs_TL_Phi_e_WMap =
        hPlot2D("", "", "TL_Phi_nFD_vs_TL_Phi_e_WMaps", "TL #phi_{nFD} vs. TL #phi_{e} used in WMaps", "#phi_{nFD} [#circ]", "#phi_{e} [#circ]", directories.AMaps_dir_map["WMaps_1e_cut"],
                "02fh_TL_Phi_nFD_vs_TL_Phi_e_WMaps", Phi_llim, Phi_ulim, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
#pragma endregion
}

void FinalStateHistograms::InitAngleHistograms(const HistogramDefinitionSettings& hDefSettings) {
    auto& directories = angle_directories_;
    const int numTH1Dbins_Ang_Plots = hDefSettings.numTH1Dbins_Ang_Plots;
    const int numTH2Dbins_Ang_Plots = hDefSettings.numTH2Dbins_Ang_Plots;
    const int numTH2Dbins_Electron_Ang_Plots = hDefSettings.numTH2Dbins_Electron_Ang_Plots;
    const int numTH2Dbins_Nucleon_Ang_Plots = hDefSettings.numTH2Dbins_Nucleon_Ang_Plots;
    const double Theta_llim_FD = hDefSettings.Theta_llim_FD;
    const double Theta_ulim_FD = hDefSettings.Theta_ulim_FD;
    const double Theta_llim_CD = hDefSettings.Theta_llim_CD;
    const double Theta_ulim_CD = hDefSettings.Theta_ulim_CD;
    const double Phi_llim = hDefSettings.Phi_llim;
    const double Phi_ulim = hDefSettings.Phi_ulim;
    const double W_llim = hDefSettings.W_llim;
    const double W_ulim = hDefSettings.W_ulim;
    const double Opening_Ang_narrow_llim = hDefSettings.Opening_Ang_narrow_llim;
    const double Opening_Ang_narrow_ulim = hDefSettings.Opening_Ang_narrow_ulim;
    const double Opening_Ang_wide_llim = hDefSettings.Opening_Ang_wide_llim;
    const double Opening_Ang_wide_ulim = hDefSettings.Opening_Ang_wide_ulim;
    const double beamE11 = hDefSettings.Momentum_ulim;

    ElectronMomSliceLimits = CustomElectronMomSliceLimits_C12x4_simulation_G18_Q204_6GeV;
    auto& HistoList = angle_histo_list_;

#pragma region /* Angle histograms */

    // Theta_e --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_e histograms
    THStack* sTheta_e = OwnHistogram(new THStack("#theta_{e} (CD & FD)", "#theta_{e} of Outgoing Electron (no #(e) cut, CD & FD);#theta_{e} [#circ];"));
    THStack* sTheta_e_1p_FD = OwnHistogram(new THStack("#theta_{e} (1p, FD)", "#theta_{e} of Outgoing Electron (1p, FD);#theta_{e} [#circ];"));
    THStack* sTheta_e_1n_FD = OwnHistogram(new THStack("#theta_{e} (1n, FD)", "#theta_{e} of Outgoing Electron (1n, FD);#theta_{e} [#circ];"));
    THStack* sTheta_e_2p_FD = OwnHistogram(new THStack("#theta_{e} (2p, FD)", "#theta_{e} of Outgoing Electron (2p, FD);#theta_{e} [#circ];"));
    THStack* sTheta_e_pFDpCD_FD = OwnHistogram(new THStack("#theta_{e} (pFDpCD, FD)", "#theta_{e} of Outgoing Electron (pFDpCD, FD);#theta_{e} [#circ];"));
    THStack* sTheta_e_nFDpCD_FD = OwnHistogram(new THStack("#theta_{e} (nFDpCD, FD)", "#theta_{e} of Outgoing Electron (nFDpCD, FD);#theta_{e} [#circ];"));

    // Theta_e histograms (no #(e) cut)
    TH1D* hTheta_e_All_e_FD =
        OwnHistogram(new TH1D("#theta_{e} (no #(e) cut, FD)", "#theta_{e} of Outgoing Electron (no #(e) cut, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_All_e_FD_Dir = directories.Angle_dir_map["Theta_e_All_e"];

    // Theta_e histograms (1e cut)
    TH1D* hTheta_e_1e_cut_FD = OwnHistogram(new TH1D("#theta_{e} (1e cut, FD)", "#theta_{e} of Outgoing Electron (1e cut, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_1e_cut_FD_Dir = directories.Angle_dir_map["Theta_e_1e_cut"];

    // Theta_e histograms (1p)
    TH1D* hTheta_e_All_Int_1p_FD =
        OwnHistogram(new TH1D("#theta_{e} (All Int., 1p, FD)", "#theta_{e} of Outgoing Electron (All Int., 1p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_QEL_1p_FD =
        OwnHistogram(new TH1D("#theta_{e} (QEL Only, 1p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_MEC_1p_FD =
        OwnHistogram(new TH1D("#theta_{e} (MEC only, 1p, FD)", "#theta_{e} of Outgoing Electron (MEC only, 1p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_RES_1p_FD =
        OwnHistogram(new TH1D("#theta_{e} (RES only, 1p, FD)", "#theta_{e} of Outgoing Electron (RES only, 1p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_DIS_1p_FD =
        OwnHistogram(new TH1D("#theta_{e} (DIS only, 1p, FD)", "#theta_{e} of Outgoing Electron (DIS only, 1p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_All_Int_1p_FD_Dir = directories.Angle_dir_map["Theta_e_1p"];
    std::string hTheta_e_QEL_1p_FD_Dir = directories.Angle_dir_map["Theta_e_1p"];
    std::string hTheta_e_MEC_1p_FD_Dir = directories.Angle_dir_map["Theta_e_1p"];
    std::string hTheta_e_RES_1p_FD_Dir = directories.Angle_dir_map["Theta_e_1p"];
    std::string hTheta_e_DIS_1p_FD_Dir = directories.Angle_dir_map["Theta_e_1p"];

    TH2D* hTheta_e_VS_P_e_1p_FD = OwnHistogram(new TH2D("#theta_{e} vs. P_{e} (All Int., 1p)", "#theta_{e} vs. P_{e} (All Int., 1p);P_{e} [GeV/c];#theta_{e} [#circ]", numTH2Dbins_Ang_Plots, 0, beamE11,
                                           numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH2D* hTheta_e_VS_W_1p_FD =
        OwnHistogram(new TH2D("#theta_{e} vs. W (All Int., 1p)", "#theta_{e} vs. W (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{e} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_P_e_1p_FD_Dir = directories.Angle_dir_map["Theta_e_1p"];
    std::string hTheta_e_VS_W_1p_FD_Dir = directories.Angle_dir_map["Theta_e_1p"];

    // Theta_e histograms (1n)
    TH1D* hTheta_e_All_Int_1n_FD =
        OwnHistogram(new TH1D("#theta_{e} (All Int., 1n, FD)", "#theta_{e} of Outgoing Electron (All Int., 1n, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_QEL_1n_FD =
        OwnHistogram(new TH1D("#theta_{e} (QEL Only, 1n, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1n, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_MEC_1n_FD =
        OwnHistogram(new TH1D("#theta_{e} (MEC only, 1n, FD)", "#theta_{e} of Outgoing Electron (MEC only, 1n, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_RES_1n_FD =
        OwnHistogram(new TH1D("#theta_{e} (RES only, 1n, FD)", "#theta_{e} of Outgoing Electron (RES only, 1n, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_DIS_1n_FD =
        OwnHistogram(new TH1D("#theta_{e} (DIS only, 1n, FD)", "#theta_{e} of Outgoing Electron (DIS only, 1n, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_All_Int_1n_FD_Dir = directories.Angle_dir_map["Theta_e_1n"];
    std::string hTheta_e_QEL_1n_FD_Dir = directories.Angle_dir_map["Theta_e_1n"];
    std::string hTheta_e_MEC_1n_FD_Dir = directories.Angle_dir_map["Theta_e_1n"];
    std::string hTheta_e_RES_1n_FD_Dir = directories.Angle_dir_map["Theta_e_1n"];
    std::string hTheta_e_DIS_1n_FD_Dir = directories.Angle_dir_map["Theta_e_1n"];

    TH2D* hTheta_e_VS_P_e_1n_FD = OwnHistogram(new TH2D("#theta_{e} vs. P_{e} (All Int., 1n)", "#theta_{e} vs. P_{e} (All Int., 1n);P_{e} [GeV/c];#theta_{e} [#circ]", numTH2Dbins_Ang_Plots, 0, beamE11,
                                           numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH2D* hTheta_e_VS_W_1n_FD =
        OwnHistogram(new TH2D("#theta_{e} vs. W (All Int., 1n)", "#theta_{e} vs. W (All Int., 1n);W = #sqrt{(#omega + m_{n})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{e} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_P_e_1n_FD_Dir = directories.Angle_dir_map["Theta_e_1n"];
    std::string hTheta_e_VS_W_1n_FD_Dir = directories.Angle_dir_map["Theta_e_1n"];

    // Theta_e histograms (2p)
    TH1D* hTheta_e_All_Int_2p_FD =
        OwnHistogram(new TH1D("#theta_{e} (All Int., 2p, FD)", "#theta_{e} of Outgoing Electron (All Int., 2p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_QEL_2p_FD =
        OwnHistogram(new TH1D("#theta_{e} (QEL Only, 2p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 2p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_MEC_2p_FD =
        OwnHistogram(new TH1D("#theta_{e} (MEC only, 2p, FD)", "#theta_{e} of Outgoing Electron (MEC only, 2p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_RES_2p_FD =
        OwnHistogram(new TH1D("#theta_{e} (RES only, 2p, FD)", "#theta_{e} of Outgoing Electron (RES only, 2p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_DIS_2p_FD =
        OwnHistogram(new TH1D("#theta_{e} (DIS only, 2p, FD)", "#theta_{e} of Outgoing Electron (DIS only, 2p, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_All_Int_2p_FD_Dir = directories.Angle_dir_map["Theta_e_2p"];
    std::string hTheta_e_QEL_2p_FD_Dir = directories.Angle_dir_map["Theta_e_2p"];
    std::string hTheta_e_MEC_2p_FD_Dir = directories.Angle_dir_map["Theta_e_2p"];
    std::string hTheta_e_RES_2p_FD_Dir = directories.Angle_dir_map["Theta_e_2p"];
    std::string hTheta_e_DIS_2p_FD_Dir = directories.Angle_dir_map["Theta_e_2p"];

    // Theta_e histograms (pFDpCD)
    TH1D* hTheta_e_All_Int_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (All Int., pFDpCD, FD)", "#theta_{e} of Outgoing Electron (All Int., pFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_QEL_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (QEL Only, pFDpCD, FD)", "#theta_{e} of Outgoing Electron (QEL Only, pFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_MEC_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (MEC only, pFDpCD, FD)", "#theta_{e} of Outgoing Electron (MEC only, pFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_RES_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (RES only, pFDpCD, FD)", "#theta_{e} of Outgoing Electron (RES only, pFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_DIS_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (DIS only, pFDpCD, FD)", "#theta_{e} of Outgoing Electron (DIS only, pFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_All_Int_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_pFDpCD"];
    std::string hTheta_e_QEL_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_pFDpCD"];
    std::string hTheta_e_MEC_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_pFDpCD"];
    std::string hTheta_e_RES_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_pFDpCD"];
    std::string hTheta_e_DIS_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_pFDpCD"];

    TH2D* hTheta_e_VS_P_e_pFDpCD_FD = OwnHistogram(new TH2D("#theta_{e} vs. P_{e} (All Int., pFDpCD)", "#theta_{e} vs. P_{e} (All Int., pFDpCD);P_{e} [GeV/c];#theta_{e} [#circ]", numTH2Dbins_Ang_Plots,
                                               0, beamE11, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH2D* hTheta_e_VS_W_pFDpCD_FD =
        OwnHistogram(new TH2D("#theta_{e} vs. W (All Int., pFDpCD)", "#theta_{e} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{e} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_P_e_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_pFDpCD"];
    std::string hTheta_e_VS_W_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_pFDpCD"];

    // Theta_e histograms (nFDpCD)
    TH1D* hTheta_e_All_Int_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (All Int., nFDpCD, FD)", "#theta_{e} of Outgoing Electron (All Int., nFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_QEL_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (QEL Only, nFDpCD, FD)", "#theta_{e} of Outgoing Electron (QEL Only, nFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_MEC_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (MEC only, nFDpCD, FD)", "#theta_{e} of Outgoing Electron (MEC only, nFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_RES_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (RES only, nFDpCD, FD)", "#theta_{e} of Outgoing Electron (RES only, nFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_e_DIS_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{e} (DIS only, nFDpCD, FD)", "#theta_{e} of Outgoing Electron (DIS only, nFDpCD, FD);#theta_{e} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_All_Int_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_nFDpCD"];
    std::string hTheta_e_QEL_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_nFDpCD"];
    std::string hTheta_e_MEC_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_nFDpCD"];
    std::string hTheta_e_RES_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_nFDpCD"];
    std::string hTheta_e_DIS_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_nFDpCD"];

    TH2D* hTheta_e_VS_P_e_nFDpCD_FD = OwnHistogram(new TH2D("#theta_{e} vs. P_{e} (All Int., nFDpCD)", "#theta_{e} vs. P_{e} (All Int., nFDpCD);P_{e} [GeV/c];#theta_{e} [#circ]", numTH2Dbins_Ang_Plots,
                                               0, beamE11, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH2D* hTheta_e_VS_W_nFDpCD_FD =
        OwnHistogram(new TH2D("#theta_{e} vs. W (All Int., nFDpCD)", "#theta_{e} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{e} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_P_e_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_nFDpCD"];
    std::string hTheta_e_VS_W_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_nFDpCD"];

    // Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_e histograms
    THStack* sPhi_e = OwnHistogram(new THStack("#phi_{e} stack (CD & FD)", "#phi_{e} of Outgoing Electron (no #(e) cut, CD & FD);#phi_{e} [#circ];"));

    // Phi_e histograms (no #(e) cut)
    TH1D* hPhi_e_All_e_FD = OwnHistogram(new TH1D("#phi_{e} (no #(e) cut, FD)", "#phi_{e} of Outgoing Electron (All Int., no #(e) cut, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_All_e_FD_Dir = directories.Angle_dir_map["Phi_e_All_e"];

    // Phi_e histograms (1e cut)
    TH1D* hPhi_e_1e_cut_FD = OwnHistogram(new TH1D("#phi_{e} (1e cut, FD)", "#phi_{e} of Outgoing Electron (All Int., 1e cut, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_1e_cut_FD_Dir = directories.Angle_dir_map["Phi_e_1e_cut"];

    // Phi_e histograms (1p)
    TH1D* hPhi_e_All_Int_1p_FD = OwnHistogram(new TH1D("#phi_{e} 1p (All Int., FD)", "#phi_{e} of Outgoing Electron (All Int., 1p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_QEL_1p_FD = OwnHistogram(new TH1D("#phi_{e} for 1p (QEL Only, FD)", "#phi_{e} of Outgoing Electron (QEL Only, 1p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_MEC_1p_FD = OwnHistogram(new TH1D("#phi_{e} for 1p (MEC only, FD)", "#phi_{e} of Outgoing Electron (MEC only, 1p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_RES_1p_FD = OwnHistogram(new TH1D("#phi_{e} for 1p (RES only, FD)", "#phi_{e} of Outgoing Electron (RES only, 1p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_DIS_1p_FD = OwnHistogram(new TH1D("#phi_{e} for 1p (DIS only, FD)", "#phi_{e} of Outgoing Electron (DIS only, 1p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_All_Int_1p_FD_Dir = directories.Angle_dir_map["Phi_e_1p"];
    std::string hPhi_e_QEL_1p_FD_Dir = directories.Angle_dir_map["Phi_e_1p"];
    std::string hPhi_e_MEC_1p_FD_Dir = directories.Angle_dir_map["Phi_e_1p"];
    std::string hPhi_e_RES_1p_FD_Dir = directories.Angle_dir_map["Phi_e_1p"];
    std::string hPhi_e_DIS_1p_FD_Dir = directories.Angle_dir_map["Phi_e_1p"];

    TH2D* hPhi_e_VS_P_e_1p_FD = OwnHistogram(new TH2D("#phi_{e} vs. P_{e} (All Int., 1p)", "#phi_{e} vs. P_{e} (All Int., 1p);P_{e} [GeV/c];#phi_{e} [#circ]", numTH2Dbins_Ang_Plots, 0, beamE11,
                                         numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_e_VS_W_1p_FD = OwnHistogram(new TH2D("#phi_{e} vs. W (All Int., 1p)", "#phi_{e} vs. W (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{e} [#circ]",
                                       numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_VS_P_e_1p_FD_Dir = directories.Angle_dir_map["Phi_e_1p"];
    std::string hPhi_e_VS_W_1p_FD_Dir = directories.Angle_dir_map["Phi_e_1p"];

    // Phi_e histograms (1n)
    TH1D* hPhi_e_All_Int_1n_FD = OwnHistogram(new TH1D("#phi_{e} 1n (All Int., FD)", "#phi_{e} of Outgoing Electron (All Int., 1n, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_QEL_1n_FD = OwnHistogram(new TH1D("#phi_{e} for 1n (QEL Only, FD)", "#phi_{e} of Outgoing Electron (QEL Only, 1n, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_MEC_1n_FD = OwnHistogram(new TH1D("#phi_{e} for 1n (MEC only, FD)", "#phi_{e} of Outgoing Electron (MEC only, 1n, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_RES_1n_FD = OwnHistogram(new TH1D("#phi_{e} for 1n (RES only, FD)", "#phi_{e} of Outgoing Electron (RES only, 1n, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_DIS_1n_FD = OwnHistogram(new TH1D("#phi_{e} for 1n (DIS only, FD)", "#phi_{e} of Outgoing Electron (DIS only, 1n, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_All_Int_1n_FD_Dir = directories.Angle_dir_map["Phi_e_1n"];
    std::string hPhi_e_QEL_1n_FD_Dir = directories.Angle_dir_map["Phi_e_1n"];
    std::string hPhi_e_MEC_1n_FD_Dir = directories.Angle_dir_map["Phi_e_1n"];
    std::string hPhi_e_RES_1n_FD_Dir = directories.Angle_dir_map["Phi_e_1n"];
    std::string hPhi_e_DIS_1n_FD_Dir = directories.Angle_dir_map["Phi_e_1n"];

    TH2D* hPhi_e_VS_P_e_1n_FD = OwnHistogram(new TH2D("#phi_{e} vs. P_{e} (All Int., 1n)", "#phi_{e} vs. P_{e} (All Int., 1n);P_{e} [GeV/c];#phi_{e} [#circ]", numTH2Dbins_Ang_Plots, 0, beamE11,
                                         numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_e_VS_W_1n_FD = OwnHistogram(new TH2D("#phi_{e} vs. W (All Int., 1n)", "#phi_{e} vs. W (All Int., 1n);W = #sqrt{(#omega + m_{n})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{e} [#circ]",
                                       numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_VS_P_e_1n_FD_Dir = directories.Angle_dir_map["Phi_e_1n"];
    std::string hPhi_e_VS_W_1n_FD_Dir = directories.Angle_dir_map["Phi_e_1n"];

    // Phi_e histograms (2p)
    TH1D* hPhi_e_All_Int_2p_FD = OwnHistogram(new TH1D("#phi_{e} 2p (All Int., FD)", "#phi_{e} of Outgoing Electron (All Int., 2p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_QEL_2p_FD = OwnHistogram(new TH1D("#phi_{e} for 2p (QEL Only, FD)", "#phi_{e} of Outgoing Electron (QEL Only, 2p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_MEC_2p_FD = OwnHistogram(new TH1D("#phi_{e} for 2p (MEC only, FD)", "#phi_{e} of Outgoing Electron (MEC only, 2p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_RES_2p_FD = OwnHistogram(new TH1D("#phi_{e} for 2p (RES only, FD)", "#phi_{e} of Outgoing Electron (RES only, 2p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_DIS_2p_FD = OwnHistogram(new TH1D("#phi_{e} for 2p (DIS only, FD)", "#phi_{e} of Outgoing Electron (DIS only, 2p, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_All_Int_2p_FD_Dir = directories.Angle_dir_map["Phi_e_2p"];
    std::string hPhi_e_QEL_2p_FD_Dir = directories.Angle_dir_map["Phi_e_2p"];
    std::string hPhi_e_MEC_2p_FD_Dir = directories.Angle_dir_map["Phi_e_2p"];
    std::string hPhi_e_RES_2p_FD_Dir = directories.Angle_dir_map["Phi_e_2p"];
    std::string hPhi_e_DIS_2p_FD_Dir = directories.Angle_dir_map["Phi_e_2p"];

    // Phi_e histograms (pFDpCD)
    TH1D* hPhi_e_All_Int_pFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} pFDpCD (All Int., FD)", "#phi_{e} of Outgoing Electron (All Int., pFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_QEL_pFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} for pFDpCD (QEL Only, FD)", "#phi_{e} of Outgoing Electron (QEL Only, pFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_MEC_pFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} for pFDpCD (MEC only, FD)", "#phi_{e} of Outgoing Electron (MEC only, pFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_RES_pFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} for pFDpCD (RES only, FD)", "#phi_{e} of Outgoing Electron (RES only, pFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_DIS_pFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} for pFDpCD (DIS only, FD)", "#phi_{e} of Outgoing Electron (DIS only, pFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_All_Int_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_pFDpCD"];
    std::string hPhi_e_QEL_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_pFDpCD"];
    std::string hPhi_e_MEC_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_pFDpCD"];
    std::string hPhi_e_RES_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_pFDpCD"];
    std::string hPhi_e_DIS_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_pFDpCD"];

    TH2D* hPhi_e_VS_P_e_pFDpCD_FD = OwnHistogram(new TH2D("#phi_{e} vs. P_{e} (All Int., pFDpCD)", "#phi_{e} vs. P_{e} (All Int., pFDpCD);P_{e} [GeV/c];#phi_{e} [#circ]", numTH2Dbins_Ang_Plots, 0,
                                             beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_e_VS_W_pFDpCD_FD =
        OwnHistogram(new TH2D("#phi_{e} vs. W (All Int., pFDpCD)", "#phi_{e} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{e} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_VS_P_e_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_pFDpCD"];
    std::string hPhi_e_VS_W_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_pFDpCD"];

    // Phi_e histograms (nFDpCD)
    TH1D* hPhi_e_All_Int_nFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} nFDpCD (All Int., FD)", "#phi_{e} of Outgoing Electron (All Int., nFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_QEL_nFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} for nFDpCD (QEL Only, FD)", "#phi_{e} of Outgoing Electron (QEL Only, nFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_MEC_nFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} for nFDpCD (MEC only, FD)", "#phi_{e} of Outgoing Electron (MEC only, nFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_RES_nFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} for nFDpCD (RES only, FD)", "#phi_{e} of Outgoing Electron (RES only, nFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_e_DIS_nFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{e} for nFDpCD (DIS only, FD)", "#phi_{e} of Outgoing Electron (DIS only, nFDpCD, FD);#phi_{e} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_All_Int_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_nFDpCD"];
    std::string hPhi_e_QEL_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_nFDpCD"];
    std::string hPhi_e_MEC_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_nFDpCD"];
    std::string hPhi_e_RES_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_nFDpCD"];
    std::string hPhi_e_DIS_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_nFDpCD"];

    TH2D* hPhi_e_VS_P_e_nFDpCD_FD = OwnHistogram(new TH2D("#phi_{e} vs. P_{e} (All Int., nFDpCD)", "#phi_{e} vs. P_{e} (All Int., nFDpCD);P_{e} [GeV/c];#phi_{e} [#circ]", numTH2Dbins_Ang_Plots, 0,
                                             beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_e_VS_W_nFDpCD_FD =
        OwnHistogram(new TH2D("#phi_{e} vs. W (All Int., nFDpCD)", "#phi_{e} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{e} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_e_VS_P_e_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_nFDpCD"];
    std::string hPhi_e_VS_W_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_e_nFDpCD"];

    // Theta_e vs. Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_e vs. Phi_e
    /* Theta_e vs. Phi_e histograms (no #(e) cut) */
    TH2D* hTheta_e_VS_Phi_e_All_e_FD = OwnHistogram(new TH2D("#theta_{e} vs. #phi_{e} (no #(e) cut, FD)", "#theta_{e} vs. #phi_{e}  (no #(e) cut, FD);#phi_{e} [#circ];#theta_{e} [#circ]",
                                                numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_Phi_e_All_e_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_All_e"];

    /* Theta_e vs. Phi_e histograms (1e cut) */
    TH2D* hTheta_e_VS_Phi_e_1e_cut_FD = OwnHistogram(new TH2D("#theta_{e} vs. #phi_{e} (1e cut, FD)", "#theta_{e} vs. #phi_{e} (1e cut, FD);#phi_{e} [#circ];#theta_{e} [#circ]",
                                                 numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_Phi_e_1e_cut_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_1e_cut"];

    hTheta_e_VS_Phi_e_BySliceOf_P_e_All_Int_1e_cut_FD =
        new hsPlots(ElectronMomSliceLimits, hsPlots::TH2D_TYPE, HistoList, &hTheta_e_VS_Phi_e_BySliceOf_P_e_All_Int_1e_cut_FD_HistoList, "Theta_e_VS_Phi_e_BySliceOf_P_e_All_Int_1e_cut_FD",
                    "#theta_{e} vs. #phi_{e} in slices of P_{e} (All Int., 1e cut, FD);#phi_{e} [#circ];#theta_{e} [#circ]", numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim,
                    numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD, "P_{e} [GeV/c]");
    std::string hTheta_e_VS_Phi_e_BySliceOf_P_e_All_Int_1e_cut_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_1e_cut"];

    hTheta_e_VS_Phi_e_BySliceOf_P_e_QEL_1e_cut_FD =
        new hsPlots(ElectronMomSliceLimits, hsPlots::TH2D_TYPE, HistoList, &hTheta_e_VS_Phi_e_BySliceOf_P_e_QEL_1e_cut_FD_HistoList, "Theta_e_VS_Phi_e_BySliceOf_P_e_QEL_1e_cut_FD",
                    "#theta_{e} vs. #phi_{e} in slices of P_{e} (QEL only, 1e cut, FD);#phi_{e} [#circ];#theta_{e} [#circ]", numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim,
                    numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD, "P_{e} [GeV/c]");
    std::string hTheta_e_VS_Phi_e_BySliceOf_P_e_QEL_1e_cut_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_1e_cut"];

    hTheta_e_VS_Phi_e_BySliceOf_P_e_MEC_1e_cut_FD =
        new hsPlots(ElectronMomSliceLimits, hsPlots::TH2D_TYPE, HistoList, &hTheta_e_VS_Phi_e_BySliceOf_P_e_MEC_1e_cut_FD_HistoList, "Theta_e_VS_Phi_e_BySliceOf_P_e_MEC_1e_cut_FD",
                    "#theta_{e} vs. #phi_{e} in slices of P_{e} (MEC only, 1e cut, FD);#phi_{e} [#circ];#theta_{e} [#circ]", numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim,
                    numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD, "P_{e} [GeV/c]");
    std::string hTheta_e_VS_Phi_e_BySliceOf_P_e_MEC_1e_cut_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_1e_cut"];

    hTheta_e_VS_Phi_e_BySliceOf_P_e_RES_1e_cut_FD =
        new hsPlots(ElectronMomSliceLimits, hsPlots::TH2D_TYPE, HistoList, &hTheta_e_VS_Phi_e_BySliceOf_P_e_RES_1e_cut_FD_HistoList, "Theta_e_VS_Phi_e_BySliceOf_P_e_RES_1e_cut_FD",
                    "#theta_{e} vs. #phi_{e} in slices of P_{e} (RES only, 1e cut, FD);#phi_{e} [#circ];#theta_{e} [#circ]", numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim,
                    numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD, "P_{e} [GeV/c]");
    std::string hTheta_e_VS_Phi_e_BySliceOf_P_e_RES_1e_cut_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_1e_cut"];

    hTheta_e_VS_Phi_e_BySliceOf_P_e_DIS_1e_cut_FD =
        new hsPlots(ElectronMomSliceLimits, hsPlots::TH2D_TYPE, HistoList, &hTheta_e_VS_Phi_e_BySliceOf_P_e_DIS_1e_cut_FD_HistoList, "Theta_e_VS_Phi_e_BySliceOf_P_e_DIS_1e_cut_FD",
                    "#theta_{e} vs. #phi_{e} in slices of P_{e} (DIS only, 1e cut, FD);#phi_{e} [#circ];#theta_{e} [#circ]", numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim,
                    numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD, "P_{e} [GeV/c]");
    std::string hTheta_e_VS_Phi_e_BySliceOf_P_e_DIS_1e_cut_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_1e_cut"];

    /* Theta_e vs. Phi_e histograms (1p) */
    TH2D* hTheta_e_VS_Phi_e_1p_FD = OwnHistogram(new TH2D("#theta_{e} vs. #phi_{e} (All Int., 1p, FD)", "#theta_{e} vs. #phi_{e} (All Int., 1p, FD);#phi_{e} [#circ];#theta_{e} [#circ]",
                                             numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_Phi_e_1p_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_1p"];

    /* Theta_e vs. Phi_e histograms (1n) */
    TH2D* hTheta_e_VS_Phi_e_1n_FD = OwnHistogram(new TH2D("#theta_{e} vs. #phi_{e} (All Int., 1n, FD)", "#theta_{e} vs. #phi_{e} (All Int., 1n, FD);#phi_{e} [#circ];#theta_{e} [#circ]",
                                             numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_Phi_e_1n_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_1n"];

    /* Theta_e vs. Phi_e histograms (2p) */
    TH2D* hTheta_e_VS_Phi_e_2p_FD = OwnHistogram(new TH2D("#theta_{e} vs. #phi_{e} (All Int., 2p, FD)", "#theta_{e} vs. #phi_{e} (All Int., 2p, FD);#phi_{e} [#circ];#theta_{e} [#circ]",
                                             numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_Phi_e_2p_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_2p"];

    /* Theta_e vs. Phi_e histograms (pFDpCD) */
    TH2D* hTheta_e_VS_Phi_e_pFDpCD_FD = OwnHistogram(new TH2D("#theta_{e} vs. #phi_{e} (All Int., pFDpCD, FD)", "#theta_{e} vs. #phi_{e} (All Int., pFDpCD, FD);#phi_{e} [#circ];#theta_{e} [#circ]",
                                                 numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_Phi_e_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_pFDpCD"];

    /* Theta_e vs. Phi_e histograms (nFDpCD) */
    TH2D* hTheta_e_VS_Phi_e_nFDpCD_FD = OwnHistogram(new TH2D("#theta_{e} vs. #phi_{e} (All Int., nFDpCD, FD)", "#theta_{e} vs. #phi_{e} (All Int., nFDpCD, FD);#phi_{e} [#circ];#theta_{e} [#circ]",
                                                 numTH2Dbins_Electron_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Electron_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_e_VS_Phi_e_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_e_VS_Phi_e_nFDpCD"];

    // Other angle plots ----------------------------------------------------------------------------------------------------------------------------------------------------

    // Other angle plots

    // Other angle plots (1e cut)

    // Neutron veto plots (1e cut)

    // CLAS12 neutrons and protons
    hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e} BV", "'Neutron Hits' vs. Electron Hits - Before Veto",
                "#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{n,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "01_Neutron_hits_vs_electron_hits_BV_1e_cut", -180, 180, -50, 50, 100, 100);
    hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{n,p} vs. #Delta#phi_{n,p} BV", "'Neutron Hits' vs. Proton Hits - Before Veto",
                                                        "#Delta#phi_{n,p} = #phi_{n}^{ECAL} - #phi_{p}^{ECAL} [#circ]", "#Delta#theta_{n,p} = #theta_{n}^{ECAL} - #theta_{p}^{ECAL} [#circ]",
                                                        directories.Angle_dir_map["Neutron_veto_1e_cut"], "02_Neutron_hits_vs_proton_hits_BV_1e_cut", -180, 180, -50, 50, 100, 100);

    // Electrons and redefined neutrons

    // Electrons and leading FD neutron
    hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_BV_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} BV", "'Leading Neutron Hits' vs. Electron Hits - Before Veto",
                "#Delta#phi_{LnFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{LnFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "03_LnFD_Neutron_hits_vs_electron_hits_BV_1e_cut", -180, 180, -50, 50, 100, 100);
    hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_AV_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} AV", "'Leading Neutron Hits' vs. Electron Hits - After Veto",
                "#Delta#phi_{LnFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{LnFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "04_LnFD_Neutron_hits_vs_electron_hits_AV_1e_cut", -180, 180, -50, 50, 100, 100);
    hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_Vetoed_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} Vetoed", "'Leading Neutron Hits' vs. Electron Hits - Vetoed Neutrons",
                "#Delta#phi_{LnFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{LnFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "05_LnFD_Neutron_hits_vs_electron_hits_Vetoed_1e_cut", -180, 180, -50, 50, 100, 100);

    // Electrons and all FD neutrons
    hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_BV_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} BV", "'Neutron Hits' vs. Electron Hits - Before Veto",
                "#Delta#phi_{nFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{nFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "06_nFD_Neutron_hits_vs_electron_hits_BV_1e_cut", -180, 180, -50, 50, 100, 100);
    hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_AV_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} AV", "'Neutron Hits' vs. Electron Hits - After Veto",
                "#Delta#phi_{nFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{nFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "07_nFD_Neutron_hits_vs_electron_hits_AV_1e_cut", -180, 180, -50, 50, 100, 100);
    hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_Vetoed_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} Vetoed", "'Neutron Hits' vs. Electron Hits - Vetoed Neutrons",
                "#Delta#phi_{nFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{nFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "08_nFD_Neutron_hits_vs_electron_hits_Vetoed_1e_cut", -180, 180, -50, 50, 100, 100);

    // Identified protons and redefined neutrons

    // Identified protons and leading FD neutron
    hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_BV_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} BV", "'Leading Neutron Hits' vs. Proton Hits - Before Veto",
                "#Delta#phi_{LnFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [#circ]", "#Delta#theta_{LnFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "09_LnFD_Neutron_hits_vs_proton_hits_BV_1e_cut", -180, 180, -50, 50, 100, 100);
    hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_AV_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} AV", "'Leading Neutron Hits' vs. Proton Hits - After Veto",
                "#Delta#phi_{LnFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [#circ]", "#Delta#theta_{LnFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "10_LnFD_Neutron_hits_vs_proton_hits_AV_1e_cut", -180, 180, -50, 50, 100, 100);
    hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_Vetoed_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} Vetoed", "'Leading Neutron Hits' vs. Proton Hits - Vetoed Neutrons",
                "#Delta#phi_{LnFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [#circ]", "#Delta#theta_{LnFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "11_LnFD_Neutron_hits_vs_proton_hits_Vetoed_1e_cut", -180, 180, -50, 50, 100, 100);

    // Identified protons and all FD neutrons
    hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_BV_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} BV", "'Neutron Hits' vs. Proton Hits - Before Veto",
                "#Delta#phi_{nFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [#circ]", "#Delta#theta_{nFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "12_nFD_Neutron_hits_vs_proton_hits_BV_1e_cut", -180, 180, -50, 50, 100, 100);
    hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_AV_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} AV", "'Neutron Hits' vs. Proton Hits - After Veto",
                "#Delta#phi_{nFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [#circ]", "#Delta#theta_{nFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "13_nFD_Neutron_hits_vs_proton_hits_AV_1e_cut", -180, 180, -50, 50, 100, 100);
    hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_Vetoed_1e_cut =
        hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} Vetoed", "'Neutron Hits' vs. Proton Hits - Vetoed Neutrons",
                "#Delta#phi_{nFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [#circ]", "#Delta#theta_{nFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_1e_cut"], "14_nFD_Neutron_hits_vs_proton_hits_Vetoed_1e_cut", -180, 180, -50, 50, 100, 100);

    // Other angle plots (1p)

    // Theta_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    // Theta_p (1p, FD only)
    THStack* sTheta_p_1p = OwnHistogram(new THStack("#theta_{p} (All Int., 1p, FD)", "#theta_{p} of Outgoing FD Proton (All Int., 1p, FD);#theta_{p} [#circ];"));
    TH1D* hTheta_p_All_Int_1p =
        OwnHistogram(new TH1D("#theta_{p} (All Int., 1p, FD)", "#theta_{p} of Outgoing FD Proton (All Int., 1p, FD);#theta_{p} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_p_All_Int_1p_Dir = directories.Angle_dir_map["Theta_p_1p"];

    TH2D* hTheta_p_VS_P_p_1p_FD = OwnHistogram(new TH2D("#theta_{p} vs. P_{p} (All Int., 1p, FD)", "#theta_{p} vs. P_{p} (All Int., 1p, FD);P_{p} [GeV/c];#theta_{p} [#circ]", numTH2Dbins_Ang_Plots, 0,
                                           beamE11, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH2D* hTheta_p_VS_W_1p_FD =
        OwnHistogram(new TH2D("#theta_{p} vs. W (All Int., 1p, FD)", "#theta_{p} vs. W (All Int., 1p, FD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{p} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_p_VS_P_p_1p_FD_Dir = directories.Angle_dir_map["Theta_p_1p"];
    std::string hTheta_p_VS_W_1p_FD_Dir = directories.Angle_dir_map["Theta_p_1p"];

    // Phi_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    // Phi_p (1p, FD only)
    THStack* sPhi_p_1p = OwnHistogram(new THStack("#phi_{p} (All Int., 1p, FD)", "#phi_{p} of Outgoing Proton (All Int., 1p, FD);#phi_{p} [#circ];"));
    TH1D* hPhi_p_All_Int_1p = OwnHistogram(new TH1D("#phi_{p} (All Int., 1p, FD)", "#phi_{p} of Outgoing Proton (All Int., 1p, FD);#phi_{p} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_p_All_Int_1p_Dir = directories.Angle_dir_map["Phi_p_1p"];

    // Theta_p vs. Phi_p ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_p vs. Phi_p histograms
    TH2D* hTheta_p_VS_Phi_p_1p_FD = OwnHistogram(new TH2D("#theta_{p} vs. #phi_{p} of FD proton (All Int., 1p)", "#theta_{p} vs. #phi_{p} of FD proton (All Int., 1p);#phi_{p} [#circ];#theta_{p} [#circ]",
                                             numTH2Dbins_Nucleon_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_p_VS_Phi_p_1p_FD_Dir = directories.Angle_dir_map["Theta_p_VS_Phi_p_1p"];

    // Theta_p_e_p_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    // Theta_p_e_p_p (1p, FD only)
    THStack* sTheta_p_e_p_p_1p = OwnHistogram(new THStack(
        "#theta_{#font[62]{P}_{e},#font[62]{P}_{p}} (All Int., 1p, FD)",
        "#theta_{#font[62]{P}_{e},#font[62]{P}_{p}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{p} (All Int., 1p, FD);#theta_{#font[62]{P}_{e},#font[62]{P}_{p}} [#circ];"));
    TH1D* hTheta_p_e_p_p_1p = OwnHistogram(new TH1D("#theta_{#font[62]{P}_{e},#font[62]{P}_{p}} (All Int., 1p, FD)",
                                       "#theta_{#font[62]{P}_{e},#font[62]{P}_{p}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{p} (All Int., 1p, FD);"
                                       "#theta_{#font[62]{P}_{e},#font[62]{P}_{p}} [#circ];",
                                       numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_p_e_p_p_1p_Dir = directories.Angle_dir_map["Opening_angle_1p"];

    // Theta_q_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p (1p, FD only)
    THStack* sTheta_q_p_p_1p =
        OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}_{p}} (All Int., 1p, FD)",
                    "#theta_{#font[62]{q},#font[62]{P}_{p}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{p} (All Int., 1p, FD);#theta_{#font[62]{q},#font[62]{P}_{p}} [#circ];"));
    TH1D* hTheta_q_p_p_1p = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{p}} (All Int., 1p, FD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{p}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{p} (All Int., 1p, FD);"
                                     "#theta_{#font[62]{q},#font[62]{P}_{p}} [#circ];",
                                     numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_p_1p_Dir = directories.Angle_dir_map["Opening_angle_1p"];
    std::string hTheta_q_p_p_1p_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_1p"];

    // Theta_q_p_p vs. |P_p|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p_p vs. |P_p|/|q| (1p, FD only)
    TH2D* hTheta_q_p_p_vs_p_p_q_1p = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{p}} vs. r=|P_{p}|/|q| (All Int., 1p, FD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{p}} vs. r=|#font[62]{P_{p}}|/|#font[62]{q}| (All Int., 1p, FD);r;#theta_{#font[62]{q},#font[62]{P}_{p}}",
                                              numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_p_vs_p_p_q_1p_Dir = directories.Angle_dir_map["Opening_angle_1p"];

    // Theta_q_p_p vs. |p_N|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p_p vs. |p_N|/|q| (1p, FD only)
    TH2D* hTheta_q_p_p_vs_p_N_q_1p = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{p}} vs. r=|P_{N}|/|q| (All Int., 1p, FD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{p}} vs. r=|#font[62]{P_{N}}|/|#font[62]{q}| (All Int., 1p, FD);r;#theta_{#font[62]{q},#font[62]{P}_{p}}",
                                              numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_p_vs_p_N_q_1p_Dir = directories.Angle_dir_map["Opening_angle_1p"];

    // Other angle plots (1n)

    // Theta_n (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_n (1n, FD only)
    THStack* sTheta_n_1n = OwnHistogram(new THStack("#theta_{n} (All Int., 1n, FD)", "#theta_{n} of Outgoing FD Neutron (All Int., 1n, FD);#theta_{n} [#circ];"));
    TH1D* hTheta_n_All_Int_1n =
        OwnHistogram(new TH1D("#theta_{n} (All Int., 1n, FD)", "#theta_{n} of Outgoing FD Neutron (All Int., 1n, FD);#theta_{n} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_n_All_Int_1n_Dir = directories.Angle_dir_map["Theta_n_1n"];

    TH2D* hTheta_n_VS_P_n_1n_FD = OwnHistogram(new TH2D("#theta_{n} vs. P_{n} (All Int., 1n, FD)", "#theta_{n} vs. P_{n} (All Int., 1n, FD);P_{n} [GeV/c];#theta_{n} [#circ]", numTH2Dbins_Ang_Plots, 0,
                                           beamE11, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH2D* hTheta_n_VS_W_1n_FD =
        OwnHistogram(new TH2D("#theta_{n} vs. W (All Int., 1n, FD)", "#theta_{n} vs. W (All Int., 1n, FD);W = #sqrt{(#omega + m_{n})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{n} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_n_VS_P_n_1n_FD_Dir = directories.Angle_dir_map["Theta_n_1n"];
    std::string hTheta_n_VS_W_1n_FD_Dir = directories.Angle_dir_map["Theta_n_1n"];

    // Phi_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_n (1n, FD only)
    THStack* sPhi_n_1n = OwnHistogram(new THStack("#phi_{n} (All Int., 1n, FD)", "#phi_{n} of Outgoing Neutron (All Int., 1n, FD);#phi_{n} [#circ];"));
    TH1D* hPhi_n_All_Int_1n = OwnHistogram(new TH1D("#phi_{n} (All Int., 1n, FD)", "#phi_{n} of Outgoing Neutron (All Int., 1n, FD);#phi_{n} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_n_All_Int_1n_Dir = directories.Angle_dir_map["Phi_n_1n"];

    // Theta_n vs. Phi_n ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_n vs. Phi_n histograms
    TH2D* hTheta_n_VS_Phi_n_1n_FD =
        OwnHistogram(new TH2D("#theta_{nFD} vs. #phi_{nFD} of FD neutron (All Int., 1n)", "#theta_{nFD} vs. #phi_{nFD} of FD neutron (All Int., 1n);#phi_{nFD} [#circ];#theta_{nFD} [#circ]",
                 numTH2Dbins_Nucleon_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_n_VS_Phi_n_1n_FD_Dir = directories.Angle_dir_map["Theta_n_VS_Phi_n_1n"];

    // Theta_p_e_p_n (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_p_e_p_n (1n, FD only)
    THStack* sTheta_p_e_p_n_1n = OwnHistogram(new THStack(
        "#theta_{#font[62]{P}_{e},#font[62]{P}_{n}} (All Int., 1n, FD)",
        "#theta_{#font[62]{P}_{e},#font[62]{P}_{n}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{n} (All Int., 1n, FD);#theta_{#font[62]{P}_{e},#font[62]{P}_{n}} [#circ];"));
    TH1D* hTheta_p_e_p_n_1n = OwnHistogram(new TH1D("#theta_{#font[62]{P}_{e},#font[62]{P}_{n}} (All Int., 1n, FD)",
                                       "#theta_{#font[62]{P}_{e},#font[62]{P}_{n}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{n} (All Int., 1n, FD);"
                                       "#theta_{#font[62]{P}_{e},#font[62]{P}_{n}} [#circ];",
                                       numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_p_e_p_n_1n_Dir = directories.Angle_dir_map["Opening_angle_1n"];

    // Theta_q_p_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p_n (1n, FD only)
    THStack* sTheta_q_p_n_1n =
        OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}_{n}} (All Int., 1n, FD)",
                    "#theta_{#font[62]{q},#font[62]{P}_{n}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{n} (All Int., 1n, FD);#theta_{#font[62]{q},#font[62]{P}_{n}} [#circ];"));
    TH1D* hTheta_q_p_n_1n = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{n}} (All Int., 1n, FD)",
                                     "#theta_{#font[62]{q},#font[62]{P}_{n}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{n} (All Int., 1n, FD);"
                                     "#theta_{#font[62]{q},#font[62]{P}_{n}} [#circ];",
                                     numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_n_1n_Dir = directories.Angle_dir_map["Opening_angle_1n"];
    std::string hTheta_q_p_n_1n_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_1n"];

    // Theta_q_p_n vs. |p_n|/|q| (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p_n vs. |p_n|/|q| (1n, FD only)
    TH2D* hTheta_q_p_n_vs_p_n_q_1n = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{n}} vs. r=|P_{n}|/|q| (All Int., 1n, FD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{n}} vs. r=|#font[62]{P}_{n}|/|#font[62]{q}| (All Int., 1n, FD);r;#theta_{#font[62]{q},#font[62]{P}_{n}}",
                                              numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_n_vs_p_n_q_1n_Dir = directories.Angle_dir_map["Opening_angle_1n"];

    // Theta_q_p_n vs. |p_N|/|q| (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p_n vs. |p_N|/|q| (1n, FD only)
    TH2D* hTheta_q_p_n_vs_p_N_q_1n = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{n}} vs. r=|P_{N}|/|q| (All Int., 1n, FD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{n}} vs. r=|#font[62]{P_{N}}|/|#font[62]{q}| (All Int., 1n, FD);r;#theta_{#font[62]{q},#font[62]{P}_{n}}",
                                              numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_n_vs_p_N_q_1n_Dir = directories.Angle_dir_map["Opening_angle_1n"];

    // Neutron veto plots (1n) ----------------------------------------------------------------------------------------------------------------------------------------------

    // Neutron veto plots (1n)
    hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n = hPlot2D("1n", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e} BV", "'Neutron Hits' vs. Electron Hits - Before Veto",
                                                      "#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{n,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                                                      directories.Angle_dir_map["Neutron_veto_1n"], "01_Neutron_hits_vs_electron_hits_BV_1n", -180, 180, -50, 50, 65, 65);
    hdTheta_n_e_VS_dPhi_n_e_Electrons_AV_1n = hPlot2D("1n", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e} AV", "'Neutron Hits' vs. Electron Hits - After Veto",
                                                      "#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{n,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                                                      directories.Angle_dir_map["Neutron_veto_1n"], "02_Neutron_hits_vs_electron_hits_AV_1n", -180, 180, -50, 50, 65, 65);
    hdTheta_n_e_VS_dPhi_n_e_Electrons_Vetoed_Neutrons_1n =
        hPlot2D("1n", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e}", "'Neutron Hits' vs. Electron Hits - Vetoed Neutrons", "#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ]",
                "#Delta#theta_{n,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]", directories.Angle_dir_map["Neutron_veto_1n"], "03_Neutron_hits_vs_electron_hits_Vetoed_1n", -180, 180,
                -50, 50, 65, 65);

    // Other angle plots (1e2pXy)

    // Phi of leading (p1) and recoil (p2) protons --------------------------------------------------------------------------------------------------------------------------

    // TODO: reorganize proprly

    // Phi of leading (p1) and recoil (p2) protons
    THStack* sPhi_Proton_1e2pXy = OwnHistogram(new THStack("#phi_{p} stack (1e2pXy, CD)", "#phi_{p} of Outgoing protons (1e2pXy, CD);#phi_{p} [#circ];"));
    TH1D* hPhi_p1_1e2pXy_CD = OwnHistogram(new TH1D("#phi_{p_{1}} (1e2pXy, CD)", ";#phi_{p_{1}} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_p2_1e2pXy_CD = OwnHistogram(new TH1D("#phi_{p_{2}} (1e2pXy, CD)", ";#phi_{p_{2}} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_p1_1e2pXy_CD_Dir = directories.Angle_dir_map["Phi_Proton_1e2pXy"];
    std::string hPhi_p2_1e2pXy_CD_Dir = directories.Angle_dir_map["Phi_Proton_1e2pXy"];

    // Other angle plots (2p)

    // Theta_p_e_p_tot (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------

    // Theta_p_e_p_tot (2p, CD & FD)
    THStack* sTheta_p_e_p_tot_2p =
        OwnHistogram(new THStack("#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} (All Int., 2p)",
                    "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{tot}=#font[62]{P}_{1}+#font[62]{P}_{2} (All Int., "
                    "2p);#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} [#circ];"));
    TH1D* hTheta_p_e_p_tot_2p =
        OwnHistogram(new TH1D("#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} (All Int., 2p)",
                 "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{tot}=#font[62]{P}_{1}+#font[62]{P}_{2} (All Int., 2p);"
                 "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} [#circ];",
                 numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_p_e_p_tot_2p_Dir = directories.Angle_dir_map["Opening_angle_2p"];

    // Theta_q_p (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p (2p, CD & FD)

    // Theta_q_p_tot (CD & FD)
    THStack* sTheta_q_p_tot_2p = OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}_{tot}} (All Int., 2p)",
                                             "#theta_{#font[62]{q},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{tot}=#font[62]{P}_{1}+#font[62]{P}_{2} (All "
                                             "Int., 2p);#theta_{#font[62]{q},#font[62]{P}_{tot}} [#circ];"));
    TH1D* hTheta_q_p_tot_2p =
        OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{tot}} (All Int., 2p)",
                 "#theta_{#font[62]{q},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{tot}=#font[62]{P}_{1}+#font[62]{P}_{2} (All Int., 2p);"
                 "#theta_{#font[62]{q},#font[62]{P}_{tot}} [#circ];",
                 numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_tot_2p_Dir = directories.Angle_dir_map["Opening_angle_2p"];

    // Theta_q_p_L and Theta_q_p_R (2p, CD & FD)
    THStack* sTheta_q_p_2p =
        OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}} (All Int., 2p)",
                    "#theta_{#font[62]{q},#font[62]{P}} - Opening Angle Between #font[62]{q} and #font[62]{P} (All Int., 2p);#theta_{#font[62]{q},#font[62]{P}} [#circ];"));
    TH1D* hTheta_q_p_L_2p =
        OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{1}} (All Int., 2p)",
                 "#theta_{#font[62]{q},#font[62]{P}_{1}} - Opening Angle Between #font[62]{q} and leading proton #font[62]{P}_{1} (All Int., 2p);#theta_{#font[62]{q},#font[62]{P}_{1}}",
                 numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_q_p_R_2p =
        OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{2}} (All Int., 2p)",
                 "#theta_{#font[62]{q},#font[62]{P}_{2}} - Opening Angle Between #font[62]{q} and recoil proton #font[62]{P}_{2} (All Int., 2p);#theta_{#font[62]{q},#font[62]{P}_{2}}",
                 numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_L_2p_Dir = directories.Angle_dir_map["Opening_angle_2p"];
    std::string hTheta_q_p_R_2p_Dir = directories.Angle_dir_map["Opening_angle_2p"];

    // Theta_q_p_L vs |P_L|/|q| (2p, CD & FD) -------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p_tot (CD & FD)
    TH2D* hTheta_q_p_L_vs_p_L_q_2p =
        OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{1}} vs. r_{1} (All Int., 2p)",
                 "#theta_{#font[62]{q},#font[62]{P}_{1}} vs. r_{1}=|#font[62]{P_{1}}|/|#font[62]{q}| (All Int., 2p);r_{1};#theta_{#font[62]{q},#font[62]{P}_{1}}", numTH2Dbins_Ang_Plots, 0,
                 1.05, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_L_vs_p_L_q_2p_Dir = directories.Angle_dir_map["Opening_angle_2p"];

    // Theta_p1_p2 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_p1_p2 (CD & FD)
    THStack* sTheta_p1_p2_2p = OwnHistogram(new THStack("#theta_{p_{1},p_{2}} (All Int., stack, 2p)", "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (2p);#theta_{p_{1},p_{2}} [#circ];"));
    TH1D* hTheta_p1_p2_All_Int_2p =
        OwnHistogram(new TH1D("#theta_{p_{1},p_{2}} (All Int., 2p)", "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (All Int., 2p);#theta_{p_{1},p_{2}} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_p1_p2_QEL_2p =
        OwnHistogram(new TH1D("#theta_{p_{1},p_{2}} (QEL only, 2p)", "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (QEL only, 2p);#theta_{p_{1},p_{2}} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_p1_p2_MEC_2p =
        OwnHistogram(new TH1D("#theta_{p_{1},p_{2}} (MEC only, 2p)", "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (MEC only, 2p);#theta_{p_{1},p_{2}} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_p1_p2_RES_2p =
        OwnHistogram(new TH1D("#theta_{p_{1},p_{2}} (RES only, 2p)", "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (RES only, 2p);#theta_{p_{1},p_{2}} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_p1_p2_DIS_2p =
        OwnHistogram(new TH1D("#theta_{p_{1},p_{2}} (DIS only, 2p)", "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (DIS only, 2p);#theta_{p_{1},p_{2}} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    std::string sTheta_p1_p2_2p_Dir = directories.Angle_dir_map["Opening_angle_2p"];
    std::string hTheta_p1_p2_All_Int_2p_Dir = directories.Angle_dir_map["Opening_angle_by_interaction_2p"];
    std::string hTheta_p1_p2_QEL_2p_Dir = directories.Angle_dir_map["Opening_angle_by_interaction_2p"];
    std::string hTheta_p1_p2_MEC_2p_Dir = directories.Angle_dir_map["Opening_angle_by_interaction_2p"];
    std::string hTheta_p1_p2_RES_2p_Dir = directories.Angle_dir_map["Opening_angle_by_interaction_2p"];
    std::string hTheta_p1_p2_DIS_2p_Dir = directories.Angle_dir_map["Opening_angle_by_interaction_2p"];

    // Theta_p1_p2 vs. W (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    // Theta_p1_p2 vs. W (CD & FD)
    TH2D* hTheta_p1_p2_vs_W_2p = OwnHistogram(new TH2D("#theta_{p_{1},p_{2}} vs. W (All Int., 2p)",
                                          "#theta_{p_{1},p_{2}} vs. W (All Int., 2p);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{p_{1},p_{2}} [#circ];",
                                          numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_p1_p2_vs_W_2p_Dir = directories.Angle_dir_map["Opening_angle_2p"];

    // Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------

    // Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (CD & FD)
    TH2D* hTheta_p1_vs_theta_p2_for_Theta_p1_p2_20_2p = OwnHistogram(new TH2D("#theta_{p_{1}} vs. #theta_{p_{1}} for #theta_{p_{1},p_{2}}<20#circ (All Int., 2p)",
                                                                 "#theta_{p_{1}} vs. #theta_{p_{2}} for #theta_{p_{1},p_{2}}<20#circ (All Int., 2p);#theta_{p_{2}} [#circ];"
                                                                 "#theta_{p_{1}} [#circ];",
                                                                 numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50));
    std::string hTheta_p1_vs_theta_p2_for_Theta_p1_p2_20_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    // Theta_pFD_vs_Theta_pCD for Theta_pFD_pCD < 20 (2p, CD & FD) ----------------------------------------------------------------------------------------------------------

    // Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (CD & FD)
    TH2D* hTheta_pFD_vs_Theta_pCD_for_Theta_pFD_pCD_20_2p = OwnHistogram(new TH2D("#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)",
                                                                     "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p);#theta_{pCD} [#circ];"
                                                                     "#theta_{pFD} [#circ];",
                                                                     numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50));
    std::string hTheta_pFD_vs_Theta_pCD_for_Theta_pFD_pCD_20_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    // dPhi_p1_p2 for Theta_p1_p2 < 20 (2p, CD & FD) ------------------------------------------------------------------------------------------------------------------------

    // dPhi_p1_p2 for Theta_p1_p2 < 20 (CD & FD)
    TH1D* hdPhi_p1_p2_for_Theta_p1_p2_20_2p = OwnHistogram(new TH1D("#Delta#phi for #theta_{p_{1},p_{2}}<20#circ (All Int., 2p)",
                                                       "#Delta#phi for #theta_{p_{1},p_{2}}<20#circ (All Int., 2p);"
                                                       "#Delta#phi = #phi_{p,1} - #phi_{p,2} [#circ];",
                                                       numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hdPhi_p1_p2_for_Theta_p1_p2_20_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    TH1D* hdPhi_p1_p2_for_Theta_p1_p2_20_ZOOMIN_2p = OwnHistogram(new TH1D("#Delta#phi for #theta_{p_{1},p_{2}}<20#circ - ZOOMIN (All Int., 2p)",
                                                              "#Delta#phi for #theta_{p_{1},p_{2}}<20#circ - ZOOMIN (All Int., 2p);"
                                                              "#Delta#phi = #phi_{p,1} - #phi_{p,2} [#circ];",
                                                              numTH1Dbins_Ang_Plots, -25, 25));
    std::string hdPhi_p1_p2_for_Theta_p1_p2_20_ZOOMIN_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    // dPhi_pFD_pCD for Theta_pFD_pCD < 20 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------

    // dPhi_p1_p2 for Theta_pFD_pCD < 20 (CD & FD)
    TH1D* hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_2p = OwnHistogram(new TH1D("#Delta#phi for #theta_{pFD,pCD}<20#circ (All Int., 2p)",
                                                           "#Delta#phi for #theta_{pFD,pCD}<20#circ (All Int., 2p);"
                                                           "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                           50, Phi_llim, Phi_ulim));
    std::string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    TH1D* hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_2p = OwnHistogram(new TH1D("#Delta#phi for #theta_{pFD,pCD}<20#circ - ZOOMIN (All Int., 2p)",
                                                                  "#Delta#phi for #theta_{pFD,pCD}<20#circ - ZOOMIN (All Int., 2p);"
                                                                  "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                  50, -40, 40));
    std::string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    // Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (2p, CD & FD) -------------------------------------------------------------------------------------------------------------

    // Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (CD & FD)
    TH2D* hTheta_p1_vs_theta_p2_forall_Theta_p1_p2_2p = OwnHistogram(new TH2D("#theta_{p_{1}} vs. #theta_{p_{1}} #forall#theta_{p_{1},p_{2}} (All Int., 2p)",
                                                                 "#theta_{p_{1}} vs. #theta_{p_{2}} for every #theta_{p_{1},p_{2}} (All Int., 2p);#theta_{p_{2}} [#circ];"
                                                                 "#theta_{p_{1}} [#circ];",
                                                                 numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50));
    std::string hTheta_p1_vs_theta_p2_forall_Theta_p1_p2_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    // Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (2p, CD & FD) ---------------------------------------------------------------------------------------------------------

    // Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (CD & FD)
    TH2D* hTheta_pFD_vs_Theta_pCD_forall_Theta_pFD_pCD_2p = OwnHistogram(new TH2D("#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p)",
                                                                     "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p);#theta_{pCD} [#circ];"
                                                                     "#theta_{pFD} [#circ];",
                                                                     numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50));
    std::string hTheta_pFD_vs_Theta_pCD_forall_Theta_pFD_pCD_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    // dPhi_p1_p2 for every Theta_p1_p2 (2p, CD & FD) -----------------------------------------------------------------------------------------------------------------------

    // dPhi_p1_p2 for every Theta_p1_p2 (CD & FD)
    TH1D* hdPhi_p1_p2_for_all_Theta_p1_p2_2p = OwnHistogram(new TH1D("#Delta#phi #forall#theta_{p_{1},p_{2}} (All Int., 2p)",
                                                        "#Delta#phi for every #theta_{p_{1},p_{2}} (All Int., 2p);"
                                                        "#Delta#phi = #phi_{p,1} - #phi_{p,2} [#circ];",
                                                        50, Phi_llim, Phi_ulim));
    std::string hdPhi_p1_p2_for_all_Theta_p1_p2_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    TH1D* hdPhi_p1_p2_for_all_Theta_p1_p2_ZOOMIN_2p = OwnHistogram(new TH1D("#Delta#phi #forall#theta_{p_{1},p_{2}} - ZOOMIN (All Int., 2p)",
                                                               "#Delta#phi for every #theta_{p_{1},p_{2}} - ZOOMIN(All Int., 2p);"
                                                               "#Delta#phi = #phi_{p,1} - #phi_{p,2} [#circ];",
                                                               50, -40, 40));
    std::string hdPhi_p1_p2_for_all_Theta_p1_p2_ZOOMIN_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    TH1D* hdPhi_p1_p2_for_small_dTheta_2p = OwnHistogram(new TH1D("#Delta#phi for small #Delta#theta_{1/2} (All Int., 2p)",
                                                     "#Delta#phi for small #Delta#theta_{1/2} = |#theta_{1/2}-40#circ|;"
                                                     "#Delta#phi = #phi_{p,1} - #phi_{p,2} [#circ];",
                                                     50, Phi_llim, Phi_ulim));
    std::string hdPhi_p1_p2_for_small_dTheta_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    TH1D* hdPhi_p1_p2_for_small_dTheta_ZOOMIN_2p = OwnHistogram(new TH1D("#Delta#phi for small #Delta#theta_{1/2} - ZOOMIN (All Int., 2p)",
                                                            "#Delta#phi for small #Delta#theta_{1/2} = |#theta_{1/2}-40#circ| - ZOOMIN;"
                                                            "#Delta#phi = #phi_{p,1} - #phi_{p,2} [#circ];",
                                                            50, -40, 40));
    std::string hdPhi_p1_p2_for_small_dTheta_ZOOMIN_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    // dPhi_pFD_pCD for every Theta_pFD_pCD (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------

    // dPhi_pFD_pCD for every Theta_pFD_pCD (CD & FD)
    TH1D* hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_2p = OwnHistogram(new TH1D("#Delta#phi for #theta_{pFD,pCD} (All Int., 2p)",
                                                            "#Delta#phi for #theta_{pFD,pCD} (All Int., 2p);"
                                                            "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                            50, Phi_llim, Phi_ulim));
    std::string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    TH1D* hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_2p = OwnHistogram(new TH1D("#Delta#phi for #theta_{pFD,pCD} - ZOOMIN (All Int., 2p)",
                                                                   "#Delta#phi for #theta_{pFD,pCD} - ZOOMIN (All Int., 2p);"
                                                                   "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                   50, -40, 40));
    std::string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    TH1D* hdPhi_pFD_pCD_for_small_dTheta_2p = OwnHistogram(new TH1D("#Delta#phi for small #Delta#theta_{pFD/pCD} (All Int., 2p)",
                                                       "#Delta#phi for small #Delta#theta_{pFD/pCD} = |#theta_{pFD/pCD}-40#circ|;"
                                                       "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                       50, Phi_llim, Phi_ulim));
    std::string hdPhi_pFD_pCD_for_small_dTheta_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    TH1D* hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_2p = OwnHistogram(new TH1D("#Delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN (All Int., 2p)",
                                                              "#Delta#phi for small #Delta#theta_{pFD/pCD} = |#theta_{pFD/pCD}-40#circ| - ZOOMIN;"
                                                              "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                              50, -40, 40));
    std::string hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_2p_Dir = directories.Angle_dir_map["Double_detection_2p"];

    // Ghost tracks handling (2p, CD only) ----------------------------------------------------------------------------------------------------------------------------------

    // Theta_p1_p2 vs. TOF1-TOF2 plots (2p)
    hTheta_p1_p2_VS_ToF1_ToF2_AC_2p =
        hPlot2D("2p", "CD-CTOF", "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC", "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC", "#theta_{p_{1},p_{2}} [#circ]", "ToF_{1}-ToF_{2} [ns]",
                directories.Angle_dir_map["CToF_hits_2p"], "01_Theta_p1_p2_VS_ToF1-ToF2_AC_2p", 0.0, 180, -3, 3, numTH2Dbins_Ang_Plots, numTH2Dbins_Ang_Plots);

    // Theta_p1_p2 vs. position1-position2 plots (2p)
    hTheta_p1_p2_VS_Pos1_Pos2_AC_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC", "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC",
                                              "#theta_{p_{1},p_{2}} [#circ]", "Position_{1}-Position_{2} [cm]", directories.Angle_dir_map["CToF_hits_2p"],
                                              "02_Theta_p1_p2_VS_Pos1-Pos2_AC_2p", 0.0, 180, 0, 100, numTH2Dbins_Ang_Plots, numTH2Dbins_Ang_Plots);

    // Other angle plots (pFDpCD)

    // Nucleons' angles plots (pFDpCD)

    // Theta_pFD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_pFD histograms
    THStack* sTheta_pFD_pFDpCD_FD = OwnHistogram(new THStack("#theta_{pFD} (pFDpCD)", "#theta_{pFD} of FD proton (pFDpCD);#theta_{pFD} [#circ];"));

    TH1D* hTheta_pFD_All_Int_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{pFD} (All Int., pFDpCD)", "#theta_{pFD} of FD proton (All Int., pFDpCD);#theta_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_pFD_QEL_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{pFD} (QEL Only, pFDpCD)", "#theta_{pFD} of FD proton (QEL Only, pFDpCD);#theta_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_pFD_MEC_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{pFD} (MEC only, pFDpCD)", "#theta_{pFD} of FD proton (MEC only, pFDpCD);#theta_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_pFD_RES_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{pFD} (RES only, pFDpCD)", "#theta_{pFD} of FD proton (RES only, pFDpCD);#theta_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_pFD_DIS_pFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{pFD} (DIS only, pFDpCD)", "#theta_{pFD} of FD proton (DIS only, pFDpCD);#theta_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_pFD_All_Int_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_pFD_pFDpCD"];
    std::string hTheta_pFD_QEL_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_pFD_pFDpCD"];
    std::string hTheta_pFD_MEC_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_pFD_pFDpCD"];
    std::string hTheta_pFD_RES_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_pFD_pFDpCD"];
    std::string hTheta_pFD_DIS_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_pFD_pFDpCD"];

    TH2D* hTheta_pFD_VS_P_pFD_pFDpCD_FD = OwnHistogram(new TH2D("#theta_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD)", "#theta_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD);P_{pFD} [GeV/c];#theta_{pFD} [#circ]",
                                                   numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH2D* hTheta_pFD_VS_W_pFDpCD_FD = OwnHistogram(new TH2D("#theta_{pFD} vs. W (All Int., pFDpCD, FD)",
                                               "#theta_{pFD} vs. W (All Int., pFDpCD, FD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{pFD} [#circ]",
                                               numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_pFD_VS_P_pFD_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_pFD_pFDpCD"];
    std::string hTheta_pFD_VS_W_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_pFD_pFDpCD"];

    // Phi_pFD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_pFD histograms
    THStack* sPhi_pFD_pFDpCD_FD = OwnHistogram(new THStack("#phi_{pFD} (pFDpCD)", "#phi_{pFD} of FD proton (pFDpCD);#phi_{pFD} [#circ];"));

    TH1D* hPhi_pFD_All_Int_pFDpCD_FD = OwnHistogram(new TH1D("#phi_{pFD} (All Int., pFDpCD)", "#phi_{pFD} of FD proton (All Int., pFDpCD);#phi_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pFD_QEL_pFDpCD_FD = OwnHistogram(new TH1D("#phi_{pFD} (QEL Only, pFDpCD)", "#phi_{pFD} of FD proton (QEL Only, pFDpCD);#phi_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pFD_MEC_pFDpCD_FD = OwnHistogram(new TH1D("#phi_{pFD} (MEC only, pFDpCD)", "#phi_{pFD} of FD proton (MEC only, pFDpCD);#phi_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pFD_RES_pFDpCD_FD = OwnHistogram(new TH1D("#phi_{pFD} (RES only, pFDpCD)", "#phi_{pFD} of FD proton (RES only, pFDpCD);#phi_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pFD_DIS_pFDpCD_FD = OwnHistogram(new TH1D("#phi_{pFD} (DIS only, pFDpCD)", "#phi_{pFD} of FD proton (DIS only, pFDpCD);#phi_{pFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_pFD_All_Int_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_pFD_pFDpCD"];
    std::string hPhi_pFD_QEL_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_pFD_pFDpCD"];
    std::string hPhi_pFD_MEC_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_pFD_pFDpCD"];
    std::string hPhi_pFD_RES_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_pFD_pFDpCD"];
    std::string hPhi_pFD_DIS_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_pFD_pFDpCD"];

    TH2D* hPhi_pFD_VS_P_pFD_pFDpCD_FD = OwnHistogram(new TH2D("#phi_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD)", "#phi_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD);P_{pFD} [GeV/c];#phi_{pFD} [#circ]",
                                                 numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_pFD_VS_W_pFDpCD_FD = OwnHistogram(new TH2D("#phi_{pFD} vs. W (All Int., pFDpCD, FD)",
                                             "#phi_{pFD} vs. W (All Int., pFDpCD, FD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                             "#phi_{pFD} [#circ]",
                                             numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_pFD_VS_P_pFD_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_pFD_pFDpCD"];
    std::string hPhi_pFD_VS_W_pFDpCD_FD_Dir = directories.Angle_dir_map["Phi_pFD_pFDpCD"];

    // Theta_pFD vs. Phi_pFD ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_pFD vs. Phi_pFD histograms
    TH2D* hTheta_pFD_VS_Phi_pFD_pFDpCD_FD =
        OwnHistogram(new TH2D("#theta_{pFD} vs. #phi_{pFD} of FD proton (All Int., pFDpCD)", "#theta_{pFD} vs. #phi_{pFD} of FD proton (All Int., pFDpCD);#phi_{pFD} [#circ];#theta_{pFD} [#circ]",
                 numTH2Dbins_Nucleon_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_pFD_VS_Phi_pFD_pFDpCD_FD_Dir = directories.Angle_dir_map["Theta_pFD_VS_Phi_pFD_pFDpCD"];

    // Theta_pCD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_pCD histograms
    THStack* sTheta_pCD_pFDpCD_CD = OwnHistogram(new THStack("#theta_{pCD} (pFDpCD)", "#theta_{pCD} of CD proton (pFDpCD);#theta_{pCD} [#circ];"));

    TH1D* hTheta_pCD_All_Int_pFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (All Int., pFDpCD)", "#theta_{pCD} of CD proton (All Int., pFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_pCD_QEL_pFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (QEL Only, pFDpCD)", "#theta_{pCD} of CD proton (QEL Only, pFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_pCD_MEC_pFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (MEC only, pFDpCD)", "#theta_{pCD} of CD proton (MEC only, pFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_pCD_RES_pFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (RES only, pFDpCD)", "#theta_{pCD} of CD proton (RES only, pFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_pCD_DIS_pFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (DIS only, pFDpCD)", "#theta_{pCD} of CD proton (DIS only, pFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    std::string hTheta_pCD_All_Int_pFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_pFDpCD"];
    std::string hTheta_pCD_QEL_pFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_pFDpCD"];
    std::string hTheta_pCD_MEC_pFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_pFDpCD"];
    std::string hTheta_pCD_RES_pFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_pFDpCD"];
    std::string hTheta_pCD_DIS_pFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_pFDpCD"];

    TH2D* hTheta_pCD_VS_P_pCD_pFDpCD_CD = OwnHistogram(new TH2D("#theta_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)", "#theta_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD);P_{pCD} [GeV/c];#theta_{pCD} [#circ]",
                                                   numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, 30, 155));
    TH2D* hTheta_pCD_VS_W_pFDpCD_CD = OwnHistogram(new TH2D("#theta_{pCD} vs. W (All Int., pFDpCD, CD)",
                                               "#theta_{pCD} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{pCD} [#circ]",
                                               numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 30, 155));
    std::string hTheta_pCD_VS_P_pCD_pFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_pFDpCD"];
    std::string hTheta_pCD_VS_W_pFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_pFDpCD"];

    // Phi_pCD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_pCD histograms
    THStack* sPhi_pCD_pFDpCD_CD = OwnHistogram(new THStack("#phi_{pCD} (pFDpCD)", "#phi_{pCD} of CD proton (pFDpCD);#phi_{pCD} [#circ];"));

    TH1D* hPhi_pCD_All_Int_pFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (All Int., pFDpCD)", "#phi_{pCD} of CD proton (All Int., pFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pCD_QEL_pFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (QEL Only, pFDpCD)", "#phi_{pCD} of CD proton (QEL Only, pFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pCD_MEC_pFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (MEC only, pFDpCD)", "#phi_{pCD} of CD proton (MEC only, pFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pCD_RES_pFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (RES only, pFDpCD)", "#phi_{pCD} of CD proton (RES only, pFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pCD_DIS_pFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (DIS only, pFDpCD)", "#phi_{pCD} of CD proton (DIS only, pFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_pCD_All_Int_pFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_pFDpCD"];
    std::string hPhi_pCD_QEL_pFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_pFDpCD"];
    std::string hPhi_pCD_MEC_pFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_pFDpCD"];
    std::string hPhi_pCD_RES_pFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_pFDpCD"];
    std::string hPhi_pCD_DIS_pFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_pFDpCD"];

    TH2D* hPhi_pCD_VS_P_pCD_pFDpCD_CD = OwnHistogram(new TH2D("#phi_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)", "#phi_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD);P_{pCD} [GeV/c];#phi_{pCD} [#circ]",
                                                 numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_pCD_VS_W_pFDpCD_CD =
        OwnHistogram(new TH2D("#phi_{pCD} vs. W (All Int., pFDpCD, CD)", "#phi_{pCD} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{pCD} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_pCD_VS_P_pCD_pFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_pFDpCD"];
    std::string hPhi_pCD_VS_W_pFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_pFDpCD"];

    // Theta_pCD vs. Phi_pCD ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_pCD vs. Phi_pCD histograms
    TH2D* hTheta_pCD_VS_Phi_pCD_pFDpCD_CD =
        OwnHistogram(new TH2D("#theta_{pCD} vs. #phi_{pCD} of CD proton (All Int., pFDpCD)", "#theta_{pCD} vs. #phi_{pCD} of CD proton (All Int., pFDpCD);#phi_{pCD} [#circ];#theta_{pCD} [#circ]",
                 numTH2Dbins_Nucleon_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, 30, 155));
    std::string hTheta_pCD_VS_Phi_pCD_pFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_VS_Phi_pCD_pFDpCD"];

    // Theta_tot ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_tot histograms
    THStack* sTheta_tot_pFDpCD = OwnHistogram(new THStack("#theta_{tot} (pFDpCD)", "#theta_{tot} of total 3-momentum (pFDpCD);#theta_{tot} [#circ];"));

    TH1D* hTheta_tot_All_Int_pFDpCD = OwnHistogram(new TH1D("#theta_{tot} (All Int., pFDpCD)", "#theta_{tot} of total 3-momentum (All Int., pFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                               Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_tot_QEL_pFDpCD = OwnHistogram(new TH1D("#theta_{tot} (QEL Only, pFDpCD)", "#theta_{tot} of total 3-momentum (QEL Only, pFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                           Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_tot_MEC_pFDpCD = OwnHistogram(new TH1D("#theta_{tot} (MEC only, pFDpCD)", "#theta_{tot} of total 3-momentum (MEC only, pFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                           Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_tot_RES_pFDpCD = OwnHistogram(new TH1D("#theta_{tot} (RES only, pFDpCD)", "#theta_{tot} of total 3-momentum (RES only, pFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                           Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_tot_DIS_pFDpCD = OwnHistogram(new TH1D("#theta_{tot} (DIS only, pFDpCD)", "#theta_{tot} of total 3-momentum (DIS only, pFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                           Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_tot_All_Int_pFDpCD_Dir = directories.Angle_dir_map["Theta_tot_pFDpCD"];
    std::string hTheta_tot_QEL_pFDpCD_Dir = directories.Angle_dir_map["Theta_tot_pFDpCD"];
    std::string hTheta_tot_MEC_pFDpCD_Dir = directories.Angle_dir_map["Theta_tot_pFDpCD"];
    std::string hTheta_tot_RES_pFDpCD_Dir = directories.Angle_dir_map["Theta_tot_pFDpCD"];
    std::string hTheta_tot_DIS_pFDpCD_Dir = directories.Angle_dir_map["Theta_tot_pFDpCD"];

    TH2D* hTheta_tot_VS_P_tot_pFDpCD = OwnHistogram(new TH2D("#theta_{tot} vs. P_{tot} (All Int., pFDpCD, CD)", "#theta_{tot} vs. P_{tot} (All Int., pFDpCD, CD);P_{tot} [GeV/c];#theta_{tot} [#circ]",
                                                numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH2D* hTheta_tot_VS_W_pFDpCD = OwnHistogram(new TH2D("#theta_{tot} vs. W (All Int., pFDpCD, CD)",
                                            "#theta_{tot} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{tot} [#circ]",
                                            numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_tot_VS_P_tot_pFDpCD_Dir = directories.Angle_dir_map["Theta_tot_pFDpCD"];
    std::string hTheta_tot_VS_W_pFDpCD_Dir = directories.Angle_dir_map["Theta_tot_pFDpCD"];

    // Phi_tot --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_tot histograms
    THStack* sPhi_tot_pFDpCD = OwnHistogram(new THStack("#phi_{tot} (pFDpCD)", "#phi_{tot} of total 3-momentum (pFDpCD);#phi_{tot} [#circ];"));

    TH1D* hPhi_tot_All_Int_pFDpCD =
        OwnHistogram(new TH1D("#phi_{tot} (All Int., pFDpCD)", "#phi_{tot} of total 3-momentum (All Int., pFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_tot_QEL_pFDpCD = OwnHistogram(new TH1D("#phi_{tot} (QEL Only, pFDpCD)", "#phi_{tot} of total 3-momentum (QEL Only, pFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_tot_MEC_pFDpCD = OwnHistogram(new TH1D("#phi_{tot} (MEC only, pFDpCD)", "#phi_{tot} of total 3-momentum (MEC only, pFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_tot_RES_pFDpCD = OwnHistogram(new TH1D("#phi_{tot} (RES only, pFDpCD)", "#phi_{tot} of total 3-momentum (RES only, pFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_tot_DIS_pFDpCD = OwnHistogram(new TH1D("#phi_{tot} (DIS only, pFDpCD)", "#phi_{tot} of total 3-momentum (DIS only, pFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_tot_All_Int_pFDpCD_Dir = directories.Angle_dir_map["Phi_tot_pFDpCD"];
    std::string hPhi_tot_QEL_pFDpCD_Dir = directories.Angle_dir_map["Phi_tot_pFDpCD"];
    std::string hPhi_tot_MEC_pFDpCD_Dir = directories.Angle_dir_map["Phi_tot_pFDpCD"];
    std::string hPhi_tot_RES_pFDpCD_Dir = directories.Angle_dir_map["Phi_tot_pFDpCD"];
    std::string hPhi_tot_DIS_pFDpCD_Dir = directories.Angle_dir_map["Phi_tot_pFDpCD"];

    TH2D* hPhi_tot_VS_P_tot_pFDpCD = OwnHistogram(new TH2D("#phi_{tot} vs. P_{tot} (All Int., pFDpCD, CD)", "#phi_{tot} vs. P_{tot} (All Int., pFDpCD, CD);P_{tot} [GeV/c];#phi_{tot} [#circ]",
                                              numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_tot_VS_W_pFDpCD =
        OwnHistogram(new TH2D("#phi_{tot} vs. W (All Int., pFDpCD, CD)", "#phi_{tot} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{tot} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_tot_VS_P_tot_pFDpCD_Dir = directories.Angle_dir_map["Phi_tot_pFDpCD"];
    std::string hPhi_tot_VS_W_pFDpCD_Dir = directories.Angle_dir_map["Phi_tot_pFDpCD"];

    // Theta_tot vs. Phi_tot ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_tot vs. Phi_tot histograms
    TH2D* hTheta_tot_VS_Phi_tot_pFDpCD = OwnHistogram(new TH2D("#theta_{tot} vs. #phi_{tot} of total 3-momentum (All Int., pFDpCD)",
                                                  "#theta_{tot} vs. #phi_{tot} of total 3-momentum (All Int., pFDpCD);#phi_{tot} [#circ];#theta_{tot} [#circ]", numTH2Dbins_Nucleon_Ang_Plots,
                                                  Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_tot_VS_Phi_tot_pFDpCD_Dir = directories.Angle_dir_map["Theta_tot_VS_Phi_tot_pFDpCD"];

    // Theta_rel ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_rel histograms
    THStack* sTheta_rel_pFDpCD = OwnHistogram(new THStack("#theta_{rel} (pFDpCD)", "#theta_{rel} of relative 3-momentum (pFDpCD);#theta_{rel} [#circ];"));

    TH1D* hTheta_rel_All_Int_pFDpCD =
        OwnHistogram(new TH1D("#theta_{rel} (All Int., pFDpCD)", "#theta_{rel} of relative 3-momentum (All Int., pFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_rel_QEL_pFDpCD = OwnHistogram(new TH1D("#theta_{rel} (QEL Only, pFDpCD)", "#theta_{rel} of relative 3-momentum (QEL Only, pFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_rel_MEC_pFDpCD = OwnHistogram(new TH1D("#theta_{rel} (MEC only, pFDpCD)", "#theta_{rel} of relative 3-momentum (MEC only, pFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_rel_RES_pFDpCD = OwnHistogram(new TH1D("#theta_{rel} (RES only, pFDpCD)", "#theta_{rel} of relative 3-momentum (RES only, pFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_rel_DIS_pFDpCD = OwnHistogram(new TH1D("#theta_{rel} (DIS only, pFDpCD)", "#theta_{rel} of relative 3-momentum (DIS only, pFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    std::string hTheta_rel_All_Int_pFDpCD_Dir = directories.Angle_dir_map["Theta_rel_pFDpCD"];
    std::string hTheta_rel_QEL_pFDpCD_Dir = directories.Angle_dir_map["Theta_rel_pFDpCD"];
    std::string hTheta_rel_MEC_pFDpCD_Dir = directories.Angle_dir_map["Theta_rel_pFDpCD"];
    std::string hTheta_rel_RES_pFDpCD_Dir = directories.Angle_dir_map["Theta_rel_pFDpCD"];
    std::string hTheta_rel_DIS_pFDpCD_Dir = directories.Angle_dir_map["Theta_rel_pFDpCD"];

    TH2D* hTheta_rel_VS_P_rel_pFDpCD = OwnHistogram(new TH2D("#theta_{rel} vs. P_{rel} (All Int., pFDpCD, CD)", "#theta_{rel} vs. P_{rel} (All Int., pFDpCD, CD);P_{rel} [GeV/c];#theta_{rel} [#circ]",
                                                numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, 30, 155));
    TH2D* hTheta_rel_VS_W_pFDpCD = OwnHistogram(new TH2D("#theta_{rel} vs. W (All Int., pFDpCD, CD)",
                                            "#theta_{rel} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{rel} [#circ]",
                                            numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 30, 155));
    std::string hTheta_rel_VS_P_rel_pFDpCD_Dir = directories.Angle_dir_map["Theta_rel_pFDpCD"];
    std::string hTheta_rel_VS_W_pFDpCD_Dir = directories.Angle_dir_map["Theta_rel_pFDpCD"];

    // Phi_rel --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_rel histograms
    THStack* sPhi_rel_pFDpCD = OwnHistogram(new THStack("#phi_{rel} (pFDpCD)", "#phi_{rel} of relative 3-momentum (pFDpCD);#phi_{rel} [#circ];"));

    TH1D* hPhi_rel_All_Int_pFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (All Int., pFDpCD)", "#phi_{rel} of relative 3-momentum (All Int., pFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_rel_QEL_pFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (QEL Only, pFDpCD)", "#phi_{rel} of relative 3-momentum (QEL Only, pFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_rel_MEC_pFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (MEC only, pFDpCD)", "#phi_{rel} of relative 3-momentum (MEC only, pFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_rel_RES_pFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (RES only, pFDpCD)", "#phi_{rel} of relative 3-momentum (RES only, pFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_rel_DIS_pFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (DIS only, pFDpCD)", "#phi_{rel} of relative 3-momentum (DIS only, pFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_rel_All_Int_pFDpCD_Dir = directories.Angle_dir_map["Phi_rel_pFDpCD"];
    std::string hPhi_rel_QEL_pFDpCD_Dir = directories.Angle_dir_map["Phi_rel_pFDpCD"];
    std::string hPhi_rel_MEC_pFDpCD_Dir = directories.Angle_dir_map["Phi_rel_pFDpCD"];
    std::string hPhi_rel_RES_pFDpCD_Dir = directories.Angle_dir_map["Phi_rel_pFDpCD"];
    std::string hPhi_rel_DIS_pFDpCD_Dir = directories.Angle_dir_map["Phi_rel_pFDpCD"];

    TH2D* hPhi_rel_VS_P_rel_pFDpCD = OwnHistogram(new TH2D("#phi_{rel} vs. P_{rel} (All Int., pFDpCD, CD)", "#phi_{rel} vs. P_{rel} (All Int., pFDpCD, CD);P_{rel} [GeV/c];#phi_{rel} [#circ]",
                                              numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_rel_VS_W_pFDpCD =
        OwnHistogram(new TH2D("#phi_{rel} vs. W (All Int., pFDpCD, CD)", "#phi_{rel} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{rel} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_rel_VS_P_rel_pFDpCD_Dir = directories.Angle_dir_map["Phi_rel_pFDpCD"];
    std::string hPhi_rel_VS_W_pFDpCD_Dir = directories.Angle_dir_map["Phi_rel_pFDpCD"];

    // Theta_rel vs. Phi_rel ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_rel vs. Phi_rel histograms
    TH2D* hTheta_rel_VS_Phi_rel_pFDpCD = OwnHistogram(new TH2D("#theta_{rel} vs. #phi_{rel} of relative 3-momentum (All Int., pFDpCD)",
                                                  "#theta_{rel} vs. #phi_{rel} of relative 3-momentum (All Int., pFDpCD);#phi_{rel} [#circ];#theta_{rel} [#circ]",
                                                  numTH2Dbins_Nucleon_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, 30, 155));
    std::string hTheta_rel_VS_Phi_rel_pFDpCD_Dir = directories.Angle_dir_map["Theta_rel_VS_Phi_rel_pFDpCD"];

    // Theta_p_e_p_tot (pFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------------

    // Theta_p_e_p_tot (pFDpCD, CD & FD)
    THStack* sTheta_p_e_p_tot_pFDpCD =
        OwnHistogram(new THStack("#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} (All Int., pFDpCD)",
                    "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{tot}=#font[62]{P}_{pFD}+#font[62]{P}_{pCD} "
                    "(All Int., pFDpCD);#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} [#circ];"));
    TH1D* hTheta_p_e_p_tot_pFDpCD =
        OwnHistogram(new TH1D("#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} (All Int., pFDpCD)",
                 "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{tot}=#font[62]{P}_{pFD}+#font[62]{P}_{pCD} "
                 "(All Int., pFDpCD);#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} [#circ];",
                 numTH1Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_p_e_p_tot_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_p_e_p_tot vs. W (pFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------

    // Theta_p_e_p_tot vs. W (pFDpCD)
    TH2D* hTheta_p_e_p_tot_vs_W_pFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} vs. W (All Int., pFDpCD)",
                                                  "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                                  "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} [#circ];",
                                                  numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_p_e_p_tot_vs_W_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_p_tot (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p (pFDpCD, CD & FD)

    // Theta_q_p_tot (CD & FD)
    THStack* sTheta_q_p_tot_pFDpCD = OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}_{tot}} (All Int., pFDpCD)",
                                                 "#theta_{#font[62]{q},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{tot}=#font[62]{P}_{pFD}+#font[62]{P}_{pCD} "
                                                 "(All Int., pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{tot}} [#circ];"));
    TH1D* hTheta_q_p_tot_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{tot}} (All Int., pFDpCD)",
                                           "#theta_{#font[62]{q},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{tot}=#font[62]{P}_{pFD}+#font[62]{P}_{pCD} "
                                           "(All Int., pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{tot}} [#circ];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_p_tot_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_p_max (CD & FD)
    TH1D* hTheta_q_p_max_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{max}} (All Int., pFDpCD)",
                                           "#theta_{#font[62]{q},#font[62]{P}_{max}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{max} "
                                           "(All Int., pFDpCD);#theta_{#font[62]{q},#font[62]{P}_{max}} [#circ];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_p_max_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_P_pL_minus_q_pR and Theta_q_p_R (pFDpCD, CD & FD)
    THStack* sTheta_P_pL_minus_q_pR_pFDpCD =
        OwnHistogram(new THStack("#theta_{#font[62]{P}_{pL}-#font[62]{q},#font[62]{P}_{pR}} (All Int., pFDpCD)",
                    "#theta_{#font[62]{P}_{pL}-#font[62]{q},#font[62]{P}_{pR}} - Opening Angle Between #font[62]{P}_{pL}-#font[62]{q} and #font[62]{P}_{pR} (All Int., pFDpCD);"
                    "#theta_{#font[62]{P}_{pL}-#font[62]{q},#font[62]{P}_{pR}} [#circ];"));
    TH1D* hTheta_P_pL_minus_q_pR_pFDpCD =
        OwnHistogram(new TH1D("#theta_{#font[62]{P}_{pL}-#font[62]{q},#font[62]{P}_{pR}} (All Int., pFDpCD)",
                 "#theta_{#font[62]{P}_{pL}-#font[62]{q},#font[62]{P}_{pR}} - Opening Angle Between #font[62]{P}_{pL}-#font[62]{q} and #font[62]{P}_{pR} (All Int., pFDpCD)"
                 ";#theta_{#font[62]{P}_{pL}-#font[62]{q},#font[62]{P}_{pR}} [#circ]",
                 numTH1Dbins_Ang_Plots, Opening_Ang_wide_llim, Opening_Ang_wide_ulim));
    std::string hTheta_P_pL_minus_q_pR_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_p_L and Theta_q_p_R (pFDpCD, CD & FD)
    THStack* sTheta_q_p_L_R_pFDpCD =
        OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}} (All Int., pFDpCD)",
                    "#theta_{#font[62]{q},#font[62]{P}} - Opening Angle Between #font[62]{q} and #font[62]{P} (All Int., pFDpCD);#theta_{#font[62]{q},#font[62]{P}} [#circ];"));
    TH1D* hTheta_q_p_L_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pL}} (All Int., pFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{pL}} - Opening Angle Between #font[62]{q} and Leading Proton #font[62]{P}_{pL} (All Int., pFDpCD)"
                                         ";#theta_{#font[62]{q},#font[62]{P}_{pL}} [#circ]",
                                         numTH1Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_q_p_R_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pR}} (All Int., pFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{pR}} - Opening Angle Between #font[62]{q} and Recoil Proton #font[62]{P}_{pR} (All Int., pFDpCD)"
                                         ";#theta_{#font[62]{q},#font[62]{P}_{pR}} [#circ]",
                                         numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_L_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];
    std::string hTheta_q_p_R_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_pFD and Theta_q_pCD (pFDpCD, CD & FD)
    THStack* sTheta_q_p_pFDpCD =
        OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}} (All Int., pFDpCD)",
                    "#theta_{#font[62]{q},#font[62]{P}} - Opening Angle Between #font[62]{q} and #font[62]{P} (All Int., pFDpCD);#theta_{#font[62]{q},#font[62]{P}} [#circ];"));
    TH1D* hTheta_q_pFD_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} (All Int., pFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{pFD}} - Opening Angle Between #font[62]{q} and FD Proton #font[62]{P}_{pFD} (All Int., pFDpCD)"
                                         ";#theta_{#font[62]{q},#font[62]{P}_{pFD}} [#circ]",
                                         numTH1Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_q_pCD_pFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pCD}} (All Int., pFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{pCD}} - Opening Angle Between #font[62]{q} and CD Proton #font[62]{P}_{pCD} (All Int., pFDpCD)"
                                         ";#theta_{#font[62]{q},#font[62]{P}_{pCD}} [#circ]",
                                         numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_pFD_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];
    std::string hTheta_q_pFD_pFDpCD_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_pFDpCD_Q2"];
    std::string hTheta_q_pCD_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];
    std::string hTheta_q_pCD_pFDpCD_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_pFDpCD_Q2"];

    // Theta_q_p_tot vs. W (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p_tot vs. W (CD & FD)
    TH2D* hTheta_q_p_tot_vs_W_pFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{tot}} vs. W (All Int., pFDpCD)",
                                                "#theta_{#font[62]{q},#font[62]{P}_{tot}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                                "#theta_{#font[62]{q},#font[62]{P}_{tot}} [#circ];",
                                                numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_p_tot_vs_W_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_P_pL_minus_q_pR vs. W (CD & FD)
    TH2D* hTheta_P_pL_minus_q_pR_vs_W_pFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{P}_{pL}-#font[62]{q},#font[62]{P}_{pR}} vs. W (All Int., pFDpCD)",
                 "#theta_{#font[62]{P}_{pL}-#font[62]{q},#font[62]{P}_{pR}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                 "#theta_{#font[62]{P}_{pL}-#font[62]{q},#font[62]{P}_{pR}} [#circ];",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_wide_llim, Opening_Ang_wide_ulim));
    std::string hTheta_P_pL_minus_q_pR_vs_W_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_p_L vs. W (CD & FD)
    TH2D* hTheta_q_p_L_vs_W_pFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pL}} vs. W (All Int., pFDpCD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{pL}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                              "#theta_{#font[62]{q},#font[62]{P}_{pL}} [#circ];",
                                              numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_p_L_vs_W_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_p_R vs. W (CD & FD)
    TH2D* hTheta_q_p_R_vs_W_pFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pR}} vs. W (All Int., pFDpCD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{pR}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                              "#theta_{#font[62]{q},#font[62]{P}_{pR}} [#circ];",
                                              numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_R_vs_W_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_pFD vs. W (CD & FD)
    TH2D* hTheta_q_pFD_vs_W_pFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. W (All Int., pFDpCD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                              "#theta_{#font[62]{q},#font[62]{P}_{pFD}} [#circ];",
                                              numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_pFD_vs_W_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_pCD vs. W (CD & FD)
    TH2D* hTheta_q_pCD_vs_W_pFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pCD}} vs. W (All Int., pFDpCD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{pCD}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                              "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [#circ];",
                                              numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_pCD_vs_W_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_p vs |P_p|/|q| (pFDpCD, CD & FD) ---------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p vs |P_p|/|q| (CD & FD)

    // Theta_q_p_L vs |P_p_L|/|q|
    TH2D* hTheta_q_p_L_vs_p_L_q_pFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pL}} vs. r_{pL} (All Int., pFDpCD)",
                                                  "#theta_{#font[62]{q},#font[62]{P}_{pL}} vs. r_{pL}=|#font[62]{P}_{pL}|/|#font[62]{q}| (All Int., pFDpCD);"
                                                  "r_{pL};#theta_{#font[62]{q},#font[62]{P}_{pL}} [#circ]",
                                                  numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_p_L_vs_p_L_q_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_p vs Theta_q_p (pFDpCD, CD & FD) -------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p_L vs Theta_q_p_R
    TH2D* hTheta_q_p_L_vs_Theta_q_p_R_pFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pL}} vs. #theta_{#font[62]{q},#font[62]{P}_{pR}} (All Int., pFDpCD)",
                                                        "#theta_{#font[62]{q},#font[62]{P}_{pL}} vs. #theta_{#font[62]{q},#font[62]{P}_{pR}} (All Int., pFDpCD);"
                                                        "#theta_{#font[62]{q},#font[62]{P}_{pL}};#theta_{#font[62]{q},#font[62]{P}_{pR}} [#circ]",
                                                        numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_L_vs_Theta_q_p_R_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_q_pFD vs Theta_q_pCD
    TH2D* hTheta_q_pFD_vs_Theta_q_pCD_pFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},#font[62]{P}_{pCD}} (All Int., pFDpCD)",
                                                        "#theta_{#font[62]{q},#font[62]{P}_{pFD}} vs. #theta_{#font[62]{q},#font[62]{P}_{pCD}} (All Int., pFDpCD);"
                                                        "#theta_{#font[62]{q},#font[62]{P}_{pFD}} [#circ];#theta_{#font[62]{q},#font[62]{P}_{pCD}} [#circ]",
                                                        numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_pFD_vs_Theta_q_pCD_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_pFD_pCD (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    // Theta_pFD_pCD (CD & FD)
    THStack* sTheta_pFD_pCD_pFDpCD = OwnHistogram(new THStack("#theta_{pFD,pCD} (All Int., pFDpCD)", "#theta_{pFD,pCD} - Opening Angle Between Protons (pFDpCD);#theta_{pFD,pCD} [#circ];"));
    TH1D* hTheta_pFD_pCD_All_Int_pFDpCD =
        OwnHistogram(new TH1D("#theta_{pFD,pCD} (All Int., pFDpCD)", "#theta_{pFD,pCD} - Opening Angle Between Protons (All Int., pFDpCD);#theta_{pFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_pFD_pCD_QEL_pFDpCD =
        OwnHistogram(new TH1D("#theta_{pFD,pCD} (QEL only, pFDpCD)", "#theta_{pFD,pCD} - Opening Angle Between Protons (QEL only, pFDpCD);#theta_{pFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_pFD_pCD_MEC_pFDpCD =
        OwnHistogram(new TH1D("#theta_{pFD,pCD} (MEC only, pFDpCD)", "#theta_{pFD,pCD} - Opening Angle Between Protons (MEC only, pFDpCD);#theta_{pFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_pFD_pCD_RES_pFDpCD =
        OwnHistogram(new TH1D("#theta_{pFD,pCD} (RES only, pFDpCD)", "#theta_{pFD,pCD} - Opening Angle Between Protons (RES only, pFDpCD);#theta_{pFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_pFD_pCD_DIS_pFDpCD =
        OwnHistogram(new TH1D("#theta_{pFD,pCD} (DIS only, pFDpCD)", "#theta_{pFD,pCD} - Opening Angle Between Protons (DIS only, pFDpCD);#theta_{pFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    std::string sTheta_pFD_pCD_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];
    std::string hTheta_pFD_pCD_All_Int_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_pFDpCD"];
    std::string hTheta_pFD_pCD_QEL_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_pFDpCD"];
    std::string hTheta_pFD_pCD_MEC_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_pFDpCD"];
    std::string hTheta_pFD_pCD_RES_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_pFDpCD"];
    std::string hTheta_pFD_pCD_DIS_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_pFDpCD"];

    // Theta_pFD_pCD vs. W (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    // Theta_pFD_pCD vs. W (CD & FD)
    TH2D* hTheta_pFD_pCD_vs_W_pFDpCD = OwnHistogram(new TH2D("#theta_{pFD,pCD} vs. W (All Int., pFDpCD)",
                                                "#theta_{pFD,pCD} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{pFD,pCD} [#circ];",
                                                numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_pFD_pCD_vs_W_pFDpCD_Dir = directories.Angle_dir_map["Opening_angles_pFDpCD"];

    // Theta_pFD_vs_Theta_pCD for Theta_pFD_pCD < 20 (CD & FD) ------------------------------------------------------------------------------------------------------

    // Theta_pFD_vs_Theta_pCD for Theta_pFD_pCD < 20 (CD & FD)
    TH2D* hTheta_pFD_vs_theta_pCD_for_Theta_pFD_pCD_20_pFDpCD = OwnHistogram(new TH2D("#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., pFDpCD)",
                                                                         "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., pFDpCD);"
                                                                         "#theta_{pCD} [#circ];#theta_{pFD} [#circ];",
                                                                         numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50));
    std::string hTheta_pFD_vs_theta_pCD_for_Theta_pFD_pCD_20_pFDpCD_Dir = directories.Angle_dir_map["Double_detection_pFDpCD"];

    // dPhi_pFD_pCD for Theta_pFD_pCD < 20 (pFDpCD, CD & FD) ----------------------------------------------------------------------------------------------------------------

    // dPhi_pFD_pCD for Theta_pFD_pCD < 20 (CD & FD)
    TH1D* hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_pFDpCD = OwnHistogram(new TH1D("#Delta#phi for #theta_{pFD,pCD}<20#circ (All Int., pFDpCD)",
                                                               "#Delta#phi for #theta_{pFD,pCD}<20#circ (All Int., pFDpCD);"
                                                               "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                               50, Phi_llim, Phi_ulim));
    std::string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_pFDpCD_Dir = directories.Angle_dir_map["Double_detection_pFDpCD"];

    TH1D* hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_pFDpCD = OwnHistogram(new TH1D("#Delta#phi for #theta_{pFD,pCD}<20#circ - ZOOMIN (All Int., pFDpCD)",
                                                                      "#Delta#phi for #theta_{pFD,pCD}<20#circ - ZOOMIN (All Int., pFDpCD);"
                                                                      "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                      50, -40, 40));
    std::string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_pFDpCD_Dir = directories.Angle_dir_map["Double_detection_pFDpCD"];

    // Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (pFDpCD, CD & FD) -----------------------------------------------------------------------------------------------------

    // Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (CD & FD)
    TH2D* hTheta_pFD_vs_theta_pCD_forall_Theta_pFD_pCD_pFDpCD = OwnHistogram(new TH2D("#theta_{pFD} vs. #theta_{pFD} #forall#theta_{pFD,pCD} (All Int., pFDpCD)",
                                                                         "#theta_{pFD} vs. #theta_{pCD} for every #theta_{pFD,pCD} (All Int., pFDpCD);"
                                                                         "#theta_{pCD} [#circ];#theta_{pFD} [#circ];",
                                                                         numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50));
    std::string hTheta_pFD_vs_theta_pCD_forall_Theta_pFD_pCD_pFDpCD_Dir = directories.Angle_dir_map["Double_detection_pFDpCD"];

    // dPhi_pFD_pCD for every Theta_pFD_pCD (pFDpCD, CD & FD) ---------------------------------------------------------------------------------------------------------------

    // dPhi_pFD_pCD for every Theta_pFD_pCD (CD & FD)
    TH1D* hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_pFDpCD = OwnHistogram(new TH1D("#Delta#phi #forall#theta_{pFD,pCD} (All Int., pFDpCD)",
                                                                "#Delta#phi for every #theta_{pFD,pCD} (All Int., pFDpCD);"
                                                                "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                50, Phi_llim, Phi_ulim));
    std::string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_pFDpCD_Dir = directories.Angle_dir_map["Double_detection_pFDpCD"];

    TH1D* hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_pFDpCD = OwnHistogram(new TH1D("#Delta#phi #forall#theta_{pFD,pCD} - ZOOMIN (All Int., pFDpCD)",
                                                                       "#Delta#phi for every #theta_{pFD,pCD} - ZOOMIN(All Int., pFDpCD);"
                                                                       "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                       50, -40, 40));
    std::string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_pFDpCD_Dir = directories.Angle_dir_map["Double_detection_pFDpCD"];

    TH1D* hdPhi_pFD_pCD_for_small_dTheta_pFDpCD = OwnHistogram(new TH1D("#Delta#phi for small #Delta#theta_{pFD/pCD} (All Int., pFDpCD)",
                                                           "#Delta#phi for small #Delta#theta_{pFD/pCD} = |#theta_{pFD/pCD}-40#circ|;"
                                                           "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                           50, Phi_llim, Phi_ulim));
    std::string hdPhi_pFD_pCD_for_small_dTheta_pFDpCD_Dir = directories.Angle_dir_map["Double_detection_pFDpCD"];

    TH1D* hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_pFDpCD = OwnHistogram(new TH1D("#Delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN (All Int., pFDpCD)",
                                                                  "#Delta#phi for small #Delta#theta_{pFD/pCD} = |#theta_{pFD/pCD}-40#circ| - ZOOMIN;"
                                                                  "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                  50, -40, 40));
    std::string hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_pFDpCD_Dir = directories.Angle_dir_map["Double_detection_pFDpCD"];

    // Ghost tracks handling (CD only) --------------------------------------------------------------------------------------------------------------------------------------

    // Theta_pFD_pCD vs. TOFpFD-TOFpCD plots (pFDpCD)
    hTheta_pFD_pCD_VS_ToFpFD_ToFpCD_AC_pFDpCD =
        hPlot2D("pFDpCD", "CD-CTOF", "#theta_{pFD,pCD} vs. ToF_{pFD}-ToF_{pCD} AC", "#theta_{pFD,pCD} vs. ToF_{pFD}-ToF_{pCD} AC", "#theta_{pFD,pCD} [#circ]", "ToF_{pFD}-ToF_{pCD} [ns]",
                directories.Angle_dir_map["CToF_hits_pFDpCD"], "01_Theta_pFD_pCD_VS_ToFpFD_ToFpCD_AC_pFDpCD", 0.0, 180, -3, 3, numTH2Dbins_Ang_Plots, numTH2Dbins_Ang_Plots);

    // Theta_pFD_pCD vs. positionpFD-positionpCD plots (pFDpCD)
    hTheta_pFD_pCD_VS_PospFD_PospCD_AC_pFDpCD = hPlot2D("pFDpCD", "CD-CTOF", "#theta_{pFD,pCD} vs. Position_{pFD}-Position_{pCD} AC", "#theta_{pFD,pCD} vs. Position_{pFD}-Position_{pCD} AC",
                                                        "#theta_{pFD,pCD} [#circ]", "Position_{pFD}-Position_{pCD} [cm]", directories.Angle_dir_map["CToF_hits_pFDpCD"],
                                                        "02_Theta_p1_p2_VS_Pos1-Pos2_AC_pFDpCD", 0.0, 180, 0, 100, numTH2Dbins_Ang_Plots, numTH2Dbins_Ang_Plots);

    // Other angle plots (nFDpCD)

    // Nucleons' angles plots (nFDpCD)

    // Theta_nFD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_nFD histograms
    THStack* sTheta_nFD_nFDpCD_FD = OwnHistogram(new THStack("#theta_{nFD} (nFDpCD)", "#theta_{nFD} of FD neutron (nFDpCD);#theta_{nFD} [#circ];"));

    TH1D* hTheta_nFD_All_Int_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{nFD} (All Int., nFDpCD)", "#theta_{nFD} of FD neutron (All Int., nFDpCD);#theta_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_nFD_QEL_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{nFD} (QEL Only, nFDpCD)", "#theta_{nFD} of FD neutron (QEL Only, nFDpCD);#theta_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_nFD_MEC_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{nFD} (MEC only, nFDpCD)", "#theta_{nFD} of FD neutron (MEC only, nFDpCD);#theta_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_nFD_RES_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{nFD} (RES only, nFDpCD)", "#theta_{nFD} of FD neutron (RES only, nFDpCD);#theta_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH1D* hTheta_nFD_DIS_nFDpCD_FD =
        OwnHistogram(new TH1D("#theta_{nFD} (DIS only, nFDpCD)", "#theta_{nFD} of FD neutron (DIS only, nFDpCD);#theta_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_nFD_All_Int_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_nFD_nFDpCD"];
    std::string hTheta_nFD_QEL_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_nFD_nFDpCD"];
    std::string hTheta_nFD_MEC_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_nFD_nFDpCD"];
    std::string hTheta_nFD_RES_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_nFD_nFDpCD"];
    std::string hTheta_nFD_DIS_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_nFD_nFDpCD"];

    TH2D* hTheta_nFD_VS_P_nFD_nFDpCD_FD = OwnHistogram(new TH2D("#theta_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", "#theta_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD);P_{nFD} [GeV/c];#theta_{nFD} [#circ]",
                                                   numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    TH2D* hTheta_nFD_VS_W_nFDpCD_FD = OwnHistogram(new TH2D("#theta_{nFD} vs. W (All Int., nFDpCD, FD)",
                                               "#theta_{nFD} vs. W (All Int., nFDpCD, FD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{nFD} [#circ]",
                                               numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_nFD_VS_P_nFD_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_nFD_nFDpCD"];
    std::string hTheta_nFD_VS_W_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_nFD_nFDpCD"];

    // Phi_nFD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_nFD histograms
    THStack* sPhi_nFD_nFDpCD_FD = OwnHistogram(new THStack("#phi_{nFD} (nFDpCD)", "#phi_{nFD} of FD neutron (nFDpCD);#phi_{nFD} [#circ];"));

    TH1D* hPhi_nFD_All_Int_nFDpCD_FD =
        OwnHistogram(new TH1D("#phi_{nFD} (All Int., nFDpCD)", "#phi_{nFD} of FD neutron (All Int., nFDpCD);#phi_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_nFD_QEL_nFDpCD_FD = OwnHistogram(new TH1D("#phi_{nFD} (QEL Only, nFDpCD)", "#phi_{nFD} of FD neutron (QEL Only, nFDpCD);#phi_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_nFD_MEC_nFDpCD_FD = OwnHistogram(new TH1D("#phi_{nFD} (MEC only, nFDpCD)", "#phi_{nFD} of FD neutron (MEC only, nFDpCD);#phi_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_nFD_RES_nFDpCD_FD = OwnHistogram(new TH1D("#phi_{nFD} (RES only, nFDpCD)", "#phi_{nFD} of FD neutron (RES only, nFDpCD);#phi_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_nFD_DIS_nFDpCD_FD = OwnHistogram(new TH1D("#phi_{nFD} (DIS only, nFDpCD)", "#phi_{nFD} of FD neutron (DIS only, nFDpCD);#phi_{nFD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_nFD_All_Int_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_nFD_nFDpCD"];
    std::string hPhi_nFD_QEL_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_nFD_nFDpCD"];
    std::string hPhi_nFD_MEC_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_nFD_nFDpCD"];
    std::string hPhi_nFD_RES_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_nFD_nFDpCD"];
    std::string hPhi_nFD_DIS_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_nFD_nFDpCD"];

    TH2D* hPhi_nFD_VS_P_nFD_nFDpCD_FD = OwnHistogram(new TH2D("#phi_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", "#phi_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD);P_{nFD} [GeV/c];#phi_{nFD} [#circ]",
                                                 numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_nFD_VS_W_nFDpCD_FD =
        OwnHistogram(new TH2D("#phi_{nFD} vs. W (All Int., nFDpCD, FD)", "#phi_{nFD} vs. W (All Int., nFDpCD, FD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{nFD} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_nFD_VS_P_nFD_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_nFD_nFDpCD"];
    std::string hPhi_nFD_VS_W_nFDpCD_FD_Dir = directories.Angle_dir_map["Phi_nFD_nFDpCD"];

    // Theta_nFD vs. Phi_nFD ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_nFD vs. Phi_nFD histograms
    TH2D* hTheta_nFD_VS_Phi_nFD_nFDpCD_FD =
        OwnHistogram(new TH2D("#theta_{nFD} vs. #phi_{nFD} of FD neutron (All Int., nFDpCD)", "#theta_{nFD} vs. #phi_{nFD} of FD neutron (All Int., nFDpCD);#phi_{nFD} [#circ];#theta_{nFD} [#circ]",
                 numTH2Dbins_Nucleon_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, Theta_llim_FD, Theta_ulim_FD));
    std::string hTheta_nFD_VS_Phi_nFD_nFDpCD_FD_Dir = directories.Angle_dir_map["Theta_nFD_VS_Phi_nFD_nFDpCD"];

    // Theta_pCD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_pCD histograms
    THStack* sTheta_pCD_nFDpCD_CD = OwnHistogram(new THStack("#theta_{pCD} (nFDpCD)", "#theta_{pCD} of CD proton (nFDpCD);#theta_{pCD} [#circ];"));

    TH1D* hTheta_pCD_All_Int_nFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (All Int., nFDpCD)", "#theta_{pCD} of CD proton (All Int., nFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_pCD_QEL_nFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (QEL Only, nFDpCD)", "#theta_{pCD} of CD proton (QEL Only, nFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_pCD_MEC_nFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (MEC only, nFDpCD)", "#theta_{pCD} of CD proton (MEC only, nFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_pCD_RES_nFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (RES only, nFDpCD)", "#theta_{pCD} of CD proton (RES only, nFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_pCD_DIS_nFDpCD_CD = OwnHistogram(new TH1D("#theta_{pCD} (DIS only, nFDpCD)", "#theta_{pCD} of CD proton (DIS only, nFDpCD);#theta_{pCD} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    std::string hTheta_pCD_All_Int_nFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_nFDpCD"];
    std::string hTheta_pCD_QEL_nFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_nFDpCD"];
    std::string hTheta_pCD_MEC_nFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_nFDpCD"];
    std::string hTheta_pCD_RES_nFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_nFDpCD"];
    std::string hTheta_pCD_DIS_nFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_nFDpCD"];

    TH2D* hTheta_pCD_VS_P_pCD_nFDpCD_CD = OwnHistogram(new TH2D("#theta_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", "#theta_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD);P_{pCD} [GeV/c];#theta_{pCD} [#circ]",
                                                   numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, 30, 155));
    TH2D* hTheta_pCD_VS_W_nFDpCD_CD = OwnHistogram(new TH2D("#theta_{pCD} vs. W (All Int., nFDpCD, CD)",
                                               "#theta_{pCD} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{pCD} [#circ]",
                                               numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 30, 155));
    std::string hTheta_pCD_VS_P_pCD_nFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_nFDpCD"];
    std::string hTheta_pCD_VS_W_nFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_nFDpCD"];

    // Phi_pCD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_pCD histograms
    THStack* sPhi_pCD_nFDpCD_CD = OwnHistogram(new THStack("#phi_{pCD} (nFDpCD)", "#phi_{pCD} of CD proton (nFDpCD);#phi_{pCD} [#circ];"));

    TH1D* hPhi_pCD_All_Int_nFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (All Int., nFDpCD)", "#phi_{pCD} of CD proton (All Int., nFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pCD_QEL_nFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (QEL Only, nFDpCD)", "#phi_{pCD} of CD proton (QEL Only, nFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pCD_MEC_nFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (MEC only, nFDpCD)", "#phi_{pCD} of CD proton (MEC only, nFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pCD_RES_nFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (RES only, nFDpCD)", "#phi_{pCD} of CD proton (RES only, nFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_pCD_DIS_nFDpCD_CD = OwnHistogram(new TH1D("#phi_{pCD} (DIS only, nFDpCD)", "#phi_{pCD} of CD proton (DIS only, nFDpCD);#phi_{pCD} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_pCD_All_Int_nFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_nFDpCD"];
    std::string hPhi_pCD_QEL_nFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_nFDpCD"];
    std::string hPhi_pCD_MEC_nFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_nFDpCD"];
    std::string hPhi_pCD_RES_nFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_nFDpCD"];
    std::string hPhi_pCD_DIS_nFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_nFDpCD"];

    TH2D* hPhi_pCD_VS_P_pCD_nFDpCD_CD = OwnHistogram(new TH2D("#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD);P_{pCD} [GeV/c];#phi_{pCD} [#circ]",
                                                 numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_pCD_VS_W_nFDpCD_CD =
        OwnHistogram(new TH2D("#phi_{pCD} vs. W (All Int., nFDpCD, CD)", "#phi_{pCD} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{pCD} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_pCD_VS_P_pCD_nFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_nFDpCD"];
    std::string hPhi_pCD_VS_W_nFDpCD_CD_Dir = directories.Angle_dir_map["Phi_pCD_nFDpCD"];

    // Theta_pCD vs. Phi_pCD ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_pCD vs. Phi_pCD histograms
    TH2D* hTheta_pCD_VS_Phi_pCD_nFDpCD_CD =
        OwnHistogram(new TH2D("#theta_{pCD} vs. #phi_{pCD} of CD proton (All Int., nFDpCD)", "#theta_{pCD} vs. #phi_{pCD} of CD proton (All Int., nFDpCD);#phi_{pCD} [#circ];#theta_{pCD} [#circ]",
                 numTH2Dbins_Nucleon_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, 30, 155));
    std::string hTheta_pCD_VS_Phi_pCD_nFDpCD_CD_Dir = directories.Angle_dir_map["Theta_pCD_VS_Phi_pCD_nFDpCD"];

    // Theta_tot ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_tot histograms
    THStack* sTheta_tot_nFDpCD = OwnHistogram(new THStack("#theta_{tot} (nFDpCD)", "#theta_{tot} of total 3-momentum (nFDpCD);#theta_{tot} [#circ];"));

    TH1D* hTheta_tot_All_Int_nFDpCD = OwnHistogram(new TH1D("#theta_{tot} (All Int., nFDpCD)", "#theta_{tot} of total 3-momentum (All Int., nFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                               Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_tot_QEL_nFDpCD = OwnHistogram(new TH1D("#theta_{tot} (QEL Only, nFDpCD)", "#theta_{tot} of total 3-momentum (QEL Only, nFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                           Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_tot_MEC_nFDpCD = OwnHistogram(new TH1D("#theta_{tot} (MEC only, nFDpCD)", "#theta_{tot} of total 3-momentum (MEC only, nFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                           Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_tot_RES_nFDpCD = OwnHistogram(new TH1D("#theta_{tot} (RES only, nFDpCD)", "#theta_{tot} of total 3-momentum (RES only, nFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                           Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_tot_DIS_nFDpCD = OwnHistogram(new TH1D("#theta_{tot} (DIS only, nFDpCD)", "#theta_{tot} of total 3-momentum (DIS only, nFDpCD);#theta_{tot} [#circ];", numTH1Dbins_Ang_Plots,
                                           Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_tot_All_Int_nFDpCD_Dir = directories.Angle_dir_map["Theta_tot_nFDpCD"];
    std::string hTheta_tot_QEL_nFDpCD_Dir = directories.Angle_dir_map["Theta_tot_nFDpCD"];
    std::string hTheta_tot_MEC_nFDpCD_Dir = directories.Angle_dir_map["Theta_tot_nFDpCD"];
    std::string hTheta_tot_RES_nFDpCD_Dir = directories.Angle_dir_map["Theta_tot_nFDpCD"];
    std::string hTheta_tot_DIS_nFDpCD_Dir = directories.Angle_dir_map["Theta_tot_nFDpCD"];

    TH2D* hTheta_tot_VS_P_tot_nFDpCD = OwnHistogram(new TH2D("#theta_{tot} vs. P_{tot} (All Int., nFDpCD, CD)", "#theta_{tot} vs. P_{tot} (All Int., nFDpCD, CD);P_{tot} [GeV/c];#theta_{tot} [#circ]",
                                                numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH2D* hTheta_tot_VS_W_nFDpCD = OwnHistogram(new TH2D("#theta_{tot} vs. W (All Int., nFDpCD, CD)",
                                            "#theta_{tot} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{tot} [#circ]",
                                            numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_tot_VS_P_tot_nFDpCD_Dir = directories.Angle_dir_map["Theta_tot_nFDpCD"];
    std::string hTheta_tot_VS_W_nFDpCD_Dir = directories.Angle_dir_map["Theta_tot_nFDpCD"];

    // Phi_tot --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_tot histograms
    THStack* sPhi_tot_nFDpCD = OwnHistogram(new THStack("#phi_{tot} (nFDpCD)", "#phi_{tot} of total 3-momentum (nFDpCD);#phi_{tot} [#circ];"));

    TH1D* hPhi_tot_All_Int_nFDpCD =
        OwnHistogram(new TH1D("#phi_{tot} (All Int., nFDpCD)", "#phi_{tot} of total 3-momentum (All Int., nFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_tot_QEL_nFDpCD = OwnHistogram(new TH1D("#phi_{tot} (QEL Only, nFDpCD)", "#phi_{tot} of total 3-momentum (QEL Only, nFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_tot_MEC_nFDpCD = OwnHistogram(new TH1D("#phi_{tot} (MEC only, nFDpCD)", "#phi_{tot} of total 3-momentum (MEC only, nFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_tot_RES_nFDpCD = OwnHistogram(new TH1D("#phi_{tot} (RES only, nFDpCD)", "#phi_{tot} of total 3-momentum (RES only, nFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_tot_DIS_nFDpCD = OwnHistogram(new TH1D("#phi_{tot} (DIS only, nFDpCD)", "#phi_{tot} of total 3-momentum (DIS only, nFDpCD);#phi_{tot} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_tot_All_Int_nFDpCD_Dir = directories.Angle_dir_map["Phi_tot_nFDpCD"];
    std::string hPhi_tot_QEL_nFDpCD_Dir = directories.Angle_dir_map["Phi_tot_nFDpCD"];
    std::string hPhi_tot_MEC_nFDpCD_Dir = directories.Angle_dir_map["Phi_tot_nFDpCD"];
    std::string hPhi_tot_RES_nFDpCD_Dir = directories.Angle_dir_map["Phi_tot_nFDpCD"];
    std::string hPhi_tot_DIS_nFDpCD_Dir = directories.Angle_dir_map["Phi_tot_nFDpCD"];

    TH2D* hPhi_tot_VS_P_tot_nFDpCD = OwnHistogram(new TH2D("#phi_{tot} vs. P_{tot} (All Int., nFDpCD, CD)", "#phi_{tot} vs. P_{tot} (All Int., nFDpCD, CD);P_{tot} [GeV/c];#phi_{tot} [#circ]",
                                              numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_tot_VS_W_nFDpCD =
        OwnHistogram(new TH2D("#phi_{tot} vs. W (All Int., nFDpCD, CD)", "#phi_{tot} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{tot} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_tot_VS_P_tot_nFDpCD_Dir = directories.Angle_dir_map["Phi_tot_nFDpCD"];
    std::string hPhi_tot_VS_W_nFDpCD_Dir = directories.Angle_dir_map["Phi_tot_nFDpCD"];

    // Theta_tot vs. Phi_tot ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_tot vs. Phi_tot histograms
    TH2D* hTheta_tot_VS_Phi_tot_nFDpCD = OwnHistogram(new TH2D("#theta_{tot} vs. #phi_{tot} of total 3-momentum (All Int., nFDpCD)",
                                                  "#theta_{tot} vs. #phi_{tot} of total 3-momentum (All Int., nFDpCD);#phi_{tot} [#circ];#theta_{tot} [#circ]", numTH2Dbins_Nucleon_Ang_Plots,
                                                  Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_tot_VS_Phi_tot_nFDpCD_Dir = directories.Angle_dir_map["Theta_tot_VS_Phi_tot_nFDpCD"];

    // Theta_rel ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_rel histograms
    THStack* sTheta_rel_nFDpCD = OwnHistogram(new THStack("#theta_{rel} (nFDpCD)", "#theta_{rel} of relative 3-momentum (nFDpCD);#theta_{rel} [#circ];"));

    TH1D* hTheta_rel_All_Int_nFDpCD =
        OwnHistogram(new TH1D("#theta_{rel} (All Int., nFDpCD)", "#theta_{rel} of relative 3-momentum (All Int., nFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_rel_QEL_nFDpCD = OwnHistogram(new TH1D("#theta_{rel} (QEL Only, nFDpCD)", "#theta_{rel} of relative 3-momentum (QEL Only, nFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_rel_MEC_nFDpCD = OwnHistogram(new TH1D("#theta_{rel} (MEC only, nFDpCD)", "#theta_{rel} of relative 3-momentum (MEC only, nFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_rel_RES_nFDpCD = OwnHistogram(new TH1D("#theta_{rel} (RES only, nFDpCD)", "#theta_{rel} of relative 3-momentum (RES only, nFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    TH1D* hTheta_rel_DIS_nFDpCD = OwnHistogram(new TH1D("#theta_{rel} (DIS only, nFDpCD)", "#theta_{rel} of relative 3-momentum (DIS only, nFDpCD);#theta_{rel} [#circ];", numTH1Dbins_Ang_Plots, 30, 155));
    std::string hTheta_rel_All_Int_nFDpCD_Dir = directories.Angle_dir_map["Theta_rel_nFDpCD"];
    std::string hTheta_rel_QEL_nFDpCD_Dir = directories.Angle_dir_map["Theta_rel_nFDpCD"];
    std::string hTheta_rel_MEC_nFDpCD_Dir = directories.Angle_dir_map["Theta_rel_nFDpCD"];
    std::string hTheta_rel_RES_nFDpCD_Dir = directories.Angle_dir_map["Theta_rel_nFDpCD"];
    std::string hTheta_rel_DIS_nFDpCD_Dir = directories.Angle_dir_map["Theta_rel_nFDpCD"];

    TH2D* hTheta_rel_VS_P_rel_nFDpCD = OwnHistogram(new TH2D("#theta_{rel} vs. P_{rel} (All Int., nFDpCD, CD)", "#theta_{rel} vs. P_{rel} (All Int., nFDpCD, CD);P_{rel} [GeV/c];#theta_{rel} [#circ]",
                                                numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, 30, 155));
    TH2D* hTheta_rel_VS_W_nFDpCD = OwnHistogram(new TH2D("#theta_{rel} vs. W (All Int., nFDpCD, CD)",
                                            "#theta_{rel} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{rel} [#circ]",
                                            numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 30, 155));
    std::string hTheta_rel_VS_P_rel_nFDpCD_Dir = directories.Angle_dir_map["Theta_rel_nFDpCD"];
    std::string hTheta_rel_VS_W_nFDpCD_Dir = directories.Angle_dir_map["Theta_rel_nFDpCD"];

    // Phi_rel --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Phi_rel histograms
    THStack* sPhi_rel_nFDpCD = OwnHistogram(new THStack("#phi_{rel} (nFDpCD)", "#phi_{rel} of relative 3-momentum (nFDpCD);#phi_{rel} [#circ];"));

    TH1D* hPhi_rel_All_Int_nFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (All Int., nFDpCD)", "#phi_{rel} of relative 3-momentum (All Int., nFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_rel_QEL_nFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (QEL Only, nFDpCD)", "#phi_{rel} of relative 3-momentum (QEL Only, nFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_rel_MEC_nFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (MEC only, nFDpCD)", "#phi_{rel} of relative 3-momentum (MEC only, nFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_rel_RES_nFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (RES only, nFDpCD)", "#phi_{rel} of relative 3-momentum (RES only, nFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH1D* hPhi_rel_DIS_nFDpCD =
        OwnHistogram(new TH1D("#phi_{rel} (DIS only, nFDpCD)", "#phi_{rel} of relative 3-momentum (DIS only, nFDpCD);#phi_{rel} [#circ];", numTH1Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_rel_All_Int_nFDpCD_Dir = directories.Angle_dir_map["Phi_rel_nFDpCD"];
    std::string hPhi_rel_QEL_nFDpCD_Dir = directories.Angle_dir_map["Phi_rel_nFDpCD"];
    std::string hPhi_rel_MEC_nFDpCD_Dir = directories.Angle_dir_map["Phi_rel_nFDpCD"];
    std::string hPhi_rel_RES_nFDpCD_Dir = directories.Angle_dir_map["Phi_rel_nFDpCD"];
    std::string hPhi_rel_DIS_nFDpCD_Dir = directories.Angle_dir_map["Phi_rel_nFDpCD"];

    TH2D* hPhi_rel_VS_P_rel_nFDpCD = OwnHistogram(new TH2D("#phi_{rel} vs. P_{rel} (All Int., nFDpCD, CD)", "#phi_{rel} vs. P_{rel} (All Int., nFDpCD, CD);P_{rel} [GeV/c];#phi_{rel} [#circ]",
                                              numTH2Dbins_Ang_Plots, 0, beamE11, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    TH2D* hPhi_rel_VS_W_nFDpCD =
        OwnHistogram(new TH2D("#phi_{rel} vs. W (All Int., nFDpCD, CD)", "#phi_{rel} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#phi_{rel} [#circ]",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Phi_llim, Phi_ulim));
    std::string hPhi_rel_VS_P_rel_nFDpCD_Dir = directories.Angle_dir_map["Phi_rel_nFDpCD"];
    std::string hPhi_rel_VS_W_nFDpCD_Dir = directories.Angle_dir_map["Phi_rel_nFDpCD"];

    // Theta_rel vs. Phi_rel ------------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_rel vs. Phi_rel histograms
    TH2D* hTheta_rel_VS_Phi_rel_nFDpCD = OwnHistogram(new TH2D("#theta_{rel} vs. #phi_{rel} of relative 3-momentum (All Int., nFDpCD)",
                                                  "#theta_{rel} vs. #phi_{rel} of relative 3-momentum (All Int., nFDpCD);#phi_{rel} [#circ];#theta_{rel} [#circ]",
                                                  numTH2Dbins_Nucleon_Ang_Plots, Phi_llim, Phi_ulim, numTH2Dbins_Nucleon_Ang_Plots, 30, 155));
    std::string hTheta_rel_VS_Phi_rel_nFDpCD_Dir = directories.Angle_dir_map["Theta_rel_VS_Phi_rel_nFDpCD"];

    // Theta_p_e_p_tot (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------------

    // Theta_p_e_p_tot (nFDpCD, CD & FD)
    THStack* sTheta_p_e_p_tot_nFDpCD =
        OwnHistogram(new THStack("#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} (All Int., nFDpCD)",
                    "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{tot}=#font[62]{P}_{nFD}+#font[62]{P}_{pCD} "
                    "(All Int., nFDpCD);#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} [#circ];"));
    TH1D* hTheta_p_e_p_tot_nFDpCD =
        OwnHistogram(new TH1D("#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} (All Int., nFDpCD)",
                 "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{P}_{e} and #font[62]{P}_{tot}=#font[62]{P}_{nFD}+#font[62]{P}_{pCD} "
                 "(All Int., nFDpCD);#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} [#circ];",
                 numTH1Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_p_e_p_tot_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_p_e_p_tot vs. W (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------

    // Theta_p_e_p_tot vs. W (nFDpCD)
    TH2D* hTheta_p_e_p_tot_vs_W_nFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} vs. W (All Int., nFDpCD)",
                                                  "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                                  "#theta_{#font[62]{P}_{e},#font[62]{P}_{tot}} [#circ];",
                                                  numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_p_e_p_tot_vs_W_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_p (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p (nFDpCD, CD & FD)

    // Theta_q_p_tot (CD & FD)
    THStack* sTheta_q_p_tot_nFDpCD = OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}_{tot}} (All Int., nFDpCD)",
                                                 "#theta_{#font[62]{q},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{tot}=#font[62]{P}_{nFD}+#font[62]{P}_{pCD} "
                                                 "(All Int., nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{tot}} [#circ];"));
    TH1D* hTheta_q_p_tot_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{tot}} (All Int., nFDpCD)",
                                           "#theta_{#font[62]{q},#font[62]{P}_{tot}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{tot}=#font[62]{P}_{nFD}+#font[62]{P}_{pCD} "
                                           "(All Int., nFDpCD);#theta_{#font[62]{q},#font[62]{P}_{tot}} [#circ];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_p_tot_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_P_nL_minus_q_nR and Theta_q_p_R (nFDpCD, CD & FD)
    THStack* sTheta_P_nL_minus_q_nR_nFDpCD =
        OwnHistogram(new THStack("#theta_{#font[62]{P}_{nL}-#font[62]{q},#font[62]{P}_{nR}} (All Int., nFDpCD)",
                    "#theta_{#font[62]{P}_{nL}-#font[62]{q},#font[62]{P}_{nR}} - Opening Angle Between #font[62]{P}_L-#font[62]{q} and #font[62]{P}_R (All Int., nFDpCD);"
                    "#theta_{#font[62]{P}_{nL}-#font[62]{q},#font[62]{P}_{nR}} [#circ];"));
    TH1D* hTheta_P_nL_minus_q_nR_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{P}_{nL}-#font[62]{q},#font[62]{P}_{nR}} (All Int., nFDpCD)",
                                                   "#theta_{#font[62]{P}_{nL}-#font[62]{q},#font[62]{P}_{nR}} - Opening Angle Between #font[62]{q} and #font[62]{P}_{nL} (All Int., nFDpCD)"
                                                   ";#theta_{#font[62]{P}_{nL}-#font[62]{q},#font[62]{P}_{nR}} [#circ]",
                                                   numTH1Dbins_Ang_Plots, Opening_Ang_wide_llim, Opening_Ang_wide_ulim));
    std::string hTheta_P_nL_minus_q_nR_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_p_L and Theta_q_p_R (nFDpCD, CD & FD)
    THStack* sTheta_q_p_L_R_nFDpCD =
        OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}} (All Int., nFDpCD)",
                    "#theta_{#font[62]{q},#font[62]{P}} - Opening Angle Between #font[62]{q} and #font[62]{P} (All Int., nFDpCD);#theta_{#font[62]{q},#font[62]{P}} [#circ];"));
    TH1D* hTheta_q_p_L_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{nL}} (All Int., nFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{nL}} - Opening Angle Between #font[62]{q} and Leading Nucleon #font[62]{P}_{nL} (All Int., nFDpCD);"
                                         "#theta_{#font[62]{q},#font[62]{P}_{nL}} [#circ]",
                                         numTH1Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_q_p_R_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{nR}} (All Int., nFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{nR}} - Opening Angle Between #font[62]{q} and Recoil Nucleon #font[62]{P}_{nR} (All Int., nFDpCD);"
                                         "#theta_{#font[62]{q},#font[62]{P}_{nR}} [#circ]",
                                         numTH1Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_L_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];
    std::string hTheta_q_p_R_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_nFD and Theta_q_pCD (nFDpCD, CD & FD)
    THStack* sTheta_q_p_nFDpCD =
        OwnHistogram(new THStack("#theta_{#font[62]{q},#font[62]{P}} (All Int., nFDpCD)",
                    "#theta_{#font[62]{q},#font[62]{P}} - Opening Angle Between #font[62]{q} and #font[62]{P} (All Int., nFDpCD);#theta_{#font[62]{q},#font[62]{P}} [#circ];"));
    TH1D* htheta_q_NFD_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} (All Int., nFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{nFD}} - Opening Angle Between #font[62]{q} and FD Neutron #font[62]{P}_{nFD} (All Int., nFDpCD);"
                                         "#theta_{#font[62]{q},#font[62]{P}_{nFD}} [#circ]",
                                         numTH1Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    TH1D* hTheta_q_pCD_nFDpCD = OwnHistogram(new TH1D("#theta_{#font[62]{q},#font[62]{P}_{pCD}} (All Int., nFDpCD)",
                                         "#theta_{#font[62]{q},#font[62]{P}_{pCD}} - Opening Angle Between #font[62]{q} and CD Proton #font[62]{P}_{pCD} (All Int., nFDpCD);"
                                         "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [#circ]",
                                         numTH1Dbins_Ang_Plots, 0, 180));
    std::string htheta_q_NFD_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];
    std::string htheta_q_NFD_nFDpCD_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_nFDpCD_theta_q_N"];
    std::string hTheta_q_pCD_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];
    std::string hTheta_q_pCD_nFDpCD_Dir_ReacMon = directories.ReacMon_dir_map["ReacMon_nFDpCD_theta_q_r"];

    // Theta_q_p_tot vs. W (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    // Theta_nFD_pCD vs. W (CD & FD)
    TH2D* hTheta_q_p_tot_vs_W_nFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{tot}} vs. W (All Int., nFDpCD)",
                                                "#theta_{#font[62]{q},#font[62]{P}_{tot}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                                "#theta_{#font[62]{q},#font[62]{P}_{tot}} [#circ];",
                                                numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_p_tot_vs_W_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_P_nL_minus_q_nR vs. W (CD & FD)
    TH2D* hTheta_P_nL_minus_q_nR_vs_W_nFDpCD =
        OwnHistogram(new TH2D("#theta_{#font[62]{P}_{nL}-#font[62]{q},#font[62]{P}_{nR}} vs. W (All Int., nFDpCD)",
                 "#theta_{#font[62]{P}_{nL}-#font[62]{q},#font[62]{P}_{nR}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                 "#theta_{#font[62]{P}_{nL}-#font[62]{q},#font[62]{P}_{nR}} [#circ];",
                 numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_wide_llim, Opening_Ang_wide_ulim));
    std::string hTheta_P_nL_minus_q_nR_vs_W_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_p_L vs. W (CD & FD)
    TH2D* hTheta_q_p_L_vs_W_nFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nL}} vs. W (All Int., nFDpCD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{nL}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                              "#theta_{#font[62]{q},#font[62]{P}_{nL}} [#circ];",
                                              numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_p_L_vs_W_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_p_R vs. W (CD & FD)
    TH2D* hTheta_q_p_R_vs_W_nFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nR}} vs. W (All Int., nFDpCD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{nR}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                              "#theta_{#font[62]{q},#font[62]{P}_{nR}} [#circ];",
                                              numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_R_vs_W_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_nFD vs. W (CD & FD)
    TH2D* htheta_q_NFD_vs_W_nFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. W (All Int., nFDpCD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                              "#theta_{#font[62]{q},#font[62]{P}_{nFD}} [#circ];",
                                              numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string htheta_q_NFD_vs_W_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_pCD vs. W (CD & FD)
    TH2D* hTheta_q_pCD_vs_W_nFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{pCD}} vs. W (All Int., nFDpCD)",
                                              "#theta_{#font[62]{q},#font[62]{P}_{pCD}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];"
                                              "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [#circ];",
                                              numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_pCD_vs_W_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_p_L vs |P_L|/|q| (nFDpCD, CD & FD) ---------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p vs |P_p|/|q| (CD & FD)
    TH2D* hTheta_q_p_L_vs_p_L_q_nFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nL}} vs. r_{nL} (All Int., nFDpCD)",
                                                  "#theta_{#font[62]{q},#font[62]{P}_{nL}} vs. r_{nL}=|#font[62]{P}_{nL}|/|#font[62]{q}| (All Int., nFDpCD);"
                                                  "r_{nFD};#theta_{#font[62]{q},#font[62]{P}_{nFD}} [#circ]",
                                                  numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim));
    std::string hTheta_q_p_L_vs_p_L_q_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_p vs Theta_q_p (nFDpCD, CD & FD) -------------------------------------------------------------------------------------------------------------------------

    // Theta_q_p_L vs Theta_q_p_R
    TH2D* hTheta_q_p_L_vs_Theta_q_p_R_nFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nL}} vs. #theta_{#font[62]{q},#font[62]{P}_{nR}} (All Int., nFDpCD)",
                                                        "#theta_{#font[62]{q},#font[62]{P}_{nL}} vs. #theta_{#font[62]{q},#font[62]{P}_{nR}} (All Int., nFDpCD);"
                                                        "#theta_{#font[62]{q},#font[62]{P}_{nL}};#theta_{#font[62]{q},#font[62]{P}_{nR}} [#circ]",
                                                        numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_q_p_L_vs_Theta_q_p_R_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_q_nFD vs Theta_q_pCD
    TH2D* htheta_q_NFD_vs_Theta_q_pCD_nFDpCD = OwnHistogram(new TH2D("#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},#font[62]{P}_{pCD}} (All Int., nFDpCD)",
                                                        "#theta_{#font[62]{q},#font[62]{P}_{nFD}} vs. #theta_{#font[62]{q},#font[62]{P}_{pCD}} (All Int., nFDpCD);"
                                                        "#theta_{#font[62]{q},#font[62]{P}_{nFD}} [#circ];#theta_{#font[62]{q},#font[62]{P}_{pCD}} [#circ]",
                                                        numTH2Dbins_Ang_Plots, Opening_Ang_narrow_llim, Opening_Ang_narrow_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string htheta_q_NFD_vs_Theta_q_pCD_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_nFD_pCD (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    // Theta_nFD_pCD (CD & FD)
    THStack* sTheta_nFD_pCD_nFDpCD = OwnHistogram(new THStack("#theta_{nFD,pCD} (All Int., nFDpCD)", "#theta_{nFD,pCD} - Opening Angle Between Protons (nFDpCD);#theta_{nFD,pCD} [#circ];"));
    TH1D* hTheta_nFD_pCD_All_Int_nFDpCD =
        OwnHistogram(new TH1D("#theta_{nFD,pCD} (All Int., nFDpCD)", "#theta_{nFD,pCD} - Opening Angle Between Protons (All Int., nFDpCD);#theta_{nFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_nFD_pCD_QEL_nFDpCD =
        OwnHistogram(new TH1D("#theta_{nFD,pCD} (QEL only, nFDpCD)", "#theta_{nFD,pCD} - Opening Angle Between Protons (QEL only, nFDpCD);#theta_{nFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_nFD_pCD_MEC_nFDpCD =
        OwnHistogram(new TH1D("#theta_{nFD,pCD} (MEC only, nFDpCD)", "#theta_{nFD,pCD} - Opening Angle Between Protons (MEC only, nFDpCD);#theta_{nFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_nFD_pCD_RES_nFDpCD =
        OwnHistogram(new TH1D("#theta_{nFD,pCD} (RES only, nFDpCD)", "#theta_{nFD,pCD} - Opening Angle Between Protons (RES only, nFDpCD);#theta_{nFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    TH1D* hTheta_nFD_pCD_DIS_nFDpCD =
        OwnHistogram(new TH1D("#theta_{nFD,pCD} (DIS only, nFDpCD)", "#theta_{nFD,pCD} - Opening Angle Between Protons (DIS only, nFDpCD);#theta_{nFD,pCD} [#circ];", numTH1Dbins_Ang_Plots, 0, 180));
    std::string sTheta_nFD_pCD_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_nFDpCD"];
    std::string hTheta_nFD_pCD_All_Int_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_nFDpCD"];
    std::string hTheta_nFD_pCD_QEL_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_nFDpCD"];
    std::string hTheta_nFD_pCD_MEC_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_nFDpCD"];
    std::string hTheta_nFD_pCD_RES_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_nFDpCD"];
    std::string hTheta_nFD_pCD_DIS_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_by_interaction_nFDpCD"];

    // Theta_nFD_pCD vs. W (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    // Theta_nFD_pCD vs. W (CD & FD)
    TH2D* hTheta_nFD_pCD_vs_W_nFDpCD = OwnHistogram(new TH2D("#theta_{nFD,pCD} vs. W (All Int., nFDpCD)",
                                                "#theta_{nFD,pCD} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #font[62]{q}^{2}}  [GeV/c^{2}];#theta_{nFD,pCD} [#circ];",
                                                numTH2Dbins_Ang_Plots, W_llim, W_ulim, numTH2Dbins_Ang_Plots, 0, 180));
    std::string hTheta_nFD_pCD_vs_W_nFDpCD_Dir = directories.Angle_dir_map["Opening_angles_nFDpCD"];

    // Theta_nFD_vs_theta_pCD for Theta_nFD_pCD < 20 (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------

    // Theta_nFD_vs_theta_pCD for Theta_nFD_pCD < 20 (CD & FD)
    TH2D* hTheta_nFD_vs_theta_pCD_for_Theta_nFD_pCD_20_nFDpCD = OwnHistogram(new TH2D("#theta_{nFD} vs. #theta_{pCD} for #theta_{nFD,pCD}<20#circ (All Int., nFDpCD)",
                                                                         "#theta_{nFD} vs. #theta_{pCD} for #theta_{nFD,pCD}<20#circ (All Int., nFDpCD);"
                                                                         "#theta_{pCD} [#circ];#theta_{nFD} [#circ];",
                                                                         numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50));
    std::string hTheta_nFD_vs_theta_pCD_for_Theta_nFD_pCD_20_nFDpCD_Dir = directories.Angle_dir_map["Double_detection_nFDpCD"];

    // dphi_nFD_pCD for Theta_nFD_pCD < 20 (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------

    // dphi_nFD_pCD for Theta_nFD_pCD < 20 (CD & FD)
    TH1D* hdphi_nFD_pCD_for_Theta_nFD_pCD_20_nFDpCD = OwnHistogram(new TH1D("#Delta#phi for #theta_{nFD,pCD}<20#circ (All Int., nFDpCD)",
                                                               "#Delta#phi for #theta_{nFD,pCD}<20#circ (All Int., nFDpCD);"
                                                               "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                               50, Phi_llim, Phi_ulim));
    std::string hdphi_nFD_pCD_for_Theta_nFD_pCD_20_nFDpCD_Dir = directories.Angle_dir_map["Double_detection_nFDpCD"];

    TH1D* hdphi_nFD_pCD_for_Theta_nFD_pCD_20_ZOOMIN_nFDpCD = OwnHistogram(new TH1D("#Delta#phi for #theta_{nFD,pCD}<20#circ - ZOOMIN (All Int., nFDpCD)",
                                                                      "#Delta#phi for #theta_{nFD,pCD}<20#circ - ZOOMIN (All Int., nFDpCD);"
                                                                      "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                      50, -40, 40));
    std::string hdphi_nFD_pCD_for_Theta_nFD_pCD_20_ZOOMIN_nFDpCD_Dir = directories.Angle_dir_map["Double_detection_nFDpCD"];

    // Theta_nFD_vs_theta_pCD for every Theta_nFD_pCD (nFDpCD, CD & FD)
    // -------------------------------------------------------------------------------------------------------------

    // Theta_nFD_vs_theta_pCD for Theta_nFD_pCD (CD & FD)
    TH2D* hTheta_nFD_vs_theta_pCD_forall_Theta_nFD_pCD_nFDpCD = OwnHistogram(new TH2D("#theta_{nFD} vs. #theta_{nFD} #forall#theta_{nFD,pCD} (All Int., nFDpCD)",
                                                                         "#theta_{nFD} vs. #theta_{pCD} for every #theta_{nFD,pCD} (All Int., nFDpCD);"
                                                                         "#theta_{pCD} [#circ];#theta_{nFD} [#circ];",
                                                                         numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50));
    std::string hTheta_nFD_vs_theta_pCD_forall_Theta_nFD_pCD_nFDpCD_Dir = directories.Angle_dir_map["Double_detection_nFDpCD"];

    // dphi_nFD_pCD for every Theta_nFD_pCD (nFDpCD, CD & FD)
    // -----------------------------------------------------------------------------------------------------------------------

    // Theta_nFD_vs_theta_pCD for every Theta_nFD_pCD (CD & FD)
    TH1D* hdphi_nFD_pCD_for_all_Theta_nFD_pCD_nFDpCD = OwnHistogram(new TH1D("#Delta#phi #forall#theta_{nFD,pCD} (All Int., nFDpCD)",
                                                                "#Delta#phi for every #theta_{nFD,pCD} (All Int., nFDpCD);"
                                                                "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                50, Phi_llim, Phi_ulim));
    std::string hdphi_nFD_pCD_for_all_Theta_nFD_pCD_nFDpCD_Dir = directories.Angle_dir_map["Double_detection_nFDpCD"];

    TH1D* hdphi_nFD_pCD_for_all_Theta_nFD_pCD_ZOOMIN_nFDpCD = OwnHistogram(new TH1D("#Delta#phi #forall#theta_{nFD,pCD} - ZOOMIN (All Int., nFDpCD)",
                                                                       "#Delta#phi for every #theta_{nFD,pCD} - ZOOMIN(All Int., nFDpCD);"
                                                                       "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                       50, -40, 40));
    std::string hdphi_nFD_pCD_for_all_Theta_nFD_pCD_ZOOMIN_nFDpCD_Dir = directories.Angle_dir_map["Double_detection_nFDpCD"];

    TH1D* hdphi_nFD_pCD_for_small_dTheta_nFDpCD = OwnHistogram(new TH1D("#Delta#phi for small #Delta#theta_{nFD/pCD} (All Int., nFDpCD)",
                                                           "#Delta#phi for small #Delta#theta_{nFD/pCD} = #theta_{nFD/pCD}-40#circ;"
                                                           "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                           50, Phi_llim, Phi_ulim));
    std::string hdphi_nFD_pCD_for_small_dTheta_nFDpCD_Dir = directories.Angle_dir_map["Double_detection_nFDpCD"];

    TH1D* hdphi_nFD_pCD_for_small_dTheta_ZOOMIN_nFDpCD = OwnHistogram(new TH1D("#Delta#phi for small #Delta#theta_{nFD/pCD} - ZOOMIN (All Int., nFDpCD)",
                                                                  "#Delta#phi for small #Delta#theta_{nFD/pCD} = #theta_{nFD/pCD}-40#circ - ZOOMIN;"
                                                                  "#Delta#phi = #phi_{pFD} - #phi_{pCD} [#circ];",
                                                                  50, -50, 05));
    std::string hdphi_nFD_pCD_for_small_dTheta_ZOOMIN_nFDpCD_Dir = directories.Angle_dir_map["Double_detection_nFDpCD"];

    // Neutron veto plots (nFDpCD) ----------------------------------------------------------------------------------------------------------------------------------------------

    // Neutron veto plots (nFDpCD)
    hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_BV_nFDpCD =
        hPlot2D("nFDpCD", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} BV", "'Neutron Hits' vs. Electron Hits - Before Veto",
                "#Delta#phi_{nFD,e} = #phi_{nFD}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{nFD,e} = #theta_{nFD}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_nFDpCD"], "01_Neutron_hits_vs_electron_hits_BV_nFDpCD", -180, 180, -50, 50, 65, 65);

    hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_AV_nFDpCD =
        hPlot2D("nFDpCD", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} AV", "'Neutron Hits' vs. Electron Hits - After Veto",
                "#Delta#phi_{nFD,e} = #phi_{nFD}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{nFD,e} = #theta_{nFD}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_nFDpCD"], "02_Neutron_hits_vs_electron_hits_AV_nFDpCD", -180, 180, -50, 50, 65, 65);

    hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_Vetoed_Neutrons_nFDpCD =
        hPlot2D("nFDpCD", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e}", "'Neutron Hits' vs. Electron Hits - Vetoed Neutrons",
                "#Delta#phi_{nFD,e} = #phi_{nFD}^{ECAL} - #phi_{e}^{ECAL} [#circ]", "#Delta#theta_{nFD,e} = #theta_{nFD}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                directories.Angle_dir_map["Neutron_veto_nFDpCD"], "03_Neutron_hits_vs_electron_hits_Vetoed_nFDpCD", -180, 180, -50, 50, 65, 65);

    BindReactionHistograms(hTheta_e_1p_FD_by_reaction, sTheta_e_1p_FD, hTheta_e_All_Int_1p_FD, hTheta_e_QEL_1p_FD, hTheta_e_MEC_1p_FD, hTheta_e_RES_1p_FD, hTheta_e_DIS_1p_FD);
    BindReactionHistograms(hTheta_e_1n_FD_by_reaction, sTheta_e_1n_FD, hTheta_e_All_Int_1n_FD, hTheta_e_QEL_1n_FD, hTheta_e_MEC_1n_FD, hTheta_e_RES_1n_FD, hTheta_e_DIS_1n_FD);
    BindReactionHistograms(hTheta_e_2p_FD_by_reaction, sTheta_e_2p_FD, hTheta_e_All_Int_2p_FD, hTheta_e_QEL_2p_FD, hTheta_e_MEC_2p_FD, hTheta_e_RES_2p_FD, hTheta_e_DIS_2p_FD);
    BindReactionHistograms(hTheta_e_pFDpCD_FD_by_reaction, sTheta_e_pFDpCD_FD, hTheta_e_All_Int_pFDpCD_FD, hTheta_e_QEL_pFDpCD_FD, hTheta_e_MEC_pFDpCD_FD, hTheta_e_RES_pFDpCD_FD,
                           hTheta_e_DIS_pFDpCD_FD);
    BindReactionHistograms(hTheta_e_nFDpCD_FD_by_reaction, sTheta_e_nFDpCD_FD, hTheta_e_All_Int_nFDpCD_FD, hTheta_e_QEL_nFDpCD_FD, hTheta_e_MEC_nFDpCD_FD, hTheta_e_RES_nFDpCD_FD,
                           hTheta_e_DIS_nFDpCD_FD);

    BindReactionHistograms(hPhi_e_1p_FD_by_reaction, nullptr, hPhi_e_All_Int_1p_FD, hPhi_e_QEL_1p_FD, hPhi_e_MEC_1p_FD, hPhi_e_RES_1p_FD, hPhi_e_DIS_1p_FD);
    BindReactionHistograms(hPhi_e_1n_FD_by_reaction, nullptr, hPhi_e_All_Int_1n_FD, hPhi_e_QEL_1n_FD, hPhi_e_MEC_1n_FD, hPhi_e_RES_1n_FD, hPhi_e_DIS_1n_FD);
    BindReactionHistograms(hPhi_e_2p_FD_by_reaction, nullptr, hPhi_e_All_Int_2p_FD, hPhi_e_QEL_2p_FD, hPhi_e_MEC_2p_FD, hPhi_e_RES_2p_FD, hPhi_e_DIS_2p_FD);
    BindReactionHistograms(hPhi_e_pFDpCD_FD_by_reaction, nullptr, hPhi_e_All_Int_pFDpCD_FD, hPhi_e_QEL_pFDpCD_FD, hPhi_e_MEC_pFDpCD_FD, hPhi_e_RES_pFDpCD_FD, hPhi_e_DIS_pFDpCD_FD);
    BindReactionHistograms(hPhi_e_nFDpCD_FD_by_reaction, nullptr, hPhi_e_All_Int_nFDpCD_FD, hPhi_e_QEL_nFDpCD_FD, hPhi_e_MEC_nFDpCD_FD, hPhi_e_RES_nFDpCD_FD, hPhi_e_DIS_nFDpCD_FD);

    BindReactionHistograms(hTheta_p1_p2_2p_by_reaction, sTheta_p1_p2_2p, hTheta_p1_p2_All_Int_2p, hTheta_p1_p2_QEL_2p, hTheta_p1_p2_MEC_2p, hTheta_p1_p2_RES_2p, hTheta_p1_p2_DIS_2p);
    BindReactionHistograms(hTheta_pFD_pFDpCD_FD_by_reaction, sTheta_pFD_pFDpCD_FD, hTheta_pFD_All_Int_pFDpCD_FD, hTheta_pFD_QEL_pFDpCD_FD, hTheta_pFD_MEC_pFDpCD_FD, hTheta_pFD_RES_pFDpCD_FD,
                           hTheta_pFD_DIS_pFDpCD_FD);
    BindReactionHistograms(hPhi_pFD_pFDpCD_FD_by_reaction, sPhi_pFD_pFDpCD_FD, hPhi_pFD_All_Int_pFDpCD_FD, hPhi_pFD_QEL_pFDpCD_FD, hPhi_pFD_MEC_pFDpCD_FD, hPhi_pFD_RES_pFDpCD_FD,
                           hPhi_pFD_DIS_pFDpCD_FD);
    BindReactionHistograms(hTheta_pCD_pFDpCD_CD_by_reaction, sTheta_pCD_pFDpCD_CD, hTheta_pCD_All_Int_pFDpCD_CD, hTheta_pCD_QEL_pFDpCD_CD, hTheta_pCD_MEC_pFDpCD_CD, hTheta_pCD_RES_pFDpCD_CD,
                           hTheta_pCD_DIS_pFDpCD_CD);
    BindReactionHistograms(hPhi_pCD_pFDpCD_CD_by_reaction, sPhi_pCD_pFDpCD_CD, hPhi_pCD_All_Int_pFDpCD_CD, hPhi_pCD_QEL_pFDpCD_CD, hPhi_pCD_MEC_pFDpCD_CD, hPhi_pCD_RES_pFDpCD_CD,
                           hPhi_pCD_DIS_pFDpCD_CD);
    BindReactionHistograms(hTheta_tot_pFDpCD_by_reaction, sTheta_tot_pFDpCD, hTheta_tot_All_Int_pFDpCD, hTheta_tot_QEL_pFDpCD, hTheta_tot_MEC_pFDpCD, hTheta_tot_RES_pFDpCD,
                           hTheta_tot_DIS_pFDpCD);
    BindReactionHistograms(hPhi_tot_pFDpCD_by_reaction, sPhi_tot_pFDpCD, hPhi_tot_All_Int_pFDpCD, hPhi_tot_QEL_pFDpCD, hPhi_tot_MEC_pFDpCD, hPhi_tot_RES_pFDpCD, hPhi_tot_DIS_pFDpCD);
    BindReactionHistograms(hTheta_rel_pFDpCD_by_reaction, sTheta_rel_pFDpCD, hTheta_rel_All_Int_pFDpCD, hTheta_rel_QEL_pFDpCD, hTheta_rel_MEC_pFDpCD, hTheta_rel_RES_pFDpCD,
                           hTheta_rel_DIS_pFDpCD);
    BindReactionHistograms(hPhi_rel_pFDpCD_by_reaction, sPhi_rel_pFDpCD, hPhi_rel_All_Int_pFDpCD, hPhi_rel_QEL_pFDpCD, hPhi_rel_MEC_pFDpCD, hPhi_rel_RES_pFDpCD, hPhi_rel_DIS_pFDpCD);
    BindReactionHistograms(hTheta_pFD_pCD_pFDpCD_by_reaction, sTheta_pFD_pCD_pFDpCD, hTheta_pFD_pCD_All_Int_pFDpCD, hTheta_pFD_pCD_QEL_pFDpCD, hTheta_pFD_pCD_MEC_pFDpCD,
                           hTheta_pFD_pCD_RES_pFDpCD, hTheta_pFD_pCD_DIS_pFDpCD);
    BindReactionHistograms(hTheta_nFD_nFDpCD_FD_by_reaction, sTheta_nFD_nFDpCD_FD, hTheta_nFD_All_Int_nFDpCD_FD, hTheta_nFD_QEL_nFDpCD_FD, hTheta_nFD_MEC_nFDpCD_FD, hTheta_nFD_RES_nFDpCD_FD,
                           hTheta_nFD_DIS_nFDpCD_FD);
    BindReactionHistograms(hPhi_nFD_nFDpCD_FD_by_reaction, sPhi_nFD_nFDpCD_FD, hPhi_nFD_All_Int_nFDpCD_FD, hPhi_nFD_QEL_nFDpCD_FD, hPhi_nFD_MEC_nFDpCD_FD, hPhi_nFD_RES_nFDpCD_FD,
                           hPhi_nFD_DIS_nFDpCD_FD);
    BindReactionHistograms(hTheta_pCD_nFDpCD_CD_by_reaction, sTheta_pCD_nFDpCD_CD, hTheta_pCD_All_Int_nFDpCD_CD, hTheta_pCD_QEL_nFDpCD_CD, hTheta_pCD_MEC_nFDpCD_CD, hTheta_pCD_RES_nFDpCD_CD,
                           hTheta_pCD_DIS_nFDpCD_CD);
    BindReactionHistograms(hPhi_pCD_nFDpCD_CD_by_reaction, sPhi_pCD_nFDpCD_CD, hPhi_pCD_All_Int_nFDpCD_CD, hPhi_pCD_QEL_nFDpCD_CD, hPhi_pCD_MEC_nFDpCD_CD, hPhi_pCD_RES_nFDpCD_CD,
                           hPhi_pCD_DIS_nFDpCD_CD);
    BindReactionHistograms(hTheta_tot_nFDpCD_by_reaction, sTheta_tot_nFDpCD, hTheta_tot_All_Int_nFDpCD, hTheta_tot_QEL_nFDpCD, hTheta_tot_MEC_nFDpCD, hTheta_tot_RES_nFDpCD,
                           hTheta_tot_DIS_nFDpCD);
    BindReactionHistograms(hPhi_tot_nFDpCD_by_reaction, sPhi_tot_nFDpCD, hPhi_tot_All_Int_nFDpCD, hPhi_tot_QEL_nFDpCD, hPhi_tot_MEC_nFDpCD, hPhi_tot_RES_nFDpCD, hPhi_tot_DIS_nFDpCD);
    BindReactionHistograms(hTheta_rel_nFDpCD_by_reaction, sTheta_rel_nFDpCD, hTheta_rel_All_Int_nFDpCD, hTheta_rel_QEL_nFDpCD, hTheta_rel_MEC_nFDpCD, hTheta_rel_RES_nFDpCD,
                           hTheta_rel_DIS_nFDpCD);
    BindReactionHistograms(hPhi_rel_nFDpCD_by_reaction, sPhi_rel_nFDpCD, hPhi_rel_All_Int_nFDpCD, hPhi_rel_QEL_nFDpCD, hPhi_rel_MEC_nFDpCD, hPhi_rel_RES_nFDpCD, hPhi_rel_DIS_nFDpCD);
    BindReactionHistograms(hTheta_nFD_pCD_nFDpCD_by_reaction, sTheta_nFD_pCD_nFDpCD, hTheta_nFD_pCD_All_Int_nFDpCD, hTheta_nFD_pCD_QEL_nFDpCD, hTheta_nFD_pCD_MEC_nFDpCD,
                           hTheta_nFD_pCD_RES_nFDpCD, hTheta_nFD_pCD_DIS_nFDpCD);
#pragma endregion
}
