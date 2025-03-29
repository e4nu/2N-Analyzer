//
// Created by alons on 27/03/2025.
//

#ifndef HISTOGRAM_FUNCTIONS_H
#define HISTOGRAM_FUNCTIONS_H

#include <TH1D.h>

#include <iostream>

namespace histogram_functions {
// FillByReaction function ----------------------------------------------------------------------------------------------------------------------------------------------

extern void FillByReaction(TH1D *H1D_All_Int, TH1D *H1D_QEL, TH1D *H1D_MEC, TH1D *H1D_RES, TH1D *H1D_DIS, bool qel, bool mec, bool res, bool dis, double Variable, double Weight);
};  // namespace histogram_functions

#endif  // HISTOGRAM_FUNCTIONS_H
