//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef CODE_SETUP_H
#define CODE_SETUP_H

#include <iostream>

#include "../analysis_samples/analysis_samples.h"
#include "../debugging/debugging.h"
#include "../path_definitions/path_definitions.h"
// #include "../truth_analysis_variables/truth_analysis_variables.h"

#ifndef ANALYSER_CODESETUP_H
#define ANALYSER_CODESETUP_H

#include <vector>

#define MomResDebugMacro false
#define FullAnalyser true
#define IndependentCanvasPDF false

#include "../../setup/analysis_samples/analysis_samples.h"
#include "../../setup/debugging/debugging.h"
#include "../../setup/path_definitions/path_definitions.h"
// #include "../../setup/truth_analysis_variables/truth_analysis_variables.h"
//
#include "../../general_utilities/basic_tools/basic_tools.h"
#include "../../general_utilities/constants/constants.h"
#include "../../plotters/PDF_canvas/PDF_canvas.h"
//
// #include "../framework/classes/Debugger/Debugger.h"
#include "../../../classes/Directories/Directories.h"
#include "../../../classes/ExperimentParameters/ExperimentParameters.h"
// #include "../../../functions/HistogramPlottingFunctions.h"

namespace code_setup {
using namespace path_definitions;
using namespace debugging;
using namespace analysis_samples;
// using namespace truth_analysis_variables;

extern const std::string Ver;

extern double BEnergyToNucleus;

extern int Target_pdg, Probe_pdg;
extern std::string Target_nucleus, Probe;
extern double BeamEnergy;
};  // namespace code_setup

#endif  // CODE_SETUP_H
