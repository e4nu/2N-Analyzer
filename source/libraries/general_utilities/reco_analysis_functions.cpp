//
// Created by Alon Sportes on 16/03/2025.
//

#include "libraries/general_utilities/reco_analysis_functions.h"

#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TString.h>
#include <TStyle.h>
#include <TTree.h>
#include <TVector3.h>

#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

// Include setup:
#include "Settings/setup/debugging.h"
#include "libraries/general_utilities/basic_tools.h"

namespace am = analysis_math;
namespace db = debugging;

namespace reco_analysis_functions {

// GetFDNeutronP function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetFDNeutronP function */
double GetFDNeutronP(const clas12::region_part_ptr& Neutron, const bool& apply_nucleon_cuts) {
    double Momentum = Neutron->getP();

    if (apply_nucleon_cuts) {
        int ParticlePDG = Neutron->par()->getPid();

        if (ParticlePDG == 2112) {
            Momentum = Neutron->getP();
        } else if (ParticlePDG == 22) {
            double Path_ph = Neutron->getPath();
            double Time_ph = Neutron->getTime();
            double Beta_ph = Neutron->par()->getBeta();
            double Time_ph_from_Beta_ph = Path_ph / (constants::c * Beta_ph);
            double Velocity_ph = Path_ph / Time_ph_from_Beta_ph;
            double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
            Momentum = constants::m_n * Beta_ph * Gamma_ph;
            (void)Time_ph;
            (void)Velocity_ph;
        } else {
            basic_tools::PrintWarning(__func__, __FILE__, __LINE__,
                                      "unexpected neutral PID " + std::to_string(ParticlePDG) + " in GetFDNeutronP(); falling back to stored reconstructed momentum.");
        }
    } else {
        Momentum = Neutron->par()->getP();
    }

    return Momentum;
}
#pragma endregion

// GetECALHitFlags function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetECALHitFlags function */
std::tuple<bool, bool, bool> GetECALHitFlags(const clas12::region_part_ptr& ParticleFD) {
    bool ParticleInPCAL = (ParticleFD->cal(clas12::PCAL)->getDetector() == 7);
    bool ParticleInECIN = (ParticleFD->cal(clas12::ECIN)->getDetector() == 7);
    bool ParticleInECOUT = (ParticleFD->cal(clas12::ECOUT)->getDetector() == 7);

    return {ParticleInPCAL, ParticleInECIN, ParticleInECOUT};
}
#pragma endregion

// CheckForNeutralFDECALHits function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CheckForNeutralFDECALHits function */
void CheckForNeutralFDECALHits(bool& ParticleInPCAL, bool& ParticleInECIN, bool& ParticleInECOUT, short& NeutralFD_ECAL_detlayer, const clas12::region_part_ptr& NeutralFD) {
    std::tie(ParticleInPCAL, ParticleInECIN, ParticleInECOUT) = GetECALHitFlags(NeutralFD);

    NeutralFD_ECAL_detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;

    db::CodeDebugger.PrintMissingNeutralFDECALHitWarning(__func__, __FILE__, __LINE__, NeutralFD, ParticleInPCAL, ParticleInECIN, ParticleInECOUT);
}
#pragma endregion

// CheckForECALHits function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CheckForECALHits function */
void CheckForECALHits(bool& ParticleInPCAL, bool& ParticleInECIN, bool& ParticleInECOUT, short& NeutralFD_ECAL_detlayer, const std::vector<clas12::region_part_ptr>& allParticles,
                      const int& i) {
    CheckForNeutralFDECALHits(ParticleInPCAL, ParticleInECIN, ParticleInECOUT, NeutralFD_ECAL_detlayer, allParticles[i]);
}
#pragma endregion

// CheckForECALHits function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CheckForECALHits function */
std::tuple<bool, bool, bool, short> CheckForECALHits(const clas12::region_part_ptr& ParticleFD) {
    auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT] = GetECALHitFlags(ParticleFD);

    short ParticleFD_ECAL_detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;

    db::CodeDebugger.PrintMissingNeutralFDECALHitWarning(__func__, __FILE__, __LINE__, ParticleFD, ParticleInPCAL, ParticleInECIN, ParticleInECOUT);

