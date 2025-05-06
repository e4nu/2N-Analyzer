//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef DEBUGGING_SETTINGS_H
#define DEBUGGING_SETTINGS_H

#include <iostream>

namespace debugging_settings {

// General debugging ----------------------------------------------------------------------------------------------------------------------------------------------------

const bool DebuggerMode = false;  // true = Debugger mode (master switch), false = Normal mode
const bool OnlyPrintNamedTesterSteps = (DebuggerMode && true);

// Acceptance maps and weight maps debugging ----------------------------------------------------------------------------------------------------------------------------

const bool AMapsDebuggerMode = (DebuggerMode && true);

// Proton veto debugging ------------------------------------------------------------------------------------------------------------------------------------------------

const bool GoodProtonsMonitorPlots = true;

const bool PrintEvents = false;
const int Ne_in_event = 1, Nf_in_event = 2, nEvents2print = 10000;
}  // namespace debugging_settings

#endif  // DEBUGGING_SETTINGS_H
