//
// Created by Alon Sportes on 26/03/2025.
//

#ifndef DRAW_AND_SAVE_ACORR_PLOTS_H
#define DRAW_AND_SAVE_ACORR_PLOTS_H

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>
#include <math.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <typeinfo>
#include <vector>

#include "../../../classes/hPlots/hPlot1D.cpp"
#include "../../../namespaces/general_utilities/utilities.h"

using namespace utilities;

namespace draw_and_save_acorr_plots {

// DrawAndSaveACorrPlots function for momentum plots:
void DrawAndSaveACorrPlots(bool save_ACorr_data, const std::string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list, TList *ACorr_data,
                           std::string &ACorr_data_Dir) {
    bool plot_errorbars = true;
    bool rebin_plots = false;

#pragma region                                                     /* Canvas definitions */
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750);  // normal res
                                                                   //    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
                                                                   //    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
                                                                   //    Canvas->cd();
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas->cd();
#pragma endregion

#pragma region /* Cloning histograms */
    TH1D *Histogram1D_REC = RPlot.GetHistogram();
    std::string RPlot_Clone_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned";
    TH1D *RPlot_Clone = (TH1D *)Histogram1D_REC->Clone((RPlot_Clone_StatsTitle).c_str());
    std::string RPlot_Clone_test_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned test";
    TH1D *RPlot_Clone_test = (TH1D *)Histogram1D_REC->Clone((RPlot_Clone_test_StatsTitle).c_str());
    std::string RPlot_Clone_test_rebined_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *RPlot_Clone_test_rebined = (TH1D *)Histogram1D_REC->Clone((RPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { RPlot_Clone_test_rebined->Rebin(2); }

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    std::string TLPlot_Clone_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned";
    TH1D *TLPlot_Clone = (TH1D *)Histogram1D_Truth->Clone((TLPlot_Clone_StatsTitle).c_str());
    std::string TLPlot_Clone_test_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test";
    TH1D *TLPlot_Clone_test = (TH1D *)Histogram1D_Truth->Clone((TLPlot_Clone_test_StatsTitle).c_str());
    std::string TLPlot_Clone_test_rebined_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *TLPlot_Clone_test_rebined = (TH1D *)Histogram1D_Truth->Clone((TLPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { TLPlot_Clone_test_rebined->Rebin(2); }
#pragma endregion

#pragma region /* Setting particle */
    std::string ACorrectionRecTitle = RPlot_Clone->GetTitle();
    std::string ACorrectionParticle = data_processor::GetParticleName(ACorrectionRecTitle);
    std::string ACorrectionParticleLC = data_processor::GetParticleNameLC(ACorrectionRecTitle);
    std::string ACorrectionParticleShort = data_processor::GetParticleNameShort(ACorrectionRecTitle);
#pragma endregion

#pragma region /* Setting title */
    std::string ACorrectionType;

    if (basic_tools::FindSubstring(ACorrectionRecTitle, "momentum")) {  // for momentum ACorrection plots
        ACorrectionType = "momentum";
    }

    std::string ACorrectionTitle = ACorrectionParticle + " " + ACorrectionType;
#pragma endregion

#pragma region /* Setting X axis label */
    std::string ACorrectionXLabel;

    if (basic_tools::FindSubstring(ACorrectionRecTitle, "momentum")) {  // for momentum ACorrection plots
        ACorrectionXLabel = "P_{" + ACorrectionParticleShort + "} [GeV/c]";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "#theta")) {  // for momentum ACorrection plots
        ACorrectionXLabel = "#theta [Deg]";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "#phi")) {  // for momentum ACorrection plots
        ACorrectionXLabel = "#phi [Deg]";
    }
#pragma endregion

#pragma region /* Setting y axis label */
    std::string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    std::string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    std::string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    std::string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    std::string ACorrectionYLabel = string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabel_Truth + "/" + xLabel_REC + "^{rec}";
//    std::string ACorrectionYLabel = "#epsilon_{eff} = " + xLabel_REC + "^{rec}" + "/" + xLabel_Truth;
#pragma endregion

#pragma region /* Setting Final state */
    std::string ACorrectionFS;

    if (basic_tools::FindSubstring(ACorrectionRecTitle, "1e_cut") || basic_tools::FindSubstring(ACorrectionRecTitle, "1e cut") || basic_tools::FindSubstring(ACorrectionRecTitle, "1e Cut")) {
        ACorrectionFS = "1e cut";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "1p")) {
        ACorrectionFS = "1p";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "1n")) {
        ACorrectionFS = "1n";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "1n1p")) {
        ACorrectionFS = "1n1p";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "1e2p")) {
        ACorrectionFS = "1e2p";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "2p")) {
        ACorrectionFS = "2p";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "pFDpCD")) {
        ACorrectionFS = "pFDpCD";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "nFDpCD")) {
        ACorrectionFS = "nFDpCD";
    }
