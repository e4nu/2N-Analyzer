//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef PATH_DEFINITIONS_H
#define PATH_DEFINITIONS_H

#include <iostream>
#include <string>

#include "../CodeDirectories.h"

/**
 * @namespace path_definitions
 * @brief A namespace for defining paths used in the analysis, including paths for input HIPO files, output plots, logs, and skipped files lists.
 * @details This namespace contains definitions for various paths used in the analysis, such as the path to the input HIPO files, the path to save the output plots, the path to save logs,
 * and the path to save lists of skipped files. These paths are defined as constants within the namespace and can be used throughout the code to ensure consistency in file management and
 * organization. The paths are defined based on a CodeDirectories struct instance called PathDefinitions, which should be defined in the
 * framework/structures/CodeDirectories/CodeDirectories.h file. The paths defined in this namespace can be used in various parts of the code for reading input files, saving output files, and
 * managing logs and skipped files lists.
 * @note It is important to ensure that the paths defined in this namespace are consistent with the directory structure of the project and with the locations where input files are stored and
 * where output files should be saved. Additionally, it is important to use these paths consistently throughout the code to avoid issues with file management and organization.
 */
namespace path_definitions {
inline CodeDirectories LoadPathDefinitions() {
#if HAS_YAML_CPP
    try {
        return Yaml_loader::LoadFromYamlFile<CodeDirectories>(std::string(CONFIG_DIR) + "/io/CodeDirectories.yaml");
    } catch (const std::exception&) {
        return Yaml_loader::LoadFromYamlFile<CodeDirectories>(std::string(CONFIG_DIR) + "/io/CodeDirectories_Default.yaml");
    }
#else
    return CodeDirectories();
#endif
}

inline const CodeDirectories PathDefinitions = LoadPathDefinitions();

const std::string HipoFilesPrefix = "*.hipo";

const std::string plots_TList_FileType = "_plots.root";
const std::string plots_TFile_FileType = "_plots.pdf";
const std::string log_file_name = "Run_log.txt";
const std::string plots_log_save_Directory = PathDefinitions.plots_path + "/" + log_file_name;

const std::string skipped_files_list_prefix = "skipped_files_list.txt";
const std::string skipped_files_list_save_Directory = PathDefinitions.plots_path + "/skipped_files_list.txt";

// const std::string custom_plots_path_prefix = "";
// const std::string custom_plots_path_prefix = "_Updated_AMaps_test_2";
const std::string custom_plots_path_prefix = PathDefinitions.custom_plots_path_prefix;
// const std::string custom_plots_path_prefix = "_Updated_AMaps_test";
// const std::string custom_plots_path_prefix = "_NoOverlappingFC_2";
// const std::string custom_plots_path_prefix = "_WithOverlappingFC_2";

}  // namespace path_definitions

#endif  // PATH_DEFINITIONS_H
