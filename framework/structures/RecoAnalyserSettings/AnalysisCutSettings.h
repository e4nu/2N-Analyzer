//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef ANALYSISCUTSETTINGS_H
#define ANALYSISCUTSETTINGS_H

#include <iostream>

// Include libraries:
#include "../../namespaces/setup/path_definitions.h"

// Include settings:
#include "AcceptanceMapsSettings.h"
#include "EventSelectionSettings.h"
#include "MomentumResolutionSettings.h"
#include "RunParameters.h"

// Include classes:
#include "../../classes/ExperimentParameters/ExperimentParameters.cpp"
#include "../../classes/Settings/Settings.cpp"

struct AnalysisCutSettings {
    // Cuts setup
    /* Settings that allow to disable/enable every cut individually */

    // clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------
    bool apply_cuts;                  // master ON/OFF switch for applying cuts
    bool clas12ana_particles;         // TODO: move from here!
    bool only_preselection_cuts;      // keep as false for regular runs!
    bool only_electron_quality_cuts;  // keep as false for regular runs!

    /* Preselection cuts (event cuts) */
    bool apply_preselection_cuts;        // master ON/OFF switch for preselection cuts
    bool apply_Vz_e_cuts;                // Electron vertex cuts
    bool apply_Vz_cuts, apply_dVz_cuts;  // Vertex cuts
    bool apply_DC_e_fiducial_cuts;       // Electron DC fiducial (edge) cuts
    bool apply_DC_fiducial_cuts;         // DC fiducial (edge) cuts

    /* Electron quality cuts */
    bool apply_electron_quality_cuts;  // master ON/OFF switch for eQC
    bool apply_Nphe_cut;               // Number of photo-electrons in HTCC cut
    bool apply_ECAL_SF_cuts;           // SF cut on both E_deb AND P_e
    bool apply_ECAL_P_cuts;            // SF cut on P_e (keep as false for now!)
    bool apply_ECAL_fiducial_cuts;     // ECAL edge cuts for other charged particles
    bool apply_Electron_beta_cut;      // Electron beta cut

    /* Chi2 cuts (= PID cuts) */
    bool apply_chi2_cuts_1e_cut;

    // My analysis cuts ---------------------------------------------------------------------------------------------------------------------------------------------------
    /* Nucleon cuts */
    bool apply_nucleon_cuts;  // set as true to get good protons and calculate upper neutron momentum th.

    /* Physical cuts */
    bool apply_nucleon_physical_cuts;  // nucleon physical cuts master
    // TODO: automate adding upper mom. th. to nucleon cuts (for nRes calc)
    bool apply_nBeta_fit_cuts;  // apply neutron upper mom. th.
    bool apply_fiducial_cuts;
    bool apply_kinematical_cuts;
    bool apply_kinematical_weights;
    bool apply_nucleon_SmearAndCorr;

    /* Reaction monitoring cuts */
    bool apply_reaction_monitoring_cuts;  // master ON/OFF switch for reaction monitoring cuts
    bool apply_P_miss_in_QE_range_cuts;
    bool apply_E_miss_in_QE_range_cuts;
    bool apply_P_miss_in_MECandSRC_range_cuts;
    bool apply_E_miss_in_MECandSRC_range_cuts;
    bool apply_xB_in_QE_range_cuts;
    bool apply_theta_q_pCD_in_lower_FSI_range_cut;

    const bool custom_cuts_naming;  // Enable custom cuts naming

    // Constructor with default values
    AnalysisCutSettings()
        // Cuts setup:
        : apply_cuts(true),
          clas12ana_particles(true),
          only_preselection_cuts(false),
          only_electron_quality_cuts(false),

          // Preselection cuts (event cuts):
          apply_preselection_cuts(true),
          apply_Vz_e_cuts(true),
          apply_Vz_cuts(true),
          apply_dVz_cuts(true),
          apply_DC_e_fiducial_cuts(true),
          apply_DC_fiducial_cuts(true),

          // Electron quality cuts:
          apply_electron_quality_cuts(true),
          apply_Nphe_cut(true),
          apply_ECAL_SF_cuts(true),
          apply_ECAL_P_cuts(false),
          apply_ECAL_fiducial_cuts(true),
          apply_Electron_beta_cut(true),

          // Chi2 cuts (= PID cuts):
          apply_chi2_cuts_1e_cut(true),

          // Nucleon cuts:
          apply_nucleon_cuts(true),

          // Physical cuts:
          apply_nucleon_physical_cuts(true),
          apply_nBeta_fit_cuts(true),
          apply_fiducial_cuts(false),
          apply_kinematical_cuts(false),
          apply_kinematical_weights(false),
          apply_nucleon_SmearAndCorr(false),

          // Reaction monitoring cuts:

        //   /* Regular */
        //   apply_reaction_monitoring_cuts(true),
        //   apply_P_miss_in_QE_range_cuts(false),
        //   apply_E_miss_in_QE_range_cuts(false),
        //   apply_P_miss_in_MECandSRC_range_cuts(false),
        //   apply_E_miss_in_MECandSRC_range_cuts(false),
        //   apply_xB_in_QE_range_cuts(false),
        //   apply_theta_q_pCD_in_lower_FSI_range_cut(false),

