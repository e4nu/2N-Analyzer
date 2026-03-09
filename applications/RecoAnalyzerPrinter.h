//
// Created by Alon Sportes on 06/03/2026.
//

// #ifndef RECOANALYZERPRINTER_H
// #define RECOANALYZERPRINTER_H
#ifdef RECOANALYZERPRINTER_H

    #include <cstddef>
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <vector>

    // Include setup:
    #include "../setup/codeSetup.h"

    // Include structures:
    #include "../framework/structures/EventCounters/EventCounters.h"
    #include "../framework/structures/RecoAnalyzerSettings/AcceptanceMapsSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/AnalysisCutSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/CutValueManager.h"
    #include "../framework/structures/RecoAnalyzerSettings/EventSelectionSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/MomentumResolutionSettings.h"
    #include "../framework/structures/RecoAnalyzerSettings/RunParameters.h"

    // Include classes:
    #include "../framework/classes/DSCuts/DSCuts.h"
    #include "../framework/classes/MomentumResolution/MomentumResolution.cpp"
    #include "../framework/classes/ParticleID/ParticleID.cpp"
    #include "../framework/classes/clas12ana/clas12ana.h"

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
                            const CutValueManager& CutManager, const ParticleID& pid, const clas12ana& clasAna, const MomentumResolution& nRes, double Pv, double Pvx, double Pvy, double Pvz,
                            bool wider_margin, bool debug_plots, bool Log_scale_Vx_plots, bool Log_scale_Vy_plots, bool Log_scale_Vz_plots, bool Log_scale_dVx_plots,
                            bool Log_scale_dVy_plots, bool Log_scale_dVz_plots, bool Skip_prob, bool TestRun, bool Plot_selector_master, bool Cut_plots_master, bool Nphe_plots,
                            bool Chi2_plots, bool Vertex_plots, bool SF_plots, bool fiducial_plots, bool Momentum_plots, bool ReacMon_plots, bool W_plots, bool Beta_plots,
                            bool Beta_vs_P_plots, bool Angle_plots_master, bool Theta_e_plots, bool Phi_e_plots, bool Momentum_transfer_plots, bool E_e_plots, bool ETrans_plots_master,
                            bool ETrans_all_plots, bool ETrans_QEL_plots, bool ETrans_MEC_plots, bool ETrans_RES_plots, bool ETrans_DIS_plots, bool Ecal_plots, bool normalize_master,
                            bool norm_Nphe_plots, bool norm_Chi2_plots, bool norm_Vertex_plots, bool norm_SF_plots, bool norm_Fiducial_plots, bool norm_Momentum_plots,
                            bool norm_ReacMon_plots, bool norm_W_plots, bool norm_Beta_plots, bool norm_Angle_plots_master, bool norm_Momentum_transfer_plots, bool norm_E_e_plots,
                            bool norm_ET_plots, bool norm_Ecal_plots, bool norm_TKI_plots, bool norm_MomRes_plots, bool norm_Multi_plots, bool delete_png_files,
                            bool delete_root_files, bool Clear_Old_Directories) const;

    // PrintExecutionSummary function -----------------------------------------------------------------------------------------------------------------------------------

    void PrintExecutionSummary(const EventCounters& event_counters, const EventSelectionSettings& ESSettings, const AnalysisCutSettings& CutSettings, const ParticleID& pid,
                               const std::string& run_plots_path, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles,
                               const RunParameters& parameters, const std::string& settings_mode) const;
};

#endif  // RECOANALYZERPRINTER_H
