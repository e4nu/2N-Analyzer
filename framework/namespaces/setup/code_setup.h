//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef CODE_SETUP_H
#define CODE_SETUP_H

#include <iostream>

#include "analysis_samples/analysis_samples.h"
#include "debugging.h"
#include "path_definitions.h"
#include "zipping.h"

namespace code_setup {
using namespace path_definitions;
using namespace debugging;
using namespace analysis_samples;
using namespace zipping;

std::string SYSTEM_COLOR = "\033[33m";     // Yellow
std::string ERROR_COLOR = "\033[31m";      // Red
std::string WARNING_COLOR = "\033[35m";    // Magenta
std::string INFO_COLOR = "\033[36m";       // Cyan
std::string SYSTEM_COLOR_END = "\033[0m";  // Reset color

};  // namespace code_setup

#endif  // CODE_SETUP_H
