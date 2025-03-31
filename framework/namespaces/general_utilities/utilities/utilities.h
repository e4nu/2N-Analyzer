//
// Created by Alon Sportes on 24/02/2025.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <THStack.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "../analysis_math/analysis_math.h"
#include "../basic_tools/basic_tools.h"
#include "../constants/constants.h"
#include "../data_processor/data_processor.h"
#include "../histogram_functions/histogram_functions.h"
#include "../lists/lists.cpp"
#include "../reco_analysis_functions/reco_analysis_functions.h"
// #include "../truth_analysis_functions/truth_analysis_functions.h"

// using namespace std;

// TODO: split into header and source files!

namespace utilities {
using namespace constants;
using namespace basic_tools;
using namespace analysis_math;
using namespace lists;
using namespace data_processor;
using namespace histogram_functions;
using namespace reco_analysis_functions;

// ConfigRegion function ------------------------------------------------------------------------------------------------------------------------------------------------

inline const std::string ConfigRegion(const std::string &Histogram2DNameCopy) { return FindSubstring(Histogram2DNameCopy, "CD") ? "CD" : "FD"; }

// ConfigSampleType function --------------------------------------------------------------------------------------------------------------------------------------------

inline const std::string ConfigSampleType(const std::string &SampleName) {
    if (FindSubstring(SampleName, "sim")) {
        return "s";
    } else if (FindSubstring(SampleName, "data")) {
        return "d";
    }

    return "";
}

// ReplaceSubstring function --------------------------------------------------------------------------------------------------------------------------------------------

// Function to replace one substring with another
inline std::string ReplaceSubstring(const std::string &input, const std::string &toReplace, const std::string &replaceWith) {
    size_t pos = input.find(toReplace);

    if (pos == string::npos) {
        // If 'toReplace' is not found, return the original string
        return input;
    }

    return input.substr(0, pos) + replaceWith + input.substr(pos + toReplace.length());
}

// TitleAligner functions -----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline void TitleAligner(T *obj, std::string &title, std::string &xLabel, const std::string &originToReplace, const std::string &replacement) {
    auto updateTitle = [&](string &str, auto setTitleFunc) {
        if (FindSubstring(str, originToReplace)) {
            str = ReplaceSubstring(str, originToReplace, replacement);
            setTitleFunc(str.c_str());
        }
    };

    updateTitle(title, [&](const char *newTitle) { obj->SetTitle(newTitle); });
    updateTitle(xLabel, [&](const char *newTitle) { obj->GetXaxis()->SetTitle(newTitle); });
}

template <typename T>
inline void TitleAligner(T *obj, std::string &title, std::string &xLabel, std::string &yLabel, const std::string &originToReplace, const std::string &replacement) {
    auto updateTitle = [&](string &str, auto setTitleFunc) {
        if (FindSubstring(str, originToReplace)) {
            str = ReplaceSubstring(str, originToReplace, replacement);
            setTitleFunc(str.c_str());
        }
    };

    updateTitle(title, [&](const char *newTitle) { obj->SetTitle(newTitle); });
    updateTitle(xLabel, [&](const char *newTitle) { obj->GetXaxis()->SetTitle(newTitle); });
    updateTitle(yLabel, [&](const char *newTitle) { obj->GetYaxis()->SetTitle(newTitle); });
}

// TitleAligner function ------------------------------------------------------------------------------------------------------------------------------------------------

inline void TitleAligner(TH1D *simHistogram, TH1D *dataHistogram, const std::string &originToReplace, const std::string &replacement) {
    auto updateTitle = [&](TH1D *hist) {
        std::string title = hist->GetTitle();
        std::string xLabel = hist->GetXaxis()->GetTitle();
        std::string yLabel = hist->GetYaxis()->GetTitle();

        if (FindSubstring(title, originToReplace)) {
            title = ReplaceSubstring(title, originToReplace, replacement);
            hist->SetTitle(title.c_str());
        }

        if (FindSubstring(xLabel, originToReplace)) {
            xLabel = ReplaceSubstring(xLabel, originToReplace, replacement);
            hist->GetXaxis()->SetTitle(xLabel.c_str());
        }

        if (FindSubstring(yLabel, originToReplace)) {
            yLabel = ReplaceSubstring(yLabel, originToReplace, replacement);
            hist->GetYaxis()->SetTitle(yLabel.c_str());
        }
    };

    updateTitle(simHistogram);
    updateTitle(dataHistogram);
}

// GetHistogramFromVector function --------------------------------------------------------------------------------------------------------------------------------------

inline TH1 *GetHistogramFromVector(const std::vector<TH1 *> &HistoList, const std::string &searchString, const std::string &searchStringOption = "name") {
    for (const auto &hist : HistoList) {
        if (!hist) continue;

        if ((searchStringOption == "Name" || searchStringOption == "name") && hist->GetName() == searchString) {
            return hist;
        } else if ((searchStringOption == "Title" || searchStringOption == "title") && hist->GetTitle() == searchString) {
            return hist;
        }
    }

    return nullptr;  // Return nullptr if no match is found
}

// FillHistogramByProcess function --------------------------------------------------------------------------------------------------------------------------------------

inline void FillHistogramByProcess(double Variable, TH1D *Histogram_All_Int, TH1D *Histogram_QE, TH1D *Histogram_MEC, TH1D *Histogram_RES, TH1D *Histogram_DIS, bool qel, bool mec, bool res,
                                   bool dis, double weight = 1., bool IsMC = true, bool IsData = false) {
    Histogram_All_Int->Fill(Variable, weight);

    if (IsMC) {
        if (qel) {
            Histogram_QE->Fill(Variable, weight);
        } else if (mec) {
            Histogram_MEC->Fill(Variable, weight);
        } else if (res) {
            Histogram_RES->Fill(Variable, weight);
        } else if (dis) {
            Histogram_DIS->Fill(Variable, weight);
        }
    }
}

// GetHistogramEntries function -----------------------------------------------------------------------------------------------------------------------------------------

inline double GetHistogramEntriesFromVector(const std::vector<TH1 *> &HistoList, const std::string &searchString, const std::string &searchStringOption = "name") {
    if (TH1 *Histogram = GetHistogramFromVector(HistoList, searchString, searchStringOption)) { return Histogram->GetEntries(); }
    return -1;  // Return -1 if no match is found
}

// drawtext function ----------------------------------------------------------------------------------------------------------------------------------------------------

inline void drawtext() {
    Int_t i, n;
    Double_t x, y;
    TLatex l;

    l.SetTextSize(0.025);
    l.SetTextFont(0);
    //    l.SetTextFont(42);
    l.SetTextAlign(21);
    l.SetTextColor(kMagenta);
    //    l.SetTextColor(kBlue);
    l.SetLineColor(kWhite);

    auto g = (TGraph *)gPad->GetListOfPrimitives()->FindObject("Graph");
    n = g->GetN();

    for (i = 0; i < n; i++) {
        g->GetPoint(i, x, y);
        l.PaintText(x, y + 0.02, Form("(%4.5f,%4.1f)", x, y));
    }
}

};  // namespace utilities

#endif  // UTILITIES_H
