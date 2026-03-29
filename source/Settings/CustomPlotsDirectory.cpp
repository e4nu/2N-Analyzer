//
// Created by Alon Sportes on 23/12/2023.
//

#include "CustomPlotsDirectory.h"

#include "../../include/Settings/AcceptanceMapsSettings.h"
#include "../../include/Settings/AnalysisCutSettings.h"
#include "../../include/Settings/EventSelectionSettings.h"
#include "../../include/Settings/MomentumResolutionSettings.h"

// ConfigureStatuses function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureStatuses */
void CustomPlotsDirectory::ConfigureStatuses(const AnalysisCutSettings& CutSettings, const EventSelectionSettings& ESSettings, const AcceptanceMapsSettings& AMapsSettings,
                                             const MomentumResolutionSettings& MomResSettings) {
    if (Custom_cuts_naming) {  // Status additions
        Clas12ana_particles_status = !CutSettings.clas12ana_particles ? "_no_clas12ana" : Clas12ana_particles_status;
        Preselection_Cuts_Status = CutSettings.only_preselection_cuts ? "_Preselection_Cuts" : Preselection_Cuts_Status;
        Electron_Quality_Cuts_Status = CutSettings.only_electron_quality_cuts ? "_eQC" : Electron_Quality_Cuts_Status;

        Nucleon_Cuts_Status = CutSettings.apply_nucleon_cuts ? "NC" : "noNC";
        FD_photons_Status = ESSettings.Enable_FD_photons ? (CutSettings.apply_nucleon_cuts ? "_wFDph" : "wFDph") : "";
        PSmearing_Status = !CutSettings.apply_nucleon_SmearAndCorr ? "" : "_wNSaC";
        FiducialCuts_Status = CutSettings.apply_fiducial_cuts ? (ESSettings.Calc_eff_overlapping_FC ? "_wFCwO" : "_wFCnoO") : "";
        KinCuts_Status = CutSettings.apply_kinematical_cuts ? (CutSettings.apply_kinematical_weights ? "wKC" : "_wKC") : "";
        KinWei_Status = CutSettings.apply_kinematical_weights ? "_wKW" : "";
        Efficiency_Status = !CutSettings.apply_nucleon_cuts ? "" : (ESSettings.Rec_wTL_ES ? (ESSettings.ZoomIn_On_mom_th_plots ? "_EffwZoomIn" : "_Eff") : Efficiency_Status);

        if (CutSettings.apply_chi2_cuts_1e_cut) {
            const std::string mom_res_test_suffix = MomResSettings.momRes_test ? "T" : "";

            if (MomResSettings.plot_and_fit_MomRes) {
                if (!MomResSettings.VaryingDelta && !MomResSettings.Calculate_momResS2 && !MomResSettings.Run_with_momResS2) {
                    MomRes_Status = "_momResS1" + mom_res_test_suffix;
                } else if (MomResSettings.VaryingDelta && MomResSettings.Calculate_momResS2 && !MomResSettings.Run_with_momResS2) {
                    MomRes_Status = "_momResS2" + mom_res_test_suffix;
                } else if (MomResSettings.VaryingDelta && !MomResSettings.Calculate_momResS2 && MomResSettings.Run_with_momResS2) {
                    MomRes_Status = "_momResS2R" + mom_res_test_suffix;
                }

                MomRes_Status = (MomRes_Status == "" || MomRes_Status == "T") ? ((!MomResSettings.momRes_test) ? "_momResUnknown" : "_momResUnknownT") : MomRes_Status;
            }

            AMaps_Status = (AMapsSettings.Generate_Electron_AMaps || AMapsSettings.Generate_Nucleon_AMaps) ? "_AMaps" : "";
            WMaps_Status = AMapsSettings.Generate_WMaps ? "_WMaps" : "";
        }

        if (CutSettings.apply_nucleon_cuts) { nECALv_Status = CutSettings.apply_neutral_ECAL_veto ? "_nECALv" : ""; }

        Additional_Status = Additional_Status + MomRes_Status + AMaps_Status + WMaps_Status + nECALv_Status;
    }

    Added_PreStatuses = Clas12ana_particles_status + Preselection_Cuts_Status + Electron_Quality_Cuts_Status;

    Added_Statuses = Preselection_Cuts_Status + Electron_Quality_Cuts_Status + Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status + PSmearing_Status + FiducialCuts_Status +
                     KinCuts_Status + KinWei_Status + Additional_Status;
}
#pragma endregion