    return {ParticleInPCAL, ParticleInECIN, ParticleInECOUT, ParticleFD_ECAL_detlayer};
}
#pragma endregion

// GetFTOFHitFlags function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetFTOFHitFlags function */
std::tuple<bool, bool, bool> GetFTOFHitFlags(const clas12::region_part_ptr& ParticleFD) {
    bool ParticleInFTOF1A = (ParticleFD->cal(clas12::FTOF1A)->getDetector() == 12);
    bool ParticleInFTOF1B = (ParticleFD->cal(clas12::FTOF1B)->getDetector() == 12);
    bool ParticleInFTOF2 = (ParticleFD->cal(clas12::FTOF2)->getDetector() == 12);

    db::CodeDebugger.PrintMissingFTOFHitWarning(__func__, __FILE__, __LINE__, ParticleFD, ParticleInFTOF1A, ParticleInFTOF1B, ParticleInFTOF2);

    return {ParticleInFTOF1A, ParticleInFTOF1B, ParticleInFTOF2};
}
#pragma endregion

// CalcPathnFD function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPathnFD function */
double CalcPathnFD(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const bool& Recalc_Path_nFD) {
    double reco_Path_nFD;

    auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = CheckForECALHits(NeutronFD);

    if (Recalc_Path_nFD) {
        TVector3 reco_nFD_vtx_3v;
        reco_nFD_vtx_3v.SetXYZ(electron->par()->getVx(), electron->par()->getVy(), electron->par()->getVz());

        TVector3 reco_nFD_hit_3v;
        reco_nFD_hit_3v.SetXYZ(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());

        TVector3 v_path_3v = reco_nFD_hit_3v - reco_nFD_vtx_3v;
        reco_Path_nFD = v_path_3v.Mag();
    } else {
        reco_Path_nFD = NeutronFD->getPath();
    }

    return reco_Path_nFD;
}
#pragma endregion

// CalcPathnFD3v function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPathnFD3v function */
TVector3 CalcPathnFD3v(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const bool& Recalc_Path_nFD) {
    TVector3 reco_Path_nFD;

    auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = CheckForECALHits(NeutronFD);

    if (Recalc_Path_nFD) {
        TVector3 reco_nFD_vtx_3v;
        reco_nFD_vtx_3v.SetXYZ(electron->par()->getVx(), electron->par()->getVy(), electron->par()->getVz());

        TVector3 reco_nFD_hit_3v;
        reco_nFD_hit_3v.SetXYZ(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());

        reco_Path_nFD = reco_nFD_hit_3v - reco_nFD_vtx_3v;
    } else {
        reco_Path_nFD.SetMagThetaPhi(NeutronFD->getPath(), NeutronFD->getTheta(), NeutronFD->getPhi());
    }

    return reco_Path_nFD;
}
#pragma endregion

// CalcToFnFD function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcToFnFD function */
double CalcToFnFD(const clas12::region_part_ptr& NeutronFD, const double& starttime, const bool& Recalc_ToF_nFD) {
    double reco_ToF_nFD;

    auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = CheckForECALHits(NeutronFD);

    if (Recalc_ToF_nFD) {
        reco_ToF_nFD = NeutronFD->cal(detlayer)->getTime() - starttime;
    } else {
        reco_ToF_nFD = NeutronFD->getTime();
    }

    return reco_ToF_nFD;
}
#pragma endregion

// CalcBetanFD function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcBetanFD function */
double CalcBetanFD(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const double& starttime, const bool& Recalc_Beta_nFD) {
    TVector3 reco_Path_nFD = CalcPathnFD3v(NeutronFD, electron, Recalc_Beta_nFD);
    double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, Recalc_Beta_nFD);
    double reco_Beta_nFD = Recalc_Beta_nFD ? reco_Path_nFD.Mag() / (reco_ToF_nFD * constants::c) : NeutronFD->par()->getBeta();

    return reco_Beta_nFD;
}
#pragma endregion

