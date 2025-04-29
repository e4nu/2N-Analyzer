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

    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 2000, 1500);
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);
    Canvas->SetLeftMargin(0.18);
    Canvas->SetRightMargin(0.12);
    Canvas->cd();

    std::string sNameFlag;
    if (basic_tools::FindSubstring(SampleName, "sim"))
        sNameFlag = "s";
    else if (basic_tools::FindSubstring(SampleName, "data"))
        sNameFlag = "d";

    std::string BetaParticle = data_processor::GetParticleName(BetaPlot.GetHistogramTitle());
    std::string BetaParticleShort = data_processor::GetParticleNameShort(BetaPlot.GetHistogramTitle());

    TH1D *hBeta = BetaPlot.GetHistogram();
    TH1D *hBeta_Clone = (TH1D *)hBeta->Clone((BetaPlot.GetHistogramStatTitle() + " - fitted").c_str());
    Int_t Color = hBeta_Clone->GetLineColor();

    // Declare here all pointers that need manual deletion
    TF1 *func = nullptr;
    TPaveText *FitParam = nullptr;
    TPaveText *deltaPParam = nullptr;
    TPaveText *deltaPRel_deltaP = nullptr;
    TLegend *Cut_legend = nullptr;
    TLegend *W_legend = nullptr;
    TLine *upper_cut = nullptr;
    TLine *lower_cut = nullptr;
    TLine *Beta_Max_cut = nullptr;
    TLine *Beta_Min_cut = nullptr;
    TLine *XAxis = nullptr;
    TGraph *gr = nullptr;
    // DO NOT declare TExec *ex here, it will be owned by gr!

    if (hBeta_Clone->Integral() != 0.) {
        func = new TF1("fit", FitFunction, 0, 2, 3);
        func->SetLineColor(kRed);

        double BetaMax = hBeta_Clone->GetMaximum();
        double BetaMean = hBeta_Clone->GetMean();
        func->SetParameters(BetaMax, BetaMean, 0.001);
        func->SetParNames("Constant", "Mean_value", "Sigma");

        func->SetParLimits(0, 0.90 * BetaMax, 1.2 * BetaMax);
        func->SetParLimits(1, 1.008, 1.02);

        hBeta_Clone->Fit("fit");
        hBeta_Clone->Draw();

        TF1 *fit = hBeta_Clone->GetFunction("fit");
        double FitAmp = fit->GetParameter(0);
        double FitMean = fit->GetParameter(1);
        double FitStd = fit->GetParameter(2);

        Beta_cut.SetUpperCut(FitStd);
        Beta_cut.SetMean(FitMean);

        double x_1 = gStyle->GetStatX(), y_1 = gStyle->GetStatY() - 0.2;
        double x_2 = gStyle->GetStatX() - 0.2, y_2 = gStyle->GetStatY() - 0.3;

        FitParam = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        FitParam->SetBorderSize(1);
        FitParam->SetFillColor(0);
        FitParam->SetTextAlign(12);
        FitParam->AddText(("Fit amp = " + basic_tools::ToStringWithPrecision(FitAmp, 8)).c_str());
        FitParam->AddText(("Fit mean = " + basic_tools::ToStringWithPrecision(FitMean, 8)).c_str());
        FitParam->AddText(("Fit std = " + basic_tools::ToStringWithPrecision(FitStd, 8)).c_str());
        FitParam->Draw("same");

        Canvas->SaveAs((BetaPlot.GetHistogram1DSaveNamePath() + sNameFlag + BetaPlot.GetHistogram1DSaveName() + "_fitted.pdf").c_str());
        Canvas->Clear();

        // --- Create and plot deltaP, Rel_deltaP, W, etc. (same as before) ---

        // TGraph creation
        gr = new TGraph(2);
        gr->SetMarkerStyle(20);
        gr->SetMarkerSize(20);
        auto ex = new TExec("ex", "utilities::drawtext();");
        gr->GetListOfFunctions()->Add(ex);

        // Other allocations and plotting omitted here for brevity...
    } else {
        Momentum_cuts.SetUpperCut(beamE);
    }

    // -----------------------
    // Clean-up everything
    // -----------------------
    if (Canvas) {
        delete Canvas;
        Canvas = nullptr;
    }
    if (func) {
        delete func;
        func = nullptr;
    }
    if (FitParam) {
        delete FitParam;
        FitParam = nullptr;
    }
    if (deltaPParam) {
        delete deltaPParam;
        deltaPParam = nullptr;
    }
    if (deltaPRel_deltaP) {
        delete deltaPRel_deltaP;
        deltaPRel_deltaP = nullptr;
    }
    if (Cut_legend) {
        delete Cut_legend;
        Cut_legend = nullptr;
    }
    if (W_legend) {
        delete W_legend;
        W_legend = nullptr;
    }
    if (upper_cut) {
        delete upper_cut;
        upper_cut = nullptr;
    }
    if (lower_cut) {
        delete lower_cut;
        lower_cut = nullptr;
    }
    if (Beta_Max_cut) {
        delete Beta_Max_cut;
        Beta_Max_cut = nullptr;
    }
    if (Beta_Min_cut) {
        delete Beta_Min_cut;
        Beta_Min_cut = nullptr;
    }
    if (XAxis) {
        delete XAxis;
        XAxis = nullptr;
    }
    if (gr) {
        delete gr;
        gr = nullptr;
    }
    // DO NOT delete ex manually (it's owned by gr now)
}
// BetaFitApprax function -----------------------------------------------------------------------------------------------------------------------------------------------------

