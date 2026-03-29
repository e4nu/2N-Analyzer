//
// Created by Alon Sportes on 06/03/2026.
//

#include "RecoAnalyzerPrinter.h"

// RecoCutsPrintOut function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* RecoCutsPrintOut function */
void RecoAnalyzerPrinter::RecoCutsPrintOut(const std::string& workingDirectory, const std::string& run_plots_path, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                                           const std::string& InputHipoFiles, const std::string& file_name, const RunParameters& parameters, const AnalysisCutSettings& CutSettings) const {
    /* Print out the cuts within the run (for self-observation) */
    constexpr int lineWidth = 100;
    const auto print_bool = [&](const std::string& label, const bool value) { bt::PrintAlignedValueColored(label, bt::BoolToString(value), lineWidth, 1); };

    std::cout << "\n\n";
    bt::PrintTitleBlock("Execution Variables", '-', lineWidth);
    bt::PrintAlignedValueColored("WorkingDirectory", workingDirectory, lineWidth, 1);
    bt::PrintWrappedLabelValueColored("run_plots_path", run_plots_path, lineWidth, 1);
    bt::PrintWrappedLabelValueColored("RecoSamplePath", "/" + RecoSamplePath + "/", lineWidth, 1);
    bt::PrintAlignedValueColored("ReconHipoDir", "/" + ReconHipoDir + "/", lineWidth, 1);
    bt::PrintWrappedLabelValueColored("InputHipoFiles", InputHipoFiles, lineWidth, 1);
    bt::PrintAlignedValueColored("Settings mode", "'" + file_name + "'", lineWidth, 1);
    bt::PrintAlignedValueColored("SampleName", parameters.SampleName, lineWidth, 1);
    bt::PrintAlignedValueColored("VaryingSampleName", parameters.VaryingSampleName, lineWidth, 1);
    bt::PrintAlignedValueColored("Target", parameters.Target + " (PDG: " + std::to_string(parameters.TargetPDG) + ")", lineWidth, 1);
    bt::PrintAlignedValueColored("Beam Energy", bt::ToStringWithPrecision(parameters.beamE, 3) + " [GeV]", lineWidth, 1);
    std::cout << "\n";

    bt::PrintSectionHeader("Cuts", lineWidth);
    bt::PrintAlignedValueColored("Cuts status", CutSettings.apply_cuts ? "enabled" : "disabled", lineWidth, 1);
    std::cout << "\n";

    print_bool("apply_cuts", CutSettings.apply_cuts);
    print_bool("clas12ana_particles", CutSettings.clas12ana_particles);
    print_bool("only_preselection_cuts", CutSettings.only_preselection_cuts);
    print_bool("only_electron_quality_cuts", CutSettings.only_electron_quality_cuts);
    std::cout << "\n";

    print_bool("apply_preselection_cuts", CutSettings.apply_preselection_cuts);
    print_bool("apply_Vz_e_cuts", CutSettings.apply_Vz_e_cuts);
    print_bool("apply_Vz_cuts", CutSettings.apply_Vz_cuts);
    print_bool("apply_dVz_cuts", CutSettings.apply_dVz_cuts);
    print_bool("apply_DC_e_fiducial_cuts", CutSettings.apply_DC_e_fiducial_cuts);
    print_bool("apply_DC_fiducial_cuts", CutSettings.apply_DC_fiducial_cuts);
    std::cout << "\n";

    print_bool("apply_electron_quality_cuts", CutSettings.apply_electron_quality_cuts);
    print_bool("apply_Nphe_cut", CutSettings.apply_Nphe_cut);
    print_bool("apply_ECAL_SF_cuts", CutSettings.apply_ECAL_SF_cuts);
    print_bool("apply_ECAL_P_cuts", CutSettings.apply_ECAL_P_cuts);
    print_bool("apply_ECAL_fiducial_cuts", CutSettings.apply_ECAL_fiducial_cuts);
    print_bool("apply_Electron_beta_cut", CutSettings.apply_Electron_beta_cut);
    print_bool("apply_chi2_cuts_1e_cut", CutSettings.apply_chi2_cuts_1e_cut);
    std::cout << "\n";

    print_bool("apply_nucleon_cuts", CutSettings.apply_nucleon_cuts);
    print_bool("apply_nucleon_PID_cuts", CutSettings.apply_nucleon_PID_cuts);
    print_bool("apply_CDEdgeCuts", CutSettings.apply_CDEdgeCuts);
    print_bool("apply_CDRegionCuts", CutSettings.apply_CDRegionCuts);
    print_bool("apply_ProtonPIDCuts", CutSettings.apply_ProtonPIDCuts);
    print_bool("apply_GhostTrackCuts", CutSettings.apply_GhostTrackCuts);
    print_bool("apply_charged_ECAL_veto", CutSettings.apply_charged_ECAL_veto);
    print_bool("apply_neutral_ECAL_veto", CutSettings.apply_neutral_ECAL_veto);
    std::cout << "\n";

    print_bool("apply_nucleon_physical_cuts", CutSettings.apply_nucleon_physical_cuts);
    print_bool("apply_nBeta_fit_cuts", CutSettings.apply_nBeta_fit_cuts);
    print_bool("apply_fiducial_cuts", CutSettings.apply_fiducial_cuts);
    print_bool("apply_kinematical_cuts", CutSettings.apply_kinematical_cuts);
    print_bool("apply_kinematical_weights", CutSettings.apply_kinematical_weights);
    print_bool("apply_nucleon_SmearAndCorr", CutSettings.apply_nucleon_SmearAndCorr);
    std::cout << "\n";

    print_bool("apply_reaction_monitoring_cuts", CutSettings.apply_reaction_monitoring_cuts);
    print_bool("apply_P_miss_in_QE_range_cuts", CutSettings.apply_P_miss_in_QE_range_cuts);
    print_bool("apply_E_miss_in_QE_range_cuts", CutSettings.apply_E_miss_in_QE_range_cuts);
    print_bool("apply_P_miss_in_MECandSRC_range_cuts", CutSettings.apply_P_miss_in_MECandSRC_range_cuts);
    print_bool("apply_E_miss_in_MECandSRC_range_cuts", CutSettings.apply_E_miss_in_MECandSRC_range_cuts);
    print_bool("apply_xB_in_QE_range_cuts", CutSettings.apply_xB_in_QE_range_cuts);
    print_bool("apply_theta_q_pCD_in_lower_FSI_range_cut", CutSettings.apply_theta_q_pCD_in_lower_FSI_range_cut);
    std::cout << "\n";
}
#pragma endregion

// InitEventPrint function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* InitEventPrint function */
void RecoAnalyzerPrinter::InitEventPrint(bool PrintEvents, const AnalysisCutSettings& CutSettings, const std::string& run_plots_path, int Ne_in_event, int Nf_in_event, int nEvents2print) {
    if (PrintEvents) {
        std::string EventPrint_save_Directory;
        if (!CutSettings.apply_chi2_cuts_1e_cut) {
            EventPrint_save_Directory = run_plots_path + "/" + "Event_Print_without_chi2.txt";
        } else if (CutSettings.apply_chi2_cuts_1e_cut) {
            EventPrint_save_Directory = run_plots_path + "/" + "Event_Print_ALL_CUTS.txt";
        }

        EventPrint.open(EventPrint_save_Directory.c_str());

        if (!CutSettings.apply_chi2_cuts_1e_cut) {
            EventPrint << "//////////////////////////////////////////////////////////////////////\n" << env::RESET_COLOR << std::flush;
            EventPrint << "// Log of number of particles in event with all cuts except chi2    //\n" << env::RESET_COLOR << std::flush;
            EventPrint << "//////////////////////////////////////////////////////////////////////\n\n" << env::RESET_COLOR << std::flush;
        } else if (CutSettings.apply_chi2_cuts_1e_cut) {
            EventPrint << "//////////////////////////////////////////////////////////////////////\n" << env::RESET_COLOR << std::flush;
            EventPrint << "// Log of number of particles in event with all cuts including chi2 //\n" << env::RESET_COLOR << std::flush;
            EventPrint << "//////////////////////////////////////////////////////////////////////\n\n" << env::RESET_COLOR << std::flush;
        }

        EventPrint << "EVENT SELECTION:\n" << env::RESET_COLOR << std::flush;
        EventPrint << "#electrons in event:\t\t" << Ne_in_event << "\n" << env::RESET_COLOR << std::flush;
        EventPrint << "minimal #hadrons in event:\t" << Nf_in_event << "\n" << env::RESET_COLOR << std::flush;
        EventPrint << "#event upper bound:\t\t" << nEvents2print << "\n\n\n" << env::RESET_COLOR << std::flush;
    }
}
#pragma endregion

// PrintEventToFile function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* PrintEventToFile function */
void RecoAnalyzerPrinter::PrintEventToFile(std::ofstream& EventPrint, bool PrintEvents, int Ne, int Ne_in_event, int Nf, int Nf_in_event, int nEvents2print, int event,
                                           std::size_t protons_size, std::size_t Kplus_size, std::size_t Kminus_size, std::size_t piplus_size, std::size_t piminus_size,
                                           std::size_t electrons_size, std::size_t deuterons_size, std::size_t neutrals_size, std::size_t otherpart_size) const {
    if (PrintEvents) {  // TODO: add to debugger class?
        const bool EventPrintSelection = (Ne == Ne_in_event && Nf >= Nf_in_event);

        if (EventPrintSelection) {
            if (event < (nEvents2print + 1)) {
                EventPrint << "--- EVENT NUMBER " << event << " ---\n" << env::RESET_COLOR << std::flush;
                EventPrint << "#particles in event:\t" << Nf << "\n" << env::RESET_COLOR << std::flush;
                EventPrint << "protons.size() = " << protons_size << "\n" << env::RESET_COLOR << std::flush;
                EventPrint << "Kplus.size() = " << Kplus_size << "\n" << env::RESET_COLOR << std::flush;
                EventPrint << "Kminus.size() = " << Kminus_size << "\n" << env::RESET_COLOR << std::flush;
                EventPrint << "piplus.size() = " << piplus_size << "\n" << env::RESET_COLOR << std::flush;
                EventPrint << "piminus.size() = " << piminus_size << "\n" << env::RESET_COLOR << std::flush;
                EventPrint << "electrons.size() = " << electrons_size << "\n" << env::RESET_COLOR << std::flush;
                EventPrint << "deuterons.size() = " << deuterons_size << "\n" << env::RESET_COLOR << std::flush;
                EventPrint << "neutrals.size() = " << neutrals_size << "\n" << env::RESET_COLOR << std::flush;
                EventPrint << "otherpart.size() = " << otherpart_size << "\n\n\n" << env::RESET_COLOR << std::flush;
            }
        }
    }
}
#pragma endregion

