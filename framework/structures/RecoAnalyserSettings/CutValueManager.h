//
// Created by Alon Sportes on 02/04/2025.
//

#ifndef CUTVALUEMANAGER_H
#define CUTVALUEMANAGER_H

#include <iostream>

// Include libraries:
#include "../../namespaces/general_utilities/analysis_math/analysis_math.h"

// Include settings:
#include "EventSelectionSettings.h"

// Include classes:
#include "../../classes/DSCuts/DSCuts.h"
#include "../../classes/ExperimentParameters/ExperimentParameters.cpp"
#include "../../classes/clas12ana/clas12ana.h"
#include "../../classes/hPlots/hPlot1D.cpp"

struct CutValueManager {
    // clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------

    // clas12ana cuts
    /* Number of Photo-electrons (Nphe) cuts (electrons only, FD) */
    DSCuts Nphe_cuts_FD;

    /* Chi2 cuts. NOTES:
     * Values for mean and sigma are filled from fit variables (overriding these values later).
     * Upper cut lim (Cuts.at(2)) is the same as the sigma that is used in clas12ana to apply PID cuts */
    DSCuts Chi2_Electron_cuts_CD = DSCuts("Chi2", "CD", "Electron", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Electron_cuts_FD = DSCuts("Chi2", "FD", "Electron", "1e cut", 0, -9999, 9999);

    DSCuts Chi2_Proton_cuts_CD = DSCuts("Chi2", "CD", "Proton", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Proton_cuts_FD = DSCuts("Chi2", "FD", "Proton", "1e cut", 0, -9999, 9999);

    DSCuts Chi2_piplus_cuts_CD = DSCuts("Chi2", "CD", "piplus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_piplus_cuts_FD = DSCuts("Chi2", "FD", "piplus", "1e cut", 0, -9999, 9999);

    DSCuts Chi2_piminus_cuts_CD = DSCuts("Chi2", "CD", "piminus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_piminus_cuts_FD = DSCuts("Chi2", "FD", "piminus", "1e cut", 0, -9999, 9999);

    // Kaon PID cuts
    /* Kaon pid cuts. Not really applied in our analysis. */
    DSCuts Chi2_Kplus_cuts_CD = DSCuts("Chi2", "CD", "Kplus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Kplus_cuts_FD = DSCuts("Chi2", "FD", "Kplus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Kminus_cuts_CD = DSCuts("Chi2", "CD", "Kminus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Kminus_cuts_FD = DSCuts("Chi2", "FD", "Kminus", "1e cut", 0, -9999, 9999);

    // Deuteron PID cuts
    /* Deuteron pid cuts. Not really applied in our analysis. */
    DSCuts Chi2_deuteron_cuts_CD = DSCuts("Chi2", "CD", "deuteron", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_deuteron_cuts_FD = DSCuts("Chi2", "FD", "deuteron", "1e cut", 0, -9999, 9999);

    /* Vertex cuts */
    DSCuts Vz_cut, Vz_cut_FD, Vz_cut_CD;
    DSCuts dVz_cuts, dVz_cuts_FD, dVz_cuts_CD;

    /* Sampling Fraction (SF) cuts (electrons only, FD) */
    DSCuts SF_cuts;
    DSCuts P_cuts;

    /* PCAL edge cuts (fiducial cuts, electrons only, FD) */
    DSCuts PCAL_edge_cuts;

    /* DC edge cuts (fiducial cuts, electrons only, FD) */
    DSCuts DC_e_edge_cuts, DC_edge_cuts;

    // Reco particles momentum thresholds (in GeV/c)
    /* Momentum thresholds (declarations) */
    DSCuts e_mom_th = DSCuts("Momentum_th", "FD", "Electron", "", 0, -9999, 9999);
    DSCuts ph_mom_th = DSCuts("Momentum_th", "FD", "Photons", "", 0, 0.3, 9999);
    DSCuts pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    DSCuts pipFD_mom_th = DSCuts("Momentum", "FD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts pipCD_mom_th = DSCuts("Momentum", "CD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    DSCuts pimFD_mom_th = DSCuts("Momentum", "FD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    DSCuts pimCD_mom_th = DSCuts("Momentum", "CD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());

    DSCuts p_mom_th, pFD_mom_th, pCD_mom_th, n_mom_th;  // Nucleons momentum thresholds for PID

    DSCuts no_p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, -9999, 9999);  // (no) momentum thresholds for (e,e'Xp)Y efficiency

    // Other cuts -------------------------------------------------------------------------------------------------------------------------------------------------------

    // Other cuts
    /* Neutron beta cuts (1n & nFDpCD, FD only) */
    DSCuts Beta_max_cut_ABF_FD_n_from_ph, Beta_max_cut_ABF_FD_n_from_ph_apprax;

    /* Neutron momentum cuts (1n & nFDpCD, FD only) */
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph;         // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_apprax;  // Approximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta.

    /* Truth-level momentum cuts */
    // TODO: remove pion mom. th. separation by CD and FD. It's useless (according to Adi)
    DSCuts TL_e_mom_cuts = DSCuts("Momentum", "FD", "Electron", "", 0, e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut());
    DSCuts TL_ph_mom_cuts = DSCuts("Momentum", "", "Photons", "", 0, ph_mom_th.GetLowerCut(), ph_mom_th.GetUpperCut());
    DSCuts TL_pip_mom_cuts = DSCuts("Momentum", "", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts TL_pipFD_mom_cuts = DSCuts("Momentum", "FD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts TL_pipCD_mom_cuts = DSCuts("Momentum", "CD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts TL_pim_mom_cuts = DSCuts("Momentum", "", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    DSCuts TL_pimFD_mom_cuts = DSCuts("Momentum", "FD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    DSCuts TL_pimCD_mom_cuts = DSCuts("Momentum", "CD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    DSCuts TL_pi0_mom_cuts = DSCuts("Momentum", "FD", "Pizero", "", 0, analysis_math::GetPi0MomTh(ph_mom_th.GetLowerCut()), 9999);

    DSCuts TL_n_mom_cuts, TL_p_mom_cuts, TL_pFD_mom_cuts, TL_pCD_mom_cuts;
    // DSCuts TL_e_mom_cuts = DSCuts("Momentum", "", "Electron", "", 0, e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut());
    // DSCuts TL_n_mom_cuts = DSCuts("Momentum", "", "Neutrons", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());
    // DSCuts TL_p_mom_cuts = DSCuts("Momentum", "", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    // DSCuts TL_pFD_mom_cuts = DSCuts("Momentum", "FD", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    // DSCuts TL_pCD_mom_cuts = DSCuts("Momentum", "CD", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    // DSCuts TL_pip_mom_cuts = DSCuts("Momentum", "", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    // DSCuts TL_pipFD_mom_cuts = DSCuts("Momentum", "FD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    // DSCuts TL_pipCD_mom_cuts = DSCuts("Momentum", "CD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    // DSCuts TL_pim_mom_cuts = DSCuts("Momentum", "", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    // DSCuts TL_pimFD_mom_cuts = DSCuts("Momentum", "FD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    // DSCuts TL_pimCD_mom_cuts = DSCuts("Momentum", "CD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    // DSCuts TL_pi0_mom_cuts = DSCuts("Momentum", "", "Pizero", "", 0, analysis_math::GetPi0MomTh(ph_mom_th.GetUpperCut()), 9999);
    // DSCuts TL_ph_mom_cuts = DSCuts("Momentum", "", "Photons", "", 0, ph_mom_th.GetLowerCut(), ph_mom_th.GetUpperCut());

    /* FD & CD theta range */
    DSCuts ThetaFD = DSCuts("Theta FD", "FD", "", "", 1, 5., 40.);
    DSCuts ThetaCD = DSCuts("Theta CD", "CD", "", "", 1, 40., 135.);

    /* FD & CD theta range (for AMaps) */
    DSCuts ThetaFD_AMaps = ThetaFD;
    // DSCuts ThetaFD_AMaps = DSCuts("Theta FD", "FD", "", "", 1, 5., 45.);

    /* Beta cut (1n, FD) */
    DSCuts Beta_cut = DSCuts("Beta_nuc", "FD", "", "1n", 1, 0, 9999);

    /* Neutron veto cut (1n & nFDpCD, FD) */
    DSCuts Neutron_veto_cut = DSCuts("Neutron veto", "FD", "", "1n", 0, 100, 9999);

    /* Ghost tracks handling (2p & pFDpCD, CD & FD) */
    DSCuts Theta_p1_cuts_2p = DSCuts("Theta_p1", "", "Proton", "2p", 40., -9999, 5.);
    DSCuts Theta_p2_cuts_2p = DSCuts("Theta_p2", "", "Proton", "2p", 40., -9999, 5.);
    DSCuts dphi_p1_p2_2p = DSCuts("dPhi_p1_p2", "", "Proton", "2p", 0, -9999, 15.);

    DSCuts Theta_pFD_cuts_2p = DSCuts("Theta_p1 leading", "", "Proton", "2p", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
    DSCuts Theta_pCD_cuts_2p = DSCuts("Theta_p2 recoil", "", "Proton", "2p", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
    DSCuts dphi_pFD_pCD_2p = DSCuts("dPhi_pFD_pCD", "", "Proton", "2p", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

    DSCuts Theta_pFD_cuts_pFDpCD = DSCuts("Theta_pFD", "", "Proton", "pFDpCD", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
    DSCuts Theta_pCD_cuts_pFDpCD = DSCuts("Theta_pCD", "", "Proton", "pFDpCD", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
    DSCuts dphi_pFD_pCD_pFDpCD = DSCuts("dPhi_pFD_pCD", "", "Proton", "pFDpCD", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

    DSCuts Theta_L_cuts_nFDpCD = DSCuts("Theta_L", "", "", "nFDpCD", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
    DSCuts Theta_R_cuts_nFDpCD = DSCuts("Theta_R", "", "", "nFDpCD", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
    DSCuts dphi_L_R_nFDpCD = DSCuts("dphi_L_R", "", "", "nFDpCD", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

    /* reco. kinematic cuts (based on nucleons' efficiency) */
    DSCuts FD_nucleon_theta_cut = DSCuts("FD Nucleon theta cut", "FD", "", "", 0, 0, 32.);
    //    DSCuts Nucleon_momentum_cut = DSCuts("Nucleon momentum cut", "FD", "", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());
    //    DSCuts FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "Protons and neutrons", 0, 1., 4.); // Original mom. KC
    //    DSCuts FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "Protons and neutrons", 0, 1., 3.); // Larry meeting (10/08/23)
    DSCuts FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "Protons and neutrons", 0, 1., 2.5);  // E4nu meeting (29/01/24)

    DSCuts MomRes_mu_cuts = DSCuts("MomRes_mu_cuts", "FD", "", "Protons and neutrons", 0, FD_nucleon_momentum_cut.GetLowerCut(), 2.2);        // E4nu meeting (29/01/24)
    DSCuts MomRes_sigma_cuts = DSCuts("MomRes_sigma_cuts", "FD", "", "Protons and neutrons", 0, FD_nucleon_momentum_cut.GetLowerCut(), 2.2);  // Adi meeting after (29/01/24)

    /* Cut for 2N reaction monitoring */
    // Cut disscussed in the e4nu collaboration meeting at JLab (March 2025)
    // P_miss_1N cuts:
    DSCuts P_miss_1N_QE_range = DSCuts("P_miss_1N QE range", "", "", "Protons and neutrons", 0, 0., 0.25);
    DSCuts P_miss_1N_MECandSRC_range = DSCuts("P_miss_1N MEC and SRC range", "", "", "Protons and neutrons", 0, 0.3, 0.6);
    DSCuts P_miss_1N_OtherReacAndFSI_range = DSCuts("P_miss_1N Other Reac And FSI", "", "", "Protons and neutrons", 0, 1., 9999);

    // E_miss_1N cuts:
    DSCuts E_miss_1N_QE_range = DSCuts("E_miss_1N QE range", "", "", "Protons and neutrons", 0, 0., 0.05);
    // DSCuts E_miss_1N_MECandSRC_range = DSCuts("E_miss_1N MEC and SRC range ExtEmiss", "", "", "Protons and neutrons", 0, 0.051, 0.5);  // TODO: get range from Larry!
    DSCuts E_miss_1N_MECandSRC_range = DSCuts("E_miss_1N MEC and SRC range", "", "", "Protons and neutrons", 0, 0.05, 0.2);  // TODO: get range from Larry!
    DSCuts E_miss_1N_OtherReacAndFSI_range = DSCuts("E_miss_1N Other Reac And FSI", "", "", "Protons and neutrons", 0, 0.5, 9999);

    // xB cuts:
    // DSCuts xB_cut = DSCuts("xB range", "", "", "Protons and neutrons", 0, 0.25, 9999);  // To kill RES or DIS events
    DSCuts xB_cut = DSCuts("xB range", "", "", "Protons and neutrons", 0, 1., 9999);  // To kill RES or DIS events

    // Theta_q_pCD cuts:
    // DSCuts Theta_q_pCD_cut = DSCuts("xB QE range", "", "", "Protons and neutrons", 0, 0., 90.);  // Gives lower FSI
    DSCuts Theta_q_pCD_cut = DSCuts("xB QE range", "", "", "Protons and neutrons", 0, 0., 40.);  // Gives lower FSI

    CutValueManager(ExperimentParameters &Experiment, const EventSelectionSettings &ESSettings) {
        Vz_cut = Experiment.GetVz_cuts();
        Vz_cut_FD = Experiment.GetVz_cuts_FD();
        Vz_cut_CD = Experiment.GetVz_cuts_CD();

        dVz_cuts = Experiment.GetdVz_cuts();
        dVz_cuts_FD = Experiment.GetdVz_cuts_FD();
        dVz_cuts_CD = Experiment.GetdVz_cuts_CD();

        if (ESSettings.limless_mom_eff_plots) {
            /* If we enforce TL cuts, don't use momentum thresholds on nucleons.
               Here we keep the same thresholds for FD neutrons and protons.  */
            p_mom_th = DSCuts("Momentum_th", "", "Protons", "", 0, -9999, 9999);
            pFD_mom_th = DSCuts("Momentum_th", "FD", "Protons", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
            pCD_mom_th = DSCuts("Momentum_th", "CD", "Protons", "", 0, -9999, 9999);
            n_mom_th = DSCuts("Momentum_th", "FD", "Neutrons", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
        } else {
            /* If we don't enforce TL cuts, use momentum thresholds on nucleons.
               Here we keep the same thresholds for FD neutrons and protons.  */
            p_mom_th = DSCuts("Momentum_th", "", "Protons", "", 0, 0.4, 9999);
            pFD_mom_th = DSCuts("Momentum_th", "FD", "Protons", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
            pCD_mom_th = DSCuts("Momentum_th", "CD", "Protons", "", 0, 0.3, 9999);
            n_mom_th = DSCuts("Momentum_th", "FD", "Neutrons", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
        }

        TL_p_mom_cuts = DSCuts("Momentum", "", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
        TL_pFD_mom_cuts = DSCuts("Momentum", "FD", "Proton", "", 0, pFD_mom_th.GetLowerCut(), pFD_mom_th.GetUpperCut());
        TL_pCD_mom_cuts = DSCuts("Momentum", "CD", "Proton", "", 0, pCD_mom_th.GetLowerCut(), pCD_mom_th.GetUpperCut());
        TL_n_mom_cuts = DSCuts("Momentum", "FD", "Neutrons", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());
    }

    void UpdateChargedParticleChi2Cuts(clas12ana &clasAna) {
        Chi2_Proton_cuts_CD.SetCutPram(clasAna.GetPidCutMean(2212, "CD"), -clasAna.GetPidCutSigma(2212, "CD"), clasAna.GetPidCutSigma(2212, "CD"));
        Chi2_Proton_cuts_FD.SetCutPram(clasAna.GetPidCutMean(2212, "FD"), -clasAna.GetPidCutSigma(2212, "FD"), clasAna.GetPidCutSigma(2212, "FD"));
        Chi2_piplus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(211, "CD"), -clasAna.GetPidCutSigma(211, "CD"), clasAna.GetPidCutSigma(211, "CD"));
        Chi2_piplus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(211, "FD"), -clasAna.GetPidCutSigma(211, "FD"), clasAna.GetPidCutSigma(211, "FD"));
        Chi2_piminus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(-211, "CD"), -clasAna.GetPidCutSigma(-211, "CD"), clasAna.GetPidCutSigma(-211, "CD"));
        Chi2_piminus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(-211, "FD"), -clasAna.GetPidCutSigma(-211, "FD"), clasAna.GetPidCutSigma(-211, "FD"));
    }

    // void FindChi2Xmax() {
    //     Chi2_Electron_cuts_FD.MeanFromHistogram = hChi2_Electron_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_Electron_1e_cut_FD.GetHistogram1D().GetMaximumBin());
    //     Chi2_Proton_cuts_CD.MeanFromHistogram = hChi2_Proton_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_Proton_1e_cut_CD.GetHistogram1D().GetMaximumBin());
    //     Chi2_Proton_cuts_FD.MeanFromHistogram = hChi2_Proton_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_Proton_1e_cut_FD.GetHistogram1D().GetMaximumBin());
    //     Chi2_Kplus_cuts_CD.MeanFromHistogram = hChi2_Kplus_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_Kplus_1e_cut_CD.GetHistogram1D().GetMaximumBin());
    //     Chi2_Kplus_cuts_FD.MeanFromHistogram = hChi2_Kplus_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_Kplus_1e_cut_FD.GetHistogram1D().GetMaximumBin());
    //     Chi2_Kminus_cuts_CD.MeanFromHistogram = hChi2_Kminus_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_Kminus_1e_cut_CD.GetHistogram1D().GetMaximumBin());
    //     Chi2_Kminus_cuts_FD.MeanFromHistogram = hChi2_Kminus_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_Kminus_1e_cut_FD.GetHistogram1D().GetMaximumBin());
    //     Chi2_piplus_cuts_CD.MeanFromHistogram = hChi2_piplus_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_piplus_1e_cut_CD.GetHistogram1D().GetMaximumBin());
    //     Chi2_piplus_cuts_FD.MeanFromHistogram = hChi2_piplus_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_piplus_1e_cut_FD.GetHistogram1D().GetMaximumBin());
    //     Chi2_piminus_cuts_CD.MeanFromHistogram = hChi2_piminus_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_piminus_1e_cut_CD.GetHistogram1D().GetMaximumBin());
    //     Chi2_piminus_cuts_FD.MeanFromHistogram = hChi2_piminus_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_piminus_1e_cut_FD.GetHistogram1D().GetMaximumBin());
    //     Chi2_deuteron_cuts_CD.MeanFromHistogram = hChi2_deuteron_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_deuteron_1e_cut_CD.GetHistogram1D().GetMaximumBin());
    //     Chi2_deuteron_cuts_FD.MeanFromHistogram = hChi2_deuteron_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_deuteron_1e_cut_FD.GetHistogram1D().GetMaximumBin());
    // }
};

#endif  // CUTVALUEMANAGER_H
