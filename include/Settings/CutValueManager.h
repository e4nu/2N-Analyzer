//
// Created by Alon Sportes on 02/04/2025.
//

#ifndef CUTVALUEMANAGER_H
#define CUTVALUEMANAGER_H

#include <iostream>

// Include YAML loader:
#include "../io/Yaml_loader.h"

// Include libraries:
#include "../libraries/general_utilities/analysis_math/analysis_math.h"
#include "../libraries/general_utilities/constants.h"

// Include settings:
#include "EventSelectionSettings.h"

// Include AnalysisTools:
#include "../AnalysisTools/DSCuts.h"
#include "../AnalysisTools/ExperimentParameters.h"
#include "../AnalysisTools/clas12ana.h"
#include "../AnalysisTools/hPlot1D.h"

/**
 * @struct CutValueManager
 * @brief A struct to manage the cut values for the analysis cuts, including chi2 cuts, vertex cuts, sampling fraction cuts, and momentum thresholds.
 * @details This struct is used to store the cut values for the analysis cuts, and to update them based on the run parameters and event selection settings. The cut values are stored in
 * DSCuts objects, which contain the mean and sigma values for the cuts, as well as the cut variable, region, particle, and applied cuts. The CutValueManager struct contains DSCuts objects
 * for each type of cut used in the analysis, such as chi2 cuts, vertex cuts, sampling fraction cuts, and momentum thresholds. The constructor initializes the cut values with default values,
 * which can be updated later based on the run parameters and event selection settings. The CutValueManager struct is used in the main code to apply the cuts to the data and simulation
 * samples. Note that the cut values are updated based on the run parameters and event selection settings, which allows for flexibility in applying different cuts for different runs or
 * analyses. The CutValueManager struct is designed to be easily extendable, allowing for the addition of new cuts or modification of existing cuts as needed for future analyses.
 * @note The cut values are stored in DSCuts objects, which contain the mean and sigma values for the cuts, as well as the cut variable, region, particle, and applied cuts. The cut values
 * can be updated based on the run parameters and event selection settings, which allows for flexibility in applying different cuts for different runs or analyses. It is important to set the
 * cut values appropriately based on the goals of the analysis and the characteristics of the data being analyzed, as well as to ensure that the cut values are consistent with the cuts being
 * applied in the main code.
 */
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
    // For e_mom_th, we use a minimal electron momentum cut of 0.8 GeV/c as, in CLAS12, we don’t trust the electron reconstruction below that (according to comments to RG-M analysis note).
    // In this project, this cut is applied on reco electrons in the updated clas12ana.
    // We place this here, to be a threshold for truth-level electrons, as they have only one electron per event
    // TODO: review this implementation, see if it works!
    DSCuts e_mom_th = DSCuts("Momentum_th", "FD", "Electron", "", 0, 0.8, 9999);
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

    /* FD & CD theta range */
    DSCuts ThetaFD = constants::ThetaFD;
    DSCuts ThetaCD = constants::ThetaCD;
    // DSCuts ThetaFD = DSCuts("Theta FD", "FD", "", "", 1, 5., 40.);
    // DSCuts ThetaCD = DSCuts("Theta CD", "CD", "", "", 1, 40., 135.);

    /* FD & CD theta range (for AMaps) */
    DSCuts ThetaFD_AMaps = ThetaFD;
    // DSCuts ThetaFD_AMaps = DSCuts("Theta FD", "FD", "", "", 1, 5., 45.);

    /* Beta cut (1n, FD) */
    DSCuts Beta_cut = DSCuts("Beta_nuc", "FD", "", "1n", 1, 0, 9999);

    /* Neutron veto cut (1n & nFDpCD, FD) */
    DSCuts Neutron_veto_cut = DSCuts("Neutron veto", "FD", "", "1n", 0, 100, 9999);
    DSCuts ChargedECALveto_veto_cut = DSCuts("Charged ECAL veto", "FD", "", "1n", 0, 0, Neutron_veto_cut.GetLowerCut());
    DSCuts NeutralECALveto_veto_cut = DSCuts("Neutral ECAL veto", "FD", "", "1n", 0, 0, 125);  // TODO: check if this cut should be updated

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
    // Cut discussed in the e4nu collaboration meeting at JLab (March 2025)
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

    CutValueManager() {}

    void UpdateCutManager(ExperimentParameters& Experiment, const EventSelectionSettings& ESSettings) {
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

    void UpdateChargedParticleChi2Cuts(clas12ana& clasAna) {
        Chi2_Proton_cuts_CD.SetCutPram(clasAna.GetPidCutMean(2212, "CD"), -clasAna.GetPidCutSigma(2212, "CD"), clasAna.GetPidCutSigma(2212, "CD"));
        Chi2_Proton_cuts_FD.SetCutPram(clasAna.GetPidCutMean(2212, "FD"), -clasAna.GetPidCutSigma(2212, "FD"), clasAna.GetPidCutSigma(2212, "FD"));
        Chi2_piplus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(211, "CD"), -clasAna.GetPidCutSigma(211, "CD"), clasAna.GetPidCutSigma(211, "CD"));
        Chi2_piplus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(211, "FD"), -clasAna.GetPidCutSigma(211, "FD"), clasAna.GetPidCutSigma(211, "FD"));
        Chi2_piminus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(-211, "CD"), -clasAna.GetPidCutSigma(-211, "CD"), clasAna.GetPidCutSigma(-211, "CD"));
        Chi2_piminus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(-211, "FD"), -clasAna.GetPidCutSigma(-211, "FD"), clasAna.GetPidCutSigma(-211, "FD"));
    }

    void FindChi2Xmax(hPlot1D& hChi2_Electron_1e_cut_FD, hPlot1D& hChi2_Proton_1e_cut_CD, hPlot1D& hChi2_Proton_1e_cut_FD, hPlot1D& hChi2_Kplus_1e_cut_CD, hPlot1D& hChi2_Kplus_1e_cut_FD,
                      hPlot1D& hChi2_Kminus_1e_cut_CD, hPlot1D& hChi2_Kminus_1e_cut_FD, hPlot1D& hChi2_piplus_1e_cut_CD, hPlot1D& hChi2_piplus_1e_cut_FD, hPlot1D& hChi2_piminus_1e_cut_CD,
                      hPlot1D& hChi2_piminus_1e_cut_FD, hPlot1D& hChi2_deuteron_1e_cut_CD, hPlot1D& hChi2_deuteron_1e_cut_FD) {
        auto FindPeakCenter = [](hPlot1D& hist, double fallback_mean) {
            auto h = hist.GetHistogram1D();
            if (h.GetEntries() <= 0 || h.GetNbinsX() <= 0) { return fallback_mean; }

            const int max_bin = h.GetMaximumBin();
            if (max_bin < 1 || max_bin > h.GetNbinsX()) { return fallback_mean; }

            return h.GetBinCenter(max_bin);
        };

        Chi2_Electron_cuts_FD.MeanFromHistogram = FindPeakCenter(hChi2_Electron_1e_cut_FD, Chi2_Electron_cuts_FD.MeanFromHistogram);
        Chi2_Electron_cuts_CD.MeanFromHistogram = Chi2_Electron_cuts_FD.MeanFromHistogram;
        Chi2_Proton_cuts_CD.MeanFromHistogram = FindPeakCenter(hChi2_Proton_1e_cut_CD, Chi2_Proton_cuts_CD.MeanFromHistogram);
        Chi2_Proton_cuts_FD.MeanFromHistogram = FindPeakCenter(hChi2_Proton_1e_cut_FD, Chi2_Proton_cuts_FD.MeanFromHistogram);
        Chi2_Kplus_cuts_CD.MeanFromHistogram = FindPeakCenter(hChi2_Kplus_1e_cut_CD, Chi2_Kplus_cuts_CD.MeanFromHistogram);
        Chi2_Kplus_cuts_FD.MeanFromHistogram = FindPeakCenter(hChi2_Kplus_1e_cut_FD, Chi2_Kplus_cuts_FD.MeanFromHistogram);
        Chi2_Kminus_cuts_CD.MeanFromHistogram = FindPeakCenter(hChi2_Kminus_1e_cut_CD, Chi2_Kminus_cuts_CD.MeanFromHistogram);
        Chi2_Kminus_cuts_FD.MeanFromHistogram = FindPeakCenter(hChi2_Kminus_1e_cut_FD, Chi2_Kminus_cuts_FD.MeanFromHistogram);
        Chi2_piplus_cuts_CD.MeanFromHistogram = FindPeakCenter(hChi2_piplus_1e_cut_CD, Chi2_piplus_cuts_CD.MeanFromHistogram);
        Chi2_piplus_cuts_FD.MeanFromHistogram = FindPeakCenter(hChi2_piplus_1e_cut_FD, Chi2_piplus_cuts_FD.MeanFromHistogram);
        Chi2_piminus_cuts_CD.MeanFromHistogram = FindPeakCenter(hChi2_piminus_1e_cut_CD, Chi2_piminus_cuts_CD.MeanFromHistogram);
        Chi2_piminus_cuts_FD.MeanFromHistogram = FindPeakCenter(hChi2_piminus_1e_cut_FD, Chi2_piminus_cuts_FD.MeanFromHistogram);
        Chi2_deuteron_cuts_CD.MeanFromHistogram = FindPeakCenter(hChi2_deuteron_1e_cut_CD, Chi2_deuteron_cuts_CD.MeanFromHistogram);
        Chi2_deuteron_cuts_FD.MeanFromHistogram = FindPeakCenter(hChi2_deuteron_1e_cut_FD, Chi2_deuteron_cuts_FD.MeanFromHistogram);
    }
};

