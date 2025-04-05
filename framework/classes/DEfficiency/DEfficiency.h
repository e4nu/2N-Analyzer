//
// Created by Alon Sportes on 08/07/2023.
//

// #ifndef DEFFICIENCY_H
// #define DEFFICIENCY_H
#ifdef DEFFICIENCY_H

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

    #include "../../namespaces/general_utilities/utilities.h"
    #include "../hPlots/hPlot1D.cpp"

using namespace utilities;

class DEfficiency {
   private:
    bool plot_errorbars = true;
    bool rebin_plots = false;

    //    TList *ACorr_data = new TList();

    /* Loading histograms */
    TH1D *Histogram1D_REC, *RPlot_Clone, *RPlot_Clone_test, *RPlot_Clone_test_rebined;
    std::string RPlot_Clone_StatsTitle, RPlot_Clone_test_StatsTitle, RPlot_Clone_test_rebined_StatsTitle;

    TH1D *Histogram1D_Truth, *TLPlot_Clone, *TLPlot_Clone_test, *TLPlot_Clone_test_rebined;
    std::string TLPlot_Clone_StatsTitle, TLPlot_Clone_test_StatsTitle, TLPlot_Clone_test_rebined_StatsTitle;

    std::string EffAndACorr_SaveNamePath;

   public:
    // constructors ---------------------------------------------------------------------------------------------------------------------------------------------------------

    // Default constructor:
    DEfficiency() = default;

    // ResetHistograms function ---------------------------------------------------------------------------------------------------------------------------------------------

    void ResetHistograms();

    // LoadHistograms functions ---------------------------------------------------------------------------------------------------------------------------------------------

    void LoadHistograms(const std::string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot);

    void LoadHistograms(const std::string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot);

    // DrawACorrHistograms function ----------------------------------------------------------------------------------------------------------------------------------------------

    void DrawACorrHistograms(bool save_ACorr_data, const std::string &SampleName, TList *Histogram_list, TList *ACorr_data, std::string &ACorr_data_Dir);

    // DrawAndSaveACorrPlots function ---------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveACorrPlots(bool save_ACorr_data, const std::string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list, TList *ACorr_data,
                               std::string &ACorr_data_Dir);

    void DrawAndSaveACorrPlots(bool save_ACorr_data, const std::string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot, TList *Histogram_list, TList *ACorr_data,
                               std::string &ACorr_data_Dir);

    // DrawAndSaveHistogram1D function --------------------------------------------------------------------------------------------------------------------------------------

    void DrawAndSaveHistogram1D(TCanvas *HistCanvas, TList *Histogram_list, TFolder *Histogram_folder, TH1D *Histogram1D, int LineStyle, int kColor, bool ShowStats, bool plot_errorbars,
                                bool rebin_plots, const std::string &HistSaveDir);
};

#endif  // DEFFICIENCY_H
