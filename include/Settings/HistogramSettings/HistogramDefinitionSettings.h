//
// Created by Alon Sportes on 17/03/2026.
//

#ifndef HISTOGRAMDEFINITIONSETTINGS_H
#define HISTOGRAMDEFINITIONSETTINGS_H

#include <cmath>

#include "../libraries/general_utilities/constants.h"
#include "AnalysisCutSettings.h"
#include "CutValueManager.h"
#include "RunParameters.h"

struct HistogramDefinitionSettings {
    /* Histogram boundary variables. Used to unify histograms to the same boundaries. */

    // ==================================================================================================================================================================
    // Number of histogram bins
    // ==================================================================================================================================================================

    /* Default */
    int numTH1Dbins = 50;
    int numTH2Dbins = 75;

    /* Momentum plots */
    int numTH2Dbins_Mom_Plots = 75;

    /* Beta plots */
    int numTH1Dbins_Beta_Plots = 65;
    int numTH2Dbins_Beta_Plots = 150;

    /* Reaction monitoring plots */
    int numTH1Dbins_ReacMon_plots = 50;
    int numTH2Dbins_ReacMon_plots = 75;

    /* W plots */
    int numTH1Dbins_W_Plots = 50;
    int numTH2Dbins_W_Plots = 75;

    /* Angle plots */
    int numTH1Dbins_Ang_Plots = 50;
    int numTH2Dbins_Ang_Plots = 75;
    int numTH2Dbins_Electron_Ang_Plots = 100;
    int numTH2Dbins_Nucleon_Ang_Plots = 75;

    /* Angle plots */
    int numTH1Dbins_Momentum_transfer_plots = 50;
    int numTH2Dbins_Momentum_transfer_plots = 75;

    /* E_e plots */
    int numTH1Dbins_E_e_Plots = 50;
    int numTH2Dbins_E_e_Plots = 75;

    /* ET plots */
    int numTH1Dbins_ET_Plots = 50;

    /* E_cal plots */
    int numTH1Dbins_E_cal_Plots = 50;
    int numTH2Dbins_E_cal_Plots = 75;

    /* TKI plots */
    int numTH1Dbins_TKI_dP_T_Plots = 50;
    int numTH1Dbins_TKI_dAlpha_T_Plots = 30;
    int numTH2Dbins_TKI_Plots = 75;

    /* Efficiency % acceptance correction plots */
    int numTH1Dbins_Mom_eff_Plots = 50;
    int numTH1Dbins_Ang_eff_Plots = 50;
    int numTH2Dbins_Ang_eff_Plots = 75;
    int numTH2Dbins_Electron_Ang_eff_Plots = 100;
    int numTH2Dbins_Nucleon_Ang_eff_Plots = 75;

    /* Acceptance maps plots */
    int numTH1Dbins_AMaps_Plots = 50;
    int numTH2Dbins_AMaps_Plots = 75;
    int numTH2Dbins_Electron_AMaps_Plots = 100;
    int numTH2Dbins_Nucleon_AMaps_Plots = 75;

    /* Neutron resolution plots */
    /* Neutron multiplicity plots */
    int numTH1Dbins_nRes_Plots = 50;
    int numTH2Dbins_nRes_Plots = 100;

    /* Neutron multiplicity plots */
    int numTH1Dbins_multi_Plots = 10;

    // ==================================================================================================================================================================
    // Histogram limits
    // ==================================================================================================================================================================

    /* Nphe boundaries */
    double Nphe_lim = 40.;

    /* Chi2 boundaries */
    double Chi2_lim = 20.;

    /* Vertex boundaries */
    double Vx_FD_ulim = 5., Vx_FD_llim = -5.;
    double Vx_CD_ulim = 2., Vx_CD_llim = -2.;
    double Vy_FD_ulim = 5., Vy_FD_llim = -5.;
    double Vy_CD_ulim = 2., Vy_CD_llim = -2.;
    double Vz_lim = 15., Vz_ulim = 15., Vz_llim = -15.;
    double Vz_FD_lim = 15., Vz_FD_ulim = 5., Vz_FD_llim = -15.;
    double Vz_CD_lim = 15., Vz_CD_ulim = 15., Vz_CD_llim = -10.;
    double dV_lim = 6., dV_ulim = 6., dV_llim = -6.;
    double dV_lim_FD = 6., dV_ulim_FD = 6., dV_llim_FD = -6.;
    double dV_lim_CD = 6., dV_ulim_CD = 6., dV_llim_CD = -6.;

