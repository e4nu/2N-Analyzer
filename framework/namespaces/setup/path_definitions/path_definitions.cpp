//
// Created by Alon Sportes on 27/03/2025.
//

#include "path_definitions.h"

namespace path_definitions {
const CodeDirectories PathDefinitions = CodeDirectories();

const std::string plots_TList_FileType = "_plots.root";
const std::string plots_TFile_FileType = "_plots.pdf";
const std::string log_file_name = "Run_log.txt";
const std::string plots_log_save_Directory = PathDefinitions.plots_path + "/" + log_file_name;
}  // namespace path_definitions
