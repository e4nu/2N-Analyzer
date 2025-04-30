//
// Created by Alon Sportes on 30/04/2025.
//

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
#include <TLatex.h>
#include <TLegend.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TObject.h>
#include <TPad.h>
#include <TPaletteAxis.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>  // for gSystem->mkdir
#include <TSystem.h>
#include <TTree.h>
#include <math.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

// Include libraries:
#include "../framework/namespaces/general_utilities/histogram_functions.h"

using namespace std;

// CompareHistogramsTester -------------------------------------------------------------------------------------------------------------------------------------------------------

void CompareHistogramsTester() {
    const char *filename = "/Users/alon/Downloads/v23/C12_simulation_SuSa_Q2_0_02_2070MeV_wFC_S03ACNC_V23/C12_simulation_SuSa_Q2_0_02_2070MeV_wFC_S03ACNC_V23_plots.root";

    TFile *file = new TFile(filename);

    //     TH1D* h1 = dynamic_cast<TH1D*>(FindHistogram(file, "hist_name", "TH1D"));
    // TH2D* h2 = dynamic_cast<TH2D*>(FindHistogram(file, "hist_name", "TH2D"));
    // THStack* hs = dynamic_cast<THStack*>(FindHistogram(file, "hist_name", "THStack"));

    THStack *sW_1e_cut = dynamic_cast<THStack *>(histogram_functions::FindHistogram(file, "W distribution (All Int., 1e cut)", "THStack"));
    THStack *sW_1p = dynamic_cast<THStack *>(histogram_functions::FindHistogram(file, "W distribution (All Int., 1p)", "THStack"));
    THStack *sW_1n = dynamic_cast<THStack *>(histogram_functions::FindHistogram(file, "W distribution (All Int., 1n)", "THStack"));
    THStack *sW_pFDpCD = dynamic_cast<THStack *>(histogram_functions::FindHistogram(file, "W distribution (All Int., pFDpCD)", "THStack"));
    THStack *sW_nFDpCD = dynamic_cast<THStack *>(histogram_functions::FindHistogram(file, "W distribution (All Int., nFDpCD)", "THStack"));

    histogram_functions::CompareHistograms({sW_1e_cut, sW_1p, sW_1n, sW_pFDpCD, sW_nFDpCD}, "./", "Histogram_Comparisons", "W_stack_ReacComp");
}
