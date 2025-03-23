#ifdef WORK_DIR
#define RECO_ANALYSIS_RUN true
#else
#define RECO_ANALYSIS_RUN false
#endif

#ifdef RECO_ANALYSIS_RUN
#include "utilities/utilities_reco_extentions.h"
#else
#include "utilities/utilities.h"
#endif