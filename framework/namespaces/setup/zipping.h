//
// Created by Alon Sportes on 24/04/2025.
//

#ifndef ZIPPING_H
#define ZIPPING_H

#include <iostream>

#include "../settings/zipping_settings.h"

namespace zipping {
using namespace zipping_settings;

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