// CalcPnFD function -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPnFD function */
double CalcPnFD(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const double& starttime, const bool& Recalc_P_nFD) {
    double Momentum;

    if (Recalc_P_nFD && (NeutronFD->par()->getPid() == 22)) {
        double reco_Path_nFD = CalcPathnFD(NeutronFD, electron, Recalc_P_nFD);
        double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, Recalc_P_nFD);
        double reco_Beta_nFD = CalcBetanFD(NeutronFD, electron, starttime, Recalc_P_nFD);
        double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));
        Momentum = constants::m_n * reco_Beta_nFD * reco_Gamma_nFD;
        (void)reco_Path_nFD;
        (void)reco_ToF_nFD;
    } else {
        Momentum = NeutronFD->getP();
    }

    return Momentum;
}
#pragma endregion

// CalcPnFD3v function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcPnFD3v function */
TVector3 CalcPnFD3v(const clas12::region_part_ptr& NeutronFD, const clas12::region_part_ptr& electron, const double& starttime, const bool& Recalc_P_nFD) {
    TVector3 reco_P_nFD_3v;

    TVector3 reco_Path_nFD = CalcPathnFD3v(NeutronFD, electron, Recalc_P_nFD);
    double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, Recalc_P_nFD);
    double reco_Beta_nFD = CalcBetanFD(NeutronFD, electron, starttime, Recalc_P_nFD);
    double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));

    if (Recalc_P_nFD && (NeutronFD->par()->getPid() == 22)) {
        reco_P_nFD_3v.SetMagThetaPhi(constants::m_n * reco_Beta_nFD * reco_Gamma_nFD, reco_Path_nFD.Theta(), reco_Path_nFD.Phi());
    } else {
        reco_P_nFD_3v.SetMagThetaPhi(NeutronFD->getP(), reco_Path_nFD.Theta(), reco_Path_nFD.Phi());
    }

    (void)reco_ToF_nFD;
    return reco_P_nFD_3v;
}
#pragma endregion

