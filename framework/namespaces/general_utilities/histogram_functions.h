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
#include <TLegend.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TObject.h>
#include <TPad.h>
#include <TPaletteAxis.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>  // for gSystem->mkdir
#include <TSystem.h>
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

// TitleAligner functions -----------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Replaces a substring in the title and x-axis label of a histogram object.
 *
 * This templated function updates both the title and x-axis label of the given object
 * (typically a TH1 or TH2) if the specified substring is found in either.
 *
 * @tparam T ROOT histogram type (e.g., TH1D, TH2D).
 * @param obj Pointer to the histogram object.
 * @param title Title string to update (in-place).
 * @param xLabel X-axis title string to update (in-place).
 * @param originToReplace Substring to search for and replace.
 * @param replacement Replacement string.
 */
template <typename T>
void TitleAligner(T *obj, std::string &title, std::string &xLabel, const std::string &originToReplace, const std::string &replacement) {
    auto updateTitle = [&](std::string &str, auto setTitleFunc) {
        if (basic_tools::FindSubstring(str, originToReplace)) {
            str = basic_tools::ReplaceSubstring(str, originToReplace, replacement);
            setTitleFunc(str.c_str());
        }
    };

    updateTitle(title, [&](const char *newTitle) { obj->SetTitle(newTitle); });
    updateTitle(xLabel, [&](const char *newTitle) { obj->GetXaxis()->SetTitle(newTitle); });
}

/**
 * @brief Replaces a substring in the title, x-axis, and y-axis labels of a histogram object.
 *
 * This templated function updates the main title and both axis labels of a histogram object
 * if the given substring is found in any of them.
 *
 * @tparam T ROOT histogram type (e.g., TH1D, TH2D).
 * @param obj Pointer to the histogram object.
 * @param title Title string to update (in-place).
 * @param xLabel X-axis title string to update (in-place).
 * @param yLabel Y-axis title string to update (in-place).
 * @param originToReplace Substring to search for and replace.
 * @param replacement Replacement string.
 */
template <typename T>
void TitleAligner(T *obj, std::string &title, std::string &xLabel, std::string &yLabel, const std::string &originToReplace, const std::string &replacement) {
    auto updateTitle = [&](std::string &str, auto setTitleFunc) {
        if (basic_tools::FindSubstring(str, originToReplace)) {
            str = basic_tools::ReplaceSubstring(str, originToReplace, replacement);
            setTitleFunc(str.c_str());
        }
    };

    updateTitle(title, [&](const char *newTitle) { obj->SetTitle(newTitle); });
    updateTitle(xLabel, [&](const char *newTitle) { obj->GetXaxis()->SetTitle(newTitle); });
    updateTitle(yLabel, [&](const char *newTitle) { obj->GetYaxis()->SetTitle(newTitle); });
}

/**
 * @brief Replaces a substring in the title and axis labels of a ROOT histogram object.
 *
 * This function modifies the title and axis labels (X and Y) of ROOT histogram objects derived from TObject.
 * It supports TH1 (including TH1D, TH2D), and THStack. The function performs a string replacement for all occurrences
 * of the specified substring in the title and axis labels.
 *
 * For TH1-derived objects (TH1, TH2, etc.), it updates:
 * - The main title (via SetTitle)
 * - The X-axis title (via GetXaxis()->SetTitle)
 * - The Y-axis title (via GetYaxis()->SetTitle)
 *
 * For THStack, it only updates:
 * - The main title (via SetTitle)
 * - The X-axis and Y-axis titles (via GetXaxis()->SetTitle, GetYaxis()->SetTitle), which are shared across the stack
 *
 * @param obj Pointer to a TObject. Must be a TH1-derived histogram or a THStack.
 * @param originToReplace Substring to search for in the title and axis labels.
 * @param replacement Replacement string to substitute for the found substring.
 */
