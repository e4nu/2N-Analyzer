//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef ANALYSIS_PHYSICS_H
#define ANALYSIS_PHYSICS_H

#include <TLorentzVector.h>
#include <TVector3.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "../basic_tools.h"
#include "../constants.h"

namespace analysis_physics {

// CalcQ2 constants -----------------------------------------------------------------------------------------------------------------------------------------------------

double CalcQ2(const TLorentzVector &Beam_4v, const TLorentzVector &P_e_4v) {
    TLorentzVector Q_4v = Beam_4v - P_e_4v;
    double Q2 = fabs(Q_4v.Mag2());
    return Q2;
}

// CalcQ2 constants -----------------------------------------------------------------------------------------------------------------------------------------------------

double CalcxB(const TLorentzVector &Beam_4v, const TLorentzVector &P_e_4v, const double &omega, const double &Nuc_mass = constants::m_p) {
    double Q2 = CalcQ2(Beam_4v, P_e_4v);
    double xB = Q2 / (2 * Nuc_mass * omega);
    return xB;
}

// CalcW constants ------------------------------------------------------------------------------------------------------------------------------------------------------

double CalcW(const double &omega, const TVector3 &q_3v, const double &Nuc_mass = constants::m_p) {
    double W = sqrt((omega + Nuc_mass) * (omega + Nuc_mass) - q_3v.Mag2());
    return W;
}

};  // namespace analysis_physics

#endif  // ANALYSIS_PHYSICS_H
