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
hsPlots myPlots(slices, hsPlots::TH2D_TYPE, "theta_phi", "Theta vs Phi",
                100, -180, 180, 100, 0, 180);

// Fill with: momentum, phi, theta
myPlots.Fill(1.5, 45, 90);
*/

class hsPlots {
   public:
    enum HistoType { TH1D_TYPE, TH2D_TYPE };

    // hsPlots constructor ----------------------------------------------------------------------------------------------------------------------------------------------

    hsPlots(const std::vector<std::vector<double>>& sliceLimits, HistoType type, vector<bool>& HistoList_skipCleaning, vector<TObject*>& HistoList, const std::string& baseName,
            const std::string& titleTemplate, const int& nbinsX, const double& xlow, const double& xup, const int& nbinsY, const double& ylow, const double& yup, std::string slice_var);

    // hsPlots Destructor -----------------------------------------------------------------------------------------------------------------------------------------------

    // Destructor to clean up the dynamically allocated histograms
    ~hsPlots();

    // Fill function ----------------------------------------------------------------------------------------------------------------------------------------------------

    void Fill(double sliceVar, double x, double y = 0, double weight = 1.0);

    // GetSlicedHistoList function --------------------------------------------------------------------------------------------------------------------------------------

    std::vector<TH1*> GetSlicedHistoList() const;

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
    // SliceLimits: 2D vector of slice limits, each inner vector contains two elements: [lower_limit, upper_limit]
    std::vector<std::vector<double>> SliceLimits;

    // SlicedHistoList: vector of histograms, each histogram corresponds to a slice defined in SliceLimits
    std::vector<TH1*> SlicedHistoList;

    // histoType: type of histogram (TH1D or TH2D)
    // This is used to determine how to create the histograms in the constructor
    HistoType histoType;

    // FindSliceIndex function ------------------------------------------------------------------------------------------------------------------------------------------

    int FindSliceIndex(double value);
};

#endif  // HSPLOTS_H