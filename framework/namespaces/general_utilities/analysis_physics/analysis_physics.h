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

// CalcQ2 function ------------------------------------------------------------------------------------------------------------------------------------------------------

double CalcQ2(const TLorentzVector &Beam_4v, const TLorentzVector &P_e_4v) {
    // TLorentzVector Q_4v = Beam_4v - P_e_4v;
    // double Q2 = fabs(Q_4v.Mag2());
    TLorentzVector q_4v = Beam_4v - P_e_4v;
    double Q2 = -q_4v.M2();
    return Q2;
}

// CalcxB function ------------------------------------------------------------------------------------------------------------------------------------------------------

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

// CalcPmiss1N3v function -----------------------------------------------------------------------------------------------------------------------------------------------

TVector3 CalcPmiss1N3v(const TVector3 &P_L_3v, const TVector3 &q_3v) {
    TVector3 P_miss_1N_3v = P_L_3v - q_3v;
    return P_miss_1N_3v;
}

// CalcEmiss1N function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcEmiss1N(const double &omega, const double &E_L, const double &mass_L = constants::m_p) {
    double E_miss_1N = omega - (E_L - mass_L);
    return E_miss_1N;
}

// CalcPmiss2N3v function -----------------------------------------------------------------------------------------------------------------------------------------------

TVector3 CalcPmiss2N3v(const TVector3 &P_L_3v, const TVector3 &P_pCD_3v, const TVector3 &q_3v) {
    TVector3 P_miss_2N_3v = P_L_3v + P_pCD_3v - q_3v;
    return P_miss_2N_3v;
}

// CalcEmiss2N function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcEmiss2N(const double &omega, const double &E_L, const double &E_pCD, const double &mass_L = constants::m_p) {
    double E_miss_2N = omega - (E_L - mass_L) - (E_pCD - constants::m_p);
    return E_miss_2N;
}

};  // namespace analysis_physics

#endif  // ANALYSIS_PHYSICS_H