        //   /* P_E_miss_in_QE */
        //   apply_reaction_monitoring_cuts(true),
        //   apply_P_miss_in_QE_range_cuts(true),
        //   apply_E_miss_in_QE_range_cuts(true),
        //   apply_P_miss_in_MECandSRC_range_cuts(false),
        //   apply_E_miss_in_MECandSRC_range_cuts(false),
        //   apply_xB_in_QE_range_cuts(false),
        //   apply_theta_q_pCD_in_lower_FSI_range_cut(false),

        //   /* P_E_miss_in_QE_theta_q_pCD */
        //   apply_reaction_monitoring_cuts(true),
        //   apply_P_miss_in_QE_range_cuts(true),
        //   apply_E_miss_in_QE_range_cuts(true),
        //   apply_P_miss_in_MECandSRC_range_cuts(false),
        //   apply_E_miss_in_MECandSRC_range_cuts(false),
        //   apply_xB_in_QE_range_cuts(false),
        //   apply_theta_q_pCD_in_lower_FSI_range_cut(true),

        //   /* P_E_miss_in_MECandSRC */
        //   apply_reaction_monitoring_cuts(true),
        //   apply_P_miss_in_QE_range_cuts(false),
        //   apply_E_miss_in_QE_range_cuts(false),
        //   apply_P_miss_in_MECandSRC_range_cuts(true),
        //   apply_E_miss_in_MECandSRC_range_cuts(true),
        //   apply_xB_in_QE_range_cuts(false),
        //   apply_theta_q_pCD_in_lower_FSI_range_cut(false),

        //   /* P_E_miss_in_MECandSRC_theta_q_pCD */
        //   apply_reaction_monitoring_cuts(true),
        //   apply_P_miss_in_QE_range_cuts(false),
        //   apply_E_miss_in_QE_range_cuts(false),
        //   apply_P_miss_in_MECandSRC_range_cuts(true),
        //   apply_E_miss_in_MECandSRC_range_cuts(true),
        //   apply_xB_in_QE_range_cuts(false),
        //   apply_theta_q_pCD_in_lower_FSI_range_cut(true),

          /* P_E_miss_in_QEandMECandSRC */
          apply_reaction_monitoring_cuts(true),
          apply_P_miss_in_QE_range_cuts(true),
          apply_E_miss_in_QE_range_cuts(true),
          apply_P_miss_in_MECandSRC_range_cuts(true),
          apply_E_miss_in_MECandSRC_range_cuts(true),
          apply_xB_in_QE_range_cuts(false),
          apply_theta_q_pCD_in_lower_FSI_range_cut(false),

        //   /* P_E_miss_in_QEandMECandSRC_theta_q_pCD */
        //   apply_reaction_monitoring_cuts(true),
        //   apply_P_miss_in_QE_range_cuts(true),
        //   apply_E_miss_in_QE_range_cuts(true),
        //   apply_P_miss_in_MECandSRC_range_cuts(true),
        //   apply_E_miss_in_MECandSRC_range_cuts(true),
        //   apply_xB_in_QE_range_cuts(false),
        //   apply_theta_q_pCD_in_lower_FSI_range_cut(true),

          custom_cuts_naming(true) {}

