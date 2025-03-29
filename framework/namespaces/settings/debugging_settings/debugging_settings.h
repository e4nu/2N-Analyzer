//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef DEBUGGING_SETTINGS_H
#define DEBUGGING_SETTINGS_H

#include <iostream>

// TODO: split into header and source files!

namespace debugging_settings {
inline const bool DebuggerMode = true;

inline const bool GoodProtonsMonitorPlots = true;

inline const bool PrintEvents = false;
inline const int Ne_in_event = 1, Nf_in_event = 2, nEvents2print = 10000;
}  // namespace debugging_settings

#endif  // DEBUGGING_SETTINGS_H