#pragma endregion

#pragma region /* Setting save directory */
    std::string ACorrectionSaveDir, ACorrectionTestSaveDir;

    if (basic_tools::FindSubstring(ACorrectionRecTitle, "Electron") || basic_tools::FindSubstring(ACorrectionRecTitle, "electron")) {
        ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/00_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
        ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
    } else {
        if (basic_tools::FindSubstring(ACorrectionRecTitle, ", FD)") || basic_tools::FindSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticle) ||
            basic_tools::FindSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticleLC)) {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/01_FD_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
        } else if (basic_tools::FindSubstring(ACorrectionRecTitle, ", CD)") || basic_tools::FindSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticle) ||
                   basic_tools::FindSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticleLC)) {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/02_CD_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
        } else {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + ACorrectionSaveDir).c_str());
    system(("mkdir -p " + ACorrectionTestSaveDir).c_str());
#pragma endregion

#pragma region /* Setting save name */
    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    std::string RPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone.pdf";
    std::string RPlot_Clone_test_SaveName = ACorrectionTestSaveDir + sNameFlag + "01a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test.pdf";
    std::string RPlot_Clone_test_rebined_SaveName =
        ACorrectionTestSaveDir + sNameFlag + "01b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test_rebined.pdf";
    std::string TLPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone.pdf";
    std::string TLPlot_Clone_test_SaveName = ACorrectionTestSaveDir + sNameFlag + "02a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test.pdf";
    std::string TLPlot_Clone_test_rebined_SaveName =
        ACorrectionTestSaveDir + sNameFlag + "02b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test_rebined.pdf";
    std::string ACorrection_plot_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_" + ACorrectionFS + ".pdf";
#pragma endregion

    TH1D *ACorrection_plot = (TH1D *)TLPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #alpha" + " (" + ACorrectionFS + ")").c_str());
    //    TH1D *ACorrection_plot = (TH1D *) RPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #epsilon_{eff}" + " (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->SetTitle((ACorrectionTitle + " acceptance correction #alpha = 1/#epsilon_{eff} (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->GetYaxis()->SetTitle((ACorrectionYLabel).c_str());
    ACorrection_plot->GetXaxis()->SetTitle((ACorrectionXLabel).c_str());

