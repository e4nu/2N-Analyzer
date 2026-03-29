//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <cstdlib>
#include <iostream>

// Include libraries:
#include "../../libraries/general_utilities/environment.h"

// Include AnalysisTools:
#include "../../AnalysisTools/Debugger.h"

namespace env = environment;

/**
 * @namespace debugging
 * @brief A namespace for debugging the code, including a Debugger class instance and settings related to debugging.
 * @details This namespace contains an instance of the Debugger class called CodeDebugger, which can be used to print out specific steps in the code for debugging purposes. The namespace
 * also includes settings related to debugging, which are defined in the debugging_settings namespace. These settings can be used to control various aspects of debugging, such as whether to
 * enable Debugger mode, whether to print out only specific steps in the code, whether to enable acceptance maps and weight maps debugging, and whether to enable proton veto debugging. The
 * CodeDebugger instance can be used in the main code to print out specific steps in the code based on the settings defined in the debugging_settings namespace, allowing for flexible and
 * targeted debugging of the code.
 * @note The Debugger class should be defined in the classes/Debugger/Debugger.cpp file, and the settings related to debugging should be defined in the
 * namespaces/settings/debugging_settings.h file. It is important to use the CodeDebugger instance in conjunction with the settings defined in the debugging_settings namespace to ensure that
 * the debugging output is consistent with the desired level of debugging and the specific steps that should be printed out during debugging. Additionally, it is important to ensure that the
 * debugging output is not too verbose or overwhelming, and that it is focused on the specific aspects of the code that are being debugged.
 */
namespace debugging {
// ======================================================================================================================================================================
// Debugging settings
// ======================================================================================================================================================================

// General debugging ----------------------------------------------------------------------------------------------------------------------------------------------------

const bool DebuggerMode = false;  // true = Debugger mode (master switch), false = Normal mode
const bool OnlyPrintNamedTesterSteps = (DebuggerMode && true);

// Acceptance maps and weight maps debugging ----------------------------------------------------------------------------------------------------------------------------

const bool AMapsDebuggerMode = (DebuggerMode && true);

// Proton veto debugging ------------------------------------------------------------------------------------------------------------------------------------------------

const bool GoodProtonsMonitorPlots = true;

const bool PrintEvents = false;
const int Ne_in_event = 1, Nf_in_event = 2, nEvents2print = 10000;

// ======================================================================================================================================================================
// Debugger class
// ======================================================================================================================================================================

// CodeDebugger declaration ---------------------------------------------------------------------------------------------------------------------------------------------

inline Debugger CodeDebugger;

}  // namespace debugging

#endif  // DEBUGGING_H
