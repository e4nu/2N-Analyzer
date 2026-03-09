//
// Created by Alon Sportes on 05/04/2025.
//

// #ifndef RECOANALYZER_H
// #define RECOANALYZER_H
#ifdef RECOANALYZER_H

    #pragma region /* Includes */

    #include <fstream>
    #include <iostream>
#include <memory>
#include <sstream>

    // Include setup:
    #include "../setup/codeSetup.h"

    // Include libraries:
    #include "../framework/namespaces/fitters/fitter_functions.h"
    #include "../framework/namespaces/general_utilities/utilities.h"
    #include "../framework/namespaces/plotters/draw_and_save_functions/draw_and_save_functions.h"

    // Include settings:
    #include "../framework/structures/RecoAnalyzerSettings/AcceptanceMapsSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/AnalysisCutSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/CutValueManager.h"
    #include "../framework/structures/RecoAnalyzerSettings/EventSelectionSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/HistogramSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/MomentumResolutionSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/RunParameters.h"

    // Include classes:
    #include "../framework/classes/AMaps/AMaps.cpp"
    #include "../framework/classes/DEfficiency/DEfficiency.cpp"
    #include "../framework/classes/DSCuts/DSCuts.h"
    #include "../framework/classes/HipoChainLoader/HipoChainLoader.cpp"
    #include "../framework/classes/MomentumResolution/MomentumResolution.cpp"
    #include "../framework/classes/ParticleID/ParticleID.cpp"
    #include "../framework/classes/Settings/Settings.cpp"
    #include "../framework/classes/TLCuts/TLCuts.cpp"
    #include "../framework/classes/clas12ana/clas12ana.h"
    // #include "../framework/classes/hPlots/hPlot1D.cpp"
    // #include "../framework/classes/hPlots/hPlot2D.cpp"
    // #include "../framework/classes/hPlots/hsPlots.cpp"
    #include "RecoAnalyzerHistograms.cpp"

    // Include CLAS12 libraries:
    #include "../framework/includes/clas12_include.h"
    #pragma endregion

using namespace utilities;
using namespace draw_and_save_functions;

namespace env = environment;
namespace pd = path_definitions;
namespace db = debugging;
namespace bt = basic_tools;
namespace am = analysis_math;
namespace ap = analysis_physics;
namespace hf = histogram_functions;

/**
 * @class RecoAnalyzer
 * @brief A class for performing reconstruction-level analysis, including functions for loading data, applying cuts, filling histograms, generating acceptance maps, and calculating
 * efficiencies.
 * @details The RecoAnalyzer class is designed to perform reconstruction-level analysis on the data and simulation samples. The class includes functions for loading data from HIPO files,
 * applying various cuts based on the settings defined in the AnalysisCutSettings struct, filling histograms with the appropriate values based on the characteristics of the events being
 * analyzed, generating acceptance maps for electrons and nucleons, and calculating efficiencies based on the acceptance maps. The class also includes functions for saving and loading
 * histograms and maps, as well as for drawing and saving visualizations of the results. The RecoAnalyzer class is used in the main code for performing the reconstruction-level analysis,
 * which is a crucial step in understanding the characteristics of the events being analyzed and extracting physics results from the data.
 * @note The RecoAnalyzer class is designed to be flexible and adaptable to different samples and analysis configurations. It is important to set the parameters for the class appropriately
 * based on the characteristics of the sample being analyzed and the goals of the analysis. For example, the settings for applying cuts should be set based on the desired level of event
 * selection, while the settings for generating acceptance maps should be set based on the desired resolution and range for the maps. Additionally, the functions for filling histograms and
 * calculating efficiencies should be used consistently throughout the code to ensure that the results are accurate and meaningful for the analysis being performed. It is also important to
 * ensure that the input variables for these functions are in the correct format and that the functions are used appropriately based on the characteristics of the data being analyzed and the
 * goals of the analysis. For example, the functions for filling histograms should be used with the appropriate variables and weights based on the characteristics of the events being
 * analyzed, while the functions for calculating efficiencies should be used with the appropriate acceptance maps and event characteristics to ensure that the calculated efficiencies are
 * accurate and meaningful for the analysis being performed.
 */
