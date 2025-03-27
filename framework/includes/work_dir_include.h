//
// Created by Alon Sportes on 27/03/2025.
//

// #ifndef WORK_DIR_INCLUDE_H
// #define WORK_DIR_INCLUDE_H

// #ifdef WORK_DIR
// const char* WorkDir = WORK_DIR;  // Define a variable using WORK_DIR
// #define RECO_ANALYSIS_RUN true
// #else
// const char* WorkDir = "UNKNOWN";
// #define RECO_ANALYSIS_RUN false
// #endif

#define RECO_ANALYSIS_RUN (getenv("IFARM_RUN") && atoi(getenv("IFARM_RUN")) == 1)
// #define RECO_ANALYSIS_RUN (getenv("IFARM_RUN") == 1)
// #define RECO_ANALYSIS_RUN (getenv("IFARM_RUN") != NULL)

#include <iostream>

void RECO_ANALYSIS_RUN_PrintOut() {
#if RECO_ANALYSIS_RUN
    std::cout << "\033[33m\nRunning in ifarm configuration (WorkDir = " << WorkDir << "; RECO_ANALYSIS_RUN = " << RECO_ANALYSIS_RUN << ")\n";
#else
    std::cout << "\033[33m\nRunning in gpvm configuration (WorkDir = " << WorkDir << "; RECO_ANALYSIS_RUN = " << RECO_ANALYSIS_RUN << ")\n\n";
#endif
}

// #endif //WORK_DIR_INCLUDE_H
