//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef CODEDIRECTORIES_H
#define CODEDIRECTORIES_H

#include <iostream>
#include <string>

#include "../../namespaces/general_utilities/basic_tools.h"

struct CodeDirectories {
    /* Histogram plots output directory */
    std::string WorkingDirectory;
    std::string plots_path;

    /* Setup directory */
    std::string SetupDirectory;

    /* Framework directory */
    std::string FrameworkDirectory;

    /* Data directory and sub-directories */
    std::string DataDirectory;
    std::string ACorrDirectory;
    std::string AcceptanceMapsDirectory;
    std::string AcceptanceWeightsDirectory;
    std::string MomentumResolutionDirectory;
    std::string NucleonCutsDirectory;
    std::string PIDCutsDirectory;

    CodeDirectories() {
        WorkingDirectory = basic_tools::GetCurrentDirectory() + "/";
        plots_path = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output";
        // plots_path = WorkingDirectory + "00_plots";

        SetupDirectory = WorkingDirectory + "setup/";

        FrameworkDirectory = WorkingDirectory + "framework/";

        DataDirectory = WorkingDirectory + "data/";
        ACorrDirectory = DataDirectory + "AcceptanceCorrections/";

        AcceptanceMapsDirectory = DataDirectory + "AcceptanceMaps/";
        AcceptanceWeightsDirectory = DataDirectory + "AcceptanceWeights/";
        MomentumResolutionDirectory = DataDirectory + "NeutronResolution/";
        NucleonCutsDirectory = DataDirectory + "NucleonCuts/";
        PIDCutsDirectory = DataDirectory + "PIDCuts/";
    }
};  // struct CodeDirectories

#endif  // CODEDIRECTORIES_H
