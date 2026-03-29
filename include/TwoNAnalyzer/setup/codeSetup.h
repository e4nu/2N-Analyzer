#ifndef ANALYZER_CODESETUP_H
#define ANALYZER_CODESETUP_H

#include <vector>

// Include setup:
#include "../../io/analysis_samples.h"

// Include Settings:
#include "../../Settings/setup/debugging.h"
#include "../../Settings/setup/path_definitions.h"
#include "../../Settings/setup/zipping.h"

// Include libraries
#include "../../libraries/general_utilities/basic_tools.h"
#include "../../libraries/general_utilities/constants.h"

// Include AnalysisTools:
#include "../../AnalysisTools/Directories.h"
#include "../../AnalysisTools/ExperimentParameters.h"

// Include functions:
#include "../../functions/HistogramPlottingFunctions.h"

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
