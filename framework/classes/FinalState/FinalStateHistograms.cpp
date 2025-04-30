//
// Created by Alon Sportes on 30/04/2025.
//

#include "FinalStateHistograms.h"

using namespace constants;

FinalState::FinalState(const clas12::region_part_ptr& electron, const std::vector<clas12::region_part_ptr>& protons, const std::vector<int>& Electron_ind,
                       const std::vector<int>& Protons_ind, const std::vector<int>& Kplus, const std::vector<int>& Kminus, const std::vector<int>& Piplus_ind,
                       const std::vector<int>& Piminus_ind, const std::vector<clas12::region_part_ptr>& deuterons, const TLorentzVector& beam_4v, const double& Pvx, const double& Pvy,
                       const double& Pvz, const CutSettings& CutSettings, const parameters_struct& parameters, const nucleonResolution& nRes, Type type) {
    this->type = type;
    auto [p1, p2] = getProtonPair(protons, Protons_ind, type);
    runSafetyChecks(Electron_ind, electron, p1, p2, Kplus, Kminus, Piplus_ind, Piminus_ind, deuterons, type);

    e = electron;

    if (p1->getRegion() == FD) {
        NucFD = p1;
        pCD = p2;
    } else if (p1->getRegion() == CD) {
        pCD = p1;
        NucFD = p2;
    } else {
        std::cout << "\033[33m\n\npFDpCD: Invalid proton regions!\n\n", exit(1);
    }

    double P_pFD_BKC = NucFD->getP();
    P_e_3v.SetMagThetaPhi(e->getP(), e->getTheta(), e->getPhi());
    q_3v.SetXYZ(Pvx - P_e_3v.Px(), Pvy - P_e_3v.Py(), Pvz - P_e_3v.Pz());
    P_T_e_3v.SetXYZ(P_e_3v.Px(), P_e_3v.Py(), 0);

    P_pFD_3v.SetMagThetaPhi(nRes.PSmear(CutSettings.apply_nucleon_SmearAndCorr, P_pFD_BKC), NucFD->getTheta(), NucFD->getPhi());
    P_pCD_3v.SetMagThetaPhi(pCD->getP(), pCD->getTheta(), pCD->getPhi());

    E_e = sqrt(m_e * m_e + P_e_3v.Mag2());
    omega = parameters.beamE - E_e;
    W = sqrt((omega + m_p) * (omega + m_p) - q_3v.Mag2());
    E_pFD = sqrt(m_p * m_p + P_pFD_3v.Mag2());
    E_pCD = sqrt(m_p * m_p + P_pCD_3v.Mag2());

    P_pL_3v = (P_pFD_3v.Mag() >= P_pCD_3v.Mag()) ? P_pFD_3v : P_pCD_3v;
    P_pR_3v = (P_pFD_3v.Mag() < P_pCD_3v.Mag()) ? P_pFD_3v : P_pCD_3v;
    P_max_3v = (P_pL_3v.Mag() >= P_pR_3v.Mag()) ? P_pL_3v : P_pR_3v;

    NucL = (P_pL_3v == P_pFD_3v) ? NucFD : pCD;
    NucR = (P_pR_3v == P_pFD_3v) ? NucFD : pCD;

    E_pL = sqrt(m_p * m_p + P_pL_3v.Mag2());
    E_pR = sqrt(m_p * m_p + P_pR_3v.Mag2());

    P_tot_3v = P_pL_3v + P_pR_3v;
    P_rel_3v = (P_pL_3v - P_pR_3v) * 0.5;
    P_pL_minus_q_v3 = P_pL_3v - q_3v;
    P_tot_minus_q_v3 = P_tot_3v - q_3v;

    P_T_L_3v = TVector3(P_pL_3v.Px(), P_pL_3v.Py(), 0);
    P_T_tot_3v = TVector3(P_pFD_3v.Px() + P_pCD_3v.Px(), P_pFD_3v.Py() + P_pCD_3v.Py(), 0);
    dP_T_L_3v = TVector3(P_e_3v.Px() + P_T_L_3v.Px(), P_e_3v.Py() + P_T_L_3v.Py(), 0);
    dP_T_tot_3v = TVector3(P_e_3v.Px() + P_tot_3v.Px(), P_e_3v.Py() + P_tot_3v.Py(), 0);

    Theta_p_e_p_tot = acos(P_e_3v.Dot(P_tot_3v) / (P_e_3v.Mag() * P_tot_3v.Mag())) * 180.0 / pi;
    Theta_q_p_tot = acos(q_3v.Dot(P_tot_3v) / (q_3v.Mag() * P_tot_3v.Mag())) * 180.0 / pi;

    P_tot_mu_4v.SetPxPyPzE(P_pL_3v.Px() + P_pR_3v.Px(), P_pL_3v.Py() + P_pR_3v.Py(), P_pL_3v.Pz() + P_pR_3v.Pz(), E_pL + E_pR);
    P_rel_mu_4v.SetPxPyPzE((P_pL_3v.Px() - P_pR_3v.Px()) / 2, (P_pL_3v.Py() - P_pR_3v.Py()) / 2, (P_pL_3v.Pz() - P_pR_3v.Pz()) / 2, (E_pL - E_pR) / 2);

    Ecal = E_e + (E_pL - m_p) + (E_pR - m_p);

    dAlpha_T_L = acos(-P_e_3v.Dot(dP_T_L_3v) / (P_T_e_3v.Mag() * dP_T_L_3v.Mag())) * 180.0 / pi;
    dAlpha_T_tot = acos(-P_e_3v.Dot(dP_T_tot_3v) / (P_T_e_3v.Mag() * dP_T_tot_3v.Mag())) * 180.0 / pi;

    dPhi_T_L = acos(-P_T_e_3v.Dot(P_T_L_3v) / (P_T_e_3v.Mag() * P_T_L_3v.Mag())) * 180.0 / pi;
    dPhi_T_tot = acos(-P_T_e_3v.Dot(P_T_tot_3v) / (P_T_e_3v.Mag() * P_T_tot_3v.Mag())) * 180.0 / pi;

    EoP_e = (e->cal(clas12::PCAL)->getEnergy() + e->cal(clas12::ECIN)->getEnergy() + e->cal(clas12::ECOUT)->getEnergy()) / P_e_3v.Mag();
    Vx_e = e->par()->getVx();
    Vy_e = e->par()->getVy();
    Vz_e = e->par()->getVz();

    Weight = 1.0;
}

