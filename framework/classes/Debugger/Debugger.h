// #ifndef DEBUGGER_H
// #define DEBUGGER_H
#ifdef DEBUGGER_H

    #include <iostream>

    // Include libraries:
    #include "../../namespaces/general_utilities/utilities.h"

    // Include classes:
    #include "../DSCuts/DSCuts.h"
    #include "../ParticleID/ParticleID.cpp"

namespace env = environment;

/**
 * @class Debugger
 * @brief A class for performing safety checks and debugging during the analysis, including checks for particle selection, acceptance maps, and event selection.
 * @details The Debugger class contains functions for performing safety checks and debugging during the analysis. These functions include checks for particle selection (e.g., checking the
 * number of particles selected, checking the properties of selected particles), checks for acceptance maps (e.g., checking whether particles are within the acceptance maps), and checks for
 * event selection (e.g., checking the properties of events that pass certain selection criteria). The Debugger class is used in the main code for performing these checks and ensuring that
 * the analysis is proceeding as expected. The functions in the Debugger class are designed to be flexible and adaptable to different analysis configurations, allowing for checks to be
 * performed at different stages of the analysis and for different types of particles and events. It is important to use the Debugger class appropriately based on the goals of the analysis
 * and the characteristics of the data being analyzed, as well as to ensure that the checks being performed are relevant and informative for the analysis being conducted.
 * @note The Debugger class is intended to be used for safety checks and debugging during the analysis, and should not be used for general-purpose functions or calculations that are not
 * related to debugging. It is important to use the Debugger class appropriately based on the goals of the analysis and the characteristics of the data being analyzed, as well as to ensure
 * that the checks being performed are relevant and informative for the analysis being conducted. Additionally, it is important to avoid using the Debugger class for functions or
 * calculations that are not related to debugging, as this can lead to confusion and make it more difficult to maintain and understand the code. The Debugger class should be used as a tool
 * for ensuring the integrity of the analysis and for identifying potential issues or inconsistencies in the data and the analysis process.
 */
class Debugger {
   private:
    int StepTester = 1;

   public:
    // default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    Debugger() = default;

    // SafetyCheck_clas12ana_particles function
    // ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_clas12ana_particles(const char* FILE, const int LINE, const bool& clas12ana_particles, std::vector<clas12::region_part_ptr>& allParticles, const int Nf);

    // SafetyCheck_FD_protons function ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_FD_protons(const char* FILE, const int LINE, const std::vector<int>& Protons_ind, std::vector<clas12::region_part_ptr>& protons, const DSCuts& pFD_mom_th);

    // SafetyCheck_CD_protons function ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_CD_protons(const char* FILE, const int LINE, const std::vector<int>& Protons_ind, std::vector<clas12::region_part_ptr>& protons, const DSCuts& pCD_mom_th);

