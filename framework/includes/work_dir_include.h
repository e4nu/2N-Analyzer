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
    std::cout << "\033[33m\nRunning in ifarm configuration (RECO_ANALYSIS_RUN = " << RECO_ANALYSIS_RUN << ")\n";
#else
    std::cout << "\033[33m\nRunning in gpvm configuration (RECO_ANALYSIS_RUN = " << RECO_ANALYSIS_RUN << ")\n";
#endif
}

// #endif //WORK_DIR_INCLUDE_H