class RecoAnalyzer {
   public:
    // RecoAnalyzer constructor -----------------------------------------------------------------------------------------------------------------------------------------

    RecoAnalyzer(const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles);

    // RecoAnalyzer destructor ------------------------------------------------------------------------------------------------------------------------------------------

    ~RecoAnalyzer() = default;

   private:
    struct InputProcessingData {
        std::string filePath;
        std::string fileInput;
        std::string plotsInput;
        ExperimentParameters Experiment;
        RunParameters parameters;
    };

    // RecoAnalyzer private methods -------------------------------------------------------------------------------------------------------------------------------------

    InputProcessingData InitializeInputProcessing(const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles) const;

    void InitializeEventSelectionSetup(EventSelectionSettings& ESSettings, AcceptanceMapsSettings& AMapsSettings, MomentumResolutionSettings& MomResSettings,
                                       const RunParameters& parameters) const;

    void InitializeCutSetup(AnalysisCutSettings& CutSettings, std::string& run_plots_path, std::string& run_plots_log_save_Directory,
                            std::string& run_skipped_files_list_save_Directory, Settings& settings, AcceptanceMapsSettings& AMapsSettings,
                            MomentumResolutionSettings& MomResSettings, EventSelectionSettings& ESSettings, const RunParameters& parameters,
                            const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles) const;

    Directories InitializeDirectories(const std::string& run_plots_path) const;

    void InitializeHistogramSettings(HistogramSettings& HistSettings, const EventSelectionSettings& ESSettings, const AcceptanceMapsSettings& AMapsSettings,
                                     const AnalysisCutSettings& CutSettings, const RunParameters& parameters, const CutValueManager& CutManager,
                                     const char* caller_function) const;

    void InitializeAcceptanceMaps(AcceptanceMapsSettings& AMapsSettings, const EventSelectionSettings& ESSettings, HistogramSettings& HistSettings,
                                  const RunParameters& parameters, Directories& directories, AMaps& aMaps_master, AMaps& wMaps_master,
                                  std::vector<std::vector<double>>& ElectronMomSliceLimits, std::vector<std::vector<double>>& NucleonMomSliceLimits) const;

    void InitializeAcceptanceCorrectionData(bool& save_ACorr_data, const EventSelectionSettings& ESSettings) const;

    void InitializeMomentumResolution(MomentumResolutionSettings& MomResSettings, const EventSelectionSettings& ESSettings, const AnalysisCutSettings& CutSettings,
                                      const RunParameters& parameters, const CutValueManager& CutManager, Directories& directories, MomentumResolution& nRes,
                                      MomentumResolution& pRes) const;

    void InitializeDebuggingSetup(ParticleID& pid, std::ofstream& EventPrint, std::string& EventPrint_save_Directory, Directories& directories,
                                  const AnalysisCutSettings& CutSettings, const std::string& run_plots_path) const;

    void ClearRunOutputFiles(const std::string& run_plots_path) const;

    void WriteParameterFileAndCopy(const std::string& filePath, const std::string& fileContents, const std::string& run_plots_path) const;
    
    void SaveCutsParameterFiles(const AnalysisCutSettings& CutSettings, const CutValueManager& CutManager, const RunParameters& parameters, const Settings& settings,
                                const std::string& run_plots_path) const;

    // RecoAnalyzer private members -------------------------------------------------------------------------------------------------------------------------------------

    const bool Clear_Old_Directories = true;

    const bool delete_png_files = true;
    const bool delete_root_files = true;
    const bool delete_txt_files = true;

    std::vector<TString> SkippedHipoChainFiles;
    
    RecoAnalyzerHistograms reco_histograms;
};

#endif  // RECOANALYZER_H
