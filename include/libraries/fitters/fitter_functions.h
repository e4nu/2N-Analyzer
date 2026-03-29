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
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>

#include <fstream>
#include <iostream>
#include <string>

#include "../../AnalysisTools/DSCuts.h"
#include "../../AnalysisTools/hPlot1D.h"
#include "../../libraries/general_utilities/utilities.h"

namespace fitter_functions {

extern int NumOfParameters;

// FitFunction function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FitFunction function */
Double_t FitFunction(Double_t* v, Double_t* par);
#pragma endregion

// BetaFit function ------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* BetaFit function */
void BetaFit(const std::string& SampleName, DSCuts& Beta_cut, DSCuts& Momentum_cuts, const hPlot1D& BetaPlot, TList* Histogram_list, const double beamE);
#pragma endregion

// BetaFitApprax function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* BetaFitApprax function */
void BetaFitApprax(const std::string& SampleName, DSCuts& Beta_cut, DSCuts& Momentum_cuts, const hPlot1D& BetaPlot, TList* Histogram_list, const double beamE);
#pragma endregion

}  // namespace fitter_functions

#endif  // FIT_FUNCTIONS_H