    // SafetyCheck_leading_FD_neutron function
    // ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_Reco_leading_FD_neutron(const char* FILE, const int LINE, const bool& apply_nucleon_cuts, const bool& ES_by_leading_FDneutron, const int& NeutronsFD_ind_mom_max,
                                             std::vector<clas12::region_part_ptr>& allParticles, std::vector<int>& NeutronsFD_ind, ParticleID& pid, clas12::region_part_ptr electron,
                                             const double& starttime);

    void SafetyCheck_Truth_leading_FD_neutron(const char* FILE, const int LINE, const bool& ES_by_leading_FDneutron, const int& TL_IDed_Leading_nFD_ind,
                                              const double& TL_IDed_Leading_nFD_momentum, const double& Leading_TL_FDNeutron_Momentum, std::vector<int> TL_NeutronsFD_mom_ind,
                                              mcpar_ptr mcpbank);

    // SafetyCheck_FD_neutron function ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_FD_neutron(const char* FILE, const int LINE, const bool& apply_nucleon_cuts, std::vector<clas12::region_part_ptr>& allParticles, const DSCuts& n_mom_th,
                                std::vector<int>& NeutronsFD_ind, ParticleID& pid, clas12::region_part_ptr electron, const double& starttime);

    // SafetyCheck_AMaps function ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_AMaps_Truth_electrons(const char* FILE, const int LINE, const int& particlePDGtmp, const bool& inFD_AMaps);

    void SafetyCheck_AMaps_Truth_protons(const char* FILE, const int LINE, const int& particlePDGtmp, const bool& inFD_AMaps);

    void SafetyCheck_AMaps_Truth_neutrons(const char* FILE, const int LINE, const int& particlePDGtmp, const bool& inFD_AMaps);

    void SafetyCheck_AMaps_Reco_leading_neutrons(const char* FILE, const int LINE, std::vector<clas12::region_part_ptr>& allParticles, const int& NeutronsFD_ind_mom_max,
                                                 const bool& hitPCAL_1e_cut, const bool& hitECIN_1e_cut, const bool& hitECOUT_1e_cut);

    // SafetyCheck_one_good_electron function ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_one_good_electron(const char* FILE, const int LINE, std::vector<clas12::region_part_ptr>& electrons);

    // SafetyCheck_1e_cut_electron function ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_1e_cut_electron(const char* FILE, const int LINE, std::vector<clas12::region_part_ptr>& electrons, const std::vector<int>& Electron_ind);

    // SafetyCheck_basic_event_selection function
    // ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_basic_event_selection(const char* FILE, const int LINE, const std::string FinaleState, std::vector<clas12::region_part_ptr>& Kplus,
                                           std::vector<clas12::region_part_ptr>& Kminus, const std::vector<int>& Piplus_ind, const std::vector<int>& Piminus_ind,
                                           const std::vector<int>& Electron_ind, std::vector<clas12::region_part_ptr>& deuterons);

    // SafetyCheck_1p function ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_1p(const char* FILE, const int LINE, const std::vector<int>& Protons_ind, clas12::region_part_ptr& e_1p, clas12::region_part_ptr& p_1p, const bool& Enable_FD_photons,
                        std::vector<int>& PhotonsFD_ind);

    // SafetyCheck_1n function ------------------------------------------------------------------------------------------------------------------------------------------------

    void SafetyCheck_1n(const char* FILE, const int LINE, std::vector<int>& NeutronsFD_ind, clas12::region_part_ptr& e_1n, clas12::region_part_ptr& n_1n, const bool& Enable_FD_photons,
                        std::vector<int>& PhotonsFD_ind, const bool& ES_by_leading_FDneutron, ParticleID& pid, std::vector<clas12::region_part_ptr>& allParticles,
                        const int& NeutronsFD_ind_mom_max, const bool& apply_nucleon_cuts, const bool& NeutronInPCAL_1n, const bool& NeutronInECIN_1n, const bool& NeutronInECOUT_1n,
                        const int& n_detlayer_1n, const double& starttime);

    // PrintStepTester function ---------------------------------------------------------------------------------------------------------------------------------------------

    void PrintStepTester(const char* FILE, const int LINE, const bool& DebuggerMode, const bool& OnlyPrintNamedTesterSteps, const std::string& StepName = "") {
        if (DebuggerMode) {
            const std::string normalizedFile = basic_tools::NormalizeFilePathForPrint(FILE != nullptr ? FILE : "");
            if (OnlyPrintNamedTesterSteps) {
                if (StepName != "") {
                    std::cerr << env::ERROR_COLOR << "\n\nStep-by-step tester is enabled:" << std::endl;
                    std::cerr << "Test number " << StepTester << std::endl;
                    std::cerr << env::ERROR_COLOR << "File:" << env::RESET_COLOR << " " << normalizedFile << env::ERROR_COLOR << ", Line:" << env::RESET_COLOR << " " << LINE << env::ERROR_COLOR
                              << ", Step: " << env::RESET_COLOR << "" << StepName << "\n\n"
                              << env::RESET_COLOR << "" << std::endl;

                    ++StepTester;
                }
            } else {
                std::cerr << env::ERROR_COLOR << "\n\nStep-by-step tester is enabled:" << std::endl;
                std::cerr << "Test number " << StepTester << std::endl;

                if (StepName == "") {
                    std::cerr << env::ERROR_COLOR << "File:" << env::RESET_COLOR << " " << normalizedFile << env::ERROR_COLOR << ", Line:" << env::RESET_COLOR << " " << LINE << "\n\n"
                              << env::RESET_COLOR << "" << std::endl;
                } else {
                    std::cerr << env::ERROR_COLOR << "File:" << env::RESET_COLOR << " " << normalizedFile << env::ERROR_COLOR << ", Line:" << env::RESET_COLOR << " " << LINE << env::ERROR_COLOR
                              << ", Step: " << env::RESET_COLOR << "" << StepName << "\n\n"
                              << env::RESET_COLOR << "" << std::endl;
                }

                ++StepTester;
            }
        }
    }

    // PrintErrorMessage function -------------------------------------------------------------------------------------------------------------------------------------------

    void PrintErrorMessage(const std::string& ErrorMessage, const std::string& FunctionName = "") {
        const std::string normalizedFile = basic_tools::NormalizeFilePathForPrint(__FILE__);
        std::cerr << env::ERROR_COLOR << "\n\nError: " << ErrorMessage << std::endl;

        if (FunctionName != "") {
            std::cerr << "File: " << normalizedFile << ", Line: " << __LINE__ << ", Function: " << FunctionName << std::endl;
        } else {
            std::cerr << "File: " << normalizedFile << ", Line: " << __LINE__ << std::endl;
        }

        std::cerr << "Aborting." << env::RESET_COLOR << "" << std::endl, exit(1);
    }

    void PrintErrorMessage(const char* FILE, const int LINE, const std::string& ErrorMessage, const std::string& FunctionName = "") {
        const std::string normalizedFile = basic_tools::NormalizeFilePathForPrint(FILE != nullptr ? FILE : "");
        std::cerr << env::ERROR_COLOR << "\n\nError: " << ErrorMessage << std::endl;

        if (FunctionName != "") {
            std::cerr << "File: " << normalizedFile << ", Line: " << LINE << ", Function: " << FunctionName << std::endl;
        } else {
            std::cerr << "File: " << normalizedFile << ", Line: " << LINE << std::endl;
        }

        std::cerr << "Aborting." << env::RESET_COLOR << "" << std::endl, exit(1);
    }

    // PrintRunConfigurationWarnings function ----------------------------------------------------------------------------------------------------------------------------------

    void PrintRunConfigurationWarnings(const char* FunctionName, const char* FILE, const int LINE, const bool& TestRun, const bool& ApplyLimiter);

    // PrintWeightMapsCommentedOutWarning function -----------------------------------------------------------------------------------------------------------------------------

    void PrintWeightMapsCommentedOutWarning(const char* FunctionName, const char* FILE, const int LINE);
};

#endif  // DEBUGGER_H