// SaveFittedPIDCutsToParFile function ----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SaveFittedPIDCutsToParFile function */
void RecoAnalyzerPrinter::SaveFittedPIDCutsToParFile(const RunParameters& parameters, const AnalysisCutSettings& CutSettings, const CutValueManager& CutManager,
                                                     const std::string& run_plots_path) const {
    if (CutSettings.apply_cuts &&  // log pid cuts only if all other cuts are enabled //TODO: review this. make code preform cuts only in this case
        (!CutSettings.only_preselection_cuts && !CutSettings.only_electron_quality_cuts) &&  // Do not log PID cuts if running in only preselection or only electron qulity cuts mode
        !CutSettings.apply_chi2_cuts_1e_cut) {
        DSCuts chi2cuts[] = {CutManager.Chi2_Proton_cuts_CD, CutManager.Chi2_Proton_cuts_FD,  CutManager.Chi2_piplus_cuts_CD,
                             CutManager.Chi2_piplus_cuts_FD, CutManager.Chi2_piminus_cuts_CD, CutManager.Chi2_piminus_cuts_FD};
        int chi2cuts_length = 6;

        ofstream FittedPIDCuts;
        std::string FittedPIDCutsFilePath = pd::PathDefinitions.PIDCutsDirectory + "Fitted_PID_Cuts_-_" + parameters.SampleName + ".par";

        FittedPIDCuts.open(FittedPIDCutsFilePath);

        FittedPIDCuts << "######################################################################\n";
        FittedPIDCuts << "# CLAS12 analysis cuts and parameters file (after chi2 Gaussian fit) #\n";
        FittedPIDCuts << "######################################################################\n";
        FittedPIDCuts << "\n# Cuts are fitted for - " + parameters.SampleName << "\n";
        FittedPIDCuts << "\n# pid cuts by detector (pid:mean:sigma) - sigma_CD=" << CutManager.Chi2_Proton_cuts_CD.FitStdFactor << ";sigma_FD=" << CutManager.Chi2_Proton_cuts_FD.FitStdFactor
                      << ":\n";

        for (int i = 0; i < chi2cuts_length; i++) {
            FittedPIDCuts << "pid_cuts" << "\t\t" << chi2cuts[i].GetPartPDG() << ":" << chi2cuts[i].Cuts.at(0) << ":" << chi2cuts[i].GetUpperCut() << ":" << chi2cuts[i].GetRegion() << "\n";
        }

        FittedPIDCuts.close();

        system(("cp " + FittedPIDCutsFilePath + " " + run_plots_path).c_str());  // Copy fitted chi2 cuts file for easy download from ifarm
    }
}
#pragma endregion

// SaveNucleonCutsToParFile function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SaveNucleonCutsToParFile function */
void RecoAnalyzerPrinter::SaveNucleonCutsToParFile(const RunParameters& parameters, const AnalysisCutSettings& CutSettings, const CutValueManager& CutManager,
                                                   const std::string& run_plots_path, const std::string& nucleon_cuts_status, const std::string& fd_photons_status,
                                                   const std::string& efficiency_status) const {
    if (!CutSettings.apply_nucleon_cuts && CutSettings.apply_chi2_cuts_1e_cut && (!CutSettings.only_preselection_cuts && !CutSettings.only_electron_quality_cuts)) {
        // log nucleon cuts
        ofstream Nucleon_Cuts;
        std::string Nucleon_CutsFilePath = pd::PathDefinitions.NucleonCutsDirectory + "Nucleon_Cuts_-_" + parameters.SampleName + ".par";

        Nucleon_Cuts.open(Nucleon_CutsFilePath);
        Nucleon_Cuts << "######################################################################\n";
        Nucleon_Cuts << "# CLAS12 analysis cuts and parameters file (after Beta Gaussian fit) #\n";
        Nucleon_Cuts << "######################################################################\n";
        Nucleon_Cuts << "\n# Cuts are fitted for - " + parameters.SampleName + ":\t" + nucleon_cuts_status + fd_photons_status + efficiency_status << "\n\n";

        // Neutron momentum cuts
        Nucleon_Cuts << "# Neutron momentum cuts (pid:mean:sigma) - sigma_FD=" << CutManager.n_mom_th.FitStdFactor << ":\n";

        Nucleon_Cuts << CutManager.n_mom_th.GetCutVariable() << "\t\t\t" << CutManager.n_mom_th.GetPartPDG() << ":" << CutManager.n_mom_th.Cuts.at(0) << ":"
                     << CutManager.n_mom_th.GetLowerCut() << ":" << CutManager.n_mom_th.GetRegion() << "\n\n";

        DSCuts Neutron_Momentum_Cuts[] = {CutManager.n_momentum_cuts_ABF_FD_n_from_ph, CutManager.n_momentum_cuts_ABF_FD_n_from_ph_apprax};
        int Neutron_Momentum_Cuts_length = 2;

        for (int i = 0; i < Neutron_Momentum_Cuts_length; i++) {
            Nucleon_Cuts << Neutron_Momentum_Cuts[i].GetCutVariable() << "\t\t\t" << Neutron_Momentum_Cuts[i].GetPartPDG() << ":" << Neutron_Momentum_Cuts[i].Cuts.at(0) << ":"
                         << Neutron_Momentum_Cuts[i].GetUpperCut() << ":" << Neutron_Momentum_Cuts[i].GetRegion() << "\n";
        }

        Nucleon_Cuts << "\n";

        // if (parameters.is6GeVSample) {
        //     // TODO: check if this should stay here!
        Nucleon_Cuts << "nRes_Momentum_cut\t\t\t2112:0:" << parameters.beamE << ":FD-ECAL  # was set manually!" << "\n\n";
        // }

        // Proton CD-FD double detection dPhi_p1_p2 cuts
        Nucleon_Cuts << "# Proton CD-FD double detection dPhi cuts (pid:mean:sigma) - sigma=" << CutManager.dphi_p1_p2_2p.FitStdFactor << ":\n";

        Nucleon_Cuts << CutManager.dphi_p1_p2_2p.GetCutVariable() << "\t\t\t" << CutManager.dphi_p1_p2_2p.GetPartPDG() << ":" << CutManager.dphi_p1_p2_2p.GetMean() << ":"
                     << CutManager.dphi_p1_p2_2p.GetUpperCut() << ":" << CutManager.dphi_p1_p2_2p.GetRegion() << "\n";
        Nucleon_Cuts << CutManager.dphi_pFD_pCD_2p.GetCutVariable() << "\t\t" << CutManager.dphi_pFD_pCD_2p.GetPartPDG() << ":" << CutManager.dphi_pFD_pCD_2p.GetMean() << ":"
                     << CutManager.dphi_pFD_pCD_2p.GetUpperCut() << ":" << CutManager.dphi_pFD_pCD_2p.GetRegion() << "\n";

        Nucleon_Cuts << "\n";

        Nucleon_Cuts.close();

        system(("cp " + Nucleon_CutsFilePath + " " + run_plots_path).c_str());  // Copy nucleon cuts file for easy download from the ifarm
    }
}
#pragma endregion

