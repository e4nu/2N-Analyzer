//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef CODEDIRECTORIES_H
#define CODEDIRECTORIES_H

#include <iostream>
#include <string>

// Include libraries:
#include "../../libraries/general_utilities/basic_tools.h"

// Include YAML loader:
#include "../io/Yaml_loader.h"

/**
 * @struct CodeDirectories
 * @brief A struct to store the directories used in the code, including the working directory, the plots output directory, the setup directory, the framework directory, and various data
 * directories for acceptance corrections, acceptance maps, acceptance weights, momentum resolution, nucleon cuts, PID cuts, and parameter cut files.
 * @details This struct is used to store the directories used in the code, which are initialized in the constructor. The directories are constructed based on the current working directory,
 * and can be modified as needed for different runs or analyses. The CodeDirectories struct is used in the main code to access the various directories for reading and writing files, such as
 * reading input data files, writing output plots, and accessing setup files.
 * @note The directories are initialized based on the current working directory, which allows for flexibility in running the code from different locations. It is important to ensure that the
 * directories are set correctly based on the structure of the code and the location of the files being accessed. For example, the plots output directory should be set to a location where
 * the user has write permissions, and the data directories should be set to the correct locations where the input data files are stored. Additionally, the setup directory should be set to
 * the location where the setup files are stored, which are used for configuring various aspects of the analysis.
 */
struct CodeDirectories {
    /* Histogram plots output directory */
    std::string WorkingDirectory;
    std::string plots_path;

    /* Setup directory */
    std::string SetupDirectory;

    /* Framework directory */
    std::string FrameworkDirectory;

    /* data directory and sub-directories */
    std::string DataDirectory;
    std::string ACorrDirectory;
    std::string AcceptanceMapsDirectory;
    std::string AcceptanceWeightsDirectory;
    std::string MomentumResolutionDirectory;
    std::string NucleonCutsDirectory;
    std::string PIDCutsDirectory;
    std::string ParameterCutFilesDirectory;

    /* include directory and sub-directories */
    std::string IncludeDirectory;

    /* source directory and sub-directories */
    std::string SourceDirectory;

    std::string custom_plots_path_prefix;

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
        ParameterCutFilesDirectory = DataDirectory + "ParameterCutFiles/";

        SourceDirectory = WorkingDirectory + "source/";

        custom_plots_path_prefix = "";
    }
};  // struct CodeDirectories

// --------------------------------------------------------------------------------------
// YAML adapter specialization for CodeDirectories
// --------------------------------------------------------------------------------------

template <>
struct Yaml_loader::SettingsAdapter<CodeDirectories> {
    static void Apply(const YAML::Node& node, CodeDirectories& s) {
        using namespace Yaml_loader;

        TryReadKey(node, "WorkingDirectory", s.WorkingDirectory);
        TryReadKey(node, "plots_path", s.plots_path);

        TryReadKey(node, "SetupDirectory", s.SetupDirectory);
        TryReadKey(node, "FrameworkDirectory", s.FrameworkDirectory);

        TryReadKey(node, "DataDirectory", s.DataDirectory);
        TryReadKey(node, "ACorrDirectory", s.ACorrDirectory);
        TryReadKey(node, "AcceptanceMapsDirectory", s.AcceptanceMapsDirectory);
        TryReadKey(node, "AcceptanceWeightsDirectory", s.AcceptanceWeightsDirectory);
        TryReadKey(node, "MomentumResolutionDirectory", s.MomentumResolutionDirectory);
        TryReadKey(node, "NucleonCutsDirectory", s.NucleonCutsDirectory);
        TryReadKey(node, "PIDCutsDirectory", s.PIDCutsDirectory);
        TryReadKey(node, "ParameterCutFilesDirectory", s.ParameterCutFilesDirectory);

        TryReadKey(node, "IncludeDirectory", s.IncludeDirectory);
        TryReadKey(node, "SourceDirectory", s.SourceDirectory);

        TryReadKey(node, "custom_plots_path_prefix", s.custom_plots_path_prefix);
    }

    static void Validate(const CodeDirectories& s) {
        if (s.WorkingDirectory.empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "WorkingDirectory cannot be empty."); }

        if (s.DataDirectory.empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "DataDirectory cannot be empty."); }
    }
};

#endif  // CODEDIRECTORIES_H
