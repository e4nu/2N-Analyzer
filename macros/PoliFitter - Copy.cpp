/* root BetaFitAndSave.cpp -q -b */

#include <iostream>
#include <fstream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
#include <TROOT.h>

//#include "../framework/classes/ExperimentParameters/ExperimentParameters.cpp"
//#include "../framework/classes/AMaps/AMaps.cpp"
//#include "../framework/classes/clas12ana/clas12ana.h"
//#include "../framework/classes/DEfficiency/DEfficiency.cpp"
//#include "../framework/classes/DSCuts/DSCuts.h"
//#include "../framework/classes/hPlots/hPlot1D.cpp"
//#include "../framework/classes/hPlots/hPlot2D.cpp"
//#include "../framework/classes/MomentumResolution/MomentumResolution.cpp"
//#include "../framework/classes/TLCuts/TLCuts.cpp"
//#include "../framework/functions/AngleCalc/GetBinFromAng.h"
//#include "../framework/functions/FitFunctions/BetaFit.h"
//#include "../framework/functions/FitFunctions/BetaFitApprax.h"
//#include "../framework/functions/DrawAndSaveFunctions/DrawAndSaveACorrPlots.h"
//#include "../framework/functions/DrawAndSaveFunctions/DrawAndSaveEfficiencyPlots.h"
//#include "../framework/functions/DrawAndSaveFunctions/DrawAndSaveFSRatio.h"
//#include "../framework/functions/FillByInt1D.h"
//#include "../framework/functions/GetPi0MomTh.h"
//#include "../framework/functions/PID_functions/ChargedParticleID.h"
//#include "../framework/functions/PID_functions/FDNeutralParticle.h"
//#include "../framework/functions/PID_functions/FDNeutralParticleID.h"
//#include "../framework/functions/PID_functions/GetFDNeutronP.h"
//#include "../framework/functions/AngleCalc/CalcdPhi.h"
//#include "../framework/functions/PID_functions/GetGoodParticles.h"
//#include "../framework/functions/PID_functions/GetGoodProtons.h"
//#include "../framework/functions/NeutronECAL_Cut_Veto.h"
//#include "../framework/functions/StackPlot3.h"
//#include "../framework/functions/StackPlot4.h"
//#include "../framework/functions/TLKinCutsCheck.h"

//#include "HipoChain.h"
//#include "clas12reader.h"

using namespace std;
//using namespace clas12;

/*
#include <iostream>
#include <fstream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
#include <TROOT.h>

#include "../framework/classes/DSCuts/DSCuts.h"
#include "../framework/functions/Math_func/poly34.cpp"
#include "../framework/functions/findSubstring.h"
#include "../framework/functions/to_string_with_precision.h"
#include "../framework/constants.h"

#include "../framework/classes/MomentumResolution/MomentumResolution.cpp"

using namespace std;
*/
/*
void drawtext() {
    Int_t i, n;
    Double_t x, y;
    TLatex l;

    l.SetTextSize(0.025);
    l.SetTextFont(0);
//    l.SetTextFont(42);
    l.SetTextAlign(21);
    l.SetTextColor(kMagenta);
    l.SetLineColor(kWhite);

    auto g = (TGraph *) gPad->GetListOfPrimitives()->FindObject("Graph");
    n = g->GetN();

    for (i = 0; i < n; i++) {
        g->GetPoint(i, x, y);
        l.PaintText(x, y + 0.02, Form("(%4.5f,%4.1f)", x, y));
    }
}

Double_t FitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; } // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}
*/

