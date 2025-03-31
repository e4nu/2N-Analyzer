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
#include "../debugging/debugging.cpp"
#include "../path_definitions/path_definitions.cpp"
// #include "../truth_analysis_variables/truth_analysis_variables.h"

#include "../../general_utilities/basic_tools/basic_tools.cpp"
#include "../../general_utilities/constants/constants.cpp"
#include "../../plotters/PDF_canvas/PDF_canvas.cpp"
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
//     #include "../debugging/debugging.cpp"
//     #include "../path_definitions/path_definitions.cpp"
//     // #include "../truth_analysis_variables/truth_analysis_variables.h"

//     #include "../../general_utilities/basic_tools/basic_tools.cpp"
//     #include "../../general_utilities/constants/constants.cpp"
//     #include "../../plotters/PDF_canvas/PDF_canvas.cpp"
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

extern const std::string Ver;

extern std::string file_name;

extern double BEnergyToNucleus;

extern int Target_pdg, Probe_pdg;
extern std::string Target_nucleus, Probe;
extern double BeamEnergy;
};  // namespace code_setup

#endif  // CODE_SETUP_H
