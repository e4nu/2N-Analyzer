//
// Created by Alon Sportes on 24/04/2025.
//

#ifndef ZIPPING_H
#define ZIPPING_H

#include <iostream>

/**
 * @namespace zipping
 * @brief A namespace for zipping the output of the code runs, including functions for saving the Plots folder as a zip file and collecting all zip files from multiple samples into one
 * directory.
 * @details This namespace contains functions for zipping the output of the code runs, such as SavePlotsToZipFile, which saves the Plots folder of a single sample as a zip file, and
 * CollectZipFiles, which collects all zip files from multiple samples into one directory. The zipping process is done using the system's zip command, and the zip files are saved in the same
 * directory as the Plots folder, with the same name as the Plots folder (with .zip extension). The settings related to zipping are defined in the zipping_settings namespace, which can be
 * used to control whether to save the Plots folder as a zip file and whether to print out the zipping process in the terminal.
 * @note The zipping process can take some time, especially if the Plots folder is large, so it is recommended to set Save_Plots_folder_to_zip_files to true only if you want to save the
 * Plots folder as a zip file. Additionally, the zipping_print_out setting can be set to true to print out the zipping process in the terminal, which can be useful for debugging or
 * monitoring the progress of the zipping process. The CollectZipFiles function can be used to collect all zip files from multiple samples into one directory, which can be useful for
 * organizing the output of multiple runs or analyses.
 */
namespace zipping {
// ======================================================================================================================================================================
// Zipping settings
// ======================================================================================================================================================================

// TODO: move zipping settings from here!

// General zipping of a sample's output ---------------------------------------------------------------------------------------------------------------------------------

// set to true to save the entire Plots folder of each sample as a zip file (saved in the same directory as the Plots folder, with the same name as the Plots folder)
const bool Save_Plots_folder_to_zip_files = true;

// set to true to print out the zipping process in the terminal (only if Save_Plots_folder_to_zip_files is true, otherwise it will be ignored)
const bool zipping_print_out = (Save_Plots_folder_to_zip_files && false);

// General zipping of all samples in the code run -----------------------------------------------------------------------------------------------------------------------

const bool Collect_all_zip_files = (Save_Plots_folder_to_zip_files && false);

// ======================================================================================================================================================================
// Zipping functions
// ======================================================================================================================================================================

// SavePlotsToZipFile function ------------------------------------------------------------------------------------------------------------------------------------------

// This function saves the output plots of a single sample into a zip file
void SavePlotsToZipFile(const bool& Save_Plots_folder_to_zip_files, const bool& zipping_print_out, const std::string& directory_path, const std::string& base_filename) {
    bool PrintOut = false;

    if (Save_Plots_folder_to_zip_files) {
        if (PrintOut) { std::cout << "\n\n\033[33mSaving plots folder to zip file...\n\033[0m"; }

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

        // std::cout << "\n\n";
    } else {
        std::cout << "\n\n\033[33mSaving plots folder to zip file is disabled...\n\033[0m";
        return;
    }
}

// CollectZipFiles function ---------------------------------------------------------------------------------------------------------------------------------------------

void CollectZipFiles(const std::string& all_collected_zips_fileName = "") { system(("source ./framework/scripts/zip_file_maker/zip_file_maker.csh " + all_collected_zips_fileName).c_str()); }

}  // namespace zipping

#endif  // ZIPPING_H
