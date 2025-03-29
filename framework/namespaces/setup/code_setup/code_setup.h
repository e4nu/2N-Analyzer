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
