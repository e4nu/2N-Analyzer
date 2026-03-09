#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include <tuple>
#include <vector>

#include "ECAL_vetoes.cpp"

namespace bt = basic_tools;
namespace am = analysis_math;
namespace raf = reco_analysis_functions;
namespace hf = histogram_functions;

void Ph_nFD_sep_body_function(int& LoopNumber, int version, double Ebeam, vector<bool> Ebeam_bool_v, int Limiter, vector<bool> CutSelector, bool use_ConstPn_samples, bool apply_neutFD_redef,
                              bool apply_cPart_ECAL_veto, double cPart_veto_radii, bool apply_nPart_ECAL_veto, double nPart_veto_radii, bool apply_no_same_sector_cuts,
                              bool apply_nFD_multi_cut, bool apply_no_FD_photons_cut, bool apply_no_other_part_multi_cut, bool apply_theta_nFD_e_vs_dTheta_cuts, bool apply_dTheta_cuts,
                              DSCuts dTheta_cuts, ParticleID pid, DSCuts e_mom_th, DSCuts pCD_mom_th, DSCuts pFD_mom_th, DSCuts phFD_mom_th, DSCuts pip_mom_th, DSCuts pipFD_mom_th,
                              DSCuts pipCD_mom_th, DSCuts pim_mom_th, DSCuts pimFD_mom_th, DSCuts pimCD_mom_th, DSCuts nFD_mom_th, bool apply_PCAL_neutral_veto, bool ConstrainTLmom,
                              bool plot_tester_histograms, std::string General_status, bool plot_AMaps, bool plot_and_fit_MomRes) {
    std::cout << "\033[33m\n\nRunning Ph_nFD_sep_body_function...\n\033[0m" << std::flush;
    std::cout << "\033[33m\n\nLoopNumber = " << LoopNumber << "\n\033[0m" << std::flush;

#pragma region /* Parameter processing */

    /* Solenoid field parameters */
    double B = 5.0;  // Tesla
    double L = 0.5;  // meters

    bool apply_p_multi_cut = (apply_no_other_part_multi_cut || false);
    bool apply_pos_multi_cut = (apply_no_other_part_multi_cut || false);
    bool apply_pip_multi_cut = (apply_no_other_part_multi_cut || false), apply_pim_multi_cut = (apply_no_other_part_multi_cut || false);
    bool apply_Kp_multi_cut = (apply_no_other_part_multi_cut || false), apply_Km_multi_cut = (apply_no_other_part_multi_cut || false);
    bool apply_D_multi_cut = (apply_no_other_part_multi_cut || false);
    bool apply_other_part_multi_cut = (apply_no_other_part_multi_cut || false);

    std::string neutFD_redef_status = "";       // We always redefine nFD, so status is empty
    std::string PCAL_neutral_veto_status = "";  // We do not use PCAL neutral veto, so status is empty

    std::string OutFolderName_prefix = bt::ToStringWithPrecision(version, 0) + "_2N_analyzer_";
    std::string OutFolderName_ver_status = "_v" + bt::ToStringWithPrecision(version, 0);
    std::string samples_status = use_ConstPn_samples ? "_CPn" : "_FPn";
    std::string ReDef_status = apply_neutFD_redef ? "_Redef" : "_clas12def";
    std::string cPart_ECAL_veto_status = apply_cPart_ECAL_veto ? "_wcEV" : "_wocEV";
    std::string nPart_ECAL_veto_status = apply_nPart_ECAL_veto ? "_wnEV" : "_wonEV";
    std::string no_same_sector_cuts_status = "";
    // std::string no_same_sector_cuts_status = apply_no_same_sector_cuts ? "_wSSC" : "_woSSC";
    std::string nFD_multi_status = apply_nFD_multi_cut ? "_wNMC" : "_woNMC";
    std::string FD_photons_status = apply_no_FD_photons_cut ? "_NoPhFDc_0" + bt::ToStringWithPrecision(10 * phFD_mom_th.GetLowerCut(), 0) : "_wPhFD";
    std::string no_other_part_status = apply_no_other_part_multi_cut ? "_woOP" : "_wOP";
    std::string dTheta_status = "";
    // std::string dTheta_status = apply_theta_nFD_e_vs_dTheta_cuts
    //                                 ? "_wdThetaC_" + bt::ToStringWithPrecision(dTheta_cuts.GetLowerCut(), 0) + "_to_" + bt::ToStringWithPrecision(dTheta_cuts.GetUpperCut(), 0)
    //                                 : "_wodThetaC";
    std::string dPhi_status = "";

    std::string sample_size_status = (Limiter == 25000000)   ? "_full"
                                     : (Limiter == 10000000) ? "_1000files"
                                     : (Limiter == 5000000)  ? "_500files"
                                     : (Limiter == 1000000)  ? "_100files"
                                     : (Limiter == 100000)   ? "_10files"
                                     : (Limiter == 10000)    ? "_1file"
                                                             : "";

    General_status = (General_status == "") ? General_status : "__" + General_status;
    General_status = General_status + sample_size_status;

#pragma endregion

#pragma region /* Setup */

    std::cout << "\033[33m\n\nSetting up code...\n\033[0m" << std::flush;

    // double Ebeam = Ebeam_v.at(Ebeam_ind);
    bool Is2GeV = Ebeam_bool_v.at(0), Is4GeV = Ebeam_bool_v.at(1), Is6GeV = Ebeam_bool_v.at(2);

    double cPart_veto_radius = cPart_veto_radii, nPart_veto_radius = nPart_veto_radii;

    bool ConstrainedE = CutSelector.at(0), OnlyGood_nFD = CutSelector.at(1), OnlyBad_nFD = CutSelector.at(2);

    std::string Ebeam_status = Is2GeV ? "_2GeV" : Is4GeV ? "_4GeV" : Is6GeV ? "_6GeV" : "_Unknown";
    std::string Ebeam_status0 = Is2GeV ? "2GeV" : Is4GeV ? "4GeV" : Is6GeV ? "6GeV" : "Unknown";
    std::string rc_factor_status = "";
    // std::string rc_factor_status = apply_cPart_ECAL_veto ? "_rc" + ToStringWithPrecision(cPart_veto_radius, 0) : "";
    std::string nPart_veto_radius_status = apply_PCAL_neutral_veto ? "_rn" + ToStringWithPrecision(nPart_veto_radius, 0) : "";
    std::string Good_nFD_status = (OnlyGood_nFD && !OnlyBad_nFD) ? "_OnlyGood" : "";
    // std::string Good_nFD_status = (OnlyGood_nFD && !OnlyBad_nFD) ? "_OnlyGood_nFD" : "";
    std::string Bad_nFD_status = (!OnlyGood_nFD && OnlyBad_nFD) ? "_OnlyBad" : "";
    // std::string Bad_nFD_status = (!OnlyGood_nFD && OnlyBad_nFD) ? "_OnlyBad_nFD" : "";
    std::string ConstrainedE_status = ConstrainedE ? "_CE" : "";

    std::string ParentOutFolderName = OutFolderName_prefix + OutFolderName_ver_status;
    std::string OutFolderName = OutFolderName_prefix + OutFolderName_ver_status + Ebeam_status + samples_status + ReDef_status + cPart_ECAL_veto_status + nPart_ECAL_veto_status +
                                no_same_sector_cuts_status + nFD_multi_status + FD_photons_status + no_other_part_status + dTheta_status + dPhi_status + neutFD_redef_status +
                                PCAL_neutral_veto_status + rc_factor_status + nPart_veto_radius_status + Good_nFD_status + Bad_nFD_status + ConstrainedE_status + General_status;

    std::string OutFileName = OutFolderName;

    if (OnlyGood_nFD == OnlyBad_nFD) { OnlyGood_nFD = OnlyBad_nFD = false; }

    double P_upperLim = ConstrainTLmom ? (Ebeam * 0.5) : (Ebeam * 1.1);
    const std::string ParentOutputDir = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + ParentOutFolderName;
    // system(("rm -rf " + ParentOutputDir).c_str());
    system(("mkdir -p " + ParentOutputDir).c_str());

    const std::string OutputDir = ParentOutputDir + "/" + OutFolderName;
    system(("rm -rf " + OutputDir).c_str());
    system(("mkdir -p " + OutputDir).c_str());

    const std::string PlotsBySliceOutputDir = OutputDir + "/PlotsBySlice";
    system(("rm -rf " + PlotsBySliceOutputDir).c_str());
    system(("mkdir -p " + PlotsBySliceOutputDir).c_str());

    std::string BaseDir = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples";
    std::string InputFiles, SampleName;
    TString Beam_energy_TString;

    if (Is2GeV) {
        InputFiles = use_ConstPn_samples ? (BaseDir + "/2070MeV_ConstPn/OutPut_en/reconhipo/*.hipo") : (BaseDir + "/2070MeV/OutPut_en/reconhipo/*.hipo");

        SampleName = "Uniform_en_sample_2070MeV";
        Beam_energy_TString = "2070MeV";
    } else if (Is4GeV) {
        InputFiles = use_ConstPn_samples ? (BaseDir + "/4029MeV_ConstPn/OutPut_en/reconhipo/*.hipo") : (BaseDir + "/4029MeV/OutPut_en/reconhipo/*.hipo");

        SampleName = "Uniform_en_sample_4029MeV";
        Beam_energy_TString = "4029MeV";
    } else if (Is6GeV) {
        InputFiles = use_ConstPn_samples ? (BaseDir + "/5986MeV_ConstPn/OutPut_en/reconhipo/*.hipo") : (BaseDir + "/5986MeV_target_zpos_test/OutPut_en/reconhipo/*.hipo");
        // InputFiles = use_ConstPn_samples ? (BaseDir + "/5986MeV_ConstPn/OutPut_en/reconhipo/*.hipo") : (BaseDir + "/5986MeV/OutPut_en/reconhipo/*.hipo");

        SampleName = "Uniform_en_sample_5986MeV";
        Beam_energy_TString = "5986MeV";
    }

    double DeltaSlices = 0.05;

    vector<vector<double>> nFD_Res_SliceLimits = Set_Mom_Res_Slices(Ebeam, nFD_mom_th.GetLowerCutConst(), DeltaSlices);

    std::cout << "\033[33m\n\nSetting up HipoChain...\n\033[0m" << std::flush;

    clas12root::HipoChain chain;
    chain.Add(InputFiles);
    chain.SetReaderTags({0});
    chain.db()->turnOffQADB();
    auto config_c12 = chain.GetC12Reader();
    const std::unique_ptr<clas12::clas12reader>& c12 = chain.C12ref();

#pragma endregion

#pragma region /* Prepare histograms */

    /////////////////////////////////////
    // Prepare histograms
    /////////////////////////////////////

    std::cout << "\033[33m\n\nPrepareing histograms...\n\033[0m" << std::flush;

    vector<TH1*> HistoList_RawTruth;

    vector<TH1*> HistoList_truth_1e_cut;

    vector<TH1*> HistoList_PID_plots;

    vector<TH1*> HistoList_raw;

    vector<TH1*> HistoList_clas12reco;

    vector<TH1*> HistoList_redef;

    vector<TH1*> HistoList;

    vector<TObject*> HistoList_nRes_BysliceOf_truth_P_nFD;
    vector<TObject*> HistoList_nRes_BysliceOf_reco_P_nFD;

    vector<TObject*> HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n;

    vector<vector<double>> dr_n_SliceLimits = Set_dr_n_Slices(10., 0., 300.);

    vector<std::string> HistSubjects;
    vector<std::string> HistSubjects2;
    vector<bool> FirstPrint;

    gStyle->SetTitleXSize(0.05), gStyle->SetTitleYSize(0.05);
    gStyle->SetTitleXOffset(0.8), gStyle->SetTitleYOffset(0.8);

#pragma region /* Truth 1e cut */

    HistSubjects.push_back("Truth 1e cut");
    HistSubjects2.push_back("Truth 1e cut");
    FirstPrint.push_back(true);

    TH1D* h_truth_P_nFD_truth_1e_cut = new TH1D("truth_P_nFD_truth_1e_cut", "P^{truth}_{nFD} (truth 1e cut);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_truth_1e_cut.push_back(h_truth_P_nFD_truth_1e_cut);
    TH1D* h_truth_theta_nFD_truth_1e_cut = new TH1D("truth_theta_nFD_truth_1e_cut", "#theta^{truth}_{nFD} (truth 1e cut);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList_truth_1e_cut.push_back(h_truth_theta_nFD_truth_1e_cut);
    TH1D* h_truth_phi_nFD_truth_1e_cut = new TH1D("truth_phi_nFD_truth_1e_cut", "#phi^{truth}_{nFD} (truth 1e cut);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList_truth_1e_cut.push_back(h_truth_phi_nFD_truth_1e_cut);
    TH2D* h_truth_theta_nFD_VS_truth_phi_nFD_truth_1e_cut =
        new TH2D("truth_theta_nFD_VS_truth_phi_nFD_truth_1e_cut", "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} (truth 1e cut);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100,
                 -180., 180., 100, 0, 50.);
    HistoList_truth_1e_cut.push_back(h_truth_theta_nFD_VS_truth_phi_nFD_truth_1e_cut);
    TH2D* h_truth_theta_nFD_VS_truth_P_nFD_truth_1e_cut =
        new TH2D("truth_theta_nFD_VS_truth_P_nFD_truth_1e_cut", "#theta^{truth}_{nFD} vs. P^{truth}_{nFD} (truth 1e cut);P^{truth}_{nFD} [GeV/c];#theta^{truth}_{nFD} [#circ]", 100, 0,
                 P_upperLim, 100, 0, 50.);
    HistoList_truth_1e_cut.push_back(h_truth_theta_nFD_VS_truth_P_nFD_truth_1e_cut);

    cout << "\033[33m\n\nFinished setting up Truth 1e cut histograms.\n\033[0m" << std::flush;

#pragma endregion

#pragma region /* RAW truth */

    HistSubjects.push_back("Raw truth");
    HistSubjects2.push_back("Raw truth");
    FirstPrint.push_back(true);

    TH1D* h_truth_P_e_RawTruth_1e_cut = new TH1D("truth_P_e_RawTruth_1e_cut", "P^{truth}_{e} in 1e cut (RawTruth);P^{truth}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_RawTruth.push_back(h_truth_P_e_RawTruth_1e_cut);
    TH1D* h_truth_theta_e_RawTruth_1e_cut = new TH1D("truth_theta_e_RawTruth_1e_cut", "#theta^{truth}_{e} in 1e cut (RawTruth);#theta^{truth}_{e} [#circ];Counts", 50, 0, 45.);
    HistoList_RawTruth.push_back(h_truth_theta_e_RawTruth_1e_cut);
    TH1D* h_truth_phi_e_RawTruth_1e_cut = new TH1D("truth_phi_e_RawTruth_1e_cut", "#phi^{truth}_{e} in 1e cut (RawTruth);#phi^{truth}_{e} [#circ];Counts", 50, -180, 180.);
    HistoList_RawTruth.push_back(h_truth_phi_e_RawTruth_1e_cut);
    TH2D* h_truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut =
        new TH2D("truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut", "#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut (RawTruth);#phi^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]", 100,
                 -180., 180., 100, 0, 50.);
    HistoList_RawTruth.push_back(h_truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut);
    TH2D* h_truth_theta_e_VS_truth_P_e_RawTruth_1e_cut =
        new TH2D("truth_theta_e_VS_truth_P_e_RawTruth_1e_cut", "#theta^{truth}_{e} vs. P^{truth}_{e} in 1e cut (RawTruth);P^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]", 100, 0,
                 P_upperLim, 100, 0, 50.);
    HistoList_RawTruth.push_back(h_truth_theta_e_VS_truth_P_e_RawTruth_1e_cut);

    TH1D* h_truth_P_nFD_RawTruth_1e_cut = new TH1D("truth_P_nFD_RawTruth_1e_cut", "P^{truth}_{nFD} in 1e cut (RawTruth);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_RawTruth.push_back(h_truth_P_nFD_RawTruth_1e_cut);
    TH1D* h_truth_theta_nFD_RawTruth_1e_cut = new TH1D("truth_theta_nFD_RawTruth_1e_cut", "#theta^{truth}_{nFD} in 1e cut (RawTruth);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList_RawTruth.push_back(h_truth_theta_nFD_RawTruth_1e_cut);
    TH1D* h_truth_phi_nFD_RawTruth_1e_cut = new TH1D("truth_phi_nFD_RawTruth_1e_cut", "#phi^{truth}_{nFD} in 1e cut (RawTruth);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList_RawTruth.push_back(h_truth_phi_nFD_RawTruth_1e_cut);
    TH2D* h_truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut =
        new TH2D("truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (RawTruth);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList_RawTruth.push_back(h_truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut);
    TH2D* h_truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut =
        new TH2D("truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut", "#theta^{truth}_{nFD} vs. P^{truth}_{nFD} in 1e cut (RawTruth);P^{truth}_{nFD} [GeV/c];#theta^{truth}_{nFD} [#circ]", 100,
                 0, P_upperLim, 100, 0, 50.);
    HistoList_RawTruth.push_back(h_truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut);

    cout << "\033[33m\n\nFinished setting up RAW truth histograms.\n\033[0m" << std::flush;

#pragma endregion

#pragma region /* RAW reco */

#pragma region /* Electron pre-selection and PID */

    TH1D* h_Vz_e_BC_1e_cut = new TH1D("Vz_e_BC_1e_cut", "V_{z}^{e} in 1e cut (before cut);V_{z}^{e} [cm];Counts", 50, -8, 8);
    HistoList_PID_plots.push_back(h_Vz_e_BC_1e_cut);
    TH1D* h_Vz_e_AC_1e_cut = new TH1D("Vz_e_AC_1e_cut", "V_{z}^{e} in 1e cut (after cut);V_{z}^{e} [cm];Counts", 50, -8, 8);
    HistoList_PID_plots.push_back(h_Vz_e_AC_1e_cut);

    TH2D* h_dc_electron_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D* h_dc_electron_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_electron_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_BC_%d", i), Form("DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_electron_hit_map_BC_1e_cut[i]);
        h_dc_electron_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_AC_%d", i), Form("DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_electron_hit_map_AC_1e_cut[i]);
    }

    TH1D* h_nphe_BC_1e_cut = new TH1D("nphe_BC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList_PID_plots.push_back(h_nphe_BC_1e_cut);
    TH1D* h_nphe_AC_1e_cut = new TH1D("nphe_AC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList_PID_plots.push_back(h_nphe_AC_1e_cut);

    TH2D* h_Edep_PCAL_VS_EC_BC_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
                 0.2, 100, 0, 0.3);
    HistoList_PID_plots.push_back(h_Edep_PCAL_VS_EC_BC_1e_cut);
    TH2D* h_Edep_PCAL_VS_EC_AC_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2,
                 100, 0, 0.3);
    HistoList_PID_plots.push_back(h_Edep_PCAL_VS_EC_AC_1e_cut);

    TH2D* h_SF_VS_P_e_BC_1e_cut =
        new TH2D("SF_VS_P_e_BC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList_PID_plots.push_back(h_SF_VS_P_e_BC_1e_cut);
    TH2D* h_SF_VS_P_e_AC_1e_cut =
        new TH2D("SF_VS_P_e_AC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList_PID_plots.push_back(h_SF_VS_P_e_AC_1e_cut);

    TH2D* h_SF_VS_Lv_BC_1e_cut = new TH2D("SF_VS_Lv_BC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_PID_plots.push_back(h_SF_VS_Lv_BC_1e_cut);
    TH2D* h_SF_VS_Lv_AC_1e_cut = new TH2D("SF_VS_Lv_AC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_PID_plots.push_back(h_SF_VS_Lv_AC_1e_cut);

    TH2D* h_SF_VS_Lw_BC_1e_cut = new TH2D("SF_VS_Lw_BC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_PID_plots.push_back(h_SF_VS_Lw_BC_1e_cut);
    TH2D* h_SF_VS_Lw_AC_1e_cut = new TH2D("SF_VS_Lw_AC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_PID_plots.push_back(h_SF_VS_Lw_AC_1e_cut);

    TH2D* h_SF_VS_Lu_BC_1e_cut = new TH2D("SF_VS_Lu_BC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_PID_plots.push_back(h_SF_VS_Lu_BC_1e_cut);
    TH2D* h_SF_VS_Lu_AC_1e_cut = new TH2D("SF_VS_Lu_AC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_PID_plots.push_back(h_SF_VS_Lu_AC_1e_cut);

    TH2D* h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
    HistoList_PID_plots.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut);
    TH2D* h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
    HistoList_PID_plots.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut);

#pragma endregion

#pragma region /* Proton pre-selection and PID */

    TH1D* h_Vz_pFD_BC_1e_cut = new TH1D("Vz_pFD_BC_1e_cut", "V_{z}^{pFD} in 1e cut (before cut);V_{z}^{pFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pFD_BC_1e_cut);
    TH1D* h_Vz_pFD_AC_1e_cut = new TH1D("Vz_pFD_AC_1e_cut", "V_{z}^{pFD} in 1e cut (after cut);V_{z}^{pFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pFD_AC_1e_cut);

    TH1D* h_dVz_pFD_BC_1e_cut = new TH1D("dVz_pFD_BC_1e_cut", "dV_{z}^{pFD} in 1e cut (before cut);dV_{z}^{pFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pFD_BC_1e_cut);
    TH1D* h_dVz_pFD_AC_1e_cut = new TH1D("dVz_pFD_AC_1e_cut", "dV_{z}^{pFD} in 1e cut (after cut);dV_{z}^{pFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pFD_AC_1e_cut);

    TH1D* h_chi2_pFD_BC_1e_cut = new TH1D("chi2_pFD_BC_1e_cut", "#chi^{2}_{pFD} in 1e cut (before cut);#chi^{2}_{pFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pFD_BC_1e_cut);
    TH1D* h_chi2_pFD_AC_1e_cut = new TH1D("chi2_pFD_AC_1e_cut", "#chi^{2}_{pFD} in 1e cut (after cut);#chi^{2}_{pFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pFD_AC_1e_cut);

    TH1D* h_Vz_pCD_BC_1e_cut = new TH1D("Vz_pCD_BC_1e_cut", "V_{z}^{pCD} in 1e cut (before cut);V_{z}^{pCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pCD_BC_1e_cut);
    TH1D* h_Vz_pCD_AC_1e_cut = new TH1D("Vz_pCD_AC_1e_cut", "V_{z}^{pCD} in 1e cut (after cut);V_{z}^{pCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pCD_AC_1e_cut);

    TH1D* h_dVz_pCD_BC_1e_cut = new TH1D("dVz_pCD_BC_1e_cut", "dV_{z}^{pCD} in 1e cut (before cut);dV_{z}^{pCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pCD_BC_1e_cut);
    TH1D* h_dVz_pCD_AC_1e_cut = new TH1D("dVz_pCD_AC_1e_cut", "dV_{z}^{pCD} in 1e cut (after cut);dV_{z}^{pCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pCD_AC_1e_cut);

    TH1D* h_chi2_pCD_BC_1e_cut = new TH1D("chi2_pCD_BC_1e_cut", "#chi^{2}_{pCD} in 1e cut (before cut);#chi^{2}_{pCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pCD_BC_1e_cut);
    TH1D* h_chi2_pCD_AC_1e_cut = new TH1D("chi2_pCD_AC_1e_cut", "#chi^{2}_{pCD} in 1e cut (after cut);#chi^{2}_{pCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pCD_AC_1e_cut);

    TH2D* h_dc_pFD_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D* h_dc_pFD_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pFD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_pFD_hit_map_BC_%d", i), Form("pFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_pFD_hit_map_BC_1e_cut[i]);
        h_dc_pFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_pFD_hit_map_AC_%d", i), Form("pFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_pFD_hit_map_AC_1e_cut[i]);
    }

#pragma endregion

#pragma region /* Piplus pre-selection and PID */

    TH1D* h_Vz_pipFD_BC_1e_cut = new TH1D("Vz_pipFD_BC_1e_cut", "V_{z}^{pipFD} in 1e cut (before cut);V_{z}^{pipFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pipFD_BC_1e_cut);
    TH1D* h_Vz_pipFD_AC_1e_cut = new TH1D("Vz_pipFD_AC_1e_cut", "V_{z}^{pipFD} in 1e cut (after cut);V_{z}^{pipFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pipFD_AC_1e_cut);

    TH1D* h_dVz_pipFD_BC_1e_cut = new TH1D("dVz_pipFD_BC_1e_cut", "dV_{z}^{pipFD} in 1e cut (before cut);dV_{z}^{pipFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pipFD_BC_1e_cut);
    TH1D* h_dVz_pipFD_AC_1e_cut = new TH1D("dVz_pipFD_AC_1e_cut", "dV_{z}^{pipFD} in 1e cut (after cut);dV_{z}^{pipFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pipFD_AC_1e_cut);

    TH1D* h_chi2_pipFD_BC_1e_cut = new TH1D("chi2_pipFD_BC_1e_cut", "#chi^{2}_{pipFD} in 1e cut (before cut);#chi^{2}_{pipFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pipFD_BC_1e_cut);
    TH1D* h_chi2_pipFD_AC_1e_cut = new TH1D("chi2_pipFD_AC_1e_cut", "#chi^{2}_{pipFD} in 1e cut (after cut);#chi^{2}_{pipFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pipFD_AC_1e_cut);

    TH1D* h_Vz_pipCD_BC_1e_cut = new TH1D("Vz_pipCD_BC_1e_cut", "V_{z}^{pipCD} in 1e cut (before cut);V_{z}^{pipCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pipCD_BC_1e_cut);
    TH1D* h_Vz_pipCD_AC_1e_cut = new TH1D("Vz_pipCD_AC_1e_cut", "V_{z}^{pipCD} in 1e cut (after cut);V_{z}^{pipCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pipCD_AC_1e_cut);

    TH1D* h_dVz_pipCD_BC_1e_cut = new TH1D("dVz_pipCD_BC_1e_cut", "dV_{z}^{pipCD} in 1e cut (before cut);dV_{z}^{pipCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pipCD_BC_1e_cut);
    TH1D* h_dVz_pipCD_AC_1e_cut = new TH1D("dVz_pipCD_AC_1e_cut", "dV_{z}^{pipCD} in 1e cut (after cut);dV_{z}^{pipCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pipCD_AC_1e_cut);

    TH1D* h_chi2_pipCD_BC_1e_cut = new TH1D("chi2_pipCD_BC_1e_cut", "#chi^{2}_{pipCD} in 1e cut (before cut);#chi^{2}_{pipCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pipCD_BC_1e_cut);
    TH1D* h_chi2_pipCD_AC_1e_cut = new TH1D("chi2_pipCD_AC_1e_cut", "#chi^{2}_{pipCD} in 1e cut (after cut);#chi^{2}_{pipCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pipCD_AC_1e_cut);

    TH2D* h_dc_pipFD_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D* h_dc_pipFD_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pipFD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_pipFD_hit_map_BC_%d", i), Form("pipFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_pipFD_hit_map_BC_1e_cut[i]);
        h_dc_pipFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_pipFD_hit_map_AC_%d", i), Form("pipFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_pipFD_hit_map_AC_1e_cut[i]);
    }

#pragma endregion

#pragma region /* Piminus pre-selection and PID */

    TH1D* h_Vz_pimFD_BC_1e_cut = new TH1D("Vz_pimFD_BC_1e_cut", "V_{z}^{pimFD} in 1e cut (before cut);V_{z}^{pimFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pimFD_BC_1e_cut);
    TH1D* h_Vz_pimFD_AC_1e_cut = new TH1D("Vz_pimFD_AC_1e_cut", "V_{z}^{pimFD} in 1e cut (after cut);V_{z}^{pimFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pimFD_AC_1e_cut);

    TH1D* h_dVz_pimFD_BC_1e_cut = new TH1D("dVz_pimFD_BC_1e_cut", "dV_{z}^{pimFD} in 1e cut (before cut);dV_{z}^{pimFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pimFD_BC_1e_cut);
    TH1D* h_dVz_pimFD_AC_1e_cut = new TH1D("dVz_pimFD_AC_1e_cut", "dV_{z}^{pimFD} in 1e cut (after cut);dV_{z}^{pimFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pimFD_AC_1e_cut);

    TH1D* h_chi2_pimFD_BC_1e_cut = new TH1D("chi2_pimFD_BC_1e_cut", "#chi^{2}_{pimFD} in 1e cut (before cut);#chi^{2}_{pimFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pimFD_BC_1e_cut);
    TH1D* h_chi2_pimFD_AC_1e_cut = new TH1D("chi2_pimFD_AC_1e_cut", "#chi^{2}_{pimFD} in 1e cut (after cut);#chi^{2}_{pimFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pimFD_AC_1e_cut);

    TH1D* h_Vz_pimCD_BC_1e_cut = new TH1D("Vz_pimCD_BC_1e_cut", "V_{z}^{pimCD} in 1e cut (before cut);V_{z}^{pimCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pimCD_BC_1e_cut);
    TH1D* h_Vz_pimCD_AC_1e_cut = new TH1D("Vz_pimCD_AC_1e_cut", "V_{z}^{pimCD} in 1e cut (after cut);V_{z}^{pimCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_pimCD_AC_1e_cut);

    TH1D* h_dVz_pimCD_BC_1e_cut = new TH1D("dVz_pimCD_BC_1e_cut", "dV_{z}^{pimCD} in 1e cut (before cut);dV_{z}^{pimCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pimCD_BC_1e_cut);
    TH1D* h_dVz_pimCD_AC_1e_cut = new TH1D("dVz_pimCD_AC_1e_cut", "dV_{z}^{pimCD} in 1e cut (after cut);dV_{z}^{pimCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_pimCD_AC_1e_cut);

    TH1D* h_chi2_pimCD_BC_1e_cut = new TH1D("chi2_pimCD_BC_1e_cut", "#chi^{2}_{pimCD} in 1e cut (before cut);#chi^{2}_{pimCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pimCD_BC_1e_cut);
    TH1D* h_chi2_pimCD_AC_1e_cut = new TH1D("chi2_pimCD_AC_1e_cut", "#chi^{2}_{pimCD} in 1e cut (after cut);#chi^{2}_{pimCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_pimCD_AC_1e_cut);

    TH2D* h_dc_pimFD_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D* h_dc_pimFD_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pimFD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_pimFD_hit_map_BC_%d", i), Form("pimFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_pimFD_hit_map_BC_1e_cut[i]);
        h_dc_pimFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_pimFD_hit_map_AC_%d", i), Form("pimFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_pimFD_hit_map_AC_1e_cut[i]);
    }

#pragma endregion

#pragma region /* K plus pre-selection and PID */

    TH1D* h_Vz_kpFD_BC_1e_cut = new TH1D("Vz_kpFD_BC_1e_cut", "V_{z}^{kpFD} in 1e cut (before cut);V_{z}^{kpFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_kpFD_BC_1e_cut);
    TH1D* h_Vz_kpFD_AC_1e_cut = new TH1D("Vz_kpFD_AC_1e_cut", "V_{z}^{kpFD} in 1e cut (after cut);V_{z}^{kpFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_kpFD_AC_1e_cut);

    TH1D* h_dVz_kpFD_BC_1e_cut = new TH1D("dVz_kpFD_BC_1e_cut", "dV_{z}^{kpFD} in 1e cut (before cut);dV_{z}^{kpFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_kpFD_BC_1e_cut);
    TH1D* h_dVz_kpFD_AC_1e_cut = new TH1D("dVz_kpFD_AC_1e_cut", "dV_{z}^{kpFD} in 1e cut (after cut);dV_{z}^{kpFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_kpFD_AC_1e_cut);

    TH1D* h_chi2_kpFD_BC_1e_cut = new TH1D("chi2_kpFD_BC_1e_cut", "#chi^{2}_{kpFD} in 1e cut (before cut);#chi^{2}_{kpFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_kpFD_BC_1e_cut);
    TH1D* h_chi2_kpFD_AC_1e_cut = new TH1D("chi2_kpFD_AC_1e_cut", "#chi^{2}_{kpFD} in 1e cut (after cut);#chi^{2}_{kpFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_kpFD_AC_1e_cut);

    TH1D* h_Vz_kpCD_BC_1e_cut = new TH1D("Vz_kpCD_BC_1e_cut", "V_{z}^{kpCD} in 1e cut (before cut);V_{z}^{kpCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_kpCD_BC_1e_cut);
    TH1D* h_Vz_kpCD_AC_1e_cut = new TH1D("Vz_kpCD_AC_1e_cut", "V_{z}^{kpCD} in 1e cut (after cut);V_{z}^{kpCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_kpCD_AC_1e_cut);

    TH1D* h_dVz_kpCD_BC_1e_cut = new TH1D("dVz_kpCD_BC_1e_cut", "dV_{z}^{kpCD} in 1e cut (before cut);dV_{z}^{kpCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_kpCD_BC_1e_cut);
    TH1D* h_dVz_kpCD_AC_1e_cut = new TH1D("dVz_kpCD_AC_1e_cut", "dV_{z}^{kpCD} in 1e cut (after cut);dV_{z}^{kpCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_kpCD_AC_1e_cut);

    TH1D* h_chi2_kpCD_BC_1e_cut = new TH1D("chi2_kpCD_BC_1e_cut", "#chi^{2}_{kpCD} in 1e cut (before cut);#chi^{2}_{kpCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_kpCD_BC_1e_cut);
    TH1D* h_chi2_kpCD_AC_1e_cut = new TH1D("chi2_kpCD_AC_1e_cut", "#chi^{2}_{kpCD} in 1e cut (after cut);#chi^{2}_{kpCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_kpCD_AC_1e_cut);

    TH2D* h_dc_kpFD_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D* h_dc_kpFD_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_kpFD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_kpFD_hit_map_BC_%d", i), Form("kpFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_kpFD_hit_map_BC_1e_cut[i]);
        h_dc_kpFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_kpFD_hit_map_AC_%d", i), Form("kpFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_kpFD_hit_map_AC_1e_cut[i]);
    }

#pragma endregion

#pragma region /* K minus pre-selection and PID */

    TH1D* h_Vz_kmFD_BC_1e_cut = new TH1D("Vz_kmFD_BC_1e_cut", "V_{z}^{kmFD} in 1e cut (before cut);V_{z}^{kmFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_kmFD_BC_1e_cut);
    TH1D* h_Vz_kmFD_AC_1e_cut = new TH1D("Vz_kmFD_AC_1e_cut", "V_{z}^{kmFD} in 1e cut (after cut);V_{z}^{kmFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_kmFD_AC_1e_cut);

    TH1D* h_dVz_kmFD_BC_1e_cut = new TH1D("dVz_kmFD_BC_1e_cut", "dV_{z}^{kmFD} in 1e cut (before cut);dV_{z}^{kmFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_kmFD_BC_1e_cut);
    TH1D* h_dVz_kmFD_AC_1e_cut = new TH1D("dVz_kmFD_AC_1e_cut", "dV_{z}^{kmFD} in 1e cut (after cut);dV_{z}^{kmFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_kmFD_AC_1e_cut);

    TH1D* h_chi2_kmFD_BC_1e_cut = new TH1D("chi2_kmFD_BC_1e_cut", "#chi^{2}_{kmFD} in 1e cut (before cut);#chi^{2}_{kmFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_kmFD_BC_1e_cut);
    TH1D* h_chi2_kmFD_AC_1e_cut = new TH1D("chi2_kmFD_AC_1e_cut", "#chi^{2}_{kmFD} in 1e cut (after cut);#chi^{2}_{kmFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_kmFD_AC_1e_cut);

    TH1D* h_Vz_kmCD_BC_1e_cut = new TH1D("Vz_kmCD_BC_1e_cut", "V_{z}^{kmCD} in 1e cut (before cut);V_{z}^{kmCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_kmCD_BC_1e_cut);
    TH1D* h_Vz_kmCD_AC_1e_cut = new TH1D("Vz_kmCD_AC_1e_cut", "V_{z}^{kmCD} in 1e cut (after cut);V_{z}^{kmCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_kmCD_AC_1e_cut);

    TH1D* h_dVz_kmCD_BC_1e_cut = new TH1D("dVz_kmCD_BC_1e_cut", "dV_{z}^{kmCD} in 1e cut (before cut);dV_{z}^{kmCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_kmCD_BC_1e_cut);
    TH1D* h_dVz_kmCD_AC_1e_cut = new TH1D("dVz_kmCD_AC_1e_cut", "dV_{z}^{kmCD} in 1e cut (after cut);dV_{z}^{kmCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_kmCD_AC_1e_cut);

    TH1D* h_chi2_kmCD_BC_1e_cut = new TH1D("chi2_kmCD_BC_1e_cut", "#chi^{2}_{kmCD} in 1e cut (before cut);#chi^{2}_{kmCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_kmCD_BC_1e_cut);
    TH1D* h_chi2_kmCD_AC_1e_cut = new TH1D("chi2_kmCD_AC_1e_cut", "#chi^{2}_{kmCD} in 1e cut (after cut);#chi^{2}_{kmCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_kmCD_AC_1e_cut);

    TH2D* h_dc_kmFD_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D* h_dc_kmFD_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_kmFD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_kmFD_hit_map_BC_%d", i), Form("kmFD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_kmFD_hit_map_BC_1e_cut[i]);
        h_dc_kmFD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_kmFD_hit_map_AC_%d", i), Form("kmFD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_kmFD_hit_map_AC_1e_cut[i]);
    }

#pragma endregion

#pragma region /* Deuteron pre-selection and PID */

    TH1D* h_Vz_D_FD_BC_1e_cut = new TH1D("Vz_D_FD_BC_1e_cut", "V_{z}^{dFD} in 1e cut (before cut);V_{z}^{dFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_D_FD_BC_1e_cut);
    TH1D* h_Vz_D_FD_AC_1e_cut = new TH1D("Vz_D_FD_AC_1e_cut", "V_{z}^{dFD} in 1e cut (after cut);V_{z}^{dFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_D_FD_AC_1e_cut);

    TH1D* h_dVz_D_FD_BC_1e_cut = new TH1D("dVz_D_FD_BC_1e_cut", "dV_{z}^{dFD} in 1e cut (before cut);dV_{z}^{dFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_D_FD_BC_1e_cut);
    TH1D* h_dVz_D_FD_AC_1e_cut = new TH1D("dVz_D_FD_AC_1e_cut", "dV_{z}^{dFD} in 1e cut (after cut);dV_{z}^{dFD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_D_FD_AC_1e_cut);

    TH1D* h_chi2_D_FD_BC_1e_cut = new TH1D("chi2_D_FD_BC_1e_cut", "#chi^{2}_{dFD} in 1e cut (before cut);#chi^{2}_{dFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_D_FD_BC_1e_cut);
    TH1D* h_chi2_D_FD_AC_1e_cut = new TH1D("chi2_D_FD_AC_1e_cut", "#chi^{2}_{dFD} in 1e cut (after cut);#chi^{2}_{dFD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_D_FD_AC_1e_cut);

    TH1D* h_Vz_D_CD_BC_1e_cut = new TH1D("Vz_D_CD_BC_1e_cut", "V_{z}^{dCD} in 1e cut (before cut);V_{z}^{dCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_D_CD_BC_1e_cut);
    TH1D* h_Vz_D_CD_AC_1e_cut = new TH1D("Vz_D_CD_AC_1e_cut", "V_{z}^{dCD} in 1e cut (after cut);V_{z}^{dCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_Vz_D_CD_AC_1e_cut);

    TH1D* h_dVz_D_CD_BC_1e_cut = new TH1D("dVz_D_CD_BC_1e_cut", "dV_{z}^{dCD} in 1e cut (before cut);dV_{z}^{dCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_D_CD_BC_1e_cut);
    TH1D* h_dVz_D_CD_AC_1e_cut = new TH1D("dVz_D_CD_AC_1e_cut", "dV_{z}^{dCD} in 1e cut (after cut);dV_{z}^{dCD} [cm];Counts", 50, -10, 10);
    HistoList_PID_plots.push_back(h_dVz_D_CD_AC_1e_cut);

    TH1D* h_chi2_D_CD_BC_1e_cut = new TH1D("chi2_D_CD_BC_1e_cut", "#chi^{2}_{dCD} in 1e cut (before cut);#chi^{2}_{dCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_D_CD_BC_1e_cut);
    TH1D* h_chi2_D_CD_AC_1e_cut = new TH1D("chi2_D_CD_AC_1e_cut", "#chi^{2}_{dCD} in 1e cut (after cut);#chi^{2}_{dCD};Counts", 50, -5, 5);
    HistoList_PID_plots.push_back(h_chi2_D_CD_AC_1e_cut);

    TH2D* h_dc_D_FD_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D* h_dc_D_FD_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_D_FD_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_D_FD_hit_map_BC_%d", i), Form("D FD DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_D_FD_hit_map_BC_1e_cut[i]);
        h_dc_D_FD_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_D_FD_hit_map_AC_%d", i), Form("D FD DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_PID_plots.push_back(h_dc_D_FD_hit_map_AC_1e_cut[i]);
    }

#pragma endregion

#pragma region /* Kinematics (RAW) */

    TH1D* h_reco_P_e_1e_cut = new TH1D("reco_P_e_1e_cut", "P^{reco}_{e} in 1e cut;P^{reco}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_raw.push_back(h_reco_P_e_1e_cut);
    TH1D* h_truth_P_e_1e_cut = new TH1D("truth_P_e_1e_cut", "P^{truth}_{e} in 1e cut;P^{truth}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_raw.push_back(h_truth_P_e_1e_cut);
    TH1D* h_reco_theta_e_1e_cut = new TH1D("reco_theta_e_1e_cut", "#theta^{reco}_{e} in 1e cut;#theta^{reco}_{e} [#circ];Counts", 50, 0, 45.);
    HistoList_raw.push_back(h_reco_theta_e_1e_cut);
    TH1D* h_truth_theta_e_1e_cut = new TH1D("truth_theta_e_1e_cut", "#theta^{truth}_{e} in 1e cut;#theta^{truth}_{e} [#circ];Counts", 50, 0, 45.);
    HistoList_raw.push_back(h_truth_theta_e_1e_cut);
    TH1D* h_reco_phi_e_1e_cut = new TH1D("reco_phi_e_1e_cut", "#phi^{reco}_{e} in 1e cut;#phi^{reco}_{e} [#circ];Counts", 50, -180, 180.);
    HistoList_raw.push_back(h_reco_phi_e_1e_cut);
    TH1D* h_truth_phi_e_1e_cut = new TH1D("truth_phi_e_1e_cut", "#phi^{truth}_{e} in 1e cut;#phi^{truth}_{e} [#circ];Counts", 50, -180, 180.);
    HistoList_raw.push_back(h_truth_phi_e_1e_cut);
    TH2D* h_reco_theta_e_VS_reco_phi_e_1e_cut =
        new TH2D("reco_theta_e_VS_reco_phi_e_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{e} in 1e cut;#phi^{reco}_{e} [#circ];#theta^{reco}_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList_raw.push_back(h_reco_theta_e_VS_reco_phi_e_1e_cut);
    TH2D* h_truth_theta_e_VS_truth_phi_e_1e_cut = new TH2D(
        "truth_theta_e_VS_truth_phi_e_1e_cut", "#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut;#phi^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList_raw.push_back(h_truth_theta_e_VS_truth_phi_e_1e_cut);

    TH1D* h_reco_P_n_1e_cut = new TH1D("reco_P_n_1e_cut", "P^{reco}_{n} in 1e cut;P^{reco}_{n} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_raw.push_back(h_reco_P_n_1e_cut);
    TH1D* h_truth_P_n_1e_cut = new TH1D("truth_P_n_1e_cut", "P^{truth}_{n} in 1e cut;P^{truth}_{n} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_raw.push_back(h_truth_P_n_1e_cut);
    TH1D* h_reco_theta_n_1e_cut = new TH1D("reco_theta_n_1e_cut", "#theta^{reco}_{n} in 1e cut;#theta^{reco}_{n} [#circ];Counts", 50, 0, 45.);
    HistoList_raw.push_back(h_reco_theta_n_1e_cut);
    TH1D* h_truth_theta_n_1e_cut = new TH1D("truth_theta_n_1e_cut", "#theta^{truth}_{n} in 1e cut;#theta^{truth}_{n} [#circ];Counts", 50, 0, 45.);
    HistoList_raw.push_back(h_truth_theta_n_1e_cut);
    TH1D* h_reco_phi_n_1e_cut = new TH1D("reco_phi_n_1e_cut", "#phi^{reco}_{n} in 1e cut;#phi^{reco}_{n} [#circ];Counts", 50, -180, 180.);
    HistoList_raw.push_back(h_reco_phi_n_1e_cut);
    TH1D* h_truth_phi_n_1e_cut = new TH1D("truth_phi_n_1e_cut", "#phi^{truth}_{n} in 1e cut;#phi^{truth}_{n} [#circ];Counts", 50, -180, 180.);
    HistoList_raw.push_back(h_truth_phi_n_1e_cut);
    TH2D* h_reco_theta_n_VS_reco_phi_n_1e_cut =
        new TH2D("reco_theta_n_VS_reco_phi_n_1e_cut", "#theta^{reco}_{n} vs. #phi^{reco}_{n} in 1e cut;#phi^{reco}_{n} [#circ];#theta^{reco}_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList_raw.push_back(h_reco_theta_n_VS_reco_phi_n_1e_cut);
    TH2D* h_truth_theta_n_VS_truth_phi_n_1e_cut = new TH2D(
        "truth_theta_n_VS_truth_phi_n_1e_cut", "#theta^{truth}_{n} vs. #phi^{truth}_{n} in 1e cut;#phi^{truth}_{n} [#circ];#theta^{truth}_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList_raw.push_back(h_truth_theta_n_VS_truth_phi_n_1e_cut);

#pragma endregion

    cout << "\033[33m\n\nFinished setting up RAW histograms.\n\033[0m" << std::flush;

#pragma endregion

#pragma region /* clas12reco */

    HistSubjects.push_back("clas12reco");
    HistSubjects2.push_back("#splitline{FD neutron from}{clas12reco}");
    FirstPrint.push_back(true);

    TH1D* h_reco_P_nFD_clas12_1e_cut = new TH1D("reco_P_nFD_clas12_1e_cut", "P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_clas12reco.push_back(h_reco_P_nFD_clas12_1e_cut);
    TH1D* h_truth_P_nFD_clas12_1e_cut = new TH1D("truth_P_nFD_clas12_1e_cut", "P^{truth}_{nFD} in 1e cut (clas12reco);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_clas12reco.push_back(h_truth_P_nFD_clas12_1e_cut);
    TH1D* h_reco_theta_nFD_clas12_1e_cut = new TH1D("reco_theta_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList_clas12reco.push_back(h_reco_theta_nFD_clas12_1e_cut);
    TH1D* h_truth_theta_nFD_clas12_1e_cut = new TH1D("truth_theta_nFD_clas12_1e_cut", "#theta^{truth}_{nFD} in 1e cut (clas12reco);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList_clas12reco.push_back(h_truth_theta_nFD_clas12_1e_cut);
    TH1D* h_reco_phi_nFD_clas12_1e_cut = new TH1D("reco_phi_nFD_clas12_1e_cut", "#phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList_clas12reco.push_back(h_reco_phi_nFD_clas12_1e_cut);
    TH1D* h_truth_phi_nFD_clas12_1e_cut = new TH1D("truth_phi_nFD_clas12_1e_cut", "#phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList_clas12reco.push_back(h_truth_phi_nFD_clas12_1e_cut);
    TH2D* h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut =
        new TH2D("reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut",
                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList_clas12reco.push_back(h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut);
    TH2D* h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut =
        new TH2D("truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList_clas12reco.push_back(h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut);
    TH2D* h_reco_P_e_VS_P_nFD_clas12_1e_cut = new TH2D(
        "reco_P_e_VS_P_nFD_clas12_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList_clas12reco.push_back(h_reco_P_e_VS_P_nFD_clas12_1e_cut);
    TH2D* h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut =
        new TH2D("reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0.,
                 50., 100, 0., Ebeam * 3.);
    HistoList_clas12reco.push_back(h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut);

    cout << "\033[33m\n\nFinished setting up clas12reco histograms.\n\033[0m" << std::flush;

#pragma endregion

#pragma region /* redef */

    HistSubjects.push_back("redef");
    HistSubjects2.push_back("redef");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_redef_1e_cut = new TH1D("reco_P_nFD_redef_1e_cut", "P^{reco}_{nFD} in 1e cut (redef);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_redef.push_back(h_reco_P_nFD_redef_1e_cut);
    TH1D* h_truth_P_nFD_redef_1e_cut = new TH1D("truth_P_nFD_redef_1e_cut", "P^{truth}_{nFD} in 1e cut (redef);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList_redef.push_back(h_truth_P_nFD_redef_1e_cut);
    TH1D* h_reco_theta_nFD_redef_1e_cut = new TH1D("reco_theta_nFD_redef_1e_cut", "#theta^{reco}_{nFD} in 1e cut (redef);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList_redef.push_back(h_reco_theta_nFD_redef_1e_cut);
    TH1D* h_truth_theta_nFD_redef_1e_cut = new TH1D("truth_theta_nFD_redef_1e_cut", "#theta^{truth}_{nFD} in 1e cut (redef);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList_redef.push_back(h_truth_theta_nFD_redef_1e_cut);
    TH1D* h_reco_phi_nFD_redef_1e_cut = new TH1D("reco_phi_nFD_redef_1e_cut", "#phi^{reco}_{nFD} in 1e cut (redef);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList_redef.push_back(h_reco_phi_nFD_redef_1e_cut);
    TH1D* h_truth_phi_nFD_redef_1e_cut = new TH1D("truth_phi_nFD_redef_1e_cut", "#phi^{truth}_{nFD} in 1e cut (redef);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList_redef.push_back(h_truth_phi_nFD_redef_1e_cut);
    TH2D* h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut =
        new TH2D("reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut", "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (redef);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]",
                 100, -180., 180., 100, 0, 50.);
    HistoList_redef.push_back(h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut);
    TH2D* h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut =
        new TH2D("truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (redef);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList_redef.push_back(h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut);
    TH2D* h_reco_P_e_VS_P_nFD_redef_1e_cut =
        new TH2D("reco_P_e_VS_P_nFD_redef_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (redef);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList_redef.push_back(h_reco_P_e_VS_P_nFD_redef_1e_cut);
    TH2D* h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut =
        new TH2D("reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (redef);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0., 50.,
                 100, 0., Ebeam * 3.);
    HistoList_redef.push_back(h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut);

    cout << "\033[33m\n\nFinished setting up redef. histograms.\n\033[0m" << std::flush;

#pragma endregion

#pragma region /* ECALveto */

    HistSubjects.push_back("ECALveto");
    HistSubjects2.push_back("#splitline{ECALveto}{and P_{nFD} thresholds}");
    FirstPrint.push_back(true);

#pragma region /* Base histograms (ECALveto) */

    TH1D* h_reco_P_e_ECALveto_1e_cut = new TH1D("reco_P_e_ECALveto_1e_cut", "P^{reco}_{e} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_e_ECALveto_1e_cut);
    TH1D* h_truth_P_e_ECALveto_1e_cut = new TH1D("truth_P_e_ECALveto_1e_cut", "P^{truth}_{e} in 1e cut (ECALveto);P^{truth}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_e_ECALveto_1e_cut);
    TH1D* h_reco_theta_e_ECALveto_1e_cut = new TH1D("reco_theta_e_ECALveto_1e_cut", "#theta^{reco}_{e} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_e_ECALveto_1e_cut);
    TH1D* h_truth_theta_e_ECALveto_1e_cut = new TH1D("truth_theta_e_ECALveto_1e_cut", "#theta^{truth}_{e} in 1e cut (ECALveto);#theta^{truth}_{e} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_e_ECALveto_1e_cut);
    TH1D* h_reco_phi_e_ECALveto_1e_cut = new TH1D("reco_phi_e_ECALveto_1e_cut", "#phi^{reco}_{e} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_e_ECALveto_1e_cut);
    TH1D* h_truth_phi_e_ECALveto_1e_cut = new TH1D("truth_phi_e_ECALveto_1e_cut", "#phi^{truth}_{e} in 1e cut (ECALveto);#phi^{truth}_{e} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_e_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_ECALveto_VS_reco_phi_e_ECALveto_1e_cut =
        new TH2D("reco_theta_e_ECALveto_VS_reco_phi_e_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{e} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#theta^{reco}_{e} [#circ]", 100,
                 -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_e_ECALveto_VS_reco_phi_e_ECALveto_1e_cut);
    TH2D* h_truth_theta_e_ECALveto_VS_truth_phi_e_ECALveto_1e_cut =
        new TH2D("truth_theta_e_ECALveto_VS_truth_phi_e_ECALveto_1e_cut", "#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut (ECALveto);#phi^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]",
                 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_e_ECALveto_VS_truth_phi_e_ECALveto_1e_cut);

    TH1D* h_reco_P_nFD_ECALveto_1e_cut = new TH1D("reco_P_nFD_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_ECALveto_1e_cut);
    TH1D* h_reco_P_nFD_PCAL_only_ECALveto_1e_cut =
        new TH1D("reco_P_nFD_PCAL_only_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut, PCAL only (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_PCAL_only_ECALveto_1e_cut);
    TH1D* h_reco_P_nFD_ECIN_only_ECALveto_1e_cut =
        new TH1D("reco_P_nFD_ECIN_only_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut, ECIN only (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_ECIN_only_ECALveto_1e_cut);
    TH1D* h_reco_P_nFD_ECOUT_only_ECALveto_1e_cut =
        new TH1D("reco_P_nFD_ECOUT_only_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut, ECOUT only (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_ECOUT_only_ECALveto_1e_cut);
    TH1D* h_truth_P_nFD_ECALveto_1e_cut = new TH1D("truth_P_nFD_ECALveto_1e_cut", "P^{truth}_{nFD} in 1e cut (ECALveto);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_nFD_ECALveto_1e_cut);
    TH1D* h_reco_theta_nFD_ECALveto_1e_cut = new TH1D("reco_theta_nFD_ECALveto_1e_cut", "#theta^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_1e_cut);
    TH1D* h_truth_theta_nFD_ECALveto_1e_cut = new TH1D("truth_theta_nFD_ECALveto_1e_cut", "#theta^{truth}_{nFD} in 1e cut (ECALveto);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_ECALveto_1e_cut);
    TH1D* h_reco_phi_nFD_ECALveto_1e_cut = new TH1D("reco_phi_nFD_ECALveto_1e_cut", "#phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_ECALveto_1e_cut);
    TH1D* h_truth_phi_nFD_ECALveto_1e_cut = new TH1D("truth_phi_nFD_ECALveto_1e_cut", "#phi^{truth}_{nFD} in 1e cut (ECALveto);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut);
    TH2D* h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut =
        new TH2D("truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (ECALveto);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut);

    TH1D* h_reco_P_LnFD_ECALveto_1e_cut = new TH1D("reco_P_LnFD_ECALveto_1e_cut", "P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_LnFD_ECALveto_1e_cut);
    TH1D* h_reco_theta_LnFD_ECALveto_1e_cut = new TH1D("reco_theta_LnFD_ECALveto_1e_cut", "#theta^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{LnFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_LnFD_ECALveto_1e_cut);
    TH1D* h_reco_phi_LnFD_ECALveto_1e_cut = new TH1D("reco_phi_LnFD_ECALveto_1e_cut", "#phi^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{LnFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut =
        new TH2D("reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut",
                 "#theta^{reco}_{LnFD} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{LnFD} [#circ];#theta^{reco}_{LnFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut);

    TH1D* h_reco_P_phFD_BPID_ECALveto_1e_cut =
        new TH1D("reco_P_phFD_BPID_ECALveto_1e_cut", "P^{reco}_{#gammaFD} in 1e cut BPID (ECALveto);P^{reco}_{#gammaFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_phFD_BPID_ECALveto_1e_cut);
    TH1D* h_reco_P_phFD_APID_ECALveto_1e_cut =
        new TH1D("reco_P_phFD_APID_ECALveto_1e_cut", "P^{reco}_{#gammaFD} in 1e cut APID (ECALveto);P^{reco}_{#gammaFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_phFD_APID_ECALveto_1e_cut);

    TH1D* h_Edep_ECAL_nFD_ECALveto_1e_cut = new TH1D("Edep_ECAL_nFD_ECALveto_1e_cut", "E^{ECAL}_{dep} in 1e cut (ECALveto);E^{ECAL}_{dep} [GeV];Counts", 50, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_nFD_ECALveto_1e_cut);
    TH1D* h_beta_nFD_ECALveto_1e_cut = new TH1D("beta_nFD_ECALveto_1e_cut", "#beta_{nFD} in 1e cut (ECALveto);#beta_{nFD};Counts", 50, 0.3, 1.1);
    HistoList.push_back(h_beta_nFD_ECALveto_1e_cut);
    TH2D* h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut =
        new TH2D("Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. #beta_{nFD} in 1e cut (ECALveto);#beta_{nFD};E^{ECAL}_{dep} [GeV]", 100, 0.3, 1.1, 100, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut);

    TH1D* h_Edep_ECAL_LnFD_ECALveto_1e_cut = new TH1D("Edep_ECAL_LnFD_ECALveto_1e_cut", "E^{ECAL}_{dep} in 1e cut (ECALveto);E^{ECAL}_{dep} [GeV];Counts", 50, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_LnFD_ECALveto_1e_cut);
    TH1D* h_beta_LnFD_ECALveto_1e_cut = new TH1D("beta_LnFD_ECALveto_1e_cut", "#beta_{LnFD} in 1e cut (ECALveto);#beta_{LnFD};Counts", 50, 0.3, 1.1);
    HistoList.push_back(h_beta_LnFD_ECALveto_1e_cut);
    TH2D* h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut =
        new TH2D("Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. #beta_{LnFD} in 1e cut (ECALveto);#beta_{LnFD};E^{ECAL}_{dep} [GeV]", 100, 0.3, 1.1, 100, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut);

    TH2D* h_nFD_PCAL_hits_VS_nFD_ECIN_hits_ECALveto_1e_cut =
        new TH2D("nFD_PCAL_hits_VS_nFD_ECIN_hits_ECALveto_1e_cut", "#(nFD hits in PCAL) vs. #(nFD hits in ECIN) in 1e cut (ECALveto);#(nFD hits in PCAL);#(nFD hits in ECIN)", 2, -0.5, 1.5,
                 2, -0.5, 1.5);
    HistoList.push_back(h_nFD_PCAL_hits_VS_nFD_ECIN_hits_ECALveto_1e_cut);
    TH2D* h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut =
        new TH2D("nFD_PCAL_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut", "#(nFD hits in PCAL) vs. #(nFD hits in ECOUT) in 1e cut (ECALveto);#(nFD hits in PCAL);#(nFD hits in ECOUT)", 2, -0.5,
                 1.5, 2, -0.5, 1.5);
    HistoList.push_back(h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut);
    TH2D* h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut =
        new TH2D("nFD_ECIN_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut", "#(nFD hits in ECIN) vs. #(nFD hits in ECOUT) in 1e cut (ECALveto);#(nFD hits in ECIN);#(nFD hits in ECOUT)", 2, -0.5,
                 1.5, 2, -0.5, 1.5);
    HistoList.push_back(h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut);

    TH2D* h_reco_P_e_VS_P_nFD_ECALveto_1e_cut = new TH2D(
        "reco_P_e_VS_P_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_P_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0, 45., 100, 0.,
                 Ebeam * 3.);
    HistoList.push_back(h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_P_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, -180., 180., 100, 0.,
                 Ebeam * 3.);
    HistoList.push_back(h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut);
    TH2D* h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_theta_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#theta^{reco}_{nFD} [#circ]", 100, 0., P_upperLim,
                 100, 0, 45.);
    HistoList.push_back(h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_theta_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, 0,
                 45., 100, 0, 45.);
    HistoList.push_back(h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_theta_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180.,
                 180., 100, 0, 45.);
    HistoList.push_back(h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut);
    TH2D* h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_phi_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#phi^{reco}_{nFD} [#circ]", 100, 0., P_upperLim, 100,
                 -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_phi_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, 0, 45.,
                 100, -180., 180.);
    HistoList.push_back(h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, -180., 180.,
                 100, -180., 180.);
    HistoList.push_back(h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100,
                 0., 45., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut);
    TH1D* h_reco_nFD_multi_AllN_ECALveto_1e_cut = new TH1D("reco_nFD_multi_AllN_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (all nFDs, ECALveto);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_AllN_ECALveto_1e_cut);
    TH1D* h_reco_nFD_multi_OnlyGood_ECALveto_1e_cut =
        new TH1D("reco_nFD_multi_OnlyGood_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (Only good nFD, ECALveto);Good nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_OnlyGood_ECALveto_1e_cut);
    TH1D* h_reco_nFD_multi_OnlyBad_ECALveto_1e_cut =
        new TH1D("reco_nFD_multi_OnlyBad_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (Only bad nFD, ECALveto);Bad nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_OnlyBad_ECALveto_1e_cut);
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut", "reco nFD multiplicity vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam * 3.,
                 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut);

    TH2D* h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut = new TH2D(
        "reco_P_e_VS_P_LnFD_ECALveto_1e_cut", "P^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];P^{reco}_{LnFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_P_LnFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];P^{reco}_{LnFD} [GeV/c]", 100, 0, 45., 100,
                 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_P_LnFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];P^{reco}_{LnFD} [GeV/c]", 100, -180., 180., 100,
                 0., Ebeam * 3.);
    HistoList.push_back(h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_theta_LnFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#theta^{reco}_{LnFD} [#circ]", 100, 0.,
                 P_upperLim, 100, 0, 45.);
    HistoList.push_back(h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#theta^{reco}_{LnFD} [#circ]", 100,
                 0, 45., 100, 0, 45.);
    HistoList.push_back(h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#theta^{reco}_{LnFD} [#circ]", 100, -180.,
                 180., 100, 0, 45.);
    HistoList.push_back(h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_phi_LnFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#phi^{reco}_{LnFD} [#circ]", 100, 0., P_upperLim, 100,
                 -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut =
        new TH2D("reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#phi^{reco}_{LnFD} [#circ]", 100, 0,
                 45., 100, -180., 180.);
    HistoList.push_back(h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut =
        new TH2D("reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#phi^{reco}_{LnFD} [#circ]", 100, -180., 180.,
                 100, -180., 180.);
    HistoList.push_back(h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut);
    TH2D* h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut =
        new TH2D("reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut", "#theta^{reco}_{LnFD} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{LnFD} [#circ];P^{reco}_{LnFD} [GeV/c]",
                 100, 0., 45., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut);

#pragma endregion

    TH1D* h_reco_dTheta_nFD_e_ECALveto_1e_cut =
        new TH1D("reco_dTheta_nFD_e_ECALveto_1e_cut", "#delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts",
                 50, -25., 10.);
    HistoList.push_back(h_reco_dTheta_nFD_e_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_ECALveto_1e_cut =
        new TH1D("reco_dPhi_nFD_e_ECALveto_1e_cut", "#delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50,
                 -180., 180.);
    HistoList.push_back(h_reco_dPhi_nFD_e_ECALveto_1e_cut);
    TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
        new TH2D("reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
                 "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                 "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_dTheta_nFD_e_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_dTheta_nFD_e_ECALveto_1e_cut",
                 "P^{reco}_{nFD} vs. #delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];P^{reco}_{nFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_dTheta_nFD_e_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
                 "P^{reco}_{nFD} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];P^{reco}_{nFD} [GeV/c]", 100,
                 -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
                 "P^{reco}_{e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0.,
                 Ebeam * 1.1, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut =
        new TH2D("nFD_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut",
                 "nFD status vs. #delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];nFD status",
                 100, -25., 10., 100, 1990, 3000.);
    HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut);

    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut =
        new TH2D("nFD_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut",
                 "nFD status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];nFD status",
                 100, -10., 5., 100, 1990, 2250.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut",
                 "e status vs. #delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -25., 10., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut",
                 "e status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -10., 5., 100, -2250., -0.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut);

    TH2D* h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_ECALveto_1e_cut =
        new TH2D("dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_ECALveto_1e_cut",
                 "'Neutron Hits' vs. Electron Hits - After Veto in 1e cut (ECALveto);#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ];#Delta#theta_{n,e} = "
                 "#theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                 100, -180, 180, 100, -25, 10);
    //  100, -180, 180, 100, -50, 50);
    HistoList.push_back(h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_ECALveto_1e_cut);

    TH1D* h_dr_n_ECALveto_1e_cut =
        new TH1D("dr_n_ECALveto_1e_cut", "|#Delta#font[62]{r}_{n}| in 1e cut (ECALveto);|#Delta#font[62]{r}_{n}| = |#font[62]{r}^{hit}_{n} - #font[62]{R}_{e}| [cm];Counts", 50, 0, 400);
    HistoList.push_back(h_dr_n_ECALveto_1e_cut);
    TH2D* h_dr_n_VS_dTheta_n_e_ECALveto_1e_cut = new TH2D(
        "dr_n_VS_dTheta_n_e_ECALveto_1e_cut",
        "|#Delta#font[62]{r}_{n}| vs. #delta#theta^{reco}_{nFD,e} - in 1e cut (ECALveto);|#Delta#font[62]{r}_{n}| = |#font[62]{r}^{hit}_{n} - #font[62]{R}_{e}| [cm];#Delta#theta_{nFD,e} = "
        "#theta_{V}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
        100, 0, 400, 100, -25, 10);
    HistoList.push_back(h_dr_n_VS_dTheta_n_e_ECALveto_1e_cut);
    TH2D* h_dr_n_VS_dPhi_n_e_ECALveto_1e_cut = new TH2D(
        "dr_n_VS_dPhi_n_e_ECALveto_1e_cut",
        "|#Delta#font[62]{r}_{n}| vs. #delta#phi^{reco}_{nFD,e} - in 1e cut (ECALveto);|#Delta#font[62]{r}_{n}| = |#font[62]{r}^{hit}_{n} - #font[62]{R}_{e}| [cm];#Delta#phi_{nFD,e} = "
        "#phi_{V}^{ECAL} - #phi_{e}^{ECAL} [#circ]",
        100, 0, 400, 100, -25, 10);
    HistoList.push_back(h_dr_n_VS_dPhi_n_e_ECALveto_1e_cut);

    std::vector<TObject*> Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_ECALveto_1e_cut;
    hsPlots h_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_ECALveto_1e_cut =
        hsPlots(dr_n_SliceLimits, hsPlots::TH2D_TYPE, HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n, &Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_ECALveto_1e_cut,
                "dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_ECALveto_1e_cut",
                "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in slices of |#Deltar_{n}| in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                100, -180., 180., 100, -25., 10., "|#Deltar_{n}| [cm]");

    TH2D* h_dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_ECALveto_1e_cut =
        new TH2D("dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_ECALveto_1e_cut",
                 "#Delta#theta_{nFD,V} vs. #Delta#phi_{nFD,V} - After Veto in 1e cut (ECALveto);#Delta#phi_{n,V} = #phi_{n}^{ECAL} - #phi_{V} [#circ];#Delta#theta_{nFD,V} = "
                 "#theta_{n}^{ECAL} - #theta_{V} [#circ]",
                 100, -180, 180, 100, -25, 10);
    //  100, -180, 180, 100, -50, 50);
    HistoList.push_back(h_dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_ECALveto_1e_cut);

#pragma region /* Same sector plots - nFD */

    TH1D* h_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH1D("reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "#delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
    HistoList.push_back(h_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);
    TH1D* h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut = new TH1D(
        "reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut",
        "#delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} - zoomin (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts", 50, -10., 5.);
    HistoList.push_back(h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH1D("reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "#delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut = new TH1D(
        "reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut",
        "#delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} - zoomin (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -50., 10.);
    HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_ECALveto_1e_cut =
        new TH1D("reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_ECALveto_1e_cut",
                 ("#delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} and " + bt::ToStringWithPrecision(dTheta_cuts.GetLowerCut()) + " #leq #delta#theta^{reco}_{nFD,e} #leq " +
                  bt::ToStringWithPrecision(dTheta_cuts.GetUpperCut()) + " (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts")
                     .c_str(),
                 50, -180., 180.);
    HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin_ECALveto_1e_cut =
        new TH1D("reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin_ECALveto_1e_cut",
                 ("#delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} and " + bt::ToStringWithPrecision(dTheta_cuts.GetLowerCut()) + " #leq #delta#theta^{reco}_{nFD,e} #leq " +
                  bt::ToStringWithPrecision(dTheta_cuts.GetUpperCut()) + " - zoomin (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts")
                     .c_str(),
                 50, -50., 10.);
    HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin_ECALveto_1e_cut);
    TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| "
                 "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);
    TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut =
        new TH2D("reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut",
                 "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} - zoomin (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| "
                 "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                 100, -50., 10., 100, -10., 5.);
    HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut);

    TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "P^{reco}_{e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| [#circ]",
                 100, 0., Ebeam * 1.1, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut",
                 "P^{reco}_{e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} - zoomin (ECALveto);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| [#circ]",
                 100, 0., Ebeam * 1.1, 100, -50., 10);
    HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut);

    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "nFD status vs. #delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];nFD status",
                 100, -25., 10., 100, 1990, 3000.);
    HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "nFD status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - "
                 "#theta^{reco}_{e} "
                 "[#circ];nFD status",
                 100, -10., 5., 100, 1990, 2250.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "e status vs. #delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -25., 10., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "e status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -10., 5., 100, -2250., -2100.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH1D* h_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH1D("reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut", "#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#theta^{reco}_{nFD,e} [#circ];Counts", 50, 0., 50.);
    HistoList.push_back(h_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD,e} vs. #delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - "
                 "#theta^{reco}_{e} "
                 "[#circ];#theta^{reco}_{nFD,e} [#circ]",
                 100, -25., 10., 100, 0., 50.);
    HistoList.push_back(h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                 "[#circ];#theta^{reco}_{nFD,e} [#circ]",
                 100, -180., 180., 100, 0., 50.);
    HistoList.push_back(h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| [#circ];#theta^{reco}_{nFD,e} [#circ]",
                 100, -50., 10., 100, 0., 25.);
    HistoList.push_back(h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut);

    TH2D* h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD,e} vs. P^{reco}_{nFD} in 1e cut for S_{nFD} = S_{e} (ECALveto);P^{reco}_{nFD} [GeV/c];#theta^{reco}_{nFD,e} [#circ]", 100, 0., Ebeam * 3., 100, 0., 50.);
    HistoList.push_back(h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "nFD status vs. #theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#theta^{reco}_{nFD,e} [#circ];nFD status", 100, 0., 50., 100, 1990, 3000.);
    HistoList.push_back(h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "nFD status vs. #theta^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (ECALveto);#theta^{reco}_{nFD,e} [#circ];nFD status", 100, 0., 20., 100, 1990, 2250.);
    HistoList.push_back(h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "e status vs. #theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (ECALveto);#theta^{reco}_{nFD,e} [#circ];e status", 100, 0., 50., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "e status vs. #theta^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (ECALveto);#theta^{reco}_{nFD,e} [#circ];e status", 100, 0., 20., 100, -2250., -0.);
    HistoList.push_back(h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut);

#pragma endregion

#pragma region /* New plots - LnFD */

    TH1D* h_reco_dTheta_LnFD_e_ECALveto_1e_cut =
        new TH1D("reco_dTheta_LnFD_e_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_LnFD_e_ECALveto_1e_cut =
        new TH1D("reco_dPhi_LnFD_e_ECALveto_1e_cut", "#Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50,
                 -180., 180.);
    HistoList.push_back(h_reco_dPhi_LnFD_e_ECALveto_1e_cut);
    TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut =
        new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut);
    TH2D* h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut =
        new TH2D("reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut",
                 "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut);
    TH2D* h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut =
        new TH2D("reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut",
                 "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                 100, -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut);

    TH2D* h_reco_P_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut",
                 "P^{reco}_{e} vs. #delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]", 100, 0.,
                 Ebeam * 1.1, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -25., 10., 100, 1990, 3000.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -10., 5., 100, 1990, 2250.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -25., 10., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -10., 5., 100, -2250., -0.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut);

    TH1D* h_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut =
        new TH1D("reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut =
        new TH1D("reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut",
                 "#Delta#phi^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut);
    TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut =
        new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut);
    TH2D* h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut =
        new TH2D("reco_P_LnFD_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut",
                 "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut);
    TH2D* h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut =
        new TH2D("reco_P_LnFD_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut",
                 "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                 100, -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -25., 10., 100, 1990, 3000.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -10., 5., 100, 1990, 2250.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -25., 10., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -10., 5., 100, -2250., -0.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut);

    TH1D* h_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut =
        new TH1D("reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut =
        new TH1D("reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut",
                 "#Delta#phi^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut);
    TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut =
        new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut);
    TH2D* h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut =
        new TH2D("reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut",
                 "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut);
    TH2D* h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut =
        new TH2D("reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut",
                 "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                 100, -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -25., 10., 100, 1990, 3000.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -10., 5., 100, 1990, 2250.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -25., 10., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -10., 5., 100, -2250., -0.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut);

    TH1D* h_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut =
        new TH1D("reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut =
        new TH1D("reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut",
                 "#Delta#phi^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut);
    TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut =
        new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut);
    TH2D* h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut =
        new TH2D("reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut",
                 "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut);
    TH2D* h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut =
        new TH2D("reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut",
                 "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                 100, -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -25., 10., 100, 1990, 3000.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -10., 5., 100, 1990, 2250.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -25., 10., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -10., 5., 100, -2250., -0.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut);

    TH2D* h_P_LnFD_Res_VS_truth_P_LnFD_ECALveto_1e_cut = new TH2D("P_LnFD_Res_VS_truth_P_LnFD_ECALveto_1e_cut",
                                                                  "R_{LnFD} vs. P^{truth}_{LnFD} in 1e cut (ECALveto);P^{truth}_{LnFD} [GeV/c];"
                                                                  "Resolution = (P^{truth}_{LnFD} - P^{reco}_{LnFD})/P^{truth}_{LnFD}",
                                                                  100, 0, Ebeam * 1.1, 100, -1.1, 1.1);
    HistoList.push_back(h_P_LnFD_Res_VS_truth_P_LnFD_ECALveto_1e_cut);
    TH2D* h_P_LnFD_Res_VS_reco_P_LnFD_ECALveto_1e_cut = new TH2D("P_LnFD_Res_VS_reco_P_LnFD_ECALveto_1e_cut",
                                                                 "R_{LnFD} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];"
                                                                 "Resolution = (P^{truth}_{LnFD} - P^{reco}_{LnFD})/P^{truth}_{LnFD}",
                                                                 100, 0, Ebeam * 1.1, 100, -1.1, 1.1);
    HistoList.push_back(h_P_LnFD_Res_VS_reco_P_LnFD_ECALveto_1e_cut);

    std::vector<TObject*> Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_ECALveto_1e_cut;
    hsPlots h_P_nFD_Res_BysliceOf_truth_P_nFD_ECALveto_1e_cut =
        hsPlots(nFD_Res_SliceLimits, hsPlots::TH1D_TYPE, HistoList_nRes_BysliceOf_truth_P_nFD, &Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_ECALveto_1e_cut,
                "P_nFD_Res_BysliceOf_truth_P_nFD_ECALveto_1e_cut",
                "P_{nFD} resolution in slices of P^{truth}_{nFD} in 1e cut (ECALveto);Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD};Counts", 50, -1.1, 1.1, 50, -1.1, 1.1,
                "P^{truth}_{nFD} [GeV/c]");
    std::vector<TObject*> Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_ECALveto_1e_cut;
    hsPlots h_P_nFD_Res_BysliceOf_reco_P_nFD_ECALveto_1e_cut =
        hsPlots(nFD_Res_SliceLimits, hsPlots::TH1D_TYPE, HistoList_nRes_BysliceOf_reco_P_nFD, &Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_ECALveto_1e_cut,
                "P_nFD_Res_BysliceOf_reco_P_nFD_ECALveto_1e_cut",
                "P_{nFD} resolution in slices of P^{reco}_{nFD} in 1e cut (ECALveto);Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD};Counts", 50, -1.1, 1.1, 50, -1.1, 1.1,
                "P^{reco}_{nFD} [GeV/c]");

#pragma region /* Same sector plots - LnFD */

    TH1D* h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut = new TH1D(
        "reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut",
        "#Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);
    TH1D* h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut =
        new TH1D("reco_dTheta_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} - zoomin (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];Counts",
                 50, -10., 5.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH1D("reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "#Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);
    TH1D* h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut = new TH1D(
        "reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut",
        "#Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} - zoomin (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -50., 10.);
    HistoList.push_back(h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut);
    TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - "
                 "#phi^{reco}_{e}| "
                 "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);
    TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut =
        new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut",
                 "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} - zoomin (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - "
                 "#phi^{reco}_{e}| "
                 "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                 100, -50., 10., 100, -10., 5.);
    HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut);

    TH2D* h_reco_P_e_VS_reco_dPhi_LnFD_e_Same_e_nFD_Sector_ECALveto_1e_cut =
        new TH2D("reco_P_e_VS_reco_dPhi_LnFD_e_Same_e_nFD_Sector_ECALveto_1e_cut",
                 "P^{reco}_{e} vs. #delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]", 100, 0.,
                 Ebeam * 1.1, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_reco_dPhi_LnFD_e_Same_e_nFD_Sector_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -25., 10., 100, 1990, 3000.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut for S_{LnFD} = S_{e} (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - "
                 "#theta^{reco}_{e} "
                 "[#circ];LnFD status",
                 100, -10., 5., 100, 1990, 2250.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -25., 10., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut for S_{LnFD} = S_{e} (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - "
                 "#theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -10., 5., 100, -2250., -0.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH1D* h_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut = new TH1D(
        "reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut", "#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#theta^{reco}_{LnFD,e} [#circ];Counts", 50, 0., 50.);
    HistoList.push_back(h_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_reco_theta_LnFD_e_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("reco_theta_LnFD_e_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "#theta^{reco}_{LnFD,e} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - "
                 "#theta^{reco}_{e} "
                 "[#circ];#theta^{reco}_{LnFD,e} [#circ]",
                 100, -25., 10., 100, 0., 50.);
    HistoList.push_back(h_reco_theta_LnFD_e_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_reco_theta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("reco_theta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ];#theta^{reco}_{LnFD,e} [#circ]",
                 100, -180., 180., 100, 0., 50.);
    HistoList.push_back(h_reco_theta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_reco_theta_LnFD_e_VS_reco_P_LnFD_Same_e_LnFD_Sector_ECALveto_1e_cut = new TH2D(
        "reco_theta_LnFD_e_VS_reco_P_LnFD_Same_e_LnFD_Sector_ECALveto_1e_cut",
        "#theta^{reco}_{LnFD,e} vs. P^{reco}_{LnFD} in 1e cut for S_{LnFD} = S_{e} (ECALveto);P^{reco}_{LnFD} [GeV/c];#theta^{reco}_{LnFD,e} [#circ]", 100, 0., Ebeam * 3., 100, 0., 50.);
    HistoList.push_back(h_reco_theta_LnFD_e_VS_reco_P_LnFD_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "LnFD status vs. #theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#theta^{reco}_{LnFD,e} [#circ];LnFD status", 100, 0., 50., 100, 1990, 3000.);
    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_LnFD_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("LnFD_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "LnFD status vs. #theta^{reco}_{LnFD,e} - zoomin - in 1e cut for S_{LnFD} = S_{e} (ECALveto);#theta^{reco}_{LnFD,e} [#circ];LnFD status", 100, 0., 20., 100, 1990, 2250.);
    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "e status vs. #theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (ECALveto);#theta^{reco}_{LnFD,e} [#circ];e status", 100, 0., 50., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut);

    TH2D* h_e_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut =
        new TH2D("e_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut",
                 "e status vs. #theta^{reco}_{LnFD,e} - zoomin - in 1e cut for S_{LnFD} = S_{e} (ECALveto);#theta^{reco}_{LnFD,e} [#circ];e status", 100, -10., 5., 100, -2250., -0.);
    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut);

#pragma endregion

#pragma endregion

    TH1D* h_v_dist_nFD_ECALveto_1e_cut = new TH1D("v_dist_nFD_ECALveto_1e_cut", "v_dist in 1e cut (ECALveto);v_dist [cm];Counts", 50, 0., 1000.);
    HistoList.push_back(h_v_dist_nFD_ECALveto_1e_cut);
    TH2D* h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut =
        new TH2D("v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut", "v_dist vs. P^{reco}_{nFD} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{nFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut);
    TH2D* h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut =
        new TH2D("v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
    HistoList.push_back(h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut);
    TH2D* h_v_dist_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut = new TH2D(
        "v_dist_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
        "v_dist vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);v_dist [cm];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0., 1000., 100, -180., 180.);
    HistoList.push_back(h_v_dist_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    TH1D* h_v_dist_LnFD_ECALveto_1e_cut = new TH1D("v_dist_LnFD_ECALveto_1e_cut", "v_dist in 1e cut (ECALveto);v_dist [cm];Counts", 50, 0., 1000.);
    HistoList.push_back(h_v_dist_LnFD_ECALveto_1e_cut);
    TH2D* h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut =
        new TH2D("v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut", "v_dist vs. P^{reco}_{LnFD} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{LnFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut);
    TH2D* h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut =
        new TH2D("v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
    HistoList.push_back(h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut);
    TH2D* h_v_dist_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut =
        new TH2D("v_dist_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut",
                 "v_dist vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);v_dist [cm];#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]", 100, 0., 1000., 100,
                 -180., 180.);
    HistoList.push_back(h_v_dist_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut);

    TH2D* h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut = new TH2D(
        "Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];E^{ECAL}_{dep} [GeV]", 100, 0., Ebeam * 3., 100, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut);
    TH2D* h_Edep_ECAL_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
        new TH2D("Edep_ECAL_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
                 "E^{ECAL}_{dep} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];E^{ECAL}_{dep} [GeV]", 100,
                 -180., 180., 100, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    TH2D* h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut =
        new TH2D("beta_n_VS_reco_P_nFD_ECALveto_1e_cut", "#beta_{nFD} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];#beta_{nFD}", 100, 0., Ebeam * 3., 100, 0.3, 1.1);
    HistoList.push_back(h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut);
    TH2D* h_beta_n_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
        new TH2D("beta_n_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
                 "#beta_{nFD} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];#beta_{nFD}", 100, -180., 180.,
                 100, 0.3, 1.1);
    HistoList.push_back(h_beta_n_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    TH2D* h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut =
        new TH2D("Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];E^{ECAL}_{dep} [GeV]", 100, 0., Ebeam * 3., 100,
                 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut);
    TH2D* h_Edep_ECAL_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut =
        new TH2D("Edep_ECAL_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut",
                 "E^{ECAL}_{dep} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                 "[#circ]];E^{ECAL}_{dep} [GeV]",
                 100, -180., 180., 100, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut);

    TH2D* h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut =
        new TH2D("beta_n_VS_reco_P_LnFD_ECALveto_1e_cut", "#beta_{LnFD} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];#beta_{LnFD}", 100, 0., Ebeam * 3., 100, 0.3, 1.1);
    HistoList.push_back(h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut);
    TH2D* h_beta_n_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut =
        new TH2D("beta_n_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut",
                 "#beta_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]];#beta_{LnFD}", 100, -180.,
                 180., 100, 0.3, 1.1);
    HistoList.push_back(h_beta_n_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut);

    cout << "\033[33m\n\nFinished setting up ECALveto histograms.\n\033[0m" << std::flush;

#pragma endregion

#pragma region /* matched */

    HistSubjects.push_back("matched");
    HistSubjects2.push_back("matched");
    FirstPrint.push_back(true);

#pragma region /* Base histograms (matched) */

    TH1D* h_reco_P_nFD_matched_1e_cut = new TH1D("reco_P_nFD_matched_1e_cut", "P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_matched_1e_cut);
    TH1D* h_reco_P_nFD_PCAL_only_matched_1e_cut =
        new TH1D("reco_P_nFD_PCAL_only_matched_1e_cut", "P^{reco}_{nFD} in 1e cut, PCAL only (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_PCAL_only_matched_1e_cut);
    TH1D* h_reco_P_nFD_ECIN_only_matched_1e_cut =
        new TH1D("reco_P_nFD_ECIN_only_matched_1e_cut", "P^{reco}_{nFD} in 1e cut, ECIN only (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_ECIN_only_matched_1e_cut);
    TH1D* h_reco_P_nFD_ECOUT_only_matched_1e_cut =
        new TH1D("reco_P_nFD_ECOUT_only_matched_1e_cut", "P^{reco}_{nFD} in 1e cut, ECOUT only (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_ECOUT_only_matched_1e_cut);
    TH1D* h_truth_P_nFD_matched_1e_cut = new TH1D("truth_P_nFD_matched_1e_cut", "P^{truth}_{nFD} in 1e cut (matched);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_nFD_matched_1e_cut);
    TH1D* h_truth_P_nFD_matched_to_reco_LnFD_matched_1e_cut =
        new TH1D("truth_P_nFD_matched_to_reco_LnFD_matched_1e_cut", "P^{truth}_{nFD} in 1e cut matched to reco LnFD (matched);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_nFD_matched_to_reco_LnFD_matched_1e_cut);
    TH1D* h_reco_theta_nFD_matched_1e_cut = new TH1D("reco_theta_nFD_matched_1e_cut", "#theta^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_matched_1e_cut);
    TH1D* h_truth_theta_nFD_matched_1e_cut = new TH1D("truth_theta_nFD_matched_1e_cut", "#theta^{truth}_{nFD} in 1e cut (matched);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_matched_1e_cut);
    TH1D* h_truth_theta_nFD_matched_to_reco_LnFD_matched_1e_cut =
        new TH1D("truth_theta_nFD_matched_to_reco_LnFD_matched_1e_cut", "#theta^{truth}_{nFD} in 1e cut matched to reco LnFD (matched);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_matched_to_reco_LnFD_matched_1e_cut);
    TH1D* h_reco_phi_nFD_matched_1e_cut = new TH1D("reco_phi_nFD_matched_1e_cut", "#phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_matched_1e_cut);
    TH1D* h_truth_phi_nFD_matched_1e_cut = new TH1D("truth_phi_nFD_matched_1e_cut", "#phi^{truth}_{nFD} in 1e cut (matched);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_matched_1e_cut);
    TH1D* h_truth_phi_nFD_matched_to_reco_LnFD_matched_1e_cut =
        new TH1D("truth_phi_nFD_matched_to_reco_LnFD_matched_1e_cut", "#phi^{truth}_{nFD} in 1e cut matched to reco LnFD (matched);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_matched_to_reco_LnFD_matched_1e_cut);
    TH2D* h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut =
        new TH2D("reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut",
                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut);
    TH2D* h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut =
        new TH2D("truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (matched);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut);
    TH2D* h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_to_reco_LnFD_matched_1e_cut = new TH2D(
        "truth_theta_nFD_matched_VS_truth_phi_nFD_matched_to_reco_LnFD_matched_1e_cut",
        "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut matched to reco LnFD (matched);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_to_reco_LnFD_matched_1e_cut);

    /*
        TH1D* h_reco_P_LnFD_matched_1e_cut = new TH1D("reco_P_LnFD_matched_1e_cut", "P^{reco}_{LnFD} in 1e cut (matched);P^{reco}_{LnFD} [GeV/c];Counts", 50, 0, P_upperLim);
        HistoList.push_back(h_reco_P_LnFD_matched_1e_cut);
        TH1D* h_reco_theta_LnFD_matched_1e_cut = new TH1D("reco_theta_LnFD_matched_1e_cut", "#theta^{reco}_{LnFD} in 1e cut (matched);#theta^{reco}_{LnFD} [#circ];Counts", 50, 0, 45.);
        HistoList.push_back(h_reco_theta_LnFD_matched_1e_cut);
        TH1D* h_reco_phi_LnFD_matched_1e_cut = new TH1D("reco_phi_LnFD_matched_1e_cut", "#phi^{reco}_{LnFD} in 1e cut (matched);#phi^{reco}_{LnFD} [#circ];Counts", 50, -180, 180.);
        HistoList.push_back(h_reco_phi_LnFD_matched_1e_cut);
        TH2D* h_reco_theta_LnFD_matched_VS_reco_phi_LnFD_matched_1e_cut =
            new TH2D("reco_theta_LnFD_matched_VS_reco_phi_LnFD_matched_1e_cut",
                     "#theta^{reco}_{LnFD} vs. #phi^{reco}_{LnFD} in 1e cut (matched);#phi^{reco}_{LnFD} [#circ];#theta^{reco}_{LnFD} [#circ]", 100, -180., 180., 100, 0, 50.);
        HistoList.push_back(h_reco_theta_LnFD_matched_VS_reco_phi_LnFD_matched_1e_cut);
    */

    TH1D* h_reco_P_phFD_BPID_matched_1e_cut =
        new TH1D("reco_P_phFD_BPID_matched_1e_cut", "P^{reco}_{#gammaFD} in 1e cut BPID (matched);P^{reco}_{#gammaFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_phFD_BPID_matched_1e_cut);
    TH1D* h_reco_P_phFD_APID_matched_1e_cut =
        new TH1D("reco_P_phFD_APID_matched_1e_cut", "P^{reco}_{#gammaFD} in 1e cut APID (matched);P^{reco}_{#gammaFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_phFD_APID_matched_1e_cut);

    TH1D* h_Edep_ECAL_nFD_matched_1e_cut = new TH1D("Edep_ECAL_nFD_matched_1e_cut", "E^{ECAL}_{dep} in 1e cut (matched);E^{ECAL}_{dep} [GeV];Counts", 50, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_nFD_matched_1e_cut);
    TH1D* h_beta_nFD_matched_1e_cut = new TH1D("beta_nFD_matched_1e_cut", "#beta_{nFD} in 1e cut (matched);#beta_{nFD};Counts", 50, 0.3, 1.1);
    HistoList.push_back(h_beta_nFD_matched_1e_cut);
    TH2D* h_Edep_ECAL_nFD_VS_beta_nFD_matched_1e_cut =
        new TH2D("Edep_ECAL_nFD_VS_beta_nFD_matched_1e_cut", "E^{ECAL}_{dep} vs. #beta_{nFD} in 1e cut (matched);#beta_{nFD};E^{ECAL}_{dep} [GeV]", 100, 0.3, 1.1, 100, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_nFD_VS_beta_nFD_matched_1e_cut);

    /*
        TH1D* h_Edep_ECAL_LnFD_matched_1e_cut = new TH1D("Edep_ECAL_LnFD_matched_1e_cut", "E^{ECAL}_{dep} in 1e cut (matched);E^{ECAL}_{dep} [GeV];Counts", 50, 0., 0.5);
        HistoList.push_back(h_Edep_ECAL_LnFD_matched_1e_cut);
        TH1D* h_beta_LnFD_matched_1e_cut = new TH1D("beta_LnFD_matched_1e_cut", "#beta_{LnFD} in 1e cut (matched);#beta_{LnFD};Counts", 50, 0.3, 1.1);
        HistoList.push_back(h_beta_LnFD_matched_1e_cut);
        TH2D* h_Edep_ECAL_LnFD_VS_beta_LnFD_matched_1e_cut =
            new TH2D("Edep_ECAL_LnFD_VS_beta_LnFD_matched_1e_cut", "E^{ECAL}_{dep} vs. #beta_{LnFD} in 1e cut (matched);#beta_{LnFD};E^{ECAL}_{dep} [GeV]", 100, 0.3, 1.1, 100, 0., 0.5);
        HistoList.push_back(h_Edep_ECAL_LnFD_VS_beta_LnFD_matched_1e_cut);
    */

    TH2D* h_nFD_PCAL_hits_VS_nFD_ECIN_hits_matched_1e_cut =
        new TH2D("nFD_PCAL_hits_VS_nFD_ECIN_hits_matched_1e_cut", "#(nFD hits in PCAL) vs. #(nFD hits in ECIN) in 1e cut (matched);#(nFD hits in PCAL);#(nFD hits in ECIN)", 2, -0.5, 1.5, 2,
                 -0.5, 1.5);
    HistoList.push_back(h_nFD_PCAL_hits_VS_nFD_ECIN_hits_matched_1e_cut);
    TH2D* h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_matched_1e_cut =
        new TH2D("nFD_PCAL_hits_VS_nFD_ECOUT_hits_matched_1e_cut", "#(nFD hits in PCAL) vs. #(nFD hits in ECOUT) in 1e cut (matched);#(nFD hits in PCAL);#(nFD hits in ECOUT)", 2, -0.5, 1.5,
                 2, -0.5, 1.5);
    HistoList.push_back(h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_matched_1e_cut);
    TH2D* h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_matched_1e_cut =
        new TH2D("nFD_ECIN_hits_VS_nFD_ECOUT_hits_matched_1e_cut", "#(nFD hits in ECIN) vs. #(nFD hits in ECOUT) in 1e cut (matched);#(nFD hits in ECIN);#(nFD hits in ECOUT)", 2, -0.5, 1.5,
                 2, -0.5, 1.5);
    HistoList.push_back(h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_matched_1e_cut);

    TH2D* h_reco_P_e_VS_P_nFD_matched_1e_cut = new TH2D("reco_P_e_VS_P_nFD_matched_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]",
                                                        100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_VS_P_nFD_matched_1e_cut =
        new TH2D("reco_theta_e_VS_P_nFD_matched_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0, 45., 100, 0.,
                 Ebeam * 3.);
    HistoList.push_back(h_reco_theta_e_VS_P_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_P_nFD_matched_1e_cut = new TH2D(
        "reco_phi_e_VS_P_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_phi_e_VS_P_nFD_matched_1e_cut);
    TH2D* h_reco_P_e_VS_theta_nFD_matched_1e_cut =
        new TH2D("reco_P_e_VS_theta_nFD_matched_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];#theta^{reco}_{nFD} [#circ]", 100, 0., P_upperLim,
                 100, 0, 45.);
    HistoList.push_back(h_reco_P_e_VS_theta_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_VS_theta_nFD_matched_1e_cut =
        new TH2D("reco_theta_e_VS_theta_nFD_matched_1e_cut", "#theta^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, 0,
                 45., 100, 0, 45.);
    HistoList.push_back(h_reco_theta_e_VS_theta_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_theta_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_theta_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180.,
                 180., 100, 0, 45.);
    HistoList.push_back(h_reco_phi_e_VS_theta_nFD_matched_1e_cut);
    TH2D* h_reco_P_e_VS_phi_nFD_matched_1e_cut = new TH2D(
        "reco_P_e_VS_phi_nFD_matched_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];#phi^{reco}_{nFD} [#circ]", 100, 0., P_upperLim, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_e_VS_phi_nFD_matched_1e_cut =
        new TH2D("reco_theta_e_VS_phi_nFD_matched_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, 0, 45.,
                 100, -180., 180.);
    HistoList.push_back(h_reco_theta_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_phi_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, -180., 180., 100,
                 -180., 180.);
    HistoList.push_back(h_reco_phi_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut =
        new TH2D("reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0.,
                 45., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut);
    TH1D* h_reco_nFD_multi_AllN_PassMatch_matched_1e_cut =
        new TH1D("reco_nFD_multi_AllN_PassMatch_matched_1e_cut", "reco nFD multiplicity in 1e cut (all nFDs, matched);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_AllN_PassMatch_matched_1e_cut);
    TH1D* h_reco_nFD_multi_OnlyGood_PassMatch_matched_1e_cut =
        new TH1D("reco_nFD_multi_OnlyGood_PassMatch_matched_1e_cut", "Multiplicity of nFDs passing match cuts in 1e cut (Only good nFD, matched);Good nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_OnlyGood_PassMatch_matched_1e_cut);
    TH1D* h_reco_nFD_multi_OnlyBad_PassMatch_matched_1e_cut =
        new TH1D("reco_nFD_multi_OnlyBad_PassMatch_matched_1e_cut", "Multiplicity of nFDs passing match cuts in 1e cut (Only bad nFD, matched);Bad nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_OnlyBad_PassMatch_matched_1e_cut);
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_PassMatch_matched_1e_cut =
        new TH2D("reco_nFD_multi_VS_reco_P_nFD_PassMatch_matched_1e_cut",
                 "Multiplicity of nFDs passing match cuts vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam * 3., 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_PassMatch_matched_1e_cut);

    TH1D* h_reco_nFD_multi_AllN_NotPassMatch_matched_1e_cut =
        new TH1D("reco_nFD_multi_AllN_NotPassMatch_matched_1e_cut", "Multiplicity of nFDs not passing match cuts in 1e cut (all nFDs, matched);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_AllN_NotPassMatch_matched_1e_cut);
    TH1D* h_reco_nFD_multi_OnlyGood_NotPassMatch_matched_1e_cut = new TH1D(
        "reco_nFD_multi_OnlyGood_NotPassMatch_matched_1e_cut", "Multiplicity of nFDs not passing match cuts in 1e cut (Only good nFD, matched);Good nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_OnlyGood_NotPassMatch_matched_1e_cut);
    TH1D* h_reco_nFD_multi_OnlyBad_NotPassMatch_matched_1e_cut =
        new TH1D("reco_nFD_multi_OnlyBad_NotPassMatch_matched_1e_cut", "Multiplicity of nFDs not passing match cuts in 1e cut (Only bad nFD, matched);Bad nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_OnlyBad_NotPassMatch_matched_1e_cut);
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_NotPassMatch_matched_1e_cut =
        new TH2D("reco_nFD_multi_VS_reco_P_nFD_NotPassMatch_matched_1e_cut",
                 "Multiplicity of nFDs not passing match cuts vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam * 3., 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_NotPassMatch_matched_1e_cut);

    /*
    TH2D* h_reco_P_e_VS_P_LnFD_matched_1e_cut = new TH2D(
            "reco_P_e_VS_P_LnFD_matched_1e_cut", "P^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];P^{reco}_{LnFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam
    * 3.); HistoList.push_back(h_reco_P_e_VS_P_LnFD_matched_1e_cut); TH2D* h_reco_theta_e_VS_P_LnFD_matched_1e_cut = new TH2D("reco_theta_e_VS_P_LnFD_matched_1e_cut", "#theta^{reco}_{e} vs.
    P^{reco}_{LnFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];P^{reco}_{LnFD} [GeV/c]", 100, 0, 45., 100, 0., Ebeam * 3.); HistoList.push_back(h_reco_theta_e_VS_P_LnFD_matched_1e_cut);
        TH2D* h_reco_phi_e_VS_P_LnFD_matched_1e_cut =
            new TH2D("reco_phi_e_VS_P_LnFD_matched_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];P^{reco}_{LnFD} [GeV/c]", 100, -180., 180., 100,
    0., Ebeam * 3.); HistoList.push_back(h_reco_phi_e_VS_P_LnFD_matched_1e_cut); TH2D* h_reco_P_e_VS_theta_LnFD_matched_1e_cut = new TH2D("reco_P_e_VS_theta_LnFD_matched_1e_cut",
    "P^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];#theta^{reco}_{LnFD} [#circ]", 100, 0., P_upperLim, 100, 0, 45.);
        HistoList.push_back(h_reco_P_e_VS_theta_LnFD_matched_1e_cut);
        TH2D* h_reco_theta_e_VS_theta_LnFD_matched_1e_cut =
            new TH2D("reco_theta_e_VS_theta_LnFD_matched_1e_cut", "#theta^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];#theta^{reco}_{LnFD} [#circ]",
    100, 0, 45., 100, 0, 45.); HistoList.push_back(h_reco_theta_e_VS_theta_LnFD_matched_1e_cut); TH2D* h_reco_phi_e_VS_theta_LnFD_matched_1e_cut = new
    TH2D("reco_phi_e_VS_theta_LnFD_matched_1e_cut", "#phi^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];#theta^{reco}_{LnFD} [#circ]", 100, -180., 180.,
    100, 0, 45.); HistoList.push_back(h_reco_phi_e_VS_theta_LnFD_matched_1e_cut); TH2D* h_reco_P_e_VS_phi_LnFD_matched_1e_cut = new TH2D("reco_P_e_VS_phi_LnFD_matched_1e_cut", "P^{reco}_{e}
    vs. #phi^{reco}_{LnFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];#phi^{reco}_{LnFD} [#circ]", 100, 0., P_upperLim, 100, -180., 180.);
        HistoList.push_back(h_reco_P_e_VS_phi_LnFD_matched_1e_cut);
        TH2D* h_reco_theta_e_VS_phi_LnFD_matched_1e_cut =
            new TH2D("reco_theta_e_VS_phi_LnFD_matched_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];#phi^{reco}_{LnFD} [#circ]", 100,
    0, 45., 100, -180., 180.); HistoList.push_back(h_reco_theta_e_VS_phi_LnFD_matched_1e_cut); TH2D* h_reco_phi_e_VS_phi_LnFD_matched_1e_cut = new
    TH2D("reco_phi_e_VS_phi_LnFD_matched_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];#phi^{reco}_{LnFD} [#circ]", 100, -180., 180., 100,
    -180., 180.); HistoList.push_back(h_reco_phi_e_VS_phi_LnFD_matched_1e_cut); TH2D* h_reco_theta_LnFD_matched_VS_P_LnFD_matched_1e_cut = new
    TH2D("reco_theta_LnFD_matched_VS_P_LnFD_matched_1e_cut", "#theta^{reco}_{LnFD} vs. P^{reco}_{LnFD} in 1e cut (matched);#theta^{reco}_{LnFD} [#circ];P^{reco}_{LnFD} [GeV/c]", 100,
                    0., 45., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_theta_LnFD_matched_VS_P_LnFD_matched_1e_cut);
    */

#pragma endregion

    TH1D* h_reco_dTheta_nFD_e_matched_1e_cut =
        new TH1D("reco_dTheta_nFD_e_matched_1e_cut", "#delta#theta^{reco}_{nFD,e} in 1e cut (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts",
                 50, -25., 10.);
    HistoList.push_back(h_reco_dTheta_nFD_e_matched_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_matched_1e_cut = new TH1D(
        "reco_dPhi_nFD_e_matched_1e_cut", "#delta#phi^{reco}_{nFD,e} in 1e cut (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_dPhi_nFD_e_matched_1e_cut);
    TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_matched_1e_cut =
        new TH2D("reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_matched_1e_cut",
                 "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                 "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_dTheta_nFD_e_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_dTheta_nFD_e_matched_1e_cut",
                 "P^{reco}_{nFD} vs. #delta#theta^{reco}_{nFD,e} in 1e cut (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];P^{reco}_{nFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_dTheta_nFD_e_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_dPhi_nFD_e_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_dPhi_nFD_e_matched_1e_cut",
                 "P^{reco}_{nFD} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];P^{reco}_{nFD} [GeV/c]", 100,
                 -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_dPhi_nFD_e_matched_1e_cut);

    TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e_matched_1e_cut =
        new TH2D("reco_P_e_VS_reco_dPhi_nFD_e_matched_1e_cut",
                 "P^{reco}_{e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (matched);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0.,
                 Ebeam * 1.1, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e_matched_1e_cut);

    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_matched_1e_cut =
        new TH2D("nFD_status_VS_reco_dTheta_nFD_e_matched_1e_cut",
                 "nFD status vs. #delta#theta^{reco}_{nFD,e} in 1e cut (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];nFD status",
                 100, -25., 10., 100, 1990, 3000.);
    HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_matched_1e_cut);

    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut =
        new TH2D("nFD_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut",
                 "nFD status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];nFD status",
                 100, -10., 5., 100, 1990, 2250.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_nFD_e_matched_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_nFD_e_matched_1e_cut",
                 "e status vs. #delta#theta^{reco}_{nFD,e} in 1e cut (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -25., 10., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_matched_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut",
                 "e status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -10., 5., 100, -2250., -0.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut);

    TH2D* h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_matched_1e_cut =
        new TH2D("dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_matched_1e_cut",
                 "'Neutron Hits' vs. Electron Hits - After Veto in 1e cut (matched);#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ];#Delta#theta_{n,e} = "
                 "#theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
                 100, -180, 180, 100, -25, 10);
    //  100, -180, 180, 100, -50, 50);
    HistoList.push_back(h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_matched_1e_cut);

    TH1D* h_dr_n_matched_1e_cut =
        new TH1D("dr_n_matched_1e_cut", "|#Delta#font[62]{r}_{n}| in 1e cut (matched);|#Delta#font[62]{r}_{n}| = |#font[62]{r}^{hit}_{n} - #font[62]{R}_{e}| [cm];Counts", 50, 0, 400);
    HistoList.push_back(h_dr_n_matched_1e_cut);
    TH2D* h_dr_n_VS_dTheta_n_e_matched_1e_cut = new TH2D(
        "dr_n_VS_dTheta_n_e_matched_1e_cut",
        "|#Delta#font[62]{r}_{n}| vs. #delta#theta^{reco}_{nFD,e} - in 1e cut (matched);|#Delta#font[62]{r}_{n}| = |#font[62]{r}^{hit}_{n} - #font[62]{R}_{e}| [cm];#Delta#theta_{nFD,e} = "
        "#theta_{V}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
        100, 0, 400, 100, -25, 10);
    HistoList.push_back(h_dr_n_VS_dTheta_n_e_matched_1e_cut);
    TH2D* h_dr_n_VS_dPhi_n_e_matched_1e_cut = new TH2D(
        "dr_n_VS_dPhi_n_e_matched_1e_cut",
        "|#Delta#font[62]{r}_{n}| vs. #delta#phi^{reco}_{nFD,e} - in 1e cut (matched);|#Delta#font[62]{r}_{n}| = |#font[62]{r}^{hit}_{n} - #font[62]{R}_{e}| [cm];#Delta#phi_{nFD,e} = "
        "#phi_{V}^{ECAL} - #phi_{e}^{ECAL} [#circ]",
        100, 0, 400, 100, -25, 10);
    HistoList.push_back(h_dr_n_VS_dPhi_n_e_matched_1e_cut);

    std::vector<TObject*> Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_matched_1e_cut;
    hsPlots h_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_matched_1e_cut =
        hsPlots(dr_n_SliceLimits, hsPlots::TH2D_TYPE, HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n, &Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_matched_1e_cut,
                "dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_matched_1e_cut",
                "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in slices of |#Deltar_{n}| in 1e cut (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                100, -180., 180., 100, -25., 10., "|#Deltar_{n}| [cm]");

    TH2D* h_dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_matched_1e_cut =
        new TH2D("dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_matched_1e_cut",
                 "#Delta#theta_{nFD,V} vs. #Delta#phi_{nFD,V} - After Veto in 1e cut (matched);#Delta#phi_{n,V} = #phi_{n}^{ECAL} - #phi_{V} [#circ];#Delta#theta_{nFD,V} = "
                 "#theta_{n}^{ECAL} - #theta_{V} [#circ]",
                 100, -180, 180, 100, -25, 10);
    //  100, -180, 180, 100, -50, 50);
    HistoList.push_back(h_dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_matched_1e_cut);

#pragma region /* Same sector plots - nFD */

    TH1D* h_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH1D("reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "#delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
    HistoList.push_back(h_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut);
    TH1D* h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut = new TH1D(
        "reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut",
        "#delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} - zoomin (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts", 50, -10., 5.);
    HistoList.push_back(h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH1D("reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "#delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
    HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut = new TH1D(
        "reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut",
        "#delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} - zoomin (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -50., 10.);
    HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_matched_1e_cut =
        new TH1D("reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_matched_1e_cut",
                 ("#delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} and " + bt::ToStringWithPrecision(dTheta_cuts.GetLowerCut()) + " #leq #delta#theta^{reco}_{nFD,e} #leq " +
                  bt::ToStringWithPrecision(dTheta_cuts.GetUpperCut()) + " (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts")
                     .c_str(),
                 50, -180., 180.);
    HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_matched_1e_cut);
    TH1D* h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin_matched_1e_cut =
        new TH1D("reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin_matched_1e_cut",
                 ("#delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} and " + bt::ToStringWithPrecision(dTheta_cuts.GetLowerCut()) + " #leq #delta#theta^{reco}_{nFD,e} #leq " +
                  bt::ToStringWithPrecision(dTheta_cuts.GetUpperCut()) + " - zoomin (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts")
                     .c_str(),
                 50, -50., 10.);
    HistoList.push_back(h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin_matched_1e_cut);
    TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| "
                 "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                 100, -180., 180., 100, -25., 10.);
    HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut);
    TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut =
        new TH2D("reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut",
                 "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} - zoomin (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| "
                 "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                 100, -50., 10., 100, -10., 5.);
    HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut);

    TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "P^{reco}_{e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| [#circ]",
                 100, 0., Ebeam * 1.1, 100, -180., 180.);
    HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut =
        new TH2D("reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut",
                 "P^{reco}_{e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} - zoomin (matched);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| [#circ]",
                 100, 0., Ebeam * 1.1, 100, -50., 10);
    HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut);

    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "nFD status vs. #delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];nFD status",
                 100, -25., 10., 100, 1990, 3000.);
    HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut",
                 "nFD status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - "
                 "#theta^{reco}_{e} "
                 "[#circ];nFD status",
                 100, -10., 5., 100, 1990, 2250.);
    //  100, -25., 10., 100, 1990, 2250.);
    HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "e status vs. #delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -25., 10., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut",
                 "e status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                 "[#circ];e status",
                 100, -10., 5., 100, -2250., -2100.);
    HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut);

    TH1D* h_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH1D("reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut", "#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#theta^{reco}_{nFD,e} [#circ];Counts", 50, 0., 50.);
    HistoList.push_back(h_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "#theta^{reco}_{nFD,e} vs. #delta#theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - "
                 "#theta^{reco}_{e} "
                 "[#circ];#theta^{reco}_{nFD,e} [#circ]",
                 100, -25., 10., 100, 0., 50.);
    HistoList.push_back(h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                 "[#circ];#theta^{reco}_{nFD,e} [#circ]",
                 100, -180., 180., 100, 0., 50.);
    HistoList.push_back(h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut =
        new TH2D("reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut",
                 "#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - "
                 "#phi^{reco}_{e}| [#circ];#theta^{reco}_{nFD,e} [#circ]",
                 100, -50., 10., 100, 0., 25.);
    HistoList.push_back(h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut);

    TH2D* h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_matched_1e_cut",
                 "#theta^{reco}_{nFD,e} vs. P^{reco}_{nFD} in 1e cut for S_{nFD} = S_{e} (matched);P^{reco}_{nFD} [GeV/c];#theta^{reco}_{nFD,e} [#circ]", 100, 0., Ebeam * 3., 100, 0., 50.);
    HistoList.push_back(h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "nFD status vs. #theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#theta^{reco}_{nFD,e} [#circ];nFD status", 100, 0., 50., 100, 1990, 3000.);
    HistoList.push_back(h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut",
                 "nFD status vs. #theta^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (matched);#theta^{reco}_{nFD,e} [#circ];nFD status", 100, 0., 20., 100, 1990, 2250.);
    HistoList.push_back(h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut",
                 "e status vs. #theta^{reco}_{nFD,e} in 1e cut for S_{nFD} = S_{e} (matched);#theta^{reco}_{nFD,e} [#circ];e status", 100, 0., 50., 100, -2250., -1990.);
    HistoList.push_back(h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut);

    TH2D* h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut =
        new TH2D("e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut",
                 "e status vs. #theta^{reco}_{nFD,e} - zoomin - in 1e cut for S_{nFD} = S_{e} (matched);#theta^{reco}_{nFD,e} [#circ];e status", 100, 0., 20., 100, -2250., -0.);
    HistoList.push_back(h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut);

#pragma endregion

    /*
    #pragma region New plots - LnFD

        TH1D* h_reco_dTheta_LnFD_e_matched_1e_cut =
            new TH1D("reco_dTheta_LnFD_e_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} in 1e cut (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_matched_1e_cut);
        TH1D* h_reco_dPhi_LnFD_e_matched_1e_cut =
            new TH1D("reco_dPhi_LnFD_e_matched_1e_cut", "#Delta#phi^{reco}_{LnFD,e} in 1e cut (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts",
    50, -180., 180.); HistoList.push_back(h_reco_dPhi_LnFD_e_matched_1e_cut); TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_matched_1e_cut = new
    TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                     100, -180., 180., 100, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_matched_1e_cut);
        TH2D* h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_matched_1e_cut =
            new TH2D("reco_P_LnFD_VS_reco_dTheta_LnFD_e_matched_1e_cut",
                     "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];P^{reco}_{LnFD} [GeV/c]",
                     100, -25., 10., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_matched_1e_cut);
        TH2D* h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_matched_1e_cut =
            new TH2D("reco_P_LnFD_VS_reco_dPhi_LnFD_e_matched_1e_cut",
                     "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ];P^{reco}_{LnFD} [GeV/c]",
                     100, -180., 180., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_matched_1e_cut);

        TH2D* h_reco_P_e_VS_reco_dPhi_LnFD_e_matched_1e_cut =
            new TH2D("reco_P_e_VS_reco_dPhi_LnFD_e_matched_1e_cut",
                     "P^{reco}_{e} vs. #delta#phi^{reco}_{LnFD,e} in 1e cut (matched);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]", 100,
    0., Ebeam * 1.1, 100, -180., 180.); HistoList.push_back(h_reco_P_e_VS_reco_dPhi_LnFD_e_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -25., 10., 100, 1990, 3000.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -10., 5., 100, 1990, 2250.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -25., 10., 100, -2250., -1990.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -10., 5., 100, -2250., -0.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_matched_1e_cut);

        TH1D* h_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut =
            new TH1D("reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut);
        TH1D* h_reco_dPhi_LnFD_e_PCAL_only_matched_1e_cut =
            new TH1D("reco_dPhi_LnFD_e_PCAL_only_matched_1e_cut",
                     "#Delta#phi^{reco}_{LnFD,e} in 1e cut, PCAL only (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
        HistoList.push_back(h_reco_dPhi_LnFD_e_PCAL_only_matched_1e_cut);
        TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_PCAL_only_matched_1e_cut =
            new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_PCAL_only_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, PCAL only (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                     100, -180., 180., 100, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_PCAL_only_matched_1e_cut);
        TH2D* h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut =
            new TH2D("reco_P_LnFD_VS_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut",
                     "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];P^{reco}_{LnFD} [GeV/c]",
                     100, -25., 10., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut);
        TH2D* h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_PCAL_only_matched_1e_cut =
            new TH2D("reco_P_LnFD_VS_reco_dPhi_LnFD_e_PCAL_only_matched_1e_cut",
                     "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, PCAL only (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ];P^{reco}_{LnFD} [GeV/c]",
                     100, -180., 180., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_PCAL_only_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -25., 10., 100, 1990, 3000.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, PCAL only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -10., 5., 100, 1990, 2250.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -25., 10., 100, -2250., -1990.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_PCAL_only_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, PCAL only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -10., 5., 100, -2250., -0.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_matched_1e_cut);

        TH1D* h_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut =
            new TH1D("reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut);
        TH1D* h_reco_dPhi_LnFD_e_ECIN_only_matched_1e_cut =
            new TH1D("reco_dPhi_LnFD_e_ECIN_only_matched_1e_cut",
                     "#Delta#phi^{reco}_{LnFD,e} in 1e cut, ECIN only (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
        HistoList.push_back(h_reco_dPhi_LnFD_e_ECIN_only_matched_1e_cut);
        TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECIN_only_matched_1e_cut =
            new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECIN_only_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECIN only (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                     100, -180., 180., 100, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECIN_only_matched_1e_cut);
        TH2D* h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut =
            new TH2D("reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut",
                     "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];P^{reco}_{LnFD} [GeV/c]",
                     100, -25., 10., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut);
        TH2D* h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECIN_only_matched_1e_cut =
            new TH2D("reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECIN_only_matched_1e_cut",
                     "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECIN only (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ];P^{reco}_{LnFD} [GeV/c]",
                     100, -180., 180., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECIN_only_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -25., 10., 100, 1990, 3000.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECIN only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -10., 5., 100, 1990, 2250.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -25., 10., 100, -2250., -1990.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_ECIN_only_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECIN only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -10., 5., 100, -2250., -0.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_matched_1e_cut);

        TH1D* h_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut =
            new TH1D("reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut);
        TH1D* h_reco_dPhi_LnFD_e_ECOUT_only_matched_1e_cut =
            new TH1D("reco_dPhi_LnFD_e_ECOUT_only_matched_1e_cut",
                     "#Delta#phi^{reco}_{LnFD,e} in 1e cut, ECOUT only (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
        HistoList.push_back(h_reco_dPhi_LnFD_e_ECOUT_only_matched_1e_cut);
        TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECOUT_only_matched_1e_cut =
            new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECOUT_only_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECOUT only (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                     100, -180., 180., 100, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECOUT_only_matched_1e_cut);
        TH2D* h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut =
            new TH2D("reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut",
                     "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];P^{reco}_{LnFD} [GeV/c]",
                     100, -25., 10., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut);
        TH2D* h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECOUT_only_matched_1e_cut =
            new TH2D("reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECOUT_only_matched_1e_cut",
                     "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECOUT only (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ];P^{reco}_{LnFD} [GeV/c]",
                     100, -180., 180., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECOUT_only_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -25., 10., 100, 1990, 3000.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECOUT only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -10., 5., 100, 1990, 2250.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -25., 10., 100, -2250., -1990.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_ECOUT_only_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECOUT only (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -10., 5., 100, -2250., -0.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_matched_1e_cut);
    */

    TH2D* h_P_nFD_Res_VS_truth_P_nFD_matched_1e_cut = new TH2D("P_nFD_Res_VS_truth_P_nFD_matched_1e_cut",
                                                               "R_{nFD} vs. P^{truth}_{nFD} in 1e cut (matched);P^{truth}_{nFD} [GeV/c];"
                                                               "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                                               100, 0, Ebeam * 1.1, 100, -1.1, 1.1);
    HistoList.push_back(h_P_nFD_Res_VS_truth_P_nFD_matched_1e_cut);
    TH2D* h_P_nFD_Res_VS_reco_P_nFD_matched_1e_cut = new TH2D("P_nFD_Res_VS_reco_P_nFD_matched_1e_cut",
                                                              "R_{nFD} vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];"
                                                              "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                                              100, 0, Ebeam * 1.1, 100, -1.1, 1.1);
    HistoList.push_back(h_P_nFD_Res_VS_reco_P_nFD_matched_1e_cut);

    std::vector<TObject*> Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_matched_1e_cut;
    hsPlots h_P_nFD_Res_BysliceOf_truth_P_nFD_matched_1e_cut =
        hsPlots(nFD_Res_SliceLimits, hsPlots::TH1D_TYPE, HistoList_nRes_BysliceOf_truth_P_nFD, &Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_matched_1e_cut,
                "P_nFD_Res_BysliceOf_truth_P_nFD_matched_1e_cut",
                "P_{nFD} resolution in slices of P^{truth}_{nFD} in 1e cut (matched);Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD};Counts", 50, -1.1, 1.1, 50, -1.1, 1.1,
                "P^{truth}_{nFD} [GeV/c]");
    std::vector<TObject*> Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_matched_1e_cut;
    hsPlots h_P_nFD_Res_BysliceOf_reco_P_nFD_matched_1e_cut =
        hsPlots(nFD_Res_SliceLimits, hsPlots::TH1D_TYPE, HistoList_nRes_BysliceOf_reco_P_nFD, &Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_matched_1e_cut,
                "P_nFD_Res_BysliceOf_reco_P_nFD_matched_1e_cut",
                "P_{nFD} resolution in slices of P^{reco}_{nFD} in 1e cut (matched);Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD};Counts", 50, -1.1, 1.1, 50, -1.1, 1.1,
                "P^{reco}_{nFD} [GeV/c]");

    /*
    #pragma region Same sector plots - LnFD

        TH1D* h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut = new TH1D(
            "reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut",
            "#Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut);
        TH1D* h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_zoomin_matched_1e_cut =
            new TH1D("reco_dTheta_LnFD_e_Same_e_LnFD_Sector_zoomin_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} - zoomin (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];Counts",
                     50, -10., 5.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_zoomin_matched_1e_cut);
        TH1D* h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut =
            new TH1D("reco_dPhi_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut",
                     "#Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180.,
    180.); HistoList.push_back(h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut); TH1D* h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_matched_1e_cut = new TH1D(
            "reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_matched_1e_cut",
            "#Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} - zoomin (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -50., 10.);
        HistoList.push_back(h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_matched_1e_cut);
        TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - "
                     "#phi^{reco}_{e}| "
                     "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                     100, -180., 180., 100, -25., 10.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut);
        TH2D* h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_matched_1e_cut =
            new TH2D("reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_matched_1e_cut",
                     "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} - zoomin (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - "
                     "#phi^{reco}_{e}| "
                     "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                     100, -50., 10., 100, -10., 5.);
        HistoList.push_back(h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_matched_1e_cut);

        TH2D* h_reco_P_e_VS_reco_dPhi_LnFD_e_Same_e_nFD_Sector_matched_1e_cut =
            new TH2D("reco_P_e_VS_reco_dPhi_LnFD_e_Same_e_nFD_Sector_matched_1e_cut",
                     "P^{reco}_{e} vs. #delta#phi^{reco}_{LnFD,e} in 1e cut (matched);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]", 100,
    0., Ebeam * 1.1, 100, -180., 180.); HistoList.push_back(h_reco_P_e_VS_reco_dPhi_LnFD_e_Same_e_nFD_Sector_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -25., 10., 100, 1990, 3000.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut",
                     "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut for S_{LnFD} = S_{e} (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - "
                     "#theta^{reco}_{e} "
                     "[#circ];LnFD status",
                     100, -10., 5., 100, 1990, 2250.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -25., 10., 100, -2250., -1990.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_e_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("e_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut",
                     "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut for S_{LnFD} = S_{e} (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - "
                     "#theta^{reco}_{e} "
                     "[#circ];e status",
                     100, -10., 5., 100, -2250., -0.);
        //  100, -25., 10., 100, 1990, 2250.);
        HistoList.push_back(h_e_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut);

        TH1D* h_reco_theta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut =
            new TH1D("reco_theta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut", "#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#theta^{reco}_{LnFD,e} [#circ];Counts", 50,
    0., 50.); HistoList.push_back(h_reco_theta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_reco_theta_LnFD_e_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("reco_theta_LnFD_e_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut",
                     "#theta^{reco}_{LnFD,e} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - "
                     "#theta^{reco}_{e} "
                     "[#circ];#theta^{reco}_{LnFD,e} [#circ]",
                     100, -25., 10., 100, 0., 50.);
        HistoList.push_back(h_reco_theta_LnFD_e_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_reco_theta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("reco_theta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut",
                     "#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ];#theta^{reco}_{LnFD,e} [#circ]",
                     100, -180., 180., 100, 0., 50.);
        HistoList.push_back(h_reco_theta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_reco_theta_LnFD_e_VS_reco_P_LnFD_Same_e_LnFD_Sector_matched_1e_cut = new TH2D(
            "reco_theta_LnFD_e_VS_reco_P_LnFD_Same_e_LnFD_Sector_matched_1e_cut",
            "#theta^{reco}_{LnFD,e} vs. P^{reco}_{LnFD} in 1e cut for S_{LnFD} = S_{e} (matched);P^{reco}_{LnFD} [GeV/c];#theta^{reco}_{LnFD,e} [#circ]", 100, 0., Ebeam * 3., 100, 0., 50.);
        HistoList.push_back(h_reco_theta_LnFD_e_VS_reco_P_LnFD_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut",
                     "LnFD status vs. #theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#theta^{reco}_{LnFD,e} [#circ];LnFD status", 100, 0., 50., 100, 1990, 3000.);
        HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_LnFD_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("LnFD_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut",
                     "LnFD status vs. #theta^{reco}_{LnFD,e} - zoomin - in 1e cut for S_{LnFD} = S_{e} (matched);#theta^{reco}_{LnFD,e} [#circ];LnFD status", 100, 0., 20., 100, 1990, 2250.);
        HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_e_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("e_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut",
                     "e status vs. #theta^{reco}_{LnFD,e} in 1e cut for S_{LnFD} = S_{e} (matched);#theta^{reco}_{LnFD,e} [#circ];e status", 100, 0., 50., 100, -2250., -1990.);
        HistoList.push_back(h_e_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_matched_1e_cut);

        TH2D* h_e_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut =
            new TH2D("e_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut",
                     "e status vs. #theta^{reco}_{LnFD,e} - zoomin - in 1e cut for S_{LnFD} = S_{e} (matched);#theta^{reco}_{LnFD,e} [#circ];e status", 100, -10., 5., 100, -2250., -0.);
        HistoList.push_back(h_e_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_matched_1e_cut);

    #pragma endregion

    #pragma endregion
    */

    TH1D* h_v_dist_nFD_matched_1e_cut = new TH1D("v_dist_nFD_matched_1e_cut", "v_dist in 1e cut (matched);v_dist [cm];Counts", 50, 0., 1000.);
    HistoList.push_back(h_v_dist_nFD_matched_1e_cut);
    TH2D* h_v_dist_nFD_VS_reco_P_nFD_matched_1e_cut =
        new TH2D("v_dist_nFD_VS_reco_P_nFD_matched_1e_cut", "v_dist vs. P^{reco}_{nFD} in 1e cut (matched);v_dist [cm];P^{reco}_{nFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_v_dist_nFD_VS_reco_P_nFD_matched_1e_cut);
    TH2D* h_v_dist_nFD_VS_reco_P_e_matched_1e_cut =
        new TH2D("v_dist_nFD_VS_reco_P_e_matched_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (matched);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
    HistoList.push_back(h_v_dist_nFD_VS_reco_P_e_matched_1e_cut);
    TH2D* h_v_dist_nFD_VS_reco_dPhi_nFD_e_matched_1e_cut = new TH2D(
        "v_dist_nFD_VS_reco_dPhi_nFD_e_matched_1e_cut",
        "v_dist vs. #delta#phi^{reco}_{nFD,e} in 1e cut (matched);v_dist [cm];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0., 1000., 100, -180., 180.);
    HistoList.push_back(h_v_dist_nFD_VS_reco_dPhi_nFD_e_matched_1e_cut);

    /*
        TH1D* h_v_dist_LnFD_matched_1e_cut = new TH1D("v_dist_LnFD_matched_1e_cut", "v_dist in 1e cut (matched);v_dist [cm];Counts", 50, 0., 1000.);
        HistoList.push_back(h_v_dist_LnFD_matched_1e_cut);
        TH2D* h_v_dist_LnFD_VS_reco_P_LnFD_matched_1e_cut =
            new TH2D("v_dist_LnFD_VS_reco_P_LnFD_matched_1e_cut", "v_dist vs. P^{reco}_{LnFD} in 1e cut (matched);v_dist [cm];P^{reco}_{LnFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
        HistoList.push_back(h_v_dist_LnFD_VS_reco_P_LnFD_matched_1e_cut);
        TH2D* h_v_dist_LnFD_VS_reco_P_e_matched_1e_cut =
            new TH2D("v_dist_LnFD_VS_reco_P_e_matched_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (matched);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
        HistoList.push_back(h_v_dist_LnFD_VS_reco_P_e_matched_1e_cut);
        TH2D* h_v_dist_LnFD_VS_reco_dPhi_LnFD_e_matched_1e_cut =
            new TH2D("v_dist_LnFD_VS_reco_dPhi_LnFD_e_matched_1e_cut",
                     "v_dist vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (matched);v_dist [cm];#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]", 100, 0., 1000., 100,
                     -180., 180.);
        HistoList.push_back(h_v_dist_LnFD_VS_reco_dPhi_LnFD_e_matched_1e_cut);
     */

    TH2D* h_Edep_ECAL_VS_reco_P_nFD_matched_1e_cut = new TH2D(
        "Edep_ECAL_VS_reco_P_nFD_matched_1e_cut", "E^{ECAL}_{dep} vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];E^{ECAL}_{dep} [GeV]", 100, 0., Ebeam * 3., 100, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_VS_reco_P_nFD_matched_1e_cut);
    TH2D* h_Edep_ECAL_VS_reco_dPhi_nFD_e_matched_1e_cut =
        new TH2D("Edep_ECAL_VS_reco_dPhi_nFD_e_matched_1e_cut",
                 "E^{ECAL}_{dep} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];E^{ECAL}_{dep} [GeV]", 100,
                 -180., 180., 100, 0., 0.5);
    HistoList.push_back(h_Edep_ECAL_VS_reco_dPhi_nFD_e_matched_1e_cut);

    TH2D* h_beta_n_VS_reco_P_nFD_matched_1e_cut =
        new TH2D("beta_n_VS_reco_P_nFD_matched_1e_cut", "#beta_{nFD} vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];#beta_{nFD}", 100, 0., Ebeam * 3., 100, 0.3, 1.1);
    HistoList.push_back(h_beta_n_VS_reco_P_nFD_matched_1e_cut);
    TH2D* h_beta_n_VS_reco_dPhi_nFD_e_matched_1e_cut =
        new TH2D("beta_n_VS_reco_dPhi_nFD_e_matched_1e_cut",
                 "#beta_{nFD} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (matched);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];#beta_{nFD}", 100, -180., 180.,
                 100, 0.3, 1.1);
    HistoList.push_back(h_beta_n_VS_reco_dPhi_nFD_e_matched_1e_cut);

    /*
        TH2D* h_Edep_ECAL_VS_reco_P_LnFD_matched_1e_cut = new TH2D(
            "Edep_ECAL_VS_reco_P_LnFD_matched_1e_cut", "E^{ECAL}_{dep} vs. P^{reco}_{LnFD} in 1e cut (matched);P^{reco}_{LnFD} [GeV/c];E^{ECAL}_{dep} [GeV]", 100, 0., Ebeam * 3., 100, 0.,
       0.5); HistoList.push_back(h_Edep_ECAL_VS_reco_P_LnFD_matched_1e_cut); TH2D* h_Edep_ECAL_VS_reco_dPhi_LnFD_e_matched_1e_cut = new TH2D("Edep_ECAL_VS_reco_dPhi_LnFD_e_matched_1e_cut",
                     "E^{ECAL}_{dep} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                     "[#circ]];E^{ECAL}_{dep} [GeV]",
                     100, -180., 180., 100, 0., 0.5);
        HistoList.push_back(h_Edep_ECAL_VS_reco_dPhi_LnFD_e_matched_1e_cut);

        TH2D* h_beta_n_VS_reco_P_LnFD_matched_1e_cut =
            new TH2D("beta_n_VS_reco_P_LnFD_matched_1e_cut", "#beta_{LnFD} vs. P^{reco}_{LnFD} in 1e cut (matched);P^{reco}_{LnFD} [GeV/c];#beta_{LnFD}", 100, 0., Ebeam * 3., 100, 0.3, 1.1);
        HistoList.push_back(h_beta_n_VS_reco_P_LnFD_matched_1e_cut);
        TH2D* h_beta_n_VS_reco_dPhi_LnFD_e_matched_1e_cut =
            new TH2D("beta_n_VS_reco_dPhi_LnFD_e_matched_1e_cut",
                     "#beta_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (matched);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]];#beta_{LnFD}", 100, -180.,
                     180., 100, 0.3, 1.1);
        HistoList.push_back(h_beta_n_VS_reco_dPhi_LnFD_e_matched_1e_cut);
     */

    cout << "\033[33m\n\nFinished setting up matched histograms.\n\033[0m" << std::flush;

#pragma endregion

#pragma endregion

#pragma region /* Prepare AMaps */

    std::cout << "\033[33m\n\nPrepareing AMAps...\n\033[0m" << std::flush;

    const std::string OutputDirAMapsPlots = OutputDir + "/01_AMaps_Plots/";
    system(("rm -rf " + OutputDirAMapsPlots).c_str());
    system(("mkdir -p " + OutputDirAMapsPlots).c_str());

    const std::string OutputDirAMapsMaps = OutputDir + "/02_AMaps_Maps/";
    system(("rm -rf " + OutputDirAMapsMaps).c_str());
    system(("mkdir -p " + OutputDirAMapsMaps).c_str());

    const std::string P_e_bin_profile = "uniform_P_e_bins";      // {reformat_e_bins , varying_P_e_bins , uniform_P_e_bins, equi_inverted_P_e}
    const std::string P_nuc_bin_profile = "uniform_P_nuc_bins";  // {equi_inverted_P_nuc , varying_P_nuc_bins , uniform_P_nuc_bins}

    // int NumberNucOfMomSlices = 25, NumberElecOfMomSlices = 60, HistElectronSliceNumOfXBins = 120, HistNucSliceNumOfXBins = 150;
    int NumberNucOfMomSlices = 25, NumberElecOfMomSlices = 60, HistElectronSliceNumOfXBins = 120, HistNucSliceNumOfXBins = 100;
    // int NumberNucOfMomSlices = 15, NumberElecOfMomSlices = 15, HistElectronSliceNumOfXBins = 100, HistNucSliceNumOfXBins = 100;

    AMaps aMaps_generator;
    aMaps_generator = AMaps(SampleName, P_e_bin_profile, P_nuc_bin_profile, Ebeam, "AMaps", OutputDirAMapsPlots, NumberNucOfMomSlices, NumberElecOfMomSlices, HistNucSliceNumOfXBins,
                            HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins, HistElectronSliceNumOfXBins);

    // FD & CD theta range:
    DSCuts ThetaFD = DSCuts("Theta FD", "FD", "", "", 1, 5., 40.);
    DSCuts ThetaCD = DSCuts("Theta CD", "CD", "", "", 1, 40., 135.);

    AMaps aMaps_FCapplier = AMaps(path_definitions::PathDefinitions.AcceptanceMapsDirectory, SampleName, Ebeam, "AMaps", /* AMapsSettings.Electron_single_slice_test= */ false,
                                  /* AMapsSettings.Nucleon_single_slice_test= */ false, /* AMapsSettings.TestSlices= */ {1, 1, 1});
    // aMaps_FCapplier = AMaps(path_definitions::PathDefinitions.AcceptanceMapsDirectory, SampleName, /* AMapsSettings.Electron_single_slice_test= */ false,
    // /* AMapsSettings.Nucleon_single_slice_test= */ false, /* AMapsSettings.TestSlices= */ {1, 1, 1});

#pragma endregion

#pragma region /* Prepare nRes */

    std::cout << "\033[33m\n\nPrepareing momentum resolution elements...\n\033[0m" << std::flush;

    const std::string OutputDirMomResPlots = OutputDir + "/03_MomRes_Plots/";
    system(("rm -rf " + OutputDirMomResPlots).c_str());
    system(("mkdir -p " + OutputDirMomResPlots).c_str());

    // Neutron resolution class declaration & definition
    MomentumResolution nRes("Neutron");

    DSCuts FD_nucleon_theta_cut = DSCuts("FD Nucleon theta cut", "FD", "", "", 0, 0, 32.);
    DSCuts n_mom_th = DSCuts("Momentum_th", "FD", "Neutrons", "", 0, 0.4, 9999);
    DSCuts FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "Protons and neutrons", 0, 1., 2.5);  // E4nu meeting (29/01/24)

    DSCuts MomRes_mu_cuts = DSCuts("MomRes_mu_cuts", "FD", "", "Protons and neutrons", 0, FD_nucleon_momentum_cut.GetLowerCut(), 2.2);        // E4nu meeting (29/01/24)
    DSCuts MomRes_sigma_cuts = DSCuts("MomRes_sigma_cuts", "FD", "", "Protons and neutrons", 0, FD_nucleon_momentum_cut.GetLowerCut(), 2.2);  // Adi meeting after (29/01/24)

    // nRes.MomResInit(plot_and_fit_MomRes, false, false, SampleName, path_definitions::PathDefinitions.NucleonCutsDirectory, Ebeam, MomRes_mu_cuts, MomRes_sigma_cuts, 0.4,
    //                 path_definitions::PathDefinitions.MomentumResolutionDirectory, OutputDirMomResPlots, DeltaSlices, false, "pol1_wKC", "pol1_wKC", true);

#pragma endregion

#pragma region /* Prepare clas12ana */

    std::cout << "\033[33m\n\nPrepareing clas12ana...\n\033[0m" << std::flush;

    clas12ana clasAna;

    clasAna.readInputParam((path_definitions::PathDefinitions.PIDCutsDirectory + "ana.par").c_str());

    clasAna.readEcalSFPar((path_definitions::PathDefinitions.PIDCutsDirectory + "paramsSF_40Ca_x2.dat").c_str());
    clasAna.setEcalSFCuts();

    clasAna.readEcalPPar((path_definitions::PathDefinitions.PIDCutsDirectory + "paramsPI_40Ca_x2.dat").c_str());
    clasAna.setEcalPCuts();
    clasAna.setEcalEdgeCuts();

    clasAna.setNpheCuts();

    // clasAna.readInputParam((path_definitions::PathDefinitions.PIDCutsDirectory + "Fitted_PID_Cuts_-_" + SampleName + ".par")
    //    .c_str());  // load sample-appropreate cuts file from CutsDirectory
    clasAna.setPidCuts();  // making f_pidCuts = true

    // clasAna.set_e_VertexCuts();                                                                   // making f_e_vertexCuts = true
    // clasAna.setVertexCuts();                                                                      // making f_vertexCuts = true
    // clasAna.setVzcuts(CutManager.Vz_cut.GetLowerCut(), CutManager.Vz_cut.GetUpperCut());          // setting Vz cuts for all (charged?) particles
    // clasAna.setVzcutsFD(CutManager.Vz_cut_FD.GetLowerCut(), CutManager.Vz_cut_FD.GetUpperCut());  // setting Vz cuts for all charged particles (FD only)
    // clasAna.setVzcutsCD(CutManager.Vz_cut_CD.GetLowerCut(), CutManager.Vz_cut_CD.GetUpperCut());  // setting Vz cuts for all charged particles (CD only)

    clasAna.set_e_DCEdgeCuts();
    clasAna.setDCEdgeCuts();

    clasAna.setVertexCorrCuts();  // making f_corr_vertexCuts = true
    // clasAna.setVertexCorrCutsLim(CutManager.dVz_cuts.GetLowerCut(), CutManager.dVz_cuts.GetUpperCut());          // setting dVz cuts (general)
    // clasAna.setVertexCorrCutsLimFD(CutManager.dVz_cuts_FD.GetLowerCut(), CutManager.dVz_cuts_FD.GetUpperCut());  // setting dVz cuts (FD only)
    // clasAna.setVertexCorrCutsLimCD(CutManager.dVz_cuts_CD.GetLowerCut(), CutManager.dVz_cuts_CD.GetUpperCut());  // setting dVz cuts (CD only)

    clasAna.readInputParam(
        (path_definitions::PathDefinitions.NucleonCutsDirectory + "Nucleon_Cuts_-_" + SampleName + ".par").c_str());  // load sample-appropreate cuts file from CutsDirectory

    clasAna.printParams();

#pragma endregion

#pragma region /* Loop over events */

    std::cout << "\033[33m\n\nLooping over events...\n\033[0m" << std::flush;

    int counter = 0;

    while (chain.Next() == true) {
        // Display completed
        ++counter;
        if ((counter % 1000000) == 0) { std::cerr << "\n" << counter / 1000000 << " million completed"; }
        if ((counter % 100000) == 0) { std::cerr << "."; }

        if (counter > Limiter) { break; }

        clasAna.Run(c12);

        double weight = 1;

        //  =======================================================================================================================================================================
        //  Setup truth particles bank and filling RawTruth
        //  =======================================================================================================================================================================

        auto c12 = chain.GetC12Reader();
        auto mceve = c12->mcevent();
        auto mcpbank = c12->mcparts();
        const Int_t Ngen = mcpbank->getRows();

        for (Int_t i = 0; i < Ngen; i++) {
            mcpbank->setEntry(i);

            auto pid_temp = mcpbank->getPid();

            auto Truth_P_temp = mcpbank->getP();
            auto Truth_theta_temp = mcpbank->getTheta() * 180 / M_PI;
            auto Truth_phi_temp = mcpbank->getPhi() * 180 / M_PI;

            if (pid_temp == 11) {
                h_truth_P_e_RawTruth_1e_cut->Fill(Truth_P_temp, weight);
                h_truth_theta_e_RawTruth_1e_cut->Fill(Truth_theta_temp, weight);
                h_truth_phi_e_RawTruth_1e_cut->Fill(Truth_phi_temp, weight);
                h_truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut->Fill(Truth_phi_temp, Truth_theta_temp, weight);
                h_truth_theta_e_VS_truth_P_e_RawTruth_1e_cut->Fill(Truth_P_temp, Truth_theta_temp, weight);
            } else if (pid_temp == 2112) {
                h_truth_P_nFD_RawTruth_1e_cut->Fill(Truth_P_temp, weight);
                h_truth_theta_nFD_RawTruth_1e_cut->Fill(Truth_theta_temp, weight);
                h_truth_phi_nFD_RawTruth_1e_cut->Fill(Truth_phi_temp, weight);
                h_truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut->Fill(Truth_phi_temp, Truth_theta_temp, weight);
                h_truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut->Fill(Truth_P_temp, Truth_theta_temp, weight);
            }
        }

        //  =======================================================================================================================================================================
        //  Setup reco particles and applying 1e cut
        //  =======================================================================================================================================================================

        // Particles from the clas12reader
        auto allParticles_det = c12->getDetParticles();
        auto electrons_det = c12->getByID(11);   // Electrons
        auto protons_det = c12->getByID(2212);   // Protons
        auto positrons_det = c12->getByID(-11);  // Positrons
        auto piplus_det = c12->getByID(211);     // pi+
        auto piminus_det = c12->getByID(-211);   // pi-
        auto Kplus_det = c12->getByID(321);      // K+
        auto Kminus_det = c12->getByID(-321);    // K-
        auto deuterons_det = c12->getByID(45);   // Deuterons
        auto other_part_det = c12->getByID(0);   // Unidentified particles

        // Particles from clas12ana
        auto allParticles = clasAna.getParticles();  // All particles after applying clas12ana cuts
        auto electrons = clasAna.getByPid(11);       // Electrons
        auto positrons = clasAna.getByPid(-11);      // Positrons
        auto protons = clasAna.getByPid(2212);       // Protons
        auto piplus = clasAna.getByPid(211);         // pi+
        auto piminus = clasAna.getByPid(-211);       // pi-
        auto Kplus = clasAna.getByPid(321);          // K+
        auto Kminus = clasAna.getByPid(-321);        // K-
        auto deuterons = clasAna.getByPid(45);       // Deuterons
        auto neutrals = clasAna.getByPid(0);         // Unidentified
        auto otherpart = clasAna.getByPid(311);      // Other particles

        std::vector<int> IDed_Protons_ind = pid.ChargedParticleID(protons, pCD_mom_th, pFD_mom_th);  // indices of identified protons (i.e., within P_p th.)
        std::vector<int> Piplus_ind = pid.ChargedParticleID(piplus, pipCD_mom_th, pipFD_mom_th);     // indices of identified pi+ (i.e., within P_pi+ th.)
        std::vector<int> Piminus_ind = pid.ChargedParticleID(piminus, pimCD_mom_th, pimFD_mom_th);   // indices of identified pi- (i.e., within P_pi- th.)

        if (electrons.size() != 1) { continue; }  // Apply 1e cut

        if (apply_pos_multi_cut && positrons.size() > 0) { continue; }         // Apply positrons multiplicity cut
        if (apply_p_multi_cut && IDed_Protons_ind.size() > 0) { continue; }    // Apply proton multiplicity cut
        if (apply_pip_multi_cut && piplus.size() > 0) { continue; }            // Apply pi+ multiplicity cut
        if (apply_pim_multi_cut && piminus.size() > 0) { continue; }           // Apply pi- multiplicity cut
        if (apply_Kp_multi_cut && Kplus.size() > 0) { continue; }              // Apply K+ multiplicity cut
        if (apply_Km_multi_cut && Kminus.size() > 0) { continue; }             // Apply K- multiplicity cut
        if (apply_D_multi_cut && deuterons.size() > 0) { continue; }           // Apply D multiplicity cut
        if (apply_other_part_multi_cut && otherpart.size() > 0) { continue; }  // Apply other part multiplicity cut

        double starttime = c12->event()->getStartTime();

        TVector3 P_b(0, 0, Ebeam);

        TVector3 reco_P_e;
        reco_P_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());

        double vtz_e = electrons[0]->par()->getVz();

        TVector3 P_q = P_b - reco_P_e;

        double nu = Ebeam - reco_P_e.Mag();
        double QSq = P_q.Mag2() - (nu * nu);
        double xB = QSq / (2 * m_n * nu);
        double WSq = (m_n * m_n) - QSq + (2 * nu * m_n);
        double theta_e = reco_P_e.Theta() * 180 / M_PI;
        double EoP_e = (electrons[0]->cal(clas12::PCAL)->getEnergy() + electrons[0]->cal(clas12::ECIN)->getEnergy() + electrons[0]->cal(clas12::ECOUT)->getEnergy()) / reco_P_e.Mag();
        double E_PCALoP_e = electrons[0]->cal(clas12::PCAL)->getEnergy() / reco_P_e.Mag();
        double E_ECINoP_e = electrons[0]->cal(clas12::ECIN)->getEnergy() / reco_P_e.Mag();
        double Edep_PCAL = electrons[0]->cal(clas12::PCAL)->getEnergy();
        double Edep_EC = electrons[0]->cal(clas12::ECIN)->getEnergy() + electrons[0]->cal(clas12::ECOUT)->getEnergy();

        auto [ElectronInPCAL, ElectronInECIN, ElectronInECOUT, electron_ECAL_detlayer] = raf::CheckForECALHits(electrons[0]);
        auto Electron_ECAL_detlayer = ElectronInPCAL ? clas12::PCAL : ElectronInECIN ? clas12::ECIN : clas12::ECOUT;

#pragma region /* A helper lambda to build the FD photons and the ECAL/matched FD neutrons */

        //  A helper lambda to build the FD photons and the ECAL/matched FD neutrons
        //  ----------------------------------------------------------------------------------------------------------------------------------------

        auto Build_FD_neutrals = [&]() {
            double Reco_P_LnFD_mag_local = -1;  // Reco leading neutron momentum
            int Reco_P_LnFD_ind_local = -1;     // Reco leading neutron index

            std::vector<region_part_ptr> Local_reco_nFD;           // All neutrons
            std::vector<region_part_ptr> Local_reco_nFD_OnlyGood;  // Only good neutrons
            std::vector<region_part_ptr> Local_reco_nFD_OnlyBad;   // Only Bad neutrons
            region_part_ptr Local_reco_LnFD;                       // Leading neutron
            std::vector<region_part_ptr> Local_gammaFD;
            std::vector<int> Local_reco_nFD_ind;

            // Local_reco_nFD = NewNeutronFDVeto(allParticles);

            // std::vector<int> Temp_local_reco_nFD_ind;
            // std::vector<region_part_ptr> Temp_local_reco_nFD = NewNeutronFDVeto(allParticles, Temp_local_reco_nFD_ind);

            // for (int i = 0; i < Temp_local_reco_nFD.size(); i++) {
            //     auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, Neutron_ECAL_detlayer] = raf::CheckForECALHits(Temp_local_reco_nFD[i]);

            //     double Momentum = raf::CalcPnFD(Temp_local_reco_nFD[i], electrons[0], starttime, apply_neutFD_redef);

            //     bool PassMomTh = (Momentum >= nFD_mom_th.GetLowerCut());
            //     // bool PassMomTh = true;
            //     bool PassECALeadgeCuts = (Temp_local_reco_nFD[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && Temp_local_reco_nFD[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);

            //     bool PassCVeto = NeutronECAL_Cut_Veto_original_custom(allParticles, electrons, Ebeam, Temp_local_reco_nFD[i], cPart_veto_radius);
            //     bool PassNVeto = NeutronECAL_Cut_NeutVeto_custom(allParticles, electrons, Ebeam, Temp_local_reco_nFD[i], nPart_veto_radius);

            //     if (PassMomTh && PassECALeadgeCuts && (!apply_cPart_ECAL_veto || PassCVeto) && (!apply_nPart_ECAL_veto || PassNVeto)) {
            //         Local_reco_nFD.push_back(Temp_local_reco_nFD[i]);
            //         Local_reco_nFD_ind.push_back(Temp_local_reco_nFD_ind[i]);
            //     }
            // }

            //
            //
            //
            //

            for (int i = 0; i < allParticles.size(); i++) {
                int pid_temp = allParticles[i]->par()->getPid();

                if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)) {  // neutral in FD
                    auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, Neutron_ECAL_detlayer] = raf::CheckForECALHits(allParticles[i]);

                    /*
                    // Unify FD neutrals:
                    if ((pid_temp == 2112) || (pid_temp == 22)) {  // Neutron or photon ID in event builder
                        double Momentum = raf::CalcPnFD(allParticles[i], electrons[0], starttime, apply_neutFD_redef);

                        double Path_nFD = raf::CalcPathnFD(allParticles[i], electrons[0],apply_neutFD_redef);
                        double reco_ToF_nFD = raf::CalcToFnFD(allParticles[i], starttime,apply_neutFD_redef);

                        bool PassMomTh = (Momentum >= nFD_mom_th.GetLowerCut());
                        bool PassECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);

                        // The vetoes have been removed when we're unifing FD neutrals, as the charged ECAL veto removes nFDs from the PCAL
                        bool PassCVeto = true;
                        // bool PassCVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);
                        bool PassNVeto = true;
                        // bool PassNVeto = NeutronECAL_Cut_NeutVeto(allParticles, electrons, Ebeam, i, nPart_veto_radius);

                        if (PassMomTh && PassECALeadgeCuts && (!apply_cPart_ECAL_veto || PassCVeto) && (!apply_nPart_ECAL_veto || PassNVeto)) {
                            Local_reco_nFD.push_back(allParticles[i]);
                            Local_reco_nFD_ind.push_back(i);

                            int nFD_nSector = allParticles[i]->cal(Neutron_ECAL_detlayer)->getSector();
                            int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();

                            if (abs(nFD_nSector - e_nSector) == 3) {
                                Local_reco_nFD_OnlyGood.push_back(allParticles[i]);
                            } else {
                                Local_reco_nFD_OnlyBad.push_back(allParticles[i]);
                            }
                        }
                    }
                    */

                    // Use redefined neutron and photon selection:
                    if (apply_neutFD_redef) {
                        if ((pid_temp == 2112) || (pid_temp == 22)) {  // Neutron or photon ID in event builder
                            if (ParticleInPCAL) {                      // A PCAL hit
                                if (pid_temp == 22) {                  // Particle is a photon
                                    double Momentum = allParticles[i]->par()->getP();
                                    bool PassMomTh = (Momentum >= phFD_mom_th.GetLowerCut());

                                    if (PassMomTh) { Local_gammaFD.push_back(allParticles[i]); }
                                }
                            } else {                                      // no PCAL hit
                                if (ParticleInECIN || ParticleInECOUT) {  // Either ECIN or ECOUT hit, pid_temp = 2112 or pid_temp = 22
                                    double Momentum = raf::CalcPnFD(allParticles[i], electrons[0], starttime, apply_neutFD_redef);

                                    double Path_nFD = raf::CalcPathnFD(allParticles[i], electrons[0], apply_neutFD_redef);
                                    double reco_ToF_nFD = raf::CalcToFnFD(allParticles[i], starttime, apply_neutFD_redef);

                                    bool PassMomTh = (Momentum >= nFD_mom_th.GetLowerCut());
                                    bool PassECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);

                                    bool PassCVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);
                                    bool PassNVeto = NeutronECAL_Cut_NeutVeto(allParticles, electrons, Ebeam, i, nPart_veto_radius);

                                    if (PassMomTh && PassECALeadgeCuts && (!apply_cPart_ECAL_veto || PassCVeto) && (!apply_nPart_ECAL_veto || PassNVeto)) {
                                        Local_reco_nFD.push_back(allParticles[i]);
                                        Local_reco_nFD_ind.push_back(i);

                                        int nFD_nSector = allParticles[i]->cal(Neutron_ECAL_detlayer)->getSector();
                                        int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();
                                        if (abs(nFD_nSector - e_nSector) == 3) {
                                            Local_reco_nFD_OnlyGood.push_back(allParticles[i]);
                                        } else {
                                            Local_reco_nFD_OnlyBad.push_back(allParticles[i]);
                                        }
                                    }
                                }
                            }
                        }
                    } else {  // Use clas12root neutron and photon selection
                        if (pid_temp == 2112) {
                            // double Momentum = allParticles[i]->par()->getP();
                            double Momentum = raf::CalcPnFD(allParticles[i], electrons[0], starttime, apply_neutFD_redef);

                            bool PassMomTh = (Momentum >= nFD_mom_th.GetLowerCut());
                            bool PassECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);

                            // The vetoes have been removed when we're not redfining FD neutrals, as the charged ECAL veto does that internally
                            bool PassCVeto = true;
                            // bool PassCVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);
                            bool PassNVeto = true;
                            // bool PassNVeto = NeutronECAL_Cut_NeutVeto(allParticles, electrons, Ebeam, i, nPart_veto_radius);

                            if (PassMomTh && PassECALeadgeCuts && (!apply_cPart_ECAL_veto || PassCVeto) && (!apply_nPart_ECAL_veto || PassNVeto)) {
                                Local_reco_nFD.push_back(allParticles[i]);
                                Local_reco_nFD_ind.push_back(i);

                                int nFD_nSector = allParticles[i]->cal(Neutron_ECAL_detlayer)->getSector();
                                int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();

                                if (abs(nFD_nSector - e_nSector) == 3) {
                                    Local_reco_nFD_OnlyGood.push_back(allParticles[i]);
                                } else {
                                    Local_reco_nFD_OnlyBad.push_back(allParticles[i]);
                                }
                            }
                        } else if (pid_temp == 22) {
                            double Momentum = allParticles[i]->par()->getP();

                            bool PassMomTh = (Momentum >= phFD_mom_th.GetLowerCut());

                            if (PassMomTh) { Local_gammaFD.push_back(allParticles[i]); }
                        }
                    }
                }
            }

            // Find leading neutron by momentum:
            for (size_t k = 0; k < Local_reco_nFD_ind.size(); ++k) {
                int idx = Local_reco_nFD_ind[k];

                double Momentum = raf::CalcPnFD(allParticles[idx], electrons[0], starttime, apply_neutFD_redef);

                if (Momentum >= Reco_P_LnFD_mag_local) {
                    Reco_P_LnFD_mag_local = Momentum;
                    Reco_P_LnFD_ind_local = idx;  // global index
                }
            }

            Local_reco_LnFD = (Reco_P_LnFD_ind_local != -1) ? allParticles[Reco_P_LnFD_ind_local] : nullptr;

            return std::make_tuple(Reco_P_LnFD_mag_local, Reco_P_LnFD_ind_local, std::move(Local_reco_nFD), std::move(Local_reco_nFD_OnlyGood), std::move(Local_reco_nFD_OnlyBad),
                                   std::move(Local_reco_LnFD), std::move(Local_gammaFD), std::move(Local_reco_nFD_ind));
        };

#pragma endregion

        //  =======================================================================================================================================================================
        //  Get truth electron variables under 1e cut for ConstrainedE (CE)
        //  =======================================================================================================================================================================

        double Truth_theta_e_CE_1e_cut, Truth_phi_e_CE_1e_cut, Truth_phi_nFD_CE_1e_cut;

        for (Int_t i = 0; i < Ngen; i++) {
            mcpbank->setEntry(i);

            auto pid_temp = mcpbank->getPid();

            auto Truth_theta_temp = mcpbank->getTheta() * 180 / M_PI;
            auto Truth_phi_temp = mcpbank->getPhi() * 180 / M_PI;

            if (pid_temp == 11) {
                Truth_theta_e_CE_1e_cut = Truth_theta_temp, Truth_phi_e_CE_1e_cut = Truth_phi_temp;
            } else if (pid_temp == 2112) {
                Truth_phi_nFD_CE_1e_cut = Truth_phi_temp;
            }
        }

        //  =======================================================================================================================================================================
        //  1e cut (reco)
        //  =======================================================================================================================================================================

#pragma region /* 1e cut (reco) */

#pragma region /* 1e cut (reco) - charged hadrons */

        //  Electron PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Electron PID cuts */

        // TODO: ALL of these plots are being filled after cuts are applied. Find a way to fill them before cuts are applied too.

        h_Vz_e_BC_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
        bool bad_Vz_e_CutCond = (electrons[0]->par()->getVz() < -1.5 || electrons[0]->par()->getVz() > 0.5);
        // bool bad_Vz_e_CutCond = (electrons[0]->par()->getVz() < -4. || electrons[0]->par()->getVz() > -2.);
        if (!bad_Vz_e_CutCond) { h_Vz_e_AC_1e_cut->Fill(electrons[0]->par()->getVz(), weight); }

        fillDCdebug(electrons[0], h_dc_electron_hit_map_BC_1e_cut, weight);
        bool bad_DC_edge_CutCond = (!raf::DCEdgeCuts(electrons[0]));
        if (!bad_DC_edge_CutCond) { raf::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_1e_cut, weight); }

        h_nphe_BC_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);
        bool bad_nphe_CutCond = (electrons[0]->che(clas12::HTCC)->getNphe() <= 2);
        if (!bad_nphe_CutCond) { h_nphe_AC_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight); }

        h_Edep_PCAL_VS_EC_BC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);
        bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
        if (!bad_Edep_PCAL_CutCond) { h_Edep_PCAL_VS_EC_AC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight); }

        h_SF_VS_P_e_BC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);
        bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
        if (!bad_SF_CutCond) { h_SF_VS_P_e_AC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight); }

        h_SF_VS_Lv_BC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
        h_SF_VS_Lw_BC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
        h_SF_VS_Lu_BC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
        bool bad_PCAL_edge_CutCond = (electrons[0]->cal(clas12::PCAL)->getLv() < 14. || electrons[0]->cal(clas12::PCAL)->getLw() < 14.);
        if (!bad_PCAL_edge_CutCond) {
            h_SF_VS_Lv_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
        }

        h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
        bool bad_diag_CutCond = (!raf::checkEcalDiagCuts(electrons[0]));
        if (!bad_diag_CutCond) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight); }

        if (bad_Vz_e_CutCond) { continue; }
        // if (bad_DC_edge_CutCond) { continue; }

        // if (bad_nphe_CutCond) { continue; }
        // if (bad_Edep_PCAL_CutCond) { continue; }
        // if (bad_SF_CutCond) { continue; }
        // if (bad_PCAL_edge_CutCond) { continue; }
        // if (bad_diag_CutCond) { continue; }

        // Force the electron to be in the expected region of phase space:
        if (ConstrainedE && (fabs(reco_P_e.Mag() - Ebeam) > 0.01 * Ebeam * 2)) { continue; }  // The resolution (sigma/P) is less than 1%, so I take twice that
        if (ConstrainedE && (fabs((reco_P_e.Theta() * 180 / M_PI) - Truth_theta_e_CE_1e_cut) > 2.)) { continue; }
        if (ConstrainedE && (fabs((reco_P_e.Phi() * 180 / M_PI) - Truth_phi_e_CE_1e_cut) > 5.)) { continue; }
        if (ConstrainedE && (am::CalcdPhi1(fabs(GetPhi_e(Beam_energy_TString, Truth_phi_nFD_CE_1e_cut) - (reco_P_e.Phi() * 180 / M_PI))) > 5.)) { continue; }

        h_reco_P_e_1e_cut->Fill(reco_P_e.Mag(), weight);
        h_reco_theta_e_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
        h_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
        h_reco_theta_e_VS_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);

#pragma endregion

        //  Proton PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Proton PID cuts */

        for (auto& i : IDed_Protons_ind) {
            // loop over protons vector
            if (protons[i]->getRegion() == FD) {
                h_Vz_pFD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight);
                // bool bad_Vz_pFD_CutCond = (protons[i]->par()->getVz() < -4. || protons[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pFD_CutCond) { h_Vz_pFD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight); }

                h_dVz_pFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight);
                // bool bad_dVz_pFD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pFD_CutCond) { h_Vz_pFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pFD_AC_1e_cut->Fill(protons[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pFD_CutCond = (protons[i]->par()->getVz() < -4. || protons[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pFD_CutCond) { h_Vz_pFD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight); }

                fillDCdebug(protons[i], h_dc_pFD_hit_map_AC_1e_cut, weight);
                // bool bad_DC_edge_CutCond = (!DCEdgeCuts(protons[i]));
                // if (!bad_DC_edge_CutCond) { fillDCdebug(protons[i], h_dc_electron_hit_map_AC_1e_cut, weight); }
            } else if (protons[i]->getRegion() == CD) {
                h_Vz_pCD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight);
                // bool bad_Vz_pCD_CutCond = (protons[i]->par()->getVz() < -4. || protons[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pCD_CutCond) { h_Vz_pCD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight); }

                h_dVz_pCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight);
                // bool bad_dVz_pCD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pCD_CutCond) { h_Vz_pCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pCD_AC_1e_cut->Fill(protons[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pCD_CutCond = (protons[i]->par()->getVz() < -4. || protons[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pCD_CutCond) { h_Vz_pCD_AC_1e_cut->Fill(protons[i]->par()->getVz(), weight); }
            }

        }  // end of ToF loop over protons vector

#pragma endregion

        //  Piplus PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Piplus PID cuts */

        for (auto& i : Piplus_ind) {
            // loop over piplus vector
            if (piplus[i]->getRegion() == FD) {
                h_Vz_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // bool bad_Vz_pipFD_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pipFD_CutCond) { h_Vz_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight); }

                h_dVz_pipFD_AC_1e_cut->Fill(vtz_e - piplus[i]->par()->getVz(), weight);
                // bool bad_dVz_pipFD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pipFD_CutCond) { h_Vz_pipFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pipFD_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pipFD_CutCond) { h_Vz_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight); }

                fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_1e_cut, weight);
                // bool bad_DC_edge_CutCond = (!DCEdgeCuts(piplus[i]));
                // if (!bad_DC_edge_CutCond) { fillDCdebug(piplus[i], h_dc_electron_hit_map_AC_1e_cut, weight); }
            } else if (piplus[i]->getRegion() == CD) {
                h_Vz_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // bool bad_Vz_pipCD_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pipCD_CutCond) { h_Vz_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight); }

                h_dVz_pipCD_AC_1e_cut->Fill(vtz_e - piplus[i]->par()->getVz(), weight);
                // bool bad_dVz_pipCD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pipCD_CutCond) { h_Vz_pipCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pipCD_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pipCD_CutCond) { h_Vz_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight); }
            }

        }  // end of ToF loop over piplus vector

#pragma endregion

        //  Piminus PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Piminus PID cuts */

        for (auto& i : Piminus_ind) {
            // loop over piminus vector
            if (piminus[i]->getRegion() == FD) {
                h_Vz_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // bool bad_Vz_pimFD_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pimFD_CutCond) { h_Vz_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight); }

                h_dVz_pimFD_AC_1e_cut->Fill(vtz_e - piminus[i]->par()->getVz(), weight);
                // bool bad_dVz_pimFD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pimFD_CutCond) { h_Vz_pimFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pimFD_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pimFD_CutCond) { h_Vz_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight); }

                fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_1e_cut, weight);
                // bool bad_DC_edge_CutCond = (!DCEdgeCuts(piminus[i]));
                // if (!bad_DC_edge_CutCond) { fillDCdebug(piminus[i], h_dc_electron_hit_map_AC_1e_cut, weight); }
            } else if (piminus[i]->getRegion() == CD) {
                h_Vz_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // bool bad_Vz_pimCD_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pimCD_CutCond) { h_Vz_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight); }

                h_dVz_pimCD_AC_1e_cut->Fill(vtz_e - piminus[i]->par()->getVz(), weight);
                // bool bad_dVz_pimCD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
                // if (!bad_dVz_pimCD_CutCond) { h_Vz_pimCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

                h_chi2_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getChi2Pid(), weight);
                // bool bad_Vz_pimCD_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // if (!bad_Vz_pimCD_CutCond) { h_Vz_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight); }
            }

        }  // end of ToF loop over piminus vector

#pragma endregion

        //  Kaons and deuterons PID
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Kaons and deuterons PID */

        // #pragma region /* Kplus PID cuts */

        //                         for (int i = 0; i < Kplus.size(); i++) {
        //                             // loop over Kplus vector
        //                             if (Kplus[i]->getRegion() == FD) {
        //                                 h_Vz_KpFD_AC_1e_cut->Fill(Kplus[i]->par()->getVz(), weight);
        //                                 // bool bad_Vz_KpFD_CutCond = (Kplus[i]->par()->getVz() < -4. || Kplus[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_KpFD_CutCond) { h_Vz_KpFD_AC_1e_cut->Fill(Kplus[i]->par()->getVz(), weight); }

        //                                 h_dVz_KpFD_AC_1e_cut->Fill(vtz_e - Kplus[i]->par()->getVz(), weight);
        //                                 // bool bad_dVz_KpFD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_dVz_KpFD_CutCond) { h_Vz_KpFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

        //                                 h_chi2_KpFD_AC_1e_cut->Fill(Kplus[i]->par()->getChi2Kd(), weight);
        //                                 // bool bad_Vz_KpFD_CutCond = (Kplus[i]->par()->getVz() < -4. || Kplus[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_KpFD_CutCond) { h_Vz_KpFD_AC_1e_cut->Fill(Kplus[i]->par()->getVz(), weight); }

        //                                 fillDCdebug(Kplus[i], h_dc_KpFD_hit_map_AC_1e_cut, weight);
        //                                 // bool bad_DC_edge_CutCond = (!DCEdgeCuts(Kplus[i]));
        //                                 // if (!bad_DC_edge_CutCond) { fillDCdebug(Kplus[i], h_dc_electron_hit_map_AC_1e_cut, weight); }
        //                             } else if (Kplus[i]->getRegion() == CD) {
        //                                 h_Vz_KpCD_AC_1e_cut->Fill(Kplus[i]->par()->getVz(), weight);
        //                                 // bool bad_Vz_KpCD_CutCond = (Kplus[i]->par()->getVz() < -4. || Kplus[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_KpCD_CutCond) { h_Vz_KpCD_AC_1e_cut->Fill(Kplus[i]->par()->getVz(), weight); }

        //                                 h_dVz_KpCD_AC_1e_cut->Fill(vtz_e - Kplus[i]->par()->getVz(), weight);
        //                                 // bool bad_dVz_KpCD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_dVz_KpCD_CutCond) { h_Vz_KpCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

        //                                 h_chi2_KpCD_AC_1e_cut->Fill(Kplus[i]->par()->getChi2Kd(), weight);
        //                                 // bool bad_Vz_KpCD_CutCond = (Kplus[i]->par()->getVz() < -4. || Kplus[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_KpCD_CutCond) { h_Vz_KpCD_AC_1e_cut->Fill(Kplus[i]->par()->getVz(), weight); }
        //                             }

        //                         }  // end of ToF loop over Kplus vector

        // #pragma endregion

        //                         //  Kminus PID cuts
        //                         //
        //                         -----------------------------------------------------------------------------------------------------------------------------------------------------

        // #pragma region /* Kminus PID cuts */

        //                         for (int i = 0; i < Kminus.size(); i++) {
        //                             // loop over Kminus vector
        //                             if (Kminus[i]->getRegion() == FD) {
        //                                 h_Vz_KmFD_AC_1e_cut->Fill(Kminus[i]->par()->getVz(), weight);
        //                                 // bool bad_Vz_KmFD_CutCond = (Kminus[i]->par()->getVz() < -4. || Kminus[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_KmFD_CutCond) { h_Vz_KmFD_AC_1e_cut->Fill(Kminus[i]->par()->getVz(), weight); }

        //                                 h_dVz_KmFD_AC_1e_cut->Fill(vtz_e - Kminus[i]->par()->getVz(), weight);
        //                                 // bool bad_dVz_KmFD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_dVz_KmFD_CutCond) { h_Vz_KmFD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

        //                                 h_chi2_KmFD_AC_1e_cut->Fill(Kminus[i]->par()->getChi2Kd(), weight);
        //                                 // bool bad_Vz_KmFD_CutCond = (Kminus[i]->par()->getVz() < -4. || Kminus[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_KmFD_CutCond) { h_Vz_KmFD_AC_1e_cut->Fill(Kminus[i]->par()->getVz(), weight); }

        //                                 fillDCdebug(Kminus[i], h_dc_KmFD_hit_map_AC_1e_cut, weight);
        //                                 // bool bad_DC_edge_CutCond = (!DCEdgeCuts(Kminus[i]));
        //                                 // if (!bad_DC_edge_CutCond) { fillDCdebug(Kminus[i], h_dc_electron_hit_map_AC_1e_cut, weight); }
        //                             } else if (Kminus[i]->getRegion() == CD) {
        //                                 h_Vz_KmCD_AC_1e_cut->Fill(Kminus[i]->par()->getVz(), weight);
        //                                 // bool bad_Vz_KmCD_CutCond = (Kminus[i]->par()->getVz() < -4. || Kminus[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_KmCD_CutCond) { h_Vz_KmCD_AC_1e_cut->Fill(Kminus[i]->par()->getVz(), weight); }

        //                                 h_dVz_KmCD_AC_1e_cut->Fill(vtz_e - Kminus[i]->par()->getVz(), weight);
        //                                 // bool bad_dVz_KmCD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_dVz_KmCD_CutCond) { h_Vz_KmCD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

        //                                 h_chi2_KmCD_AC_1e_cut->Fill(Kminus[i]->par()->getChi2Kd(), weight);
        //                                 // bool bad_Vz_KmCD_CutCond = (Kminus[i]->par()->getVz() < -4. || Kminus[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_KmCD_CutCond) { h_Vz_KmCD_AC_1e_cut->Fill(Kminus[i]->par()->getVz(), weight); }
        //                             }

        //                         }  // end of ToF loop over Kminus vector

        // #pragma endregion

        //                         //  D PID cuts
        //                         //
        //                         -----------------------------------------------------------------------------------------------------------------------------------------------------

        // #pragma region /* D PID cuts */

        //                         for (int i = 0; i < deuterons.size(); i++) {
        //                             // loop over deuterons vector
        //                             if (deuterons[i]->getRegion() == FD) {
        //                                 h_Vz_D_FD_AC_1e_cut->Fill(deuterons[i]->par()->getVz(), weight);
        //                                 // bool bad_Vz_D_FD_CutCond = (deuterons[i]->par()->getVz() < -4. || deuterons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_D_FD_CutCond) { h_Vz_D_FD_AC_1e_cut->Fill(deuterons[i]->par()->getVz(), weight); }

        //                                 h_dVz_D_FD_AC_1e_cut->Fill(vtz_e - deuterons[i]->par()->getVz(), weight);
        //                                 // bool bad_dVz_D_FD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_dVz_D_FD_CutCond) { h_Vz_D_FD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

        //                                 h_chi2_D_FD_AC_1e_cut->Fill(deuterons[i]->par()->getChi2Kd(), weight);
        //                                 // bool bad_Vz_D_FD_CutCond = (deuterons[i]->par()->getVz() < -4. || deuterons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_D_FD_CutCond) { h_Vz_D_FD_AC_1e_cut->Fill(deuterons[i]->par()->getVz(), weight); }

        //                                 fillDCdebug(deuterons[i], h_dc_D_FD_hit_map_AC_1e_cut, weight);
        //                                 // bool bad_DC_edge_CutCond = (!DCEdgeCuts(deuterons[i]));
        //                                 // if (!bad_DC_edge_CutCond) { fillDCdebug(deuterons[i], h_dc_electron_hit_map_AC_1e_cut, weight); }
        //                             } else if (deuterons[i]->getRegion() == CD) {
        //                                 h_Vz_D_CD_AC_1e_cut->Fill(deuterons[i]->par()->getVz(), weight);
        //                                 // bool bad_Vz_D_CD_CutCond = (deuterons[i]->par()->getVz() < -4. || deuterons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_D_CD_CutCond) { h_Vz_D_CD_AC_1e_cut->Fill(deuterons[i]->par()->getVz(), weight); }

        //                                 h_dVz_D_CD_AC_1e_cut->Fill(vtz_e - deuterons[i]->par()->getVz(), weight);
        //                                 // bool bad_dVz_D_CD_CutCond = (vtz_e - protons[i]->par()->getVz() < -4. || vtz_e - protons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_dVz_D_CD_CutCond) { h_Vz_D_CD_AC_1e_cut->Fill(vtz_e - protons[i]->par()->getVz(), weight); }

        //                                 h_chi2_D_CD_AC_1e_cut->Fill(deuterons[i]->par()->getChi2Kd(), weight);
        //                                 // bool bad_Vz_D_CD_CutCond = (deuterons[i]->par()->getVz() < -4. || deuterons[i]->par()->getVz() > -2.);
        //                                 // if (!bad_Vz_D_CD_CutCond) { h_Vz_D_CD_AC_1e_cut->Fill(deuterons[i]->par()->getVz(), weight); }
        //                             }

        //                         }  // end of ToF loop over deuterons vector

        // #pragma endregion

#pragma endregion

#pragma endregion

        //  =======================================================================================================================================================================
        //  1e cut (truth)
        //  =======================================================================================================================================================================

#pragma region /* 1e cut (truth) */

        double Truth_P_LnFD_mag = -1;  // Truth leading neutron momentum
        int Truth_P_LnFD_ind = -1;     // Truth leading neutron index

        bool TLpassCuts = true;  // TODO: figure out if this is needed

        vector<int> truth_NeutronsFD;

        for (Int_t i = 0; i < Ngen; i++) {
            mcpbank->setEntry(i);

            auto pid_temp = mcpbank->getPid();
            auto p = mcpbank->getP(), px = mcpbank->getPx(), py = mcpbank->getPy(), pz = mcpbank->getPz();

            bool PassMomTh = (p >= nFD_mom_th.GetLowerCut());

            if (ConstrainTLmom && (pid_temp == 2112 && p > 2.)) {
                TLpassCuts = false;
                continue;
            }

            if (pid_temp == 11) {
                TVector3 truth_P_e;
                truth_P_e.SetXYZ(px, py, pz);

                h_truth_P_e_1e_cut->Fill(truth_P_e.Mag(), weight);
                h_truth_theta_e_1e_cut->Fill(truth_P_e.Theta() * 180 / M_PI, weight);
                h_truth_phi_e_1e_cut->Fill(truth_P_e.Phi() * 180 / M_PI, weight);
                h_truth_theta_e_VS_truth_phi_e_1e_cut->Fill(truth_P_e.Phi() * 180 / M_PI, truth_P_e.Theta() * 180 / M_PI, weight);

                h_truth_P_e_ECALveto_1e_cut->Fill(truth_P_e.Mag(), weight);
                h_truth_theta_e_ECALveto_1e_cut->Fill(truth_P_e.Theta() * 180 / M_PI, weight);
                h_truth_phi_e_ECALveto_1e_cut->Fill(truth_P_e.Phi() * 180 / M_PI, weight);
                h_truth_theta_e_ECALveto_VS_truth_phi_e_ECALveto_1e_cut->Fill(truth_P_e.Phi() * 180 / M_PI, truth_P_e.Theta() * 180 / M_PI, weight);
            } else if (pid_temp == 2112) {
                TVector3 truth_P_n;
                truth_P_n.SetXYZ(px, py, pz);

                bool Truth_InFD =
                    aMaps_FCapplier.IsInFDQuery(/* (!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps || AMapsSettings.Generate_Nucleon_AMaps))= */ false, ThetaFD,
                                                "Neutron", truth_P_n.Mag(), truth_P_n.Theta() * 180.0 / pi, truth_P_n.Phi() * 180.0 / pi, false);

                h_truth_P_n_1e_cut->Fill(truth_P_n.Mag(), weight);
                h_truth_theta_n_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                h_truth_phi_n_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                h_truth_theta_n_VS_truth_phi_n_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);

                if ((truth_P_n.Theta() * 180 / M_PI >= 5.) && (truth_P_n.Theta() * 180 / M_PI <= 35.)) {
                    if (truth_P_n.Mag() >= Truth_P_LnFD_mag) {
                        Truth_P_LnFD_mag = truth_P_n.Mag();
                        Truth_P_LnFD_ind = i;
                    }

                    h_truth_P_nFD_clas12_1e_cut->Fill(truth_P_n.Mag(), weight);
                    h_truth_theta_nFD_clas12_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                    h_truth_phi_nFD_clas12_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                    h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);

                    h_truth_P_nFD_redef_1e_cut->Fill(truth_P_n.Mag(), weight);
                    h_truth_theta_nFD_redef_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                    h_truth_phi_nFD_redef_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                    h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);

                    if (PassMomTh) {
                        if (Truth_InFD) {
                            truth_NeutronsFD.push_back(i);

                            // Detection efficiency denominator histogram, common to all detection efficiency plots. Filled the truth P_nFD distribution that pass momentum and fiducial cuts
                            // to obtain the (e,e')n events, under the one reco electron cut.
                            h_truth_P_nFD_truth_1e_cut->Fill(truth_P_n.Mag(), weight);
                            h_truth_theta_nFD_truth_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                            h_truth_phi_nFD_truth_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                            h_truth_theta_nFD_VS_truth_phi_nFD_truth_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);
                            h_truth_theta_nFD_VS_truth_P_nFD_truth_1e_cut->Fill(truth_P_n.Mag(), truth_P_n.Theta() * 180 / M_PI, weight);

                            h_truth_P_nFD_ECALveto_1e_cut->Fill(truth_P_n.Mag(), weight);
                            h_truth_theta_nFD_ECALveto_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                            h_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                            h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);
                        }

                        // Fill acceptance efficiency denominator histograms with the truth P_nFD distribution that pass momentum and fiducial cuts to
                        // obtain the (e,e')n events, under the one reco electron cut.
                        aMaps_generator.hFillMaps("TL", "Neutron", truth_P_n.Mag(), truth_P_n.Theta() * 180 / M_PI, truth_P_n.Phi() * 180 / M_PI, weight);
                    }
                }
            }
        }

        if (ConstrainTLmom && !TLpassCuts) { continue; }

        if (truth_NeutronsFD.size() != 1) { continue; }

        // Fill leading FD neutron acceptance maps
        if ((Truth_P_LnFD_ind != -1) && (Truth_P_LnFD_mag > 0)) {
            mcpbank->setEntry(Truth_P_LnFD_ind);

            int particlePDGtmp = mcpbank->getPid();

            double Particle_TL_Momentum = RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
            double Particle_TL_Theta = acos((mcpbank->getPz()) / RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

            bool PassMomTh = (Particle_TL_Momentum >= nFD_mom_th.GetLowerCut());

            // if (PassMomTh) { aMaps_generator.hFillMaps("TL", "Neutron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, weight); }  // end of if id. TL leading neutron
        }

#pragma endregion

        //  Raw plots
        //  ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Raw plots */

        vector<region_part_ptr> neutrons_FD_raw;
        vector<region_part_ptr> photons_FD_raw;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();
            int Momentum = allParticles[i]->par()->getP();

            if (allParticles[i]->getRegion() == FD) {  // RAW neutrons and photons in FD
                if (pid_temp == 2112) {
                    if (Momentum >= nFD_mom_th.GetLowerCut()) { neutrons_FD_raw.push_back(allParticles[i]); }
                } else if (pid_temp == 22) {
                    if (Momentum >= phFD_mom_th.GetLowerCut()) { photons_FD_raw.push_back(allParticles[i]); }
                }
            }
        }

        if ((!apply_nFD_multi_cut || neutrons_FD_raw.size() == 1) && (!apply_no_FD_photons_cut || photons_FD_raw.size() == 0)) {
            for (int i = 0; i < neutrons_FD_raw.size(); i++) {
                auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, Neutron_ECAL_detlayer] = raf::CheckForECALHits(neutrons_FD_raw[i]);

                if (apply_no_same_sector_cuts && (neutrons_FD_raw[i]->cal(Neutron_ECAL_detlayer)->getSector() != electrons[0]->cal(Electron_ECAL_detlayer)->getSector())) { continue; }

                TVector3 reco_P_n;
                reco_P_n.SetMagThetaPhi(neutrons_FD_raw[i]->getP(), neutrons_FD_raw[i]->getTheta(), neutrons_FD_raw[i]->getPhi());

                h_reco_P_n_1e_cut->Fill(reco_P_n.Mag(), weight);
                h_reco_theta_n_1e_cut->Fill(reco_P_n.Theta() * 180 / M_PI, weight);
                h_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, weight);
                h_reco_theta_n_VS_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, reco_P_n.Theta() * 180 / M_PI, weight);
            }
        }

#pragma endregion

        //  Clas12reco plots
        //  -----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Clas12reco plots */

        vector<region_part_ptr> neutrons_FD_clas12;
        vector<region_part_ptr> photons_FD_clas12;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();
            int Momentum = allParticles[i]->par()->getP();

            if (allParticles[i]->getRegion() == FD) {  // RAW neutrons and photons in FD
                if (pid_temp == 2112) {
                    if (Momentum >= nFD_mom_th.GetLowerCut()) { neutrons_FD_clas12.push_back(allParticles[i]); }
                } else if (pid_temp == 22) {
                    if (Momentum >= phFD_mom_th.GetLowerCut()) { photons_FD_clas12.push_back(allParticles[i]); }
                }
            }
        }

        if ((!apply_nFD_multi_cut || neutrons_FD_clas12.size() == 1) && (!apply_no_FD_photons_cut || photons_FD_clas12.size() == 0)) {
            for (int i = 0; i < neutrons_FD_clas12.size(); i++) {
                auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, Neutron_ECAL_detlayer] = raf::CheckForECALHits(neutrons_FD_clas12[i]);

                if (apply_no_same_sector_cuts && (neutrons_FD_clas12[i]->cal(Neutron_ECAL_detlayer)->getSector() != electrons[0]->cal(Electron_ECAL_detlayer)->getSector())) { continue; }

                TVector3 reco_P_nFD;
                reco_P_nFD.SetMagThetaPhi(raf::CalcPnFD(neutrons_FD_clas12[i], electrons[0], starttime, apply_neutFD_redef), neutrons_FD_clas12[i]->getTheta(),
                                          neutrons_FD_clas12[i]->getPhi());

                h_reco_P_nFD_clas12_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_clas12_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_clas12_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_P_e_VS_P_nFD_clas12_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            }
        }

#pragma endregion

        //  Redef plots
        //  ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Redef plots */

        vector<region_part_ptr> neutrons_FD_redef;
        vector<region_part_ptr> photons_FD_redef;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();

            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)) {  // If particle is neutral and in the FD
                auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, Neutron_ECAL_detlayer] = raf::CheckForECALHits(allParticles[i]);

                if ((pid_temp == 2112) || (pid_temp == 22)) {
                    if (ParticleInPCAL) {
                        if (pid_temp == 22) {
                            double Momentum = allParticles[i]->par()->getP();

                            if (Momentum >= phFD_mom_th.GetLowerCut()) { photons_FD_redef.push_back(allParticles[i]); }
                        }
                    } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                        if (ParticleInECIN || ParticleInECOUT) {
                            double Momentum = raf::CalcPnFD(allParticles[i], electrons[0], starttime, apply_neutFD_redef);

                            if (Momentum >= nFD_mom_th.GetLowerCut()) { neutrons_FD_redef.push_back(allParticles[i]); }
                        }
                    }
                }  // end of clas12root neutron or 'photon' if
            }  // end of neutral and in the FD if
        }

        if ((!apply_nFD_multi_cut || neutrons_FD_redef.size() == 1) && (!apply_no_FD_photons_cut || photons_FD_redef.size() == 0)) {
            for (int i = 0; i < neutrons_FD_redef.size(); i++) {
                auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, Neutron_ECAL_detlayer] = raf::CheckForECALHits(neutrons_FD_redef[i]);

                if (apply_no_same_sector_cuts && (neutrons_FD_redef[i]->cal(Neutron_ECAL_detlayer)->getSector() != electrons[0]->cal(Electron_ECAL_detlayer)->getSector())) { continue; }

                if (ParticleInPCAL) { cout << "\n\nError! neutrons_FD_redef is in the PCAL! Aborting...\n\n", exit(1); }

                TVector3 reco_P_nFD;
                reco_P_nFD.SetMagThetaPhi(raf::CalcPnFD(neutrons_FD_redef[i], electrons[0], starttime, apply_neutFD_redef), neutrons_FD_redef[i]->getTheta(), neutrons_FD_redef[i]->getPhi());

                h_reco_P_nFD_redef_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_redef_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_redef_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_P_e_VS_P_nFD_redef_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            }
        }

#pragma endregion

        //  ECALveto plots
        //  ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ECALveto plots */

#pragma region /* FD neutrals definitions (ECALveto) */

        double Reco_P_LnFD_mag_ECALveto = -1;  // Reco leading neutron momentum
        int Reco_P_LnFD_ind_ECALveto = -1;     // Reco leading neutron index

        std::vector<region_part_ptr> neutrons_FD_ECALveto;
        std::vector<region_part_ptr> neutrons_FD_ECALveto_OnlyGood;
        std::vector<region_part_ptr> neutrons_FD_ECALveto_OnlyBad;
        region_part_ptr neutrons_FD_ECALveto_LnFD;
        std::vector<region_part_ptr> photons_FD_ECALveto;
        std::vector<int> neutrons_FD_ECALveto_ind;

        std::tie(Reco_P_LnFD_mag_ECALveto, Reco_P_LnFD_ind_ECALveto, neutrons_FD_ECALveto, neutrons_FD_ECALveto_OnlyGood, neutrons_FD_ECALveto_OnlyBad, neutrons_FD_ECALveto_LnFD,
                 photons_FD_ECALveto, neutrons_FD_ECALveto_ind) = Build_FD_neutrals();

#pragma endregion

#pragma region /* Fill FD neutrals histograms (ECALveto) */

        for (int i = 0; i < photons_FD_ECALveto.size(); i++) { h_reco_P_phFD_BPID_ECALveto_1e_cut->Fill(photons_FD_ECALveto[i]->par()->getP(), weight); }

        if ((!apply_nFD_multi_cut || neutrons_FD_ECALveto.size() == 1)        // Apply nFD multiplicity cut
            && (!apply_no_FD_photons_cut || photons_FD_ECALveto.size() == 0)  // Apply no FD photons above threshold cut
        ) {
            // // Make sure the vectors are the same size
            // if (neutrons_FD_ECALveto.size() != neutrons_FD_ECALveto_ind.size()) {
            //     cout << "\n\nError! neutrons_FD_ECALveto.size() is different from neutrons_FD_ECALveto_ind.size()! Aborting...\n\n", exit(1);
            // }

#pragma region /* Fill leading FD neutron acceptance maps (ECALveto) */

            if (Reco_P_LnFD_ind_ECALveto != -1) {
                auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, Neutron_ECAL_detlayer] = raf::CheckForECALHits(neutrons_FD_ECALveto_LnFD);

                double Mom_neut_1e_cut = raf::CalcPnFD(neutrons_FD_ECALveto_LnFD, electrons[0], starttime, apply_neutFD_redef);
                double Theta_neut_1e_cut = neutrons_FD_ECALveto_LnFD->getTheta() * 180.0 / pi;
                double Phi_neut_1e_cut = neutrons_FD_ECALveto_LnFD->getPhi() * 180.0 / pi;

                bool PassMomTh = (Mom_neut_1e_cut >= nFD_mom_th.GetLowerCut());
                bool PassECALeadgeCuts = (neutrons_FD_ECALveto_LnFD->cal(Neutron_ECAL_detlayer)->getLv() > 14. && neutrons_FD_ECALveto_LnFD->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                bool NeutronPassVeto_1e_cut = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, Reco_P_LnFD_ind_ECALveto, cPart_veto_radius);

                bool PassPhi_nFDCuts = true;
                int nFD_nSector = neutrons_FD_ECALveto_LnFD->cal(Neutron_ECAL_detlayer)->getSector();
                int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();
                if (OnlyGood_nFD) { PassPhi_nFDCuts = (abs(nFD_nSector - e_nSector) == 3); }
                if (OnlyBad_nFD) { PassPhi_nFDCuts = !(abs(nFD_nSector - e_nSector) == 3); }

                // if (PassMomTh && PassECALeadgeCuts && NeutronPassVeto_1e_cut && PassPhi_nFDCuts)  // FOR nFD eff test!
                // {
                //     aMaps_generator.hFillMaps("Reco", "Neutron", Mom_neut_1e_cut, Theta_neut_1e_cut, Phi_neut_1e_cut, weight);
                // }

            }  // end of if id. reco leading neutron

#pragma endregion

#pragma region /* Filling FD leading neutron plots (ECALveto) */

            if (neutrons_FD_ECALveto_LnFD) {
                auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = raf::CheckForECALHits(neutrons_FD_ECALveto_LnFD);

                bool LnFD_pass_dSector_cuts = true;
                int LnFD_nSector = neutrons_FD_ECALveto_LnFD->cal(detlayer)->getSector();
                int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();
                if (OnlyGood_nFD) { LnFD_pass_dSector_cuts = (abs(LnFD_nSector - e_nSector) == 3); }
                if (OnlyBad_nFD) { LnFD_pass_dSector_cuts = !(abs(LnFD_nSector - e_nSector) == 3); }
                bool Same_e_LnFD_Sector_ECALveto = (neutrons_FD_ECALveto_LnFD->cal(detlayer)->getSector() == electrons[0]->cal(Electron_ECAL_detlayer)->getSector());

                if (OnlyGood_nFD && !LnFD_pass_dSector_cuts) { continue; }
                if (OnlyBad_nFD && !LnFD_pass_dSector_cuts) { continue; }

                if (apply_no_same_sector_cuts && (neutrons_FD_ECALveto_LnFD->cal(detlayer)->getSector() != electrons[0]->cal(Electron_ECAL_detlayer)->getSector())) { continue; }

                // if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_ECALveto is in the PCAL! Aborting...\n\n", exit(1); }

                double Path_LnFD = raf::CalcPathnFD(neutrons_FD_ECALveto_LnFD, electrons[0], apply_neutFD_redef);
                double reco_ToF_LnFD = raf::CalcToFnFD(neutrons_FD_ECALveto_LnFD, starttime, apply_neutFD_redef);
                double reco_Beta_LnFD = apply_neutFD_redef ? Path_LnFD / (reco_ToF_LnFD * c) : neutrons_FD_ECALveto_LnFD->par()->getBeta();
                double Edep_ECAL_LnFD =
                    neutrons_FD_ECALveto_LnFD->cal(clas12::PCAL)->getEnergy() + neutrons_FD_ECALveto_LnFD->cal(ECIN)->getEnergy() + neutrons_FD_ECALveto_LnFD->cal(ECOUT)->getEnergy();

                int PCAL_hit = ParticleInPCAL ? 1 : 0;
                int ECIN_hit = ParticleInECIN ? 1 : 0;
                int ECOUT_hit = ParticleInECOUT ? 1 : 0;

                TVector3 reco_P_LnFD;
                // reco_P_LnFD.SetMagThetaPhi(neutrons_FD_ECALveto_LnFD->getP(), neutrons_FD_ECALveto_LnFD->getTheta(), neutrons_FD_ECALveto_LnFD->getPhi());
                reco_P_LnFD.SetMagThetaPhi(raf::CalcPnFD(neutrons_FD_ECALveto_LnFD, electrons[0], starttime, apply_neutFD_redef), neutrons_FD_ECALveto_LnFD->getTheta(),
                                           neutrons_FD_ECALveto_LnFD->getPhi());

                // Safety check to ensure that Reco_P_LnFD_mag_ECALveto is indeed the largest momentum neutron:
                for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
                    if ((raf::CalcPnFD(neutrons_FD_ECALveto[i], electrons[0], starttime, apply_neutFD_redef) > reco_P_LnFD.Mag()) &&
                        (neutrons_FD_ECALveto_ind.at(i) != Reco_P_LnFD_ind_ECALveto)) {
                        cout << "\n\nError! Reco_P_LnFD_mag_ECALveto is not the largest momentum neutron!\n";
                        cout << "Reco_P_LnFD_mag_ECALveto = " << Reco_P_LnFD_mag_ECALveto << "\n";
                        cout << "neutrons_FD_ECALveto_ind.at(i) = " << neutrons_FD_ECALveto_ind.at(i) << "\n";
                        cout << "reco_P_LnFD.Mag() = " << reco_P_LnFD.Mag() << "\n";
                        cout << "Reco_P_LnFD_ind_ECALveto = " << Reco_P_LnFD_ind_ECALveto << "\n";
                        cout << "Aborting...\n\n", exit(1);
                    }
                }

                double dTheta = (reco_P_LnFD.Theta() - reco_P_e.Theta()) * 180 / M_PI;
                double dPhi = am::CalcdPhi1((reco_P_LnFD.Phi() - reco_P_e.Phi()) * 180 / M_PI);

                bool DoesNotPass_theta_nFD_e_vs_dTheta_cuts =
                    (apply_theta_nFD_e_vs_dTheta_cuts &&
                     (Same_e_LnFD_Sector_ECALveto && (((dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetLowerCut()) && ((dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetUpperCut()))));

                if (DoesNotPass_theta_nFD_e_vs_dTheta_cuts) { continue; }

                h_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Mag(), weight);
                h_reco_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Phi() * 180 / M_PI, weight);
                h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Phi() * 180 / M_PI, reco_P_LnFD.Theta() * 180 / M_PI, weight);

                h_Edep_ECAL_LnFD_ECALveto_1e_cut->Fill(Edep_ECAL_LnFD, weight);
                h_beta_LnFD_ECALveto_1e_cut->Fill(reco_Beta_LnFD, weight);
                h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut->Fill(reco_Beta_LnFD, Edep_ECAL_LnFD, weight);

                h_nFD_PCAL_hits_VS_nFD_ECIN_hits_ECALveto_1e_cut->Fill(PCAL_hit, ECIN_hit, weight);
                h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut->Fill(PCAL_hit, ECOUT_hit, weight);
                h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut->Fill(ECIN_hit, ECOUT_hit, weight);

                h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_LnFD.Mag(), weight);
                h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_LnFD.Mag(), weight);
                h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_LnFD.Mag(), weight);
                h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_LnFD.Theta() * 180 / M_PI, weight);
                h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_LnFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_LnFD.Theta() * 180 / M_PI, weight);
                h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_LnFD.Phi() * 180 / M_PI, weight);
                h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_LnFD.Phi() * 180 / M_PI, weight);
                h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_LnFD.Phi() * 180 / M_PI, weight);
                h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Theta() * 180 / M_PI, reco_P_LnFD.Mag(), weight);

                h_reco_dTheta_LnFD_e_ECALveto_1e_cut->Fill(dTheta, weight);
                h_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, weight);
                h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Mag(), weight);
                h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Mag(), weight);
                h_reco_P_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                h_LnFD_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                h_e_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                if (neutrons_FD_ECALveto_LnFD->cal(detlayer)->getSector() == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
                    h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, weight);
                    h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut->Fill(dTheta, weight);
                    h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dPhi, weight);
                    h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, weight);
                    h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);

                    h_reco_P_e_VS_reco_dPhi_LnFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                    h_LnFD_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                    h_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, weight);

                    h_reco_theta_LnFD_e_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, weight);
                    h_reco_theta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, weight);
                    h_reco_theta_LnFD_e_VS_reco_P_LnFD_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Mag(), reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, weight);

                    h_LnFD_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, neutrons_FD_ECALveto_LnFD->par()->getStatus(),
                                                                                                weight);
                    h_LnFD_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI,
                                                                                                       neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_e_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                }

                if (ParticleInPCAL) {
                    h_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dTheta, weight);
                    h_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dPhi, weight);
                    h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Mag(), weight);
                    h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Mag(), weight);
                    h_LnFD_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                } else if (ParticleInECIN) {
                    h_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dTheta, weight);
                    h_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dPhi, weight);
                    h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Mag(), weight);
                    h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Mag(), weight);
                    h_LnFD_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                } else if (ParticleInECOUT) {
                    h_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, weight);
                    h_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dPhi, weight);
                    h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Mag(), weight);
                    h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Mag(), weight);
                    h_LnFD_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                }

                TVector3 v_nhit_ECALveto(neutrons_FD_ECALveto_LnFD->cal(detlayer)->getX(), neutrons_FD_ECALveto_LnFD->cal(detlayer)->getY(),
                                         neutrons_FD_ECALveto_LnFD->cal(detlayer)->getZ());
                TVector3 shifted_e_unit_v_ECALveto = Get_shifted_e_unit_v_NeutVeto(electrons, v_nhit_ECALveto, B, L);
                TVector3 v_dist_ECALveto = v_nhit_ECALveto - shifted_e_unit_v_ECALveto;

                for (int j = 0; j < allParticles.size(); j++) {
                    if (allParticles[j]->par()->getCharge() == 0) {
                        bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                        TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                        if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                            v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
                            TVector3 v_dist = v_nhit_ECALveto - v_neutral_hit;

                            h_v_dist_LnFD_ECALveto_1e_cut->Fill(v_dist.Mag(), weight);
                            h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_LnFD.Mag(), weight);
                            h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                            h_v_dist_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(v_dist.Mag(), dPhi, weight);
                        }
                    }
                }

                h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Mag(), Edep_ECAL_LnFD, weight);
                h_Edep_ECAL_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, Edep_ECAL_LnFD, weight);
                h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Mag(), reco_Beta_LnFD, weight);
                h_beta_n_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, reco_Beta_LnFD, weight);

                if (plot_and_fit_MomRes) {
                    int Neutron_match_counter = 0;
                    double RecoNeutronP_Debug = reco_P_LnFD.Mag();
                    double RecoNeutronTheta_Debug = reco_P_LnFD.Theta() * 180.0 / pi;
                    double RecoNeutronPhi_Debug = reco_P_LnFD.Phi() * 180.0 / pi;

                    for (Int_t i = 0; i < Ngen; i++) {
                        mcpbank->setEntry(i);

                        /* TL neutron kinematic variables */
                        double TLNeutronP = analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                        double TLNeutronTheta = acos(mcpbank->getPz() / TLNeutronP) * 180.0 / pi;
                        double TLNeutronPhi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                        /* Reco neutron kinematic variables */
                        double RecoNeutronP = RecoNeutronP_Debug;
                        double RecoNeutronTheta = RecoNeutronTheta_Debug;
                        double RecoNeutronPhi = RecoNeutronPhi_Debug;

                        /* TL-Reco angle difference */
                        double dNeutronTheta = TLNeutronTheta - RecoNeutronTheta;
                        double dNeutronPhi = am::CalcdPhi1(TLNeutronPhi - RecoNeutronPhi);

                        int pid_nRes = mcpbank->getPid();

                        // nRes cuts

                        // nRes good neutron cuts
                        bool nRes_TL_Pass_PIDCut = (pid_nRes == 2112);

                        bool Reco_InFD = true;
                        // bool Reco_InFD = aMaps_FCapplier.IsInFDQuery((!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps ||
                        // AMapsSettings.Generate_Nucleon_AMaps)),
                        //                                           CutManager.ThetaFD, "Neutron", RecoNeutronP, RecoNeutronTheta, RecoNeutronPhi, false);
                        bool TL_InFD = true;
                        // bool TL_InFD = aMaps_FCapplier.IsInFDQuery((!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps ||
                        // AMapsSettings.Generate_Nucleon_AMaps)),
                        //                                         CutManager.ThetaFD, "Neutron", TLNeutronP, TLNeutronTheta, TLNeutronPhi, false);
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
                            if (nRes_Pass_dThetaCut && nRes_Pass_dPhiCut) {
                                double nResolution = (TLNeutronP - RecoNeutronP) / TLNeutronP;

                                h_P_LnFD_Res_VS_truth_P_LnFD_ECALveto_1e_cut->Fill(TLNeutronP, nResolution, weight);
                                h_P_LnFD_Res_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(RecoNeutronP, nResolution, weight);

                                h_P_nFD_Res_BysliceOf_truth_P_nFD_ECALveto_1e_cut.Fill(TLNeutronP, nResolution, nResolution, weight);
                                h_P_nFD_Res_BysliceOf_reco_P_nFD_ECALveto_1e_cut.Fill(RecoNeutronP, nResolution, nResolution, weight);
                            }
                        }
                    }  // end of resolution calculation if
                }  // end of if plot_and_fit_MomRes
            }  // end of if neutrons_FD_ECALveto_LnFD

#pragma endregion

#pragma region /* Filling FD neutron plots (ECALveto) */

            for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
                auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = raf::CheckForECALHits(neutrons_FD_ECALveto[i]);

                bool nFD_pass_dSector_cuts = true;
                int nFD_nSector = neutrons_FD_ECALveto[i]->cal(detlayer)->getSector();
                int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();
                if (OnlyGood_nFD) { nFD_pass_dSector_cuts = (abs(nFD_nSector - e_nSector) == 3); }
                if (OnlyBad_nFD) { nFD_pass_dSector_cuts = !(abs(nFD_nSector - e_nSector) == 3); }
                bool Same_e_nFD_Sector_ECALveto = (neutrons_FD_ECALveto[i]->cal(detlayer)->getSector() == electrons[0]->cal(Electron_ECAL_detlayer)->getSector());

                if (OnlyGood_nFD && !nFD_pass_dSector_cuts) { continue; }
                if (OnlyBad_nFD && !nFD_pass_dSector_cuts) { continue; }

                if (apply_no_same_sector_cuts && (neutrons_FD_ECALveto[i]->cal(detlayer)->getSector() != electrons[0]->cal(Electron_ECAL_detlayer)->getSector())) { continue; }

                // if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_ECALveto is in the PCAL! Aborting...\n\n", exit(1); }

                double Path_nFD = raf::CalcPathnFD(neutrons_FD_ECALveto[i], electrons[0], apply_neutFD_redef);
                double reco_ToF_nFD = raf::CalcToFnFD(neutrons_FD_ECALveto[i], starttime, apply_neutFD_redef);
                double reco_Beta_nFD = apply_neutFD_redef ? Path_nFD / (reco_ToF_nFD * c) : neutrons_FD_ECALveto[i]->par()->getBeta();
                double Edep_ECAL_nFD =
                    neutrons_FD_ECALveto[i]->cal(clas12::PCAL)->getEnergy() + neutrons_FD_ECALveto[i]->cal(ECIN)->getEnergy() + neutrons_FD_ECALveto[i]->cal(ECOUT)->getEnergy();

                TVector3 reco_P_nFD;
                // reco_P_nFD.SetMagThetaPhi(neutrons_FD_ECALveto[i]->getP(), neutrons_FD_ECALveto[i]->getTheta(), neutrons_FD_ECALveto[i]->getPhi());
                reco_P_nFD.SetMagThetaPhi(raf::CalcPnFD(neutrons_FD_ECALveto[i], electrons[0], starttime, apply_neutFD_redef), neutrons_FD_ECALveto[i]->getTheta(),
                                          neutrons_FD_ECALveto[i]->getPhi());

                double dTheta = (reco_P_nFD.Theta() - reco_P_e.Theta()) * 180 / M_PI;
                double dPhi = am::CalcdPhi1((reco_P_nFD.Phi() - reco_P_e.Phi()) * 180 / M_PI);

                bool DoesNotPass_theta_nFD_e_vs_dTheta_cuts =
                    (apply_theta_nFD_e_vs_dTheta_cuts &&
                     (Same_e_nFD_Sector_ECALveto && (((dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetLowerCut()) && ((dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetUpperCut()))));

                if (DoesNotPass_theta_nFD_e_vs_dTheta_cuts) { continue; }

                bool Pass_dTheta_status_ECALveto = true;
                bool Pass_dPhi_status_ECALveto = true;

                if (Pass_dTheta_status_ECALveto && Pass_dPhi_status_ECALveto) {
                    h_reco_P_e_ECALveto_1e_cut->Fill(reco_P_e.Mag(), weight);
                    h_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
                    h_reco_phi_e_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
                    h_reco_theta_e_ECALveto_VS_reco_phi_e_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);

                    h_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);

                    if (ParticleInPCAL) {
                        h_reco_P_nFD_PCAL_only_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                    } else if (ParticleInECIN) {
                        h_reco_P_nFD_ECIN_only_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                    } else if (ParticleInECOUT) {
                        h_reco_P_nFD_ECOUT_only_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                    }

                    h_reco_theta_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);

                    h_Edep_ECAL_nFD_ECALveto_1e_cut->Fill(Edep_ECAL_nFD, weight);
                    h_beta_nFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, weight);
                    h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, Edep_ECAL_nFD, weight);

                    h_reco_P_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
                    h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                    h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                    h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                    h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_ECALveto.size(), weight);

                    h_reco_dTheta_nFD_e_ECALveto_1e_cut->Fill(dTheta, weight);
                    h_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, weight);
                    h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    h_reco_P_nFD_VS_reco_dTheta_nFD_e_ECALveto_1e_cut->Fill(dTheta, reco_P_nFD.Mag(), weight);
                    h_reco_P_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, reco_P_nFD.Mag(), weight);
                    h_reco_P_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                    h_nFD_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                    h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                    if (neutrons_FD_ECALveto[i]->cal(detlayer)->getSector() == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
                        h_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, weight);
                        h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(dTheta, weight);
                        h_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dPhi, weight);
                        h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, weight);

                        if ((dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetLowerCut() && (dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetUpperCut()) {
                            h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_ECALveto_1e_cut->Fill(dPhi, weight);
                            h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin_ECALveto_1e_cut->Fill(dPhi, weight);
                        }

                        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);

                        h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                        h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                        h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                        h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                        h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                        h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                        h_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);

                        h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dPhi, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);

                        h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, neutrons_FD_ECALveto[i]->par()->getStatus(),
                                                                                                 weight);
                        h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, neutrons_FD_ECALveto[i]->par()->getStatus(),
                                                                                                        weight);
                        h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                        h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                    }

                    TVector3 v_nhit_ECALveto(neutrons_FD_ECALveto[i]->cal(detlayer)->getX(), neutrons_FD_ECALveto[i]->cal(detlayer)->getY(), neutrons_FD_ECALveto[i]->cal(detlayer)->getZ());
                    TVector3 shifted_e_unit_v_ECALveto = Get_shifted_e_unit_v_NeutVeto(electrons, v_nhit_ECALveto, B, L);
                    TVector3 v_dist_ECALveto = v_nhit_ECALveto - shifted_e_unit_v_ECALveto;

                    h_dr_n_ECALveto_1e_cut->Fill(v_dist_ECALveto.Mag(), weight);
                    h_dr_n_VS_dTheta_n_e_ECALveto_1e_cut->Fill(v_dist_ECALveto.Mag(), dTheta, weight);
                    h_dr_n_VS_dPhi_n_e_ECALveto_1e_cut->Fill(v_dist_ECALveto.Mag(), dPhi, weight);

                    h_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_ECALveto_1e_cut.Fill(v_dist_ECALveto.Mag(), dPhi, dTheta, weight);

                    h_dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_ECALveto_1e_cut->Fill(v_dist_ECALveto.Phi() * 180 / M_PI, v_dist_ECALveto.Theta() * 180 / M_PI, weight);

                    for (int j = 0; j < allParticles.size(); j++) {
                        if (allParticles[j]->par()->getCharge() == 0) {
                            bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                            TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                            if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                                v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
                                TVector3 v_dist = v_nhit_ECALveto - v_neutral_hit;

                                h_v_dist_nFD_ECALveto_1e_cut->Fill(v_dist.Mag(), weight);
                                h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                                h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                                h_v_dist_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(v_dist.Mag(), dPhi, weight);
                            }
                        }
                    }

                    h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), Edep_ECAL_nFD, weight);
                    h_Edep_ECAL_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, Edep_ECAL_nFD, weight);
                    h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), reco_Beta_nFD, weight);
                    h_beta_n_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, reco_Beta_nFD, weight);

                    // Fake FD neutrons handling (neutron veto)
                    TVector3 nFD_hit_3V, e_hit_3V;

                    double nFD_hit_Phi, nFD_hit_Theta, e_hit_Phi, e_hit_Theta, dPhi_hit, dTheta_hit;

                    if (!ParticleInPCAL && (ParticleInECIN || ParticleInECOUT)) {
                        // if neutron did not hit PCAL, and hit either ECIN or ECOUT

                        // neutron ECIN/ECAL hit vector and angles:
                        nFD_hit_3V.SetXYZ(neutrons_FD_ECALveto[i]->cal(detlayer)->getX(), neutrons_FD_ECALveto[i]->cal(detlayer)->getY(), neutrons_FD_ECALveto[i]->cal(detlayer)->getZ());
                        nFD_hit_Theta = nFD_hit_3V.Theta() * 180 / pi, nFD_hit_Phi = nFD_hit_3V.Phi() * 180 / pi;

                        if ((detlayer == clas12::ECIN) && (electrons[0]->cal(clas12::ECIN)->getZ() != 0)) {
                            e_hit_3V.SetXYZ(electrons[0]->cal(clas12::ECIN)->getX(), electrons[0]->cal(clas12::ECIN)->getY(), electrons[0]->cal(clas12::ECIN)->getZ());
                            e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                        } else if ((detlayer == clas12::ECOUT) && (electrons[0]->cal(clas12::ECOUT)->getZ() != 0)) {
                            e_hit_3V.SetXYZ(electrons[0]->cal(clas12::ECOUT)->getX(), electrons[0]->cal(clas12::ECOUT)->getY(), electrons[0]->cal(clas12::ECOUT)->getZ());
                            e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                        } else {
                            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
                            e_hit_3V.SetXYZ(electrons[0]->traj(clas12::ECAL, trajlayer)->getX(), electrons[0]->traj(clas12::ECAL, trajlayer)->getY(),
                                            electrons[0]->traj(clas12::ECAL, trajlayer)->getZ());
                            e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                        }

                        dPhi_hit = am::CalcdPhi1(nFD_hit_Phi - e_hit_Phi), dTheta_hit = nFD_hit_Theta - e_hit_Theta;
                    }  // end of if neutron did not hit PCAL & hit either ECIN or ECOUT

                    // bool NeutronPassVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);

                    h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_ECALveto_1e_cut->Fill(dPhi_hit, dTheta_hit, weight);
                }  // end of if pass Pass_dTheta_status_ECALveto && Pass_dPhi_status_ECALveto
            }  // end of for loop over neutrons_FD_ECALveto

            for (int i = 0; i < photons_FD_ECALveto.size(); i++) { h_reco_P_phFD_APID_ECALveto_1e_cut->Fill(photons_FD_ECALveto[i]->par()->getP(), weight); }

            if (neutrons_FD_ECALveto.size() != 0) { h_reco_nFD_multi_AllN_ECALveto_1e_cut->Fill(neutrons_FD_ECALveto.size(), weight); }
            if (neutrons_FD_ECALveto_OnlyGood.size() != 0) { h_reco_nFD_multi_OnlyGood_ECALveto_1e_cut->Fill(neutrons_FD_ECALveto_OnlyGood.size(), weight); }
            if (neutrons_FD_ECALveto_OnlyBad.size() != 0) { h_reco_nFD_multi_OnlyBad_ECALveto_1e_cut->Fill(neutrons_FD_ECALveto_OnlyBad.size(), weight); }

#pragma endregion

        }  // end of if pass Pass_nFD_Multi_cut_ECALveto && Pass_no_FD_photon_cut_ECALveto

#pragma endregion

#pragma endregion

        //  Matched plots
        //  -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Matched plots */

#pragma region /* FD neutrals definitions (matched) */

        double Reco_P_LnFD_mag_matched = -1;  // Reco leading neutron momentum
        int Reco_P_LnFD_ind_matched = -1;     // Reco leading neutron index

        std::vector<region_part_ptr> neutrons_FD_matched;
        std::vector<region_part_ptr> neutrons_FD_matched_OnlyGood;
        std::vector<region_part_ptr> neutrons_FD_matched_OnlyBad;
        region_part_ptr neutrons_FD_matched_LnFD;
        std::vector<region_part_ptr> photons_FD_matched;
        std::vector<int> neutrons_FD_matched_ind;

        // Vectors for multiplicity plots
        std::vector<region_part_ptr> neutrons_FD_PassMatch;
        std::vector<region_part_ptr> neutrons_FD_PassMatch_OnlyGood;
        std::vector<region_part_ptr> neutrons_FD_PassMatch_OnlyBad;
        std::vector<region_part_ptr> neutrons_FD_NotPassMatch;
        std::vector<region_part_ptr> neutrons_FD_NotPassMatch_OnlyGood;
        std::vector<region_part_ptr> neutrons_FD_NotPassMatch_OnlyBad;

        std::tie(Reco_P_LnFD_mag_matched, Reco_P_LnFD_ind_matched, neutrons_FD_matched, neutrons_FD_matched_OnlyGood, neutrons_FD_matched_OnlyBad, neutrons_FD_matched_LnFD,
                 photons_FD_matched, neutrons_FD_matched_ind) = Build_FD_neutrals();

#pragma endregion

#pragma region /* Fill FD neutrals histograms (matched) */

        // Filling phFD momentum BPID (matched):
        for (int i = 0; i < photons_FD_matched.size(); i++) { h_reco_P_phFD_BPID_matched_1e_cut->Fill(photons_FD_matched[i]->par()->getP(), weight); }

        if ((!apply_nFD_multi_cut || neutrons_FD_matched.size() == 1)        // Apply nFD multiplicity cut
            && (!apply_no_FD_photons_cut || photons_FD_matched.size() == 0)  // Apply no FD photons above threshold cut
        ) {
            // if (neutrons_FD_matched.size() != neutrons_FD_matched_ind.size()) {
            //     cout << "\n\nError! neutrons_FD_matched.size() is different from neutrons_FD_matched_ind.size()! Aborting...\n\n", exit(1);
            // }

#pragma region /* Fill leading FD neutron plots and matching (matched) */

            bool PassMatchingCuts_LnFD = false;

            // Check if leading neutron passes matching cuts:
            if (neutrons_FD_matched_LnFD) {
                for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                    mcpbank->setEntry(truth_NeutronsFD.at(j));

                    TVector3 reco_P_LnFD;
                    // reco_P_LnFD.SetMagThetaPhi(neutrons_FD_matched_LnFD->getP(), neutrons_FD_matched_LnFD->getTheta(), neutrons_FD_matched_LnFD->getPhi());
                    reco_P_LnFD.SetMagThetaPhi(raf::CalcPnFD(neutrons_FD_matched_LnFD, electrons[0], starttime, apply_neutFD_redef), neutrons_FD_matched_LnFD->getTheta(),
                                               neutrons_FD_matched_LnFD->getPhi());

                    double Temp_truth_P_nFD = mcpbank->getP();
                    double Temp_truth_theta_nFD = mcpbank->getTheta() * 180 / M_PI;
                    double Temp_truth_phi_nFD = mcpbank->getPhi() * 180 / M_PI;

                    double reco_Theta = reco_P_LnFD.Theta() * 180 / M_PI;
                    double reco_Phi = reco_P_LnFD.Phi() * 180 / M_PI;

                    bool thetaCut = (fabs(Temp_truth_theta_nFD - reco_Theta) <= 2.);
                    bool phiCut = (fabs(am::CalcdPhi1(Temp_truth_phi_nFD - reco_Phi)) <= 5.);

                    // Check if reco leading neutron is in FD fiducial volume:
                    bool Reco_InFD =
                        aMaps_FCapplier.IsInFDQuery(/* (!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps || AMapsSettings.Generate_Nucleon_AMaps))= */ false,
                                                    ThetaFD, "Neutron", reco_P_LnFD.Mag(), reco_P_LnFD.Theta() * 180.0 / pi, reco_P_LnFD.Phi() * 180.0 / pi, false);

                    // if ((thetaCut && phiCut) && Reco_InFD) {  // Leading neutron passes matching cuts and is in FD fiducial volume
                    if ((thetaCut && phiCut)) {  // Leading neutron passes matching cuts and is in FD fiducial volume
                        PassMatchingCuts_LnFD = true;

                        h_truth_P_nFD_matched_to_reco_LnFD_matched_1e_cut->Fill(Temp_truth_P_nFD, weight);
                        h_truth_theta_nFD_matched_to_reco_LnFD_matched_1e_cut->Fill(Temp_truth_theta_nFD, weight);
                        h_truth_phi_nFD_matched_to_reco_LnFD_matched_1e_cut->Fill(Temp_truth_phi_nFD, weight);
                        h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_to_reco_LnFD_matched_1e_cut->Fill(Temp_truth_phi_nFD, Temp_truth_theta_nFD, weight);
                    }
                }  // end of if id. reco leading neutron
            }

#pragma endregion

#pragma region /* Filling FD neutron plots (matched) */

            for (int i = 0; i < neutrons_FD_matched.size(); i++) {
                auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = raf::CheckForECALHits(neutrons_FD_matched[i]);

                bool nFD_pass_dSector_cuts = true;
                int nFD_nSector = neutrons_FD_matched[i]->cal(detlayer)->getSector();
                int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();
                if (OnlyGood_nFD) { nFD_pass_dSector_cuts = (abs(nFD_nSector - e_nSector) == 3); }
                if (OnlyBad_nFD) { nFD_pass_dSector_cuts = !(abs(nFD_nSector - e_nSector) == 3); }
                bool Same_e_nFD_Sector_matched = (neutrons_FD_matched[i]->cal(detlayer)->getSector() == electrons[0]->cal(Electron_ECAL_detlayer)->getSector());

                if (OnlyGood_nFD && !nFD_pass_dSector_cuts) { continue; }
                if (OnlyBad_nFD && !nFD_pass_dSector_cuts) { continue; }

                if (apply_no_same_sector_cuts && (neutrons_FD_matched[i]->cal(detlayer)->getSector() != electrons[0]->cal(Electron_ECAL_detlayer)->getSector())) { continue; }

                // if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_matched is in the PCAL! Aborting...\n\n", exit(1); }

                double Path_nFD = raf::CalcPathnFD(neutrons_FD_matched[i], electrons[0], apply_neutFD_redef);
                double reco_ToF_nFD = raf::CalcToFnFD(neutrons_FD_matched[i], starttime, apply_neutFD_redef);
                double reco_Beta_nFD = apply_neutFD_redef ? Path_nFD / (reco_ToF_nFD * c) : neutrons_FD_matched[i]->par()->getBeta();
                double Edep_ECAL_nFD =
                    neutrons_FD_matched[i]->cal(clas12::PCAL)->getEnergy() + neutrons_FD_matched[i]->cal(ECIN)->getEnergy() + neutrons_FD_matched[i]->cal(ECOUT)->getEnergy();

                int PCAL_hit = ParticleInPCAL ? 1 : 0;
                int ECIN_hit = ParticleInECIN ? 1 : 0;
                int ECOUT_hit = ParticleInECOUT ? 1 : 0;

                TVector3 reco_P_nFD;
                // reco_P_nFD.SetMagThetaPhi(neutrons_FD_matched[i]->getP(), neutrons_FD_matched[i]->getTheta(), neutrons_FD_matched[i]->getPhi());
                reco_P_nFD.SetMagThetaPhi(raf::CalcPnFD(neutrons_FD_matched[i], electrons[0], starttime, apply_neutFD_redef), neutrons_FD_matched[i]->getTheta(),
                                          neutrons_FD_matched[i]->getPhi());

                bool Reco_InFD =
                    aMaps_FCapplier.IsInFDQuery(/* (!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps || AMapsSettings.Generate_Nucleon_AMaps))= */ false, ThetaFD,
                                                "Neutron", reco_P_nFD.Mag(), reco_P_nFD.Theta() * 180.0 / pi, reco_P_nFD.Phi() * 180.0 / pi, false);

                double dTheta = (reco_P_nFD.Theta() - reco_P_e.Theta()) * 180 / M_PI;
                double dPhi = am::CalcdPhi1((reco_P_nFD.Phi() - reco_P_e.Phi()) * 180 / M_PI);

                bool DoesNotPass_theta_nFD_e_vs_dTheta_cuts =
                    (apply_theta_nFD_e_vs_dTheta_cuts &&
                     (Same_e_nFD_Sector_matched && (((dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetLowerCut()) && ((dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetUpperCut()))));

                if (DoesNotPass_theta_nFD_e_vs_dTheta_cuts) { continue; }

                bool PassMatchingCuts = false;

                // For every reco nFD, loop over all truth nFD in the event and find a match
                for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                    mcpbank->setEntry(truth_NeutronsFD.at(j));

                    double Temp_truth_P_nFD = mcpbank->getP();
                    double Temp_truth_theta_nFD = mcpbank->getTheta() * 180 / M_PI;
                    double Temp_truth_phi_nFD = mcpbank->getPhi() * 180 / M_PI;

                    double reco_Theta = reco_P_nFD.Theta() * 180 / M_PI;
                    double reco_Phi = reco_P_nFD.Phi() * 180 / M_PI;

                    bool thetaCut = (fabs(Temp_truth_theta_nFD - reco_Theta) <= 2.);
                    bool phiCut = (fabs(am::CalcdPhi1(Temp_truth_phi_nFD - reco_Phi)) <= 5.);

                    if (thetaCut && phiCut) {
                        PassMatchingCuts = true;

                        // TODO: move to a differant numerator with the leading neutron, after talking with Adi
                        // Detection efficiency numerator histogram. Filled the truth P_nFD distribution of the truth nFD that passes matching cuts with the i-th reco nFD. This gives us
                        // (e,e'n) events.
                        h_truth_P_nFD_matched_1e_cut->Fill(Temp_truth_P_nFD, weight);
                        h_truth_theta_nFD_matched_1e_cut->Fill(Temp_truth_theta_nFD, weight);
                        h_truth_phi_nFD_matched_1e_cut->Fill(Temp_truth_phi_nFD, weight);
                        h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut->Fill(Temp_truth_phi_nFD, Temp_truth_theta_nFD, weight);

                        // TODO: Should this be here? this means that for every reco nFD that matches to a truth nFD, we fill the numerator, meaning that if 2 reco nFD match to the same
                        // truth nFD Acceptance efficiency numerator histograms filled with the truth P_nFD distribution of the truth nFD that passes matching cuts with the i-th reco nFD.
                        // This gives us (e,e'n) events.
                        aMaps_generator.hFillMaps("Reco", "Neutron", Temp_truth_P_nFD, Temp_truth_theta_nFD, Temp_truth_phi_nFD, weight);
                    }
                }

                // if (true) {
                // if (PassMatchingCuts_LnFD) {
                // if (PassMatchingCuts_LnFD && Reco_InFD) {
                // if (PassMatchingCuts && Reco_InFD) {
                if (PassMatchingCuts) {
                    // bool PassMatchingCuts = false;

                    // // For every reco nFD, loop over all truth nFD in the event and find a match
                    // for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                    //     mcpbank->setEntry(truth_NeutronsFD.at(j));

                    //     double Temp_truth_P_nFD = mcpbank->getP();
                    //     double Temp_truth_theta_nFD = mcpbank->getTheta() * 180 / M_PI;
                    //     double Temp_truth_phi_nFD = mcpbank->getPhi() * 180 / M_PI;

                    //     double reco_Theta = reco_P_nFD.Theta() * 180 / M_PI;
                    //     double reco_Phi = reco_P_nFD.Phi() * 180 / M_PI;

                    //     bool thetaCut = (fabs(Temp_truth_theta_nFD - reco_Theta) <= 2.);
                    //     bool phiCut = (fabs(am::CalcdPhi1(Temp_truth_phi_nFD - reco_Phi)) <= 5.);

                    //     if (thetaCut && phiCut) {
                    //         PassMatchingCuts = true;

                    //         // TODO: move to a differant numerator with the leading neutron, after talking with Adi
                    //         // Detection efficiency numerator histogram. Filled the truth P_nFD distribution of the truth nFD that passes matching cuts with the i-th reco nFD. This gives
                    //         us
                    //         // (e,e'n) events.
                    //         h_truth_P_nFD_matched_1e_cut->Fill(Temp_truth_P_nFD, weight);
                    //         h_truth_theta_nFD_matched_1e_cut->Fill(Temp_truth_theta_nFD, weight);
                    //         h_truth_phi_nFD_matched_1e_cut->Fill(Temp_truth_phi_nFD, weight);
                    //         h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut->Fill(Temp_truth_phi_nFD, Temp_truth_theta_nFD, weight);

                    //         // TODO: Should this be here? this means that for every reco nFD that matches to a truth nFD, we fill the numerator, meaning that if 2 reco nFD match to the
                    //         same
                    //         // truth nFD Acceptance efficiency numerator histograms filled with the truth P_nFD distribution of the truth nFD that passes matching cuts with the i-th reco
                    //         // nFD. This gives us (e,e'n) events.
                    //         aMaps_generator.hFillMaps("Reco", "Neutron", Temp_truth_P_nFD, Temp_truth_theta_nFD, Temp_truth_phi_nFD, weight);
                    //     }
                    // }

                    neutrons_FD_PassMatch.push_back(neutrons_FD_matched[i]);

                    int nFD_nSector = neutrons_FD_matched[i]->cal(detlayer)->getSector();
                    int e_nSector = electrons[0]->cal(detlayer)->getSector();

                    if (abs(nFD_nSector - e_nSector) == 3) {
                        neutrons_FD_PassMatch_OnlyGood.push_back(neutrons_FD_matched[i]);
                    } else {
                        neutrons_FD_PassMatch_OnlyBad.push_back(neutrons_FD_matched[i]);
                    }

                    h_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight);

                    if (ParticleInPCAL) {
                        h_reco_P_nFD_PCAL_only_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                    } else if (ParticleInECIN) {
                        h_reco_P_nFD_ECIN_only_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                    } else if (ParticleInECOUT) {
                        h_reco_P_nFD_ECOUT_only_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                    }

                    h_reco_theta_nFD_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);

                    h_Edep_ECAL_nFD_matched_1e_cut->Fill(Edep_ECAL_nFD, weight);
                    h_beta_nFD_matched_1e_cut->Fill(reco_Beta_nFD, weight);
                    h_Edep_ECAL_nFD_VS_beta_nFD_matched_1e_cut->Fill(reco_Beta_nFD, Edep_ECAL_nFD, weight);

                    h_nFD_PCAL_hits_VS_nFD_ECIN_hits_matched_1e_cut->Fill(PCAL_hit, ECIN_hit, weight);
                    h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_matched_1e_cut->Fill(PCAL_hit, ECOUT_hit, weight);
                    h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_matched_1e_cut->Fill(ECIN_hit, ECOUT_hit, weight);

                    h_reco_P_e_VS_P_nFD_matched_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
                    h_reco_theta_e_VS_P_nFD_matched_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                    h_reco_phi_e_VS_P_nFD_matched_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                    h_reco_P_e_VS_theta_nFD_matched_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_theta_e_VS_theta_nFD_matched_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_e_VS_theta_nFD_matched_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_P_e_VS_phi_nFD_matched_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_theta_e_VS_phi_nFD_matched_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_phi_e_VS_phi_nFD_matched_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                    h_reco_nFD_multi_VS_reco_P_nFD_PassMatch_matched_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_matched.size(), weight);

                    h_reco_dTheta_nFD_e_matched_1e_cut->Fill(dTheta, weight);
                    h_reco_dPhi_nFD_e_matched_1e_cut->Fill(dPhi, weight);
                    h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_matched_1e_cut->Fill(dPhi, dTheta, weight);
                    h_reco_P_nFD_VS_reco_dTheta_nFD_e_matched_1e_cut->Fill(dTheta, reco_P_nFD.Mag(), weight);
                    h_reco_P_nFD_VS_reco_dPhi_nFD_e_matched_1e_cut->Fill(dPhi, reco_P_nFD.Mag(), weight);
                    h_reco_P_e_VS_reco_dPhi_nFD_e_matched_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                    h_nFD_status_VS_reco_dTheta_nFD_e_matched_1e_cut->Fill(dTheta, neutrons_FD_matched[i]->par()->getStatus(), weight);
                    h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut->Fill(dTheta, neutrons_FD_matched[i]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_nFD_e_matched_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                    if (neutrons_FD_matched[i]->cal(detlayer)->getSector() == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
                        h_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, weight);
                        h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut->Fill(dTheta, weight);
                        h_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dPhi, weight);
                        h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut->Fill(dPhi, weight);

                        if ((dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetLowerCut() && (dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetUpperCut()) {
                            h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_matched_1e_cut->Fill(dPhi, weight);
                            h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin_matched_1e_cut->Fill(dPhi, weight);
                        }

                        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dPhi, dTheta, weight);
                        h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut->Fill(dPhi, dTheta, weight);

                        h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                        h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                        h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, neutrons_FD_matched[i]->par()->getStatus(), weight);
                        h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, neutrons_FD_matched[i]->par()->getStatus(), weight);
                        h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                        h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                        h_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);

                        h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dPhi, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut->Fill(dPhi, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Mag(), reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);

                        h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, neutrons_FD_matched[i]->par()->getStatus(), weight);
                        h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, neutrons_FD_matched[i]->par()->getStatus(),
                                                                                                       weight);
                        h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                        h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                    }

                    TVector3 v_nhit_matched(neutrons_FD_matched[i]->cal(detlayer)->getX(), neutrons_FD_matched[i]->cal(detlayer)->getY(), neutrons_FD_matched[i]->cal(detlayer)->getZ());
                    TVector3 shifted_e_unit_v_matched = Get_shifted_e_unit_v_NeutVeto(electrons, v_nhit_matched, B, L);
                    TVector3 v_dist_matched = v_nhit_matched - shifted_e_unit_v_matched;

                    h_dr_n_matched_1e_cut->Fill(v_dist_matched.Mag(), weight);
                    h_dr_n_VS_dTheta_n_e_matched_1e_cut->Fill(v_dist_matched.Mag(), dTheta, weight);
                    h_dr_n_VS_dPhi_n_e_matched_1e_cut->Fill(v_dist_matched.Mag(), dPhi, weight);

                    h_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_matched_1e_cut.Fill(v_dist_matched.Mag(), dPhi, dTheta, weight);

                    h_dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_matched_1e_cut->Fill(v_dist_matched.Phi() * 180 / M_PI, v_dist_matched.Theta() * 180 / M_PI, weight);

                    for (int j = 0; j < allParticles.size(); j++) {
                        if (allParticles[j]->par()->getCharge() == 0) {
                            bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                            TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                            if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                                v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
                                TVector3 v_dist = v_nhit_matched - v_neutral_hit;

                                h_v_dist_nFD_matched_1e_cut->Fill(v_dist.Mag(), weight);
                                h_v_dist_nFD_VS_reco_P_nFD_matched_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                                h_v_dist_nFD_VS_reco_P_e_matched_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                                h_v_dist_nFD_VS_reco_dPhi_nFD_e_matched_1e_cut->Fill(v_dist.Mag(), dPhi, weight);
                            }
                        }
                    }

                    h_Edep_ECAL_VS_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), Edep_ECAL_nFD, weight);
                    h_Edep_ECAL_VS_reco_dPhi_nFD_e_matched_1e_cut->Fill(dPhi, Edep_ECAL_nFD, weight);
                    h_beta_n_VS_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), reco_Beta_nFD, weight);
                    h_beta_n_VS_reco_dPhi_nFD_e_matched_1e_cut->Fill(dPhi, reco_Beta_nFD, weight);

                    // Fake FD neutrons handling (neutron veto)
                    TVector3 nFD_hit_3V, e_hit_3V;

                    double nFD_hit_Phi, nFD_hit_Theta, e_hit_Phi, e_hit_Theta, dPhi_hit, dTheta_hit;

                    if (!ParticleInPCAL && (ParticleInECIN || ParticleInECOUT)) {
                        // if neutron did not hit PCAL, and hit either ECIN or ECOUT

                        // neutron ECIN/ECAL hit vector and angles:
                        nFD_hit_3V.SetXYZ(neutrons_FD_matched[i]->cal(detlayer)->getX(), neutrons_FD_matched[i]->cal(detlayer)->getY(), neutrons_FD_matched[i]->cal(detlayer)->getZ());
                        nFD_hit_Theta = nFD_hit_3V.Theta() * 180 / pi, nFD_hit_Phi = nFD_hit_3V.Phi() * 180 / pi;

                        if ((detlayer == clas12::ECIN) && (electrons[0]->cal(clas12::ECIN)->getZ() != 0)) {
                            e_hit_3V.SetXYZ(electrons[0]->cal(clas12::ECIN)->getX(), electrons[0]->cal(clas12::ECIN)->getY(), electrons[0]->cal(clas12::ECIN)->getZ());
                            e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                        } else if ((detlayer == clas12::ECOUT) && (electrons[0]->cal(clas12::ECOUT)->getZ() != 0)) {
                            e_hit_3V.SetXYZ(electrons[0]->cal(clas12::ECOUT)->getX(), electrons[0]->cal(clas12::ECOUT)->getY(), electrons[0]->cal(clas12::ECOUT)->getZ());
                            e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                        } else {
                            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
                            e_hit_3V.SetXYZ(electrons[0]->traj(clas12::ECAL, trajlayer)->getX(), electrons[0]->traj(clas12::ECAL, trajlayer)->getY(),
                                            electrons[0]->traj(clas12::ECAL, trajlayer)->getZ());
                            e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                        }

                        dPhi_hit = am::CalcdPhi1(nFD_hit_Phi - e_hit_Phi), dTheta_hit = nFD_hit_Theta - e_hit_Theta;
                    }  // end of if neutron did not hit PCAL & hit either ECIN or ECOUT

                    // bool NeutronPassVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);

                    h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_matched_1e_cut->Fill(dPhi_hit, dTheta_hit, weight);

                    if (plot_and_fit_MomRes) {
                        // if ((neutrons_FD_matched_ind.at(i) == Reco_P_LnFD_ind_matched) && plot_and_fit_MomRes) {
                        int Neutron_match_counter = 0;
                        double RecoNeutronP_Debug = reco_P_nFD.Mag();
                        double RecoNeutronTheta_Debug = reco_P_nFD.Theta() * 180.0 / pi;
                        double RecoNeutronPhi_Debug = reco_P_nFD.Phi() * 180.0 / pi;

                        for (Int_t i = 0; i < Ngen; i++) {
                            mcpbank->setEntry(i);

                            /* TL neutron kinematic variables */
                            double TLNeutronP = analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                            double TLNeutronTheta = acos(mcpbank->getPz() / TLNeutronP) * 180.0 / pi;
                            double TLNeutronPhi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                            /* Reco neutron kinematic variables */
                            double RecoNeutronP = RecoNeutronP_Debug;
                            double RecoNeutronTheta = RecoNeutronTheta_Debug;
                            double RecoNeutronPhi = RecoNeutronPhi_Debug;

                            /* TL-Reco angle difference */
                            double dNeutronTheta = TLNeutronTheta - RecoNeutronTheta;
                            double dNeutronPhi = am::CalcdPhi1(TLNeutronPhi - RecoNeutronPhi);

                            int pid_nRes = mcpbank->getPid();

                            // nRes cuts

                            // nRes good neutron cuts
                            bool nRes_TL_Pass_PIDCut = (pid_nRes == 2112);

                            bool Reco_InFD = true;
                            // bool Reco_InFD = aMaps_FCapplier.IsInFDQuery((!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps ||
                            // AMapsSettings.Generate_Nucleon_AMaps)),
                            //                                           CutManager.ThetaFD, "Neutron", RecoNeutronP, RecoNeutronTheta, RecoNeutronPhi, false);
                            bool TL_InFD = true;
                            // bool TL_InFD = aMaps_FCapplier.IsInFDQuery((!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps ||
                            // AMapsSettings.Generate_Nucleon_AMaps)),
                            //                                         CutManager.ThetaFD, "Neutron", TLNeutronP, TLNeutronTheta, TLNeutronPhi, false);
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
                                if (nRes_Pass_dThetaCut && nRes_Pass_dPhiCut) {
                                    double nResolution = (TLNeutronP - RecoNeutronP) / TLNeutronP;

                                    h_P_nFD_Res_VS_truth_P_nFD_matched_1e_cut->Fill(TLNeutronP, nResolution, weight);
                                    h_P_nFD_Res_VS_reco_P_nFD_matched_1e_cut->Fill(RecoNeutronP, nResolution, weight);

                                    h_P_nFD_Res_BysliceOf_truth_P_nFD_matched_1e_cut.Fill(TLNeutronP, nResolution, nResolution, weight);
                                    h_P_nFD_Res_BysliceOf_reco_P_nFD_matched_1e_cut.Fill(RecoNeutronP, nResolution, nResolution, weight);
                                }
                            }
                        }  // end of resolution calculation if
                    }  // end of if plot_and_fit_MomRes
                } else {
                    neutrons_FD_NotPassMatch.push_back(neutrons_FD_matched[i]);

                    int nFD_nSector = neutrons_FD_matched[i]->cal(detlayer)->getSector();
                    int e_nSector = electrons[0]->cal(detlayer)->getSector();

                    if (abs(nFD_nSector - e_nSector) == 3) {
                        neutrons_FD_NotPassMatch_OnlyGood.push_back(neutrons_FD_matched[i]);
                    } else {
                        neutrons_FD_NotPassMatch_OnlyBad.push_back(neutrons_FD_matched[i]);
                    }

                    h_reco_nFD_multi_VS_reco_P_nFD_NotPassMatch_matched_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_matched.size(), weight);
                }  // end of if else pass PassMatchingCuts
            }  // end of for loop over neutrons_FD_matched

            // Filling phFD momentum APID (matched):
            for (int i = 0; i < photons_FD_matched.size(); i++) { h_reco_P_phFD_APID_matched_1e_cut->Fill(photons_FD_matched[i]->par()->getP(), weight); }

            if (neutrons_FD_PassMatch.size() != 0) { h_reco_nFD_multi_AllN_PassMatch_matched_1e_cut->Fill(neutrons_FD_PassMatch.size(), weight); }
            if (neutrons_FD_PassMatch_OnlyGood.size() != 0) { h_reco_nFD_multi_OnlyGood_PassMatch_matched_1e_cut->Fill(neutrons_FD_PassMatch_OnlyGood.size(), weight); }
            if (neutrons_FD_PassMatch_OnlyBad.size() != 0) { h_reco_nFD_multi_OnlyBad_PassMatch_matched_1e_cut->Fill(neutrons_FD_PassMatch_OnlyBad.size(), weight); }

            if (neutrons_FD_NotPassMatch.size() != 0) { h_reco_nFD_multi_AllN_NotPassMatch_matched_1e_cut->Fill(neutrons_FD_NotPassMatch.size(), weight); }
            if (neutrons_FD_NotPassMatch_OnlyGood.size() != 0) { h_reco_nFD_multi_OnlyGood_NotPassMatch_matched_1e_cut->Fill(neutrons_FD_NotPassMatch_OnlyGood.size(), weight); }
            if (neutrons_FD_NotPassMatch_OnlyBad.size() != 0) { h_reco_nFD_multi_OnlyBad_NotPassMatch_matched_1e_cut->Fill(neutrons_FD_NotPassMatch_OnlyBad.size(), weight); }

#pragma endregion

        }  // end of if pass Pass_nFD_Multi_cut_matched && Pass_no_FD_photon_cut_matched

#pragma endregion

#pragma endregion

#pragma endregion

    }  // end of if while loop over events

#pragma endregion

#pragma region /* Organize histograms */

    /////////////////////////////////////////////////////
    // Organize histograms
    /////////////////////////////////////////////////////

    std::cout << "\033[33m\n\nOrganizeing histograms...\n\033[0m" << std::flush;

    cout << counter << endl;

    // A helper lambda that sets the style for plot lists
    auto SetHistogramListStyle = [](std::vector<TH1*>& histos) {
        for (auto* h : histos) {
            if (!h) continue;

            if (h->InheritsFrom("TH1D")) {
                h->Sumw2();
                h->SetMinimum(0);
                h->SetLineWidth(2);
                h->SetLineColor(kRed);
            }

            if (auto* xa = h->GetXaxis()) xa->CenterTitle();
            if (auto* ya = h->GetYaxis()) ya->CenterTitle();
        }
    };

    SetHistogramListStyle(HistoList_truth_1e_cut);
    SetHistogramListStyle(HistoList_RawTruth);
    SetHistogramListStyle(HistoList_PID_plots);
    SetHistogramListStyle(HistoList_raw);
    SetHistogramListStyle(HistoList_clas12reco);
    SetHistogramListStyle(HistoList_redef);
    SetHistogramListStyle(HistoList);

    // A helper lambda for centering axis titles
    auto SetHistogramListStyle_hsPlots = [](std::vector<TObject*> objList) {
        for (auto* obj : objList) {
            if (obj->InheritsFrom("TH1D")) {
                auto* h = (TH1D*)obj;
                h->Sumw2(), h->SetLineWidth(2), h->SetLineColor(kRed);
                h->GetXaxis()->CenterTitle(), h->GetYaxis()->CenterTitle();
            } else if (obj->InheritsFrom("TH2D")) {
                auto* h = (TH2D*)obj;
                h->GetXaxis()->CenterTitle(), h->GetYaxis()->CenterTitle();
            } else if (obj->InheritsFrom("TGraph")) {
                auto* g = (TGraph*)obj;
                g->GetXaxis()->CenterTitle(), g->GetYaxis()->CenterTitle();
            }
        }
    };

    SetHistogramListStyle_hsPlots(Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_ECALveto_1e_cut);
    SetHistogramListStyle_hsPlots(Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_ECALveto_1e_cut);
    SetHistogramListStyle_hsPlots(Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_ECALveto_1e_cut);

    SetHistogramListStyle_hsPlots(Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_matched_1e_cut);
    SetHistogramListStyle_hsPlots(Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_matched_1e_cut);
    SetHistogramListStyle_hsPlots(Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_matched_1e_cut);

    /////////////////////////////////////////////////////
    // Setting up detection efficiency plots
    /////////////////////////////////////////////////////

#pragma region /* Setting up detection efficiency plots */
    vector<vector<TH1*>> HistoList_eff_plots_3x1;
    vector<TH1*> HistoList_eff_plots_1x1;

    // h_truth_P_nFD_truth_1e_cut is the detection efficiency denominator histogram, common to all detection efficiency plots. Filled the truth P_nFD distribution that pass momentum and
    // fiducial cuts to obtain the (e,e')n events, under the one reco electron cut.
    TH1D* h_eff_P_nFD_1e_cut_denominator = (TH1D*)h_truth_P_nFD_truth_1e_cut->Clone((std::string(h_truth_P_nFD_truth_1e_cut->GetName()) + "_denominator").c_str());

    // Get detection efficiency as a function of reco P_nFD by dividing reco P_nFD by the denominator truth P_nFD
    TH1D* h_eff_P_nFD_ECALveto_1e_cut_numerator = (TH1D*)h_reco_P_nFD_ECALveto_1e_cut->Clone((std::string(h_reco_P_nFD_ECALveto_1e_cut->GetName()) + "_numerator").c_str());
    TH1D* h_eff_P_nFD_ECALveto_1e_cut = (TH1D*)h_eff_P_nFD_ECALveto_1e_cut_numerator->Clone("eff_P_nFD_ECALveto_1e_cut");
    h_eff_P_nFD_ECALveto_1e_cut->Divide(h_eff_P_nFD_1e_cut_denominator);
    h_eff_P_nFD_ECALveto_1e_cut->SetName("nFD_det_eff_reco_P_nFD_1e_cut");
    h_eff_P_nFD_ECALveto_1e_cut->GetYaxis()->SetTitle("Detection Efficiency");
    h_eff_P_nFD_ECALveto_1e_cut->SetTitle("nFD det. eff. from P^{reco}_{nFD}/P^{truth}_{nFD} in 1e cut");
    if (bt::FindSubstring(h_eff_P_nFD_ECALveto_1e_cut->GetTitle(), "det. eff.") && (h_eff_P_nFD_ECALveto_1e_cut->GetMaximum() > 1.)) { h_eff_P_nFD_ECALveto_1e_cut->SetMaximum(1.); }
    if (bt::FindSubstring(h_eff_P_nFD_ECALveto_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_ECALveto_1e_cut->GetXaxis()->SetRangeUser(0., 8.); }
    if (bt::FindSubstring(h_eff_P_nFD_ECALveto_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_ECALveto_1e_cut->GetYaxis()->SetRangeUser(0., 1.); }
    if (bt::FindSubstring(h_eff_P_nFD_ECALveto_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_ECALveto_1e_cut->SetStats(0); }
    HistoList_eff_plots_3x1.push_back({h_eff_P_nFD_ECALveto_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_ECALveto_1e_cut});
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_ECALveto_1e_cut_numerator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_1e_cut_denominator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_ECALveto_1e_cut);

    // Get detection efficiency as a function of truth P_nFD (matched to reco any nFD) by dividing truth P_nFD of matched neutrons by the denominator truth P_nFD from above
    // TODO: move to a differant numerator with the leading neutron, after talking with Adi
    // h_truth_P_nFD_matched_1e_cut is the detection efficiency numerator histogram. Filled the truth P_nFD distribution of the truth nFD that passes matching cuts with the i-th reco nFD.
    // This gives us (e,e'n) events.
    TH1D* h_eff_P_nFD_matched_1e_cut_numerator = (TH1D*)h_truth_P_nFD_matched_1e_cut->Clone((std::string(h_truth_P_nFD_matched_1e_cut->GetName()) + "_numerator").c_str());
    TH1D* h_eff_P_nFD_matched_1e_cut = (TH1D*)h_eff_P_nFD_matched_1e_cut_numerator->Clone("eff_P_nFD_matched_1e_cut");
    h_eff_P_nFD_matched_1e_cut->Divide(h_eff_P_nFD_1e_cut_denominator);
    h_eff_P_nFD_matched_1e_cut->SetName("nFD_det_eff_truth_P_nFD_matched_1e_cut");
    h_eff_P_nFD_matched_1e_cut->SetTitle("nFD det. eff. as a function of P^{truth}_{nFD} in 1e cut");
    h_eff_P_nFD_matched_1e_cut->GetXaxis()->SetTitle("P^{truth}_{nFD} [GeV/c]");
    h_eff_P_nFD_matched_1e_cut->GetYaxis()->SetTitle("Detection Efficiency");
    if (bt::FindSubstring(h_eff_P_nFD_matched_1e_cut->GetTitle(), "det. eff.") && (h_eff_P_nFD_matched_1e_cut->GetMaximum() > 1.)) { h_eff_P_nFD_matched_1e_cut->SetMaximum(1.); }
    if (bt::FindSubstring(h_eff_P_nFD_matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_matched_1e_cut->GetXaxis()->SetRangeUser(0., 8.); }
    if (bt::FindSubstring(h_eff_P_nFD_matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_matched_1e_cut->GetYaxis()->SetRangeUser(0., 1.); }
    if (bt::FindSubstring(h_eff_P_nFD_matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_matched_1e_cut->SetStats(0); }
    HistoList_eff_plots_3x1.push_back({h_eff_P_nFD_matched_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_matched_1e_cut});
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_matched_1e_cut_numerator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_1e_cut_denominator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_matched_1e_cut);

    // Get detection efficiency as a function of truth P_nFD (matched to reco LnFD) by dividing truth P_nFD of matched neutrons by the denominator truth P_nFD from above
    TH1D* h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut_numerator =
        (TH1D*)h_truth_P_nFD_matched_to_reco_LnFD_matched_1e_cut->Clone((std::string(h_truth_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetName()) + "_numerator").c_str());
    TH1D* h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut = (TH1D*)h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut_numerator->Clone("eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut");
    h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->Sumw2();
    h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->Divide(h_eff_P_nFD_1e_cut_denominator);
    h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->SetName("LnFD_det_eff_truth_P_LnFD_matched_to_reco_LnFD_matched_1e_cut");
    h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->SetTitle("LnFD det. eff. as a function of P^{truth}_{LnFD} in 1e cut matched to reco LnFD");
    h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetXaxis()->SetTitle("P^{truth}_{LnFD} [GeV/c]");
    h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetYaxis()->SetTitle("Detection Efficiency");
    if (bt::FindSubstring(h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetTitle(), "det. eff.") && (h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetMaximum() > 1.)) {
        h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->SetMaximum(1.);
    }
    if (bt::FindSubstring(h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetXaxis()->SetRangeUser(0., 8.); }
    if (bt::FindSubstring(h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetYaxis()->SetRangeUser(0., 1.); }
    if (bt::FindSubstring(h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut->SetStats(0); }
    HistoList_eff_plots_3x1.push_back({h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut});
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut_numerator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_1e_cut_denominator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut);

#pragma endregion

    /////////////////////////////////////////////////////
    // Now create the output PDFs
    /////////////////////////////////////////////////////
    int pixelx = 1980, pixely = 1530;
    TCanvas* myText = new TCanvas("myText", "myText", pixelx, pixely);
    TLatex titles, Slide_titles, subtitles, text;
    titles.SetTextSize(0.05), Slide_titles.SetTextSize(0.06), subtitles.SetTextSize(0.04), text.SetTextSize(0.03);

    gStyle->SetOptStat("ourmen");

#pragma region /* General histograms */

    if (plot_tester_histograms) {
        std::cout << "\033[33m\n\nCreating output PDFs...\n\033[0m" << std::flush;

#pragma region /* Helper lambdas for plotting histograms */

        // A helper function to draw the first pages:
        auto DrawIntroPages = [&](const std::string& TempfileName, const std::string& TempCustomTitle = "") {
            myText->cd();
            titles.DrawLatex(0.05, 0.90, (TempCustomTitle != "") ? TempCustomTitle.c_str() : "Uniform (e,e'n) events (truth-level)");

            subtitles.DrawLatex(0.05, 0.80, "Sample parameters");

            text.DrawLatex(0.05, 0.70, "OutFolderName:");
            text.DrawLatex(0.05, 0.65, ("#font[42]{" + OutFolderName + "}").c_str());

            text.DrawLatex(0.05, 0.55, "Sample properties:");
            text.DrawLatex(0.10, 0.50, ("Beam energy: #font[42]{" + Ebeam_status0 + "}").c_str());
            text.DrawLatex(0.10, 0.45, ("Sample name: #font[42]{" + SampleName + "}").c_str());
            text.DrawLatex(0.10, 0.40, ("Limiter: #font[42]{" + ToStringWithPrecision(Limiter, 0) + "}").c_str());

            text.DrawLatex(0.05, 0.30, "Run mode:");
            text.DrawLatex(0.10, 0.25, ("ConstrainedE: #font[42]{" + bt::BoolToString(ConstrainedE) + "}").c_str());
            text.DrawLatex(0.10, 0.20, ("OnlyGood_nFD: #font[42]{" + bt::BoolToString(OnlyGood_nFD) + "}").c_str());
            text.DrawLatex(0.10, 0.15, ("OnlyBad_nFD: #font[42]{" + bt::BoolToString(OnlyBad_nFD) + "}").c_str());

            myText->Print(TempfileName.c_str(), "pdf");
            myText->Clear();

            subtitles.DrawLatex(0.05, 0.90, "Run parameters");

            // text.DrawLatex(0.05, 0.80, "Code settings:");
            text.DrawLatex(0.05, 0.85, ("use_ConstPn_samples:               #font[42]{" + bt::BoolToString(use_ConstPn_samples) + "}").c_str());
            text.DrawLatex(0.05, 0.80, ("apply_neutFD_redef:                #font[42]{" + bt::BoolToString(apply_neutFD_redef) + "}").c_str());
            text.DrawLatex(0.05, 0.75, ("apply_cPart_ECAL_veto:             #font[42]{" + bt::BoolToString(apply_cPart_ECAL_veto) + "}").c_str());
            text.DrawLatex(0.05, 0.70, ("cPart_veto_radius (for ECAL veto): #font[42]{" + bt::ToStringWithPrecision(cPart_veto_radius, 0) + " [cm]}").c_str());
            text.DrawLatex(0.05, 0.65, ("apply_nPart_ECAL_veto:             #font[42]{" + bt::BoolToString(apply_nPart_ECAL_veto) + "}").c_str());
            text.DrawLatex(0.05, 0.60, ("nPart_veto_radius (for ECAL veto): #font[42]{" + bt::ToStringWithPrecision(nPart_veto_radius, 0) + " [cm]}").c_str());
            text.DrawLatex(0.05, 0.55, ("apply_no_same_sector_cuts:         #font[42]{" + bt::BoolToString(apply_no_same_sector_cuts) + "}").c_str());
            text.DrawLatex(0.05, 0.50, ("apply_nFD_multi_cut:               #font[42]{" + bt::BoolToString(apply_nFD_multi_cut) + "}").c_str());
            text.DrawLatex(0.05, 0.45, ("apply_no_FD_photons_cut:           #font[42]{" + bt::BoolToString(apply_no_FD_photons_cut) + "}").c_str());
            text.DrawLatex(0.05, 0.40, ("apply_no_other_part_multi_cut:     #font[42]{" + bt::BoolToString(apply_no_other_part_multi_cut) + "}").c_str());
            text.DrawLatex(0.05, 0.35, ("apply_theta_nFD_e_vs_dTheta_cuts:  #font[42]{" + bt::BoolToString(apply_theta_nFD_e_vs_dTheta_cuts) + "}").c_str());

            myText->Print(TempfileName.c_str(), "pdf");
            myText->Clear();

            subtitles.DrawLatex(0.05, 0.90, "Momentum thresholds");

            text.DrawLatex(0.05, 0.80, "Momentum thresholds (neutrals):");
            text.DrawLatex(0.10, 0.75, ("P_{nFD} threshold: #font[42]{" + bt::ToStringWithPrecision(nFD_mom_th.GetLowerCut(), 2) + " [GeV/c]}").c_str());
            text.DrawLatex(0.10, 0.70, ("P_{#gammaFD} threshold: #font[42]{" + bt::ToStringWithPrecision(phFD_mom_th.GetLowerCut(), 2) + " [GeV/c]}").c_str());

            text.DrawLatex(0.05, 0.60, "Momentum thresholds (other particles):");
            text.DrawLatex(0.10, 0.55, ("P_{pFD} threshold: #font[42]{" + bt::ToStringWithPrecision(pFD_mom_th.GetLowerCut(), 2) + " [GeV/c]}").c_str());
            text.DrawLatex(0.10, 0.50, ("P_{pCD} threshold: #font[42]{" + bt::ToStringWithPrecision(pCD_mom_th.GetLowerCut(), 2) + " [GeV/c]}").c_str());
            text.DrawLatex(0.10, 0.45, ("P_{#pi^{+}FD} threshold: #font[42]{" + bt::ToStringWithPrecision(pipFD_mom_th.GetLowerCut(), 2) + " [GeV/c]}").c_str());
            text.DrawLatex(0.10, 0.40, ("P_{#pi^{+}CD} threshold: #font[42]{" + bt::ToStringWithPrecision(pipCD_mom_th.GetLowerCut(), 2) + " [GeV/c]}").c_str());
            text.DrawLatex(0.10, 0.35, ("P_{#pi^{-}FD} threshold: #font[42]{" + bt::ToStringWithPrecision(pimFD_mom_th.GetLowerCut(), 2) + " [GeV/c]}").c_str());
            text.DrawLatex(0.10, 0.30, ("P_{#pi^{-}CD} threshold: #font[42]{" + bt::ToStringWithPrecision(pimCD_mom_th.GetLowerCut(), 2) + " [GeV/c]}").c_str());

            myText->Print(TempfileName.c_str(), "pdf");
            myText->Clear();
        };

        // A helper lambda to convert vector<TH1*> to vector<TObject*>
        auto ToObjVec = [](const std::vector<TH1*>& v) {
            std::vector<TObject*> out;
            out.reserve(v.size());

            for (auto* h : v) {
                if (h) { out.push_back((TObject*)h); }
            }

            return out;
        };

        // General, single-column multipage PDF writer (owns its canvas)
        auto GeneratePDFOutput = [&](const std::string& TempOutputDir, const std::string& TempOutFolderName, const std::vector<TObject*>& TempHistoList,
                                     const std::string& TempCustomTitle = "", bool add_subject_pages = false) {
            gStyle->SetOptStat("ourmen");

            // Dedicated canvas for this section
            TCanvas* page = new TCanvas(("page_" + TempOutFolderName).c_str(), ("page_" + TempOutFolderName).c_str(), pixelx, pixely);

            double originalTopMargin = page->GetTopMargin();
            double originalLeftMargin = page->GetLeftMargin();
            double originalRightMargin = page->GetRightMargin();
            double originalBottomMargin = page->GetBottomMargin();

            std::string PDF_fileName = TempOutputDir + "/" + TempOutFolderName + ".pdf";
            myText->SaveAs((PDF_fileName + "[").c_str());
            const char* fileName = PDF_fileName.c_str();

            auto MoveStatsBox = [&](TH1* h, double dx_offset, double dy_offset) {
                if (!h) { return; }

                gPad->Modified();
                gPad->Update();

                if (auto* stats = (TPaveStats*)h->FindObject("stats")) {
                    stats->SetX1NDC(stats->GetX1NDC() + dx_offset);
                    stats->SetY1NDC(stats->GetY1NDC() + dy_offset);
                    stats->SetX2NDC(stats->GetX2NDC() + dx_offset);
                    stats->SetY2NDC(stats->GetY2NDC() + dy_offset);
                    gPad->Modified();
                    gPad->Update();
                }
            };

            // Intro / title pages
            DrawIntroPages(fileName, TempCustomTitle);

            for (size_t i = 0; i < TempHistoList.size(); ++i) {
                auto* obj = TempHistoList[i];

                if (!obj) { continue; }

                // Optional subject divider pages
                if (add_subject_pages) {
                    for (size_t j = 0; j < HistSubjects.size(); ++j) {
                        if (FirstPrint.at(j) && FindSubstring(obj->GetTitle(), HistSubjects.at(j))) {
                            myText->cd();
                            titles.DrawLatex(0.3, 0.5, HistSubjects2.at(j).c_str());
                            myText->Print(fileName, "pdf");
                            myText->Clear();
                            FirstPrint.at(j) = false;

                            break;
                        }
                    }
                }

                const std::string title = obj->GetTitle();
                const bool Is_hsPlot = bt::FindSubstring(title, "Slice limits:");

                page->cd();
                page->cd()->SetGrid();
                page->cd()->SetBottomMargin(0.14);
                page->cd()->SetLeftMargin(0.16);
                page->cd()->SetRightMargin(Is_hsPlot ? 0.23 : 0.12);
                page->cd()->SetTopMargin(Is_hsPlot ? 0.16 : originalTopMargin);

                if (obj->InheritsFrom("TH1") || obj->InheritsFrom("TH2")) {
                    auto* h1 = (TH1*)obj;

                    if (auto* ya = h1->GetYaxis()) ya->SetTitleOffset(1.5);
                    if (auto* xa = h1->GetXaxis()) xa->SetTitleOffset(1.1);
                } else if (obj->InheritsFrom("TGraph")) {
                    auto* g = (TGraph*)obj;

                    if (auto* ya = g->GetYaxis()) ya->SetTitleOffset(1.5);
                    if (auto* xa = g->GetXaxis()) xa->SetTitleOffset(1.1);
                }

                if (obj->InheritsFrom("TH1D")) {
                    auto* h = (TH1D*)obj;
                    h->Draw();

                    if (bt::FindSubstring(h->GetTitle(), "det. eff.") && (h->GetMaximum() > 1.)) { h->SetMaximum(1.); }

                    gPad->Modified();
                    gPad->Update();

                    if (Is_hsPlot) MoveStatsBox(h, 0., -0.1);

                    if (bt::FindSubstring(h->GetTitle(), "det. eff.")) { h->SetStats(0); }
                } else if (obj->InheritsFrom("TH2D")) {
                    auto* h = (TH2D*)obj;
                    page->SetLogz(0);

                    if (bt::FindSubstring(h->GetName(), "PCAL") && !bt::FindSubstring(h->GetName(), "sampling fraction")) { page->SetLogz(1); }

                    h->Draw("colz");

                    gPad->Modified();
                    gPad->Update();

                    if (Is_hsPlot) MoveStatsBox(h, 0., -0.1);

                    if (h->GetEntries() != 0) {
                        gPad->Update();

                        if (auto* palette = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette")) {
                            palette->SetY2NDC(Is_hsPlot ? 0.45 : 0.55);
                            gPad->Modified();
                            gPad->Update();
                        }
                    }

                    page->SetLogz(1);
                    page->Print(fileName, "pdf");

                    page->SetLogz(0);
                } else if (obj->InheritsFrom("TGraph")) {
                    ((TGraph*)obj)->Draw("ap");
                }

                page->Print(fileName, "pdf");
                page->Clear();
            }

            page->Print((PDF_fileName + "]").c_str(), "pdf");

            hf::FixPDFOrientation(PDF_fileName);
            hf::ReassignPDFBookmarks(bt::GetCurrentDirectory() + "/", PDF_fileName, PDF_fileName);

            TFile* outFile_root = new TFile((TempOutputDir + "/" + TempOutFolderName + ".root").c_str(), "RECREATE");
            outFile_root->cd();

            for (auto* obj : TempHistoList) {
                if (obj) { obj->Write(); }
            }

            outFile_root->Close();

            delete page;
        };

        // Grid variant for grouped (e.g., 3x1) pages
        auto GeneratePDFOutputGrid = [&](const std::string& TempOutputDir, const std::string& TempOutFolderName, const std::vector<std::vector<TH1*>>& Groups, int cols, int rows,
                                         const std::string& TempCustomTitle = "") {
            gStyle->SetOptStat("ourmen");
            TCanvas* page = new TCanvas(("page_" + TempOutFolderName).c_str(), ("page_" + TempOutFolderName).c_str(), pixelx * cols, pixely * rows);

            std::string PDF_fileName = TempOutputDir + "/" + TempOutFolderName + ".pdf";
            myText->SaveAs((PDF_fileName + "[").c_str());
            const char* fileName = PDF_fileName.c_str();

            DrawIntroPages(fileName, TempCustomTitle);

            for (const auto& group : Groups) {
                page->Divide(cols, rows);

                for (size_t j = 0; j < group.size(); ++j) {
                    page->cd((int)j + 1);
                    gPad->SetGrid();
                    gPad->SetBottomMargin(0.14);
                    gPad->SetLeftMargin(0.16);
                    gPad->SetRightMargin(0.12);

                    if (auto* h1 = dynamic_cast<TH1*>(group[j])) {
                        if (auto* ya = h1->GetYaxis()) { ya->SetTitleOffset(1.5); }
                        if (auto* xa = h1->GetXaxis()) { xa->SetTitleOffset(1.1); }

                        if (bt::FindSubstring(h1->GetTitle(), "det. eff.") && (h1->GetMaximum() > 1.)) { h1->SetMaximum(1.); }
                        if (bt::FindSubstring(h1->GetTitle(), "det. eff.")) { h1->SetStats(0); }
                    }

                    if (group[j]->InheritsFrom("TH2D")) {
                        ((TH2D*)group[j])->Draw("colz");

                        gPad->Modified();
                        gPad->Update();

                        if (((TH2D*)group[j])->GetEntries() != 0) {
                            gPad->Update();

                            if (auto* palette = (TPaletteAxis*)((TH2D*)group[j])->GetListOfFunctions()->FindObject("palette")) {
                                palette->SetY2NDC(0.55);
                                gPad->Modified();
                                gPad->Update();
                            }
                        }
                    } else {
                        group[j]->Draw();
                    }
                }

                page->Print(fileName, "pdf");
                page->Clear();
            }

            page->Print((PDF_fileName + "]").c_str(), "pdf");

            hf::FixPDFOrientation(PDF_fileName);
            hf::ReassignPDFBookmarks(bt::GetCurrentDirectory() + "/", PDF_fileName, PDF_fileName);

            TFile* outFile_root = new TFile((TempOutputDir + "/" + TempOutFolderName + ".root").c_str(), "RECREATE");
            outFile_root->cd();

            for (const auto& group : Groups) {
                for (auto* h : group) {
                    if (h) { h->Write(); }
                }
            }

            outFile_root->Close();

            delete page;
        };

#pragma endregion

#pragma region /* Generate output PDFs for each histogram set */

#ifndef ENABLE_EFF_PLOT_OUTPUT
    #define ENABLE_EFF_PLOT_OUTPUT 1
        // #define ENABLE_EFF_PLOT_OUTPUT 0
#endif
        // RawTruth
        GeneratePDFOutput(OutputDir, "RawTruth", ToObjVec(HistoList_RawTruth), "Uniform (e,e'n) - RawTruth", /*add_subject_pages=*/true);

        // truth_1e_cut
        GeneratePDFOutput(OutputDir, "truth_1e_cut", ToObjVec(HistoList_truth_1e_cut), "Uniform (e,e'n) - truth_1e_cut", /*add_subject_pages=*/true);

        // PID
        GeneratePDFOutput(OutputDir, "PID_plots", ToObjVec(HistoList_PID_plots), "Uniform (e,e'n) - PID plots", /*add_subject_pages=*/true);

        // RAW
        GeneratePDFOutput(OutputDir, "raw_reco_nFD_plots", ToObjVec(HistoList_raw), "Uniform (e,e'n) - Raw plots", /*add_subject_pages=*/true);

        // clas12reco
        GeneratePDFOutput(OutputDir, "clas12reco_nFD_plots", ToObjVec(HistoList_clas12reco), "Uniform (e,e'n) - clas12reco plots", /*add_subject_pages=*/true);

        // redef
        GeneratePDFOutput(OutputDir, "redef_nFD_plots", ToObjVec(HistoList_redef), "Uniform (e,e'n) - redef plots", /*add_subject_pages=*/true);

        // ECALveto (your main HistoList)
        GeneratePDFOutput(OutputDir, OutFileName, ToObjVec(HistoList), "Uniform (e,e'n) - ECALveto plots", /*add_subject_pages=*/false);

        // Detection efficiency: 3x1 triplets
#if ENABLE_EFF_PLOT_OUTPUT
        GeneratePDFOutputGrid(OutputDir, "eff_plots_3x1", HistoList_eff_plots_3x1, 3, 1, "Uniform (e,e'n) - eff 3x1 plots");

        // Detection efficiency: 1x1 list
        GeneratePDFOutput(OutputDir, "eff_plots_1x1", ToObjVec(HistoList_eff_plots_1x1), "Uniform (e,e'n) - eff plots 1x1", /*add_subject_pages=*/false);
#endif

        // Sliced dTheta_n_e_VS_dPhi_n plots
        GeneratePDFOutput(PlotsBySliceOutputDir, "dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_ECALveto", Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_ECALveto_1e_cut,
                          "Uniform (e,e'n) - #delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} plots in slices of |#Deltar_{n}|");
        GeneratePDFOutput(PlotsBySliceOutputDir, "dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_matched", Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_matched_1e_cut,
                          "Uniform (e,e'n) - #delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} plots in slices of |#Deltar_{n}|");

        // Sliced P_nFD resolution plots
        GeneratePDFOutput(PlotsBySliceOutputDir, "Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_ECALveto_1e_cut", Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_ECALveto_1e_cut,
                          "Uniform (e,e'n) - Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_ECALveto_1e_cut");
        GeneratePDFOutput(PlotsBySliceOutputDir, "Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_ECALveto_1e_cut", Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_ECALveto_1e_cut,
                          "Uniform (e,e'n) - Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_ECALveto_1e_cut");
        GeneratePDFOutput(PlotsBySliceOutputDir, "Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_matched_1e_cut", Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_matched_1e_cut,
                          "Uniform (e,e'n) - Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_matched_1e_cut");
        GeneratePDFOutput(PlotsBySliceOutputDir, "Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_matched_1e_cut", Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_matched_1e_cut,
                          "Uniform (e,e'n) - Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_matched_1e_cut");

#pragma endregion

#pragma region /* Compare histograms */

        hf::CompareHistograms({h_reco_P_e_VS_P_nFD_ECALveto_1e_cut, h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut, h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut}, OutputDir,
                              "Histogram_Comparisons", "Reco_nFD_ECALveto_1e_cut");
        hf::CompareHistograms({h_reco_P_e_VS_P_nFD_matched_1e_cut, h_Edep_ECAL_nFD_VS_beta_nFD_matched_1e_cut, h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_matched_1e_cut}, OutputDir,
                              "Histogram_Comparisons", "Reco_nFD_matched_1e_cut");

        hf::CompareHistograms({h_reco_nFD_multi_AllN_ECALveto_1e_cut, h_reco_nFD_multi_OnlyGood_ECALveto_1e_cut, h_reco_nFD_multi_OnlyBad_ECALveto_1e_cut}, OutputDir,
                              "Histogram_Comparisons", "reco_nFD_multi_AllN_ECALveto_1e_cut");
        hf::CompareHistograms({h_reco_nFD_multi_AllN_PassMatch_matched_1e_cut, h_reco_nFD_multi_OnlyGood_PassMatch_matched_1e_cut, h_reco_nFD_multi_OnlyBad_PassMatch_matched_1e_cut},
                              OutputDir, "Histogram_Comparisons", "reco_nFD_multi_AllN_PassMatch_matched_1e_cut");

#if ENABLE_EFF_PLOT_OUTPUT
        hf::CompareHistograms({h_eff_P_nFD_ECALveto_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_ECALveto_1e_cut}, OutputDir, "Histogram_Comparisons",
                              "nFD_det_eff_by_reco_P_nFD_ECALveto_1e_cut");
        hf::CompareHistograms({h_eff_P_nFD_matched_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_matched_1e_cut}, OutputDir, "Histogram_Comparisons",
                              "nFD_det_eff_by_truth_P_nFD_matched_1e_cut");
        hf::CompareHistograms({h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_matched_to_reco_LnFD_matched_1e_cut}, OutputDir,
                              "Histogram_Comparisons", "nFD_det_eff_by_truth_P_nFD_matched_to_reco_LnFD_matched_1e_cut");
#endif

#pragma endregion
    }

#pragma endregion

#pragma region /* Acceptance maps */

    if (plot_AMaps) {
        std::cout << "\033[33m\n\nSaving AMaps plots...\n\033[0m" << std::flush;

        TCanvas* myCanvas_aMaps = new TCanvas("myCanvas_aMaps", "myCanvas_aMaps", pixelx, pixely);

        myCanvas_aMaps->cd();
        myCanvas_aMaps->SetBottomMargin(0.14), myCanvas_aMaps->SetLeftMargin(0.16), myCanvas_aMaps->SetRightMargin(0.12);

        aMaps_generator.DrawAndSaveMaps(SampleName, myCanvas_aMaps, OutputDirAMapsMaps);
    }

#pragma endregion

#pragma region /* Momentum resolution */

    // if (plot_and_fit_MomRes) {
    //     std::cout << "\033[33m\n\nSaving momentum resolution plots plots...\n\033[0m" << std::flush;

    //     TCanvas* myCanvas_MomRes = new TCanvas("myCanvas_MomRes", "myCanvas_MomRes", pixelx, pixely);

    //     nRes.SliceFitDrawAndSaveByType(SampleName, Ebeam);
    //     nRes.LogResDataToFile(SampleName, OutputDirMomResPlots, path_definitions::PathDefinitions.MomentumResolutionDirectory);
    //     nRes.DrawAndSaveResSlices(SampleName, myCanvas_MomRes, OutputDirMomResPlots, path_definitions::PathDefinitions.MomentumResolutionDirectory);
    // }

#pragma endregion

#pragma endregion

    gDirectory->Clear();

    ++LoopNumber;
}  // end of GammaNeutronFD_separation_test function