// SaveSetupToLogFile function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SaveSetupToLogFile function */
void RecoAnalyzerPrinter::SaveSetupToLogFile(const std::string& run_plots_log_save_Directory, const std::string& run_plots_path, const RunParameters& parameters,
                                             const EventCounters& event_counters, int num_of_good_hipo_files, int total_num_of_hipo_files, std::size_t skipped_hipo_chain_files_size,
                                             const EventSelectionSettings& ESSettings, const AcceptanceMapsSettings& AMapsSettings, const MomentumResolutionSettings& MomResSettings,
                                             const AnalysisCutSettings& CutSettings, const CutValueManager& CutManager, const reco_cli::RecoCliOptions& cli, const ParticleID& pid,
                                             const clas12ana& clasAna, const MomentumResolution& nRes, double Pv, double Pvx, double Pvy, double Pvz, bool Skip_prob, bool TestRun,
                                             const HistogramSettings& HSettings, bool delete_png_files, bool delete_root_files, bool Clear_Old_Directories) const {
    std::ofstream myLogFile;
    myLogFile.open(run_plots_log_save_Directory.c_str());
    if (!myLogFile.is_open()) {
        bt::PrintWarning(__func__, __FILE__, __LINE__, "[RecoAnalyzerPrinter] Failed to open log file: " + run_plots_log_save_Directory);
        return;
    }

    constexpr int lineWidth = 100;
    const auto print_value = [&](const std::string& label, const auto& value) {
        std::string normalizedLabel = label;
        while (!normalizedLabel.empty() && (normalizedLabel.back() == ' ' || normalizedLabel.back() == '\t')) { normalizedLabel.pop_back(); }
        while (!normalizedLabel.empty() && (normalizedLabel.back() == ':' || normalizedLabel.back() == '=')) { normalizedLabel.pop_back(); }
        while (!normalizedLabel.empty() && (normalizedLabel.back() == ' ' || normalizedLabel.back() == '\t')) { normalizedLabel.pop_back(); }
        bt::PrintAlignedValueToFile(myLogFile, normalizedLabel, value, lineWidth, 1);
    };

    const auto print_bool = [&](const std::string& label, const bool value) { print_value(label, bt::BoolToString(value)); };
    const auto print_wrapped_value = [&](const std::string& label, const auto& value) {
        std::string normalizedLabel = label;
        while (!normalizedLabel.empty() && (normalizedLabel.back() == ' ' || normalizedLabel.back() == '\t')) { normalizedLabel.pop_back(); }
        while (!normalizedLabel.empty() && (normalizedLabel.back() == ':' || normalizedLabel.back() == '=')) { normalizedLabel.pop_back(); }
        while (!normalizedLabel.empty() && (normalizedLabel.back() == ' ' || normalizedLabel.back() == '\t')) { normalizedLabel.pop_back(); }
        bt::PrintWrappedLabelValue(myLogFile, normalizedLabel, value, lineWidth, 1);
    };

    const auto print_range = [&](const std::string& label, const auto& lower, const auto& upper) { print_value(label, "{" + std::to_string(lower) + ", " + std::to_string(upper) + "}"); };

    constexpr int headerWidth = 100;
    const auto print_banner = [&](const std::string& label) {
        bt::PrintLine(myLogFile, '/', headerWidth);
        const int innerWidth = headerWidth - 4;
        const int leftPad = std::max(0, (innerWidth - static_cast<int>(label.size())) / 2);
        const int rightPad = std::max(0, innerWidth - static_cast<int>(label.size()) - leftPad);
        myLogFile << "//" << std::string(leftPad, ' ') << label << std::string(rightPad, ' ') << "//\n";
        bt::PrintLine(myLogFile, '/', headerWidth);
        myLogFile << "\n";
    };

    const auto print_section_header = [&](const std::string& label) {
        myLogFile << "\n";
        bt::PrintLine(myLogFile, '=', headerWidth);
        myLogFile << label << "\n";
        bt::PrintLine(myLogFile, '=', headerWidth);
        myLogFile << "\n";
    };

    const auto print_subheader = [&](const std::string& label) {
        myLogFile << "-- " << label << " ";
        myLogFile << std::string(std::max(0, headerWidth - 3 - static_cast<int>(label.size())), '-') << "\n";
    };

    print_banner("2N Analyzer Output");

    myLogFile << "Code ran with the following setup:" << "\n\n";

    const std::string quotedWorkingDir = bt::QuoteForShell(pd::PathDefinitions.WorkingDirectory);
    const std::string gitRepositoryTopLevel = bt::RunShellCommand("git -C " + quotedWorkingDir + " rev-parse --show-toplevel");
    const std::string gitRemoteOrigin = bt::RunShellCommand("git -C " + quotedWorkingDir + " config --get remote.origin.url");
    const std::string gitBranch = bt::RunShellCommand("git -C " + quotedWorkingDir + " branch --show-current");
    const std::string gitCommitMessage = bt::RunShellCommand("git -C " + quotedWorkingDir + " log -1 --pretty=%s");
    const std::string gitFullCommitHash = bt::RunShellCommand("git -C " + quotedWorkingDir + " rev-parse HEAD");
    const std::string gitCommitDateTime = bt::RunShellCommand("git -C " + quotedWorkingDir + " log -1 --pretty=%cI");
    const std::string gitCommitAuthor = bt::RunShellCommand("git -C " + quotedWorkingDir + " log -1 --pretty=%an\\ <%ae\\>");
    const std::string gitNearestTag = bt::RunShellCommand("git -C " + quotedWorkingDir + " describe --tags --always");
    const std::string gitSymbolicRefBranch = bt::RunShellCommand("git -C " + quotedWorkingDir + " symbolic-ref -q --short HEAD");
    const std::string gitUpstreamBranch = bt::RunShellCommand("git -C " + quotedWorkingDir + " rev-parse --abbrev-ref --symbolic-full-name @{upstream}");
    const std::string gitAheadBehindRaw = bt::RunShellCommand("git -C " + quotedWorkingDir + " rev-list --left-right --count @{upstream}...HEAD");
    const std::string gitStatusPorcelain = bt::RunShellCommand("git -C " + quotedWorkingDir + " status --porcelain");

    int gitStatusEntries = 0;
    if (!gitStatusPorcelain.empty()) {
        std::istringstream statusStream(gitStatusPorcelain);
        std::string line;
        while (std::getline(statusStream, line)) {
            if (!line.empty()) { ++gitStatusEntries; }
        }
    }

    std::string gitAheadBehindSummary = "Not available";
    if (!gitAheadBehindRaw.empty()) {
        std::istringstream aheadBehindStream(gitAheadBehindRaw);
        int behind = 0;
        int ahead = 0;
        if (aheadBehindStream >> behind >> ahead) { gitAheadBehindSummary = "ahead " + std::to_string(ahead) + ", behind " + std::to_string(behind); }
    }

    const bool gitContextAvailable = !gitFullCommitHash.empty();
    const std::string gitRepository = !gitRemoteOrigin.empty() ? gitRemoteOrigin : (!gitRepositoryTopLevel.empty() ? gitRepositoryTopLevel : "Not available");
    const std::string gitBranchDisplay = !gitContextAvailable ? "Not available" : (!gitSymbolicRefBranch.empty() ? gitSymbolicRefBranch : (!gitBranch.empty() ? gitBranch : "DETACHED"));
    const std::string gitDetachedHeadStatus = !gitContextAvailable ? "Not available" : (gitSymbolicRefBranch.empty() ? "true" : "false");
    const std::string gitStatusSummary = !gitContextAvailable ? "Not available" : (gitStatusEntries == 0 ? "clean" : ("dirty (" + std::to_string(gitStatusEntries) + " changed entries)"));

    const std::string gitHubRepoUrl = bt::GitHubRepoUrlFromRemote(gitRemoteOrigin);
    const std::string gitRefForLink = !gitFullCommitHash.empty() ? gitFullCommitHash : gitBranchDisplay;
    const std::string gitHubFilesLink = (!gitHubRepoUrl.empty() && !gitRefForLink.empty()) ? (gitHubRepoUrl + "/tree/" + gitRefForLink) : "Not available";

    // Git information
    print_section_header("Git information");

    print_value("Git repository", gitRepository);
    print_value("Git branch", gitBranchDisplay);
    print_value("Git commit message", !gitCommitMessage.empty() ? gitCommitMessage : "Not available");
    print_value("Git full commit hash", !gitFullCommitHash.empty() ? gitFullCommitHash : "Not available");
    print_value("Git commit date/time", !gitCommitDateTime.empty() ? gitCommitDateTime : "Not available");
    print_value("Git commit author", !gitCommitAuthor.empty() ? gitCommitAuthor : "Not available");
    print_value("Git status (porcelain summary)", gitStatusSummary);
    print_value("Git nearest tag", !gitNearestTag.empty() ? gitNearestTag : "Not available");
    print_value("Git HEAD detached", gitDetachedHeadStatus);
    print_value("Git tracking branch", !gitUpstreamBranch.empty() ? gitUpstreamBranch : "Not available");
    print_value("Git tracking ahead/behind", gitAheadBehindSummary);
    print_value("GitHub files link", gitHubFilesLink);
    myLogFile << "\n";

    print_section_header("Loaded YAML files");

    print_value("directories_file", cli.directories_file);
    print_value("event_selection_file", cli.event_selection_file);
    print_value("sample_chain_file", cli.sample_chain_file);
    print_value("cut_values_file", cli.cut_values_file);
    print_value("analysis_cuts_file", cli.analysis_cuts_file);
    print_value("acceptance_maps_file", cli.acceptance_maps_file);
    print_value("momentum_resolution_file", cli.momentum_resolution_file);
    myLogFile << "\n";

    print_section_header("CLI settings");

    print_bool("help", cli.help);
    print_value("sample_index", cli.sample_index);
    print_bool("delete_png_files", cli.delete_png_files);
    print_bool("delete_root_files", cli.delete_root_files);
    print_bool("delete_txt_files", cli.delete_txt_files);
    print_bool("Clear_Old_Directories", cli.Clear_Old_Directories);
    print_bool("Skip_prob", cli.Skip_prob);
    print_value("Skip_prob_limit", cli.Skip_prob_limit);
    print_bool("TestRun", cli.TestRun);
    print_value("n_events", cli.n_events);
    print_bool("save_ACorr_data", cli.save_ACorr_data);
    print_bool("verbose", cli.verbose);
    myLogFile << "\n";

    // Input
    print_section_header("Input");

    myLogFile << "WorkingDirectory: " << pd::PathDefinitions.WorkingDirectory << "\n";
    myLogFile << "run_plots_path: " << run_plots_path << "\n";
    myLogFile << "SampleName: " << parameters.SampleName << "\n";
    myLogFile << "VaryingSampleName: " << parameters.VaryingSampleName << "\n\n";

    print_bool("isLocal:\t\t\t", parameters.isLocal);
    print_bool("isMC:\t\t", parameters.isMC);
    print_bool("isData:\t\t\t", parameters.isData);
    print_bool("is2GeVSample:\t\t", parameters.is2GeVSample);
    print_bool("is4GeVSample:\t\t", parameters.is4GeVSample);
    myLogFile << "is6GeVSample:\t\t" << bt::BoolToString(parameters.is6GeVSample) << "\n\n";

    myLogFile << "NumOfGoodHipoFiles:\t\t" << num_of_good_hipo_files << "\n\n";
    myLogFile << "TotalNumOfHipoFiles:\t\t" << total_num_of_hipo_files << "\n\n";

    myLogFile << "SkippedHipoChainFiles.size():\t\t" << skipped_hipo_chain_files_size << "\n\n";

    // setup

    // Plot setup
    print_section_header("Plot setup");

    print_value("wider_margin", bt::BoolToString(HSettings.wider_margin));
    print_value("debug_plots", bt::BoolToString(HSettings.debug_plots));
    myLogFile << "\n";

    print_value("Log_scale_Vx_plots", bt::BoolToString(HSettings.Log_scale_Vx_plots));
    print_value("Log_scale_Vy_plots", bt::BoolToString(HSettings.Log_scale_Vy_plots));
    print_value("Log_scale_Vz_plots", bt::BoolToString(HSettings.Log_scale_Vz_plots));
    print_value("Log_scale_dVx_plots", bt::BoolToString(HSettings.Log_scale_dVx_plots));
    print_value("Log_scale_dVy_plots", bt::BoolToString(HSettings.Log_scale_dVy_plots));
    print_value("Log_scale_dVz_plots", bt::BoolToString(HSettings.Log_scale_dVz_plots));
    myLogFile << "\n";

    // Calculation setup
    print_section_header("Calculation setup");

    print_value("calculate_1p", bt::BoolToString(ESSettings.calculate_1p));
    print_value("calculate_1n", bt::BoolToString(ESSettings.calculate_1n));
    print_value("calculate_2p", bt::BoolToString(ESSettings.calculate_2p));
    myLogFile << "\n";
    print_value("calculate_pFDpCD", bt::BoolToString(ESSettings.calculate_pFDpCD));
    print_value("calculate_nFDpCD", bt::BoolToString(ESSettings.calculate_nFDpCD));
    myLogFile << "\n";

    print_subheader("Truth level calculation setup");
    print_value("calculate_truth_level", bt::BoolToString(ESSettings.calculate_truth_level));
    print_value("TL_plots_only_for_NC", bt::BoolToString(ESSettings.TL_plots_only_for_NC));
    print_value("fill_TL_plots", bt::BoolToString(ESSettings.fill_TL_plots));
    print_value("ZoomIn_On_mom_th_plots", bt::BoolToString(ESSettings.ZoomIn_On_mom_th_plots));
    print_value("Eff_calc_with_one_reco_electron", bt::BoolToString(ESSettings.Eff_calc_with_one_reco_electron));
    print_value("Calc_inc_eff_with_varying_theta", bt::BoolToString(ESSettings.Calc_inc_eff_with_varying_theta));
    print_value("Calc_1n_n_eff_with_smaller_theta", bt::BoolToString(ESSettings.Calc_1n_n_eff_with_smaller_theta));
    print_value("Calc_eff_overlapping_FC", bt::BoolToString(ESSettings.Calc_eff_overlapping_FC));
    print_value("Rec_wTL_ES", bt::BoolToString(ESSettings.Rec_wTL_ES));
    myLogFile << "\n";

    print_value("limless_mom_eff_plots", bt::BoolToString(ESSettings.limless_mom_eff_plots));
    myLogFile << "\n";

    print_subheader("FD neutrals settings");
    print_value("Enable_FD_photons", bt::BoolToString(ESSettings.Enable_FD_photons));
    print_value("Enable_FD_neutrons", bt::BoolToString(ESSettings.Enable_FD_neutrons));
    print_value("Count_FD_neurton_and_photon_hits", bt::BoolToString(ESSettings.Count_FD_neurton_and_photon_hits));

    print_value("ES_by_leading_FDneutron", bt::BoolToString(ESSettings.ES_by_leading_FDneutron));
    myLogFile << "\n";

    print_subheader("AMaps settings");
    print_value("Generate_Electron_AMaps", bt::BoolToString(AMapsSettings.Generate_Electron_AMaps));
    print_value("Generate_Nucleon_AMaps", bt::BoolToString(AMapsSettings.Generate_Nucleon_AMaps));
    print_value("Generate_WMaps", bt::BoolToString(AMapsSettings.Generate_WMaps));
    print_value("AMaps_calc_with_one_reco_electron", bt::BoolToString(AMapsSettings.AMaps_calc_with_one_reco_electron));
    print_value("P_e_bin_profile", AMapsSettings.P_e_bin_profile);
    print_value("P_nuc_bin_profile", AMapsSettings.P_nuc_bin_profile);
    print_value("Electron_single_slice_test", bt::BoolToString(AMapsSettings.Electron_single_slice_test));
    print_value("Nucleon_single_slice_test", bt::BoolToString(AMapsSettings.Nucleon_single_slice_test));
    print_value("TestSlices",
                "{" + std::to_string(AMapsSettings.TestSlices.at(0)) + ", " + std::to_string(AMapsSettings.TestSlices.at(1)) + ", " + std::to_string(AMapsSettings.TestSlices.at(2)) + "}");
    myLogFile << "\n";

    print_subheader("nRES settings");
    print_value("plot_and_fit_MomRes", bt::BoolToString(MomResSettings.plot_and_fit_MomRes));
    print_value("Calculate_momResS2", bt::BoolToString(MomResSettings.Calculate_momResS2));
    print_value("DeltaSlices", MomResSettings.DeltaSlices);
    print_value("VaryingDelta", bt::BoolToString(MomResSettings.VaryingDelta));
    print_value("ForceSmallpResLimits", bt::BoolToString(MomResSettings.ForceSmallpResLimits));
    print_value("AggregationEventTh", MomResSettings.AggregationEventTh);
    print_value("SmearMode", MomResSettings.SmearMode);
    print_value("CorrMode", MomResSettings.CorrMode);
    print_value("Run_with_momResS2", bt::BoolToString(MomResSettings.Run_with_momResS2));
    print_value("momRes_test", bt::BoolToString(MomResSettings.momRes_test));
    myLogFile << "\n";

    print_subheader("Other run parameters");
    print_value("Probe", Probe + " (PDG: " + std::to_string(Probe_pdg) + ")");
    print_value("Target", Target_nucleus + " (PDG: " + std::to_string(Target_pdg) + ")");
    myLogFile << "\n";

    print_value("m_e", constants::m_e);
    print_value("m_p", constants::m_p);
    print_value("m_n", constants::m_n);
    print_value("m_pizero", constants::m_pizero);
    print_value("m_piplus", constants::m_piplus);
    print_value("m_piminus", constants::m_piminus);
    print_value("m_Kzero", constants::m_Kzero);
    print_value("m_Kplus", constants::m_Kplus);
    print_value("m_Kminus", constants::m_Kminus);
    myLogFile << "\n";

    print_value("beamE", std::to_string(parameters.beamE) + " [GeV]");
    print_value("Pv", Pv);
    print_value("Pvx", Pvx);
    print_value("Pvy", Pvy);
    print_value("Pvz", Pvz);
    myLogFile << "\n";

    // Directory settings
    print_section_header("Directory settings");

    print_value("Clear_Old_Directories", bt::BoolToString(Clear_Old_Directories));
    myLogFile << "\n";

    print_value("Save_Plots_folder_to_zip_files", bt::BoolToString(Save_Plots_folder_to_zip_files));
    print_value("zipping_print_out", bt::BoolToString(zipping_print_out));

    // Plot selector
    print_section_header("Plot selector");

    print_value("Skip_prob", bt::BoolToString(Skip_prob));

    print_value("TestRun", bt::BoolToString(TestRun));

    print_value("Plot_selector_master", bt::BoolToString(HSettings.Plot_selector_master));

    myLogFile << "\n";
    print_subheader("Cut variable plots");
    print_value("Cut_plots_master", bt::BoolToString(HSettings.Cut_plots_master));
    print_value("Vertex_plots", bt::BoolToString(HSettings.Vertex_plots));
    print_value("Vertex_corr_plots", bt::BoolToString(HSettings.Vertex_corr_plots));
    print_value("DC_edge_plots", bt::BoolToString(HSettings.DC_edge_plots));
    print_value("Nphe_plots", bt::BoolToString(HSettings.Nphe_plots));
    print_value("Edep_plots", bt::BoolToString(HSettings.Edep_plots));
    print_value("SF_plots", bt::BoolToString(HSettings.SF_plots));
    print_value("ECAL_edge", bt::BoolToString(HSettings.ECAL_edge));
    print_value("Chi2_plots", bt::BoolToString(HSettings.Chi2_plots));
    print_value("pCD_PID_plots", bt::BoolToString(HSettings.pCD_PID_plots));
    print_value("CD_edge_plots", bt::BoolToString(HSettings.CD_edge_plots));
    print_value("Momentum_plots", bt::BoolToString(HSettings.Momentum_plots));

    myLogFile << "\n";
    print_subheader("Reaction monitoring plots");
    print_value("ReacMon_plots", bt::BoolToString(HSettings.ReacMon_plots));

    myLogFile << "\n";
    print_subheader("W plots");
    print_value("W_plots", bt::BoolToString(HSettings.W_plots));

    myLogFile << "\n";
    print_subheader("Beta plots");
    print_value("Beta_plots", bt::BoolToString(HSettings.Beta_plots));
    print_value("Beta_vs_P_plots", bt::BoolToString(HSettings.Beta_vs_P_plots));

    myLogFile << "\n";
    print_subheader("Angle plots");
    print_value("Angle_plots_master", bt::BoolToString(HSettings.Angle_plots_master));
    print_value("Theta_e_plots", bt::BoolToString(HSettings.Theta_e_plots));
    print_value("Phi_e_plots", bt::BoolToString(HSettings.Phi_e_plots));

    myLogFile << "\n";
    print_subheader("Momentum transfer plots");
    print_value("Momentum_transfer_plots", bt::BoolToString(HSettings.Momentum_transfer_plots));

    myLogFile << "\n";
    print_subheader("E_e plots");
    print_value("E_e_plots", bt::BoolToString(HSettings.E_e_plots));

    myLogFile << "\n";
    print_subheader("ET plots");
    print_value("ETrans_plots_master", bt::BoolToString(HSettings.ETrans_plots_master));
    print_value("ETrans_all_plots", bt::BoolToString(HSettings.ETrans_all_plots));
    print_value("ETrans_All_Int_plots", bt::BoolToString(HSettings.ETrans_All_Int_plots));
    print_value("ETrans_QEL_plots", bt::BoolToString(HSettings.ETrans_QEL_plots));
    print_value("ETrans_MEC_plots", bt::BoolToString(HSettings.ETrans_MEC_plots));
    print_value("ETrans_RES_plots", bt::BoolToString(HSettings.ETrans_RES_plots));
    print_value("ETrans_DIS_plots", bt::BoolToString(HSettings.ETrans_DIS_plots));

    myLogFile << "\n";
    print_subheader("E_cal plots");
    print_value("E_cal_plots", bt::BoolToString(HSettings.Ecal_plots));

    myLogFile << "\n";
    print_subheader("Transverse variables plots");
    print_value("TKI_plots", bt::BoolToString(HSettings.TKI_plots));

    myLogFile << "\n";
    print_subheader("ToF plots");
    print_value("ToF_plots", bt::BoolToString(HSettings.ToF_plots));

    myLogFile << "\n";
    print_subheader("Efficiency plots");
    print_value("Efficiency_plots", bt::BoolToString(HSettings.Efficiency_plots));
    print_value("TL_after_Acceptance_Maps_plots", bt::BoolToString(HSettings.TL_after_Acceptance_Maps_plots));

    myLogFile << "\n";
    print_subheader("Acceptance and W maps plots");
    print_value("AMaps_plots", bt::BoolToString(HSettings.AMaps_plots));
    print_value("WMaps_plots", bt::BoolToString(HSettings.WMaps_plots));

    myLogFile << "\n";
    print_subheader("Resolution plots");
    print_value("Resolution_plots", bt::BoolToString(HSettings.Resolution_plots));

    myLogFile << "\n";
    print_subheader("Multiplicity plots");
    print_value("Multiplicity_plots", bt::BoolToString(HSettings.Multiplicity_plots));

    myLogFile << "\n";
    print_subheader("Final state ratio plots");
    print_value("FSR_1D_plots", bt::BoolToString(HSettings.FSR_1D_plots));
    print_value("FSR_2D_plots", bt::BoolToString(HSettings.FSR_2D_plots));

    // Normalization setup
    print_section_header("Normalization setup");

    print_value("normalize_master", bt::BoolToString(HSettings.normalize_master));
    myLogFile << "\n";

    print_value("norm_Vertex_plots", bt::BoolToString(HSettings.norm_Vertex_plots));
    print_value("norm_Vertex_corr_plots", bt::BoolToString(HSettings.norm_Vertex_corr_plots));
    print_value("norm_DC_edge_plots", bt::BoolToString(HSettings.norm_DC_edge_plots));
    print_value("norm_Nphe_plots", bt::BoolToString(HSettings.norm_Nphe_plots));
    print_value("norm_Edep_plots", bt::BoolToString(HSettings.norm_Edep_plots));
    print_value("norm_SF_plots", bt::BoolToString(HSettings.norm_SF_plots));
    print_value("norm_ECAL_edge", bt::BoolToString(HSettings.norm_ECAL_edge));
    print_value("norm_Chi2_plots", bt::BoolToString(HSettings.norm_Chi2_plots));
    print_value("norm_pCD_PID_plots", bt::BoolToString(HSettings.norm_pCD_PID_plots));
    print_value("norm_CD_edge_plots", bt::BoolToString(HSettings.norm_CD_edge_plots));
    print_value("norm_Momentum_plots", bt::BoolToString(HSettings.norm_Momentum_plots));
    myLogFile << "\n";

    print_value("norm_ReacMon_plots", bt::BoolToString(HSettings.norm_ReacMon_plots));
    myLogFile << "\n";

    print_value("norm_W_plots", bt::BoolToString(HSettings.norm_W_plots));
    print_value("norm_Beta_plots", bt::BoolToString(HSettings.norm_Beta_plots));
    print_value("norm_Angle_plots_master", bt::BoolToString(HSettings.norm_Angle_plots_master));
    print_value("norm_Momentum_transfer_plots", bt::BoolToString(HSettings.norm_Momentum_transfer_plots));
    print_value("norm_E_e_plots", bt::BoolToString(HSettings.norm_E_e_plots));
    print_value("norm_ET_plots", bt::BoolToString(HSettings.norm_ET_plots));
    print_value("norm_Ecal_plots", bt::BoolToString(HSettings.norm_Ecal_plots));
    print_value("norm_TKI_plots", bt::BoolToString(HSettings.norm_TKI_plots));
    myLogFile << "\n";

    print_value("norm_MomRes_plots", bt::BoolToString(HSettings.norm_MomRes_plots));
    print_value("norm_Multi_plots", bt::BoolToString(HSettings.norm_Multi_plots));
    myLogFile << "\n";

    // Delete setup
    print_section_header("Delete setup");

    print_value("delete_png_files", bt::BoolToString(delete_png_files));
    print_value("delete_root_files", bt::BoolToString(delete_root_files));
    myLogFile << "\n";

    // Cut application setup
    print_section_header("Cut application setup");

    print_subheader("clas12ana cuts");
    print_value("apply_cuts", bt::BoolToString(CutSettings.apply_cuts));
    myLogFile << "\n";

    print_value("clas12ana_particles", bt::BoolToString(CutSettings.clas12ana_particles));
    myLogFile << "\n";  // TODO: move form here!

    print_value("only_preselection_cuts", bt::BoolToString(CutSettings.only_preselection_cuts));
    print_value("only_electron_quality_cuts", bt::BoolToString(CutSettings.only_electron_quality_cuts));
    myLogFile << "\n";

    print_value("apply_preselection_cuts", bt::BoolToString(CutSettings.apply_preselection_cuts));
    print_value("apply_Vz_e_cuts", bt::BoolToString(CutSettings.apply_Vz_e_cuts));
    print_value("apply_Vz_cuts", bt::BoolToString(CutSettings.apply_Vz_cuts));
    print_value("apply_dVz_cuts", bt::BoolToString(CutSettings.apply_dVz_cuts));
    print_value("apply_DC_e_fiducial_cuts", bt::BoolToString(CutSettings.apply_DC_e_fiducial_cuts));
    print_value("apply_DC_fiducial_cuts", bt::BoolToString(CutSettings.apply_DC_fiducial_cuts));
    myLogFile << "\n";

    print_value("apply_electron_quality_cuts", bt::BoolToString(CutSettings.apply_electron_quality_cuts));
    print_value("apply_Nphe_cut", bt::BoolToString(CutSettings.apply_Nphe_cut));
    print_value("apply_ECAL_SF_cuts", bt::BoolToString(CutSettings.apply_ECAL_SF_cuts));
    print_value("apply_ECAL_P_cuts", bt::BoolToString(CutSettings.apply_ECAL_P_cuts));
    print_value("apply_ECAL_fiducial_cuts", bt::BoolToString(CutSettings.apply_ECAL_fiducial_cuts));
    myLogFile << "\n";
    print_value("apply_Electron_beta_cut", bt::BoolToString(CutSettings.apply_Electron_beta_cut));
    myLogFile << "\n";

    print_value("apply_chi2_cuts_1e_cut", bt::BoolToString(CutSettings.apply_chi2_cuts_1e_cut));
    myLogFile << "\n";

    print_subheader("My analysis cuts");
    print_value("apply_nucleon_cuts", bt::BoolToString(CutSettings.apply_nucleon_cuts));
    myLogFile << "\n";

    print_value("apply_nucleon_PID_cuts", bt::BoolToString(CutSettings.apply_nucleon_PID_cuts));
    print_value("apply_CDEdgeCuts", bt::BoolToString(CutSettings.apply_CDEdgeCuts));
    print_value("apply_CDRegionCuts", bt::BoolToString(CutSettings.apply_CDRegionCuts));
    print_value("apply_ProtonPIDCuts", bt::BoolToString(CutSettings.apply_ProtonPIDCuts));
    print_value("apply_GhostTrackCuts", bt::BoolToString(CutSettings.apply_GhostTrackCuts));
    print_value("apply_charged_ECAL_veto", bt::BoolToString(CutSettings.apply_charged_ECAL_veto));
    print_value("apply_neutral_ECAL_veto", bt::BoolToString(CutSettings.apply_neutral_ECAL_veto));
    myLogFile << "\n";

    print_value("apply_nucleon_physical_cuts", bt::BoolToString(CutSettings.apply_nucleon_physical_cuts));
    print_value("apply_nBeta_fit_cuts", bt::BoolToString(CutSettings.apply_nBeta_fit_cuts));
    print_value("apply_fiducial_cuts", bt::BoolToString(CutSettings.apply_fiducial_cuts));
    print_value("apply_kinematical_cuts", bt::BoolToString(CutSettings.apply_kinematical_cuts));
    print_value("apply_kinematical_weights", bt::BoolToString(CutSettings.apply_kinematical_weights));
    print_value("apply_nucleon_SmearAndCorr", bt::BoolToString(CutSettings.apply_nucleon_SmearAndCorr));
    myLogFile << "\n";

    print_bool("apply_reaction_monitoring_cuts", CutSettings.apply_reaction_monitoring_cuts);
    print_bool("apply_P_miss_in_QE_range_cuts", CutSettings.apply_P_miss_in_QE_range_cuts);
    print_bool("apply_E_miss_in_QE_range_cuts", CutSettings.apply_E_miss_in_QE_range_cuts);
    print_bool("apply_P_miss_in_MECandSRC_range_cuts", CutSettings.apply_P_miss_in_MECandSRC_range_cuts);
    print_bool("apply_E_miss_in_MECandSRC_range_cuts", CutSettings.apply_E_miss_in_MECandSRC_range_cuts);
    print_bool("apply_xB_in_QE_range_cuts", CutSettings.apply_xB_in_QE_range_cuts);
    print_bool("apply_theta_q_pCD_in_lower_FSI_range_cut", CutSettings.apply_theta_q_pCD_in_lower_FSI_range_cut);
    myLogFile << "\n";

    // cuts

    // #Photo-electrons (Nphe) cuts (electrons only, FD)
    print_section_header("#Photo-electrons (Nphe) cuts (electrons only, FD)");

    print_value("Nphe_lower_cut", CutManager.Nphe_cuts_FD.GetLowerCut());
    print_value("Nphe_upper_cut", CutManager.Nphe_cuts_FD.GetUpperCut());
    myLogFile << "\n";

    // Chi2 cuts
    print_section_header("Chi2 cuts");

    myLogFile << "\n-- Electrons --------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Electrons in FD (1e cut) -----------------------------------------------" << "\n";
    print_value("Chi2 lower cut", CutManager.Chi2_Electron_cuts_FD.GetLowerCut());
    print_value("Chi2 upper cut", CutManager.Chi2_Electron_cuts_FD.GetUpperCut());
    print_value("Chi2 hist mean", CutManager.Chi2_Electron_cuts_FD.MeanFromHistogram);
    myLogFile << "\n";

    // Chi2_Proton
    myLogFile << "\n\n-- Protons ----------------------------------------------------------------" << "\n\n";

    const auto print_chi2_block = [&](const std::string& title, const auto& cuts) {
        myLogFile << "\n" << title << "\n";
        print_value("Chi2 std factor", cuts.GetStdFactor());
        print_value("Chi2 fit std", cuts.GetUpperCut() / cuts.GetStdFactor());
        print_value("Chi2 fit mean", cuts.GetMean());
        print_value("Chi2 hist mean", cuts.MeanFromHistogram);
        print_value("Chi2 lower cut", cuts.GetLowerCut());
        print_value("Chi2 upper cut", cuts.GetUpperCut());
        myLogFile << "\n";
    };

    print_chi2_block("-- Protons in CD (1e cut) -------------------------------------------------", CutManager.Chi2_Proton_cuts_CD);
    print_chi2_block("-- Protons in FD (1e cut) -------------------------------------------------", CutManager.Chi2_Proton_cuts_FD);

    // Chi2_Kplus
    myLogFile << "\n-- K+ ---------------------------------------------------------------------" << "\n\n";

    print_chi2_block("-- Kplus in CD (1e cut) ---------------------------------------------------", CutManager.Chi2_Kplus_cuts_CD);
    print_chi2_block("-- Kplus in FD (1e cut) ---------------------------------------------------", CutManager.Chi2_Kplus_cuts_FD);

    // Chi2_Kminus
    myLogFile << "\n-- K- ---------------------------------------------------------------------" << "\n\n";

    print_chi2_block("-- Kminus in CD (1e cut) --------------------------------------------------", CutManager.Chi2_Kminus_cuts_CD);
    print_chi2_block("-- Kminus in FD (1e cut) --------------------------------------------------", CutManager.Chi2_Kminus_cuts_FD);

    // Chi2_piplus
    myLogFile << "\n-- pi+ --------------------------------------------------------------------" << "\n\n";

    print_chi2_block("-- piplus in CD (1e cut) --------------------------------------------------", CutManager.Chi2_piplus_cuts_CD);
    print_chi2_block("-- piplus in FD (1e cut) --------------------------------------------------", CutManager.Chi2_piplus_cuts_FD);

    // Chi2_piminus
    myLogFile << "\n-- pi- --------------------------------------------------------------------" << "\n\n";

    print_chi2_block("-- piminus in CD (1e cut) -------------------------------------------------", CutManager.Chi2_piminus_cuts_CD);
    print_chi2_block("-- piminus in FD (1e cut) -------------------------------------------------", CutManager.Chi2_piminus_cuts_FD);

    // Chi2_deuteron
    myLogFile << "\n-- deuteron ---------------------------------------------------------------" << "\n\n";

    print_chi2_block("-- deuteron in CD (1e cut) -------------------------------------------------", CutManager.Chi2_deuteron_cuts_CD);
    print_chi2_block("-- deuteron in FD (1e cut) -------------------------------------------------", CutManager.Chi2_deuteron_cuts_FD);

    // Vertex cuts (CD & FD)
    print_section_header("V cuts (CD & FD)");

    print_value("Vz_cut.GetUpperCut()", CutManager.Vz_cut.GetUpperCut());
    print_value("Vz_cut.GetLowerCut()", CutManager.Vz_cut.GetLowerCut());
    myLogFile << "\n";
    print_value("Vz_cut_FD.GetUpperCut()", CutManager.Vz_cut_FD.GetUpperCut());
    print_value("Vz_cut_FD.GetLowerCut()", CutManager.Vz_cut_FD.GetLowerCut());
    myLogFile << "\n";
    print_value("Vz_cut_CD.GetUpperCut()", CutManager.Vz_cut_CD.GetUpperCut());
    print_value("Vz_cut_CD.GetLowerCut()", CutManager.Vz_cut_CD.GetLowerCut());
    myLogFile << "\n";

    // dV cuts (CD & FD)
    print_section_header("dV cuts (CD & FD)");

    print_value("dVz_cuts.GetUpperCut()", CutManager.dVz_cuts.GetUpperCut());
    print_value("dVz_cuts.GetLowerCut()", CutManager.dVz_cuts.GetLowerCut());
    myLogFile << "\n";
    print_value("dVz_cuts_FD.GetUpperCut()", CutManager.dVz_cuts_FD.GetUpperCut());
    print_value("dVz_cuts_FD.GetLowerCut()", CutManager.dVz_cuts_FD.GetLowerCut());
    myLogFile << "\n";
    print_value("dVz_cuts_CD.GetUpperCut()", CutManager.dVz_cuts_CD.GetUpperCut());
    print_value("dVz_cuts_CD.GetLowerCut()", CutManager.dVz_cuts_CD.GetLowerCut());
    myLogFile << "\n";

    // Sampling Fraction (SF) cuts (electrons only, FD)
    print_section_header("Sampling Fraction (SF) cuts (electrons only, FD)");

    print_value("SF_1e_upper_cut", clasAna.getEcalSFUpperCut());
    print_value("SF_1e_lower_cut", clasAna.getEcalSFLowerCut());

    // ECAL fiducial cuts (electrons only, FD)
    print_section_header("ECAL fiducial cuts (electrons only, FD)");

    print_value("fiducial_cut_Lv", clasAna.getEcalEdgeCuts());
    print_value("fiducial_cut_Lw", clasAna.getEcalEdgeCuts());
    myLogFile << "\n";

    // Momentum thresholds
    print_section_header("Momentum thresholds {min, max}");

    print_range("e_mom_th", CutManager.e_mom_th.GetLowerCut(), CutManager.e_mom_th.GetUpperCut());
    print_range("p_mom_th", CutManager.p_mom_th.GetLowerCut(), CutManager.p_mom_th.GetUpperCut());
    print_range("pFD_mom_th", CutManager.pFD_mom_th.GetLowerCut(), CutManager.pFD_mom_th.GetUpperCut());
    print_range("pCD_mom_th", CutManager.pCD_mom_th.GetLowerCut(), CutManager.pCD_mom_th.GetUpperCut());
    print_range("n_mom_th", CutManager.n_mom_th.GetLowerCut(), CutManager.n_mom_th.GetUpperCut());
    print_range("ph_mom_th", CutManager.ph_mom_th.GetLowerCut(), CutManager.ph_mom_th.GetUpperCut());
    print_range("pip_mom_th", CutManager.pip_mom_th.GetLowerCut(), CutManager.pip_mom_th.GetUpperCut());
    print_range("pipFD_mom_th", CutManager.pipFD_mom_th.GetLowerCut(), CutManager.pipFD_mom_th.GetUpperCut());
    print_range("pipCD_mom_th", CutManager.pipCD_mom_th.GetLowerCut(), CutManager.pipCD_mom_th.GetUpperCut());
    print_range("pim_mom_th", CutManager.pim_mom_th.GetLowerCut(), CutManager.pim_mom_th.GetUpperCut());
    print_range("pimFD_mom_th", CutManager.pimFD_mom_th.GetLowerCut(), CutManager.pimFD_mom_th.GetUpperCut());
    print_range("pimCD_mom_th", CutManager.pimCD_mom_th.GetLowerCut(), CutManager.pimCD_mom_th.GetUpperCut());
    myLogFile << "\n";

    // Momentum cuts
    print_section_header("Momentum cuts {min, max}");

    myLogFile << "\n";
    print_subheader("Momentum cuts (1n, FD only)");
    print_range("Neutrons (n_momentum_cuts - ECAL)", CutManager.n_momentum_cuts_ABF_FD_n_from_ph.GetLowerCut(), CutManager.n_momentum_cuts_ABF_FD_n_from_ph.GetUpperCut());
    print_range("Neutrons (n_momentum_cuts - ECAL apprax)", CutManager.n_momentum_cuts_ABF_FD_n_from_ph_apprax.GetLowerCut(),
                CutManager.n_momentum_cuts_ABF_FD_n_from_ph_apprax.GetUpperCut());

    // TL Momentum thresholds
    print_section_header("TL Momentum thresholds {min, max}");

    print_range("TL_e_mom_cuts", CutManager.TL_e_mom_cuts.GetLowerCut(), CutManager.TL_e_mom_cuts.GetUpperCut());
    print_range("TL_n_mom_cuts", CutManager.TL_n_mom_cuts.GetLowerCut(), CutManager.TL_n_mom_cuts.GetUpperCut());
    print_range("TL_p_mom_cuts", CutManager.TL_p_mom_cuts.GetLowerCut(), CutManager.TL_p_mom_cuts.GetUpperCut());
    print_range("TL_pFD_mom_cuts", CutManager.TL_pFD_mom_cuts.GetLowerCut(), CutManager.TL_pFD_mom_cuts.GetUpperCut());
    print_range("TL_pCD_mom_cuts", CutManager.TL_pCD_mom_cuts.GetLowerCut(), CutManager.TL_pCD_mom_cuts.GetUpperCut());
    print_range("TL_pip_mom_cuts", CutManager.TL_pip_mom_cuts.GetLowerCut(), CutManager.TL_pip_mom_cuts.GetUpperCut());
    print_range("TL_pipFD_mom_cuts", CutManager.TL_pipFD_mom_cuts.GetLowerCut(), CutManager.TL_pipFD_mom_cuts.GetUpperCut());
    print_range("TL_pipCD_mom_cuts", CutManager.TL_pipCD_mom_cuts.GetLowerCut(), CutManager.TL_pipCD_mom_cuts.GetUpperCut());
    print_range("TL_pim_mom_cuts", CutManager.TL_pim_mom_cuts.GetLowerCut(), CutManager.TL_pim_mom_cuts.GetUpperCut());
    print_range("TL_pimFD_mom_cuts", CutManager.TL_pimFD_mom_cuts.GetLowerCut(), CutManager.TL_pimFD_mom_cuts.GetUpperCut());
    print_range("TL_pimCD_mom_cuts", CutManager.TL_pimCD_mom_cuts.GetLowerCut(), CutManager.TL_pimCD_mom_cuts.GetUpperCut());
    print_range("TL_pi0_mom_cuts", CutManager.TL_pi0_mom_cuts.GetLowerCut(), CutManager.TL_pi0_mom_cuts.GetUpperCut());
    print_range("TL_ph_mom_cuts", CutManager.TL_ph_mom_cuts.GetLowerCut(), CutManager.TL_ph_mom_cuts.GetUpperCut());
    myLogFile << "\n";

    // Beta cut (1n, FD)
    print_section_header("Beta cut (1n, FD)");

    print_value("Beta_cut.GetUpperCut() (1 fit std)", CutManager.Beta_cut.GetUpperCut());
    print_value("Beta_cut.GetMean()", CutManager.Beta_cut.GetMean());
    myLogFile << "\n";

    // Nucleon kinematical cuts (FD)
    print_section_header("Nucleon kinematical cuts (FD)");

    print_value("FD_nucleon_theta_cut.GetUpperCut()", CutManager.FD_nucleon_theta_cut.GetUpperCut());
    print_value("FD_nucleon_theta_cut.GetLowerCut()", CutManager.FD_nucleon_theta_cut.GetLowerCut());
    myLogFile << "\n";

    print_value("FD_nucleon_momentum_cut.GetUpperCut()", CutManager.FD_nucleon_momentum_cut.GetUpperCut());
    print_value("FD_nucleon_momentum_cut.GetLowerCut()", CutManager.FD_nucleon_momentum_cut.GetLowerCut());
    myLogFile << "\n";

    print_value("MomRes_mu_cuts.GetUpperCut()", CutManager.MomRes_mu_cuts.GetUpperCut());
    print_value("MomRes_mu_cuts.GetLowerCut()", CutManager.MomRes_mu_cuts.GetLowerCut());
    print_value("MomRes_sigma_cuts.GetUpperCut()", CutManager.MomRes_sigma_cuts.GetUpperCut());
    print_value("MomRes_sigma_cuts.GetLowerCut()", CutManager.MomRes_sigma_cuts.GetLowerCut());
    myLogFile << "\n";

    print_value("P_miss_1N_QE_range.GetUpperCut()", CutManager.P_miss_1N_QE_range.GetUpperCut());
    print_value("P_miss_1N_QE_range.GetLowerCut()", CutManager.P_miss_1N_QE_range.GetLowerCut());
    print_value("E_miss_1N_QE_range.GetUpperCut()", CutManager.E_miss_1N_QE_range.GetUpperCut());
    print_value("E_miss_1N_QE_range.GetLowerCut()", CutManager.E_miss_1N_QE_range.GetLowerCut());
    print_value("P_miss_1N_MECandSRC_range.GetUpperCut()", CutManager.P_miss_1N_MECandSRC_range.GetUpperCut());
    print_value("P_miss_1N_MECandSRC_range.GetLowerCut()", CutManager.P_miss_1N_MECandSRC_range.GetLowerCut());
    print_value("E_miss_1N_MECandSRC_range.GetUpperCut()", CutManager.E_miss_1N_MECandSRC_range.GetUpperCut());
    print_value("E_miss_1N_MECandSRC_range.GetLowerCut()", CutManager.E_miss_1N_MECandSRC_range.GetLowerCut());
    print_value("P_miss_1N_OtherReacAndFSI_range.GetUpperCut()", CutManager.P_miss_1N_OtherReacAndFSI_range.GetUpperCut());
    print_value("P_miss_1N_OtherReacAndFSI_range.GetLowerCut()", CutManager.P_miss_1N_OtherReacAndFSI_range.GetLowerCut());
    print_value("E_miss_1N_OtherReacAndFSI_range.GetUpperCut()", CutManager.E_miss_1N_OtherReacAndFSI_range.GetUpperCut());
    print_value("E_miss_1N_OtherReacAndFSI_range.GetLowerCut()", CutManager.E_miss_1N_OtherReacAndFSI_range.GetLowerCut());
    print_value("xB_cut.GetUpperCut()", CutManager.xB_cut.GetUpperCut());
    print_value("xB_cut.GetLowerCut()", CutManager.xB_cut.GetLowerCut());
    print_value("Theta_q_pCD_cut.GetUpperCut()", CutManager.Theta_q_pCD_cut.GetUpperCut());
    print_value("Theta_q_pCD_cut.GetLowerCut()", CutManager.Theta_q_pCD_cut.GetLowerCut());
    myLogFile << "\n";

    // Neutron veto cut (1n & nFDpCD, FD)
    print_section_header("Neutron veto cut (1n & nFDpCD, FD)");

    print_value("Neutron_veto_cut.GetLowerCut()", CutManager.Neutron_veto_cut.GetLowerCut());
    myLogFile << "\n";

    // Ghost tracks handling (2p, CD & FD)
    print_section_header("Ghost tracks handling (2p, CD & FD)");

    print_value("Theta_p1_cuts_2p", CutManager.Theta_p1_cuts_2p.GetUpperCut());
    print_value("Theta_p1_cuts_2p mean", CutManager.Theta_p1_cuts_2p.GetMean());
    print_value("Theta_p2_cuts_2p", CutManager.Theta_p2_cuts_2p.GetUpperCut());
    print_value("Theta_p2_cuts_2p mean", CutManager.Theta_p2_cuts_2p.GetMean());
    print_value("dphi_p1_p2_2p.GetUpperCut()", CutManager.dphi_p1_p2_2p.GetUpperCut());
    print_value("dphi_p1_p2_2p.GetMean()", CutManager.dphi_p1_p2_2p.GetMean());

    myLogFile << "\n";
    print_value("dphi_pFD_pCD_2p.GetUpperCut()", CutManager.dphi_pFD_pCD_2p.GetUpperCut());
    print_value("dphi_pFD_pCD_2p.GetMean()", CutManager.dphi_pFD_pCD_2p.GetMean());
    myLogFile << "\n";

    // momRes correction and smearing coefficients
    print_section_header("momRes correction and smearing coefficients");

    // momRes run status
    if (MomResSettings.plot_and_fit_MomRes) {
        if (!MomResSettings.Calculate_momResS2 && !MomResSettings.Run_with_momResS2) {
            if (!MomResSettings.VaryingDelta) {
                myLogFile << "\nThis run calculated resolution with small 0.05 GeV/c slices!\n\n";
            } else {
                myLogFile << "\nThis is a momResS1 calculation run.\n\n";
            }
        } else {
            if (MomResSettings.momRes_test) {
                if (MomResSettings.Calculate_momResS2) {
                    myLogFile << "\nThis is a momResS2 calculation test run.\n\n";
                } else if (MomResSettings.Run_with_momResS2) {
                    myLogFile << "\nThis is a momResS calculation test run with stage 2 results.\n\n";
                }
            } else {
                if (MomResSettings.Calculate_momResS2) {
                    myLogFile << "\nThis is a momResS2 calculation run.\n\n";
                } else if (MomResSettings.Run_with_momResS2) {
                    myLogFile << "\nThis is a momResS calculation run with stage 2 results.\n\n";
                }
            }
        }
    } else {
        if (MomResSettings.Run_with_momResS2) {
            myLogFile << "\nMomRes is running with stage 2 results.\n";
        } else {
            myLogFile << "\nMomRes is running with stage 1 or stage 0 results.\n";
        }
    }

    myLogFile << "\n";
    print_subheader("Neutron correction");
    print_value("CorrMode", nRes.Get_CorrMode());
    myLogFile << "\n";
    print_wrapped_value("Correction loading path", nRes.Get_Loaded_Corr_coefficients_path());
    myLogFile << "\n";

    std::vector<double> Corr_coefficients_values = nRes.Get_Loaded_Corr_coefficients_values();
    std::vector<std::string> Corr_coefficients_names = nRes.Get_Loaded_Corr_coefficients_names();

    for (std::size_t i = 0; i < Corr_coefficients_values.size(); i++) { print_value(Corr_coefficients_names.at(i), Corr_coefficients_values.at(i)); }

    myLogFile << "\n";
    print_subheader("Proton smearing");
    print_value("SmearMode", nRes.Get_SmearMode());
    myLogFile << "\n";
    print_wrapped_value("Smearing loading path", nRes.Get_Loaded_Std_coefficients_path());
    myLogFile << "\n";

    std::vector<double> Std_coefficients_values = nRes.Get_Loaded_Smear_coefficients_values();
    std::vector<std::string> Std_coefficients_names = nRes.Get_Loaded_Smear_coefficients_names();

    for (std::size_t i = 0; i < Std_coefficients_values.size(); i++) { print_value(Std_coefficients_names.at(i), Std_coefficients_values.at(i)); }

    myLogFile << "\n";
    print_value("Random number generator", nRes.GetRandNumGenerator());
    myLogFile << "\n";

    // Event counts
    print_section_header("Event counts");

    print_subheader("Total counts");
    print_value("Total #(events)", event_counters.num_of_events);
    print_value("Total #(events) w/o any e", event_counters.num_of_events_without_any_e);
    print_value("Total #(events) w/ any e", event_counters.num_of_events_with_any_e);
    myLogFile << "\n";

    print_value("Total #(QEL events)", event_counters.num_of_QEL_events);
    print_value("Total #(MEC events)", event_counters.num_of_MEC_events);
    print_value("Total #(RES events)", event_counters.num_of_RES_events);
    print_value("Total #(DIS events)", event_counters.num_of_DIS_events);
    print_value("QEL + MEC + RES + DIS", event_counters.num_of_QEL_events + event_counters.num_of_MEC_events + event_counters.num_of_RES_events + event_counters.num_of_DIS_events);
    myLogFile << "\n";

    print_subheader("Events with electrons counts");
    print_value("#(events) w/ at least 1e", event_counters.num_of_events_with_at_least_1e);
    print_value("#(events) w/ more then 1e", event_counters.num_of_events_more_then_1e);
    print_value("#(events) w/ exactly 1e", event_counters.num_of_events_with_exactly_1e);
    print_value("#(events) w/ exactly 1e (from file)", event_counters.num_of_events_with_exactly_1e_from_file);
    myLogFile << "\n";

    print_value("Total #(QEL events) 1e cut", event_counters.num_of_QEL_events_1e_cut);
    print_value("Total #(MEC events) 1e cut", event_counters.num_of_MEC_events_1e_cut);
    print_value("Total #(RES events) 1e cut", event_counters.num_of_RES_events_1e_cut);
    print_value("Total #(DIS events) 1e cut", event_counters.num_of_DIS_events_1e_cut);
    print_value("QEL + MEC + RES + DIS (1e cut)",
                event_counters.num_of_QEL_events_1e_cut + event_counters.num_of_MEC_events_1e_cut + event_counters.num_of_RES_events_1e_cut + event_counters.num_of_DIS_events_1e_cut);
    myLogFile << "\n";

    if (ESSettings.Count_FD_neurton_and_photon_hits) {
        print_subheader("FD neutron hit counts in ECAL");
        print_value("num_of_events_with_nFD_CLA12", event_counters.num_of_events_with_nFD_CLA12);
        print_value("num_of_events_with_nFD_CLA12_PCAL", event_counters.num_of_events_with_nFD_CLA12_PCAL);
        print_value("num_of_events_with_nFD_CLA12_ECIN", event_counters.num_of_events_with_nFD_CLA12_ECIN);
        print_value("num_of_events_with_nFD_CLA12_ECOUT", event_counters.num_of_events_with_nFD_CLA12_ECOUT);
        print_value("num_of_events_with_nFD_CLA12_EC", event_counters.num_of_events_with_nFD_CLA12_EC);
        myLogFile << "\n";

        print_subheader("FD photon hit counts in ECAL");
        print_value("num_of_events_with_phFD_CLA12", event_counters.num_of_events_with_phFD_CLA12);
        print_value("num_of_events_with_phFD_CLA12_PCAL", event_counters.num_of_events_with_phFD_CLA12_PCAL);
        print_value("num_of_events_with_phFD_CLA12_ECIN", event_counters.num_of_events_with_phFD_CLA12_ECIN);
        print_value("num_of_events_with_phFD_CLA12_ECOUT", event_counters.num_of_events_with_phFD_CLA12_ECOUT);
        print_value("num_of_events_with_phFD_CLA12_EC", event_counters.num_of_events_with_phFD_CLA12_EC);
        myLogFile << "\n";
    }

    print_subheader("Counting events with FD neutrons");
    print_value("#(events) w/ 1 FD neutrons", event_counters.num_of_events_1n_in_FD);
    print_value("#(events) w/ 2 FD neutrons", event_counters.num_of_events_2n_in_FD);
    print_value("#(events) w/ 3 FD neutrons", event_counters.num_of_events_3n_in_FD);
    print_value("#(events) w/ more than 3 FD neutrons", event_counters.num_of_events_Xn_in_FD);
    myLogFile << "\n";

    print_subheader("1e1p event counts");
    print_value("#(events) w/ 1e1p", event_counters.num_of_events_with_1e1p);
    myLogFile << "\n";

    print_subheader("1e2p event counts");
    print_value("#(events) w/ 1e2p", event_counters.num_of_events_with_1e2p);
    myLogFile << "\n";

    print_subheader("1epFDpCD event counts");
    print_value("#(events) w/ 1epFDpCD", event_counters.num_of_events_with_1epFDpCD);
    myLogFile << "\n";

    print_subheader("2p event counts");
    print_value("num_of_events_2p_wFakeProtons", event_counters.num_of_events_2p_wFakeProtons);
    myLogFile << "\n";

    if (CutSettings.apply_nucleon_cuts) {
        print_value("num_of_RM_2p_events_sCTOFhp", pid.num_of_RM_2p_events_sCTOFhp);
        print_value("num_of_AD_2p_events_from_3p_sCTOFhp", pid.num_of_AD_2p_events_from_3p_sCTOFhp);
        print_value("num_of_AD_2p_events_from_4p_sCTOFhp", pid.num_of_AD_2p_events_from_4p_sCTOFhp);
        print_value("num_of_AD_2p_events_from_Xp_sCTOFhp", pid.num_of_AD_2p_events_from_Xp_sCTOFhp);
        print_value("num_of_RM_2p_events_dCDaFDd", pid.num_of_RM_2p_events_dCDaFDd);
        print_value("num_of_AD_2p_events_from_3p_dCDaFDd", pid.num_of_AD_2p_events_from_3p_dCDaFDd);
        print_value("num_of_AD_2p_events_from_4p_dCDaFDd", pid.num_of_AD_2p_events_from_4p_dCDaFDd);
        print_value("num_of_AD_2p_events_from_Xp_dCDaFDd", pid.num_of_AD_2p_events_from_Xp_dCDaFDd);
        myLogFile << "\n";

        print_value("num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd", pid.num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd);
        myLogFile << "\n";

        print_value("num_of_events_2p (from monitoring)", event_counters.num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd +
                                                              pid.num_of_AD_2p_events_from_3p_sCTOFhp + pid.num_of_AD_2p_events_from_4p_sCTOFhp + pid.num_of_AD_2p_events_from_3p_dCDaFDd +
                                                              pid.num_of_AD_2p_events_from_4p_dCDaFDd);

        print_value("num_of_events_2p (from monitoring; no mixed)", event_counters.num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd +
                                                                        pid.num_of_AD_2p_events_from_3p_sCTOFhp + pid.num_of_AD_2p_events_from_4p_sCTOFhp +
                                                                        pid.num_of_AD_2p_events_from_3p_dCDaFDd + pid.num_of_AD_2p_events_from_4p_dCDaFDd -
                                                                        pid.num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd);
        myLogFile << "\n";
    }

    print_value("#(events) 2p", event_counters.num_of_events_2p);
    myLogFile << "\n";

    print_subheader("pFDpCD event counts");
    print_value("#(events) pFDpCD", event_counters.num_of_events_pFDpCD);
    myLogFile << "\n";

    print_subheader("Event counts");
    print_value("num_of_events_1p_inFD", event_counters.num_of_events_1p_inFD);
    myLogFile << "\n";

    print_value("num_of_events_1n_inFD", event_counters.num_of_events_1n_inFD);
    print_value("num_of_events_1n_inFD_AV", event_counters.num_of_events_1n_inFD_AV);
    print_value("num_of_events_1n_inFD_AV_wmt1gTLn", event_counters.num_of_events_1n_inFD_AV_wmt1gTLn);
    myLogFile << "\n";

    print_value("num_of_events_2p_wFakeProtons", event_counters.num_of_events_2p_wFakeProtons);
    print_value("num_of_events_2p", event_counters.num_of_events_2p);
    myLogFile << "\n";

    print_value("num_of_events_1epFDpCD", event_counters.num_of_events_with_1epFDpCD);
    print_value("num_of_events_1epFDpFD", event_counters.num_of_events_with_1epFDpFD);
    print_value("num_of_events_1epCDpCD", event_counters.num_of_events_with_1epCDpCD);
    myLogFile << "\n";

    print_value("num_of_events_pFDpCD", event_counters.num_of_events_pFDpCD);
    myLogFile << "\n";

    print_value("num_of_events_nFDpCD", event_counters.num_of_events_nFDpCD);
    print_value("num_of_events_nFDpCD_AV", event_counters.num_of_events_nFDpCD_AV);
    myLogFile << "\n\n";

    // content of FittedPIDCuts.par file
    if (CutSettings.apply_cuts && !CutSettings.apply_chi2_cuts_1e_cut && (!CutSettings.only_preselection_cuts && CutSettings.only_electron_quality_cuts)) {
        print_section_header("content of FittedPIDCuts.par file");

        DSCuts chi2cuts[] = {CutManager.Chi2_Proton_cuts_CD, CutManager.Chi2_Proton_cuts_FD,  CutManager.Chi2_piplus_cuts_CD,
                             CutManager.Chi2_piplus_cuts_FD, CutManager.Chi2_piminus_cuts_CD, CutManager.Chi2_piminus_cuts_FD};
        int chi2cuts_length = 6;

        myLogFile << "######################################################################\n";
        myLogFile << "# CLAS12 analysis cuts and parameters file (after chi2 Gaussian fit) #\n";
        myLogFile << "######################################################################\n";
        myLogFile << "\n";
        myLogFile << "# pid cuts by detector (pid:mean:sigma) - sigma_CD=" << CutManager.Chi2_Proton_cuts_CD.FitStdFactor << ";sigma_FD=" << CutManager.Chi2_Proton_cuts_FD.FitStdFactor
                  << ":\n";

        for (int i = 0; i < chi2cuts_length; i++) {
            myLogFile << "pid_cuts" << "\t\t" << chi2cuts[i].GetPartPDG() << ":" << chi2cuts[i].Cuts.at(0) << ":" << chi2cuts[i].GetUpperCut() << ":" << chi2cuts[i].GetRegion() << "\n";
        }
    }

    myLogFile.close();
}
#pragma endregion