    /* SF boundries */
    double SF_ulim = 0.32, SF_llim = 0.12;

    /* Momentum boundries */
    double Momentum_llim = 0., Momentum_ulim = 0.;
    double FDMomentum_llim = 0., FDMomentum_ulim = 0.;
    double CDMomentum_llim = 0., CDMomentum_ulim = 0.;
    double P_nucFD_llim = 0., P_nucFD_ulim = 0.;
    double P_nucCD_llim = 0., P_nucCD_ulim = 0.;

    /* Reaction monitoring boundries */
    double P_miss_llim = 0., P_miss_ulim = 0.;
    double E_miss_llim = 0., E_miss_ulim = 0.;
    double xB_llim = 0., xB_ulim = 1.5;
    double theta_q_llim = 0., theta_q_ulim = 180.;

    /* W boundries */
    double W_llim = 0.35, W_ulim = 0.;

    /* Beta boundries */
    double dBeta_sigma_lim = 0.1;
    double Beta_dist_ulim = 1.1, Beta_dist_llim = 0.9;

    double dBeta_sigma_ZOOMOUT_lim = 0.1;
    double Beta_dist_ZOOMOUT_ulim = 1.1, Beta_dist_ZOOMOUT_llim = 0.9;

    double Beta_lim_const = 3.;
    double Beta_lim = 3.;
    double P_lim = 0.;

    /* Angle boundries */
    double Theta_llim_FD = 0., Theta_ulim_FD = 50.;
    double Theta_llim_CD = 30., Theta_ulim_CD = 155.;
    double Opening_Ang_wide_llim = 0., Opening_Ang_wide_ulim = 180.;
    double Opening_Ang_narrow_llim = 0., Opening_Ang_narrow_ulim = 100.;
    double Phi_llim = -180., Phi_ulim = 180.;

    /* Momentum transfer boundries */
    double Q2_llim_FD = 0., Q2_ulim_FD = 0.;

    /* TKI boundries */
    double dP_T_lim = 3.;

