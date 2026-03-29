//
// Created by Alon Sportes on 06/03/2026.
//

#ifndef RECOANALYZERPRINTER_H
#define RECOANALYZERPRINTER_H

#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Include io:
#include "io/reco_cli.h"

// Include setup:
#include "../setup/codeSetup.h"

// Include Settings:
#include "../../AnalysisTools/EventCounters.h"
#include "../../Settings/AcceptanceMapsSettings.h"
#include "../../Settings/AnalysisCutSettings.h"
#include "../../Settings/CutValueManager.h"
#include "../../Settings/EventSelectionSettings.h"
#include "../../Settings/HistogramSettings/HistogramSettings.h"
#include "../../Settings/MomentumResolutionSettings.h"
#include "../../Settings/RunParameters.h"

// Include AnalysisTools:
#include "../../AnalysisTools/DSCuts.h"
#include "../../AnalysisTools/MomentumResolution.h"
#include "../../AnalysisTools/ParticleID.h"
#include "../../AnalysisTools/clas12ana.h"

namespace env = environment;
namespace pd = path_definitions;
namespace bt = basic_tools;

class RecoAnalyzerPrinter {
   protected:
    // ==================================================================================================================================================================
    // Destructor
    // ==================================================================================================================================================================

    virtual ~RecoAnalyzerPrinter() = default;

    // ==================================================================================================================================================================
    // Protected members
    // ==================================================================================================================================================================

    ofstream EventPrint;

    // ==================================================================================================================================================================
    // Protected methods
    // ==================================================================================================================================================================

    // RecoCutsPrintOut function ----------------------------------------------------------------------------------------------------------------------------------------

    void RecoCutsPrintOut(const std::string& workingDirectory, const std::string& run_plots_path, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                          const std::string& InputHipoFiles, const std::string& file_name, const RunParameters& parameters, const AnalysisCutSettings& CutSettings) const;

    // InitEventPrint function ------------------------------------------------------------------------------------------------------------------------------------------

    void InitEventPrint(bool PrintEvents, const AnalysisCutSettings& CutSettings, const std::string& run_plots_path, int Ne_in_event, int Nf_in_event, int nEvents2print);

    // PrintEventToFile function ----------------------------------------------------------------------------------------------------------------------------------------

    void PrintEventToFile(std::ofstream& EventPrint, bool PrintEvents, int Ne, int Ne_in_event, int Nf, int Nf_in_event, int nEvents2print, int event, std::size_t protons_size,
                          std::size_t Kplus_size, std::size_t Kminus_size, std::size_t piplus_size, std::size_t piminus_size, std::size_t electrons_size, std::size_t deuterons_size,
                          std::size_t neutrals_size, std::size_t otherpart_size) const;

    // SaveFittedPIDCutsToParFile function ------------------------------------------------------------------------------------------------------------------------------

    void SaveFittedPIDCutsToParFile(const RunParameters& parameters, const AnalysisCutSettings& CutSettings, const CutValueManager& CutManager, const std::string& run_plots_path) const;

    // SaveNucleonCutsToParFile function --------------------------------------------------------------------------------------------------------------------------------

    void SaveNucleonCutsToParFile(const RunParameters& parameters, const AnalysisCutSettings& CutSettings, const CutValueManager& CutManager, const std::string& run_plots_path,
                                  const std::string& nucleon_cuts_status, const std::string& fd_photons_status, const std::string& efficiency_status) const;

    // SaveSetupToLogFile function --------------------------------------------------------------------------------------------------------------------------------------

    void SaveSetupToLogFile(const std::string& run_plots_log_save_Directory, const std::string& run_plots_path, const RunParameters& parameters, const EventCounters& event_counters,
                            int num_of_good_hipo_files, int total_num_of_hipo_files, std::size_t skipped_hipo_chain_files_size, const EventSelectionSettings& ESSettings,
                            const AcceptanceMapsSettings& AMapsSettings, const MomentumResolutionSettings& MomResSettings, const AnalysisCutSettings& CutSettings,
                            const CutValueManager& CutManager, const reco_cli::RecoCliOptions& cli, const ParticleID& pid, const clas12ana& clasAna,
                            const MomentumResolution& nRes, double Pv, double Pvx, double Pvy, double Pvz, bool Skip_prob, bool TestRun, const HistogramSettings& histogram_settings,
                            bool delete_png_files, bool delete_root_files, bool Clear_Old_Directories) const;

    // PrintExecutionSummary function -----------------------------------------------------------------------------------------------------------------------------------

    void PrintExecutionSummary(const EventCounters& event_counters, const EventSelectionSettings& ESSettings, const AnalysisCutSettings& CutSettings, const ParticleID& pid,
                               const std::string& run_plots_path, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles,
                               const RunParameters& parameters, const std::string& settings_mode) const;
};

#endif  // RECOANALYZERPRINTER_H