// PrintExecutionSummary function ---------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* PrintExecutionSummary function */
void RecoAnalyzerPrinter::PrintExecutionSummary(const EventCounters& event_counters, const EventSelectionSettings& ESSettings, const AnalysisCutSettings& CutSettings, const ParticleID& pid,
                                                const std::string& run_plots_path, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles,
                                                const RunParameters& parameters, const std::string& settings_mode) const {
    constexpr int lineWidth = 100;

    std::cout << "\n\n";
    bt::PrintTitleBlock("Execution Summary", '=', lineWidth);

    bt::PrintSectionHeader("Event Counts", lineWidth);
    bt::PrintAlignedValueColored("Total #(QEL events)", event_counters.num_of_QEL_events, lineWidth, 1);
    bt::PrintAlignedValueColored("Total #(MEC events)", event_counters.num_of_MEC_events, lineWidth, 1);
    bt::PrintAlignedValueColored("Total #(RES events)", event_counters.num_of_RES_events, lineWidth, 1);
    bt::PrintAlignedValueColored("Total #(DIS events)", event_counters.num_of_DIS_events, lineWidth, 1);
    bt::PrintAlignedValueColored("QEL + MEC + RES + DIS",
                                 event_counters.num_of_QEL_events + event_counters.num_of_MEC_events + event_counters.num_of_RES_events + event_counters.num_of_DIS_events, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("Total #(events)", event_counters.num_of_events, lineWidth, 1);
    bt::PrintAlignedValueColored("Total #(events) w/o any e", event_counters.num_of_events_without_any_e, lineWidth, 1);
    bt::PrintAlignedValueColored("Total #(events) w/ any e", event_counters.num_of_events_with_any_e, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("#(events) w/ at least 1e", event_counters.num_of_events_with_at_least_1e, lineWidth, 1);
    bt::PrintAlignedValueColored("#(events) w/ more than 1e", event_counters.num_of_events_more_then_1e, lineWidth, 1);
    bt::PrintAlignedValueColored("#(events) w/ exactly 1e", event_counters.num_of_events_with_exactly_1e, lineWidth, 1);
    bt::PrintAlignedValueColored("#(events) w/ exactly 1e (from file)", event_counters.num_of_events_with_exactly_1e_from_file, lineWidth, 1);
    std::cout << "\n";

    if (ESSettings.Count_FD_neurton_and_photon_hits) {
        bt::PrintAlignedValueColored("num_of_events_with_nFD_CLA12", event_counters.num_of_events_with_nFD_CLA12, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_events_with_nFD_CLA12_PCAL", event_counters.num_of_events_with_nFD_CLA12_PCAL, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_events_with_nFD_CLA12_ECIN", event_counters.num_of_events_with_nFD_CLA12_ECIN, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_events_with_nFD_CLA12_ECOUT", event_counters.num_of_events_with_nFD_CLA12_ECOUT, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_events_with_nFD_CLA12_EC", event_counters.num_of_events_with_nFD_CLA12_EC, lineWidth, 1);
        std::cout << "\n";

        bt::PrintAlignedValueColored("num_of_events_with_phFD_CLA12", event_counters.num_of_events_with_phFD_CLA12, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_events_with_phFD_CLA12_PCAL", event_counters.num_of_events_with_phFD_CLA12_PCAL, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_events_with_phFD_CLA12_ECIN", event_counters.num_of_events_with_phFD_CLA12_ECIN, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_events_with_phFD_CLA12_ECOUT", event_counters.num_of_events_with_phFD_CLA12_ECOUT, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_events_with_phFD_CLA12_EC", event_counters.num_of_events_with_phFD_CLA12_EC, lineWidth, 1);
        std::cout << "\n";
    }

    bt::PrintAlignedValueColored("Total #(QEL events) 1e cut", event_counters.num_of_QEL_events_1e_cut, lineWidth, 1);
    bt::PrintAlignedValueColored("Total #(MEC events) 1e cut", event_counters.num_of_MEC_events_1e_cut, lineWidth, 1);
    bt::PrintAlignedValueColored("Total #(RES events) 1e cut", event_counters.num_of_RES_events_1e_cut, lineWidth, 1);
    bt::PrintAlignedValueColored("Total #(DIS events) 1e cut", event_counters.num_of_DIS_events_1e_cut, lineWidth, 1);
    bt::PrintAlignedValueColored(
        "QEL + MEC + RES + DIS (1e cut)",
        event_counters.num_of_QEL_events_1e_cut + event_counters.num_of_MEC_events_1e_cut + event_counters.num_of_RES_events_1e_cut + event_counters.num_of_DIS_events_1e_cut, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("#(events) w/ 1e1p", event_counters.num_of_events_with_1e1p, lineWidth, 1);
    bt::PrintAlignedValueColored("#(events) w/ 1e2p", event_counters.num_of_events_with_1e2p, lineWidth, 1);
    bt::PrintAlignedValueColored("#(events) w/ 1epFDpCD", event_counters.num_of_events_with_1epFDpCD, lineWidth, 1);
    bt::PrintAlignedValueColored("num_of_events_2p_wFakeProtons", event_counters.num_of_events_2p_wFakeProtons, lineWidth, 1);
    std::cout << "\n";

    if (CutSettings.apply_nucleon_cuts) {
        bt::PrintAlignedValueColored("num_of_RM_2p_events_sCTOFhp", pid.num_of_RM_2p_events_sCTOFhp, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_AD_2p_events_from_3p_sCTOFhp", pid.num_of_AD_2p_events_from_3p_sCTOFhp, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_AD_2p_events_from_4p_sCTOFhp", pid.num_of_AD_2p_events_from_4p_sCTOFhp, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_AD_2p_events_from_Xp_sCTOFhp", pid.num_of_AD_2p_events_from_Xp_sCTOFhp, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_RM_2p_events_dCDaFDd", pid.num_of_RM_2p_events_dCDaFDd, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_AD_2p_events_from_3p_dCDaFDd", pid.num_of_AD_2p_events_from_3p_dCDaFDd, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_AD_2p_events_from_4p_dCDaFDd", pid.num_of_AD_2p_events_from_4p_dCDaFDd, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_AD_2p_events_from_Xp_dCDaFDd", pid.num_of_AD_2p_events_from_Xp_dCDaFDd, lineWidth, 1);
        bt::PrintAlignedValueColored("num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd", pid.num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd, lineWidth, 1);

        bt::PrintAlignedValueColored("num_of_events_2p (from monitoring)",
                                     event_counters.num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd +
                                         pid.num_of_AD_2p_events_from_3p_sCTOFhp + pid.num_of_AD_2p_events_from_4p_sCTOFhp + pid.num_of_AD_2p_events_from_3p_dCDaFDd +
                                         pid.num_of_AD_2p_events_from_4p_dCDaFDd,
                                     lineWidth, 1);

        bt::PrintAlignedValueColored("num_of_events_2p (from monitoring; no mixed)",
                                     event_counters.num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd +
                                         pid.num_of_AD_2p_events_from_3p_sCTOFhp + pid.num_of_AD_2p_events_from_4p_sCTOFhp + pid.num_of_AD_2p_events_from_3p_dCDaFDd +
                                         pid.num_of_AD_2p_events_from_4p_dCDaFDd - pid.num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd,
                                     lineWidth, 1);
        std::cout << "\n";
    }

    bt::PrintAlignedValueColored("#(events) 2p", event_counters.num_of_events_2p, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("#(events) pFDpCD", event_counters.num_of_events_pFDpCD, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("num_of_events_1p_inFD", event_counters.num_of_events_1p_inFD, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("num_of_events_1n_inFD", event_counters.num_of_events_1n_inFD, lineWidth, 1);
    bt::PrintAlignedValueColored("num_of_events_1n_inFD_AV", event_counters.num_of_events_1n_inFD_AV, lineWidth, 1);
    bt::PrintAlignedValueColored("num_of_events_1n_inFD_AV_wmt1gTLn", event_counters.num_of_events_1n_inFD_AV_wmt1gTLn, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("num_of_events_2p_wFakeProtons", event_counters.num_of_events_2p_wFakeProtons, lineWidth, 1);
    bt::PrintAlignedValueColored("num_of_events_2p", event_counters.num_of_events_2p, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("num_of_events_1epFDpCD", event_counters.num_of_events_with_1epFDpCD, lineWidth, 1);
    bt::PrintAlignedValueColored("num_of_events_1epFDpFD", event_counters.num_of_events_with_1epFDpFD, lineWidth, 1);
    bt::PrintAlignedValueColored("num_of_events_1epCDpCD", event_counters.num_of_events_with_1epCDpCD, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("#(events) pFDpCD", event_counters.num_of_events_pFDpCD, lineWidth, 1);
    std::cout << "\n";

    bt::PrintAlignedValueColored("num_of_events_nFDpCD", event_counters.num_of_events_nFDpCD, lineWidth, 1);
    bt::PrintAlignedValueColored("num_of_events_nFDpCD_AV", event_counters.num_of_events_nFDpCD_AV, lineWidth, 1);
    std::cout << "\n";

    bt::PrintTitleBlock("Execution Variables", '-', lineWidth);
    bt::PrintAlignedValueColored("WorkingDirectory", pd::PathDefinitions.WorkingDirectory, lineWidth, 1);
    bt::PrintWrappedLabelValueColored("run_plots_path", run_plots_path, lineWidth, 1);
    bt::PrintWrappedLabelValueColored("RecoSamplePath", "/" + RecoSamplePath + "/", lineWidth, 1);
    bt::PrintAlignedValueColored("ReconHipoDir", "/" + ReconHipoDir + "/", lineWidth, 1);
    bt::PrintWrappedLabelValueColored("InputHipoFiles", InputHipoFiles, lineWidth, 1);
    bt::PrintAlignedValueColored("SampleName", parameters.SampleName, lineWidth, 1);
    bt::PrintAlignedValueColored("VaryingSampleName", parameters.VaryingSampleName, lineWidth, 1);
    bt::PrintAlignedValueColored("apply_cuts", "'" + bt::BoolToString(CutSettings.apply_cuts) + "'", lineWidth, 1);
    bt::PrintAlignedValueColored("Settings mode", "'" + settings_mode + "'", lineWidth, 1);
    bt::PrintAlignedValueColored("Beam Energy", bt::ToStringWithPrecision(parameters.beamE, 3) + " [GeV]", lineWidth, 1);
    bt::PrintAlignedValueColored("Target", parameters.Target + " (PDG: " + std::to_string(parameters.TargetPDG) + ")", lineWidth, 1);

    bt::PrintLineColored('=', lineWidth);
    std::cout << env::SYSTEM_COLOR << "\n\n" << env::RESET_COLOR << std::flush;
}
#pragma endregion
