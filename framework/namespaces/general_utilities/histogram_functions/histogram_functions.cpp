//
// Created by alons on 27/03/2025.
//

#include "histogram_functions.h"

namespace histogram_functions {
// FillByReaction function ----------------------------------------------------------------------------------------------------------------------------------------------

void FillByReaction(TH1D *H1D_All_Int, TH1D *H1D_QEL, TH1D *H1D_MEC, TH1D *H1D_RES, TH1D *H1D_DIS, bool qel, bool mec, bool res, bool dis, double Variable, double Weight) {
    H1D_All_Int->Fill(Variable, Weight);

    if (qel) {
        H1D_QEL->Fill(Variable, Weight);
    } else if (mec) {
        H1D_MEC->Fill(Variable, Weight);
    } else if (res) {
        H1D_RES->Fill(Variable, Weight);
    } else if (dis) {
        H1D_DIS->Fill(Variable, Weight);
    }
}
};  // namespace histogram_functions
