//
// Created by Alon Sportes on 14/05/2023.
//

#ifndef PLOTSEFF2X3_H
#define PLOTSEFF2X3_H

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

#include "../classes/hPlots/hPlot1D.cpp"
#include "EventProperties/GetParticleNameShort_rm.h"
#include "EventProperties/GetParticleName_rm.h"
#include "GeneralFunctions.h"

using namespace std;

void PlotsEff2x3(string SampleName, const hPlot1D &REC_Plot_1p_hist, const hPlot1D &TL_Plot_1p_hist, const hPlot1D &REC_Plot_1n_hist, const hPlot1D &TL_Plot_1n_hist, std::string SavePath) {
    #pragma region /* Canvas definitions */
    TCanvas *Canvas2x6 = new TCanvas("Canvas2x6", "Canvas2x6", 1000 * 2, 750 * 3);  // normal res

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas2x6->Divide(2, 3);
    #pragma endregion

    TH1D *REC_Plot_1p = REC_Plot_1p_hist.GetHistogram();
    std::string REC_Plot_1p_Clone_StatsTitle = "reco. " + REC_Plot_1p_hist.GetHistogramStatTitle() + " - cloned";
    TH1D *REC_Plot_1p_Clone = (TH1D *)REC_Plot_1p->Clone((REC_Plot_1p_Clone_StatsTitle).c_str());
    REC_Plot_1p_Clone->Rebin(2);

    TH1D *TL_Plot_1p = TL_Plot_1p_hist.GetHistogram();
    std::string TL_Plot_1p_Clone_StatsTitle = "TL. " + TL_Plot_1p_hist.GetHistogramStatTitle() + " - cloned";
    TH1D *TL_Plot_1p_Clone = (TH1D *)TL_Plot_1p->Clone((TL_Plot_1p_Clone_StatsTitle).c_str());
    TL_Plot_1p->Rebin(2);

    TH1D *REC_Plot_1n = REC_Plot_1n_hist.GetHistogram();
    std::string REC_Plot_1n_Clone_StatsTitle = "reco. " + REC_Plot_1n_hist.GetHistogramStatTitle() + " - cloned";
    TH1D *REC_Plot_1n_Clone = (TH1D *)REC_Plot_1n->Clone((REC_Plot_1n_Clone_StatsTitle).c_str());
    REC_Plot_1n->Rebin(2);

    TH1D *TL_Plot_1n = TL_Plot_1n_hist.GetHistogram();
    std::string TL_Plot_1n_Clone_StatsTitle = "TL. " + TL_Plot_1n_hist.GetHistogramStatTitle() + " - cloned";
    TH1D *TL_Plot_1n_Clone = (TH1D *)TL_Plot_1n->Clone((TL_Plot_1n_Clone_StatsTitle).c_str());
    TL_Plot_1n->Rebin(2);

    std::string EfficiencyRecTitle_1p = REC_Plot_1p_Clone->GetTitle();
    std::string EfficiencyParticle_1p = GetParticleName(EfficiencyRecTitle_1p);
    std::string EfficiencyType_1p;

    std::string EfficiencyRecTitle_1n = REC_Plot_1n_Clone->GetTitle();
    std::string EfficiencyParticle_1n = GetParticleName(EfficiencyRecTitle_1n);
    std::string EfficiencyType_1n;

    if (findSubstring(EfficiencyRecTitle, "momentum")) {  // for momentum efficiency plots
        EfficiencyType = "momentum";
    }

    std::string EfficiencyTitle = EfficiencyParticle + " " + EfficiencyType;

    Efficiency_plot_1p->SetTitle((EfficiencyTitle + " efficiency #epsilon (" + EfficiencyFS + ")").c_str());
    Efficiency_plot_1p->GetYaxis()->SetTitle((EfficiencyYLabel).c_str());
    Efficiency_plot_1p->GetXaxis()->SetTitle((EfficiencyXLabel).c_str());

    TH1D *Eff_Plot_1p = REC_Plot_1p_hist.GetHistogram();
    std::string Eff_Plot_1p_Clone_StatsTitle = "Eff. " + REC_Plot_1p_hist.GetHistogramStatTitle() + " - cloned";
    TH1D *Eff_Plot_1p_Clone = (TH1D *)Eff_Plot_1p->Clone((Eff_Plot_1p_Clone_StatsTitle).c_str());
    //    REC_Plot_1p_Clone->Rebin(2);

    TH1D *Eff_Plot_1n = REC_Plot_1n_hist.GetHistogram();
    std::string Eff_Plot_1n_Clone_StatsTitle = "Eff. " + REC_Plot_1n_hist.GetHistogramStatTitle() + " - cloned";
    TH1D *Eff_Plot_1n_Clone = (TH1D *)Eff_Plot_1n->Clone((Eff_Plot_1n_Clone_StatsTitle).c_str());
    //    REC_Plot_1p_Clone->Rebin(2);

    #pragma endregion

    #pragma region /* Setting title */

    #pragma region /* Setting sNameFlag */
    std::string sNameFlag;

    if (findSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }
    #pragma endregion

    #pragma region /* Canvas definitions */
    if (!REC_Plot_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TL_Plot_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!REC_Plot_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TL_Plot_1n) { cout << "\n\nEmpty hist\n\n\n"; }

    Canvas2x6->cd(1);
    Canvas2x6->cd(1)->SetGrid();
    Canvas2x6->cd(1)->SetBottomMargin(0.14);
    Canvas2x6->cd(1)->SetLeftMargin(0.17);
    Canvas2x6->cd(1)->SetRightMargin(0.12);
    REC_Plot_1p->Draw();

    Canvas2x6->cd(2);
    Canvas2x6->cd(2)->SetGrid();
    Canvas2x6->cd(2)->SetBottomMargin(0.14);
    Canvas2x6->cd(2)->SetLeftMargin(0.17);
    Canvas2x6->cd(2)->SetRightMargin(0.12);
    TL_Plot_1p->Draw();

    Canvas2x6->cd(3);
    Canvas2x6->cd(3)->SetGrid();
    Canvas2x6->cd(3)->SetBottomMargin(0.14);
    Canvas2x6->cd(3)->SetLeftMargin(0.17);
    Canvas2x6->cd(3)->SetRightMargin(0.12);
    Eff_Plot_1p->Draw();

    Canvas2x6->cd(4);
    Canvas2x6->cd(4)->SetGrid();
    Canvas2x6->cd(4)->SetBottomMargin(0.14);
    Canvas2x6->cd(4)->SetLeftMargin(0.17);
    Canvas2x6->cd(4)->SetRightMargin(0.12);
    REC_Plot_1n->Draw();

    Canvas2x6->cd(5);
    Canvas2x6->cd(5)->SetGrid();
    Canvas2x6->cd(5)->SetBottomMargin(0.14);
    Canvas2x6->cd(5)->SetLeftMargin(0.17);
    Canvas2x6->cd(5)->SetRightMargin(0.12);
    TL_Plot_1n->Draw();

    Canvas2x6->cd(6);
    Canvas2x6->cd(6)->SetGrid();
    Canvas2x6->cd(6)->SetBottomMargin(0.14);
    Canvas2x6->cd(6)->SetLeftMargin(0.17);
    Canvas2x6->cd(6)->SetRightMargin(0.12);
    Eff_Plot_1n->Draw();

    Canvas2x6->SaveAs(saveName.c_str());
    Canvas2x6->Clear();
    #pragma endregion

    std::string SaveDir = SavePath + sNameFlag + BetaPlot.GetHistogram1DSaveName() + "_fitted.pdf";
    const char *hBeta_CloneSaveDir = SaveDir.c_str();
    Canvas->SaveAs(hBeta_CloneSaveDir);

    Canvas->Clear();
}

#endif  // PLOTSEFF2X3_H