// --------------------------------------------------------------------------------------
// YAML adapter specialization for CutValueManager
// --------------------------------------------------------------------------------------

template <>
struct Yaml_loader::SettingsAdapter<CutValueManager> {
    static void Apply(const YAML::Node& node, CutValueManager& s) {
        using namespace Yaml_loader;

        auto apply_cut = [&](const char* key, DSCuts& cut) {
            if (HasKey(node, key)) { ApplyYamlNodeToSettings(node[key], cut); }
        };

        // clas12ana cuts
        apply_cut("Nphe_cuts_FD", s.Nphe_cuts_FD);
        apply_cut("Chi2_Electron_cuts_CD", s.Chi2_Electron_cuts_CD);
        apply_cut("Chi2_Electron_cuts_FD", s.Chi2_Electron_cuts_FD);
        apply_cut("Chi2_Proton_cuts_CD", s.Chi2_Proton_cuts_CD);
        apply_cut("Chi2_Proton_cuts_FD", s.Chi2_Proton_cuts_FD);
        apply_cut("Chi2_piplus_cuts_CD", s.Chi2_piplus_cuts_CD);
        apply_cut("Chi2_piplus_cuts_FD", s.Chi2_piplus_cuts_FD);
        apply_cut("Chi2_piminus_cuts_CD", s.Chi2_piminus_cuts_CD);
        apply_cut("Chi2_piminus_cuts_FD", s.Chi2_piminus_cuts_FD);
        apply_cut("Chi2_Kplus_cuts_CD", s.Chi2_Kplus_cuts_CD);
        apply_cut("Chi2_Kplus_cuts_FD", s.Chi2_Kplus_cuts_FD);
        apply_cut("Chi2_Kminus_cuts_CD", s.Chi2_Kminus_cuts_CD);
        apply_cut("Chi2_Kminus_cuts_FD", s.Chi2_Kminus_cuts_FD);
        apply_cut("Chi2_deuteron_cuts_CD", s.Chi2_deuteron_cuts_CD);
        apply_cut("Chi2_deuteron_cuts_FD", s.Chi2_deuteron_cuts_FD);
        apply_cut("Vz_cut", s.Vz_cut);
        apply_cut("Vz_cut_FD", s.Vz_cut_FD);
        apply_cut("Vz_cut_CD", s.Vz_cut_CD);
        apply_cut("dVz_cuts", s.dVz_cuts);
        apply_cut("dVz_cuts_FD", s.dVz_cuts_FD);
        apply_cut("dVz_cuts_CD", s.dVz_cuts_CD);
        apply_cut("SF_cuts", s.SF_cuts);
        apply_cut("P_cuts", s.P_cuts);
        apply_cut("PCAL_edge_cuts", s.PCAL_edge_cuts);
        apply_cut("DC_e_edge_cuts", s.DC_e_edge_cuts);
        apply_cut("DC_edge_cuts", s.DC_edge_cuts);

        // Momentum thresholds and truth-level thresholds
        apply_cut("e_mom_th", s.e_mom_th);
        apply_cut("ph_mom_th", s.ph_mom_th);
        apply_cut("pip_mom_th", s.pip_mom_th);
        apply_cut("pipFD_mom_th", s.pipFD_mom_th);
        apply_cut("pipCD_mom_th", s.pipCD_mom_th);
        apply_cut("pim_mom_th", s.pim_mom_th);
        apply_cut("pimFD_mom_th", s.pimFD_mom_th);
        apply_cut("pimCD_mom_th", s.pimCD_mom_th);
        apply_cut("p_mom_th", s.p_mom_th);
        apply_cut("pFD_mom_th", s.pFD_mom_th);
        apply_cut("pCD_mom_th", s.pCD_mom_th);
        apply_cut("n_mom_th", s.n_mom_th);
        apply_cut("no_p_mom_th", s.no_p_mom_th);
        apply_cut("TL_e_mom_cuts", s.TL_e_mom_cuts);
        apply_cut("TL_ph_mom_cuts", s.TL_ph_mom_cuts);
        apply_cut("TL_pip_mom_cuts", s.TL_pip_mom_cuts);
        apply_cut("TL_pipFD_mom_cuts", s.TL_pipFD_mom_cuts);
        apply_cut("TL_pipCD_mom_cuts", s.TL_pipCD_mom_cuts);
        apply_cut("TL_pim_mom_cuts", s.TL_pim_mom_cuts);
        apply_cut("TL_pimFD_mom_cuts", s.TL_pimFD_mom_cuts);
        apply_cut("TL_pimCD_mom_cuts", s.TL_pimCD_mom_cuts);
        apply_cut("TL_pi0_mom_cuts", s.TL_pi0_mom_cuts);
        apply_cut("TL_n_mom_cuts", s.TL_n_mom_cuts);
        apply_cut("TL_p_mom_cuts", s.TL_p_mom_cuts);
        apply_cut("TL_pFD_mom_cuts", s.TL_pFD_mom_cuts);
        apply_cut("TL_pCD_mom_cuts", s.TL_pCD_mom_cuts);

        // Detector geometry / veto / momentum-resolution related cuts
        apply_cut("Beta_max_cut_ABF_FD_n_from_ph", s.Beta_max_cut_ABF_FD_n_from_ph);
        apply_cut("Beta_max_cut_ABF_FD_n_from_ph_apprax", s.Beta_max_cut_ABF_FD_n_from_ph_apprax);
        apply_cut("n_momentum_cuts_ABF_FD_n_from_ph", s.n_momentum_cuts_ABF_FD_n_from_ph);
        apply_cut("n_momentum_cuts_ABF_FD_n_from_ph_apprax", s.n_momentum_cuts_ABF_FD_n_from_ph_apprax);
        apply_cut("ThetaFD", s.ThetaFD);
        apply_cut("ThetaCD", s.ThetaCD);
        apply_cut("ThetaFD_AMaps", s.ThetaFD_AMaps);
        apply_cut("Beta_cut", s.Beta_cut);
        apply_cut("Neutron_veto_cut", s.Neutron_veto_cut);
        apply_cut("ChargedECALveto_veto_cut", s.ChargedECALveto_veto_cut);
        apply_cut("NeutralECALveto_veto_cut", s.NeutralECALveto_veto_cut);

        // Ghost-track handling and topology-dependent cuts
        apply_cut("Theta_p1_cuts_2p", s.Theta_p1_cuts_2p);
        apply_cut("Theta_p2_cuts_2p", s.Theta_p2_cuts_2p);
        apply_cut("dphi_p1_p2_2p", s.dphi_p1_p2_2p);
        apply_cut("Theta_pFD_cuts_2p", s.Theta_pFD_cuts_2p);
        apply_cut("Theta_pCD_cuts_2p", s.Theta_pCD_cuts_2p);
        apply_cut("dphi_pFD_pCD_2p", s.dphi_pFD_pCD_2p);
        apply_cut("Theta_pFD_cuts_pFDpCD", s.Theta_pFD_cuts_pFDpCD);
        apply_cut("Theta_pCD_cuts_pFDpCD", s.Theta_pCD_cuts_pFDpCD);
        apply_cut("dphi_pFD_pCD_pFDpCD", s.dphi_pFD_pCD_pFDpCD);
        apply_cut("Theta_L_cuts_nFDpCD", s.Theta_L_cuts_nFDpCD);
        apply_cut("Theta_R_cuts_nFDpCD", s.Theta_R_cuts_nFDpCD);
        apply_cut("dphi_L_R_nFDpCD", s.dphi_L_R_nFDpCD);

        // Reco kinematics and reaction monitoring
        apply_cut("FD_nucleon_theta_cut", s.FD_nucleon_theta_cut);
        apply_cut("FD_nucleon_momentum_cut", s.FD_nucleon_momentum_cut);
        apply_cut("MomRes_mu_cuts", s.MomRes_mu_cuts);
        apply_cut("MomRes_sigma_cuts", s.MomRes_sigma_cuts);
        apply_cut("P_miss_1N_QE_range", s.P_miss_1N_QE_range);
        apply_cut("P_miss_1N_MECandSRC_range", s.P_miss_1N_MECandSRC_range);
        apply_cut("P_miss_1N_OtherReacAndFSI_range", s.P_miss_1N_OtherReacAndFSI_range);
        apply_cut("E_miss_1N_QE_range", s.E_miss_1N_QE_range);
        apply_cut("E_miss_1N_MECandSRC_range", s.E_miss_1N_MECandSRC_range);
        apply_cut("E_miss_1N_OtherReacAndFSI_range", s.E_miss_1N_OtherReacAndFSI_range);
        apply_cut("xB_cut", s.xB_cut);
        apply_cut("Theta_q_pCD_cut", s.Theta_q_pCD_cut);
    }

