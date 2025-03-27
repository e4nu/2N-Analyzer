//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef CODEDIRECTORIES_H
#define CODEDIRECTORIES_H

#include <string>

#include "../../namespaces/general_utilities/basic_tools/basic_tools.h"

struct CodeDirectories {
    std::string WorkingDirectory;
    std::string plots_path;
    std::string SettingsDirectory;
    std::string SourceDirectory;
    std::string DataDirectory;
    std::string ACorrDirectory;
    std::string AcceptanceMapsDirectory;
    std::string AcceptanceWeightsDirectory;
    std::string MomentumResolutionDirectory;
    std::string NucleonCutsDirectory;
    std::string PIDCutsDirectory;

    CodeDirectories() {
        WorkingDirectory(basic_tools::GetCurrentDirectory() + "/");
        plots_path("/lustre24/expphy/volatile/clas12/asportes/Analysis_output");

        SettingsDirectory(WorkingDirectory + "setup/");

        SourceDirectory(WorkingDirectory + "source/");

        DataDirectory(SourceDirectory + "data/");
        ACorrDirectory(DataDirectory + "AcceptanceCorrections/");

        AcceptanceMapsDirectory(DataDirectory + "AcceptanceMaps/");
        AcceptanceWeightsDirectory(DataDirectory + "AcceptanceWeights/");
        MomentumResolutionDirectory(DataDirectory + "NeutronResolution/");
        NucleonCutsDirectory(DataDirectory + "NucleonCuts/");
        PIDCutsDirectory(DataDirectory + "PIDCuts/");
    }
    // CodeDirectories()
    //     : WorkingDirectory(basic_tools::GetCurrentDirectory() + "/"),
    //       plots_path("/lustre24/expphy/volatile/clas12/asportes/Analysis_output"),
    //       SettingsDirectory(WorkingDirectory + "setup/"),
    //       SourceDirectory(WorkingDirectory + "source/"),
    //       DataDirectory(SourceDirectory + "data/"),
    //       ACorrDirectory(DataDirectory + "AcceptanceCorrections/"),
    //       AcceptanceMapsDirectory(DataDirectory + "AcceptanceMaps/"),
    //       AcceptanceWeightsDirectory(DataDirectory + "AcceptanceWeights/"),
    //       MomentumResolutionDirectory(DataDirectory + "NeutronResolution/"),
    //       NucleonCutsDirectory(DataDirectory + "NucleonCuts/"),
    //       PIDCutsDirectory(DataDirectory + "PIDCuts/") {}
};  // struct CodeDirectories

#endif  // CODEDIRECTORIES_H