void FinalState::SetAnalysisCuts(const CutSettings& CutSettings, const CutManager& CutManager, const AMapsManager& aMaps_master, const AMapsSettings& AMapsSettings,
                                 const ESSettings& ESSettings) {
    FD_Theta_Cut = ((P_pFD_3v.Theta() * 180.0 / constants::pi) <= CutManager.FD_nucleon_theta_cut.GetUpperCut());

    FD_Momentum_Cut = ((P_pFD_3v.Mag() <= CutManager.FD_nucleon_momentum_cut.GetUpperCut()) && (P_pFD_3v.Mag() >= CutManager.FD_nucleon_momentum_cut.GetLowerCut()));

    e_withinFC = aMaps_master.IsInFDQuery((!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps || AMapsSettings.Generate_Nucleon_AMaps)), CutManager.ThetaFD,
                                          "Electron", P_e_3v.Mag(), P_e_3v.Theta() * 180.0 / constants::pi, P_e_3v.Phi() * 180.0 / constants::pi);

    pFD_withinFC = aMaps_master.IsInFDQuery((!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps || AMapsSettings.Generate_Nucleon_AMaps)), CutManager.ThetaFD,
                                            "Proton", NucFD->getP(), P_pFD_3v.Theta() * 180.0 / constants::pi, P_pFD_3v.Phi() * 180.0 / constants::pi, ESSettings.Calc_eff_overlapping_FC);

    Pass_Kin_Cuts = ((!CutSettings.apply_kinematical_cuts || (FD_Theta_Cut && FD_Momentum_Cut)) && (!CutSettings.apply_fiducial_cuts || (e_withinFC && pFD_withinFC)));

    P_miss_in_QE_range = (P_miss_3v.Mag() >= CutManager.P_miss_1N_QE_range.GetLowerCut() && P_miss_3v.Mag() <= CutManager.P_miss_1N_QE_range.GetUpperCut());

    P_miss_in_MECandSRC_range = (P_miss_3v.Mag() >= CutManager.P_miss_1N_MECandSRC_range.GetLowerCut() && P_miss_3v.Mag() <= CutManager.P_miss_1N_MECandSRC_range.GetUpperCut());

    P_miss_in_Pass_ReacMon_cuts = ((!CutSettings.apply_P_miss_in_QE_range_cuts && !CutSettings.apply_P_miss_in_MECandSRC_range_cuts) ||
                                   (CutSettings.apply_P_miss_in_QE_range_cuts && !CutSettings.apply_P_miss_in_MECandSRC_range_cuts && P_miss_in_QE_range) ||
                                   (!CutSettings.apply_P_miss_in_QE_range_cuts && CutSettings.apply_P_miss_in_MECandSRC_range_cuts && P_miss_in_MECandSRC_range) ||
                                   (CutSettings.apply_P_miss_in_QE_range_cuts && CutSettings.apply_P_miss_in_MECandSRC_range_cuts && (P_miss_in_QE_range || P_miss_in_MECandSRC_range)));

    E_miss_in_QE_range = (E_miss_1N >= CutManager.E_miss_1N_QE_range.GetLowerCut() && E_miss_1N <= CutManager.E_miss_1N_QE_range.GetUpperCut());

    E_miss_in_MECandSRC_range = (E_miss_1N >= CutManager.E_miss_1N_MECandSRC_range.GetLowerCut() && E_miss_1N <= CutManager.E_miss_1N_MECandSRC_range.GetUpperCut());

    E_miss_in_Pass_ReacMon_cuts = ((!CutSettings.apply_E_miss_in_QE_range_cuts && !CutSettings.apply_E_miss_in_MECandSRC_range_cuts) ||
                                   (CutSettings.apply_E_miss_in_QE_range_cuts && !CutSettings.apply_E_miss_in_MECandSRC_range_cuts && E_miss_in_QE_range) ||
                                   (!CutSettings.apply_E_miss_in_QE_range_cuts && CutSettings.apply_E_miss_in_MECandSRC_range_cuts && E_miss_in_MECandSRC_range) ||
                                   (CutSettings.apply_E_miss_in_QE_range_cuts && CutSettings.apply_E_miss_in_MECandSRC_range_cuts && (E_miss_in_QE_range || E_miss_in_MECandSRC_range)));

    xB_in_QE_range = (xB >= CutManager.xB_cut.GetLowerCut() && xB <= CutManager.xB_cut.GetUpperCut());

    theta_q_pCD_in_lower_FSI_range = (Theta_q_pCD >= CutManager.Theta_q_pCD_cut.GetLowerCut() && Theta_q_pCD <= CutManager.Theta_q_pCD_cut.GetUpperCut());

    Pass_ReacMon_cuts = (P_miss_in_Pass_ReacMon_cuts && E_miss_in_Pass_ReacMon_cuts && (!CutSettings.apply_xB_in_QE_range_cuts || xB_in_QE_range) &&
                         (!CutSettings.apply_theta_q_pCD_in_lower_FSI_range_cut || theta_q_pCD_in_lower_FSI_range));
}

