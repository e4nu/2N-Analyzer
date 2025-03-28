#include "constants.h"

namespace constants {
// CLAS12 constants -----------------------------------------------------------------------------------------------------------------------------------------------------
/* FD & CD theta range */
DSCuts ThetaFD = DSCuts("Theta FD range", "FD", "", "", 1, 5., 40.);
DSCuts ThetaCD = DSCuts("Theta CD range", "CD", "", "", 1, 40., 135.);
};  // namespace constants
