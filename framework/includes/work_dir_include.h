//
// Created by Alon Sportes on 27/03/2025.
//

// #ifndef WORK_DIR_INCLUDE_H
// #define WORK_DIR_INCLUDE_H

#include <iostream>

#ifdef WORK_DIR
std::cout << "WORK_DIR is defined: " << WORK_DIR << std::endl;
#define RECO_ANALYSIS_RUN true
#else
std::cout << "WORK_DIR is not defined, using default: " << workDir << std::endl;
#define RECO_ANALYSIS_RUN false
#endif

void RECO_ANALYSIS_RUN_PrintOut() {
#if RECO_ANALYSIS_RUN
    std::cout << "\033[33m\nRunning in ifarm configuration (WORK_DIR = " << WORK_DIR << "; RECO_ANALYSIS_RUN = " << basic_tools::BoolToString(RECO_ANALYSIS_RUN) << ")\n";
#else
    std::cout << "\033[33m\nRunning in gpvm configuration (WORK_DIR = " << WORK_DIR << "; RECO_ANALYSIS_RUN = " << basic_tools::BoolToString(RECO_ANALYSIS_RUN) << ")\n\n";
#endif
}

// #endif //WORK_DIR_INCLUDE_H