    static void Validate(const CutValueManager& s) {
        auto validate_cut = [](const DSCuts& cut) { Yaml_loader::ValidateSettings(cut); };

        validate_cut(s.Nphe_cuts_FD);
        validate_cut(s.Chi2_Electron_cuts_CD);
        validate_cut(s.Chi2_Electron_cuts_FD);
        validate_cut(s.Chi2_Proton_cuts_CD);
        validate_cut(s.Chi2_Proton_cuts_FD);
        validate_cut(s.Chi2_piplus_cuts_CD);
        validate_cut(s.Chi2_piplus_cuts_FD);
        validate_cut(s.Chi2_piminus_cuts_CD);
        validate_cut(s.Chi2_piminus_cuts_FD);
        validate_cut(s.Chi2_Kplus_cuts_CD);
        validate_cut(s.Chi2_Kplus_cuts_FD);
        validate_cut(s.Chi2_Kminus_cuts_CD);
        validate_cut(s.Chi2_Kminus_cuts_FD);
        validate_cut(s.Chi2_deuteron_cuts_CD);
        validate_cut(s.Chi2_deuteron_cuts_FD);
        validate_cut(s.Vz_cut);
        validate_cut(s.Vz_cut_FD);
        validate_cut(s.Vz_cut_CD);
        validate_cut(s.dVz_cuts);
        validate_cut(s.dVz_cuts_FD);
        validate_cut(s.dVz_cuts_CD);
        validate_cut(s.SF_cuts);
        validate_cut(s.P_cuts);
        validate_cut(s.PCAL_edge_cuts);
        validate_cut(s.DC_e_edge_cuts);
        validate_cut(s.DC_edge_cuts);
        validate_cut(s.e_mom_th);
        validate_cut(s.ph_mom_th);
        validate_cut(s.pip_mom_th);
        validate_cut(s.pipFD_mom_th);
        validate_cut(s.pipCD_mom_th);
        validate_cut(s.pim_mom_th);
        validate_cut(s.pimFD_mom_th);
        validate_cut(s.pimCD_mom_th);
        validate_cut(s.p_mom_th);
        validate_cut(s.pFD_mom_th);
        validate_cut(s.pCD_mom_th);
        validate_cut(s.n_mom_th);
        validate_cut(s.no_p_mom_th);
        validate_cut(s.Beta_max_cut_ABF_FD_n_from_ph);
        validate_cut(s.Beta_max_cut_ABF_FD_n_from_ph_apprax);
        validate_cut(s.n_momentum_cuts_ABF_FD_n_from_ph);
        validate_cut(s.n_momentum_cuts_ABF_FD_n_from_ph_apprax);
        validate_cut(s.TL_e_mom_cuts);
        validate_cut(s.TL_ph_mom_cuts);
        validate_cut(s.TL_pip_mom_cuts);
        validate_cut(s.TL_pipFD_mom_cuts);
        validate_cut(s.TL_pipCD_mom_cuts);
        validate_cut(s.TL_pim_mom_cuts);
        validate_cut(s.TL_pimFD_mom_cuts);
        validate_cut(s.TL_pimCD_mom_cuts);
        validate_cut(s.TL_pi0_mom_cuts);
        validate_cut(s.TL_n_mom_cuts);
        validate_cut(s.TL_p_mom_cuts);
        validate_cut(s.TL_pFD_mom_cuts);
        validate_cut(s.TL_pCD_mom_cuts);
        validate_cut(s.ThetaFD);
        validate_cut(s.ThetaCD);
        validate_cut(s.ThetaFD_AMaps);
        validate_cut(s.Beta_cut);
        validate_cut(s.Neutron_veto_cut);
        validate_cut(s.ChargedECALveto_veto_cut);
        validate_cut(s.NeutralECALveto_veto_cut);
        validate_cut(s.Theta_p1_cuts_2p);
        validate_cut(s.Theta_p2_cuts_2p);
        validate_cut(s.dphi_p1_p2_2p);
        validate_cut(s.Theta_pFD_cuts_2p);
        validate_cut(s.Theta_pCD_cuts_2p);
        validate_cut(s.dphi_pFD_pCD_2p);
        validate_cut(s.Theta_pFD_cuts_pFDpCD);
        validate_cut(s.Theta_pCD_cuts_pFDpCD);
        validate_cut(s.dphi_pFD_pCD_pFDpCD);
        validate_cut(s.Theta_L_cuts_nFDpCD);
        validate_cut(s.Theta_R_cuts_nFDpCD);
        validate_cut(s.dphi_L_R_nFDpCD);
        validate_cut(s.FD_nucleon_theta_cut);
        validate_cut(s.FD_nucleon_momentum_cut);
        validate_cut(s.MomRes_mu_cuts);
        validate_cut(s.MomRes_sigma_cuts);
        validate_cut(s.P_miss_1N_QE_range);
        validate_cut(s.P_miss_1N_MECandSRC_range);
        validate_cut(s.P_miss_1N_OtherReacAndFSI_range);
        validate_cut(s.E_miss_1N_QE_range);
        validate_cut(s.E_miss_1N_MECandSRC_range);
        validate_cut(s.E_miss_1N_OtherReacAndFSI_range);
        validate_cut(s.xB_cut);
        validate_cut(s.Theta_q_pCD_cut);
    }
};

#endif  // CUTVALUEMANAGER_H