    void UpdateHDefSettings(const RunParameters& parameters, const AnalysisCutSettings& CutSettings, const CutValueManager& CutManager) {
        numTH1Dbins = 50;
        numTH2Dbins = 75;

        if (parameters.isData) { numTH2Dbins *= 2; }

        numTH2Dbins_Mom_Plots = numTH2Dbins;

        numTH1Dbins_Beta_Plots = 65;
        numTH2Dbins_Beta_Plots = numTH2Dbins * 2;

        numTH1Dbins_ReacMon_plots = numTH1Dbins;
        numTH2Dbins_ReacMon_plots = numTH2Dbins;

        numTH1Dbins_W_Plots = numTH1Dbins;
        numTH2Dbins_W_Plots = numTH2Dbins;

        numTH1Dbins_Ang_Plots = numTH1Dbins;
        numTH2Dbins_Ang_Plots = numTH2Dbins;
        numTH2Dbins_Electron_Ang_Plots = 100;
        numTH2Dbins_Nucleon_Ang_Plots = 75;

        numTH1Dbins_Momentum_transfer_plots = numTH1Dbins;
        numTH2Dbins_Momentum_transfer_plots = numTH2Dbins;

        numTH1Dbins_E_e_Plots = numTH1Dbins;
        numTH2Dbins_E_e_Plots = numTH2Dbins;

        numTH1Dbins_ET_Plots = numTH1Dbins;

        numTH1Dbins_E_cal_Plots = numTH1Dbins;
        numTH2Dbins_E_cal_Plots = numTH2Dbins;

        numTH1Dbins_TKI_dP_T_Plots = numTH1Dbins;
        numTH1Dbins_TKI_dAlpha_T_Plots = 30;
        numTH2Dbins_TKI_Plots = numTH2Dbins;

        numTH1Dbins_Mom_eff_Plots = numTH1Dbins;
        numTH1Dbins_Ang_eff_Plots = numTH1Dbins_Ang_Plots;
        numTH2Dbins_Ang_eff_Plots = numTH2Dbins_Ang_Plots;
        numTH2Dbins_Electron_Ang_eff_Plots = numTH2Dbins_Electron_Ang_Plots;
        numTH2Dbins_Nucleon_Ang_eff_Plots = numTH2Dbins_Nucleon_Ang_Plots;

        numTH1Dbins_AMaps_Plots = numTH1Dbins_Ang_Plots;
        numTH2Dbins_AMaps_Plots = numTH2Dbins_Ang_Plots;
        numTH2Dbins_Electron_AMaps_Plots = numTH2Dbins_Electron_Ang_Plots;
        numTH2Dbins_Nucleon_AMaps_Plots = numTH2Dbins_Nucleon_Ang_Plots;

        numTH1Dbins_nRes_Plots = numTH1Dbins_Ang_Plots;
        numTH2Dbins_nRes_Plots = numTH2Dbins_Nucleon_Ang_Plots * 4 / 3;

        numTH1Dbins_multi_Plots = 10;

        Nphe_lim = 40.;
        Chi2_lim = CutSettings.apply_cuts ? 9. : 20.;

        Vz_lim = 15.;
        Vz_ulim = Vz_lim;
        Vz_llim = -Vz_lim;
        Vz_FD_lim = Vz_lim;
        Vz_FD_ulim = Vz_ulim * (1. / 3.);
        Vz_FD_llim = Vz_llim;
        Vz_CD_lim = Vz_lim;
        Vz_CD_ulim = Vz_ulim;
        Vz_CD_llim = Vz_llim * (2. / 3.);
        dV_lim = 8.;
        dV_ulim = dV_lim;
        dV_llim = -dV_lim;
        dV_lim_FD = dV_lim;
        dV_ulim_FD = dV_ulim;
        dV_llim_FD = dV_llim;
        dV_lim_CD = dV_lim;
        dV_ulim_CD = dV_ulim;
        dV_llim_CD = dV_llim;

        if (CutSettings.apply_cuts) {
            const double dVz_lim = CutManager.Vz_cut.GetUpperCut() - CutManager.Vz_cut.GetLowerCut();
            const double dVz_FD_lim = CutManager.Vz_cut_FD.GetUpperCut() - CutManager.Vz_cut_FD.GetLowerCut();
            const double dVz_CD_lim = CutManager.Vz_cut_CD.GetUpperCut() - CutManager.Vz_cut_CD.GetLowerCut();
            const double ddV_lim = CutManager.dVz_cuts.GetUpperCut() - CutManager.dVz_cuts.GetLowerCut();
            const double ddV_lim_FD = CutManager.dVz_cuts_FD.GetUpperCut() - CutManager.dVz_cuts_FD.GetLowerCut();
            const double ddV_lim_CD = CutManager.dVz_cuts_CD.GetUpperCut() - CutManager.dVz_cuts_CD.GetLowerCut();

            Vz_ulim = CutManager.Vz_cut.GetUpperCut() + 0.2 * dVz_lim;
            Vz_llim = CutManager.Vz_cut.GetLowerCut() - 0.1 * dVz_lim;
            Vz_FD_ulim = CutManager.Vz_cut_FD.GetUpperCut() + 0.2 * dVz_FD_lim;
            Vz_FD_llim = CutManager.Vz_cut_FD.GetLowerCut() - 0.1 * dVz_FD_lim;
            Vz_CD_ulim = CutManager.Vz_cut_CD.GetUpperCut() + 0.2 * dVz_CD_lim;
            Vz_CD_llim = CutManager.Vz_cut_CD.GetLowerCut() - 0.1 * dVz_CD_lim;
            dV_ulim = CutManager.dVz_cuts.GetUpperCut() + 0.2 * ddV_lim;
            dV_llim = CutManager.dVz_cuts.GetLowerCut() - 0.1 * ddV_lim;
            dV_ulim_FD = CutManager.dVz_cuts_FD.GetUpperCut() + 0.2 * ddV_lim_FD;
            dV_llim_FD = CutManager.dVz_cuts_FD.GetLowerCut() - 0.1 * ddV_lim_FD;
            dV_ulim_CD = CutManager.dVz_cuts_CD.GetUpperCut() + 0.2 * ddV_lim_CD;
            dV_llim_CD = CutManager.dVz_cuts_CD.GetLowerCut() - 0.1 * ddV_lim_CD;

            Vz_lim = 8.;
            Vz_FD_lim = 8.;
            Vz_CD_lim = 8.;
            dV_lim = 8.;
            dV_lim_FD = 8.;
            dV_lim_CD = 8.;
        }

        SF_ulim = 0.31;
        SF_llim = 0.13;

        // Default:
        Momentum_llim = 0.;
        Momentum_ulim = parameters.beamE * 1.1;

        // FD nucleons (1nFD, 1pFD, pFDpCD and nFDpCD):
        FDMomentum_llim = Momentum_llim;
        FDMomentum_ulim = Momentum_ulim;

        // CD nucleons (pFDpCD & nFDpCD):
        CDMomentum_llim = 0.;
        CDMomentum_ulim = parameters.beamE / 2;

        // Default:
        P_nucFD_llim = 0.;
        P_nucFD_ulim = parameters.beamE * 1.1;

        // CD nucleons (pFDpCD & nFDpCD):
        P_nucCD_llim = 0.;
        P_nucCD_ulim = parameters.beamE / 2;

        if (CutSettings.apply_kinematical_cuts) {
            P_nucFD_llim = CutManager.FD_nucleon_momentum_cut.GetLowerCut();
            P_nucFD_ulim = CutManager.FD_nucleon_momentum_cut.GetUpperCut() * 1.1;
            P_nucCD_llim = 0.4;
            P_nucCD_ulim = 2.5;
            FDMomentum_llim = CutManager.FD_nucleon_momentum_cut.GetLowerCut() * 0.8;
            FDMomentum_ulim = CutManager.FD_nucleon_momentum_cut.GetUpperCut() * 1.2;
        }

        P_miss_llim = 0.;
        P_miss_ulim = parameters.beamE * 1.1;
        E_miss_llim = 0.;
        E_miss_ulim = parameters.beamE * 0.9;
        xB_llim = 0.;
        xB_ulim = 1.5;
        theta_q_llim = 0.;
        theta_q_ulim = 180.;

        W_llim = 0.35;
        W_ulim = 1.1 * std::sqrt((parameters.beamE + constants::m_p) * (parameters.beamE + constants::m_p) - parameters.beamE * parameters.beamE);

        dBeta_sigma_lim = 0.1;
        Beta_dist_ulim = 1 + dBeta_sigma_lim;
        Beta_dist_llim = 1 - dBeta_sigma_lim;

        dBeta_sigma_ZOOMOUT_lim = 0.1;
        Beta_dist_ZOOMOUT_ulim = 1 + dBeta_sigma_ZOOMOUT_lim;
        Beta_dist_ZOOMOUT_llim = 0.9;

        Beta_lim_const = 3.;
        Beta_lim = 3.;
        P_lim = parameters.beamE * 1.425;

        if (CutSettings.apply_cuts) {
            Beta_lim = 1.25;
            P_lim = parameters.beamE * 1.1;
        }

        Theta_llim_FD = 0.;
        Theta_ulim_FD = 50.;
        Theta_llim_CD = 30.;
        Theta_ulim_CD = 155.;
        Opening_Ang_wide_llim = 0.;
        Opening_Ang_wide_ulim = 180.;
        Opening_Ang_narrow_llim = 0.;
        Opening_Ang_narrow_ulim = 100.;
        Phi_llim = -180.;
        Phi_ulim = 180.;

        Q2_llim_FD = 0.;
        Q2_ulim_FD = parameters.beamE * 1.1;

        if (parameters.is2GeVSample) {
            // Q2_ulim_FD = 0.15;
            Q2_ulim_FD = 0.8;
            // Q2_ulim_FD = 1;
        } else if (parameters.is4GeVSample) {
            // Q2_ulim_FD = 1;
            Q2_ulim_FD = 3;
        } else if (parameters.is6GeVSample) {
            // Q2_ulim_FD = 1.5;
            Q2_ulim_FD = 5;
        }

        dP_T_lim = 3.;
    }
};

#endif  // HISTOGRAMDEFINITIONSETTINGS_H
