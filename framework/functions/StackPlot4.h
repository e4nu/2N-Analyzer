//
// Created by Alon Sportes on 12/05/2023.
//

#ifndef STACKPLOT4_H
#define STACKPLOT4_H

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

#include "../classes/hPlots/hPlot1D.cpp"
#include "GeneralFunctions.h"

using namespace std;

void StackPlot4(string &SampleName, TList *Histogram_list, hPlot1D const &Hist1, const std::string &Hist1_legened, hPlot1D const &Hist2, const std::string &Hist2_legened, hPlot1D const &Hist3,
                const std::string &Hist3_legened, hPlot1D const &Hist4, const std::string &Hist4_legened) {
    #pragma region /* Canvas definitions */
    //    TCanvas *StackPlot4Canvas = new TCanvas("StackPlot4Canvas", "StackPlot4Canvas", 1000, 750); // normal res
    TCanvas *StackPlot4Canvas = new TCanvas("StackPlot4Canvas", "StackPlot4Canvas", 1000 * 2, 750 * 2);  // double res

    StackPlot4Canvas->SetBottomMargin(0.14);
    StackPlot4Canvas->SetLeftMargin(0.17);
    StackPlot4Canvas->SetRightMargin(0.12);

    StackPlot4Canvas->SetGrid();

    StackPlot4Canvas->cd();
    #pragma endregion

    THStack *HistogramStack = new THStack((Hist1.GetHistogramStatTitle()).c_str(), (Hist1.GetHistogramTitle() + ";" + Hist1.GetXaxisTitle() + ";").c_str());

    TH1D *Histogram1 = Hist1.GetHistogram();
    TH1D *Histogram1_Clone = (TH1D *)Histogram1->Clone((Hist1.GetHistogramStatTitle() + " - cloned").c_str());
    Histogram1_Clone->SetStats(0);
    HistogramStack->Add(Histogram1_Clone);

    TH1D *Histogram2 = Hist2.GetHistogram();
    TH1D *Histogram2_Clone = (TH1D *)Histogram2->Clone((Hist2.GetHistogramStatTitle() + " - cloned").c_str());
    Histogram2_Clone->SetStats(0);
    HistogramStack->Add(Histogram2_Clone);

    TH1D *Histogram3 = Hist3.GetHistogram();
    TH1D *Histogram3_Clone = (TH1D *)Histogram3->Clone((Hist3.GetHistogramStatTitle() + " - cloned").c_str());
    Histogram3_Clone->SetStats(0);
    HistogramStack->Add(Histogram3_Clone);

    TH1D *Histogram4 = Hist4.GetHistogram();
    //    TH1D *Histogram4_Clone = (TH1D *) Histogram4->Clone((Hist1.GetHistogramStatTitle() + " - Stack").c_str());
    TH1D *Histogram4_Clone = (TH1D *)Histogram4->Clone((Hist4.GetHistogramStatTitle() + " - cloned").c_str());
    Histogram4_Clone->SetStats(0);
    HistogramStack->Add(Histogram4_Clone);

    #pragma region /* setting sNameFlag */
    std::string sNameFlag;

    if (findSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }
    #pragma endregion

    double Histogram1D_integral;  // To be calculated only if normalize_Histogram

    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2 + 0.1;
    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3 + 0.1;
    //    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
    //    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;

    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    std::string Histogram1DTitle = Hist1.GetHistogramTitle();
    std::string Histogram1DTitleReactions;
    std::string xLable = Hist1.GetXaxisTitle();
    std::string finalState = Hist1.GetFinalState();
    std::string HistogramStackSaveNamePath0 = Hist1.GetHistogram1DSaveNamePath().substr(0, Hist1.GetHistogram1DSaveNamePath().find_last_of("/"));
    std::string HistogramStackSaveNamePath1 = HistogramStackSaveNamePath0.substr(0, HistogramStackSaveNamePath0.find_last_of("/"));
    std::string HistogramStackSaveNamePath = HistogramStackSaveNamePath1.substr(0, HistogramStackSaveNamePath1.find_last_of("/")) + "/";
    std::string HistogramStackSaveName = HistogramStackSaveNamePath1.substr(HistogramStackSaveNamePath1.find_last_of('/') + 1) + "_Stack4";
    //    std::string HistogramStackSaveName = Hist1.GetHistogram1DSaveName() + "_Stack";

    cout << "\n\n\n\nHistogramStackSaveNamePath0 = " << HistogramStackSaveNamePath0 << "\n";
    cout << "HistogramStackSaveNamePath1 = " << HistogramStackSaveNamePath1 << "\n";
    cout << "HistogramStackSaveNamePath = " << HistogramStackSaveNamePath << "\n";
    cout << "HistogramStackSaveName = " << HistogramStackSaveName << "\n\n\n\n";

    double TitleSize = Hist1.GetTitleSize();
    double LabelSize = Hist1.GetLabelSizeX();

    bool centerTitle = Hist1.GetCenterTitle();
    bool normalize_Histogram = Hist1.GetNormalizeHistogram();

    Histogram1_Clone->SetLineWidth(4), Histogram1_Clone->SetLineColor(kBlack), Histogram1_Clone->SetLineStyle(5);
    Histogram2_Clone->SetLineWidth(2), Histogram2_Clone->SetLineColor(kBlue);
    Histogram3_Clone->SetLineWidth(2), Histogram3_Clone->SetLineColor(kRed + 1);
    Histogram4_Clone->SetLineWidth(2), Histogram4_Clone->SetLineColor(kGreen);

    HistogramStack->Draw("nostack");
    HistogramStack->GetHistogram()->GetXaxis()->SetTitleSize(TitleSize);
    HistogramStack->GetHistogram()->GetXaxis()->SetLabelSize(LabelSize);
    HistogramStack->GetHistogram()->GetXaxis()->CenterTitle(centerTitle);
    HistogramStack->GetHistogram()->GetYaxis()->SetLabelSize(LabelSize);
    HistogramStack->GetHistogram()->GetYaxis()->CenterTitle(centerTitle);

    if (normalize_Histogram) {
        std::string title;

        if (Histogram1DTitleReactions != "") {
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")" + " - Normalized";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
            }
        } else if (Histogram1DTitleReactions == "") {
            if (finalState == "") {
                title = Histogram1DTitle + " - Normalized";
            } else {
                title = Histogram1DTitle + " (" + finalState + ")" + " - Normalized";
            }
        }

        const char *HistogramTitle = title.c_str();
        HistogramStack->SetTitle(HistogramTitle);
        HistogramStack->GetYaxis()->SetTitle("Probability (%)");
        HistogramStack->GetHistogram()->GetYaxis()->SetTitleSize(TitleSize);

        if (Histogram1->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            HistogramStack->Draw();
            displayText->Draw();
        }
    } else {
        std::string title;

        if (Histogram1DTitleReactions != "") {
            if (finalState == "") {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ")";
            } else {
                title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
            }
        } else if (Histogram1DTitleReactions == "") {
            if (finalState == "") {
                title = Histogram1DTitle;
            } else {
                title = Histogram1DTitle + " (" + finalState + ")";
            }
        }

        const char *HistogramTitle = title.c_str();
        HistogramStack->SetTitle(HistogramTitle);

        if (Histogram1->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->AddText("Empty histogram");
            displayText->SetTextAlign(22);
            HistogramStack->Draw();
            displayText->Draw();
        } else if (Histogram1->Integral() != 0.) {
            HistogramStack->GetYaxis()->SetTitle("Number of events");
            HistogramStack->GetHistogram()->GetYaxis()->SetTitleSize(TitleSize);
        }
    }

    auto HistogramStackLegend = new TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);

    if (Histogram1->Integral() != 0.) {
        TLegendEntry *Histogram1_Entry = HistogramStackLegend->AddEntry(Histogram1_Clone, Hist1_legened.c_str(), "l");
        TLegendEntry *Histogram2_Entry = HistogramStackLegend->AddEntry(Histogram2_Clone, Hist2_legened.c_str(), "l");
        TLegendEntry *Histogram3_Entry = HistogramStackLegend->AddEntry(Histogram3_Clone, Hist3_legened.c_str(), "l");
        TLegendEntry *Histogram4_Entry = HistogramStackLegend->AddEntry(Histogram4_Clone, Hist4_legened.c_str(), "l");
        HistogramStackLegend->Draw();
    }

    Histogram_list->Add(HistogramStack);

    if (finalState.empty()) {
        StackPlot4Canvas->SaveAs((HistogramStackSaveNamePath + HistogramStackSaveName + ".pdf").c_str());
        //        StackPlot4Canvas->SaveAs((HistogramStackSaveNamePath + HistogramStackSaveName + "_" + Histogram1DTitleReactions + ".pdf").c_str());
    } else {
        StackPlot4Canvas->SaveAs((HistogramStackSaveNamePath + HistogramStackSaveName + ".pdf").c_str());
        //        StackPlot4Canvas->SaveAs((HistogramStackSaveNamePath + HistogramStackSaveName + "_" + Histogram1DTitleReactions + "_" + finalState + ".pdf").c_str());
    }

    StackPlot4Canvas->Clear();
}

#endif  // STACKPLOT4_H
