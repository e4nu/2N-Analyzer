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

// #include <cstdlib>  // For getenv()
// #include <cstdlib>  // For atoi()
// #include <iostream>

#ifdef IFARM_RUN
    #if IFARM_RUN == 1
        #define RECO_ANALYSIS_RUN true
    #else
        #define RECO_ANALYSIS_RUN false
    #endif
#else
    #define RECO_ANALYSIS_RUN false
#endif
// // Define RECO_ANALYSIS_RUN as a runtime constant
// const bool RECO_ANALYSIS_RUN = (getenv("IFARM_RUN") && atoi(getenv("IFARM_RUN")) == 1);
// // #define RECO_ANALYSIS_RUN (getenv("IFARM_RUN") == 1)
// // #define RECO_ANALYSIS_RUN (getenv("IFARM_RUN") != NULL)

void RECO_ANALYSIS_RUN_PrintOut() {
#if RECO_ANALYSIS_RUN
    std::cout << "\033[33m\nRunning in ifarm configuration (IFARM_RUN = " << getenv("IFARM_RUN") << "; RECO_ANALYSIS_RUN = " << RECO_ANALYSIS_RUN << ")\n";
#else
    std::cout << "\033[33m\nRunning in gpvm configuration (IFARM_RUN = " << getenv("IFARM_RUN") << "; RECO_ANALYSIS_RUN = " << RECO_ANALYSIS_RUN << ")\n\n";
#endif
}

// #endif //WORK_DIR_INCLUDE_H