#pragma region /* Plotting and saving RPlot_Clone_test */
    RPlot_Clone_test->SetLineStyle(1);
    RPlot_Clone_test->SetLineColor(kBlue);
    RPlot_Clone_test->Draw();
    RPlot_Clone_test->SetStats(1);
    Histogram_list->Add(RPlot_Clone_test);

    Canvas->SaveAs((RPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving TLPlot_Clone_test */
    TLPlot_Clone_test->SetLineStyle(1);
    TLPlot_Clone_test->SetLineColor(kBlue);
    TLPlot_Clone_test->Draw();
    TLPlot_Clone_test->SetStats(1);
    Histogram_list->Add(TLPlot_Clone_test);

    Canvas->SaveAs((TLPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving RPlot_Clone_test_rebined */
    RPlot_Clone_test_rebined->SetLineStyle(1);
    RPlot_Clone_test_rebined->SetLineColor(kBlue);
    RPlot_Clone_test_rebined->Draw();
    RPlot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(RPlot_Clone_test_rebined);

    Canvas->SaveAs((RPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving TLPlot_Clone_test_rebined */
    TLPlot_Clone_test_rebined->SetLineStyle(1);
    TLPlot_Clone_test_rebined->SetLineColor(kBlue);
    TLPlot_Clone_test_rebined->Draw();
    TLPlot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(TLPlot_Clone_test_rebined);

    Canvas->SaveAs((TLPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving RPlot_Clone */
    RPlot_Clone->SetLineStyle(1);
    RPlot_Clone->SetLineColor(kBlue);

    if (plot_errorbars) { RPlot_Clone->Sumw2(); }

    if (rebin_plots) { RPlot_Clone->Rebin(2); }
    RPlot_Clone->Draw();
    RPlot_Clone->SetStats(1);
    Histogram_list->Add(RPlot_Clone);

    Canvas->SaveAs((RPlot_Clone_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving TLPlot_Clone */
    TLPlot_Clone->SetLineStyle(1);
    TLPlot_Clone->SetLineColor(kBlue);

    if (plot_errorbars) { TLPlot_Clone->Sumw2(); }

    if (rebin_plots) { TLPlot_Clone->Rebin(2); }
    TLPlot_Clone->Draw();
    TLPlot_Clone->SetStats(1);
    Histogram_list->Add(TLPlot_Clone);

    Canvas->SaveAs((TLPlot_Clone_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving ACorrection_plot */
    ACorrection_plot->SetLineStyle(1);
    ACorrection_plot->SetLineColor(kBlue);

    if (plot_errorbars) { ACorrection_plot->Sumw2(); }

    if (rebin_plots) { ACorrection_plot->Rebin(2); }
    ACorrection_plot->Divide(RPlot_Clone);
    //    ACorrection_plot->Divide(TLPlot_Clone);
    ACorrection_plot->Draw();
    ACorrection_plot->SetStats(0);
    Histogram_list->Add(ACorrection_plot);
    Canvas->SaveAs((ACorrection_plot_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Save acceptance correction data */
    if (save_ACorr_data) {
        system(("rm -r " + ACorr_data_Dir).c_str());     // clear old ACorr_data_Dir
        system(("mkdir -p " + ACorr_data_Dir).c_str());  // recreate ACorr_data_Dir

        std::string ACorr_data_StatsTitle;

        if ((ACorrectionFS == "pFDpCD") || (ACorrectionFS == "nFDpCD")) {
            if (basic_tools::FindSubstring(ACorrectionRecTitle, ", FD)") || basic_tools::FindSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticle) ||
                basic_tools::FindSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticleLC)) {
                ACorr_data_StatsTitle = "FD_" + ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
            } else if (basic_tools::FindSubstring(ACorrectionRecTitle, ", CD)") || basic_tools::FindSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticle) ||
                       basic_tools::FindSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticleLC)) {
                ACorr_data_StatsTitle = "CD_" + ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
            } else {
                ACorr_data_StatsTitle = ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
            }
        } else {
            ACorr_data_StatsTitle = ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
        }

        TH1D *ACorr_factor = (TH1D *)ACorrection_plot->Clone(ACorr_data_StatsTitle.c_str());

        ACorr_factor->SetLineStyle(1);
        ACorr_factor->SetLineColor(kBlue);

        if (plot_errorbars) { ACorr_factor->Sumw2(); }

        //    ACorr_factor->Rebin(2);
        //    ACorr_factor->Divide(RPlot_Clone);
        ////    ACorr_factor->Divide(TLPlot_Clone);
        ACorr_factor->Draw();
        ACorr_factor->SetStats(0);
        ACorr_data->Add(ACorr_factor);
        //        Canvas->SaveAs((ACorr_factor_SaveName).c_str());
        Canvas->Clear();
    }
#pragma endregion

    delete Canvas;
}

// DrawAndSaveACorrPlots function for angle plots:
void DrawAndSaveACorrPlots(bool save_ACorr_data, const std::string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot, TList *Histogram_list, TList *ACorr_data, std::string &ACorr_data_Dir) {
    bool plot_errorbars = true;
    bool rebin_plots = false;

#pragma region                                                     /* Canvas definitions */
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750);  // normal res
                                                                   //    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
                                                                   //    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas->cd();
#pragma endregion

#pragma region /* Setting particle */
    std::string ACorrectionRecTitle = RPlot->GetTitle();
    std::string ACorrectionParticle = data_processor::GetParticleName(ACorrectionRecTitle);
    std::string ACorrectionParticleLC = data_processor::GetParticleNameLC(ACorrectionRecTitle);
    std::string ACorrectionParticleShort = data_processor::GetParticleNameShort(ACorrectionRecTitle);
#pragma endregion

#pragma region /* Setting Final state */
    std::string ACorrectionFS;

    if (basic_tools::FindSubstring(ACorrectionRecTitle, "1e_cut") || basic_tools::FindSubstring(ACorrectionRecTitle, "1e cut") || basic_tools::FindSubstring(ACorrectionRecTitle, "1e Cut")) {
        ACorrectionFS = "1e cut";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "1p")) {
        ACorrectionFS = "1p";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "1n")) {
        ACorrectionFS = "1n";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "1n1p")) {
        ACorrectionFS = "1n1p";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "1e2p")) {
        ACorrectionFS = "1e2p";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "2p")) {
        ACorrectionFS = "2p";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "pFDpCD")) {
        ACorrectionFS = "pFDpCD";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "nFDpCD")) {
        ACorrectionFS = "nFDpCD";
    }
#pragma endregion

#pragma region /* Setting stats box title */
    std::string ACorrectionStatsType;

    if (basic_tools::FindSubstring(ACorrectionRecTitle, "#theta")) {  // for momentum ACorrection plots
        ACorrectionStatsType = "#theta_{" + ACorrectionParticleShort + "}";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "#phi")) {  // for momentum ACorrection plots
        ACorrectionStatsType = "#phi_{" + ACorrectionParticleShort + "}";
    }

    std::string ACorrectionStatsTitle = ACorrectionStatsType + " (" + ACorrectionFS + ")";
#pragma endregion

#pragma region /* Cloning histograms */
    std::string RPlot_Clone_StatsTitle = "reco. " + ACorrectionStatsTitle + " - cloned";
    TH1D *RPlot_Clone = (TH1D *)RPlot->Clone((RPlot_Clone_StatsTitle).c_str());
    std::string RPlot_Clone_test_StatsTitle = "reco. " + ACorrectionStatsTitle + " - cloned test";
    TH1D *RPlot_Clone_test = (TH1D *)RPlot->Clone((RPlot_Clone_test_StatsTitle).c_str());
    std::string RPlot_Clone_test_rebined_StatsTitle = "reco. " + ACorrectionStatsTitle + " - cloned test rebined";
    TH1D *RPlot_Clone_test_rebined = (TH1D *)RPlot->Clone((RPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { RPlot_Clone_test_rebined->Rebin(2); }

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    std::string TLPlot_Clone_StatsTitle = "Truth " + ACorrectionStatsTitle + " - cloned";
    TH1D *TLPlot_Clone = (TH1D *)Histogram1D_Truth->Clone((TLPlot_Clone_StatsTitle).c_str());
    std::string TLPlot_Clone_test_StatsTitle = "Truth " + ACorrectionStatsTitle + " - cloned test";
    TH1D *TLPlot_Clone_test = (TH1D *)Histogram1D_Truth->Clone((TLPlot_Clone_test_StatsTitle).c_str());
    std::string TLPlot_Clone_test_rebined_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *TLPlot_Clone_test_rebined = (TH1D *)Histogram1D_Truth->Clone((TLPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { TLPlot_Clone_test_rebined->Rebin(2); }
#pragma endregion

#pragma region /* Setting title */
    std::string ACorrectionType, ACorrectionTitle;

    if (basic_tools::FindSubstring(ACorrectionRecTitle, "momentum")) {  // for momentum ACorrection plots
        ACorrectionType = "momentum";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "#theta")) {  // for theta ACorrection plots
        ACorrectionType = "theta";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "#phi")) {  // for phi ACorrection plots
        ACorrectionType = "phi";
    }

    if (ACorrectionType == "momentum") {
        ACorrectionTitle = ACorrectionParticle + " " + ACorrectionType;
    } else {
        ACorrectionTitle = "#" + ACorrectionType + "_{" + ACorrectionParticleShort + "}";
    }

//    std::string ACorrectionTitle = ACorrectionParticle + " " + ACorrectionType;
#pragma endregion

#pragma region /* Setting X axis label */
    std::string ACorrectionXLabel;

    if (basic_tools::FindSubstring(ACorrectionRecTitle, "momentum")) {  // for momentum ACorrection plots
        ACorrectionXLabel = "P_{" + ACorrectionParticleShort + "} [GeV/c]";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "#theta")) {  // for momentum ACorrection plots
        ACorrectionXLabel = ACorrectionTitle + " [Deg]";
        //        ACorrectionXLabel = "#theta [Deg]";
    } else if (basic_tools::FindSubstring(ACorrectionRecTitle, "#phi")) {  // for momentum ACorrection plots
        ACorrectionXLabel = ACorrectionTitle + " [Deg]";
        //        ACorrectionXLabel = "#phi [Deg]";
    }
#pragma endregion

#pragma region /* Setting y axis label */
    std::string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    std::string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    std::string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    std::string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    std::string ACorrectionYLabel = string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabel_Truth + "/" + xLabel_REC + "^{rec}";
//    std::string ACorrectionYLabel = "#epsilon_{eff} = " + xLabel_REC + "^{rec}" + "/" + xLabel_Truth;
#pragma endregion

#pragma region /* Setting save directory */
    std::string ACorrectionSaveDir, ACorrectionTestSaveDir;

    if (basic_tools::FindSubstring(ACorrectionRecTitle, "Electron") || basic_tools::FindSubstring(ACorrectionRecTitle, "electron")) {
        ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/00_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
        ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
    } else {
        if (basic_tools::FindSubstring(ACorrectionRecTitle, ", FD)") || basic_tools::FindSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticle) ||
            basic_tools::FindSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticleLC)) {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/01_FD_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
        } else if (basic_tools::FindSubstring(ACorrectionRecTitle, ", CD)") || basic_tools::FindSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticle) ||
                   basic_tools::FindSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticleLC)) {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/02_CD_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
        } else {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + ACorrectionSaveDir).c_str());
    system(("mkdir -p " + ACorrectionTestSaveDir).c_str());
