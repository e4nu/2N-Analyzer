//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef PATH_DEFINITIONS_H
#define PATH_DEFINITIONS_H

#include <iostream>
#include <string>

#include "../../../structures/CodeDirectories/CodeDirectories.h"

namespace path_definitions {
const CodeDirectories& PathDefinitions = CodeDirectories();

const std::string plots_TList_FileType = "_plots.root";
const std::string plots_TFile_FileType = "_plots.pdf";
const std::string log_file_name = "Run_log.txt";
const std::string plots_log_save_Directory = PathDefinitions.plots_path + "/" + log_file_name;

// // Define immutable directory paths
// const std::string WorkingDirectory = basic_tools::GetCurrentDirectory() + "/";
// const std::string plots_path = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output";

// /* setup directory */
// const std::string SettingsDirectory = WorkingDirectory + "setup/";

// /* framework directories */
// const std::string FrameworkDirectory = WorkingDirectory + "framework/";

// /* source directories */
// const std::string SourceDirectory = WorkingDirectory + "source/";
// const std::string DataDirectory = SourceDirectory + "data/";
// const std::string ACorrDirectory = DataDirectory + "AcceptanceCorrections/";
// const std::string AcceptanceMapsDirectory = DataDirectory + "AcceptanceMaps/";
// const std::string AcceptanceWeightsDirectory = DataDirectory + "AcceptanceWeights/";
// const std::string MomentumResolutionDirectory = DataDirectory + "NeutronResolution/";
// const std::string NucleonCutsDirectory = DataDirectory + "NucleonCuts/";
// const std::string PIDCutsDirectory = DataDirectory + "PIDCuts/";

// const std::string plots_TList_FileType = "_plots.root";
// const std::string plots_TFile_FileType = "_plots.pdf";
// const std::string log_file_name = "Run_log.txt";
// const std::string plots_log_save_Directory = plots_path + "/" + log_file_name;
}  // namespace path_definitions

#endif  // PATH_DEFINITIONS_H
