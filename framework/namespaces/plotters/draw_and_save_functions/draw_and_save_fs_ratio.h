//
// Created by Alon Sportes on 26/03/2025.
//

/*
to change FSR, change:

type function
title function
SettingSaveNames
SetSaveDir
SetXAxisTitle
SetYAxisTitle
*/

#ifndef DRAW_AND_SAVE_FS_RATIO_H
#define DRAW_AND_SAVE_FS_RATIO_H

#include <TFolder.h>
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
#include "../../../classes/hPlots/hPlot2D.h"
#include "../../../namespaces/general_utilities/utilities/utilities.h"

// using namespace std;
using namespace utilities;

// TODO: move this into a class with the proper functions

// TODO: finish debugging TFolder addition!

namespace draw_and_save_fs_ratio {
const bool Equi_z_2D = true;

// DrawAndSaveFSRatio in 1D plots ---------------------------------------------------------------------------------------------------------------------------------------

void DrawAndSaveFSRatio(const std::string &SampleName, const hPlot1D &pFDpCD_Plot, const hPlot1D &nFDpCD_Plot, TList *Histogram_list);

void DrawAndSaveFSRatio(const std::string &SampleName, const hPlot1D &pFDpCD_Plot, TH1D *nFDpCD_Plot, TList *Histogram_list);

void DrawAndSaveFSRatio(const std::string &SampleName, TH1D *pFDpCD_Plot, const std::string &pFDpCD_PlotSaveNamePath, TH1D *nFDpCD_Plot, TList *Histogram_list);

// DrawAndSaveFSRatio in 2D plots ---------------------------------------------------------------------------------------------------------------------------------------

void DrawAndSaveFSRatio(const std::string &SampleName, const hPlot2D &pFDpCD_Plot, const hPlot2D &nFDpCD_Plot, TList *Histogram_list);

void DrawAndSaveFSRatio(const std::string &SampleName, const hPlot2D &pFDpCD_Plot, TH2D *nFDpCD_Plot, TList *Histogram_list);

void DrawAndSaveFSRatio(const std::string &SampleName, TH2D *pFDpCD_Plot, const std::string &pFDpCD_PlotSaveNamePath, TH2D *nFDpCD_Plot, TList *Histogram_list);

};  // namespace draw_and_save_fs_ratio

#endif  // DRAW_AND_SAVE_FS_RATIO_H