    void RefreshSettings(const RunParameters& parameters, EventSelectionSettings& ESSettings, AcceptanceMapsSettings& AMapsSettings, MomentumResolutionSettings& MomResSettings) {
        // Auto-disable variables
        if (only_preselection_cuts || only_electron_quality_cuts) {
            apply_cuts = false;
        } else {
            if (AMapsSettings.Generate_Electron_AMaps) {
                apply_cuts = false;  // Electron acceptance maps (for fiducial cuts) should not use any electron PID or pre-selection cuts!
            }

            if (AMapsSettings.Generate_Nucleon_AMaps) {
                apply_cuts = true;

                apply_preselection_cuts = true;
                apply_Vz_e_cuts = true, apply_Vz_cuts = apply_dVz_cuts = false;
                apply_DC_e_fiducial_cuts = true, apply_DC_fiducial_cuts = false;

                apply_electron_quality_cuts = apply_Nphe_cut = apply_ECAL_SF_cuts = true;
                apply_ECAL_P_cuts = false;
                apply_ECAL_fiducial_cuts = apply_Electron_beta_cut = true;

                apply_chi2_cuts_1e_cut = false;
            }
        }

        if (!apply_cuts) {
            if (!only_preselection_cuts) { apply_preselection_cuts = false; }
            if (!only_electron_quality_cuts) { apply_electron_quality_cuts = false; }

            apply_chi2_cuts_1e_cut = apply_nucleon_cuts = false;
        }

        if (!apply_preselection_cuts) { apply_Vz_e_cuts = apply_Vz_cuts = apply_dVz_cuts = apply_DC_e_fiducial_cuts = apply_DC_fiducial_cuts = false; }

        if (!apply_electron_quality_cuts) { apply_Nphe_cut = apply_ECAL_SF_cuts = apply_ECAL_P_cuts = apply_ECAL_fiducial_cuts = apply_Electron_beta_cut = false; }

        if (!apply_chi2_cuts_1e_cut) { apply_nucleon_cuts = false; }

        // TODO: uncomment Generate_WMaps in here in the future!
        if (!apply_nucleon_cuts) { /* AMapsSettings.Generate_WMaps = */
            apply_nucleon_physical_cuts = false;
        }

        if (!apply_nucleon_physical_cuts) {
            apply_nBeta_fit_cuts = apply_fiducial_cuts = apply_kinematical_cuts = apply_kinematical_weights = apply_nucleon_SmearAndCorr = false;
            apply_reaction_monitoring_cuts = false;
        } else {
            if (MomResSettings.Calculate_momResS2) { apply_nucleon_SmearAndCorr = true; }
        }

        if (!apply_reaction_monitoring_cuts) {
            apply_P_miss_in_QE_range_cuts = apply_E_miss_in_QE_range_cuts = apply_P_miss_in_MECandSRC_range_cuts = apply_E_miss_in_MECandSRC_range_cuts = false;
            apply_xB_in_QE_range_cuts = apply_theta_q_pCD_in_lower_FSI_range_cut = false;
        }

        if (AMapsSettings.Generate_WMaps) { apply_fiducial_cuts = false; }

        if (!MomResSettings.VaryingDelta) { apply_nucleon_SmearAndCorr = false; }

        if (parameters.isData) { 
            // no TL calculation, AMap/WMap generation nor nRes calculation when running on data
            ESSettings.calculate_truth_level = AMapsSettings.Generate_WMaps = MomResSettings.plot_and_fit_MomRes = MomResSettings.momRes_test = false; }

        if (!ESSettings.calculate_truth_level) { AMapsSettings.AMaps_calc_with_one_reco_electron = ESSettings.fill_TL_plots = ESSettings.Rec_wTL_ES = false; }

        if (ESSettings.Rec_wTL_ES) {
            /* if ESSettings.Rec_wTL_ES = true, there are no momentum thresholds, and we get an infinite loop in the nRes slice calculations!
               Additionally, there is no need to calculate the resolution and efficiency in the same time! */
            MomResSettings.plot_and_fit_MomRes = false;
        } else if (!ESSettings.Rec_wTL_ES) {
            /* if ESSettings.Rec_wTL_ES = false, keep fiducial cuts with the overlapping maps! (safety measure) */
            ESSettings.Calc_eff_overlapping_FC = true;
        }

        if (!MomResSettings.plot_and_fit_MomRes) { MomResSettings.Calculate_momResS2 = false; }

        if ((MomResSettings.Calculate_momResS2    // Don't run calculate momResS2 and run on it at the same time
             && MomResSettings.Run_with_momResS2  // Don't run calculate momResS2 and small momentum slices at the same time
             ) ||
            (MomResSettings.Calculate_momResS2 && !MomResSettings.VaryingDelta)) {
            std::cout << "\033[33m\n\nmomRes order error! Aborting...\n\n", exit(1);
        }
    }

    void CustomNamingRefresh(Settings& settings, const AcceptanceMapsSettings& AMapsSettings, const MomentumResolutionSettings& MomResSettings, const EventSelectionSettings& ESSettings,
                             const RunParameters& parameters, std::string& run_plots_path, std::string& run_plots_log_save_Directory, std::string& skipped_files_list_save_Directory) {
        /* Save plots to custom-named folders, to allow multi-sample runs at once. */
        settings.SetCustomCutsNaming(custom_cuts_naming);
        settings.ConfigureStatuses(apply_cuts, clas12ana_particles, only_preselection_cuts, apply_chi2_cuts_1e_cut, only_electron_quality_cuts, apply_nucleon_cuts,
                                   ESSettings.Enable_FD_photons, apply_nucleon_SmearAndCorr, apply_kinematical_cuts, apply_kinematical_weights, apply_fiducial_cuts,
                                   (AMapsSettings.Generate_Electron_AMaps || AMapsSettings.Generate_Nucleon_AMaps), MomResSettings.plot_and_fit_MomRes, MomResSettings.VaryingDelta,
                                   MomResSettings.Calculate_momResS2, MomResSettings.Run_with_momResS2, MomResSettings.momRes_test, ESSettings.Rec_wTL_ES, ESSettings.ZoomIn_On_mom_th_plots);

        settings.SetPaths(path_definitions::PathDefinitions.WorkingDirectory, parameters.SampleName, run_plots_path, apply_cuts, apply_chi2_cuts_1e_cut, apply_nucleon_cuts);
        settings.GetPlotsPath(run_plots_path);
        settings.GetPlotsLogSaveDirectory(run_plots_log_save_Directory);
        settings.GetSkippedHipoFilesLogSaveDirectory(skipped_files_list_save_Directory);
    }
};

#endif  // ANALYSISCUTSETTINGS_H
