//
// Created by Alon Sportes on 26/03/2025.
//

#ifndef FIT_FUNCTIONS_H
#define FIT_FUNCTIONS_H

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>

#include <fstream>
#include <iostream>
#include <string>

#include "../../classes/DSCuts/DSCuts.h"
#include "../../classes/hPlots/hPlot1D.cpp"
#include "../general_utilities/analysis_math/analysis_math.h"
#include "../general_utilities/constants.h"
#include "../general_utilities/utilities.h"

namespace fitter_functions {
using namespace analysis_math;

// FitFunction function --------------------------------------------------------------------------------------------------------------------------------------------------

// NOTE:
// TF1 in ROOT expects a function with exactly the signature:
// Double_t func(Double_t* x, Double_t* par);
// And since NumOfParameters is meant to be a constant for each fit function, we define it externally and set it before calling TF1 when deeded.
static int NumOfParameters = 3;  // Define it externally

Double_t FitFunction(Double_t *v, Double_t *par) {
    if (NumOfParameters != 2 && NumOfParameters != 3) {
        std::cerr << "fitter_functions::FitFunction: invalid number of parameters! Choose 2 or 3. Aborting..." << std::endl;
        exit(1);
    }

    Double_t arg = (NumOfParameters == 2) ? (par[1] != 0 ? (v[0] - 1) / par[1] : 0) : (par[2] != 0 ? (v[0] - par[1]) / par[2] : 0);

    return par[0] * TMath::Exp(-0.5 * arg * arg);
}

// BetaFit function -----------------------------------------------------------------------------------------------------------------------------------------------------

void BetaFit(const std::string &SampleName, DSCuts &Beta_cut, DSCuts &Momentum_cuts, const hPlot1D &BetaPlot, TList *Histogram_list, const double beamE) {
    double W_yLLim = -0.1, W_yULim = 0.1, W_xLLim = 0.9, W_xULim = 1.;
    double deltaPRel_UncertaintyU = 0.2, deltaPRel_UncertaintyL = 0.1;

    #pragma region /* Canvas definitions */
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000 * 2, 750 * 2);  // normal res
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.18);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas->cd();
    #pragma endregion

    #pragma region /* Setting sNameFlag */
    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }
    #pragma endregion

    #pragma region /* Setting particle */
    std::string BetaParticle = data_processor::GetParticleName(BetaPlot.GetHistogramTitle());
    std::string BetaParticleShort = data_processor::GetParticleNameShort(BetaPlot.GetHistogramTitle());

    #pragma region /* Setting histogram */
    TH1D *hBeta = BetaPlot.GetHistogram();
    TH1D *hBeta_Clone = (TH1D *)hBeta->Clone((BetaPlot.GetHistogramStatTitle() + " - fitted").c_str());
    Int_t Color = hBeta_Clone->GetLineColor();
    #pragma endregion

    if (hBeta_Clone->Integral() != 0.) {
        #pragma region /* Preforming a fit */
        TF1 *func = new TF1("fit", FitFunction, 0, 2, 3);  // create a function with 3 parameters in the range [-3,3]
        func->SetLineColor(kRed);

        double BetaMax = hBeta_Clone->GetMaximum();
        double BetaMean = hBeta_Clone->GetMean();

        func->SetParameters(BetaMax, BetaMean, 0.001);  // start fit with histogram's max and mean
        func->SetParNames("Constant", "Mean_value", "Sigma");

        cout << "Beta histogram {max, mean}:\t{" << BetaMax << ", " << BetaMean << "}\n\n\n\n";

        // Adding limits to "Constant"
        double BetaConstantUlim = 1.2 * BetaMax;
        double BetaConstantLlim = 0.90 * BetaMax;
        //    double BetaConstantLlim = 0.67 * BetaMax;
        func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
        cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";

        // Adding limits to "Mean_value"
        //    double BetaMean_valueUlim = 1.0075;
        double BetaMean_valueUlim = 1.02;
        double BetaMean_valueLlim = 1.008;
        //        double BetaMean_valueLlim = 1.0075;
        //        double BetaMean_valueLlim = 1.005;
        func->SetParLimits(1, BetaMean_valueLlim, BetaMean_valueUlim);
        cout << "Beta Mean_value {Llim, Ulim}:\t{" << BetaMean_valueLlim << ", " << BetaMean_valueUlim << "}\n\n";

        //    // Adding limits to "Sigma"
        ////    double BetaSigmaUlim = 0.015;
        //    double BetaSigmaUlim = 0.01;
        //    double BetaSigmaLlim = 0.00001;
        //    func->SetParLimits(2, BetaSigmaLlim, BetaSigmaUlim);
        //    cout << "Beta Sigma {Llim, Ulim}:\t{" << BetaSigmaLlim << ", " << BetaSigmaUlim << "}\n\n";

        cout << "\n";
        hBeta_Clone->Fit("fit");
        hBeta_Clone->Draw();
        cout << "\n";

        TF1 *fit = hBeta_Clone->GetFunction("fit");
        double FitAmp = fit->GetParameter(0);   // get p0
        double FitMean = fit->GetParameter(1);  // get p1
        double FitStd = fit->GetParameter(2);   // get p2

        Beta_cut.SetUpperCut(fit->GetParameter(2));
        Beta_cut.SetMean(fit->GetParameter(1));
        #pragma endregion

        #pragma region /* Drawing fit parameters and saving */
        double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
        double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;

        double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
        double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;
        //    double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
        //    double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.245;

        TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
        FitParam->SetBorderSize(1);
        FitParam->SetTextFont(0);
        FitParam->SetFillColor(0);
        FitParam->SetTextAlign(12);
        FitParam->AddText(("Fit amp = " + basic_tools::ToStringWithPrecision(FitAmp, 8)).c_str());
        FitParam->AddText(("Fit mean = " + basic_tools::ToStringWithPrecision(FitMean, 8)).c_str());
        FitParam->AddText(("Fit std = " + basic_tools::ToStringWithPrecision(FitStd, 8)).c_str());
        ((TText *)FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
        FitParam->Draw("same");

        std::string hBeta_CloneSaveNameDir = BetaPlot.GetHistogram1DSaveNamePath() + sNameFlag + BetaPlot.GetHistogram1DSaveName() + "_fitted.pdf";
        const char *hBeta_CloneSaveDir = hBeta_CloneSaveNameDir.c_str();
        Canvas->SaveAs(hBeta_CloneSaveDir);

        Canvas->Clear();
        #pragma endregion

        #pragma region /* Plot deltaP as function of beta */
        std::string deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaPlot.GetFinalState() + ")";
        std::string deltaPTitle = BetaParticle + " momentum uncertainty #deltaP_{" + BetaParticleShort + "} (" + BetaPlot.GetFinalState() + ")";
        std::string deltaPfunc = to_string(m_n * FitStd) + "/ ( (1 - x*x) * sqrt(1 - x*x) )";

        auto *deltaP = new TF1(deltaPStatsTitle.c_str(), deltaPfunc.c_str(), 0.9, 1);
        deltaP->SetTitle(deltaPTitle.c_str());
        deltaP->GetXaxis()->SetTitleSize(0.06);
        deltaP->GetXaxis()->SetLabelSize(0.0425);
        deltaP->GetXaxis()->CenterTitle(true);
        deltaP->GetXaxis()->SetTitle("#beta");
        deltaP->GetYaxis()->SetRangeUser(0., 30.);
        deltaP->GetYaxis()->SetTitleSize(0.06);
        deltaP->GetYaxis()->SetLabelSize(0.0425);
        deltaP->GetYaxis()->CenterTitle(true);
        deltaP->GetYaxis()->SetTitle(("#deltaP_{" + BetaParticleShort + "} = #frac{m_{" + BetaParticleShort + "}#delta#beta}{(1 - #beta^{2})^{3/2}}").c_str());
        //    deltaP->GetYaxis()->SetTitle(("#deltaP_{" + BetaParticleShort + "} = #frac{m_{" + BetaParticleShort + "}#cdot#delta#beta}{(1-#beta^{2})^{3/2}}").c_str());

        //    deltaP->SetLineColor(kBlack);
        deltaP->SetLineColor(Color);
        deltaP->Draw();
        Histogram_list->Add(deltaP);

        TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05 + 0.15, x_2_FitParam, y_2_FitParam + 0.15, "NDC");
        //    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.1, x_2_FitParam, y_2_FitParam, "NDC");
        //    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
        deltaPParam->SetBorderSize(1);
        deltaPParam->SetTextFont(0);
        deltaPParam->SetTextSize(0.03);
        deltaPParam->SetFillColor(0);
        deltaPParam->SetTextAlign(12);
        deltaPParam->AddText(("d#beta = " + to_string(FitStd)).c_str());
        //    deltaPParam->AddText(("#delta#beta = " + basic_tools::ToStringWithPrecision(FitStd, 8)).c_str());
        deltaPParam->Draw("same");

        std::string deltaPSaveNameDir = BetaPlot.GetHistogram1DSaveNamePath() + sNameFlag + "02a_P_" + BetaParticleShort + "_uncertainty" + BetaPlot.GetFinalState() + ".pdf";
        const char *deltaPSaveDir = deltaPSaveNameDir.c_str();
        Canvas->SaveAs(deltaPSaveDir);

        Canvas->Clear();
        #pragma endregion

        #pragma region /* Solve deltaP/P for beta in range 0.9<=beta<1 */
        double Beta_Max, P_Beta_Max, Beta_Min, P_Beta_Min;
        double Beta_Max_sol[3], Beta_Min_sol[3];

        SolveP3(Beta_Max_sol, 0, -1, FitStd / deltaPRel_UncertaintyU);
        SolveP3(Beta_Min_sol, 0, -1, FitStd / deltaPRel_UncertaintyL);

        cout << "\nSolutions for W(beta) = 0 for 20%:\n";

        for (int i = 0; i < 3; i++) {
            cout << "Beta_Max_sol[" << i << "] = " << Beta_Max_sol[i] << "\n";

            // TODO: see if other checks for the solution are required!!!
            if (Beta_Max_sol[i] >= 0.9 && Beta_Max_sol[i] < 1) { Beta_Max = Beta_Max_sol[i]; }
        }

        P_Beta_Max = m_n * Beta_Max / sqrt(1 - Beta_Max * Beta_Max);

        cout << "W(beta) const Max = " << FitStd / deltaPRel_UncertaintyU << "\n";
        cout << "Beta_Max = " << Beta_Max << " is chosen\n";
        cout << "P(Beta_Max) = " << P_Beta_Max << "\n\n";

        cout << "Solutions for W(beta) = 0 for 10%:\n";

        for (int i = 0; i < 3; i++) {
            cout << "Beta_Min_sol[" << i << "] = " << Beta_Min_sol[i] << "\n";

            // TODO: see if other checks for the solution are required!!!
            if (Beta_Min_sol[i] >= 0.9 && Beta_Min_sol[i] < 1) { Beta_Min = Beta_Min_sol[i]; }
        }

        P_Beta_Min = m_n * Beta_Min / sqrt(1 - Beta_Min * Beta_Min);

        cout << "W(beta) const Max = " << FitStd / deltaPRel_UncertaintyL << "\n";
        cout << "Beta_Min = " << Beta_Min << " is chosen\n";
        cout << "P(Beta_Min) = " << P_Beta_Min << "\n\n";

        Momentum_cuts.SetUpperCut(P_Beta_Max);
        #pragma endregion

        #pragma region /* Plot deltaP/P as function of beta */
        std::string Rel_deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaPlot.GetFinalState() + ")";
        std::string Rel_deltaPTitle = BetaParticle + " relative uncertainty #deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "}" + " (" + BetaPlot.GetFinalState() + ")";
        std::string Rel_deltaPfunc = to_string(FitStd) + "/ ( (1 - x*x) * x )";

        auto *Rel_deltaP = new TF1(Rel_deltaPStatsTitle.c_str(), Rel_deltaPfunc.c_str(), 0.9, 1);
        Rel_deltaP->SetTitle(Rel_deltaPTitle.c_str());
        Rel_deltaP->GetXaxis()->SetTitleSize(0.06);
        Rel_deltaP->GetXaxis()->SetLabelSize(0.0425);
        Rel_deltaP->GetXaxis()->CenterTitle(true);
        Rel_deltaP->GetXaxis()->SetTitle("#beta");
        Rel_deltaP->GetYaxis()->SetRangeUser(0., 0.5);
        Rel_deltaP->GetYaxis()->SetTitleSize(0.06);
        Rel_deltaP->GetYaxis()->SetLabelSize(0.0425);
        Rel_deltaP->GetYaxis()->CenterTitle(true);
        Rel_deltaP->GetYaxis()->SetTitle(("#frac{#deltaP_{" + BetaParticleShort + "}}{P_{" + BetaParticleShort + "}} = #frac{#delta#beta}{(1 - #beta^{2})#beta}").c_str());
        Rel_deltaP->SetLineColor(kBlack);
        Rel_deltaP->SetLineWidth(2);
        Rel_deltaP->Draw();
        Histogram_list->Add(Rel_deltaP);

        TPaveText *deltaPRel_deltaP = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05, x_2_FitParam, y_2_FitParam, "NDC");
        deltaPRel_deltaP->SetBorderSize(1);
        deltaPRel_deltaP->SetTextFont(0);
        deltaPRel_deltaP->SetTextSize(0.03);
        deltaPRel_deltaP->SetFillColor(0);
        deltaPRel_deltaP->SetTextAlign(12);
        deltaPRel_deltaP->AddText(("d#beta = " + to_string(FitStd)).c_str());
        deltaPRel_deltaP->Draw("same");

        TLine *upper_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyU, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyU);
        upper_cut->SetLineWidth(2);
        upper_cut->SetLineColor(kBlue);
        upper_cut->Draw("same");

        TLine *lower_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyL, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyL);
        lower_cut->SetLineWidth(2);
        lower_cut->SetLineColor(kRed);
        lower_cut->Draw("same");

        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.125, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
        TLegendEntry *Cut_legend_deltaPRel_deltaP = Cut_legend->AddEntry(deltaPRel_deltaP, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "}").c_str(), "l");
        TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, "20% cut", "l");
        TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, "10% cut", "l");
        Cut_legend->Draw("same");

        std::string Rel_deltaPSaveNameDir = BetaPlot.GetHistogram1DSaveNamePath() + sNameFlag + "02b_P_" + BetaParticleShort + "_rel_uncertainty" + BetaPlot.GetFinalState() + ".pdf";

        const Int_t n = 2;
        auto gr = new TGraph(n);
        gr->SetMarkerStyle(20);
        gr->SetMarkerSize(20);
        auto ex = new TExec("ex", "utilities::drawtext();");
        gr->GetListOfFunctions()->Add(ex);
        gr->SetPoint(0, Beta_Max, deltaPRel_UncertaintyU);
        gr->SetPoint(1, Beta_Min, deltaPRel_UncertaintyL);
        gr->Draw("same");

        const char *Rel_deltaPSaveDir = Rel_deltaPSaveNameDir.c_str();
        Canvas->SaveAs(Rel_deltaPSaveDir);
        Canvas->Clear();
        #pragma endregion

        #pragma region /* Plot w as function of beta */
        std::string WStatsTitle = "W(#beta) (" + BetaPlot.GetFinalState() + ")";
        std::string WTitle = "The W(#beta) function (" + BetaPlot.GetFinalState() + ")";
        std::string W_Maxfunc = "x*x*x - x + " + to_string(FitStd / deltaPRel_UncertaintyU);
        std::string W_Minfunc = "x*x*x - x + " + to_string(FitStd / deltaPRel_UncertaintyL);

        auto *W_Max = new TF1(WStatsTitle.c_str(), W_Maxfunc.c_str(), 0.9, 1);
        W_Max->SetLineWidth(2);
        W_Max->SetLineStyle(2);
        W_Max->SetTitle(WTitle.c_str());
        W_Max->GetXaxis()->SetTitleSize(0.06);
        W_Max->GetXaxis()->SetLabelSize(0.0425);
        W_Max->GetXaxis()->CenterTitle(true);
        W_Max->GetXaxis()->SetTitle("#beta");
        W_Max->GetYaxis()->SetRangeUser(W_yLLim, W_yULim);
        W_Max->GetYaxis()->SetTitleSize(0.06);
        W_Max->GetYaxis()->SetLabelSize(0.0425);
        W_Max->GetYaxis()->CenterTitle(true);
        W_Max->GetYaxis()->SetTitle(("W(#beta) = #beta^{3} - #beta + #delta#beta(#frac{#deltaP_{" + BetaParticleShort + "}}{P_{" + BetaParticleShort + "}})^{-1}").c_str());
        W_Max->SetLineColor(kBlack);
        W_Max->SetLineWidth(2);
        W_Max->Draw();
        Histogram_list->Add(W_Max);

        auto *W_Min = new TF1(WStatsTitle.c_str(), W_Minfunc.c_str(), 0.9, 1);
        W_Min->SetLineWidth(2);
        W_Min->SetLineStyle(10);
        W_Min->SetLineColor(kBlack);
        W_Min->SetLineWidth(2);
        W_Min->Draw("same");
        Histogram_list->Add(W_Min);

        TLine *Beta_Max_cut = new TLine(Beta_Max, W_yULim, Beta_Max, W_yLLim);
        Beta_Max_cut->SetLineWidth(2);
        Beta_Max_cut->SetLineColor(kBlue);
        Beta_Max_cut->Draw("same");

        TLine *Beta_Min_cut = new TLine(Beta_Min, W_yULim, Beta_Min, W_yLLim);
        Beta_Min_cut->SetLineWidth(2);
        Beta_Min_cut->SetLineColor(kRed);
        Beta_Min_cut->Draw("same");

        TLine *XAxis = new TLine(W_xLLim, 0., W_xULim, 0.);
        XAxis->SetLineColor(kBlack);
        XAxis->Draw("same");

        auto W_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.1, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
        TLegendEntry *W_legend_upper_lim = W_legend->AddEntry(Beta_Max_cut, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "} = 0.2").c_str(), "l");
        TLegendEntry *W_legend_lower_lim = W_legend->AddEntry(Beta_Min_cut, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "} = 0.1").c_str(), "l");
        W_legend->Draw("same");

        std::string WSaveNameDir = BetaPlot.GetHistogram1DSaveNamePath() + sNameFlag + "03_W_function.pdf";
        const char *WSaveDir = WSaveNameDir.c_str();
        Canvas->SaveAs(WSaveDir);
        Canvas->Clear();
        #pragma endregion

    } else {
        Momentum_cuts.SetUpperCut(beamE);
    }
}

