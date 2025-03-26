//
// Created by Alon Sportes on 26/03/2025.
//

#ifndef DRAW_AND_SAVE_EFFICIENCY_PLOTS_H
#define DRAW_AND_SAVE_EFFICIENCY_PLOTS_H

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
#include "../../../namespaces/general_utilities/utilities/utilities.h"

using namespace std;
using namespace utilities;

namespace draw_and_save_efficiency_plots {
void DrawAndSaveEfficiencyPlots(const std::string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list) {
    bool plot_errorbars = true;
    bool rebin_plots = false;

    //<editor-fold desc="Canvas definitions">
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
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
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
    //</editor-fold>

    //<editor-fold desc="Setting particle">
    std::string EfficiencyRecTitle = RPlot_Clone->GetTitle();
    std::string EfficiencyParticle = data_processor::GetParticleName(EfficiencyRecTitle);
    std::string EfficiencyParticleDir = data_processor::GetParticleName(EfficiencyRecTitle, true);
    std::string EfficiencyParticleLC = data_processor::GetParticleNameLC(EfficiencyRecTitle);
    std::string EfficiencyParticleShort = data_processor::GetParticleNameShort(EfficiencyRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting region">
    std::string EfficiencyParticleRegion = "", EfficiencyParticleRegionDir = "";

    std::string Region = RPlot.GetDetectorRegion();

    if ((Region != "") && (EfficiencyParticle != "Electron")) {
        EfficiencyParticleRegion = Region + " ";
        EfficiencyParticleRegionDir = Region;
    }
    //</editor-fold>

    //<editor-fold desc="Setting title">
    std::string EfficiencyType;

    if (basic_tools::FindSubstring(EfficiencyRecTitle, "momentum")) {  // for momentum efficiency plots
        EfficiencyType = "momentum";
    }

    std::string EfficiencyTitle = EfficiencyParticleRegion + EfficiencyParticle + " " + EfficiencyType;
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    std::string EfficiencyXLabel;

    if (basic_tools::FindSubstring(EfficiencyRecTitle, "momentum")) {  // for momentum efficiency plots
        EfficiencyXLabel = "P_{" + EfficiencyParticleShort + "} [GeV/c]";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "#theta")) {  // for momentum efficiency plots
        EfficiencyXLabel = "#theta [Deg]";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "#phi")) {  // for momentum efficiency plots
        EfficiencyXLabel = "#phi [Deg]";
    }
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    std::string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    std::string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    std::string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    std::string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    std::string EfficiencyYLabel = "#epsilon_{eff} = #frac{reco.}{truth}";
    //</editor-fold>

    //<editor-fold desc="Setting Final state">
    std::string EfficiencyFS, EfficiencyFSDir;

