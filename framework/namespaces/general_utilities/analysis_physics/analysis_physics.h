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

TVector3 CalcPmiss1N3v(const TVector3 &P_nucL_3v, const TVector3 &q_3v) {
    TVector3 P_miss_1N_3v = P_nucL_3v - q_3v;
    return P_miss_1N_3v;
}

// CalcEmiss1N function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcEmiss1N(const double &omega, const double &E_nucL, const double &mass_nucL = constants::m_p) {
    double T_L = E_nucL - mass_nucL;  // Relativistic kinematic energy of the leading (FD) nucleon
    double E_miss_1N = omega - T_L;
    return E_miss_1N;
}

// CalcPmiss2N3v function -----------------------------------------------------------------------------------------------------------------------------------------------

TVector3 CalcPmiss2N3v(const TVector3 &P_nucL_3v, const TVector3 &P_nucR_3v, const TVector3 &q_3v) {
    TVector3 P_miss_2N_3v = P_nucL_3v + P_nucR_3v - q_3v;
    return P_miss_2N_3v;
}

// CalcEmiss2N function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcEmiss2N(const double &omega, const double &E_nucL, const double &E_nucR, const double &mass_nucL = constants::m_p) {
    double T_L = E_nucL - mass_nucL, T_pCD = E_nucR - mass_nucL;  // Relativistic kinematic energy of the leading (FD) and recoil (CD) nucleons
    double E_miss_2N = omega - T_L - T_pCD;
    return E_miss_2N;
}

};  // namespace analysis_physics

#endif  // ANALYSIS_PHYSICS_H
