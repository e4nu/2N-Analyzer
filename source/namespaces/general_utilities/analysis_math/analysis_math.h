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

using namespace std;

namespace analysis_math {

// Mathematical constants -----------------------------------------------------------------------------------------------------------------------------------------------

const double pi = M_PI;

// RadToDeg function ----------------------------------------------------------------------------------------------------------------------------------------------------

double RadToDeg(const double &rad) { return rad * 180. / pi; }

// DegToRad function ----------------------------------------------------------------------------------------------------------------------------------------------------

double DegToRad(const double &deg) { return deg * pi / 180.; }

// CalcTheta_rad function -----------------------------------------------------------------------------------------------------------------------------------------------

double CalcTheta_rad(const double &x, const double &y, const double &z) { return acos(z / sqrt(x * x + y * y + z * z)); }

// CalcTheta_deg function -----------------------------------------------------------------------------------------------------------------------------------------------

double CalcTheta_deg(const double &x, const double &y, const double &z) { return RadToDeg(CalcTheta_rad(x, y, z)); }

// CalcPhi_rad function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPhi_rad(const double &x, const double &y) { return atan2(y, x); }

// CalcPhi_deg function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPhi_deg(const double &x, const double &y, const double &z) { return RadToDeg(CalcPhi_rad(x, y)); }

// RadCalc function ------------------------------------------------------------------------------------------------------------------------------------------------------

inline double RadCalc(double x, double y, double z) { return sqrt(x * x + y * y + z * z); }

// GetPi0MomTh function -------------------------------------------------------------------------------------------------------------------------------------------------

double GetPi0MomTh(const double &ph_mom_th) {
    if (std::abs(ph_mom_th) == 9999) { return -9999; }
    double pi0_mom_th = std::sqrt(4 * ph_mom_th * ph_mom_th - constants::m_pizero * constants::m_pizero);
    return pi0_mom_th;
}

// CalcdTheta function --------------------------------------------------------------------------------------------------------------------------------------------------

double CalcdTheta(double dThetaTemp) { return std::abs(dThetaTemp); }

// CalcdPhi function ----------------------------------------------------------------------------------------------------------------------------------------------------

double CalcdPhi1(double dPhiTemp) {
    double dPhi = (dPhiTemp > 180.0) ? (dPhiTemp - 360.0) : (dPhiTemp < -180.0) ? (dPhiTemp + 360.0) : dPhiTemp;

    // if (dPhiTemp > 180.0) {
    //     dPhi = dPhiTemp - 360.0;
    // } else if (dPhiTemp < -180.0) {
    //     dPhi = dPhiTemp + 360.0;
    // } else {
    //     dPhi = dPhiTemp;
    // }

    return dPhi;
}

// GetPhi_e function ----------------------------------------------------------------------------------------------------------------------------------------------------

double GetPhi_e(TString OutPutFolder, double phi_N) {
    double phi_e_offset = 0.;  // Electron phi_e offset due to the solenoid field

    std::string OutPutFolder0(OutPutFolder.Data());

    if (basic_tools::FindSubstring(OutPutFolder0, "2070MeV")) {
        phi_e_offset = 16.;
    } else if (basic_tools::FindSubstring(OutPutFolder0, "4029MeV")) {
        phi_e_offset = 7.;
    } else if (basic_tools::FindSubstring(OutPutFolder0, "5986MeV")) {
        phi_e_offset = 5.;
    }

    std::vector<double> possible_angles = {-120, -60, 0, 60, 120, 180};

    // Calculate the target angle (180 degrees away from phi_N)
    double target_angle = CalcdPhi1(phi_N + 180);

    // Find the closest possible angle
    double closest_angle = possible_angles[0];
    double min_diff = std::abs(CalcdPhi1(target_angle - closest_angle));

    for (double angle : possible_angles) {
        double diff = std::abs(CalcdPhi1(target_angle - angle));
        if (diff < min_diff) {
            min_diff = diff;
            closest_angle = angle;
        }
    }

    return closest_angle + phi_e_offset;
    // return closest_angle;
}

// GetBinFromAng function -----------------------------------------------------------------------------------------------------------------------------------------------

int GetBinFromAng(double Angle, double AngleBins, double AngleMin, double AngleMax, bool printOut = false, const std::string &AngleType = "") {
    int Bin = 0;
    //    int Bin = -1;

    double Delta = (AngleMax - AngleMin) / AngleBins, min, max;

    for (int i = 0; i < AngleBins; i++) {
        double min_i, max_i;

        min_i = AngleMin + i * Delta, max_i = min_i + Delta;
        min = min_i, max = max_i;

        if (Angle >= min_i && Angle <= max_i) {
            if (printOut) {
                cout << "\n\n\nAngleType = " << AngleType << "\n";
                cout << "Angle = " << Angle << "\n";
                cout << "max_i = " << max_i << "\n";
                cout << "min_i = " << min_i << "\n";
                cout << "i = " << i << "\n";
                cout << "Delta = " << Delta << "\n\n\n";
            }

            Bin = i;
            break;
        }
    }  // end of loop over electrons vector

    return Bin;
}

}  // namespace analysis_math

#endif  // ANALYSIS_MATH_H
