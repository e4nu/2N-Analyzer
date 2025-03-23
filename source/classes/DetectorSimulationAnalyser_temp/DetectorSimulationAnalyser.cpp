#include "HistogramSetup.cpp"
// #include "DetectorSimulationAnalyser.h"

void DetectorSimulationAnalyser::EventAnalyser(const string &AnalyseFilePath, const string &AnalyseFileSample, const string &AnalyseFile) {
    CodeDebugger.PrintStepTester(DebuggerMode);

    cout << "\033[33m\n\n===========================================================================\n\033[0m";
    cout << "\033[33m\t\t\tDetector simulation analyser\n\033[0m";
    cout << "\033[33m===========================================================================\n\n\033[0m";

    CodeSetup(AnalyseFilePath, AnalyseFileSample, AnalyseFile);
    ProcessEvents();
    SavePlots();
}

void DetectorSimulationAnalyser::CodeSetup(const string &AnalyseFilePath, const string &AnalyseFileSample, const string &AnalyseFile) {
    CodeDebugger.PrintStepTester(DebuggerMode);

    InputProcessing(AnalyseFilePath, AnalyseFileSample, AnalyseFile);
    EventSelectionSetup();
    CutSetup();
    DirectorySetup();
    PlotSelectorSetup();
    NormalizationSetup();
    DeleteSetup();
    HistogramSetup();
    AcceptanceMapsSetup();
    AcceptanceCorrectionSetup();
    NeutronResolutionSetup();
    DebuggingSetup();
}

void DetectorSimulationAnalyser::InputProcessing(const string &AnalyseFilePath, const string &AnalyseFileSample, const string &AnalyseFile) {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // ======================================================================================================================================================================
    // Input processing
    // ======================================================================================================================================================================

    // Input processing
    /* Initial input processing of loaded files (given by AnalyseFile) */

    /* Determine file path and name */
    this->LoadedInput = AnalyseFile;  // AnalyseFile is taken from codeSetup.h
    this->filePath = LoadedInput.substr(0, LoadedInput.find_last_of('/') + 1);
    this->fileInput = LoadedInput.substr(LoadedInput.find_last_of('/') + 1);
    this->plotsInput = fileInput.substr(0, fileInput.find_last_of(".hipo") - 4);

    /* Configure and get run parameters */
    this->Experiment = Experiment(AnalyseFilePath, AnalyseFileSample);
    this->SampleName = Experiment.ConfigureSampleName(AnalyseFilePath, AnalyseFileSample);  // Configure SampleName from input
    this->VaryingSampleName = Experiment.GetVaryingSampleName();                            // Get VaryingSampleName (configured from SampleName) - for data runs!
    this->beamE = Experiment.GetBeamEnergy();                                               // Configure beam energy from SampleName
    this->Target = Experiment.GetTargetElement();                                           // Configure target (element) from SampleName
    this->TargetPDG = Experiment.GetTargetElementPDG();                                     // Configure target PDG from SampleName
    this->isLocal = Experiment.SLocal();
    this->isMC = Experiment.SSample();
    this->isData = Experiment.DSample();
    this->is2GeVSample = Experiment.IsBeamAt2GeV();
    this->is4GeVSample = Experiment.IsBeamAt4GeV();
    this->is6GeVSample = Experiment.IsBeamAt6GeV();
}

void DetectorSimulationAnalyser::EventSelectionSetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // ======================================================================================================================================================================
    // Event selection setup
    // ======================================================================================================================================================================

    // Event selection setup
    /* Settings to enable/disable specific FS plot calculations (Rec only): */

    /* Final states to analyse (1N & 2N) */
    this->calculate_1p = true, this->calculate_1n = true;
    this->calculate_2p = true, this->calculate_pFDpCD = true, this->calculate_nFDpCD = true;

    /* Truth level calculation setup */
    this->calculate_truth_level = true;  // TL master ON/OFF switch
    this->TL_plots_only_for_NC = false;  // TL plots only AFTER beta fit
    this->fill_TL_plots = true;
    this->ZoomIn_On_mom_th_plots = false;          // momentum th. efficiencies with zoomin
    this->Eff_calc_with_one_reco_electron = true;  // keep as true in normal runs
    this->Calc_inc_eff_with_varying_theta = false;
    this->Calc_1n_n_eff_with_smaller_theta = false;
    this->Calc_eff_overlapping_FC = true;  // keep as true in normal runs
    this->Rec_wTL_ES = true;               // Calculate efficiency - force TL event selection on reco. plots

    this->limless_mom_eff_plots = false;

    /* FD neutrals settings */
    this->Enable_FD_photons = false;  // keep as false to decrease RES & DIS
    this->Enable_FD_neutrons = true;  // keep as false to increse eff. plots
    this->Count_FD_neurton_and_photon_hits = true;

    // TODO: add this switch to event selection variables!
    this->ES_by_leading_FDneutron = true;

    /* Acceptance maps setup */
    this->Generate_Electron_AMaps = false;  // Generate electron acceptance maps
    this->Generate_Nucleon_AMaps = true;    // Generate nucleon acceptance maps
    this->Generate_WMaps = false;           // Generate efficiency maps
    this->AMaps_calc_with_one_reco_electron = true;
    this->P_e_bin_profile = "uniform_P_e_bins";      // {reformat_e_bins , varying_P_e_bins , uniform_P_e_bins, equi_inverted_P_e}
    this->P_nuc_bin_profile = "uniform_P_nuc_bins";  // {equi_inverted_P_nuc , varying_P_nuc_bins , uniform_P_nuc_bins}
    this->Electron_single_slice_test = false;        // keep as false for normal runs!
    this->Nucleon_single_slice_test = false;         // keep as false for normal runs!
    this->TestSlices = {1, 1, 1};                    // {ElectronTestSlice, ProtonTestSlice, NeutronTestSlice}

    /* Neutron resolution setup */
    // TODO: align neutron and proton momRes calculations!
    this->plot_and_fit_MomRes = false;  // Generate nRes plots
    this->Calculate_momResS2 = false;   // Calculate momResS2 variables
    this->DeltaSlices = 0.05;
    this->VaryingDelta = true;          // 1st momResS1 w/ VaryingDelta = false
    this->ForceSmallpResLimits = true;  // 1st momResS1 w/ VaryingDelta = false
    this->SmearMode = "pol1_wKC";
    this->CorrMode = "pol1_wKC";
    this->Run_with_momResS2 = true;  // Smear w/ momResS2 & correct w/ momResS1
    this->momRes_test = false;       // false by default
    /*
    MomRes run order guide:
    1. momResS1 calculation 1:
                           1a:  VaryingDelta = false  , plot_and_fit_MomRes = true  , Calculate_momResS2 = false , Run_with_momResS2 = false
                           1b:  VaryingDelta = true , plot_and_fit_MomRes = true  , Calculate_momResS2 = false , Run_with_momResS2 = false
    3. momResS2 calculation:    VaryingDelta = true , plot_and_fit_MomRes = true  , Calculate_momResS2 = true  , Run_with_momResS2 = false
    4. momResS2 run:            VaryingDelta = true , plot_and_fit_MomRes = false , Calculate_momResS2 = false , Run_with_momResS2 = true
    */

    // Auto-disable variables
    if (Generate_Electron_AMaps && Generate_Nucleon_AMaps) {
        cout << "\n\nGenerate AMaps: Generate_Electron_AMaps and Generate_Nucleon_AMaps can't be true at the same time! Exiting...", exit(0);
    }

    if (Generate_Electron_AMaps && !findSubstring(SampleName, "Uniform_1e")) { Generate_Electron_AMaps = false; }

    if (Generate_Nucleon_AMaps && (!findSubstring(SampleName, "Uniform_ep")) && !findSubstring(SampleName, "Uniform_en")) { Generate_Nucleon_AMaps = false; }

    if (plot_and_fit_MomRes && (Calculate_momResS2 || Run_with_momResS2)) { ForceSmallpResLimits = false; }
}

void DetectorSimulationAnalyser::CutSetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // ======================================================================================================================================================================
    // Cut setup
    // ======================================================================================================================================================================

    CodeDebugger.PrintStepTester(DebuggerMode);

    // Cut setup ---------------------------------------------------------------------------------------------------------------------------------------------------------

    // Cuts setup
    /* Settings that allow to disable/enable every cut individually */

    // clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------
    this->apply_cuts = true;                   // master ON/OFF switch for applying cuts
    this->clas12ana_particles = true;          // TODO: move form here!
    this->only_preselection_cuts = false;      // keep as false for regular runs!
    this->only_electron_quality_cuts = false;  // keep as false for regular runs!

    /* Preselection cuts (event cuts) */
    this->apply_preselection_cuts = true;               // master ON/OFF switch for preselection cuts
    this->apply_Vz_e_cuts = true;                       // Electron vertex cuts
    this->apply_Vz_cuts = true, apply_dVz_cuts = true;  // Vertex cuts
    this->apply_DC_e_fiducial_cuts = true;              // Electron DC fiducial (edge) cuts
    this->apply_DC_fiducial_cuts = true;                // DC fiducial (edge) cuts

    /* Electron quality cuts */
    this->apply_electron_quality_cuts = true;  // master ON/OFF switch for eQC
    this->apply_Nphe_cut = true;               // Number of photo-electrons in HTCC cut
    this->apply_ECAL_SF_cuts = true;           // SF cut on both E_deb AND P_e
    this->apply_ECAL_P_cuts = false;           // SF cut on P_e (keep as false for now!)
    this->apply_ECAL_fiducial_cuts = true;     // ECAL edge cuts for other charged particles
    this->apply_Electron_beta_cut = true;      // Electron beta cut

    /* Chi2 cuts (= PID cuts) */
    this->apply_chi2_cuts_1e_cut = true;

    // My analysis cuts ---------------------------------------------------------------------------------------------------------------------------------------------------
    /* Nucleon cuts */
    this->apply_nucleon_cuts = true;  // set as true to get good protons and calculate upper neutron momentum th.

    /* Physical cuts */
    this->apply_nucleon_physical_cuts = true;  // nucleon physical cuts master
    // TODO: automate adding upper mom. th. to nucleon cuts (for nRes calc)
    this->apply_nBeta_fit_cuts = true;  // apply neutron upper mom. th.
    this->apply_fiducial_cuts = false;
    this->apply_kinematical_cuts = false;
    this->apply_kinematical_weights = false;
    this->apply_nucleon_SmearAndCorr = false;

    // Custom cuts naming & print out execution variables

    // Auto-disable variables
    if (only_preselection_cuts || only_electron_quality_cuts) {
        apply_cuts = false;
    } else {
        if (Generate_Electron_AMaps) {
            apply_cuts = false;  // Electron acceptance maps (for fuducial cuts) should not use any electron PID or pre-selection cuts!
        }

        if (Generate_Nucleon_AMaps) {
            apply_cuts = true;

            apply_preselection_cuts = true;
            apply_Vz_e_cuts = true, apply_Vz_cuts = apply_dVz_cuts = false;
            apply_DC_e_fiducial_cuts = true, apply_DC_fiducial_cuts = false;

            apply_electron_quality_cuts = apply_Nphe_cut = apply_ECAL_SF_cuts = true;
            apply_ECAL_P_cuts = false;
            apply_ECAL_fiducial_cuts = apply_Electron_beta_cut = true;

            apply_chi2_cuts_1e_cut = false;
        }
    }

    if (!apply_cuts) {
        if (!only_preselection_cuts) { apply_preselection_cuts = false; }

        if (!only_electron_quality_cuts) { apply_electron_quality_cuts = false; }

        apply_chi2_cuts_1e_cut = apply_nucleon_cuts = false;

        // Generate_Electron_AMaps = true;
    }

    if (!apply_preselection_cuts) { apply_Vz_e_cuts = apply_Vz_cuts = apply_dVz_cuts = apply_DC_e_fiducial_cuts = apply_DC_fiducial_cuts = false; }

    if (!apply_electron_quality_cuts) { apply_Nphe_cut = apply_ECAL_SF_cuts = apply_ECAL_P_cuts = apply_ECAL_fiducial_cuts = apply_Electron_beta_cut = false; }
    // else
    // {
    //     apply_Nphe_cut = apply_ECAL_SF_cuts = apply_ECAL_fiducial_cuts = apply_Electron_beta_cut = true;
    //     // apply_Nphe_cut = apply_ECAL_SF_cuts = apply_ECAL_P_cuts = apply_ECAL_fiducial_cuts = apply_Electron_beta_cut = true;
    // }

    if (!apply_chi2_cuts_1e_cut) { apply_nucleon_cuts = false; }

    if (!apply_nucleon_cuts) { Generate_WMaps = apply_nucleon_physical_cuts = false; }

    if (!apply_nucleon_physical_cuts) {
        apply_nBeta_fit_cuts = apply_fiducial_cuts = apply_kinematical_cuts = apply_kinematical_weights = apply_nucleon_SmearAndCorr = false;
    } else {
        if (Calculate_momResS2) { apply_nucleon_SmearAndCorr = true; }
    }

    if (Generate_WMaps) { apply_fiducial_cuts = false; }

    if (!VaryingDelta) { apply_nucleon_SmearAndCorr = false; }

    if (isData) {
        // no TL calculation, AMap,WMap generation nor nRes calculation when running on data
        calculate_truth_level = Generate_WMaps = plot_and_fit_MomRes = momRes_test = false;
    }

    if (!calculate_truth_level) { AMaps_calc_with_one_reco_electron = fill_TL_plots = Rec_wTL_ES = false; }

    if (Rec_wTL_ES) {
        /* if Rec_wTL_ES = true, there are no momentum thresholds, and we get an infinite loop in the nRes slice calculations!
           Additionally, there is no need to calculate the resolution and efficiency in the same time! */
        plot_and_fit_MomRes = false;
    } else if (!Rec_wTL_ES) {
        /* if Rec_wTL_ES = false, keep fiducial cuts with the overlapping maps! (safety measure) */
        Calc_eff_overlapping_FC = true;
    }

    if (!plot_and_fit_MomRes) { Calculate_momResS2 = false; }

    if ((Calculate_momResS2 && Run_with_momResS2)  // Don't run calculate momResS2 and run on it at the same time
        || (Calculate_momResS2 && !VaryingDelta)   // Don't run calculate momResS2 and small momentum slices at the same time
    ) {
        cout << "\033[33m\n\nmomRes order error! Exiting...\n\n", exit(0);
    }

    // Custom cuts naming

    /* Save plots to custom-named folders, to allow multi-sample runs at once. */
    this->custom_cuts_naming = true;
    this->run_plots_path = plots_path;
    this->run_plots_log_save_Directory = plots_log_save_Directory;
    settings.SetCustomCutsNaming(custom_cuts_naming);
    settings.ConfigureStatuses(apply_cuts, clas12ana_particles, only_preselection_cuts, apply_chi2_cuts_1e_cut, only_electron_quality_cuts, apply_nucleon_cuts, Enable_FD_photons,
                               apply_nucleon_SmearAndCorr, apply_kinematical_cuts, apply_kinematical_weights, apply_fiducial_cuts, (Generate_Electron_AMaps || Generate_Nucleon_AMaps),
                               plot_and_fit_MomRes, VaryingDelta, Calculate_momResS2, Run_with_momResS2, momRes_test, Rec_wTL_ES, ZoomIn_On_mom_th_plots);
    settings.SetPaths(WorkingDirectory, SampleName, run_plots_path, apply_cuts, apply_chi2_cuts_1e_cut, apply_nucleon_cuts);
    settings.GetPlotsPath(run_plots_path);
    settings.GetPlotsLogSaveDirectory(run_plots_log_save_Directory);

    PrintCuts();

    // Cut declarations -----------------------------------------------------------------------------------------------------------------------------------------------------

    DefineCuts();
}

void DetectorSimulationAnalyser::PrintCuts() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // Print out execution variables
    /* Print out execution variables (for self observation) */
    cout << "\033[33m-- Execution variables ----------------------------------------------------\n\033[0m";
    cout << "\033[33mWorkingDirectory:\t" << WorkingDirectory << "\n\033[0m";
    cout << "\033[33mrun_plots_path:\t\t" << run_plots_path << "\n\n\033[0m";

    cout << "\033[33mAnalyseFilePath:\t" << "/" << AnalyseFilePath << "/" << "\n\033[0m";
    cout << "\033[33mAnalyseFileSample:\t" << "/" << AnalyseFileSample << "/" << "\n\033[0m";
    cout << "\033[33mAnalyseFile:\t\t" << AnalyseFile << "\n\033[0m";
    cout << "\033[33mSettings mode:\t\t'" << file_name << "'\n\n\033[0m";

    cout << "\033[33mSampleName:\t\t" << SampleName << "\n\033[0m";
    cout << "\033[33mVaryingSampleName:\t" << VaryingSampleName << "\n\033[0m";
    cout << "\033[33mTarget:\t\t\t" << Target << " (PDG: " << TargetPDG << ")\n\033[0m";
    cout << "\033[33mBeam Energy:\t\t" << beamE << " [GeV]\n\n\n\n\033[0m";

    // Cuts output
    /* Print out the cuts within the run (for self-observation) */
    cout << "\033[33mCuts are " << (apply_cuts ? "enabled" : "disabled") << ":\n\033[0m";

    cout << "\033[33mapply_cuts:\t\t\t" << BoolToString(apply_cuts) << "\n\033[0m";
    cout << "\033[33mclas12ana_particles:\t\t" << BoolToString(clas12ana_particles) << "\n\033[0m";  // TODO: move form here!
    cout << "\033[33monly_preselection_cuts:\t\t" << BoolToString(only_preselection_cuts) << "\n\033[0m";
    cout << "\033[33monly_electron_quality_cuts:\t" << BoolToString(only_electron_quality_cuts) << "\n\n\033[0m";

    cout << "\033[33mapply_preselection_cuts:\t" << BoolToString(apply_preselection_cuts) << "\n\033[0m";
    cout << "\033[33mapply_Vz_e_cuts:\t\t\t" << BoolToString(apply_Vz_e_cuts) << "\n\033[0m";
    cout << "\033[33mapply_Vz_cuts:\t\t\t" << BoolToString(apply_Vz_cuts) << "\n\033[0m";
    cout << "\033[33mapply_dVz_cuts:\t\t\t" << BoolToString(apply_dVz_cuts) << "\n\033[0m";
    cout << "\033[33mapply_DC_e_fiducial_cuts:\t\t" << BoolToString(apply_DC_e_fiducial_cuts) << "\n\033[0m";
    cout << "\033[33mapply_DC_fiducial_cuts:\t\t" << BoolToString(apply_DC_fiducial_cuts) << "\n\n\033[0m";

    cout << "\033[33mapply_electron_quality_cuts:\t" << BoolToString(apply_electron_quality_cuts) << "\n\033[0m";
    cout << "\033[33mapply_Nphe_cut:\t\t\t" << BoolToString(apply_Nphe_cut) << "\n\033[0m";
    cout << "\033[33mapply_ECAL_SF_cuts:\t\t" << BoolToString(apply_ECAL_SF_cuts) << "\n\033[0m";
    cout << "\033[33mapply_ECAL_P_cuts:\t\t" << BoolToString(apply_ECAL_P_cuts) << "\n\033[0m";
    cout << "\033[33mapply_ECAL_fiducial_cuts:\t" << BoolToString(apply_ECAL_fiducial_cuts) << "\n\033[0m";
    cout << "\033[33mapply_Electron_beta_cut:\t" << BoolToString(apply_Electron_beta_cut) << "\n\n\033[0m";

    cout << "\033[33mapply_chi2_cuts_1e_cut:\t\t" << BoolToString(apply_chi2_cuts_1e_cut) << "\n\033[0m";

    cout << "\033[33mapply_nucleon_cuts:\t\t" << BoolToString(apply_nucleon_cuts) << "\n\n\033[0m";

    cout << "\033[33mapply_nucleon_physical_cuts:\t" << BoolToString(apply_nucleon_physical_cuts) << "\n\033[0m";
    cout << "\033[33mapply_nBeta_fit_cuts:\t\t" << BoolToString(apply_nBeta_fit_cuts) << "\n\033[0m";
    cout << "\033[33mapply_fiducial_cuts:\t\t" << BoolToString(apply_fiducial_cuts) << "\n\033[0m";
    cout << "\033[33mapply_kinematical_cuts:\t\t" << BoolToString(apply_kinematical_cuts) << "\n\033[0m";
    cout << "\033[33mapply_kinematical_weights:\t" << BoolToString(apply_kinematical_weights) << "\n\033[0m";
    cout << "\033[33mapply_nucleon_SmearAndCorr:\t" << BoolToString(apply_nucleon_SmearAndCorr) << "\n\n\033[0m";
}

void DetectorSimulationAnalyser::DefineCuts() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // Cuts declarations
    /* Log cut values to be used later when applying them. */

    // clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------

    // clas12ana cuts
    /* Number of Photo-electrons (Nphe) cuts (electrons only, FD) */
    this->Nphe_cuts_FD;

    /* Chi2 cuts. NOTES:
     * Values for mean and sigma are filled from fit variables (overating these values later).
     * Upper cut lim (Cuts.at(2)) is the same as the sigma that is used in clas12ana to apply PID cuts */
    this->Chi2_Electron_cuts_CD = DSCuts("Chi2", "CD", "Electron", "1e cut", 0, -9999, 9999);
    this->Chi2_Electron_cuts_FD = DSCuts("Chi2", "FD", "Electron", "1e cut", 0, -9999, 9999);

    this->Chi2_Proton_cuts_CD = DSCuts("Chi2", "CD", "Proton", "1e cut", 0, -9999, 9999);
    this->Chi2_Proton_cuts_FD = DSCuts("Chi2", "FD", "Proton", "1e cut", 0, -9999, 9999);

    this->Chi2_piplus_cuts_CD = DSCuts("Chi2", "CD", "piplus", "1e cut", 0, -9999, 9999);
    this->Chi2_piplus_cuts_FD = DSCuts("Chi2", "FD", "piplus", "1e cut", 0, -9999, 9999);

    this->Chi2_piminus_cuts_CD = DSCuts("Chi2", "CD", "piminus", "1e cut", 0, -9999, 9999);
    this->Chi2_piminus_cuts_FD = DSCuts("Chi2", "FD", "piminus", "1e cut", 0, -9999, 9999);

    // Kaon PID cuts
    /* Kaon pid cuts. Not really applied in our analysis. */
    this->Chi2_Kplus_cuts_CD = DSCuts("Chi2", "CD", "Kplus", "1e cut", 0, -9999, 9999);
    this->Chi2_Kplus_cuts_FD = DSCuts("Chi2", "FD", "Kplus", "1e cut", 0, -9999, 9999);
    this->Chi2_Kminus_cuts_CD = DSCuts("Chi2", "CD", "Kminus", "1e cut", 0, -9999, 9999);
    this->Chi2_Kminus_cuts_FD = DSCuts("Chi2", "FD", "Kminus", "1e cut", 0, -9999, 9999);

    // Deuteron PID cuts
    /* Deuteron pid cuts. Not really applied in our analysis. */
    this->Chi2_deuteron_cuts_CD = DSCuts("Chi2", "CD", "deuteron", "1e cut", 0, -9999, 9999);
    this->Chi2_deuteron_cuts_FD = DSCuts("Chi2", "FD", "deuteron", "1e cut", 0, -9999, 9999);

    /* Vertex cuts */
    this->Vz_cut = Experiment.GetVz_cuts(), Vz_cut_FD = Experiment.GetVz_cuts_FD(), Vz_cut_CD = Experiment.GetVz_cuts_CD();
    this->dVz_cuts = Experiment.GetdVz_cuts(), dVz_cuts_FD = Experiment.GetdVz_cuts_FD(), dVz_cuts_CD = Experiment.GetdVz_cuts_CD();

    /* Sampling Fraction (SF) cuts (electrons only, FD) */
    this->SF_cuts;
    //    this->P_cuts;

    /* PCAL edge cuts (fiducial cuts ,electrons only, FD) */
    this->PCAL_edge_cuts;

    /* DC edge cuts (fiducial cuts ,electrons only, FD) */
    this->DC_e_edge_cuts, DC_edge_cuts;

    // Reco particles momentum thresholds
    /* Momentum thresholds (declarations) */
    this->e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
    this->no_p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, -9999, 9999);  // (no) momentum thresholds for (e,e'Xp)Y efficiency
    this->pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    this->pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    this->ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);

    this->p_mom_th, n_mom_th;  // Nucleons momentum thresholds for PID

    if (limless_mom_eff_plots) {
        /* If we enforce TL cuts, don't use momentum thresholds on nucleons. */
        p_mom_th = DSCuts("Momentum_th", "", "Protons", "", 0, -9999, 9999), n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, -9999, 9999);
    } else {
        /* If we don't enforce TL cuts, use momentum thresholds on nucleons. */
        p_mom_th = DSCuts("Momentum_th", "", "Protons", "", 0, 0.4, 9999), n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, 0.4, 9999);
    }

    // Other cuts -------------------------------------------------------------------------------------------------------------------------------------------------------

    // Other cuts
    /* Neutron beta cuts (1n & nFDpCD, FD only) */
    this->Beta_max_cut_ABF_FD_n_from_ph, Beta_max_cut_ABF_FD_n_from_ph_apprax;

    /* Neutron momentum cuts (1n & nFDpCD, FD only) */
    this->n_momentum_cuts_ABF_FD_n_from_ph;         // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    this->n_momentum_cuts_ABF_FD_n_from_ph_apprax;  // Approximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta.

    /* Truth-level momentum cuts */
    // TODO: remove pion mom. th. separation by CD and FD. It's useless (according to Adi)
    this->TL_e_mom_cuts = DSCuts("Momentum", "", "Electron", "", 0, e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut());
    this->TL_n_mom_cuts = DSCuts("Momentum", "", "Neutrons", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());
    this->TL_p_mom_cuts = DSCuts("Momentum", "", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    this->TL_pFD_mom_cuts = DSCuts("Momentum", "FD", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    this->TL_pCD_mom_cuts = DSCuts("Momentum", "CD", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    this->TL_pip_mom_cuts = DSCuts("Momentum", "", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    this->TL_pipFD_mom_cuts = DSCuts("Momentum", "FD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    this->TL_pipCD_mom_cuts = DSCuts("Momentum", "CD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    this->TL_pim_mom_cuts = DSCuts("Momentum", "", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    this->TL_pimFD_mom_cuts = DSCuts("Momentum", "FD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    this->TL_pimCD_mom_cuts = DSCuts("Momentum", "CD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    this->TL_pi0_mom_cuts = DSCuts("Momentum", "", "Pizero", "", 0, GetPi0MomTh(ph_mom_th), 9999);
    this->TL_ph_mom_cuts = DSCuts("Momentum", "", "Photons", "", 0, ph_mom_th.GetLowerCut(), ph_mom_th.GetUpperCut());

    /* FD & CD theta range */
    this->ThetaFD = DSCuts("Theta FD", "FD", "", "", 1, 5., 40.);
    this->ThetaCD = DSCuts("Theta CD", "CD", "", "", 1, 40., 135.);

    /* FD & CD theta range (for AMaps) */
    this->ThetaFD_AMaps = DSCuts("Theta FD", "FD", "", "", 1, 5., 45.);

    /* Beta cut (1n, FD) */
    this->Beta_cut = DSCuts("Beta_nuc", "FD", "", "1n", 1, 0, 9999);

    /* Neutron veto cut (1n & nFDpCD, FD) */
    this->Neutron_veto_cut = DSCuts("Neutron veto", "FD", "", "1n", 0, 100, 9999);

    /* Ghost tracks handling (2p & pFDpCD, CD & FD) */
    this->Theta_p1_cuts_2p = DSCuts("Theta_p1", "", "Proton", "2p", 40., -9999, 5.);
    this->Theta_p2_cuts_2p = DSCuts("Theta_p2", "", "Proton", "2p", 40., -9999, 5.);
    this->dphi_p1_p2_2p = DSCuts("dPhi_p1_p2", "", "Proton", "2p", 0, -9999, 15.);

    this->Theta_pFD_cuts_2p = DSCuts("Theta_p1 leading", "", "Proton", "2p", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
    this->Theta_pCD_cuts_2p = DSCuts("Theta_p2 recoil", "", "Proton", "2p", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
    this->dphi_pFD_pCD_2p = DSCuts("dPhi_pFD_pCD", "", "Proton", "2p", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

    this->Theta_pFD_cuts_pFDpCD = DSCuts("Theta_pFD", "", "Proton", "pFDpCD", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
    this->Theta_pCD_cuts_pFDpCD = DSCuts("Theta_pCD", "", "Proton", "pFDpCD", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
    this->dphi_pFD_pCD_pFDpCD = DSCuts("dPhi_pFD_pCD", "", "Proton", "pFDpCD", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

    this->Theta_L_cuts_nFDpCD = DSCuts("Theta_L", "", "", "nFDpCD", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
    this->Theta_R_cuts_nFDpCD = DSCuts("Theta_R", "", "", "nFDpCD", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
    this->dphi_L_R_nFDpCD = DSCuts("dphi_L_R", "", "", "nFDpCD", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

    /* reco. kinematic cuts (based on nucleons' efficiency) */
    this->FD_nucleon_theta_cut = DSCuts("FD Nucleon theta cut", "FD", "", "", 0, 0, 32.);
    //    this->Nucleon_momentum_cut = DSCuts("Nucleon momentum cut", "FD", "", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());
    //    this->FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "Protons and neutrons", 0, 1., 4.); // Original mom. KC
    //    this->FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "Protons and neutrons", 0, 1., 3.); // Larry meeting (10/08/23)
    this->FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "Protons and neutrons", 0, 1., 2.5);  // E4nu meeting (29/01/24)

    this->MomRes_mu_cuts = DSCuts("MomRes_mu_cuts", "FD", "", "Protons and neutrons", 0, FD_nucleon_momentum_cut.GetLowerCut(), 2.2);
    // E4nu meeting (29/01/24)
    this->MomRes_sigma_cuts = DSCuts("MomRes_sigma_cuts", "FD", "", "Protons and neutrons", 0, FD_nucleon_momentum_cut.GetLowerCut(), 2.2);
    // Adi meeting after (29/01/24)
}

void DetectorSimulationAnalyser::DirectorySetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // ======================================================================================================================================================================
    // Other setup
    // ======================================================================================================================================================================

    //  Directory settings ------------------------------------------------------------------------------------------------------------------------------------------------

    // Directory settings
    /* Code for creating directories.
     * Directory creation is done in the Directories class.
     * Added for the case that plots out folder does not exist and for organization.
     * All cut plots are separate from the analysis plots, and withing the 01_Cuts_plots folder. */
    cout << "\033[33m\nCreating plot directories...\033[0m";

    this->Plots_Folder = run_plots_path;
    this->Clear_Old_Directories = true;

    this->directories = Directories(Plots_Folder, Clear_Old_Directories);

    this->Save_Plots_folder_to_zip_files = true;
    cout << "\033[33m done.\n\n\033[0m";

    // TList definition -----------------------------------------------------------------------------------------------------------------------------------------------------

    // TList definition
    /* Definition of plots TLists used to save all plots to .root file. */

    /* General plots TList */
    this->plots = new TList();
    this->listName = run_plots_path + "/" + SampleName + plots_TList_FileType;
    this->TListName = listName.c_str();

    // TFile definition -----------------------------------------------------------------------------------------------------------------------------------------------------

    // TFile definition
    /* Definition of plots TFile used to save all plots to .pdf file. */

    /* General plots PDF file */
    this->Histogram_OutPDF_fileName = run_plots_path + "/" + SampleName + plots_TFile_FileType;
    this->Histogram_OutPDF = Histogram_OutPDF_fileName.c_str();
}

void DetectorSimulationAnalyser::PlotSelectorSetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // Plot selector --------------------------------------------------------------------------------------------------------------------------------------------------------

    // Plot selector
    /* Here are boolean variables used to turn ON/OFF the different plots of the code.
       Plot_selector_master must remain true, set it OFF only for debugging. */
    this->TestRun = false;  // set as false for a full run
    this->ApplyLimiter = false;
    this->Limiter = 1000000;

    // Set enabled plots
    if (!TestRun) {
        // Plot everything (full run)
        /* Master plots variable */
        Plot_selector_master = true;  // Master plot selector for analysis

        /* Cut variable plots */
        Cut_plots_master = true;  // Master cut plots selector
        Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true, Momentum_plots = true;

        /* W plots */
        W_plots = true;

        /* Beta plots */
        Beta_plots = true;
        Beta_vs_P_plots = true;

        /* Angle plots */
        Angle_plots_master = true;  // Master angle plots selector
        Theta_e_plots = true, Phi_e_plots = true;

        /* Momentum transfer plots */
        Momentum_transfer_plots = true;

        /* E_e plots */
        E_e_plots = true;

        /* ET plots */
        ETrans_plots_master = true;  // Master ET plots selector
        ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

        /* Ecal plots */
        Ecal_plots = true;

        /* Transverse variables plots */
        TKI_plots = true;

        /* ToF plots */
        ToF_plots = false;

        /* Efficiency plots */
        Efficiency_plots = true;
        TL_after_Acceptance_Maps_plots = true;

        /* Resolution plots */
        AMaps_plots = true;
        WMaps_plots = true;

        /* Resolution plots */
        Resolution_plots = true;

        /* Multiplicity plots */
        Multiplicity_plots = true;

        /* Final state ratio plots */
        FSR_1D_plots = true;
        FSR_2D_plots = true;  // disabled below if HipoChainLength is less than 100
    } else {
        // Selected plots (test run)
        /* Master plots variable */
        Plot_selector_master = true;  // Master plot selector for analysis

        /* Cut variable plots */
        Cut_plots_master = true;
        // Cut_plots_master = false;  // Master cut plots selector
        // Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true;
        Nphe_plots = false, Chi2_plots = false, Vertex_plots = false, SF_plots = false, fiducial_plots = false;

        Momentum_plots = true;
        // Momentum_plots = false;

        /* W plots */
        //     W_plots = true;
        W_plots = false;

        /* Beta plots */
        //     Beta_plots = true;
        Beta_plots = false;
        //     Beta_vs_P_plots = true;
        Beta_vs_P_plots = false;

        /* Angle plots */
        //        Angle_plots_master = true; // Master angle plots selector
        //        Theta_e_plots = true, Phi_e_plots = true;
        Angle_plots_master = false;  // Master angle plots selector
        Theta_e_plots = false, Phi_e_plots = false;

        /* Momentum transfer plots */
        // Momentum_transfer_plots = true;
        Momentum_transfer_plots = false;

        /* E_e plots */
        //     E_e_plots = true;
        E_e_plots = false;

        /* ET plots */
        //     ETrans_plots_master = true; // Master ET plots selector
        ETrans_plots_master = false;  // Master ET plots selector
        ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

        /* Ecal plots */
        //     Ecal_plots = true;
        Ecal_plots = false;

        /* Transverse variables plots */
        //     TKI_plots = true;
        TKI_plots = false;

        /* ToF plots */
        //     ToF_plots = true;
        ToF_plots = false;

        /* Efficiency plots */
        // Efficiency_plots = true;
        Efficiency_plots = false;
        // TL_after_Acceptance_Maps_plots = true;
        TL_after_Acceptance_Maps_plots = false;

        /* Resolution plots */
        AMaps_plots = true;
        // AMaps_plots = false;
        // WMaps_plots = true;
        WMaps_plots = false;

        /* Resolution plots */
        // Resolution_plots = true;
        Resolution_plots = false;

        /* Multiplicity plots */
        Multiplicity_plots = true;
        // Multiplicity_plots = false;

        /* Final state ratio plots */
        // FSR_1D_plots = true;
        // FSR_2D_plots = true;  // disabled below if HipoChainLength is 2 or lower
        FSR_1D_plots = false;
        FSR_2D_plots = false;  // disabled below if HipoChainLength is 2 or lower
    }

    /* Other setup variables */
    this->wider_margin = true;  // Wider margin for plots
    this->debug_plots = true;   // Print out clas12ana debugging plots

    this->Log_scale_Vx_plots = false, this->Log_scale_Vy_plots = false, this->Log_scale_Vz_plots = true;
    this->Log_scale_dVx_plots = false, this->Log_scale_dVy_plots = false, this->Log_scale_dVz_plots = true;

    // Auto-disable plot selector variables
    if (!Plot_selector_master) {
        Cut_plots_master = W_plots = Beta_plots = Beta_vs_P_plots = Angle_plots_master = Momentum_transfer_plots = E_e_plots = ETrans_plots_master = Ecal_plots = false;
        TKI_plots = ToF_plots = Efficiency_plots = AMaps_plots = WMaps_plots = Resolution_plots = Multiplicity_plots = false;
        FSR_1D_plots = FSR_2D_plots = false;
    }

    if (!Cut_plots_master) { Nphe_plots = Chi2_plots = Vertex_plots = SF_plots = fiducial_plots = Momentum_plots = false; }

    if (!Angle_plots_master) { Theta_e_plots = Phi_e_plots = false; }

    if (!ETrans_plots_master) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }

    if (!fill_TL_plots) { Efficiency_plots = TL_after_Acceptance_Maps_plots = false; }

    if (!Generate_Electron_AMaps && !Generate_Nucleon_AMaps) { AMaps_plots = false; }

    if (!Generate_WMaps) { WMaps_plots = false; }

    if (!apply_nucleon_cuts || (Electron_single_slice_test || Nucleon_single_slice_test)) { FSR_1D_plots = FSR_2D_plots = false; }

    if (TestRun || ApplyLimiter) {
        if (TestRun) { cout << "\033[31m\n\nNOTE: running code in testing mode!\n\033[0m"; }

        if (ApplyLimiter) { cout << "\033[31m\n\nNOTE: running code with a limiter on number of events!\n\n\033[0m"; }
    }
}

void DetectorSimulationAnalyser::NormalizationSetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // Normalization setup -----------------------------------------------------------------------------------------------------------------------------------------------

    // Normalization setup
    /* Here are boolean variables used to turn ON/OFF the different plot normalizations of the code.
     * Enable of presentations only, since event count is important otherwise. */

    this->normalize_master = false;

    this->norm_Nphe_plots = false, this->norm_Chi2_plots = false, this->norm_Vertex_plots = false, this->norm_SF_plots = false, this->norm_Fiducial_plots = false,
    this->norm_Momentum_plots = false;

    this->norm_W_plots = false, this->norm_Beta_plots = false, this->norm_Angle_plots_master = false, this->norm_Momentum_transfer_plots = false, this->norm_E_e_plots = false,
    this->norm_ET_plots = false;
    this->norm_Ecal_plots = false, this->norm_TKI_plots = false, this->norm_MomRes_plots = false, this->norm_Multi_plots = false;

    // Auto-disable plot normalization variables
    if (!normalize_master) {
        // Disable all normalizations if normalize_master == false
        norm_Nphe_plots = norm_Chi2_plots = norm_Vertex_plots = norm_SF_plots = norm_Fiducial_plots = norm_Momentum_plots = false;
        norm_Angle_plots_master = norm_Momentum_transfer_plots = norm_E_e_plots = norm_ET_plots = norm_Ecal_plots = norm_TKI_plots = false;
        norm_MomRes_plots = norm_Multi_plots = false;
    }
}

void DetectorSimulationAnalyser::DeleteSetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // Delete setup ------------------------------------------------------------------------------------------------------------------------------------------------------

    // Delete setup
    /* Clear files from previous runs (to prevent mix fo plots from different codes). */

    this->delete_png_files = true, this->delete_root_files = true, this->delete_txt_files = true;

    /* Delete existing .txt files */
    if (delete_txt_files) { system(("find " + run_plots_path + " -type f -iname '*.txt' -delete").c_str()); }

    // Deleting files by cases
    if (delete_png_files && !delete_root_files) {
        cout << "\033[33m\nClearing old plots...\033[0m";
        system(("find " + run_plots_path + " -type f -iname '*.png' -delete").c_str());  // Delete existing .png files
        cout << "\033[33m done.\n\n\033[0m";
    } else if (!delete_png_files && delete_root_files) {
        cout << "\033[33m\nClearing old root files...\033[0m";
        system(("find " + run_plots_path + " -type f -iname '*.root' -delete").c_str());  // Delete existing .root files
        cout << "\033[33m done.\n\n\033[0m";
    } else if (delete_png_files && delete_root_files) {
        cout << "\033[33m\nClearing old plots & root files...\033[0m";
        system(("find " + run_plots_path + " -type f -iname '*.png' -delete").c_str());   // Delete existing .png files
        system(("find " + run_plots_path + " -type f -iname '*.root' -delete").c_str());  // Delete existing .root files
        cout << "\033[33m done.\n\n\033[0m";
    } else {
        cout << "\033[33m\nNo files were cleared.\n\n\033[0m";
    }
}

void DetectorSimulationAnalyser::AcceptanceMapsSetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // Acceptance maps generation -------------------------------------------------------------------------------------------------------------------------------------------

    // Acceptance maps generation
    /* Acceptance maps are handled completely by the AMaps class */
    cout << "\033[33m\nSetting Acceptance maps...\033[0m";

    if (!calculate_truth_level) { Generate_WMaps = false; }

    if (!Generate_Electron_AMaps && !Generate_Nucleon_AMaps) { AMaps_plots = false; }

    if (!Generate_WMaps) { WMaps_plots = false; }

    /* Set Bins by case */
    int NumberNucOfMomSlices = 15, NumberElecOfMomSlices = 15, HistElectronSliceNumOfXBins = 100, HistNucSliceNumOfXBins = 100;

    if (Generate_Electron_AMaps || Generate_Nucleon_AMaps) {
        this->aMaps_master = AMaps(SampleName, P_e_bin_profile, P_nuc_bin_profile, beamE, "AMaps", directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], NumberNucOfMomSlices,
                                   NumberElecOfMomSlices, HistNucSliceNumOfXBins, HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins, HistElectronSliceNumOfXBins);
    } else {
        this->aMaps_master = AMaps(AcceptanceMapsDirectory, VaryingSampleName, Electron_single_slice_test, Nucleon_single_slice_test, TestSlices);
    }

    if (Generate_WMaps) {
        this->wMaps_master = AMaps(SampleName, P_e_bin_profile, P_nuc_bin_profile, beamE, "WMaps", directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], NumberNucOfMomSlices,
                                   NumberElecOfMomSlices, HistNucSliceNumOfXBins, HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins, HistElectronSliceNumOfXBins);
    } else {
        this->wMaps_master = AMaps(AcceptanceWeightsDirectory, VaryingSampleName, Electron_single_slice_test, Nucleon_single_slice_test, TestSlices);
    }

    cout << "\033[33m done.\n\n\033[0m";

    if (Generate_Electron_AMaps) { cout << "\033[33m\n\nGenerating electron AMaps\n\n\033[0m"; }

    if (Generate_Nucleon_AMaps) { cout << "\033[33m\n\nGenerating nucleon AMaps\n\n\033[0m"; }
}

void DetectorSimulationAnalyser::AcceptanceCorrectionSetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // Acceptance correction data -------------------------------------------------------------------------------------------------------------------------------------------

    // Acceptance correction
    /* Neutron resolution fits is handled completely by the MomentumResolution class */
    cout << "\033[33m\nSetting acceptance correction data...\033[0m";

    this->save_ACorr_data = false;

    this->ACorr_data = new TList();
    this->ACorr_data_Dir = ACorrDirectory + SampleName;
    this->ACorr_data_listName = ACorr_data_Dir + "/" + "ACorr_data_-_" + SampleName + ".root";
    this->ACorr_DataName = ACorr_data_listName.c_str();

    if (!calculate_truth_level) { save_ACorr_data = false; }

    cout << "\033[33m done.\n\n\033[0m";
}

void DetectorSimulationAnalyser::NeutronResolutionSetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // Neutron resolution & proton smearing ---------------------------------------------------------------------------------------------------------------------------------

    // Neutron resolution
    /* Neutron resolution fits is handled completely by the MomentumResolution class */
    cout << "\033[33m\nSetting neutron resolution data...\033[0m";

    if (!calculate_truth_level) { plot_and_fit_MomRes = false; }  // Disable resolution-related operations if not calculating TL plots

    if (!apply_nucleon_cuts) { plot_and_fit_MomRes = false; }  // Disable resolution-related operations in initial runs

    if (!plot_and_fit_MomRes) { Calculate_momResS2 = false; }

    /* Comment to test smearing and shift */
    //    if (apply_nucleon_SmearAndCorr) { plot_and_fit_MomRes = false; }  // Disable resolution-related operations when applying proton smearing

    nRes.MomResInit(plot_and_fit_MomRes, Calculate_momResS2, Run_with_momResS2, VaryingSampleName, NucleonCutsDirectory, beamE, MomRes_mu_cuts, MomRes_sigma_cuts, n_mom_th.GetLowerCut(),
                    MomentumResolutionDirectory, directories.Resolution_Directory_map["nRes_plots_1n_Directory"], DeltaSlices, VaryingDelta, SmearMode, CorrMode, momRes_test);
    pRes.MomResInit(plot_and_fit_MomRes, Calculate_momResS2, Run_with_momResS2, VaryingSampleName, NucleonCutsDirectory, beamE, MomRes_mu_cuts, MomRes_sigma_cuts, p_mom_th.GetLowerCut(),
                    MomentumResolutionDirectory, directories.Resolution_Directory_map["pRes_plots_1p_Directory"], DeltaSlices, VaryingDelta, SmearMode, CorrMode, momRes_test,
                    ForceSmallpResLimits);

    cout << "\033[33m\ndone.\n\n\033[0m";
}

void DetectorSimulationAnalyser::DebuggingSetup() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    // Debugging setup ---------------------------------------------------------------------------------------------------------------------------------------------------

    // Debugging setup
    /* Saving a printout of the number of particles in nEvents2print events. Used for clas12ana debugging. */

    this->GoodProtonsMonitorPlots = true;

    pid.SetGPMonitoringPlots(GoodProtonsMonitorPlots, directories.Angle_Directory_map["CToF_hits_monitoring_2p_Directory"],
                             directories.Angle_Directory_map["Double_detection_monitoring_2p_Directory"]);

    this->PrintEvents = false;
    this->Ne_in_event = 1, this->Nf_in_event = 2, this->nEvents2print = 10000;

    if (PrintEvents) {
        if (!apply_chi2_cuts_1e_cut) {
            EventPrint_save_Directory = run_plots_path + "/" + "Event_Print_without_chi2.txt";
        } else if (apply_chi2_cuts_1e_cut) {
            EventPrint_save_Directory = run_plots_path + "/" + "Event_Print_ALL_CUTS.txt";
        }

        EventPrint.open(EventPrint_save_Directory.c_str());

        if (!apply_chi2_cuts_1e_cut) {
            EventPrint << "//////////////////////////////////////////////////////////////////////\n\033[0m";
            EventPrint << "// Log of number of particles in event with all cuts except chi2    //\n\033[0m";
            EventPrint << "//////////////////////////////////////////////////////////////////////\n\n\033[0m";
        } else if (apply_chi2_cuts_1e_cut) {
            EventPrint << "//////////////////////////////////////////////////////////////////////\n\033[0m";
            EventPrint << "// Log of number of particles in event with all cuts including chi2 //\n\033[0m";
            EventPrint << "//////////////////////////////////////////////////////////////////////\n\n\033[0m";
        }

        EventPrint << "EVENT SELECTION:\n\033[0m";
        EventPrint << "#electrons in event:\t\t" << Ne_in_event << "\n\033[0m";
        EventPrint << "minimal #hadrons in event:\t" << Nf_in_event << "\n\033[0m";
        EventPrint << "#event upper bound:\t\t" << nEvents2print << "\n\n\n\033[0m";
    }
}

void DetectorSimulationAnalyser::ProcessEvents() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                                         Code execution                                                                              //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    CodeDebugger.PrintStepTester(DebuggerMode);

    // Code execution
    cout << "\033[33m\nReading target parameter files...\n\n\033[0m";

    // Setting and loading cuts (via clas12ana)
    clas12ana clasAna;

    if (apply_cuts) {
        // Cuts on electrons only:
        if (apply_ECAL_SF_cuts) {
            // making f_ecalSFCuts = true
            // TODO: ask justin what are these cuts:
            // TODO: ask justin for these cuts for LH2 and C12 (and other elements)
            clasAna.readEcalSFPar((PIDCutsDirectory + "paramsSF_40Ca_x2.dat").c_str());
            // TODO: RECHECK WHAT ARE THE CUTS HERE:
            SF_cuts = DSCuts("SF", "FD", "Electron", "1e cut", 0.24865, clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut());

            clasAna.setEcalSFCuts();
        }

        if (apply_ECAL_P_cuts) {
            // making f_ecalSFCuts = true
            // TODO: ask justin what are these cuts:
            // TODO: ask justin for these cuts for LH2 and C12 (and other elements)
            clasAna.readEcalPPar((PIDCutsDirectory + "paramsPI_40Ca_x2.dat").c_str());

            clasAna.setEcalPCuts();
        }

        if (apply_ECAL_fiducial_cuts) {
            // making f_ecalEdgeCuts = true (ECAL fiducial cuts)
            PCAL_edge_cuts = DSCuts("PCAL edge", "FD", "Electron", "1e cut", 0, clasAna.getEcalEdgeCuts());
            clasAna.setEcalEdgeCuts();
        }

        if (apply_Nphe_cut) {
            // making f_NpheCuts = true (HTCC cuts)
            Nphe_cuts_FD = DSCuts("Nphe", "FD", "Electron", "1e cut", 0, clasAna.getNpheCuts());
            clasAna.setNpheCuts();
        }

        // Cuts on all charged hadrons:
        if (!apply_chi2_cuts_1e_cut) {
            clasAna.readInputParam((PIDCutsDirectory + "ana.par").c_str());
        } else if (apply_chi2_cuts_1e_cut) {
            cout << "\033[33m\nLoading fitted pid cuts...\n\n\033[0m";
            clasAna.readInputParam((PIDCutsDirectory + "Fitted_PID_Cuts_-_" + SampleName + ".par").c_str());  // load sample-appropreate cuts file from CutsDirectory

            /* Overwriting PID cuts according to SampleName */
            Chi2_Proton_cuts_CD.SetCutPram(clasAna.GetPidCutMean(2212, "CD"), -clasAna.GetPidCutSigma(2212, "CD"), clasAna.GetPidCutSigma(2212, "CD"));
            Chi2_Proton_cuts_FD.SetCutPram(clasAna.GetPidCutMean(2212, "FD"), -clasAna.GetPidCutSigma(2212, "FD"), clasAna.GetPidCutSigma(2212, "FD"));
            Chi2_piplus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(211, "CD"), -clasAna.GetPidCutSigma(211, "CD"), clasAna.GetPidCutSigma(211, "CD"));
            Chi2_piplus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(211, "FD"), -clasAna.GetPidCutSigma(211, "FD"), clasAna.GetPidCutSigma(211, "FD"));
            Chi2_piminus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(-211, "CD"), -clasAna.GetPidCutSigma(-211, "CD"), clasAna.GetPidCutSigma(-211, "CD"));
            Chi2_piminus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(-211, "FD"), -clasAna.GetPidCutSigma(-211, "FD"), clasAna.GetPidCutSigma(-211, "FD"));

            clasAna.setPidCuts();  // making f_pidCuts = true
        }

        // Cuts on all charged particles:
        if (apply_Vz_e_cuts || apply_Vz_cuts) {
            if (apply_Vz_e_cuts) {
                clasAna.set_e_VertexCuts();  // making f_e_vertexCuts = true
            }

            if (apply_Vz_cuts) {
                clasAna.setVertexCuts();  // making f_vertexCuts = true
            }

            clasAna.setVzcuts(Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut());          // setting Vz cuts for all (charged?) particles
            clasAna.setVzcutsFD(Vz_cut_FD.GetLowerCut(), Vz_cut_FD.GetUpperCut());  // setting Vz cuts for all charged particles (FD only)
            clasAna.setVzcutsCD(Vz_cut_CD.GetLowerCut(), Vz_cut_CD.GetUpperCut());  // setting Vz cuts for all charged particles (CD only)
        }

        if (apply_DC_e_fiducial_cuts || apply_DC_fiducial_cuts) {
            if (apply_DC_e_fiducial_cuts) {
                // making f_e_DCEdgeCuts = true (DC fiducial cuts?)
                DC_e_edge_cuts = DSCuts("DC edge", "FD", "Electron", "1e cut", 0, clasAna.getDCEdgeCuts());
                clasAna.set_e_DCEdgeCuts();
            }

            if (apply_DC_fiducial_cuts) {
                // making f_e_DCEdgeCuts = true (DC fiducial cuts?)
                DC_edge_cuts = DSCuts("DC edge", "FD", "Charged particles", "1e cut", 0, clasAna.getDCEdgeCuts());
                clasAna.setDCEdgeCuts();
            }
        }

        if (apply_dVz_cuts) {
            clasAna.setVertexCorrCuts();                                                           // making f_corr_vertexCuts = true
            clasAna.setVertexCorrCutsLim(dVz_cuts.GetLowerCut(), dVz_cuts.GetUpperCut());          // setting dVz cuts (general)
            clasAna.setVertexCorrCutsLimFD(dVz_cuts_FD.GetLowerCut(), dVz_cuts_FD.GetUpperCut());  // setting dVz cuts (FD only)
            clasAna.setVertexCorrCutsLimCD(dVz_cuts_CD.GetLowerCut(), dVz_cuts_CD.GetUpperCut());  // setting dVz cuts (CD only)
        }

        if (!apply_nucleon_cuts) {
            /* Setting neutron momentum cut before beta fit (i.e., no cut!) */
            n_momentum_cuts_ABF_FD_n_from_ph = DSCuts("Momentum_cuts_ECAL", "FD-ECAL", "Neutron", "", 0, n_mom_th.GetLowerCut(), 9999);
            n_momentum_cuts_ABF_FD_n_from_ph_apprax = DSCuts("Momentum_cuts_ECAL_apprax", "FD-ECAL_apprax", "Neutron", "", 0, n_mom_th.GetLowerCut(), 9999);

            /* Setting variables to log beta fit parameters into (i.e., no cut!) */
            Beta_max_cut_ABF_FD_n_from_ph = DSCuts("Beta_cut_ECAL", "FD-ECAL", "", "nFDpCD", 1, -9999, 9999);
            Beta_max_cut_ABF_FD_n_from_ph_apprax = DSCuts("Beta_cut_ECAL_apprax", "FD-ECAL_apprax", "", "1n", 1, -9999, 9999);
        } else {
            cout << "\033[33m\n\nLoading fitted Beta cuts...\n\n\033[0m";
            clasAna.readInputParam((NucleonCutsDirectory + "Nucleon_Cuts_-_" + SampleName + ".par").c_str());  // load sample-appropreate cuts file from CutsDirectory

            /* Setting nucleon cuts - only if NOT plotting efficiency plots! */
            if (limless_mom_eff_plots || is2GeVSample) {
                /* If sample is with 2GeV beam energy, no fit is needed. */
                n_mom_th.SetUpperCut(beamE);
                TL_n_mom_cuts.SetUpperCut(beamE);
            } else {
                /* Else, load values from fit. */
                if (apply_nBeta_fit_cuts) {
                    n_mom_th.SetUpperCut(clasAna.getNeutronMomentumCut());
                    TL_n_mom_cuts.SetUpperCut(clasAna.getNeutronMomentumCut());
                    Beta_cut.SetUpperCut(clasAna.getNeutralBetaCut());  // Log values of beta fit cut (for monitoring)
                    Beta_cut.SetMean(clasAna.getNeutralBetaCutMean());  // Log values of beta fit cut (for monitoring)
                }
            }

            dphi_p1_p2_2p.SetMean(clasAna.getdPhiCutMean());
            dphi_pFD_pCD_2p.SetMean(clasAna.getdPhiCutMean());
            dphi_pFD_pCD_pFDpCD.SetMean(clasAna.getdPhiCutMean());
        }

        clasAna.printParams();
    } else if (only_preselection_cuts || only_electron_quality_cuts) {
        // Cuts on all charged particles:
        if (only_preselection_cuts) {
            if (apply_Vz_e_cuts || apply_Vz_cuts) {
                if (apply_Vz_e_cuts) {
                    clasAna.set_e_VertexCuts();  // making f_e_vertexCuts = true
                }

                if (apply_Vz_cuts) {
                    clasAna.setVertexCuts();  // making f_vertexCuts = true
                }

                clasAna.setVzcuts(Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut());          // setting Vz cuts for all (charged?) particles
                clasAna.setVzcutsFD(Vz_cut_FD.GetLowerCut(), Vz_cut_FD.GetUpperCut());  // setting Vz cuts for all charged particles (FD only)
                clasAna.setVzcutsCD(Vz_cut_CD.GetLowerCut(), Vz_cut_CD.GetUpperCut());  // setting Vz cuts for all charged particles (CD only)
            }

            if (apply_DC_e_fiducial_cuts || apply_DC_fiducial_cuts) {
                if (apply_DC_e_fiducial_cuts) {
                    // making f_e_DCEdgeCuts = true (DC fiducial cuts?)
                    DC_e_edge_cuts = DSCuts("DC edge", "FD", "Electron", "1e cut", 0, clasAna.getDCEdgeCuts());
                    clasAna.set_e_DCEdgeCuts();
                }

                if (apply_DC_fiducial_cuts) {
                    // making f_e_DCEdgeCuts = true (DC fiducial cuts?)
                    DC_edge_cuts = DSCuts("DC edge", "FD", "Charged particles", "1e cut", 0, clasAna.getDCEdgeCuts());
                    clasAna.setDCEdgeCuts();
                }
            }

            if (apply_dVz_cuts) {
                clasAna.setVertexCorrCuts();                                                           // making f_corr_vertexCuts = true
                clasAna.setVertexCorrCutsLim(dVz_cuts.GetLowerCut(), dVz_cuts.GetUpperCut());          // setting dVz cuts (general)
                clasAna.setVertexCorrCutsLimFD(dVz_cuts_FD.GetLowerCut(), dVz_cuts_FD.GetUpperCut());  // setting dVz cuts (FD only)
                clasAna.setVertexCorrCutsLimCD(dVz_cuts_CD.GetLowerCut(), dVz_cuts_CD.GetUpperCut());  // setting dVz cuts (CD only)
            }
        }

        // Cuts on electrons only:
        if (only_electron_quality_cuts) {
            if (apply_ECAL_SF_cuts) {
                // making f_ecalSFCuts = true
                // TODO: ask justin what are these cuts:
                // TODO: ask justin for these cuts for LH2 and C12 (and other elements)
                clasAna.readEcalSFPar((PIDCutsDirectory + "paramsSF_40Ca_x2.dat").c_str());
                // TODO: RECHECK WHAT ARE THE CUTS HERE:
                SF_cuts = DSCuts("SF", "FD", "Electron", "1e cut", 0.24865, clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut());

                clasAna.setEcalSFCuts();
            }

            if (apply_ECAL_P_cuts) {
                // making f_ecalSFCuts = true
                // TODO: ask justin what are these cuts:
                // TODO: ask justin for these cuts for LH2 and C12 (and other elements)
                clasAna.readEcalPPar((PIDCutsDirectory + "paramsPI_40Ca_x2.dat").c_str());

                clasAna.setEcalPCuts();
            }

            if (apply_ECAL_fiducial_cuts) {
                // making f_ecalEdgeCuts = true (ECAL fiducial cuts)
                PCAL_edge_cuts = DSCuts("PCAL edge", "FD", "Electron", "1e cut", 0, clasAna.getEcalEdgeCuts());
                clasAna.setEcalEdgeCuts();
            }

            if (apply_Nphe_cut) {
                // making f_NpheCuts = true (HTCC cuts)
                Nphe_cuts_FD = DSCuts("Nphe", "FD", "Electron", "1e cut", 0, clasAna.getNpheCuts());
                clasAna.setNpheCuts();
            }
        }
    }

    // Setting HipoChain
    cout << "\033[33m\n\nSetting HipoChain...\n\n\033[0m";

    clas12root::HipoChain chain;
    Experiment.AddToHipoChain(chain, SampleName, AnalyseFilePath, AnalyseFileSample, AnalyseFile);
    chain.SetReaderTags({0});
    int HipoChainLength = chain.GetNFiles();
    auto config_c12 = chain.GetC12Reader();
    auto &c12 = chain.C12ref();

    auto db = TDatabasePDG::Instance();
    chain.db()->turnOffQADB();

    // Auto-disable variables accoding to HipoChain length
    cout << "\033[33m\n\nHipoChain loaded! HipoChainLength = " << HipoChainLength << "\n\033[0m";

    if (HipoChainLength < 100) {
        /* This avoids a crush when plotting 2D final state ratios */
        FSR_2D_plots = false;
    }

    //  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //  Setting beam particle's momentum
    //  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    CodeDebugger.PrintStepTester(DebuggerMode);

    // Setting beam particle's momentum
    double Pv = beamE, Pvx = 0., Pvy = 0., Pvz = Pv;  // Assuming momentum of incoming lepton is in the z direction

    TLorentzVector beam(0, 0, beamE, beamE);
    TVector3 Pv_3v(0, 0, beamE);

    //  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //  Counting variable definitions
    //  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Counting variable definitions
    int num_of_events = 0, num_of_events_without_any_e = 0, num_of_events_with_any_e = 0;
    int num_of_QEL_events = 0, num_of_MEC_events = 0, num_of_RES_events = 0, num_of_DIS_events = 0;

    int num_of_events_with_at_least_1e = 0, num_of_events_with_exactly_1e = 0, num_of_events_with_exactly_1e_from_file = 0, num_of_events_more_then_1e = 0;
    int num_of_QEL_events_1e_cut = 0, num_of_MEC_events_1e_cut = 0, num_of_RES_events_1e_cut = 0, num_of_DIS_events_1e_cut = 0;

    /* Counting FD neurton and photon hits in the ECAL */
    // TODO: rename variable (not crucial)
    int num_of_events_with_nFD_CLA12 = 0;
    int num_of_events_with_nFD_CLA12_PCAL = 0, num_of_events_with_nFD_CLA12_ECIN = 0, num_of_events_with_nFD_CLA12_ECOUT = 0, num_of_events_with_nFD_CLA12_EC = 0;
    int num_of_events_with_phFD_CLA12 = 0;
    int num_of_events_with_phFD_CLA12_PCAL = 0, num_of_events_with_phFD_CLA12_ECIN = 0, num_of_events_with_phFD_CLA12_ECOUT = 0, num_of_events_with_phFD_CLA12_EC = 0;

    int num_of_events_1n_in_FD = 0, num_of_events_2n_in_FD = 0, num_of_events_3n_in_FD = 0, num_of_events_Xn_in_FD = 0;

    int num_of_events_1e1p_all = 0, num_of_events_with_1e1p = 0;

    int num_of_events_1p_inFD = 0;

    int num_of_events_1n_inFD = 0, num_of_events_1n_inFD_AV = 0, num_of_events_1n_inFD_AV_wmt1gTLn = 0;

    int num_of_events_1e2p_all = 0, num_of_events_1e2p_all_woFDphotons = 0, num_of_events_with_1e2p = 0;
    int num_of_events_2p_wFakeProtons = 0, num_of_events_2p = 0;

    int num_of_events_pFDpCD = 0;
    int num_of_events_with_1epFDpCD = 0, num_of_events_with_1epFDpFD = 0, num_of_events_with_1epCDpCD = 0;

    int num_of_events_nFDpCD = 0;
    int num_of_events_nFDpCD_AV = 0;

    //  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //  Looping over each HipoChain files
    //  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    cout << "\033[33m\n\nLooping over chain files...\n\n\033[0m";

    CodeDebugger.PrintStepTester(DebuggerMode);

    while (chain.Next()) {
        // loop over events
        CodeDebugger.PrintStepTester(DebuggerMode);

        ++num_of_events;  // logging Total #(events) in sample

        /* Particles outside clas12ana */
        auto allParticles_det = c12->getDetParticles();
        auto electrons_det = c12->getByID(11);

        clasAna.Run(c12);

        /* allParticles vector from clas12ana (my addition). Used mostly for 1n & nFDpCD.  */
        auto allParticles = clasAna.getParticles();

        /* All of these particles are with clas12ana cuts. Only cuts missing are momentum and beta(?) cuts - to be applied later */
        vector<region_part_ptr> neutrons, protons, Kplus, Kminus, piplus, piminus, electrons, deuterons, neutrals, otherpart;

        if (clas12ana_particles) {
            // Get particle outside from clas12ana:
            neutrons = clasAna.getByPid(2112);  // Neutrons
            protons = clasAna.getByPid(2212);   // Protons
            Kplus = clasAna.getByPid(321);      // K+
            Kminus = clasAna.getByPid(-321);    // K-
            piplus = clasAna.getByPid(211);     // pi+
            piminus = clasAna.getByPid(-211);   // pi-
            electrons = clasAna.getByPid(11);   // Electrons

            deuterons = clasAna.getByPid(45);   // Deuterons
            neutrals = clasAna.getByPid(0);     // Unidentified
            otherpart = clasAna.getByPid(311);  // Other particles
        } else {
            // Get particle outside of clas12ana:
            neutrons = c12->getByID(2112);  // Neutrons
            protons = c12->getByID(2212);   // Protons
            Kplus = c12->getByID(321);      // K+
            Kminus = c12->getByID(-321);    // K-
            piplus = c12->getByID(211);     // pi+
            piminus = c12->getByID(-211);   // pi-
            electrons = c12->getByID(11);   // Electrons

            deuterons = c12->getByID(45);   // Deuterons
            neutrals = c12->getByID(0);     // Unidentified
            otherpart = c12->getByID(311);  // Other particles
        }

        int Nn = neutrons.size(), Np = protons.size(), Nkp = Kplus.size(), Nkm = Kminus.size(), Npip = piplus.size(), Npim = piminus.size(), Ne = electrons.size();
        int Nd = deuterons.size(), Nneut = neutrals.size(), No = otherpart.size();

        /* Total number of particles in event (= Nf) */
        int Nf = Nn + Np + Nkp + Nkm + Npip + Npim + Ne + Nd + Nneut + No;

        // Configure good particles & basic event selection
        /* Configure particles within general momentum cuts (i.e. "identified particles") */

        // Charged particles' identification
        vector<int> Electron_ind = pid.ChargedParticleID(electrons, e_mom_th);

        vector<int> IDed_Protons_ind = pid.ChargedParticleID(protons, p_mom_th);  // indices of identified protons (i.e., within P_p th.)
        vector<int> Protons_ind = pid.GetGoodProtons(apply_nucleon_cuts, protons, IDed_Protons_ind, Theta_p1_cuts_2p, Theta_p2_cuts_2p,
                                                     dphi_pFD_pCD_2p);  // good identified protons (no sCTOFhp and no dCDaFDd)

        vector<int> Piplus_ind = pid.ChargedParticleID(piplus, pip_mom_th);
        vector<int> Piminus_ind = pid.ChargedParticleID(piminus, pim_mom_th);

        // Charged particles for inclusive efficiency
        // Proton vectors for (e,e'Xp)Y efficiency
        vector<int> All_Protons_ind = pid.ChargedParticleID(protons, no_p_mom_th);  // indices of all protons (i.e., without P_p th.)
        vector<int> All_gProtons_ind = pid.GetGoodProtons(apply_nucleon_cuts, protons, All_Protons_ind, Theta_p1_cuts_2p, Theta_p2_cuts_2p,
                                                          dphi_pFD_pCD_2p);  // good protons (no sCTOFhp and no dCDaFDd) - WITHOUT mom. th.

        // Neutral particles' identification (FD only)
        /* Get FD neutrons and photons, according to the definitions: */
        // FD neutrons and photons to be set by definition - before momentum th. & any cuts:
        vector<int> ReDef_neutrons_FD, ReDef_photons_FD;
        // Get FD neutrons and photons, according to the definitions (ORIGINAL!):
        pid.ReDefFDNeutrals(allParticles, ReDef_neutrons_FD, ReDef_photons_FD);
        // FD neutron with maximal momentum:
        int NeutronsFD_ind_max = pid.GetLnFDIndex(allParticles, ReDef_neutrons_FD, apply_nucleon_cuts);

        /* Get FD neutrons and photons above momentum threshold (noNeutCuts): */
        // FD neutrons and photons by definition (after momentum th. only!):
        vector<int> NeutronsFD_ind_noNeutCuts, PhotonsFD_ind_noNeutCuts;
        pid.FDNeutralParticleID(allParticles, NeutronsFD_ind_noNeutCuts, ReDef_neutrons_FD, n_mom_th, PhotonsFD_ind_noNeutCuts, ReDef_photons_FD, ph_mom_th, apply_nucleon_cuts);
        // FD neutron (with momentum th.) with maximal momentum:
        int NeutronsFD_ind_mom_max_noNeutCuts = pid.GetLnFDIndex(allParticles, NeutronsFD_ind_noNeutCuts, apply_nucleon_cuts);

        /* Get FD neutrons and photons above momentum threshold and after ECAL veto and after ECAL edge cuts: */
        // FD neutrons and photons by definition (after momentum th. & ECAL & edge cuts):
        vector<int> NeutronsFD_ind, PhotonsFD_ind;
        pid.FDNeutralParticleID(allParticles, electrons, NeutronsFD_ind, ReDef_neutrons_FD, n_mom_th, PhotonsFD_ind, ReDef_photons_FD, ph_mom_th, Neutron_veto_cut, beamE,
                                clasAna.getEcalEdgeCuts(), clasAna.getEcalEdgeCuts(), apply_nucleon_cuts);
        //        pid.FDNeutralParticleID(allParticles, NeutronsFD_ind, ReDef_neutrons_FD, n_mom_th, PhotonsFD_ind, ReDef_photons_FD, ph_mom_th,
        //                                apply_nucleon_cuts);
        // FD neutron (with momentum th.) with maximal momentum:
        int NeutronsFD_ind_mom_max = pid.GetLnFDIndex(allParticles, NeutronsFD_ind, apply_nucleon_cuts);

        // Counting events with good FD neutrons
        if (NeutronsFD_ind.size() == 1) {
            ++num_of_events_1n_in_FD;
        } else if (NeutronsFD_ind.size() == 2) {
            ++num_of_events_2n_in_FD;
        } else if (NeutronsFD_ind.size() == 3) {
            ++num_of_events_3n_in_FD;
        } else if (NeutronsFD_ind.size() > 3) {
            ++num_of_events_Xn_in_FD;
        }

        // Safety checks
        for (int &i : ReDef_neutrons_FD) {
            if (!((allParticles[i]->par()->getPid() == 2112) || (allParticles[i]->par()->getPid() == 22))) {
                cout << "\033[33m\n\nReDef_neutrons_FD: A neutron PDG is not 2112 or 22 (" << allParticles[i]->par()->getPid() << ")! Exiting...\n\n", exit(0);
            }

            bool NeutronInPCAL_test = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool NeutronInECIN_test = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool NeutronInECOUT_test = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECIN hit

            if (NeutronInPCAL_test) { cout << "\033[33m\n\nReDef_neutrons_FD test: a neutron have been found with a PCAL hit! Exiting...\n\n", exit(0); }

            if (!(NeutronInECIN_test || NeutronInECOUT_test)) {
                cout << "\033[33m\n\nReDef_neutrons_FD test: a neutron have been found without either ECIN or ECOUT hit! Exiting...\n\n", exit(0);
            }
        }

        for (int &i : ReDef_photons_FD) {
            if (allParticles[i]->par()->getPid() != 22) {
                cout << "\033[33m\n\nReDef_photons_FD test: A photon PDG is not 2112 or 22 (" << allParticles[i]->par()->getPid() << ")! Exiting...\n\n", exit(0);
            }

            bool PhotonInPCAL_test = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);  // PCAL hit

            if (!PhotonInPCAL_test) { cout << "\033[33m\n\n1n: a photon have been found without a PCAL hit! Exiting...\n\n", exit(0); }
        }

        // Setting up event selection
        /* set up basic event selection: */
        bool single_electron = (Electron_ind.size() == 1);                               // one electron above momentum threshold
        bool no_carged_Kaons = ((Nkp == 0) && (Nkm == 0));                               // no charged Kaons whatsoever
        bool no_carged_pions = ((Piplus_ind.size() == 0) && (Piminus_ind.size() == 0));  // no charged Pions above momentum threshold
        bool no_deuterons = (Nd == 0);                                                   // no Deuterons whatsoever

        bool basic_event_selection = (single_electron && no_carged_Kaons && no_carged_pions && no_deuterons && (Enable_FD_photons || (PhotonsFD_ind.size() == 0)));

        double Weight = 1;

        // Process ID
        bool qel = false, mec = false, res = false, dis = false;
        double processID = c12->mcevent()->getWeight();  // code = 1.,2.,3.,4. = type = qel, mec, res, dis

        if (processID == 1.) {
            ++num_of_QEL_events;
            qel = true;
        } else if (processID == 2.) {
            ++num_of_MEC_events;
            mec = true;
        } else if (processID == 3.) {
            ++num_of_RES_events;
            res = true;
        } else if (processID == 4.) {
            ++num_of_DIS_events;
            dis = true;
        }

        // Debugging & monitoring

        /* Safety checks */

        // Safety check for clas12ana particles - check that allParticles.size(), Nf are the same:
        CodeDebugger.SafetyCheck_clas12ana_particles(__FILE__, __LINE__, clas12ana_particles, allParticles, Nf);

        // Safety checks for FD protons:
        CodeDebugger.SafetyCheck_FD_protons(__FILE__, __LINE__, Protons_ind, protons, p_mom_th);

        // Safety checks for FD neutrons - checks for leading FD neutron
        CodeDebugger.SafetyCheck_Reco_leading_FD_neutron(__FILE__, __LINE__, apply_nucleon_cuts, ES_by_leading_FDneutron, NeutronsFD_ind_mom_max, allParticles, NeutronsFD_ind, pid);

        //  Safety checks for FD neutrons - checks for FD neutrons
        CodeDebugger.SafetyCheck_FD_neutron(__FILE__, __LINE__, apply_nucleon_cuts, allParticles, n_mom_th, NeutronsFD_ind, pid);

        // Some event counting
        if (electrons_det.size() == 1) { ++num_of_events_with_exactly_1e_from_file; }

        if (Ne == 0) {
            // Log total #(events) with and without electrons
            ++num_of_events_without_any_e;  // logging Total #(events) w/o any e
        } else {
            ++num_of_events_with_any_e;  // logging Total #(events) w/ any e
        }

        if (Ne >= 1) {
            // At least 1e cut (log only)
            ++num_of_events_with_at_least_1e;  // logging #(events) w/ at least 1e

            if (Ne > 1) { ++num_of_events_more_then_1e; /* logging #(events) w/ more than 1e */ }
        }

        // Print events to file
        if (PrintEvents)  // TODO: add to debugger class?
        {
            bool EventPrintSelection = (Ne == Ne_in_event && Nf >= Nf_in_event);
            int event = c12->runconfig()->getEvent();

            if (EventPrintSelection) {
                if (event < (nEvents2print + 1)) {
                    EventPrint << "--- EVENT NUMBER " << event << " ---\n\033[0m";
                    EventPrint << "#particles in event:\t" << Nf << "\n\033[0m";
                    EventPrint << "protons.size() = " << protons.size() << "\n\033[0m";
                    EventPrint << "Kplus.size() = " << Kplus.size() << "\n\033[0m";
                    EventPrint << "Kminus.size() = " << Kminus.size() << "\n\033[0m";
                    EventPrint << "piplus.size() = " << piplus.size() << "\n\033[0m";
                    EventPrint << "piminus.size() = " << piminus.size() << "\n\033[0m";
                    EventPrint << "electrons.size() = " << electrons.size() << "\n\033[0m";
                    EventPrint << "deuterons.size() = " << deuterons.size() << "\n\033[0m";
                    EventPrint << "neutrals.size() = " << neutrals.size() << "\n\033[0m";
                    EventPrint << "otherpart.size() = " << otherpart.size() << "\n\n\n\033[0m";
                }
            }
        }

        // Monitoring handling fake protons
        if (GoodProtonsMonitorPlots && basic_event_selection) {
            if (IDed_Protons_ind.size() == 2) { ++num_of_events_2p_wFakeProtons; }

            pid.GPMonitoring(GoodProtonsMonitorPlots, protons, IDed_Protons_ind, Protons_ind, Theta_p1_cuts_2p, Theta_p2_cuts_2p, dphi_p1_p2_2p, Weight);
        }

        CodeDebugger.PrintStepTester(DebuggerMode);

        //  Filling truth level histograms (lundfile loop) ----------------------------------------------------------------------------------------------------------------------

        // TODO: confirm that the TL kin cuts are working!

        // Filling truth level histograms (lundfile loop)
        bool TL_Event_Selection_inclusive = true;
        bool TL_Event_Selection_1p = true, TL_Event_Selection_1n = true, TL_Event_Selection_pFDpCD = true, TL_Event_Selection_nFDpCD = true;

        double TL_nFD_mom, TL_nFD_theta, TL_nFD_phi;  // FOR nFD eff test!

        if (calculate_truth_level && (!TL_plots_only_for_NC || apply_nucleon_cuts) && isMC) {
            // run only for CLAS12 simulation & AFTER beta fit
            auto mcpbank = c12->mcparts();
            const Int_t Ngen = mcpbank->getRows();

            bool TL_fiducial_cuts;

            if (apply_fiducial_cuts) {
                TL_fiducial_cuts = true;
            } else {
                TL_fiducial_cuts = false;
            }

            // counting TL particles
            /* Particle index vectors */
            vector<int> TL_Electron_ind, TL_Neutrons_ind, TL_Protons_ind, TL_piplus_ind, TL_piminus_ind, TL_pizero_ind, TL_Photons_ind, TL_OtherPart_ind;

            /* Particle index vectors (FD & CD) */
            vector<int> TL_ElectronFD_ind, TL_NeutronsFD_ind, TL_ProtonsFD_ind, TL_piplusFD_ind, TL_piminusFD_ind, TL_pi0FD_ind, TL_PhotonsFD_ind;
            vector<int> TL_ProtonsCD_ind, TL_piplusCD_ind, TL_piminusCD_ind;

            /* Particle index vectors (for particles above momentum threshold) */
            vector<int> TL_Electron_mom_ind, TL_Neutrons_mom_ind, TL_Protons_mom_ind, TL_piplus_mom_ind, TL_piminus_mom_ind, TL_pizero_mom_ind, TL_Photons_mom_ind;

            /* Particle index vectors (for particles above momentum threshold, FD & CD) */
            vector<int> TL_ElectronFD_mom_ind, TL_NeutronsFD_mom_ind, TL_NeutronsFD_max_mom_ind, TL_pi0FD_mom_ind, TL_PhotonsFD_mom_ind;
            vector<int> TL_ProtonsFD_mom_ind, TL_piplusFD_mom_ind, TL_piminusFD_mom_ind;
            vector<int> TL_ProtonsCD_mom_ind, TL_piplusCD_mom_ind, TL_piminusCD_mom_ind;

            /* Particle index vectors (for FD particles above momentum threshold and within fiducial cuts (wFC)) */
            vector<int> TL_ElectronFD_wFC_mom_ind, TL_NeutronsFD_wFC_mom_ind, TL_ProtonsFD_wFC_mom_ind;

            double TL_Leading_nFD_momentum = -1;       // Leading nFD without momentum thresholds
            int TL_Leading_nFD_ind = -1;               // Leading nFD without momentum thresholds
            double TL_IDed_Leading_nFD_momentum = -1;  // Leading nFD with momentum thresholds
            int TL_IDed_Leading_nFD_ind = -1;          // Leading nFD with momentum thresholds
            double Leading_TL_FDNeutron_Momentum, Leading_TL_FDNeutron_Theta, Leading_TL_FDNeutron_Phi;
            bool Leading_Neutron_inFD_wFC;

            for (Int_t i = 0; i < Ngen; i++) {
                mcpbank->setEntry(i);

                int particlePDGtmp = mcpbank->getPid();

                double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
                double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                bool inFD = ((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= ThetaFD.GetUpperCut()));
                bool inCD = ((Particle_TL_Theta > ThetaCD.GetLowerCut()) && (Particle_TL_Theta <= ThetaCD.GetUpperCut()));

                if (particlePDGtmp == 11) {
                    bool e_inFD = aMaps_master.IsInFDQuery((!TL_fiducial_cuts), ThetaFD, "Electron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi);

                    if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) { TL_Electron_mom_ind.push_back(i); }

                    TL_Electron_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) {
                            TL_ElectronFD_mom_ind.push_back(i);

                            if (e_inFD) { TL_ElectronFD_wFC_mom_ind.push_back(i); }
                        }

                        TL_ElectronFD_ind.push_back(i);
                    }
                } else if (particlePDGtmp == 2112) {
                    bool n_inFD = aMaps_master.IsInFDQuery((!TL_fiducial_cuts), ThetaFD, "Neutron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Calc_eff_overlapping_FC);

                    if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) { TL_Neutrons_mom_ind.push_back(i); }

                    TL_Neutrons_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) {
                            TL_NeutronsFD_mom_ind.push_back(i);

                            if (Particle_TL_Momentum >= TL_IDed_Leading_nFD_momentum) {
                                TL_IDed_Leading_nFD_momentum = Particle_TL_Momentum;
                                TL_IDed_Leading_nFD_ind = i;
                            }

                            if (n_inFD) { TL_NeutronsFD_wFC_mom_ind.push_back(i); }
                        }

                        TL_NeutronsFD_ind.push_back(i);

                        if (Particle_TL_Momentum >= TL_Leading_nFD_momentum) {
                            TL_Leading_nFD_momentum = Particle_TL_Momentum;
                            TL_Leading_nFD_ind = i;
                        }
                    }
                } else if (particlePDGtmp == 2212) {
                    bool p_inFD = aMaps_master.IsInFDQuery((!TL_fiducial_cuts), ThetaFD, "Proton", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Calc_eff_overlapping_FC);

                    if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) { TL_Protons_mom_ind.push_back(i); }

                    TL_Protons_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_pFD_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pFD_mom_cuts.GetUpperCut())) {
                            TL_ProtonsFD_mom_ind.push_back(i);

                            if (p_inFD) { TL_ProtonsFD_wFC_mom_ind.push_back(i); }
                        }

                        TL_ProtonsFD_ind.push_back(i);
                    } else if (inCD) {
                        if ((Particle_TL_Momentum >= TL_pCD_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pCD_mom_cuts.GetUpperCut())) { TL_ProtonsCD_mom_ind.push_back(i); }

                        TL_ProtonsCD_ind.push_back(i);
                    }
                } else if (particlePDGtmp == 211) {
                    if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) { TL_piplus_mom_ind.push_back(i); }

                    TL_piplus_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_pipFD_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pipFD_mom_cuts.GetUpperCut())) { TL_piplusFD_mom_ind.push_back(i); }

                        TL_piplusFD_ind.push_back(i);
                    } else if (inCD) {
                        if ((Particle_TL_Momentum >= TL_pipCD_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pipCD_mom_cuts.GetUpperCut())) { TL_piplusCD_mom_ind.push_back(i); }

                        TL_piplusCD_ind.push_back(i);
                    }
                } else if (particlePDGtmp == -211) {
                    if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) { TL_piminus_mom_ind.push_back(i); }

                    TL_piminus_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_pimFD_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pimFD_mom_cuts.GetUpperCut())) { TL_piminusFD_mom_ind.push_back(i); }

                        TL_piminusFD_ind.push_back(i);
                    } else if (inCD) {
                        if ((Particle_TL_Momentum >= TL_pimCD_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pimCD_mom_cuts.GetUpperCut())) { TL_piminusCD_mom_ind.push_back(i); }

                        TL_piminusCD_ind.push_back(i);
                    }
                } else if (particlePDGtmp == 111) {
                    if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) { TL_pizero_mom_ind.push_back(i); }

                    TL_pizero_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) { TL_pi0FD_mom_ind.push_back(i); }

                        TL_pi0FD_ind.push_back(i);
                    }
                } else if (particlePDGtmp == 22) {
                    if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) { TL_Photons_mom_ind.push_back(i); }

                    TL_Photons_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) { TL_PhotonsFD_mom_ind.push_back(i); }

                        TL_PhotonsFD_ind.push_back(i);
                    }
                } else {
                    TL_OtherPart_ind.push_back(i);
                }
            }

            // Handling leading FD neutron
            if (TL_IDed_Leading_nFD_ind != -1) { TL_NeutronsFD_max_mom_ind.push_back(TL_IDed_Leading_nFD_ind); }

            // Check that leading FD neutron is within FC
            if (TL_IDed_Leading_nFD_ind != -1) {
                mcpbank->setEntry(TL_IDed_Leading_nFD_ind);

                Leading_TL_FDNeutron_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                Leading_TL_FDNeutron_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
                Leading_TL_FDNeutron_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                Leading_Neutron_inFD_wFC = aMaps_master.IsInFDQuery((!TL_fiducial_cuts), ThetaFD, "Neutron", Leading_TL_FDNeutron_Momentum, Leading_TL_FDNeutron_Theta,
                                                                    Leading_TL_FDNeutron_Phi, Calc_eff_overlapping_FC);
            } else {
                Leading_Neutron_inFD_wFC = false;
            }

            // Safety check for truth leading FD neutron
            CodeDebugger.SafetyCheck_Truth_leading_FD_neutron(__FILE__, __LINE__, ES_by_leading_FDneutron, TL_IDed_Leading_nFD_ind, TL_IDed_Leading_nFD_momentum,
                                                              Leading_TL_FDNeutron_Momentum, TL_NeutronsFD_mom_ind, mcpbank);

            // Event selection for TL plots

            // Setting up event selection for AMaps
            bool TL_Event_Selection_1e_cut_AMaps = ((TL_Electron_mom_ind.size() == 1) && (TL_ElectronFD_mom_ind.size() == 1));
            // One id. FD electron above momentum th.

            // Setting up basic TL event selection
            bool TL_Event_Selection_1e_cut = (TL_Event_Selection_1e_cut_AMaps && TL_ElectronFD_mom_ind.size() == TL_ElectronFD_wFC_mom_ind.size());
            // One id. FD electron above momentum threshold
            TL_Event_Selection_inclusive = TL_Event_Selection_1e_cut;
            bool no_TL_pip = (TL_piplusFD_mom_ind.size() == 0 && TL_piplusCD_mom_ind.size() == 0);    // No pi+ above momentum threshold (CD & FD)
            bool no_TL_pim = (TL_piminusFD_mom_ind.size() == 0 && TL_piminusCD_mom_ind.size() == 0);  // No pi- above momentum threshold (CD & FD)
            bool no_TL_cPions = (no_TL_pip && no_TL_pim);                                             // No id. cPions above momentum threshold
            bool no_TL_OtherPart = (TL_OtherPart_ind.size() == 0);                                    // No other part. above momentum threshold
            bool no_TL_FDpi0 = (Enable_FD_photons || (TL_pi0FD_mom_ind.size() == 0));                 // No id. pi0 in the FD above momentum threshold
            bool no_TL_FDPhotons = (Enable_FD_photons || (TL_PhotonsFD_mom_ind.size() == 0));         // No id. photons in the FD above momentum threshold
            bool TL_Basic_ES = (TL_Event_Selection_1e_cut && no_TL_cPions && no_TL_OtherPart && no_TL_FDpi0 && no_TL_FDPhotons);

            // Setting up 1p TL event selection
            // 1p = one id. FD proton (any or no FD neutrons, according to the value of Enable_FD_neutrons):
            bool TL_FDneutrons_1p = (Enable_FD_neutrons || (TL_NeutronsFD_mom_ind.size() == 0));  // no id. FD neutrons for Enable_FD_neutrons = false
            bool no_CDproton_1p = (TL_ProtonsCD_mom_ind.size() == 0);
            bool one_FDproton_1p = ((TL_ProtonsFD_mom_ind.size() == 1) && (TLKinCutsCheck(c12, apply_kinematical_cuts, TL_ProtonsFD_mom_ind, FD_nucleon_theta_cut, FD_nucleon_momentum_cut)));
            bool FDproton_wFC_1p = (TL_ProtonsFD_mom_ind.size() == TL_ProtonsFD_wFC_mom_ind.size());  // id. FD proton is within fiducial cuts (wFC)
            TL_Event_Selection_1p = (TL_Basic_ES && TL_FDneutrons_1p && no_CDproton_1p && one_FDproton_1p && FDproton_wFC_1p);

            // Setting up 1n TL event selection
            // 1n = any number of id. FD neutron (we look at the leading nFD) & no id. protons:
            bool one_FDneutron_1n = ((TL_IDed_Leading_nFD_ind != -1) &&  // for TL_IDed_Leading_nFD_ind = -1 we don't have any nFD
                                     (TLKinCutsCheck(c12, apply_kinematical_cuts, TL_IDed_Leading_nFD_ind, FD_nucleon_theta_cut, FD_nucleon_momentum_cut)));
            bool no_protons_1n = ((TL_ProtonsCD_mom_ind.size() == 0) && (TL_ProtonsFD_mom_ind.size() == 0));
            bool FDneutron_wFC_1p = Leading_Neutron_inFD_wFC;  // leading nFD is within fiducial cuts (wFC)
            TL_Event_Selection_1n = (TL_Basic_ES && one_FDneutron_1n && no_protons_1n && FDneutron_wFC_1p);

            // Setting up pFDpCD TL event selection
            // pFDpCD = One id. FD proton & one id. CD proton:
            bool one_CDproton_pFDpCD = (TL_ProtonsCD_mom_ind.size() == 1);
            bool one_FDproton_pFDpCD =
                ((TL_ProtonsFD_mom_ind.size() == 1) && (TLKinCutsCheck(c12, apply_kinematical_cuts, TL_ProtonsFD_mom_ind, FD_nucleon_theta_cut, FD_nucleon_momentum_cut)));
            bool TL_FDneutrons_pFDpCD = (Enable_FD_neutrons || (TL_NeutronsFD_mom_ind.size() == 0));
            // no id. FD neutrons for Enable_FD_neutrons = false
            bool FDproton_wFC_pFDpCD = (TL_ProtonsFD_mom_ind.size() == TL_ProtonsFD_wFC_mom_ind.size());
            // id. FD proton is within fiducial cuts (wFC)
            TL_Event_Selection_pFDpCD = (TL_Basic_ES && one_CDproton_pFDpCD && one_FDproton_pFDpCD && TL_FDneutrons_pFDpCD && FDproton_wFC_pFDpCD);

            // Setting up nFDpCD TL event selection
            // nFDpCD = any number of id. FD neutron (we look at the leading nFD) & one id. CD proton:
            bool one_CDproton_nFDpCD = (TL_ProtonsCD_mom_ind.size() == 1);
            bool no_FDproton_nFDpCD = (TL_ProtonsFD_mom_ind.size() == 0);
            bool one_FDNeutron_nFDpCD = ((TL_IDed_Leading_nFD_ind != -1) &&  // for TL_IDed_Leading_nFD_ind = -1 we don't have any nFD
                                         (TLKinCutsCheck(c12, apply_kinematical_cuts, TL_IDed_Leading_nFD_ind, FD_nucleon_theta_cut, FD_nucleon_momentum_cut)));
            bool FDneutron_wFC_nFDpCD = Leading_Neutron_inFD_wFC;  // leading nFD is within fiducial cuts (wFC)
            TL_Event_Selection_nFDpCD = (TL_Basic_ES && one_CDproton_nFDpCD && no_FDproton_nFDpCD && one_FDNeutron_nFDpCD && FDneutron_wFC_nFDpCD);

            // Fill TL histograms
            for (Int_t i = 0; i < Ngen; i++) {
                mcpbank->setEntry(i);

                int particlePDGtmp = mcpbank->getPid();

                double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
                double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                bool inFD = ((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= ThetaFD.GetUpperCut()));
                bool inCD = ((Particle_TL_Theta > ThetaCD.GetLowerCut()) && (Particle_TL_Theta <= ThetaCD.GetUpperCut()));

                if (fill_TL_plots) {
                    if (particlePDGtmp == 11) {
                        // is electron
                        if (TL_Event_Selection_1e_cut) {
                            if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) {
                                hP_e_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                                hTheta_e_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                                hPhi_e_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                                hTheta_e_vs_Phi_e_truth_1e_cut.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                            }

                            hP_e_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_e_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_e_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                // inclusive efficiency plots
                                hP_e_truth_1e_cut_FD.hFill(Particle_TL_Momentum, Weight);
                                hP_e_truth_1e_cut_FD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                            }
                        }

                        if (TL_Event_Selection_1p) {
                            if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) {
                                hP_e_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                                hTheta_e_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                                hPhi_e_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                                hTheta_e_vs_Phi_e_truth_1p.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                            }

                            hP_e_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_e_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_e_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_1n) {
                            if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) {
                                hP_e_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                hTheta_e_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                hPhi_e_AC_truth_1n.hFill(Particle_TL_Phi, Weight);
                                hTheta_e_vs_Phi_e_truth_1n.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                            }

                            hP_e_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_e_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_e_BC_truth_1n.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_pFDpCD) {
                            if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) {
                                hP_e_AC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_e_AC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_e_AC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                                hTheta_e_vs_Phi_e_truth_pFDpCD.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                            }

                            hP_e_BC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_e_BC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_e_BC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_nFDpCD) {
                            if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) {
                                hP_e_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_e_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_e_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                                hTheta_e_vs_Phi_e_truth_nFDpCD.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                            }

                            hP_e_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_e_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_e_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                        }
                    } else if (particlePDGtmp == 2112) {
                        // is neutron
                        bool n_inFD = aMaps_master.IsInFDQuery((!TL_fiducial_cuts), ThetaFD, "Neutron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Calc_eff_overlapping_FC);

                        if (TL_Event_Selection_1e_cut) {
                            if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) {
                                hP_n_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                                hTheta_n_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                                hPhi_n_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);

                                if (n_inFD) { hTheta_nFD_vs_Phi_nFD_truth_1e_cut.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight); }
                            }

                            hP_n_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_n_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_n_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);

                            if (n_inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                // inclusive efficiency plots (LnFD and all nFD)
                                hP_nFD_truth_1e_cut_FD.hFill(Particle_TL_Momentum, Weight);
                                hP_nFD_truth_1e_cut_FD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);

                                if (i == TL_IDed_Leading_nFD_ind) {
                                    hP_LnFD_truth_1e_cut_FD.hFill(Particle_TL_Momentum, Weight);
                                    hP_LnFD_truth_1e_cut_FD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }
                            }
                        }

                        if (TL_Event_Selection_1p) {
                            if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) {
                                hP_n_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                                hTheta_n_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                                hPhi_n_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_n_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_n_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_n_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_1n) {
                            if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) {
                                hP_n_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                hTheta_n_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                hPhi_n_AC_truth_1n.hFill(Particle_TL_Phi, Weight);

                                if (  //
                                      // (TL_NeutronsFD_mom_ind.size() == 1) &&  // FOR nFD eff test!
                                    n_inFD && (!Calc_1n_n_eff_with_smaller_theta || (Particle_TL_Theta <= 35.)) && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))  //
                                ) {
                                    // 1n efficiency plots (LnFD)
                                    if (ES_by_leading_FDneutron) {
                                        if ((TL_IDed_Leading_nFD_ind != -1) && (i == TL_IDed_Leading_nFD_ind)) {
                                            hP_nFD_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                            hTheta_nFD_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                            hPhi_nFD_AC_truth_1n.hFill(Particle_TL_Phi, Weight);
                                            hTheta_nFD_vs_Phi_nFD_truth_1n.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                                        }
                                    } else {
                                        hP_nFD_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                        hTheta_nFD_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                        hPhi_nFD_AC_truth_1n.hFill(Particle_TL_Phi, Weight);
                                        hTheta_nFD_vs_Phi_nFD_truth_1n.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                                    }
                                }
                            }

                            if (true)  // FOR nFD eff test!
                            // if (TL_NeutronsFD_mom_ind.size() == 1)  // FOR nFD eff test!
                            {
                                hP_n_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                hTheta_n_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                hPhi_n_BC_truth_1n.hFill(Particle_TL_Phi, Weight);

                                if (n_inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // 1n efficiency plots (nFD)
                                    hP_nFD_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_nFD_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                    hPhi_nFD_BC_truth_1n.hFill(Particle_TL_Phi, Weight);
                                }
                            }
                        }

                        if (TL_Event_Selection_pFDpCD) {
                            if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) {
                                hP_n_AC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_n_AC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_n_AC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_n_BC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_n_BC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_n_BC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_nFDpCD) {
                            if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) {
                                hP_n_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_n_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_n_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);

                                if (n_inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // 1nFD1pCD efficiency plots (nFD)
                                    if (ES_by_leading_FDneutron) {
                                        if ((TL_IDed_Leading_nFD_ind != -1) && (i == TL_IDed_Leading_nFD_ind)) {
                                            hP_nFD_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                            hTheta_nFD_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                            hPhi_nFD_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                                            hTheta_nFD_vs_Phi_nFD_truth_nFDpCD.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                                        }
                                    } else {
                                        hP_nFD_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                        hTheta_nFD_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                        hPhi_nFD_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                                        hTheta_nFD_vs_Phi_nFD_truth_nFDpCD.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                                    }
                                }
                            }

                            hP_n_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_n_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_n_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);

                            if (n_inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                // 1nFD1pCD efficiency plots (nFD)
                                hP_nFD_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_nFD_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_nFD_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                            }
                        }
                    } else if (particlePDGtmp == 2212) {
                        // is proton
                        bool p_inFD = aMaps_master.IsInFDQuery((!TL_fiducial_cuts), ThetaFD, "Proton", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Calc_eff_overlapping_FC);

                        if (TL_Event_Selection_1e_cut) {
                            if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) {
                                hP_p_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                                hTheta_p_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                                hPhi_p_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);

                                if (p_inFD) { hTheta_pFD_vs_Phi_pFD_truth_1e_cut.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight); }
                            }

                            hP_p_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_p_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_p_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);

                            if (!Calc_inc_eff_with_varying_theta) {
                                // Inclusive proton efficiency plots WITHOUT FD-CD overlap in theta
                                if (inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // inclusive efficiency plots (FD protons)
                                    hP_p_truth_1e_cut_FD.hFill(Particle_TL_Momentum, Weight);
                                    hP_p_truth_1e_cut_FD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }

                                if (inCD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // inclusive efficiency plots (CD protons)
                                    hP_p_truth_1e_cut_CD.hFill(Particle_TL_Momentum, Weight);
                                    hP_p_truth_1e_cut_CD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }
                            } else {
                                // Inclusive proton efficiency plots WITH FD-CD overlap in theta
                                if (((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= 45)) && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // inclusive efficiency plots (FD protons)
                                    hP_p_truth_1e_cut_FD.hFill(Particle_TL_Momentum, Weight);
                                    hP_p_truth_1e_cut_FD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }

                                if (((Particle_TL_Theta > 35) && (Particle_TL_Theta <= ThetaCD.GetUpperCut())) && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // inclusive efficiency plots (CD protons)
                                    hP_p_truth_1e_cut_CD.hFill(Particle_TL_Momentum, Weight);
                                    hP_p_truth_1e_cut_CD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }
                            }
                        }

                        if (TL_Event_Selection_1p) {
                            if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) {
                                hP_p_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                                hTheta_p_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                                hPhi_p_AC_truth_1p.hFill(Particle_TL_Phi, Weight);

                                if (p_inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // 1p efficiency plots (pFD)
                                    hP_pFD_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pFD_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pFD_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                                    hTheta_pFD_vs_Phi_pFD_truth_1p.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                                }
                            }

                            hP_p_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_p_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_p_BC_truth_1p.hFill(Particle_TL_Phi, Weight);

                            if (p_inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                // 1p efficiency plots (pFD)
                                hP_pFD_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pFD_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                                hPhi_pFD_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        if (TL_Event_Selection_1n) {
                            if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) {
                                hP_p_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                hTheta_p_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                hPhi_p_AC_truth_1n.hFill(Particle_TL_Phi, Weight);

                                if (inFD) {
                                    hP_p_AC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_p_AC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_p_AC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                                }

                                if (inCD) {
                                    hP_p_AC_truth_1n_CD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_p_AC_truth_1n_CD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_p_AC_truth_1n_CD.hFill(Particle_TL_Phi, Weight);
                                }

                                if (!(inCD || inFD)) {
                                    hP_p_AC_truth_1n_undet.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_p_AC_truth_1n_undet.hFill(Particle_TL_Theta, Weight);
                                    hPhi_p_AC_truth_1n_undet.hFill(Particle_TL_Phi, Weight);
                                }
                            }

                            hP_p_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_p_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_p_BC_truth_1n.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_p_BC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_p_BC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_p_BC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                            }

                            if (inCD) {
                                hP_p_BC_truth_1n_CD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_p_BC_truth_1n_CD.hFill(Particle_TL_Theta, Weight);
                                hPhi_p_BC_truth_1n_CD.hFill(Particle_TL_Phi, Weight);
                            }

                            if (!(inCD || inFD)) {
                                hP_p_BC_truth_1n_undet.hFill(Particle_TL_Momentum, Weight);
                                hTheta_p_BC_truth_1n_undet.hFill(Particle_TL_Theta, Weight);
                                hPhi_p_BC_truth_1n_undet.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        if (TL_Event_Selection_pFDpCD) {
                            if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) {
                                hP_p_AC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_p_AC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_p_AC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);

                                if (p_inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // 1pFD1pCD efficiency plots (pFD)
                                    hP_pFD_AC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pFD_AC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pFD_AC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                                    hTheta_pFD_vs_Phi_pFD_truth_pFDpCD.hFill(Particle_TL_Phi, Particle_TL_Theta, Weight);
                                }

                                if (inCD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // 1pFD1pCD efficiency plots (pCD)
                                    hP_pCD_AC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pCD_AC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pCD_AC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                                }
                            }

                            hP_p_BC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_p_BC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_p_BC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);

                            if (p_inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                // 1pFD1pCD efficiency plots (pFD)
                                hP_pFD_BC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pFD_BC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pFD_BC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                            }

                            if (inCD) {
                                hP_pCD_BC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pCD_BC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pCD_BC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        if (TL_Event_Selection_nFDpCD) {
                            if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) {
                                hP_p_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_p_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_p_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);

                                if (p_inFD) {
                                    hP_pFD_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pFD_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pFD_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                                }

                                if (inCD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // 1nFD1pCD efficiency plots (pCD)
                                    hP_pCD_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pCD_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pCD_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                                }
                            }

                            hP_p_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_p_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_p_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);

                            if (p_inFD) {
                                hP_pFD_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pFD_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pFD_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                            }

                            if (inCD) {
                                hP_pCD_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pCD_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pCD_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                            }
                        }
                    } else if (particlePDGtmp == 211) {
                        // is pi+
                        if (TL_Event_Selection_1e_cut) {
                            if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) {
                                hP_pip_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pip_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                                hPhi_pip_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pip_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pip_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_pip_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);

                            if ((inCD || inFD) && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                // inclusive efficiency plots (pi+)
                                hP_piplus_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                                hP_piplus_truth_1e_cut_ZOOMIN.hFill(Particle_TL_Momentum, Weight);

                                if (inFD) {
                                    hP_piplus_truth_1e_cut_FD.hFill(Particle_TL_Momentum, Weight);
                                    hP_piplus_truth_1e_cut_FD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }

                                if (inCD) {
                                    hP_piplus_truth_1e_cut_CD.hFill(Particle_TL_Momentum, Weight);
                                    hP_piplus_truth_1e_cut_CD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }
                            }
                        }

                        if (TL_Event_Selection_1p) {
                            if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) {
                                hP_pip_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pip_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                                hPhi_pip_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pip_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pip_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_pip_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_1n) {
                            if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) {
                                hP_pip_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pip_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                hPhi_pip_AC_truth_1n.hFill(Particle_TL_Phi, Weight);

                                if (inFD) {
                                    hP_pip_AC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pip_AC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pip_AC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                                }

                                if (inCD) {
                                    hP_pip_AC_truth_1n_CD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pip_AC_truth_1n_CD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pip_AC_truth_1n_CD.hFill(Particle_TL_Phi, Weight);
                                }

                                if (!(inCD || inFD)) {
                                    hP_pip_AC_truth_1n_undet.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pip_AC_truth_1n_undet.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pip_AC_truth_1n_undet.hFill(Particle_TL_Phi, Weight);
                                }
                            }

                            hP_pip_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pip_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_pip_BC_truth_1n.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_pip_BC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pip_BC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pip_BC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                            }

                            if (inCD) {
                                hP_pip_BC_truth_1n_CD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pip_BC_truth_1n_CD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pip_BC_truth_1n_CD.hFill(Particle_TL_Phi, Weight);
                            }

                            if (!(inCD || inFD)) {
                                hP_pip_BC_truth_1n_undet.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pip_BC_truth_1n_undet.hFill(Particle_TL_Theta, Weight);
                                hPhi_pip_BC_truth_1n_undet.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        if (TL_Event_Selection_pFDpCD) {
                            if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) {
                                hP_pip_AC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pip_AC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pip_AC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pip_BC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pip_BC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_pip_BC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_nFDpCD) {
                            if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) {
                                hP_pip_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pip_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pip_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pip_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pip_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_pip_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                        }
                    } else if (particlePDGtmp == -211) {
                        // is pi-
                        if (TL_Event_Selection_1e_cut) {
                            if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) {
                                hP_pim_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pim_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                                hPhi_pim_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pim_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pim_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_pim_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);

                            if ((inCD || inFD) && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                // inclusive efficiency plots (pi-)
                                hP_piminus_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                                hP_piminus_truth_1e_cut_ZOOMIN.hFill(Particle_TL_Momentum, Weight);

                                if (inFD) {
                                    hP_piminus_truth_1e_cut_FD.hFill(Particle_TL_Momentum, Weight);
                                    hP_piminus_truth_1e_cut_FD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }

                                if (inCD) {
                                    hP_piminus_truth_1e_cut_CD.hFill(Particle_TL_Momentum, Weight);
                                    hP_piminus_truth_1e_cut_CD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }
                            }
                        }

                        if (TL_Event_Selection_1p) {
                            if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) {
                                hP_pim_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pim_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                                hPhi_pim_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pim_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pim_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_pim_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_1n) {
                            if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) {
                                hP_pim_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pim_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                hPhi_pim_AC_truth_1n.hFill(Particle_TL_Phi, Weight);

                                if (inFD) {
                                    hP_pim_AC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pim_AC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pim_AC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                                }

                                if (inCD) {
                                    hP_pim_AC_truth_1n_CD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pim_AC_truth_1n_CD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pim_AC_truth_1n_CD.hFill(Particle_TL_Phi, Weight);
                                }

                                if (!(inCD || inFD)) {
                                    hP_pim_AC_truth_1n_undet.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_pim_AC_truth_1n_undet.hFill(Particle_TL_Theta, Weight);
                                    hPhi_pim_AC_truth_1n_undet.hFill(Particle_TL_Phi, Weight);
                                }
                            }

                            hP_pim_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pim_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_pim_BC_truth_1n.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_pim_BC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pim_BC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pim_BC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                            }

                            if (inCD) {
                                hP_pim_BC_truth_1n_CD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pim_BC_truth_1n_CD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pim_BC_truth_1n_CD.hFill(Particle_TL_Phi, Weight);
                            }

                            if (!(inCD || inFD)) {
                                hP_pim_BC_truth_1n_undet.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pim_BC_truth_1n_undet.hFill(Particle_TL_Theta, Weight);
                                hPhi_pim_BC_truth_1n_undet.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        if (TL_Event_Selection_pFDpCD) {
                            if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) {
                                hP_pim_AC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pim_AC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pim_AC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pim_BC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pim_BC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_pim_BC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_nFDpCD) {
                            if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) {
                                hP_pim_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pim_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pim_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pim_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pim_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_pim_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                        }
                    } else if (particlePDGtmp == 111) {
                        // is pi0
                        if (TL_Event_Selection_1e_cut) {
                            if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) {
                                hP_pi0_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pi0_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                                hPhi_pi0_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pi0_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pi0_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_pi0_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_1p) {
                            if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) {
                                hP_pi0_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pi0_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                                hPhi_pi0_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pi0_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pi0_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_pi0_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_1n) {
                            if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) {
                                hP_pi0_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pi0_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                hPhi_pi0_AC_truth_1n.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pi0_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pi0_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_pi0_BC_truth_1n.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_pFDpCD) {
                            if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) {
                                hP_pi0_AC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pi0_AC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pi0_AC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pi0_BC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pi0_BC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_pi0_BC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);
                        }

                        if (TL_Event_Selection_nFDpCD) {
                            if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) {
                                hP_pi0_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_pi0_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_pi0_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_pi0_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pi0_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_pi0_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);
                        }
                    } else if (particlePDGtmp == 22) {
                        // is photon
                        if (TL_Event_Selection_1e_cut) {
                            if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) {
                                hP_ph_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                            }

                            hP_ph_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);

                            if (!Calc_inc_eff_with_varying_theta) {
                                if (inFD && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // inclusive efficiency plots (photons)
                                    hP_ph_truth_1e_cut_FD.hFill(Particle_TL_Momentum, Weight);
                                    hP_ph_truth_1e_cut_FD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }
                            } else {
                                if (((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= 35.)) && (!Eff_calc_with_one_reco_electron || (electrons.size() == 1))) {
                                    // inclusive efficiency plots (photons)
                                    hP_ph_truth_1e_cut_FD.hFill(Particle_TL_Momentum, Weight);
                                    hP_ph_truth_1e_cut_FD_ZOOMIN.hFill(Particle_TL_Momentum, Weight);
                                }
                            }
                        }

                        if (TL_Event_Selection_1p) {
                            if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) {
                                hP_ph_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_AC_truth_1p.hFill(Particle_TL_Phi, Weight);

                                if (inFD) {
                                    hP_ph_AC_truth_1p_FD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_ph_AC_truth_1p_FD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_ph_AC_truth_1p_FD.hFill(Particle_TL_Phi, Weight);
                                }
                            }

                            hP_ph_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_BC_truth_1p.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_ph_BC_truth_1p_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_BC_truth_1p_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_BC_truth_1p_FD.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        if (TL_Event_Selection_1n) {
                            if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) {
                                hP_ph_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_AC_truth_1n.hFill(Particle_TL_Phi, Weight);

                                if (inFD) {
                                    hP_ph_AC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_ph_AC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_ph_AC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                                }
                            }

                            hP_ph_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_BC_truth_1n.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_ph_BC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_BC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_BC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        if (TL_Event_Selection_pFDpCD) {
                            if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) {
                                hP_ph_AC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_AC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_AC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);

                                if (inFD) {
                                    hP_ph_AC_truth_pFDpCD_FD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_ph_AC_truth_pFDpCD_FD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_ph_AC_truth_pFDpCD_FD.hFill(Particle_TL_Phi, Weight);
                                }
                            }

                            hP_ph_BC_truth_pFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_BC_truth_pFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_BC_truth_pFDpCD.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_ph_BC_truth_pFDpCD_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_BC_truth_pFDpCD_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_BC_truth_pFDpCD_FD.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        if (TL_Event_Selection_nFDpCD) {
                            if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) {
                                hP_ph_AC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_AC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_AC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);

                                if (inFD) {
                                    hP_ph_AC_truth_nFDpCD_FD.hFill(Particle_TL_Momentum, Weight);
                                    hTheta_ph_AC_truth_nFDpCD_FD.hFill(Particle_TL_Theta, Weight);
                                    hPhi_ph_AC_truth_nFDpCD_FD.hFill(Particle_TL_Phi, Weight);
                                }
                            }

                            hP_ph_BC_truth_nFDpCD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_BC_truth_nFDpCD.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_BC_truth_nFDpCD.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_ph_BC_truth_nFDpCD_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_BC_truth_nFDpCD_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_BC_truth_nFDpCD_FD.hFill(Particle_TL_Phi, Weight);
                            }
                        }
                    }
                }
            }  // end of for loop over TL particles

            // Fill AMaps
            double Electron_TL_Momentum, Electron_TL_Theta, Electron_TL_Phi;

            for (Int_t i = 0; i < Ngen; i++) {
                mcpbank->setEntry(i);

                int particlePDGtmp = mcpbank->getPid();

                double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
                double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                bool inFD_AMaps = ((Particle_TL_Theta >= ThetaFD_AMaps.GetLowerCut()) && (Particle_TL_Theta <= ThetaFD_AMaps.GetUpperCut()));

                // Fill electron acceptance maps
                if (Generate_Electron_AMaps && TL_Event_Selection_1e_cut_AMaps && inFD_AMaps) {
                    // NOTE: here we fill Acceptance maps before their generation - no fiducial cuts yet!
                    if (particlePDGtmp == 11) {
                        /* Fill TL electron acceptance maps */

                        // Safety checks for TL electrons (AMaps & WMaps)
                        CodeDebugger.SafetyCheck_AMaps_Truth_electrons(__FILE__, __LINE__, particlePDGtmp, inFD_AMaps);

                        Electron_TL_Momentum = Particle_TL_Momentum;
                        Electron_TL_Theta = Particle_TL_Theta;
                        Electron_TL_Phi = Particle_TL_Phi;

                        hTL_P_e_AMaps.hFill(Particle_TL_Momentum, Weight);
                        hTL_P_e_vs_TL_Theta_e_AMap.hFill(Particle_TL_Momentum, Particle_TL_Theta, Weight);
                        hTL_P_e_vs_TL_Phi_e_AMap.hFill(Particle_TL_Momentum, Particle_TL_Phi, Weight);
                        aMaps_master.hFillHitMaps("TL", "Electron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Weight);
                    }
                }

                // Fill proton acceptance maps
                if (Generate_Nucleon_AMaps && TL_Event_Selection_1e_cut_AMaps && inFD_AMaps) {
                    // NOTE: here we fill Acceptance maps before their generation - no fiducial cuts yet!
                    if ((particlePDGtmp == 2212) && (!AMaps_calc_with_one_reco_electron || (electrons.size() == 1))) {
                        /* Fill all TL FD proton acceptance maps */

                        if ((Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut()) && (Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut())) {
                            // if id. TL proton

                            // Safety checks for TL protons (AMaps & WMaps)
                            CodeDebugger.SafetyCheck_AMaps_Truth_protons(__FILE__, __LINE__, particlePDGtmp, inFD_AMaps);

                            hTL_P_pFD_AMaps.hFill(Particle_TL_Momentum, Weight);
                            hTL_P_pFD_vs_TL_Theta_pFD_AMap.hFill(Particle_TL_Momentum, Particle_TL_Theta, Weight);
                            hTL_P_pFD_vs_TL_Phi_pFD_AMap.hFill(Particle_TL_Momentum, Particle_TL_Phi, Weight);
                            hTL_P_pFD_vs_TL_P_e_AMap.hFill(Particle_TL_Momentum, Electron_TL_Momentum, Weight);
                            hTL_P_pFD_vs_TL_Theta_e_AMap.hFill(Particle_TL_Momentum, Electron_TL_Theta, Weight);
                            hTL_P_pFD_vs_TL_Phi_e_AMap.hFill(Particle_TL_Momentum, Electron_TL_Phi, Weight);
                            aMaps_master.hFillHitMaps("TL", "Proton", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Weight);
                        }  // end of if id. TL proton
                    }
                }

                // Fill electron and proton efficiency maps
                if (Generate_WMaps && TL_Event_Selection_1e_cut_AMaps && inFD_AMaps) {
                    // NOTE: here we fill efficiency maps before their generation - no fiducial cuts yet!
                    if (particlePDGtmp == 11) {
                        /* Fill TL electron efficiency maps */

                        // Safety checks for TL electrons (AMaps & WMaps)
                        CodeDebugger.SafetyCheck_AMaps_Truth_electrons(__FILE__, __LINE__, particlePDGtmp, inFD_AMaps);

                        Electron_TL_Momentum = Particle_TL_Momentum;
                        Electron_TL_Theta = Particle_TL_Theta;
                        Electron_TL_Phi = Particle_TL_Phi;

                        hTL_P_e_WMaps.hFill(Particle_TL_Momentum, Weight);
                        hTL_P_e_vs_TL_Theta_e_WMap.hFill(Particle_TL_Momentum, Particle_TL_Theta, Weight);
                        hTL_P_e_vs_TL_Phi_e_WMap.hFill(Particle_TL_Momentum, Particle_TL_Phi, Weight);
                        wMaps_master.hFillHitMaps("TL", "Electron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Weight);
                    } else if ((particlePDGtmp == 2212) && (!AMaps_calc_with_one_reco_electron || (electrons.size() == 1))) {
                        /* Fill all TL FD proton efficiency maps */

                        if ((Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut()) && (Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut())) {
                            // if id. TL proton

                            // Safety checks for TL protons (AMaps & WMaps)
                            CodeDebugger.SafetyCheck_AMaps_Truth_protons(__FILE__, __LINE__, particlePDGtmp, inFD_AMaps);

                            bool FD_Theta_Cut_TL_protons = (Particle_TL_Theta <= FD_nucleon_theta_cut.GetUpperCut());
                            bool FD_Momentum_Cut_TL_protons =
                                ((Particle_TL_Momentum <= FD_nucleon_momentum_cut.GetUpperCut()) && (Particle_TL_Momentum >= FD_nucleon_momentum_cut.GetLowerCut()));

                            if (FD_Theta_Cut_TL_protons && FD_Momentum_Cut_TL_protons) {
                                hTL_P_pFD_WMaps.hFill(Particle_TL_Momentum, Weight);
                                hTL_P_pFD_vs_TL_Theta_pFD_WMap.hFill(Particle_TL_Momentum, Particle_TL_Theta, Weight);
                                hTL_P_pFD_vs_TL_Phi_pFD_WMap.hFill(Particle_TL_Momentum, Particle_TL_Phi, Weight);
                                hTL_P_pFD_vs_TL_P_e_WMap.hFill(Particle_TL_Momentum, Electron_TL_Momentum, Weight);
                                hTL_P_pFD_vs_TL_Theta_e_WMap.hFill(Particle_TL_Momentum, Electron_TL_Theta, Weight);
                                hTL_P_pFD_vs_TL_Phi_e_WMap.hFill(Particle_TL_Momentum, Electron_TL_Phi, Weight);
                                wMaps_master.hFillHitMaps("TL", "Proton", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Weight);
                            }
                        }  // end of if id. TL proton
                    }
                }
            }  // end of for loop over TL particles

            // Fill leading FD neutron acceptance maps
            if (
                // if ((TL_NeutronsFD_mom_ind.size() == 1) &&  // FOR nFD eff test!
                Generate_Nucleon_AMaps && TL_Event_Selection_1e_cut_AMaps && (!AMaps_calc_with_one_reco_electron || (electrons.size() == 1)) && ES_by_leading_FDneutron &&
                ((TL_IDed_Leading_nFD_ind != -1) && (TL_IDed_Leading_nFD_momentum > 0))) {
                /* Fill leading TL FD neutron acceptance maps */

                mcpbank->setEntry(TL_IDed_Leading_nFD_ind);

                int particlePDGtmp = mcpbank->getPid();

                double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
                double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                bool inFD = ((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= ThetaFD.GetUpperCut()));
                bool inCD = ((Particle_TL_Theta > ThetaCD.GetLowerCut()) && (Particle_TL_Theta <= ThetaCD.GetUpperCut()));

                if ((Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut()) && (Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut())) {
                    // if id. TL leading neutron

                    // Safety checks for TL neutrons (AMaps & WMaps)
                    CodeDebugger.SafetyCheck_AMaps_Truth_neutrons(__FILE__, __LINE__, particlePDGtmp, inFD);

                    TL_nFD_theta = Particle_TL_Theta;  // FOR nFD eff test!
                    TL_nFD_phi = Particle_TL_Phi;      // FOR nFD eff test!

                    hTL_P_nFD_AMaps.hFill(Particle_TL_Momentum, Weight);
                    hTL_P_nFD_vs_TL_Theta_nFD_AMap.hFill(Particle_TL_Momentum, Particle_TL_Theta, Weight);
                    hTL_P_nFD_vs_TL_Phi_nFD_AMap.hFill(Particle_TL_Momentum, Particle_TL_Phi, Weight);
                    hTL_P_nFD_vs_TL_P_e_AMap.hFill(Particle_TL_Momentum, Electron_TL_Momentum, Weight);
                    hTL_P_nFD_vs_TL_Theta_e_AMap.hFill(Particle_TL_Momentum, Electron_TL_Theta, Weight);
                    hTL_P_nFD_vs_TL_Phi_e_AMap.hFill(Particle_TL_Momentum, Electron_TL_Phi, Weight);
                    aMaps_master.hFillHitMaps("TL", "Neutron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Weight);
                }  // end of if id. TL leading neutron
            }

            // Fill leading FD neutron efficiency maps
            if (Generate_WMaps && TL_Event_Selection_1e_cut_AMaps && (!AMaps_calc_with_one_reco_electron || (electrons.size() == 1)) && ES_by_leading_FDneutron &&
                ((TL_IDed_Leading_nFD_ind != -1) && (TL_IDed_Leading_nFD_momentum > 0))) {
                /* Fill leading TL FD neutron efficiency maps */

                mcpbank->setEntry(TL_IDed_Leading_nFD_ind);

                int particlePDGtmp = mcpbank->getPid();

                double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
                double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                bool inFD = ((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= ThetaFD.GetUpperCut()));
                bool inCD = ((Particle_TL_Theta > ThetaCD.GetLowerCut()) && (Particle_TL_Theta <= ThetaCD.GetUpperCut()));

                if ((Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut()) && (Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut())) {
                    // if id. TL leading neutron

                    // Safety checks for TL neutrons (AMaps & WMaps)
                    CodeDebugger.SafetyCheck_AMaps_Truth_neutrons(__FILE__, __LINE__, particlePDGtmp, inFD);

                    bool FD_Theta_Cut_TL_neutrons = (Particle_TL_Theta <= FD_nucleon_theta_cut.GetUpperCut());
                    bool FD_Momentum_Cut_TL_neutrons = ((Particle_TL_Momentum <= FD_nucleon_momentum_cut.GetUpperCut()) && (Particle_TL_Momentum >= FD_nucleon_momentum_cut.GetLowerCut()));

                    if (FD_Theta_Cut_TL_neutrons && FD_Momentum_Cut_TL_neutrons) {
                        hTL_P_nFD_WMaps.hFill(Particle_TL_Momentum, Weight);
                        hTL_P_nFD_vs_TL_Theta_nFD_WMap.hFill(Particle_TL_Momentum, Particle_TL_Theta, Weight);
                        hTL_P_nFD_vs_TL_Phi_nFD_WMap.hFill(Particle_TL_Momentum, Particle_TL_Phi, Weight);
                        hTL_P_nFD_vs_TL_P_e_WMap.hFill(Particle_TL_Momentum, Electron_TL_Momentum, Weight);
                        hTL_P_nFD_vs_TL_Theta_e_WMap.hFill(Particle_TL_Momentum, Electron_TL_Theta, Weight);
                        hTL_P_nFD_vs_TL_Phi_e_WMap.hFill(Particle_TL_Momentum, Electron_TL_Phi, Weight);
                        wMaps_master.hFillHitMaps("TL", "Neutron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, Weight);
                    }
                }  // end of if id. TL leading neutron
            }
        }

        //  Fill All particles (All e) plots ------------------------------------------------------------------------------------------------------------------------------------

        CodeDebugger.PrintStepTester(DebuggerMode);

        // All particles plots
        /* Declaration of electron variables for all particles analysis.
         * Variables with "tmp" will be used only in all particles plots.
         * Variables without "tmp" will be used later as well.
         * If electrons.size() == 1 (always the case when applyng clas12ana cuts), then e_out is the outgoing electron 4-momentum. */
        TLorentzVector e_out, Q;
        double Theta_e_tmp, Phi_e_tmp, P_e_tmp, Q2;

        // Electron plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < Ne; i++) {
            TVector3 P_e_All_e_3v, q_All_e_3v;
            P_e_All_e_3v.SetMagThetaPhi(electrons[i]->getP(), electrons[i]->getTheta(), electrons[i]->getPhi());
            q_All_e_3v = TVector3(Pvx - P_e_All_e_3v.Px(), Pvy - P_e_All_e_3v.Py(), Pvz - P_e_All_e_3v.Pz());  // 3 momentum transfer

            Theta_e_tmp = electrons[i]->getTheta() * 180.0 / pi;  // Theta_e_tmp in deg
            Phi_e_tmp = electrons[i]->getPhi() * 180.0 / pi;      // Phi_e_tmp in deg
            P_e_tmp = electrons[i]->par()->getP();                // temp electron momentum
            e_out.SetPxPyPzE(electrons[i]->par()->getPx(), electrons[i]->par()->getPy(), electrons[i]->par()->getPz(), sqrt(m_e * m_e + P_e_tmp * P_e_tmp));
            Q = beam - e_out;  // definition of 4-momentum transfer
            Q2 = fabs(Q.Mag2());

            if (electrons[i]->getRegion() == FD) {
                hChi2_Electron_FD.hFill(electrons[i]->par()->getChi2Pid(), Weight);

                hBeta_vs_P_Electrons_Only_FD.hFill(electrons[i]->getP(), electrons[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_All_e_FD.hFill(electrons[i]->getP(), electrons[i]->par()->getBeta(), Weight);

                hTheta_e_All_e_FD->Fill(Theta_e_tmp);
                hPhi_e_All_e_FD->Fill(Phi_e_tmp);
                hTheta_e_VS_Phi_e_All_e_FD->Fill(Phi_e_tmp, Theta_e_tmp);

                FillByInt(hQ2_All_e, hQ2_QEL_All_e, hQ2_MEC_All_e, hQ2_RES_All_e, hQ2_DIS_All_e, qel, mec, res, dis, Q2, Weight);
                FillByInt(hq_3v_All_e, hq_3v_QEL_All_e, hq_3v_MEC_All_e, hq_3v_RES_All_e, hq_3v_DIS_All_e, qel, mec, res, dis, q_All_e_3v.Mag(), Weight);
            }
        }  // end of loop over electrons vector

        // Proton plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < Np; i++) {
            if (protons[i]->getRegion() == CD) {
                hChi2_Proton_CD.hFill(protons[i]->par()->getChi2Pid(), Weight);

                hBeta_vs_P_Protons_Only_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_All_e_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
            } else if (protons[i]->getRegion() == FD) {
                hChi2_Proton_FD.hFill(protons[i]->par()->getChi2Pid(), Weight);

                hBeta_vs_P_Protons_Only_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_All_e_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
            }
        }  // end of loop over protons vector

        // Other particles plots

        // All particles Beta vs. P plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < allParticles_det.size(); i++) {
            if (allParticles_det[i]->getRegion() == FD) {
                hBeta_vs_P_FD.hFill(allParticles_det[i]->getP(), allParticles_det[i]->par()->getBeta(), Weight);
            } else if (allParticles_det[i]->getRegion() == CD) {
                hBeta_vs_P_CD.hFill(allParticles_det[i]->getP(), allParticles_det[i]->par()->getBeta(), Weight);
            }
        }  // end of loop over electrons vector

        // Neutrons Beta vs. P plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < neutrons.size(); i++) {
            if (neutrons[i]->getRegion() == FD) {
                hBeta_vs_P_Neutrons_Only_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);

                if (neutrons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);
                } else if (neutrons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);
                } else if (neutrons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);
                }
            } else if (neutrons[i]->getRegion() == CD) {
                hBeta_vs_P_Neutrons_Only_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);

                if (neutrons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);
                } else if (neutrons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);
                } else if (neutrons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);
                }
            }
        }  // end of loop over neutrons vector

        // Kplus Beta vs. P plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < Kplus.size(); i++) {
            if (Kplus[i]->getRegion() == CD) {
                hBeta_vs_P_positive_part_All_e_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
            } else if (Kplus[i]->getRegion() == FD) {
                hBeta_vs_P_positive_part_All_e_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
            }
        }  // end of loop over Kplus vector

        // Kminus Beta vs. P plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < Kminus.size(); i++) {
            if (Kminus[i]->getRegion() == CD) {
                hBeta_vs_P_negative_part_All_e_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
            } else if (Kminus[i]->getRegion() == FD) {
                hBeta_vs_P_negative_part_All_e_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
            }
        }  // end of loop over Kminus vector

        // piplus Beta vs. P plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < piplus.size(); i++) {
            if (piplus[i]->getRegion() == CD) {
                hBeta_vs_P_positive_part_All_e_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
            } else if (piplus[i]->getRegion() == FD) {
                hBeta_vs_P_positive_part_All_e_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
            }
        }  // end of loop over piplus vector

        // piminus Beta vs. P plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
                hBeta_vs_P_negative_part_All_e_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
            } else if (piminus[i]->getRegion() == FD) {
                hBeta_vs_P_negative_part_All_e_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
            }
        }  // end of loop over piminus vector

        // deuterons Beta vs. P plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < deuterons.size(); i++) {
            if (deuterons[i]->getRegion() == CD) {
                if (deuterons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                }
            } else if (deuterons[i]->getRegion() == FD) {
                if (deuterons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                }
            }
        }  // end of loop over deuterons vector

        //  1e cut --------------------------------------------------------------------------------------------------------------------------------------------------------------

        CodeDebugger.PrintStepTester(DebuggerMode);

        // 1e cut
        /* Here we apply electron cut on everything that follows (1p, nFDpCD, 1e2p and 2p) */

        // TODO: replace electrons[0] with electrons at Electron_ind.at(0)

        // Applying 1e cuts
        /* Applying rough 1e cut */
        if (Ne != 1) { continue; }  // the rough 1e cut

        if (ApplyLimiter && (num_of_events > Limiter)) { break; }  // FOR nFD eff test!

        // Safety checks (1e cut)
        CodeDebugger.SafetyCheck_one_good_electron(__FILE__, __LINE__, electrons);

        // Applying electron beta cut
        bool Bad_Electron_beta;

        if (apply_Electron_beta_cut) {
            Bad_Electron_beta = (electrons[0]->par()->getBeta() > 1.2);
        } else {
            Bad_Electron_beta = false;
        }

        if (Bad_Electron_beta) { continue; }

        /* Safety check that we are looking at 1e cut. */
        CodeDebugger.SafetyCheck_1e_cut_electron(__FILE__, __LINE__, electrons, Electron_ind);

        // events counts (1e cut)
        ++num_of_events_with_exactly_1e;  // logging #(events) w/ exactly 1e (after beta cut)

        // events counts with protons (1e cut)
        if (processID == 1.) {
            ++num_of_QEL_events_1e_cut;
            qel = true;
        } else if (processID == 2.) {
            ++num_of_MEC_events_1e_cut;
            mec = true;
        } else if (processID == 3.) {
            ++num_of_RES_events_1e_cut;
            res = true;
        } else if (processID == 4.) {
            ++num_of_DIS_events_1e_cut;
            dis = true;
        }

        /* Electron 1e cut variables definitions */
        TVector3 P_e_1e_cut_3v, q_1e_cut_3v;
        TLorentzVector e_out_1e_cut, Q_1e_cut;

        P_e_1e_cut_3v.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
        q_1e_cut_3v = TVector3(Pvx - P_e_1e_cut_3v.Px(), Pvy - P_e_1e_cut_3v.Py(), Pvz - P_e_1e_cut_3v.Pz());  // 3 momentum transfer

        e_out_1e_cut.SetPxPyPzE(electrons[0]->par()->getPx(), electrons[0]->par()->getPy(), electrons[0]->par()->getPz(), sqrt(m_e * m_e + P_e_1e_cut_3v.Mag2()));
        Q_1e_cut = beam - e_out_1e_cut;  // definition of 4-momentum transfer
        double Q2_1e_cut = fabs(Q_1e_cut.Mag2());

        double P_e_1e_cut = P_e_1e_cut_3v.Mag(), E_e_1e_cut = sqrt(m_e * m_e + P_e_1e_cut * P_e_1e_cut);
        double omega_1e_cut = beamE - E_e_1e_cut, W_1e_cut = sqrt((omega_1e_cut + m_p) * (omega_1e_cut + m_p) - q_1e_cut_3v.Mag2());
        double Theta_e = P_e_1e_cut_3v.Theta() * 180.0 / pi, Phi_e = P_e_1e_cut_3v.Phi() * 180.0 / pi;  // Theta_e, Phi_e in deg
        double Vx_e = electrons[0]->par()->getVx(), Vy_e = electrons[0]->par()->getVy(), Vz_e = electrons[0]->par()->getVz();
        double Weight_1e_cut = Weight;

        // Fill momentum threshold plots (1e cut, CD & FD)
        if (!Rec_wTL_ES || TL_Event_Selection_inclusive) {
            for (auto &e : electrons) {
                bool e_Pass_FC =
                    aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Electron", e->getP(), e->getTheta() * 180.0 / pi, e->getPhi() * 180.0 / pi);

                if (!apply_fiducial_cuts || e_Pass_FC) {
                    hP_e_reco_1e_cut_FD.hFill(e->getP(), Weight), hP_e_reco_1e_cut_FD_ZOOMIN.hFill(e->getP(), Weight);
                    hP_e_vs_Theta_e_reco_1e_cut_FD.hFill(e->getP(), e->getTheta() * 180.0 / pi, Weight);
                }
            }

            // All protons BPID (CD & FD)
            for (auto &i : All_gProtons_ind) {
                if (protons[i]->getRegion() == FD) {
                    bool p_Pass_FC = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Proton", protons[i]->getP(), protons[i]->getTheta() * 180.0 / pi,
                                                              protons[i]->getPhi() * 180.0 / pi, Calc_eff_overlapping_FC);

                    if (!apply_fiducial_cuts || p_Pass_FC) {
                        hP_p_reco_1e_cut_FD.hFill(protons[i]->getP(), Weight), hP_p_reco_1e_cut_FD_ZOOMIN.hFill(protons[i]->getP(), Weight);
                        hP_p_vs_Theta_p_reco_1e_cut_FD.hFill(protons[i]->getP(), protons[i]->getTheta() * 180.0 / pi, Weight);
                    }
                } else if (protons[i]->getRegion() == CD) {
                    hP_p_reco_1e_cut_CD.hFill(protons[i]->getP(), Weight), hP_p_reco_1e_cut_CD_ZOOMIN.hFill(protons[i]->getP(), Weight);
                    hP_p_vs_Theta_p_reco_1e_cut_CD.hFill(protons[i]->getP(), protons[i]->getTheta() * 180.0 / pi, Weight);
                }
            }

            // FD neutrons

            // FD neutrons (BPID)
            if (NeutronsFD_ind_max != -1) {
                // mom. distributions for leading nFD (BPID)
                double NeutronMomentum_1e_cut = pid.GetFDNeutronP(allParticles[NeutronsFD_ind_max], apply_nucleon_cuts);
                double NeutronTheta_1e_cut = allParticles[NeutronsFD_ind_max]->getTheta() * 180.0 / pi;
                double NeutronPhi_1e_cut = allParticles[NeutronsFD_ind_max]->getPhi() * 180.0 / pi;

                bool n_Pass_FC = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Neutron", NeutronMomentum_1e_cut, NeutronTheta_1e_cut,
                                                          NeutronPhi_1e_cut, Calc_eff_overlapping_FC);
                bool NeutronPassVeto_Test = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, NeutronsFD_ind_max, Neutron_veto_cut.GetLowerCut());

                if ((!apply_fiducial_cuts || n_Pass_FC) && NeutronPassVeto_Test) {
                    hP_LnFD_reco_BPID_1e_cut_FD.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_LnFD_reco_BPID_1e_cut_FD_ZOOMIN.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_LnFD_reco_BPID_1e_cut_FD_ZOOMOUT.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_LnFD_vs_Theta_LnFD_reco_BPID_1e_cut_FD.hFill(NeutronMomentum_1e_cut, NeutronTheta_1e_cut, Weight);
                }
            }

            for (int &i : ReDef_neutrons_FD) {
                // mom. distributions for all nFD (BPID)
                double NeutronMomentum_1e_cut = pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts);
                double NeutronTheta_1e_cut = allParticles[i]->getTheta() * 180.0 / pi;
                double NeutronPhi_1e_cut = allParticles[i]->getPhi() * 180.0 / pi;

                bool n_Pass_FC = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Neutron", NeutronMomentum_1e_cut, NeutronTheta_1e_cut,
                                                          NeutronPhi_1e_cut, Calc_eff_overlapping_FC);
                bool NeutronPassVeto_Test = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, Neutron_veto_cut.GetLowerCut());

                if ((!apply_fiducial_cuts || n_Pass_FC) && NeutronPassVeto_Test) {
                    hP_nFD_reco_BPID_1e_cut_FD.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_nFD_reco_BPID_1e_cut_FD_ZOOMIN.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_nFD_reco_BPID_1e_cut_FD_ZOOMOUT.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_nFD_vs_Theta_nFD_reco_BPID_1e_cut_FD.hFill(NeutronMomentum_1e_cut, NeutronTheta_1e_cut, Weight);
                }
            }

            // FD neutrons (APID)
            if (NeutronsFD_ind_mom_max != -1) {
                // mom. distributions for leading nFD (APID)
                double NeutronMomentum_1e_cut = pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], apply_nucleon_cuts);
                double NeutronTheta_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getTheta() * 180.0 / pi;
                double NeutronPhi_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getPhi() * 180.0 / pi;

                bool n_Pass_FC = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Neutron", NeutronMomentum_1e_cut, NeutronTheta_1e_cut,
                                                          NeutronPhi_1e_cut, Calc_eff_overlapping_FC);
                bool NeutronPassVeto_Test = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, NeutronsFD_ind_mom_max, Neutron_veto_cut.GetLowerCut());

                if ((!apply_fiducial_cuts || n_Pass_FC) && NeutronPassVeto_Test) {
                    hP_LnFD_reco_APID_1e_cut_FD.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_LnFD_reco_APID_1e_cut_FD_ZOOMIN.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_LnFD_reco_APID_1e_cut_FD_ZOOMOUT.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_LnFD_vs_Theta_LnFD_reco_APID_1e_cut_FD.hFill(NeutronMomentum_1e_cut, NeutronTheta_1e_cut, Weight);
                }
            }

            for (int &i : NeutronsFD_ind) {
                // mom. distributions for all nFD (APID)
                double NeutronMomentum_1e_cut = pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts);
                double NeutronTheta_1e_cut = allParticles[i]->getTheta() * 180.0 / pi;
                double NeutronPhi_1e_cut = allParticles[i]->getPhi() * 180.0 / pi;

                bool n_Pass_FC = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Neutron", NeutronMomentum_1e_cut, NeutronTheta_1e_cut,
                                                          NeutronPhi_1e_cut, Calc_eff_overlapping_FC);
                bool NeutronPassVeto_Test = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, Neutron_veto_cut.GetLowerCut());

                if ((!apply_fiducial_cuts || n_Pass_FC) && NeutronPassVeto_Test) {
                    hP_nFD_reco_APID_1e_cut_FD.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_nFD_reco_APID_1e_cut_FD_ZOOMIN.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_nFD_reco_APID_1e_cut_FD_ZOOMOUT.hFill(NeutronMomentum_1e_cut, Weight);
                    hP_nFD_vs_Theta_nFD_reco_APID_1e_cut_FD.hFill(NeutronMomentum_1e_cut, NeutronTheta_1e_cut, Weight);
                }
            }

            for (auto &pip : piplus) {
                hP_piplus_reco_1e_cut.hFill(pip->getP(), Weight);
                hP_piplus_reco_1e_cut_ZOOMIN.hFill(pip->getP(), Weight);

                if (pip->getRegion() == FD) {
                    hP_piplus_reco_1e_cut_FD.hFill(pip->getP(), Weight);
                    hP_piplus_reco_1e_cut_FD_ZOOMIN.hFill(pip->getP(), Weight);
                    hP_piplus_vs_Theta_piplus_reco_1e_cut_FD.hFill(pip->getP(), pip->getTheta() * 180.0 / pi, Weight);
                } else if (pip->getRegion() == CD) {
                    hP_piplus_reco_1e_cut_CD.hFill(pip->getP(), Weight);
                    hP_piplus_reco_1e_cut_CD_ZOOMIN.hFill(pip->getP(), Weight);
                    hP_piplus_vs_Theta_piplus_reco_1e_cut_CD.hFill(pip->getP(), pip->getTheta() * 180.0 / pi, Weight);
                }
            }

            for (auto &pim : piminus) {
                hP_piminus_reco_1e_cut.hFill(pim->getP(), Weight);
                hP_piminus_reco_1e_cut_ZOOMIN.hFill(pim->getP(), Weight);

                if (pim->getRegion() == FD) {
                    hP_piminus_reco_1e_cut_FD.hFill(pim->getP(), Weight);
                    hP_piminus_reco_1e_cut_FD_ZOOMIN.hFill(pim->getP(), Weight);
                    hP_piminus_vs_Theta_piminus_reco_1e_cut_FD.hFill(pim->getP(), pim->getTheta() * 180.0 / pi, Weight);
                } else if (pim->getRegion() == CD) {
                    hP_piminus_reco_1e_cut_CD.hFill(pim->getP(), Weight);
                    hP_piminus_reco_1e_cut_CD_ZOOMIN.hFill(pim->getP(), Weight);
                    hP_piminus_vs_Theta_piminus_reco_1e_cut_CD.hFill(pim->getP(), pim->getTheta() * 180.0 / pi, Weight);
                }
            }

            // FD photons

            // FD photons (BPID)
            for (int &i : ReDef_photons_FD) {
                hP_ph_reco_BPID_1e_cut_FD.hFill(allParticles[i]->getP(), Weight);
                hP_ph_reco_BPID_1e_cut_FD_ZOOMIN.hFill(allParticles[i]->getP(), Weight);
                hP_ph_vs_Theta_ph_reco_BPID_1e_cut_FD.hFill(allParticles[i]->getP(), allParticles[i]->getTheta() * 180.0 / pi, Weight);
            }

            // FD photons (APID)
            for (int &i : PhotonsFD_ind) {
                hP_ph_reco_APID_1e_cut_FD.hFill(allParticles[i]->getP(), Weight);
                hP_ph_reco_APID_1e_cut_FD_ZOOMIN.hFill(allParticles[i]->getP(), Weight);
                hP_ph_vs_Theta_ph_reco_APID_1e_cut_FD.hFill(allParticles[i]->getP(), allParticles[i]->getTheta() * 180.0 / pi, Weight);
            }
        }

        // FD Neutron and photon detection probability (1e cut, CD & FD)
        if (Count_FD_neurton_and_photon_hits) {
            for (auto &n : neutrons) {
                if (n->getRegion() == FD) {
                    ++num_of_events_with_nFD_CLA12;

                    bool NeutronInPCAL_1e_cut = (n->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                    bool NeutronInECIN_1e_cut = (n->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                    bool NeutronInECOUT_1e_cut = (n->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                    if (NeutronInPCAL_1e_cut) {
                        ++num_of_events_with_nFD_CLA12_PCAL;
                    } else if (NeutronInECIN_1e_cut || NeutronInECOUT_1e_cut) {
                        ++num_of_events_with_nFD_CLA12_EC;

                        if (NeutronInECIN_1e_cut) {
                            ++num_of_events_with_nFD_CLA12_ECIN;
                        } else if (NeutronInECOUT_1e_cut) {
                            ++num_of_events_with_nFD_CLA12_ECOUT;
                        }
                    }
                }
            }

            for (int i = 0; i < allParticles.size(); i++) {
                if ((allParticles[i]->par()->getPid() == 22) && (allParticles[i]->getRegion() == FD)) {
                    ++num_of_events_with_phFD_CLA12;

                    bool PhotonInPCAL_1e_cut = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                    bool PhotonInECIN_1e_cut = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                    bool PhotonInECOUT_1e_cut = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                    if (PhotonInPCAL_1e_cut) {
                        ++num_of_events_with_phFD_CLA12_PCAL;
                    } else if (PhotonInECIN_1e_cut || PhotonInECOUT_1e_cut) {
                        ++num_of_events_with_phFD_CLA12_EC;

                        if (PhotonInECIN_1e_cut) {
                            ++num_of_events_with_phFD_CLA12_ECIN;
                        } else if (PhotonInECOUT_1e_cut) {
                            ++num_of_events_with_phFD_CLA12_ECOUT;
                        }
                    }
                }
            }
        }

        // Testing electron cuts
        /* Here we plot cut histograms:
         * If cuts are turned off (apply_cuts == false) - we plot each cut parameter before and after the cut.
         * If cuts are applied (apply_cuts), we plot the parameters for the 1e cut only */

        // Testing cuts
        /* Testing SF cuts */
        double EoP_e = (electrons[0]->cal(clas12::PCAL)->getEnergy() + electrons[0]->cal(ECIN)->getEnergy() + electrons[0]->cal(ECOUT)->getEnergy()) / P_e_1e_cut;

        if (!apply_cuts) {
            /* SF plots before cuts */
            hSF_1e_cut_BC_FD.hFill(EoP_e, Weight), hSF_VS_P_e_1e_cut_BC_FD.hFill(P_e_1e_cut, EoP_e, Weight);

            /* SF plots after cuts */
            if ((EoP_e >= clasAna.getEcalSFLowerCut()) && (EoP_e <= clasAna.getEcalSFUpperCut())) {
                hSF_1e_cut_AC_FD.hFill(EoP_e, Weight), hSF_VS_P_e_1e_cut_AC_FD.hFill(P_e_1e_cut, EoP_e, Weight);
            }
        } else {
            hSF_1e_cut_BC_FD.hFill(EoP_e, Weight), hSF_VS_P_e_1e_cut_BC_FD.hFill(P_e_1e_cut, EoP_e, Weight);
        }

        /* Testing fiducial cuts */
        if (!apply_cuts) {
            /* Fiducial plots before cuts */
            hVcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, Weight);
            hWcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, Weight);
            hUcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, Weight);

            /* Fiducial plots after cuts */
            if (electrons[0]->cal(clas12::PCAL)->getLv() >= clasAna.getEcalEdgeCuts()) { hVcal_VS_EoP_1e_cut_AC_PCAL.hFill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, Weight); }

            if (electrons[0]->cal(clas12::PCAL)->getLw() >= clasAna.getEcalEdgeCuts()) { hWcal_VS_EoP_1e_cut_AC_PCAL.hFill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, Weight); }

            if (electrons[0]->cal(clas12::PCAL)->getLu() >= clasAna.getEcalEdgeCuts()) { hUcal_VS_EoP_1e_cut_AC_PCAL.hFill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, Weight); }
        } else {
            hVcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, Weight);
            hWcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, Weight);
            hUcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, Weight);
        }

        hPCAL_acceptance_1e_cut.hFill(electrons[0]->cal(clas12::PCAL)->getX(), electrons[0]->cal(clas12::PCAL)->getY(), Weight);

        /* Testing Nphe cuts */
        int Nphe = electrons[0]->che(clas12::HTCC)->getNphe();

        if (!apply_cuts) {
            /* Nphe plots before cuts */
            hNphe_1e_cut_BC_FD.hFill(Nphe, Weight);

            /* Nphe plots after cuts */
            if (Nphe >= clasAna.getNpheCuts()) { hNphe_1e_cut_AC_FD.hFill(Nphe, Weight); }
        } else {
            hNphe_1e_cut_BC_FD.hFill(Nphe, Weight);
        }

        // Filling 1e cut plots

        // Fill Electron plots (1e cut, FD only)
        hChi2_Electron_1e_cut_FD.hFill(electrons[0]->par()->getChi2Pid(), Weight);

        hVx_Electron_1e_cut_FD.hFill(electrons[0]->par()->getVx(), Weight);
        hVy_Electron_1e_cut_FD.hFill(electrons[0]->par()->getVy(), Weight);
        hVz_Electron_1e_cut_FD.hFill(electrons[0]->par()->getVz(), Weight);
        hVz_Electron_vs_P_e_1e_cut_FD.hFill(P_e_1e_cut, electrons[0]->par()->getVz(), Weight);
        hVz_Electron_vs_Theta_e_1e_cut_FD.hFill(Theta_e, electrons[0]->par()->getVz(), Weight);
        hVz_Electron_vs_Phi_e_1e_cut_FD.hFill(Phi_e, electrons[0]->par()->getVz(), Weight);

        hBeta_vs_P_1e_cut_Electrons_Only_FD.hFill(P_e_1e_cut, electrons[0]->par()->getBeta(), Weight);
        hBeta_vs_P_negative_part_1e_cut_FD.hFill(P_e_1e_cut, electrons[0]->par()->getBeta(), Weight);

        hP_e_1e_cut_FD.hFill(P_e_1e_cut, Weight);

        hTheta_e_1e_cut_FD->Fill(Theta_e, Weight);
        hPhi_e_1e_cut_FD->Fill(Phi_e, Weight);
        hTheta_e_VS_Phi_e_1e_cut_FD->Fill(Phi_e, Theta_e, Weight);

        hE_e_VS_Theta_e_All_Int_1e_cut_FD->Fill(Theta_e, E_e_1e_cut, Weight);
        hE_e_All_Int_1e_cut_FD->Fill(E_e_1e_cut, Weight);

        if (qel) {
            hE_e_QEL_1e_cut_FD->Fill(E_e_1e_cut, Weight);
            hE_e_VS_Theta_e_QEL_1e_cut_FD->Fill(Theta_e, E_e_1e_cut, Weight);
        } else if (mec) {
            hE_e_MEC_1e_cut_FD->Fill(E_e_1e_cut, Weight);
            hE_e_VS_Theta_e_MEC_1e_cut_FD->Fill(Theta_e, E_e_1e_cut, Weight);
        } else if (res) {
            hE_e_RES_1e_cut_FD->Fill(E_e_1e_cut, Weight);
            hE_e_VS_Theta_e_RES_1e_cut_FD->Fill(Theta_e, E_e_1e_cut, Weight);
        } else if (dis) {
            hE_e_DIS_1e_cut_FD->Fill(E_e_1e_cut, Weight);
            hE_e_VS_Theta_e_DIS_1e_cut_FD->Fill(Theta_e, E_e_1e_cut, Weight);
        }

        // Fill Proton plots (1e cut, CD & FD)
        for (auto &p : protons) {
            if (p->getRegion() == CD) {
                hChi2_Proton_1e_cut_CD.hFill(p->par()->getChi2Pid(), Weight);

                hVx_Proton_1e_cut_CD.hFill(p->par()->getVx(), Weight);
                hVy_Proton_1e_cut_CD.hFill(p->par()->getVy(), Weight);
                hVz_Proton_1e_cut_CD.hFill(p->par()->getVz(), Weight);
                hVz_Proton_vs_P_p_1e_cut_CD.hFill(p->getP(), p->par()->getVz(), Weight);
                hVz_Proton_vs_Theta_p_1e_cut_CD.hFill(p->getTheta() * 180.0 / pi, p->par()->getVz(), Weight);
                hVz_Proton_vs_Phi_p_1e_cut_CD.hFill(p->getPhi() * 180.0 / pi, p->par()->getVz(), Weight);

                hdVx_Proton_CD_1e_cut.hFill(p->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_Proton_CD_1e_cut.hFill(p->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_Proton_CD_1e_cut.hFill(p->par()->getVz() - electrons[0]->par()->getVz(), Weight);
                hdVz_Proton_vs_P_p_1e_cut_CD.hFill(p->getP(), p->par()->getVz() - electrons[0]->par()->getVz(), Weight);
                hdVz_Proton_vs_Theta_p_1e_cut_CD.hFill(p->getTheta() * 180.0 / pi, p->par()->getVz() - electrons[0]->par()->getVz(), Weight);
                hdVz_Proton_vs_Phi_p_1e_cut_CD.hFill(p->getPhi() * 180.0 / pi, p->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_Protons_Only_CD.hFill(p->getP(), p->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_CD.hFill(p->getP(), p->par()->getBeta(), Weight);

                hP_p_1e_cut_CD.hFill(p->getP(), Weight);
            } else if (p->getRegion() == FD) {
                hChi2_Proton_1e_cut_FD.hFill(p->par()->getChi2Pid(), Weight);

                hVx_Proton_1e_cut_FD.hFill(p->par()->getVx(), Weight);
                hVy_Proton_1e_cut_FD.hFill(p->par()->getVy(), Weight);
                hVz_Proton_1e_cut_FD.hFill(p->par()->getVz(), Weight);
                hVz_Proton_vs_P_p_1e_cut_FD.hFill(p->getP(), p->par()->getVz(), Weight);
                hVz_Proton_vs_Theta_p_1e_cut_FD.hFill(p->getTheta() * 180.0 / pi, p->par()->getVz(), Weight);
                hVz_Proton_vs_Phi_p_1e_cut_FD.hFill(p->getPhi() * 180.0 / pi, p->par()->getVz(), Weight);

                hdVx_Proton_FD_1e_cut.hFill(p->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_Proton_FD_1e_cut.hFill(p->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_Proton_FD_1e_cut.hFill(p->par()->getVz() - electrons[0]->par()->getVz(), Weight);
                hdVz_Proton_vs_P_p_1e_cut_FD.hFill(p->getP(), p->par()->getVz() - electrons[0]->par()->getVz(), Weight);
                hdVz_Proton_vs_Theta_p_1e_cut_FD.hFill(p->getTheta() * 180.0 / pi, p->par()->getVz() - electrons[0]->par()->getVz(), Weight);
                hdVz_Proton_vs_Phi_p_1e_cut_FD.hFill(p->getPhi() * 180.0 / pi, p->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_Protons_Only_FD.hFill(p->getP(), p->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_FD.hFill(p->getP(), p->par()->getBeta(), Weight);

                hP_p_1e_cut_FD.hFill(p->getP(), Weight);
            }
        }  // end of loop over protons vector

        // Fill Neutron plots (1e cut, CD & FD)

        // Leading FD Neutrons
        if (NeutronsFD_ind_mom_max != -1) {
            /* Leading FD Neutrons after mom. th. */
            hP_LnFD_APID_1e_cut_FD.hFill(pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], apply_nucleon_cuts), Weight);
            hP_LnFD_APID_1e_cut_ZOOMOUT_FD.hFill(pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], apply_nucleon_cuts), Weight);
            hP_LnFD_APIDandNS_1e_cut_FD.hFill(nRes.NCorr(apply_nucleon_SmearAndCorr, pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], apply_nucleon_cuts)), Weight);
            hP_LnFD_APIDandNS_1e_cut_ZOOMOUT_FD.hFill(nRes.NCorr(apply_nucleon_SmearAndCorr, pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], apply_nucleon_cuts)), Weight);
        }

        if (NeutronsFD_ind_max != -1) {
            /* Leading FD Neutrons before mom. th. */
            hP_LnFD_BPID_1e_cut_FD.hFill(pid.GetFDNeutronP(allParticles[NeutronsFD_ind_max], apply_nucleon_cuts), Weight);
            hP_LnFD_BPID_1e_cut_ZOOMOUT_FD.hFill(pid.GetFDNeutronP(allParticles[NeutronsFD_ind_max], apply_nucleon_cuts), Weight);
        }

        // All FD Neutrons
        /* All FD Neutrons after mom. th. */
        for (int &i : NeutronsFD_ind) {
            hP_nFD_APID_1e_cut_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight);
            hP_nFD_APID_1e_cut_ZOOMOUT_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight);
            hP_nFD_APIDandNS_1e_cut_FD.hFill(nRes.NCorr(apply_nucleon_SmearAndCorr, pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts)), Weight);
            hP_nFD_APIDandNS_1e_cut_ZOOMOUT_FD.hFill(nRes.NCorr(apply_nucleon_SmearAndCorr, pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts)), Weight);

            hBeta_vs_P_1e_cut_Neutrons_Only_FD_ByDef.hFill(nRes.NCorr(apply_nucleon_SmearAndCorr, pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts)), allParticles[i]->par()->getBeta(),
                                                           Weight);
        }

        /* All FD Neutrons before mom. th. */
        for (int &i : ReDef_neutrons_FD) {
            hP_nFD_BPID_1e_cut_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight);
            hP_nFD_BPID_1e_cut_ZOOMOUT_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight);
        }

        // Fill W (1e cut, CD & FD)
        FillByInt(hW_All_Int_1e_cut, hW_QEL_1e_cut, hW_MEC_1e_cut, hW_RES_1e_cut, hW_DIS_1e_cut, qel, mec, res, dis, W_1e_cut, Weight_1e_cut);
        hW_VS_q_3v_1e_cut->Fill(W_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);
        hW_VS_omega_1e_cut->Fill(W_1e_cut, omega_1e_cut, Weight_1e_cut);

        if (qel) {
            hW_VS_q_3v_QEL_1e_cut->Fill(W_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);
            hW_VS_omega_QEL_1e_cut->Fill(W_1e_cut, omega_1e_cut, Weight_1e_cut);
        } else if (mec) {
            hW_VS_q_3v_MEC_1e_cut->Fill(W_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);
            hW_VS_omega_MEC_1e_cut->Fill(W_1e_cut, omega_1e_cut, Weight_1e_cut);
        } else if (res) {
            hW_VS_q_3v_RES_1e_cut->Fill(W_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);
            hW_VS_omega_RES_1e_cut->Fill(W_1e_cut, omega_1e_cut, Weight_1e_cut);
        } else if (dis) {
            hW_VS_q_3v_DIS_1e_cut->Fill(W_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);
            hW_VS_omega_DIS_1e_cut->Fill(W_1e_cut, omega_1e_cut, Weight_1e_cut);
        }

        // Fill momentum transfer plots (1e cut, CD & FD)
        FillByInt(hQ2_1e_cut, hQ2_QEL_1e_cut, hQ2_MEC_1e_cut, hQ2_RES_1e_cut, hQ2_DIS_1e_cut, qel, mec, res, dis, Q2_1e_cut, Weight);
        FillByInt(hq_3v_1e_cut, hq_3v_QEL_1e_cut, hq_3v_MEC_1e_cut, hq_3v_RES_1e_cut, hq_3v_DIS_1e_cut, qel, mec, res, dis, q_1e_cut_3v.Mag(), Weight);

        hQ2_VS_W_1e_cut->Fill(W_1e_cut, Q2_1e_cut, Weight_1e_cut);
        hQ2_VS_q_3v_1e_cut->Fill(q_1e_cut_3v.Mag(), Q2_1e_cut, Weight_1e_cut);
        hQ2_VS_omega_1e_cut->Fill(omega_1e_cut, Q2_1e_cut, Weight_1e_cut);
        hq_3v_VS_omega_1e_cut->Fill(omega_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);

        if (qel) {
            hQ2_VS_W_QEL_1e_cut->Fill(W_1e_cut, Q2_1e_cut, Weight_1e_cut);
            hQ2_VS_q_3v_QEL_1e_cut->Fill(q_1e_cut_3v.Mag(), Q2_1e_cut, Weight_1e_cut);
            hQ2_VS_omega_QEL_1e_cut->Fill(omega_1e_cut, Q2_1e_cut, Weight_1e_cut);
            hq_3v_VS_omega_QEL_1e_cut->Fill(omega_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);
        } else if (mec) {
            hQ2_VS_W_MEC_1e_cut->Fill(W_1e_cut, Q2_1e_cut, Weight_1e_cut);
            hQ2_VS_q_3v_MEC_1e_cut->Fill(q_1e_cut_3v.Mag(), Q2_1e_cut, Weight_1e_cut);
            hQ2_VS_omega_MEC_1e_cut->Fill(omega_1e_cut, Q2_1e_cut, Weight_1e_cut);
            hq_3v_VS_omega_MEC_1e_cut->Fill(omega_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);
        } else if (res) {
            hQ2_VS_W_RES_1e_cut->Fill(W_1e_cut, Q2_1e_cut, Weight_1e_cut);
            hQ2_VS_q_3v_RES_1e_cut->Fill(q_1e_cut_3v.Mag(), Q2_1e_cut, Weight_1e_cut);
            hQ2_VS_omega_RES_1e_cut->Fill(omega_1e_cut, Q2_1e_cut, Weight_1e_cut);
            hq_3v_VS_omega_RES_1e_cut->Fill(omega_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);
        } else if (dis) {
            hQ2_VS_W_DIS_1e_cut->Fill(W_1e_cut, Q2_1e_cut, Weight_1e_cut);
            hQ2_VS_q_3v_DIS_1e_cut->Fill(q_1e_cut_3v.Mag(), Q2_1e_cut, Weight_1e_cut);
            hQ2_VS_omega_DIS_1e_cut->Fill(omega_1e_cut, Q2_1e_cut, Weight_1e_cut);
            hq_3v_VS_omega_DIS_1e_cut->Fill(omega_1e_cut, q_1e_cut_3v.Mag(), Weight_1e_cut);
        }

        // Fill Beta vs. P for other particles (1e cut, CD & FD)

        // All particles Beta vs. P plots (no #(electron) cut, CD & FD)
        for (int i = 0; i < allParticles.size(); i++) {
            if (allParticles[i]->getRegion() == FD) {
                hBeta_vs_P_1e_cut_FD.hFill(allParticles[i]->getP(), allParticles[i]->par()->getBeta(), Weight);
            } else if (allParticles[i]->getRegion() == CD) {
                hBeta_vs_P_1e_cut_CD.hFill(allParticles[i]->getP(), allParticles[i]->par()->getBeta(), Weight);
            }
        }  // end of loop over electrons vector

        // Kplus Beta vs. P plots (1e cut, CD & FD)
        for (int i = 0; i < Kplus.size(); i++) {
            if (Kplus[i]->getRegion() == CD) {
                hChi2_Kplus_1e_cut_CD.hFill(Kplus[i]->par()->getChi2Pid(), Weight);

                hVx_Kplus_1e_cut_CD.hFill(Kplus[i]->par()->getVx(), Weight);
                hVy_Kplus_1e_cut_CD.hFill(Kplus[i]->par()->getVy(), Weight);
                hVz_Kplus_1e_cut_CD.hFill(Kplus[i]->par()->getVz(), Weight);

                hdVx_Kplus_CD_1e_cut.hFill(Kplus[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_Kplus_CD_1e_cut.hFill(Kplus[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_Kplus_CD_1e_cut.hFill(Kplus[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_Kplus_Only_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);

                hP_Kplus_1e_cut_CD.hFill(Kplus[i]->getP(), Weight);
            } else if (Kplus[i]->getRegion() == FD) {
                hChi2_Kplus_1e_cut_FD.hFill(Kplus[i]->par()->getChi2Pid(), Weight);

                hVx_Kplus_1e_cut_FD.hFill(Kplus[i]->par()->getVx(), Weight);
                hVy_Kplus_1e_cut_FD.hFill(Kplus[i]->par()->getVy(), Weight);
                hVz_Kplus_1e_cut_FD.hFill(Kplus[i]->par()->getVz(), Weight);

                hdVx_Kplus_FD_1e_cut.hFill(Kplus[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_Kplus_FD_1e_cut.hFill(Kplus[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_Kplus_FD_1e_cut.hFill(Kplus[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_Kplus_Only_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);

                hP_Kplus_1e_cut_FD.hFill(Kplus[i]->getP(), Weight);
            }
        }  // end of loop over Kplus vector

        // Neutrons from clas12 Beta vs. P plots (1e cut, CD & FD)
        for (int i = 0; i < neutrons.size(); i++) {
            if (neutrons[i]->getRegion() == CD) {
                hBeta_vs_P_1e_cut_Neutrons_Only_CD_CLAS12.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);
            } else if (neutrons[i]->getRegion() == FD) {
                hBeta_vs_P_1e_cut_Neutrons_Only_FD_CLAS12.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(), Weight);
            }
        }  // end of loop over neutrons vector

        // Kminus Beta vs. P plots (1e cut, CD & FD)
        for (int i = 0; i < Kminus.size(); i++) {
            if (Kminus[i]->getRegion() == CD) {
                hChi2_Kminus_1e_cut_CD.hFill(Kminus[i]->par()->getChi2Pid(), Weight);

                hVx_Kminus_1e_cut_CD.hFill(Kminus[i]->par()->getVx(), Weight);
                hVy_Kminus_1e_cut_CD.hFill(Kminus[i]->par()->getVy(), Weight);
                hVz_Kminus_1e_cut_CD.hFill(Kminus[i]->par()->getVz(), Weight);

                hdVx_Kminus_CD_1e_cut.hFill(Kminus[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_Kminus_CD_1e_cut.hFill(Kminus[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_Kminus_CD_1e_cut.hFill(Kminus[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_Kminus_Only_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_1e_cut_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);

                hP_Kminus_1e_cut_CD.hFill(Kminus[i]->getP(), Weight);
            } else if (Kminus[i]->getRegion() == FD) {
                hChi2_Kminus_1e_cut_FD.hFill(Kminus[i]->par()->getChi2Pid(), Weight);

                hVx_Kminus_1e_cut_FD.hFill(Kminus[i]->par()->getVx(), Weight);
                hVy_Kminus_1e_cut_FD.hFill(Kminus[i]->par()->getVy(), Weight);
                hVz_Kminus_1e_cut_FD.hFill(Kminus[i]->par()->getVz(), Weight);

                hdVx_Kminus_FD_1e_cut.hFill(Kminus[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_Kminus_FD_1e_cut.hFill(Kminus[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_Kminus_FD_1e_cut.hFill(Kminus[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_Kminus_Only_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_1e_cut_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);

                hP_Kminus_1e_cut_FD.hFill(Kminus[i]->getP(), Weight);
            }
        }  // end of loop over Kminus vector

        // piplus Beta vs. P plots (1e cut, CD & FD)
        for (int i = 0; i < piplus.size(); i++) {
            if (piplus[i]->getRegion() == CD) {
                hChi2_piplus_1e_cut_CD.hFill(piplus[i]->par()->getChi2Pid(), Weight);

                hVx_piplus_1e_cut_CD.hFill(piplus[i]->par()->getVx(), Weight);
                hVy_piplus_1e_cut_CD.hFill(piplus[i]->par()->getVy(), Weight);
                hVz_piplus_1e_cut_CD.hFill(piplus[i]->par()->getVz(), Weight);

                hdVx_piplus_CD_1e_cut.hFill(piplus[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_piplus_CD_1e_cut.hFill(piplus[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_piplus_CD_1e_cut.hFill(piplus[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_piplus_Only_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);

                hP_piplus_1e_cut_CD.hFill(piplus[i]->getP(), Weight);
            } else if (piplus[i]->getRegion() == FD) {
                hChi2_piplus_1e_cut_FD.hFill(piplus[i]->par()->getChi2Pid(), Weight);

                hVx_piplus_1e_cut_FD.hFill(piplus[i]->par()->getVx(), Weight);
                hVy_piplus_1e_cut_FD.hFill(piplus[i]->par()->getVy(), Weight);
                hVz_piplus_1e_cut_FD.hFill(piplus[i]->par()->getVz(), Weight);

                hdVx_piplus_FD_1e_cut.hFill(piplus[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_piplus_FD_1e_cut.hFill(piplus[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_piplus_FD_1e_cut.hFill(piplus[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_piplus_Only_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);

                hP_piplus_1e_cut_FD.hFill(piplus[i]->getP(), Weight);
            }
        }  // end of loop over piplus vector

        // piminus Beta vs. P plots (1e cut, CD & FD)
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
                hChi2_piminus_1e_cut_CD.hFill(piminus[i]->par()->getChi2Pid(), Weight);

                hVx_piminus_1e_cut_CD.hFill(piminus[i]->par()->getVx(), Weight);
                hVy_piminus_1e_cut_CD.hFill(piminus[i]->par()->getVy(), Weight);
                hVz_piminus_1e_cut_CD.hFill(piminus[i]->par()->getVz(), Weight);

                hdVx_piminus_CD_1e_cut.hFill(piminus[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_piminus_CD_1e_cut.hFill(piminus[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_piminus_CD_1e_cut.hFill(piminus[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_piminus_Only_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_1e_cut_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);

                hP_piminus_1e_cut_CD.hFill(piminus[i]->getP(), Weight);
            } else if (piminus[i]->getRegion() == FD) {
                hChi2_piminus_1e_cut_FD.hFill(piminus[i]->par()->getChi2Pid(), Weight);

                hVx_piminus_1e_cut_FD.hFill(piminus[i]->par()->getVx(), Weight);
                hVy_piminus_1e_cut_FD.hFill(piminus[i]->par()->getVy(), Weight);
                hVz_piminus_1e_cut_FD.hFill(piminus[i]->par()->getVz(), Weight);

                hdVx_piminus_FD_1e_cut.hFill(piminus[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_piminus_FD_1e_cut.hFill(piminus[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_piminus_FD_1e_cut.hFill(piminus[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_piminus_Only_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_1e_cut_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);

                hP_piminus_1e_cut_FD.hFill(piminus[i]->getP(), Weight);
            }
        }  // end of loop over piminus vector

        // neutrals Beta vs. P plots (1e cut, CD & FD)
        for (int i = 0; i < neutrals.size(); i++) {
            if (neutrals[i]->getRegion() == CD) {
                if (neutrals[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                }
            } else if (neutrals[i]->getRegion() == FD) {
                if (neutrals[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                }
            }
        }  // end of loop over Kplus vector

        // deuterons Beta vs. P plots (1e cut, CD & FD)
        for (int i = 0; i < deuterons.size(); i++) {
            if (deuterons[i]->getRegion() == CD) {
                hChi2_deuteron_1e_cut_CD.hFill(deuterons[i]->par()->getChi2Pid(), Weight);

                hVx_Deuteron_1e_cut_CD.hFill(deuterons[i]->par()->getVx(), Weight);
                hVy_Deuteron_1e_cut_CD.hFill(deuterons[i]->par()->getVy(), Weight);
                hVz_Deuteron_1e_cut_CD.hFill(deuterons[i]->par()->getVz(), Weight);

                hdVx_Deuteron_CD_1e_cut.hFill(deuterons[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_Deuteron_CD_1e_cut.hFill(deuterons[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_Deuteron_CD_1e_cut.hFill(deuterons[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                if (deuterons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                }

                hP_deuteron_1e_cut_CD.hFill(deuterons[i]->getP(), Weight);
            } else if (deuterons[i]->getRegion() == FD) {
                hChi2_deuteron_1e_cut_FD.hFill(deuterons[i]->par()->getChi2Pid(), Weight);

                hVx_Deuteron_1e_cut_FD.hFill(deuterons[i]->par()->getVx(), Weight);
                hVy_Deuteron_1e_cut_FD.hFill(deuterons[i]->par()->getVy(), Weight);
                hVz_Deuteron_1e_cut_FD.hFill(deuterons[i]->par()->getVz(), Weight);

                hdVx_Deuteron_FD_1e_cut.hFill(deuterons[i]->par()->getVx() - electrons[0]->par()->getVx(), Weight);
                hdVy_Deuteron_FD_1e_cut.hFill(deuterons[i]->par()->getVy() - electrons[0]->par()->getVy(), Weight);
                hdVz_Deuteron_FD_1e_cut.hFill(deuterons[i]->par()->getVz() - electrons[0]->par()->getVz(), Weight);

                if (deuterons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                }

                hP_deuteron_1e_cut_FD.hFill(deuterons[i]->getP(), Weight);
            }
        }  // end of loop over Kplus vector

        // otherpart Beta vs. P plots (1e cut, CD & FD)
        for (int i = 0; i < otherpart.size(); i++) {
            if (otherpart[i]->getRegion() == CD) {
                if (otherpart[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                }
            } else if (otherpart[i]->getRegion() == FD) {
                if (otherpart[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                }
            }
        }  // end of loop over Kplus vector

        // Fill ToF plots for electron (1e cut, FD)
        // TODO: rename and move from ToF to angles folder

        bool e_hit_PCAL_1e_cut = (electrons[0]->cal(clas12::PCAL)->getDetector() == 7);  // check if electron hit the PCAL

        // CLAS12 neutrons
        // TODO: ask Justin if electrons and/or protons can be detected in the FD WITHOUT hitting the PCAL
        if (e_hit_PCAL_1e_cut) {
            // if there's an electron hit in the PCAL
            // electron PCAL hit vector and angles:
            TVector3 e_hit_1e_cut_3v(electrons[0]->cal(clas12::PCAL)->getX(), electrons[0]->cal(clas12::PCAL)->getY(), electrons[0]->cal(clas12::PCAL)->getZ());
            double e_hit_Theta_1e_cut = e_hit_1e_cut_3v.Theta() * 180 / pi, e_hit_Phi_1e_cut = e_hit_1e_cut_3v.Phi() * 180 / pi;

            for (auto &n : neutrons) {
                // loop on every detected neutron
                // check neutron hits in the PCAL, ECIN and ECOUT:
                bool n_hit_PCAL_1e_cut = (n->cal(clas12::PCAL)->getDetector() == 7);
                bool n_hit_ECIN_1e_cut = (n->cal(clas12::ECIN)->getDetector() == 7);
                bool n_hit_ECOUT_1e_cut = (n->cal(clas12::ECOUT)->getDetector() == 7);

                if (!n_hit_PCAL_1e_cut && (n_hit_ECIN_1e_cut || n_hit_ECOUT_1e_cut)) {
                    // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT
                    auto n_detlayer_1e_cut = n_hit_ECIN_1e_cut ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

                    // neutron ECIN/ECAL hit vector and angles:
                    TVector3 n_hit_1e_cut_3v(n->cal(n_detlayer_1e_cut)->getX(), n->cal(n_detlayer_1e_cut)->getY(), n->cal(n_detlayer_1e_cut)->getZ());
                    double n_hit_Theta_1e_cut = n_hit_1e_cut_3v.Theta() * 180 / pi, n_hit_Phi_1e_cut = n_hit_1e_cut_3v.Phi() * 180 / pi;

                    // subtracting the angles between the neutron hit and electron hit to see if we have fake neutrons:
                    hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1e_cut.hFill(CalcdPhi(n_hit_Phi_1e_cut - e_hit_Phi_1e_cut), n_hit_Theta_1e_cut - e_hit_Theta_1e_cut, Weight);
                }
            }  // end of ToF loop over neutrons vector
        }

        // Redefined neutrons (leading FD neutron)
        if (NeutronsFD_ind_mom_max_noNeutCuts != -1) {
            // if there's an electron hit in the PCAL
            // check neutron hits in the PCAL, ECIN and ECOUT:
            bool LnFD_hit_PCAL_1e_cut = (allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(clas12::PCAL)->getDetector() == 7);
            bool LnFD_hit_ECIN_1e_cut = (allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(clas12::ECIN)->getDetector() == 7);
            bool LnFD_hit_ECOUT_1e_cut = (allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(clas12::ECOUT)->getDetector() == 7);

            if (!LnFD_hit_PCAL_1e_cut && (LnFD_hit_ECIN_1e_cut || LnFD_hit_ECOUT_1e_cut)) {
                auto LnFD_detlayer_1e_cut = LnFD_hit_ECIN_1e_cut ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

                // neutron ECIN/ECAL hit vector and angles:
                TVector3 LnFD_hit_1e_cut_3v(allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(LnFD_detlayer_1e_cut)->getX(),
                                            allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(LnFD_detlayer_1e_cut)->getY(),
                                            allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(LnFD_detlayer_1e_cut)->getZ());
                TVector3 e_hit_1e_cut_3v, dist_1e_cut_3v;

                if ((LnFD_detlayer_1e_cut == clas12::ECIN) && (electrons[0]->cal(clas12::ECIN)->getZ() != 0)) {
                    /* if both particles hit the inner calorimeter, use the inner calorimeter to determine e_hit_1e_cut_3v */
                    e_hit_1e_cut_3v.SetXYZ(electrons[0]->cal(clas12::ECIN)->getX(), electrons[0]->cal(clas12::ECIN)->getY(), electrons[0]->cal(clas12::ECIN)->getZ());
                } else if ((LnFD_detlayer_1e_cut == clas12::ECOUT) && (electrons[0]->cal(clas12::ECOUT)->getZ() != 0)) {
                    /* if both particles hit the outer calorimeter, use the outer calorimeter to determine e_hit_1e_cut_3v */
                    e_hit_1e_cut_3v.SetXYZ(electrons[0]->cal(clas12::ECOUT)->getX(), electrons[0]->cal(clas12::ECOUT)->getY(), electrons[0]->cal(clas12::ECOUT)->getZ());
                } else {
                    /* the neutral has to hit either the ECIN or ECOUT.
                       If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
                    int trajlayer = (LnFD_detlayer_1e_cut == clas12::ECIN) ? 4 : 7;
                    e_hit_1e_cut_3v.SetXYZ(electrons[0]->traj(clas12::ECAL, trajlayer)->getX(), electrons[0]->traj(clas12::ECAL, trajlayer)->getY(),
                                           electrons[0]->traj(clas12::ECAL, trajlayer)->getZ());
                }

                double e_hit_Theta_1e_cut = e_hit_1e_cut_3v.Theta() * 180 / pi, e_hit_Phi_1e_cut = e_hit_1e_cut_3v.Phi() * 180 / pi;
                double LnFD_hit_Theta_1e_cut = LnFD_hit_1e_cut_3v.Theta() * 180 / pi, LnFD_hit_Phi_1e_cut = LnFD_hit_1e_cut_3v.Phi() * 180 / pi;
                //                dist_1e_cut_3v = LnFD_hit_1e_cut_3v - e_hit_1e_cut_3v;

                // subtracting the angles between the neutron hit and electron hit to see if we have fake neutrons:
                hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_BV_1e_cut.hFill(CalcdPhi(LnFD_hit_Phi_1e_cut - e_hit_Phi_1e_cut), LnFD_hit_Theta_1e_cut - e_hit_Theta_1e_cut, Weight);

                bool NeutronPassVeto_1e_cut = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, NeutronsFD_ind_mom_max_noNeutCuts, Neutron_veto_cut.GetLowerCut());

                if (NeutronPassVeto_1e_cut) {
                    hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_AV_1e_cut.hFill(CalcdPhi(LnFD_hit_Phi_1e_cut - e_hit_Phi_1e_cut), LnFD_hit_Theta_1e_cut - e_hit_Theta_1e_cut, Weight);
                } else {
                    hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_Vetoed_1e_cut.hFill(CalcdPhi(LnFD_hit_Phi_1e_cut - e_hit_Phi_1e_cut), LnFD_hit_Theta_1e_cut - e_hit_Theta_1e_cut, Weight);
                }
            }
        }

        // Redefined neutrons (all FD neutrons)
        if (e_hit_PCAL_1e_cut) {
            // if there's an electron hit in the PCAL
            for (auto &i : NeutronsFD_ind_noNeutCuts) {
                // check neutron hits in the PCAL, ECIN and ECOUT:
                bool nFD_hit_PCAL_1e_cut = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);
                bool nFD_hit_ECIN_1e_cut = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);
                bool nFD_hit_ECOUT_1e_cut = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);

                if (!nFD_hit_PCAL_1e_cut && (nFD_hit_ECIN_1e_cut || nFD_hit_ECOUT_1e_cut)) {
                    auto nFD_detlayer_1e_cut = nFD_hit_ECIN_1e_cut ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

                    // neutron ECIN/ECAL hit vector and angles:
                    TVector3 nFD_hit_1e_cut_3v(allParticles[i]->cal(nFD_detlayer_1e_cut)->getX(), allParticles[i]->cal(nFD_detlayer_1e_cut)->getY(),
                                               allParticles[i]->cal(nFD_detlayer_1e_cut)->getZ());
                    TVector3 e_hit_1e_cut_3v, dist_1e_cut_3v;

                    if ((nFD_detlayer_1e_cut == clas12::ECIN) && (electrons[0]->cal(clas12::ECIN)->getZ() != 0)) {
                        /* if both particles hit the inner calorimeter, use the inner calorimeter to determine e_hit_1e_cut_3v */
                        e_hit_1e_cut_3v.SetXYZ(electrons[0]->cal(clas12::ECIN)->getX(), electrons[0]->cal(clas12::ECIN)->getY(), electrons[0]->cal(clas12::ECIN)->getZ());
                    } else if ((nFD_detlayer_1e_cut == clas12::ECOUT) && (electrons[0]->cal(clas12::ECOUT)->getZ() != 0)) {
                        /* if both particles hit the outer calorimeter, use the outer calorimeter to determine e_hit_1e_cut_3v */
                        e_hit_1e_cut_3v.SetXYZ(electrons[0]->cal(clas12::ECOUT)->getX(), electrons[0]->cal(clas12::ECOUT)->getY(), electrons[0]->cal(clas12::ECOUT)->getZ());
                    } else {
                        /* the neutral has to hit either the ECIN or ECOUT.
                           If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
                        int trajlayer = (nFD_detlayer_1e_cut == clas12::ECIN) ? 4 : 7;
                        e_hit_1e_cut_3v.SetXYZ(electrons[0]->traj(clas12::ECAL, trajlayer)->getX(), electrons[0]->traj(clas12::ECAL, trajlayer)->getY(),
                                               electrons[0]->traj(clas12::ECAL, trajlayer)->getZ());
                    }

                    double e_hit_Theta_1e_cut = e_hit_1e_cut_3v.Theta() * 180 / pi, e_hit_Phi_1e_cut = e_hit_1e_cut_3v.Phi() * 180 / pi;
                    double nFD_hit_Theta_1e_cut = nFD_hit_1e_cut_3v.Theta() * 180 / pi, nFD_hit_Phi_1e_cut = nFD_hit_1e_cut_3v.Phi() * 180 / pi;
                    //                    dist_1e_cut_3v = nFD_hit_1e_cut_3v - e_hit_1e_cut_3v;

                    // subtracting the angles between the neutron hit and electron hit to see if we have fake neutrons:
                    hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_BV_1e_cut.hFill(CalcdPhi(nFD_hit_Phi_1e_cut - e_hit_Phi_1e_cut), nFD_hit_Theta_1e_cut - e_hit_Theta_1e_cut, Weight);

                    bool NeutronPassVeto_1e_cut = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, Neutron_veto_cut.GetLowerCut());

                    if (NeutronPassVeto_1e_cut) {
                        hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_AV_1e_cut.hFill(CalcdPhi(nFD_hit_Phi_1e_cut - e_hit_Phi_1e_cut), nFD_hit_Theta_1e_cut - e_hit_Theta_1e_cut, Weight);
                    } else {
                        hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_Vetoed_1e_cut.hFill(CalcdPhi(nFD_hit_Phi_1e_cut - e_hit_Phi_1e_cut), nFD_hit_Theta_1e_cut - e_hit_Theta_1e_cut, Weight);
                    }
                }
            }
        }

        // Fill ToF plots for protons (1e cut, FD)

        // CLAS12 neutrons
        for (auto &n : neutrons) {
            // loop on every detected neutron
            // check neutron (n) hits in the PCAL, ECIN and ECOUT:
            bool n_hit_PCAL_1e_cut = (n->cal(clas12::PCAL)->getDetector() == 7);
            bool n_hit_ECIN_1e_cut = (n->cal(clas12::ECIN)->getDetector() == 7);
            bool n_hit_ECOUT_1e_cut = (n->cal(clas12::ECOUT)->getDetector() == 7);

            // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT:
            if (!n_hit_PCAL_1e_cut && (n_hit_ECIN_1e_cut || n_hit_ECOUT_1e_cut)) {
                auto n_detlayer_1e_cut = n_hit_ECIN_1e_cut ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

                // neutron (n)'s ECIN/ECAL hit vector and angles:
                TVector3 n_hit_1e_cut_3v(n->cal(n_detlayer_1e_cut)->getX(), n->cal(n_detlayer_1e_cut)->getY(), n->cal(n_detlayer_1e_cut)->getZ());
                double n_hit_Theta_1e_cut = n_hit_1e_cut_3v.Theta() * 180 / pi, n_hit_Phi_1e_cut = n_hit_1e_cut_3v.Phi() * 180 / pi;

                for (auto &p : protons) {
                    // loop over protons vector
                    bool p_hit_PCAL_1e_cut = (p->cal(clas12::PCAL)->getDetector() == 7);  // check if proton hit the PCAL
                    // TODO: rename and move from ToF to angles folder

                    // TODO: ask Justin if electrons and/or protons can be detected in the FD WITHOUT hitting the PCAL
                    if (p_hit_PCAL_1e_cut) {
                        // if there's a proton hit in the PCAL
                        // proton PCAL hit vector and angles:
                        TVector3 p_hit_1e_cut_3v(p->cal(clas12::PCAL)->getX(), p->cal(clas12::PCAL)->getY(), p->cal(clas12::PCAL)->getZ());
                        double p_hit_Theta_1e_cut = p_hit_1e_cut_3v.Theta() * 180 / pi, p_hit_Phi_1e_cut = p_hit_1e_cut_3v.Phi() * 180 / pi;

                        // subtracting the angles between the neutron (n) hit and proton hit to see if we have fake neutrons:
                        hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1e_cut.hFill(CalcdPhi(n_hit_Phi_1e_cut - p_hit_Phi_1e_cut), n_hit_Theta_1e_cut - p_hit_Theta_1e_cut, Weight);
                    }
                }  // end of ToF loop over protons vector
            }
        }  // end of ToF loop over neutrons vector

        // Redefined neutrons (leading FD neutron)
        if (NeutronsFD_ind_mom_max_noNeutCuts != -1) {
            // check neutron (n) hits in the PCAL, ECIN and ECOUT:
            bool LnFD_hit_PCAL_1e_cut = (allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(clas12::PCAL)->getDetector() == 7);
            bool LnFD_hit_ECIN_1e_cut = (allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(clas12::ECIN)->getDetector() == 7);
            bool LnFD_hit_ECOUT_1e_cut = (allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(clas12::ECOUT)->getDetector() == 7);

            // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT:
            if (!LnFD_hit_PCAL_1e_cut && (LnFD_hit_ECIN_1e_cut || LnFD_hit_ECOUT_1e_cut)) {
                auto LnFD_detlayer_1e_cut = LnFD_hit_ECIN_1e_cut ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

                // neutron (n)'s ECIN/ECAL hit vector and angles:
                TVector3 LnFD_hit_1e_cut_3v(allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(LnFD_detlayer_1e_cut)->getX(),
                                            allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(LnFD_detlayer_1e_cut)->getY(),
                                            allParticles[NeutronsFD_ind_mom_max_noNeutCuts]->cal(LnFD_detlayer_1e_cut)->getZ());

                for (auto &i : Protons_ind) {
                    // loop over protons vector
                    if (protons[i]->getRegion() == FD) {
                        TVector3 p_hit_1e_cut_3v, dist_1e_cut_3v;

                        if ((LnFD_detlayer_1e_cut == clas12::ECIN) && (protons[i]->cal(clas12::ECIN)->getZ() != 0)) {
                            /* if both particles hit the inner calorimeter, use the inner calorimeter to determine p_hit_1e_cut_3v */
                            p_hit_1e_cut_3v.SetXYZ(protons[i]->cal(clas12::ECIN)->getX(), protons[i]->cal(clas12::ECIN)->getY(), protons[i]->cal(clas12::ECIN)->getZ());
                        } else if ((LnFD_detlayer_1e_cut == clas12::ECOUT) && (protons[i]->cal(clas12::ECOUT)->getZ() != 0)) {
                            /* if both particles hit the outer calorimeter, use the outer calorimeter to determine p_hit_1e_cut_3v */
                            p_hit_1e_cut_3v.SetXYZ(protons[i]->cal(clas12::ECOUT)->getX(), protons[i]->cal(clas12::ECOUT)->getY(), protons[i]->cal(clas12::ECOUT)->getZ());
                        } else {
                            /* the neutral has to hit either the ECIN or ECOUT.
                               If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
                            int trajlayer = (LnFD_detlayer_1e_cut == clas12::ECIN) ? 4 : 7;
                            p_hit_1e_cut_3v.SetXYZ(protons[i]->traj(clas12::ECAL, trajlayer)->getX(), protons[i]->traj(clas12::ECAL, trajlayer)->getY(),
                                                   protons[i]->traj(clas12::ECAL, trajlayer)->getZ());
                        }

                        double LnFD_hit_Theta_1e_cut = LnFD_hit_1e_cut_3v.Theta() * 180 / pi, LnFD_hit_Phi_1e_cut = LnFD_hit_1e_cut_3v.Phi() * 180 / pi;
                        double p_hit_Theta_1e_cut = p_hit_1e_cut_3v.Theta() * 180 / pi, p_hit_Phi_1e_cut = p_hit_1e_cut_3v.Phi() * 180 / pi;

                        // subtracting the angles between the neutron (n) hit and proton hit to see if we have fake neutrons:
                        hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_BV_1e_cut.hFill(CalcdPhi(LnFD_hit_Phi_1e_cut - p_hit_Phi_1e_cut), LnFD_hit_Theta_1e_cut - p_hit_Theta_1e_cut, Weight);

                        bool NeutronPassVeto_1e_cut = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, NeutronsFD_ind_mom_max_noNeutCuts, Neutron_veto_cut.GetLowerCut());

                        if (NeutronPassVeto_1e_cut) {
                            hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_AV_1e_cut.hFill(CalcdPhi(LnFD_hit_Phi_1e_cut - p_hit_Phi_1e_cut), LnFD_hit_Theta_1e_cut - p_hit_Theta_1e_cut, Weight);
                        } else {
                            hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_Vetoed_1e_cut.hFill(CalcdPhi(LnFD_hit_Phi_1e_cut - p_hit_Phi_1e_cut), LnFD_hit_Theta_1e_cut - p_hit_Theta_1e_cut, Weight);
                        }
                    }
                }  // end of ToF loop over protons vector
            }
        }

        // Redefined neutrons (all FD neutrons)
        for (auto &i : NeutronsFD_ind_noNeutCuts) {
            // check neutron (n) hits in the PCAL, ECIN and ECOUT:
            bool nFD_hit_PCAL_1e_cut = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);
            bool nFD_hit_ECIN_1e_cut = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);
            bool nFD_hit_ECOUT_1e_cut = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);

            // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT:
            if (!nFD_hit_PCAL_1e_cut && (nFD_hit_ECIN_1e_cut || nFD_hit_ECOUT_1e_cut)) {
                auto nFD_detlayer_1e_cut = nFD_hit_ECIN_1e_cut ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

                // neutron (n)'s ECIN/ECAL hit vector and angles:
                TVector3 nFD_hit_1e_cut_3v(allParticles[i]->cal(nFD_detlayer_1e_cut)->getX(), allParticles[i]->cal(nFD_detlayer_1e_cut)->getY(),
                                           allParticles[i]->cal(nFD_detlayer_1e_cut)->getZ());

                for (auto &j : Protons_ind) {
                    // loop over protons vector
                    if (protons[j]->getRegion() == FD) {
                        TVector3 p_hit_1e_cut_3v, dist_1e_cut_3v;

                        if ((nFD_detlayer_1e_cut == clas12::ECIN) && (protons[j]->cal(clas12::ECIN)->getZ() != 0)) {
                            /* if both particles hit the inner calorimeter, use the inner calorimeter to determine p_hit_1e_cut_3v */
                            p_hit_1e_cut_3v.SetXYZ(protons[j]->cal(clas12::ECIN)->getX(), protons[j]->cal(clas12::ECIN)->getY(), protons[j]->cal(clas12::ECIN)->getZ());
                        } else if ((nFD_detlayer_1e_cut == clas12::ECOUT) && (protons[j]->cal(clas12::ECOUT)->getZ() != 0)) {
                            /* if both particles hit the outer calorimeter, use the outer calorimeter to determine p_hit_1e_cut_3v */
                            p_hit_1e_cut_3v.SetXYZ(protons[j]->cal(clas12::ECOUT)->getX(), protons[j]->cal(clas12::ECOUT)->getY(), protons[j]->cal(clas12::ECOUT)->getZ());
                        } else {
                            /* the neutral has to hit either the ECIN or ECOUT.
                               If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
                            int trajlayer = (nFD_detlayer_1e_cut == clas12::ECIN) ? 4 : 7;
                            p_hit_1e_cut_3v.SetXYZ(protons[j]->traj(clas12::ECAL, trajlayer)->getX(), protons[j]->traj(clas12::ECAL, trajlayer)->getY(),
                                                   protons[j]->traj(clas12::ECAL, trajlayer)->getZ());
                        }

                        double nFD_hit_Theta_1e_cut = nFD_hit_1e_cut_3v.Theta() * 180 / pi, nFD_hit_Phi_1e_cut = nFD_hit_1e_cut_3v.Phi() * 180 / pi;
                        double p_hit_Theta_1e_cut = p_hit_1e_cut_3v.Theta() * 180 / pi, p_hit_Phi_1e_cut = p_hit_1e_cut_3v.Phi() * 180 / pi;

                        hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_BV_1e_cut.hFill(CalcdPhi(nFD_hit_Phi_1e_cut - p_hit_Phi_1e_cut), nFD_hit_Theta_1e_cut - p_hit_Theta_1e_cut, Weight);

                        bool NeutronPassVeto_1e_cut = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, Neutron_veto_cut.GetLowerCut());

                        if (NeutronPassVeto_1e_cut) {
                            hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_AV_1e_cut.hFill(CalcdPhi(nFD_hit_Phi_1e_cut - p_hit_Phi_1e_cut), nFD_hit_Theta_1e_cut - p_hit_Theta_1e_cut, Weight);
                        } else {
                            hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_Vetoed_1e_cut.hFill(CalcdPhi(nFD_hit_Phi_1e_cut - p_hit_Phi_1e_cut), nFD_hit_Theta_1e_cut - p_hit_Theta_1e_cut, Weight);
                        }
                    }
                }
            }  // end of ToF loop over protons vector
        }

        // Filling reco. Acceptance maps
        if (Generate_Electron_AMaps) {
            // Filling electron reco. Acceptance maps
            if (electrons[0]->getRegion() == FD) {
                /* Fill reco electron acceptance maps */
                hElectronAMapBC.hFill(Phi_e, Theta_e, Weight);
                hReco_P_e_AMaps.hFill(P_e_1e_cut, Weight);
                hReco_P_e_vs_Reco_Theta_e_AMap.hFill(P_e_1e_cut, Theta_e, Weight);
                hReco_P_e_vs_Reco_Phi_e_AMap.hFill(P_e_1e_cut, Phi_e, Weight);
                aMaps_master.hFillHitMaps("Reco", "Electron", P_e_1e_cut, Theta_e, Phi_e, Weight);
            }
        }

        // Filling nucleon reco. Acceptance maps
        if (Generate_Nucleon_AMaps) {
            // Filling proton reco. Acceptance maps
            for (int &i : Protons_ind) {
                /* Fill all reco FD proton acceptance maps */
                if (protons[i]->getRegion() == FD) {
                    double Reco_Proton_Momentum = protons[i]->getP();

                    if ((Reco_Proton_Momentum <= p_mom_th.GetUpperCut()) && (Reco_Proton_Momentum >= p_mom_th.GetLowerCut())) {
                        // if id. reco proton
                        hProtonAMapBC.hFill(protons[i]->getPhi() * 180.0 / pi, protons[i]->getTheta() * 180.0 / pi, Weight);
                        hReco_P_pFD_AMaps.hFill(Reco_Proton_Momentum, Weight);
                        hReco_P_pFD_vs_Reco_Theta_pFD_AMap.hFill(Reco_Proton_Momentum, protons[i]->getTheta() * 180.0 / pi, Weight);
                        hReco_P_pFD_vs_Reco_Phi_pFD_AMap.hFill(Reco_Proton_Momentum, protons[i]->getPhi() * 180.0 / pi, Weight);
                        hReco_P_pFD_vs_Reco_P_e_AMap.hFill(Reco_Proton_Momentum, P_e_1e_cut, Weight);
                        hReco_P_pFD_vs_Reco_Theta_e_AMap.hFill(Reco_Proton_Momentum, Theta_e, Weight);
                        hReco_P_pFD_vs_Reco_Phi_e_AMap.hFill(Reco_Proton_Momentum, Phi_e, Weight);
                        aMaps_master.hFillHitMaps("Reco", "Proton", Reco_Proton_Momentum, protons[i]->getTheta() * 180.0 / pi, protons[i]->getPhi() * 180.0 / pi, Weight);
                    }  // end of if id. reco proton
                }
            }

            // Filling neurton reco. Acceptance maps
            if (
                // if ((NeutronsFD_ind.size() == 1) &&  // FOR nFD eff test!
                ES_by_leading_FDneutron) {
                if (NeutronsFD_ind_mom_max != -1) {
                    // if NeutronsFD_ind_mom_max == -1, there are no neutrons above momentum th. in the event
                    /* Fill leading reco FD neutron acceptance maps */
                    bool hitPCAL_1e_cut = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                    bool hitECIN_1e_cut = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                    bool hitECOUT_1e_cut = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
                    auto n_detlayer_1e_cut = hitECIN_1e_cut ? clas12::ECIN : clas12::ECOUT;                                 // find first layer of hit

                    // Safety checks that leading nFD is neutron by definition (AMaps & WMaps)
                    CodeDebugger.SafetyCheck_AMaps_Reco_leading_neutrons(__FILE__, __LINE__, allParticles, NeutronsFD_ind_mom_max, hitPCAL_1e_cut, hitECIN_1e_cut, hitECOUT_1e_cut);

                    if (true) {
                        // if (allParticles[NeutronsFD_ind_mom_max]->cal(n_detlayer_1e_cut)->getLv() > clasAna.getEcalEdgeCuts() &&
                        //     allParticles[NeutronsFD_ind_mom_max]->cal(n_detlayer_1e_cut)->getLw() > clasAna.getEcalEdgeCuts()) {
                        // if neutron is within fiducial cuts

                        bool NeutronPassVeto_1e_cut = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, NeutronsFD_ind_mom_max, Neutron_veto_cut.GetLowerCut());

                        // double Mom_neut_1e_cut = pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], true);  // if neutron is within fiducial cuts
                        double Mom_neut_1e_cut = pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], apply_nucleon_cuts);
                        double Theta_neut_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getTheta() * 180.0 / pi;
                        double Phi_neut_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getPhi() * 180.0 / pi;

                        if ((Mom_neut_1e_cut >= n_mom_th.GetLowerCut()) && (Mom_neut_1e_cut <= beamE))  // FOR nFD eff test!
                        // if ((Mom_neut_1e_cut <= n_mom_th.GetUpperCut()) && (Mom_neut_1e_cut >= n_mom_th.GetLowerCut()))
                        {
                            // if id. reco leading neutron

                            bool GoodTLMatch_AMaps = ((fabs(TL_nFD_theta - Theta_neut_1e_cut) < 2.) && (fabs(CalcdPhi(TL_nFD_phi - Phi_neut_1e_cut)) < 5.));  // FOR nFD eff test!

                            // if neutron passes ECAL veto:
                            if (NeutronPassVeto_1e_cut)
                            // if (NeutronPassVeto_1e_cut && GoodTLMatch_AMaps)  // FOR nFD eff test!
                            {
                                hReco_P_nFD_AMaps.hFill(Mom_neut_1e_cut, Weight);
                                hNeutronAMapBC.hFill(Phi_neut_1e_cut, Theta_neut_1e_cut, Weight);
                                hReco_P_nFD_vs_Reco_Theta_nFD_AMap.hFill(Mom_neut_1e_cut, Theta_neut_1e_cut, Weight);
                                hReco_P_nFD_vs_Reco_Phi_nFD_AMap.hFill(Mom_neut_1e_cut, Phi_neut_1e_cut, Weight);
                                hReco_P_nFD_vs_Reco_P_e_AMap.hFill(Mom_neut_1e_cut, P_e_1e_cut, Weight);
                                hReco_P_nFD_vs_Reco_Theta_e_AMap.hFill(Mom_neut_1e_cut, Theta_e, Weight);
                                hReco_P_nFD_vs_Reco_Phi_e_AMap.hFill(Mom_neut_1e_cut, Phi_e, Weight);
                                aMaps_master.hFillHitMaps("Reco", "Neutron", Mom_neut_1e_cut, Theta_neut_1e_cut, Phi_neut_1e_cut, Weight);
                            }  // end of if pass neutron ECAL veto
                        }  // end of if id. reco leading neutron
                    }  // end of if neutron is within fiducial cuts
                }
            }
        }  // end of fill Acceptance maps if

        // Filling reco. efficiency maps
        if (Generate_WMaps) {
            // Filling electron reco. efficiency maps
            if (electrons[0]->getRegion() == FD) {
                /* Fill reco electron efficiency maps */
                hReco_P_e_WMaps.hFill(P_e_1e_cut, Weight);
                hElectronAMapBCwKC.hFill(Phi_e, Theta_e, Weight);
                hReco_P_e_vs_Reco_Theta_e_WMap.hFill(P_e_1e_cut, Theta_e, Weight);
                hReco_P_e_vs_Reco_Phi_e_WMap.hFill(P_e_1e_cut, Phi_e, Weight);
                wMaps_master.hFillHitMaps("Reco", "Electron", P_e_1e_cut, Theta_e, Phi_e, Weight);
            }

            // Filling proton reco. efficiency maps
            for (int &i : Protons_ind) {
                /* Fill all reco FD proton efficiency maps */
                if (protons[i]->getRegion() == FD) {
                    double Reco_Proton_Momentum = protons[i]->getP();

                    if ((Reco_Proton_Momentum <= p_mom_th.GetUpperCut()) && (Reco_Proton_Momentum >= p_mom_th.GetLowerCut())) {
                        // if id. reco proton
                        bool FD_Theta_Cut_Reco_protons = ((protons[i]->getTheta() * 180.0 / pi) <= FD_nucleon_theta_cut.GetUpperCut());
                        bool FD_Momentum_Cut_Reco_protons =
                            ((Reco_Proton_Momentum <= FD_nucleon_momentum_cut.GetUpperCut()) && (Reco_Proton_Momentum >= FD_nucleon_momentum_cut.GetLowerCut()));

                        if (FD_Theta_Cut_Reco_protons && FD_Momentum_Cut_Reco_protons) {
                            hReco_P_pFD_WMaps.hFill(Reco_Proton_Momentum, Weight);
                            hProtonAMapBCwKC.hFill(protons[i]->getPhi() * 180.0 / pi, protons[i]->getTheta() * 180.0 / pi, Weight);
                            hReco_P_pFD_vs_Reco_Theta_pFD_WMap.hFill(Reco_Proton_Momentum, protons[i]->getTheta() * 180.0 / pi, Weight);
                            hReco_P_pFD_vs_Reco_Phi_pFD_WMap.hFill(Reco_Proton_Momentum, protons[i]->getPhi() * 180.0 / pi, Weight);
                            hReco_P_pFD_vs_Reco_P_e_WMap.hFill(Reco_Proton_Momentum, P_e_1e_cut, Weight);
                            hReco_P_pFD_vs_Reco_Theta_e_WMap.hFill(Reco_Proton_Momentum, Theta_e, Weight);
                            hReco_P_pFD_vs_Reco_Phi_e_WMap.hFill(Reco_Proton_Momentum, Phi_e, Weight);
                            wMaps_master.hFillHitMaps("Reco", "Proton", Reco_Proton_Momentum, protons[i]->getTheta() * 180.0 / pi, protons[i]->getPhi() * 180.0 / pi, Weight);
                        }
                    }  // end of if id. reco proton
                }
            }

            // Filling neurton reco. efficiency maps
            if (ES_by_leading_FDneutron) {
                if (NeutronsFD_ind_mom_max != -1) {
                    // if NeutronsFD_ind_mom_max == -1, there are no neutrons above momentum th. in the event
                    /* Fill leading reco FD neutron efficiency maps */
                    bool hitPCAL_1e_cut = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                    bool hitECIN_1e_cut = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                    bool hitECOUT_1e_cut = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
                    auto n_detlayer_1e_cut = hitECIN_1e_cut ? clas12::ECIN : clas12::ECOUT;                                 // find first layer of hit

                    // Safety checks that leading nFD is neutron by definition (AMaps & WMaps)
                    CodeDebugger.SafetyCheck_AMaps_Reco_leading_neutrons(__FILE__, __LINE__, allParticles, NeutronsFD_ind_mom_max, hitPCAL_1e_cut, hitECIN_1e_cut, hitECOUT_1e_cut);

                    if (allParticles[NeutronsFD_ind_mom_max]->cal(n_detlayer_1e_cut)->getLv() > clasAna.getEcalEdgeCuts() &&
                        allParticles[NeutronsFD_ind_mom_max]->cal(n_detlayer_1e_cut)->getLw() > clasAna.getEcalEdgeCuts()) {
                        // if neutron is within fiducial cuts

                        bool NeutronPassVeto_1e_cut = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, NeutronsFD_ind_mom_max, Neutron_veto_cut.GetLowerCut());

                        double Mom_neut_1e_cut = pid.GetFDNeutronP(allParticles[NeutronsFD_ind_mom_max], apply_nucleon_cuts);
                        double Theta_neut_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getTheta() * 180.0 / pi;
                        double Phi_neut_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getPhi() * 180.0 / pi;

                        if ((Mom_neut_1e_cut <= n_mom_th.GetUpperCut()) && (Mom_neut_1e_cut >= n_mom_th.GetLowerCut())) {
                            // if id. reco leading neutron
                            bool FD_Theta_Cut_Reco_neutrons = (Theta_neut_1e_cut <= FD_nucleon_theta_cut.GetUpperCut());
                            bool FD_Momentum_Cut_Reco_neutrons = ((Mom_neut_1e_cut <= FD_nucleon_momentum_cut.GetUpperCut()) && (Mom_neut_1e_cut >= FD_nucleon_momentum_cut.GetLowerCut()));

                            // if neutron passes ECAL veto:
                            if (NeutronPassVeto_1e_cut) {
                                if (FD_Theta_Cut_Reco_neutrons && FD_Momentum_Cut_Reco_neutrons) {
                                    hReco_P_nFD_WMaps.hFill(Mom_neut_1e_cut, Weight);
                                    hNeutronAMapBCwKC.hFill(Phi_neut_1e_cut, Theta_neut_1e_cut, Weight);
                                    hReco_P_nFD_vs_Reco_Theta_nFD_WMap.hFill(Mom_neut_1e_cut, Theta_neut_1e_cut, Weight);
                                    hReco_P_nFD_vs_Reco_Phi_nFD_WMap.hFill(Mom_neut_1e_cut, Phi_neut_1e_cut, Weight);
                                    hReco_P_nFD_vs_Reco_P_e_WMap.hFill(Mom_neut_1e_cut, P_e_1e_cut, Weight);
                                    hReco_P_nFD_vs_Reco_Theta_e_WMap.hFill(Mom_neut_1e_cut, Theta_e, Weight);
                                    hReco_P_nFD_vs_Reco_Phi_e_WMap.hFill(Mom_neut_1e_cut, Phi_e, Weight);
                                    wMaps_master.hFillHitMaps("Reco", "Neutron", Mom_neut_1e_cut, Theta_neut_1e_cut, Phi_neut_1e_cut, Weight);
                                }
                            }  // end of if pass neutron ECAL veto
                        }  // end of if id. reco leading neutron
                    }  // end of if neutron is within fiducial cuts
                }
            }
        }  // end of fill efficiency maps if

        // Fill neutron multiplicity plots (1e cut)
        pid.FillNeutMultiPlots(allParticles, electrons, Weight, beamE, Neutron_veto_cut.GetLowerCutConst(), hNeut_Multi_By_clas12pid_BPID_BV_1e_cut_FD,
                               hNeut_Multi_By_clas12pid_BPID_AV_1e_cut_FD, neutrons, hNeut_Multi_By_clas12pid_APID_BV_1e_cut_FD, hNeut_Multi_By_clas12pid_APID_AV_1e_cut_FD, neutrons);
        pid.FillNeutMultiPlots(allParticles, electrons, Weight, beamE, Neutron_veto_cut.GetLowerCutConst(), hNeut_Multi_By_Redef_BPID_BV_1e_cut_FD, hNeut_Multi_By_Redef_BPID_AV_1e_cut_FD,
                               ReDef_neutrons_FD, hNeut_Multi_By_Redef_APID_BV_1e_cut_FD, hNeut_Multi_By_Redef_APID_AV_1e_cut_FD, NeutronsFD_ind);

        //  1p (FD only) --------------------------------------------------------------------------------------------------------------------------------------------------------

        CodeDebugger.PrintStepTester(DebuggerMode);

        // 1p (FD only)
        /* 1p event selection: 1p = Protons_ind.size() = 1 in the FD, any number of FD neutrons and any number of other neutrals and particles with pdg=0.*/
        bool one_rec_FD_proton_1p = ((Protons_ind.size() == 1) && (protons[Protons_ind.at(0)]->getRegion() == FD));
        bool reco_FD_Neutrons_1p = (Enable_FD_neutrons || (NeutronsFD_ind.size() == 0));  // no id. FD neutrons for Enable_FD_neutrons = false
        bool event_selection_1p = (basic_event_selection && one_rec_FD_proton_1p && reco_FD_Neutrons_1p);
        bool apply_TL_1p_ES = (!Rec_wTL_ES || TL_Event_Selection_1p);

        if (calculate_1p && event_selection_1p && apply_TL_1p_ES) {
            // for 1p calculations (with any number of neutrals)

            // Setting particle vectors & SaS variable (for code organization)
            /* Defining initial particle vectors: */
            region_part_ptr e_1p = electrons[Electron_ind.at(0)];
            region_part_ptr p_1p = protons[Protons_ind.at(0)];

            /* Safety check that we are looking at 1p */
            CodeDebugger.SafetyCheck_basic_event_selection(__FILE__, __LINE__, "1p", Kplus, Kminus, Piplus_ind, Piminus_ind, Electron_ind, deuterons);
            CodeDebugger.SafetyCheck_1p(__FILE__, __LINE__, Protons_ind, e_1p, p_1p, Enable_FD_photons, PhotonsFD_ind);

            // Setting 1p analysis variables
            double ProtonMomBKC_1p = p_1p->getP();  // proton momentum before smearing or kinematical cuts

            TVector3 P_e_1p_3v, q_1p_3v, P_p_1p_3v, P_T_e_1p_3v, P_T_p_1p_3v, dP_T_1p_3v, P_N_1p_3v;

            P_e_1p_3v.SetMagThetaPhi(e_1p->getP(), e_1p->getTheta(), e_1p->getPhi());                                              // electron 3 momentum
            q_1p_3v = TVector3(Pvx - P_e_1p_3v.Px(), Pvy - P_e_1p_3v.Py(), Pvz - P_e_1p_3v.Pz());                                  // 3 momentum transfer
            P_p_1p_3v.SetMagThetaPhi(nRes.PSmear(apply_nucleon_SmearAndCorr, ProtonMomBKC_1p), p_1p->getTheta(), p_1p->getPhi());  // proton 3 momentum
            P_T_e_1p_3v = TVector3(P_e_1p_3v.Px(), P_e_1p_3v.Py(), 0);                                                             // electron transverse momentum
            P_T_p_1p_3v = TVector3(P_p_1p_3v.Px(), P_p_1p_3v.Py(), 0);                                                             // proton transverse momentum

            double E_e_1p = sqrt(m_e * m_e + P_e_1p_3v.Mag2()), E_p_1p = sqrt(m_p * m_p + P_p_1p_3v.Mag2()), Ecal_1p, dAlpha_T_1p, dPhi_T_1p;
            double omega_1p = beamE - E_e_1p, W_1p = sqrt((omega_1p + m_p) * (omega_1p + m_p) - q_1p_3v.Mag2());
            double Theta_p_e_p_p_1p, Theta_q_p_p_1p;
            double EoP_e_1p = (e_1p->cal(clas12::PCAL)->getEnergy() + e_1p->cal(ECIN)->getEnergy() + e_1p->cal(ECOUT)->getEnergy()) / P_e_1p_3v.Mag();
            double Vx_e_1p = e_1p->par()->getVx(), Vy_e_1p = e_1p->par()->getVy(), Vz_e_1p = e_1p->par()->getVz();

            /* Setting Q2 (1p) */
            TLorentzVector e_out_1p, Q_1p;
            double Q2_1p;
            e_out_1p.SetPxPyPzE(e_1p->par()->getPx(), e_1p->par()->getPy(), e_1p->par()->getPz(), E_e_1p);
            Q_1p = beam - e_out_1p;  // definition of 4-momentum transfer
            Q2_1p = fabs(Q_1p.Mag2());
            double xB_1p = Q2_1p / (2 * m_p * omega_1p);

            /* Setting particle angles (1p) */
            double Theta_e_1p = e_1p->getTheta() * 180.0 / pi, Phi_e_1p = e_1p->getPhi() * 180.0 / pi;  // Theta_e_1p, Phi_e_1p in deg
            double Theta_p_1p = p_1p->getTheta() * 180.0 / pi, Phi_p_1p = p_1p->getPhi() * 180.0 / pi;  // Theta_pFD_1p, Phi_pFD_1p in deg

            /* Weights -> before proton shifting; because proton detection is good! */
            double Weight_1p = wMaps_master.GetWeight(apply_kinematical_weights, "Proton", ProtonMomBKC_1p, Theta_p_1p, Phi_p_1p);

            // Setting kinematical cuts -----------------------------------------------------------------------------------------------------------------------------

            // Setting kinematical cuts
            /* Protons have lower momentum threshold & we don't lose good protons -> proton smearing before kin cuts but after FC */
            bool FD_Theta_Cut_1p = ((P_p_1p_3v.Theta() * 180.0 / pi) <= FD_nucleon_theta_cut.GetUpperCut());
            bool FD_Momentum_Cut_1p =
                ((P_p_1p_3v.Mag() <= FD_nucleon_momentum_cut.GetUpperCut()) && (P_p_1p_3v.Mag() >= FD_nucleon_momentum_cut.GetLowerCut()));  // Momentum kin cut after proton smearing
            bool e_withinFC_1p = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Electron", P_e_1p_3v.Mag(), P_e_1p_3v.Theta() * 180.0 / pi,
                                                          P_e_1p_3v.Phi() * 180.0 / pi);
            bool p_withinFC_1p = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Proton", ProtonMomBKC_1p, P_p_1p_3v.Theta() * 180.0 / pi,
                                                          P_p_1p_3v.Phi() * 180.0 / pi, Calc_eff_overlapping_FC);

            bool Pass_Kin_Cuts_1p = ((!apply_kinematical_cuts || (FD_Theta_Cut_1p && FD_Momentum_Cut_1p)) && (!apply_fiducial_cuts || (e_withinFC_1p && p_withinFC_1p)));

            // Fillings 1p histograms -------------------------------------------------------------------------------------------------------------------------------

            // Fillings 1p histograms
            if (Pass_Kin_Cuts_1p) {
                ++num_of_events_1p_inFD;  // 1p event count after momentum and theta_p cuts

                // Filling cut variable plots (1p)
                /* Filling Nphe plots (1p) */
                hNphe_1p_FD.hFill(e_1p->che(clas12::HTCC)->getNphe(), Weight_1p);

                /* Filling Chi2 histograms (1p) */
                // Electrton Chi2 (1p):
                hChi2_Electron_1p_FD.hFill(e_1p->par()->getChi2Pid(), Weight_1p);

                // Proton Chi2 (1p):
                if (p_1p->getRegion() == CD) {
                    hChi2_Proton_1p_CD.hFill(p_1p->par()->getChi2Pid(), Weight_1p);
                } else if (p_1p->getRegion() == FD) {
                    hChi2_Proton_1p_FD.hFill(p_1p->par()->getChi2Pid(), Weight_1p);
                }

                /* Filling dVx, dVy, dVz (1p) */
                for (auto &p : protons) {
                    double Vx_p_1p = p->par()->getVx(), Vy_p_1p = p->par()->getVy(), Vz_p_1p = p->par()->getVz();
                    double dVx = Vx_p_1p - Vx_e_1p, dVy = Vy_p_1p - Vy_e_1p, dVz = Vz_p_1p - Vz_e_1p;

                    hdVx_1p.hFill(dVx, Weight_1p), hdVy_1p.hFill(dVy, Weight_1p), hdVz_1p.hFill(dVz, Weight_1p);
                }  // end of loop over protons vector

                /* Filling SF histograms (1p) */
                hSF_1p_FD.hFill(EoP_e_1p, Weight_1p), hSF_VS_P_e_1p_FD.hFill(P_e_1p_3v.Mag(), EoP_e_1p, Weight_1p);

                /* Filling fiducial plots (1p) */
                hVcal_VS_EoP_1p_PCAL.hFill(e_1p->cal(clas12::PCAL)->getLv(), EoP_e_1p, Weight_1p);
                hWcal_VS_EoP_1p_PCAL.hFill(e_1p->cal(clas12::PCAL)->getLw(), EoP_e_1p, Weight_1p);

                // Filling momentum threshold histograms (1p)
                /* Filling momentum threshold histograms (1p) */

                // Electron momentum (1p)
                hP_e_APID_1p_FD.hFill(P_e_1p_3v.Mag(), Weight_1p);  // after mom. th.

                for (int i = 0; i < Ne; i++) {
                    if (electrons[i]->getRegion() == FD) { hP_e_BPID_1p_FD.hFill(electrons[i]->getP(), Weight_1p); }  // before mom. th.
                }

                // Proton momentum (1p)
                /* Protons after mom. th. */
                for (int &i : Protons_ind) {
                    if (protons[i]->getRegion() == CD) {
                        hP_p_APID_1p_CD.hFill(protons[i]->getP(), Weight_1p);  // after mom. th.
                        hP_p_APIDandPS_1p_CD.hFill(nRes.PSmear(apply_nucleon_SmearAndCorr, protons[i]->getP()), Weight_1p);
                        // after mom. th. & smearing
                    } else if (protons[i]->getRegion() == FD) {
                        hP_p_APID_1p_FD.hFill(protons[i]->getP(), Weight_1p);  // after mom. th.
                        hP_p_APIDandPS_1p_FD.hFill(nRes.PSmear(apply_nucleon_SmearAndCorr, protons[i]->getP()), Weight_1p);
                        // after mom. th. & smearing
                    }
                }

                /* Protons before mom. th. */
                for (int i = 0; i < Np; i++) {
                    if (protons[i]->getRegion() == CD) {
                        hP_p_BPID_1p_CD.hFill(protons[i]->getP(), Weight_1p);  // before mom. th.
                    } else if (protons[i]->getRegion() == FD) {
                        hP_p_BPID_1p_FD.hFill(protons[i]->getP(), Weight_1p);  // before mom. th.
                    }
                }

                // Piplus momentum (1p)
                for (int &i : Piplus_ind) {
                    if (piplus[i]->getRegion() == CD) {
                        hP_piplus_APID_1p_CD.hFill(piplus[i]->getP(), Weight_1p);  // after mom. th.
                    } else if (piplus[i]->getRegion() == FD) {
                        hP_piplus_APID_1p_FD.hFill(piplus[i]->getP(), Weight_1p);  // after mom. th.
                    }
                }

                for (int i = 0; i < Npip; i++) {
                    if (piplus[i]->getRegion() == CD) {
                        hP_piplus_BPID_1p_CD.hFill(piplus[i]->getP(), Weight_1p);  // before mom. th.
                    } else if (piplus[i]->getRegion() == FD) {
                        hP_piplus_BPID_1p_FD.hFill(piplus[i]->getP(), Weight_1p);  // before mom. th.
                    }
                }

                // Piminus momentum (1p)
                for (int &i : Piminus_ind) {
                    if (piminus[i]->getRegion() == CD) {
                        hP_piminus_APID_1p_CD.hFill(piminus[i]->getP(), Weight_1p);  // after mom. th.
                    } else if (piminus[i]->getRegion() == FD) {
                        hP_piminus_APID_1p_FD.hFill(piminus[i]->getP(), Weight_1p);  // after mom. th.
                    }
                }

                for (int i = 0; i < Npim; i++) {
                    if (piminus[i]->getRegion() == CD) {
                        hP_piminus_BPID_1p_CD.hFill(piminus[i]->getP(), Weight_1p);  // before mom. th.
                    } else if (piminus[i]->getRegion() == FD) {
                        hP_piminus_BPID_1p_FD.hFill(piminus[i]->getP(), Weight_1p);  // before mom. th.
                    }
                }

                // Photon momentum (1p)
                for (int &i : PhotonsFD_ind) { hP_ph_APID_1p_FD.hFill(allParticles[i]->getP(), Weight_1p); }  // after mom. th.

                for (int &i : ReDef_photons_FD) { hP_ph_BPID_1p_FD.hFill(allParticles[i]->getP(), Weight_1p); }  // before mom. th.

                // Neutron momentum (1p)
                for (int &i : NeutronsFD_ind) { hP_n_APID_1p_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_1p); }  // after mom. th.

                for (int &i : ReDef_neutrons_FD) { hP_n_BPID_1p_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_1p); }  // before mom. th.

                // Filling Beta vs. P plots (1p)

                // Beta vs. P from electrons (1p, CD & FD)
                if (e_1p->getRegion() == FD) {
                    hBeta_vs_P_1p_FD.hFill(e_1p->getP(), e_1p->par()->getBeta(), Weight_1p);
                    hBeta_vs_P_1p_Electrons_Only_FD.hFill(e_1p->getP(), e_1p->par()->getBeta(), Weight_1p);
                    hBeta_vs_P_negative_part_1p_FD.hFill(e_1p->getP(), e_1p->par()->getBeta(), Weight_1p);
                }

                // Beta vs. P from protons (1p, CD & FD)
                // TODO: get rid of the loop. there is only one proton here.
                for (int i = 0; i < Np; i++) {
                    if (protons[i]->getRegion() == CD) {
                        hBeta_vs_P_1p_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_1p);
                        hBeta_vs_P_1p_Protons_Only_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_1p);
                        hBeta_vs_P_positive_part_1p_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_1p);
                    } else if (protons[i]->getRegion() == FD) {
                        hBeta_vs_P_1p_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_1p);
                        hBeta_vs_P_1p_Protons_Only_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_1p);
                        hBeta_vs_P_positive_part_1p_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_1p);
                    }
                }  // end of loop over protons vector

                // Filling electron plots (1p)
                hTheta_e_All_Int_1p_FD->Fill(Theta_e_1p, Weight_1p);
                hPhi_e_All_Int_1p_FD->Fill(Phi_e_1p, Weight_1p);
                hPhi_e_VS_P_e_1p_FD->Fill(P_e_1p_3v.Mag(), Phi_e_1p, Weight_1p);
                hPhi_e_VS_W_1p_FD->Fill(W_1p, Phi_e_1p, Weight_1p);
                hTheta_e_VS_Phi_e_1p_FD->Fill(Phi_e_1p, Theta_e_1p, Weight_1p);
                hTheta_e_VS_P_e_1p_FD->Fill(P_e_1p_3v.Mag(), Theta_e_1p, Weight_1p);
                hTheta_e_VS_W_1p_FD->Fill(W_1p, Theta_e_1p, Weight_1p);

                hE_e_All_Int_1p_FD->Fill(E_e_1p, Weight_1p);
                hE_e_VS_Theta_e_All_Int_1p_FD->Fill(Theta_e_1p, E_e_1p, Weight_1p);

                hET_All_Ang_All_Int_1p_FD->Fill(beamE - E_e_1p, Weight_1p);

                if ((Theta_e_1p >= 14.0) && (Theta_e_1p <= 16.0)) {
                    hET15_All_Int_1p_FD->Fill(beamE - E_e_1p, Weight_1p);
                    hE_e_15_All_Int_1p_FD->Fill(E_e_1p, Weight_1p);
                }

                if (qel) {
                    hTheta_e_QEL_1p_FD->Fill(Theta_e_1p, Weight_1p);
                    hPhi_e_QEL_1p_FD->Fill(Phi_e_1p, Weight_1p);
                    hE_e_QEL_1p_FD->Fill(E_e_1p, Weight_1p);
                    hE_e_VS_Theta_e_QEL_1p_FD->Fill(Theta_e_1p, E_e_1p, Weight_1p);

                    hET_All_Ang_QEL_1p_FD->Fill(beamE - E_e_1p, Weight_1p);

                    if ((Theta_e_1p >= 14.0) && (Theta_e_1p <= 16.0)) {
                        hET15_QEL_1p_FD->Fill(beamE - E_e_1p, Weight_1p);
                        hE_e_15_QEL_1p_FD->Fill(E_e_1p, Weight_1p);
                    }
                } else if (mec) {
                    hTheta_e_MEC_1p_FD->Fill(Theta_e_1p, Weight_1p);
                    hPhi_e_MEC_1p_FD->Fill(Phi_e_1p, Weight_1p);
                    hE_e_MEC_1p_FD->Fill(E_e_1p, Weight_1p);
                    hE_e_VS_Theta_e_MEC_1p_FD->Fill(Theta_e_1p, E_e_1p, Weight_1p);

                    hET_All_Ang_MEC_1p_FD->Fill(beamE - E_e_1p, Weight_1p);

                    if ((Theta_e_1p >= 14.0) && (Theta_e_1p <= 16.0)) {
                        hET15_MEC_1p_FD->Fill(beamE - E_e_1p, Weight_1p);
                        hE_e_15_MEC_1p_FD->Fill(E_e_1p, Weight_1p);
                    }
                } else if (res) {
                    hTheta_e_RES_1p_FD->Fill(Theta_e_1p, Weight_1p);
                    hPhi_e_RES_1p_FD->Fill(Phi_e_1p, Weight_1p);
                    hE_e_RES_1p_FD->Fill(E_e_1p, Weight_1p);
                    hE_e_VS_Theta_e_RES_1p_FD->Fill(Theta_e_1p, E_e_1p, Weight_1p);

                    hET_All_Ang_RES_1p_FD->Fill(beamE - E_e_1p, Weight_1p);

                    if ((Theta_e_1p >= 14.0) && (Theta_e_1p <= 16.0)) {
                        hET15_RES_1p_FD->Fill(beamE - E_e_1p, Weight_1p);
                        hE_e_15_RES_1p_FD->Fill(E_e_1p, Weight_1p);
                    }
                } else if (dis) {
                    hTheta_e_DIS_1p_FD->Fill(Theta_e_1p, Weight_1p);
                    hPhi_e_DIS_1p_FD->Fill(Phi_e_1p, Weight_1p);
                    hE_e_DIS_1p_FD->Fill(E_e_1p, Weight_1p);
                    hE_e_VS_Theta_e_DIS_1p_FD->Fill(Theta_e_1p, E_e_1p, Weight_1p);

                    hET_All_Ang_DIS_1p_FD->Fill(beamE - E_e_1p, Weight_1p);

                    if ((Theta_e_1p >= 14.0) && (Theta_e_1p <= 16.0)) {
                        hET15_DIS_1p_FD->Fill(beamE - E_e_1p, Weight_1p);
                        hE_e_15_DIS_1p_FD->Fill(E_e_1p, Weight_1p);
                    }
                }

                // Fill momentum transfer plots (1p)
                FillByInt(hQ2_1p, hQ2_QEL_1p, hQ2_MEC_1p, hQ2_RES_1p, hQ2_DIS_1p, qel, mec, res, dis, Q2_1p, Weight_1p);
                FillByInt(hq_3v_1p, hq_3v_QEL_1p, hq_3v_MEC_1p, hq_3v_RES_1p, hq_3v_DIS_1p, qel, mec, res, dis, q_1p_3v.Mag(), Weight_1p);

                hQ2_VS_W_1p->Fill(W_1p, Q2_1p, Weight_1p);
                hQ2_VS_q_3v_1p->Fill(q_1p_3v.Mag(), Q2_1p, Weight_1p);
                hQ2_VS_omega_1p->Fill(omega_1p, Q2_1p, Weight_1p);
                hq_3v_VS_omega_1p->Fill(omega_1p, q_1p_3v.Mag(), Weight_1p);

                if (qel) {
                    hQ2_VS_W_QEL_1p->Fill(W_1p, Q2_1p, Weight_1p);
                    hQ2_VS_q_3v_QEL_1p->Fill(q_1p_3v.Mag(), Q2_1p, Weight_1p);
                    hQ2_VS_omega_QEL_1p->Fill(omega_1p, Q2_1p, Weight_1p);
                    hq_3v_VS_omega_QEL_1p->Fill(omega_1p, q_1p_3v.Mag(), Weight_1p);
                } else if (mec) {
                    hQ2_VS_W_MEC_1p->Fill(W_1p, Q2_1p, Weight_1p);
                    hQ2_VS_q_3v_MEC_1p->Fill(q_1p_3v.Mag(), Q2_1p, Weight_1p);
                    hQ2_VS_omega_MEC_1p->Fill(omega_1p, Q2_1p, Weight_1p);
                    hq_3v_VS_omega_MEC_1p->Fill(omega_1p, q_1p_3v.Mag(), Weight_1p);
                } else if (res) {
                    hQ2_VS_W_RES_1p->Fill(W_1p, Q2_1p, Weight_1p);
                    hQ2_VS_q_3v_RES_1p->Fill(q_1p_3v.Mag(), Q2_1p, Weight_1p);
                    hQ2_VS_omega_RES_1p->Fill(omega_1p, Q2_1p, Weight_1p);
                    hq_3v_VS_omega_RES_1p->Fill(omega_1p, q_1p_3v.Mag(), Weight_1p);
                } else if (dis) {
                    hQ2_VS_W_DIS_1p->Fill(W_1p, Q2_1p, Weight_1p);
                    hQ2_VS_q_3v_DIS_1p->Fill(q_1p_3v.Mag(), Q2_1p, Weight_1p);
                    hQ2_VS_omega_DIS_1p->Fill(omega_1p, Q2_1p, Weight_1p);
                    hq_3v_VS_omega_DIS_1p->Fill(omega_1p, q_1p_3v.Mag(), Weight_1p);
                }

                // Filling Ecal plots (1p)
                Ecal_1p = E_e_1p + (E_p_1p - m_p);

                hEcal_All_Int_1p->Fill(Ecal_1p, Weight_1p);  // Fill Ecal for all interactions

                if (qel) {
                    hEcal_QEL_1p->Fill(Ecal_1p, Weight_1p);  // Fill Ecal for QEL only
                } else if (mec) {
                    hEcal_MEC_1p->Fill(Ecal_1p, Weight_1p);  // Fill Ecal for MEC only
                } else if (res) {
                    hEcal_RES_1p->Fill(Ecal_1p, Weight_1p);  // Fill Ecal for RES only
                } else if (dis) {
                    hEcal_DIS_1p->Fill(Ecal_1p, Weight_1p);  // Fill Ecal for DIS only
                }

                if (Ecal_1p > beamE) {
                    hEcal_vs_P_e_test_1p->Fill(P_e_1p_3v.Mag(), Ecal_1p, Weight_1p);
                    hEcal_vs_Theta_e_test_1p->Fill(Theta_e_1p, Ecal_1p, Weight_1p);
                    hEcal_vs_Phi_e_test_1p->Fill(Phi_e_1p, Ecal_1p, Weight_1p);
                    hEcal_vs_P_p_test_1p->Fill(P_p_1p_3v.Mag(), Ecal_1p, Weight_1p);
                    hEcal_vs_Theta_p_test_1p->Fill(Theta_p_1p, Ecal_1p, Weight_1p);
                    hEcal_vs_Phi_p_test_1p->Fill(Phi_p_1p, Ecal_1p, Weight_1p);
                }

                // Fill neutron multiplicity plots (1p)
                pid.FillNeutMultiPlots(allParticles, electrons, Weight_1p, beamE, Neutron_veto_cut.GetLowerCutConst(), hNeut_Multi_By_Redef_BPID_BV_1p_FD, hNeut_Multi_By_Redef_BPID_AV_1p_FD,
                                       ReDef_neutrons_FD, hNeut_Multi_By_Redef_APID_BV_1p_FD, hNeut_Multi_By_Redef_APID_AV_1p_FD, NeutronsFD_ind);

                // Fill W (1p)
                FillByInt(hW_All_Int_1p, hW_QEL_1p, hW_MEC_1p, hW_RES_1p, hW_DIS_1p, qel, mec, res, dis, W_1p, Weight_1p);
                hW_VS_q_3v_1p->Fill(W_1p, q_1p_3v.Mag(), Weight_1p);
                hW_VS_omega_1p->Fill(W_1p, omega_1p, Weight_1p);

                if (qel) {
                    hW_VS_q_3v_QEL_1p->Fill(W_1p, q_1p_3v.Mag(), Weight_1p);
                    hW_VS_omega_QEL_1p->Fill(W_1p, omega_1p, Weight_1p);
                } else if (mec) {
                    hW_VS_q_3v_MEC_1p->Fill(W_1p, q_1p_3v.Mag(), Weight_1p);
                    hW_VS_omega_MEC_1p->Fill(W_1p, omega_1p, Weight_1p);
                } else if (res) {
                    hW_VS_q_3v_RES_1p->Fill(W_1p, q_1p_3v.Mag(), Weight_1p);
                    hW_VS_omega_RES_1p->Fill(W_1p, omega_1p, Weight_1p);
                } else if (dis) {
                    hW_VS_q_3v_DIS_1p->Fill(W_1p, q_1p_3v.Mag(), Weight_1p);
                    hW_VS_omega_DIS_1p->Fill(W_1p, omega_1p, Weight_1p);
                }

                hP_pFD_APID_1p.hFill(ProtonMomBKC_1p, Weight_1p);       // FD proton (1p)
                hP_pFD_APIDandPS_1p.hFill(P_p_1p_3v.Mag(), Weight_1p);  // FD proton after smearing (1p)
                hP_pFD_APIDandPS_VS_W_1p->Fill(W_1p, P_p_1p_3v.Mag(), Weight_1p);

                if (qel) {
                    hP_pFD_APIDandPS_VS_W_QEL_1p->Fill(W_1p, P_p_1p_3v.Mag(), Weight_1p);
                } else if (mec) {
                    hP_pFD_APIDandPS_VS_W_MEC_1p->Fill(W_1p, P_p_1p_3v.Mag(), Weight_1p);
                } else if (res) {
                    hP_pFD_APIDandPS_VS_W_RES_1p->Fill(W_1p, P_p_1p_3v.Mag(), Weight_1p);
                } else if (dis) {
                    hP_pFD_APIDandPS_VS_W_DIS_1p->Fill(W_1p, P_p_1p_3v.Mag(), Weight_1p);
                }

                dP_T_1p_3v = TVector3(P_T_e_1p_3v.Px() + P_T_p_1p_3v.Px(), P_T_e_1p_3v.Py() + P_T_p_1p_3v.Py(), 0);
                dAlpha_T_1p = acos(-(P_e_1p_3v.Px() * dP_T_1p_3v.Px() + P_e_1p_3v.Py() * dP_T_1p_3v.Py() + P_e_1p_3v.Pz() * dP_T_1p_3v.Pz()) / (P_T_e_1p_3v.Mag() * dP_T_1p_3v.Mag())) *
                              180.0 / pi;  // dP_T_1p_3v.Pz() = 0; dAlpha_T_1p in deg
                hdP_T_1p->Fill(dP_T_1p_3v.Mag(), Weight_1p);
                hdAlpha_T_1p->Fill(dAlpha_T_1p, Weight_1p);
                hdP_T_vs_dAlpha_T_1p->Fill(dAlpha_T_1p, dP_T_1p_3v.Mag(), Weight_1p);

                dPhi_T_1p =
                    acos(-(P_T_e_1p_3v.Px() * P_T_p_1p_3v.Px() + P_T_e_1p_3v.Py() * P_T_p_1p_3v.Py() + P_T_e_1p_3v.Pz() * P_T_p_1p_3v.Pz()) / (P_T_e_1p_3v.Mag() * P_T_p_1p_3v.Mag())) *
                    180.0 / pi;  // P_T_p_1p_3v.Pz() = 0; dPhi_T_1p in deg
                hdPhi_T_1p->Fill(dPhi_T_1p, Weight_1p);

                hEcal_vs_P_e_1p->Fill(P_e_1p_3v.Mag(), Ecal_1p, Weight_1p);
                hEcal_vs_Theta_e_1p->Fill(Theta_e_1p, Ecal_1p, Weight_1p);
                hEcal_vs_Phi_e_1p->Fill(Phi_e_1p, Ecal_1p, Weight_1p);
                hEcal_vs_P_p_1p->Fill(P_p_1p_3v.Mag(), Ecal_1p, Weight_1p);
                hEcal_vs_Theta_p_1p->Fill(Theta_p_1p, Ecal_1p, Weight_1p);
                hEcal_vs_Phi_p_1p->Fill(Phi_p_1p, Ecal_1p, Weight_1p);
                hEcal_vs_dAlpha_T_1p->Fill(dAlpha_T_1p, Ecal_1p, Weight_1p);
                hEcal_vs_dP_T_1p->Fill(dP_T_1p_3v.Mag(), Ecal_1p, Weight_1p);

                hTheta_p_All_Int_1p->Fill(Theta_p_1p, Weight_1p);
                hTheta_p_VS_P_p_1p_FD->Fill(P_p_1p_3v.Mag(), Theta_p_1p, Weight_1p);
                hTheta_p_VS_W_1p_FD->Fill(W_1p, Theta_p_1p, Weight_1p);
                hPhi_p_All_Int_1p->Fill(Phi_p_1p, Weight_1p);
                hTheta_p_VS_Phi_p_1p_FD->Fill(Phi_p_1p, Theta_p_1p, Weight_1p);

                Theta_p_e_p_p_1p = acos((P_e_1p_3v.Px() * P_p_1p_3v.Px() + P_e_1p_3v.Py() * P_p_1p_3v.Py() + P_e_1p_3v.Pz() * P_p_1p_3v.Pz()) / (P_e_1p_3v.Mag() * P_p_1p_3v.Mag())) * 180.0 /
                                   pi;  // Theta_p_e_p_p_1p in deg
                hTheta_p_e_p_p_1p->Fill(Theta_p_e_p_p_1p, Weight_1p);

                Theta_q_p_p_1p = acos((q_1p_3v.Px() * P_p_1p_3v.Px() + q_1p_3v.Py() * P_p_1p_3v.Py() + q_1p_3v.Pz() * P_p_1p_3v.Pz()) / (q_1p_3v.Mag() * P_p_1p_3v.Mag())) * 180.0 /
                                 pi;  // Theta_q_p_p_1p in deg
                hTheta_q_p_p_1p->Fill(Theta_q_p_p_1p, Weight_1p);

                hTheta_q_p_p_vs_p_p_q_1p->Fill(P_p_1p_3v.Mag() / q_1p_3v.Mag(), Theta_q_p_p_1p, Weight_1p);

                P_N_1p_3v = TVector3(P_e_1p_3v.Px() + P_p_1p_3v.Px() - Pvx, P_e_1p_3v.Py() + P_p_1p_3v.Py() - Pvy, P_e_1p_3v.Pz() + P_p_1p_3v.Pz() - Pvz);
                hTheta_q_p_p_vs_p_N_q_1p->Fill(P_N_1p_3v.Mag() / q_1p_3v.Mag(), Theta_q_p_p_1p, Weight_1p);

                // Fill resolution histograms (1p)
                if (plot_and_fit_MomRes) {
                    auto mcpbank_pRes = c12->mcparts();
                    const Int_t Ngen_pRes = mcpbank_pRes->getRows();

                    int Proton_match_counter = 0;
                    double RecoProtonP_Debug = P_p_1p_3v.Mag();
                    double RecoProtonTheta_Debug = P_p_1p_3v.Theta() * 180.0 / pi;
                    double RecoProtonPhi_Debug = P_p_1p_3v.Phi() * 180.0 / pi;

                    for (Int_t i = 0; i < Ngen_pRes; i++) {
                        mcpbank_pRes->setEntry(i);

                        /* TL Proton kinematic variables */
                        double TLProtonP = rCalc(mcpbank_pRes->getPx(), mcpbank_pRes->getPy(), mcpbank_pRes->getPz());
                        double TLProtonTheta = acos(mcpbank_pRes->getPz() / TLProtonP) * 180.0 / pi;
                        double TLProtonPhi = atan2(mcpbank_pRes->getPy(), mcpbank_pRes->getPx()) * 180.0 / pi;

                        /* Reco Proton kinematic variables */
                        double RecoProtonP = RecoProtonP_Debug;
                        double RecoProtonTheta = RecoProtonTheta_Debug;
                        double RecoProtonPhi = RecoProtonPhi_Debug;

                        /* TL-Reco angle difference */
                        double dProtonTheta = TLProtonTheta - RecoProtonTheta;
                        double dProtonPhi = CalcdPhi(TLProtonPhi - RecoProtonPhi);

                        int pid_pRes = mcpbank_pRes->getPid();
                        //                        auto pid = mcpbank_pRes->getPid();

                        // pRes cuts

                        // pRes good Proton cuts
                        bool pRes_TL_Pass_PIDCut = (pid_pRes == 2212);

                        bool Reco_InFD = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Proton", RecoProtonP, RecoProtonTheta, RecoProtonPhi, false);
                        bool TL_InFD = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Proton", TLProtonP, TLProtonTheta, TLProtonPhi, false);
                        bool pRes_Pass_FiducialCuts = (Reco_InFD && TL_InFD);

                        bool Reco_Theta_kinCut = (RecoProtonTheta <= FD_nucleon_theta_cut.GetUpperCut());
                        bool TL_Theta_kinCuts = (TLProtonTheta <= FD_nucleon_theta_cut.GetUpperCut());
                        bool pRes_Pass_ThetaKinCut = (Reco_Theta_kinCut && TL_Theta_kinCuts);

                        bool pRes_Reco_Pass_Proton_MomKinCut = ((RecoProtonP >= p_mom_th.GetLowerCut()) && (RecoProtonP <= beamE));
                        bool pRes_TL_Pass_Proton_MomKinCut = ((TLProtonP >= p_mom_th.GetLowerCut()) && (TLProtonP <= beamE));

                        // pRes matching cuts
                        double dPhiCut = 5., dThetaCut = 2.;  // TODO: add to a DSCuts variable
                        bool pRes_Pass_dThetaCut = (fabs(dProtonTheta) < dThetaCut);
                        bool pRes_Pass_dPhiCut = (fabs(dProtonPhi) < dPhiCut);

                        if (pRes_TL_Pass_PIDCut && pRes_Pass_FiducialCuts && pRes_Pass_ThetaKinCut && pRes_Reco_Pass_Proton_MomKinCut && pRes_TL_Pass_Proton_MomKinCut) {
                            /* Plots for TL Protons passing pRes cuts */
                            hdTheta_pFD_TL_BC_1p.hFill(dProtonTheta, Weight);
                            hdTheta_pFD_TL_ZOOMIN_BC_1p.hFill(dProtonTheta, Weight);
                            hdPhi_pFD_TL_BC_1p.hFill(dProtonPhi, Weight);
                            hdPhi_pFD_TL_ZOOMIN_BC_1p.hFill(dProtonPhi, Weight);

                            hTheta_pFD_TL_ApResC_1p.hFill(TLProtonTheta, Weight);
                            hPhi_pFD_TL_ApResC_1p.hFill(TLProtonPhi, Weight);
                            hTheta_pFD_TL_VS_Phi_pFD_TL_ApResC_1p->Fill(TLProtonPhi, TLProtonTheta, Weight);

                            if (pRes_Pass_dPhiCut) {
                                /* Plots for TL Protons passing pRes_Pass_dPhiCut */
                                hdTheta_pFD_TL_AdPC_1p.hFill(dProtonTheta, Weight);
                                hdTheta_pFD_TL_ZOOMIN_AdPC_1p.hFill(dProtonTheta, Weight);
                                hdPhi_pFD_TL_AdPC_1p.hFill(dProtonPhi, Weight);
                                hdPhi_pFD_TL_ZOOMIN_AdPC_1p.hFill(dProtonPhi, Weight);
                            }

                            if (pRes_Pass_dThetaCut && pRes_Pass_dPhiCut) {
                                ++Proton_match_counter;

                                /* Plots for TL Protons passing matching cuts */
                                hTheta_pFD_TL_MatchedP_1p.hFill(TLProtonTheta, Weight);
                                hPhi_pFD_TL_MatchedP_1p.hFill(TLProtonPhi, Weight);
                                hTheta_pFD_TL_VS_Phi_pFD_TL_MatchedP_1p->Fill(TLProtonPhi, TLProtonTheta, Weight);

                                /* Filling pRes plots */
                                double pResolution = (TLProtonP - RecoProtonP) / TLProtonP;
                                pRes.hFillResPlotsByType(TLProtonP, RecoProtonP, pResolution, Weight);

                                hP_pFD_Res_1p.hFill(pResolution, Weight);
                                hP_pFD_Res_VS_TL_P_pFD_1p->Fill(TLProtonP, pResolution, Weight);
                                hP_pFD_Res_VS_TL_P_pFD_ZOOMIN_1p->Fill(TLProtonP, pResolution, Weight);
                                hP_pFD_Res_VS_Reco_P_pFD_1p->Fill(ProtonMomBKC_1p, pResolution, Weight);
                                hP_pFD_Res_VS_Reco_P_pFD_ZOOMIN_1p->Fill(ProtonMomBKC_1p, pResolution, Weight);
                                hP_pFD_Res_VS_Smear_Reco_P_pFD_1p->Fill(RecoProtonP, pResolution, Weight);
                                hP_pFD_Res_VS_Smear_Reco_P_pFD_ZOOMIN_1p->Fill(RecoProtonP, pResolution, Weight);
                                hReco_P_pFD_pRes_1p.hFill(RecoProtonP, Weight);
                                hTL_P_pFD_pRes_1p.hFill(TLProtonP, Weight);
                                hTL_P_pFD_vs_Reco_P_pFD_1p.hFill(ProtonMomBKC_1p, RecoProtonP, Weight);
                                hTL_P_pFD_vs_Smear_Reco_P_pFD_1p.hFill(TLProtonP, RecoProtonP, Weight);
                            }
                        }

                        /* Res plots for thesis */
                        // TODO: figure out if these plots are needed
                        if (pRes_TL_Pass_PIDCut && pRes_Pass_FiducialCuts && pRes_Pass_ThetaKinCut && ((RecoProtonP >= p_mom_th.GetLowerCut()) && (RecoProtonP <= p_mom_th.GetUpperCut())) &&
                            ((TLProtonP >= p_mom_th.GetLowerCut()) && (TLProtonP <= p_mom_th.GetUpperCut()))) {
                            if (pRes_Pass_dThetaCut && pRes_Pass_dPhiCut) {
                                double pResolution = (TLProtonP - RecoProtonP) / TLProtonP;
                                hP_pFD_Res_VS_TL_P_pFD_noKC_1p->Fill(TLProtonP, pResolution, Weight);
                                hP_pFD_Res_VS_Reco_P_pFD_noKC_1p->Fill(ProtonMomBKC_1p, pResolution, Weight);
                                hP_pFD_Res_VS_Smear_Reco_P_pFD_noKC_1p->Fill(RecoProtonP, pResolution, Weight);
                            }
                        }
                    }  // end of for loop over TL particles

                    if (Proton_match_counter != 0) {
                        hpRes_Match_Multi_1p.hFill(Proton_match_counter, Weight);
                        hpRes_Match_Multi_vs_Reco_P_pFD_1p.hFill(Proton_match_counter, RecoProtonP_Debug, Weight);
                        hpRes_Match_Multi_vs_Reco_Theta_pFD_1p.hFill(Proton_match_counter, RecoProtonTheta_Debug, Weight);
                        hpRes_Match_Multi_vs_Reco_Phi_pFD_1p.hFill(Proton_match_counter, RecoProtonPhi_Debug, Weight);
                    }
                }  // end of resolution calculation if
            }  // end of pass kinematical cuts (1p) if
        }  // end of 1p cuts if

        //  1n (FD only) --------------------------------------------------------------------------------------------------------------------------------------------------------

        CodeDebugger.PrintStepTester(DebuggerMode);

        // 1n (FD only)
        /* 1n event selection: 1n = any number of id. FD neutrons (we look at the leading nFD), with no charged particles (except electrons) and any number of other
                                    neutrals and particles with pdg=0. */
        bool no_protons_1n = (Protons_ind.size() == 0);                   // there are no id. protons in both CD and FD
        bool at_least_one_FDneutron_1n = (NeutronsFD_ind_mom_max != -1);  // for NeutronsFD_ind_mom_max = -1 we don't have any nFD
        bool event_selection_1n = (basic_event_selection && no_protons_1n && at_least_one_FDneutron_1n);

        bool apply_TL_1n_ES = (!Rec_wTL_ES || TL_Event_Selection_1n);

        if (  // FOR nFD eff test!
              // if ((NeutronsFD_ind.size() == 1) &&  // FFOR nFD eff test!
            calculate_1n && event_selection_1n && apply_TL_1n_ES) {
            // for 1n calculations (with any number of neutrals)
            ++num_of_events_1n_inFD;  // 1n event count after momentum and theta_n cuts

            // Setting particle vectors & SaS variable (for code organization)
            /* Defining initial particle vectors: */

            // Setting FD neutron index (1n)
            int n_ind_1n;

            if (ES_by_leading_FDneutron) {
                n_ind_1n = NeutronsFD_ind_mom_max;
            } else {
                n_ind_1n = NeutronsFD_ind.at(0);
            }

            region_part_ptr e_1n = electrons[Electron_ind.at(0)];
            region_part_ptr n_1n = allParticles[n_ind_1n];  // neutron with the largest momentum magnitude

            bool NeutronInPCAL_1n = (n_1n->cal(clas12::PCAL)->getDetector() == 7);                                   // PCAL hit
            bool NeutronInECIN_1n = (n_1n->cal(clas12::ECIN)->getDetector() == 7);                                   // ECIN hit
            bool NeutronInECOUT_1n = (n_1n->cal(clas12::ECOUT)->getDetector() == 7);                                 // ECOUT hit
            auto n_detlayer_1n = NeutronInPCAL_1n ? clas12::PCAL : NeutronInECIN_1n ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit

            /* Safety check that we are looking at 1n */
            CodeDebugger.SafetyCheck_basic_event_selection(__FILE__, __LINE__, "1n", Kplus, Kminus, Piplus_ind, Piminus_ind, Electron_ind, deuterons);
            CodeDebugger.SafetyCheck_1n(__FILE__, __LINE__, NeutronsFD_ind, e_1n, n_1n, Enable_FD_photons, PhotonsFD_ind, ES_by_leading_FDneutron, pid, allParticles, NeutronsFD_ind_mom_max,
                                        apply_nucleon_cuts, NeutronInPCAL_1n, NeutronInECIN_1n, NeutronInECOUT_1n, n_detlayer_1n);

            // Setting 1n analysis variables
            double NeutronMomBKC_1n = pid.GetFDNeutronP(n_1n, apply_nucleon_cuts);  // neutron momentum before shift for kin cuts

            TVector3 P_e_1n_3v, q_1n_3v, P_n_1n_3v, P_T_e_1n_3v, P_T_n_1n_3v, dP_T_1n_3v, P_N_1n_3v;

            P_e_1n_3v.SetMagThetaPhi(e_1n->getP(), e_1n->getTheta(), e_1n->getPhi());              // electron 3 momentum
            q_1n_3v = TVector3(Pvx - P_e_1n_3v.Px(), Pvy - P_e_1n_3v.Py(), Pvz - P_e_1n_3v.Pz());  // 3 momentum transfer
            P_n_1n_3v.SetMagThetaPhi(nRes.NCorr(apply_nucleon_SmearAndCorr, NeutronMomBKC_1n), n_1n->getTheta(), n_1n->getPhi());
            // neutron 3 momentum
            P_T_e_1n_3v = TVector3(P_e_1n_3v.Px(), P_e_1n_3v.Py(), 0);  // electron t. momentum
            P_T_n_1n_3v = TVector3(P_n_1n_3v.Px(), P_n_1n_3v.Py(), 0);  // neutron t. momentum

            double E_e_1n = sqrt(m_e * m_e + P_e_1n_3v.Mag2()), E_n_1n = sqrt(m_n * m_n + P_n_1n_3v.Mag2()), Ecal_1n, dAlpha_T_1n, dPhi_T_1n;
            double omega_1n = beamE - E_e_1n, W_1n = sqrt((omega_1n + m_n) * (omega_1n + m_n) - q_1n_3v.Mag2());
            double Theta_p_e_p_n_1n, Theta_q_p_n_1n;
            double EoP_e_1n = (e_1n->cal(clas12::PCAL)->getEnergy() + e_1n->cal(ECIN)->getEnergy() + e_1n->cal(ECOUT)->getEnergy()) / P_e_1n_3v.Mag();
            double Vx_e_1n = e_1n->par()->getVx(), Vy_e_1n = e_1n->par()->getVy(), Vz_e_1n = e_1n->par()->getVz();

            /* Setting Q2 (1n) */
            TLorentzVector e_out_1n, Q_1n;
            double Q2_1n;
            e_out_1n.SetPxPyPzE(e_1n->par()->getPx(), e_1n->par()->getPy(), e_1n->par()->getPz(), E_e_1n);
            Q_1n = beam - e_out_1n;  // definition of 4-momentum transfer
            Q2_1n = fabs(Q_1n.Mag2());
            double xB_1n = Q2_1n / (2 * m_n * omega_1n);

            /* Setting particle angles (1n) */
            double Theta_e_1n = e_1n->getTheta() * 180.0 / pi, Phi_e_1n = e_1n->getPhi() * 180.0 / pi;  // Theta_e_1n, Phi_e_1n in deg
            double Theta_n_1n = n_1n->getTheta() * 180.0 / pi, Phi_n_1n = n_1n->getPhi() * 180.0 / pi;  // Theta_pFD_1n, Phi_pFD_1n in deg

            /* Weights -> after neutron shifting; because we want to match the currected neutron momentum to the proton maps! */
            double Weight_1n = wMaps_master.GetWeight(apply_kinematical_weights, "Neutron", P_n_1n_3v.Mag(), P_n_1n_3v.Theta() * 180 / pi, P_n_1n_3v.Phi() * 180 / pi);

            // Fake FD neutrons handling (neutron veto) -----------------------------------------------------------------------------------------------------------------

            // Fake FD neutrons handling (neutron veto)
            TVector3 n_hit_1n_3v, e_hit_1n_3v;

            double n_hit_Phi_1n, n_hit_Theta_1n, e_hit_Phi_1n, e_hit_Theta_1n, dPhi_hit_1n, dTheta_hit_1n;

            if (!NeutronInPCAL_1n && (NeutronInECIN_1n || NeutronInECOUT_1n)) {
                // if neutron did not hit PCAL, and hit either ECIN or ECOUT

                // neutron ECIN/ECAL hit vector and angles:
                n_hit_1n_3v.SetXYZ(n_1n->cal(n_detlayer_1n)->getX(), n_1n->cal(n_detlayer_1n)->getY(), n_1n->cal(n_detlayer_1n)->getZ());
                n_hit_Theta_1n = n_hit_1n_3v.Theta() * 180 / pi, n_hit_Phi_1n = n_hit_1n_3v.Phi() * 180 / pi;

                if ((n_detlayer_1n == clas12::ECIN) && (e_1n->cal(clas12::ECIN)->getZ() != 0)) {
                    e_hit_1n_3v.SetXYZ(e_1n->cal(clas12::ECIN)->getX(), e_1n->cal(clas12::ECIN)->getY(), e_1n->cal(clas12::ECIN)->getZ());
                    e_hit_Theta_1n = e_hit_1n_3v.Theta() * 180 / pi, e_hit_Phi_1n = e_hit_1n_3v.Phi() * 180 / pi;
                } else if ((n_detlayer_1n == clas12::ECOUT) && (e_1n->cal(clas12::ECOUT)->getZ() != 0)) {
                    e_hit_1n_3v.SetXYZ(e_1n->cal(clas12::ECOUT)->getX(), e_1n->cal(clas12::ECOUT)->getY(), e_1n->cal(clas12::ECOUT)->getZ());
                    e_hit_Theta_1n = e_hit_1n_3v.Theta() * 180 / pi, e_hit_Phi_1n = e_hit_1n_3v.Phi() * 180 / pi;
                } else {
                    int trajlayer = (n_detlayer_1n == clas12::ECIN) ? 4 : 7;
                    e_hit_1n_3v.SetXYZ(e_1n->traj(clas12::ECAL, trajlayer)->getX(), e_1n->traj(clas12::ECAL, trajlayer)->getY(), e_1n->traj(clas12::ECAL, trajlayer)->getZ());
                    e_hit_Theta_1n = e_hit_1n_3v.Theta() * 180 / pi, e_hit_Phi_1n = e_hit_1n_3v.Phi() * 180 / pi;
                }

                dPhi_hit_1n = CalcdPhi(n_hit_Phi_1n - e_hit_Phi_1n), dTheta_hit_1n = n_hit_Theta_1n - e_hit_Theta_1n;

                // subtracting the angles between the neutron hit and electron hit to see if we have fake neutrons:
                hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n.hFill(dPhi_hit_1n, dTheta_hit_1n, Weight_1n);
            }  // end of if neutron did not hit PCAL & hit either ECIN or ECOUT

            bool NeutronPassVeto_1n = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, n_ind_1n, Neutron_veto_cut.GetLowerCut());

            /* Log vetoed neutron values (for self-consistency) */
            if (!NeutronPassVeto_1n) { hdTheta_n_e_VS_dPhi_n_e_Electrons_Vetoed_Neutrons_1n.hFill(dPhi_hit_1n, dTheta_hit_1n, Weight_1n); }

            // Setting kinematical cuts ---------------------------------------------------------------------------------------------------------------------------------

            // Setting kinematical cuts
            /* Good neutrons are within momentum kin cuts (below l. KC -> efficiency; above u. KC -> beta fit) -> momentum kin cut before neutron shifting */
            /* We want to compare FD neutrons with FD protons in the same momentum region -> additional momentum kin cut after neutron shifting */
            /* Fiducial cuts -> after neutron shifting; because we want to match the corrected neutron momentum to the proton maps! */
            bool FD_Theta_Cut_1n = ((P_n_1n_3v.Theta() * 180.0 / pi) <= FD_nucleon_theta_cut.GetUpperCut());
            bool FD_Momentum_Cut_BS_1n =
                ((NeutronMomBKC_1n <= FD_nucleon_momentum_cut.GetUpperCut()) && (NeutronMomBKC_1n >= FD_nucleon_momentum_cut.GetLowerCut()));  // Momentum kin cut before neutron shifting
            bool FD_Momentum_Cut_AS_1n = ((P_n_1n_3v.Mag() <= FD_nucleon_momentum_cut.GetUpperCut()) && (P_n_1n_3v.Mag() >= FD_nucleon_momentum_cut.GetLowerCut()));
            // Additional momentum kin cut after neutron shifting
            bool e_withinFC_1n = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Electron", P_e_1n_3v.Mag(), P_e_1n_3v.Theta() * 180.0 / pi,
                                                          P_e_1n_3v.Phi() * 180.0 / pi);
            bool n_withinFC_1n = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Neutron", P_n_1n_3v.Mag(), P_n_1n_3v.Theta() * 180.0 / pi,
                                                          P_n_1n_3v.Phi() * 180.0 / pi, Calc_eff_overlapping_FC);

            bool Pass_Kin_Cuts_1n =
                ((!apply_kinematical_cuts || (FD_Theta_Cut_1n && FD_Momentum_Cut_BS_1n && FD_Momentum_Cut_AS_1n)) && (!apply_fiducial_cuts || (e_withinFC_1n && n_withinFC_1n)));

            // Fillings 1n histograms -----------------------------------------------------------------------------------------------------------------------------------

            // Applying neutron veto and Fillings 1n histograms

            if (NeutronPassVeto_1n && Pass_Kin_Cuts_1n) {
                ++num_of_events_1n_inFD_AV;

                // Filling cut variable plots (1n)
                /* Filling Nphe plots (1n) */
                hNphe_1n_FD.hFill(e_1n->che(clas12::HTCC)->getNphe(), Weight_1n);

                /* Filling electrton Chi2 histograms (1n) */
                hChi2_Electron_1n_FD.hFill(e_1n->par()->getChi2Pid(), Weight_1n);

                /* Filling dVx, dVy, dVz (1n) */
                // TODO: recheck with Adi rather or not to remove these plots
                if (ES_by_leading_FDneutron) {
                    double Vx_n_1n = allParticles[NeutronsFD_ind_mom_max]->par()->getVx();
                    double Vy_n_1n = allParticles[NeutronsFD_ind_mom_max]->par()->getVy();
                    double Vz_n_1n = allParticles[NeutronsFD_ind_mom_max]->par()->getVz();
                    double dVx = Vx_n_1n - Vx_e_1n, dVy = Vy_n_1n - Vy_e_1n, dVz = Vz_n_1n - Vz_e_1n;

                    hdVx_1n.hFill(dVx, Weight_1n), hdVy_1n.hFill(dVy, Weight_1n), hdVz_1n.hFill(dVz, Weight_1n);
                } else {
                    for (auto &i : NeutronsFD_ind) {
                        double Vx_n_1n = allParticles[i]->par()->getVx(), Vy_n_1n = allParticles[i]->par()->getVy(), Vz_n_1n = allParticles[i]->par()->getVz();
                        double dVx = Vx_n_1n - Vx_e_1n, dVy = Vy_n_1n - Vy_e_1n, dVz = Vz_n_1n - Vz_e_1n;

                        hdVx_1n.hFill(dVx, Weight_1n), hdVy_1n.hFill(dVy, Weight_1n), hdVz_1n.hFill(dVz, Weight_1n);
                    }  // end of loop over neutrons vector
                }

                /* Filling SF histograms (1n) */
                hSF_1n_FD.hFill(EoP_e_1n, Weight_1n), hSF_VS_P_e_1n_FD.hFill(P_e_1n_3v.Mag(), EoP_e_1n, Weight_1n);

                /* Filling fiducial plots (1n) */
                hVcal_VS_EoP_1n_PCAL.hFill(e_1n->cal(clas12::PCAL)->getLv(), EoP_e_1n, Weight_1n);
                hWcal_VS_EoP_1n_PCAL.hFill(e_1n->cal(clas12::PCAL)->getLw(), EoP_e_1n, Weight_1n);

                // Filling momentum threshold histograms (1n)
                /* Filling momentum histograms (1n) */

                // Electron momentum (1n)
                hP_e_APID_1n_FD.hFill(P_e_1n_3v.Mag(), Weight_1n);  // after mom. th.

                for (int i = 0; i < Ne; i++) {
                    if (electrons[i]->getRegion() == FD) { hP_e_BPID_1n_FD.hFill(P_e_1n_3v.Mag(), Weight_1n); }  // before mom. th.
                }

                // Neutron momentum (1n)

                // Neutron momentum - all contributions (1n)
                /* FD Neutrons after mom. th. */
                for (int &i : NeutronsFD_ind) {
                    hP_n_APID_1n_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_1n);
                    hP_n_APID_1n_ZOOMOUT_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_1n);
                    hP_n_APIDandNS_1n_FD.hFill(nRes.NCorr(apply_nucleon_SmearAndCorr, pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts)), Weight_1n);
                    hP_n_APIDandNS_1n_ZOOMOUT_FD.hFill(nRes.NCorr(apply_nucleon_SmearAndCorr, pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts)), Weight_1n);
                }

                /* FD Neutrons before mom. th. */
                for (int &i : ReDef_neutrons_FD) {
                    hP_n_BPID_1n_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_1n);
                    hP_n_BPID_1n_ZOOMOUT_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_1n);
                }

                // Neutron momentum - verified neutrons (1n)
                /* Neutron mom. before th. (verified neutrons) */
                for (int i = 0; i < neutrons.size(); i++) {
                    bool inPCALtmp = (neutrons[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                    bool inECINtmp = (neutrons[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                    bool inECOUTtmp = (neutrons[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                    if ((neutrons[i]->getRegion() == FD) && (!inPCALtmp && (inECINtmp || inECOUTtmp))) {
                        hP_n_VN_BPID_1n_FD.hFill(neutrons[i]->getP(), Weight_1n);  // before mom. th.
                    }
                }

                /* Neutron mom. after th. (verified neutrons) */
                for (int &i : NeutronsFD_ind) {
                    int ParticlePDGtmp = allParticles[i]->par()->getPid();

                    bool inPCALtmp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                    bool inECINtmp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                    bool inECOUTtmp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                    if ((allParticles[i]->getRegion() == FD) && (ParticlePDGtmp == 2112) && (!inPCALtmp && (inECINtmp || inECOUTtmp))) {
                        hP_n_VN_APID_1n_FD.hFill(allParticles[i]->getP(), Weight_1n);  // after mom. th.
                    }
                }

                // Neutron momentum - from 'photons' (1n)
                /* Neutron mom before cuts (from 'photons') */
                for (int i = 0; i < allParticles.size(); i++) {
                    int ParticlePDGtmp = allParticles[i]->par()->getPid();

                    bool inPCALtmp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                    bool inECINtmp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                    bool inECOUTtmp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                    // 'photon' mom before cuts:
                    if ((allParticles[i]->getRegion() == FD) && (ParticlePDGtmp == 22) && (!inPCALtmp && (inECINtmp || inECOUTtmp))) {
                        hP_n_Ph_BPID_1n_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_1n);  // before mom. th.
                    }
                }

                /* Neutron mom after cuts (from 'photons') */
                for (int &i : NeutronsFD_ind) {
                    int ParticlePDGtmp = allParticles[i]->par()->getPid();

                    bool inPCALtmp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                    bool inECINtmp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                    bool inECOUTtmp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                    if ((allParticles[i]->getRegion() == FD) && (ParticlePDGtmp == 22) && (!inPCALtmp && (inECINtmp || inECOUTtmp))) {
                        hP_n_Ph_APID_1n_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_1n);  // after mom. th.
                    }
                }

                // Proton momentum (1n)
                for (int &i : Protons_ind) {
                    if (protons[i]->getRegion() == CD) {
                        hP_p_APID_1n_CD.hFill(protons[i]->getP(), Weight_1n);  // after mom. th.
                    } else if (protons[i]->getRegion() == FD) {
                        hP_p_APID_1n_FD.hFill(protons[i]->getP(), Weight_1n);  // after mom. th.
                    }
                }

                for (int i = 0; i < Np; i++) {
                    if (protons[i]->getRegion() == CD) {
                        hP_p_BPID_1n_CD.hFill(protons[i]->getP(), Weight_1n);  // before mom cuts
                    } else if (protons[i]->getRegion() == FD) {
                        hP_p_BPID_1n_FD.hFill(protons[i]->getP(), Weight_1n);  // before mom cuts
                    }
                }

                // Piplus momentum (1n)
                for (int &i : Piplus_ind) {
                    if (piplus[i]->getRegion() == CD) {
                        hP_piplus_APID_1n_CD.hFill(piplus[i]->getP(), Weight_1n);  // after mom. th.
                    } else if (piplus[i]->getRegion() == FD) {
                        hP_piplus_APID_1n_FD.hFill(piplus[i]->getP(), Weight_1n);  // after mom. th.
                    }
                }

                for (int i = 0; i < Npip; i++) {
                    if (piplus[i]->getRegion() == CD) {
                        hP_piplus_BPID_1n_CD.hFill(piplus[i]->getP(), Weight_1n);  // before mom cuts
                    } else if (piplus[i]->getRegion() == FD) {
                        hP_piplus_BPID_1n_FD.hFill(piplus[i]->getP(), Weight_1n);  // before mom cuts
                    }
                }

                // Piminus momentum (1n)
                for (int &i : Piminus_ind) {
                    if (piminus[i]->getRegion() == CD) {
                        hP_piminus_APID_1n_CD.hFill(piminus[i]->getP(), Weight_1n);  // after mom. th.
                    } else if (piminus[i]->getRegion() == FD) {
                        hP_piminus_APID_1n_FD.hFill(piminus[i]->getP(), Weight_1n);  // after mom. th.
                    }
                }

                for (int i = 0; i < Npim; i++) {
                    if (piminus[i]->getRegion() == CD) {
                        hP_piminus_BPID_1n_CD.hFill(piminus[i]->getP(), Weight_1n);  // before mom cuts
                    } else if (piminus[i]->getRegion() == FD) {
                        hP_piminus_BPID_1n_FD.hFill(piminus[i]->getP(), Weight_1n);  // before mom cuts
                    }
                }

                // Photon momentum (1n)
                for (int &i : PhotonsFD_ind) { hP_ph_APID_1n_FD.hFill(allParticles[i]->getP(), Weight_1n); }  // after mom. th.

                for (int &i : ReDef_photons_FD) { hP_ph_BPID_1n_FD.hFill(allParticles[i]->getP(), Weight_1n); }  // before mom. th.

                // Filling Beta plots (1n)

                // Fill Beta plots (1n, FD only)

                // Beta plots for neutrons from 'photons' (1n, FD)
                if (!ES_by_leading_FDneutron) {
                    for (int &i : NeutronsFD_ind) {
                        int PDGtmp = allParticles[i]->par()->getPid();
                        double P_n_temp = pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

                        bool inPCALtmp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                        bool inECINtmp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                        bool inECOUTtmp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                        if (PDGtmp == 22) {
                            if (!(allParticles[i]->getRegion() == FD)) { cout << "\033[33m\n\nBeta_n_1n: neutron is not in FD! Exiting...\n\n", exit(0); }
                            if (!(!inPCALtmp && (inECINtmp || inECOUTtmp))) { cout << "\033[33m\n\nBeta_n_1n: photon is not a neutron! Exiting...\n\n", exit(0); }

                            // Filling beta of neutrons from 'photons' - all sectors
                            hBeta_n_from_ph_01_1n_FD.hFill(allParticles[i]->par()->getBeta());
                            hBeta_n_from_ph_01_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                            hBeta_vs_P_1n_Neutrons_Only_from_photons_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_1n);

                            // Beta_n_from_ph - !PCAL
                            if (!inPCALtmp) {
                                hBeta_n_from_ph_02_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_n_from_ph_02_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                            }

                            // Beta_n_from_ph - !PCAL && ECIN
                            if (!inPCALtmp && inECINtmp) {
                                hBeta_n_from_ph_03_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_n_from_ph_03_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                            }

                            // Beta_n_from_ph - !PCAL && !ECIN && ECOUT
                            if (!inPCALtmp && !inECINtmp && inECOUTtmp) {
                                hBeta_n_from_ph_04_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_n_from_ph_04_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                            }
                        }
                    }
                } else {
                    /* Fill beta plots for leading FD neutron event selection */
                    int PDGtmp = allParticles[n_ind_1n]->par()->getPid();
                    double P_n_temp = pid.GetFDNeutronP(allParticles[n_ind_1n], apply_nucleon_cuts);

                    bool inPCALtmp = (allParticles[n_ind_1n]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                    bool inECINtmp = (allParticles[n_ind_1n]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                    bool inECOUTtmp = (allParticles[n_ind_1n]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

                    if (PDGtmp == 22) {
                        if (!(allParticles[n_ind_1n]->getRegion() == FD)) { cout << "\033[33m\n\nBeta_n_1n: neutron is not in FD! Exiting...\n\n", exit(0); }
                        if (!(!inPCALtmp && (inECINtmp || inECOUTtmp))) { cout << "\033[33m\n\nBeta_n_1n: photon is not a neutron! Exiting...\n\n", exit(0); }

                        // Filling beta of neutrons from 'photons' - all sectors
                        hBeta_n_from_ph_01_1n_FD.hFill(allParticles[n_ind_1n]->par()->getBeta());
                        hBeta_n_from_ph_01_1n_ZOOMOUT_FD.hFill(allParticles[n_ind_1n]->par()->getBeta());
                        hBeta_vs_P_1n_Neutrons_Only_from_photons_FD.hFill(P_n_temp, allParticles[n_ind_1n]->par()->getBeta(), Weight_1n);

                        // Beta_n_from_ph - !PCAL
                        if (!inPCALtmp) {
                            hBeta_n_from_ph_02_1n_FD.hFill(allParticles[n_ind_1n]->par()->getBeta());
                            hBeta_n_from_ph_02_1n_ZOOMOUT_FD.hFill(allParticles[n_ind_1n]->par()->getBeta());
                        }

                        // Beta_n_from_ph - !PCAL && ECIN
                        if (!inPCALtmp && inECINtmp) {
                            hBeta_n_from_ph_03_1n_FD.hFill(allParticles[n_ind_1n]->par()->getBeta());
                            hBeta_n_from_ph_03_1n_ZOOMOUT_FD.hFill(allParticles[n_ind_1n]->par()->getBeta());
                        }

                        // Beta_n_from_ph - !PCAL && !ECIN && ECOUT
                        if (!inPCALtmp && !inECINtmp && inECOUTtmp) {
                            hBeta_n_from_ph_04_1n_FD.hFill(allParticles[n_ind_1n]->par()->getBeta());
                            hBeta_n_from_ph_04_1n_ZOOMOUT_FD.hFill(allParticles[n_ind_1n]->par()->getBeta());
                        }
                    }
                }

                // Beta vs. P from identified particles

                // Beta vs. P from identified electrons (1n, FD)
                /* loop over Electron_ind, so that, is Electron_ind.size() != 1, you'll see what thw other electrons have */
                for (int &i : Electron_ind) {
                    double P_e_temp = electrons[i]->getP();

                    if (electrons[i]->getRegion() == FD) {
                        hBeta_vs_P_1n_FD.hFill(P_e_temp, electrons[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Electrons_Only_FD.hFill(P_e_temp, electrons[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_neg_part_1n_FD.hFill(P_e_temp, electrons[i]->par()->getBeta(), Weight_1n);
                    }
                }  // end of loop over identified electrons vector

                // Beta vs. P from identified neutrons (1n, FD)
                for (int &i : NeutronsFD_ind) {
                    double P_n_temp = pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

                    if (allParticles[i]->getRegion() == CD) {
                        hBeta_vs_P_1n_CD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Neutrons_Only_CD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_neut_part_1n_CD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                    } else if (allParticles[i]->getRegion() == FD) {
                        hBeta_vs_P_1n_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Neutrons_Only_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Neutrons_Only_ZOOMOUT_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_neut_part_1n_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                    }
                }  // end of loop over identified neutrons vector

                // Beta vs. P from identified photons (1n, FD)
                for (int &i : PhotonsFD_ind) {
                    double P_ph_temp = allParticles[i]->getP();

                    if (allParticles[i]->getRegion() == CD) {
                        hBeta_vs_P_1n_CD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Photons_Only_CD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_neut_part_1n_CD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                    } else if (allParticles[i]->getRegion() == FD) {
                        hBeta_vs_P_1n_FD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Photons_Only_FD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_neut_part_1n_FD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_1n);
                    }
                }  // end of loop over identified photons vector

                // Beta vs. P from identified protons (1n, FD)
                for (int &i : Protons_ind) {
                    double P_p_temp = protons[i]->getP();

                    if (protons[i]->getRegion() == CD) {
                        hBeta_vs_P_1n_CD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Protons_Only_CD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_pos_part_1n_CD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_1n);
                    } else if (protons[i]->getRegion() == FD) {
                        hBeta_vs_P_1n_FD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Protons_Only_FD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_pos_part_1n_FD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_1n);
                    }
                }  // end of loop over identified protons vector

                // Beta vs. P from identified piplus (1n, FD)
                for (int &i : Piplus_ind) {
                    double P_pip_temp = piplus[i]->getP();

                    if (piplus[i]->getRegion() == CD) {
                        hBeta_vs_P_1n_CD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Piplus_Only_CD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_pos_part_1n_CD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_1n);
                    } else if (piplus[i]->getRegion() == FD) {
                        hBeta_vs_P_1n_FD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Piplus_Only_FD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_pos_part_1n_FD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_1n);
                    }
                }  // end of loop over identified piplus vector

                // Beta vs. P from identified piminus (1n, FD)
                for (int &i : Piminus_ind) {
                    double P_pim_temp = piminus[i]->getP();

                    if (piminus[i]->getRegion() == CD) {
                        hBeta_vs_P_1n_CD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Piminus_Only_CD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_neg_part_1n_CD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_1n);
                    } else if (piminus[i]->getRegion() == FD) {
                        hBeta_vs_P_1n_FD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Piminus_Only_FD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_neg_part_1n_FD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_1n);
                    }
                }  // end of loop over identified piminus vector

                // Beta vs. P from other particles (1n, FD)
                /* This is for self-consistency. Contributions from other particles should be zero */

                // Beta vs. P from Kplus (1n, FD)
                for (int i = 0; i < Kplus.size(); i++) {
                    double P_Kp_temp = Kplus[i]->getP();

                    if (Kplus[i]->getRegion() == CD) {
                        hBeta_vs_P_1n_CD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Kplus_Only_CD.hFill(P_Kp_temp, piplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_pos_part_1n_CD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight_1n);
                    } else if (Kplus[i]->getRegion() == FD) {
                        hBeta_vs_P_1n_FD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Kplus_Only_FD.hFill(P_Kp_temp, piplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_pos_part_1n_FD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight_1n);
                    }
                }  // end of loop over Kplus vector

                // Beta vs. P from Kminus (1n, FD)
                for (int i = 0; i < Kminus.size(); i++) {
                    double P_Km_temp = Kminus[i]->getP();

                    if (Kminus[i]->getRegion() == CD) {
                        hBeta_vs_P_1n_CD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Kminus_Only_CD.hFill(P_Km_temp, piplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_neg_part_1n_CD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight_1n);
                    } else if (Kminus[i]->getRegion() == FD) {
                        hBeta_vs_P_1n_FD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_1n_Kminus_Only_FD.hFill(P_Km_temp, piplus[i]->par()->getBeta(), Weight_1n);
                        hBeta_vs_P_neg_part_1n_FD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight_1n);
                    }
                }  // end of loop over Kminus vector

                // Filling electron plots (1n)
                hTheta_e_All_Int_1n_FD->Fill(Theta_e_1n, Weight_1n);
                hPhi_e_All_Int_1n_FD->Fill(Phi_e_1n, Weight_1n);
                hPhi_e_VS_P_e_1n_FD->Fill(P_e_1n_3v.Mag(), Phi_e_1n, Weight_1n);
                hPhi_e_VS_W_1n_FD->Fill(W_1n, Phi_e_1n, Weight_1n);
                hTheta_e_VS_Phi_e_1n_FD->Fill(Phi_e_1n, Theta_e_1n, Weight_1n);
                hTheta_e_VS_P_e_1n_FD->Fill(P_e_1n_3v.Mag(), Theta_e_1n, Weight_1n);
                hTheta_e_VS_W_1n_FD->Fill(W_1n, Theta_e_1n, Weight_1n);

                hE_e_All_Int_1n_FD->Fill(E_e_1n, Weight_1n);
                hE_e_VS_Theta_e_All_Int_1n_FD->Fill(Theta_e_1n, E_e_1n, Weight_1n);

                hET_All_Ang_All_Int_1n_FD->Fill(beamE - E_e_1n, Weight_1n);

                if ((Theta_e_1n >= 14.0) && (Theta_e_1n <= 16.0)) {
                    hET15_All_Int_1n_FD->Fill(beamE - E_e_1n, Weight_1n);
                    hE_e_15_All_Int_1n_FD->Fill(E_e_1n, Weight_1n);
                }

                if (qel) {
                    hTheta_e_QEL_1n_FD->Fill(Theta_e_1n, Weight_1n);
                    hPhi_e_QEL_1n_FD->Fill(Phi_e_1n, Weight_1n);
                    hE_e_QEL_1n_FD->Fill(E_e_1n, Weight_1n);
                    hE_e_VS_Theta_e_QEL_1n_FD->Fill(Theta_e_1n, E_e_1n, Weight_1n);

                    hET_All_Ang_QEL_1n_FD->Fill(beamE - E_e_1n, Weight_1n);

                    if ((Theta_e_1n >= 14.0) && (Theta_e_1n <= 16.0)) {
                        hET15_QEL_1n_FD->Fill(beamE - E_e_1n, Weight_1n);
                        hE_e_15_QEL_1n_FD->Fill(E_e_1n, Weight_1n);
                    }
                } else if (mec) {
                    hTheta_e_MEC_1n_FD->Fill(Theta_e_1n, Weight_1n);
                    hPhi_e_MEC_1n_FD->Fill(Phi_e_1n, Weight_1n);
                    hE_e_MEC_1n_FD->Fill(E_e_1n, Weight_1n);
                    hE_e_VS_Theta_e_MEC_1n_FD->Fill(Theta_e_1n, E_e_1n, Weight_1n);

                    hET_All_Ang_MEC_1n_FD->Fill(beamE - E_e_1n, Weight_1n);

                    if ((Theta_e_1n >= 14.0) && (Theta_e_1n <= 16.0)) {
                        hET15_MEC_1n_FD->Fill(beamE - E_e_1n, Weight_1n);
                        hE_e_15_MEC_1n_FD->Fill(E_e_1n, Weight_1n);
                    }
                } else if (res) {
                    hTheta_e_RES_1n_FD->Fill(Theta_e_1n, Weight_1n);
                    hPhi_e_RES_1n_FD->Fill(Phi_e_1n, Weight_1n);
                    hE_e_RES_1n_FD->Fill(E_e_1n, Weight_1n);
                    hE_e_VS_Theta_e_RES_1n_FD->Fill(Theta_e_1n, E_e_1n, Weight_1n);

                    hET_All_Ang_RES_1n_FD->Fill(beamE - E_e_1n, Weight_1n);

                    if ((Theta_e_1n >= 14.0) && (Theta_e_1n <= 16.0)) {
                        hET15_RES_1n_FD->Fill(beamE - E_e_1n, Weight_1n);
                        hE_e_15_RES_1n_FD->Fill(E_e_1n, Weight_1n);
                    }
                } else if (dis) {
                    hTheta_e_DIS_1n_FD->Fill(Theta_e_1n, Weight_1n);
                    hPhi_e_DIS_1n_FD->Fill(Phi_e_1n, Weight_1n);
                    hE_e_DIS_1n_FD->Fill(E_e_1n, Weight_1n);
                    hE_e_VS_Theta_e_DIS_1n_FD->Fill(Theta_e_1n, E_e_1n, Weight_1n);

                    hET_All_Ang_DIS_1n_FD->Fill(beamE - E_e_1n, Weight_1n);

                    if ((Theta_e_1n >= 14.0) && (Theta_e_1n <= 16.0)) {
                        hET15_DIS_1n_FD->Fill(beamE - E_e_1n, Weight_1n);
                        hE_e_15_DIS_1n_FD->Fill(E_e_1n, Weight_1n);
                    }
                }

                // Fill momentum transfer plots (1n)
                FillByInt(hQ2_1n, hQ2_QEL_1n, hQ2_MEC_1n, hQ2_RES_1n, hQ2_DIS_1n, qel, mec, res, dis, Q2_1n, Weight_1n);
                FillByInt(hq_3v_1n, hq_3v_QEL_1n, hq_3v_MEC_1n, hq_3v_RES_1n, hq_3v_DIS_1n, qel, mec, res, dis, q_1n_3v.Mag(), Weight_1n);

                hQ2_VS_W_1n->Fill(W_1n, Q2_1n, Weight_1n);
                hQ2_VS_q_3v_1n->Fill(q_1n_3v.Mag(), Q2_1n, Weight_1n);
                hQ2_VS_omega_1n->Fill(omega_1n, Q2_1n, Weight_1n);
                hq_3v_VS_omega_1n->Fill(omega_1n, q_1n_3v.Mag(), Weight_1n);

                if (qel) {
                    hQ2_VS_W_QEL_1n->Fill(W_1n, Q2_1n, Weight_1n);
                    hQ2_VS_q_3v_QEL_1n->Fill(q_1n_3v.Mag(), Q2_1n, Weight_1n);
                    hQ2_VS_omega_QEL_1n->Fill(omega_1n, Q2_1n, Weight_1n);
                    hq_3v_VS_omega_QEL_1n->Fill(omega_1n, q_1n_3v.Mag(), Weight_1n);
                } else if (mec) {
                    hQ2_VS_W_MEC_1n->Fill(W_1n, Q2_1n, Weight_1n);
                    hQ2_VS_q_3v_MEC_1n->Fill(q_1n_3v.Mag(), Q2_1n, Weight_1n);
                    hQ2_VS_omega_MEC_1n->Fill(omega_1n, Q2_1n, Weight_1n);
                    hq_3v_VS_omega_MEC_1n->Fill(omega_1n, q_1n_3v.Mag(), Weight_1n);
                } else if (res) {
                    hQ2_VS_W_RES_1n->Fill(W_1n, Q2_1n, Weight_1n);
                    hQ2_VS_q_3v_RES_1n->Fill(q_1n_3v.Mag(), Q2_1n, Weight_1n);
                    hQ2_VS_omega_RES_1n->Fill(omega_1n, Q2_1n, Weight_1n);
                    hq_3v_VS_omega_RES_1n->Fill(omega_1n, q_1n_3v.Mag(), Weight_1n);
                } else if (dis) {
                    hQ2_VS_W_DIS_1n->Fill(W_1n, Q2_1n, Weight_1n);
                    hQ2_VS_q_3v_DIS_1n->Fill(q_1n_3v.Mag(), Q2_1n, Weight_1n);
                    hQ2_VS_omega_DIS_1n->Fill(omega_1n, Q2_1n, Weight_1n);
                    hq_3v_VS_omega_DIS_1n->Fill(omega_1n, q_1n_3v.Mag(), Weight_1n);
                }

                // Filling Ecal plots (1n)
                Ecal_1n = E_e_1n + (E_n_1n - m_n);

                hEcal_All_Int_1n->Fill(Ecal_1n, Weight_1n);  // Fill Ecal for all interactions

                if (qel) {
                    hEcal_QEL_1n->Fill(Ecal_1n, Weight_1n);  // Fill Ecal for QEL only
                } else if (mec) {
                    hEcal_MEC_1n->Fill(Ecal_1n, Weight_1n);  // Fill Ecal for MEC only
                } else if (res) {
                    hEcal_RES_1n->Fill(Ecal_1n, Weight_1n);  // Fill Ecal for RES only
                } else if (dis) {
                    hEcal_DIS_1n->Fill(Ecal_1n, Weight_1n);  // Fill Ecal for DIS only
                }

                if (Ecal_1n > beamE) {
                    hEcal_vs_P_e_test_1n->Fill(P_e_1n_3v.Mag(), Ecal_1n, Weight_1n);
                    hEcal_vs_Theta_e_test_1n->Fill(Theta_e_1n, Ecal_1n, Weight_1n);
                    hEcal_vs_Phi_e_test_1n->Fill(Phi_e_1n, Ecal_1n, Weight_1n);
                    hEcal_vs_P_n_test_1n->Fill(P_n_1n_3v.Mag(), Ecal_1n, Weight_1n);
                    hEcal_vs_Theta_n_test_1n->Fill(Theta_n_1n, Ecal_1n, Weight_1n);
                    hEcal_vs_Phi_n_test_1n->Fill(Phi_n_1n, Ecal_1n, Weight_1n);
                }

                // Fill neutron multiplicity plots (1n)
                pid.FillNeutMultiPlots(allParticles, electrons, Weight_1n, beamE, Neutron_veto_cut.GetLowerCutConst(), hNeut_Multi_By_Redef_BPID_BV_1n_FD, hNeut_Multi_By_Redef_BPID_AV_1n_FD,
                                       ReDef_neutrons_FD, hNeut_Multi_By_Redef_APID_BV_1n_FD, hNeut_Multi_By_Redef_APID_AV_1n_FD, NeutronsFD_ind);

                // Fill W (1n)
                FillByInt(hW_All_Int_1n, hW_QEL_1n, hW_MEC_1n, hW_RES_1n, hW_DIS_1n, qel, mec, res, dis, W_1n, Weight_1n);
                hW_VS_q_3v_1n->Fill(W_1n, q_1n_3v.Mag(), Weight_1n);
                hW_VS_omega_1n->Fill(W_1n, omega_1n, Weight_1n);

                if (qel) {
                    hW_VS_q_3v_QEL_1n->Fill(W_1n, q_1n_3v.Mag(), Weight_1n);
                    hW_VS_omega_QEL_1n->Fill(W_1n, omega_1n, Weight_1n);
                } else if (mec) {
                    hW_VS_q_3v_MEC_1n->Fill(W_1n, q_1n_3v.Mag(), Weight_1n);
                    hW_VS_omega_MEC_1n->Fill(W_1n, omega_1n, Weight_1n);
                } else if (res) {
                    hW_VS_q_3v_RES_1n->Fill(W_1n, q_1n_3v.Mag(), Weight_1n);
                    hW_VS_omega_RES_1n->Fill(W_1n, omega_1n, Weight_1n);
                } else if (dis) {
                    hW_VS_q_3v_DIS_1n->Fill(W_1n, q_1n_3v.Mag(), Weight_1n);
                    hW_VS_omega_DIS_1n->Fill(W_1n, omega_1n, Weight_1n);
                }

                hP_nFD_APID_1n.hFill(NeutronMomBKC_1n, Weight_1n);              // Leading FD neutron (1n)
                hP_nFD_APID_1n_ZOOMOUT.hFill(NeutronMomBKC_1n, Weight_1n);      // Leading FD neutron (1n)
                hP_nFD_APIDandNS_1n.hFill(P_n_1n_3v.Mag(), Weight_1n);          // Leading FD neutron after shifting (1n)
                hP_nFD_APIDandNS_1n_ZOOMOUT.hFill(P_n_1n_3v.Mag(), Weight_1n);  // Leading FD neutron after shifting (1n)
                hP_nFD_APIDandNS_VS_W_1n->Fill(W_1n, P_n_1n_3v.Mag(), Weight_1n);
                hP_nFD_vs_P_e_1n.hFill(P_n_1n_3v.Mag(), P_e_1n_3v.Mag(), Weight_1n);
                hP_nFD_vs_P_e_ZoomOut_1n.hFill(P_n_1n_3v.Mag(), P_e_1n_3v.Mag(), Weight_1n);
                hP_nFD_vs_theta_e_1n.hFill(P_n_1n_3v.Mag(), Theta_e_1n, Weight_1n);
                hP_nFD_vs_theta_e_ZoomOut_1n.hFill(P_n_1n_3v.Mag(), Theta_e_1n, Weight_1n);
                hP_nFD_vs_phi_e_1n.hFill(P_n_1n_3v.Mag(), Phi_e_1n, Weight_1n);
                hP_nFD_vs_phi_e_ZoomOut_1n.hFill(P_n_1n_3v.Mag(), Phi_e_1n, Weight_1n);
                htheta_nFD_vs_P_e_1n.hFill(Theta_n_1n, P_e_1n_3v.Mag(), Weight_1n);
                htheta_nFD_vs_theta_e_1n.hFill(Theta_n_1n, Theta_e_1n, Weight_1n);
                htheta_nFD_vs_phi_e_1n.hFill(Theta_n_1n, Phi_e_1n, Weight_1n);
                hphi_nFD_vs_P_e_1n.hFill(Phi_n_1n, P_e_1n_3v.Mag(), Weight_1n);
                hphi_nFD_vs_theta_e_1n.hFill(Phi_n_1n, Theta_e_1n, Weight_1n);
                hphi_nFD_vs_phi_e_1n.hFill(Phi_n_1n, Phi_e_1n, Weight_1n);

                if (qel) {
                    hP_nFD_APIDandNS_VS_W_QEL_1n->Fill(W_1n, P_n_1n_3v.Mag(), Weight_1n);
                } else if (mec) {
                    hP_nFD_APIDandNS_VS_W_MEC_1n->Fill(W_1n, P_n_1n_3v.Mag(), Weight_1n);
                } else if (res) {
                    hP_nFD_APIDandNS_VS_W_RES_1n->Fill(W_1n, P_n_1n_3v.Mag(), Weight_1n);
                } else if (dis) {
                    hP_nFD_APIDandNS_VS_W_DIS_1n->Fill(W_1n, P_n_1n_3v.Mag(), Weight_1n);
                }

                dP_T_1n_3v = TVector3(P_T_e_1n_3v.Px() + P_T_n_1n_3v.Px(), P_T_e_1n_3v.Py() + P_T_n_1n_3v.Py(), 0);
                dAlpha_T_1n = acos(-(P_e_1n_3v.Px() * dP_T_1n_3v.Px() + P_e_1n_3v.Py() * dP_T_1n_3v.Py() + P_e_1n_3v.Pz() * dP_T_1n_3v.Pz()) / (P_T_e_1n_3v.Mag() * dP_T_1n_3v.Mag())) *
                              180.0 / pi;  // dP_T_1n_3v.Pz() = 0; dAlpha_T_1n in deg
                hdP_T_1n->Fill(dP_T_1n_3v.Mag(), Weight_1n);
                hdAlpha_T_1n->Fill(dAlpha_T_1n, Weight_1n);
                hdP_T_vs_dAlpha_T_1n->Fill(dAlpha_T_1n, dP_T_1n_3v.Mag(), Weight_1n);

                dPhi_T_1n =
                    acos(-(P_T_e_1n_3v.Px() * P_T_n_1n_3v.Px() + P_T_e_1n_3v.Py() * P_T_n_1n_3v.Py() + P_T_e_1n_3v.Pz() * P_T_n_1n_3v.Pz()) / (P_T_e_1n_3v.Mag() * P_T_n_1n_3v.Mag())) *
                    180.0 / pi;  // P_T_n_1n_3v.Pz() = 0; dPhi_T_1n in deg
                hdPhi_T_1n->Fill(dPhi_T_1n, Weight_1n);

                hEcal_vs_P_e_1n->Fill(P_e_1n_3v.Mag(), Ecal_1n, Weight_1n);
                hEcal_vs_Theta_e_1n->Fill(Theta_e_1n, Ecal_1n, Weight_1n);
                hEcal_vs_Phi_e_1n->Fill(Phi_e_1n, Ecal_1n, Weight_1n);
                hEcal_vs_P_n_1n->Fill(P_n_1n_3v.Mag(), Ecal_1n, Weight_1n);
                hEcal_vs_Theta_n_1n->Fill(Theta_n_1n, Ecal_1n, Weight_1n);
                hEcal_vs_Phi_n_1n->Fill(Phi_n_1n, Ecal_1n, Weight_1n);
                hEcal_vs_dAlpha_T_1n->Fill(dAlpha_T_1n, Ecal_1n, Weight_1n);
                hEcal_vs_dP_T_1n->Fill(dP_T_1n_3v.Mag(), Ecal_1n, Weight_1n);

                hTheta_n_All_Int_1n->Fill(Theta_n_1n, Weight_1n);
                hTheta_n_VS_P_n_1n_FD->Fill(P_n_1n_3v.Mag(), Theta_n_1n, Weight_1n);
                hTheta_n_VS_W_1n_FD->Fill(W_1n, Theta_n_1n, Weight_1n);
                hPhi_n_All_Int_1n->Fill(Phi_n_1n, Weight_1n);
                hTheta_n_VS_Phi_n_1n_FD->Fill(Phi_n_1n, Theta_n_1n, Weight_1n);

                Theta_p_e_p_n_1n = acos((P_e_1n_3v.Px() * P_n_1n_3v.Px() + P_e_1n_3v.Py() * P_n_1n_3v.Py() + P_e_1n_3v.Pz() * P_n_1n_3v.Pz()) / (P_e_1n_3v.Mag() * P_n_1n_3v.Mag())) * 180.0 /
                                   pi;  // Theta_p_e_p_n_1n in deg
                hTheta_p_e_p_n_1n->Fill(Theta_p_e_p_n_1n, Weight_1n);

                Theta_q_p_n_1n = acos((q_1n_3v.Px() * P_n_1n_3v.Px() + q_1n_3v.Py() * P_n_1n_3v.Py() + q_1n_3v.Pz() * P_n_1n_3v.Pz()) / (q_1n_3v.Mag() * P_n_1n_3v.Mag())) * 180.0 /
                                 pi;  // Theta_q_p_n_1n in deg
                hTheta_q_p_n_1n->Fill(Theta_q_p_n_1n, Weight_1n);

                hTheta_q_p_n_vs_p_n_q_1n->Fill(P_n_1n_3v.Mag() / q_1n_3v.Mag(), Theta_q_p_n_1n, Weight_1n);

                P_N_1n_3v = TVector3(P_e_1n_3v.Px() + P_n_1n_3v.Px() - Pvx, P_e_1n_3v.Py() + P_n_1n_3v.Py() - Pvy, P_e_1n_3v.Pz() + P_n_1n_3v.Pz() - Pvz);
                hTheta_q_p_n_vs_p_N_q_1n->Fill(P_N_1n_3v.Mag() / q_1n_3v.Mag(), Theta_q_p_n_1n, Weight_1n);

                hdTheta_n_e_VS_dPhi_n_e_Electrons_AV_1n.hFill(dPhi_hit_1n, dTheta_hit_1n, Weight_1n);

                // Fill resolution histograms (1n)
                if (plot_and_fit_MomRes) {
                    auto mcpbank_nRes = c12->mcparts();
                    const Int_t Ngen_nRes = mcpbank_nRes->getRows();

                    int Neutron_match_counter = 0;
                    double RecoNeutronP_Debug = P_n_1n_3v.Mag();
                    double RecoNeutronTheta_Debug = P_n_1n_3v.Theta() * 180.0 / pi;
                    double RecoNeutronPhi_Debug = P_n_1n_3v.Phi() * 180.0 / pi;

                    for (Int_t i = 0; i < Ngen_nRes; i++) {
                        mcpbank_nRes->setEntry(i);

                        /* TL neutron kinematic variables */
                        double TLNeutronP = rCalc(mcpbank_nRes->getPx(), mcpbank_nRes->getPy(), mcpbank_nRes->getPz());
                        double TLNeutronTheta = acos(mcpbank_nRes->getPz() / TLNeutronP) * 180.0 / pi;
                        double TLNeutronPhi = atan2(mcpbank_nRes->getPy(), mcpbank_nRes->getPx()) * 180.0 / pi;

                        /* Reco neutron kinematic variables */
                        double RecoNeutronP = RecoNeutronP_Debug;
                        double RecoNeutronTheta = RecoNeutronTheta_Debug;
                        double RecoNeutronPhi = RecoNeutronPhi_Debug;

                        /* TL-Reco angle difference */
                        double dNeutronTheta = TLNeutronTheta - RecoNeutronTheta;
                        double dNeutronPhi = CalcdPhi(TLNeutronPhi - RecoNeutronPhi);

                        int pid_nRes = mcpbank_nRes->getPid();
                        //                        auto pid = mcpbank_nRes->getPid();

                        // nRes cuts

                        // nRes good neutron cuts
                        bool nRes_TL_Pass_PIDCut = (pid_nRes == 2112);

                        bool Reco_InFD =
                            aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Neutron", RecoNeutronP, RecoNeutronTheta, RecoNeutronPhi, false);
                        bool TL_InFD = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Neutron", TLNeutronP, TLNeutronTheta, TLNeutronPhi, false);
                        bool nRes_Pass_FiducialCuts = (Reco_InFD && TL_InFD);

                        bool Reco_Theta_kinCut = (RecoNeutronTheta <= FD_nucleon_theta_cut.GetUpperCut());
                        bool TL_Theta_kinCuts = (TLNeutronTheta <= FD_nucleon_theta_cut.GetUpperCut());
                        bool nRes_Pass_ThetaKinCut = (Reco_Theta_kinCut && TL_Theta_kinCuts);

                        bool nRes_Reco_Pass_Neutron_MomKinCut = ((RecoNeutronP >= n_mom_th.GetLowerCut()) && (RecoNeutronP <= n_mom_th.GetUpperCut()));
                        bool nRes_TL_Pass_Neutron_MomKinCut = ((TLNeutronP >= n_mom_th.GetLowerCut()) && (TLNeutronP <= n_mom_th.GetUpperCut()));

                        // nRes matching cuts
                        double dPhiCut = 5., dThetaCut = 2.;  // TODO: add to a DSCuts variable
                        bool nRes_Pass_dThetaCut = (fabs(dNeutronTheta) < dThetaCut);
                        bool nRes_Pass_dPhiCut = (fabs(dNeutronPhi) < dPhiCut);

                        if (nRes_TL_Pass_PIDCut && nRes_Pass_FiducialCuts && nRes_Pass_ThetaKinCut && nRes_Reco_Pass_Neutron_MomKinCut && nRes_TL_Pass_Neutron_MomKinCut) {
                            /* Plots for TL neutrons passing nRes cuts */
                            hdTheta_nFD_TL_BC_1n.hFill(dNeutronTheta, Weight);
                            hdTheta_nFD_TL_ZOOMIN_BC_1n.hFill(dNeutronTheta, Weight);
                            hdPhi_nFD_TL_BC_1n.hFill(dNeutronPhi, Weight);
                            hdPhi_nFD_TL_ZOOMIN_BC_1n.hFill(dNeutronPhi, Weight);

                            hTheta_nFD_TL_AnResC_1n.hFill(TLNeutronTheta, Weight);
                            hPhi_nFD_TL_AnResC_1n.hFill(TLNeutronPhi, Weight);
                            hTheta_nFD_TL_VS_Phi_nFD_TL_AnResC_1n->Fill(TLNeutronPhi, TLNeutronTheta, Weight);

                            if (nRes_Pass_dPhiCut) {
                                /* Plots for TL neutrons passing nRes_Pass_dPhiCut */
                                hdTheta_nFD_TL_AdPC_1n.hFill(dNeutronTheta, Weight);
                                hdTheta_nFD_TL_ZOOMIN_AdPC_1n.hFill(dNeutronTheta, Weight);
                                hdPhi_nFD_TL_AdPC_1n.hFill(dNeutronPhi, Weight);
                                hdPhi_nFD_TL_ZOOMIN_AdPC_1n.hFill(dNeutronPhi, Weight);
                            }

                            if (nRes_Pass_dThetaCut && nRes_Pass_dPhiCut) {
                                /* Basic reco variables */
                                bool ECIN_HIT = (n_1n->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                                bool ECOUT_HIT = (n_1n->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
                                auto Detlayer_1n = ECIN_HIT ? clas12::ECIN : clas12::ECOUT;       // determine the earliest layer of the neutral hit

                                double t_start = c12->event()->getStartTime();  // Event start time

                                double Reco_L = n_1n->getPath();                                           // Reco neutron path
                                double Reco_beta = n_1n->par()->getBeta();                                 // Reco neutron beta
                                double Reco_t_nFD = n_1n->getTime();                                       // Reco neutron ToF 1
                                double Reco_t_ToF_from_beta = Reco_L / (c * Reco_beta);                    // Reco neutron ToF from beta
                                double Reco_calc_t_nFD_ToF = n_1n->cal(Detlayer_1n)->getTime() - t_start;  // Reco neutron ToF 2

                                /* Basic truth variables */
                                double Truth_E_nFD = sqrt(m_n * m_n + TLNeutronP * TLNeutronP);
                                double Truth_beta = TLNeutronP / Truth_E_nFD;

                                /* Effective distance */
                                double Eff_dist_TL = c * Truth_beta * Reco_t_ToF_from_beta;
                                double Eff_dist_calc = c * Truth_beta * Reco_calc_t_nFD_ToF;

                                /* Distance offset */
                                double DeltaL_TL = Eff_dist_TL - Reco_L;
                                double DeltaL_calc = Eff_dist_calc - Reco_L;

                                //                                if (fabs(DeltaL_calc) > 50.) {
                                //                                if (fabs(DeltaL_calc) < 50.) {
                                if (true) {
                                    ++Neutron_match_counter;

                                    /* Plots for TL neutrons passing matching cuts */
                                    hTheta_nFD_TL_MatchedN_1n.hFill(TLNeutronTheta, Weight);
                                    hPhi_nFD_TL_MatchedN_1n.hFill(TLNeutronPhi, Weight);
                                    hTheta_nFD_TL_VS_Phi_nFD_TL_MatchedN_1n->Fill(TLNeutronPhi, TLNeutronTheta, Weight);

                                    /* Filling nRes plots */
                                    double nResolution = (TLNeutronP - RecoNeutronP) / TLNeutronP;
                                    nRes.hFillResPlotsByType(TLNeutronP, RecoNeutronP, nResolution, Weight);

                                    hP_nFD_Res_1n.hFill(nResolution, Weight);
                                    hP_nFD_Res_VS_TL_P_nFD_1n->Fill(TLNeutronP, nResolution, Weight);
                                    hP_nFD_Res_VS_TL_P_nFD_ZOOMIN_1n->Fill(TLNeutronP, nResolution, Weight);
                                    hP_nFD_Res_VS_Reco_P_nFD_1n->Fill(NeutronMomBKC_1n, nResolution, Weight);
                                    hP_nFD_Res_VS_Reco_P_nFD_ZOOMIN_1n->Fill(NeutronMomBKC_1n, nResolution, Weight);
                                    hP_nFD_Res_VS_Corr_Reco_P_nFD_1n->Fill(RecoNeutronP, nResolution, Weight);
                                    hP_nFD_Res_VS_Corr_Reco_P_nFD_ZOOMIN_1n->Fill(RecoNeutronP, nResolution, Weight);
                                    hReco_P_nFD_nRes_1n.hFill(RecoNeutronP, Weight);
                                    hTL_P_nFD_nRes_1n.hFill(TLNeutronP, Weight);
                                    hTL_P_nFD_vs_Reco_P_nFD_1n.hFill(NeutronMomBKC_1n, RecoNeutronP, Weight);
                                    hTL_P_nFD_vs_Corr_Reco_P_nFD_1n.hFill(TLNeutronP, RecoNeutronP, Weight);

                                    // Linear fit justification plots

                                    // basic variables
                                    hReco_L_1n.hFill(Reco_L, Weight);
                                    hReco_L_VS_reco_P_nFD_1n.hFill(Reco_L, RecoNeutronP, Weight);
                                    hReco_L_VS_truth_P_nFD_1n.hFill(Reco_L, TLNeutronP, Weight);
                                    hReco_L_VS_R_nFD_1n.hFill(Reco_L, nResolution, Weight);
                                    hReco_L_VS_reco_theta_nFD_1n.hFill(Reco_L, RecoNeutronTheta, Weight);
                                    hReco_L_VS_reco_phi_nFD_1n.hFill(Reco_L, RecoNeutronPhi, Weight);

                                    if (ECIN_HIT && !ECOUT_HIT) {
                                        hReco_L_ECIN_1n.hFill(Reco_L, Weight);
                                        hReco_L_VS_reco_P_nFD_ECIN_1n.hFill(Reco_L, RecoNeutronP, Weight);
                                        hReco_L_VS_truth_P_nFD_ECIN_1n.hFill(Reco_L, TLNeutronP, Weight);
                                        hReco_L_VS_R_nFD_ECIN_1n.hFill(Reco_L, nResolution, Weight);
                                    }

                                    if (ECOUT_HIT && !ECIN_HIT) {
                                        hReco_L_ECOUT_1n.hFill(Reco_L, Weight);
                                        hReco_L_VS_reco_P_nFD_ECOUT_1n.hFill(Reco_L, RecoNeutronP, Weight);
                                        hReco_L_VS_truth_P_nFD_ECOUT_1n.hFill(Reco_L, TLNeutronP, Weight);
                                        hReco_L_VS_R_nFD_ECOUT_1n.hFill(Reco_L, nResolution, Weight);
                                    }

                                    hReco_t_ToF_1n.hFill(Reco_t_nFD, Weight);
                                    hReco_t_ToF_VS_reco_P_nFD_1n.hFill(Reco_t_nFD, RecoNeutronP, Weight);
                                    hReco_t_ToF_VS_truth_P_nFD_1n.hFill(Reco_t_nFD, TLNeutronP, Weight);
                                    hReco_t_ToF_VS_R_nFD_1n.hFill(Reco_t_nFD, nResolution, Weight);

                                    hReco_beta_1n.hFill(Reco_beta, Weight);
                                    hReco_beta_VS_reco_P_nFD_1n.hFill(RecoNeutronP, Reco_beta, Weight);
                                    hReco_beta_VS_truth_P_nFD_1n.hFill(TLNeutronP, Reco_beta, Weight);
                                    hReco_beta_VS_R_nFD_1n.hFill(Reco_beta, nResolution, Weight);

                                    hReco_ToF_from_beta_1n.hFill(Reco_t_ToF_from_beta, Weight);
                                    hReco_ToF_from_beta_VS_reco_P_nFD_1n.hFill(Reco_t_ToF_from_beta, RecoNeutronP, Weight);
                                    hReco_ToF_from_beta_VS_truth_P_nFD_1n.hFill(Reco_t_ToF_from_beta, TLNeutronP, Weight);
                                    hReco_ToF_from_beta_VS_R_nFD_1n.hFill(Reco_t_ToF_from_beta, nResolution, Weight);

                                    hReco_calc_ToF_1n.hFill(Reco_calc_t_nFD_ToF, Weight);
                                    hReco_calc_ToF_VS_reco_P_nFD_1n.hFill(Reco_calc_t_nFD_ToF, RecoNeutronP, Weight);
                                    hReco_calc_ToF_VS_truth_P_nFD_1n.hFill(Reco_calc_t_nFD_ToF, TLNeutronP, Weight);
                                    hReco_calc_ToF_VS_R_nFD_1n.hFill(Reco_calc_t_nFD_ToF, nResolution, Weight);

                                    // Distance offset
                                    hEff_dist_TL_1n.hFill(Eff_dist_TL, Weight);
                                    hEff_dist_TL_VS_reco_P_nFD_1n.hFill(Eff_dist_TL, RecoNeutronP, Weight);
                                    hEff_dist_TL_VS_truth_P_nFD_1n.hFill(Eff_dist_TL, TLNeutronP, Weight);
                                    hEff_dist_TL_VS_R_nFD_1n.hFill(Eff_dist_TL, nResolution, Weight);

                                    hEff_dist_calc_1n.hFill(Eff_dist_calc, Weight);
                                    hEff_dist_calc_VS_reco_P_nFD_1n.hFill(Eff_dist_calc, RecoNeutronP, Weight);
                                    hEff_dist_calc_VS_truth_P_nFD_1n.hFill(Eff_dist_calc, TLNeutronP, Weight);
                                    hEff_dist_calc_VS_R_nFD_1n.hFill(Eff_dist_calc, nResolution, Weight);

                                    hDeltaL_TL_1n.hFill(DeltaL_TL, Weight);
                                    hDeltaL_TL_VS_reco_P_nFD_1n.hFill(DeltaL_TL, RecoNeutronP, Weight);
                                    hDeltaL_TL_VS_truth_P_nFD_1n.hFill(DeltaL_TL, TLNeutronP, Weight);
                                    hDeltaL_TL_VS_R_nFD_1n.hFill(DeltaL_TL, nResolution, Weight);
                                    hDeltaL_TL_VS_Eff_dist_TL_1n.hFill(DeltaL_TL, Eff_dist_TL, Weight);
                                    hDeltaL_TL_VS_Reco_L_1n.hFill(DeltaL_TL, Reco_L, Weight);

                                    hDeltaL_calc_1n.hFill(DeltaL_calc, Weight);
                                    hDeltaL_calc_VS_reco_P_nFD_1n.hFill(DeltaL_calc, RecoNeutronP, Weight);
                                    hDeltaL_calc_VS_truth_P_nFD_1n.hFill(DeltaL_calc, TLNeutronP, Weight);
                                    hDeltaL_calc_VS_R_nFD_1n.hFill(DeltaL_calc, nResolution, Weight);
                                    hDeltaL_calc_VS_Eff_dist_TL_1n.hFill(DeltaL_calc, Eff_dist_TL, Weight);
                                    hDeltaL_calc_VS_Reco_L_1n.hFill(DeltaL_calc, Reco_L, Weight);

                                    if (nResolution < 0.4) {
                                        hDeltaL_TL_below_0_4_1n.hFill(DeltaL_TL, Weight);
                                        hDeltaL_TL_VS_reco_P_nFD_below_0_4_1n.hFill(DeltaL_TL, RecoNeutronP, Weight);
                                        hDeltaL_TL_VS_truth_P_nFD_below_0_4_1n.hFill(DeltaL_TL, TLNeutronP, Weight);
                                        hDeltaL_TL_VS_R_nFD_below_0_4_1n.hFill(DeltaL_TL, nResolution, Weight);
                                    } else {
                                        hDeltaL_TL_above_0_4_1n.hFill(DeltaL_TL, Weight);
                                        hDeltaL_TL_VS_reco_P_nFD_above_0_4_1n.hFill(DeltaL_TL, RecoNeutronP, Weight);
                                        hDeltaL_TL_VS_truth_P_nFD_above_0_4_1n.hFill(DeltaL_TL, TLNeutronP, Weight);
                                        hDeltaL_TL_VS_R_nFD_above_0_4_1n.hFill(DeltaL_TL, nResolution, Weight);
                                    }

                                    // Time error
                                    /* Distance offset */
                                    double TOF_error_calc = -(Reco_calc_t_nFD_ToF * (1 - Reco_beta * Reco_beta)) * nResolution;
                                    hDeltat_ToF_reco_1n.hFill(TOF_error_calc, Weight);
                                    hDeltat_ToF_reco_VS_reco_P_nFD_1n.hFill(TOF_error_calc, RecoNeutronP, Weight);
                                    hDeltat_ToF_reco_VS_truth_P_nFD_1n.hFill(TOF_error_calc, TLNeutronP, Weight);
                                    hDeltat_ToF_reco_VS_R_nFD_1n.hFill(TOF_error_calc, nResolution, Weight);
                                }
                            }
                        }

                        /* Res plots for thesis */
                        // TODO: figure out if these plots are needed
                        if (nRes_TL_Pass_PIDCut && nRes_Pass_FiducialCuts && nRes_Pass_ThetaKinCut &&
                            ((RecoNeutronP >= n_mom_th.GetLowerCut()) && (RecoNeutronP <= n_mom_th.GetUpperCut())) &&
                            ((TLNeutronP >= n_mom_th.GetLowerCut()) && (TLNeutronP <= n_mom_th.GetUpperCut()))) {
                            if (nRes_Pass_dThetaCut && nRes_Pass_dPhiCut) {
                                double nResolution = (TLNeutronP - RecoNeutronP) / TLNeutronP;
                                hP_nFD_Res_VS_TL_P_nFD_noKC_1n->Fill(TLNeutronP, nResolution, Weight);
                                hP_nFD_Res_VS_Reco_P_nFD_noKC_1n->Fill(NeutronMomBKC_1n, nResolution, Weight);
                                hP_nFD_Res_VS_Corr_Reco_P_nFD_noKC_1n->Fill(RecoNeutronP, nResolution, Weight);
                            }
                        }
                    }  // end of for loop over TL particles

                    if (Neutron_match_counter != 0) {
                        hnRes_Match_Multi_1n.hFill(Neutron_match_counter, Weight);
                        hnRes_Match_Multi_vs_Reco_P_nFD_1n.hFill(Neutron_match_counter, RecoNeutronP_Debug, Weight);
                        hnRes_Match_Multi_vs_Reco_Theta_nFD_1n.hFill(Neutron_match_counter, RecoNeutronTheta_Debug, Weight);
                        hnRes_Match_Multi_vs_Reco_Phi_nFD_1n.hFill(Neutron_match_counter, RecoNeutronPhi_Debug, Weight);
                    }
                }  // end of resolution calculation if
            }  // end of NeutronPassVeto_1n is true (i.e. if neutron did not hit PCAL & hit either ECIN or ECOUT) & pass kinematical cuts (1n) if
        }  // end of 1n cuts if

        //  1e2pXy (or (e,e'pp)X) -----------------------------------------------------------------------------------------------------------------------------------------------

        // 1e2pXy
        if (Np == 2) {
            // 2p and everything else is allowed
            // TODO: reorganize properly
            TVector3 P_p_first_1e2pXy, P_p_second_1e2pXy;  // P_L = leading proton (p1); P_R = recoil proton (p2)
            P_p_first_1e2pXy.SetMagThetaPhi(protons[0]->getP(), protons[0]->getTheta(), protons[0]->getPhi());
            P_p_second_1e2pXy.SetMagThetaPhi(protons[1]->getP(), protons[1]->getTheta(), protons[1]->getPhi());

            if (P_p_first_1e2pXy.Mag() >= P_p_second_1e2pXy.Mag()) {
                // if p_first is leading
                if (protons[0]->getRegion() == CD) { hPhi_p1_1e2pXy_CD->Fill(protons[0]->getPhi() * 180.0 / pi); }
                if (protons[1]->getRegion() == CD) { hPhi_p2_1e2pXy_CD->Fill(protons[1]->getPhi() * 180.0 / pi); }
            } else {
                // else if p_second is leading
                if (protons[1]->getRegion() == CD) { hPhi_p1_1e2pXy_CD->Fill(protons[1]->getPhi() * 180.0 / pi); }
                if (protons[0]->getRegion() == CD) { hPhi_p2_1e2pXy_CD->Fill(protons[0]->getPhi() * 180.0 / pi); }
            }
        }

        //  2p (FD & CD) --------------------------------------------------------------------------------------------------------------------------------------------------------

        // 2p (FD & CD)
        /* 2p event selection: 2p = Protons_ind.size() = 2, any id. FD neutrons and any number of other neutrals and particles with pdg=0. */
        bool reco_FD_Neutrons_2p = (Enable_FD_neutrons || (NeutronsFD_ind.size() == 0));  // no id. FD neutrons for Enable_FD_neutrons = false
        bool two_protons_2p = (Protons_ind.size() == 2);
        bool event_selection_2p = (basic_event_selection && reco_FD_Neutrons_2p && two_protons_2p);

        if (calculate_2p && event_selection_2p) {
            // for 2p calculations (with any number of neutrals, neutrons and pdg=0)
            ++num_of_events_with_1e2p;  // logging #(events) w/ 1e2p

            // Safety checks (2p)
            /* Safety check that we are looking at 2p */
            if (Protons_ind.size() != 2) { cout << "\033[33m\n\n2p: Protons_ind.size() is different than 2! Exiting...\n\n", exit(0); }

            CodeDebugger.SafetyCheck_basic_event_selection(__FILE__, __LINE__, "2p", Kplus, Kminus, Piplus_ind, Piminus_ind, Electron_ind, deuterons);

            /* Setting particle vectors (for code organization) */
            auto e_2p = electrons[Electron_ind.at(0)];
            auto p_first_2p = protons[Protons_ind.at(0)];
            auto p_second_2p = protons[Protons_ind.at(1)];

            /* NOTE: p_first corresponds to protons[Protons_ind.at(0)] & p_second corresponds to protons[Protons_ind.at(1)] */
            TVector3 P_e_2p_3v, q_2p_3v, P_p_first_2p_3v, P_p_second_2p_3v, P_tot_2p_3v, P_1_2p_3v, P_2_2p_3v;
            TVector3 P_T_e_2p_3v, P_T_L_2p_3v, P_T_tot_2p_3v, dP_T_L_2p_3v, dP_T_tot_2p_3v;
            TLorentzVector e_out_2p, Q_2p;

            P_e_2p_3v.SetMagThetaPhi(e_2p->getP(), e_2p->getTheta(), e_2p->getPhi());                              // electron 3 momentum
            q_2p_3v = TVector3(Pvx - P_e_2p_3v.Px(), Pvy - P_e_2p_3v.Py(), Pvz - P_e_2p_3v.Pz());                  // 3 momentum transfer
            P_T_e_2p_3v = TVector3(P_e_2p_3v.Px(), P_e_2p_3v.Py(), 0);                                             // electron transverse momentum
            P_p_first_2p_3v.SetMagThetaPhi(p_first_2p->getP(), p_first_2p->getTheta(), p_first_2p->getPhi());      // first proton in protons vector
            P_p_second_2p_3v.SetMagThetaPhi(p_second_2p->getP(), p_second_2p->getTheta(), p_second_2p->getPhi());  // second proton in protons vector

            e_out_2p.SetPxPyPzE(electrons[0]->par()->getPx(), electrons[0]->par()->getPy(), electrons[0]->par()->getPz(), sqrt(m_e * m_e + P_e_2p_3v.Mag2()));
            Q_2p = beam - e_out_2p;  // definition of 4-momentum transfer
            double Q2_2p = fabs(Q_2p.Mag2());

            double E_e_2p = sqrt(m_e * m_e + P_e_2p_3v.Mag2()), omega_2p = beamE - E_e_2p, W_2p = sqrt((omega_2p + m_p) * (omega_2p + m_p) - q_2p_3v.Mag2());
            double E_1_2p, E_2_2p, Theta_p_e_p_tot_2p, Theta_q_p_tot_2p, Theta_q_p_L_2p, Theta_q_p_R_2p;
            double dAlpha_T_L_2p, dAlpha_T_tot_2p, dPhi_T_L_2p, dPhi_T_tot_2p, Ecal_2p;

            // Determining leading, recoil protons and their angles (2p)
            int lead_p_2p_ind, recoil_p_2p_ind;  // indices of leading and recoil protons

            if (P_p_first_2p_3v.Mag() >= P_p_second_2p_3v.Mag()) {
                // If p_first = protons[0] is leading proton
                P_1_2p_3v = TVector3(P_p_first_2p_3v.Px(), P_p_first_2p_3v.Py(), P_p_first_2p_3v.Pz());
                P_2_2p_3v = TVector3(P_p_second_2p_3v.Px(), P_p_second_2p_3v.Py(), P_p_second_2p_3v.Pz());
                lead_p_2p_ind = Protons_ind.at(0), recoil_p_2p_ind = Protons_ind.at(1);
            } else {
                // else if p_second = p_second_2p is leading proton
                P_2_2p_3v = TVector3(P_p_first_2p_3v.Px(), P_p_first_2p_3v.Py(), P_p_first_2p_3v.Pz());
                P_1_2p_3v = TVector3(P_p_second_2p_3v.Px(), P_p_second_2p_3v.Py(), P_p_second_2p_3v.Pz());
                lead_p_2p_ind = Protons_ind.at(1), recoil_p_2p_ind = Protons_ind.at(0);
            }

            /* Angles of leading and recoil protons */
            double Theta_p1 = P_1_2p_3v.Theta() * 180.0 / pi, Theta_p2 = P_2_2p_3v.Theta() * 180.0 / pi;  // Theta_p1, Theta_p2 in deg
            double Phi_p1 = P_1_2p_3v.Phi() * 180.0 / pi, Phi_p2 = P_2_2p_3v.Phi() * 180.0 / pi;          // Phi_p1, Phi_p2 in deg
            double Theta_p1_p2_2p = acos((P_1_2p_3v.Px() * P_2_2p_3v.Px() + P_1_2p_3v.Py() * P_2_2p_3v.Py() + P_1_2p_3v.Pz() * P_2_2p_3v.Pz()) / (P_1_2p_3v.Mag() * P_2_2p_3v.Mag())) *
                                    180.0 / pi;  // Theta_p1_p2_2p in deg

            //  Fillings 2p histograms ---------------------------------------------------------------------------------------------------------------------------------

            CodeDebugger.PrintStepTester(DebuggerMode);

            // Fillings 2p histograms
            ++num_of_events_2p;

            // Filling cut parameters histograms (2p)
            /* Filling Chi2 histograms (2p) */

            // Electrton Chi2 (2p):
            if (e_2p->getRegion() == FD) { hChi2_Electron_2p_FD.hFill(e_2p->par()->getChi2Pid(), Weight); }

            // Proton0 Chi2 (2p):
            if (p_first_2p->getRegion() == CD) {
                hChi2_Proton_2p_CD.hFill(p_first_2p->par()->getChi2Pid(), Weight);
            } else if (p_first_2p->getRegion() == FD) {
                hChi2_Proton_2p_FD.hFill(p_first_2p->par()->getChi2Pid(), Weight);
            }

            // Proton1 Chi2 (2p):
            if (p_second_2p->getRegion() == CD) {
                hChi2_Proton_2p_CD.hFill(p_second_2p->par()->getChi2Pid(), Weight);
            } else if (p_second_2p->getRegion() == FD) {
                hChi2_Proton_2p_FD.hFill(p_second_2p->par()->getChi2Pid(), Weight);
            }

            /* Filling dVx, dVy, dVz (2p) */
            for (auto &p : protons) {
                double Vx_p_2p = p->par()->getVx(), Vy_p_2p = p->par()->getVy(), Vz_p_2p = p->par()->getVz();
                double dVx = Vx_p_2p - Vx_e, dVy = Vy_p_2p - Vy_e, dVz = Vz_p_2p - Vz_e;

                hdVx_2p.hFill(dVx, Weight), hdVy_2p.hFill(dVy, Weight), hdVz_2p.hFill(dVz, Weight);
            }  // end of loop over protons vector

            /* Filling SF histograms (2p) */
            hSF_2p_FD.hFill(EoP_e, Weight), hSF_VS_P_e_2p_FD.hFill(P_e_1e_cut, EoP_e, Weight);

            /* Filling fiducial plots (2p) */
            hVcal_VS_EoP_2p_PCAL.hFill(e_2p->cal(clas12::PCAL)->getLv(), EoP_e, Weight);
            hWcal_VS_EoP_2p_PCAL.hFill(e_2p->cal(clas12::PCAL)->getLw(), EoP_e, Weight);

            /* Filling Nphe plots (2p) */
            hNphe_2p_FD.hFill(Nphe, Weight);

            // Filling momentum histograms (2p)

            // Electron momentum (2p)
            if (e_2p->getRegion() == FD) { hP_e_APID_2p_FD.hFill(P_e_1e_cut, Weight); }

            for (int i = 0; i < Ne; i++) {
                if (electrons[i]->getRegion() == FD) { hP_e_BPID_2p_FD.hFill(P_e_1e_cut, Weight); }
            }  // before mom. th.

            // Proton momentum (2p)
            // TODO: remove 2p plots for protons in CD (we're looking at 2p in the FD only!)
            for (int &i : Protons_ind) {
                if (protons[i]->getRegion() == CD) {
                    hP_p_APID_2p_CD.hFill(protons[i]->getP(), Weight);  // after mom. th.
                } else if (protons[i]->getRegion() == FD) {
                    hP_p_APID_2p_FD.hFill(protons[i]->getP(), Weight);  // after mom. th.
                }
            }

            for (int i = 0; i < Np; i++) {
                if (protons[i]->getRegion() == CD) {
                    hP_p_BPID_2p_CD.hFill(protons[i]->getP(), Weight);  // before mom. th.
                } else if (protons[i]->getRegion() == FD) {
                    hP_p_BPID_2p_FD.hFill(protons[i]->getP(), Weight);  // before mom. th.
                }
            }

            // Piplus momentum (2p)
            for (int &i : Piplus_ind) {
                if (piplus[i]->getRegion() == CD) {
                    hP_piplus_APID_2p_CD.hFill(piplus[i]->getP(), Weight);  // after mom. th.
                } else if (piplus[i]->getRegion() == FD) {
                    hP_piplus_APID_2p_FD.hFill(piplus[i]->getP(), Weight);  // after mom. th.
                }
            }

            for (int i = 0; i < Npip; i++) {
                if (piplus[i]->getRegion() == CD) {
                    hP_piplus_BPID_2p_CD.hFill(piplus[i]->getP(), Weight);  // before mom. th.
                } else if (piplus[i]->getRegion() == FD) {
                    hP_piplus_BPID_2p_FD.hFill(piplus[i]->getP(), Weight);  // before mom. th.
                }
            }

            // Piminus momentum (2p)
            for (int &i : Piminus_ind) {
                if (piminus[i]->getRegion() == CD) {
                    hP_piminus_APID_2p_CD.hFill(piminus[i]->getP(), Weight);  // after mom. th.
                } else if (piminus[i]->getRegion() == FD) {
                    hP_piminus_APID_2p_FD.hFill(piminus[i]->getP(), Weight);  // after mom. th.
                }
            }

            for (int i = 0; i < piminus.size(); i++) {
                if (piminus[i]->getRegion() == CD) {
                    hP_piminus_BPID_2p_CD.hFill(piminus[i]->getP(), Weight);  // before mom. th.
                } else if (piminus[i]->getRegion() == FD) {
                    hP_piminus_BPID_2p_FD.hFill(piminus[i]->getP(), Weight);  // before mom. th.
                }
            }

            // Photon momentum (2p)
            for (int &i : PhotonsFD_ind) { hP_ph_APID_2p_FD.hFill(allParticles[i]->getP(), Weight); }  // after mom. th.

            for (int &i : ReDef_photons_FD) { hP_ph_BPID_2p_FD.hFill(allParticles[i]->getP(), Weight); }  // before mom. th.

            // Neutron momentum (2p)
            for (int &i : NeutronsFD_ind) { hP_n_APID_2p_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight); }  // after mom. th.

            for (int &i : ReDef_neutrons_FD) { hP_n_BPID_2p_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight); }  // before mom. th.

            // Filling Beta vs. P plots (2p)

            // Beta vs. P from electrons (2p, CD & FD)
            if (e_2p->getRegion() == FD) {
                hBeta_vs_P_2p_FD.hFill(e_2p->getP(), e_2p->par()->getBeta(), Weight);
                hBeta_vs_P_2p_Electrons_Only_FD.hFill(e_2p->getP(), e_2p->par()->getBeta(), Weight);

                if (e_2p->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_2p_FD.hFill(e_2p->getP(), e_2p->par()->getBeta(), Weight);
                } else if (e_2p->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_2p_FD.hFill(e_2p->getP(), e_2p->par()->getBeta(), Weight);
                } else if (e_2p->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_2p_FD.hFill(e_2p->getP(), e_2p->par()->getBeta(), Weight);
                }
            }

            // Beta vs. P from protons (2p, CD & FD)
            for (int i = 0; i < Np; i++) {
                if (protons[i]->getRegion() == CD) {
                    hBeta_vs_P_2p_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_2p_Protons_Only_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_positive_part_2p_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                } else if (protons[i]->getRegion() == FD) {
                    hBeta_vs_P_2p_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_2p_Protons_Only_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_positive_part_2p_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                }
            }  // end of loop over protons vector

            // Beta vs. P from other particles (2p, CD & FD)
            /* This is for self-consistency. Contributions from other particles should be zero */

            // Beta vs. P from Kplus (2p, CD & FD)
            for (int i = 0; i < Kplus.size(); i++) {
                if (Kplus[i]->getRegion() == CD) {
                    hBeta_vs_P_2p_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_positive_part_2p_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                } else if (Kplus[i]->getRegion() == FD) {
                    hBeta_vs_P_2p_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_positive_part_2p_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                }
            }  // end of loop over Kplus vector

            // Beta vs. P from Kminus (2p, CD & FD)
            for (int i = 0; i < Kminus.size(); i++) {
                if (Kminus[i]->getRegion() == CD) {
                    hBeta_vs_P_2p_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_negative_part_2p_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                } else if (Kminus[i]->getRegion() == FD) {
                    hBeta_vs_P_2p_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_negative_part_2p_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                }
            }  // end of loop over Kminus vector

            // Beta vs. P from piplus (2p, CD & FD)
            for (int i = 0; i < piplus.size(); i++) {
                if (piplus[i]->getRegion() == CD) {
                    hBeta_vs_P_2p_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_positive_part_2p_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                } else if (piplus[i]->getRegion() == FD) {
                    hBeta_vs_P_2p_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_positive_part_2p_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                }
            }  // end of loop over piplus vector

            // Beta vs. P from piminus (2p, CD & FD)
            for (int i = 0; i < piminus.size(); i++) {
                if (piminus[i]->getRegion() == CD) {
                    hBeta_vs_P_2p_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_negative_part_2p_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                } else if (piminus[i]->getRegion() == FD) {
                    hBeta_vs_P_2p_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                    hBeta_vs_P_negative_part_2p_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                }
            }  // end of loop over piminus vector

            for (auto &e : electrons) {
                if (e->getRegion() == FD) {
                    hTheta_e_All_Int_2p_FD->Fill(Theta_e, Weight);
                    hPhi_e_All_Int_2p_FD->Fill(Phi_e, Weight);
                    hTheta_e_VS_Phi_e_2p_FD->Fill(Phi_e, Theta_e, Weight);

                    hE_e_All_Int_2p_FD->Fill(E_e_1e_cut, Weight);
                    hE_e_VS_Theta_e_All_Int_2p_FD->Fill(Theta_e, E_e_1e_cut, Weight);

                    hET_All_Ang_All_Int_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                    if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                        hET15_All_Int_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                        hE_e_15_All_Int_2p_FD->Fill(E_e_1e_cut, Weight);
                    }

                    if (qel) {
                        hTheta_e_QEL_2p_FD->Fill(Theta_e, Weight);
                        hPhi_e_QEL_2p_FD->Fill(Phi_e, Weight);
                        hE_e_QEL_2p_FD->Fill(E_e_1e_cut, Weight);
                        hE_e_VS_Theta_e_QEL_2p_FD->Fill(Theta_e, E_e_1e_cut, Weight);

                        hET_All_Ang_QEL_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_QEL_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                            hE_e_15_QEL_2p_FD->Fill(E_e_1e_cut, Weight);
                        }
                    } else if (mec) {
                        hTheta_e_MEC_2p_FD->Fill(Theta_e, Weight);
                        hPhi_e_MEC_2p_FD->Fill(Phi_e, Weight);
                        hE_e_MEC_2p_FD->Fill(E_e_1e_cut, Weight);
                        hE_e_VS_Theta_e_MEC_2p_FD->Fill(Theta_e, E_e_1e_cut, Weight);

                        hET_All_Ang_MEC_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_MEC_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                            hE_e_15_MEC_2p_FD->Fill(E_e_1e_cut, Weight);
                        }
                    } else if (res) {
                        hTheta_e_RES_2p_FD->Fill(Theta_e, Weight);
                        hPhi_e_RES_2p_FD->Fill(Phi_e, Weight);
                        hE_e_RES_2p_FD->Fill(E_e_1e_cut, Weight);
                        hE_e_VS_Theta_e_RES_2p_FD->Fill(Theta_e, E_e_1e_cut, Weight);

                        hET_All_Ang_RES_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_RES_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                            hE_e_15_RES_2p_FD->Fill(E_e_1e_cut, Weight);
                        }
                    } else if (dis) {
                        hTheta_e_DIS_2p_FD->Fill(Theta_e, Weight);
                        hPhi_e_DIS_2p_FD->Fill(Phi_e, Weight);
                        hE_e_DIS_2p_FD->Fill(E_e_1e_cut, Weight);
                        hE_e_VS_Theta_e_DIS_2p_FD->Fill(Theta_e, E_e_1e_cut, Weight);

                        hET_All_Ang_DIS_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_DIS_2p_FD->Fill(beamE - E_e_1e_cut, Weight);
                            hE_e_15_DIS_2p_FD->Fill(E_e_1e_cut, Weight);
                        }
                    }
                }
            }  // end of loop over electrons vector

            /* Filling Momentum transfer histograms (2p) */
            if (e_2p->getRegion() == FD) {
                // Fill momentum transfer plots (1n)
                FillByInt(hQ2_2p, hQ2_QEL_2p, hQ2_MEC_2p, hQ2_RES_2p, hQ2_DIS_2p, qel, mec, res, dis, Q2_2p, Weight);
                FillByInt(hq_3v_2p, hq_3v_QEL_2p, hq_3v_MEC_2p, hq_3v_RES_2p, hq_3v_DIS_2p, qel, mec, res, dis, q_2p_3v.Mag(), Weight);

                hQ2_VS_W_2p->Fill(W_2p, Q2_2p, Weight);
                hQ2_VS_q_3v_2p->Fill(q_2p_3v.Mag(), Q2_2p, Weight);
                hQ2_VS_omega_2p->Fill(omega_2p, Q2_2p, Weight);
                hq_3v_VS_omega_2p->Fill(omega_2p, q_2p_3v.Mag(), Weight);

                if (qel) {
                    hQ2_VS_W_QEL_2p->Fill(W_2p, Q2_2p, Weight);
                    hQ2_VS_q_3v_QEL_2p->Fill(q_2p_3v.Mag(), Q2_2p, Weight);
                    hQ2_VS_omega_QEL_2p->Fill(omega_2p, Q2_2p, Weight);
                    hq_3v_VS_omega_QEL_2p->Fill(omega_2p, q_2p_3v.Mag(), Weight);
                } else if (mec) {
                    hQ2_VS_W_MEC_2p->Fill(W_2p, Q2_2p, Weight);
                    hQ2_VS_q_3v_MEC_2p->Fill(q_2p_3v.Mag(), Q2_2p, Weight);
                    hQ2_VS_omega_MEC_2p->Fill(omega_2p, Q2_2p, Weight);
                    hq_3v_VS_omega_MEC_2p->Fill(omega_2p, q_2p_3v.Mag(), Weight);
                } else if (res) {
                    hQ2_VS_W_RES_2p->Fill(W_2p, Q2_2p, Weight);
                    hQ2_VS_q_3v_RES_2p->Fill(q_2p_3v.Mag(), Q2_2p, Weight);
                    hQ2_VS_omega_RES_2p->Fill(omega_2p, Q2_2p, Weight);
                    hq_3v_VS_omega_RES_2p->Fill(omega_2p, q_2p_3v.Mag(), Weight);
                } else if (dis) {
                    hQ2_VS_W_DIS_2p->Fill(W_2p, Q2_2p, Weight);
                    hQ2_VS_q_3v_DIS_2p->Fill(q_2p_3v.Mag(), Q2_2p, Weight);
                    hQ2_VS_omega_DIS_2p->Fill(omega_2p, Q2_2p, Weight);
                    hq_3v_VS_omega_DIS_2p->Fill(omega_2p, q_2p_3v.Mag(), Weight);
                }
            }

            // Fill W (2p)
            FillByInt(hW_All_Int_2p, hW_QEL_2p, hW_MEC_2p, hW_RES_2p, hW_DIS_2p, qel, mec, res, dis, W_2p, Weight);
            hW_VS_q_3v_2p->Fill(W_2p, q_2p_3v.Mag(), Weight);
            hW_VS_omega_2p->Fill(W_2p, omega_2p, Weight);

            if (qel) {
                hW_VS_q_3v_QEL_2p->Fill(W_2p, q_2p_3v.Mag(), Weight);
                hW_VS_omega_QEL_2p->Fill(W_2p, omega_2p, Weight);
            } else if (mec) {
                hW_VS_q_3v_MEC_2p->Fill(W_2p, q_2p_3v.Mag(), Weight);
                hW_VS_omega_MEC_2p->Fill(W_2p, omega_2p, Weight);
            } else if (res) {
                hW_VS_q_3v_RES_2p->Fill(W_2p, q_2p_3v.Mag(), Weight);
                hW_VS_omega_RES_2p->Fill(W_2p, omega_2p, Weight);
            } else if (dis) {
                hW_VS_q_3v_DIS_2p->Fill(W_2p, q_2p_3v.Mag(), Weight);
                hW_VS_omega_DIS_2p->Fill(W_2p, omega_2p, Weight);
            }

            hP_p_1_2p.hFill(P_1_2p_3v.Mag(), Weight);  // Leading proton (2p)
            hP_p_2_2p.hFill(P_2_2p_3v.Mag(), Weight);  // Recoil proton (2p)
            hP_p_1_vs_P_p_2_2p.hFill(P_1_2p_3v.Mag(), P_2_2p_3v.Mag(), Weight);

            P_tot_2p_3v = TVector3(P_p_first_2p_3v.Px() + P_p_second_2p_3v.Px(), P_p_first_2p_3v.Py() + P_p_second_2p_3v.Py(),
                                   P_p_first_2p_3v.Pz() + P_p_second_2p_3v.Pz());  // P_tot = P_1 + P_2
            P_T_L_2p_3v = TVector3(P_1_2p_3v.Px(), P_1_2p_3v.Py(), 0);             // transverse part of P_1
            P_T_tot_2p_3v = TVector3(P_p_first_2p_3v.Px() + P_p_second_2p_3v.Px(), P_p_first_2p_3v.Py() + P_p_second_2p_3v.Py(), 0);
            // transverse part of P_tot
            dP_T_L_2p_3v = TVector3(P_e_2p_3v.Px() + P_T_L_2p_3v.Px(), P_e_2p_3v.Py() + P_T_L_2p_3v.Py(), 0);
            dP_T_tot_2p_3v = TVector3(P_e_2p_3v.Px() + P_1_2p_3v.Px() + P_2_2p_3v.Px(), P_e_2p_3v.Py() + P_1_2p_3v.Py() + P_2_2p_3v.Py(), 0);

            Theta_p_e_p_tot_2p = acos((P_e_2p_3v.Px() * P_tot_2p_3v.Px() + P_e_2p_3v.Py() * P_tot_2p_3v.Py() + P_e_2p_3v.Pz() * P_tot_2p_3v.Pz()) / (P_e_2p_3v.Mag() * P_tot_2p_3v.Mag())) *
                                 180.0 / pi;  // Theta_p_e_p_tot_2p in deg
            hTheta_p_e_p_tot_2p->Fill(Theta_p_e_p_tot_2p, Weight);

            hTheta_p1_p2_All_Int_2p->Fill(Theta_p1_p2_2p, Weight);

            if (qel) {
                hTheta_p1_p2_QEL_2p->Fill(Theta_p1_p2_2p, Weight);
            } else if (mec) {
                hTheta_p1_p2_MEC_2p->Fill(Theta_p1_p2_2p, Weight);
            } else if (res) {
                hTheta_p1_p2_RES_2p->Fill(Theta_p1_p2_2p, Weight);
            } else if (dis) {
                hTheta_p1_p2_DIS_2p->Fill(Theta_p1_p2_2p, Weight);
            }

            hTheta_p1_p2_vs_W_2p->Fill(W_2p, Theta_p1_p2_2p, Weight);

            // Filling double-detection plots (2p)

            // Filling double-detection plots for 2p
            double dPhi_hit_2p = CalcdPhi(Phi_p1 - Phi_p2);

            if (Theta_p1_p2_2p < 20.) {
                hTheta_p1_vs_theta_p2_for_Theta_p1_p2_20_2p->Fill(Theta_p2, Theta_p1, Weight);
                hdPhi_p1_p2_for_Theta_p1_p2_20_2p->Fill(dPhi_hit_2p, Weight);
                hdPhi_p1_p2_for_Theta_p1_p2_20_ZOOMIN_2p->Fill(dPhi_hit_2p, Weight);
            }

            hTheta_p1_vs_theta_p2_forall_Theta_p1_p2_2p->Fill(Theta_p2, Theta_p1, Weight);
            hdPhi_p1_p2_for_all_Theta_p1_p2_2p->Fill(dPhi_hit_2p, Weight);
            hdPhi_p1_p2_for_all_Theta_p1_p2_ZOOMIN_2p->Fill(dPhi_hit_2p, Weight);

            if ((fabs(Theta_p1 - Theta_p1_cuts_2p.GetMean()) < Theta_p1_cuts_2p.GetUpperCut()) && (fabs(Theta_p2 - Theta_p2_cuts_2p.GetMean()) < Theta_p2_cuts_2p.GetUpperCut())) {
                hdPhi_p1_p2_for_small_dTheta_2p->Fill(dPhi_hit_2p, Weight);
                hdPhi_p1_p2_for_small_dTheta_ZOOMIN_2p->Fill(dPhi_hit_2p, Weight);
            }

            // Filling double-detection plots for pFDpCD
            if ((p_first_2p->getRegion() == FD && p_second_2p->getRegion() == CD) || (p_first_2p->getRegion() == CD && p_second_2p->getRegion() == FD)) {
                double dPhi_hit_pFDpCD_2p = CalcdPhi(p_first_2p, p_second_2p);

                double Theta_pFD, Theta_pCD;

                if (p_first_2p->getRegion() == FD && p_second_2p->getRegion() == CD) {
                    Theta_pFD = p_first_2p->getTheta() * 180.0 / pi, Theta_pCD = p_second_2p->getTheta() * 180.0 / pi;
                } else if (p_first_2p->getRegion() == CD && p_second_2p->getRegion() == FD) {
                    Theta_pFD = p_second_2p->getTheta() * 180.0 / pi, Theta_pCD = p_first_2p->getTheta() * 180.0 / pi;
                }

                if (Theta_p1_p2_2p < 20.) {
                    // Theta_p1_p2_2p does not change for pFDpCD, so no new calculation is needed
                    hTheta_pFD_vs_Theta_pCD_for_Theta_pFD_pCD_20_2p->Fill(Theta_pCD, Theta_pFD, Weight);
                    hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_2p->Fill(dPhi_hit_pFDpCD_2p, Weight);
                    hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_2p->Fill(dPhi_hit_pFDpCD_2p, Weight);
                }

                hTheta_pFD_vs_Theta_pCD_forall_Theta_pFD_pCD_2p->Fill(Theta_pCD, Theta_pFD, Weight);
                hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_2p->Fill(dPhi_hit_pFDpCD_2p, Weight);
                hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_2p->Fill(dPhi_hit_pFDpCD_2p, Weight);

                if ((fabs(Theta_pFD - Theta_pFD_cuts_2p.GetMean()) < Theta_pFD_cuts_2p.GetUpperCut()) && (fabs(Theta_pCD - Theta_pCD_cuts_2p.GetMean()) < Theta_pCD_cuts_2p.GetUpperCut())) {
                    hdPhi_pFD_pCD_for_small_dTheta_2p->Fill(dPhi_hit_pFDpCD_2p, Weight);
                    hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_2p->Fill(dPhi_hit_pFDpCD_2p, Weight);
                }
            }

            Theta_q_p_tot_2p = acos((q_2p_3v.Px() * P_tot_2p_3v.Px() + q_2p_3v.Py() * P_tot_2p_3v.Py() + q_2p_3v.Pz() * P_tot_2p_3v.Pz()) / (q_2p_3v.Mag() * P_tot_2p_3v.Mag())) * 180.0 /
                               pi;  // Theta_q_p_tot_2p in deg
            hTheta_q_p_tot_2p->Fill(Theta_q_p_tot_2p, Weight);

            Theta_q_p_L_2p = acos((q_2p_3v.Px() * P_1_2p_3v.Px() + q_2p_3v.Py() * P_1_2p_3v.Py() + q_2p_3v.Pz() * P_1_2p_3v.Pz()) / (q_2p_3v.Mag() * P_1_2p_3v.Mag())) * 180.0 /
                             pi;  // Theta_q_p_L_2p = Theta_q_p_1_2p in deg
            Theta_q_p_R_2p = acos((q_2p_3v.Px() * P_2_2p_3v.Px() + q_2p_3v.Py() * P_2_2p_3v.Py() + q_2p_3v.Pz() * P_2_2p_3v.Pz()) / (q_2p_3v.Mag() * P_2_2p_3v.Mag())) * 180.0 /
                             pi;  // Theta_q_p_R_2p = Theta_q_p_2_2p in deg
            hTheta_q_p_L_2p->Fill(Theta_q_p_L_2p, Weight);
            hTheta_q_p_R_2p->Fill(Theta_q_p_R_2p, Weight);
            hTheta_q_p_L_vs_p_L_q_2p->Fill(P_1_2p_3v.Mag() / q_2p_3v.Mag(), Theta_q_p_L_2p, Weight);

            E_1_2p = sqrt(m_p * m_p + P_1_2p_3v.Mag2());
            E_2_2p = sqrt(m_p * m_p + P_2_2p_3v.Mag2());
            Ecal_2p = E_e_2p + (E_1_2p - m_p) + (E_2_2p - m_p);

            // Filling Ecal plots
            hEcal_All_Int_2p->Fill(Ecal_2p, Weight);  // Fill Ecal for all interactions

            if (qel) {
                hEcal_QEL_2p->Fill(Ecal_2p, Weight);  // Fill Ecal for QEL only
            } else if (mec) {
                hEcal_MEC_2p->Fill(Ecal_2p, Weight);  // Fill Ecal for MEC only
            } else if (res) {
                hEcal_RES_2p->Fill(Ecal_2p, Weight);  // Fill Ecal for RES only
            } else if (dis) {
                hEcal_DIS_2p->Fill(Ecal_2p, Weight);  // Fill Ecal for DIS only
            }

            hdP_T_L_2p->Fill(dP_T_L_2p_3v.Mag(), Weight);
            hdP_T_tot_2p->Fill(dP_T_tot_2p_3v.Mag(), Weight);

            dAlpha_T_L_2p = acos(-(P_e_2p_3v.Px() * dP_T_L_2p_3v.Px() + P_e_2p_3v.Py() * dP_T_L_2p_3v.Py() + P_e_2p_3v.Pz() * dP_T_L_2p_3v.Pz()) / (P_T_e_2p_3v.Mag() * dP_T_L_2p_3v.Mag())) *
                            180.0 / pi;  // dP_T_L_2p_3v.Pz() = 0; dAlpha_T_L_2p in deg
            dAlpha_T_tot_2p =
                acos(-(P_e_2p_3v.Px() * dP_T_tot_2p_3v.Px() + P_e_2p_3v.Py() * dP_T_tot_2p_3v.Py() + P_e_2p_3v.Pz() * dP_T_tot_2p_3v.Pz()) / (P_T_e_2p_3v.Mag() * dP_T_tot_2p_3v.Mag())) *
                180.0 / pi;  // dP_T_tot_2p_3v.Pz() = 0; dAlpha_T_tot_2p in deg
            hdAlpha_T_L_2p->Fill(dAlpha_T_L_2p, Weight);
            hdAlpha_T_tot_2p->Fill(dAlpha_T_tot_2p, Weight);

            hdP_T_L_vs_dAlpha_T_L_2p->Fill(dAlpha_T_L_2p, dP_T_L_2p_3v.Mag(), Weight);
            hdP_T_tot_vs_dAlpha_T_tot_2p->Fill(dAlpha_T_tot_2p, dP_T_tot_2p_3v.Mag(), Weight);

            dPhi_T_L_2p = acos(-(P_T_e_2p_3v.Px() * P_T_L_2p_3v.Px() + P_T_e_2p_3v.Py() * P_T_L_2p_3v.Py() + P_T_e_2p_3v.Pz() * P_T_L_2p_3v.Pz()) / (P_T_e_2p_3v.Mag() * P_T_L_2p_3v.Mag())) *
                          180.0 / pi;  // P_T_L_2p_3v.Pz() = 0; dPhi_T_L_2p in deg
            dPhi_T_tot_2p =
                acos(-(P_T_e_2p_3v.Px() * P_T_tot_2p_3v.Px() + P_T_e_2p_3v.Py() * P_T_tot_2p_3v.Py() + P_T_e_2p_3v.Pz() * P_T_tot_2p_3v.Pz()) / (P_T_e_2p_3v.Mag() * P_T_tot_2p_3v.Mag())) *
                180.0 / pi;  // P_T_tot_2p_3v.Pz() = 0; dPhi_T_tot_2p in deg
            hdPhi_T_L_2p->Fill(dPhi_T_L_2p, Weight);
            hdPhi_T_tot_2p->Fill(dPhi_T_tot_2p, Weight);

            hEcal_vs_dAlpha_T_L_2p->Fill(dAlpha_T_L_2p, Ecal_2p, Weight);
            hEcal_vs_dAlpha_T_tot_2p->Fill(dAlpha_T_tot_2p, Ecal_2p, Weight);
            hEcal_vs_dP_T_L_2p->Fill(dP_T_L_2p_3v.Mag(), Ecal_2p, Weight);
            hEcal_vs_dP_T_tot_2p->Fill(dP_T_tot_2p_3v.Mag(), Ecal_2p, Weight);

            if ((p_first_2p->getRegion() == CD) && (p_second_2p->getRegion() == CD)) {
                // if both 2p protons are in the CD
                TVector3 p1_hit_pos, p2_hit_pos, pos_diff;
                p1_hit_pos.SetXYZ(p_first_2p->sci(clas12::CTOF)->getX(), p_first_2p->sci(clas12::CTOF)->getY(), p_first_2p->sci(clas12::CTOF)->getZ());
                p2_hit_pos.SetXYZ(p_second_2p->sci(clas12::CTOF)->getX(), p_second_2p->sci(clas12::CTOF)->getY(), p_second_2p->sci(clas12::CTOF)->getZ());

                pos_diff.SetXYZ(p1_hit_pos.Px() - p2_hit_pos.Px(), p1_hit_pos.Py() - p2_hit_pos.Py(), p1_hit_pos.Pz() - p2_hit_pos.Pz());
                double time_diff = protons[lead_p_2p_ind]->getTime() - protons[recoil_p_2p_ind]->getTime();

                hTheta_p1_p2_VS_ToF1_ToF2_AC_2p.hFill(Theta_p1_p2_2p, time_diff, Weight);
                hTheta_p1_p2_VS_Pos1_Pos2_AC_2p.hFill(Theta_p1_p2_2p, pos_diff.Mag(), Weight);
            }
        }  // end of 2p cuts if

        //  pFDpCD --------------------------------------------------------------------------------------------------------------------------------------------------------------

        CodeDebugger.PrintStepTester(DebuggerMode);

        // pFDpCD

        // Configure if event is pFDpCD
        /* This event selection is a subgroup of the 2p selectin. Here we figure out if the event is pFDpCD or not. */
        bool is_pFDpCD = false;

        if (calculate_pFDpCD && event_selection_2p) {
            // if we have a 2p event with one id. proton in the CD and one in the FD, then is_pFDpCD is true
            is_pFDpCD = (((protons[Protons_ind.at(0)]->getRegion() == FD) && (protons[Protons_ind.at(1)]->getRegion() == CD)) ||
                         ((protons[Protons_ind.at(0)]->getRegion() == CD) && (protons[Protons_ind.at(1)]->getRegion() == FD)));
        }

        bool apply_TL_pFDpCD_ES = (!Rec_wTL_ES || TL_Event_Selection_pFDpCD);

        // Raw pFDpCD event counts
        if (calculate_pFDpCD && event_selection_2p) {
            if ((protons[Protons_ind.at(0)]->getRegion() == FD) && (protons[Protons_ind.at(1)]->getRegion() == FD)) {
                ++num_of_events_with_1epFDpFD;
            } else if ((protons[Protons_ind.at(0)]->getRegion() == CD) && (protons[Protons_ind.at(1)]->getRegion() == CD)) {
                ++num_of_events_with_1epCDpCD;
            }
        }

        if (calculate_pFDpCD && event_selection_2p && is_pFDpCD && apply_TL_pFDpCD_ES) {
            // for 2p calculations that is also pFDpCD
            ++num_of_events_with_1epFDpCD;  // logging #(events) w/ 1epFDpCD

            // Setting particle vectors (for code organization) (pFDpCD)
            /* Defining initial particle vectors. NOTE:
             * p_first corresponds to protons[Protons_ind.at(0)] & p_second corresponds to protons[Protons_ind.at(1)]
             * P_1 corresponds to leading proton & P_2 corresponds to the recoil */

            region_part_ptr e_pFDpCD = electrons[Electron_ind.at(0)];
            region_part_ptr p_first_pFDpCD = protons[Protons_ind.at(0)];
            region_part_ptr p_second_pFDpCD = protons[Protons_ind.at(1)];

            /* Declaring Determining particle vectors: */
            region_part_ptr pCD_pFDpCD, pFD_pFDpCD;

            if (protons[Protons_ind.at(0)]->getRegion() == FD) {
                pFD_pFDpCD = protons[Protons_ind.at(0)], pCD_pFDpCD = protons[Protons_ind.at(1)];
            } else if (protons[Protons_ind.at(0)]->getRegion() == CD) {
                pCD_pFDpCD = protons[Protons_ind.at(0)], pFD_pFDpCD = protons[Protons_ind.at(1)];
            }

            // Safety checks (pFDpCD)
            /* Safety check that we are looking at pFDpCD */
            if (e_pFDpCD->getRegion() != FD) { cout << "\033[33m\n\npFDpCD: Electron is not in the FD! Exiting...\n\n", exit(0); }
            if (pFD_pFDpCD->getRegion() != FD) { cout << "\033[33m\n\npFDpCD: nFD is not in the FD! Exiting...\n\n", exit(0); }
            if (pCD_pFDpCD->getRegion() != CD) { cout << "\033[33m\n\npFDpCD: pCD is not in the CD! Exiting...\n\n", exit(0); }
            if (Protons_ind.size() != 2) { cout << "\033[33m\n\npFDpCD: Protons_ind.size() is different than 2! Exiting...\n\n", exit(0); }

            if (protons[Protons_ind.at(0)]->getRegion() == protons[Protons_ind.at(1)]->getRegion()) { cout << "\033[33m\n\npFDpCD: Protons are in the same region! Exiting...\n\n", exit(0); }

            if (e_pFDpCD->getRegion() != FD) { cout << "\033[33m\n\npFDpCD: Electron is not in the FD! Exiting...\n\n", exit(0); }
            if (pFD_pFDpCD->getRegion() != FD) { cout << "\033[33m\n\npFDpCD: pFD is not in the FD! Exiting...\n\n", exit(0); }
            if (pCD_pFDpCD->getRegion() != CD) { cout << "\033[33m\n\npFDpCD: pCD is not in the CD! Exiting...\n\n", exit(0); }

            CodeDebugger.SafetyCheck_basic_event_selection(__FILE__, __LINE__, "pFDpCD", Kplus, Kminus, Piplus_ind, Piminus_ind, Electron_ind, deuterons);

            // Setting pFDpCD analysis variables
            double ProtonMomBKC_pFDpCD = pFD_pFDpCD->getP();  // proton momentum before smearing or kinematical cuts

            TVector3 P_e_pFDpCD_3v, q_pFDpCD_3v, P_pFD_pFDpCD_3v, P_pCD_pFDpCD_3v;
            TVector3 P_miss_pFDpCD_3v, P_tot_pFDpCD_3v, P_rel_pFDpCD_3v, P_max_pFDpCD_3v, P_pL_pFDpCD_3v, P_pR_pFDpCD_3v;
            TVector3 P_pL_minus_q_pFDpCD_v3, P_tot_minus_q_pFDpCD_v3;
            TVector3 P_T_e_pFDpCD_3v, P_T_L_pFDpCD_3v, P_T_tot_pFDpCD_3v, dP_T_L_pFDpCD_3v, dP_T_tot_pFDpCD_3v;
            TLorentzVector P_tot_mu_pFDpCD_4v, P_rel_mu_pFDpCD_4v;

            P_e_pFDpCD_3v.SetMagThetaPhi(e_pFDpCD->getP(), e_pFDpCD->getTheta(), e_pFDpCD->getPhi());              // electron 3 momentum
            q_pFDpCD_3v = TVector3(Pvx - P_e_pFDpCD_3v.Px(), Pvy - P_e_pFDpCD_3v.Py(), Pvz - P_e_pFDpCD_3v.Pz());  // 3 momentum transfer
            P_T_e_pFDpCD_3v = TVector3(P_e_pFDpCD_3v.Px(), P_e_pFDpCD_3v.Py(), 0);                                 // electron transverse momentum
            P_pFD_pFDpCD_3v.SetMagThetaPhi(nRes.PSmear(apply_nucleon_SmearAndCorr, ProtonMomBKC_pFDpCD), pFD_pFDpCD->getTheta(),
                                           pFD_pFDpCD->getPhi());                                              // pFD 3 momentum
            P_pCD_pFDpCD_3v.SetMagThetaPhi(pCD_pFDpCD->getP(), pCD_pFDpCD->getTheta(), pCD_pFDpCD->getPhi());  // pCD 3 momentum

            double E_e_pFDpCD = sqrt(m_e * m_e + P_e_pFDpCD_3v.Mag2()), omega_pFDpCD = beamE - E_e_pFDpCD;
            double W_pFDpCD = sqrt((omega_pFDpCD + m_p) * (omega_pFDpCD + m_p) - q_pFDpCD_3v.Mag2());
            double E_pL_pFDpCD, E_pR_pFDpCD;
            double Theta_p_e_p_tot_pFDpCD, Theta_q_p_tot_pFDpCD, Theta_P_pL_minus_q_pR_pFDpCD, Theta_q_p_L_pFDpCD, Theta_q_p_R_pFDpCD, Theta_q_pFD_pFDpCD, Theta_q_pCD_pFDpCD;
            double dAlpha_T_L_pFDpCD, dAlpha_T_tot_pFDpCD, dPhi_T_L_pFDpCD, dPhi_T_tot_pFDpCD, Ecal_pFDpCD;
            double EoP_e_pFDpCD = (e_pFDpCD->cal(PCAL)->getEnergy() + e_pFDpCD->cal(ECIN)->getEnergy() + e_pFDpCD->cal(ECOUT)->getEnergy()) / P_e_pFDpCD_3v.Mag();
            double Vx_e_pFDpCD = e_pFDpCD->par()->getVx(), Vy_e_pFDpCD = e_pFDpCD->par()->getVy(), Vz_e_pFDpCD = e_pFDpCD->par()->getVz();

            /* Setting Q2 */
            TLorentzVector e_out_pFDpCD, Q_pFDpCD;
            double Q2_pFDpCD;
            e_out_pFDpCD.SetPxPyPzE(e_pFDpCD->par()->getPx(), e_pFDpCD->par()->getPy(), e_pFDpCD->par()->getPz(), E_e_pFDpCD);
            Q_pFDpCD = beam - e_out_pFDpCD;  // definition of 4-momentum transfer
            Q2_pFDpCD = fabs(Q_pFDpCD.Mag2());
            double xB_pFDpCD = Q2_pFDpCD / (2 * m_p * omega_pFDpCD);  // TODO: ask Adi which mass should I use here

            // Determining leading, recoil protons and their angles (pFDpCD)
            /* Determining leading and recoil particles (leading = particle with greater momentum) */
            if (P_pFD_pFDpCD_3v.Mag() >= P_pCD_pFDpCD_3v.Mag()) {
                P_pL_pFDpCD_3v = P_pFD_pFDpCD_3v;
                P_pR_pFDpCD_3v = P_pCD_pFDpCD_3v;
            } else {
                P_pL_pFDpCD_3v = P_pCD_pFDpCD_3v;
                P_pR_pFDpCD_3v = P_pFD_pFDpCD_3v;
            }

            /* Determining higher momentum proton: */
            if (P_pL_pFDpCD_3v.Mag() >= P_pR_pFDpCD_3v.Mag()) {
                P_max_pFDpCD_3v = P_pL_pFDpCD_3v;
            } else {
                P_max_pFDpCD_3v = P_pR_pFDpCD_3v;
            }

            /* Setting particle angles */
            double Theta_L_pFDpCD = P_pL_pFDpCD_3v.Theta() * 180.0 / pi, Phi_L_pFDpCD = P_pL_pFDpCD_3v.Phi() * 180.0 / pi;
            // Theta_L_pFDpCD, Phi_L_pFDpCD in deg
            double Theta_R_pFDpCD = P_pR_pFDpCD_3v.Theta() * 180.0 / pi, Phi_R_pFDpCD = P_pR_pFDpCD_3v.Phi() * 180.0 / pi;
            // Theta_R_pFDpCD, Phi_R_pFDpCD in deg
            double dPhi_hit_pFDpCD = CalcdPhi(Phi_L_pFDpCD - Phi_R_pFDpCD);
            double Theta_pFD_pCD_pFDpCD = acos((P_pL_pFDpCD_3v.Px() * P_pR_pFDpCD_3v.Px() + P_pL_pFDpCD_3v.Py() * P_pR_pFDpCD_3v.Py() + P_pL_pFDpCD_3v.Pz() * P_pR_pFDpCD_3v.Pz()) /
                                               (P_pL_pFDpCD_3v.Mag() * P_pR_pFDpCD_3v.Mag())) *
                                          180.0 / pi;  // Theta_pFD_pCD_pFDpCD in deg

            /* Setting total and relative momenta */
            P_tot_pFDpCD_3v = TVector3(P_pL_pFDpCD_3v.Px() + P_pR_pFDpCD_3v.Px(), P_pL_pFDpCD_3v.Py() + P_pR_pFDpCD_3v.Py(),
                                       P_pL_pFDpCD_3v.Pz() + P_pR_pFDpCD_3v.Pz());  // P_tot = P_pL + P_pR
            P_rel_pFDpCD_3v = TVector3((P_pL_pFDpCD_3v.Px() - P_pR_pFDpCD_3v.Px()) / 2, (P_pL_pFDpCD_3v.Py() - P_pR_pFDpCD_3v.Py()) / 2,
                                       (P_pL_pFDpCD_3v.Pz() - P_pR_pFDpCD_3v.Pz()) / 2);  // P_rel = (P_pL - P_pR) / 2
            P_pL_minus_q_pFDpCD_v3 = TVector3(P_pL_pFDpCD_3v.Px() - q_pFDpCD_3v.Px(), P_pL_pFDpCD_3v.Py() - q_pFDpCD_3v.Py(),
                                              P_pL_pFDpCD_3v.Pz() - q_pFDpCD_3v.Pz());  // P_pL - q
            P_tot_minus_q_pFDpCD_v3 = TVector3(P_tot_pFDpCD_3v.Px() - q_pFDpCD_3v.Px(), P_tot_pFDpCD_3v.Py() - q_pFDpCD_3v.Py(),
                                               P_tot_pFDpCD_3v.Pz() - q_pFDpCD_3v.Pz());  // P_tot - q

            /* Setting particle angles */
            double Theta_e_pFDpCD = e_pFDpCD->getTheta() * 180.0 / pi, Phi_e_pFDpCD = e_pFDpCD->getPhi() * 180.0 / pi;
            // Theta_e_pFDpCD, Phi_e_pFDpCD in deg
            double Theta_pFD_pFDpCD = pFD_pFDpCD->getTheta() * 180.0 / pi, Phi_pFD_pFDpCD = pFD_pFDpCD->getPhi() * 180.0 / pi;
            // Theta_pFD_pFDpCD, Phi_pFD_pFDpCD in deg
            double Theta_pCD_pFDpCD = pCD_pFDpCD->getTheta() * 180.0 / pi, Phi_pCD_pFDpCD = pCD_pFDpCD->getPhi() * 180.0 / pi;
            // Theta_pCD_pFDpCD, Phi_pCD_pFDpCD in deg
            double Theta_tot_pFDpCD = P_tot_pFDpCD_3v.Theta() * 180.0 / pi, Phi_tot_pFDpCD = P_tot_pFDpCD_3v.Phi() * 180.0 / pi;  // in deg
            double Theta_rel_pFDpCD = P_rel_pFDpCD_3v.Theta() * 180.0 / pi, Phi_rel_pFDpCD = P_rel_pFDpCD_3v.Phi() * 180.0 / pi;  // in deg

            /* Weights -> before proton shifting; because proton detection is good! */
            double Weight_pFDpCD = wMaps_master.GetWeight(apply_kinematical_weights, "Proton", ProtonMomBKC_pFDpCD, Theta_pFD_pFDpCD, Phi_pFD_pFDpCD);

            // Setting kinematical cuts ---------------------------------------------------------------------------------------------------------------------------------

            // Setting kinematical cuts
            /* Protons have lower momentum threshold & we don't lose good protons -> proton smearing before kin cuts */
            bool FD_Theta_Cut_pFDpCD = ((P_pFD_pFDpCD_3v.Theta() * 180.0 / pi) <= FD_nucleon_theta_cut.GetUpperCut());
            bool FD_Momentum_Cut_pFDpCD = ((P_pFD_pFDpCD_3v.Mag() <= FD_nucleon_momentum_cut.GetUpperCut()) && (P_pFD_pFDpCD_3v.Mag() >= FD_nucleon_momentum_cut.GetLowerCut()));
            // Momentum kin cut after proton smearing
            bool e_withinFC_pFDpCD = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Electron", P_e_pFDpCD_3v.Mag(),
                                                              P_e_pFDpCD_3v.Theta() * 180.0 / pi, P_e_pFDpCD_3v.Phi() * 180.0 / pi);
            bool pFD_withinFC_pFDpCD = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Proton", ProtonMomBKC_pFDpCD,
                                                                P_pFD_pFDpCD_3v.Theta() * 180.0 / pi, P_pFD_pFDpCD_3v.Phi() * 180.0 / pi, Calc_eff_overlapping_FC);

            bool Pass_Kin_Cuts_pFDpCD =
                ((!apply_kinematical_cuts || (FD_Theta_Cut_pFDpCD && FD_Momentum_Cut_pFDpCD)) && (!apply_fiducial_cuts || (e_withinFC_pFDpCD && pFD_withinFC_pFDpCD)));

            //  Fillings pFDpCD histograms ------------------------------------------------------------------------------------------------------------------------------

            // Fillings pFDpCD histograms
            if (Pass_Kin_Cuts_pFDpCD) {
                ++num_of_events_pFDpCD;

                // Filling cut parameters histograms (pFDpCD)
                /* Filling Nphe plots (pFDpCD) */
                hNphe_pFDpCD_FD.hFill(e_pFDpCD->che(clas12::HTCC)->getNphe(), Weight_pFDpCD);

                /* Filling Chi2 histograms (pFDpCD) */
                hChi2_Electron_pFDpCD_FD.hFill(e_pFDpCD->par()->getChi2Pid(), Weight_pFDpCD);
                hChi2_Proton_pFDpCD_CD.hFill(pCD_pFDpCD->par()->getChi2Pid(), Weight_pFDpCD);
                hChi2_Proton_pFDpCD_FD.hFill(pFD_pFDpCD->par()->getChi2Pid(), Weight_pFDpCD);

                // Filling dVx, dVy, dVz histograms (pFDpCD)

                // All protons (pFDpCD)
                for (auto &p : protons) {
                    double Vx_p_pFDpCD = p->par()->getVx(), Vy_p_pFDpCD = p->par()->getVy(), Vz_p_pFDpCD = p->par()->getVz();
                    double dVx_pFDpCD = Vx_p_pFDpCD - Vx_e_pFDpCD, dVy_pFDpCD = Vy_p_pFDpCD - Vy_e_pFDpCD, dVz_pFDpCD = Vz_p_pFDpCD - Vz_e_pFDpCD;

                    hdVx_pFDpCD.hFill(dVx_pFDpCD, Weight_pFDpCD), hdVy_pFDpCD.hFill(dVy_pFDpCD, Weight_pFDpCD), hdVz_pFDpCD.hFill(dVz_pFDpCD, Weight_pFDpCD);
                }  // end of loop over protons vector

                // ID. pFD and pCD protons (pFDpCD)
                double Vx_pFD_pFDpCD = pFD_pFDpCD->par()->getVx(), Vy_pFD_pFDpCD = pFD_pFDpCD->par()->getVy(), Vz_pFD_pFDpCD = pFD_pFDpCD->par()->getVz();
                double dVx_pFD_pFDpCD = Vx_pFD_pFDpCD - Vx_e_pFDpCD, dVy_pFD_pFDpCD = Vy_pFD_pFDpCD - Vy_e_pFDpCD, dVz_pFD_pFDpCD = Vz_pFD_pFDpCD - Vz_e_pFDpCD;
                double Vx_pCD_pFDpCD = pCD_pFDpCD->par()->getVx(), Vy_pCD_pFDpCD = pCD_pFDpCD->par()->getVy(), Vz_pCD_pFDpCD = pCD_pFDpCD->par()->getVz();
                double dVx_pCD_pFDpCD = Vx_pCD_pFDpCD - Vx_e_pFDpCD, dVy_pCD_pFDpCD = Vy_pCD_pFDpCD - Vy_e_pFDpCD, dVz_pCD_pFDpCD = Vz_pCD_pFDpCD - Vz_e_pFDpCD;

                hdVx_pFD_pFDpCD.hFill(dVx_pFD_pFDpCD, Weight_pFDpCD);
                hdVy_pFD_pFDpCD.hFill(dVy_pFD_pFDpCD, Weight_pFDpCD);
                hdVz_pFD_pFDpCD.hFill(dVz_pFD_pFDpCD, Weight_pFDpCD);
                hdVx_pCD_pFDpCD.hFill(dVx_pCD_pFDpCD, Weight_pFDpCD);
                hdVy_pCD_pFDpCD.hFill(dVy_pCD_pFDpCD, Weight_pFDpCD);
                hdVz_pCD_pFDpCD.hFill(dVz_pCD_pFDpCD, Weight_pFDpCD);

                /* Filling SF histograms (pFDpCD) */
                hSF_pFDpCD_FD.hFill(EoP_e_pFDpCD, Weight_pFDpCD), hSF_VS_P_e_pFDpCD_FD.hFill(P_e_pFDpCD_3v.Mag(), EoP_e_pFDpCD, Weight_pFDpCD);

                /* Filling fiducial plots (pFDpCD) */
                hVcal_VS_EoP_pFDpCD_PCAL.hFill(e_pFDpCD->cal(clas12::PCAL)->getLv(), EoP_e_pFDpCD, Weight_pFDpCD);
                hWcal_VS_EoP_pFDpCD_PCAL.hFill(e_pFDpCD->cal(clas12::PCAL)->getLw(), EoP_e_pFDpCD, Weight_pFDpCD);

                // Filling momentum histograms (pFDpCD)

                // Electron momentum (pFDpCD)
                hP_e_APID_pFDpCD_FD.hFill(P_e_pFDpCD_3v.Mag(), Weight_pFDpCD);  // after mom. th.

                for (int i = 0; i < electrons.size(); i++) {
                    if (electrons[i]->getRegion() == FD) { hP_e_BPID_pFDpCD_FD.hFill(P_e_pFDpCD_3v.Mag(), Weight_pFDpCD); }  // before mom. th.
                }

                // Proton momentum (pFDpCD)
                hP_p_APIDandPS_pFDpCD_FD.hFill(P_pFD_pFDpCD_3v.Mag(), Weight_pFDpCD);  // after mom. th. & smearing

                for (int &i : Protons_ind) {
                    if (protons[i]->getRegion() == CD) {
                        hP_p_APID_pFDpCD_CD.hFill(protons[i]->getP(), Weight_pFDpCD);  // after mom. th.
                    } else if (protons[i]->getRegion() == FD) {
                        hP_p_APID_pFDpCD_FD.hFill(protons[i]->getP(), Weight_pFDpCD);  // after mom. th.
                    }
                }

                for (int i = 0; i < protons.size(); i++) {
                    if (protons[i]->getRegion() == CD) {
                        hP_p_BPID_pFDpCD_CD.hFill(protons[i]->getP(), Weight_pFDpCD);  // before mom. th.
                    } else if (protons[i]->getRegion() == FD) {
                        hP_p_BPID_pFDpCD_FD.hFill(protons[i]->getP(), Weight_pFDpCD);  // before mom. th.
                    }
                }

                // Piplus momentum (pFDpCD)
                for (int &i : Piplus_ind) {
                    if (piplus[i]->getRegion() == CD) {
                        hP_piplus_APID_pFDpCD_CD.hFill(piplus[i]->getP(), Weight_pFDpCD);  // after mom. th.
                    } else if (piplus[i]->getRegion() == FD) {
                        hP_piplus_APID_pFDpCD_FD.hFill(piplus[i]->getP(), Weight_pFDpCD);  // after mom. th.
                    }
                }

                for (int i = 0; i < piplus.size(); i++) {
                    if (piplus[i]->getRegion() == CD) {
                        hP_piplus_BPID_pFDpCD_CD.hFill(piplus[i]->getP(), Weight_pFDpCD);  // before mom. th.
                    } else if (piplus[i]->getRegion() == FD) {
                        hP_piplus_BPID_pFDpCD_FD.hFill(piplus[i]->getP(), Weight_pFDpCD);  // before mom. th.
                    }
                }

                // Piminus momentum (pFDpCD)
                for (int &i : Piminus_ind) {
                    if (piminus[i]->getRegion() == CD) {
                        hP_piminus_APID_pFDpCD_CD.hFill(piminus[i]->getP(), Weight_pFDpCD);  // after mom. th.
                    } else if (piminus[i]->getRegion() == FD) {
                        hP_piminus_APID_pFDpCD_FD.hFill(piminus[i]->getP(), Weight_pFDpCD);  // after mom. th.
                    }
                }

                for (int i = 0; i < piminus.size(); i++) {
                    if (piminus[i]->getRegion() == CD) {
                        hP_piminus_BPID_pFDpCD_CD.hFill(piminus[i]->getP(), Weight_pFDpCD);  // before mom. th.
                    } else if (piminus[i]->getRegion() == FD) {
                        hP_piminus_BPID_pFDpCD_FD.hFill(piminus[i]->getP(), Weight_pFDpCD);  // before mom. th.
                    }
                }

                // Photon momentum (pFDpCD)
                for (int &i : PhotonsFD_ind) { hP_ph_APID_pFDpCD_FD.hFill(allParticles[i]->getP(), Weight_pFDpCD); }  // after mom. th.

                for (int &i : ReDef_photons_FD) { hP_ph_BPID_pFDpCD_FD.hFill(allParticles[i]->getP(), Weight_pFDpCD); }  // before mom. th.

                // Neutron momentum (pFDpCD)
                for (int &i : NeutronsFD_ind) { hP_n_APID_pFDpCD_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_pFDpCD); }  // after mom. th.

                for (int &i : ReDef_neutrons_FD) { hP_n_BPID_pFDpCD_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_pFDpCD); }  // before mom. th.

                // Filling Beta vs. P plots (pFDpCD)

                // Beta vs. P from electrons (pFDpCD, CD & FD)
                if (e_pFDpCD->getRegion() == FD) {
                    hBeta_vs_P_pFDpCD_FD.hFill(e_pFDpCD->getP(), e_pFDpCD->par()->getBeta(), Weight_pFDpCD);
                    hBeta_vs_P_pFDpCD_Electrons_Only_FD.hFill(e_pFDpCD->getP(), e_pFDpCD->par()->getBeta(), Weight_pFDpCD);

                    if (e_pFDpCD->par()->getCharge() == 1) {
                        hBeta_vs_P_positive_part_pFDpCD_FD.hFill(e_pFDpCD->getP(), e_pFDpCD->par()->getBeta(), Weight_pFDpCD);
                    } else if (e_pFDpCD->par()->getCharge() == 0) {
                        hBeta_vs_P_neutral_part_pFDpCD_FD.hFill(e_pFDpCD->getP(), e_pFDpCD->par()->getBeta(), Weight_pFDpCD);
                    } else if (e_pFDpCD->par()->getCharge() == -1) {
                        hBeta_vs_P_negative_part_pFDpCD_FD.hFill(e_pFDpCD->getP(), e_pFDpCD->par()->getBeta(), Weight_pFDpCD);
                    }
                }

                // Beta vs. P from protons (pFDpCD, CD & FD)
                for (int i = 0; i < protons.size(); i++) {
                    if (protons[i]->getRegion() == CD) {
                        hBeta_vs_P_pFDpCD_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_pFDpCD_Protons_Only_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_positive_part_pFDpCD_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_pFDpCD);
                    } else if (protons[i]->getRegion() == FD) {
                        hBeta_vs_P_pFDpCD_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_pFDpCD_Protons_Only_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_positive_part_pFDpCD_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight_pFDpCD);
                    }
                }  // end of loop over protons vector

                hBeta_vs_P_pFD_pFDpCD_Protons_Only_FD.hFill(pFD_pFDpCD->getP(), pFD_pFDpCD->par()->getBeta(), Weight_pFDpCD);
                hBeta_vs_P_pCD_pFDpCD_Protons_Only_CD.hFill(pCD_pFDpCD->getP(), pCD_pFDpCD->par()->getBeta(), Weight_pFDpCD);

                // Beta vs. P from other particles (pFDpCD, CD & FD)
                /* This is for self-consistency. Contributions from other particles should be zero */

                // Beta vs. P from Kplus (pFDpCD, CD & FD)
                for (int i = 0; i < Kplus.size(); i++) {
                    if (Kplus[i]->getRegion() == CD) {
                        hBeta_vs_P_pFDpCD_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_positive_part_pFDpCD_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight_pFDpCD);
                    } else if (Kplus[i]->getRegion() == FD) {
                        hBeta_vs_P_pFDpCD_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_positive_part_pFDpCD_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight_pFDpCD);
                    }
                }  // end of loop over Kplus vector

                // Beta vs. P from Kminus (pFDpCD, CD & FD)
                for (int i = 0; i < Kminus.size(); i++) {
                    if (Kminus[i]->getRegion() == CD) {
                        hBeta_vs_P_pFDpCD_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_negative_part_pFDpCD_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight_pFDpCD);
                    } else if (Kminus[i]->getRegion() == FD) {
                        hBeta_vs_P_pFDpCD_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_negative_part_pFDpCD_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight_pFDpCD);
                    }
                }  // end of loop over Kminus vector

                // Beta vs. P from piplus (pFDpCD, CD & FD)
                for (int i = 0; i < piplus.size(); i++) {
                    if (piplus[i]->getRegion() == CD) {
                        hBeta_vs_P_pFDpCD_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_positive_part_pFDpCD_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight_pFDpCD);
                    } else if (piplus[i]->getRegion() == FD) {
                        hBeta_vs_P_pFDpCD_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_positive_part_pFDpCD_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight_pFDpCD);
                    }
                }  // end of loop over piplus vector

                // Beta vs. P from piminus (pFDpCD, CD & FD)
                for (int i = 0; i < piminus.size(); i++) {
                    if (piminus[i]->getRegion() == CD) {
                        hBeta_vs_P_pFDpCD_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_negative_part_pFDpCD_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight_pFDpCD);
                    } else if (piminus[i]->getRegion() == FD) {
                        hBeta_vs_P_pFDpCD_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight_pFDpCD);
                        hBeta_vs_P_negative_part_pFDpCD_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight_pFDpCD);
                    }
                }  // end of loop over piminus vector

                // Filling electron histograms (pFDpCD)
                hTheta_e_All_Int_pFDpCD_FD->Fill(Theta_e_pFDpCD, Weight_pFDpCD);
                hPhi_e_All_Int_pFDpCD_FD->Fill(Phi_e_pFDpCD, Weight_pFDpCD);
                hPhi_e_VS_P_e_pFDpCD_FD->Fill(P_e_pFDpCD_3v.Mag(), Phi_e_pFDpCD, Weight_pFDpCD);
                hPhi_e_VS_W_pFDpCD_FD->Fill(W_pFDpCD, Phi_e_pFDpCD, Weight_pFDpCD);
                hTheta_e_VS_Phi_e_pFDpCD_FD->Fill(Phi_e_pFDpCD, Theta_e_pFDpCD, Weight_pFDpCD);
                hTheta_e_VS_P_e_pFDpCD_FD->Fill(P_e_pFDpCD_3v.Mag(), Theta_e_pFDpCD, Weight_pFDpCD);
                hTheta_e_VS_W_pFDpCD_FD->Fill(W_pFDpCD, Theta_e_pFDpCD, Weight_pFDpCD);

                hE_e_All_Int_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                hE_e_VS_Theta_e_All_Int_pFDpCD_FD->Fill(Theta_e_pFDpCD, E_e_pFDpCD, Weight_pFDpCD);

                hET_All_Ang_All_Int_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);

                if ((Theta_e_pFDpCD >= 14.0) && (Theta_e_pFDpCD <= 16.0)) {
                    hET15_All_Int_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);
                    hE_e_15_All_Int_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                }

                if (qel) {
                    hTheta_e_QEL_pFDpCD_FD->Fill(Theta_e_pFDpCD, Weight_pFDpCD);
                    hPhi_e_QEL_pFDpCD_FD->Fill(Phi_e_pFDpCD, Weight_pFDpCD);
                    hE_e_QEL_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                    hE_e_VS_Theta_e_QEL_pFDpCD_FD->Fill(Theta_e_pFDpCD, E_e_pFDpCD, Weight_pFDpCD);

                    hET_All_Ang_QEL_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);

                    if ((Theta_e_pFDpCD >= 14.0) && (Theta_e_pFDpCD <= 16.0)) {
                        hET15_QEL_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);
                        hE_e_15_QEL_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                    }
                } else if (mec) {
                    hTheta_e_MEC_pFDpCD_FD->Fill(Theta_e_pFDpCD, Weight_pFDpCD);
                    hPhi_e_MEC_pFDpCD_FD->Fill(Phi_e_pFDpCD, Weight_pFDpCD);
                    hE_e_MEC_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                    hE_e_VS_Theta_e_MEC_pFDpCD_FD->Fill(Theta_e_pFDpCD, E_e_pFDpCD, Weight_pFDpCD);

                    hET_All_Ang_MEC_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);
                    if ((Theta_e_pFDpCD >= 14.0) && (Theta_e_pFDpCD <= 16.0)) {
                        hET15_MEC_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);
                        hE_e_15_MEC_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                    }
                } else if (res) {
                    hTheta_e_RES_pFDpCD_FD->Fill(Theta_e_pFDpCD, Weight_pFDpCD);
                    hPhi_e_RES_pFDpCD_FD->Fill(Phi_e_pFDpCD, Weight_pFDpCD);
                    hE_e_RES_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                    hE_e_VS_Theta_e_RES_pFDpCD_FD->Fill(Theta_e_pFDpCD, E_e_pFDpCD, Weight_pFDpCD);

                    hET_All_Ang_RES_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);

                    if ((Theta_e_pFDpCD >= 14.0) && (Theta_e_pFDpCD <= 16.0)) {
                        hET15_RES_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);
                        hE_e_15_RES_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                    }
                } else if (dis) {
                    hTheta_e_DIS_pFDpCD_FD->Fill(Theta_e_pFDpCD, Weight_pFDpCD);
                    hPhi_e_DIS_pFDpCD_FD->Fill(Phi_e_pFDpCD, Weight_pFDpCD);
                    hE_e_DIS_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                    hE_e_VS_Theta_e_DIS_pFDpCD_FD->Fill(Theta_e_pFDpCD, E_e_pFDpCD, Weight_pFDpCD);

                    hET_All_Ang_DIS_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);

                    if ((Theta_e_pFDpCD >= 14.0) && (Theta_e_pFDpCD <= 16.0)) {
                        hET15_DIS_pFDpCD_FD->Fill(beamE - E_e_pFDpCD, Weight_pFDpCD);
                        hE_e_15_DIS_pFDpCD_FD->Fill(E_e_pFDpCD, Weight_pFDpCD);
                    }
                }

                // Fill momentum transfer plots (pFDpCD)
                FillByInt(hQ2_pFDpCD, hQ2_QEL_pFDpCD, hQ2_MEC_pFDpCD, hQ2_RES_pFDpCD, hQ2_DIS_pFDpCD, qel, mec, res, dis, Q2_pFDpCD, Weight_pFDpCD);
                FillByInt(hq_3v_pFDpCD, hq_3v_QEL_pFDpCD, hq_3v_MEC_pFDpCD, hq_3v_RES_pFDpCD, hq_3v_DIS_pFDpCD, qel, mec, res, dis, q_pFDpCD_3v.Mag(), Weight_pFDpCD);

                hQ2_VS_W_pFDpCD->Fill(W_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                hQ2_VS_q_3v_pFDpCD->Fill(q_pFDpCD_3v.Mag(), Q2_pFDpCD, Weight_pFDpCD);
                hQ2_VS_omega_pFDpCD->Fill(omega_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                hq_3v_VS_omega_pFDpCD->Fill(omega_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);

                if (qel) {
                    hQ2_VS_W_QEL_pFDpCD->Fill(W_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                    hQ2_VS_q_3v_QEL_pFDpCD->Fill(q_pFDpCD_3v.Mag(), Q2_pFDpCD, Weight_pFDpCD);
                    hQ2_VS_omega_QEL_pFDpCD->Fill(omega_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                    hq_3v_VS_omega_QEL_pFDpCD->Fill(omega_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);
                } else if (mec) {
                    hQ2_VS_W_MEC_pFDpCD->Fill(W_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                    hQ2_VS_q_3v_MEC_pFDpCD->Fill(q_pFDpCD_3v.Mag(), Q2_pFDpCD, Weight_pFDpCD);
                    hQ2_VS_omega_MEC_pFDpCD->Fill(omega_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                    hq_3v_VS_omega_MEC_pFDpCD->Fill(omega_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);
                } else if (res) {
                    hQ2_VS_W_RES_pFDpCD->Fill(W_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                    hQ2_VS_q_3v_RES_pFDpCD->Fill(q_pFDpCD_3v.Mag(), Q2_pFDpCD, Weight_pFDpCD);
                    hQ2_VS_omega_RES_pFDpCD->Fill(omega_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                    hq_3v_VS_omega_RES_pFDpCD->Fill(omega_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);
                } else if (dis) {
                    hQ2_VS_W_DIS_pFDpCD->Fill(W_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                    hQ2_VS_q_3v_DIS_pFDpCD->Fill(q_pFDpCD_3v.Mag(), Q2_pFDpCD, Weight_pFDpCD);
                    hQ2_VS_omega_DIS_pFDpCD->Fill(omega_pFDpCD, Q2_pFDpCD, Weight_pFDpCD);
                    hq_3v_VS_omega_DIS_pFDpCD->Fill(omega_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);
                }

                // Filling angle plots (pFDpCD)

                // FD proton angle plots (pFDpCD)
                FillByInt(hTheta_pFD_All_Int_pFDpCD_FD, hTheta_pFD_QEL_pFDpCD_FD, hTheta_pFD_MEC_pFDpCD_FD, hTheta_pFD_RES_pFDpCD_FD, hTheta_pFD_DIS_pFDpCD_FD, qel, mec, res, dis,
                          Theta_pFD_pFDpCD, Weight_pFDpCD);
                hTheta_pFD_VS_P_pFD_pFDpCD_FD->Fill(P_pFD_pFDpCD_3v.Mag(), Theta_pFD_pFDpCD, Weight_pFDpCD);
                hTheta_pFD_VS_W_pFDpCD_FD->Fill(W_pFDpCD, Theta_pFD_pFDpCD, Weight_pFDpCD);

                FillByInt(hPhi_pFD_All_Int_pFDpCD_FD, hPhi_pFD_QEL_pFDpCD_FD, hPhi_pFD_MEC_pFDpCD_FD, hPhi_pFD_RES_pFDpCD_FD, hPhi_pFD_DIS_pFDpCD_FD, qel, mec, res, dis, Phi_pFD_pFDpCD,
                          Weight_pFDpCD);
                hPhi_pFD_VS_P_pFD_pFDpCD_FD->Fill(P_pFD_pFDpCD_3v.Mag(), Phi_pFD_pFDpCD, Weight_pFDpCD);
                hPhi_pFD_VS_W_pFDpCD_FD->Fill(W_pFDpCD, Phi_pFD_pFDpCD, Weight_pFDpCD);

                hTheta_pFD_VS_Phi_pFD_pFDpCD_FD->Fill(Phi_pFD_pFDpCD, Theta_pFD_pFDpCD, Weight_pFDpCD);

                // CD proton angle plots (pFDpCD)
                FillByInt(hTheta_pCD_All_Int_pFDpCD_CD, hTheta_pCD_QEL_pFDpCD_CD, hTheta_pCD_MEC_pFDpCD_CD, hTheta_pCD_RES_pFDpCD_CD, hTheta_pCD_DIS_pFDpCD_CD, qel, mec, res, dis,
                          Theta_pCD_pFDpCD, Weight_pFDpCD);
                hTheta_pCD_VS_P_pCD_pFDpCD_CD->Fill(P_pCD_pFDpCD_3v.Mag(), Theta_pCD_pFDpCD, Weight_pFDpCD);
                hTheta_pCD_VS_W_pFDpCD_CD->Fill(W_pFDpCD, Theta_pCD_pFDpCD, Weight_pFDpCD);

                FillByInt(hPhi_pCD_All_Int_pFDpCD_CD, hPhi_pCD_QEL_pFDpCD_CD, hPhi_pCD_MEC_pFDpCD_CD, hPhi_pCD_RES_pFDpCD_CD, hPhi_pCD_DIS_pFDpCD_CD, qel, mec, res, dis, Phi_pCD_pFDpCD,
                          Weight_pFDpCD);
                hPhi_pCD_VS_P_pCD_pFDpCD_CD->Fill(P_pCD_pFDpCD_3v.Mag(), Phi_pCD_pFDpCD, Weight_pFDpCD);
                hPhi_pCD_VS_W_pFDpCD_CD->Fill(W_pFDpCD, Phi_pCD_pFDpCD, Weight_pFDpCD);

                hTheta_pCD_VS_Phi_pCD_pFDpCD_CD->Fill(Phi_pCD_pFDpCD, Theta_pCD_pFDpCD, Weight_pFDpCD);

                // Total momentum angle plots (pFDpCD)
                FillByInt(hTheta_tot_All_Int_pFDpCD, hTheta_tot_QEL_pFDpCD, hTheta_tot_MEC_pFDpCD, hTheta_tot_RES_pFDpCD, hTheta_tot_DIS_pFDpCD, qel, mec, res, dis, Theta_tot_pFDpCD,
                          Weight_pFDpCD);
                hTheta_tot_VS_P_tot_pFDpCD->Fill(P_tot_pFDpCD_3v.Mag(), Theta_tot_pFDpCD, Weight_pFDpCD);
                hTheta_tot_VS_W_pFDpCD->Fill(W_pFDpCD, Theta_tot_pFDpCD, Weight_pFDpCD);

                FillByInt(hPhi_tot_All_Int_pFDpCD, hPhi_tot_QEL_pFDpCD, hPhi_tot_MEC_pFDpCD, hPhi_tot_RES_pFDpCD, hPhi_tot_DIS_pFDpCD, qel, mec, res, dis, Phi_tot_pFDpCD, Weight_pFDpCD);
                hPhi_tot_VS_P_tot_pFDpCD->Fill(P_tot_pFDpCD_3v.Mag(), Phi_tot_pFDpCD, Weight_pFDpCD);
                hPhi_tot_VS_W_pFDpCD->Fill(W_pFDpCD, Phi_tot_pFDpCD, Weight_pFDpCD);

                hTheta_tot_VS_Phi_tot_pFDpCD->Fill(Phi_tot_pFDpCD, Theta_tot_pFDpCD, Weight_pFDpCD);

                // Relative momentum angle plots (pFDpCD)
                FillByInt(hTheta_rel_All_Int_pFDpCD, hTheta_rel_QEL_pFDpCD, hTheta_rel_MEC_pFDpCD, hTheta_rel_RES_pFDpCD, hTheta_rel_DIS_pFDpCD, qel, mec, res, dis, Theta_rel_pFDpCD,
                          Weight_pFDpCD);
                hTheta_rel_VS_P_rel_pFDpCD->Fill(P_rel_pFDpCD_3v.Mag(), Theta_rel_pFDpCD, Weight_pFDpCD);
                hTheta_rel_VS_W_pFDpCD->Fill(W_pFDpCD, Theta_rel_pFDpCD, Weight_pFDpCD);

                FillByInt(hPhi_rel_All_Int_pFDpCD, hPhi_rel_QEL_pFDpCD, hPhi_rel_MEC_pFDpCD, hPhi_rel_RES_pFDpCD, hPhi_rel_DIS_pFDpCD, qel, mec, res, dis, Phi_rel_pFDpCD, Weight_pFDpCD);
                hPhi_rel_VS_P_rel_pFDpCD->Fill(P_rel_pFDpCD_3v.Mag(), Phi_rel_pFDpCD, Weight_pFDpCD);
                hPhi_rel_VS_W_pFDpCD->Fill(W_pFDpCD, Phi_rel_pFDpCD, Weight_pFDpCD);

                hTheta_rel_VS_Phi_rel_pFDpCD->Fill(Phi_rel_pFDpCD, Theta_rel_pFDpCD, Weight_pFDpCD);

                // Fill neutron multiplicity plots (pFDpCD)
                pid.FillNeutMultiPlots(allParticles, electrons, Weight_pFDpCD, beamE, Neutron_veto_cut.GetLowerCutConst(), hNeut_Multi_By_Redef_BPID_BV_pFDpCD_FD,
                                       hNeut_Multi_By_Redef_BPID_AV_pFDpCD_FD, ReDef_neutrons_FD, hNeut_Multi_By_Redef_APID_BV_pFDpCD_FD, hNeut_Multi_By_Redef_APID_AV_pFDpCD_FD,
                                       NeutronsFD_ind);

                // Fill W (pFDpCD)
                FillByInt(hW_All_Int_pFDpCD, hW_QEL_pFDpCD, hW_MEC_pFDpCD, hW_RES_pFDpCD, hW_DIS_pFDpCD, qel, mec, res, dis, W_pFDpCD, Weight_pFDpCD);
                hW_VS_q_3v_pFDpCD->Fill(W_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hW_VS_omega_pFDpCD->Fill(W_pFDpCD, omega_pFDpCD, Weight_pFDpCD);

                if (qel) {
                    hW_VS_q_3v_QEL_pFDpCD->Fill(W_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hW_VS_omega_QEL_pFDpCD->Fill(W_pFDpCD, omega_pFDpCD, Weight_pFDpCD);
                } else if (mec) {
                    hW_VS_q_3v_MEC_pFDpCD->Fill(W_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hW_VS_omega_MEC_pFDpCD->Fill(W_pFDpCD, omega_pFDpCD, Weight_pFDpCD);
                } else if (res) {
                    hW_VS_q_3v_RES_pFDpCD->Fill(W_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hW_VS_omega_RES_pFDpCD->Fill(W_pFDpCD, omega_pFDpCD, Weight_pFDpCD);
                } else if (dis) {
                    hW_VS_q_3v_DIS_pFDpCD->Fill(W_pFDpCD, q_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hW_VS_omega_DIS_pFDpCD->Fill(W_pFDpCD, omega_pFDpCD, Weight_pFDpCD);
                }

                hP_tot_pFDpCD.hFill(P_tot_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hP_rel_pFDpCD.hFill(P_rel_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hP_tot_vs_P_rel_pFDpCD.hFill(P_tot_pFDpCD_3v.Mag(), P_rel_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hP_tot_minus_q_pFDpCD.hFill(P_tot_minus_q_pFDpCD_v3.Mag(), Weight_pFDpCD);
                hP_tot_minus_q_vs_P_tot_pFDpCD.hFill(P_tot_pFDpCD_3v.Mag(), P_tot_minus_q_pFDpCD_v3.Mag(), Weight_pFDpCD);
                hP_tot_minus_q_vs_q_pFDpCD.hFill(q_pFDpCD_3v.Mag(), P_tot_minus_q_pFDpCD_v3.Mag(), Weight_pFDpCD);

                hP_pFD_pFDpCD.hFill(P_pFD_pFDpCD_3v.Mag(), Weight_pFDpCD);  // FD proton (pFDpCD)
                hP_pCD_pFDpCD.hFill(P_pCD_pFDpCD_3v.Mag(), Weight_pFDpCD);  // CD proton (pFDpCD)
                hP_pL_pFDpCD.hFill(P_pL_pFDpCD_3v.Mag(), Weight_pFDpCD);    // Leading proton (pFDpCD)
                hP_pR_pFDpCD.hFill(P_pR_pFDpCD_3v.Mag(), Weight_pFDpCD);    // Recoil proton (pFDpCD)

                hP_pFD_VS_W_pFDpCD->Fill(W_pFDpCD, P_pFD_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hP_pCD_VS_W_pFDpCD->Fill(W_pFDpCD, P_pCD_pFDpCD_3v.Mag(), Weight_pFDpCD);

                if (qel) {
                    hP_pFD_VS_W_QEL_pFDpCD->Fill(W_pFDpCD, P_pFD_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hP_pCD_VS_W_QEL_pFDpCD->Fill(W_pFDpCD, P_pCD_pFDpCD_3v.Mag(), Weight_pFDpCD);
                } else if (mec) {
                    hP_pFD_VS_W_MEC_pFDpCD->Fill(W_pFDpCD, P_pFD_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hP_pCD_VS_W_MEC_pFDpCD->Fill(W_pFDpCD, P_pCD_pFDpCD_3v.Mag(), Weight_pFDpCD);
                } else if (res) {
                    hP_pFD_VS_W_RES_pFDpCD->Fill(W_pFDpCD, P_pFD_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hP_pCD_VS_W_RES_pFDpCD->Fill(W_pFDpCD, P_pCD_pFDpCD_3v.Mag(), Weight_pFDpCD);
                } else if (dis) {
                    hP_pFD_VS_W_DIS_pFDpCD->Fill(W_pFDpCD, P_pFD_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hP_pCD_VS_W_DIS_pFDpCD->Fill(W_pFDpCD, P_pCD_pFDpCD_3v.Mag(), Weight_pFDpCD);
                }

                hP_pL_vs_P_pR_pFDpCD.hFill(P_pL_pFDpCD_3v.Mag(), P_pR_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hP_pFD_vs_P_pCD_pFDpCD.hFill(P_pFD_pFDpCD_3v.Mag(), P_pCD_pFDpCD_3v.Mag(), Weight_pFDpCD);

                P_T_L_pFDpCD_3v = TVector3(P_pL_pFDpCD_3v.Px(), P_pL_pFDpCD_3v.Py(), 0);  // transverse part of P_pFD
                P_T_tot_pFDpCD_3v = TVector3(P_pFD_pFDpCD_3v.Px() + P_pCD_pFDpCD_3v.Px(), P_pFD_pFDpCD_3v.Py() + P_pCD_pFDpCD_3v.Py(), 0);
                // transverse part of P_tot
                dP_T_L_pFDpCD_3v = TVector3(P_e_pFDpCD_3v.Px() + P_T_L_pFDpCD_3v.Px(), P_e_pFDpCD_3v.Py() + P_T_L_pFDpCD_3v.Py(), 0);
                dP_T_tot_pFDpCD_3v = TVector3(P_e_pFDpCD_3v.Px() + P_tot_pFDpCD_3v.Px(), P_e_pFDpCD_3v.Py() + P_tot_pFDpCD_3v.Py(), 0);

                Theta_p_e_p_tot_pFDpCD = acos((P_e_pFDpCD_3v.Px() * P_tot_pFDpCD_3v.Px() + P_e_pFDpCD_3v.Py() * P_tot_pFDpCD_3v.Py() + P_e_pFDpCD_3v.Pz() * P_tot_pFDpCD_3v.Pz()) /
                                              (P_e_pFDpCD_3v.Mag() * P_tot_pFDpCD_3v.Mag())) *
                                         180.0 / pi;  // Theta_p_e_p_tot_pFDpCD in deg
                hTheta_p_e_p_tot_pFDpCD->Fill(Theta_p_e_p_tot_pFDpCD, Weight_pFDpCD);
                hTheta_p_e_p_tot_vs_W_pFDpCD->Fill(W_pFDpCD, Theta_p_e_p_tot_pFDpCD, Weight_pFDpCD);

                // Filling Theta_pFD_pCD plots (pFDpCD)

                // General Theta_pFD_pCD plots (pFDpCD)
                FillByInt(hTheta_pFD_pCD_All_Int_pFDpCD, hTheta_pFD_pCD_QEL_pFDpCD, hTheta_pFD_pCD_MEC_pFDpCD, hTheta_pFD_pCD_RES_pFDpCD, hTheta_pFD_pCD_DIS_pFDpCD, qel, mec, res, dis,
                          Theta_pFD_pCD_pFDpCD, Weight_pFDpCD);
                hTheta_pFD_pCD_vs_W_pFDpCD->Fill(W_pFDpCD, Theta_pFD_pCD_pFDpCD, Weight_pFDpCD);

                // Plots for small Theta_pFD_pCD (pFDpCD)
                if (Theta_pFD_pCD_pFDpCD < 20.) {
                    hTheta_pFD_vs_theta_pCD_for_Theta_pFD_pCD_20_pFDpCD->Fill(Theta_pCD_pFDpCD, Theta_pFD_pFDpCD, Weight_pFDpCD);
                    hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_pFDpCD->Fill(dPhi_hit_pFDpCD, Weight_pFDpCD);
                    hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_pFDpCD->Fill(dPhi_hit_pFDpCD, Weight_pFDpCD);
                }

                hTheta_pFD_vs_theta_pCD_forall_Theta_pFD_pCD_pFDpCD->Fill(Theta_pCD_pFDpCD, Theta_pFD_pFDpCD, Weight_pFDpCD);
                hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_pFDpCD->Fill(dPhi_hit_pFDpCD, Weight_pFDpCD);
                hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_pFDpCD->Fill(dPhi_hit_pFDpCD, Weight_pFDpCD);

                if ((fabs(Theta_pFD_pFDpCD - Theta_pFD_cuts_pFDpCD.GetMean()) < Theta_pFD_cuts_pFDpCD.GetUpperCut()) &&
                    (fabs(Theta_pCD_pFDpCD - Theta_pCD_cuts_pFDpCD.GetMean()) < Theta_pCD_cuts_pFDpCD.GetUpperCut())) {
                    hdPhi_pFD_pCD_for_small_dTheta_pFDpCD->Fill(dPhi_hit_pFDpCD, Weight_pFDpCD);
                    hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_pFDpCD->Fill(dPhi_hit_pFDpCD, Weight_pFDpCD);
                }

                Theta_q_p_tot_pFDpCD = acos((q_pFDpCD_3v.Px() * P_tot_pFDpCD_3v.Px() + q_pFDpCD_3v.Py() * P_tot_pFDpCD_3v.Py() + q_pFDpCD_3v.Pz() * P_tot_pFDpCD_3v.Pz()) /
                                            (q_pFDpCD_3v.Mag() * P_tot_pFDpCD_3v.Mag())) *
                                       180.0 / pi;  // Theta_q_p_tot_pFDpCD in deg
                hTheta_q_p_tot_pFDpCD->Fill(Theta_q_p_tot_pFDpCD, Weight_pFDpCD);
                hTheta_q_p_tot_vs_W_pFDpCD->Fill(W_pFDpCD, Theta_q_p_tot_pFDpCD, Weight_pFDpCD);
                hTheta_q_p_max_pFDpCD->Fill((P_max_pFDpCD_3v.Angle(q_pFDpCD_3v) * 180.0 / pi), Weight_pFDpCD);

                Theta_P_pL_minus_q_pR_pFDpCD =
                    acos((P_pL_minus_q_pFDpCD_v3.Px() * P_pR_pFDpCD_3v.Px() + P_pL_minus_q_pFDpCD_v3.Py() * P_pR_pFDpCD_3v.Py() + P_pL_minus_q_pFDpCD_v3.Pz() * P_pR_pFDpCD_3v.Pz()) /
                         (P_pL_minus_q_pFDpCD_v3.Mag() * P_pR_pFDpCD_3v.Mag())) *
                    180.0 / pi;  // Theta_P_pL_minus_q_pR_pFDpCD in deg
                Theta_q_p_L_pFDpCD = acos((q_pFDpCD_3v.Px() * P_pL_pFDpCD_3v.Px() + q_pFDpCD_3v.Py() * P_pL_pFDpCD_3v.Py() + q_pFDpCD_3v.Pz() * P_pL_pFDpCD_3v.Pz()) /
                                          (q_pFDpCD_3v.Mag() * P_pL_pFDpCD_3v.Mag())) *
                                     180.0 / pi;  // Theta_q_p_L_pFDpCD = Theta_q_p_1_pFDpCD in deg
                Theta_q_p_R_pFDpCD = acos((q_pFDpCD_3v.Px() * P_pR_pFDpCD_3v.Px() + q_pFDpCD_3v.Py() * P_pR_pFDpCD_3v.Py() + q_pFDpCD_3v.Pz() * P_pR_pFDpCD_3v.Pz()) /
                                          (q_pFDpCD_3v.Mag() * P_pR_pFDpCD_3v.Mag())) *
                                     180.0 / pi;  // Theta_q_p_R_pFDpCD = Theta_q_p_2_pFDpCD in deg
                Theta_q_pFD_pFDpCD = acos((q_pFDpCD_3v.Px() * P_pFD_pFDpCD_3v.Px() + q_pFDpCD_3v.Py() * P_pFD_pFDpCD_3v.Py() + q_pFDpCD_3v.Pz() * P_pFD_pFDpCD_3v.Pz()) /
                                          (q_pFDpCD_3v.Mag() * P_pFD_pFDpCD_3v.Mag())) *
                                     180.0 / pi;  // Theta_q_p_L_pFDpCD = Theta_q_p_1_pFDpCD in deg
                Theta_q_pCD_pFDpCD = acos((q_pFDpCD_3v.Px() * P_pCD_pFDpCD_3v.Px() + q_pFDpCD_3v.Py() * P_pCD_pFDpCD_3v.Py() + q_pFDpCD_3v.Pz() * P_pCD_pFDpCD_3v.Pz()) /
                                          (q_pFDpCD_3v.Mag() * P_pCD_pFDpCD_3v.Mag())) *
                                     180.0 / pi;  // Theta_q_p_R_pFDpCD = Theta_q_p_2_pFDpCD in deg
                hTheta_P_pL_minus_q_pR_pFDpCD->Fill(Theta_P_pL_minus_q_pR_pFDpCD, Weight_pFDpCD);
                hTheta_P_pL_minus_q_pR_vs_W_pFDpCD->Fill(W_pFDpCD, Theta_P_pL_minus_q_pR_pFDpCD, Weight_pFDpCD);
                hTheta_q_p_L_pFDpCD->Fill(Theta_q_p_L_pFDpCD, Weight_pFDpCD);
                hTheta_q_p_R_pFDpCD->Fill(Theta_q_p_R_pFDpCD, Weight_pFDpCD);
                hTheta_q_pFD_pFDpCD->Fill(Theta_q_pFD_pFDpCD, Weight_pFDpCD);
                hTheta_q_pCD_pFDpCD->Fill(Theta_q_pCD_pFDpCD, Weight_pFDpCD);
                hTheta_q_p_L_vs_W_pFDpCD->Fill(W_pFDpCD, Theta_q_p_L_pFDpCD, Weight_pFDpCD);
                hTheta_q_p_R_vs_W_pFDpCD->Fill(W_pFDpCD, Theta_q_p_R_pFDpCD, Weight_pFDpCD);
                hTheta_q_pFD_vs_W_pFDpCD->Fill(W_pFDpCD, Theta_q_pFD_pFDpCD, Weight_pFDpCD);
                hTheta_q_pCD_vs_W_pFDpCD->Fill(W_pFDpCD, Theta_q_pCD_pFDpCD, Weight_pFDpCD);
                hTheta_q_p_L_vs_p_L_q_pFDpCD->Fill(P_pL_pFDpCD_3v.Mag() / q_pFDpCD_3v.Mag(), Theta_q_p_L_pFDpCD, Weight_pFDpCD);
                hTheta_q_p_L_vs_Theta_q_p_R_pFDpCD->Fill(Theta_q_p_L_pFDpCD, Theta_q_p_R_pFDpCD, Weight_pFDpCD);
                hTheta_q_pFD_vs_Theta_q_pCD_pFDpCD->Fill(Theta_q_pFD_pFDpCD, Theta_q_pCD_pFDpCD, Weight_pFDpCD);

                E_pL_pFDpCD = sqrt(m_p * m_p + P_pL_pFDpCD_3v.Mag2());
                E_pR_pFDpCD = sqrt(m_p * m_p + P_pR_pFDpCD_3v.Mag2());

                P_tot_mu_pFDpCD_4v.SetPxPyPzE(P_pL_pFDpCD_3v.Px() + P_pR_pFDpCD_3v.Px(), P_pL_pFDpCD_3v.Py() + P_pR_pFDpCD_3v.Py(), P_pL_pFDpCD_3v.Pz() + P_pR_pFDpCD_3v.Pz(),
                                              E_pL_pFDpCD + E_pR_pFDpCD);
                P_rel_mu_pFDpCD_4v.SetPxPyPzE((P_pL_pFDpCD_3v.Px() - P_pR_pFDpCD_3v.Px()) / 2, (P_pL_pFDpCD_3v.Py() - P_pR_pFDpCD_3v.Py()) / 2,
                                              (P_pL_pFDpCD_3v.Pz() - P_pR_pFDpCD_3v.Pz()) / 2, (E_pL_pFDpCD - E_pR_pFDpCD) / 2);
                hP_tot_mu_pFDpCD.hFill(P_tot_mu_pFDpCD_4v.Mag(), Weight_pFDpCD);
                hP_rel_mu_pFDpCD.hFill(P_rel_mu_pFDpCD_4v.Mag(), Weight_pFDpCD);
                hP_tot_mu_vs_P_rel_mu_pFDpCD.hFill(P_tot_mu_pFDpCD_4v.Mag(), P_rel_mu_pFDpCD_4v.Mag(), Weight_pFDpCD);

                // Filling Ecal plots
                Ecal_pFDpCD = E_e_pFDpCD + (E_pL_pFDpCD - m_p) + (E_pR_pFDpCD - m_p);

                hEcal_All_Int_pFDpCD->Fill(Ecal_pFDpCD, Weight_pFDpCD);  // Fill Ecal for all interactions

                if (qel) {
                    hEcal_QEL_pFDpCD->Fill(Ecal_pFDpCD, Weight_pFDpCD);  // Fill Ecal for QEL only
                } else if (mec) {
                    hEcal_MEC_pFDpCD->Fill(Ecal_pFDpCD, Weight_pFDpCD);  // Fill Ecal for MEC only
                } else if (res) {
                    hEcal_RES_pFDpCD->Fill(Ecal_pFDpCD, Weight_pFDpCD);  // Fill Ecal for RES only
                } else if (dis) {
                    hEcal_DIS_pFDpCD->Fill(Ecal_pFDpCD, Weight_pFDpCD);  // Fill Ecal for DIS only
                }

                if (Ecal_pFDpCD > beamE) {
                    hEcal_vs_P_e_test_pFDpCD->Fill(P_e_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_Theta_e_test_pFDpCD->Fill(Theta_e_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_Phi_e_test_pFDpCD->Fill(Phi_e_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_P_pFD_test_pFDpCD->Fill(P_pFD_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_P_pCD_test_pFDpCD->Fill(P_pCD_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_Theta_pFD_test_pFDpCD->Fill(Theta_pFD_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_Phi_pFD_test_pFDpCD->Fill(Phi_pFD_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_Theta_pCD_test_pFDpCD->Fill(Theta_pCD_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_Phi_pCD_test_pFDpCD->Fill(Phi_pCD_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                }

                hdP_T_L_pFDpCD->Fill(dP_T_L_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hdP_T_L_vs_W_pFDpCD->Fill(W_pFDpCD, dP_T_L_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hdP_T_tot_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hdP_T_tot_vs_W_pFDpCD->Fill(W_pFDpCD, dP_T_tot_pFDpCD_3v.Mag(), Weight_pFDpCD);

                dAlpha_T_L_pFDpCD = acos(-(P_e_pFDpCD_3v.Px() * dP_T_L_pFDpCD_3v.Px() + P_e_pFDpCD_3v.Py() * dP_T_L_pFDpCD_3v.Py() + P_e_pFDpCD_3v.Pz() * dP_T_L_pFDpCD_3v.Pz()) /
                                         (P_T_e_pFDpCD_3v.Mag() * dP_T_L_pFDpCD_3v.Mag())) *
                                    180.0 / pi;
                // dP_T_L_pFDpCD_3v.Pz() = 0; dAlpha_T_L_pFDpCD in deg
                dAlpha_T_tot_pFDpCD = acos(-(P_e_pFDpCD_3v.Px() * dP_T_tot_pFDpCD_3v.Px() + P_e_pFDpCD_3v.Py() * dP_T_tot_pFDpCD_3v.Py() + P_e_pFDpCD_3v.Pz() * dP_T_tot_pFDpCD_3v.Pz()) /
                                           (P_T_e_pFDpCD_3v.Mag() * dP_T_tot_pFDpCD_3v.Mag())) *
                                      180.0 / pi;
                // dP_T_tot_pFDpCD_3v.Pz() = 0; dAlpha_T_tot_pFDpCD in deg
                hdAlpha_T_L_pFDpCD->Fill(dAlpha_T_L_pFDpCD, Weight_pFDpCD);
                hdAlpha_T_L_vs_W_pFDpCD->Fill(W_pFDpCD, dAlpha_T_L_pFDpCD, Weight_pFDpCD);
                hdAlpha_T_tot_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Weight_pFDpCD);
                hdAlpha_T_tot_vs_W_pFDpCD->Fill(W_pFDpCD, dAlpha_T_tot_pFDpCD, Weight_pFDpCD);

                if (qel) {
                    hdP_T_tot_QEL_Only_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hdAlpha_T_tot_QEL_Only_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Weight_pFDpCD);
                } else if (mec) {
                    hdP_T_tot_MEC_Only_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hdAlpha_T_tot_MEC_Only_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Weight_pFDpCD);
                } else if (res) {
                    hdP_T_tot_RES_Only_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hdAlpha_T_tot_RES_Only_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Weight_pFDpCD);
                } else if (dis) {
                    hdP_T_tot_DIS_Only_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Weight_pFDpCD);
                    hdAlpha_T_tot_DIS_Only_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Weight_pFDpCD);
                }

                hdP_T_L_vs_dAlpha_T_L_pFDpCD->Fill(dAlpha_T_L_pFDpCD, dP_T_L_pFDpCD_3v.Mag(), Weight_pFDpCD);
                hdP_T_tot_vs_dAlpha_T_tot_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, dP_T_tot_pFDpCD_3v.Mag(), Weight_pFDpCD);

                dPhi_T_L_pFDpCD = acos(-(P_T_e_pFDpCD_3v.Px() * P_T_L_pFDpCD_3v.Px() + P_T_e_pFDpCD_3v.Py() * P_T_L_pFDpCD_3v.Py() + P_T_e_pFDpCD_3v.Pz() * P_T_L_pFDpCD_3v.Pz()) /
                                       (P_T_e_pFDpCD_3v.Mag() * P_T_L_pFDpCD_3v.Mag())) *
                                  180.0 / pi;
                // P_T_L_pFDpCD_3v.Pz() = 0; dPhi_T_L_pFDpCD in deg
                dPhi_T_tot_pFDpCD = acos(-(P_T_e_pFDpCD_3v.Px() * P_T_tot_pFDpCD_3v.Px() + P_T_e_pFDpCD_3v.Py() * P_T_tot_pFDpCD_3v.Py() + P_T_e_pFDpCD_3v.Pz() * P_T_tot_pFDpCD_3v.Pz()) /
                                         (P_T_e_pFDpCD_3v.Mag() * P_T_tot_pFDpCD_3v.Mag())) *
                                    180.0 / pi;
                // P_T_tot_pFDpCD_3v.Pz() = 0; dPhi_T_tot_pFDpCD in deg
                hdPhi_T_L_pFDpCD->Fill(dPhi_T_L_pFDpCD, Weight_pFDpCD);
                hdPhi_T_tot_pFDpCD->Fill(dPhi_T_tot_pFDpCD, Weight_pFDpCD);

                hEcal_vs_P_e_pFDpCD->Fill(P_e_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_Theta_e_pFDpCD->Fill(Theta_e_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_Phi_e_pFDpCD->Fill(Phi_e_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_P_pFD_pFDpCD->Fill(P_pFD_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_P_pCD_pFDpCD->Fill(P_pCD_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_Theta_pFD_pFDpCD->Fill(Theta_pFD_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_Phi_pFD_pFDpCD->Fill(Phi_pFD_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_Theta_pCD_pFDpCD->Fill(Theta_pCD_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_Phi_pCD_pFDpCD->Fill(Phi_pCD_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_dAlpha_T_L_pFDpCD->Fill(dAlpha_T_L_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_dAlpha_T_tot_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_dP_T_L_pFDpCD->Fill(dP_T_L_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_dP_T_tot_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                hEcal_vs_W_pFDpCD->Fill(W_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);

                if (qel) {
                    hEcal_vs_dAlpha_T_tot_QEL_Only_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_dP_T_tot_QEL_Only_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                } else if (mec) {
                    hEcal_vs_dAlpha_T_tot_MEC_Only_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_dP_T_tot_MEC_Only_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                } else if (res) {
                    hEcal_vs_dAlpha_T_tot_RES_Only_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_dP_T_tot_RES_Only_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                } else if (dis) {
                    hEcal_vs_dAlpha_T_tot_DIS_Only_pFDpCD->Fill(dAlpha_T_tot_pFDpCD, Ecal_pFDpCD, Weight_pFDpCD);
                    hEcal_vs_dP_T_tot_DIS_Only_pFDpCD->Fill(dP_T_tot_pFDpCD_3v.Mag(), Ecal_pFDpCD, Weight_pFDpCD);
                }
            }
        }  // end of 1epFDpCD & pFDpCD cuts if

        //  nFDpCD --------------------------------------------------------------------------------------------------------------------------------------------------------------

        CodeDebugger.PrintStepTester(DebuggerMode);

        // nFDpCD

        // Configure if event is nFDpCD
        /* nFDpCD event selection: nFDpCD = one id. proton in the CD, any number of id. FD neutrons (we look at the leading nFD) and any number of neutrons, other
                                            neutrals and particles with pdg=0.*/
        bool one_CDproton_nFDpCD = (Protons_ind.size() == 1 && protons[Protons_ind.at(0)]->getRegion() == CD);
        // there's only one id. proton + this proton is in the CD
        bool at_least_one_FDneutron_nFDpCD = (NeutronsFD_ind_mom_max != -1);  // for NeutronsFD_ind_mom_max = -1 we don't have any nFD
        bool event_selection_nFDpCD = (basic_event_selection && one_CDproton_nFDpCD && at_least_one_FDneutron_nFDpCD);

        bool apply_TL_nFDpCD_ES = (!Rec_wTL_ES || TL_Event_Selection_nFDpCD);

        if (calculate_nFDpCD && event_selection_nFDpCD && apply_TL_nFDpCD_ES) {
            // for nFDpCD calculations
            ++num_of_events_nFDpCD;  // logging #(events) w/ 1enFDpCD

            // Setting particle vectors (for code organization) (nFDpCD)
            /* Defining initial particle vectors: */

            // Setting FD neutron index (nFDpCD)
            int nFD_ind_nFDpCD;

            if (ES_by_leading_FDneutron) {
                nFD_ind_nFDpCD = NeutronsFD_ind_mom_max;
            } else {
                nFD_ind_nFDpCD = NeutronsFD_ind.at(0);
            }

            region_part_ptr e_nFDpCD = electrons[Electron_ind.at(0)];
            region_part_ptr nFD_nFDpCD = allParticles[nFD_ind_nFDpCD];  // neutron with the largest momentum magnitude
            region_part_ptr pCD_nFDpCD = protons[Protons_ind.at(0)];

            // Safety checks (nFDpCD)
            /* Safety check that we are looking at nFDpCD */
            // TODO: reorgenize these Safety checks
            if (e_nFDpCD->getRegion() != FD) { cout << "\033[33m\n\nnFDpCD: Electron is not in the FD! Exiting...\n\n", exit(0); }
            if (nFD_nFDpCD->getRegion() != FD) { cout << "\033[33m\n\nnFDpCD: nFD is not in the FD! Exiting...\n\n", exit(0); }
            if (pCD_nFDpCD->getRegion() != CD) { cout << "\033[33m\n\nnFDpCD: pCD is not in the CD! Exiting...\n\n", exit(0); }

            if (!(Enable_FD_photons || (PhotonsFD_ind.size() == 0))) {
                cout << "\033[33m\n\nnFDpCD: PhotonsFD_ind.size() is non-zero (" << PhotonsFD_ind.size() << ")! Exiting...\n\n", exit(0);
            }

            if (Protons_ind.size() != 1) { cout << "\033[33m\n\nnFDpCD: Protons_ind.size() is different than 2! Exiting...\n\n", exit(0); }

            CodeDebugger.SafetyCheck_basic_event_selection(__FILE__, __LINE__, "nFDpCD", Kplus, Kminus, Piplus_ind, Piminus_ind, Electron_ind, deuterons);

            for (int &i : NeutronsFD_ind) {
                bool NeutronInPCAL_nFDpCD = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);  // PCAL hit
                if (NeutronInPCAL_nFDpCD) { cout << "\033[33m\n\nnFDpCD: a neutron have been found with a PCAL hit! Exiting...\n\n", exit(0); }
                if (!((allParticles[i]->par()->getPid() == 2112) || (allParticles[i]->par()->getPid() == 22))) {
                    cout << "\033[33m\n\nnFDpCD: A neutron PDG is not 2112 or 22 (" << allParticles[i]->par()->getPid() << ")! Exiting...\n\n", exit(0);
                }
            }

            for (int &i : PhotonsFD_ind) {
                bool PhotonInPCAL_nFDpCD = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);  // PCAL hit
                if (!PhotonInPCAL_nFDpCD) { cout << "\033[33m\n\nnFDpCD: a photon have been found without a PCAL hit! Exiting...\n\n", exit(0); }
                if (allParticles[i]->par()->getPid() != 22) {
                    cout << "\033[33m\n\nnFDpCD: A photon PDG is not 2112 or 22 (" << allParticles[i]->par()->getPid() << ")! Exiting...\n\n", exit(0);
                }
            }

            // Setting nFDpCD analysis variables
            double NeutronMomBKC_nFDpCD = pid.GetFDNeutronP(nFD_nFDpCD, apply_nucleon_cuts);  // neutron momentum before shift for kin cuts

            TVector3 P_e_nFDpCD_3v, q_nFDpCD_3v, P_nFD_nFDpCD_3v, P_pCD_nFDpCD_3v;
            TVector3 P_miss_nFDpCD_3v, P_tot_nFDpCD_3v, P_rel_nFDpCD_3v, P_nL_nFDpCD_3v, P_nR_nFDpCD_3v;
            TVector3 P_nL_minus_q_nFDpCD_v3, P_tot_minus_q_nFDpCD_v3;
            TVector3 P_T_e_nFDpCD_3v, P_T_L_nFDpCD_3v, P_T_tot_nFDpCD_3v, dP_T_L_nFDpCD_3v, dP_T_tot_nFDpCD_3v;
            TLorentzVector P_tot_mu_nFDpCD_4v, P_rel_mu_nFDpCD_4v;

            P_e_nFDpCD_3v.SetMagThetaPhi(e_nFDpCD->getP(), e_nFDpCD->getTheta(), e_nFDpCD->getPhi());              // electron 3 momentum
            q_nFDpCD_3v = TVector3(Pvx - P_e_nFDpCD_3v.Px(), Pvy - P_e_nFDpCD_3v.Py(), Pvz - P_e_nFDpCD_3v.Pz());  // 3 momentum transfer
            P_T_e_nFDpCD_3v = TVector3(P_e_nFDpCD_3v.Px(), P_e_nFDpCD_3v.Py(), 0);                                 // electron transverse momentum
            P_nFD_nFDpCD_3v.SetMagThetaPhi(nRes.NCorr(apply_nucleon_SmearAndCorr, NeutronMomBKC_nFDpCD), nFD_nFDpCD->getTheta(),
                                           nFD_nFDpCD->getPhi());                                              // FD neutron 3 momentum
            P_pCD_nFDpCD_3v.SetMagThetaPhi(pCD_nFDpCD->getP(), pCD_nFDpCD->getTheta(), pCD_nFDpCD->getPhi());  // CD proton 3 momentum

            double E_e_nFDpCD = sqrt(m_e * m_e + P_e_nFDpCD_3v.Mag2()), omega_nFDpCD = beamE - E_e_nFDpCD;
            double W_nFDpCD = sqrt((omega_nFDpCD + m_p) * (omega_nFDpCD + m_p) - q_nFDpCD_3v.Mag2());
            double E_nFD_nFDpCD, E_pCD_nFDpCD, E_nL_nFDpCD, E_nR_nFDpCD;
            double Theta_p_e_p_tot_nFDpCD, Theta_q_p_tot_nFDpCD, Theta_P_nL_minus_q_nR_nFDpCD, Theta_q_p_L_nFDpCD, Theta_q_p_R_nFDpCD, Theta_q_nFD_nFDpCD, Theta_q_pCD_nFDpCD;
            double dAlpha_T_L_nFDpCD, dAlpha_T_tot_nFDpCD, dPhi_T_L_nFDpCD, dPhi_T_tot_nFDpCD, Ecal_nFDpCD;
            double EoP_e_nFDpCD = (e_nFDpCD->cal(PCAL)->getEnergy() + e_nFDpCD->cal(ECIN)->getEnergy() + e_nFDpCD->cal(ECOUT)->getEnergy()) / P_e_nFDpCD_3v.Mag();
            double Vx_e_nFDpCD = e_nFDpCD->par()->getVx(), Vy_e_nFDpCD = e_nFDpCD->par()->getVy(), Vz_e_nFDpCD = e_nFDpCD->par()->getVz();

            /* Setting Q2 */
            TLorentzVector e_out_nFDpCD, Q_nFDpCD;
            double Q2_nFDpCD;
            e_out_nFDpCD.SetPxPyPzE(e_nFDpCD->par()->getPx(), e_nFDpCD->par()->getPy(), e_nFDpCD->par()->getPz(), E_e_nFDpCD);
            Q_nFDpCD = beam - e_out_nFDpCD;  // definition of 4-momentum transfer
            Q2_nFDpCD = fabs(Q_nFDpCD.Mag2());
            double xB_nFDpCD = Q2_nFDpCD / (2 * m_p * omega_nFDpCD);  // TODO: ask Adi which mass should I use here

            // Determining leading, recoil protons and their angles (nFDpCD)
            double m_L, m_R;  // Leading and recoil nucleon masses

            /* Determining leading and recoil particles (leading = particle with greater momentum) */
            if (P_nFD_nFDpCD_3v.Mag() >= P_pCD_nFDpCD_3v.Mag()) {
                P_nL_nFDpCD_3v = P_nFD_nFDpCD_3v;  // FD neutron is leading
                P_nR_nFDpCD_3v = P_pCD_nFDpCD_3v;
                m_L = m_n;  // FD neutron is leading
                m_R = m_p;
            } else {
                P_nL_nFDpCD_3v = P_pCD_nFDpCD_3v;  // CD proton is leading
                P_nR_nFDpCD_3v = P_nFD_nFDpCD_3v;
                m_L = m_p;  // CD proton is leading
                m_R = m_n;
            }

            /* Setting particle angles */
            double Theta_L_nFDpCD = P_nL_nFDpCD_3v.Theta() * 180.0 / pi, Phi_L_nFDpCD = P_nL_nFDpCD_3v.Phi() * 180.0 / pi;
            // Theta_L_nFDpCD, Phi_L_nFDpCD in deg
            double Theta_R_nFDpCD = P_nR_nFDpCD_3v.Theta() * 180.0 / pi, Phi_R_nFDpCD = P_nR_nFDpCD_3v.Phi() * 180.0 / pi;
            // Theta_R_nFDpCD, Phi_R_nFDpCD in deg
            double dPhi_hit_nFDpCD = Phi_L_nFDpCD - Phi_R_nFDpCD;
            double Theta_nFD_pCD_nFDpCD = acos((P_nFD_nFDpCD_3v.Px() * P_pCD_nFDpCD_3v.Px() + P_nFD_nFDpCD_3v.Py() * P_pCD_nFDpCD_3v.Py() + P_nFD_nFDpCD_3v.Pz() * P_pCD_nFDpCD_3v.Pz()) /
                                               (P_nFD_nFDpCD_3v.Mag() * P_pCD_nFDpCD_3v.Mag())) *
                                          180.0 / pi;  // Theta_nFD_pCD_nFDpCD in deg

            /* Setting total and relative momenta */
            P_tot_nFDpCD_3v = TVector3(P_nL_nFDpCD_3v.Px() + P_nR_nFDpCD_3v.Px(), P_nL_nFDpCD_3v.Py() + P_nR_nFDpCD_3v.Py(),
                                       P_nL_nFDpCD_3v.Pz() + P_nR_nFDpCD_3v.Pz());  // P_tot = P_nL + P_nR
            P_rel_nFDpCD_3v = TVector3((P_nL_nFDpCD_3v.Px() - P_nR_nFDpCD_3v.Px()) / 2, (P_nL_nFDpCD_3v.Py() - P_nR_nFDpCD_3v.Py()) / 2,
                                       (P_nL_nFDpCD_3v.Pz() - P_nR_nFDpCD_3v.Pz()) / 2);  // P_rel = (P_nL - P_nR) / 2
            P_nL_minus_q_nFDpCD_v3 = TVector3(P_nL_nFDpCD_3v.Px() - q_nFDpCD_3v.Px(), P_nL_nFDpCD_3v.Py() - q_nFDpCD_3v.Py(),
                                              P_nL_nFDpCD_3v.Pz() - q_nFDpCD_3v.Pz());  // P_nL - q
            P_tot_minus_q_nFDpCD_v3 = TVector3(P_tot_nFDpCD_3v.Px() - q_nFDpCD_3v.Px(), P_tot_nFDpCD_3v.Py() - q_nFDpCD_3v.Py(),
                                               P_tot_nFDpCD_3v.Pz() - q_nFDpCD_3v.Pz());  // P_tot - q

            /* Setting particle angles */
            double Theta_e_nFDpCD = e_nFDpCD->getTheta() * 180.0 / pi, Phi_e_nFDpCD = e_nFDpCD->getPhi() * 180.0 / pi;
            // Theta_e_nFDpCD, Phi_e_nFDpCD in deg
            double Theta_nFD_nFDpCD = nFD_nFDpCD->getTheta() * 180.0 / pi, Phi_nFD_nFDpCD = nFD_nFDpCD->getPhi() * 180.0 / pi;
            // Theta_nFD_nFDpCD, Phi_nFD_nFDpCD in deg
            double Theta_pCD_nFDpCD = pCD_nFDpCD->getTheta() * 180.0 / pi, Phi_pCD_nFDpCD = pCD_nFDpCD->getPhi() * 180.0 / pi;
            // Theta_pCD_nFDpCD, Phi_pCD_nFDpCD in deg
            double Theta_tot_nFDpCD = P_tot_nFDpCD_3v.Theta() * 180.0 / pi, Phi_tot_nFDpCD = P_tot_nFDpCD_3v.Phi() * 180.0 / pi;  // in deg
            double Theta_rel_nFDpCD = P_rel_nFDpCD_3v.Theta() * 180.0 / pi, Phi_rel_nFDpCD = P_rel_nFDpCD_3v.Phi() * 180.0 / pi;  // in deg

            /* Weights -> after neutron shifting; because we want to match the currected neutron momentum to the proton maps! */
            double Weight_nFDpCD = wMaps_master.GetWeight(apply_kinematical_weights, "Neutron", P_nFD_nFDpCD_3v.Mag(), Theta_nFD_nFDpCD, Phi_nFD_nFDpCD);

            // Fake FD neutrons handling (neutron veto) -----------------------------------------------------------------------------------------------------------------

            // Fake FD neutrons handling (neutron veto)
            int NeutronPDG_nFDpCD = nFD_nFDpCD->par()->getPid();

            bool NeutronInPCAL_nFDpCD = (nFD_nFDpCD->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool NeutronInECIN_nFDpCD = (nFD_nFDpCD->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool NeutronInECOUT_nFDpCD = (nFD_nFDpCD->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

            // Safety check (nFDpCD)
            /* Safety check that we are looking at good neutron (BEFORE VETO!!!) */
            if (nFD_nFDpCD->getRegion() != FD) { cout << "\033[33m\n\nnFDpCD: neutron is not in FD! Exiting...\n\n", exit(0); }
            if (!((NeutronPDG_nFDpCD == 22) || (NeutronPDG_nFDpCD == 2112))) {
                cout << "\033[33m\n\nnFDpCD: neutral PDG is not 2112 or 22 (" << NeutronPDG_nFDpCD << ")! Exiting...\n\n", exit(0);
            }
            if (NeutronInPCAL_nFDpCD) { cout << "\033[33m\n\nnFDpCD: neutron hit in PCAL! Exiting...\n\n", exit(0); }
            if (!(NeutronInECIN_nFDpCD || NeutronInECOUT_nFDpCD)) { cout << "\033[33m\n\nnFDpCD: no neutron hit in ECIN or ECOUT! Exiting...\n\n", exit(0); }

            TVector3 n_hit_nFDpCD_3v, e_hit_nFDpCD_3v;

            double nFD_hit_Theta_nFDpCD, nFD_hit_Phi_nFDpCD;
            double e_hit_Theta_nFDpCD, e_hit_Phi_nFDpCD, dTheta_hit_e_nFD_nFDpCD, dPhi_hit_e_nFD_nFDpCD;

            if (!NeutronInPCAL_nFDpCD && (NeutronInECIN_nFDpCD || NeutronInECOUT_nFDpCD)) {
                // if neutron did not hit PCAL, and hit either ECIN or ECOUT
                auto nFD_detlayer_nFDpCD = NeutronInECIN_nFDpCD ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

                // neutron ECIN/ECAL hit vector and angles:
                n_hit_nFDpCD_3v.SetXYZ(nFD_nFDpCD->cal(nFD_detlayer_nFDpCD)->getX(), nFD_nFDpCD->cal(nFD_detlayer_nFDpCD)->getY(), nFD_nFDpCD->cal(nFD_detlayer_nFDpCD)->getZ());
                nFD_hit_Theta_nFDpCD = n_hit_nFDpCD_3v.Theta() * 180 / pi, nFD_hit_Phi_nFDpCD = n_hit_nFDpCD_3v.Phi() * 180 / pi;

                if ((nFD_detlayer_nFDpCD == clas12::ECIN) && (e_nFDpCD->cal(clas12::ECIN)->getZ() != 0)) {
                    e_hit_nFDpCD_3v.SetXYZ(e_nFDpCD->cal(clas12::ECIN)->getX(), e_nFDpCD->cal(clas12::ECIN)->getY(), e_nFDpCD->cal(clas12::ECIN)->getZ());
                    e_hit_Theta_nFDpCD = e_hit_nFDpCD_3v.Theta() * 180 / pi, e_hit_Phi_nFDpCD = e_hit_nFDpCD_3v.Phi() * 180 / pi;
                } else if ((nFD_detlayer_nFDpCD == clas12::ECOUT) && (e_nFDpCD->cal(clas12::ECOUT)->getZ() != 0)) {
                    e_hit_nFDpCD_3v.SetXYZ(e_nFDpCD->cal(clas12::ECOUT)->getX(), e_nFDpCD->cal(clas12::ECOUT)->getY(), e_nFDpCD->cal(clas12::ECOUT)->getZ());
                    e_hit_Theta_nFDpCD = e_hit_nFDpCD_3v.Theta() * 180 / pi, e_hit_Phi_nFDpCD = e_hit_nFDpCD_3v.Phi() * 180 / pi;
                } else {
                    int trajlayer = (nFD_detlayer_nFDpCD == clas12::ECIN) ? 4 : 7;
                    e_hit_nFDpCD_3v.SetXYZ(e_nFDpCD->traj(clas12::ECAL, trajlayer)->getX(), e_nFDpCD->traj(clas12::ECAL, trajlayer)->getY(), e_nFDpCD->traj(clas12::ECAL, trajlayer)->getZ());
                    e_hit_Theta_nFDpCD = e_hit_nFDpCD_3v.Theta() * 180 / pi, e_hit_Phi_nFDpCD = e_hit_nFDpCD_3v.Phi() * 180 / pi;
                }

                dPhi_hit_e_nFD_nFDpCD = nFD_hit_Phi_nFDpCD - e_hit_Phi_nFDpCD, dTheta_hit_e_nFD_nFDpCD = nFD_hit_Theta_nFDpCD - e_hit_Theta_nFDpCD;

                // Shift dPhi (from symmetry)
                if (dPhi_hit_e_nFD_nFDpCD > 180) {
                    dPhi_hit_e_nFD_nFDpCD = nFD_hit_Phi_nFDpCD - e_hit_Phi_nFDpCD - 360;
                } else if (dPhi_hit_e_nFD_nFDpCD < -180) {
                    dPhi_hit_e_nFD_nFDpCD = nFD_hit_Phi_nFDpCD - e_hit_Phi_nFDpCD + 360;
                }

                // subtracting the angles between the neutron hit and electron hit to see if we have fake neutrons:
                hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_BV_nFDpCD.hFill(dPhi_hit_e_nFD_nFDpCD, dTheta_hit_e_nFD_nFDpCD, Weight_nFDpCD);
            }  // end of if neutron did not hit PCAL & hit either ECIN or ECOUT

            bool NeutronPassVeto_nFDpCD = pid.NeutronECAL_Cut_Veto(allParticles, electrons, beamE, nFD_ind_nFDpCD, Neutron_veto_cut.GetLowerCut());

            /* Log vetoed neutron values (for self-consistency) */
            if (!NeutronPassVeto_nFDpCD) { hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_Vetoed_Neutrons_nFDpCD.hFill(dPhi_hit_e_nFD_nFDpCD, dTheta_hit_e_nFD_nFDpCD, Weight_nFDpCD); }

            // Setting kinematical cuts ---------------------------------------------------------------------------------------------------------------------------------

            // Setting kinematical cuts
            /* Good neutrons are within momentum kin cuts (below l. KC -> efficiency; above u. KC -> beta fit) -> momentum kin cut before neutron shifting */
            /* We want to compare FD neutrons with FD protons in the same momentum region -> additional momentum kin cut after neutron shifting */
            /* Fiducial cuts -> after neutron shifting; because we want to match the currected neutron momentum to the proton maps! */
            bool FD_Theta_Cut_nFDpCD = ((P_nFD_nFDpCD_3v.Theta() * 180.0 / pi) <= FD_nucleon_theta_cut.GetUpperCut());
            bool FD_Momentum_Cut_BS_nFDpCD = ((NeutronMomBKC_nFDpCD <= FD_nucleon_momentum_cut.GetUpperCut()) && (NeutronMomBKC_nFDpCD >= FD_nucleon_momentum_cut.GetLowerCut()));
            // Momentum kin cut before neutron shifting
            bool FD_Momentum_Cut_AS_nFDpCD = ((P_nFD_nFDpCD_3v.Mag() <= FD_nucleon_momentum_cut.GetUpperCut()) && (P_nFD_nFDpCD_3v.Mag() >= FD_nucleon_momentum_cut.GetLowerCut()));
            // Additional momentum kin cut after neutron shifting
            bool e_withinFC_nFDpCD = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Electron", P_e_nFDpCD_3v.Mag(),
                                                              P_e_nFDpCD_3v.Theta() * 180.0 / pi, P_e_nFDpCD_3v.Phi() * 180.0 / pi);
            bool nFD_withinFC_nFDpCD = aMaps_master.IsInFDQuery((Generate_Electron_AMaps || Generate_Nucleon_AMaps), ThetaFD, "Neutron", P_nFD_nFDpCD_3v.Mag(),
                                                                P_nFD_nFDpCD_3v.Theta() * 180.0 / pi, P_nFD_nFDpCD_3v.Phi() * 180.0 / pi, Calc_eff_overlapping_FC);

            bool Pass_Kin_Cuts_nFDpCD = ((!apply_kinematical_cuts || (FD_Theta_Cut_nFDpCD && FD_Momentum_Cut_BS_nFDpCD && FD_Momentum_Cut_AS_nFDpCD)) &&
                                         (!apply_fiducial_cuts || (e_withinFC_nFDpCD && nFD_withinFC_nFDpCD)));

            //  Fillings nFDpCD histograms ------------------------------------------------------------------------------------------------------------------------------

            // Fillings nFDpCD histograms
            if (NeutronPassVeto_nFDpCD && Pass_Kin_Cuts_nFDpCD) {
                ++num_of_events_nFDpCD_AV;

                // Filling cut parameters histograms (nFDpCD)
                /* Filling Nphe plots (nFDpCD) */
                hNphe_nFDpCD_FD.hFill(e_nFDpCD->che(clas12::HTCC)->getNphe(), Weight_nFDpCD);

                /* Filling Chi2 histograms (nFDpCD) */
                hChi2_Electron_nFDpCD_FD.hFill(e_nFDpCD->par()->getChi2Pid(), Weight_nFDpCD);
                hChi2_Proton_nFDpCD_CD.hFill(pCD_nFDpCD->par()->getChi2Pid(), Weight_nFDpCD);
                hChi2_Proton_nFDpCD_FD.hFill(nFD_nFDpCD->par()->getChi2Pid(), Weight_nFDpCD);

                // Filling dVx, dVy, dVz histograms (nFDpCD)

                // All protons (nFDpCD)
                for (auto &p : protons) {
                    double Vx_p_nFDpCD = p->par()->getVx(), Vy_p_nFDpCD = p->par()->getVy(), Vz_p_nFDpCD = p->par()->getVz();
                    double dVx_nFDpCD = Vx_p_nFDpCD - Vx_e_nFDpCD, dVy_nFDpCD = Vy_p_nFDpCD - Vy_e_nFDpCD, dVz_nFDpCD = Vz_p_nFDpCD - Vz_e_nFDpCD;

                    hdVx_nFDpCD.hFill(dVx_nFDpCD, Weight_nFDpCD), hdVy_nFDpCD.hFill(dVy_nFDpCD, Weight_nFDpCD), hdVz_nFDpCD.hFill(dVz_nFDpCD, Weight_nFDpCD);
                }  // end of loop over protons vector

                // Id. pCD protons (nFDpCD)
                double Vx_pCD_nFDpCD = pCD_nFDpCD->par()->getVx(), Vy_pCD_nFDpCD = pCD_nFDpCD->par()->getVy(), Vz_pCD_nFDpCD = pCD_nFDpCD->par()->getVz();
                double dVx_pCD_nFDpCD = Vx_pCD_nFDpCD - Vx_e_nFDpCD, dVy_pCD_nFDpCD = Vy_pCD_nFDpCD - Vy_e_nFDpCD, dVz_pCD_nFDpCD = Vz_pCD_nFDpCD - Vz_e_nFDpCD;

                hdVx_pCD_nFDpCD.hFill(dVx_pCD_nFDpCD, Weight_nFDpCD);
                hdVy_pCD_nFDpCD.hFill(dVy_pCD_nFDpCD, Weight_nFDpCD);
                hdVz_pCD_nFDpCD.hFill(dVz_pCD_nFDpCD, Weight_nFDpCD);

                /* Filling SF histograms (nFDpCD) */
                hSF_nFDpCD_FD.hFill(EoP_e_nFDpCD, Weight_nFDpCD), hSF_VS_P_e_nFDpCD_FD.hFill(P_e_nFDpCD_3v.Mag(), EoP_e_nFDpCD, Weight_nFDpCD);

                /* Filling fiducial plots (nFDpCD) */
                hVcal_VS_EoP_nFDpCD_PCAL.hFill(e_nFDpCD->cal(clas12::PCAL)->getLv(), EoP_e_nFDpCD, Weight_nFDpCD);
                hWcal_VS_EoP_nFDpCD_PCAL.hFill(e_nFDpCD->cal(clas12::PCAL)->getLw(), EoP_e_nFDpCD, Weight_nFDpCD);

                // Filling momentum histograms (nFDpCD)

                // Electron momentum (nFDpCD)
                hP_e_APID_nFDpCD_FD.hFill(P_e_nFDpCD_3v.Mag(), Weight_nFDpCD);  // after mom. th.

                for (int i = 0; i < electrons.size(); i++) {
                    if (electrons[i]->getRegion() == FD) { hP_e_BPID_nFDpCD_FD.hFill(P_e_nFDpCD_3v.Mag(), Weight_nFDpCD); }  // before mom. th.
                }

                // Proton momentum (nFDpCD)
                for (int &i : Protons_ind) {
                    if (protons[i]->getRegion() == CD) {
                        hP_p_APID_nFDpCD_CD.hFill(protons[i]->getP(), Weight_nFDpCD);  // after mom. th.
                    } else if (protons[i]->getRegion() == FD) {
                        hP_p_APID_nFDpCD_FD.hFill(protons[i]->getP(), Weight_nFDpCD);  // after mom. th.
                    }
                }

                for (int i = 0; i < protons.size(); i++) {
                    if (protons[i]->getRegion() == CD) {
                        hP_p_BPID_nFDpCD_CD.hFill(protons[i]->getP(), Weight_nFDpCD);  // before mom. th.
                    } else if (protons[i]->getRegion() == FD) {
                        hP_p_BPID_nFDpCD_FD.hFill(protons[i]->getP(), Weight_nFDpCD);  // before mom. th.
                    }
                }

                // Piplus momentum (nFDpCD)
                for (int &i : Piplus_ind) {
                    if (piplus[i]->getRegion() == CD) {
                        hP_piplus_APID_nFDpCD_CD.hFill(piplus[i]->getP(), Weight_nFDpCD);  // after mom. th.
                    } else if (piplus[i]->getRegion() == FD) {
                        hP_piplus_APID_nFDpCD_FD.hFill(piplus[i]->getP(), Weight_nFDpCD);  // after mom. th.
                    }
                }

                for (int i = 0; i < piplus.size(); i++) {
                    if (piplus[i]->getRegion() == CD) {
                        hP_piplus_BPID_nFDpCD_CD.hFill(piplus[i]->getP(), Weight_nFDpCD);  // before mom. th.
                    } else if (piplus[i]->getRegion() == FD) {
                        hP_piplus_BPID_nFDpCD_FD.hFill(piplus[i]->getP(), Weight_nFDpCD);  // before mom. th.
                    }
                }

                // Piminus momentum (nFDpCD)
                for (int &i : Piminus_ind) {
                    if (piminus[i]->getRegion() == CD) {
                        hP_piminus_APID_nFDpCD_CD.hFill(piminus[i]->getP(), Weight_nFDpCD);  // after mom. th.
                    } else if (piminus[i]->getRegion() == FD) {
                        hP_piminus_APID_nFDpCD_FD.hFill(piminus[i]->getP(), Weight_nFDpCD);  // after mom. th.
                    }
                }

                for (int i = 0; i < piminus.size(); i++) {
                    if (piminus[i]->getRegion() == CD) {
                        hP_piminus_BPID_nFDpCD_CD.hFill(piminus[i]->getP(), Weight_nFDpCD);  // before mom. th.
                    } else if (piminus[i]->getRegion() == FD) {
                        hP_piminus_BPID_nFDpCD_FD.hFill(piminus[i]->getP(), Weight_nFDpCD);  // before mom. th.
                    }
                }

                // Photon momentum (nFDpCD)
                for (int &i : PhotonsFD_ind) { hP_ph_APID_nFDpCD_FD.hFill(allParticles[i]->getP(), Weight_nFDpCD); }  // after mom. th.

                for (int &i : ReDef_photons_FD) { hP_ph_BPID_nFDpCD_FD.hFill(allParticles[i]->getP(), Weight_nFDpCD); }  // before mom. th.

                // Neutron momentum (nFDpCD)
                for (int &i : NeutronsFD_ind) {
                    double TempNeutonMomentum = pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

                    hP_n_APID_nFDpCD_FD.hFill(TempNeutonMomentum, Weight_nFDpCD);
                    hP_n_APIDandNS_nFDpCD_FD.hFill(nRes.NCorr(apply_nucleon_SmearAndCorr, TempNeutonMomentum), Weight_nFDpCD);
                }  // after mom. th.

                for (int &i : ReDef_neutrons_FD) { hP_n_BPID_nFDpCD_FD.hFill(pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts), Weight_nFDpCD); }  // before mom. th.

                // Filling Beta vs. P plots (nFDpCD)

                // Beta vs. P from identified particles

                // Beta vs. P from identified electrons (nFDpCD, FD)
                /* loop over Electron_ind, so that, is Electron_ind.size() != 1, you'll see what thw other electrons have */
                for (int &i : Electron_ind) {
                    double P_e_temp = electrons[i]->getP();

                    if (electrons[i]->getRegion() == FD) {
                        hBeta_vs_P_nFDpCD_FD.hFill(P_e_temp, electrons[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Electrons_Only_FD.hFill(P_e_temp, electrons[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_neg_part_nFDpCD_FD.hFill(P_e_temp, electrons[i]->par()->getBeta(), Weight_nFDpCD);
                    }
                }  // end of loop over identified electrons vector

                // Beta vs. P from identified neutrons (nFDpCD, FD)
                for (int &i : NeutronsFD_ind) {
                    double P_n_temp = pid.GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

                    if (allParticles[i]->getRegion() == CD) {
                        hBeta_vs_P_nFDpCD_CD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Neutrons_Only_CD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_neut_part_nFDpCD_CD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                    } else if (allParticles[i]->getRegion() == FD) {
                        hBeta_vs_P_nFDpCD_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Neutrons_Only_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Neutrons_Only_ZOOMOUT_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_neut_part_nFDpCD_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                    }
                }  // end of loop over identified neutrons vector

                // Beta vs. P from identified photons (nFDpCD, FD)
                for (int &i : PhotonsFD_ind) {
                    double P_ph_temp = allParticles[i]->getP();

                    if (allParticles[i]->getRegion() == CD) {
                        hBeta_vs_P_nFDpCD_CD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Photons_Only_CD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_neut_part_nFDpCD_CD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                    } else if (allParticles[i]->getRegion() == FD) {
                        hBeta_vs_P_nFDpCD_FD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Photons_Only_FD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_neut_part_nFDpCD_FD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight_nFDpCD);
                    }
                }  // end of loop over identified photons vector

                // Beta vs. P from identified protons (nFDpCD, FD)
                for (int &i : Protons_ind) {
                    double P_p_temp = protons[i]->getP();

                    if (protons[i]->getRegion() == CD) {
                        hBeta_vs_P_nFDpCD_CD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Protons_Only_CD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_pos_part_nFDpCD_CD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_nFDpCD);
                    } else if (protons[i]->getRegion() == FD) {
                        hBeta_vs_P_nFDpCD_FD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Protons_Only_FD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_pos_part_nFDpCD_FD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight_nFDpCD);
                    }
                }  // end of loop over identified protons vector

                // Beta vs. P from identified piplus (nFDpCD, FD)
                for (int &i : Piplus_ind) {
                    double P_pip_temp = piplus[i]->getP();

                    if (piplus[i]->getRegion() == CD) {
                        hBeta_vs_P_nFDpCD_CD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Piplus_Only_CD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_pos_part_nFDpCD_CD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                    } else if (piplus[i]->getRegion() == FD) {
                        hBeta_vs_P_nFDpCD_FD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Piplus_Only_FD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_pos_part_nFDpCD_FD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                    }
                }  // end of loop over identified piplus vector

                // Beta vs. P from identified piminus (nFDpCD, FD)
                for (int &i : Piminus_ind) {
                    double P_pim_temp = piminus[i]->getP();

                    if (piminus[i]->getRegion() == CD) {
                        hBeta_vs_P_nFDpCD_CD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Piminus_Only_CD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_neg_part_nFDpCD_CD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_nFDpCD);
                    } else if (piminus[i]->getRegion() == FD) {
                        hBeta_vs_P_nFDpCD_FD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Piminus_Only_FD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_neg_part_nFDpCD_FD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight_nFDpCD);
                    }
                }  // end of loop over identified piminus vector

                // Beta vs. P from other particles (nFDpCD, FD)
                /* This is for self-consistency. Contributions from other particles should be zero */

                // Beta vs. P from Kplus (nFDpCD, FD)
                for (int i = 0; i < Kplus.size(); i++) {
                    double P_Kp_temp = Kplus[i]->getP();

                    if (Kplus[i]->getRegion() == CD) {
                        hBeta_vs_P_nFDpCD_CD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Kplus_Only_CD.hFill(P_Kp_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_pos_part_nFDpCD_CD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight_nFDpCD);
                    } else if (Kplus[i]->getRegion() == FD) {
                        hBeta_vs_P_nFDpCD_FD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Kplus_Only_FD.hFill(P_Kp_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_pos_part_nFDpCD_FD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight_nFDpCD);
                    }
                }  // end of loop over Kplus vector

                // Beta vs. P from Kminus (nFDpCD, FD)
                for (int i = 0; i < Kminus.size(); i++) {
                    double P_Km_temp = Kminus[i]->getP();

                    if (Kminus[i]->getRegion() == CD) {
                        hBeta_vs_P_nFDpCD_CD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Kminus_Only_CD.hFill(P_Km_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_neg_part_nFDpCD_CD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight_nFDpCD);
                    } else if (Kminus[i]->getRegion() == FD) {
                        hBeta_vs_P_nFDpCD_FD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_nFDpCD_Kminus_Only_FD.hFill(P_Km_temp, piplus[i]->par()->getBeta(), Weight_nFDpCD);
                        hBeta_vs_P_neg_part_nFDpCD_FD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight_nFDpCD);
                    }
                }  // end of loop over Kminus vector

                // Filling electron histograms (nFDpCD)
                hTheta_e_All_Int_nFDpCD_FD->Fill(Theta_e_nFDpCD, Weight_nFDpCD);
                hPhi_e_All_Int_nFDpCD_FD->Fill(Phi_e_nFDpCD, Weight_nFDpCD);
                hPhi_e_VS_P_e_nFDpCD_FD->Fill(P_e_nFDpCD_3v.Mag(), Phi_e_nFDpCD, Weight_nFDpCD);
                hPhi_e_VS_W_nFDpCD_FD->Fill(W_nFDpCD, Phi_e_nFDpCD, Weight_nFDpCD);
                hTheta_e_VS_Phi_e_nFDpCD_FD->Fill(Phi_e_nFDpCD, Theta_e_nFDpCD, Weight_nFDpCD);
                hTheta_e_VS_P_e_nFDpCD_FD->Fill(P_e_nFDpCD_3v.Mag(), Theta_e_nFDpCD, Weight_nFDpCD);
                hTheta_e_VS_W_nFDpCD_FD->Fill(W_nFDpCD, Theta_e_nFDpCD, Weight_nFDpCD);

                hE_e_All_Int_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                hE_e_VS_Theta_e_All_Int_nFDpCD_FD->Fill(Theta_e_nFDpCD, E_e_nFDpCD, Weight_nFDpCD);

                hET_All_Ang_All_Int_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);

                if ((Theta_e_nFDpCD >= 14.0) && (Theta_e_nFDpCD <= 16.0)) {
                    hET15_All_Int_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);
                    hE_e_15_All_Int_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                }

                if (qel) {
                    hTheta_e_QEL_nFDpCD_FD->Fill(Theta_e_nFDpCD, Weight_nFDpCD);
                    hPhi_e_QEL_nFDpCD_FD->Fill(Phi_e_nFDpCD, Weight_nFDpCD);
                    hE_e_QEL_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                    hE_e_VS_Theta_e_QEL_nFDpCD_FD->Fill(Theta_e_nFDpCD, E_e_nFDpCD, Weight_nFDpCD);

                    hET_All_Ang_QEL_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);

                    if ((Theta_e_nFDpCD >= 14.0) && (Theta_e_nFDpCD <= 16.0)) {
                        hET15_QEL_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);
                        hE_e_15_QEL_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                    }
                } else if (mec) {
                    hTheta_e_MEC_nFDpCD_FD->Fill(Theta_e_nFDpCD, Weight_nFDpCD);
                    hPhi_e_MEC_nFDpCD_FD->Fill(Phi_e_nFDpCD, Weight_nFDpCD);
                    hE_e_MEC_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                    hE_e_VS_Theta_e_MEC_nFDpCD_FD->Fill(Theta_e_nFDpCD, E_e_nFDpCD, Weight_nFDpCD);

                    hET_All_Ang_MEC_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);
                    if ((Theta_e_nFDpCD >= 14.0) && (Theta_e_nFDpCD <= 16.0)) {
                        hET15_MEC_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);
                        hE_e_15_MEC_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                    }
                } else if (res) {
                    hTheta_e_RES_nFDpCD_FD->Fill(Theta_e_nFDpCD, Weight_nFDpCD);
                    hPhi_e_RES_nFDpCD_FD->Fill(Phi_e_nFDpCD, Weight_nFDpCD);
                    hE_e_RES_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                    hE_e_VS_Theta_e_RES_nFDpCD_FD->Fill(Theta_e_nFDpCD, E_e_nFDpCD, Weight_nFDpCD);

                    hET_All_Ang_RES_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);

                    if ((Theta_e_nFDpCD >= 14.0) && (Theta_e_nFDpCD <= 16.0)) {
                        hET15_RES_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);
                        hE_e_15_RES_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                    }
                } else if (dis) {
                    hTheta_e_DIS_nFDpCD_FD->Fill(Theta_e_nFDpCD, Weight_nFDpCD);
                    hPhi_e_DIS_nFDpCD_FD->Fill(Phi_e_nFDpCD, Weight_nFDpCD);
                    hE_e_DIS_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                    hE_e_VS_Theta_e_DIS_nFDpCD_FD->Fill(Theta_e_nFDpCD, E_e_nFDpCD, Weight_nFDpCD);

                    hET_All_Ang_DIS_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);

                    if ((Theta_e_nFDpCD >= 14.0) && (Theta_e_nFDpCD <= 16.0)) {
                        hET15_DIS_nFDpCD_FD->Fill(beamE - E_e_nFDpCD, Weight_nFDpCD);
                        hE_e_15_DIS_nFDpCD_FD->Fill(E_e_nFDpCD, Weight_nFDpCD);
                    }
                }

                // Fill momentum transfer plots (nFDpCD)
                FillByInt(hQ2_nFDpCD, hQ2_QEL_nFDpCD, hQ2_MEC_nFDpCD, hQ2_RES_nFDpCD, hQ2_DIS_nFDpCD, qel, mec, res, dis, Q2_nFDpCD, Weight_nFDpCD);
                FillByInt(hq_3v_nFDpCD, hq_3v_QEL_nFDpCD, hq_3v_MEC_nFDpCD, hq_3v_RES_nFDpCD, hq_3v_DIS_nFDpCD, qel, mec, res, dis, q_nFDpCD_3v.Mag(), Weight_nFDpCD);

                hQ2_VS_W_nFDpCD->Fill(W_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                hQ2_VS_q_3v_nFDpCD->Fill(q_nFDpCD_3v.Mag(), Q2_nFDpCD, Weight_nFDpCD);
                hQ2_VS_omega_nFDpCD->Fill(omega_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                hq_3v_VS_omega_nFDpCD->Fill(omega_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);

                if (qel) {
                    hQ2_VS_W_QEL_nFDpCD->Fill(W_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                    hQ2_VS_q_3v_QEL_nFDpCD->Fill(q_nFDpCD_3v.Mag(), Q2_nFDpCD, Weight_nFDpCD);
                    hQ2_VS_omega_QEL_nFDpCD->Fill(omega_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                    hq_3v_VS_omega_QEL_nFDpCD->Fill(omega_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);
                } else if (mec) {
                    hQ2_VS_W_MEC_nFDpCD->Fill(W_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                    hQ2_VS_q_3v_MEC_nFDpCD->Fill(q_nFDpCD_3v.Mag(), Q2_nFDpCD, Weight_nFDpCD);
                    hQ2_VS_omega_MEC_nFDpCD->Fill(omega_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                    hq_3v_VS_omega_MEC_nFDpCD->Fill(omega_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);
                } else if (res) {
                    hQ2_VS_W_RES_nFDpCD->Fill(W_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                    hQ2_VS_q_3v_RES_nFDpCD->Fill(q_nFDpCD_3v.Mag(), Q2_nFDpCD, Weight_nFDpCD);
                    hQ2_VS_omega_RES_nFDpCD->Fill(omega_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                    hq_3v_VS_omega_RES_nFDpCD->Fill(omega_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);
                } else if (dis) {
                    hQ2_VS_W_DIS_nFDpCD->Fill(W_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                    hQ2_VS_q_3v_DIS_nFDpCD->Fill(q_nFDpCD_3v.Mag(), Q2_nFDpCD, Weight_nFDpCD);
                    hQ2_VS_omega_DIS_nFDpCD->Fill(omega_nFDpCD, Q2_nFDpCD, Weight_nFDpCD);
                    hq_3v_VS_omega_DIS_nFDpCD->Fill(omega_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);
                }

                // Filling angle plots (nFDpCD)

                // FD neutron angle plots (nFDpCD)
                FillByInt(hTheta_nFD_All_Int_nFDpCD_FD, hTheta_nFD_QEL_nFDpCD_FD, hTheta_nFD_MEC_nFDpCD_FD, hTheta_nFD_RES_nFDpCD_FD, hTheta_nFD_DIS_nFDpCD_FD, qel, mec, res, dis,
                          Theta_nFD_nFDpCD, Weight_nFDpCD);
                hTheta_nFD_VS_P_nFD_nFDpCD_FD->Fill(P_nFD_nFDpCD_3v.Mag(), Theta_nFD_nFDpCD, Weight_nFDpCD);
                hTheta_nFD_VS_W_nFDpCD_FD->Fill(W_nFDpCD, Theta_nFD_nFDpCD, Weight_nFDpCD);

                FillByInt(hPhi_nFD_All_Int_nFDpCD_FD, hPhi_nFD_QEL_nFDpCD_FD, hPhi_nFD_MEC_nFDpCD_FD, hPhi_nFD_RES_nFDpCD_FD, hPhi_nFD_DIS_nFDpCD_FD, qel, mec, res, dis, Phi_nFD_nFDpCD,
                          Weight_nFDpCD);
                hPhi_nFD_VS_P_nFD_nFDpCD_FD->Fill(P_nFD_nFDpCD_3v.Mag(), Phi_nFD_nFDpCD, Weight_nFDpCD);
                hPhi_nFD_VS_W_nFDpCD_FD->Fill(W_nFDpCD, Phi_nFD_nFDpCD, Weight_nFDpCD);

                hTheta_nFD_VS_Phi_nFD_nFDpCD_FD->Fill(Phi_nFD_nFDpCD, Theta_nFD_nFDpCD, Weight_nFDpCD);

                // CD proton angle plots (nFDpCD)
                FillByInt(hTheta_pCD_All_Int_nFDpCD_CD, hTheta_pCD_QEL_nFDpCD_CD, hTheta_pCD_MEC_nFDpCD_CD, hTheta_pCD_RES_nFDpCD_CD, hTheta_pCD_DIS_nFDpCD_CD, qel, mec, res, dis,
                          Theta_pCD_nFDpCD, Weight_nFDpCD);
                hTheta_pCD_VS_P_pCD_nFDpCD_CD->Fill(P_pCD_nFDpCD_3v.Mag(), Theta_pCD_nFDpCD, Weight_nFDpCD);
                hTheta_pCD_VS_W_nFDpCD_CD->Fill(W_nFDpCD, Theta_pCD_nFDpCD, Weight_nFDpCD);

                FillByInt(hPhi_pCD_All_Int_nFDpCD_CD, hPhi_pCD_QEL_nFDpCD_CD, hPhi_pCD_MEC_nFDpCD_CD, hPhi_pCD_RES_nFDpCD_CD, hPhi_pCD_DIS_nFDpCD_CD, qel, mec, res, dis, Phi_pCD_nFDpCD,
                          Weight_nFDpCD);
                hPhi_pCD_VS_P_pCD_nFDpCD_CD->Fill(P_pCD_nFDpCD_3v.Mag(), Phi_pCD_nFDpCD, Weight_nFDpCD);
                hPhi_pCD_VS_W_nFDpCD_CD->Fill(W_nFDpCD, Phi_pCD_nFDpCD, Weight_nFDpCD);

                hTheta_pCD_VS_Phi_pCD_nFDpCD_CD->Fill(Phi_pCD_nFDpCD, Theta_pCD_nFDpCD, Weight_nFDpCD);

                // Total momentum angle plots (nFDpCD)
                FillByInt(hTheta_tot_All_Int_nFDpCD, hTheta_tot_QEL_nFDpCD, hTheta_tot_MEC_nFDpCD, hTheta_tot_RES_nFDpCD, hTheta_tot_DIS_nFDpCD, qel, mec, res, dis, Theta_tot_nFDpCD,
                          Weight_nFDpCD);
                hTheta_tot_VS_P_tot_nFDpCD->Fill(P_tot_nFDpCD_3v.Mag(), Theta_tot_nFDpCD, Weight_nFDpCD);
                hTheta_tot_VS_W_nFDpCD->Fill(W_nFDpCD, Theta_tot_nFDpCD, Weight_nFDpCD);

                FillByInt(hPhi_tot_All_Int_nFDpCD, hPhi_tot_QEL_nFDpCD, hPhi_tot_MEC_nFDpCD, hPhi_tot_RES_nFDpCD, hPhi_tot_DIS_nFDpCD, qel, mec, res, dis, Phi_tot_nFDpCD, Weight_nFDpCD);
                hPhi_tot_VS_P_tot_nFDpCD->Fill(P_tot_nFDpCD_3v.Mag(), Phi_tot_nFDpCD, Weight_nFDpCD);
                hPhi_tot_VS_W_nFDpCD->Fill(W_nFDpCD, Phi_tot_nFDpCD, Weight_nFDpCD);

                hTheta_tot_VS_Phi_tot_nFDpCD->Fill(Phi_tot_nFDpCD, Theta_tot_nFDpCD, Weight_nFDpCD);

                // Relative momentum angle plots (nFDpCD)
                FillByInt(hTheta_rel_All_Int_nFDpCD, hTheta_rel_QEL_nFDpCD, hTheta_rel_MEC_nFDpCD, hTheta_rel_RES_nFDpCD, hTheta_rel_DIS_nFDpCD, qel, mec, res, dis, Theta_rel_nFDpCD,
                          Weight_nFDpCD);
                hTheta_rel_VS_P_rel_nFDpCD->Fill(P_rel_nFDpCD_3v.Mag(), Theta_rel_nFDpCD, Weight_nFDpCD);
                hTheta_rel_VS_W_nFDpCD->Fill(W_nFDpCD, Theta_rel_nFDpCD, Weight_nFDpCD);

                FillByInt(hPhi_rel_All_Int_nFDpCD, hPhi_rel_QEL_nFDpCD, hPhi_rel_MEC_nFDpCD, hPhi_rel_RES_nFDpCD, hPhi_rel_DIS_nFDpCD, qel, mec, res, dis, Phi_rel_nFDpCD, Weight_nFDpCD);
                hPhi_rel_VS_P_rel_nFDpCD->Fill(P_rel_nFDpCD_3v.Mag(), Phi_rel_nFDpCD, Weight_nFDpCD);
                hPhi_rel_VS_W_nFDpCD->Fill(W_nFDpCD, Phi_rel_nFDpCD, Weight_nFDpCD);

                hTheta_rel_VS_Phi_rel_nFDpCD->Fill(Phi_rel_nFDpCD, Theta_rel_nFDpCD, Weight_nFDpCD);

                // Fill neutron multiplicity plots (nFDpCD)
                pid.FillNeutMultiPlots(allParticles, electrons, Weight_nFDpCD, beamE, Neutron_veto_cut.GetLowerCutConst(), hNeut_Multi_By_Redef_BPID_BV_nFDpCD_FD,
                                       hNeut_Multi_By_Redef_BPID_AV_nFDpCD_FD, ReDef_neutrons_FD, hNeut_Multi_By_Redef_APID_BV_nFDpCD_FD, hNeut_Multi_By_Redef_APID_AV_nFDpCD_FD,
                                       NeutronsFD_ind);

                // Fill W (nFDpCD)
                FillByInt(hW_All_Int_nFDpCD, hW_QEL_nFDpCD, hW_MEC_nFDpCD, hW_RES_nFDpCD, hW_DIS_nFDpCD, qel, mec, res, dis, W_nFDpCD, Weight_nFDpCD);
                hW_VS_q_3v_nFDpCD->Fill(W_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hW_VS_omega_nFDpCD->Fill(W_nFDpCD, omega_nFDpCD, Weight_nFDpCD);

                if (qel) {
                    hW_VS_q_3v_QEL_nFDpCD->Fill(W_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hW_VS_omega_QEL_nFDpCD->Fill(W_nFDpCD, omega_nFDpCD, Weight_nFDpCD);
                } else if (mec) {
                    hW_VS_q_3v_MEC_nFDpCD->Fill(W_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hW_VS_omega_MEC_nFDpCD->Fill(W_nFDpCD, omega_nFDpCD, Weight_nFDpCD);
                } else if (res) {
                    hW_VS_q_3v_RES_nFDpCD->Fill(W_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hW_VS_omega_RES_nFDpCD->Fill(W_nFDpCD, omega_nFDpCD, Weight_nFDpCD);
                } else if (dis) {
                    hW_VS_q_3v_DIS_nFDpCD->Fill(W_nFDpCD, q_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hW_VS_omega_DIS_nFDpCD->Fill(W_nFDpCD, omega_nFDpCD, Weight_nFDpCD);
                }

                hP_tot_nFDpCD.hFill(P_tot_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hP_rel_nFDpCD.hFill(P_rel_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hP_tot_vs_P_rel_nFDpCD.hFill(P_tot_nFDpCD_3v.Mag(), P_rel_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hP_tot_minus_q_nFDpCD.hFill(P_tot_minus_q_nFDpCD_v3.Mag(), Weight_nFDpCD);
                hP_tot_minus_q_vs_P_tot_nFDpCD.hFill(P_tot_nFDpCD_3v.Mag(), P_tot_minus_q_nFDpCD_v3.Mag(), Weight_nFDpCD);
                hP_tot_minus_q_vs_q_nFDpCD.hFill(q_nFDpCD_3v.Mag(), P_tot_minus_q_nFDpCD_v3.Mag(), Weight_nFDpCD);

                hP_nFD_nFDpCD.hFill(P_nFD_nFDpCD_3v.Mag(), Weight_nFDpCD);          // nFD momentum (nFDpCD)
                hP_nFD_nFDpCD_ZoomOut.hFill(P_nFD_nFDpCD_3v.Mag(), Weight_nFDpCD);  // nFD momentum (nFDpCD)
                hP_pCD_nFDpCD.hFill(P_pCD_nFDpCD_3v.Mag(), Weight_nFDpCD);          // pCD momentum (nFDpCD)
                hP_nL_nFDpCD.hFill(P_nL_nFDpCD_3v.Mag(), Weight_nFDpCD);            // Leading nucleon (nFDpCD)
                hP_nR_nFDpCD.hFill(P_nR_nFDpCD_3v.Mag(), Weight_nFDpCD);            // Recoil nucleon (nFDpCD)

                hP_nFD_VS_W_nFDpCD->Fill(W_nFDpCD, P_nFD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hP_pCD_VS_W_nFDpCD->Fill(W_nFDpCD, P_pCD_nFDpCD_3v.Mag(), Weight_nFDpCD);

                if (qel) {
                    hP_nFD_VS_W_QEL_nFDpCD->Fill(W_nFDpCD, P_nFD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hP_pCD_VS_W_QEL_nFDpCD->Fill(W_nFDpCD, P_pCD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                } else if (mec) {
                    hP_nFD_VS_W_MEC_nFDpCD->Fill(W_nFDpCD, P_nFD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hP_pCD_VS_W_MEC_nFDpCD->Fill(W_nFDpCD, P_pCD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                } else if (res) {
                    hP_nFD_VS_W_RES_nFDpCD->Fill(W_nFDpCD, P_nFD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hP_pCD_VS_W_RES_nFDpCD->Fill(W_nFDpCD, P_pCD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                } else if (dis) {
                    hP_nFD_VS_W_DIS_nFDpCD->Fill(W_nFDpCD, P_nFD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hP_pCD_VS_W_DIS_nFDpCD->Fill(W_nFDpCD, P_pCD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                }

                hP_nL_vs_P_nR_nFDpCD.hFill(P_nL_nFDpCD_3v.Mag(), P_nR_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hP_nFD_vs_P_pCD_nFDpCD.hFill(P_nFD_nFDpCD_3v.Mag(), P_pCD_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hP_nFD_vs_P_e_nFDpCD.hFill(P_nFD_nFDpCD_3v.Mag(), P_e_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hP_nFD_vs_P_e_ZoomOut_nFDpCD.hFill(P_nFD_nFDpCD_3v.Mag(), P_e_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hP_nFD_vs_theta_e_nFDpCD.hFill(P_nFD_nFDpCD_3v.Mag(), Theta_e_nFDpCD, Weight_nFDpCD);
                hP_nFD_vs_theta_e_ZoomOut_nFDpCD.hFill(P_nFD_nFDpCD_3v.Mag(), Theta_e_nFDpCD, Weight_nFDpCD);
                hP_nFD_vs_phi_e_nFDpCD.hFill(P_nFD_nFDpCD_3v.Mag(), Phi_e_nFDpCD, Weight_nFDpCD);
                hP_nFD_vs_phi_e_ZoomOut_nFDpCD.hFill(P_nFD_nFDpCD_3v.Mag(), Phi_e_nFDpCD, Weight_nFDpCD);
                htheta_nFD_vs_P_e_nFDpCD.hFill(Theta_nFD_nFDpCD, P_e_nFDpCD_3v.Mag(), Weight_nFDpCD);
                htheta_nFD_vs_theta_e_nFDpCD.hFill(Theta_nFD_nFDpCD, Theta_e_nFDpCD, Weight_nFDpCD);
                htheta_nFD_vs_phi_e_nFDpCD.hFill(Theta_nFD_nFDpCD, Phi_e_nFDpCD, Weight_nFDpCD);
                hphi_nFD_vs_P_e_nFDpCD.hFill(Phi_nFD_nFDpCD, P_e_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hphi_nFD_vs_theta_e_nFDpCD.hFill(Phi_nFD_nFDpCD, Theta_e_nFDpCD, Weight_nFDpCD);
                hphi_nFD_vs_phi_e_nFDpCD.hFill(Phi_nFD_nFDpCD, Phi_e_nFDpCD, Weight_nFDpCD);

                P_T_L_nFDpCD_3v = TVector3(P_nL_nFDpCD_3v.Px(), P_nL_nFDpCD_3v.Py(), 0);  // transverse part of P_1
                P_T_tot_nFDpCD_3v = TVector3(P_nFD_nFDpCD_3v.Px() + P_pCD_nFDpCD_3v.Px(), P_nFD_nFDpCD_3v.Py() + P_pCD_nFDpCD_3v.Py(), 0);
                // transverse part of P_tot
                dP_T_L_nFDpCD_3v = TVector3(P_e_nFDpCD_3v.Px() + P_T_L_nFDpCD_3v.Px(), P_e_nFDpCD_3v.Py() + P_T_L_nFDpCD_3v.Py(), 0);
                dP_T_tot_nFDpCD_3v = TVector3(P_e_nFDpCD_3v.Px() + P_tot_nFDpCD_3v.Px(), P_e_nFDpCD_3v.Py() + P_tot_nFDpCD_3v.Py(), 0);

                Theta_p_e_p_tot_nFDpCD = acos((P_e_nFDpCD_3v.Px() * P_tot_nFDpCD_3v.Px() + P_e_nFDpCD_3v.Py() * P_tot_nFDpCD_3v.Py() + P_e_nFDpCD_3v.Pz() * P_tot_nFDpCD_3v.Pz()) /
                                              (P_e_nFDpCD_3v.Mag() * P_tot_nFDpCD_3v.Mag())) *
                                         180.0 / pi;  // Theta_p_e_p_tot_nFDpCD in deg
                hTheta_p_e_p_tot_nFDpCD->Fill(Theta_p_e_p_tot_nFDpCD, Weight_nFDpCD);
                hTheta_p_e_p_tot_vs_W_nFDpCD->Fill(W_nFDpCD, Theta_p_e_p_tot_nFDpCD, Weight_nFDpCD);

                // Filling Theta_nFD_pCD plots (nFDpCD)

                // General Theta_nFD_pCD plots (nFDpCD)
                FillByInt(hTheta_nFD_pCD_All_Int_nFDpCD, hTheta_nFD_pCD_QEL_nFDpCD, hTheta_nFD_pCD_MEC_nFDpCD, hTheta_nFD_pCD_RES_nFDpCD, hTheta_nFD_pCD_DIS_nFDpCD, qel, mec, res, dis,
                          Theta_nFD_pCD_nFDpCD, Weight_nFDpCD);
                hTheta_nFD_pCD_vs_W_nFDpCD->Fill(W_nFDpCD, Theta_nFD_pCD_nFDpCD, Weight_nFDpCD);

                // Plots for small Theta_nFD_pCD (nFDpCD)
                if (Theta_nFD_pCD_nFDpCD < 20.) {
                    hTheta_nFD_vs_theta_pCD_for_Theta_nFD_pCD_20_nFDpCD->Fill(Theta_R_nFDpCD, Theta_L_nFDpCD, Weight_nFDpCD);
                    hdphi_nFD_pCD_for_Theta_nFD_pCD_20_nFDpCD->Fill(dPhi_hit_e_nFD_nFDpCD, Weight_nFDpCD);
                    hdphi_nFD_pCD_for_Theta_nFD_pCD_20_ZOOMIN_nFDpCD->Fill(dPhi_hit_e_nFD_nFDpCD, Weight_nFDpCD);
                }

                hTheta_nFD_vs_theta_pCD_forall_Theta_nFD_pCD_nFDpCD->Fill(Theta_R_nFDpCD, Theta_L_nFDpCD, Weight_nFDpCD);
                hdphi_nFD_pCD_for_all_Theta_nFD_pCD_nFDpCD->Fill(dPhi_hit_e_nFD_nFDpCD, Weight_nFDpCD);
                hdphi_nFD_pCD_for_all_Theta_nFD_pCD_ZOOMIN_nFDpCD->Fill(dPhi_hit_e_nFD_nFDpCD, Weight_nFDpCD);

                if ((fabs(Theta_L_nFDpCD - Theta_L_cuts_nFDpCD.GetMean()) < Theta_L_cuts_nFDpCD.GetUpperCut()) &&
                    (fabs(Theta_R_nFDpCD - Theta_R_cuts_nFDpCD.GetMean()) < Theta_R_cuts_nFDpCD.GetUpperCut())) {
                    hdphi_nFD_pCD_for_small_dTheta_nFDpCD->Fill(dPhi_hit_e_nFD_nFDpCD, Weight_nFDpCD);
                    hdphi_nFD_pCD_for_small_dTheta_ZOOMIN_nFDpCD->Fill(dPhi_hit_e_nFD_nFDpCD, Weight_nFDpCD);
                }

                Theta_q_p_tot_nFDpCD = acos((q_nFDpCD_3v.Px() * P_tot_nFDpCD_3v.Px() + q_nFDpCD_3v.Py() * P_tot_nFDpCD_3v.Py() + q_nFDpCD_3v.Pz() * P_tot_nFDpCD_3v.Pz()) /
                                            (q_nFDpCD_3v.Mag() * P_tot_nFDpCD_3v.Mag())) *
                                       180.0 / pi;  // Theta_q_p_tot_nFDpCD in deg
                hTheta_q_p_tot_nFDpCD->Fill(Theta_q_p_tot_nFDpCD, Weight_nFDpCD);
                hTheta_q_p_tot_vs_W_nFDpCD->Fill(W_nFDpCD, Theta_q_p_tot_nFDpCD, Weight_nFDpCD);

                Theta_P_nL_minus_q_nR_nFDpCD =
                    acos((P_nL_minus_q_nFDpCD_v3.Px() * P_nR_nFDpCD_3v.Px() + P_nL_minus_q_nFDpCD_v3.Py() * P_nR_nFDpCD_3v.Py() + P_nL_minus_q_nFDpCD_v3.Pz() * P_nR_nFDpCD_3v.Pz()) /
                         (P_nL_minus_q_nFDpCD_v3.Mag() * P_nR_nFDpCD_3v.Mag())) *
                    180.0 / pi;  // Theta_P_nL_minus_q_nR_nFDpCD in deg
                Theta_q_p_L_nFDpCD = acos((q_nFDpCD_3v.Px() * P_nL_nFDpCD_3v.Px() + q_nFDpCD_3v.Py() * P_nL_nFDpCD_3v.Py() + q_nFDpCD_3v.Pz() * P_nL_nFDpCD_3v.Pz()) /
                                          (q_nFDpCD_3v.Mag() * P_nL_nFDpCD_3v.Mag())) *
                                     180.0 / pi;  // Theta_q_p_L_nFDpCD = Theta_q_p_1_nFDpCD in deg
                Theta_q_p_R_nFDpCD = acos((q_nFDpCD_3v.Px() * P_nR_nFDpCD_3v.Px() + q_nFDpCD_3v.Py() * P_nR_nFDpCD_3v.Py() + q_nFDpCD_3v.Pz() * P_nR_nFDpCD_3v.Pz()) /
                                          (q_nFDpCD_3v.Mag() * P_nR_nFDpCD_3v.Mag())) *
                                     180.0 / pi;  // Theta_q_p_R_nFDpCD = Theta_q_p_2_nFDpCD in deg
                Theta_q_nFD_nFDpCD = acos((q_nFDpCD_3v.Px() * P_nFD_nFDpCD_3v.Px() + q_nFDpCD_3v.Py() * P_nFD_nFDpCD_3v.Py() + q_nFDpCD_3v.Pz() * P_nFD_nFDpCD_3v.Pz()) /
                                          (q_nFDpCD_3v.Mag() * P_nFD_nFDpCD_3v.Mag())) *
                                     180.0 / pi;  // Theta_q_p_L_nFDpCD = Theta_q_p_1_nFDpCD in deg
                Theta_q_pCD_nFDpCD = acos((q_nFDpCD_3v.Px() * P_pCD_nFDpCD_3v.Px() + q_nFDpCD_3v.Py() * P_pCD_nFDpCD_3v.Py() + q_nFDpCD_3v.Pz() * P_pCD_nFDpCD_3v.Pz()) /
                                          (q_nFDpCD_3v.Mag() * P_pCD_nFDpCD_3v.Mag())) *
                                     180.0 / pi;  // Theta_q_p_R_nFDpCD = Theta_q_p_2_nFDpCD in deg
                hTheta_P_nL_minus_q_nR_nFDpCD->Fill(Theta_P_nL_minus_q_nR_nFDpCD, Weight_nFDpCD);
                hTheta_P_nL_minus_q_nR_vs_W_nFDpCD->Fill(W_nFDpCD, Theta_P_nL_minus_q_nR_nFDpCD, Weight_nFDpCD);
                hTheta_q_p_L_nFDpCD->Fill(Theta_q_p_L_nFDpCD, Weight_nFDpCD);
                hTheta_q_p_R_nFDpCD->Fill(Theta_q_p_R_nFDpCD, Weight_nFDpCD);
                hTheta_q_nFD_nFDpCD->Fill(Theta_q_nFD_nFDpCD, Weight_nFDpCD);
                hTheta_q_pCD_nFDpCD->Fill(Theta_q_pCD_nFDpCD, Weight_nFDpCD);
                hTheta_q_p_L_vs_W_nFDpCD->Fill(W_nFDpCD, Theta_q_p_L_nFDpCD, Weight_nFDpCD);
                hTheta_q_p_R_vs_W_nFDpCD->Fill(W_nFDpCD, Theta_q_p_R_nFDpCD, Weight_nFDpCD);
                hTheta_q_nFD_vs_W_nFDpCD->Fill(W_nFDpCD, Theta_q_nFD_nFDpCD, Weight_nFDpCD);
                hTheta_q_pCD_vs_W_nFDpCD->Fill(W_nFDpCD, Theta_q_pCD_nFDpCD, Weight_nFDpCD);
                hTheta_q_p_L_vs_p_L_q_nFDpCD->Fill(P_nL_nFDpCD_3v.Mag() / q_nFDpCD_3v.Mag(), Theta_q_p_L_nFDpCD, Weight_nFDpCD);
                hTheta_q_p_L_vs_Theta_q_p_R_nFDpCD->Fill(Theta_q_p_L_nFDpCD, Theta_q_p_R_nFDpCD, Weight_nFDpCD);
                hTheta_q_nFD_vs_Theta_q_pCD_nFDpCD->Fill(Theta_q_nFD_nFDpCD, Theta_q_pCD_nFDpCD, Weight_nFDpCD);

                E_nFD_nFDpCD = sqrt(m_n * m_n + P_nFD_nFDpCD_3v.Mag2());
                E_pCD_nFDpCD = sqrt(m_p * m_p + P_pCD_nFDpCD_3v.Mag2());
                E_nL_nFDpCD = sqrt(m_L * m_L + P_nL_nFDpCD_3v.Mag2());
                E_nR_nFDpCD = sqrt(m_R * m_R + P_nR_nFDpCD_3v.Mag2());

                P_tot_mu_nFDpCD_4v.SetPxPyPzE(P_nL_nFDpCD_3v.Px() + P_nR_nFDpCD_3v.Px(), P_nL_nFDpCD_3v.Py() + P_nR_nFDpCD_3v.Py(), P_nL_nFDpCD_3v.Pz() + P_nR_nFDpCD_3v.Pz(),
                                              E_nL_nFDpCD + E_nR_nFDpCD);
                P_rel_mu_nFDpCD_4v.SetPxPyPzE((P_nL_nFDpCD_3v.Px() - P_nR_nFDpCD_3v.Px()) / 2, (P_nL_nFDpCD_3v.Py() - P_nR_nFDpCD_3v.Py()) / 2,
                                              (P_nL_nFDpCD_3v.Pz() - P_nR_nFDpCD_3v.Pz()) / 2, (E_nL_nFDpCD - E_nR_nFDpCD) / 2);
                hP_tot_mu_nFDpCD.hFill(P_tot_mu_nFDpCD_4v.Mag(), Weight_nFDpCD);
                hP_rel_mu_nFDpCD.hFill(P_rel_mu_nFDpCD_4v.Mag(), Weight_nFDpCD);
                hP_tot_mu_vs_P_rel_mu_nFDpCD.hFill(P_tot_mu_nFDpCD_4v.Mag(), P_rel_mu_nFDpCD_4v.Mag(), Weight_nFDpCD);

                // Filling Ecal plots
                Ecal_nFDpCD = E_e_nFDpCD + (E_nFD_nFDpCD - m_n) + (E_pCD_nFDpCD - m_p);

                hEcal_All_Int_nFDpCD->Fill(Ecal_nFDpCD, Weight_nFDpCD);  // Fill Ecal for all interactions

                if (qel) {
                    hEcal_QEL_nFDpCD->Fill(Ecal_nFDpCD, Weight_nFDpCD);  // Fill Ecal for QEL only
                } else if (mec) {
                    hEcal_MEC_nFDpCD->Fill(Ecal_nFDpCD, Weight_nFDpCD);  // Fill Ecal for MEC only
                } else if (res) {
                    hEcal_RES_nFDpCD->Fill(Ecal_nFDpCD, Weight_nFDpCD);  // Fill Ecal for RES only
                } else if (dis) {
                    hEcal_DIS_nFDpCD->Fill(Ecal_nFDpCD, Weight_nFDpCD);  // Fill Ecal for DIS only
                }

                if (Ecal_nFDpCD > beamE) {
                    hEcal_vs_P_e_test_nFDpCD->Fill(P_e_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_Theta_e_test_nFDpCD->Fill(Theta_e_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_Phi_e_test_nFDpCD->Fill(Phi_e_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_P_nFD_test_nFDpCD->Fill(P_nFD_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_P_pCD_test_nFDpCD->Fill(P_pCD_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_Theta_nFD_test_nFDpCD->Fill(Theta_nFD_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_Phi_nFD_test_nFDpCD->Fill(Phi_nFD_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_Theta_pCD_test_nFDpCD->Fill(Theta_pCD_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_Phi_pCD_test_nFDpCD->Fill(Phi_pCD_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                }

                hdP_T_L_nFDpCD->Fill(dP_T_L_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hdP_T_L_vs_W_nFDpCD->Fill(W_nFDpCD, dP_T_L_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hdP_T_tot_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hdP_T_tot_vs_W_nFDpCD->Fill(W_nFDpCD, dP_T_tot_nFDpCD_3v.Mag(), Weight_nFDpCD);

                dAlpha_T_L_nFDpCD = acos(-(P_e_nFDpCD_3v.Px() * dP_T_L_nFDpCD_3v.Px() + P_e_nFDpCD_3v.Py() * dP_T_L_nFDpCD_3v.Py() + P_e_nFDpCD_3v.Pz() * dP_T_L_nFDpCD_3v.Pz()) /
                                         (P_T_e_nFDpCD_3v.Mag() * dP_T_L_nFDpCD_3v.Mag())) *
                                    180.0 / pi;
                // dP_T_L_nFDpCD_3v.Pz() = 0; dAlpha_T_L_nFDpCD in deg
                dAlpha_T_tot_nFDpCD = acos(-(P_e_nFDpCD_3v.Px() * dP_T_tot_nFDpCD_3v.Px() + P_e_nFDpCD_3v.Py() * dP_T_tot_nFDpCD_3v.Py() + P_e_nFDpCD_3v.Pz() * dP_T_tot_nFDpCD_3v.Pz()) /
                                           (P_T_e_nFDpCD_3v.Mag() * dP_T_tot_nFDpCD_3v.Mag())) *
                                      180.0 / pi;
                // dP_T_tot_nFDpCD_3v.Pz() = 0; dAlpha_T_tot_nFDpCD in deg
                hdAlpha_T_L_nFDpCD->Fill(dAlpha_T_L_nFDpCD, Weight_nFDpCD);
                hdAlpha_T_L_vs_W_nFDpCD->Fill(W_nFDpCD, dAlpha_T_L_nFDpCD, Weight_nFDpCD);
                hdAlpha_T_tot_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Weight_nFDpCD);
                hdAlpha_T_tot_vs_W_nFDpCD->Fill(W_nFDpCD, dAlpha_T_tot_nFDpCD, Weight_nFDpCD);

                if (qel) {
                    hdP_T_tot_QEL_Only_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hdAlpha_T_tot_QEL_Only_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Weight_nFDpCD);
                } else if (mec) {
                    hdP_T_tot_MEC_Only_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hdAlpha_T_tot_MEC_Only_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Weight_nFDpCD);
                } else if (res) {
                    hdP_T_tot_RES_Only_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hdAlpha_T_tot_RES_Only_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Weight_nFDpCD);
                } else if (dis) {
                    hdP_T_tot_DIS_Only_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Weight_nFDpCD);
                    hdAlpha_T_tot_DIS_Only_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Weight_nFDpCD);
                }

                hdP_T_L_vs_dAlpha_T_L_nFDpCD->Fill(dAlpha_T_L_nFDpCD, dP_T_L_nFDpCD_3v.Mag(), Weight_nFDpCD);
                hdP_T_tot_vs_dAlpha_T_tot_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, dP_T_tot_nFDpCD_3v.Mag(), Weight_nFDpCD);

                dPhi_T_L_nFDpCD = acos(-(P_T_e_nFDpCD_3v.Px() * P_T_L_nFDpCD_3v.Px() + P_T_e_nFDpCD_3v.Py() * P_T_L_nFDpCD_3v.Py() + P_T_e_nFDpCD_3v.Pz() * P_T_L_nFDpCD_3v.Pz()) /
                                       (P_T_e_nFDpCD_3v.Mag() * P_T_L_nFDpCD_3v.Mag())) *
                                  180.0 / pi;
                // P_T_L_nFDpCD_3v.Pz() = 0; dPhi_T_L_nFDpCD in deg
                dPhi_T_tot_nFDpCD = acos(-(P_T_e_nFDpCD_3v.Px() * P_T_tot_nFDpCD_3v.Px() + P_T_e_nFDpCD_3v.Py() * P_T_tot_nFDpCD_3v.Py() + P_T_e_nFDpCD_3v.Pz() * P_T_tot_nFDpCD_3v.Pz()) /
                                         (P_T_e_nFDpCD_3v.Mag() * P_T_tot_nFDpCD_3v.Mag())) *
                                    180.0 / pi;  // P_T_tot_nFDpCD_3v.Pz() = 0; dPhi_T_tot_nFDpCD in deg
                hdPhi_T_L_nFDpCD->Fill(dPhi_T_L_nFDpCD, Weight_nFDpCD);
                hdPhi_T_tot_nFDpCD->Fill(dPhi_T_tot_nFDpCD, Weight_nFDpCD);

                hEcal_vs_P_e_nFDpCD->Fill(P_e_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_Theta_e_nFDpCD->Fill(Theta_e_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_Phi_e_nFDpCD->Fill(Phi_e_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_P_nFD_nFDpCD->Fill(P_nFD_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_P_pCD_nFDpCD->Fill(P_pCD_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_Theta_nFD_nFDpCD->Fill(Theta_nFD_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_Phi_nFD_nFDpCD->Fill(Phi_nFD_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_Theta_pCD_nFDpCD->Fill(Theta_pCD_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_Phi_pCD_nFDpCD->Fill(Phi_pCD_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_dAlpha_T_L_nFDpCD->Fill(dAlpha_T_L_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_dAlpha_T_tot_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_dP_T_L_nFDpCD->Fill(dP_T_L_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_dP_T_tot_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                hEcal_vs_W_nFDpCD->Fill(W_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);

                if (qel) {
                    hEcal_vs_dAlpha_T_tot_QEL_Only_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_dP_T_tot_QEL_Only_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                } else if (mec) {
                    hEcal_vs_dAlpha_T_tot_MEC_Only_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_dP_T_tot_MEC_Only_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                } else if (res) {
                    hEcal_vs_dAlpha_T_tot_RES_Only_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_dP_T_tot_RES_Only_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                } else if (dis) {
                    hEcal_vs_dAlpha_T_tot_DIS_Only_nFDpCD->Fill(dAlpha_T_tot_nFDpCD, Ecal_nFDpCD, Weight_nFDpCD);
                    hEcal_vs_dP_T_tot_DIS_Only_nFDpCD->Fill(dP_T_tot_nFDpCD_3v.Mag(), Ecal_nFDpCD, Weight_nFDpCD);
                }

                hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_AV_nFDpCD.hFill(dPhi_hit_e_nFD_nFDpCD, dTheta_hit_e_nFD_nFDpCD, Weight_nFDpCD);
            }
        }  // end of 1enFDpCD & nFDpCD cuts if

        CodeDebugger.PrintStepTester(DebuggerMode);
    }  // end of while
    // </editor-fold>
}

void DetectorSimulationAnalyser::SaveLog() {
    CodeDebugger.PrintStepTester(DebuggerMode);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                                          Saving histogram list and finishing execution                                                              //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Saving and finishing execution

    // Saving proton pid cuts to .par file ----------------------------------------------------------------------------------------------------------------------------------

    // Saving pid cuts to .par file
    if (apply_cuts &&  // log pid cuts only if all other cuts are enabled //TODO: review this. make code preform cuts only in this case
        (!only_preselection_cuts && !only_electron_quality_cuts) &&
        // Do not log PID cuts if running in only preselection or only electron qulity cuts mode
        !apply_chi2_cuts_1e_cut) {
        DSCuts chi2cuts[] = {Chi2_Proton_cuts_CD, Chi2_Proton_cuts_FD, Chi2_piplus_cuts_CD, Chi2_piplus_cuts_FD, Chi2_piminus_cuts_CD, Chi2_piminus_cuts_FD};
        int chi2cuts_length = 6;

        ofstream FittedPIDCuts;
        std::string FittedPIDCutsFilePath = PIDCutsDirectory + "Fitted_PID_Cuts_-_" + SampleName + ".par";

        FittedPIDCuts.open(FittedPIDCutsFilePath);

        FittedPIDCuts << "######################################################################\n";
        FittedPIDCuts << "# CLAS12 analysis cuts and parameters file (after chi2 Gaussian fit) #\n";
        FittedPIDCuts << "######################################################################\n";
        FittedPIDCuts << "\n# Cuts are fitted for - " + SampleName << "\n";
        FittedPIDCuts << "\n# pid cuts by detector (pid:mean:sigma) - sigma_CD=" << Chi2_Proton_cuts_CD.FitStdFactor << ";sigma_FD=" << Chi2_Proton_cuts_FD.FitStdFactor << ":\n";

        for (int i = 0; i < chi2cuts_length; i++) {
            FittedPIDCuts << "pid_cuts" << "\t\t" << chi2cuts[i].GetPartPDG() << ":" << chi2cuts[i].Cuts.at(0) << ":" << chi2cuts[i].GetUpperCut() << ":" << chi2cuts[i].GetRegion() << "\n";
        }

        FittedPIDCuts.close();

        system(("cp " + FittedPIDCutsFilePath + " " + run_plots_path).c_str());  // Copy fitted chi2 cuts file for easy download from ifarm
    }

    // Saving nucleon cuts to .par file
    if (!apply_nucleon_cuts && apply_chi2_cuts_1e_cut && (!only_preselection_cuts && !only_electron_quality_cuts)) {
        // log nucleon cuts
        ofstream Nucleon_Cuts;
        std::string Nucleon_CutsFilePath = NucleonCutsDirectory + "Nucleon_Cuts_-_" + SampleName + ".par";

        Nucleon_Cuts.open(Nucleon_CutsFilePath);
        Nucleon_Cuts << "######################################################################\n";
        Nucleon_Cuts << "# CLAS12 analysis cuts and parameters file (after Beta Gaussian fit) #\n";
        Nucleon_Cuts << "######################################################################\n";
        Nucleon_Cuts << "\n# Cuts are fitted for - " + SampleName + ":\t" + settings.GetNucleon_Cuts_Status() + settings.GetFD_photons_Status() + settings.GetEfficiency_Status() << "\n\n";

        // Neutron momentum cuts
        Nucleon_Cuts << "# Neutron momentum cuts (pid:mean:sigma) - sigma_FD=" << n_mom_th.FitStdFactor << ":\n";

        Nucleon_Cuts << n_mom_th.GetCutVariable() << "\t\t\t" << n_mom_th.GetPartPDG() << ":" << n_mom_th.Cuts.at(0) << ":" << n_mom_th.GetLowerCut() << ":" << n_mom_th.GetRegion()
                     << "\n\n";

        DSCuts Neutron_Momentum_Cuts[] = {n_momentum_cuts_ABF_FD_n_from_ph, n_momentum_cuts_ABF_FD_n_from_ph_apprax};
        int Neutron_Momentum_Cuts_length = 2;

        for (int i = 0; i < Neutron_Momentum_Cuts_length; i++) {
            Nucleon_Cuts << Neutron_Momentum_Cuts[i].GetCutVariable() << "\t\t\t" << Neutron_Momentum_Cuts[i].GetPartPDG() << ":" << Neutron_Momentum_Cuts[i].Cuts.at(0) << ":"
                         << Neutron_Momentum_Cuts[i].GetUpperCut() << ":" << Neutron_Momentum_Cuts[i].GetRegion() << "\n";
        }

        Nucleon_Cuts << "\n";

        if (is6GeVSample) {
            // TODO: check if this should stay here!
            Nucleon_Cuts << "nRes_Momentum_cut\t\t\t2112:0:" << beamE << ":FD-ECAL  # was set manually!" << "\n\n";
        }

        // Proton CD-FD double detection dPhi_p1_p2 cuts
        Nucleon_Cuts << "# Proton CD-FD double detection dPhi cuts (pid:mean:sigma) - sigma=" << dphi_p1_p2_2p.FitStdFactor << ":\n";

        Nucleon_Cuts << dphi_p1_p2_2p.GetCutVariable() << "\t\t\t" << dphi_p1_p2_2p.GetPartPDG() << ":" << dphi_p1_p2_2p.GetMean() << ":" << dphi_p1_p2_2p.GetUpperCut() << ":"
                     << dphi_p1_p2_2p.GetRegion() << "\n";
        Nucleon_Cuts << dphi_pFD_pCD_2p.GetCutVariable() << "\t\t" << dphi_pFD_pCD_2p.GetPartPDG() << ":" << dphi_pFD_pCD_2p.GetMean() << ":" << dphi_pFD_pCD_2p.GetUpperCut() << ":"
                     << dphi_pFD_pCD_2p.GetRegion() << "\n";

        Nucleon_Cuts << "\n";

        Nucleon_Cuts.close();

        system(("cp " + Nucleon_CutsFilePath + " " + run_plots_path).c_str());  // Copy nucleon cuts file for easy download from the ifarm
    }

    // Saving debug files ---------------------------------------------------------------------------------------------------------------------------------------------------

    // Saving debug files
    if (PrintEvents == true) { EventPrint.close(); }

    if (debug_plots == true) {
        cout << "\033[33m\n\nSaving debugging plots...\n\n\033[0m";
        TString hit_map_ref_filePath = run_plots_path + "/" + "hit_map_ref.root";
        clasAna.set_hit_map_ref_fileName(hit_map_ref_filePath);

        TString debug_filePath = run_plots_path + "/" + "DebugOutputFile.root";
        clasAna.setdebug_fileName(debug_filePath);
        clasAna.WriteDebugPlots();
    } else {
        cout << "\033[33m\n\nDebugging plots are disabled...\n\n\033[0m";
    }

    // Saving setup to log file ------------------------------------------------------------------------------------------------------------------------------------------

    // Saving setup to log file
    ofstream myLogFile;
    myLogFile.open(run_plots_log_save_Directory.c_str());

    myLogFile << "///////////////////////////////////////////////////////////////////////////\n";
    myLogFile << "// Run was with '" << file_name << "' setup mode\n";
    myLogFile << "// Input file was " << LoadedInput << "\n";
    myLogFile << "// Code version was " << Ver << "\n";
    myLogFile << "// Analysis mode was 'Detector simulation'\n";
    myLogFile << "///////////////////////////////////////////////////////////////////////////\n\n";
    myLogFile << "Code ran with the following setup:" << "\n\n";

    // Input
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Input\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "LoadedInput: " << LoadedInput << "\n";
    myLogFile << "filePath: " << filePath << "\n";
    myLogFile << "fileInput: " << fileInput << "\n";
    myLogFile << "plotsInput: " << plotsInput << "\n\n";

    myLogFile << "WorkingDirectory: " << WorkingDirectory << "\n";
    myLogFile << "run_plots_path: " << run_plots_path << "\n";
    myLogFile << "SampleName: " << SampleName << "\n";
    myLogFile << "VaryingSampleName: " << VaryingSampleName << "\n\n";

    myLogFile << "isLocal:\t\t\t" << BoolToString(isLocal) << "\n";
    myLogFile << "isMC:\t\t" << BoolToString(isMC) << "\n";
    myLogFile << "isData:\t\t\t" << BoolToString(isData) << "\n";
    myLogFile << "is2GeVSample:\t\t" << BoolToString(is2GeVSample) << "\n";
    myLogFile << "is4GeVSample:\t\t" << BoolToString(is4GeVSample) << "\n";
    myLogFile << "is6GeVSample:\t\t" << BoolToString(is6GeVSample) << "\n\n";

    myLogFile << "HipoChainLength:\t\t" << HipoChainLength << "\n\n";

    // setup

    // Plot setup
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Plot setup\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "wider_margin = " << BoolToString(wider_margin) << "\n";
    myLogFile << "debug_plots = " << BoolToString(debug_plots) << "\n\n";

    myLogFile << "Log_scale_Vx_plots = " << BoolToString(Log_scale_Vx_plots) << "\n";
    myLogFile << "Log_scale_Vy_plots = " << BoolToString(Log_scale_Vy_plots) << "\n";
    myLogFile << "Log_scale_Vz_plots = " << BoolToString(Log_scale_Vz_plots) << "\n";
    myLogFile << "Log_scale_dVx_plots = " << BoolToString(Log_scale_dVx_plots) << "\n";
    myLogFile << "Log_scale_dVy_plots = " << BoolToString(Log_scale_dVy_plots) << "\n";
    myLogFile << "Log_scale_dVz_plots = " << BoolToString(Log_scale_dVz_plots) << "\n\n";

    // Calculation setup
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Calculation setup\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "calculate_1p = " << BoolToString(calculate_1p) << "\n";
    myLogFile << "calculate_1n = " << BoolToString(calculate_1n) << "\n";
    myLogFile << "calculate_2p = " << BoolToString(calculate_2p) << "\n\n";
    myLogFile << "calculate_pFDpCD = " << BoolToString(calculate_pFDpCD) << "\n";
    myLogFile << "calculate_nFDpCD = " << BoolToString(calculate_nFDpCD) << "\n\n";

    myLogFile << "-- Truth level calculation setup ------------------------------------------\n";
    myLogFile << "calculate_truth_level = " << BoolToString(calculate_truth_level) << "\n";
    myLogFile << "TL_plots_only_for_NC = " << BoolToString(TL_plots_only_for_NC) << "\n";
    myLogFile << "fill_TL_plots = " << BoolToString(fill_TL_plots) << "\n";
    myLogFile << "ZoomIn_On_mom_th_plots = " << BoolToString(ZoomIn_On_mom_th_plots) << "\n";
    myLogFile << "Eff_calc_with_one_reco_electron = " << BoolToString(Eff_calc_with_one_reco_electron) << "\n";
    myLogFile << "Calc_inc_eff_with_varying_theta = " << BoolToString(Calc_inc_eff_with_varying_theta) << "\n";
    myLogFile << "Calc_1n_n_eff_with_smaller_theta = " << BoolToString(Calc_1n_n_eff_with_smaller_theta) << "\n";
    myLogFile << "Calc_eff_overlapping_FC = " << BoolToString(Calc_eff_overlapping_FC) << "\n";
    myLogFile << "Rec_wTL_ES = " << BoolToString(Rec_wTL_ES) << "\n\n";

    myLogFile << "limless_mom_eff_plots = " << BoolToString(limless_mom_eff_plots) << "\n\n";

    myLogFile << "-- FD neutrals settings ---------------------------------------------------\n";
    myLogFile << "Enable_FD_photons = " << BoolToString(Enable_FD_photons) << "\n";
    myLogFile << "Enable_FD_neutrons = " << BoolToString(Enable_FD_neutrons) << "\n";
    myLogFile << "Count_FD_neurton_and_photon_hits = " << BoolToString(Count_FD_neurton_and_photon_hits) << "\n";

    myLogFile << "ES_by_leading_FDneutron = " << BoolToString(ES_by_leading_FDneutron) << "\n\n";

    myLogFile << "-- AMaps settings ---------------------------------------------------------\n";
    myLogFile << "Generate_Electron_AMaps = " << BoolToString(Generate_Electron_AMaps) << "\n";
    myLogFile << "Generate_Nucleon_AMaps = " << BoolToString(Generate_Nucleon_AMaps) << "\n";
    myLogFile << "Generate_WMaps = " << BoolToString(Generate_WMaps) << "\n";
    myLogFile << "AMaps_calc_with_one_reco_electron = " << BoolToString(AMaps_calc_with_one_reco_electron) << "\n";
    myLogFile << "P_e_bin_profile = " << P_e_bin_profile << "\n";
    myLogFile << "P_nuc_bin_profile = " << P_nuc_bin_profile << "\n";
    myLogFile << "Electron_single_slice_test = " << BoolToString(Electron_single_slice_test) << "\n";
    myLogFile << "Nucleon_single_slice_test = " << BoolToString(Nucleon_single_slice_test) << "\n";
    myLogFile << "TestSlices = {" << TestSlices.at(0) << ", " << TestSlices.at(1) << ", " << TestSlices.at(2) << "}\n\n";

    myLogFile << "-- nRES settings ----------------------------------------------------------\n";
    myLogFile << "plot_and_fit_MomRes = " << BoolToString(plot_and_fit_MomRes) << "\n";
    myLogFile << "Calculate_momResS2 = " << BoolToString(Calculate_momResS2) << "\n";
    myLogFile << "DeltaSlices = " << DeltaSlices << "\n";
    myLogFile << "VaryingDelta = " << BoolToString(VaryingDelta) << "\n";
    myLogFile << "ForceSmallpResLimits = " << BoolToString(ForceSmallpResLimits) << "\n";
    myLogFile << "SmearMode = " << SmearMode << "\n";
    myLogFile << "CorrMode = " << CorrMode << "\n";
    myLogFile << "Run_with_momResS2 = " << BoolToString(Run_with_momResS2) << "\n";
    myLogFile << "momRes_test = " << BoolToString(momRes_test) << "\n\n";

    myLogFile << "-- Other run parameters ---------------------------------------------------\n";
    myLogFile << "Probe = " << Probe << " (PDG: " << Probe_pdg << ")" << "\n";
    myLogFile << "Target = " << Target_nucleus << " (PDG: " << Target_pdg << ")" << "\n\n";

    myLogFile << "m_e = " << m_e << "\n";
    myLogFile << "m_p = " << m_p << "\n";
    myLogFile << "m_n = " << m_n << "\n";
    myLogFile << "m_pizero = " << m_pizero << "\n";
    myLogFile << "m_piplus = " << m_piplus << "\n";
    myLogFile << "m_piminus = " << m_piminus << "\n";
    myLogFile << "m_Kzero = " << m_Kzero << "\n";
    myLogFile << "m_Kplus = " << m_Kplus << "\n";
    myLogFile << "m_Kminus = " << m_Kminus << "\n\n";

    myLogFile << "beamE = " << beamE << " [GeV]\n";
    myLogFile << "Pv = " << Pv << "\n";
    myLogFile << "Pvx = " << Pvx << "\n";
    myLogFile << "Pvy = " << Pvy << "\n";
    myLogFile << "Pvz = " << Pvz << "\n\n";

    // Directory settings
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Directory settings\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Clear_Old_Directories = " << BoolToString(Clear_Old_Directories) << "\n\n";

    myLogFile << "Save_Plots_folder_to_zip_files = " << BoolToString(Save_Plots_folder_to_zip_files) << "\n";

    // Plot selector
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Plot selector\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "TestRun = " << BoolToString(TestRun) << "\n";

    myLogFile << "Plot_selector_master = " << BoolToString(Plot_selector_master) << "\n";

    myLogFile << "\n-- Cut variable plots -----------------------------------------------------" << "\n";
    myLogFile << "Cut_plots_master = " << BoolToString(Cut_plots_master) << "\n";
    myLogFile << "Nphe_plots = " << BoolToString(Nphe_plots) << "\n";
    myLogFile << "Chi2_plots = " << BoolToString(Chi2_plots) << "\n";
    myLogFile << "Vertex_plots = " << BoolToString(Vertex_plots) << "\n";
    myLogFile << "SF_plots = " << BoolToString(SF_plots) << "\n";
    myLogFile << "fiducial_plots = " << BoolToString(fiducial_plots) << "\n";
    myLogFile << "Momentum_plots = " << BoolToString(Momentum_plots) << "\n";

    myLogFile << "\n-- W plots ----------------------------------------------------------------" << "\n";
    myLogFile << "W_plots = " << BoolToString(W_plots) << "\n";

    myLogFile << "\n-- Beta plots -------------------------------------------------------------" << "\n";
    myLogFile << "Beta_plots = " << BoolToString(Beta_plots) << "\n";
    myLogFile << "Beta_vs_P_plots = " << BoolToString(Beta_vs_P_plots) << "\n";

    myLogFile << "\n-- Angle plots ------------------------------------------------------------" << "\n";
    myLogFile << "Angle_plots_master = " << BoolToString(Angle_plots_master) << "\n";
    myLogFile << "Theta_e_plots = " << BoolToString(Theta_e_plots) << "\n";
    myLogFile << "Phi_e_plots = " << BoolToString(Phi_e_plots) << "\n";

    myLogFile << "\n-- Momentum transfer plots ------------------------------------------------" << "\n";
    myLogFile << "Momentum_transfer_plots = " << BoolToString(Momentum_transfer_plots) << "\n";

    myLogFile << "\n-- E_e plots --------------------------------------------------------------" << "\n";
    myLogFile << "E_e_plots = " << BoolToString(E_e_plots) << "\n";

    myLogFile << "\n-- ET plots ---------------------------------------------------------------" << "\n";
    myLogFile << "ETrans_plots_master = " << BoolToString(ETrans_plots_master) << "\n";
    myLogFile << "ETrans_all_plots = " << BoolToString(ETrans_all_plots) << "\n";
    myLogFile << "ETrans_QEL_plots = " << BoolToString(ETrans_QEL_plots) << "\n";
    myLogFile << "ETrans_MEC_plots = " << BoolToString(ETrans_MEC_plots) << "\n";
    myLogFile << "ETrans_RES_plots = " << BoolToString(ETrans_RES_plots) << "\n";
    myLogFile << "ETrans_DIS_plots = " << BoolToString(ETrans_DIS_plots) << "\n";

    myLogFile << "\n-- E_cal plots ------------------------------------------------------------" << "\n";
    myLogFile << "E_cal_plots = " << BoolToString(Ecal_plots) << "\n";

    // Normalization setup
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Normalization setup\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "normalize_master = " << BoolToString(normalize_master) << "\n\n";

    myLogFile << "norm_Nphe_plots = " << BoolToString(norm_Nphe_plots) << "\n";
    myLogFile << "norm_Chi2_plots = " << BoolToString(norm_Chi2_plots) << "\n";
    myLogFile << "norm_Vertex_plots = " << BoolToString(norm_Vertex_plots) << "\n";
    myLogFile << "norm_SF_plots = " << BoolToString(norm_SF_plots) << "\n";
    myLogFile << "norm_Fiducial_plots = " << BoolToString(norm_Fiducial_plots) << "\n";
    myLogFile << "norm_Momentum_plots = " << BoolToString(norm_Momentum_plots) << "\n\n";

    myLogFile << "norm_W_plots = " << BoolToString(norm_W_plots) << "\n";
    myLogFile << "norm_Beta_plots = " << BoolToString(norm_Beta_plots) << "\n";
    myLogFile << "norm_Angle_plots_master = " << BoolToString(norm_Angle_plots_master) << "\n";
    myLogFile << "norm_Momentum_transfer_plots = " << BoolToString(norm_Momentum_transfer_plots) << "\n";
    myLogFile << "norm_E_e_plots = " << BoolToString(norm_E_e_plots) << "\n";
    myLogFile << "norm_ET_plots = " << BoolToString(norm_ET_plots) << "\n";
    myLogFile << "norm_Ecal_plots = " << BoolToString(norm_Ecal_plots) << "\n";
    myLogFile << "norm_TKI_plots = " << BoolToString(norm_TKI_plots) << "\n";
    myLogFile << "norm_MomRes_plots = " << BoolToString(norm_MomRes_plots) << "\n";
    myLogFile << "norm_Multi_plots = " << BoolToString(norm_Multi_plots) << "\n\n";

    // Delete setup
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Delete setup\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "delete_png_files = " << BoolToString(delete_png_files) << "\n";
    myLogFile << "delete_root_files = " << BoolToString(delete_root_files) << "\n\n";

    // Cut application setup
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Cut application setup\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "-- clas12ana cuts ---------------------------------------------------------\n";
    myLogFile << "apply_cuts = " << BoolToString(apply_cuts) << "\n\n";

    myLogFile << "clas12ana_particles = " << BoolToString(clas12ana_particles) << "\n\n";  // TODO: move form here!

    myLogFile << "only_preselection_cuts = " << BoolToString(only_preselection_cuts) << "\n";
    myLogFile << "only_electron_quality_cuts = " << BoolToString(only_electron_quality_cuts) << "\n\n";

    myLogFile << "apply_preselection_cuts = " << BoolToString(apply_preselection_cuts) << "\n";
    myLogFile << "apply_Vz_e_cuts = " << BoolToString(apply_Vz_e_cuts) << "\n";
    myLogFile << "apply_Vz_cuts = " << BoolToString(apply_Vz_cuts) << "\n";
    myLogFile << "apply_dVz_cuts = " << BoolToString(apply_dVz_cuts) << "\n";
    myLogFile << "apply_DC_e_fiducial_cuts = " << BoolToString(apply_DC_e_fiducial_cuts) << "\n";
    myLogFile << "apply_DC_fiducial_cuts = " << BoolToString(apply_DC_fiducial_cuts) << "\n\n";

    myLogFile << "apply_electron_quality_cuts = " << BoolToString(apply_electron_quality_cuts) << "\n";
    myLogFile << "apply_Nphe_cut = " << BoolToString(apply_Nphe_cut) << "\n";
    myLogFile << "apply_ECAL_SF_cuts = " << BoolToString(apply_ECAL_SF_cuts) << "\n";
    myLogFile << "apply_ECAL_P_cuts = " << BoolToString(apply_ECAL_P_cuts) << "\n";
    myLogFile << "apply_ECAL_fiducial_cuts = " << BoolToString(apply_ECAL_fiducial_cuts) << "\n\n";
    myLogFile << "apply_Electron_beta_cut = " << BoolToString(apply_Electron_beta_cut) << "\n\n";

    myLogFile << "apply_chi2_cuts_1e_cut = " << BoolToString(apply_chi2_cuts_1e_cut) << "\n";

    myLogFile << "-- My analysis cuts -------------------------------------------------------\n";
    myLogFile << "apply_nucleon_cuts = " << BoolToString(apply_nucleon_cuts) << "\n\n";

    myLogFile << "apply_nucleon_physical_cuts = " << BoolToString(apply_nucleon_physical_cuts) << "\n";
    myLogFile << "apply_nBeta_fit_cuts = " << BoolToString(apply_nBeta_fit_cuts) << "\n";
    myLogFile << "apply_fiducial_cuts = " << BoolToString(apply_fiducial_cuts) << "\n";
    myLogFile << "apply_kinematical_cuts = " << BoolToString(apply_kinematical_cuts) << "\n";
    myLogFile << "apply_kinematical_weights = " << BoolToString(apply_kinematical_weights) << "\n";
    myLogFile << "apply_nucleon_SmearAndCorr = " << BoolToString(apply_nucleon_SmearAndCorr) << "\n\n";

    // cuts

    // #Photo-electrons (Nphe) cuts (electrons only, FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "#Photo-electrons (Nphe) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Nphe_lower_cut:" << std::setw(59) << Nphe_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Nphe_upper_cut:" << std::setw(59) << Nphe_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2 cuts
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Chi2 cuts\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "\n-- Electrons --------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Electrons in FD (1e cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Electron_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Electron_cuts_FD.GetUpperCut() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Electron_cuts_FD.MeanFromHistogram << "\n\n";

    // Chi2_Proton
    myLogFile << "\n\n-- Protons ----------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Protons in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Proton_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Proton_cuts_CD.GetUpperCut() / Chi2_Proton_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Proton_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Proton_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Proton_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Proton_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- Protons in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Proton_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Proton_cuts_FD.GetUpperCut() / Chi2_Proton_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Proton_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Proton_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Proton_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Proton_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_Kplus
    myLogFile << "\n-- K+ ---------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Kplus in CD (1e cut) ---------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Kplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Kplus_cuts_CD.GetUpperCut() / Chi2_Kplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Kplus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Kplus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Kplus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Kplus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- Kplus in FD (1e cut) ---------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Kplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Kplus_cuts_FD.GetUpperCut() / Chi2_Kplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Kplus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Kplus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Kplus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Kplus_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_Kminus
    myLogFile << "\n-- K- ---------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Kminus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Kminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Kminus_cuts_CD.GetUpperCut() / Chi2_Kminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Kminus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Kminus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Kminus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Kminus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- Kminus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Kminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Kminus_cuts_FD.GetUpperCut() / Chi2_Kminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Kminus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Kminus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Kminus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Kminus_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_piplus
    myLogFile << "\n-- pi+ --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- piplus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_piplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_piplus_cuts_CD.GetUpperCut() / Chi2_piplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_piplus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_piplus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_piplus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_piplus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- piplus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_piplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_piplus_cuts_FD.GetUpperCut() / Chi2_piplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_piplus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_piplus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_piplus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_piplus_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_piminus
    myLogFile << "\n-- pi- --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- piminus in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_piminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_piminus_cuts_CD.GetUpperCut() / Chi2_piminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_piminus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_piminus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_piminus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_piminus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- piminus in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_piminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_piminus_cuts_FD.GetUpperCut() / Chi2_piminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_piminus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_piminus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_piminus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_piminus_cuts_FD.GetUpperCut() << "\n\n";

    // Chi2_deuteron
    myLogFile << "\n-- deuteron ---------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- deuteron in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_deuteron_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_deuteron_cuts_CD.GetUpperCut() / Chi2_deuteron_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_deuteron_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_deuteron_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_deuteron_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_deuteron_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- deuteron in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_deuteron_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_deuteron_cuts_FD.GetUpperCut() / Chi2_deuteron_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_deuteron_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_deuteron_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_deuteron_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_deuteron_cuts_FD.GetUpperCut() << "\n\n";

    // Vertex cuts (CD & FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "V cuts (CD & FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Vz_cut.GetUpperCut() = " << Vz_cut.GetUpperCut() << "\n";
    myLogFile << "Vz_cut.GetLowerCut() = " << Vz_cut.GetLowerCut() << "\n\n";
    myLogFile << "Vz_cut_FD.GetUpperCut() = " << Vz_cut_FD.GetUpperCut() << "\n";
    myLogFile << "Vz_cut_FD.GetLowerCut() = " << Vz_cut_FD.GetLowerCut() << "\n\n";
    myLogFile << "Vz_cut_CD.GetUpperCut() = " << Vz_cut_CD.GetUpperCut() << "\n";
    myLogFile << "Vz_cut_CD.GetLowerCut() = " << Vz_cut_CD.GetLowerCut() << "\n\n";

    // dV cuts (CD & FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "dV cuts (CD & FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "dVz_cuts.GetUpperCut() = " << dVz_cuts.GetUpperCut() << "\n";
    myLogFile << "dVz_cuts.GetLowerCut() = " << dVz_cuts.GetLowerCut() << "\n\n";
    myLogFile << "dVz_cuts_FD.GetUpperCut() = " << dVz_cuts_FD.GetUpperCut() << "\n";
    myLogFile << "dVz_cuts_FD.GetLowerCut() = " << dVz_cuts_FD.GetLowerCut() << "\n\n";
    myLogFile << "dVz_cuts_CD.GetUpperCut() = " << dVz_cuts_CD.GetUpperCut() << "\n";
    myLogFile << "dVz_cuts_CD.GetLowerCut() = " << dVz_cuts_CD.GetLowerCut() << "\n\n";

    // Sampling Fraction (SF) cuts (electrons only, FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Sampling Fraction (SF) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "SF_1e_upper_cut = " << clasAna.getEcalSFUpperCut() << "\n";
    myLogFile << "SF_1e_lower_cut = " << clasAna.getEcalSFLowerCut() << "\n";

    // ECAL fiducial cuts (electrons only, FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "ECAL fiducial cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "fiducial_cut_Lv = " << clasAna.getEcalEdgeCuts() << "\n";
    myLogFile << "fiducial_cut_Lw = " << clasAna.getEcalEdgeCuts() << "\n\n";

    // Momentum thresholds
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Momentum thresholds {min, max}\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "e_mom_th = {" << e_mom_th.GetLowerCut() << ", " << e_mom_th.GetUpperCut() << "}\n";
    myLogFile << "n_mom_th = {" << n_mom_th.GetLowerCut() << ", " << n_mom_th.GetUpperCut() << "}\n";
    myLogFile << "p_mom_th = {" << p_mom_th.GetLowerCut() << ", " << p_mom_th.GetUpperCut() << "}\n";
    myLogFile << "ph_mom_th = {" << ph_mom_th.GetLowerCut() << ", " << ph_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pip_mom_th = {" << pip_mom_th.GetLowerCut() << ", " << pip_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pim_mom_th = {" << pim_mom_th.GetLowerCut() << ", " << pim_mom_th.GetUpperCut() << "}\n";

    // Momentum cuts
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Momentum cuts {min, max}\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "\n-- Momentum cuts (1n, FD only) --------------------------------------------" << "\n";
    myLogFile << "Neutrons (n_momentum_cuts - ECAL):\t\t{" << n_momentum_cuts_ABF_FD_n_from_ph.GetLowerCut() << ", " << n_momentum_cuts_ABF_FD_n_from_ph.GetUpperCut() << "}\n";
    myLogFile << "Neutrons (n_momentum_cuts - ECAL apprax):\t{" << n_momentum_cuts_ABF_FD_n_from_ph_apprax.GetLowerCut() << ", " << n_momentum_cuts_ABF_FD_n_from_ph_apprax.GetUpperCut()
              << "}\n";

    // TL Momentum thresholds
    myLogFile << "\n===========================================================================\n";
    myLogFile << "TL Momentum thresholds {min, max}\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "TL_e_mom_cuts:\t{" << TL_e_mom_cuts.GetLowerCut() << ", " << TL_e_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_n_mom_cuts:\t{" << TL_n_mom_cuts.GetLowerCut() << ", " << TL_n_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_p_mom_cuts:\t{" << TL_p_mom_cuts.GetLowerCut() << ", " << TL_p_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pFD_mom_cuts:\t{" << TL_pFD_mom_cuts.GetLowerCut() << ", " << TL_pFD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pCD_mom_cuts:\t{" << TL_pCD_mom_cuts.GetLowerCut() << ", " << TL_pCD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pip_mom_cuts:\t{" << TL_pip_mom_cuts.GetLowerCut() << ", " << TL_pip_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pipFD_mom_cuts:\t{" << TL_pipFD_mom_cuts.GetLowerCut() << ", " << TL_pipFD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pipCD_mom_cuts:\t{" << TL_pipCD_mom_cuts.GetLowerCut() << ", " << TL_pipCD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pim_mom_cuts:\t{" << TL_pim_mom_cuts.GetLowerCut() << ", " << TL_pim_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pimFD_mom_cuts:\t{" << TL_pimFD_mom_cuts.GetLowerCut() << ", " << TL_pimFD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pimCD_mom_cuts:\t{" << TL_pimCD_mom_cuts.GetLowerCut() << ", " << TL_pimCD_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pi0_mom_cuts:\t{" << TL_pi0_mom_cuts.GetLowerCut() << ", " << TL_pi0_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_ph_mom_cuts:\t{" << TL_ph_mom_cuts.GetLowerCut() << ", " << TL_ph_mom_cuts.GetUpperCut() << "}\n\n";

    // Beta cut (1n, FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Beta cut (1n, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Beta_cut.GetUpperCut() (1 fit std) = " << Beta_cut.GetUpperCut() << "\n";
    myLogFile << "Beta_cut.GetMean() = " << Beta_cut.GetMean() << "\n\n";

    // Nucleon kinematical cuts (FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Nucleon kinematical cuts (FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "FD_nucleon_theta_cut.GetUpperCut() = " << FD_nucleon_theta_cut.GetUpperCut() << "\n";
    myLogFile << "FD_nucleon_theta_cut.GetLowerCut() = " << FD_nucleon_theta_cut.GetLowerCut() << "\n\n";

    myLogFile << "FD_nucleon_momentum_cut.GetUpperCut() = " << FD_nucleon_momentum_cut.GetUpperCut() << "\n";
    myLogFile << "FD_nucleon_momentum_cut.GetLowerCut() = " << FD_nucleon_momentum_cut.GetLowerCut() << "\n\n";

    myLogFile << "MomRes_mu_cuts.GetUpperCut() = " << MomRes_mu_cuts.GetUpperCut() << "\n";
    myLogFile << "MomRes_mu_cuts.GetLowerCut() = " << MomRes_mu_cuts.GetLowerCut() << "\n";
    myLogFile << "MomRes_sigma_cuts.GetUpperCut() = " << MomRes_sigma_cuts.GetUpperCut() << "\n";
    myLogFile << "MomRes_sigma_cuts.GetLowerCut() = " << MomRes_sigma_cuts.GetLowerCut() << "\n\n";

    // Neutron veto cut (1n & nFDpCD, FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Neutron veto cut (1n & nFDpCD, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Neutron_veto_cut.GetLowerCut() = " << Neutron_veto_cut.GetLowerCut() << "\n\n";

    // Ghost tracks handling (2p, CD & FD)
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Ghost tracks handling (2p, CD & FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Theta_p1_cuts_2p = " << Theta_p1_cuts_2p.GetUpperCut() << "\n";
    myLogFile << "Theta_p1_cuts_2p mean = " << Theta_p1_cuts_2p.GetMean() << "\n";
    myLogFile << "Theta_p2_cuts_2p = " << Theta_p2_cuts_2p.GetUpperCut() << "\n";
    myLogFile << "Theta_p2_cuts_2p mean = " << Theta_p2_cuts_2p.GetMean() << "\n";
    myLogFile << "dphi_p1_p2_2p.GetUpperCut() = " << dphi_p1_p2_2p.GetUpperCut() << "\n";
    myLogFile << "dphi_p1_p2_2p.GetMean() = " << dphi_p1_p2_2p.GetMean() << "\n\n";
    myLogFile << "dphi_pFD_pCD_2p.GetUpperCut() = " << dphi_pFD_pCD_2p.GetUpperCut() << "\n";
    myLogFile << "dphi_pFD_pCD_2p.GetMean() = " << dphi_pFD_pCD_2p.GetMean() << "\n\n";

    // momRes correction and smearing coefficients
    myLogFile << "\n===========================================================================\n";
    myLogFile << "momRes correction and smearing coefficients\n";
    myLogFile << "===========================================================================\n";

    // momRes run status
    if (plot_and_fit_MomRes) {
        if (!Calculate_momResS2 && !Run_with_momResS2) {
            if (!VaryingDelta) {
                myLogFile << "\nThis run calculated resolution with small 0.05 GeV/c slices!\n\n";
            } else {
                myLogFile << "\nThis is a momResS1 calculation run.\n\n";
            }
        } else {
            if (momRes_test) {
                if (Calculate_momResS2) {
                    myLogFile << "\nThis is a momResS2 calculation test run.\n\n";
                } else if (Run_with_momResS2) {
                    myLogFile << "\nThis is a momResS calculation test run with stage 2 results.\n\n";
                }
            } else {
                if (Calculate_momResS2) {
                    myLogFile << "\nThis is a momResS2 calculation run.\n\n";
                } else if (Run_with_momResS2) {
                    myLogFile << "\nThis is a momResS calculation run with stage 2 results.\n\n";
                }
            }
        }
    } else {
        if (Run_with_momResS2) {
            myLogFile << "\nMomRes is running with stage 2 results.\n";
        } else {
            myLogFile << "\nMomRes is running with stage 1 or stage 0 results.\n";
        }
    }

    myLogFile << "\n-- Neutron correction -----------------------------------------------------" << "\n";
    myLogFile << "CorrMode = " << nRes.Get_CorrMode() << "\n\n";
    myLogFile << "Correction loading path:\n" << nRes.Get_Loaded_Corr_coefficients_path() << "\n\n";

    vector<double> Corr_coefficients_values = nRes.Get_Loaded_Corr_coefficients_values();
    vector<string> Corr_coefficients_names = nRes.Get_Loaded_Corr_coefficients_names();

    for (int i = 0; i < Corr_coefficients_values.size(); i++) { myLogFile << Corr_coefficients_names.at(i) << " = " << Corr_coefficients_values.at(i) << "\n"; }

    myLogFile << "\n-- Proton smearing --------------------------------------------------------" << "\n";
    myLogFile << "SmearMode = " << nRes.Get_SmearMode() << "\n\n";
    myLogFile << "Smearing loading path:\n" << nRes.Get_Loaded_Std_coefficients_path() << "\n\n";

    vector<double> Std_coefficients_values = nRes.Get_Loaded_Smear_coefficients_values();
    vector<string> Std_coefficients_names = nRes.Get_Loaded_Smear_coefficients_names();

    for (int i = 0; i < Std_coefficients_values.size(); i++) { myLogFile << Std_coefficients_names.at(i) << " = " << Std_coefficients_values.at(i) << "\n"; }

    myLogFile << "\nRandom number generator = " << nRes.GetRandNumGenerator() << "\n\n";

    // Event counts
    myLogFile << "===========================================================================\n";
    myLogFile << "Event counts\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "-- Total counts -----------------------------------------------------------\n";
    myLogFile << "Total #(events):\t\t\t\t" << num_of_events << "\n";
    myLogFile << "Total #(events) w/o any e:\t\t\t" << num_of_events_without_any_e << "\n";
    myLogFile << "Total #(events) w/ any e:\t\t\t" << num_of_events_with_any_e << "\n\n";

    myLogFile << "Total #(QEL events):\t\t\t\t" << num_of_QEL_events << "\n";
    myLogFile << "Total #(MEC events):\t\t\t\t" << num_of_MEC_events << "\n";
    myLogFile << "Total #(RES events):\t\t\t\t" << num_of_RES_events << "\n";
    myLogFile << "Total #(DIS events):\t\t\t\t" << num_of_DIS_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS:\t\t\t\t" << num_of_QEL_events + num_of_MEC_events + num_of_RES_events + num_of_DIS_events << "\n\n";

    myLogFile << "-- Events with electrons counts -------------------------------------------\n";
    myLogFile << "#(events) w/ at least 1e:\t\t\t" << num_of_events_with_at_least_1e << "\n";
    myLogFile << "#(events) w/ more then 1e:\t\t\t" << num_of_events_more_then_1e << "\n";
    myLogFile << "#(events) w/ exactly 1e:\t\t\t" << num_of_events_with_exactly_1e << "\n";
    myLogFile << "#(events) w/ exactly 1e (from file):\t\t" << num_of_events_with_exactly_1e_from_file << "\n\n";

    myLogFile << "Total #(QEL events) 1e cut:\t\t\t" << num_of_QEL_events_1e_cut << "\n";
    myLogFile << "Total #(MEC events) 1e cut:\t\t\t" << num_of_MEC_events_1e_cut << "\n";
    myLogFile << "Total #(RES events) 1e cut:\t\t\t" << num_of_RES_events_1e_cut << "\n";
    myLogFile << "Total #(DIS events) 1e cut:\t\t\t" << num_of_DIS_events_1e_cut << "\n";
    myLogFile << "QEL + MEC + RES + DIS:\t\t\t\t" << num_of_QEL_events_1e_cut + num_of_MEC_events_1e_cut + num_of_RES_events_1e_cut + num_of_DIS_events_1e_cut << "\n\n";

    if (Count_FD_neurton_and_photon_hits) {
        myLogFile << "-- FD neutron hit counts in ECAL ------------------------------------------\n";
        myLogFile << "num_of_events_with_nFD_CLA12:\t\t" << num_of_events_with_nFD_CLA12 << "\n";
        myLogFile << "num_of_events_with_nFD_CLA12_PCAL:\t" << num_of_events_with_nFD_CLA12_PCAL << "\n";
        myLogFile << "num_of_events_with_nFD_CLA12_ECIN:\t" << num_of_events_with_nFD_CLA12_ECIN << "\n";
        myLogFile << "num_of_events_with_nFD_CLA12_ECOUT:\t" << num_of_events_with_nFD_CLA12_ECOUT << "\n";
        myLogFile << "num_of_events_with_nFD_CLA12_EC:\t" << num_of_events_with_nFD_CLA12_EC << "\n\n";

        myLogFile << "-- FD photon hit counts in ECAL -------------------------------------------\n";
        myLogFile << "num_of_events_with_phFD_CLA12:\t\t" << num_of_events_with_phFD_CLA12 << "\n";
        myLogFile << "num_of_events_with_phFD_CLA12_PCAL:\t" << num_of_events_with_phFD_CLA12_PCAL << "\n";
        myLogFile << "num_of_events_with_phFD_CLA12_ECIN:\t" << num_of_events_with_phFD_CLA12_ECIN << "\n";
        myLogFile << "num_of_events_with_phFD_CLA12_ECOUT:\t" << num_of_events_with_phFD_CLA12_ECOUT << "\n";
        myLogFile << "num_of_events_with_phFD_CLA12_EC:\t" << num_of_events_with_phFD_CLA12_EC << "\n\n";
    }

    myLogFile << "-- Counting events with FD neutrons ---------------------------------------\n";
    myLogFile << "#(events) w/ 1 FD neutrons:\t\t" << num_of_events_1n_in_FD << "\n";
    myLogFile << "#(events) w/ 2 FD neutrons:\t\t" << num_of_events_2n_in_FD << "\n";
    myLogFile << "#(events) w/ 3 FD neutrons:\t\t" << num_of_events_3n_in_FD << "\n";
    myLogFile << "#(events) w/ more than 3 FD neutrons:\t" << num_of_events_Xn_in_FD << "\n\n";

    myLogFile << "-- 1e1p event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e1p:\t\t\t\t" << num_of_events_with_1e1p << "\n\n";

    myLogFile << "-- 1e2p event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e2p:\t\t\t\t" << num_of_events_with_1e2p << "\n\n";

    myLogFile << "-- 1epFDpCD event counts --------------------------------------------------\n";
    myLogFile << "#(events) w/ 1epFDpCD:\t\t\t\t" << num_of_events_with_1epFDpCD << "\n\n";

    myLogFile << "-- 2p event counts --------------------------------------------------------\n";
    myLogFile << "num_of_events_2p_wFakeProtons:\t\t\t" << num_of_events_2p_wFakeProtons << "\n\n";

    if (apply_nucleon_cuts) {
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
                  << num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd + pid.num_of_AD_2p_events_from_3p_sCTOFhp +
                         pid.num_of_AD_2p_events_from_4p_sCTOFhp + pid.num_of_AD_2p_events_from_3p_dCDaFDd + pid.num_of_AD_2p_events_from_4p_dCDaFDd
                  << "\n\n";

        myLogFile << "num_of_events_2p (from monitoring; no mixed):\t\t"
                  << num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd + pid.num_of_AD_2p_events_from_3p_sCTOFhp +
                         pid.num_of_AD_2p_events_from_4p_sCTOFhp + pid.num_of_AD_2p_events_from_3p_dCDaFDd + pid.num_of_AD_2p_events_from_4p_dCDaFDd - pid.num_of_RM_2p_events_dCDaFDd
                  << "\n\n";
    }

    myLogFile << "#(events) 2p:\t\t\t\t\t" << num_of_events_2p << "\n\n";

    myLogFile << "-- pFDpCD event counts ----------------------------------------------------\n";
    myLogFile << "#(events) pFDpCD:\t\t\t\t" << num_of_events_pFDpCD << "\n\n";

    myLogFile << "-- Event counts -----------------------------------------------------------\n";
    myLogFile << "num_of_events_1p_inFD:\t\t\t\t" << num_of_events_1p_inFD << "\n\n";

    myLogFile << "num_of_events_1n_inFD:\t\t\t\t" << num_of_events_1n_inFD << "\n";
    myLogFile << "num_of_events_1n_inFD_AV:\t\t\t" << num_of_events_1n_inFD_AV << "\n";
    myLogFile << "num_of_events_1n_inFD_AV_wmt1gTLn:\t\t" << num_of_events_1n_inFD_AV_wmt1gTLn << "\n\n";

    myLogFile << "num_of_events_2p_wFakeProtons:\t\t\t" << num_of_events_2p_wFakeProtons << "\n";
    myLogFile << "num_of_events_2p:\t\t\t\t" << num_of_events_2p << "\n\n";

    myLogFile << "num_of_events_1epFDpCD:\t\t\t\t" << num_of_events_with_1epFDpCD << "\n";
    myLogFile << "num_of_events_1epFDpFD:\t\t\t\t" << num_of_events_with_1epFDpFD << "\n";
    myLogFile << "num_of_events_1epCDpCD:\t\t\t\t" << num_of_events_with_1epCDpCD << "\n\n";

    myLogFile << "num_of_events_pFDpCD:\t\t\t\t" << num_of_events_pFDpCD << "\n\n";

    myLogFile << "num_of_events_nFDpCD:\t\t\t\t" << num_of_events_nFDpCD << "\n";
    myLogFile << "num_of_events_nFDpCD_AV:\t\t\t" << num_of_events_nFDpCD_AV << "\n\n\n";

    // content of FittedPIDCuts.par file
    if (apply_cuts && !apply_chi2_cuts_1e_cut && (!only_preselection_cuts && only_electron_quality_cuts)) {
        myLogFile << "===========================================================================\n";
        myLogFile << "content of FittedPIDCuts.par file\n";
        myLogFile << "===========================================================================\n\n";

        DSCuts chi2cuts[] = {Chi2_Proton_cuts_CD, Chi2_Proton_cuts_FD, Chi2_piplus_cuts_CD, Chi2_piplus_cuts_FD, Chi2_piminus_cuts_CD, Chi2_piminus_cuts_FD};
        int chi2cuts_length = 6;

        myLogFile << "######################################################################\n";
        myLogFile << "# CLAS12 analysis cuts and parameters file (after chi2 Gaussian fit) #\n";
        myLogFile << "######################################################################\n";
        myLogFile << "\n";
        myLogFile << "# pid cuts by detector (pid:mean:sigma) - sigma_CD=" << Chi2_Proton_cuts_CD.FitStdFactor << ";sigma_FD=" << Chi2_Proton_cuts_FD.FitStdFactor << ":\n";

        for (int i = 0; i < chi2cuts_length; i++) {
            myLogFile << "pid_cuts" << "\t\t" << chi2cuts[i].GetPartPDG() << ":" << chi2cuts[i].Cuts.at(0) << ":" << chi2cuts[i].GetUpperCut() << ":" << chi2cuts[i].GetRegion() << "\n";
        }
    }

    myLogFile.close();

    // Saving histogram list and finishing execution ------------------------------------------------------------------------------------------------------------------------

    // Saving histogram list and finishing execution

    // Saving histogram list
    cout << "\033[33m\n\nSaving histogram list...\033[0m";

    // Saving histogram TList
    TFile *plots_fout = new TFile(TListName, "recreate");
    plots_fout->cd();
    plots->Write();
    plots_fout->Write();
    plots_fout->Close();

    // Saving Acceptance correction data
    TFile *ACorr_data_fout;

    if (save_ACorr_data) {
        ACorr_data_fout = new TFile(ACorr_DataName, "recreate");
        ACorr_data_fout->cd();
        ACorr_data->Write();
        ACorr_data_fout->Write();
        ACorr_data_fout->Close();
    }

    cout << "\033[33m done.\n\n\033[0m";

    // finishing execution
    cout << "\033[33m\n\n===========================================================================\n\033[0m";
    cout << "\033[33m\t\t\tExecution summary\n\033[0m";
    cout << "\033[33m===========================================================================\n\n\033[0m";

    cout << "\033[33m---------------------------------------------------------------------------\n\033[0m";
    cout << "\033[33m\t\t\tEvent counts\n\033[0m";
    cout << "\033[33m---------------------------------------------------------------------------\n\n\033[0m";

    cout << "\033[33m-- Inclusive TL counts ----------------------------------------------------\n\033[0m";
    cout << "\033[33mTotal #(QEL events):\t\t\t" << num_of_QEL_events << "\n\033[0m";
    cout << "\033[33mTotal #(MEC events):\t\t\t" << num_of_MEC_events << "\n\033[0m";
    cout << "\033[33mTotal #(RES events):\t\t\t" << num_of_RES_events << "\n\033[0m";
    cout << "\033[33mTotal #(DIS events):\t\t\t" << num_of_DIS_events << "\n\033[0m";
    cout << "\033[33mQEL + MEC + RES + DIS:\t\t\t" << num_of_QEL_events + num_of_MEC_events + num_of_RES_events + num_of_DIS_events << "\n\n\033[0m";

    cout << "\033[33m-- Total counts -----------------------------------------------------------\n\033[0m";
    cout << "\033[33mTotal #(events):\t\t\t" << num_of_events << "\n\033[0m";
    cout << "\033[33mTotal #(events) w/o any e:\t\t" << num_of_events_without_any_e << "\n\033[0m";
    cout << "\033[33mTotal #(events) w/ any e:\t\t" << num_of_events_with_any_e << "\n\n\033[0m";

    cout << "\033[33m-- Events with electrons counts -------------------------------------------\n\033[0m";
    cout << "\033[33m#(events) w/ at least 1e:\t\t" << num_of_events_with_at_least_1e << "\n\033[0m";
    cout << "\033[33m#(events) w/ more then 1e:\t\t" << num_of_events_more_then_1e << "\n\033[0m";
    cout << "\033[33m#(events) w/ exactly 1e:\t\t" << num_of_events_with_exactly_1e << "\n\n\033[0m";
    cout << "\033[33m#(events) w/ exactly 1e (from file):\t\t" << num_of_events_with_exactly_1e_from_file << "\n\n\033[0m";

    if (Count_FD_neurton_and_photon_hits) {
        cout << "\033[33m-- FD neutron hit counts in ECAL ------------------------------------------\n\033[0m";
        cout << "\033[33mnum_of_events_with_nFD_CLA12:\t\t" << num_of_events_with_nFD_CLA12 << "\n\033[0m";
        cout << "\033[33mnum_of_events_with_nFD_CLA12_PCAL:\t" << num_of_events_with_nFD_CLA12_PCAL << "\n\033[0m";
        cout << "\033[33mnum_of_events_with_nFD_CLA12_ECIN:\t" << num_of_events_with_nFD_CLA12_ECIN << "\n\033[0m";
        cout << "\033[33mnum_of_events_with_nFD_CLA12_ECOUT:\t" << num_of_events_with_nFD_CLA12_ECOUT << "\n\033[0m";
        cout << "\033[33mnum_of_events_with_nFD_CLA12_EC:\t" << num_of_events_with_nFD_CLA12_EC << "\n\n\033[0m";

        cout << "\033[33m-- FD photon hit counts in ECAL -------------------------------------------\n\033[0m";
        cout << "\033[33mnum_of_events_with_phFD_CLA12:\t\t" << num_of_events_with_phFD_CLA12 << "\n\033[0m";
        cout << "\033[33mnum_of_events_with_phFD_CLA12_PCAL:\t" << num_of_events_with_phFD_CLA12_PCAL << "\n\033[0m";
        cout << "\033[33mnum_of_events_with_phFD_CLA12_ECIN:\t" << num_of_events_with_phFD_CLA12_ECIN << "\n\033[0m";
        cout << "\033[33mnum_of_events_with_phFD_CLA12_ECOUT:\t" << num_of_events_with_phFD_CLA12_ECOUT << "\n\033[0m";
        cout << "\033[33mnum_of_events_with_phFD_CLA12_EC:\t" << num_of_events_with_phFD_CLA12_EC << "\n\n\033[0m";
    }

    cout << "\033[33m-- Inclusive Reco counts --------------------------------------------------\n\033[0m";
    cout << "\033[33mTotal #(QEL events) 1e cut:\t\t" << num_of_QEL_events_1e_cut << "\n\033[0m";
    cout << "\033[33mTotal #(MEC events) 1e cut:\t\t" << num_of_MEC_events_1e_cut << "\n\033[0m";
    cout << "\033[33mTotal #(RES events) 1e cut:\t\t" << num_of_RES_events_1e_cut << "\n\033[0m";
    cout << "\033[33mTotal #(DIS events) 1e cut:\t\t" << num_of_DIS_events_1e_cut << "\n\033[0m";
    cout << "\033[33mQEL + MEC + RES + DIS:\t\t\t" << num_of_QEL_events_1e_cut + num_of_MEC_events_1e_cut + num_of_RES_events_1e_cut + num_of_DIS_events_1e_cut << "\n\n\033[0m";

    cout << "\033[33m-- 1e1p event counts ------------------------------------------------------\n\033[0m";
    cout << "\033[33m#(events) w/ 1e1p:\t\t\t" << num_of_events_with_1e1p << "\n\n\033[0m";

    cout << "\033[33m-- 1e2p event counts ------------------------------------------------------\n\033[0m";
    cout << "\033[33m#(events) w/ 1e2p:\t\t\t" << num_of_events_with_1e2p << "\n\n\033[0m";

    cout << "\033[33m-- 1epFDpCD event counts --------------------------------------------------\n\033[0m";
    cout << "\033[33m#(events) w/ 1epFDpCD:\t\t\t" << num_of_events_with_1epFDpCD << "\n\n\033[0m";

    cout << "\033[33m-- 2p event counts --------------------------------------------------------\n\033[0m";
    cout << "\033[33mnum_of_events_2p_wFakeProtons:\t\t" << num_of_events_2p_wFakeProtons << "\n\n\033[0m";

    if (apply_nucleon_cuts) {
        cout << "\033[33mnum_of_RM_2p_events_sCTOFhp:\t\t" << pid.num_of_RM_2p_events_sCTOFhp << "\n\033[0m";
        cout << "\033[33mnum_of_AD_2p_events_from_3p_sCTOFhp:\t" << pid.num_of_AD_2p_events_from_3p_sCTOFhp << "\n\033[0m";
        cout << "\033[33mnum_of_AD_2p_events_from_4p_sCTOFhp:\t" << pid.num_of_AD_2p_events_from_4p_sCTOFhp << "\n\033[0m";
        cout << "\033[33mnum_of_AD_2p_events_from_Xp_sCTOFhp:\t" << pid.num_of_AD_2p_events_from_Xp_sCTOFhp << "\n\033[0m";
        cout << "\033[33mnum_of_RM_2p_events_dCDaFDd:\t\t" << pid.num_of_RM_2p_events_dCDaFDd << "\n\033[0m";
        cout << "\033[33mnum_of_AD_2p_events_from_3p_dCDaFDd:\t" << pid.num_of_AD_2p_events_from_3p_dCDaFDd << "\n\033[0m";
        cout << "\033[33mnum_of_AD_2p_events_from_4p_dCDaFDd:\t" << pid.num_of_AD_2p_events_from_4p_dCDaFDd << "\n\033[0m";
        cout << "\033[33mnum_of_AD_2p_events_from_Xp_dCDaFDd:\t" << pid.num_of_AD_2p_events_from_Xp_dCDaFDd << "\n\n\033[0m";

        cout << "\033[33mnum_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd:\t" << pid.num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd << "\n\n\033[0m";

        cout << "\033[33mnum_of_events_2p (from monitoring):\t"
             << num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd + pid.num_of_AD_2p_events_from_3p_sCTOFhp +
                    pid.num_of_AD_2p_events_from_4p_sCTOFhp + pid.num_of_AD_2p_events_from_3p_dCDaFDd + pid.num_of_AD_2p_events_from_4p_dCDaFDd
             << "\n\n\033[0m";

        cout << "\033[33mnum_of_events_2p (from monitoring; no mixed):\t"
             << num_of_events_2p_wFakeProtons - pid.num_of_RM_2p_events_sCTOFhp - pid.num_of_RM_2p_events_dCDaFDd + pid.num_of_AD_2p_events_from_3p_sCTOFhp +
                    pid.num_of_AD_2p_events_from_4p_sCTOFhp + pid.num_of_AD_2p_events_from_3p_dCDaFDd + pid.num_of_AD_2p_events_from_4p_dCDaFDd -
                    pid.num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd
             << "\n\n\033[0m";
    }

    cout << "\033[33m#(events) 2p:\t\t\t\t" << num_of_events_2p << "\n\n\033[0m";

    cout << "\033[33m-- pFDpCD event counts ----------------------------------------------------\n\033[0m";
    cout << "\033[33m#(events) pFDpCD:\t\t\t" << num_of_events_pFDpCD << "\n\n\033[0m";

    cout << "\033[33m-- Event counts -----------------------------------------------------------\n\033[0m";
    cout << "\033[33mnum_of_events_1p_inFD:\t\t\t" << num_of_events_1p_inFD << "\n\n\033[0m";

    cout << "\033[33mnum_of_events_1n_inFD:\t\t\t" << num_of_events_1n_inFD << "\n\033[0m";
    cout << "\033[33mnum_of_events_1n_inFD_AV:\t\t" << num_of_events_1n_inFD_AV << "\n\033[0m";
    cout << "\033[33mnum_of_events_1n_inFD_AV_wmt1gTLn:\t" << num_of_events_1n_inFD_AV_wmt1gTLn << "\n\n\033[0m";

    cout << "\033[33mnum_of_events_2p_wFakeProtons:\t\t" << num_of_events_2p_wFakeProtons << "\n\033[0m";
    cout << "\033[33mnum_of_events_2p:\t\t\t" << num_of_events_2p << "\n\n\033[0m";

    cout << "\033[33mnum_of_events_1epFDpCD:\t\t\t" << num_of_events_with_1epFDpCD << "\n\033[0m";
    cout << "\033[33mnum_of_events_1epFDpFD:\t\t\t" << num_of_events_with_1epFDpFD << "\n\033[0m";
    cout << "\033[33mnum_of_events_1epCDpCD:\t\t\t" << num_of_events_with_1epCDpCD << "\n\n\033[0m";

    cout << "\033[33mnum_of_events_pFDpCD:\t\t\t" << num_of_events_pFDpCD << "\n\n\033[0m";

    cout << "\033[33mnum_of_events_nFDpCD:\t\t\t" << num_of_events_nFDpCD << "\n\033[0m";
    cout << "\033[33mnum_of_events_nFDpCD_AV:\t\t" << num_of_events_nFDpCD_AV << "\n\n\033[0m";

    cout << "\033[33m---------------------------------------------------------------------------\n\033[0m";
    cout << "\033[33m\t\t\tExecution variables\n\033[0m";
    cout << "\033[33m---------------------------------------------------------------------------\n\n\033[0m";

    cout << "\033[33mWorkingDirectory:\t" << WorkingDirectory << "\n\033[0m";
    cout << "\033[33mrun_plots_path:\t\t" << run_plots_path << "\n\n\033[0m";

    cout << "\033[33mAnalyseFilePath:\t" << "/" << AnalyseFilePath << "/" << "\n\033[0m";
    cout << "\033[33mAnalyseFileSample:\t" << "/" << AnalyseFileSample << "/" << "\n\033[0m";
    cout << "\033[33mAnalyseFile:\t\t" << AnalyseFile << "\n\n\033[0m";

    cout << "\033[33mSampleName:\t\t" << SampleName << "\n\033[0m";
    cout << "\033[33mVaryingSampleName:\t" << VaryingSampleName << "\n\n\033[0m";

    cout << "\033[33mapply_cuts:\t\t'" << BoolToString(apply_cuts) << "'\n\033[0m";
    cout << "\033[33mSettings mode:\t\t'" << file_name << "'\n\n\033[0m";

    cout << "\033[33mBeam Energy:\t\t" << beamE << " [GeV]\n\033[0m";
    cout << "\033[33mTarget:\t\t\t" << Target << " (PDG: " << TargetPDG << ")\n\n\033[0m";

    cout << "\033[33mOperation finished (AnalyserVersion = " << AnalyserVersion << ")." << "\n\n\033[0m";

    if (TestRun || ApplyLimiter) {
        if (TestRun) { cout << "\033[31m\n\nNOTE: running code in testing mode!\n\033[0m"; }

        if (ApplyLimiter) { cout << "\033[31m\n\nNOTE: running code with a limiter on number of events!\n\n\033[0m"; }
    }

    if (Save_Plots_folder_to_zip_files) {
        // TODO: this saves the plots folder in multiple folders in the save path - fix!

        // cout << "\n\nrun_plots_path: " << run_plots_path << "\n";
        // cout << "settings.GetRun_dir_name(): " << settings.GetRun_dir_name() << "\n";
        // exit(0);
        string zip_filename = settings.GetRun_dir_name() + ".zip";
        string zip_input_path = run_plots_path;
        string zip_input = run_plots_path + "/" + zip_filename;
        string zip_output_path = run_plots_path;
        cout << "\n\nMaking zip file...\n";
        cout << "zip_filename: " << zip_filename << "\n";
        cout << "zip_input_path: " << zip_input_path << "\n";
        cout << "zip_input: " << zip_input << "\n";
        cout << "zip_output_path: " << zip_output_path << "\n";
        system(("zip -r " + zip_input + " " + zip_output_path).c_str());
        // system(("mv -r " + run_plots_path + "/" + settings.GetRun_dir_name() + "/" + settings.GetRun_dir_name() + ".zip " + run_plots_path).c_str());
    }
}