#pragma endregion

#pragma region /* Setting save name */
    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    std::string RPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone.pdf";
    std::string RPlot_Clone_test_SaveName = ACorrectionTestSaveDir + sNameFlag + "01a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test.pdf";
    std::string RPlot_Clone_test_rebined_SaveName =
        ACorrectionTestSaveDir + sNameFlag + "01b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test_rebined.pdf";
    std::string TLPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone.pdf";
    std::string TLPlot_Clone_test_SaveName = ACorrectionTestSaveDir + sNameFlag + "02a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test.pdf";
    std::string TLPlot_Clone_test_rebined_SaveName =
        ACorrectionTestSaveDir + sNameFlag + "02b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test_rebined.pdf";
    std::string ACorrection_plot_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_" + ACorrectionFS + ".pdf";
#pragma endregion

    TH1D *ACorrection_plot = (TH1D *)TLPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #alpha" + " (" + ACorrectionFS + ")").c_str());
    //    TH1D *ACorrection_plot = (TH1D *) RPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #epsilon_{eff}" + " (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->SetTitle((ACorrectionTitle + " acceptance correction #alpha = 1/#epsilon_{eff} (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->GetYaxis()->SetTitle((ACorrectionYLabel).c_str());
    ACorrection_plot->GetXaxis()->SetTitle((ACorrectionXLabel).c_str());

