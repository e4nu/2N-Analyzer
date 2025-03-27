#include <iostream>

#define STRINGIFY(x) #x
#define IFARM_RUN_STR(x) STRINGIFY(x)

#pragma message "IFARM_RUN is defined as " IFARM_RUN_STR(IFARM_RUN)

#ifdef IFARM_RUN
#define RECO_ANALYSIS_RUN true
#else
#define RECO_ANALYSIS_RUN false
#endif

void RECO_ANALYSIS_RUN_PrintOut() {
#if RECO_ANALYSIS_RUN
    std::cout << "\033[33m\nRunning in ifarm configuration (RECO_ANALYSIS_RUN = " << RECO_ANALYSIS_RUN << ")\n";
#else
    std::cout << "\033[33m\nRunning in gpvm configuration (RECO_ANALYSIS_RUN = " << RECO_ANALYSIS_RUN << ")\n\n";
#endif
}
