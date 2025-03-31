//
// Created by alons on 22/01/2024.
//

#ifndef PARTICLEID_H
#define PARTICLEID_H
// #ifdef PARTICLEID_H

#include <TF1.h>
#include <math.h>

#include <iostream>
#include <map>
#include <vector>

#include "../../namespaces/general_utilities/analysis_math/analysis_math_include.h"
#include "../../namespaces/general_utilities/constants/constants.h"
#include "../../namespaces/general_utilities/reco_analysis_functions/reco_analysis_functions.h"
// #include "../../functions/NeutronECAL_Cut_Veto.h"
// #include "../../functions/PID_functions/GetFDNeutronP.h"
#include "../DSCuts/DSCuts.h"
#include "../hPlots/hPlot1D.cpp"
#include "../hPlots/hPlot2D.cpp"
//
#include "clas12reader.h"
#include "region_particle.h"

#if !defined(MOMENTUMRESOLUTION_H)
    #include "../MomentumResolution/MomentumResolution.cpp"
#endif

// using namespace std;
using namespace clas12;
using namespace constants;
// using namespace analysis_math;

class ParticleID {
   public:
    // TODO: move from here!
    //<editor-fold desc="Good protons monitoring histograms">
    //  Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only) ------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only)">
    hPlot2D hdTheta_pi_pj_VS_ToFi_ToFj_BC_2idp_2p, hdTheta_pi_pj_VS_ToFi_ToFj_RE_2idp_2p;
    hPlot2D hdTheta_pi_pj_VS_ToFi_ToFj_BC_3idp_2p, hdTheta_pi_pj_VS_ToFi_ToFj_AE_3idp_2p;
    hPlot2D hdTheta_pi_pj_VS_ToFi_ToFj_BC_4idp_2p, hdTheta_pi_pj_VS_ToFi_ToFj_AE_4idp_2p;
    //</editor-fold>

    // Theta_p1_p2 vs. position1-position2 plots (2p, CD only) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. position1-position2 plots (2p, CD only)">
    hPlot2D hTheta_pi_pj_VS_Posi_Posj_BC_2idp_2p, hTheta_pi_pj_VS_Posi_Posj_RE_2idp_2p;
    hPlot2D hTheta_pi_pj_VS_Posi_Posj_BC_3idp_2p, hTheta_pi_pj_VS_Posi_Posj_AE_3idp_2p;
    hPlot2D hTheta_pi_pj_VS_Posi_Posj_BC_4idp_2p, hTheta_pi_pj_VS_Posi_Posj_AE_4idp_2p;
    //</editor-fold>

    // Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD)">
    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p;
    std::string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p;
    std::string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p;
    std::string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p;
    std::string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p;
    std::string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p;
    std::string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p_Dir;
    //</editor-fold>

    // Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (2p, CD & FD) -------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (2p, CD & FD)">
    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p;
    std::string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p;
    std::string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p;
    std::string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p;
    std::string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p;
    std::string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p;
    std::string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p_Dir;
    //</editor-fold>

    /* Counting variables for good protons monitoring */
    int num_of_RM_2p_events_sCTOFhp = 0, num_of_AD_2p_events_from_3p_sCTOFhp = 0, num_of_AD_2p_events_from_4p_sCTOFhp = 0;
    int num_of_AD_2p_events_from_Xp_sCTOFhp = 0;  // events with 5p or more
    int num_of_RM_2p_events_dCDaFDd = 0, num_of_AD_2p_events_from_3p_dCDaFDd = 0, num_of_AD_2p_events_from_4p_dCDaFDd = 0;
    int num_of_AD_2p_events_from_Xp_dCDaFDd = 0;  // events with 5p or more

    int num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd = 0;  // events with mixed sCTOFhp dCDaFDd
    //</editor-fold>

    // Default constructor ---------------------------------------------------------------------------------------------------------------------------

    ParticleID() = default;

    // NeutronECAL_Cut_Veto function -----------------------------------------------------------------------------------------------------------------

    bool NeutronECAL_Cut_Veto(std::vector<region_part_ptr> &allParticles, std::vector<region_part_ptr> &electrons, const double &beamE, const int &index, const double &veto_cut);

    // ChargedParticleID function --------------------------------------------------------------------------------------------------------------------

    std::vector<int> ChargedParticleID(std::vector<region_part_ptr> &Particle, const DSCuts &Momentum_th);

    // FDNeutralParticle function --------------------------------------------------------------------------------------------------------------------

    void ReDefFDNeutrals(std::vector<region_part_ptr> allParticles, std::vector<int> &ID_Neutrons_FD, std::vector<int> &ID_Photons_FD);

    // FDNeutralParticleID function ------------------------------------------------------------------------------------------------------------------

    void FDNeutralParticleID(std::vector<region_part_ptr> allParticles, std::vector<region_part_ptr> electrons, std::vector<int> &FD_Neutrons_within_PID_cuts,
                             std::vector<int> &ID_Neutrons_FD, DSCuts &Neutron_momentum_th, std::vector<int> &FD_Photons_within_th, std::vector<int> &ID_Photons_FD,
                             DSCuts &Photon_momentum_th, DSCuts &Neutron_veto_cut, const double &beamE, const double &ECAL_V_edge_cut, const double &ECAL_W_edge_cut,
                             const bool &apply_nucleon_cuts);

