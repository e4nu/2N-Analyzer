//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef CODE_SETUP_H
#define CODE_SETUP_H

#define MomResDebugMacro false
#define FullAnalyser true
#define IndependentCanvasPDF false

#include <iostream>
#include <vector>

#include "../analysis_samples/analysis_samples.h"
#include "../debugging/debugging.h"
#include "../path_definitions/path_definitions.h"
// #include "../truth_analysis_variables/truth_analysis_variables.h"

#include "../../general_utilities/basic_tools/basic_tools.h"
#include "../../general_utilities/constants/constants.h"
#include "../../plotters/PDF_canvas/PDF_canvas.h"
//
#include "../../../classes/Directories/Directories.cpp"
#include "../../../classes/ExperimentParameters/ExperimentParameters.cpp"
#include "../../../functions/HistogramPlottingFunctions.cpp"
// #if CMakeBuidIncludes
//     #include "../analysis_samples/analysis_samples.h"
//     #include "../debugging/debugging.h"
//     #include "../path_definitions/path_definitions.h"
//     // #include "../truth_analysis_variables/truth_analysis_variables.h"

//     #include "../../general_utilities/basic_tools/basic_tools.h"
//     #include "../../general_utilities/constants/constants.h"
//     #include "../../plotters/PDF_canvas/PDF_canvas.h"
//     //
//     #include "../../../classes/Directories/Directories.h"
//     #include "../../../classes/ExperimentParameters/ExperimentParameters.h"
//     #include "../../../functions/HistogramPlottingFunctions.h"
// #else
//     #include "../analysis_samples/analysis_samples.h"
//     #include "../debugging/debugging.h"
//     #include "../path_definitions/path_definitions.h"
//     // #include "../truth_analysis_variables/truth_analysis_variables.h"

//     #include "../../general_utilities/basic_tools/basic_tools.cpp"
//     #include "../../general_utilities/constants/constants.h"
//     #include "../../plotters/PDF_canvas/PDF_canvas.h"
//     //
//     #include "../../../classes/Directories/Directories.cpp"
//     #include "../../../classes/ExperimentParameters/ExperimentParameters.cpp"
//     #include "../../../functions/HistogramPlottingFunctions.cpp"
// #endif

namespace code_setup {
using namespace path_definitions;
using namespace debugging;
using namespace analysis_samples;
using namespace PDF_canvas;
// using namespace truth_analysis_variables;

const std::string Ver = "DetSim testings";

std::string file_name = "general_file";

double BEnergyToNucleus = 9999;  // TODO: check if I really need these variables

int Target_pdg = 9999, Probe_pdg = 11;           // TODO: check if I really need these variables
std::string Target_nucleus = "A", Probe = "e-";  // TODO: check if I really need these variables
double BeamEnergy = 0.;                          // TODO: check if I really need this variable
};  // namespace code_setup

#endif  // CODE_SETUP_H
