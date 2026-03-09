//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef CODE_SETUP_H
#define CODE_SETUP_H

#include <iostream>
#include <string_view>

#include "analysis_samples/analysis_samples.h"
#include "debugging.h"
#include "path_definitions.h"
#include "zipping.h"

/**
 * @namespace code_setup
 * @brief A namespace for setting up the code, including paths, debugging, analysis samples, and zipping functions.
 * @details This namespace contains various components for setting up the code, such as path definitions for input and output files, debugging settings and a Debugger instance for printing
 * out specific steps in the code, definitions of analysis samples for different runs and configurations, and functions for zipping the output of the code runs. The components in this
 * namespace are used in the main code to ensure consistency in file management, to facilitate debugging, to define the analysis samples being analyzed, and to manage the output of the code
 * runs through zipping functions. The code_setup namespace serves as a centralized location for these components, allowing for better organization and maintainability of the code.
 * @note It is important to use the components in this namespace consistently throughout the code to ensure that the paths, debugging settings, analysis samples, and zipping functions are
 * used correctly and effectively. For example, the path definitions should be used for reading input files and saving output files, the debugging settings and CodeDebugger instance should
 * be used for printing out specific steps in the code during debugging, the analysis samples should be defined and used for controlling the analysis based on the characteristics of the
 * samples being analyzed, and the zipping functions should be used for managing the output of the code runs. By using these components consistently, you can ensure that the code is
 * organized, maintainable, and effective in achieving its goals.
 */
namespace code_setup {
using namespace path_definitions;
using namespace debugging;
using namespace analysis_samples;
using namespace zipping;
};  // namespace code_setup

#endif  // CODE_SETUP_H
