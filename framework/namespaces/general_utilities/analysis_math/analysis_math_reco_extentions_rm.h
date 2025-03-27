//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef ANALYSIS_MATH_RECO_EXTENTIONS_H
#define ANALYSIS_MATH_RECO_EXTENTIONS_H

#include "analysis_math.h"
//
#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTree.h>
#include <math.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <typeinfo>
#include <vector>

#include "../../../classes/DSCuts/DSCuts.h"
// #include "poly_solver.h"
//
#include "HipoChain.h"
#include "clas12reader.h"

using namespace clas12;

namespace analysis_math_reco_extentions {
using namespace analysis_math;

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

};  // namespace analysis_math_reco_extentions

#endif  // ANALYSIS_MATH_RECO_EXTENTIONS_H
