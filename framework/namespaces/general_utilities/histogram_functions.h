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

namespace bt = basic_tools;

namespace histogram_functions {

// FillByInt1D function -------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * Fills a 1D histogram with integer-based bin indexing.
 *
 * @param hist  Pointer to the TH1 histogram to be filled.
 * @param bin   Bin index (0-based).
 * @param weight  Optional weight for the bin content (default is 1.0).
 */
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

/**
 * Fills a 2D histogram with integer-based bin indexing.
 *
 * @param hist  Pointer to the TH2 histogram to be filled.
 * @param xbin  Bin index along the X-axis (0-based).
 * @param ybin  Bin index along the Y-axis (0-based).
 * @param weight  Optional weight for the bin content (default is 1.0).
 */
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

/**
 * Fills the appropriate hsPlots histogram(s) based on the interaction type flags.
 *
 * This function dispatches filling events to the correct hsPlots objects for
 * all interactions and for each specific interaction type (QEL, MEC, RES, DIS).
 *
 * @param hsPlots_All_Int   hsPlots object for all interactions
 * @param hsPlots_QEL       hsPlots object for QEL interactions
 * @param hsPlots_MEC       hsPlots object for MEC interactions
 * @param hsPlots_RES       hsPlots object for RES interactions
 * @param hsPlots_DIS       hsPlots object for DIS interactions
 * @param type              Histogram type (TH1D_TYPE or TH2D_TYPE)
 * @param qel               True if this event is QEL
 * @param mec               True if this event is MEC
 * @param res               True if this event is RES
 * @param dis               True if this event is DIS
 * @param Slice_variable    The slicing variable (usually for multi-histograms)
 * @param Variable_x        X value to fill
 * @param Variable_y        Y value to fill (for 2D histograms)
 * @param Weight            Optional event weight (default 1.0)
 */
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

// FitPeakToGaussian functions ------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Fits a Gaussian function to the peak of a histogram and returns the mean and its error.
 *
 * If fit limits are not provided, they are inferred based on the histogram peak and Ebeam status.
 *
 * @param hist Pointer to the TH1D histogram to be fitted.
 * @param fitLimits Optional vector of two doubles specifying fit range [min, max].
 * @return std::pair<double, double> containing the fitted mean and its uncertainty.
 *
 * @example
 *   auto [mean, error] = fit_peak_gaussian(h1);
 *   std::cout << "Peak at: " << mean << " ± " << error << std::endl;
 */
// std::pair<double, double> FitPeakToGaussian(TH1D *hist, std::vector<double> fitLimits = {}) {
//     double fitMin, fitMax;

//     // Return NaNs if histogram is empty
//     if (hist->GetEntries() == 0) {
//         std::cerr << "Histogram is empty. Returning NaN." << std::endl;
//         return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
//     }

//     // Use default fit limits based on histogram characteristics if none provided
//     if (fitLimits.empty()) {
//         // If no limits are provided, use the histogram's peak center
//         double peakCenter = hist->GetBinCenter(hist->GetMaximumBin());

//         if (bt::FindSubstring(hist->GetName(), "_e_") || bt::FindSubstring(hist->GetName(), "_pipCD_") || bt::FindSubstring(hist->GetName(), "_pimCD_")) {
//             if (peakCenter < 0) {
//                 // If peak is negative, set limits accordingly
//                 fitMin = -std::fabs(peakCenter * 1.1);
//                 fitMax = -std::fabs(peakCenter * 0.9);
//             } else {
//                 // If peak is positive, set limits accordingly
//                 fitMin = std::fabs(peakCenter * 0.9);
//                 fitMax = std::fabs(peakCenter * 1.1);
//             }
//         } else {
//             // if (Ebeam_status_1 == "2GeV") {
//             //     if (peakCenter < 0) {
//             //         fitMin = -std::fabs(peakCenter * 1.4);
//             //         fitMax = -std::fabs(peakCenter * 0.6);
//             //     } else {
//             //         fitMin = std::fabs(peakCenter * 0.6);
//             //         fitMax = std::fabs(peakCenter * 1.4);
//             //     }
//             // } else {
//                 if (peakCenter < 0) {
//                     fitMin = -std::fabs(peakCenter * 1.2);
//                     fitMax = -std::fabs(peakCenter * 0.8);
//                 } else {
//                     fitMin = std::fabs(peakCenter * 0.8);
//                     fitMax = std::fabs(peakCenter * 1.2);
//                 }
//             // }
//         }
//     } else if (fitLimits.size() == 2) {
//         // Use provided fit limits
//         fitMin = fitLimits[0];
//         fitMax = fitLimits[1];
//     } else {
//         // Return NaNs if fitLimits vector is invalid
//         std::cerr << "Error: fitLimits must contain exactly two elements." << std::endl;
//         return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
//     }

//     // Construct and fit a Gaussian
//     TF1 *fit = new TF1("fit", "gaus", fitMin, fitMax);
//     hist->Fit(fit, "RQ");  // R = use range, Q = quiet

//     // Set visual color and attach to histogram
//     fit->SetLineColor(kViolet);
//     hist->GetListOfFunctions()->Clear();
//     hist->GetListOfFunctions()->Add(fit);

//     // Extract fit results
//     double mean = fit->GetParameter(1);
//     double error = fit->GetParError(1);
//     auto result = std::make_pair(mean, error);

//     // Clean up fit object
//     delete fit;

//     return result;
// }

// SanitizeForBookmark functions ----------------------------------------------------------------------------------------------------------------------------------------

/**
 * Sanitizes a string to make it safe for use as a PDF bookmark title.
 *
 * - Replaces "#pi^{+}" with "pi plus"
 * - Replaces "#pi^{-}" with "pi minus"
 * - Removes all characters except alphanumerics, space, '-', and '_'
 *
 * @param s The original string to sanitize
 * @return A sanitized version of the string suitable for bookmarks
 */
std::string SanitizeForBookmark(const std::string &s) {
    std::string out;
    for (char c : s) {
        if (c == '>' || (c >= 32 && c != '/' && c != '\\'))  // keep '>' and printable characters except '/' and '\'
            out += c;
    }
    return out;
}
// std::string SanitizeForBookmark(const std::string &s) {
//     std::string modified = s;
//     size_t pos;

//     // Replace #pi^{+} with "pi plus"
//     while ((pos = modified.find("#pi^{+}")) != std::string::npos) { modified.replace(pos, 7, "pi plus"); }

//     // Replace #pi^{-} with "pi minus"
//     while ((pos = modified.find("#pi^{-}")) != std::string::npos) { modified.replace(pos, 7, "pi minus"); }

//     std::string out;
//     for (char c : modified) {
//         if (isalnum(c) || c == ' ' || c == '-' || c == '_') {
//             out += c;  // Only append allowed characters
//         }
//     }

//     return out;
// }

// ReassignPDFBookmarks functions ---------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Reassigns cleaned and organized bookmarks to a PDF file.
 *
 * This function is used to reprocess bookmarks in a PDF file using a Java tool. It:
 *   1. Extracts the current bookmarks to a JSON file using the Java tool.
 *   2. Strips all existing bookmarks from the PDF using Ghostscript.
 *   3. Reapplies the bookmarks from the JSON to the stripped PDF using the Java tool.
 *
 * The Java class `ReassignBookmarksTool` must be present and compiled in the specified working directory.
 * Its required dependencies (PDFBox, Jackson, etc.) must be present in the `lib/` subdirectory.
 *
 * @param WorkingDir     Absolute path to the directory containing the Java ReassignBookmarksTool and its `lib/` folder.
 * @param inputPDF       Full path to the input PDF file whose bookmarks are to be reassigned.
 * @param outputPDF      Full path to the output PDF file that will be created with the reassigned bookmarks.
 * @param hierarchical   Optional flag indicating whether bookmarks should be structured hierarchically.
 *
 * @note This function calls external tools (`java`, `gs`) via `system()` and assumes they are available in the environment.
 */
void ReassignPDFBookmarks(const std::string WorkingDir, const std::string &inputPDF, const std::string &outputPDF, bool hierarchical = false) {
    std::string toolDir = WorkingDir + "framework/java/ReassignBookmarksTool/";
    std::string libDir = toolDir + "lib/*";
    std::string classpath = toolDir + ":" + libDir;  // include toolDir explicitly

    std::string bookmarksJSON = inputPDF.substr(0, inputPDF.find_last_of('/')) + "/bookmarks.json";

    std::string extractCmd = "java -cp \"" + classpath + "\" ReassignBookmarksTool extract \"" + inputPDF + "\" \"" + bookmarksJSON + "\"";

    std::string noBookmarkPDF = inputPDF.substr(0, inputPDF.find_last_of('/')) + "/no_bookmarks.pdf";
    // std::string noBookmarkPDF = "no_bookmarks.pdf";

    std::string stripCmd = "java -cp \"" + classpath + "\" ReassignBookmarksTool strip \"" + inputPDF + "\" \"" + noBookmarkPDF + "\" preserveText";
    // std::string gsCmd = "gs -q -o \"" + noBookmarkPDF + "\" -sDEVICE=pdfwrite -dSAFER -dBATCH -dNOPAUSE -dNoOutputFonts -dPDFSETTINGS=/prepress \"" + inputPDF + "\"";

    std::string reassignCmd = "java -cp \"" + classpath + "\" ReassignBookmarksTool reassign \"" + noBookmarkPDF + "\" \"" + bookmarksJSON + "\" \"" + outputPDF + "\"";

    if (hierarchical) reassignCmd += " hierarchical";

    // Print commands (with color formatting)
    std::cout << "\n";
    std::cout << "\033[33m" << "Classpath:     " << "\033[0m" << classpath << "\n";
    std::cout << "\033[33m" << "bookmarksJSON: " << "\033[0m" << bookmarksJSON << "\n";
    std::cout << "\033[33m" << "extractCmd:    " << "\033[0m" << extractCmd << "\n";
    std::cout << "\033[33m" << "stripCmd:      " << "\033[0m" << stripCmd << "\n";
    std::cout << "\033[33m" << "reassignCmd:   " << "\033[0m" << reassignCmd << "\n\n";

    // Run steps
    system(extractCmd.c_str());
    system(stripCmd.c_str());
    system(reassignCmd.c_str());

    std::cout << "\n";
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
        if (bt::FindSubstring(str, originToReplace)) {
            str = bt::ReplaceSubstring(str, originToReplace, replacement);
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
        if (bt::FindSubstring(str, originToReplace)) {
            str = bt::ReplaceSubstring(str, originToReplace, replacement);
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
        if (bt::FindSubstring(str, originToReplace)) {
            str = bt::ReplaceSubstring(str, originToReplace, replacement);
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

        if (bt::FindSubstring(title, originToReplace)) {
            title = bt::ReplaceSubstring(title, originToReplace, replacement);
            hist->SetTitle(title.c_str());
        }

        if (bt::FindSubstring(xLabel, originToReplace)) {
            xLabel = bt::ReplaceSubstring(xLabel, originToReplace, replacement);
            hist->GetXaxis()->SetTitle(xLabel.c_str());
        }

        if (bt::FindSubstring(yLabel, originToReplace)) {
            yLabel = bt::ReplaceSubstring(yLabel, originToReplace, replacement);
            hist->GetYaxis()->SetTitle(yLabel.c_str());
        }
    };

    updateTitle(simHistogram);
    updateTitle(dataHistogram);
}

// DrawAndSaveHistogramsToPDF function ----------------------------------------------------------------------------------------------------------------------------------

/**
 * DrawAndSaveHistogramsToPDF
 * ---------------------------
 * Draws a list of 1D and 2D histograms (TH1 and TH2) from the input vector and saves them as individual pages
 * in a multipage PDF file. Empty histograms are marked with a notice instead of being drawn.
 *
 * @param HistoList     Vector of TH1 or TH2 pointers (casted as TObject).
 * @param outputPDF     Full path to the output multipage PDF file.
 * @param title         Optional title drawn at the top of each page.
 */
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
    text.DrawLatex(0.2, 0.65, ("Beam energy: " + bt::GetBeamEnergyFromDouble(beamE) + " [GeV]").c_str());

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
        if (!bt::FindSubstring(objName, histNameSubstring) || !obj->IsA()->InheritsFrom(desiredClass.c_str())) {
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

/**
 * DrawEmptyHistogramNotice
 * -------------------------
 * Displays a placeholder message on the current canvas when a histogram is empty,
 * indicating that no data is available for the given plot.
 *
 * @param label  The title or label of the missing/empty histogram to show in the notice.
 */
void DrawEmptyHistogramNotice(double x_1, double y_1, double x_2, double y_2, double diplayTextSize = 0.1) {
    TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
    displayText->SetTextSize(diplayTextSize);
    displayText->SetFillColor(0);
    displayText->AddText("Empty histogram");
    displayText->SetTextAlign(22);
    displayText->Draw();
}

// IsHistogramEmpty function --------------------------------------------------------------------------------------------------------------------------------------------

/**
 * IsHistogramEmpty
 * ----------------
 * Checks whether a given histogram (1D or 2D) is empty, i.e., has zero entries.
 *
 * @param hist  Pointer to a TH1 histogram (TH1D, TH1F, TH2D, etc.).
 * @return      True if the histogram has no entries, false otherwise.
 */
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

/**
 * DrawTHStack
 * -----------
 * Draws a THStack of histograms, applies styling, sets axis labels and titles, and adds an optional legend.
 * Useful for stacked histogram visualizations.
 *
 * @param stack     Pointer to the THStack object to draw.
 * @param legend    Pointer to a TLegend object (optional, may be nullptr).
 * @param xtitle    X-axis title.
 * @param ytitle    Y-axis title.
 * @param logY      Whether to set the Y-axis to log scale.
 */
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

// FixPDFOrientation ----------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Fixes the orientation of a PDF file to prevent it from being rotated when inserted into software like PowerPoint.
 *
 * This function uses Ghostscript (gs) to rewrite the PDF with the auto-rotation disabled. By default, many programs such as
 * PowerPoint rely on internal rotation metadata embedded in PDF files to decide how to render the page, which can result in
 * sideways or upside-down plots when inserting ROOT-generated PDFs.
 *
 * The Ghostscript command removes the auto-rotation flag by setting `-dAutoRotatePages=/None`. It rewrites the PDF quietly
 * (using `-q`) and with high quality (`-dPDFSETTINGS=/prepress`), and then replaces the original file with the corrected one.
 *
 * @param pdfFilePath The full path to the PDF file to be fixed. The file must have a ".pdf" extension.
 *
 * Example usage:
 *   histogram_functions::FixPDFOrientation("/path/to/plot.pdf");
 */
void FixPDFOrientation(const std::string &pdfFilePath) {
    // Ensure the file ends with ".pdf"
    if (pdfFilePath.find(".pdf") != std::string::npos) {
        // Construct Ghostscript command:
        // -q               : quiet mode (no standard output)
        // -o file.tmp      : output file name
        // -sDEVICE=pdfwrite: output format is PDF
        // -dPDFSETTINGS=/prepress : high-quality output
        // -dAutoRotatePages=/None : disable automatic rotation
        // -dNOPAUSE -dBATCH : process without pausing or interactive input
        std::string fix_rotation_cmd = "gs -q -o \"" + pdfFilePath +
                                       ".tmp\" "
                                       "-sDEVICE=pdfwrite "
                                       "-dPDFSETTINGS=/prepress "
                                       "-dAutoRotatePages=/None "
                                       "-dNOPAUSE -dBATCH "
                                       "\"" +
                                       pdfFilePath + "\" && mv \"" + pdfFilePath + ".tmp\" \"" + pdfFilePath + "\"";

        // Execute the system command
        gSystem->Exec(fix_rotation_cmd.c_str());
    }
}

// CompareHistograms -------------------------------------------------------------------------------------------------------------------------------------------------------

void CompareHistograms(const std::vector<TObject *> &histograms, const std::string &saveDirectory, const std::string &saveDirectoryName = "", const std::string &ComparisonName = "") {
    size_t nHistos = histograms.size();

    if (nHistos != 2 && nHistos != 4 && nHistos != 5 && nHistos != 6) {
        std::cerr << "\n\nhistogram_functions::CompareHistograms: ERROR! Only supports 2, 4, 5, or 6 histograms!\n\n" << std::endl;
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
    } else if (nHistos == 5) {
        padMapping = {1, 2, 3, 5, 6};
    } else if (nHistos == 6) {
        padMapping = {1, 2, 3, 4, 5, 6};
    }

    // Create output directory if needed
    std::string savePath = (saveDirectoryName != "") ? saveDirectory + "/" + saveDirectoryName : saveDirectory;
    if (!savePath.empty() && savePath.back() != '/' && savePath.back() != '\\') savePath += "/";
    if (gSystem->AccessPathName(savePath.c_str())) { gSystem->mkdir(savePath.c_str(), true); }

    // ------------------
    // Linear Scale Canvas
    // ------------------
    TCanvas TempCanvas("TempCanvas", "Histograms - Linear Scale", canvasWidth, canvasHeight);
    TempCanvas.Divide(nCols, nRows);

    for (size_t i = 0; i < nHistos; ++i) {
        TempCanvas.cd(padMapping[i]);
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
            ((TH1D *)histograms[i])->GetXaxis()->SetTitleOffset(1.0);
            ((TH1D *)histograms[i])->SetLineColor(kRed);
            ((TH1D *)histograms[i])->SetMarkerColor(kRed);
            // ((TH1D *)histograms[i])->SetLineColor(kBlue);
            ((TH1D *)histograms[i])->SetLineWidth(1);
            ((TH1D *)histograms[i])->SetLineStyle(1);
            ((TH1D *)histograms[i])->Draw();
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
    FixPDFOrientation(linearFile);

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
            gPad->SetLogy(0);  // reset log scale if needed

            ((TH1D *)histograms[i])->SetMinimum(0.5);  // set positive Y-min to enable log scale
            ((TH1D *)histograms[i])->SetLineColor(kRed);
            ((TH1D *)histograms[i])->SetMarkerColor(kRed);
            // ((TH1D *)histograms[i])->SetLineColor(kBlue);
            ((TH1D *)histograms[i])->SetLineWidth(1);
            ((TH1D *)histograms[i])->SetLineStyle(1);
            ((TH1D *)histograms[i])->Draw();
            gPad->SetLogy(1);
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
    FixPDFOrientation(linearFile);
}

};  // namespace histogram_functions

#endif  // HISTOGRAM_FUNCTIONS_H
