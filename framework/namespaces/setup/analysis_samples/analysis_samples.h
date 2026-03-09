//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef ANALYSIS_SAMPLES_H
#define ANALYSIS_SAMPLES_H

#include <iostream>

// Include libraries:
#include "../../general_utilities/utilities.h"

// Include structs:
#include "data_samples.h"
#include "simulation_samples.h"
#include "uniform_samples.h"

namespace yl = Yaml_loader;

/**
 * @namespace analysis_samples
 * @brief A namespace for defining the analysis samples, including data samples, simulation samples, and uniform samples.
 * @details This namespace contains definitions for the analysis samples being analyzed in the code, including data samples, simulation samples, and uniform samples. The samples are defined
 * in separate header files (data_samples.h, simulation_samples.h, and uniform_samples.h) and are included in this namespace. The ConfigSampleChain function is used to configure the sample
 * chain based on the specific sample being analyzed, which allows for flexibility in controlling the analysis based on the characteristics of the sample. The analysis samples defined in
 * this namespace are used in the main code to control various aspects of the analysis based on the characteristics of the samples being analyzed.
 * @note It is important to define the analysis samples appropriately based on the goals of the analysis and the characteristics of the data being analyzed. The ConfigSampleChain function
 * should be used to configure the sample chain based on the specific sample being analyzed, which allows for flexibility in controlling the analysis based on the characteristics of the
 * sample. Additionally, it is important to ensure that the paths and configurations for each sample are set correctly to avoid issues with reading input files or saving output files during
 * the analysis.
 */
namespace analysis_samples {
using namespace data_samples;
using namespace simulation_samples;
using namespace uniform_samples;

std::vector<std::string> RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v;
std::string RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles;

// TODO: get rid of:
// std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles

// file_name definition -------------------------------------------------------------------------------------------------------------------------------------------------

std::string file_name = "general_file";

// HIPO files for Q2 study ----------------------------------------------------------------------------------------------------------------------------------------------

void ConfigSampleChain() {
    // ==================================================================================================================================================================
    // Data runs
    // ==================================================================================================================================================================

    // H1 ---------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* H1 2.07052GeV - run 015634 (ifarm DATA files - H1 DATA files) */
    // Config_H1_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* H1 5.98636GeV - run 015028 (ifarm DATA files - H1 DATA files) */
    // Config_H1_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // D2 ---------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* D2 2.07052GeV - all runs (ifarm DATA files - D2 DATA files) */
    // Config_D2_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* D2 5.98636GeV - run 015443 (ifarm DATA files - D2 DATA files) */
    // Config_D2_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // C12 --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // /* C12 2.07052GeV - run 015664 (ifarm DATA files - C12 DATA files) */
    // Config_C12_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // /* C12 4.02962GeV - run 015778 (ifarm DATA files - C12 DATA files) */
    // Config_C12_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* C12x4 5.98636GeV - run 015188 (ifarm DATA files - C12x4 DATA files) */
    // Config_C12_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // Ar40 -------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* Ar40 2.07052GeV - run 015672 (ifarm DATA files - Ar40 DATA files) */
    // Config_Ar40_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* Ar40 4.02962GeV - run 015743 (ifarm DATA files - Ar40 DATA files) */
    // Config_Ar40_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* Ar40 5.98636GeV - run 015792 (ifarm DATA files - Ar40 DATA files) */
    // Config_Ar40_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // ==================================================================================================================================================================
    // Uniform simulation files
    // ==================================================================================================================================================================
    // These samples were used to generate the acceptance maps

    // Electron tester samples ------------------------------------------------------------------------------------------------------------------------------------------

    // /* Uniform electron tester samples */
    // Config_Uniform_tester_e_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // Uniform samples --------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* Uniform samples @ 2GeV */
    // Config_Uniform_1e_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_en_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_ep_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* Uniform samples @ 4GeV */
    // Config_Uniform_1e_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_en_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_ep_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* Uniform samples @ 6GeV */
    // Config_Uniform_1e_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_en_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_ep_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // ==================================================================================================================================================================
    // GENIE simulation files
    // ==================================================================================================================================================================

    // Simulation samples -----------------------------------------------------------------------------------------------------------------------------------------------

    // Corrected geometry (devGEMC)
    // ==================================================================================================================================================================

    /* C12 ; G18 & SuSa ; 2GeV - corrected geometry (no FC) */
    Config_C12_G18_2GeV_Corr1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    Config_C12_SuSa_2GeV_Corr1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    /* C12 ; G18 & SuSa ; 4GeV - corrected geometry (no FC) */
    Config_C12_G18_4GeV_Corr1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    Config_C12_SuSa_4GeV_Corr1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    /* Ar40 ; G18 & SuSa ; 2GeV - corrected geometry (no FC) */
    Config_Ar40_G18_2GeV_Corr1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    Config_Ar40_SuSa_2GeV_Corr1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    /* Ar40 ; G18 & SuSa ; 4GeV - corrected geometry (no FC) */
    Config_Ar40_G18_4GeV_Corr1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    Config_Ar40_SuSa_4GeV_Corr1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // Older samples
    // ==================================================================================================================================================================

    // /* C12 G18 2GeV */
    // Config_C12_G18_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, false);  // no
    // FC Config_C12_G18_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, true);   //
    // with FC

    // /* C12 G18 4GeV */
    // Config_C12_G18_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, false);  // no
    // FC Config_C12_G18_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, true);   //
    // with FC

    // /* C12 SuSa 2GeV */
    // Config_C12_SuSa_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, false);  // no
    // FC Config_C12_SuSa_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, true);   //
    // with FC

    // /* C12 SuSa 4GeV */
    // Config_C12_SuSa_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, false);  // no
    // FC Config_C12_SuSa_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, true);   //
    // with FC

    // ==================================================================================================================================================================
    // Old simulation files for local runs
    // ==================================================================================================================================================================

    // ==================================================================================================================================================================
    // Other ROOT files
    // ==================================================================================================================================================================
}

}  // namespace analysis_samples

#endif  // ANALYSIS_SAMPLES_H
