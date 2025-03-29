//
// Created by Alon Sportes on 26/03/2025.
//

#ifndef DRAW_AND_SAVE_ACORR_PLOTS_H
#define DRAW_AND_SAVE_ACORR_PLOTS_H

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
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

#include "../../../classes/hPlots/hPlot1D.h"
#include "../../../namespaces/general_utilities/utilities/utilities.h"

// using namespace std;
using namespace utilities;

namespace draw_and_save_acorr_plots {

// DrawAndSaveACorrPlots function for momentum plots --------------------------------------------------------------------------------------------------------------------

void DrawAndSaveACorrPlots(bool save_ACorr_data, const std::string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list, TList *ACorr_data,
                           std::string &ACorr_data_Dir);

// DrawAndSaveACorrPlots function for angle plots -----------------------------------------------------------------------------------------------------------------------

void DrawAndSaveACorrPlots(bool save_ACorr_data, const std::string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot, TList *Histogram_list, TList *ACorr_data, std::string &ACorr_data_Dir);

};  // namespace draw_and_save_acorr_plots

#endif  // DRAW_AND_SAVE_ACORR_PLOTS_H
