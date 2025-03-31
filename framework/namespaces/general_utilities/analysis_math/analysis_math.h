//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef ANALYSIS_MATH_H
#define ANALYSIS_MATH_H

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "../basic_tools.h"
#include "../constants.h"
//
// #include "analysis_math_reco_extentions.h"
#include "poly_solver.cpp"

// using namespace std;

namespace analysis_math {
using namespace poly_solver;

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

// CalcdPhi function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------------

double CalcdPhi2(region_part_ptr proton1, region_part_ptr proton2) {
    if (proton1->getRegion() == proton2->getRegion()) {
        std::cerr << "\n\nCalcdPhi2: protons are in the same region (" << proton1->getRegion() << " & " << proton2->getRegion() << ")! Exiting...\n";
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

    return CalcdPhi1(Phi_pFD_deg - Phi_pCD_deg);
}

// TLKinCutsCheck function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------

/* TLKinCutsCheck for a general vector of particles */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader> &c12, bool apply_kinematical_cuts, const vector<int> &FD_nucleon, const DSCuts &FD_nucleon_theta_cut,
                    const DSCuts &FD_nucleon_momentum_cut) {
    auto mcpbank = c12->mcparts();

    if (!apply_kinematical_cuts) {
        return true;
    } else {
        for (int i : FD_nucleon) {
            mcpbank->setEntry(i);

            int particlePDGtmp = mcpbank->getPid();

            double Particle_TL_Momentum = analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
            double Particle_TL_Theta = acos((mcpbank->getPz()) / analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

            bool Pass_FD_nucleon_theta_cuts = ((Particle_TL_Theta >= FD_nucleon_theta_cut.GetLowerCutConst()) && (Particle_TL_Theta <= FD_nucleon_theta_cut.GetUpperCutConst()));
            bool Pass_FD_nucleon_momentum_cuts =
                ((Particle_TL_Momentum >= FD_nucleon_momentum_cut.GetLowerCutConst()) && (Particle_TL_Momentum <= FD_nucleon_momentum_cut.GetUpperCutConst()));

            if (!(Pass_FD_nucleon_theta_cuts && Pass_FD_nucleon_momentum_cuts)) { return false; }
        }

        return true;
    }
}

// TLKinCutsCheck function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------

/* TLKinCutsCheck for leading FD neutrons */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader> &c12, bool apply_kinematical_cuts, const int TL_IDed_neutrons_FD_mom_max, const DSCuts &FD_nucleon_theta_cut,
                    const DSCuts &FD_nucleon_momentum_cut) {
    auto mcpbank = c12->mcparts();

    if (!apply_kinematical_cuts) {
        return true;
    } else {
        if (TL_IDed_neutrons_FD_mom_max == -1) {
            return false;
        } else {
            mcpbank->setEntry(TL_IDed_neutrons_FD_mom_max);

            double Particle_TL_Momentum = analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
            double Particle_TL_Theta = acos((mcpbank->getPz()) / analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

            bool Pass_FD_nucleon_momentum_cuts =
                ((Particle_TL_Momentum >= FD_nucleon_momentum_cut.GetLowerCutConst()) && (Particle_TL_Momentum <= FD_nucleon_momentum_cut.GetUpperCutConst()));
            bool Pass_FD_nucleon_theta_cuts = ((Particle_TL_Theta >= FD_nucleon_theta_cut.GetLowerCutConst()) && (Particle_TL_Theta <= FD_nucleon_theta_cut.GetUpperCutConst()));

            if (Pass_FD_nucleon_theta_cuts && Pass_FD_nucleon_momentum_cuts) {
                return true;
            } else {
                return false;
            }
        }
    }
}

};  // namespace analysis_math

#endif  // ANALYSIS_MATH_H
