//
// Created by Alon Sportes on 05/04/2025.
//

// #ifndef RECOANALYZER_H
// #define RECOANALYZER_H
#ifdef RECOANALYZER_H

    #pragma region /* Includes */

    #include <iostream>

    // Include setup:
    #include "../setup/codeSetup.h"
    #include "RecoAnalyzerPrinter.cpp"

    // Include libraries:
    #include "../framework/namespaces/fitters/fitter_functions.h"
    #include "../framework/namespaces/general_utilities/utilities.h"
    #include "../framework/namespaces/plotters/draw_and_save_functions/draw_and_save_functions.h"

    // Include structs:
    #include "../framework/structures/EventCounters/EventCounters.h"

    // Include settings:
    #include "../framework/structures/RecoAnalyzerSettings/AcceptanceMapsSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/AnalysisCutSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/CutValueManager.h"
    #include "../framework/structures/RecoAnalyzerSettings/EventSelectionSettings.h"
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
    #include "../framework/classes/hPlots/hPlot1D.cpp"
    #include "../framework/classes/hPlots/hPlot2D.cpp"
    #include "../framework/classes/hPlots/hsPlots.cpp"

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
class RecoAnalyzer : protected RecoAnalyzerPrinter {
   public:
    // ==================================================================================================================================================================
    // Constructor
    // ==================================================================================================================================================================

    RecoAnalyzer(const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles);

    // ==================================================================================================================================================================
    // Destructor
    // ==================================================================================================================================================================

    ~RecoAnalyzer();

    RecoAnalyzer(const RecoAnalyzer&) = delete;
    RecoAnalyzer& operator=(const RecoAnalyzer&) = delete;
    RecoAnalyzer(RecoAnalyzer&&) = delete;
    RecoAnalyzer& operator=(RecoAnalyzer&&) = delete;

   private:
    // ==================================================================================================================================================================
    // Private members
    // ==================================================================================================================================================================

    // General ----------------------------------------------------------------------------------------------------------------------------------------------------------

    const bool delete_png_files = true, delete_root_files = true, delete_txt_files = true;

    std::vector<TString> SkippedHipoChainFiles;

    // Setup
    // ==================================================================================================================================================================

    Settings settings;  // Settings class

    // Input processing objects:
    ExperimentParameters Experiment;
    const RunParameters parameters;

    // Settings structs:
    EventSelectionSettings ESSettings;
    AcceptanceMapsSettings AMapsSettings;
    MomentumResolutionSettings MomResSettings;
    AnalysisCutSettings CutSettings;
    CutValueManager CutManager;

    // Paths and directories:
    std::string run_plots_path = pd::PathDefinitions.plots_path;
    std::string run_plots_log_save_Directory = pd::plots_log_save_Directory;
    std::string run_skipped_files_list_save_Directory = pd::skipped_files_list_save_Directory;

    const bool Clear_Old_Directories = true;

    Directories directories;

    // Plots lists and vectors
    // ==================================================================================================================================================================

    TList* plots = nullptr;

    std::vector<TObject*> HistoList;

    std::vector<TObject*> hTheta_e_VS_Phi_e_BySliceOf_P_e_All_Int_1e_cut_FD_HistoList;
    std::vector<TObject*> hTheta_e_VS_Phi_e_BySliceOf_P_e_QEL_1e_cut_FD_HistoList;
    std::vector<TObject*> hTheta_e_VS_Phi_e_BySliceOf_P_e_MEC_1e_cut_FD_HistoList;
    std::vector<TObject*> hTheta_e_VS_Phi_e_BySliceOf_P_e_RES_1e_cut_FD_HistoList;
    std::vector<TObject*> hTheta_e_VS_Phi_e_BySliceOf_P_e_DIS_1e_cut_FD_HistoList;

    // HipoChain
    // ==================================================================================================================================================================

    /*
    For full runs - use
            Skip_prob = false
    */
    // bool Skip_prob = false;  // Set true to bypass probing and trust all expanded files.
    bool Skip_prob = false;  // Set true to bypass probing and trust all expanded files.

    int Skip_prob_limit = 1000000;

    HipoChainLoader::Options opt{};

    // TestRun & Limiter
    // ==================================================================================================================================================================

    /*
    For full runs - use
            TestRun = false
            ApplyLimiter = false
    */
    const bool TestRun = false;
    const bool ApplyLimiter = false;
    // const int Limiter = 20000000;
    // const int Limiter = 10000000;
    // const int Limiter = 5000000;
    const int Limiter = 2500000;
    // const int Limiter = 1000000;
    // const int Limiter = 500000;
    // const int Limiter = 30000;
    // const int Limiter = 10000;

    // Acceptance maps & weights
    // ==================================================================================================================================================================

    AMaps aMaps_master, wMaps_master;

    std::vector<std::vector<double>> ElectronMomSliceLimits;
    std::vector<std::vector<double>> NucleonMomSliceLimits;

    // Acceptance correction data
    // ==================================================================================================================================================================

    bool save_ACorr_data = false;

    DEfficiency eff;

    TList* ACorr_data = nullptr;

    std::string ACorr_data_Dir;
    std::string ACorr_data_listName;

    // Momentum resolution
    // ==================================================================================================================================================================

    MomentumResolution nRes{"Neutron"}, pRes{"Proton"};

    // PID
    // ==================================================================================================================================================================

    clas12ana clasAna;
    ParticleID pid;

    // TCanvas
    // ==================================================================================================================================================================

    TCanvas* MainCanvas = nullptr;

    // Beta vs. P TF1 plots
    // ==================================================================================================================================================================

    TF1* beta_neutron = nullptr;
    TF1* beta_proton = nullptr;
    TF1* beta_Kplus = nullptr;
    TF1* beta_Kminus = nullptr;
    TF1* beta_Kzero = nullptr;
    TF1* beta_piplus = nullptr;
    TF1* beta_piminus = nullptr;
    TF1* beta_pizero = nullptr;
    TF1* beta_electron = nullptr;
    TF1* beta_photon = nullptr;

    // TFiles
    // ==================================================================================================================================================================

    TFile* plots_fout = nullptr;
    TFile* ACorr_data_fout = nullptr;

    // Execution counters ---------------------------------------------------------------------------------------------------------------------------------------------

    EventCounters event_counters;

    // ==================================================================================================================================================================
    // Private methods
    // ==================================================================================================================================================================

    void RefreshSkipProb() {
        if (ApplyLimiter && Limiter <= Skip_prob_limit) { Skip_prob = true; }
    }

    void ClearRunOutputFiles(const std::string& run_plots_path) const;

    void ResetExecutionCounters();

    void InitMaps(bool& AMaps_plots, bool& WMaps_plots);

    void InitMomentumResolution();

    void ZipOutPut(const std::string& run_plots_path, const Settings& settings) const;
};

#endif  // RECOANALYZER_H
