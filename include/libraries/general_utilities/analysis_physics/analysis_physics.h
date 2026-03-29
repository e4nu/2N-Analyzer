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

// Include libraries:
#include "../basic_tools.h"
#include "../constants.h"

/**
 * @namespace analysis_physics
 * @brief A namespace for physics calculations used in the analysis, including functions for calculating kinematic quantities such as Q2, xB, W, missing momentum, and missing energy for 1N
 * and 2N processes.
 * @details This namespace contains various functions for calculating physics quantities that are relevant for the analysis of electron scattering events. The functions include calculations
 * for Q2, xB, W, missing momentum for 1N and 2N processes, and missing energy for 1N and 2N processes. The functions take as input the relevant kinematic variables, such as the beam and
 * scattered electron four-vectors, the energy transfer, the three-momentum transfer, and the energies of the leading and recoil nucleons. The functions return the calculated physics
 * quantities, which can be used in the main code for performing various analyses, such as applying cuts based on these quantities or filling histograms with these quantities.
 * @note The functions in this namespace are designed to be used in the main code for performing various analyses based on the calculated physics quantities. It is important to ensure that
 * the input variables are in the correct units and that the calculations are performed in the correct coordinate system. For example, the beam and scattered electron four-vectors should be
 * in the same units (e.g., GeV) and the angles should be calculated in the correct coordinate system (e.g., lab frame). Additionally, the functions assume that the relevant kinematic
 * variables are defined appropriately for the processes being analyzed (e.g., the energy transfer should be defined as the difference between the beam and scattered electron energies).
 */
namespace analysis_physics {

// ======================================================================================================================================================================
// Calculate physics quantities
// ======================================================================================================================================================================

// CalcQ2 function -------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcQ2 function */
inline double CalcQ2(const TLorentzVector& Beam_4v, const TLorentzVector& P_e_4v) {
    TLorentzVector q_4v = Beam_4v - P_e_4v;
    double Q2 = -q_4v.M2();
    return Q2;
}
#pragma endregion

// CalcxB function -------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcxB function */
inline double CalcxB(const TLorentzVector& Beam_4v, const TLorentzVector& P_e_4v, const double& omega, const double& Nuc_mass = constants::m_p) {
    double Q2 = CalcQ2(Beam_4v, P_e_4v);
    double xB = Q2 / (2 * Nuc_mass * omega);
    return xB;
}
#pragma endregion

// CalcW constants ------------------------------------------------------------------------------------------------------------------------------------------------------

// CalcW function --------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcW function */
inline double CalcW(const double& omega, const TVector3& q_3v, const double& Nuc_mass = constants::m_p) {
    double W = sqrt((omega + Nuc_mass) * (omega + Nuc_mass) - q_3v.Mag2());
    return W;
}
#pragma endregion

// CalcPmiss1N3v function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPmiss1N3v function */
inline TVector3 CalcPmiss1N3v(const TVector3& P_nucL_3v, const TVector3& q_3v) {
    TVector3 P_miss_1N_3v = P_nucL_3v - q_3v;
    return P_miss_1N_3v;
}
#pragma endregion

// CalcEmiss1N function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcEmiss1N function */
inline double CalcEmiss1N(const double& omega, const double& E_nucL, const double& mass_nucL = constants::m_p) {
    double T_L = E_nucL - mass_nucL;  // Relativistic kinematic energy of the leading (FD) nucleon
    double E_miss_1N = omega - T_L;
    return E_miss_1N;
}
#pragma endregion

// CalcPmiss2N3v function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPmiss2N3v function */
inline TVector3 CalcPmiss2N3v(const TVector3& P_nucL_3v, const TVector3& P_nucR_3v, const TVector3& q_3v) {
    TVector3 P_miss_2N_3v = P_nucL_3v + P_nucR_3v - q_3v;
    return P_miss_2N_3v;
}
#pragma endregion

// CalcEmiss2N function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcEmiss2N function */
inline double CalcEmiss2N(const double& omega, const double& E_nucL, const double& E_nucR, const double& mass_nucL = constants::m_p) {
    double T_L = E_nucL - mass_nucL, T_pCD = E_nucR - mass_nucL;  // Relativistic kinematic energy of the leading (FD) and recoil (CD) nucleons
    double E_miss_2N = omega - T_L - T_pCD;
    return E_miss_2N;
}
#pragma endregion

};  // namespace analysis_physics

#endif  // ANALYSIS_PHYSICS_H
