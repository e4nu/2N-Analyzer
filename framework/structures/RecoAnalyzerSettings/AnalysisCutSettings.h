//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef ANALYSISCUTSETTINGS_H
#define ANALYSISCUTSETTINGS_H

#include <iostream>

// Include libraries:
#include "../../namespaces/general_utilities/utilities.h"
#include "../../namespaces/setup/path_definitions.h"

// Include settings:
#include "AcceptanceMapsSettings.h"
#include "CutValueManager.h"
#include "EventSelectionSettings.h"
#include "MomentumResolutionSettings.h"
#include "RunParameters.h"

// Include classes:
#include "../../classes/ExperimentParameters/ExperimentParameters.cpp"
#include "../../classes/Settings/Settings.cpp"
#include "../../classes/clas12ana/clas12ana.cpp"

namespace pd = path_definitions;

/**
 * @struct AnalysisCutSettings
 * @brief Settings related to analysis cuts, including various types of cuts such as preselection cuts, electron quality cuts, chi2 cuts, nucleon cuts, physical cuts, and reaction monitoring
 * cuts.
 * @details These settings are used in the main code to control the application of various cuts during the analysis. The settings include master switches for applying cuts and specific types
 * of cuts, as well as options for custom naming of cuts. The constructor initializes the settings with default values, which can be modified as needed for different runs or analyses.
 * @note The apply_cuts setting is a master switch for applying any cuts, while the other settings control specific types of cuts. It is important to set these settings appropriately based
 * on the goals of the analysis and the characteristics of the data being analyzed.
 */
struct AnalysisCutSettings {
    // Cuts setup
    /* Settings that allow to disable/enable every cut individually */

    // ==================================================================================================================================================================
    // Cut declarations
    // ==================================================================================================================================================================

    // Analysis cuts
    // ==================================================================================================================================================================

    // Top-level cut switches -------------------------------------------------------------------------------------------------------------------------------------------

    bool apply_cuts;                  // Master ON/OFF switch for applying cuts
    bool clas12ana_particles;         // TODO: move from here!
    bool only_preselection_cuts;      // keep as false for regular runs!
    bool only_electron_quality_cuts;  // keep as false for regular runs!

    // Preselection cuts (event cuts) -----------------------------------------------------------------------------------------------------------------------------------

    bool apply_preselection_cuts;        // Master ON/OFF switch for preselection cuts
    bool apply_Vz_e_cuts;                // Electron vertex cuts
    bool apply_Vz_cuts, apply_dVz_cuts;  // Vertex cuts
    bool apply_DC_e_fiducial_cuts;       // Electron DC fiducial (edge) cuts
    bool apply_DC_fiducial_cuts;         // DC fiducial (edge) cuts

    // Electron quality cuts --------------------------------------------------------------------------------------------------------------------------------------------

    bool apply_electron_quality_cuts;  // Master ON/OFF switch for eQC
    bool apply_Nphe_cut;               // Number of photo-electrons in HTCC cut
    bool apply_ECAL_SF_cuts;           // SF cut on both E_deb AND P_e
    bool apply_ECAL_P_cuts;            // SF cut on P_e (keep as false for now!)
    bool apply_ECAL_fiducial_cuts;     // ECAL edge cuts for other charged particles
    bool apply_Electron_beta_cut;      // Electron beta cut

    // Chi2 cuts (= PID cuts) -------------------------------------------------------------------------------------------------------------------------------------------

    bool apply_chi2_cuts_1e_cut;

    // Analysis cuts
    // ==================================================================================================================================================================

    // Top-level nucleon cuts switch -----------------------------------------------------------------------------------------------------------------------------------

    bool apply_nucleon_cuts;  // Master ON/OFF switch for all nucleon cuts

    // Nucleon PID cuts ------------------------------------------------------------------------------------------------------------------------------------------------