    void FDNeutralParticleID(std::vector<region_part_ptr> allParticles, std::vector<int> &FD_Neutrons_within_th, std::vector<int> &ID_Neutrons_FD, DSCuts &Neutron_momentum_th,
                             std::vector<int> &FD_Photons_within_th, std::vector<int> &ID_Photons_FD, DSCuts &Photon_momentum_th, const bool &apply_nucleon_cuts);

    // GetLnFDIndex function ------------------------------------------------------------------------------------------------------------------------

    int GetLnFDIndex(std::vector<region_part_ptr> allParticles, std::vector<int> &FD_Neutrons_within_th, const bool &apply_nucleon_cuts);

    int GetCorrLnFDIndex(MomentumResolution &nRes, std::vector<region_part_ptr> allParticles, std::vector<int> &FD_Neutrons_within_th, const bool &apply_nucleon_cuts,
                         const bool &apply_nucleon_SmearAndCorr);

    // GetFDNeutronP function ------------------------------------------------------------------------------------------------------------------------

    double GetFDNeutronP(region_part_ptr &Neutron, const bool &apply_nucleon_cuts);

    // GetFDNeutrons function ------------------------------------------------------------------------------------------------------------------------

    std::vector<int> GetFDNeutrons(std::vector<region_part_ptr> &allParticles, const DSCuts &Momentum_cuts, const bool &apply_nucleon_cuts);

    // GetFDPhotons function -------------------------------------------------------------------------------------------------------------------------

    std::vector<int> GetFDPhotons(std::vector<region_part_ptr> &allParticles, const DSCuts &Momentum_cuts);

    // GetGoodParticles function ---------------------------------------------------------------------------------------------------------------------

    std::vector<int> GetGoodParticles(std::vector<region_part_ptr> &Particle, const DSCuts &Momentum_cuts);

    // GetGoodProtons function -----------------------------------------------------------------------------------------------------------------------

    std::vector<int> GetGoodProtons(const bool &apply_nucleon_cuts, std::vector<region_part_ptr> &protons, const std::vector<int> &IDProtons_ind, const DSCuts &Theta_p1_cuts_2p,
                                    const DSCuts &Theta_p2_cuts_2p, const DSCuts &dphi_pFD_pCD_2p);

    void SetGPMonitoringPlots(const bool &GoodProtonsMonitorPlots, std::string CToF_hits_monitoring_2p_Directory, std::string Double_detection_monitoring_2p_Directory);

    void GPMonitoring(const bool &GoodProtonsMonitorPlots, std::vector<region_part_ptr> &protons, const std::vector<int> &IDProtons_ind, const std::vector<int> &Protons_ind,
                      const DSCuts &Theta_p1_cuts_2p, const DSCuts &Theta_p2_cuts_2p, const DSCuts &dphi_p1_p2_2p, const double &Weight);

    // nParticleID function --------------------------------------------------------------------------------------------------------------------------

    void nParticleID(std::vector<region_part_ptr> &allParticles, std::vector<int> &ID_Neutrons_FD, const DSCuts &Neutron_momentum_th, std::vector<int> &ID_Photons_FD,
                     const DSCuts &Photon_momentum_th, const bool &apply_nucleon_cuts);

    // Fill neutron multiplicity plots functions -----------------------------------------------------------------------------------------------------

    // TODO: move out of this class

    // Neutrons by CLAS12 PID
    void FillNeutMultiPlots(std::vector<region_part_ptr> &allParticles, std::vector<region_part_ptr> &electrons, const double &Weight, const double &beamE, const double &ECAL_veto_cut,
                            hPlot1D &hNeutronMulti_BPID_BV, hPlot1D &hNeutronMulti_BPID_AV, std::vector<region_part_ptr> &Neutrons_BPID, hPlot1D &hNeutronMulti_APID_BV,
                            hPlot1D &hNeutronMulti_APID_AV, std::vector<region_part_ptr> &Neutrons_APID);

    void FillMultiPlots(std::vector<region_part_ptr> &allParticles, std::vector<region_part_ptr> &electrons, const double &Weight, const double &beamE, const double &ECAL_veto_cut,
                        hPlot1D &hNeutronMulti_BV, hPlot1D &hNeutronMulti_AV, std::vector<region_part_ptr> &Neutrons);

    // Neutrons by redefinition
    void FillNeutMultiPlots(std::vector<region_part_ptr> &allParticles, std::vector<region_part_ptr> &electrons, const double &Weight, const double &beamE, const double &ECAL_veto_cut,
                            hPlot1D &hNeutronMulti_BPID_BV, hPlot1D &hNeutronMulti_BPID_AV, std::vector<int> &Neutrons_BPID, hPlot1D &hNeutronMulti_APID_BV, hPlot1D &hNeutronMulti_APID_AV,
                            std::vector<int> &Neutrons_APID);

    void FillMultiPlots(std::vector<region_part_ptr> &allParticles, std::vector<region_part_ptr> &electrons, const double &Weight, const double &beamE, const double &ECAL_veto_cut,
                        hPlot1D &hNeutronMulti_BV, hPlot1D &hNeutronMulti_AV, std::vector<int> &Neutrons_indices);
};

#endif  // PARTICLEID_H