// BetaFitApprax function -----------------------------------------------------------------------------------------------------------------------------------------------------

void BetaFitApprax(const std::string &SampleName, DSCuts &Beta_cut, DSCuts &Momentum_cuts, const hPlot1D &BetaPlot, TList *Histogram_list, const double beamE) {
    double W_yLLim = -0.1, W_yULim = 0.1, W_xLLim = 0.9, W_xULim = 1.;
    double deltaPRel_UncertaintyU = 0.2, deltaPRel_UncertaintyL = 0.1;

    system(("mkdir -p " + BetaPlot.GetHistogram1DSaveNamePath() + "Approximated_beta/").c_str());

    #pragma region /* Canvas definitions */
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000 * 2, 750 * 2);  // normal res
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.18);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas->cd();

    #pragma region /* Setting sNameFlag */
    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }
    #pragma endregion

    #pragma region /* Setting particle */
    std::string BetaParticle = data_processor::GetParticleName(BetaPlot.GetHistogramTitle());
    std::string BetaParticleShort = data_processor::GetParticleNameShort(BetaPlot.GetHistogramTitle());

    #pragma region /* Setting histogram */
    TH1D *hBeta = BetaPlot.GetHistogram();
    TH1D *hBeta_Clone = (TH1D *)hBeta->Clone((BetaPlot.GetHistogramStatTitle() + " - fitted").c_str());
    Int_t Color = hBeta_Clone->GetLineColor();
    #pragma endregion

    if (hBeta_Clone->Integral() != 0.) {
        #pragma region /* Preforming a fit */
        TF1 *func = new TF1("fit", FitFunction, 0, 2, 3);  // create a function with 3 parameters in the range [-3,3]
        func->SetLineColor(kRed);

        double BetaMax = hBeta_Clone->GetMaximum();
        double BetaMean = hBeta_Clone->GetMean();

        func->SetParameters(BetaMax, BetaMean, 0.001);  // start fit with histogram's max and mean
        func->SetParNames("Constant", "Mean_value", "Sigma");

        cout << "Beta histogram {max, mean}:\t{" << BetaMax << ", " << BetaMean << "}\n\n\n\n";

        // Adding limits to "Constant"
        double BetaConstantUlim = 1.2 * BetaMax;
        double BetaConstantLlim = 0.90 * BetaMax;
        //    double BetaConstantLlim = 0.67 * BetaMax;
        func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
        cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";

        // Adding limits to "Mean_value"
        //    double BetaMean_valueUlim = 1.0075;
        double BetaMean_valueUlim = 1.02;
        double BetaMean_valueLlim = 1.0075;
        //        double BetaMean_valueLlim = 1.005;
        func->SetParLimits(1, BetaMean_valueLlim, BetaMean_valueUlim);
        cout << "Beta Mean_value {Llim, Ulim}:\t{" << BetaMean_valueLlim << ", " << BetaMean_valueUlim << "}\n\n";

        //    // Adding limits to "Sigma"
        ////    double BetaSigmaUlim = 0.015;
        //    double BetaSigmaUlim = 0.01;
        //    double BetaSigmaLlim = 0.00001;
        //    func->SetParLimits(2, BetaSigmaLlim, BetaSigmaUlim);
        //    cout << "Beta Sigma {Llim, Ulim}:\t{" << BetaSigmaLlim << ", " << BetaSigmaUlim << "}\n\n";

        cout << "\n";
        hBeta_Clone->Fit("fit");
        hBeta_Clone->Draw();
        cout << "\n";

        TF1 *fit = hBeta_Clone->GetFunction("fit");
        double FitAmp = fit->GetParameter(0);   // get p0
        double FitMean = fit->GetParameter(1);  // get p1
        double FitStd = fit->GetParameter(2);   // get p2

        Beta_cut.SetUpperCut(fit->GetParameter(2));
        #pragma endregion

        #pragma region /* Drawing fit parameters and saving */
        double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
        double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;

        double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
        double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;
        //    double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
        //    double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.245;

        TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
        FitParam->SetBorderSize(1);
        FitParam->SetTextFont(0);
        FitParam->SetFillColor(0);
        FitParam->SetTextAlign(12);
        FitParam->AddText(("Fit amp = " + basic_tools::ToStringWithPrecision(FitAmp, 8)).c_str());
        FitParam->AddText(("Fit mean = " + basic_tools::ToStringWithPrecision(FitMean, 8)).c_str());
        FitParam->AddText(("Fit std = " + basic_tools::ToStringWithPrecision(FitStd, 8)).c_str());
        ((TText *)FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
        FitParam->Draw("same");

        std::string hBeta_CloneSaveNameDir = BetaPlot.GetHistogram1DSaveNamePath() + "Approximatied_beta/" + sNameFlag + BetaPlot.GetHistogram1DSaveName() + "_fitted.pdf";
        const char *hBeta_CloneSaveDir = hBeta_CloneSaveNameDir.c_str();
        Canvas->SaveAs(hBeta_CloneSaveDir);

        Canvas->Clear();
        #pragma endregion

        #pragma region /* Plot deltaP as function of beta */
        std::string deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaPlot.GetFinalState() + ")";
        std::string deltaPTitle = BetaParticle + " momentum uncertainty #deltaP_{" + BetaParticleShort + "} (" + BetaPlot.GetFinalState() + ")";
        std::string deltaPfunc = to_string(m_n * FitStd) + "/ ( (1 - x*x) * sqrt(1 - x*x) )";

        auto *deltaP = new TF1(deltaPStatsTitle.c_str(), deltaPfunc.c_str(), 0.9, 1);
        deltaP->SetTitle(deltaPTitle.c_str());
        deltaP->GetXaxis()->SetTitleSize(0.06);
        deltaP->GetXaxis()->SetLabelSize(0.0425);
        deltaP->GetXaxis()->CenterTitle(true);
        deltaP->GetXaxis()->SetTitle("#beta");
        deltaP->GetYaxis()->SetRangeUser(0., 30.);
        deltaP->GetYaxis()->SetTitleSize(0.06);
        deltaP->GetYaxis()->SetLabelSize(0.0425);
        deltaP->GetYaxis()->CenterTitle(true);
        deltaP->GetYaxis()->SetTitle(("#deltaP_{" + BetaParticleShort + "} = #frac{m_{" + BetaParticleShort + "}#delta#beta}{(1 - #beta^{2})^{3/2}}").c_str());
        //    deltaP->GetYaxis()->SetTitle(("#deltaP_{" + BetaParticleShort + "} = #frac{m_{" + BetaParticleShort + "}#cdot#delta#beta}{(1-#beta^{2})^{3/2}}").c_str());

        //    deltaP->SetLineColor(kBlack);
        deltaP->SetLineColor(Color);
        deltaP->Draw();
        Histogram_list->Add(deltaP);

        TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05 + 0.15, x_2_FitParam, y_2_FitParam + 0.15, "NDC");
        //    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.1, x_2_FitParam, y_2_FitParam, "NDC");
        //    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
        deltaPParam->SetBorderSize(1);
        deltaPParam->SetTextFont(0);
        deltaPParam->SetTextSize(0.03);
        deltaPParam->SetFillColor(0);
        deltaPParam->SetTextAlign(12);
        deltaPParam->AddText(("d#beta = " + to_string(FitStd)).c_str());
        //    deltaPParam->AddText(("#delta#beta = " + basic_tools::ToStringWithPrecision(FitStd, 8)).c_str());
        deltaPParam->Draw("same");

        std::string deltaPSaveNameDir =
            BetaPlot.GetHistogram1DSaveNamePath() + "Approximatied_beta/" + sNameFlag + "02a_P_" + BetaParticleShort + "_uncertainty_" + BetaPlot.GetFinalState() + ".pdf";
        const char *deltaPSaveDir = deltaPSaveNameDir.c_str();
        Canvas->SaveAs(deltaPSaveDir);

        Canvas->Clear();
        #pragma endregion

        #pragma region /* Solve deltaP/P for beta in range 0.9<=beta<1 */
        double Beta_Max_Apprax, P_Beta_Max_Apprax, Beta_Min_Apprax, P_Beta_Min_Apprax;

        cout << "\nSolutions for deltaP/P = 20%:\n";

        Beta_Max_Apprax = sqrt(1 - FitStd / deltaPRel_UncertaintyU);
        P_Beta_Max_Apprax = m_n * Beta_Max_Apprax / sqrt(1 - Beta_Max_Apprax * Beta_Max_Apprax);

        cout << "Beta_Max_Apprax = " << Beta_Max_Apprax << " is chosen\n";
        cout << "P(Beta_Max_Apprax) = " << P_Beta_Max_Apprax << "\n\n";

        cout << "Solutions for deltaP/P = 10%:\n";

        Beta_Min_Apprax = sqrt(1 - FitStd / deltaPRel_UncertaintyL);
        P_Beta_Min_Apprax = m_n * Beta_Min_Apprax / sqrt(1 - Beta_Min_Apprax * Beta_Min_Apprax);

        cout << "Beta_Min_Apprax = " << Beta_Min_Apprax << " is chosen\n";
        cout << "P(Beta_Min_Apprax) = " << P_Beta_Min_Apprax << "\n\n";

        ////        cout << "\n\n\n\n" << BetaPlot.GetHistogram1DSaveNamePath() << "Approximatied_beta/" << "\n\n\n\n";
        //        exit(1);

        Momentum_cuts.SetUpperCut(P_Beta_Max_Apprax);
        #pragma endregion

        #pragma region /* Plot deltaP/P as function of beta */
        std::string Rel_deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaPlot.GetFinalState() + ")";
        std::string Rel_deltaPTitle =
            BetaParticle + " relative uncertainty #deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "}" + " (apprax. ," + BetaPlot.GetFinalState() + ")";
        std::string Rel_deltaPfunc = to_string(FitStd) + "/ (1 - x*x)";

        auto *Rel_deltaP = new TF1(Rel_deltaPStatsTitle.c_str(), Rel_deltaPfunc.c_str(), 0.9, 1);
        Rel_deltaP->SetTitle(Rel_deltaPTitle.c_str());
        Rel_deltaP->GetXaxis()->SetTitleSize(0.06);
        Rel_deltaP->GetXaxis()->SetLabelSize(0.0425);
        Rel_deltaP->GetXaxis()->CenterTitle(true);
        Rel_deltaP->GetXaxis()->SetTitle("#beta");
        Rel_deltaP->GetYaxis()->SetRangeUser(0., 0.5);
        Rel_deltaP->GetYaxis()->SetTitleSize(0.06);
        Rel_deltaP->GetYaxis()->SetLabelSize(0.0425);
        Rel_deltaP->GetYaxis()->CenterTitle(true);
        Rel_deltaP->GetYaxis()->SetTitle(("#frac{#deltaP_{" + BetaParticleShort + "}}{P_{" + BetaParticleShort + "}} = #frac{#delta#beta}{1 - #beta^{2}}").c_str());
        Rel_deltaP->SetLineColor(kBlack);
        Rel_deltaP->SetLineWidth(2);
        Rel_deltaP->Draw();
        Histogram_list->Add(Rel_deltaP);

        TPaveText *deltaPRel_deltaP = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05, x_2_FitParam, y_2_FitParam, "NDC");
        deltaPRel_deltaP->SetBorderSize(1);
        deltaPRel_deltaP->SetTextFont(0);
        deltaPRel_deltaP->SetTextSize(0.03);
        deltaPRel_deltaP->SetFillColor(0);
        deltaPRel_deltaP->SetTextAlign(12);
        deltaPRel_deltaP->AddText(("d#beta = " + to_string(FitStd)).c_str());
        deltaPRel_deltaP->Draw("same");

        TLine *upper_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyU, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyU);
        upper_cut->SetLineWidth(2);
        upper_cut->SetLineColor(kBlue);
        upper_cut->Draw("same");

        TLine *lower_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyL, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyL);
        lower_cut->SetLineWidth(2);
        lower_cut->SetLineColor(kRed);
        lower_cut->Draw("same");

        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.125, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
        TLegendEntry *Cut_legend_deltaPRel_deltaP = Cut_legend->AddEntry(deltaPRel_deltaP, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "}").c_str(), "l");
        TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, "20% cut", "l");
        TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, "10% cut", "l");
        Cut_legend->Draw("same");

        std::string Rel_deltaPSaveNameDir =
            BetaPlot.GetHistogram1DSaveNamePath() + "Approximatied_beta/" + sNameFlag + "02b_P_" + BetaParticleShort + "_apprax_rel_uncertainty_" + BetaPlot.GetFinalState() + ".pdf";

        const Int_t n = 2;
        auto gr = new TGraph(n);
        gr->SetMarkerStyle(20);
        gr->SetMarkerSize(20);
        auto ex = new TExec("ex", "utilities::drawtext();");
        gr->GetListOfFunctions()->Add(ex);
        gr->SetPoint(0, Beta_Max_Apprax, deltaPRel_UncertaintyU);
        gr->SetPoint(1, Beta_Min_Apprax, deltaPRel_UncertaintyL);
        gr->Draw("same");

        const char *Rel_deltaPSaveDir = Rel_deltaPSaveNameDir.c_str();
        Canvas->SaveAs(Rel_deltaPSaveDir);
        Canvas->Clear();
        #pragma endregion

    } else {
        Momentum_cuts.SetUpperCut(beamE);
    }
}

};  // namespace fitter_functions

#endif  // FIT_FUNCTIONS_H
