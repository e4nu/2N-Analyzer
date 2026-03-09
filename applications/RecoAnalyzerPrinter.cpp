//
// Created by Alon Sportes on 06/03/2026.
//

#ifndef RECOANALYZERPRINTER_H
#define RECOANALYZERPRINTER_H

#include "RecoAnalyzerPrinter.h"

// RecoCutsPrintOut function --------------------------------------------------------------------------------------------------------------------------------------------

void RecoAnalyzerPrinter::RecoCutsPrintOut(const std::string& workingDirectory, const std::string& run_plots_path, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                                           const std::string& InputHipoFiles, const std::string& file_name, const RunParameters& parameters, const AnalysisCutSettings& CutSettings) const {
    /* Print out the cuts within the run (for self-observation) */
    constexpr int lineWidth = 91;
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

// InitEventPrint function ----------------------------------------------------------------------------------------------------------------------------------------------

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

// PrintEventToFile function --------------------------------------------------------------------------------------------------------------------------------------------

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

// SaveFittedPIDCutsToParFile function ----------------------------------------------------------------------------------------------------------------------------------

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

// SaveNucleonCutsToParFile function ------------------------------------------------------------------------------------------------------------------------------------

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

// SaveSetupToLogFile function ------------------------------------------------------------------------------------------------------------------------------------------

void RecoAnalyzerPrinter::SaveSetupToLogFile(const std::string& run_plots_log_save_Directory, const std::string& run_plots_path, const RunParameters& parameters,
                                             const EventCounters& event_counters, int num_of_good_hipo_files, int total_num_of_hipo_files, std::size_t skipped_hipo_chain_files_size,
                                             const EventSelectionSettings& ESSettings, const AcceptanceMapsSettings& AMapsSettings, const MomentumResolutionSettings& MomResSettings,
                                             const AnalysisCutSettings& CutSettings, const CutValueManager& CutManager, const ParticleID& pid, const clas12ana& clasAna,
                                             const MomentumResolution& nRes, double Pv, double Pvx, double Pvy, double Pvz, bool wider_margin, bool debug_plots, bool Log_scale_Vx_plots,
                                             bool Log_scale_Vy_plots, bool Log_scale_Vz_plots, bool Log_scale_dVx_plots, bool Log_scale_dVy_plots, bool Log_scale_dVz_plots, bool Skip_prob,
                                             bool TestRun, bool Plot_selector_master, bool Cut_plots_master, bool Nphe_plots, bool Chi2_plots, bool Vertex_plots, bool SF_plots,
                                             bool fiducial_plots, bool Momentum_plots, bool ReacMon_plots, bool W_plots, bool Beta_plots, bool Beta_vs_P_plots, bool Angle_plots_master,
                                             bool Theta_e_plots, bool Phi_e_plots, bool Momentum_transfer_plots, bool E_e_plots, bool ETrans_plots_master, bool ETrans_all_plots,
                                             bool ETrans_QEL_plots, bool ETrans_MEC_plots, bool ETrans_RES_plots, bool ETrans_DIS_plots, bool Ecal_plots, bool normalize_master,
                                             bool norm_Nphe_plots, bool norm_Chi2_plots, bool norm_Vertex_plots, bool norm_SF_plots, bool norm_Fiducial_plots, bool norm_Momentum_plots,
                                             bool norm_ReacMon_plots, bool norm_W_plots, bool norm_Beta_plots, bool norm_Angle_plots_master, bool norm_Momentum_transfer_plots,
                                             bool norm_E_e_plots, bool norm_ET_plots, bool norm_Ecal_plots, bool norm_TKI_plots, bool norm_MomRes_plots, bool norm_Multi_plots,
                                             bool delete_png_files, bool delete_root_files, bool Clear_Old_Directories) const {
    std::ofstream myLogFile;
    myLogFile.open(run_plots_log_save_Directory.c_str());
    if (!myLogFile.is_open()) {
        bt::PrintWarning(__func__, __FILE__, __LINE__, "[RecoAnalyzerPrinter] Failed to open log file: " + run_plots_log_save_Directory);
        return;
    }
    constexpr int lineWidth = 91;
    const auto print_value = [&](const std::string& label, const auto& value) {
        std::string normalizedLabel = label;
        while (!normalizedLabel.empty() && (normalizedLabel.back() == ' ' || normalizedLabel.back() == '\t')) { normalizedLabel.pop_back(); }
        while (!normalizedLabel.empty() && (normalizedLabel.back() == ':' || normalizedLabel.back() == '=')) { normalizedLabel.pop_back(); }
        while (!normalizedLabel.empty() && (normalizedLabel.back() == ' ' || normalizedLabel.back() == '\t')) { normalizedLabel.pop_back(); }

        std::ostringstream valueStream;
        valueStream << value;
        const std::string valueStr = valueStream.str();
        const std::string prefix = normalizedLabel + ":";

        const int spaces = lineWidth - static_cast<int>(prefix.size()) - static_cast<int>(valueStr.size());
        if (spaces < 1) {
            myLogFile << prefix << " " << valueStr << "\n";
            return;
        }
        myLogFile << prefix << std::string(spaces, ' ') << valueStr << "\n";
    };

    const auto print_bool = [&](const std::string& label, const bool value) { print_value(label, bt::BoolToString(value)); };

    myLogFile << "///////////////////////////////////////////////////////////////////////////\n";
    myLogFile << "//                          2N Analyzer Output                           //\n";
    myLogFile << "///////////////////////////////////////////////////////////////////////////\n\n";

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
    const std::string gitRepository =
        !gitRemoteOrigin.empty() ? gitRemoteOrigin : (!gitRepositoryTopLevel.empty() ? gitRepositoryTopLevel : "Not available");
    const std::string gitBranchDisplay =
        !gitContextAvailable ? "Not available" : (!gitSymbolicRefBranch.empty() ? gitSymbolicRefBranch : (!gitBranch.empty() ? gitBranch : "DETACHED"));
    const std::string gitDetachedHeadStatus = !gitContextAvailable ? "Not available" : (gitSymbolicRefBranch.empty() ? "true" : "false");
    const std::string gitStatusSummary =
        !gitContextAvailable ? "Not available" : (gitStatusEntries == 0 ? "clean" : ("dirty (" + std::to_string(gitStatusEntries) + " changed entries)"));

    const std::string gitHubRepoUrl = bt::GitHubRepoUrlFromRemote(gitRemoteOrigin);
    const std::string gitRefForLink = !gitFullCommitHash.empty() ? gitFullCommitHash : gitBranchDisplay;
    const std::string gitHubFilesLink =
        (!gitHubRepoUrl.empty() && !gitRefForLink.empty()) ? (gitHubRepoUrl + "/tree/" + gitRefForLink) : "Not available";

    // Git information
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Git information\n";
    myLogFile << "===========================================================================\n\n";

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

    // Input
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Input\n";
    myLogFile << "===========================================================================\n\n";

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
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Plot setup\n";
    myLogFile << "===========================================================================\n\n";

    print_value("wider_margin", bt::BoolToString(wider_margin));
    print_value("debug_plots", bt::BoolToString(debug_plots));
    myLogFile << "\n";

    print_value("Log_scale_Vx_plots", bt::BoolToString(Log_scale_Vx_plots));
    print_value("Log_scale_Vy_plots", bt::BoolToString(Log_scale_Vy_plots));
    print_value("Log_scale_Vz_plots", bt::BoolToString(Log_scale_Vz_plots));
    print_value("Log_scale_dVx_plots", bt::BoolToString(Log_scale_dVx_plots));
    print_value("Log_scale_dVy_plots", bt::BoolToString(Log_scale_dVy_plots));
    print_value("Log_scale_dVz_plots", bt::BoolToString(Log_scale_dVz_plots));
    myLogFile << "\n";

    // Calculation setup
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Calculation setup\n";
    myLogFile << "===========================================================================\n\n";

    print_value("calculate_1p", bt::BoolToString(ESSettings.calculate_1p));
    print_value("calculate_1n", bt::BoolToString(ESSettings.calculate_1n));
    print_value("calculate_2p", bt::BoolToString(ESSettings.calculate_2p));
    myLogFile << "\n";
    print_value("calculate_pFDpCD", bt::BoolToString(ESSettings.calculate_pFDpCD));
    print_value("calculate_nFDpCD", bt::BoolToString(ESSettings.calculate_nFDpCD));
    myLogFile << "\n";

    myLogFile << "-- Truth level calculation setup ------------------------------------------\n";
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

    myLogFile << "-- FD neutrals settings ---------------------------------------------------\n";
    print_value("Enable_FD_photons", bt::BoolToString(ESSettings.Enable_FD_photons));
    print_value("Enable_FD_neutrons", bt::BoolToString(ESSettings.Enable_FD_neutrons));
    print_value("Count_FD_neurton_and_photon_hits", bt::BoolToString(ESSettings.Count_FD_neurton_and_photon_hits));

    print_value("ES_by_leading_FDneutron", bt::BoolToString(ESSettings.ES_by_leading_FDneutron));
    myLogFile << "\n";

    myLogFile << "-- AMaps settings ---------------------------------------------------------\n";
    print_value("Generate_Electron_AMaps", bt::BoolToString(AMapsSettings.Generate_Electron_AMaps));
    print_value("Generate_Nucleon_AMaps", bt::BoolToString(AMapsSettings.Generate_Nucleon_AMaps));
    print_value("Generate_WMaps", bt::BoolToString(AMapsSettings.Generate_WMaps));
    print_value("AMaps_calc_with_one_reco_electron", bt::BoolToString(AMapsSettings.AMaps_calc_with_one_reco_electron));
    print_value("P_e_bin_profile", AMapsSettings.P_e_bin_profile);
    print_value("P_nuc_bin_profile", AMapsSettings.P_nuc_bin_profile);
    print_value("Electron_single_slice_test", bt::BoolToString(AMapsSettings.Electron_single_slice_test));
    print_value("Nucleon_single_slice_test", bt::BoolToString(AMapsSettings.Nucleon_single_slice_test));
    myLogFile << "TestSlices = {" << AMapsSettings.TestSlices.at(0) << ", " << AMapsSettings.TestSlices.at(1) << ", " << AMapsSettings.TestSlices.at(2) << "}\n\n";

    myLogFile << "-- nRES settings ----------------------------------------------------------\n";
    print_value("plot_and_fit_MomRes", bt::BoolToString(MomResSettings.plot_and_fit_MomRes));
    print_value("Calculate_momResS2", bt::BoolToString(MomResSettings.Calculate_momResS2));
    print_value("DeltaSlices", MomResSettings.DeltaSlices);
    print_value("VaryingDelta", bt::BoolToString(MomResSettings.VaryingDelta));
    print_value("ForceSmallpResLimits", bt::BoolToString(MomResSettings.ForceSmallpResLimits));
    print_value("SmearMode", MomResSettings.SmearMode);
    print_value("CorrMode", MomResSettings.CorrMode);
    print_value("Run_with_momResS2", bt::BoolToString(MomResSettings.Run_with_momResS2));
    print_value("momRes_test", bt::BoolToString(MomResSettings.momRes_test));
    myLogFile << "\n";

    myLogFile << "-- Other run parameters ---------------------------------------------------\n";
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
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Directory settings\n";
    myLogFile << "===========================================================================\n\n";

    print_value("Clear_Old_Directories", bt::BoolToString(Clear_Old_Directories));
    myLogFile << "\n";

    print_value("Save_Plots_folder_to_zip_files", bt::BoolToString(Save_Plots_folder_to_zip_files));
    print_value("zipping_print_out", bt::BoolToString(zipping_print_out));

    // Plot selector
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Plot selector\n";
    myLogFile << "===========================================================================\n\n";

    print_value("Skip_prob", bt::BoolToString(Skip_prob));

    print_value("TestRun", bt::BoolToString(TestRun));

    print_value("Plot_selector_master", bt::BoolToString(Plot_selector_master));

    myLogFile << "\n-- Cut variable plots -----------------------------------------------------" << "\n";
    print_value("Cut_plots_master", bt::BoolToString(Cut_plots_master));
    print_value("Nphe_plots", bt::BoolToString(Nphe_plots));
    print_value("Chi2_plots", bt::BoolToString(Chi2_plots));
    print_value("Vertex_plots", bt::BoolToString(Vertex_plots));
    print_value("SF_plots", bt::BoolToString(SF_plots));
    print_value("fiducial_plots", bt::BoolToString(fiducial_plots));
    print_value("Momentum_plots", bt::BoolToString(Momentum_plots));

    myLogFile << "\n-- Reaction monitoring plots ----------------------------------------------" << "\n";
    print_value("ReacMon_plots", bt::BoolToString(ReacMon_plots));

    myLogFile << "\n-- W plots ----------------------------------------------------------------" << "\n";
    print_value("W_plots", bt::BoolToString(W_plots));

    myLogFile << "\n-- Beta plots -------------------------------------------------------------" << "\n";
    print_value("Beta_plots", bt::BoolToString(Beta_plots));
    print_value("Beta_vs_P_plots", bt::BoolToString(Beta_vs_P_plots));

    myLogFile << "\n-- Angle plots ------------------------------------------------------------" << "\n";
    print_value("Angle_plots_master", bt::BoolToString(Angle_plots_master));
    print_value("Theta_e_plots", bt::BoolToString(Theta_e_plots));
    print_value("Phi_e_plots", bt::BoolToString(Phi_e_plots));

    myLogFile << "\n-- Momentum transfer plots ------------------------------------------------" << "\n";
    print_value("Momentum_transfer_plots", bt::BoolToString(Momentum_transfer_plots));

    myLogFile << "\n-- E_e plots --------------------------------------------------------------" << "\n";
    print_value("E_e_plots", bt::BoolToString(E_e_plots));

    myLogFile << "\n-- ET plots ---------------------------------------------------------------" << "\n";
    print_value("ETrans_plots_master", bt::BoolToString(ETrans_plots_master));
    print_value("ETrans_all_plots", bt::BoolToString(ETrans_all_plots));
    print_value("ETrans_QEL_plots", bt::BoolToString(ETrans_QEL_plots));
    print_value("ETrans_MEC_plots", bt::BoolToString(ETrans_MEC_plots));
    print_value("ETrans_RES_plots", bt::BoolToString(ETrans_RES_plots));
    print_value("ETrans_DIS_plots", bt::BoolToString(ETrans_DIS_plots));

    myLogFile << "\n-- E_cal plots ------------------------------------------------------------" << "\n";
    print_value("E_cal_plots", bt::BoolToString(Ecal_plots));

    // Normalization setup
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Normalization setup\n";
    myLogFile << "===========================================================================\n\n";

    print_value("normalize_master", bt::BoolToString(normalize_master));
    myLogFile << "\n";

    print_value("norm_Nphe_plots", bt::BoolToString(norm_Nphe_plots));
    print_value("norm_Chi2_plots", bt::BoolToString(norm_Chi2_plots));
    print_value("norm_Vertex_plots", bt::BoolToString(norm_Vertex_plots));
    print_value("norm_SF_plots", bt::BoolToString(norm_SF_plots));
    print_value("norm_Fiducial_plots", bt::BoolToString(norm_Fiducial_plots));
    print_value("norm_Momentum_plots", bt::BoolToString(norm_Momentum_plots));
    myLogFile << "\n";

    print_value("norm_ReacMon_plots", bt::BoolToString(norm_ReacMon_plots));
    myLogFile << "\n";

    print_value("norm_W_plots", bt::BoolToString(norm_W_plots));
    print_value("norm_Beta_plots", bt::BoolToString(norm_Beta_plots));
    print_value("norm_Angle_plots_master", bt::BoolToString(norm_Angle_plots_master));
    print_value("norm_Momentum_transfer_plots", bt::BoolToString(norm_Momentum_transfer_plots));
    print_value("norm_E_e_plots", bt::BoolToString(norm_E_e_plots));
    print_value("norm_ET_plots", bt::BoolToString(norm_ET_plots));
    print_value("norm_Ecal_plots", bt::BoolToString(norm_Ecal_plots));
    print_value("norm_TKI_plots", bt::BoolToString(norm_TKI_plots));
    myLogFile << "\n";

    print_value("norm_MomRes_plots", bt::BoolToString(norm_MomRes_plots));
    print_value("norm_Multi_plots", bt::BoolToString(norm_Multi_plots));
    myLogFile << "\n";

    // Delete setup
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Delete setup\n";
    myLogFile << "===========================================================================\n\n";

    print_value("delete_png_files", bt::BoolToString(delete_png_files));
    print_value("delete_root_files", bt::BoolToString(delete_root_files));
    myLogFile << "\n";

    // Cut application setup
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Cut application setup\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "-- clas12ana cuts ---------------------------------------------------------\n";
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

    myLogFile << "-- My analysis cuts -------------------------------------------------------\n";
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

    print_bool("apply_reaction_monitoring_cuts:\t", CutSettings.apply_reaction_monitoring_cuts);
    print_bool("apply_P_miss_in_QE_range_cuts:\t", CutSettings.apply_P_miss_in_QE_range_cuts);
    print_bool("apply_E_miss_in_QE_range_cuts:\t", CutSettings.apply_E_miss_in_QE_range_cuts);
    print_bool("apply_P_miss_in_MECandSRC_range_cuts:\t", CutSettings.apply_P_miss_in_MECandSRC_range_cuts);
    print_bool("apply_E_miss_in_MECandSRC_range_cuts:\t", CutSettings.apply_E_miss_in_MECandSRC_range_cuts);
    print_bool("apply_xB_in_QE_range_cuts:\t", CutSettings.apply_xB_in_QE_range_cuts);
    myLogFile << "apply_theta_q_pCD_in_lower_FSI_range_cut:\t" << bt::BoolToString(CutSettings.apply_theta_q_pCD_in_lower_FSI_range_cut) << "\n\n";

    // cuts

    // #Photo-electrons (Nphe) cuts (electrons only, FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "#Photo-electrons (Nphe) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Nphe_lower_cut:" << std::setw(59) << CutManager.Nphe_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Nphe_upper_cut:" << std::setw(59) << CutManager.Nphe_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2 cuts
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Chi2 cuts\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "\n-- Electrons --------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Electrons in FD (1e cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_Electron_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_Electron_cuts_FD.GetUpperCut() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_Electron_cuts_FD.MeanFromHistogram << "\n\n";

    // Chi2_Proton
    myLogFile << "\n\n-- Protons ----------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Protons in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_Proton_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_Proton_cuts_CD.GetUpperCut() / CutManager.Chi2_Proton_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_Proton_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_Proton_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_Proton_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_Proton_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- Protons in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_Proton_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_Proton_cuts_FD.GetUpperCut() / CutManager.Chi2_Proton_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_Proton_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_Proton_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_Proton_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_Proton_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_Kplus
    myLogFile << "\n-- K+ ---------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Kplus in CD (1e cut) ---------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_Kplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_Kplus_cuts_CD.GetUpperCut() / CutManager.Chi2_Kplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_Kplus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_Kplus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_Kplus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_Kplus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- Kplus in FD (1e cut) ---------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_Kplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_Kplus_cuts_FD.GetUpperCut() / CutManager.Chi2_Kplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_Kplus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_Kplus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_Kplus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_Kplus_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_Kminus
    myLogFile << "\n-- K- ---------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Kminus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_Kminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_Kminus_cuts_CD.GetUpperCut() / CutManager.Chi2_Kminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_Kminus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_Kminus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_Kminus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_Kminus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- Kminus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_Kminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_Kminus_cuts_FD.GetUpperCut() / CutManager.Chi2_Kminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_Kminus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_Kminus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_Kminus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_Kminus_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_piplus
    myLogFile << "\n-- pi+ --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- piplus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_piplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_piplus_cuts_CD.GetUpperCut() / CutManager.Chi2_piplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_piplus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_piplus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_piplus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_piplus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- piplus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_piplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_piplus_cuts_FD.GetUpperCut() / CutManager.Chi2_piplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_piplus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_piplus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_piplus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_piplus_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_piminus
    myLogFile << "\n-- pi- --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- piminus in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_piminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_piminus_cuts_CD.GetUpperCut() / CutManager.Chi2_piminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_piminus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_piminus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_piminus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_piminus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- piminus in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_piminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_piminus_cuts_FD.GetUpperCut() / CutManager.Chi2_piminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_piminus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_piminus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_piminus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_piminus_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_deuteron
    myLogFile << "\n-- deuteron ---------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- deuteron in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_deuteron_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_deuteron_cuts_CD.GetUpperCut() / CutManager.Chi2_deuteron_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_deuteron_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_deuteron_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_deuteron_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_deuteron_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- deuteron in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << CutManager.Chi2_deuteron_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << CutManager.Chi2_deuteron_cuts_FD.GetUpperCut() / CutManager.Chi2_deuteron_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << CutManager.Chi2_deuteron_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << CutManager.Chi2_deuteron_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << CutManager.Chi2_deuteron_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << CutManager.Chi2_deuteron_cuts_FD.GetUpperCut() << "\n\n";

    // Vertex cuts (CD & FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "V cuts (CD & FD)\n";
    myLogFile << "===========================================================================\n\n";

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
    myLogFile << "\n===========================================================================\n";
    myLogFile << "dV cuts (CD & FD)\n";
    myLogFile << "===========================================================================\n\n";

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
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Sampling Fraction (SF) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    print_value("SF_1e_upper_cut", clasAna.getEcalSFUpperCut());
    print_value("SF_1e_lower_cut", clasAna.getEcalSFLowerCut());

    // ECAL fiducial cuts (electrons only, FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "ECAL fiducial cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    print_value("fiducial_cut_Lv", clasAna.getEcalEdgeCuts());
    print_value("fiducial_cut_Lw", clasAna.getEcalEdgeCuts());
    myLogFile << "\n";

    // Momentum thresholds
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Momentum thresholds {min, max}\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "e_mom_th = {" << CutManager.e_mom_th.GetLowerCut() << ", " << CutManager.e_mom_th.GetUpperCut() << "}\n";
    myLogFile << "p_mom_th = {" << CutManager.p_mom_th.GetLowerCut() << ", " << CutManager.p_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pFD_mom_th = {" << CutManager.pFD_mom_th.GetLowerCut() << ", " << CutManager.pFD_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pCD_mom_th = {" << CutManager.pCD_mom_th.GetLowerCut() << ", " << CutManager.pCD_mom_th.GetUpperCut() << "}\n";
    myLogFile << "n_mom_th = {" << CutManager.n_mom_th.GetLowerCut() << ", " << CutManager.n_mom_th.GetUpperCut() << "}\n";
    myLogFile << "ph_mom_th = {" << CutManager.ph_mom_th.GetLowerCut() << ", " << CutManager.ph_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pip_mom_th = {" << CutManager.pip_mom_th.GetLowerCut() << ", " << CutManager.pip_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pipFD_mom_th = {" << CutManager.pipFD_mom_th.GetLowerCut() << ", " << CutManager.pipFD_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pipCD_mom_th = {" << CutManager.pipCD_mom_th.GetLowerCut() << ", " << CutManager.pipCD_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pim_mom_th = {" << CutManager.pim_mom_th.GetLowerCut() << ", " << CutManager.pim_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pimFD_mom_th = {" << CutManager.pimFD_mom_th.GetLowerCut() << ", " << CutManager.pimFD_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pimCD_mom_th = {" << CutManager.pimCD_mom_th.GetLowerCut() << ", " << CutManager.pimCD_mom_th.GetUpperCut() << "}\n\n";

    // Momentum cuts
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Momentum cuts {min, max}\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "\n-- Momentum cuts (1n, FD only) --------------------------------------------" << "\n";
    myLogFile << "Neutrons (n_momentum_cuts - ECAL):\t\t{" << CutManager.n_momentum_cuts_ABF_FD_n_from_ph.GetLowerCut() << ", " << CutManager.n_momentum_cuts_ABF_FD_n_from_ph.GetUpperCut()
              << "}\n";
    myLogFile << "Neutrons (n_momentum_cuts - ECAL apprax):\t{" << CutManager.n_momentum_cuts_ABF_FD_n_from_ph_apprax.GetLowerCut() << ", "
              << CutManager.n_momentum_cuts_ABF_FD_n_from_ph_apprax.GetUpperCut() << "}\n";

    // TL Momentum thresholds
    myLogFile << "\n===========================================================================\n";
    myLogFile << "TL Momentum thresholds {min, max}\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "TL_e_mom_cuts:\t{" << CutManager.TL_e_mom_cuts.GetLowerCut() << ", " << CutManager.TL_e_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_n_mom_cuts:\t{" << CutManager.TL_n_mom_cuts.GetLowerCut() << ", " << CutManager.TL_n_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_p_mom_cuts:\t{" << CutManager.TL_p_mom_cuts.GetLowerCut() << ", " << CutManager.TL_p_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pFD_mom_cuts:\t{" << CutManager.TL_pFD_mom_cuts.GetLowerCut() << ", " << CutManager.TL_pFD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pCD_mom_cuts:\t{" << CutManager.TL_pCD_mom_cuts.GetLowerCut() << ", " << CutManager.TL_pCD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pip_mom_cuts:\t{" << CutManager.TL_pip_mom_cuts.GetLowerCut() << ", " << CutManager.TL_pip_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pipFD_mom_cuts:\t{" << CutManager.TL_pipFD_mom_cuts.GetLowerCut() << ", " << CutManager.TL_pipFD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pipCD_mom_cuts:\t{" << CutManager.TL_pipCD_mom_cuts.GetLowerCut() << ", " << CutManager.TL_pipCD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pim_mom_cuts:\t{" << CutManager.TL_pim_mom_cuts.GetLowerCut() << ", " << CutManager.TL_pim_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pimFD_mom_cuts:\t{" << CutManager.TL_pimFD_mom_cuts.GetLowerCut() << ", " << CutManager.TL_pimFD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pimCD_mom_cuts:\t{" << CutManager.TL_pimCD_mom_cuts.GetLowerCut() << ", " << CutManager.TL_pimCD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pi0_mom_cuts:\t{" << CutManager.TL_pi0_mom_cuts.GetLowerCut() << ", " << CutManager.TL_pi0_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_ph_mom_cuts:\t{" << CutManager.TL_ph_mom_cuts.GetLowerCut() << ", " << CutManager.TL_ph_mom_cuts.GetUpperCut() << "}\n\n";

    // Beta cut (1n, FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Beta cut (1n, FD)\n";
    myLogFile << "===========================================================================\n\n";

    print_value("Beta_cut.GetUpperCut() (1 fit std)", CutManager.Beta_cut.GetUpperCut());
    print_value("Beta_cut.GetMean()", CutManager.Beta_cut.GetMean());
    myLogFile << "\n";

    // Nucleon kinematical cuts (FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Nucleon kinematical cuts (FD)\n";
    myLogFile << "===========================================================================\n\n";

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
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Neutron veto cut (1n & nFDpCD, FD)\n";
    myLogFile << "===========================================================================\n\n";

    print_value("Neutron_veto_cut.GetLowerCut()", CutManager.Neutron_veto_cut.GetLowerCut());
    myLogFile << "\n";

    // Ghost tracks handling (2p, CD & FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Ghost tracks handling (2p, CD & FD)\n";
    myLogFile << "===========================================================================\n\n";

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
    myLogFile << "\n===========================================================================\n";
    myLogFile << "momRes correction and smearing coefficients\n";
    myLogFile << "===========================================================================\n";

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

    myLogFile << "\n-- Neutron correction -----------------------------------------------------" << "\n";
    print_value("CorrMode", nRes.Get_CorrMode());
    myLogFile << "\n";
    myLogFile << "Correction loading path:\n" << nRes.Get_Loaded_Corr_coefficients_path() << "\n\n";

    std::vector<double> Corr_coefficients_values = nRes.Get_Loaded_Corr_coefficients_values();
    std::vector<std::string> Corr_coefficients_names = nRes.Get_Loaded_Corr_coefficients_names();

    for (std::size_t i = 0; i < Corr_coefficients_values.size(); i++) { myLogFile << Corr_coefficients_names.at(i) << " = " << Corr_coefficients_values.at(i) << "\n"; }

    myLogFile << "\n-- Proton smearing --------------------------------------------------------" << "\n";
    print_value("SmearMode", nRes.Get_SmearMode());
    myLogFile << "\n";
    myLogFile << "Smearing loading path:\n" << nRes.Get_Loaded_Std_coefficients_path() << "\n\n";

    std::vector<double> Std_coefficients_values = nRes.Get_Loaded_Smear_coefficients_values();
    std::vector<std::string> Std_coefficients_names = nRes.Get_Loaded_Smear_coefficients_names();

    for (std::size_t i = 0; i < Std_coefficients_values.size(); i++) { myLogFile << Std_coefficients_names.at(i) << " = " << Std_coefficients_values.at(i) << "\n"; }

    print_value("\nRandom number generator", nRes.GetRandNumGenerator());
    myLogFile << "\n";

    // Event counts
    myLogFile << "===========================================================================\n";
    myLogFile << "Event counts\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "-- Total counts -----------------------------------------------------------\n";
    myLogFile << "Total #(events):\t\t\t\t" << event_counters.num_of_events << "\n";
    myLogFile << "Total #(events) w/o any e:\t\t\t" << event_counters.num_of_events_without_any_e << "\n";
    myLogFile << "Total #(events) w/ any e:\t\t\t" << event_counters.num_of_events_with_any_e << "\n\n";

    myLogFile << "Total #(QEL events):\t\t\t\t" << event_counters.num_of_QEL_events << "\n";
    myLogFile << "Total #(MEC events):\t\t\t\t" << event_counters.num_of_MEC_events << "\n";
    myLogFile << "Total #(RES events):\t\t\t\t" << event_counters.num_of_RES_events << "\n";
    myLogFile << "Total #(DIS events):\t\t\t\t" << event_counters.num_of_DIS_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS:\t\t\t\t" << event_counters.num_of_QEL_events + event_counters.num_of_MEC_events + event_counters.num_of_RES_events + event_counters.num_of_DIS_events
              << "\n\n";

    myLogFile << "-- Events with electrons counts -------------------------------------------\n";
    myLogFile << "#(events) w/ at least 1e:\t\t\t" << event_counters.num_of_events_with_at_least_1e << "\n";
    myLogFile << "#(events) w/ more then 1e:\t\t\t" << event_counters.num_of_events_more_then_1e << "\n";
    myLogFile << "#(events) w/ exactly 1e:\t\t\t" << event_counters.num_of_events_with_exactly_1e << "\n";
    myLogFile << "#(events) w/ exactly 1e (from file):\t\t" << event_counters.num_of_events_with_exactly_1e_from_file << "\n\n";

    myLogFile << "Total #(QEL events) 1e cut:\t\t\t" << event_counters.num_of_QEL_events_1e_cut << "\n";
    myLogFile << "Total #(MEC events) 1e cut:\t\t\t" << event_counters.num_of_MEC_events_1e_cut << "\n";
    myLogFile << "Total #(RES events) 1e cut:\t\t\t" << event_counters.num_of_RES_events_1e_cut << "\n";
    myLogFile << "Total #(DIS events) 1e cut:\t\t\t" << event_counters.num_of_DIS_events_1e_cut << "\n";
    myLogFile << "QEL + MEC + RES + DIS:\t\t\t\t"
              << event_counters.num_of_QEL_events_1e_cut + event_counters.num_of_MEC_events_1e_cut + event_counters.num_of_RES_events_1e_cut + event_counters.num_of_DIS_events_1e_cut
              << "\n\n";

    if (ESSettings.Count_FD_neurton_and_photon_hits) {
        myLogFile << "-- FD neutron hit counts in ECAL ------------------------------------------\n";
        myLogFile << "num_of_events_with_nFD_CLA12:\t\t" << event_counters.num_of_events_with_nFD_CLA12 << "\n";
        myLogFile << "num_of_events_with_nFD_CLA12_PCAL:\t" << event_counters.num_of_events_with_nFD_CLA12_PCAL << "\n";
        myLogFile << "num_of_events_with_nFD_CLA12_ECIN:\t" << event_counters.num_of_events_with_nFD_CLA12_ECIN << "\n";
        myLogFile << "num_of_events_with_nFD_CLA12_ECOUT:\t" << event_counters.num_of_events_with_nFD_CLA12_ECOUT << "\n";
        myLogFile << "num_of_events_with_nFD_CLA12_EC:\t" << event_counters.num_of_events_with_nFD_CLA12_EC << "\n\n";

        myLogFile << "-- FD photon hit counts in ECAL -------------------------------------------\n";
        myLogFile << "num_of_events_with_phFD_CLA12:\t\t" << event_counters.num_of_events_with_phFD_CLA12 << "\n";
        myLogFile << "num_of_events_with_phFD_CLA12_PCAL:\t" << event_counters.num_of_events_with_phFD_CLA12_PCAL << "\n";
        myLogFile << "num_of_events_with_phFD_CLA12_ECIN:\t" << event_counters.num_of_events_with_phFD_CLA12_ECIN << "\n";
        myLogFile << "num_of_events_with_phFD_CLA12_ECOUT:\t" << event_counters.num_of_events_with_phFD_CLA12_ECOUT << "\n";
        myLogFile << "num_of_events_with_phFD_CLA12_EC:\t" << event_counters.num_of_events_with_phFD_CLA12_EC << "\n\n";
    }

    myLogFile << "-- Counting events with FD neutrons ---------------------------------------\n";
    myLogFile << "#(events) w/ 1 FD neutrons:\t\t" << event_counters.num_of_events_1n_in_FD << "\n";
    myLogFile << "#(events) w/ 2 FD neutrons:\t\t" << event_counters.num_of_events_2n_in_FD << "\n";
    myLogFile << "#(events) w/ 3 FD neutrons:\t\t" << event_counters.num_of_events_3n_in_FD << "\n";
    myLogFile << "#(events) w/ more than 3 FD neutrons:\t" << event_counters.num_of_events_Xn_in_FD << "\n\n";

    myLogFile << "-- 1e1p event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e1p:\t\t\t\t" << event_counters.num_of_events_with_1e1p << "\n\n";

    myLogFile << "-- 1e2p event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e2p:\t\t\t\t" << event_counters.num_of_events_with_1e2p << "\n\n";

    myLogFile << "-- 1epFDpCD event counts --------------------------------------------------\n";
    myLogFile << "#(events) w/ 1epFDpCD:\t\t\t\t" << event_counters.num_of_events_with_1epFDpCD << "\n\n";

    myLogFile << "-- 2p event counts --------------------------------------------------------\n";
    myLogFile << "num_of_events_2p_wFakeProtons:\t\t\t" << event_counters.num_of_events_2p_wFakeProtons << "\n\n";

    if (CutSettings.apply_nucleon_cuts) {
        myLogFile << "num_of_RM_2p_events_sCTOFhp:\t\t\t" << pid.num_of_RM_2p_events_sCTOFhp << "\n";
        myLogFile << "num_of_AD_2p_events_from_3p_sCTOFhp:\t\t" << pid.num_of_AD_2p_events_from_3p_sCTOFhp << "\n";
        myLogFile << "num_of_AD_2p_events_from_4p_sCTOFhp:\t\t" << pid.num_of_AD_2p_events_from_4p_sCTOFhp << "\n";
        myLogFile << "num_of_AD_2p_events_from_Xp_sCTOFhp:\t\t" << pid.num_of_AD_2p_events_from_Xp_sCTOFhp << "\n";
        myLogFile << "num_of_RM_2p_events_dCDaFDd:\t\t\t" << pid.num_of_RM_2p_events_dCDaFDd << "\n";
        myLogFile << "num_of_AD_2p_events_from_3p_dCDaFDd:\t\t" << pid.num_of_AD_2p_events_from_3p_dCDaFDd << "\n";
        myLogFile << "num_of_AD_2p_events_from_4p_dCDaFDd:\t\t" << pid.num_of_AD_2p_events_from_4p_dCDaFDd << "\n";
        myLogFile << "num_of_AD_2p_events_from_Xp_dCDaFDd:\t\t" << pid.num_of_AD_2p_events_from_Xp_dCDaFDd << "\n\n";

        myLogFile << "num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd:\t\t" << pid.num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd << "\n\n";

        myLogFile << "num_of_events_2p (from monitoring):\t\t"
                  << event_counters.num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd + pid.num_of_AD_2p_events_from_3p_sCTOFhp +
                         pid.num_of_AD_2p_events_from_4p_sCTOFhp + pid.num_of_AD_2p_events_from_3p_dCDaFDd + pid.num_of_AD_2p_events_from_4p_dCDaFDd
                  << "\n\n";

        myLogFile << "num_of_events_2p (from monitoring; no mixed):\t\t"
                  << event_counters.num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd + pid.num_of_AD_2p_events_from_3p_sCTOFhp +
                         pid.num_of_AD_2p_events_from_4p_sCTOFhp + pid.num_of_AD_2p_events_from_3p_dCDaFDd + pid.num_of_AD_2p_events_from_4p_dCDaFDd -
                         pid.num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd
                  << "\n\n";
    }

    myLogFile << "#(events) 2p:\t\t\t\t\t" << event_counters.num_of_events_2p << "\n\n";

    myLogFile << "-- pFDpCD event counts ----------------------------------------------------\n";
    myLogFile << "#(events) pFDpCD:\t\t\t\t" << event_counters.num_of_events_pFDpCD << "\n\n";

    myLogFile << "-- Event counts -----------------------------------------------------------\n";
    myLogFile << "num_of_events_1p_inFD:\t\t\t\t" << event_counters.num_of_events_1p_inFD << "\n\n";

    myLogFile << "num_of_events_1n_inFD:\t\t\t\t" << event_counters.num_of_events_1n_inFD << "\n";
    myLogFile << "num_of_events_1n_inFD_AV:\t\t\t" << event_counters.num_of_events_1n_inFD_AV << "\n";
    myLogFile << "num_of_events_1n_inFD_AV_wmt1gTLn:\t\t" << event_counters.num_of_events_1n_inFD_AV_wmt1gTLn << "\n\n";

    myLogFile << "num_of_events_2p_wFakeProtons:\t\t\t" << event_counters.num_of_events_2p_wFakeProtons << "\n";
    myLogFile << "num_of_events_2p:\t\t\t\t" << event_counters.num_of_events_2p << "\n\n";

    myLogFile << "num_of_events_1epFDpCD:\t\t\t\t" << event_counters.num_of_events_with_1epFDpCD << "\n";
    myLogFile << "num_of_events_1epFDpFD:\t\t\t\t" << event_counters.num_of_events_with_1epFDpFD << "\n";
    myLogFile << "num_of_events_1epCDpCD:\t\t\t\t" << event_counters.num_of_events_with_1epCDpCD << "\n\n";

    myLogFile << "num_of_events_pFDpCD:\t\t\t\t" << event_counters.num_of_events_pFDpCD << "\n\n";

    myLogFile << "num_of_events_nFDpCD:\t\t\t\t" << event_counters.num_of_events_nFDpCD << "\n";
    myLogFile << "num_of_events_nFDpCD_AV:\t\t\t" << event_counters.num_of_events_nFDpCD_AV << "\n\n\n";

    // content of FittedPIDCuts.par file
    if (CutSettings.apply_cuts && !CutSettings.apply_chi2_cuts_1e_cut && (!CutSettings.only_preselection_cuts && CutSettings.only_electron_quality_cuts)) {
        myLogFile << "===========================================================================\n";
        myLogFile << "content of FittedPIDCuts.par file\n";
        myLogFile << "===========================================================================\n\n";

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

// PrintExecutionSummary function ---------------------------------------------------------------------------------------------------------------------------------------

void RecoAnalyzerPrinter::PrintExecutionSummary(const EventCounters& event_counters, const EventSelectionSettings& ESSettings, const AnalysisCutSettings& CutSettings, const ParticleID& pid,
                                                const std::string& run_plots_path, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles,
                                                const RunParameters& parameters, const std::string& settings_mode) const {
    constexpr int lineWidth = 91;

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

#endif  // RECOANALYZERPRINTER_H