    if (basic_tools::FindSubstring(EfficiencyRecTitle, "1e_cut") || basic_tools::FindSubstring(EfficiencyRecTitle, "1e cut") || basic_tools::FindSubstring(EfficiencyRecTitle, "1e Cut")) {
        EfficiencyFS = "1e cut";
        EfficiencyFSDir = "1e_cut";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "1p")) {
        EfficiencyFS = "1p";
        EfficiencyFSDir = EfficiencyFS;
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "1n")) {
        EfficiencyFS = "1n";
        EfficiencyFSDir = EfficiencyFS;

    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "1n1p")) {
        EfficiencyFS = "1n1p";
        EfficiencyFSDir = EfficiencyFS;

    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "1e2p")) {
        EfficiencyFS = "1e2p";
        EfficiencyFSDir = EfficiencyFS;

    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "2p")) {
        EfficiencyFS = "2p";
        EfficiencyFSDir = EfficiencyFS;

    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "pFDpCD")) {
        EfficiencyFS = "pFDpCD";
        EfficiencyFSDir = EfficiencyFS;

    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "nFDpCD")) {
        EfficiencyFS = "nFDpCD";
        EfficiencyFSDir = EfficiencyFS;
    }
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    std::string EfficiencySaveDir, EfficiencyTestSaveDir;

    if (basic_tools::FindSubstring(EfficiencyRecTitle, "Electron") || basic_tools::FindSubstring(EfficiencyRecTitle, "electron")) {
        EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/00_" + EfficiencyParticleDir + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFSDir + "/";
        EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test/";
    } else {
        if (basic_tools::FindSubstring(EfficiencyRecTitle, ", FD)") || basic_tools::FindSubstring(EfficiencyRecTitle, "FD " + EfficiencyParticle) ||
            basic_tools::FindSubstring(EfficiencyRecTitle, "FD " + EfficiencyParticleLC)) {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/01_FD_" + EfficiencyParticleDir + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFSDir + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "_FD/";
        } else if (basic_tools::FindSubstring(EfficiencyRecTitle, ", CD)") || basic_tools::FindSubstring(EfficiencyRecTitle, "CD " + EfficiencyParticle) ||
                   basic_tools::FindSubstring(EfficiencyRecTitle, "CD " + EfficiencyParticleLC)) {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/02_CD_" + EfficiencyParticleDir + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFSDir + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "_CD/";
        } else {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/" + EfficiencyParticleDir + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFSDir + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + EfficiencySaveDir).c_str());
    system(("mkdir -p " + EfficiencyTestSaveDir).c_str());
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    std::string RPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFSDir + "_" + "Rec_Clone.png";
    std::string RPlot_Clone_test_SaveName = EfficiencyTestSaveDir + sNameFlag + "01a_" + EfficiencyParticleDir + "_" + EfficiencyType + "_" + EfficiencyFSDir + "_" + "Rec_Clone_test.png";
    std::string RPlot_Clone_test_rebined_SaveName =
        EfficiencyTestSaveDir + sNameFlag + "01b_" + EfficiencyParticleDir + "_" + EfficiencyType + "_" + EfficiencyFSDir + "_" + "Rec_Clone_test_rebined.png";
    std::string TLPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticleDir + "_" + EfficiencyType + "_" + EfficiencyFSDir + "_" + "Truth_Clone.png";
    std::string TLPlot_Clone_test_SaveName = EfficiencyTestSaveDir + sNameFlag + "02a_" + EfficiencyParticleDir + "_" + EfficiencyType + "_" + EfficiencyFSDir + "_" + "Truth_Clone_test.png";
    std::string TLPlot_Clone_test_rebined_SaveName =
        EfficiencyTestSaveDir + sNameFlag + "02b_" + EfficiencyParticleDir + "_" + EfficiencyType + "_" + EfficiencyFSDir + "_" + "Truth_Clone_test_rebined.png";
    std::string Efficiency_plot_SaveName =
        EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticleRegionDir + +"_" + EfficiencyParticleDir + "_" + EfficiencyType + "_efficiency_" + EfficiencyFSDir + ".png";
    //</editor-fold>

    TH1D *Efficiency_plot = (TH1D *)Histogram1D_REC->Clone((EfficiencyParticleRegion + EfficiencyParticle + " " + EfficiencyType + " #epsilon_{eff}" + " (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->SetTitle((EfficiencyTitle + " efficiency #epsilon_{eff} (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->GetYaxis()->SetTitle((EfficiencyYLabel).c_str());
    Efficiency_plot->GetXaxis()->SetTitle((EfficiencyXLabel).c_str());

    std::string Efficiency_plot_Name = Efficiency_plot->GetName();
    std::string TFolder_Name = Efficiency_plot_Name + " folder";
    TFolder *EfficiencyComponentPlots = new TFolder(TFolder_Name.c_str(), TFolder_Name.c_str());

    //<editor-fold desc="Plotting and saving RPlot_Clone_test">
    RPlot_Clone_test->SetLineStyle(1);
    RPlot_Clone_test->SetLineColor(kBlue);
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
    EfficiencyComponentPlots->Add(RPlot_Clone_test);
    //    Histogram_list->Add(RPlot_Clone_test);

    Canvas->SaveAs((RPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone_test">
    TLPlot_Clone_test->SetLineStyle(1);
    TLPlot_Clone_test->SetLineColor(kBlue);
    TLPlot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    TLPlot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    TLPlot_Clone_test->GetXaxis()->CenterTitle(true);
    TLPlot_Clone_test->GetYaxis()->SetTitle("Number of events");
    TLPlot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    TLPlot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    TLPlot_Clone_test->GetYaxis()->CenterTitle(true);
    TLPlot_Clone_test->SetLineWidth(2);
    TLPlot_Clone_test->Draw();
    TLPlot_Clone_test->SetStats(1);
    EfficiencyComponentPlots->Add(TLPlot_Clone_test);
    //    Histogram_list->Add(TLPlot_Clone_test);

    Canvas->SaveAs((TLPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving RPlot_Clone_test_rebined">
    RPlot_Clone_test_rebined->SetLineStyle(1);
    RPlot_Clone_test_rebined->SetLineColor(kBlue);
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
    EfficiencyComponentPlots->Add(RPlot_Clone_test_rebined);
    //    Histogram_list->Add(RPlot_Clone_test_rebined);

    Canvas->SaveAs((RPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone_test_rebined">
    TLPlot_Clone_test_rebined->SetLineStyle(1);
    TLPlot_Clone_test_rebined->SetLineColor(kBlue);
    TLPlot_Clone_test_rebined->GetXaxis()->SetTitleSize(0.06);
    TLPlot_Clone_test_rebined->GetXaxis()->SetLabelSize(0.0425);
    TLPlot_Clone_test_rebined->GetXaxis()->CenterTitle(true);
    TLPlot_Clone_test_rebined->GetYaxis()->SetTitle("Number of events");
    TLPlot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    TLPlot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    TLPlot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    TLPlot_Clone_test_rebined->SetLineWidth(2);
    TLPlot_Clone_test_rebined->Draw();
    TLPlot_Clone_test_rebined->SetStats(1);
    EfficiencyComponentPlots->Add(TLPlot_Clone_test_rebined);
    //    Histogram_list->Add(TLPlot_Clone_test_rebined);

    Canvas->SaveAs((TLPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving RPlot_Clone">
    RPlot_Clone->SetLineStyle(1);
    RPlot_Clone->SetLineColor(kBlue);
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
    EfficiencyComponentPlots->Add(RPlot_Clone);
    //    Histogram_list->Add(RPlot_Clone);

    Canvas->SaveAs((RPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone">
    TLPlot_Clone->SetLineStyle(1);
    TLPlot_Clone->SetLineColor(kBlue);
    TLPlot_Clone->GetXaxis()->SetTitleSize(0.06);
    TLPlot_Clone->GetXaxis()->SetLabelSize(0.0425);
    TLPlot_Clone->GetXaxis()->CenterTitle(true);
    TLPlot_Clone->GetYaxis()->SetTitle("Number of events");
    TLPlot_Clone->GetYaxis()->SetTitleSize(0.06);
    TLPlot_Clone->GetYaxis()->SetLabelSize(0.0425);
    TLPlot_Clone->GetYaxis()->CenterTitle(true);
    TLPlot_Clone->SetLineWidth(2);

    if (plot_errorbars) { TLPlot_Clone->Sumw2(); }

    if (rebin_plots) { TLPlot_Clone->Rebin(2); }

    TLPlot_Clone->Draw();
    TLPlot_Clone->SetStats(1);
    EfficiencyComponentPlots->Add(TLPlot_Clone);
    //    Histogram_list->Add(TLPlot_Clone);

    Canvas->SaveAs((TLPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving Efficiency_plot">
    Efficiency_plot->SetLineStyle(1);
    Efficiency_plot->SetLineColor(kBlue);
    Efficiency_plot->GetXaxis()->SetTitleSize(0.06);
    Efficiency_plot->GetXaxis()->SetLabelSize(0.0425);
    Efficiency_plot->GetXaxis()->CenterTitle(true);
    Efficiency_plot->GetYaxis()->SetTitle("Number of events");
    Efficiency_plot->GetYaxis()->SetTitleSize(0.06);
    Efficiency_plot->GetYaxis()->SetLabelSize(0.0425);
    Efficiency_plot->GetYaxis()->CenterTitle(true);
    Efficiency_plot->SetLineWidth(2);

    if ((EfficiencyFS == "1e cut") && (EfficiencyParticle == "Photon")) {
        Efficiency_plot->GetYaxis()->SetRangeUser(0., 1.1);
    } else {
        Efficiency_plot->GetYaxis()->SetRangeUser(0., 1.);
    }

    if (plot_errorbars) { Efficiency_plot->Sumw2(); }

    if (rebin_plots) { Efficiency_plot->Rebin(2); }

    Efficiency_plot->Divide(TLPlot_Clone);
    Efficiency_plot->Draw();
    Efficiency_plot->SetStats(0);

    Histogram_list->Add(EfficiencyComponentPlots);
    Histogram_list->Add(Efficiency_plot);

    Canvas->SaveAs((Efficiency_plot_SaveName).c_str());

    Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.6);
    Canvas->SaveAs((Efficiency_plot_SaveName.substr(0, Efficiency_plot_SaveName.find_last_of(".png") - 3) + "_ZoomIn1.png").c_str());

    Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.3);
    Canvas->SaveAs((Efficiency_plot_SaveName.substr(0, Efficiency_plot_SaveName.find_last_of(".png") - 3) + "_ZoomIn2.png").c_str());

    Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.1);
    Canvas->SaveAs((Efficiency_plot_SaveName.substr(0, Efficiency_plot_SaveName.find_last_of(".png") - 3) + "_ZoomIn3.png").c_str());

    Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.05);
    Canvas->SaveAs((Efficiency_plot_SaveName.substr(0, Efficiency_plot_SaveName.find_last_of(".png") - 3) + "_ZoomIn4.png").c_str());

    Canvas->Clear();
    //</editor-fold>

    delete Canvas;
}

void DrawAndSaveEfficiencyPlots(const std::string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot, TList *Histogram_list) {
    bool plot_errorbars = true;
    bool rebin_plots = false;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750);  // normal res
                                                                   //    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
                                                                   //    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Setting particle">
    std::string EfficiencyRecTitle = RPlot->GetTitle();
    std::string EfficiencyParticle = data_processor::GetParticleName(EfficiencyRecTitle);
    std::string EfficiencyParticleLC = data_processor::GetParticleNameLC(EfficiencyRecTitle);
    std::string EfficiencyParticleShort = data_processor::GetParticleNameShort(EfficiencyRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting Final state">
    std::string EfficiencyFS;

    if (basic_tools::FindSubstring(EfficiencyRecTitle, "1e_cut") || basic_tools::FindSubstring(EfficiencyRecTitle, "1e cut") || basic_tools::FindSubstring(EfficiencyRecTitle, "1e Cut")) {
        EfficiencyFS = "1e cut";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "1p")) {
        EfficiencyFS = "1p";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "1n")) {
        EfficiencyFS = "1n";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "1n1p")) {
        EfficiencyFS = "1n1p";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "1e2p")) {
        EfficiencyFS = "1e2p";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "2p")) {
        EfficiencyFS = "2p";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "pFDpCD")) {
        EfficiencyFS = "pFDpCD";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "nFDpCD")) {
        EfficiencyFS = "nFDpCD";
    }
    //</editor-fold>

    //<editor-fold desc="Setting stats box title">
    std::string EfficiencyStatsType;

    if (basic_tools::FindSubstring(EfficiencyRecTitle, "#theta")) {  // for momentum efficiency plots
        EfficiencyStatsType = "#theta_{" + EfficiencyParticleShort + "}";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "#phi")) {  // for momentum efficiency plots
        EfficiencyStatsType = "#phi_{" + EfficiencyParticleShort + "}";
    }

    //    if (basic_tools::FindSubstring(EfficiencyRecTitle, "ZOOMIN")) { EfficiencyStatsType = EfficiencyStatsType + " - ZOOMIN"; }

    std::string EfficiencyStatsTitle = EfficiencyStatsType + " (" + EfficiencyFS + ")";
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    std::string RPlot_Clone_StatsTitle = "reco. " + EfficiencyStatsTitle + " - cloned";
    TH1D *RPlot_Clone = (TH1D *)RPlot->Clone((RPlot_Clone_StatsTitle).c_str());
    std::string RPlot_Clone_test_StatsTitle = "reco. " + EfficiencyStatsTitle + " - cloned test";
    TH1D *RPlot_Clone_test = (TH1D *)RPlot->Clone((RPlot_Clone_test_StatsTitle).c_str());
    std::string RPlot_Clone_test_rebined_StatsTitle = "reco. " + EfficiencyStatsTitle + " - cloned test rebined";
    TH1D *RPlot_Clone_test_rebined = (TH1D *)RPlot->Clone((RPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { RPlot_Clone_test_rebined->Rebin(2); }

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    std::string TLPlot_Clone_StatsTitle = "Truth " + EfficiencyStatsTitle + " - cloned";
    TH1D *TLPlot_Clone = (TH1D *)Histogram1D_Truth->Clone((TLPlot_Clone_StatsTitle).c_str());
    std::string TLPlot_Clone_test_StatsTitle = "Truth " + EfficiencyStatsTitle + " - cloned test";
    TH1D *TLPlot_Clone_test = (TH1D *)Histogram1D_Truth->Clone((TLPlot_Clone_test_StatsTitle).c_str());
    std::string TLPlot_Clone_test_rebined_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *TLPlot_Clone_test_rebined = (TH1D *)Histogram1D_Truth->Clone((TLPlot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { TLPlot_Clone_test_rebined->Rebin(2); }
    //</editor-fold>

    //<editor-fold desc="Setting title">
    std::string EfficiencyType, EfficiencyTitle;

    if (basic_tools::FindSubstring(EfficiencyRecTitle, "momentum")) {  // for momentum efficiency plots
        EfficiencyType = "momentum";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "#theta")) {  // for theta efficiency plots
        EfficiencyType = "theta";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "#phi")) {  // for phi efficiency plots
        EfficiencyType = "phi";
    }

    if (EfficiencyType == "momentum") {
        EfficiencyTitle = EfficiencyParticle + " " + EfficiencyType;
    } else {
        EfficiencyTitle = "#" + EfficiencyType + "_{" + EfficiencyParticleShort + "}";
    }

    //    std::string EfficiencyTitle = EfficiencyParticle + " " + EfficiencyType;
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    std::string EfficiencyXLabel;

    if (basic_tools::FindSubstring(EfficiencyRecTitle, "momentum")) {  // for momentum efficiency plots
        EfficiencyXLabel = "P_{" + EfficiencyParticleShort + "} [GeV/c]";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "#theta")) {  // for momentum efficiency plots
        EfficiencyXLabel = EfficiencyTitle + " [Deg]";
        //        EfficiencyXLabel = "#theta [Deg]";
    } else if (basic_tools::FindSubstring(EfficiencyRecTitle, "#phi")) {  // for momentum efficiency plots
        EfficiencyXLabel = EfficiencyTitle + " [Deg]";
        //        EfficiencyXLabel = "#phi [Deg]";
    }
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    std::string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    std::string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    std::string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    std::string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    std::string EfficiencyYLabel = "#epsilon_{eff} = #frac{reco.}{truth}";
    //    std::string EfficiencyYLabel = "#epsilon_{eff} = reco./truth";
    //    std::string EfficiencyYLabel = "#epsilon_{eff} = recotruth";
    //    std::string EfficiencyYLabel = "#epsilon_{eff} = " + xLabel_REC + "^{rec}" + "/" + xLabel_Truth;
    //    std::string EfficiencyYLabel = "#epsilon_{eff} = " + xLabel_REC + "/" + xLabel_Truth;
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    std::string EfficiencySaveDir, EfficiencyTestSaveDir;

    if (basic_tools::FindSubstring(EfficiencyRecTitle, "Electron") || basic_tools::FindSubstring(EfficiencyRecTitle, "electron")) {
        EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/00_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
        EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test/";
    } else {
        if (basic_tools::FindSubstring(EfficiencyRecTitle, ", FD)") || basic_tools::FindSubstring(EfficiencyRecTitle, "FD " + EfficiencyParticle) ||
            basic_tools::FindSubstring(EfficiencyRecTitle, "FD " + EfficiencyParticleLC)) {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/01_FD_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "_FD/";
        } else if (basic_tools::FindSubstring(EfficiencyRecTitle, ", CD)") || basic_tools::FindSubstring(EfficiencyRecTitle, "CD " + EfficiencyParticle) ||
                   basic_tools::FindSubstring(EfficiencyRecTitle, "CD " + EfficiencyParticleLC)) {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/02_CD_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "_CD/";
        } else {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + EfficiencySaveDir).c_str());
    system(("mkdir -p " + EfficiencyTestSaveDir).c_str());
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    std::string sNameFlag;

    if (basic_tools::FindSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (basic_tools::FindSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    std::string RPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone.png";
    std::string RPlot_Clone_test_SaveName = EfficiencyTestSaveDir + sNameFlag + "01a_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone_test.png";
    std::string RPlot_Clone_test_rebined_SaveName =
        EfficiencyTestSaveDir + sNameFlag + "01b_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone_test_rebined.png";
    std::string TLPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone.png";
    std::string TLPlot_Clone_test_SaveName = EfficiencyTestSaveDir + sNameFlag + "02a_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone_test.png";
    std::string TLPlot_Clone_test_rebined_SaveName =
        EfficiencyTestSaveDir + sNameFlag + "02b_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone_test_rebined.png";
    std::string Efficiency_plot_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_" + EfficiencyFS + ".png";
    //    std::string RPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone.png";
    //    std::string RPlot_Clone_test_SaveName = EfficiencyTestSaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone_test.png";
    //    std::string TLPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone.png";
    //    std::string TLPlot_Clone_test_SaveName = EfficiencyTestSaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone_test.png";
    //    std::string Efficiency_plot_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_" + EfficiencyFS + ".png";
    //</editor-fold>

    TH1D *Efficiency_plot = (TH1D *)RPlot_Clone->Clone((EfficiencyParticle + " " + EfficiencyType + " #epsilon_{eff}" + " (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->SetTitle((EfficiencyTitle + " efficiency #epsilon_{eff} (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->GetYaxis()->SetTitle((EfficiencyYLabel).c_str());
    Efficiency_plot->GetXaxis()->SetTitle((EfficiencyXLabel).c_str());

    std::string Efficiency_plot_Name = Efficiency_plot->GetName();
    std::string TFolder_Name = Efficiency_plot_Name + " folder";
    TFolder *EfficiencyComponentPlots = new TFolder(TFolder_Name.c_str(), TFolder_Name.c_str());

    //<editor-fold desc="Plotting and saving RPlot_Clone_test">
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
    EfficiencyComponentPlots->Add(RPlot_Clone_test);
    //    Histogram_list->Add(RPlot_Clone_test);

    Canvas->SaveAs((RPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone_test">
    TLPlot_Clone_test->SetLineStyle(1);
    TLPlot_Clone_test->SetLineColor(kBlue);
    TLPlot_Clone_test->Draw();
    TLPlot_Clone_test->SetStats(1);
    EfficiencyComponentPlots->Add(TLPlot_Clone_test);
    //    Histogram_list->Add(TLPlot_Clone_test);

    Canvas->SaveAs((TLPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving RPlot_Clone_test_rebined">
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
    EfficiencyComponentPlots->Add(RPlot_Clone_test_rebined);
    //    Histogram_list->Add(RPlot_Clone_test_rebined);

    Canvas->SaveAs((RPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone_test_rebined">
    TLPlot_Clone_test_rebined->SetLineStyle(1);
    TLPlot_Clone_test_rebined->SetLineColor(kBlue);
    TLPlot_Clone_test_rebined->Draw();
    TLPlot_Clone_test_rebined->SetStats(1);
    EfficiencyComponentPlots->Add(TLPlot_Clone_test_rebined);
    //    Histogram_list->Add(TLPlot_Clone_test_rebined);

    Canvas->SaveAs((TLPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving RPlot_Clone">
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
    EfficiencyComponentPlots->Add(RPlot_Clone);
    //    Histogram_list->Add(RPlot_Clone);

    Canvas->SaveAs((RPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone">
    TLPlot_Clone->SetLineStyle(1);
    TLPlot_Clone->SetLineColor(kBlue);

    if (plot_errorbars) { TLPlot_Clone->Sumw2(); }

    if (rebin_plots) { TLPlot_Clone->Rebin(2); }
    TLPlot_Clone->Draw();
    TLPlot_Clone->SetStats(1);
    EfficiencyComponentPlots->Add(TLPlot_Clone);
    //    Histogram_list->Add(TLPlot_Clone);

    Canvas->SaveAs((TLPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving Efficiency_plot">
    Efficiency_plot->SetLineStyle(1);
    Efficiency_plot->SetLineColor(kBlue);
    Efficiency_plot->GetXaxis()->SetTitleSize(0.06);
    Efficiency_plot->GetXaxis()->SetLabelSize(0.0425);
    Efficiency_plot->GetXaxis()->CenterTitle(true);
    Efficiency_plot->GetYaxis()->SetTitleSize(0.06);
    Efficiency_plot->GetYaxis()->SetLabelSize(0.0425);
    Efficiency_plot->GetYaxis()->CenterTitle(true);
    Efficiency_plot->SetLineWidth(2);
    Efficiency_plot->GetYaxis()->SetRangeUser(0., 1.);

    if (plot_errorbars) { Efficiency_plot->Sumw2(); }

    if (rebin_plots) { Efficiency_plot->Rebin(2); }
    Efficiency_plot->Divide(TLPlot_Clone);
    Efficiency_plot->SetStats(0);
    Efficiency_plot->Draw();

    Histogram_list->Add(EfficiencyComponentPlots);
    Histogram_list->Add(Efficiency_plot);
    Canvas->SaveAs((Efficiency_plot_SaveName).c_str());

    Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.6);
    Canvas->SaveAs((Efficiency_plot_SaveName.substr(0, Efficiency_plot_SaveName.find_last_of(".png") - 3) + "_ZoomIn1.png").c_str());

    Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.3);
    Canvas->SaveAs((Efficiency_plot_SaveName.substr(0, Efficiency_plot_SaveName.find_last_of(".png") - 3) + "_ZoomIn2.png").c_str());

    Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.1);
    Canvas->SaveAs((Efficiency_plot_SaveName.substr(0, Efficiency_plot_SaveName.find_last_of(".png") - 3) + "_ZoomIn3.png").c_str());

    Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.05);
    Canvas->SaveAs((Efficiency_plot_SaveName.substr(0, Efficiency_plot_SaveName.find_last_of(".png") - 3) + "_ZoomIn4.png").c_str());

    Canvas->Clear();
    //</editor-fold>

    delete Canvas;
}
}

#endif //DRAW_AND_SAVE_EFFICIENCY_PLOTS_H
