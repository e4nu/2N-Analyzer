
//
// Created by Alon Sportes on 03/04/2025.
//

#ifndef PRINTERS_H
#define PRINTERS_H

#include <iostream>

#include "../../structures/RecoAnalyserSettings/AcceptanceMapsSettings.h"
#include "../../structures/RecoAnalyserSettings/AnalysisCutSettings.h"
#include "../../structures/RecoAnalyserSettings/CutValueManager.h"
#include "../../structures/RecoAnalyserSettings/EventSelectionSettings.h"
#include "../../structures/RecoAnalyserSettings/MomentumResolutionSettings.h"
#include "../../structures/RecoAnalyserSettings/RunParameters.h"

namespace printers {

// RecoCutsPrintOut function --------------------------------------------------------------------------------------------------------------------------------------------

void RecoCutsPrintOut(const std::string& workingDirectory, const std::string& run_plots_path, const std::string& AnalyzeFilePath, const std::string& AnalyzeFileSample,
                      const std::string& AnalyzeFile, const std::string& file_name, const RunParameters& parameters, const AnalysisCutSettings& CutSettings) {
    /* Print out the cuts within the run (for self-observation) */

    std::cout << "\033[33m-- Execution variables ----------------------------------------------------\n";
    std::cout << "\033[33mWorkingDirectory:\033[0m\t" << workingDirectory << "\n";
    std::cout << "\033[33mrun_plots_path:\033[0m\t\t" << run_plots_path << "\n\n";

    std::cout << "\033[33mAnalyzeFilePath:\033[0m\t" << "/" << AnalyzeFilePath << "/" << "\n";
    std::cout << "\033[33mAnalyzeFileSample:\033[0m\t" << "/" << AnalyzeFileSample << "/" << "\n";
    std::cout << "\033[33mAnalyzeFile:\033[0m\t\t" << AnalyzeFile << "\n";
    std::cout << "\033[33mSettings mode:\033[0m\t\t'" << file_name << "'\n\n";

    std::cout << "\033[33mSampleName:\033[0m\t\t" << parameters.SampleName << "\n";
    std::cout << "\033[33mVaryingSampleName:\033[0m\t" << parameters.VaryingSampleName << "\n";
    std::cout << "\033[33mTarget:\033[0m\t\t\t" << parameters.Target << " (PDG: " << parameters.TargetPDG << ")\n";
    std::cout << "\033[33mBeam Energy:\033[0m\t\t" << parameters.beamE << " [GeV]\n\n\n\n";

    // Cuts output
    if (!CutSettings.apply_cuts) {
        std::cout << "\033[33mCuts are disabled:\n";
    } else {
        std::cout << "\033[33mCuts are enabled:\n";
    }

    std::cout << "\033[33mapply_cuts:\033[0m\t\t\t" << basic_tools::BoolToString(CutSettings.apply_cuts) << "\n";
    std::cout << "\033[33mclas12ana_particles:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.clas12ana_particles) << "\n";
    std::cout << "\033[33monly_preselection_cuts:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.only_preselection_cuts) << "\n";
    std::cout << "\033[33monly_electron_quality_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.only_electron_quality_cuts) << "\n\n";

    std::cout << "\033[33mapply_preselection_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_preselection_cuts) << "\n";
    std::cout << "\033[33mapply_Vz_e_cuts:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.apply_Vz_e_cuts) << "\n";
    std::cout << "\033[33mapply_Vz_cuts:\033[0m\t\t\t" << basic_tools::BoolToString(CutSettings.apply_Vz_cuts) << "\n";
    std::cout << "\033[33mapply_dVz_cuts:\033[0m\t\t\t" << basic_tools::BoolToString(CutSettings.apply_dVz_cuts) << "\n";
    std::cout << "\033[33mapply_DC_e_fiducial_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_DC_e_fiducial_cuts) << "\n";
    std::cout << "\033[33mapply_DC_fiducial_cuts:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.apply_DC_fiducial_cuts) << "\n\n";

    std::cout << "\033[33mapply_electron_quality_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_electron_quality_cuts) << "\n";
    std::cout << "\033[33mapply_Nphe_cut:\033[0m\t\t\t" << basic_tools::BoolToString(CutSettings.apply_Nphe_cut) << "\n";
    std::cout << "\033[33mapply_ECAL_SF_cuts:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.apply_ECAL_SF_cuts) << "\n";
    std::cout << "\033[33mapply_ECAL_P_cuts:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.apply_ECAL_P_cuts) << "\n";
    std::cout << "\033[33mapply_ECAL_fiducial_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_ECAL_fiducial_cuts) << "\n";
    std::cout << "\033[33mapply_Electron_beta_cut:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_Electron_beta_cut) << "\n\n";

    std::cout << "\033[33mapply_chi2_cuts_1e_cut:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.apply_chi2_cuts_1e_cut) << "\n";
    std::cout << "\033[33mapply_nucleon_cuts:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.apply_nucleon_cuts) << "\n\n";
    std::cout << "\033[33mapply_nucleon_physical_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_nucleon_physical_cuts) << "\n";
    std::cout << "\033[33mapply_nBeta_fit_cuts:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.apply_nBeta_fit_cuts) << "\n";
    std::cout << "\033[33mapply_fiducial_cuts:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.apply_fiducial_cuts) << "\n";
    std::cout << "\033[33mapply_kinematical_cuts:\033[0m\t\t" << basic_tools::BoolToString(CutSettings.apply_kinematical_cuts) << "\n";
    std::cout << "\033[33mapply_kinematical_weights:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_kinematical_weights) << "\n";
    std::cout << "\033[33mapply_nucleon_SmearAndCorr:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_nucleon_SmearAndCorr) << "\n\n";

    std::cout << "\033[33mapply_reaction_monitoring_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_reaction_monitoring_cuts) << "\n";
    std::cout << "\033[33mapply_P_miss_in_QE_range_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_P_miss_in_QE_range_cuts) << "\n";
    std::cout << "\033[33mapply_E_miss_in_QE_range_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_E_miss_in_QE_range_cuts) << "\n";
    std::cout << "\033[33mapply_P_miss_in_MECandSRC_range_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_P_miss_in_MECandSRC_range_cuts) << "\n";
    std::cout << "\033[33mapply_E_miss_in_MECandSRC_range_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_E_miss_in_MECandSRC_range_cuts) << "\n";
    std::cout << "\033[33mapply_xB_in_QE_range_cuts:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_xB_in_QE_range_cuts) << "\n\n";
    std::cout << "\033[33mapply_theta_q_pCD_in_lower_FSI_range_cut:\033[0m\t" << basic_tools::BoolToString(CutSettings.apply_theta_q_pCD_in_lower_FSI_range_cut) << "\n\n";
}

// SavePlotsToZipFile function ------------------------------------------------------------------------------------------------------------------------------------------

void SavePlotsToZipFile(const bool& Save_Plots_folder_to_zip_files, const bool& zipping_print_out, const std::string& directory_path, const std::string& base_filename) {
    bool PrintOut = false;

    if (Save_Plots_folder_to_zip_files) {
        std::cout << "\n\n\033[33mSaving plots folder to zip file...\n\033[0m";

        std::string zip_filename = base_filename + ".zip";
        std::string zip_command;

        if (zipping_print_out) {
            zip_command = "cd " + directory_path + " && zip -r " + zip_filename + " ./*";
        } else {
            zip_command = "cd " + directory_path + " && zip -rq " + zip_filename + " ./*";
        }

        system(zip_command.c_str());

        if (PrintOut) {
            std::cout << "\n\nMaking zip file...\n";
            std::cout << "zip_filename: " << zip_filename << "\n";
            std::cout << "zip_command: " << zip_command << "\n";
        }

        std::cout << "\n\n";
    } else {
        std::cout << "\n\n\033[33mSaving plots folder to zip file is disabled...\n\033[0m";
        return;
    }
}

};  // namespace printers

#endif  // PRINTERS_H
