//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef CODEDIRECTORIES_H
#define CODEDIRECTORIES_H

#include <string>

#include "../../namespaces/general_utilities/basic_tools/basic_tools_include.h"
// #if CMakeBuidIncludes
//     #include "../../namespaces/general_utilities/basic_tools/basic_tools.h"
// #else
//     #include "../../namespaces/general_utilities/basic_tools/basic_tools.cpp"
// #endif

struct CodeDirectories {
    /* Histogram plots output directory */
    std::string WorkingDirectory;
    std::string plots_path;

    /* Setup directory */
    std::string SetupDirectory;

    /* Framework directory */
    std::string FrameworkDirectory;

    /* Data directory and sub-directories */
    // std::string SourceDirectory;
    std::string DataDirectory;
    std::string ACorrDirectory;
    std::string AcceptanceMapsDirectory;
    std::string AcceptanceWeightsDirectory;
    std::string MomentumResolutionDirectory;
    std::string NucleonCutsDirectory;
    std::string PIDCutsDirectory;

    CodeDirectories();
};

#endif  // CODEDIRECTORIES_H
