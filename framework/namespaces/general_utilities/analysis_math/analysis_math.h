//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef ANALYSIS_MATH_H
#define ANALYSIS_MATH_H

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "../basic_tools/basic_tools.h"
#include "../constants/constants.h"
//
#include "polynomial_solver.h"
//
#include "HipoChain.h"
#include "clas12reader.h"

// using namespace std;
using namespace clas12;

namespace analysis_math {
using namespace polynomial_solver;

// Mathematical constants -----------------------------------------------------------------------------------------------------------------------------------------------

const double pi = M_PI;

// RadToDeg function ----------------------------------------------------------------------------------------------------------------------------------------------------

double RadToDeg(const double &rad);

// DegToRad function ----------------------------------------------------------------------------------------------------------------------------------------------------

double DegToRad(const double &deg);

// CalcTheta_rad function -----------------------------------------------------------------------------------------------------------------------------------------------

double CalcTheta_rad(const double &x, const double &y, const double &z);

// CalcTheta_deg function -----------------------------------------------------------------------------------------------------------------------------------------------

double CalcTheta_deg(const double &x, const double &y, const double &z);

// CalcPhi_rad function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPhi_rad(const double &x, const double &y);

// CalcPhi_deg function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPhi_deg(const double &x, const double &y, const double &z);

// RadCalc function ------------------------------------------------------------------------------------------------------------------------------------------------------

inline double RadCalc(double x, double y, double z);

// GetPi0MomTh function -------------------------------------------------------------------------------------------------------------------------------------------------

double GetPi0MomTh(const double &ph_mom_th);

// CalcdTheta function --------------------------------------------------------------------------------------------------------------------------------------------------

double CalcdTheta(double dThetaTemp);

// CalcdPhi function ----------------------------------------------------------------------------------------------------------------------------------------------------

double CalcdPhi1(double dPhiTemp);

// GetPhi_e function ----------------------------------------------------------------------------------------------------------------------------------------------------

double GetPhi_e(TString OutPutFolder, double phi_N);

// GetBinFromAng function -----------------------------------------------------------------------------------------------------------------------------------------------

int GetBinFromAng(double Angle, double AngleBins, double AngleMin, double AngleMax, bool printOut = false, const std::string &AngleType = "");

// CalcdPhi function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------------

double CalcdPhi2(region_part_ptr proton1, region_part_ptr proton2);

// TLKinCutsCheck function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------

/* TLKinCutsCheck for a general vector of particles */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader> &c12, bool apply_kinematical_cuts, const std::vector<int> &FD_nucleon, const DSCuts &FD_nucleon_theta_cut,
                    const DSCuts &FD_nucleon_momentum_cut);

// TLKinCutsCheck function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------

/* TLKinCutsCheck for leading FD neutrons */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader> &c12, bool apply_kinematical_cuts, const int TL_IDed_neutrons_FD_mom_max, const DSCuts &FD_nucleon_theta_cut,
                    const DSCuts &FD_nucleon_momentum_cut);

};  // namespace analysis_math

#endif  // ANALYSIS_MATH_H
