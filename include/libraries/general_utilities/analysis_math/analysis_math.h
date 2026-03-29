//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef ANALYSIS_MATH_H
#define ANALYSIS_MATH_H

#include <TString.h>

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "../basic_tools.h"
#include "../constants.h"
#include "../variable_correctors/variable_correctors.h"
#include "CLAS12/clas12_include.h"
#include "poly_solver.h"

namespace analysis_math {
using namespace poly_solver;
using namespace variable_correctors;

constexpr double pi = M_PI;

// RadToDeg function -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* RadToDeg function */
double RadToDeg(const double& rad);
#pragma endregion

// DegToRad function -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DegToRad function */
double DegToRad(const double& deg);
#pragma endregion

// CalcTheta_rad function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcTheta_rad function */
double CalcTheta_rad(const double& x, const double& y, const double& z);
#pragma endregion

// CalcTheta_deg function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcTheta_deg function */
double CalcTheta_deg(const double& x, const double& y, const double& z);
#pragma endregion

// CalcPhi_rad function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPhi_rad function */
double CalcPhi_rad(const double& x, const double& y);
#pragma endregion

// CalcPhi_deg function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPhi_deg function */
double CalcPhi_deg(const double& x, const double& y, const double& z);
#pragma endregion

// RadCalc function ------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* RadCalc function */
double RadCalc(double x, double y, double z);
#pragma endregion

// GetPi0MomTh function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetPi0MomTh function */
double GetPi0MomTh(const double& ph_mom_th);
#pragma endregion

// CalcdTheta function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcdTheta function */
double CalcdTheta(double dThetaTemp);
#pragma endregion

// CalcdPhi1 function ----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcdPhi1 function */
double CalcdPhi1(double dPhiTemp);
#pragma endregion

// GetPhi_e function -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetPhi_e function */
double GetPhi_e(TString OutPutFolder, double phi_N);
#pragma endregion

// GetBinFromAng function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetBinFromAng function */
int GetBinFromAng(double Angle, double AngleBins, double AngleMin, double AngleMax, bool printOut = false, const std::string& AngleType = "");
#pragma endregion

// CalcdPhi2 function ----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcdPhi2 function */
double CalcdPhi2(clas12::region_part_ptr proton1, clas12::region_part_ptr proton2);
#pragma endregion

// TLKinCutsCheck function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* TLKinCutsCheck function */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader>& c12, bool apply_kinematical_cuts, const std::vector<int>& FD_nucleon, const DSCuts& FD_nucleon_theta_cut,
                    const DSCuts& FD_nucleon_momentum_cut);
#pragma endregion

// TLKinCutsCheck function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* TLKinCutsCheck function */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader>& c12, bool apply_kinematical_cuts, const int TL_IDed_neutrons_FD_mom_max, const DSCuts& FD_nucleon_theta_cut,
                    const DSCuts& FD_nucleon_momentum_cut);
#pragma endregion

}  // namespace analysis_math

#endif  // ANALYSIS_MATH_H