// checkEcalDiagCuts function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* checkEcalDiagCuts function */
bool checkEcalDiagCuts(const clas12::region_part_ptr& electrons) {
    double ecal_diag_cut = 0.2;
    double mom = electrons->par()->getP();
    if (electrons->par()->getPid() == 11) {
        if ((electrons->cal(clas12::PCAL)->getEnergy() + electrons->cal(clas12::ECIN)->getEnergy()) / mom > ecal_diag_cut && mom > 4.5) {
            return true;
        } else if (mom <= 4.5) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}
#pragma endregion

// DCEdgeCuts function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DCEdgeCuts function */
bool DCEdgeCuts(clas12::region_part_ptr p) {
    std::vector<double> dc_edge_cut_el = {4.5, 3.5, 7.5};
    std::vector<double> dc_edge_cut_ptr = {2.5, 3, 10.5};

    if (p->par()->getCharge() != 0) {
        auto traj_index_1 = p->traj(DC, 6)->getIndex();
        auto traj_index_2 = p->traj(DC, 18)->getIndex();
        auto traj_index_3 = p->traj(DC, 36)->getIndex();
        auto traj_edge_1 = p->traj(DC, 6)->getFloat("edge", traj_index_1);
        auto traj_edge_2 = p->traj(DC, 18)->getFloat("edge", traj_index_2);
        auto traj_edge_3 = p->traj(DC, 36)->getFloat("edge", traj_index_3);

        if (p->par()->getCharge() < 0 && (dc_edge_cut_el.size() == 3 && traj_edge_1 > dc_edge_cut_el[0] && traj_edge_2 > dc_edge_cut_el[1] && traj_edge_3 > dc_edge_cut_el[2])) {
            return true;
        } else if (p->par()->getCharge() > 0 && (dc_edge_cut_ptr.size() == 3 && traj_edge_1 > dc_edge_cut_ptr[0] && traj_edge_2 > dc_edge_cut_ptr[1] && traj_edge_3 > dc_edge_cut_ptr[2])) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}
#pragma endregion

// fillDCdebug function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* fillDCdebug function */
void fillDCdebug(clas12::region_part_ptr p, TH2D** h, const double& weight) {
    h[1]->Fill(p->traj(DC, 6)->getX(), p->traj(DC, 6)->getY(), weight);
    h[2]->Fill(p->traj(DC, 18)->getX(), p->traj(DC, 18)->getY(), weight);
    h[3]->Fill(p->traj(DC, 36)->getX(), p->traj(DC, 36)->getY(), weight);
}
#pragma endregion

// CalcSolenoidPhiShift function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcSolenoidPhiShift function */
double CalcSolenoidPhiShift(const clas12::region_part_ptr& charged_particle, const double& B, const double& L) {
    constexpr double kTmmGeV = 0.299792458;
    const double q = static_cast<double>(charged_particle->par()->getCharge());
    const double P_charged = charged_particle->par()->getP();
    if (!(P_charged > 0.0) || !std::isfinite(P_charged)) {
#ifdef DEBUG_VETO
        std::cerr << "[CalcSolenoidPhiShift] non-physical p = " << P_charged << "\n";
#endif
        return 0.0;
    }

    double BL_int = B * L;
    double SolenoidPhiShift = (kTmmGeV * q * BL_int) / (2.0 * P_charged);
    return SolenoidPhiShift;
}
#pragma endregion

// GetShiftedUnitMomentum function ---------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetShiftedUnitMomentum function */
TVector3 GetShiftedUnitMomentum(const clas12::region_part_ptr& charged_particle, const TVector3& v_nhit_3v, const double& B, const double& L) {
    if (v_nhit_3v.Mag() == 0.0 || !std::isfinite(v_nhit_3v.Mag())) {
#ifdef DEBUG_VETO
        std::cerr << "[GetShiftedUnitMomentum] v_nhit_3v has zero/non-finite magnitude, returning (0,0,0)\n";
#endif
        return TVector3(0, 0, 0);
    }
    TVector3 ShiftedUnitMomentum_3v;
    double Sol_dPhi = CalcSolenoidPhiShift(charged_particle, B, L);
    ShiftedUnitMomentum_3v.SetMagThetaPhi(v_nhit_3v.Mag(), charged_particle->getTheta(), charged_particle->getPhi() + Sol_dPhi);
    return ShiftedUnitMomentum_3v;
}
#pragma endregion

// NeutronECAL_Cut_Veto function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutronECAL_Cut_Veto function */
bool NeutronECAL_Cut_Veto(const std::vector<clas12::region_part_ptr>& allParticles, const std::vector<clas12::region_part_ptr>& electrons, const double& beamE, const int& index,
                          const double& veto_cut) {
    TVector3 p_b_3v(0, 0, beamE);
    TVector3 p_e_3v;
    p_e_3v.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 p_q_3v = p_b_3v - p_e_3v;

    if (allParticles[index]->par()->getCharge() != 0) { return false; }

    TVector3 p_n_Angles_3v;
    p_n_Angles_3v.SetMagThetaPhi(1.0, allParticles[index]->getTheta(), allParticles[index]->getPhi());

    const bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);
    const bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);
    const bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);
    const auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT;

    TVector3 v_nhit_3v(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());

    const double beta = allParticles[index]->par()->getBeta();
    const double gamma = 1.0 / sqrt(1.0 - (beta * beta));
    const double theta_n = analysis_math::RadToDeg(p_n_Angles_3v.Theta());
    const double theta_q = analysis_math::RadToDeg(p_q_3v.Theta());
    const double theta_nq = analysis_math::RadToDeg(p_n_Angles_3v.Angle(p_q_3v));

    if (beta < 0) { return false; }
    if (theta_n < 1.0) { return false; }
    if (!(IC || OC)) { return false; }
    if (PC) { return false; }

    bool Veto = false;
    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; }
        TVector3 v_chit_3v;

        if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            v_chit_3v.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist_3v = v_nhit_3v - v_chit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            v_chit_3v.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist_3v = v_nhit_3v - v_chit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else {
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit_3v.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                             allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist_3v = v_nhit_3v - v_chit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        }
    }

    (void)gamma;
    (void)theta_q;
    (void)theta_nq;
    if (Veto) { return false; }
    return true;
}
#pragma endregion