void TitleAligner(TObject *obj, const std::string &originToReplace, const std::string &replacement) {
    if (!obj) return;

    std::string title = obj->GetTitle();
    auto updateTitle = [&](std::string &str, auto setTitleFunc) {
        if (basic_tools::FindSubstring(str, originToReplace)) {
            str = basic_tools::ReplaceSubstring(str, originToReplace, replacement);
            setTitleFunc(str.c_str());
        }
    };

    // Safely set the main title only if obj inherits from TNamed
    if (auto namedObj = dynamic_cast<TNamed *>(obj)) {
        updateTitle(title, [&](const char *newTitle) { namedObj->SetTitle(newTitle); });
    }

    if (obj->InheritsFrom(TH1::Class())) {
        TH1 *h = static_cast<TH1 *>(obj);
        std::string xLabel = h->GetXaxis()->GetTitle();
        std::string yLabel = h->GetYaxis()->GetTitle();
        updateTitle(xLabel, [&](const char *newTitle) { h->GetXaxis()->SetTitle(newTitle); });
        updateTitle(yLabel, [&](const char *newTitle) { h->GetYaxis()->SetTitle(newTitle); });
    } else if (obj->InheritsFrom(THStack::Class())) {
        THStack *stack = static_cast<THStack *>(obj);
        TH1 *frameHist = stack->GetHistogram();
        if (!frameHist) return;
        std::string xLabel = frameHist->GetXaxis()->GetTitle();
        std::string yLabel = frameHist->GetYaxis()->GetTitle();
        updateTitle(xLabel, [&](const char *newTitle) { frameHist->GetXaxis()->SetTitle(newTitle); });
        updateTitle(yLabel, [&](const char *newTitle) { frameHist->GetYaxis()->SetTitle(newTitle); });
    }
}

/**
 * @brief Updates the titles and axis labels of two TH1D histograms simultaneously.
 *
 * This version of TitleAligner applies the replacement logic to both simulation and data histograms.
 *
 * @param simHistogram Pointer to the simulation histogram (TH1D).
 * @param dataHistogram Pointer to the data histogram (TH1D).
 * @param originToReplace Substring to search for and replace in titles and labels.
 * @param replacement Replacement string.
 */
