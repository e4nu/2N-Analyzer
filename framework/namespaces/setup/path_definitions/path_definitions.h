//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef PATH_DEFINITIONS_H
#define PATH_DEFINITIONS_H

#include <iostream>
#include <string>

#include "../../../structures/CodeDirectories/CodeDirectories.cpp"
// #if CMakeBuidIncludes
// #include "../../../structures/CodeDirectories/CodeDirectories.h"
// #else
// #include "../../../structures/CodeDirectories/CodeDirectories.cpp"
// #endif

namespace path_definitions {
const CodeDirectories PathDefinitions = CodeDirectories();

const std::string plots_TList_FileType = "_plots.root";
const std::string plots_TFile_FileType = "_plots.pdf";
const std::string log_file_name = "Run_log.txt";
const std::string plots_log_save_Directory = PathDefinitions.plots_path + "/" + log_file_name;
}  // namespace path_definitions

#endif  // PATH_DEFINITIONS_H