void PoliFitter() {

//    #pragma region /* Original */
//    Double_t x[] = {1, 2, 3, 4, 5, 6, 7, 8};
//    Double_t y[] = {35, 57, 25, 22, 55, 47, 89, 44};
//    TGraph *g = new TGraph((sizeof(x) / sizeof(Double_t)), x, y);
//    TF1 *f = new TF1("f", "[2] * x * x + [1] * x + [0]");
//    g->Fit(f);
//    g->Draw("AL");
//    #pragma endregion

//    #pragma region /* First good */
//    Double_t x[] = {1, 2, 3, 4, 5, 6, 7, 8};
//    Double_t y[] = {35, 57, 25, 22, 55, 47, 89, 44};
//
//    TGraph *g = new TGraph((sizeof(x) / sizeof(Double_t)), x, y);
//
//    TF1 *f = new TF1("f", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C
//
//    g->Fit(f);
//    g->Draw("AL");
//
//    double A = f->GetParameter(0); // get [0]
//    double B = f->GetParameter(1); // get [1]
//    double C = f->GetParameter(2); // get [2]
//
//    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
//    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
//
//    double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
//    double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;
//
//    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
//    FitParam->SetBorderSize(1);
//    FitParam->SetTextFont(0);
//    FitParam->SetFillColor(0);
//    FitParam->SetTextAlign(12);
//    FitParam->AddText(("Fit A = " + to_string(A)).c_str());
//    FitParam->AddText(("Fit B = " + to_string(B)).c_str());
//    FitParam->AddText(("Fit C = " + to_string(C)).c_str());
////    FitParam->AddText(("Fit A = " + to_string_with_precision(A, 8)).c_str());
////    FitParam->AddText(("Fit B = " + to_string_with_precision(B, 8)).c_str());
////    FitParam->AddText(("Fit C = " + to_string_with_precision(C, 8)).c_str());
////    ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
//    FitParam->Draw("same");
//    #pragma endregion

    #pragma region /* First good */

//    /* Histogram plots output directory */
//    std::string WorkingDirectory = "../";
//
///* framework directory and directories */
//    std::string SourceDirectory = WorkingDirectory + "framework" + "/";
//    std::string DataDirectory = SourceDirectory + "data" + "/";
//    std::string ACorrDirectory = DataDirectory + "AcceptanceCorrections" + "/";
//    std::string AcceptanceMapsDirectory = DataDirectory + "AcceptanceMaps" + "/";
//    std::string AcceptanceWeightsDirectory = DataDirectory + "AcceptanceWeights" + "/";
//    std::string MomentumResolutionDirectory = DataDirectory + "MomentumResolution" + "/";
//    std::string NucleonCutsDirectory = DataDirectory + "NucleonCuts" + "/";
//    std::string PIDCutsDirectory = DataDirectory + "PIDCuts" + "/";
//
//    std::string SampleName = "C12x4_simulation_G18_Q204_6GeV";
//
//    bool plot_and_fit_MomRes = true; // Generate nRes plots
//    const double DeltaSlices = 0.05;
//    const bool VaryingDelta = true;
//    const std::string SmearMode = "pol1", CorrMode = "pol1";
//    bool nRes_test = false; // false by default

//    MomentumResolution nRes;
//    nRes = MomentumResolution(SampleName, NucleonCutsDirectory, "Neutron", /* beamE */ 5.98636, /*n_mom_th.GetLowerCut()*/ 0.4,
//                             /* directories.Resolution_dir_map["pRes_plots_1p_Directory"] */ "03_1n/01_Neutron_resolution_plots_1n",
//                             DeltaSlices, VaryingDelta, SmearMode, CorrMode, nRes_test);

    Double_t x[] = {1, 2, 3, 4, 5, 6, 7, 8};
    Double_t y[] = {35, 57, 25, 22, 55, 47, 89, 44};

    TGraph *g = new TGraph((sizeof(x) / sizeof(Double_t)), x, y);

    TF1 *f = new TF1("f", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C

    g->Fit(f);
    g->Draw("AL");

    double A = f->GetParameter(0); // get [0]
    double B = f->GetParameter(1); // get [1]
    double B = f->GetParameter(1); // get [1]
    double B = f->GetParameter(1); // get [1]
    double C = f->GetParameter(2); // get [2]

    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;

    double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
    double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetTextFont(0);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("Fit A = " + to_string(A)).c_str());
    FitParam->AddText(("Fit B = " + to_string(B)).c_str());
    FitParam->AddText(("Fit C = " + to_string(C)).c_str());
//    FitParam->AddText(("Fit A = " + to_string_with_precision(A, 8)).c_str());
//    FitParam->AddText(("Fit B = " + to_string_with_precision(B, 8)).c_str());
//    FitParam->AddText(("Fit C = " + to_string_with_precision(C, 8)).c_str());
//    ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
    FitParam->Draw("same");
    #pragma endregion

//    #pragma region /* Old stuff */
//    cout << "\n\n";
//
//    //    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-_ALL_CUTS/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
////    std::string SampleName = "C12_simulation_6GeV_T5_first_10";
//
////    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
////    std::string SampleName = "C12_simulation_6GeV_T5_first_100";
//
////    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
////    std::string SampleName = "C12_simulation_6GeV_T5_first_250";
//
////    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_-02_ALL_CUTS_NoBetaCut_wPhotonsFD/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
////    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_WithBetaCut_fixedTLcuts/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//    TFile *f = new TFile("00_plots_C12_simulation_6GeV_T5_-02_ALL_CUTS_noNC_/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//    std::string SampleName = "C12_simulation_6GeV_T5";
//
//    cout << "\nSampleName:\t" << SampleName << "\n\n";
//
//    DSCuts Beta_cut = DSCuts("Beta_nuc", "FD", "", "1n", 1, -9999, 9999); /* Beta cut (1n, FD) */
//    DSCuts n_momentum_cuts = DSCuts("Momentum", "", "Neutron", "", 0, 0.3, 9999); /* Momentum cuts (thresholds) */
//
//    double W_yLLim = -0.1, W_yULim = 0.1, W_xLLim = 0.9, W_xULim = 1.;
//    double deltaPRel_UncertaintyU = 0.2, deltaPRel_UncertaintyL = 0.1;
//
//    #pragma region /* Canvas definitions */
//    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
//    Canvas->SetGrid();
//    Canvas->SetBottomMargin(0.14);
//
//    Canvas->SetLeftMargin(0.18);
//    Canvas->SetRightMargin(0.12);
//
//    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
//
//    Canvas->cd();
//    #pragma endregion
//
//    #pragma region /* Setting sNameFlag */
//    std::string sNameFlag;
//
//    if (findSubstring(SampleName, "sim")) {
//        sNameFlag = "s";
//    } else if (findSubstring(SampleName, "data")) {
//        sNameFlag = "d";
//    }
//    #pragma endregion
//
//    #pragma region /* Setting particle */
//    std::string BetaTitle = "Neutron #beta from 'photons'";
//
//    std::string BetaParticle, BetaParticleShort;
//
//    if (findSubstring(BetaTitle, "Electron") || findSubstring(BetaTitle, "electron")) {
//        BetaParticle = "Electron";
//        BetaParticleShort = "e";
//    } else if (findSubstring(BetaTitle, "Proton") || findSubstring(BetaTitle, "proton")) {
//        BetaParticle = "Proton";
//        BetaParticleShort = "p";
//    } else if (findSubstring(BetaTitle, "Neutron") || findSubstring(BetaTitle, "neutron")) {
//        BetaParticle = "Neutron";
//        BetaParticleShort = "n";
//    } else if (findSubstring(BetaTitle, "#pi^{+}")) {
//        BetaParticle = "Piplus";
//        BetaParticleShort = "#pi^{+}";
//    } else if (findSubstring(BetaTitle, "#pi^{-}")) {
//        BetaParticle = "Piminus";
//        BetaParticleShort = "#pi^{-}";
//    } else if (findSubstring(BetaTitle, "K^{+}")) {
//        BetaParticle = "Kplus";
//        BetaParticleShort = "K^{+}";
//    } else if (findSubstring(BetaTitle, "K^{-}")) {
//        BetaParticle = "Kminus";
//        BetaParticleShort = "K^{-}";
//    } else if (findSubstring(BetaTitle, "#gamma") || findSubstring(BetaTitle, "photon")
//               || findSubstring(BetaTitle, "Photon")) {
//        BetaParticle = "Photon";
//        BetaParticleShort = "#gamma";
//    }
//    #pragma endregion
//
//    #pragma region /* Setting final State */
//    std::string BetaFinalState = "1n";
//    #pragma endregion
//
//    #pragma region /* Setting histogram and preforming a fit */
//    TH1D *hpx = (TH1D *) f->Get("#beta of n from '#gamma' (1n, FD)");
//    TH1D *hBeta_Clone = (TH1D *) hpx->Clone("#beta of n from '#gamma' - fitted");
//    Int_t Color = hBeta_Clone->GetLineColor();
//
//    TF1 *func = new TF1("fit", FitFunction, 0, 2, 3); // create a function with 3 parameters in the range [-3,3]
//    func->SetLineColor(kRed);
//
//    double BetaMax = hBeta_Clone->GetMaximum();
//    double BetaMean = hBeta_Clone->GetMean();
//
//    func->SetParameters(BetaMax, BetaMean, 0.001); // start fit with histogram's max and mean
//    func->SetParNames("Constant", "Mean_value", "Sigma");
//
//    cout << "Beta histogram {max, mean}:\t{" << BetaMax << ", " << BetaMean << "}\n\n\n\n";
//
//////    // Adding limits to "Constant"
//////    double BetaConstantUlim = 1.2 * BetaMax;
//////    double BetaConstantLlim = 0.95 * BetaMax;
//////    func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
//////    cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";
////    // Adding limits to "Constant"
////    double BetaConstantUlim = 1.2 * BetaMax;
////    double BetaConstantLlim = BetaMax;
//////    double BetaConstantLlim = 0.90 * BetaMax;
//////    double BetaConstantLlim = 0.67 * BetaMax;
////    func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
////    cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";
//
//    // Adding limits to "Constant"
//    double BetaConstantUlim = 1.5 * BetaMax;
////    double BetaConstantLlim = BetaMax;
//    double BetaConstantLlim = 0.90 * BetaMax;
////    double BetaConstantLlim = 0.67 * BetaMax;
//    func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
//    cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";
//
//
//    // Adding limits to "Mean_value"
//    double BetaMean_valueUlim = 1.02;
//    double BetaMean_valueLlim = 1.01;
////    double BetaMean_valueLlim = 1.008;
////    double BetaMean_valueLlim = 1.0075;
//    func->SetParLimits(1, BetaMean_valueLlim, BetaMean_valueUlim);
//    cout << "Beta Mean_value {Llim, Ulim}:\t{" << BetaMean_valueLlim << ", " << BetaMean_valueUlim << "}\n\n";
////    // Adding limits to "Mean_value"
////    double BetaMean_valueUlim = 1.02;
////    double BetaMean_valueLlim = 1.005;
////    func->SetParLimits(1, BetaMean_valueLlim, BetaMean_valueUlim);
////    cout << "Beta Mean_value {Llim, Ulim}:\t{" << BetaMean_valueLlim << ", " << BetaMean_valueUlim << "}\n\n";
//
//
////    // Adding limits to "Sigma"
//////    double BetaSigmaUlim = 0.015;
////    double BetaSigmaUlim = 0.01;
////    double BetaSigmaLlim = 0.00001;
////    func->SetParLimits(2, BetaSigmaLlim, BetaSigmaUlim);
////    cout << "Beta Sigma {Llim, Ulim}:\t{" << BetaSigmaLlim << ", " << BetaSigmaUlim << "}\n\n";
//
//
//
//    cout << "\n";
//    hBeta_Clone->Fit("fit");
//    hBeta_Clone->Draw();
//    cout << "\n";
//
//    TF1 *fit = hBeta_Clone->GetFunction("fit");
//    double FitAmp = fit->GetParameter(0); // get p0
//    double FitMean = fit->GetParameter(1); // get p1
//    double FitStd = fit->GetParameter(2); // get p2
//
//    Beta_cut.SetUpperCut(fit->GetParameter(2));
//    #pragma endregion
//
//    #pragma region /* Drawing fit parameters and saving */
//    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
//    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
//
//    double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
//    double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;
//
//    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
//    FitParam->SetBorderSize(1);
//    FitParam->SetTextFont(0);
//    FitParam->SetFillColor(0);
//    FitParam->SetTextAlign(12);
//    FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 8)).c_str());
//    FitParam->AddText(("Fit mean = " + to_string_with_precision(FitMean, 8)).c_str());
//    FitParam->AddText(("Fit std = " + to_string_with_precision(FitStd, 8)).c_str());
//    ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
//    FitParam->Draw("same");
//
//    std::string hBeta_CloneSaveNameDir = "./" + sNameFlag + "01_Beta_Neutron_from_photons_1n" + "_fitted.png";
//    const char *hBeta_CloneSaveDir = hBeta_CloneSaveNameDir.c_str();
//    Canvas->SaveAs(hBeta_CloneSaveDir);
//
//    Canvas->Clear();
//    #pragma endregion
//
//    #pragma region /* Plot deltaP as function of beta */
//    std::string deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
//    std::string deltaPTitle = BetaParticle + " momentum uncertainty #deltaP_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
//    std::string deltaPfunc = to_string(m_n * FitStd) + "/ ( (1 - x*x) * sqrt(1 - x*x) )";
//
//    auto *deltaP = new TF1(deltaPStatsTitle.c_str(), deltaPfunc.c_str(), 0.9, 1);
//    deltaP->SetTitle(deltaPTitle.c_str());
//    deltaP->GetXaxis()->SetTitleSize(0.06);
//    deltaP->GetXaxis()->SetLabelSize(0.0425);
//    deltaP->GetXaxis()->CenterTitle(true);
//    deltaP->GetXaxis()->SetTitle("#beta");
//    deltaP->GetYaxis()->SetRangeUser(0., 30.);
//    deltaP->GetYaxis()->SetTitleSize(0.06);
//    deltaP->GetYaxis()->SetLabelSize(0.0425);
//    deltaP->GetYaxis()->CenterTitle(true);
//    deltaP->GetYaxis()->SetTitle(("#deltaP_{" + BetaParticleShort + "} = #frac{m_{" + BetaParticleShort + "}#delta#beta}{(1 - #beta^{2})^{3/2}}").c_str());
//
//    deltaP->SetLineColor(Color);
//    deltaP->Draw();
//
//    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05 + 0.15, x_2_FitParam, y_2_FitParam + 0.15, "NDC");
////    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.1, x_2_FitParam, y_2_FitParam, "NDC");
////    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
//    deltaPParam->SetBorderSize(1);
//    deltaPParam->SetTextFont(0);
//    deltaPParam->SetTextSize(0.03);
//    deltaPParam->SetFillColor(0);
//    deltaPParam->SetTextAlign(12);
//    deltaPParam->AddText(("d#beta = " + to_string(FitStd)).c_str());
////    deltaPParam->AddText(("#delta#beta = " + to_string_with_precision(FitStd, 8)).c_str());
//    deltaPParam->Draw("same");
//
//    std::string deltaPSaveNameDir = "./" + sNameFlag + "02a_P_" + BetaParticleShort + "_uncertainty" + BetaFinalState + ".png";
//    const char *deltaPSaveDir = deltaPSaveNameDir.c_str();
//    Canvas->SaveAs(deltaPSaveDir);
//
//    Canvas->Clear();
//    #pragma endregion
//
//    #pragma region /* Solve deltaP/P for beta in range 0.9<=beta<1 */
//    double Beta_Max, P_Beta_Max, Beta_Min, P_Beta_Min;
//    double Beta_Max_sol[3], Beta_Min_sol[3];
//
//    SolveP3(Beta_Max_sol, 0, -1, FitStd / deltaPRel_UncertaintyU);
//    SolveP3(Beta_Min_sol, 0, -1, FitStd / deltaPRel_UncertaintyL);
//
//    cout << "\nSolutions for W(beta) = 0 for 20%:\n";
//
//    for (int i = 0; i < 3; i++) {
//        cout << "Beta_Max_sol[" << i << "] = " << Beta_Max_sol[i] << "\n";
//
//        //TODO: see if other checks for the solution are required!!!
//        if (Beta_Max_sol[i] >= 0.9 && Beta_Max_sol[i] < 1) { Beta_Max = Beta_Max_sol[i]; }
//    }
//
//    P_Beta_Max = m_n * Beta_Max / sqrt(1 - Beta_Max * Beta_Max);
//
//    cout << "W(beta) const Max = " << FitStd / deltaPRel_UncertaintyU << "\n";
//    cout << "Beta_Max = " << Beta_Max << " is chosen\n";
//    cout << "P(Beta_Max) = " << P_Beta_Max << "\n\n";
//
//    cout << "Solutions for W(beta) = 0 for 10%:\n";
//
//    for (int i = 0; i < 3; i++) {
//        cout << "Beta_Min_sol[" << i << "] = " << Beta_Min_sol[i] << "\n";
//
//        //TODO: see if other checks for the solution are required!!!
//        if (Beta_Min_sol[i] >= 0.9 && Beta_Min_sol[i] < 1) { Beta_Min = Beta_Min_sol[i]; }
//    }
//
//    P_Beta_Min = m_n * Beta_Min / sqrt(1 - Beta_Min * Beta_Min);
//
//    cout << "W(beta) const Max = " << FitStd / deltaPRel_UncertaintyL << "\n";
//    cout << "Beta_Min = " << Beta_Min << " is chosen\n";
//    cout << "P(Beta_Min) = " << P_Beta_Min << "\n\n";
//
//    n_momentum_cuts.SetUpperCut(P_Beta_Max);
//    #pragma endregion
//
//    #pragma region /* Plot deltaP/P as function of beta */
//    std::string Rel_deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
//    std::string Rel_deltaPTitle = BetaParticle + " relative uncertainty #deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
//    std::string Rel_deltaPfunc = to_string(FitStd) + "/ ( (1 - x*x) * x )";
//
//    auto *Rel_deltaP = new TF1(Rel_deltaPStatsTitle.c_str(), Rel_deltaPfunc.c_str(), 0.9, 1);
//    Rel_deltaP->SetTitle(Rel_deltaPTitle.c_str());
//    Rel_deltaP->GetXaxis()->SetTitleSize(0.06);
//    Rel_deltaP->GetXaxis()->SetLabelSize(0.0425);
//    Rel_deltaP->GetXaxis()->CenterTitle(true);
//    Rel_deltaP->GetXaxis()->SetTitle("#beta");
//    Rel_deltaP->GetYaxis()->SetRangeUser(0., 0.5);
//    Rel_deltaP->GetYaxis()->SetTitleSize(0.06);
//    Rel_deltaP->GetYaxis()->SetLabelSize(0.0425);
//    Rel_deltaP->GetYaxis()->CenterTitle(true);
//    Rel_deltaP->GetYaxis()->SetTitle(("#frac{#deltaP_{" + BetaParticleShort + "}}{P_{" + BetaParticleShort + "}} = #frac{1}{(1 - #beta^{2})#beta}").c_str());
//    Rel_deltaP->SetLineColor(kBlack);
//    Rel_deltaP->SetLineWidth(2);
//    Rel_deltaP->Draw();
//
//    TPaveText *deltaPRel_deltaP = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05, x_2_FitParam, y_2_FitParam, "NDC");
//    deltaPRel_deltaP->SetBorderSize(1);
//    deltaPRel_deltaP->SetTextFont(0);
//    deltaPRel_deltaP->SetTextSize(0.03);
//    deltaPRel_deltaP->SetFillColor(0);
//    deltaPRel_deltaP->SetTextAlign(12);
//    deltaPRel_deltaP->AddText(("d#beta = " + to_string(FitStd)).c_str());
//    deltaPRel_deltaP->Draw("same");
//
//    TLine *upper_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyU, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyU);
//    upper_cut->SetLineWidth(2);
//    upper_cut->SetLineColor(kBlue);
//    upper_cut->Draw("same");
//
//    TLine *lower_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyL, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyL);
//    lower_cut->SetLineWidth(2);
//    lower_cut->SetLineColor(kRed);
//    lower_cut->Draw("same");
//
//    auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.125, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
//    TLegendEntry *Cut_legend_deltaPRel_deltaP = Cut_legend->AddEntry(deltaPRel_deltaP, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "}").c_str(),
//                                                                     "l");
//    TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, "20% cut", "l");
//    TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, "10% cut", "l");
//    Cut_legend->Draw("same");
//
//    std::string Rel_deltaPSaveNameDir = "./" + sNameFlag + "02b_P_" + BetaParticleShort + "_rel_uncertainty" + BetaFinalState + ".png";
//
//    const Int_t n = 2;
//    auto gr = new TGraph(n);
//    gr->SetMarkerStyle(20);
//    gr->SetMarkerSize(20);
//    auto ex = new TExec("ex", "drawtext();");
//    gr->GetListOfFunctions()->Add(ex);
//    gr->SetPoint(0, Beta_Max, deltaPRel_UncertaintyU);
//    gr->SetPoint(1, Beta_Min, deltaPRel_UncertaintyL);
//    gr->Draw("same");
//
///*
//    const Int_t n_Max = 1;
//    auto gr_Max = new TGraph(n_Max);
//    gr_Max->SetMarkerStyle(20);
//    auto ex_Max = new TExec("ex_Max", "drawtext_Max();");
//    gr_Max->GetListOfFunctions()->Add(ex_Max);
//    gr_Max->SetPoint(0, Beta_Max, deltaPRel_UncertaintyU);
//    gr_Max->Draw("same");
//
//    const Int_t n_Min = 1;
//    auto gr_Min = new TGraph(n_Min);
//    gr_Min->SetMarkerStyle(20);
//    auto ex_Min = new TExec("ex_Min", "drawtext_Min();");
//    gr_Min->GetListOfFunctions()->Add(ex_Min);
//    gr_Min->SetPoint(0, Beta_Min, deltaPRel_UncertaintyL);
//    gr_Min->Draw("same");
//*/
//
//    const char *Rel_deltaPSaveDir = Rel_deltaPSaveNameDir.c_str();
//    Canvas->SaveAs(Rel_deltaPSaveDir);
//    Canvas->Clear();
//    #pragma endregion
//
//    #pragma region /* Plot w as function of beta */
//    std::string WStatsTitle = "W(#beta) (" + BetaFinalState + ")";
//    std::string WTitle = "The W(#beta) function (" + BetaFinalState + ")";
//    std::string W_Maxfunc = "x*x*x - x + " + to_string(FitStd / deltaPRel_UncertaintyU);
//    std::string W_Minfunc = "x*x*x - x + " + to_string(FitStd / deltaPRel_UncertaintyL);
//
//    auto *W_Max = new TF1(WStatsTitle.c_str(), W_Maxfunc.c_str(), 0.9, 1);
//    W_Max->SetLineWidth(2);
//    W_Max->SetLineStyle(2);
//    W_Max->SetTitle(WTitle.c_str());
//    W_Max->GetXaxis()->SetTitleSize(0.06);
//    W_Max->GetXaxis()->SetLabelSize(0.0425);
//    W_Max->GetXaxis()->CenterTitle(true);
//    W_Max->GetXaxis()->SetTitle("#beta");
//    W_Max->GetYaxis()->SetRangeUser(W_yLLim, W_yULim);
//    W_Max->GetYaxis()->SetTitleSize(0.06);
//    W_Max->GetYaxis()->SetLabelSize(0.0425);
//    W_Max->GetYaxis()->CenterTitle(true);
//    W_Max->GetYaxis()->SetTitle(("W(#beta) = #beta^{3} - #beta + #delta#beta(#frac{#deltaP_{" + BetaParticleShort + "}}{P_{" + BetaParticleShort + "}})^{-1}").c_str());
//    W_Max->SetLineColor(kBlack);
//    W_Max->SetLineWidth(2);
//    W_Max->Draw();
//
//    auto *W_Min = new TF1(WStatsTitle.c_str(), W_Minfunc.c_str(), 0.9, 1);
//    W_Min->SetLineWidth(2);
//    W_Min->SetLineStyle(10);
//    W_Min->SetLineColor(kBlack);
//    W_Min->SetLineWidth(2);
//    W_Min->Draw("same");
//
//    TLine *Beta_Max_cut = new TLine(Beta_Max, W_yULim, Beta_Max, W_yLLim);
//    Beta_Max_cut->SetLineWidth(2);
//    Beta_Max_cut->SetLineColor(kBlue);
//    Beta_Max_cut->Draw("same");
//
//    TLine *Beta_Min_cut = new TLine(Beta_Min, W_yULim, Beta_Min, W_yLLim);
//    Beta_Min_cut->SetLineWidth(2);
//    Beta_Min_cut->SetLineColor(kRed);
//    Beta_Min_cut->Draw("same");
//
//    TLine *XAxis = new TLine(W_xLLim, 0., W_xULim, 0.);
//    XAxis->SetLineColor(kBlack);
//    XAxis->Draw("same");
//
//    auto W_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.1, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
//    TLegendEntry *W_legend_upper_lim = W_legend->AddEntry(Beta_Max_cut, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "} = 0.2").c_str(), "l");
//    TLegendEntry *W_legend_lower_lim = W_legend->AddEntry(Beta_Min_cut, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "} = 0.1").c_str(), "l");
//    W_legend->Draw("same");
//
//    std::string WSaveNameDir = "./" + sNameFlag + "03_W_function.png";
//    const char *WSaveDir = WSaveNameDir.c_str();
//    Canvas->SaveAs(WSaveDir);
//    Canvas->Clear();
//    #pragma endregion
//    #pragma endregion

}