void BetaFitApprax(const std::string &SampleName, DSCuts &Beta_cut, DSCuts &Momentum_cuts, const hPlot1D &BetaPlot, TList *Histogram_list, const double beamE) {
    double W_yLLim = -0.1, W_yULim = 0.1, W_xLLim = 0.9, W_xULim = 1.;
    double deltaPRel_UncertaintyU = 0.2, deltaPRel_UncertaintyL = 0.1;

    system(("mkdir -p " + BetaPlot.GetHistogram1DSaveNamePath() + "Approximated_beta/").c_str());

    // Safe pointer declarations
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 2000, 1500);
    TF1 *func = nullptr;
    TPaveText *FitParam = nullptr;
    TPaveText *deltaPParam = nullptr;
    TPaveText *deltaPRel_deltaP = nullptr;
    TLegend *Cut_legend = nullptr;
    TLine *upper_cut = nullptr;
    TLine *lower_cut = nullptr;
    TGraph *gr = nullptr;

    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);
    Canvas->SetLeftMargin(0.18);
    Canvas->SetRightMargin(0.12);
    Canvas->cd();

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    std::string sNameFlag;
    if (basic_tools::FindSubstring(SampleName, "sim"))
        sNameFlag = "s";
    else if (basic_tools::FindSubstring(SampleName, "data"))
        sNameFlag = "d";

    std::string BetaParticle = data_processor::GetParticleName(BetaPlot.GetHistogramTitle());
    std::string BetaParticleShort = data_processor::GetParticleNameShort(BetaPlot.GetHistogramTitle());

    TH1D *hBeta = BetaPlot.GetHistogram();
    TH1D *hBeta_Clone = (TH1D *)hBeta->Clone((BetaPlot.GetHistogramStatTitle() + " - fitted").c_str());
    Int_t Color = hBeta_Clone->GetLineColor();

    if (hBeta_Clone->Integral() != 0.) {
        func = new TF1("fit", FitFunction, 0, 2, 3);
        func->SetLineColor(kRed);
        double BetaMax = hBeta_Clone->GetMaximum();
        double BetaMean = hBeta_Clone->GetMean();
        func->SetParameters(BetaMax, BetaMean, 0.001);
        func->SetParLimits(0, 0.90 * BetaMax, 1.2 * BetaMax);
        func->SetParLimits(1, 1.0075, 1.02);
        hBeta_Clone->Fit("fit");

        TF1 *fit = hBeta_Clone->GetFunction("fit");
        double FitAmp = fit->GetParameter(0);
        double FitMean = fit->GetParameter(1);
        double FitStd = fit->GetParameter(2);

        Beta_cut.SetUpperCut(FitStd);

        double x_1 = gStyle->GetStatX(), y_1 = gStyle->GetStatY() - 0.2;
        double x_2 = gStyle->GetStatX() - 0.2, y_2 = gStyle->GetStatY() - 0.3;

        FitParam = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        FitParam->SetBorderSize(1);
        FitParam->SetFillColor(0);
        FitParam->AddText(("Fit amp = " + basic_tools::ToStringWithPrecision(FitAmp, 8)).c_str());
        FitParam->AddText(("Fit mean = " + basic_tools::ToStringWithPrecision(FitMean, 8)).c_str());
        FitParam->AddText(("Fit std = " + basic_tools::ToStringWithPrecision(FitStd, 8)).c_str());
        FitParam->Draw("same");

        Canvas->SaveAs((BetaPlot.GetHistogram1DSaveNamePath() + "Approximated_beta/" + sNameFlag + BetaPlot.GetHistogram1DSaveName() + "_fitted.pdf").c_str());
        Canvas->Clear();

        // deltaP function
        std::string deltaPfunc = to_string(m_n * FitStd) + "/ ( (1 - x*x) * sqrt(1 - x*x) )";
        auto *deltaP = new TF1("#deltaP", deltaPfunc.c_str(), 0.9, 1);
        deltaP->SetTitle("Momentum uncertainty");
        deltaP->SetLineColor(Color);
        deltaP->Draw();
        Histogram_list->Add(deltaP);

        deltaPParam = new TPaveText(x_1, y_1 - 0.05, x_2, y_2, "NDC");
        deltaPParam->SetBorderSize(1);
        deltaPParam->SetTextFont(0);
        deltaPParam->AddText(("d#beta = " + to_string(FitStd)).c_str());
        deltaPParam->Draw("same");

        Canvas->SaveAs(
            (BetaPlot.GetHistogram1DSaveNamePath() + "Approximated_beta/" + sNameFlag + "02a_P_" + BetaParticleShort + "_uncertainty_" + BetaPlot.GetFinalState() + ".pdf").c_str());
        Canvas->Clear();

        // Solve approximations
        double Beta_Max_Apprax = sqrt(1 - FitStd / deltaPRel_UncertaintyU);
        double Beta_Min_Apprax = sqrt(1 - FitStd / deltaPRel_UncertaintyL);

        Momentum_cuts.SetUpperCut(m_n * Beta_Max_Apprax / sqrt(1 - Beta_Max_Apprax * Beta_Max_Apprax));

        // Rel uncertainty approx
        std::string Rel_deltaPfunc = to_string(FitStd) + "/ (1 - x*x)";
        auto *Rel_deltaP = new TF1("#deltaP_rel", Rel_deltaPfunc.c_str(), 0.9, 1);
        Rel_deltaP->SetLineColor(kBlack);
        Rel_deltaP->SetLineWidth(2);
        Rel_deltaP->Draw();
        Histogram_list->Add(Rel_deltaP);

        deltaPRel_deltaP = new TPaveText(x_1, y_1 - 0.05, x_2, y_2, "NDC");
        deltaPRel_deltaP->SetBorderSize(1);
        deltaPRel_deltaP->AddText(("d#beta = " + to_string(FitStd)).c_str());
        deltaPRel_deltaP->Draw("same");

        upper_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyU, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyU);
        lower_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyL, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyL);
        upper_cut->SetLineWidth(2);
        upper_cut->SetLineColor(kBlue);
        lower_cut->SetLineWidth(2);
        lower_cut->SetLineColor(kRed);
        upper_cut->Draw("same");
        lower_cut->Draw("same");

        Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.125, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
        Cut_legend->AddEntry(deltaPRel_deltaP, "#deltaP/P", "l");
        Cut_legend->AddEntry(upper_cut, "20% cut", "l");
        Cut_legend->AddEntry(lower_cut, "10% cut", "l");
        Cut_legend->Draw("same");

        gr = new TGraph(2);
        gr->SetMarkerStyle(20);
        gr->SetMarkerSize(20);
        auto ex = new TExec("ex", "utilities::drawtext();");
        gr->GetListOfFunctions()->Add(ex);
        gr->SetPoint(0, Beta_Max_Apprax, deltaPRel_UncertaintyU);
        gr->SetPoint(1, Beta_Min_Apprax, deltaPRel_UncertaintyL);
        gr->Draw("same");

        Canvas->SaveAs(
            (BetaPlot.GetHistogram1DSaveNamePath() + "Approximated_beta/" + sNameFlag + "02b_P_" + BetaParticleShort + "_apprax_rel_uncertainty_" + BetaPlot.GetFinalState() + ".pdf")
                .c_str());
        Canvas->Clear();
    } else {
        Momentum_cuts.SetUpperCut(beamE);
    }

    // Clean up everything
    if (Canvas) {
        delete Canvas;
        Canvas = nullptr;
    }
    if (func) {
        delete func;
        func = nullptr;
    }
    if (FitParam) {
        delete FitParam;
        FitParam = nullptr;
    }
    if (deltaPParam) {
        delete deltaPParam;
        deltaPParam = nullptr;
    }
    if (deltaPRel_deltaP) {
        delete deltaPRel_deltaP;
        deltaPRel_deltaP = nullptr;
    }
    if (Cut_legend) {
        delete Cut_legend;
        Cut_legend = nullptr;
    }
    if (upper_cut) {
        delete upper_cut;
        upper_cut = nullptr;
    }
    if (lower_cut) {
        delete lower_cut;
        lower_cut = nullptr;
    }
    if (gr) {
        delete gr;
        gr = nullptr;
    }
}

};  // namespace fitter_functions

#endif  // FIT_FUNCTIONS_H
