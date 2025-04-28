//
// Created by Alon Sportes on 07/04/2025.
//

#ifndef HISTOGRAM_FUNCTIONS_H
#define HISTOGRAM_FUNCTIONS_H

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2.h>
#include <TH2D.h>
#include <THStack.h>
#include <TLatex.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TObject.h>
#include <TPad.h>
#include <TPaletteAxis.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>  // for gSystem->mkdir
#include <TTree.h>
#include <math.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

// Include libraries:
#include "basic_tools.h"

// Include classes:
#include "../../classes/hPlots/hsPlots.cpp"

namespace histogram_functions {

// FillByInt1D function -------------------------------------------------------------------------------------------------------------------------------------------------

void FillByInt1D(TH1D *H1D_All_Int, TH1D *H1D_QEL, TH1D *H1D_MEC, TH1D *H1D_RES, TH1D *H1D_DIS, const bool &qel, const bool &mec, const bool &res, const bool &dis, const double &Variable,
                 const double &Weight) {
    H1D_All_Int->Fill(Variable, Weight);

    if (qel) {
        H1D_QEL->Fill(Variable, Weight);
    } else if (mec) {
        H1D_MEC->Fill(Variable, Weight);
    } else if (res) {
        H1D_RES->Fill(Variable, Weight);
    } else if (dis) {
        H1D_DIS->Fill(Variable, Weight);
    }
}

// FillByInt2D function -------------------------------------------------------------------------------------------------------------------------------------------------

void FillByInt2D(TH2D *H1D_All_Int, TH2D *H1D_QEL, TH2D *H1D_MEC, TH2D *H1D_RES, TH2D *H1D_DIS, const bool &qel, const bool &mec, const bool &res, const bool &dis, const double &Variable_x,
                 const double &Variable_y, const double &Weight) {
    H1D_All_Int->Fill(Variable_x, Variable_y, Weight);

    if (qel) {
        H1D_QEL->Fill(Variable_x, Variable_y, Weight);
    } else if (mec) {
        H1D_MEC->Fill(Variable_x, Variable_y, Weight);
    } else if (res) {
        H1D_RES->Fill(Variable_x, Variable_y, Weight);
    } else if (dis) {
        H1D_DIS->Fill(Variable_x, Variable_y, Weight);
    }
}

// FillByInthsPlots function --------------------------------------------------------------------------------------------------------------------------------------------

void FillByInthsPlots(hsPlots &hsPlots_All_Int, hsPlots &hsPlots_QEL, hsPlots &hsPlots_MEC, hsPlots &hsPlots_RES, hsPlots &hsPlots_DIS, hsPlots::HistoType type, const bool &qel,
                      const bool &mec, const bool &res, const bool &dis, const double &Slice_variable, const double &Variable_x, const double &Variable_y, const double &Weight = 1.0) {
    bool PrintOut = false;

    if (PrintOut) { std::cout << "\n\nFill all int start\n\n"; }

    if (type == hsPlots::TH1D_TYPE) {
        hsPlots_All_Int.Fill(Slice_variable, Variable_x, Weight);
    } else if (type == hsPlots::TH2D_TYPE) {
        hsPlots_All_Int.Fill(Slice_variable, Variable_x, Variable_y, Weight);
    }

    if (PrintOut) { std::cout << "\n\nFill all int end\n\n"; }

    if (qel) {
        if (PrintOut) { std::cout << "\n\nFill QEL start\n\n"; }

        if (type == hsPlots::TH1D_TYPE) {
            hsPlots_QEL.Fill(Slice_variable, Variable_x, Weight);
        } else if (type == hsPlots::TH2D_TYPE) {
            hsPlots_QEL.Fill(Slice_variable, Variable_x, Variable_y, Weight);
        }

        if (PrintOut) { std::cout << "\n\nFill QEL end\n\n"; }

    } else if (mec) {
        if (PrintOut) { std::cout << "\n\nFill MEC start\n\n"; }

        if (type == hsPlots::TH1D_TYPE) {
            hsPlots_MEC.Fill(Slice_variable, Variable_x, Weight);
        } else if (type == hsPlots::TH2D_TYPE) {
            hsPlots_MEC.Fill(Slice_variable, Variable_x, Variable_y, Weight);
        }

        if (PrintOut) { std::cout << "\n\nFill MEC end\n\n"; }

    } else if (res) {
        if (PrintOut) { std::cout << "\n\nFill RES start\n\n"; }

        if (type == hsPlots::TH1D_TYPE) {
            hsPlots_RES.Fill(Slice_variable, Variable_x, Weight);
        } else if (type == hsPlots::TH2D_TYPE) {
            hsPlots_RES.Fill(Slice_variable, Variable_x, Variable_y, Weight);
        }

        if (PrintOut) { std::cout << "\n\nFill RES end\n\n"; }

    } else if (dis) {
        if (PrintOut) { std::cout << "\n\nFill DIS start\n\n"; }

        if (type == hsPlots::TH1D_TYPE) {
            hsPlots_DIS.Fill(Slice_variable, Variable_x, Weight);
        } else if (type == hsPlots::TH2D_TYPE) {
            hsPlots_DIS.Fill(Slice_variable, Variable_x, Variable_y, Weight);
        }

        if (PrintOut) { std::cout << "\n\nFill DIS end\n\n"; }
    }
}

// DrawAndSaveHistogramsToPDF function ----------------------------------------------------------------------------------------------------------------------------------

// Function to render and save histograms from a list to a PDF
void DrawAndSaveHistogramsToPDF(TCanvas *MainCanvas, const std::vector<TObject *> &HistoList, const std::string &Histogram_OutPDF_fileName_str, char *Histogram_OutPDF_fileName_char,
                                const std::string &SampleName, const std::string &VaryingSampleName, const double &beamE) {
    int pixelx = 1980;
    int pixely = 1530;
    TCanvas *TextCanvas = new TCanvas("TextCanvas", "TextCanvas", pixelx, pixely);
    TLatex titles;
    TLatex text;
    titles.SetTextSize(0.1);
    text.SetTextSize(0.05);

    sprintf(Histogram_OutPDF_fileName_char, "%s[", Histogram_OutPDF_fileName_str.c_str());
    TextCanvas->SaveAs(Histogram_OutPDF_fileName_char);
    sprintf(Histogram_OutPDF_fileName_char, "%s", Histogram_OutPDF_fileName_str.c_str());

    TextCanvas->cd();

    titles.DrawLatex(0.05, 0.90, "2N analyzer output");
    text.DrawLatex(0.05, 0.80, SampleName.c_str());
    text.DrawLatex(0.05, 0.75, VaryingSampleName.c_str());
    text.DrawLatex(0.2, 0.65, ("Beam energy: " + basic_tools::GetBeamEnergyFromDouble(beamE) + " [GeV]").c_str());

    TextCanvas->Print(Histogram_OutPDF_fileName_char, "pdf");
    TextCanvas->Clear();

    MainCanvas->cd();
    MainCanvas->Clear();

    for (int i = 0; i < HistoList.size(); i++) {
        MainCanvas->cd();

        MainCanvas->cd()->SetGrid();
        MainCanvas->cd()->SetBottomMargin(0.14);
        MainCanvas->cd()->SetLeftMargin(0.16);
        MainCanvas->cd()->SetRightMargin(0.12);

        gStyle->SetOptStat("ourmen");

        if (HistoList[i]->InheritsFrom("TH2")) {
            TH2 *h2 = dynamic_cast<TH2 *>(HistoList[i]);
            if (!h2) continue;
            h2->GetYaxis()->SetTitleOffset(1.5);
            h2->GetXaxis()->SetTitleOffset(1.1);
            gPad->SetRightMargin(0.23);

            h2->Draw("colz");

            if (h2->GetEntries() != 0) {
                gPad->Update();
                TPaletteAxis *palette = (TPaletteAxis *)h2->GetListOfFunctions()->FindObject("palette");
                if (palette) palette->SetY2NDC(0.50);
                gPad->Modified();
                gPad->Update();
            }
        } else if (HistoList[i]->InheritsFrom("TH1")) {
            TH1 *h1 = dynamic_cast<TH1 *>(HistoList[i]);
            if (!h1) continue;
            h1->GetYaxis()->SetTitleOffset(1.5);
            h1->GetXaxis()->SetTitleOffset(1.1);
            gPad->SetRightMargin(0.23);

            h1->Draw();
        }

        MainCanvas->Print(Histogram_OutPDF_fileName_char, "pdf");
        MainCanvas->Clear();
    }

    sprintf(Histogram_OutPDF_fileName_char, "%s]", Histogram_OutPDF_fileName_str.c_str());
    MainCanvas->Print(Histogram_OutPDF_fileName_char, "pdf");
}

// DrawEmptyHistogramNotice function ------------------------------------------------------------------------------------------------------------------------------------

// This function saves some reusable code. It is also defined in hsPlots, yet it is placed here to avoid include errors
void DrawEmptyHistogramNotice(double x_1, double y_1, double x_2, double y_2, double diplayTextSize = 0.1) {
    TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
    displayText->SetTextSize(diplayTextSize);
    displayText->SetFillColor(0);
    displayText->AddText("Empty histogram");
    displayText->SetTextAlign(22);
    displayText->Draw();
}

// IsHistogramEmpty function --------------------------------------------------------------------------------------------------------------------------------------------

bool IsHistogramEmpty(TObject *obj) {
    if (obj->InheritsFrom(TH1::Class())) {
        TH1 *h = (TH1 *)obj;
        return (h->GetEntries() == 0 || h->Integral() == 0);
    } else if (obj->InheritsFrom(THStack::Class())) {
        THStack *hs = (THStack *)obj;
        TList *hists = hs->GetHists();
        if (!hists) return true;
        TIter next(hists);
        while (TObject *subobj = next()) {
            if (subobj->InheritsFrom(TH1::Class())) {
                TH1 *h = (TH1 *)subobj;
                if (h->GetEntries() != 0 && h->Integral() != 0) return false;
            }
        }
        return true;
    } else if (obj->InheritsFrom(TH2::Class())) {
        TH2 *h2 = (TH2 *)obj;
        return (h2->GetEntries() == 0 || h2->Integral() == 0);
    }
    return true;
}

// CompareHistograms -------------------------------------------------------------------------------------------------------------------------------------------------------

void CompareHistograms(const std::vector<TObject *> &histograms, const std::string &saveDirectory) {
    size_t nHistos = histograms.size();

    if (nHistos != 4 && nHistos != 5) {
        std::cerr << "\n\nhistogram_functions::CompareHistograms: ERROR! CompareHistograms only supports 4 or 5 histograms!\n\n" << std::endl;
        return;
    }

    int nCols = (nHistos == 4) ? 2 : 3;
    int nRows = 2;

    std::vector<int> padMapping;
    if (nHistos == 4) {
        padMapping = {1, 2, 3, 4};  // 2x2
    } else if (nHistos == 5) {
        padMapping = {1, 2, 3, 5, 6};  // 3x2
    }

    // ------------------
    // Linear Scale Canvas
    // ------------------
    TCanvas TempCanvas("TempCanvas", "Histograms - Linear Scale", 1200, 800);
    TempCanvas.Divide(nCols, nRows);

    for (size_t i = 0; i < nHistos; ++i) {
        TempCanvas.cd(padMapping[i]);
        gPad->SetGrid();
        gPad->SetBottomMargin(0.14);
        gPad->SetLeftMargin(0.16);
        gPad->SetRightMargin(0.12);

        if (IsHistogramEmpty(histograms[i])) {
            DrawEmptyHistogramNotice(0.2, 0.4, 0.8, 0.6);
        } else if (histograms[i]->InheritsFrom(TH1D::Class())) {
            ((TH1D *)histograms[i])->Draw("HISTE");
        } else if (histograms[i]->InheritsFrom(TH2D::Class())) {
            TH2D *h2 = (TH2D *)histograms[i];
            h2->Draw("COLZ");
            gPad->Update();
            TPaletteAxis *palette = (TPaletteAxis *)h2->GetListOfFunctions()->FindObject("palette");
            if (palette) palette->SetY2NDC(0.50);
            gPad->Modified();
            gPad->Update();
        } else if (histograms[i]->InheritsFrom(THStack::Class())) {
            ((THStack *)histograms[i])->Draw("NOSTACK HIST");
        } else {
            std::cerr << "Warning: Object " << i << " is not a recognized histogram type!" << std::endl;
        }
    }

    TempCanvas.Update();
    TempCanvas.Draw();

    std::string savePath = saveDirectory;
    if (!savePath.empty() && savePath.back() != '/' && savePath.back() != '\\') { savePath += "/"; }
    std::string linearFile = savePath + "comparison_linear_scale.pdf";

    TempCanvas.SaveAs(linearFile.c_str());

    // ------------------
    // Log Scale Canvas
    // ------------------

    std::string logDir = savePath + "log_scale_plots/";
    if (gSystem->AccessPathName(logDir.c_str())) { gSystem->mkdir(logDir.c_str(), true); }

    TCanvas TempCanvas_log("TempCanvas_log", "Histograms - Log Scale", 1200, 800);
    TempCanvas_log.Divide(nCols, nRows);

    for (size_t i = 0; i < nHistos; ++i) {
        TempCanvas_log.cd(padMapping[i]);
        gPad->SetGrid();
        gPad->SetBottomMargin(0.14);
        gPad->SetLeftMargin(0.16);
        gPad->SetRightMargin(0.12);

        if (IsHistogramEmpty(histograms[i])) {
            DrawEmptyHistogramNotice(0.2, 0.4, 0.8, 0.6);
        } else if (histograms[i]->InheritsFrom(TH1D::Class())) {
            gPad->SetLogy(1);
            ((TH1D *)histograms[i])->Draw("HISTE");
        } else if (histograms[i]->InheritsFrom(TH2D::Class())) {
            gPad->SetLogz(1);
            TH2D *h2 = (TH2D *)histograms[i];
            h2->Draw("COLZ");
            gPad->Update();
            TPaletteAxis *palette = (TPaletteAxis *)h2->GetListOfFunctions()->FindObject("palette");
            if (palette) palette->SetY2NDC(0.50);
            gPad->Modified();
            gPad->Update();
        } else if (histograms[i]->InheritsFrom(THStack::Class())) {
            gPad->SetLogy(1);
            ((THStack *)histograms[i])->Draw("NOSTACK HIST");
        } else {
            std::cerr << "\n\nhistogram_functions::CompareHistograms: Warning: Object " << i << " is not a recognized histogram type!\n\n" << std::endl;
        }
    }

    TempCanvas_log.Update();
    TempCanvas_log.Draw();

    std::string logFile = logDir + "comparison_log_scale.pdf";

    TempCanvas_log.SaveAs(logFile.c_str());
}

};  // namespace histogram_functions

#endif  // HISTOGRAM_FUNCTIONS_H
