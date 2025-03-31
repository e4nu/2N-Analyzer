//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <iostream>

#include "../../../classes/Debugger/Debugger.cpp"
#include "../../settings/debugging_settings/debugging_settings.cpp"
// #if CMakeBuidIncludes
//     #include "../../../classes/Debugger/Debugger.h"
//     #include "../../settings/debugging_settings/debugging_settings.h"
// #else
//     #include "../../../classes/Debugger/Debugger.cpp"
//     #include "../../settings/debugging_settings/debugging_settings.cpp"
// #endif

namespace debugging {
using namespace debugging_settings;

// CodeDebugger declaration ---------------------------------------------------------------------------------------------------------------------------------------------

extern Debugger CodeDebugger;

}  // namespace debugging

#endif  // DEBUGGING_H
