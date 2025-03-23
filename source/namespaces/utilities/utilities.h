//
// Created by Alon Sportes on 24/02/2025.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TMath.h>

#include <iostream>
#include <map>
#include <tuple>

#include "../basic_tools/basic_tools.h"
#include "../constants/constants.h"
#include "../lists/lists.h"
// #include "../truth_analysis_functions/truth_analysis_functions.h"

using namespace std;

namespace utilities {
using namespace constants;
using namespace basic_tools;
using namespace analysis_math;
using namespace lists;

// ConfigRegion function ------------------------------------------------------------------------------------------------------------------------------------------------

const string ConfigRegion(const string &Histogram2DNameCopy) { return FindSubstring(Histogram2DNameCopy, "CD") ? "CD" : "FD"; }

// ConfigSampleType function --------------------------------------------------------------------------------------------------------------------------------------------

const string ConfigSampleType(const string &SampleName) {
    if (FindSubstring(SampleName, "sim")) {
        return "s";
    } else if (FindSubstring(SampleName, "data")) {
        return "d";
    }

    return "";
}

// ReplaceSubstring function --------------------------------------------------------------------------------------------------------------------------------------------

// Function to replace one substring with another
string ReplaceSubstring(const string &input, const string &toReplace, const string &replaceWith) {
    size_t pos = input.find(toReplace);

    if (pos == string::npos) {
        // If 'toReplace' is not found, return the original string
        return input;
    }

    return input.substr(0, pos) + replaceWith + input.substr(pos + toReplace.length());
}

// TitleAligner functions -----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void TitleAligner(T *obj, string &title, string &xLabel, const string &originToReplace, const string &replacement) {
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
void TitleAligner(T *obj, string &title, string &xLabel, string &yLabel, const string &originToReplace, const string &replacement) {
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

void TitleAligner(TH1D *simHistogram, TH1D *dataHistogram, const string &originToReplace, const string &replacement) {
    auto updateTitle = [&](TH1D *hist) {
        string title = hist->GetTitle();
        string xLabel = hist->GetXaxis()->GetTitle();
        string yLabel = hist->GetYaxis()->GetTitle();

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

TH1 *GetHistogramFromVector(const std::vector<TH1 *> &HistoList, const string &searchString, const string &searchStringOption = "name") {
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

void FillHistogramByProcess(double Variable, TH1D *Histogram_All_Int, TH1D *Histogram_QE, TH1D *Histogram_MEC, TH1D *Histogram_RES, TH1D *Histogram_DIS, bool qel, bool mec, bool res,
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

double GetHistogramEntriesFromVector(const std::vector<TH1 *> &HistoList, const string &searchString, const string &searchStringOption = "name") {
    if (TH1 *Histogram = GetHistogramFromVector(HistoList, searchString, searchStringOption)) { return Histogram->GetEntries(); }
    return -1;  // Return -1 if no match is found
}

// FitFunction function --------------------------------------------------------------------------------------------------------------------------------------------------

Double_t FitFunction(Double_t *v, Double_t *par, const int NumOfParameters = 3) {
    if (NumOfParameters != 2 && NumOfParameters != 3) {
        cerr << "utilities::FitFunction: invalid number of parameters! Choose 2 or 3. Aborting..." << endl;
        exit(1);
    }

    Double_t arg = (NumOfParameters == 2) ? (par[1] != 0 ? (v[0] - 1) / par[1] : 0) : (par[2] != 0 ? (v[0] - par[1]) / par[2] : 0);

    return par[0] * TMath::Exp(-0.5 * arg * arg);
}

}  // namespace utilities

#endif  // UTILITIES_H
