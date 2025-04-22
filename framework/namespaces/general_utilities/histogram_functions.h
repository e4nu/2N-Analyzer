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
#include <TH2.h>
#include <TLatex.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TObject.h>
#include <TPad.h>
#include <TPaletteAxis.h>
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
#include <string>
#include <typeinfo>
#include <vector>

// Include libraries:
#include "basic_tools.h"

// Include classes:
#include "../../classes/hPlots/hsPlots.cpp"

namespace histogram_functions {

// FillByInt1D function -------------------------------------------------------------------------------------------------------------------------------------------------

void FillByInt1D(TH1D *H1D_All_Int, TH1D *H1D_QEL, TH1D *H1D_MEC, TH1D *H1D_RES, TH1D *H1D_DIS, bool qel, bool mec, bool res, bool dis, double Variable, double Weight) {
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

void FillByInt2D(TH2D *H1D_All_Int, TH2D *H1D_QEL, TH2D *H1D_MEC, TH2D *H1D_RES, TH2D *H1D_DIS, bool qel, bool mec, bool res, bool dis, double Variable_x, double Variable_y, double Weight) {
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

void FillByInthsPlots(hsPlots hsPlots_All_Int, hsPlots hsPlots_QEL, hsPlots hsPlots_MEC, hsPlots hsPlots_RES, hsPlots hsPlots_DIS, HistoType type, bool qel, bool mec, bool res, bool dis,
                      double Slice_variable, double Variable_x, double Variable_y, double Weight = 1.0) {
    if (type == TH1D_TYPE) {
        hsPlots_All_Int.Fill(Slice_variable, Variable_x, Weight);
    } else if (type == TH2D_TYPE) {
        hsPlots_All_Int.Fill(Slice_variable, Variable_x, Variable_y, Weight);
    }

    if (qel) {
        if (type == TH1D_TYPE) {
            hsPlots_QEL.Fill(Slice_variable, Variable_x, Weight);
        } else if (type == TH2D_TYPE) {
            hsPlots_QEL.Fill(Slice_variable, Variable_x, Variable_y, Weight);
        }
    } else if (mec) {
        if (type == TH1D_TYPE) {
            hsPlots_MEC.Fill(Slice_variable, Variable_x, Weight);
        } else if (type == TH2D_TYPE) {
            hsPlots_MEC.Fill(Slice_variable, Variable_x, Variable_y, Weight);
        }
    } else if (res) {
        if (type == TH1D_TYPE) {
            hsPlots_RES.Fill(Slice_variable, Variable_x, Weight);
        } else if (type == TH2D_TYPE) {
            hsPlots_RES.Fill(Slice_variable, Variable_x, Variable_y, Weight);
        }
    } else if (dis) {
        if (type == TH1D_TYPE) {
            hsPlots_DIS.Fill(Slice_variable, Variable_x, Weight);
        } else if (type == TH2D_TYPE) {
            hsPlots_DIS.Fill(Slice_variable, Variable_x, Variable_y, Weight);
        }
    }
}

// GetHistogramFromVector function --------------------------------------------------------------------------------------------------------------------------------------

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
                if (palette) palette->SetY2NDC(0.65);
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

};  // namespace histogram_functions

#endif  // HISTOGRAM_FUNCTIONS_H
