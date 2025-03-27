//
// Created by Alon Sportes on 27/03/2025.
//

// #ifndef WORK_DIR_INCLUDE_H
// #define WORK_DIR_INCLUDE_H

#ifdef WORK_DIR
#define RECO_ANALYSIS_RUN true
#else
#define RECO_ANALYSIS_RUN false
#endif

#include <iostream>

void RECO_ANALYSIS_RUN_PrintOut() {
#if RECO_ANALYSIS_RUN
    std::cout << "\033[33m\nRunning in ifarm configuration (WORK_DIR = " << WORK_DIR << "; RECO_ANALYSIS_RUN = " << basic_tools::BoolToString(RECO_ANALYSIS_RUN) << ")\n";
#else
    std::cout << "\033[33m\nRunning in gpvm configuration (WORK_DIR = " << WORK_DIR << "; RECO_ANALYSIS_RUN = " << basic_tools::BoolToString(RECO_ANALYSIS_RUN) << ")\n\n";
#endif
}

// #endif //WORK_DIR_INCLUDE_H