// ChargedECALveto function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ChargedECALveto function */
bool ChargedECALveto(const std::vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr& electron, const clas12::region_part_ptr& NeutronFD, const double& veto_cut,
                     const double& starttime, const bool& Recalc_P_nFD, const bool& apply_PCAL_veto) {
    bool recalc_P_nFD = (starttime == 9999) ? Recalc_P_nFD : false;

    const double reco_Path_nFD = CalcPathnFD(NeutronFD, electron, recalc_P_nFD);
    const double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, recalc_P_nFD);
    const double reco_Beta_nFD = CalcBetanFD(NeutronFD, electron, starttime, recalc_P_nFD);
    const double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));
    const auto [PC, IC, OC, detlayer] = CheckForECALHits(NeutronFD);
    const TVector3 reco_nFD_hit_3v(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());

    TVector3 reco_P_nFD_3v;
    reco_P_nFD_3v = CalcPnFD3v(NeutronFD, electron, starttime, recalc_P_nFD);

    if (apply_PCAL_veto && (!(IC || OC) || PC)) { return false; }
    if (NeutronFD->par()->getCharge() != 0) { return false; }
    if (reco_Beta_nFD < 0.0) { return false; }
    if (am::RadToDeg(reco_P_nFD_3v.Theta()) < 1.0) { return false; }

    bool Veto = false;

    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; }

        TVector3 reco_ChargedFD_hit_3v;

        if ((detlayer == clas12::PCAL) && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
            reco_ChargedFD_hit_3v.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
            TVector3 v_dist_3v = reco_nFD_hit_3v - reco_ChargedFD_hit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            reco_ChargedFD_hit_3v.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist_3v = reco_nFD_hit_3v - reco_ChargedFD_hit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            reco_ChargedFD_hit_3v.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist_3v = reco_nFD_hit_3v - reco_ChargedFD_hit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        } else {
            const int trajlayer = (detlayer == clas12::PCAL) ? 1 : (detlayer == clas12::ECIN) ? 4 : 7;
            reco_ChargedFD_hit_3v.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                                         allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist_3v = reco_nFD_hit_3v - reco_ChargedFD_hit_3v;
            if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
        }
    }

    (void)reco_Path_nFD;
    (void)reco_ToF_nFD;
    (void)reco_Gamma_nFD;
    if (Veto) { return false; }
    return true;
}
#pragma endregion

// NeutralECALveto function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutralECALveto function */
bool NeutralECALveto(const std::vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr& electron, const clas12::region_part_ptr& NeutronFD, const double& veto_cut,
                     const double& starttime, const bool& Recalc_P_nFD, const bool& apply_PCAL_veto, const double B, const double L) {
    bool recalc_P_nFD = (starttime == 9999) ? Recalc_P_nFD : false;

    const double reco_Path_nFD = CalcPathnFD(NeutronFD, electron, recalc_P_nFD);
    const double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, recalc_P_nFD);
    const double reco_Beta_nFD = CalcBetanFD(NeutronFD, electron, starttime, recalc_P_nFD);
    const double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));
    const auto [PC, IC, OC, detlayer] = CheckForECALHits(NeutronFD);
    const TVector3 reco_nFD_hit_3v(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());

    TVector3 reco_P_nFD_3v;
    reco_P_nFD_3v = CalcPnFD3v(NeutronFD, electron, starttime, recalc_P_nFD);

    if (apply_PCAL_veto && (!(IC || OC) || PC)) { return false; }
    if (NeutronFD->par()->getCharge() != 0) { return false; }
    if (reco_Beta_nFD < 0.0) { return false; }
    if (am::RadToDeg(reco_P_nFD_3v.Theta()) < 1.0) { return false; }

    bool Veto = false;

    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; }
        TVector3 ShiftedUnitMomentum_3v = GetShiftedUnitMomentum(allParticles[j], reco_nFD_hit_3v, B, L);
        TVector3 v_dist_3v = reco_nFD_hit_3v - ShiftedUnitMomentum_3v;
        if (v_dist_3v.Mag() < veto_cut) { Veto = true; }
    }

    (void)reco_Path_nFD;
    (void)reco_ToF_nFD;
    (void)reco_Gamma_nFD;
    if (Veto) { return false; }
    return true;
}
#pragma endregion

}  // namespace reco_analysis_functions
