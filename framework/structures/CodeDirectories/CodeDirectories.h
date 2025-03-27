//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef CODEDIRECTORIES_H
#define CODEDIRECTORIES_H

#include <string>

#include "../../namespaces/general_utilities/basic_tools/basic_tools.h"

struct CodeDirectories {
    const std::string WorkingDirectory;
    const std::string plots_path;
    const std::string SettingsDirectory;
    const std::string SourceDirectory;
    const std::string DataDirectory;
    const std::string ACorrDirectory;
    const std::string AcceptanceMapsDirectory;
    const std::string AcceptanceWeightsDirectory;
    const std::string MomentumResolutionDirectory;
    const std::string NucleonCutsDirectory;
    const std::string PIDCutsDirectory;

    CodeDirectories()
        : WorkingDirectory(basic_tools::GetCurrentDirectory() + "/"),
          plots_path("/lustre24/expphy/volatile/clas12/asportes/Analysis_output"),
          SettingsDirectory(WorkingDirectory + "setup/"),
          SourceDirectory(WorkingDirectory + "source/"),
          DataDirectory(SourceDirectory + "data/"),
          ACorrDirectory(DataDirectory + "AcceptanceCorrections/"),
          AcceptanceMapsDirectory(DataDirectory + "AcceptanceMaps/"),
          AcceptanceWeightsDirectory(DataDirectory + "AcceptanceWeights/"),
          MomentumResolutionDirectory(DataDirectory + "NeutronResolution/"),
          NucleonCutsDirectory(DataDirectory + "NucleonCuts/"),
          PIDCutsDirectory(DataDirectory + "PIDCuts/") {}
};  // struct CodeDirectories

#endif  // CODEDIRECTORIES_H
