//
// Created by Alon Sportes on 26/03/2025.
//

#ifndef FIT_FUNCTIONS_H
#define FIT_FUNCTIONS_H

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TExec.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>

#include <fstream>
#include <iostream>
#include <string>

#include "../../general_utilities/analysis_math/analysis_math_include.h"
#include "../../general_utilities/constants/constants.h"
#include "../../general_utilities/utilities/utilities.h"
//
#include "../../../classes/DSCuts/DSCuts.h"
#include "../../../classes/hPlots/hPlot1D.cpp"

namespace fitter_functions {
using namespace analysis_math;

// FitFunction function --------------------------------------------------------------------------------------------------------------------------------------------------

// NOTE:
// TF1 in ROOT expects a function with exactly the signature:
// Double_t func(Double_t* x, Double_t* par);
// And since NumOfParameters is meant to be a constant for each fit function, we define it externally and set it before calling TF1 when deeded.
static int NumOfParameters = 3;  // Define it externally

Double_t FitFunction(Double_t *v, Double_t *par);

// BetaFit function -----------------------------------------------------------------------------------------------------------------------------------------------------

void BetaFit(const std::string &SampleName, DSCuts &Beta_cut, DSCuts &Momentum_cuts, const hPlot1D &BetaPlot, TList *Histogram_list, const double beamE);

// BetaFitApprax function -----------------------------------------------------------------------------------------------------------------------------------------------------

void BetaFitApprax(const std::string &SampleName, DSCuts &Beta_cut, DSCuts &Momentum_cuts, const hPlot1D &BetaPlot, TList *Histogram_list, const double beamE);

};  // namespace fitter_functions

#endif  // FIT_FUNCTIONS_H
