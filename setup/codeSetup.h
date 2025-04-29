#ifndef ANALYZER_CODESETUP_H
#define ANALYZER_CODESETUP_H

#include <vector>

#define MomResDebugMacro false
#define FullAnalyzer true
#define IndependentCanvasPDF false

// Include setup:
#include "../framework/namespaces/setup/analysis_samples/analysis_samples.h"
#include "../framework/namespaces/setup/debugging.h"
#include "../framework/namespaces/setup/path_definitions.h"
#include "../framework/namespaces/setup/zipping.h"

// Include libraries
#include "../framework/namespaces/general_utilities/basic_tools.h"
#include "../framework/namespaces/general_utilities/constants.h"
// #include "../framework/namespaces/plotters/PDF_canvas/PDF_canvas.h"

// Include classes:
#include "../framework/classes/Directories/Directories.cpp"
#include "../framework/classes/ExperimentParameters/ExperimentParameters.cpp"
#include "../framework/functions/HistogramPlottingFunctions.h"

// using namespace PDF_canvas;
using namespace path_definitions;
using namespace debugging;
using namespace analysis_samples;
using namespace zipping;

std::string Ver = "Next-Gen 2N analysis";

double BEnergyToNucleus;

int Target_pdg, Probe_pdg;
std::string Target_nucleus, Probe;
double BeamEnergy;

#endif  // ANALYZER_CODESETUP_H