#pragma region /* Plotting and saving RPlot_Clone_test */
    RPlot_Clone_test->SetLineStyle(1);
    RPlot_Clone_test->SetLineColor(kBlue);
    RPlot_Clone_test->SetStats(1);
    RPlot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    RPlot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test->GetXaxis()->CenterTitle(true);
    RPlot_Clone_test->GetYaxis()->SetTitle("Number of events");
    RPlot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    RPlot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test->GetYaxis()->CenterTitle(true);
    RPlot_Clone_test->SetLineWidth(2);
    RPlot_Clone_test->Draw();
    RPlot_Clone_test->SetStats(1);
    Histogram_list->Add(RPlot_Clone_test);

    Canvas->SaveAs((RPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving TLPlot_Clone_test */
    TLPlot_Clone_test->SetLineStyle(1);
    TLPlot_Clone_test->SetLineColor(kBlue);
    TLPlot_Clone_test->Draw();
    TLPlot_Clone_test->SetStats(1);
    Histogram_list->Add(TLPlot_Clone_test);

    Canvas->SaveAs((TLPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving RPlot_Clone_test_rebined */
    RPlot_Clone_test_rebined->SetLineStyle(1);
    RPlot_Clone_test_rebined->SetLineColor(kBlue);
    RPlot_Clone_test_rebined->SetStats(1);
    RPlot_Clone_test_rebined->GetXaxis()->SetTitleSize(0.06);
    RPlot_Clone_test_rebined->GetXaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test_rebined->GetXaxis()->CenterTitle(true);
    RPlot_Clone_test_rebined->GetYaxis()->SetTitle("Number of events");
    RPlot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    RPlot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    RPlot_Clone_test_rebined->SetLineWidth(2);
    RPlot_Clone_test_rebined->Draw();
    RPlot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(RPlot_Clone_test_rebined);

    Canvas->SaveAs((RPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving TLPlot_Clone_test_rebined */
    TLPlot_Clone_test_rebined->SetLineStyle(1);
    TLPlot_Clone_test_rebined->SetLineColor(kBlue);
    TLPlot_Clone_test_rebined->Draw();
    TLPlot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(TLPlot_Clone_test_rebined);

    Canvas->SaveAs((TLPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving RPlot_Clone */
    RPlot_Clone->SetLineStyle(1);
    RPlot_Clone->SetLineColor(kBlue);
    RPlot_Clone->SetStats(1);

    RPlot_Clone->GetXaxis()->SetTitleSize(0.06);
    RPlot_Clone->GetXaxis()->SetLabelSize(0.0425);
    RPlot_Clone->GetXaxis()->CenterTitle(true);
    RPlot_Clone->GetYaxis()->SetTitle("Number of events");
    RPlot_Clone->GetYaxis()->SetTitleSize(0.06);
    RPlot_Clone->GetYaxis()->SetLabelSize(0.0425);
    RPlot_Clone->GetYaxis()->CenterTitle(true);
    RPlot_Clone->SetLineWidth(2);

    if (plot_errorbars) { RPlot_Clone->Sumw2(); }

    if (rebin_plots) { RPlot_Clone->Rebin(2); }
    RPlot_Clone->Draw();
    RPlot_Clone->SetStats(1);
    Histogram_list->Add(RPlot_Clone);

    Canvas->SaveAs((RPlot_Clone_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving TLPlot_Clone */
    TLPlot_Clone->SetLineStyle(1);
    TLPlot_Clone->SetLineColor(kBlue);

    if (plot_errorbars) { TLPlot_Clone->Sumw2(); }

    if (rebin_plots) { TLPlot_Clone->Rebin(2); }
    TLPlot_Clone->Draw();
    TLPlot_Clone->SetStats(1);
    Histogram_list->Add(TLPlot_Clone);

    Canvas->SaveAs((TLPlot_Clone_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Plotting and saving ACorrection_plot */
    ACorrection_plot->SetLineStyle(1);
    ACorrection_plot->SetLineColor(kBlue);

    ACorrection_plot->GetXaxis()->SetTitleSize(0.06);
    ACorrection_plot->GetXaxis()->SetLabelSize(0.0425);
    ACorrection_plot->GetXaxis()->CenterTitle(true);
    ACorrection_plot->GetYaxis()->SetTitleSize(0.06);
    ACorrection_plot->GetYaxis()->SetLabelSize(0.0425);
    ACorrection_plot->GetYaxis()->CenterTitle(true);
    ACorrection_plot->SetLineWidth(2);

    if (plot_errorbars) { ACorrection_plot->Sumw2(); }

    if (rebin_plots) { ACorrection_plot->Rebin(2); }
    ACorrection_plot->Divide(RPlot_Clone);
    //    ACorrection_plot->Divide(TLPlot_Clone);
    ACorrection_plot->SetStats(0);
    ACorrection_plot->Draw();

    Histogram_list->Add(ACorrection_plot);
    Canvas->SaveAs((ACorrection_plot_SaveName).c_str());
    Canvas->Clear();
#pragma endregion

#pragma region /* Save acceptance correction data */
    if (save_ACorr_data) {
        system(("rm -r " + ACorr_data_Dir).c_str());     // clear old ACorr_data_Dir
        system(("mkdir -p " + ACorr_data_Dir).c_str());  // recreate ACorr_data_Dir

        std::string ACorr_data_StatsTitle;

        if (((ACorrectionFS == "pFDpCD") || (ACorrectionFS == "nFDpCD")) && (ACorrectionParticle != "Electron")) {
            if (basic_tools::FindSubstring(ACorrectionRecTitle, ", FD)") || basic_tools::FindSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticle) ||
                basic_tools::FindSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticleLC)) {
                ACorr_data_StatsTitle = "FD_" + ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
            } else if (basic_tools::FindSubstring(ACorrectionRecTitle, ", CD)") || basic_tools::FindSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticle) ||
                       basic_tools::FindSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticleLC)) {
                ACorr_data_StatsTitle = "CD_" + ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
            } else {
                ACorr_data_StatsTitle = ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
            }
        } else {
            ACorr_data_StatsTitle = ACorrectionParticle + "_" + ACorrectionType + "_" + "ACorr_" + ACorrectionFS;
        }

        TH1D *ACorr_factor = (TH1D *)ACorrection_plot->Clone(ACorr_data_StatsTitle.c_str());

        ACorr_factor->SetLineStyle(1);
        ACorr_factor->SetLineColor(kBlue);

        if (plot_errorbars) { ACorr_factor->Sumw2(); }

        //    ACorr_factor->Rebin(2);
        //    ACorr_factor->Divide(RPlot_Clone);
        ////    ACorr_factor->Divide(TLPlot_Clone);
        ACorr_factor->Draw();
        ACorr_factor->SetStats(0);
        ACorr_data->Add(ACorr_factor);
        //        Canvas->SaveAs((ACorr_factor_SaveName).c_str());
        Canvas->Clear();
    }
#pragma endregion

    delete Canvas;
}
};  // namespace draw_and_save_acorr_plots

#endif  // DRAW_AND_SAVE_ACORR_PLOTS_H