void TitleAligner(TH1D *simHistogram, TH1D *dataHistogram, const std::string &originToReplace, const std::string &replacement) {
    auto updateTitle = [&](TH1D *hist) {
        std::string title = hist->GetTitle();
        std::string xLabel = hist->GetXaxis()->GetTitle();
        std::string yLabel = hist->GetYaxis()->GetTitle();

        if (basic_tools::FindSubstring(title, originToReplace)) {
            title = basic_tools::ReplaceSubstring(title, originToReplace, replacement);
            hist->SetTitle(title.c_str());
        }

        if (basic_tools::FindSubstring(xLabel, originToReplace)) {
            xLabel = basic_tools::ReplaceSubstring(xLabel, originToReplace, replacement);
            hist->GetXaxis()->SetTitle(xLabel.c_str());
        }

        if (basic_tools::FindSubstring(yLabel, originToReplace)) {
            yLabel = basic_tools::ReplaceSubstring(yLabel, originToReplace, replacement);
            hist->GetYaxis()->SetTitle(yLabel.c_str());
        }
    };

    updateTitle(simHistogram);
    updateTitle(dataHistogram);
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
                if (palette) palette->SetY2NDC(0.475);
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

// FindHistogram function -----------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Searches a ROOT file for the first histogram whose name contains a given substring and matches a specified class type (e.g., "TH1D", "TH2D", "THStack").
 *
 * The function loops through all keys in the ROOT file and checks each object:
 * - It must inherit from the specified class (e.g., "TH1D")
 * - Its name must contain the provided substring
 *
 * If a matching object is found, it is cloned to detach it from ROOT file ownership and returned to the caller.
 * This ensures safe memory management and avoids potential use-after-free errors.
 *
 * @param file Pointer to the ROOT file to search.
 * @param histNameSubstring Substring to match in the histogram's name.
 * @param desiredClass The expected ROOT class name of the object (e.g., "TH1D", "TH2D", "THStack").
 * @return TObject* Pointer to the cloned histogram object. The caller is responsible for deleting it.
 *
 * @example
 * TFile *file = TFile::Open("histos.root");
 * TH1D *hist = dynamic_cast<TH1D*>(FindHistogram(file, "myHist", "TH1D"));
 * if (hist) hist->Draw();
 */
TObject *FindHistogram(TFile *file, const char *histNameSubstring, const std::string &desiredClass) {
    bool PrintOut = false;
    bool PrintOut1 = false;

    TObject *foundHistogram = nullptr;
    bool histogramFound = false;
    std::string foundHistName;

    // Step 1: Create an iterator over all keys in the ROOT file
    TIter next(file->GetListOfKeys());
    TKey *key;

    // Step 2: Loop through each key in the ROOT file
    while ((key = (TKey *)next())) {
        // Step 2a: Read the object from the file
        TObject *obj = key->ReadObj();
        if (!obj) continue;  // If object couldn't be read, skip

        std::string objName = obj->GetName();
        if (PrintOut1) std::cout << objName << "\n\n";

        // Step 2b: Check if the object matches the name substring and inherits from the desired class
        // - If either check fails, delete the object and continue to the next key
        if (!basic_tools::FindSubstring(objName, histNameSubstring) || !obj->IsA()->InheritsFrom(desiredClass.c_str())) {
            delete obj;  // Safe to delete since we won't keep this object
            continue;
        }

        // Step 2c: If matches, clone the object to detach from file and safely return to caller
        foundHistogram = obj->Clone();
        // Step 2d: Unlink from any ROOT directory (avoids automatic file cleanup)
        if (foundHistogram->InheritsFrom(TH1::Class())) { ((TH1 *)foundHistogram)->SetDirectory(nullptr); }
        histogramFound = true;
        foundHistName = obj->ClassName();

        // Step 2e: Delete the original object (we only keep the clone)
        delete obj;
        break;  // Only return the first matching histogram
    }

    // Step 3: Handle the case where no histogram was found
    if (!histogramFound) {
        std::cerr << "\n\nFindHistogram: could not find histogram!\n";
        std::cerr << "Histogram name substring = " << histNameSubstring << "\n";
        std::cerr << "Desired class = " << desiredClass << "\n";
        exit(1);
    } else if (PrintOut) {
        std::cout << "\n\nFindHistogram: histogram found!\n";
        std::cout << "FoundHistName = " << foundHistName << "\n";
    }

    // Step 4: Return the cloned histogram (caller takes ownership and must delete)
    return foundHistogram;
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

// DrawTHStack ----------------------------------------------------------------------------------------------------------------------------------------------------------

void DrawTHStack(THStack *stack, bool useLogScale) {
    if (!stack) { return; }

    if (useLogScale) { gPad->SetLogy(1); }

    TList *histList = stack->GetHists();
    if (!histList) { return; }

    // Prepare histogram pointers
    TH1 *H1D_All_Int = nullptr;
    TH1 *H1D_QEL = nullptr;
    TH1 *H1D_MEC = nullptr;
    TH1 *H1D_RES = nullptr;
    TH1 *H1D_DIS = nullptr;

    // Sort histograms by title
    TIter next(histList);
    while (TObject *obj = next()) {
        if (obj->InheritsFrom(TH1::Class())) {
            if (!((TH1 *)obj)->GetSumw2N()) { ((TH1 *)obj)->Sumw2(); }
            // ((TH1 *)obj)->Sumw2(kTRUE);
            ((TH1 *)obj)->SetMarkerSize(0);

            TH1 *h = (TH1 *)obj;
            TString title = h->GetTitle();

            if (title.Contains("All Int.")) {
                H1D_All_Int = h;
            } else if (title.Contains("QE")) {
                H1D_QEL = h;
            } else if (title.Contains("MEC")) {
                H1D_MEC = h;
            } else if (title.Contains("RES")) {
                H1D_RES = h;
            } else if (title.Contains("DIS")) {
                H1D_DIS = h;
            }
        }
    }

    // Set styles and Sumw2
    if (H1D_All_Int) {
        H1D_All_Int->SetLineWidth(2);
        // H1D_All_Int->SetLineWidth(4);
        H1D_All_Int->SetLineColor(kBlack);
        H1D_All_Int->SetLineStyle(5);
        H1D_All_Int->SetMarkerSize(0);
        H1D_All_Int->SetMarkerColor(kBlack);
        if (!H1D_All_Int->GetSumw2N()) { H1D_All_Int->Sumw2(); }
        // H1D_All_Int->Sumw2(kTRUE);
    }
    if (H1D_QEL) {
        H1D_QEL->SetLineWidth(1);
        // H1D_QEL->SetLineWidth(2);
        H1D_QEL->SetLineColor(kAzure + 10);
        // H1D_QEL->SetLineColor(kBlue);
        H1D_QEL->SetMarkerSize(0);
        H1D_QEL->SetMarkerColor(kAzure + 10);
        // H1D_QEL->SetMarkerColor(kBlue);
        if (!H1D_QEL->GetSumw2N()) { H1D_QEL->Sumw2(); }
        // H1D_QEL->Sumw2(kTRUE);
    }
    if (H1D_MEC) {
        H1D_MEC->SetLineWidth(1);
        // H1D_MEC->SetLineWidth(2);
        H1D_MEC->SetLineColor(kViolet);
        // H1D_MEC->SetLineColor(kRed + 1);
        H1D_MEC->SetMarkerSize(0);
        H1D_MEC->SetMarkerColor(kViolet);
        // H1D_MEC->SetMarkerColor(kRed + 1);
        if (!H1D_MEC->GetSumw2N()) { H1D_MEC->Sumw2(); }
        // H1D_MEC->Sumw2(kTRUE);
    }
    if (H1D_RES) {
        H1D_RES->SetLineWidth(1);
        // H1D_RES->SetLineWidth(2);
        H1D_RES->SetLineColor(kGreen);
        // H1D_RES->SetLineColor(kGreen + 1);
        H1D_RES->SetMarkerSize(0);
        H1D_RES->SetMarkerColor(kGreen);
        // H1D_RES->SetMarkerColor(kGreen + 1);
        if (!H1D_RES->GetSumw2N()) { H1D_RES->Sumw2(); }
        // H1D_RES->Sumw2(kTRUE);
    }
    if (H1D_DIS) {
        H1D_DIS->SetLineWidth(1);
        // H1D_DIS->SetLineWidth(2);
        H1D_DIS->SetLineColor(kOrange + 7);
        // H1D_DIS->SetLineColor(kOrange + 6);
        H1D_DIS->SetMarkerSize(0);
        H1D_DIS->SetMarkerColor(kOrange + 7);
        // H1D_DIS->SetMarkerColor(kOrange + 6);
        if (!H1D_DIS->GetSumw2N()) { H1D_DIS->Sumw2(); }
        // H1D_DIS->Sumw2(kTRUE);
    }

    // Draw the stack
    stack->Draw("NOSTACK");

    // Add legend if needed
    if (H1D_All_Int && H1D_All_Int->Integral() != 0.) {
        auto Histogram1DStackLegend = new TLegend(0.76, 0.624, 0.865, 0.89);
        Histogram1DStackLegend->SetTextSize(0.03);
        if (H1D_All_Int) { Histogram1DStackLegend->AddEntry(H1D_All_Int, "All int.", "l"); }
        if (H1D_QEL) { Histogram1DStackLegend->AddEntry(H1D_QEL, "QE", "l"); }
        if (H1D_MEC) { Histogram1DStackLegend->AddEntry(H1D_MEC, "MEC", "l"); }
        if (H1D_RES) { Histogram1DStackLegend->AddEntry(H1D_RES, "RES", "l"); }
        if (H1D_DIS) { Histogram1DStackLegend->AddEntry(H1D_DIS, "DIS", "l"); }
        Histogram1DStackLegend->Draw();
    }
}

// CompareHistograms -------------------------------------------------------------------------------------------------------------------------------------------------------

void CompareHistograms(const std::vector<TObject *> &histograms, const std::string &saveDirectory, const std::string &saveDirectoryName = "", const std::string &ComparisonName = "") {
    size_t nHistos = histograms.size();

    if (nHistos != 2 && nHistos != 4 && nHistos != 5) {
        std::cerr << "\n\nhistogram_functions::CompareHistograms: ERROR! Only supports 2, 4, or 5 histograms!\n\n" << std::endl;
        return;
    }

    int nCols = (nHistos == 2 || nHistos == 4) ? 2 : 3;
    int nRows = (nHistos == 2) ? 1 : 2;

    int canvasWidth = 1000 * nCols;
    int canvasHeight = 750 * nRows;

    std::vector<int> padMapping;
    if (nHistos == 2) {
        padMapping = {1, 2};
    } else if (nHistos == 4) {
        padMapping = {1, 2, 3, 4};
        // canvasHeight = 440 * nRows;
    } else if (nHistos == 5) {
        padMapping = {1, 2, 3, 5, 6};
        // canvasHeight = 440 * nRows;
    }

    // Create output directory if needed
    std::string savePath = (saveDirectoryName != "") ? saveDirectory + "/" + saveDirectoryName : saveDirectory;
    if (!savePath.empty() && savePath.back() != '/' && savePath.back() != '\\') savePath += "/";
    if (gSystem->AccessPathName(savePath.c_str())) { gSystem->mkdir(savePath.c_str(), true); }

    // ------------------
    // Linear Scale Canvas
    // ------------------
    TCanvas TempCanvas("TempCanvas", "Histograms - Linear Scale", canvasWidth, canvasHeight);
    // TCanvas TempCanvas("TempCanvas", "Histograms - Linear Scale", 1000 * nCols, 450 * nRows);
    // TCanvas TempCanvas("TempCanvas", "Histograms - Linear Scale", 1000 * nCols, 750 * nRows);
    TempCanvas.Divide(nCols, nRows);

    for (size_t i = 0; i < nHistos; ++i) {
        TempCanvas.cd(padMapping[i]);
        // gPad->SetGrid();
        gPad->SetBottomMargin(0.14);
        gPad->SetLeftMargin(0.16);
        gPad->SetRightMargin(0.12);

        if (IsHistogramEmpty(histograms[i])) {
            DrawEmptyHistogramNotice(0.2, 0.4, 0.8, 0.6);
        } else if (histograms[i]->InheritsFrom(TH1D::Class())) {
            if (nHistos != 5) { gPad->SetGrid(); }

            ((TH1D *)histograms[i])->SetStats(1);
            gStyle->SetOptStat("ourmen");

            ((TH1D *)histograms[i])->GetYaxis()->SetTitleOffset(1.3);
            // ((TH1D *)histograms[i])->GetYaxis()->SetTitleOffset(1.0);
            ((TH1D *)histograms[i])->GetXaxis()->SetTitleOffset(1.0);
            ((TH1D *)histograms[i])->SetLineColor(kBlue);
            ((TH1D *)histograms[i])->SetLineWidth(1);
            ((TH1D *)histograms[i])->SetLineStyle(1);
            ((TH1D *)histograms[i])->Draw();
            gPad->Update();  // So that statbox will be found

            TPaveStats *stats = (TPaveStats *)((TH1 *)histograms[i])->FindObject("stats");
            if (stats) {
                double dx = stats->GetX2NDC() - stats->GetX1NDC();
                double dy = stats->GetY2NDC() - stats->GetY1NDC();
                stats->SetX2NDC(0.98);
                stats->SetX1NDC(0.98 - dx);
                stats->SetY2NDC(0.875);
                stats->SetY1NDC(0.875 - dy);
            }
        } else if (histograms[i]->InheritsFrom(TH2D::Class())) {
            gPad->SetGrid();

            TH2D *h2 = (TH2D *)histograms[i];

            h2->SetStats(1);
            gStyle->SetOptStat("ourmen");

            h2->GetYaxis()->SetTitleOffset(1.0);

            h2->Draw("COLZ");
            gPad->Update();
            TPaletteAxis *palette = (TPaletteAxis *)h2->GetListOfFunctions()->FindObject("palette");
            if (palette) palette->SetY2NDC(0.475);
            gPad->Modified();
            gPad->Update();

            TPaveStats *stats = (TPaveStats *)((TH1 *)histograms[i])->FindObject("stats");
            if (stats) {
                double dx = stats->GetX2NDC() - stats->GetX1NDC();
                double dy = stats->GetY2NDC() - stats->GetY1NDC();
                stats->SetX2NDC(0.98);
                stats->SetX1NDC(0.98 - dx);
                stats->SetY2NDC(0.875);
                stats->SetY1NDC(0.875 - dy);
            }
        } else if (histograms[i]->InheritsFrom(THStack::Class())) {
            if (nHistos != 5) { gPad->SetGrid(); }

            DrawTHStack((THStack *)histograms[i], /* useLogScale = */ false);
        } else {
            std::cerr << "\n\nhistogram_functions::CompareHistograms: Warning: Object " << i << " is not a recognized histogram type!\n\n" << std::endl;
        }
    }

    TempCanvas.Update();
    TempCanvas.Draw();

    std::string linearFile = (ComparisonName != "") ? savePath + ComparisonName + "_linear_scale.pdf" : savePath + "comparison_linear_scale.pdf";
    TempCanvas.SaveAs(linearFile.c_str());

    // ------------------
    // Log Scale Canvas
    // ------------------

    std::string logDir = savePath + "log_scale_plots/";
    if (gSystem->AccessPathName(logDir.c_str())) { gSystem->mkdir(logDir.c_str(), true); }

    TCanvas TempCanvas_log("TempCanvas_log", "Histograms - Log Scale", 1000 * nCols, 750 * nRows);
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
            ((TH1D *)histograms[i])->SetLineColor(kBlue);
            ((TH1D *)histograms[i])->SetLineWidth(1);
            ((TH1D *)histograms[i])->SetLineStyle(1);
            ((TH1D *)histograms[i])->Draw();
            gPad->Update();  // So that statbox will be found

            TPaveStats *stats = (TPaveStats *)((TH1 *)histograms[i])->FindObject("stats");
            if (stats) {
                double dx = stats->GetX2NDC() - stats->GetX1NDC();
                double dy = stats->GetY2NDC() - stats->GetY1NDC();
                stats->SetX2NDC(0.98);
                stats->SetX1NDC(0.98 - dx);
                stats->SetY2NDC(0.875);
                stats->SetY1NDC(0.875 - dy);
            }
        } else if (histograms[i]->InheritsFrom(TH2D::Class())) {
            gPad->SetLogz(1);
            TH2D *h2 = (TH2D *)histograms[i];
            h2->Draw("COLZ");
            gPad->Update();
            TPaletteAxis *palette = (TPaletteAxis *)h2->GetListOfFunctions()->FindObject("palette");
            if (palette) palette->SetY2NDC(0.475);
            gPad->Modified();
            gPad->Update();

            TPaveStats *stats = (TPaveStats *)((TH1 *)histograms[i])->FindObject("stats");
            if (stats) {
                double dx = stats->GetX2NDC() - stats->GetX1NDC();
                double dy = stats->GetY2NDC() - stats->GetY1NDC();
                stats->SetX2NDC(0.98);
                stats->SetX1NDC(0.98 - dx);
                stats->SetY2NDC(0.875);
                stats->SetY1NDC(0.875 - dy);
            }
        } else if (histograms[i]->InheritsFrom(THStack::Class())) {
            DrawTHStack((THStack *)histograms[i], /* useLogScale = */ true);
        }
    }

    TempCanvas_log.Update();
    TempCanvas_log.Draw();

    std::string logFile = (ComparisonName != "") ? logDir + ComparisonName + "_log_scale.pdf" : logDir + "comparison_log_scale.pdf";
    TempCanvas_log.SaveAs(logFile.c_str());
}

};  // namespace histogram_functions

#endif  // HISTOGRAM_FUNCTIONS_H
