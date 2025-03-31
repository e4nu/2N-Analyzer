//
// Created by Alon Sportes on 27/03/2025.
//

#include "CodeDirectories.h"

CodeDirectories::CodeDirectories() {
    WorkingDirectory = basic_tools::GetCurrentDirectory() + "/";
    plots_path = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output";
    // plots_path = WorkingDirectory + "00_plots";

    SetupDirectory = WorkingDirectory + "setup/";

    FrameworkDirectory = WorkingDirectory + "framework/";

    // SourceDirectory = WorkingDirectory + "source/";

    DataDirectory = WorkingDirectory + "data/";
    // DataDirectory = SourceDirectory + "data/";
    ACorrDirectory = DataDirectory + "AcceptanceCorrections/";

    AcceptanceMapsDirectory = DataDirectory + "AcceptanceMaps/";
    AcceptanceWeightsDirectory = DataDirectory + "AcceptanceWeights/";
    MomentumResolutionDirectory = DataDirectory + "NeutronResolution/";
    NucleonCutsDirectory = DataDirectory + "NucleonCuts/";
    PIDCutsDirectory = DataDirectory + "PIDCuts/";
}
