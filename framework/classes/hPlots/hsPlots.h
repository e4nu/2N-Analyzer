//
// Created by Alon Sportes on 11/04/2025.
//

// #ifndef HSPLOTS_H
// #define HSPLOTS_H
#ifdef HSPLOTS_H

#include <TCanvas.h>
#include <TClass.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TString.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/*
Example usage:

std::vector<std::vector<double>> slices = {{0, 1}, {1, 2}, {2, 3}};
std::vector<TObject*> HistoList;
std::vector<TH1*> SlicedHistoList;

hsPlots myPlots(slices, hsPlots::TH2D_TYPE, HistoList, &SlicedHistoList,
                "theta_phi", "Theta vs Phi;#phi [deg];#theta [deg]",
                100, -180, 180, 100, 0, 180, "theta");

// Fill with: theta slice variable, phi, theta
myPlots.Fill(1.5, 45, 90);

// Save plots to disk
myPlots.SaveHistograms("output_dir", "hist_output");
*/

// hsPlots manages a list of sliced histograms (TH1D or TH2D) created from user-defined ranges.
// Histogram objects are stored externally and passed via pointer, enabling ownership control outside the class.

class hsPlots {
   public:
    enum HistoType { TH1D_TYPE, TH2D_TYPE };

    // hsPlots constructor ----------------------------------------------------------------------------------------------------------------------------------------------
    // @param SlicedHistoListPtr Pointer to a vector where the histograms will be stored.
    //        The caller is responsible for managing the lifetime of the vector and the histograms inside it.
    hsPlots(const std::vector<std::vector<double>>& sliceLimits, HistoType type, std::vector<TObject*>& HistoList, std::vector<TObject*>* SlicedHistoListPtr, const std::string& baseName,
            const std::string& titleTemplate, const int& nbinsX, const double& xlow, const double& xup, const int& nbinsY, const double& ylow, const double& yup, std::string slice_var);

    // hsPlots Destructor ----------------------------------------------------------------------------------------------------------------------------------------------
    // Destructor deletes all histograms pointed to by SlicedHistoListPtr.
    // NOTE: Only use this class if you intend the class to own and delete those histograms.
    ~hsPlots();

    // Fill function ----------------------------------------------------------------------------------------------------------------------------------------------------

    void Fill(double sliceVar, double x, double y = 0, double weight = 1.0);

    // GetSlicedHistoList function --------------------------------------------------------------------------------------------------------------------------------------
    // Returns a copy of the vector of histogram pointers. Original ownership is not transferred.
    std::vector<TObject*> GetSlicedHistoList() const;

    // DrawEmptyHistogramNotice function --------------------------------------------------------------------------------------------------------------------------------

    // This function saves some reusable code. It is also defined in histograms_functions, yet it is placed here to avoid include errors
    static void DrawEmptyHistogramNotice(double x_1, double y_1, double x_2, double y_2, double diplayTextSize = 0.1) {
        TPaveText* displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->AddText("Empty histogram");
        displayText->SetTextAlign(22);
        displayText->Draw();
    }

    // SaveHistograms function -------------------------------------------------------------------------------------------------------------------------------------

    void SaveHistograms(const std::string& outputDir, const std::string& baseFileName) const;

   private:
    bool ForceFullVarRange = false;
    
    // SliceLimits: 2D vector of slice limits, each inner vector contains two elements: [lower_limit, upper_limit]
    std::vector<std::vector<double>> SliceLimits;

    // Pointer to externally managed vector of histograms.
    // Each histogram corresponds to a slice defined in SliceLimits.
    // The external owner is responsible for memory deallocation.
    std::vector<TObject*>* SlicedHistoListPtr;

    // histoType: type of histogram (TH1D or TH2D)
    // This is used to determine how to create the histograms in the constructor
    HistoType histoType;

    // FindSliceIndex function ------------------------------------------------------------------------------------------------------------------------------------------

    int FindSliceIndex(double value);
};

#endif  // HSPLOTS_H