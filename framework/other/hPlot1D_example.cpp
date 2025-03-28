//
// Created by alons on 06/04/2023.
//

#include <TApplication.h>
#include <TBenchmark.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <vector>

#include "HipoChain.h"
#include "clas12reader.h"
#include "framework/classes/hPlots/hPlot1D.h"
#include "setup/codeSetup.h"

using namespace std;
using namespace clas12;

void hPlot1D_example() {
    TCanvas *c1 = new TCanvas("canvas", "canvas", 1650, 1150);
    c1->SetGrid();
    c1->SetBottomMargin(0.14);
    c1->SetLeftMargin(0.14);
    c1->SetRightMargin(0.12);

    c1->cd();

    hPlot1D testHist = hPlot1D("hst", "ht", "xat", 0, 1.25);
    testHist.hFill(1);
    testHist.SimpleDraw();
    testHist.SimpleSave(c1);
}