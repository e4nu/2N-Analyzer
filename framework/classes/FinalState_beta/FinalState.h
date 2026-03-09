//
// Created by Alon Sportes on 30/04/2025.
//

#ifndef FINALSTATE_H
#define FINALSTATE_H

#include <cmath>
#include <iostream>
#include <vector>

#include "AMapsManager.h"
#include "CutManager.h"
#include "CutSettings.h"
#include "FinalStateHistograms.h"
#include "TH1D.h"
#include "TH2D.h"
#include "analysis_physics.h"
#include "clas12reader.h"
#include "constants.h"
#include "debugging.h"
#include "nucleonResolution.h"
#include "region_particle.h"

class FinalState : public FinalStateHistograms {
   public:
    virtual ~FinalState() = default;
    enum Type { TwoNuc_pFDpCD, TwoNuc_nFDpCD, TwoNuc_2p, OneNuc_1p, OneNuc_1n };

    Type type;

    clas12::region_part_ptr e, NucFD, pCD;
    clas12::region_part_ptr NucL, NucR;

    TVector3 P_e_3v, q_3v, P_pFD_3v, P_pCD_3v;
    TVector3 P_miss_3v, P_tot_3v, P_rel_3v, P_max_3v, P_pL_3v, P_pR_3v;
    TVector3 P_pL_minus_q_v3, P_tot_minus_q_v3;
    TVector3 P_T_e_3v, P_T_L_3v, P_T_tot_3v, dP_T_L_3v, dP_T_tot_3v;
    TLorentzVector P_tot_mu_4v, P_rel_mu_4v;

    double Q2, xB, omega, W;
    double E_pFD, E_pCD, E_pL, E_pR, E_e;
    double E_miss_1N, E_miss_2N;
    double Theta_p_e_p_tot, Theta_q_p_tot, Theta_P_pL_minus_q_pR;
    double Theta_q_p_L, Theta_q_p_R, Theta_q_pFD, Theta_q_pCD;
    double Theta_e, Phi_e, Theta_pFD, Phi_pFD, Theta_pCD, Phi_pCD;
    double Theta_tot, Phi_tot, Theta_rel, Phi_rel;
    double dAlpha_T_L, dAlpha_T_tot, dPhi_T_L, dPhi_T_tot;
    double dPhi_hit, Theta_pFD_pCD;
    double EoP_e, Vx_e, Vy_e, Vz_e;
    double Ecal;
    double Weight;

    // Analysis cuts flags
    bool FD_Theta_Cut, FD_Momentum_Cut, e_withinFC, pFD_withinFC;
    bool Pass_Kin_Cuts, P_miss_in_QE_range, P_miss_in_MECandSRC_range, P_miss_in_Pass_ReacMon_cuts;
    bool E_miss_in_QE_range, E_miss_in_MECandSRC_range, E_miss_in_Pass_ReacMon_cuts;
    bool xB_in_QE_range, theta_q_pCD_in_lower_FSI_range, Pass_ReacMon_cuts;

    FinalState(const clas12::region_part_ptr& electron, const std::vector<clas12::region_part_ptr>& protons, const std::vector<int>& Electron_ind, const std::vector<int>& Protons_ind,
               const std::vector<int>& Kplus, const std::vector<int>& Kminus, const std::vector<int>& Piplus_ind, const std::vector<int>& Piminus_ind,
               const std::vector<clas12::region_part_ptr>& deuterons, const TLorentzVector& beam_4v, const double& Pvx, const double& Pvy, const double& Pvz, const CutSettings& CutSettings,
               const parameters_struct& parameters, const nucleonResolution& nRes, Type type);

    void SetAnalysisCuts(const CutSettings& CutSettings, const CutManager& CutManager, const AMapsManager& aMaps_master, const AMapsSettings& AMapsSettings, const ESSettings& ESSettings);

   private:
    double Phi_L() const;
    double Phi_R() const;

    std::pair<clas12::region_part_ptr, clas12::region_part_ptr> getProtonPair(const std::vector<clas12::region_part_ptr>& protons, const std::vector<int>& Protons_ind, Type type) const;

    void runSafetyChecks(const std::vector<int>& Electron_ind, const clas12::region_part_ptr& electron, const clas12::region_part_ptr& p1, const clas12::region_part_ptr& p2,
                         const std::vector<int>& Kplus, const std::vector<int>& Kminus, const std::vector<int>& Piplus_ind, const std::vector<int>& Piminus_ind,
                         const std::vector<clas12::region_part_ptr>& deuterons, Type type) const;
};

#endif  // FINALSTATE_H