// SetPaths function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetPaths function */
void CustomPlotsDirectory::SetPaths(const std::string& WorkingDirectory, const std::string& SampleName, const std::string& plots_path, const bool& apply_cuts,
                                    const bool& apply_chi2_cuts_1e_cut, const bool& apply_nucleon_cuts) {
    std::string run, Global_Run_Status_Local;

    if (Global_Run_Status == "") {
        Global_Run_Status_Local = Global_Run_Status;
    } else {
        Global_Run_Status_Local = "_" + Global_Run_Status;
    }

    if (plots_path == WorkingDirectory + "00_plots")  // If default output folder
    {
        if (!apply_cuts) {
            // Stage 0 - no cuts
            run = SampleName + Stage0_prefix + Added_PreStatuses + Global_Run_Status_Local;
            Plots_path = WorkingDirectory + run;
            Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
            Skipped_files_list_save_Directory = Plots_path + "/" + "Skipped_files_list_" + run + ".txt";
        } else {
            if (!apply_chi2_cuts_1e_cut) {
                // Stage 1 - with cuts except PID (chi2) cuts
                run = SampleName + Stage1_prefix + Added_PreStatuses + Global_Run_Status_Local;
                Plots_path = WorkingDirectory + run, Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
                Skipped_files_list_save_Directory = Plots_path + "/" + "Skipped_files_list_" + run + ".txt";
            } else if (apply_chi2_cuts_1e_cut) {
                if (!apply_nucleon_cuts) {
                    // Stage 2 - set nucleon cuts (neutron beta fit & proton double detection cuts)
                    run = SampleName + Stage2_prefix + Added_Statuses + Global_Run_Status_Local;
                    Plots_path = WorkingDirectory + run, Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
                    Skipped_files_list_save_Directory = Plots_path + "/" + "Skipped_files_list_" + run + ".txt";
                } else {
                    // Stage 3 - other runs
                    run = SampleName + Stage3_prefix + Added_Statuses + Global_Run_Status_Local;
                    Plots_path = WorkingDirectory + run, Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
                    Skipped_files_list_save_Directory = Plots_path + "/" + "Skipped_files_list_" + run + ".txt";
                }
            }
        }
    } else  // Else, custom output folder
    {
        if (!apply_cuts) {
            // Stage 0 - no cuts
            run = SampleName + Stage0_prefix + Added_PreStatuses + Global_Run_Status_Local;
            Plots_path = plots_path + "/" + run;
            Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
        } else {
            if (!apply_chi2_cuts_1e_cut) {
                // Stage 1 - with cuts except PID (chi2) cuts
                run = SampleName + Stage1_prefix + Added_PreStatuses + Global_Run_Status_Local;
                Plots_path = plots_path + "/" + run;
                Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
            } else if (apply_chi2_cuts_1e_cut) {
                if (!apply_nucleon_cuts) {
                    // Stage 2 - set nucleon cuts (neutron beta fit & proton double detection cuts)
                    run = SampleName + Stage2_prefix + Added_Statuses + Global_Run_Status_Local;
                    Plots_path = plots_path + "/" + run;
                    Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
                } else {
                    // Stage 3 - other runs
                    run = SampleName + Stage3_prefix + Added_Statuses + Global_Run_Status_Local;
                    Plots_path = plots_path + "/" + run;
                    Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
                }
            }
        }
    }

    Run_dir_name = run;
}

#pragma endregion
