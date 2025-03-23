//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef ANALYSIS_MATH_RECO_EXTENTIONS_H
#define ANALYSIS_MATH_RECO_EXTENTIONS_H

#include "analysis_math.h"
//
#include "HipoChain.h"
#include "clas12reader.h"

using namespace clas12;

namespace analysis_math_reco_extentions {
using namespace analysis_math;

// CalcdPhi function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------------

double CalcdPhi(region_part_ptr proton1, region_part_ptr proton2) {
    if (proton1->getRegion() == proton2->getRegion()) {
        std::cerr << "\n\nCalcdPhi: protons are in the same region (" << proton1->getRegion() << " & " << proton2->getRegion() << ")! Exiting...\n";
        exit(0);
    }

    double Phi_pFD_deg = 0.0, Phi_pCD_deg = 0.0;

    if (proton1->getRegion() == FD && proton2->getRegion() == CD) {
        Phi_pFD_deg = RadToDeg(proton1->getPhi());
        Phi_pCD_deg = RadToDeg(proton2->getPhi());
    } else if (proton1->getRegion() == CD && proton2->getRegion() == FD) {
        Phi_pFD_deg = RadToDeg(proton2->getPhi());
        Phi_pCD_deg = RadToDeg(proton1->getPhi());
    }

    return CalcdPhi(Phi_pFD_deg - Phi_pCD_deg);
}

}  // namespace analysis_math_reco_extentions

#endif  // ANALYSIS_MATH_RECO_EXTENTIONS_H