    bool apply_nucleon_PID_cuts;   // Master ON/OFF switch for nucleon PID cuts
    bool apply_CDEdgeCuts;         // CD edge cuts for charged hadrons
    bool apply_CDRegionCuts;       // CD region cuts for charged hadrons
    bool apply_ProtonPIDCuts;      // PID of CD protons handled not by chi2pid (CLAS) but our own
    bool apply_GhostTrackCuts;     // Ghost track cuts in CD (for protons)
    bool apply_charged_ECAL_veto;  // Charged particle veto in ECAL (for FD neutrons)
    bool apply_neutral_ECAL_veto;  // Neutral particle veto in ECAL (for FD neutrons)

    // Nucleon physical cuts -------------------------------------------------------------------------------------------------------------------------------------------

    // TODO: automate adding upper mom. th. to nucleon cuts (for nRes calc)
    bool apply_nucleon_physical_cuts;  // nucleon physical cuts master
    bool apply_nBeta_fit_cuts;         // apply neutron upper mom. th.
    bool apply_fiducial_cuts;
    bool apply_kinematical_cuts;
    bool apply_kinematical_weights;
    bool apply_nucleon_SmearAndCorr;

    // Reaction monitoring cuts ----------------------------------------------------------------------------------------------------------------------------------------

    bool apply_reaction_monitoring_cuts;  // Master ON/OFF switch for reaction monitoring cuts
    bool apply_P_miss_in_QE_range_cuts;
    bool apply_E_miss_in_QE_range_cuts;
    bool apply_P_miss_in_MECandSRC_range_cuts;
    bool apply_E_miss_in_MECandSRC_range_cuts;
    bool apply_xB_in_QE_range_cuts;
    bool apply_theta_q_pCD_in_lower_FSI_range_cut;

    const bool custom_cuts_naming;  // Enable custom cuts naming

    // ==================================================================================================================================================================
    // Constructor
    // ==================================================================================================================================================================

    AnalysisCutSettings()
        :  // Analysis cuts
           // ===========================================================================================================================================================

          // Top-level cut switches -------------------------------------------------------------------------------------------------------------------------------------

          apply_cuts(true),
          clas12ana_particles(true),
          only_preselection_cuts(false),
          only_electron_quality_cuts(false),

          // Preselection cuts (event cuts) -----------------------------------------------------------------------------------------------------------------------------

          apply_preselection_cuts(true),
          apply_Vz_e_cuts(true),
          apply_Vz_cuts(true),
          apply_dVz_cuts(true),
          apply_DC_e_fiducial_cuts(true),
          apply_DC_fiducial_cuts(true),

          // Electron quality cuts --------------------------------------------------------------------------------------------------------------------------------------

          apply_electron_quality_cuts(true),
          apply_Nphe_cut(true),
          apply_ECAL_SF_cuts(true),
          apply_ECAL_P_cuts(true),
          apply_ECAL_fiducial_cuts(true),
          apply_Electron_beta_cut(true),

          // Chi2 cuts (= PID cuts) -------------------------------------------------------------------------------------------------------------------------------------

          apply_chi2_cuts_1e_cut(true),

          // Analysis cuts
          // ============================================================================================================================================================

          // Top-level nucleon cuts switch ------------------------------------------------------------------------------------------------------------------------------

          apply_nucleon_cuts(true),

          // Nucleon PID cuts -------------------------------------------------------------------------------------------------------------------------------------------

          apply_nucleon_PID_cuts(true),
          apply_CDEdgeCuts(true),
          apply_CDRegionCuts(true),
          apply_ProtonPIDCuts(true),
          apply_GhostTrackCuts(true),
          apply_charged_ECAL_veto(true),
          apply_neutral_ECAL_veto(false),

          // Nucleon physical cuts --------------------------------------------------------------------------------------------------------------------------------------

          apply_nucleon_physical_cuts(true),
          apply_nBeta_fit_cuts(false),
          apply_fiducial_cuts(false),
          apply_kinematical_cuts(false),
          apply_kinematical_weights(false),
          apply_nucleon_SmearAndCorr(false),

          // Reaction monitoring cuts -----------------------------------------------------------------------------------------------------------------------------------

