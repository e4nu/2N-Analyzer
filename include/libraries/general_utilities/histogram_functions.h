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
#include <TKey.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TObject.h>
#include <TPad.h>
#include <TPaletteAxis.h>
#include <TPaveStats.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>

#include <string>
#include <vector>

#include "../../AnalysisTools/hsPlots.h"
#include "basic_tools.h"

namespace bt = basic_tools;

namespace histogram_functions {

// FillByInt1D function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FillByInt1D function */
void FillByInt1D(TH1D* H1D_All_Int, TH1D* H1D_QEL, TH1D* H1D_MEC, TH1D* H1D_RES, TH1D* H1D_DIS, const bool& qel, const bool& mec, const bool& res, const bool& dis, const double& Variable,
                 const double& Weight);
#pragma endregion

// FillByInt2D function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FillByInt2D function */
void FillByInt2D(TH2D* H1D_All_Int, TH2D* H1D_QEL, TH2D* H1D_MEC, TH2D* H1D_RES, TH2D* H1D_DIS, const bool& qel, const bool& mec, const bool& res, const bool& dis, const double& Variable_x,
                 const double& Variable_y, const double& Weight);
#pragma endregion

// FillByInthsPlots function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FillByInthsPlots function */
void FillByInthsPlots(hsPlots& hsPlots_All_Int, hsPlots& hsPlots_QEL, hsPlots& hsPlots_MEC, hsPlots& hsPlots_RES, hsPlots& hsPlots_DIS, hsPlots::HistoType type, const bool& qel,
                      const bool& mec, const bool& res, const bool& dis, const double& Slice_variable, const double& Variable_x, const double& Variable_y, const double& Weight = 1.0);
#pragma endregion

// SanitizeForBookmark function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SanitizeForBookmark function */
std::string SanitizeForBookmark(const std::string& s);
#pragma endregion

// ReassignPDFBookmarks function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReassignPDFBookmarks function */
void ReassignPDFBookmarks(const std::string WorkingDir, const std::string& inputPDF, const std::string& outputPDF, bool hierarchical = false);
#pragma endregion

// TitleAligner function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* TitleAligner function */
template <typename T>
void TitleAligner(T* obj, std::string& title, std::string& xLabel, const std::string& originToReplace, const std::string& replacement) {
    auto updateTitle = [&](std::string& str, auto setTitleFunc) {
        if (bt::FindSubstring(str, originToReplace)) {
            str = bt::ReplaceSubstring(str, originToReplace, replacement);
            setTitleFunc(str.c_str());
        }
    };

    updateTitle(title, [&](const char* newTitle) { obj->SetTitle(newTitle); });
    updateTitle(xLabel, [&](const char* newTitle) { obj->GetXaxis()->SetTitle(newTitle); });
}
#pragma endregion

// TitleAligner function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* TitleAligner function */
template <typename T>
void TitleAligner(T* obj, std::string& title, std::string& xLabel, std::string& yLabel, const std::string& originToReplace, const std::string& replacement) {
    auto updateTitle = [&](std::string& str, auto setTitleFunc) {
        if (bt::FindSubstring(str, originToReplace)) {
            str = bt::ReplaceSubstring(str, originToReplace, replacement);
            setTitleFunc(str.c_str());
        }
    };

    updateTitle(title, [&](const char* newTitle) { obj->SetTitle(newTitle); });
    updateTitle(xLabel, [&](const char* newTitle) { obj->GetXaxis()->SetTitle(newTitle); });
    updateTitle(yLabel, [&](const char* newTitle) { obj->GetYaxis()->SetTitle(newTitle); });
}
#pragma endregion

// TitleAligner function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* TitleAligner function */
void TitleAligner(TObject* obj, const std::string& originToReplace, const std::string& replacement);
#pragma endregion

// TitleAligner function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* TitleAligner function */
void TitleAligner(TH1D* simHistogram, TH1D* dataHistogram, const std::string& originToReplace, const std::string& replacement);
#pragma endregion

// DrawAndSaveHistogramsToPDF function -----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveHistogramsToPDF function */
void DrawAndSaveHistogramsToPDF(TCanvas* MainCanvas, const std::vector<TObject*>& HistoList, const std::string& Histogram_OutPDF_fileName_str, const std::string& SampleName,
                                const std::string& VaryingSampleName, const double& beamE);
#pragma endregion

// FindHistogram function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FindHistogram function */
TObject* FindHistogram(TFile* file, const char* histNameSubstring, const std::string& desiredClass);
#pragma endregion

// DrawEmptyHistogramNotice function -------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawEmptyHistogramNotice function */
void DrawEmptyHistogramNotice(double x_1, double y_1, double x_2, double y_2, double diplayTextSize = 0.1);
#pragma endregion

// IsHistogramEmpty function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* IsHistogramEmpty function */
bool IsHistogramEmpty(TObject* obj);
#pragma endregion

// DrawTHStack function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawTHStack function */
void DrawTHStack(THStack* stack, bool useLogScale);
#pragma endregion

// FixPDFOrientation function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FixPDFOrientation function */
void FixPDFOrientation(const std::string& pdfFilePath);
#pragma endregion

// Shared histogram title/save helpers ----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Shared histogram title/save helpers */
inline std::string JoinHistogramContext(const std::string& reactions, const std::string& finalState, const std::string& detectorRegion) {
    std::string context;

    if (!reactions.empty()) { context += reactions; }
    if (!finalState.empty()) { context += (context.empty() ? "" : ", ") + finalState; }
    if (!detectorRegion.empty()) { context += (context.empty() ? "" : ", ") + detectorRegion; }

    return context;
}

inline std::string JoinHistogram2DContext(const std::string& reactions, const std::string& finalState, const std::string& detectorRegion) {
    return JoinHistogramContext(reactions, finalState, detectorRegion);
}

inline std::string BuildHistogramTitle(const std::string& baseTitle, const std::string& reactions, const std::string& finalState, const std::string& detectorRegion, bool normalized) {
    const std::string context = JoinHistogramContext(reactions, finalState, detectorRegion);
    return baseTitle + (context.empty() ? "" : " (" + context + ")") + (normalized ? " - Normalized" : "");
}

inline std::string BuildStandardHistTitle(const std::string& histogramTitle, const std::string& histogramTitleReactions, const std::string& finalState, bool normalized) {
    return histogramTitle + " (" + histogramTitleReactions + (finalState.empty() ? "" : ", " + finalState) + ")" + (normalized ? " - Normalized" : "");
}

inline std::string BuildDisplayHistTitle(const std::string& histogramTitle, const std::string& histogramTitleReactions, const std::string& finalState, bool title2) {
    return title2 ? (finalState.empty() ? histogramTitle : histogramTitle + " (" + finalState + ")")
                  : BuildStandardHistTitle(histogramTitle, histogramTitleReactions, finalState, false);
}

inline std::string BuildSaveName(const std::string& savePath, const std::string& sampleFlag, const std::string& saveName, const std::string& scaleSuffix,
                                 const std::string& detectorRegion = "") {
    return savePath + sampleFlag + saveName + scaleSuffix + (detectorRegion.empty() ? "" : "_" + detectorRegion) + ".pdf";
}

inline std::string BuildScaleSaveName(const std::string& histogramSaveNamePath, const std::string& histogramSaveName, const std::string& scaleSuffix,
                                      const std::string& finalState) {
    return histogramSaveNamePath + histogramSaveName + scaleSuffix + (finalState.empty() ? "" : "_" + finalState) + ".pdf";
}

inline std::string GetSampleNameFlag(const std::string& sampleName) { return bt::FindSubstring(sampleName, "sim") ? "s" : (bt::FindSubstring(sampleName, "data") ? "d" : ""); }

inline std::string Build2DScaleSaveName(const std::string& saveDir, const std::string& sampleFlag, const std::string& saveName, const std::string& scaleSuffix) {
    return saveDir + sampleFlag + saveName + scaleSuffix + ".pdf";
}

inline std::string Build2DScaleSaveNameInFolder(const std::string& saveDir, const std::string& folder, const std::string& sampleFlag, const std::string& saveName,
                                                const std::string& scaleSuffix) {
    return saveDir + "/" + folder + "/" + sampleFlag + saveName + scaleSuffix + ".pdf";
}
#pragma endregion

// CompareHistograms function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CompareHistograms function */
void CompareHistograms(const std::vector<TObject*>& histograms, const std::string& saveDirectory, const std::string& saveDirectoryName = "", const std::string& ComparisonName = "");
#pragma endregion

}  // namespace histogram_functions

#endif  // HISTOGRAM_FUNCTIONS_H
