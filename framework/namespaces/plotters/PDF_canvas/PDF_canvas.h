#ifndef PDF_CANVAS_H
#define PDF_CANVAS_H

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

#include "../../settings/PDF_canvas_settings/PDF_canvas_settings.h"

// TODO: split into header and source files!

// ======================================================================================================================================================================
// PDF file canvas settings
// ======================================================================================================================================================================

namespace PDF_canvas {
using namespace PDF_canvas_settings;

// Canvas functions -----------------------------------------------------------------------------------------------------------------------------------------------------

void ConfigCanvasPDF();

void SetCanvasPDFind(int& CanvasPDFind);

void CopyPadContent(TPad* sourcePad, TPad* targetPad);

};  // namespace PDF_canvas

#endif  // PDF_CANVAS_H