double FinalState::Phi_L() const { return P_pL_3v.Phi() * 180.0 / pi; }
double FinalState::Phi_R() const { return P_pR_3v.Phi() * 180.0 / pi; }

std::pair<clas12::region_part_ptr, clas12::region_part_ptr> FinalState::getProtonPair(const std::vector<clas12::region_part_ptr>& protons, const std::vector<int>& Protons_ind,
                                                                                      Type type) const {
    if (type == TwoNuc_pFDpCD) {
        if (Protons_ind.size() != 2) { std::cout << "\033[33m\n\npFDpCD: Protons_ind.size() is different than 2! Aborting...\n\n", exit(1); }
        return {protons[Protons_ind.at(0)], protons[Protons_ind.at(1)]};
    }
    return {nullptr, nullptr};
}

void FinalState::runSafetyChecks(const std::vector<int>& Electron_ind, const clas12::region_part_ptr& electron, const clas12::region_part_ptr& p1, const clas12::region_part_ptr& p2,
                                 const std::vector<int>& Kplus, const std::vector<int>& Kminus, const std::vector<int>& Piplus_ind, const std::vector<int>& Piminus_ind,
                                 const std::vector<clas12::region_part_ptr>& deuterons, Type type) const {
    if (type == TwoNuc_pFDpCD) {
        if (electron->getRegion() != FD) { std::cout << "\033[33m\n\npFDpCD: Electron is not in the FD! Aborting...\n\n", exit(1); }
        if (p1->getRegion() == p2->getRegion()) { std::cout << "\033[33m\n\npFDpCD: Protons are in the same region! Aborting...\n\n", exit(1); }
    }
    debugging::CodeDebugger.SafetyCheck_basic_event_selection(__FILE__, __LINE__, "pFDpCD", Kplus, Kminus, Piplus_ind, Piminus_ind, Electron_ind, deuterons);
}

void FinalState::SetAnalysisCuts(const CutSettings& CutSettings, const CutManager& CutManager, const AMapsManager& aMaps_master, const AMapsSettings& AMapsSettings,
                                 const ESSettings& ESSettings) {
    if (this->NucFD && this->pCD && this->e && this->NucL && this->NucR && this->P_pFD_3v.Mag() > 0) {
        if (true) {  // Replace with type check when type is stored
            // (all logic from original SetAnalysisCuts here)
        }
    }
}