          apply_reaction_monitoring_cuts(false),
          apply_P_miss_in_QE_range_cuts(false),
          apply_E_miss_in_QE_range_cuts(false),
          apply_P_miss_in_MECandSRC_range_cuts(false),
          apply_E_miss_in_MECandSRC_range_cuts(false),
          apply_xB_in_QE_range_cuts(false),
          apply_theta_q_pCD_in_lower_FSI_range_cut(false),

          custom_cuts_naming(true) {}

    // ==================================================================================================================================================================
    // Functions
    // ==================================================================================================================================================================

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

        if (!apply_nucleon_cuts) {
            AMapsSettings.Generate_WMaps = false;  // WMaps should not be generated if there are no nucleon cuts, since they are used for efficiency calculations with nucleon cuts!
            apply_nucleon_PID_cuts = apply_nucleon_physical_cuts = false;
        }

        if (!apply_nucleon_PID_cuts) { apply_CDEdgeCuts = apply_CDRegionCuts = apply_ProtonPIDCuts = apply_GhostTrackCuts = apply_charged_ECAL_veto = apply_neutral_ECAL_veto = false; }

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
            ESSettings.calculate_truth_level = AMapsSettings.Generate_WMaps = MomResSettings.plot_and_fit_MomRes = MomResSettings.momRes_test = false;
        }

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
            bt::ExitWithError(__func__, __FILE__, __LINE__,
                              "momRes order error! Check the settings for Calculate_momResS2, Run_with_momResS2 and VaryingDelta in MomentumResolutionSettings!");
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

    void SetClas12anaCuts(clas12ana& clasAna, CutValueManager& CutManager, const RunParameters& parameters, const EventSelectionSettings& ESSettings) {
        if (apply_cuts) {
            // Cuts on electrons only:
            if (apply_ECAL_SF_cuts) {
                clasAna.readEcalSFPar((pd::PathDefinitions.ParameterCutFilesDirectory + "paramsSF_40Ca_x2.dat").c_str());
                CutManager.SF_cuts = DSCuts("SF", "FD", "Electron", "1e cut", 0.24865, clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut());

                clasAna.setEcalSFCuts();  // making f_ecalSFCuts = true
            } else {
                clasAna.setEcalSFCuts(false);  // making f_ecalSFCuts = false
            }

            if (apply_ECAL_P_cuts) {
                clasAna.readEcalPPar((pd::PathDefinitions.ParameterCutFilesDirectory + "paramsPI_40Ca_x2.dat").c_str());

                clasAna.setEcalPCuts();  // making f_ecalSFCuts = true
            } else {
                clasAna.setEcalPCuts(false);  // making f_ecalSFCuts = false
            }

            if (apply_ECAL_fiducial_cuts) {
                CutManager.PCAL_edge_cuts = DSCuts("PCAL edge", "FD", "Electron", "1e cut", 0, clasAna.getEcalEdgeCuts());
                clasAna.setEcalEdgeCuts();  // making f_ecalEdgeCuts = true (ECAL fiducial cuts)
            } else {
                clasAna.setEcalEdgeCuts(false);  // making f_ecalEdgeCuts = false (ECAL fiducial cuts)
            }

            if (apply_CDEdgeCuts) {
                clasAna.setCDEdgeCuts();  // making f_CDEdgeCuts = true (ECAL fiducial cuts)
            } else {
                clasAna.setCDEdgeCuts(false);  // making f_CDEdgeCuts = false (ECAL fiducial cuts)
            }

            if (apply_CDRegionCuts) {
                clasAna.setCDRegionCuts();  // making f_CDRegionCuts = true (ECAL fiducial cuts)
            } else {
                clasAna.setCDRegionCuts(false);  // making f_CDRegionCuts = false (ECAL fiducial cuts)
            }

            if (apply_ProtonPIDCuts) {
                clasAna.setProtonPidCuts();  // making f_ProtonPIDCuts = true (Proton PID cuts)
            } else {
                clasAna.setProtonPidCuts(false);  // making f_ProtonPIDCuts = false (Proton PID cuts)
            }

            if (apply_GhostTrackCuts) {
                clasAna.setGhostTrackCuts();  // making f_GhostTrackCuts = true (Ghost track cuts in CD)
            } else {
                clasAna.setGhostTrackCuts(false);  // making f_GhostTrackCuts = false (Ghost track cuts in CD)
            }

            if (apply_Nphe_cut) {
                CutManager.Nphe_cuts_FD = DSCuts("Nphe", "FD", "Electron", "1e cut", 0, clasAna.getNpheCuts());
                clasAna.setNpheCuts();  // making f_NpheCuts = true (HTCC cuts)
            } else {
                clasAna.setNpheCuts(false);  // making f_NpheCuts = false (HTCC cuts)
            }

            // Cuts on all charged hadrons:
            if (!apply_chi2_cuts_1e_cut) {
                clasAna.readInputParam((pd::PathDefinitions.ParameterCutFilesDirectory + "ana_no_cuts.par").c_str());

                clasAna.setPidCuts(false);  // making f_pidCuts = false (no PID cuts)
            } else if (apply_chi2_cuts_1e_cut) {
                std::cout << env::SYSTEM_COLOR << "\nLoading fitted pid cuts...\n\n" << env::RESET_COLOR << std::flush;
                clasAna.readAdditionalInputParam(
                    (pd::PathDefinitions.PIDCutsDirectory + "Fitted_PID_Cuts_-_" + parameters.SampleName + ".par").c_str());  // load sample-appropreate cuts file from CutsDirectory

                /* Overwriting PID cuts according to parameters.SampleName */
                CutManager.UpdateChargedParticleChi2Cuts(clasAna);

                clasAna.setPidCuts();  // making f_pidCuts = true
            }

            // Cuts on all charged particles:
            if (apply_Vz_e_cuts || apply_Vz_cuts) {
                if (apply_Vz_e_cuts) {
                    clasAna.set_e_VertexCuts();  // making f_e_vertexCuts = true
                } else {
                    clasAna.set_e_VertexCuts(false);  // making f_e_vertexCuts = false
                }

                if (apply_Vz_cuts) {
                    clasAna.setVertexCuts();  // making f_vertexCuts = true
                } else {
                    clasAna.setVertexCuts(false);  // making f_vertexCuts = false
                }

                clasAna.setVzcuts(CutManager.Vz_cut.GetLowerCut(), CutManager.Vz_cut.GetUpperCut());          // setting Vz cuts for all (charged?) particles
                clasAna.setVzcutsFD(CutManager.Vz_cut_FD.GetLowerCut(), CutManager.Vz_cut_FD.GetUpperCut());  // setting Vz cuts for all charged particles (FD only)
                clasAna.setVzcutsCD(CutManager.Vz_cut_CD.GetLowerCut(), CutManager.Vz_cut_CD.GetUpperCut());  // setting Vz cuts for all charged particles (CD only)
            } else {
                clasAna.set_e_VertexCuts(false);  // making f_e_vertexCuts = false
                clasAna.setVertexCuts(false);     // making f_vertexCuts = false

                clasAna.setVzcuts(-9999, 9999);    // setting Vz cuts for all (charged?) particles
                clasAna.setVzcutsFD(-9999, 9999);  // setting Vz cuts for all charged particles (FD only)
                clasAna.setVzcutsCD(-9999, 9999);  // setting Vz cuts for all charged particles (CD only)
            }

            if (apply_DC_e_fiducial_cuts || apply_DC_fiducial_cuts) {
                if (apply_DC_e_fiducial_cuts) {
                    CutManager.DC_e_edge_cuts = DSCuts("DC edge", "FD", "Electron", "1e cut", 0, clasAna.getDCEdgeCuts());
                    clasAna.set_e_DCEdgeCuts();  // making f_e_DCEdgeCuts = true (DC fiducial cuts?)
                } else {
                    clasAna.set_e_DCEdgeCuts(false);  // making f_e_DCEdgeCuts = false (DC fiducial cuts?)
                }

                if (apply_DC_fiducial_cuts) {
                    CutManager.DC_edge_cuts = DSCuts("DC edge", "FD", "Charged particles", "1e cut", 0, clasAna.getDCEdgeCuts());
                    clasAna.setDCEdgeCuts();  // making f_e_DCEdgeCuts = true (DC fiducial cuts?)
                } else {
                    clasAna.setDCEdgeCuts(false);  // making f_e_DCEdgeCuts = false (DC fiducial cuts?)
                }
            }

            if (apply_dVz_cuts) {
                clasAna.setVertexCorrCuts();                                                                                 // making f_corr_vertexCuts = true
                clasAna.setVertexCorrCutsLim(CutManager.dVz_cuts.GetLowerCut(), CutManager.dVz_cuts.GetUpperCut());          // setting dVz cuts (general)
                clasAna.setVertexCorrCutsLimFD(CutManager.dVz_cuts_FD.GetLowerCut(), CutManager.dVz_cuts_FD.GetUpperCut());  // setting dVz cuts (FD only)
                clasAna.setVertexCorrCutsLimCD(CutManager.dVz_cuts_CD.GetLowerCut(), CutManager.dVz_cuts_CD.GetUpperCut());  // setting dVz cuts (CD only)
            } else {
                clasAna.setVertexCorrCuts(false);             // making f_corr_vertexCuts = false
                clasAna.setVertexCorrCutsLim(-9999, 9999);    // setting dVz cuts (general)
                clasAna.setVertexCorrCutsLimFD(-9999, 9999);  // setting dVz cuts (FD only)
                clasAna.setVertexCorrCutsLimCD(-9999, 9999);  // setting dVz cuts (CD only)
            }

            if (!apply_nucleon_cuts) {
                /* Setting neutron momentum cut before beta fit (i.e., no cut!) */
                CutManager.n_momentum_cuts_ABF_FD_n_from_ph = DSCuts("Momentum_cuts_ECAL", "FD-ECAL", "Neutron", "", 0, CutManager.n_mom_th.GetLowerCut(), 9999);
                CutManager.n_momentum_cuts_ABF_FD_n_from_ph_apprax = DSCuts("Momentum_cuts_ECAL_apprax", "FD-ECAL_apprax", "Neutron", "", 0, CutManager.n_mom_th.GetLowerCut(), 9999);

                /* Setting variables to log beta fit parameters into (i.e., no cut!) */
                CutManager.Beta_max_cut_ABF_FD_n_from_ph = DSCuts("Beta_cut_ECAL", "FD-ECAL", "", "nFDpCD", 1, -9999, 9999);
                CutManager.Beta_max_cut_ABF_FD_n_from_ph_apprax = DSCuts("Beta_cut_ECAL_apprax", "FD-ECAL_apprax", "", "1n", 1, -9999, 9999);
            } else {
                std::cout << env::SYSTEM_COLOR << "\n\nLoading fitted Beta cuts...\n\n" << env::RESET_COLOR << std::flush;
                clasAna.readAdditionalInputParam(
                    (pd::PathDefinitions.NucleonCutsDirectory + "Nucleon_Cuts_-_" + parameters.SampleName + ".par").c_str());  // load sample-appropreate cuts file from CutsDirectory

                /* Setting nucleon cuts - only if NOT plotting efficiency plots! */
                if (ESSettings.limless_mom_eff_plots || parameters.is2GeVSample) {
                    /* If sample is with 2GeV beam energy, no fit is needed. */
                    CutManager.n_mom_th.SetUpperCut(parameters.beamE);
                    CutManager.TL_n_mom_cuts.SetUpperCut(parameters.beamE);
                } else {
                    /* Else, load values from fit. */
                    if (apply_nBeta_fit_cuts) {
                        CutManager.n_mom_th.SetUpperCut(clasAna.getNeutronMomentumCut());
                        CutManager.TL_n_mom_cuts.SetUpperCut(clasAna.getNeutronMomentumCut());
                        CutManager.Beta_cut.SetUpperCut(clasAna.getNeutralBetaCut());  // Log values of beta fit cut (for monitoring)
                        CutManager.Beta_cut.SetMean(clasAna.getNeutralBetaCutMean());  // Log values of beta fit cut (for monitoring)
                    }
                }

                CutManager.dphi_p1_p2_2p.SetMean(clasAna.getdPhiCutMean());
                CutManager.dphi_pFD_pCD_2p.SetMean(clasAna.getdPhiCutMean());
                CutManager.dphi_pFD_pCD_pFDpCD.SetMean(clasAna.getdPhiCutMean());
            }

            clasAna.printParams();
        } else if (only_preselection_cuts || only_electron_quality_cuts) {
            // Cuts on all charged particles:
            if (only_preselection_cuts) {
                if (apply_Vz_e_cuts || apply_Vz_cuts) {
                    if (apply_Vz_e_cuts) {
                        clasAna.set_e_VertexCuts();  // making f_e_vertexCuts = true
                    } else {
                        clasAna.set_e_VertexCuts(false);  // making f_e_vertexCuts = false
                    }

                    if (apply_Vz_cuts) {
                        clasAna.setVertexCuts();  // making f_vertexCuts = true
                    } else {
                        clasAna.setVertexCuts(false);  // making f_vertexCuts = false
                    }

                    clasAna.setVzcuts(CutManager.Vz_cut.GetLowerCut(), CutManager.Vz_cut.GetUpperCut());          // setting Vz cuts for all (charged?) particles
                    clasAna.setVzcutsFD(CutManager.Vz_cut_FD.GetLowerCut(), CutManager.Vz_cut_FD.GetUpperCut());  // setting Vz cuts for all charged particles (FD only)
                    clasAna.setVzcutsCD(CutManager.Vz_cut_CD.GetLowerCut(), CutManager.Vz_cut_CD.GetUpperCut());  // setting Vz cuts for all charged particles (CD only)
                } else {
                    clasAna.set_e_VertexCuts(false);  // making f_e_vertexCuts = false
                    clasAna.setVertexCuts(false);     // making f_vertexCuts = false

                    clasAna.setVzcuts(-9999, 9999);    // setting Vz cuts for all (charged?) particles
                    clasAna.setVzcutsFD(-9999, 9999);  // setting Vz cuts for all charged particles (FD only)
                    clasAna.setVzcutsCD(-9999, 9999);  // setting Vz cuts for all charged particles (CD only)
                }

                if (apply_DC_e_fiducial_cuts || apply_DC_fiducial_cuts) {
                    if (apply_DC_e_fiducial_cuts) {
                        CutManager.DC_e_edge_cuts = DSCuts("DC edge", "FD", "Electron", "1e cut", 0, clasAna.getDCEdgeCuts());
                        clasAna.set_e_DCEdgeCuts();
                    } else {
                        clasAna.set_e_DCEdgeCuts(false);  // making f_e_DCEdgeCuts = true (DC fiducial cuts?)
                    }

                    if (apply_DC_fiducial_cuts) {
                        CutManager.DC_edge_cuts = DSCuts("DC edge", "FD", "Charged particles", "1e cut", 0, clasAna.getDCEdgeCuts());
                        clasAna.setDCEdgeCuts();
                    } else {
                        clasAna.setDCEdgeCuts(false);  // making f_e_DCEdgeCuts = true (DC fiducial cuts?)
                    }
                }

                if (apply_dVz_cuts) {
                    clasAna.setVertexCorrCuts();                                                                                 // making f_corr_vertexCuts = true
                    clasAna.setVertexCorrCutsLim(CutManager.dVz_cuts.GetLowerCut(), CutManager.dVz_cuts.GetUpperCut());          // setting dVz cuts (general)
                    clasAna.setVertexCorrCutsLimFD(CutManager.dVz_cuts_FD.GetLowerCut(), CutManager.dVz_cuts_FD.GetUpperCut());  // setting dVz cuts (FD only)
                    clasAna.setVertexCorrCutsLimCD(CutManager.dVz_cuts_CD.GetLowerCut(), CutManager.dVz_cuts_CD.GetUpperCut());  // setting dVz cuts (CD only)
                } else {
                    clasAna.setVertexCorrCuts(false);             // making f_corr_vertexCuts = false
                    clasAna.setVertexCorrCutsLim(-9999, 9999);    // setting dVz cuts (general)
                    clasAna.setVertexCorrCutsLimFD(-9999, 9999);  // setting dVz cuts (FD only)
                    clasAna.setVertexCorrCutsLimCD(-9999, 9999);  // setting dVz cuts (CD only)
                }
            }

            // Cuts on electrons only:
            if (only_electron_quality_cuts) {
                if (apply_ECAL_SF_cuts) {
                    clasAna.readEcalSFPar((pd::PathDefinitions.ParameterCutFilesDirectory + "paramsSF_40Ca_x2.dat").c_str());
                    CutManager.SF_cuts = DSCuts("SF", "FD", "Electron", "1e cut", 0.24865, clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut());

                    clasAna.setEcalSFCuts();  // making f_ecalSFCuts = true
                } else {
                    clasAna.setEcalSFCuts(false);  // making f_ecalSFCuts = false
                }

                if (apply_ECAL_P_cuts) {
                    clasAna.readEcalPPar((pd::PathDefinitions.ParameterCutFilesDirectory + "paramsPI_40Ca_x2.dat").c_str());

                    clasAna.setEcalPCuts();  // making f_ecalSFCuts = true
                } else {
                    clasAna.setEcalPCuts(false);  // making f_ecalSFCuts = false
                }

                if (apply_ECAL_fiducial_cuts) {
                    CutManager.PCAL_edge_cuts = DSCuts("PCAL edge", "FD", "Electron", "1e cut", 0, clasAna.getEcalEdgeCuts());
                    clasAna.setEcalEdgeCuts();  // making f_ecalEdgeCuts = true (ECAL fiducial cuts)
                } else {
                    clasAna.setEcalEdgeCuts(false);  // making f_ecalEdgeCuts = false (ECAL fiducial cuts)
                }

                if (apply_CDEdgeCuts) {
                    clasAna.setCDEdgeCuts();  // making f_CDEdgeCuts = true (ECAL fiducial cuts)
                } else {
                    clasAna.setCDEdgeCuts(false);  // making f_CDEdgeCuts = false (ECAL fiducial cuts)
                }

                if (apply_CDRegionCuts) {
                    clasAna.setCDRegionCuts();  // making f_CDRegionCuts = true (ECAL fiducial cuts)
                } else {
                    clasAna.setCDRegionCuts(false);  // making f_CDRegionCuts = false (ECAL fiducial cuts)
                }

                if (apply_ProtonPIDCuts) {
                    clasAna.setProtonPidCuts();  // making f_ProtonPIDCuts = true (Proton PID cuts)
                } else {
                    clasAna.setProtonPidCuts(false);  // making f_ProtonPIDCuts = false (Proton PID cuts)
                }

                if (apply_GhostTrackCuts) {
                    clasAna.setGhostTrackCuts();  // making f_GhostTrackCuts = true (Ghost track cuts in CD)
                } else {
                    clasAna.setGhostTrackCuts(false);  // making f_GhostTrackCuts = false (Ghost track cuts in CD)
                }

                if (apply_Nphe_cut) {
                    CutManager.Nphe_cuts_FD = DSCuts("Nphe", "FD", "Electron", "1e cut", 0, clasAna.getNpheCuts());
                    clasAna.setNpheCuts();  // making f_NpheCuts = true (HTCC cuts)
                } else {
                    clasAna.setNpheCuts(false);  // making f_NpheCuts = false (HTCC cuts)
                }
            }
        }
    }
};

#endif  // ANALYSISCUTSETTINGS_H
