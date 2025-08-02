// framework nFD_eff_tester_run.sh
// git pull && clas12root -b -q testers/nFD_eff_test.cpp

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include "GammaNeutronFD_separation_test_functions.cpp"

namespace bt = basic_tools;
// namespace am = analysis_math;
// namespace raf = reco_analysis_functions;
namespace hf = histogram_functions;

void GammaNeutronFD_separation_test() {
    cout << "\n\nInitiating GammaNeutronFD_separation_test.cpp\n";

    int version = 25;

    bool use_ConstPn_samples = false;

    vector<double> cPart_veto_radii = {100};
    vector<double> nPart_veto_radii = {100, 125, 150, 175, 200, 250};

    vector<double> Ebeam_v = {2.07052};
    vector<vector<bool>> Ebeam_bool_v = {{true, false, false}};
    // vector<double> Ebeam_v = {4.02962};
    // vector<vector<bool>> Ebeam_bool_v = {{false, true, false}};
    // vector<double> Ebeam_v = {5.98636};
    // vector<vector<bool>> Ebeam_bool_v = {{false, false, true}};
    // vector<double> Ebeam_v = {2.07052, 4.02962, 5.98636};
    // vector<vector<bool>> Ebeam_bool_v = {{true, false, false}, {false, true, false}, {false, false, true}};

    // int Limiter = 25000000;  // 2500 files
    // int Limiter = 10000000;  // 1000 files
    int Limiter = 1000000;  // 100 files
    // int Limiter = 100000;  // 10 files
    // int Limiter = 10000;  // 1 file

    bool apply_nFD_multi_cut = false;

    bool apply_ECAL_veto = false;

    bool apply_dTheta_cuts = true;
    bool apply_dPhi_cuts = true;

    bool apply_neutFD_redef = true;

    bool apply_PCAL_neutral_veto = false;

    // CutSelector = {ConstrainedE, OnlyGood_nFD, OnlyBad_nFD}
    // vector<vector<bool>> CutSelector = {{false, false, true}};  // OnlyBad and no ConstrainedE
    // vector<vector<bool>> CutSelector = {{false, false, true}, {true, false, true}};  // OnlyBad and with and without ConstrainedE
    // vector<vector<bool>> CutSelector = {{false, false, false}, {false, true, false}, {false, false, true}};  // OnlyGood and OnlyBad without ConstrainedE
    // vector<vector<bool>> CutSelector = {{true, false, false}, {true, true, false}, {true, false, true}};   // OnlyGood and OnlyBad with ConstrainedE
    vector<vector<bool>> CutSelector = {{true, false, false},  {true, true, false},  {true, false, true},
                                        {false, false, false}, {false, true, false}, {false, false, true}};  // Run all combinations

    bool ConstrainTLmom = false;

    bool plot_tester_histograms = true;

    bool plot_AMaps = false;
    // bool plot_AMaps = true;

    std::string OutFolderName_prefix = "a" + bt::ToStringWithPrecision(version, 0) + "_2N_analyzer_";
    std::string OutFolderName_ver_status = "_v" + bt::ToStringWithPrecision(version, 0);
    std::string samples_status = use_ConstPn_samples ? "_CPn" : "";
    std::string nFD_multi_status = apply_nFD_multi_cut ? "_wNMC" : "_woNMC";
    std::string dTheta_status = "";
    // std::string dTheta_status = apply_dTheta_cuts ? "_wdThetaC" : "_wodThetaC";
    std::string dPhi_status = apply_dPhi_cuts ? "_wdPhiC" : "_wodPhiC";
    std::string neutFD_redef_status = apply_neutFD_redef ? "_RDed" : "_c12n";
    std::string ECAL_veto_status = apply_ECAL_veto ? "_wEV" : "_woEV";
    std::string PCAL_neutral_veto_status = "";
    // std::string PCAL_neutral_veto_status = apply_PCAL_neutral_veto ? "_wPnV" : "_woPnV";
    std::string General_status = "small_EV_test";

    nPart_veto_radii = (!apply_PCAL_neutral_veto) ? vector<double>{0} : nPart_veto_radii;

    for (int Selector_ind = 0; Selector_ind < CutSelector.size(); Selector_ind++) {
        for (int rc_ind = 0; rc_ind < cPart_veto_radii.size(); rc_ind++) {
            for (int rn_ind = 0; rn_ind < nPart_veto_radii.size(); rn_ind++) {
                for (int Ebeam_ind = 0; Ebeam_ind < Ebeam_v.size(); Ebeam_ind++) {
#pragma region /* Setup */
                    double Ebeam = Ebeam_v.at(Ebeam_ind);
                    bool Is2GeV = Ebeam_bool_v.at(Ebeam_ind).at(0), Is4GeV = Ebeam_bool_v.at(Ebeam_ind).at(1), Is6GeV = Ebeam_bool_v.at(Ebeam_ind).at(2);

                    double cPart_veto_radius = cPart_veto_radii.at(rc_ind), nPart_veto_radius = nPart_veto_radii.at(rn_ind);

                    bool ConstrainedE = CutSelector.at(Selector_ind).at(0);
                    bool OnlyGood_nFD = CutSelector.at(Selector_ind).at(1);
                    bool OnlyBad_nFD = CutSelector.at(Selector_ind).at(2);

                    std::string Ebeam_status = Is2GeV ? "_2GeV" : Is4GeV ? "_4GeV" : Is6GeV ? "_6GeV" : "_Unknown";
                    std::string Ebeam_status0 = Is2GeV ? "2GeV" : Is4GeV ? "4GeV" : Is6GeV ? "6GeV" : "Unknown";
                    std::string rc_factor_status = apply_ECAL_veto ? "_rc" + ToStringWithPrecision(cPart_veto_radius, 0) : "";
                    std::string nPart_veto_radius_status = apply_PCAL_neutral_veto ? "_rn" + ToStringWithPrecision(nPart_veto_radius, 0) : "";
                    std::string Good_nFD_status = (OnlyGood_nFD && !OnlyBad_nFD) ? "_OnlyGood_nFD" : "";
                    std::string Bad_nFD_status = (!OnlyGood_nFD && OnlyBad_nFD) ? "_OnlyBad_nFD" : "";
                    std::string ConstrainedE_status = ConstrainedE ? "_CE" : "";

                    std::string OutFolderName = OutFolderName_prefix + OutFolderName_ver_status + Ebeam_status + samples_status + nFD_multi_status + dTheta_status + dPhi_status +
                                                neutFD_redef_status + ECAL_veto_status + PCAL_neutral_veto_status + rc_factor_status + nPart_veto_radius_status + Good_nFD_status +
                                                Bad_nFD_status + ConstrainedE_status + General_status;

                    std::string OutFileName = OutFolderName;

                    if (OnlyGood_nFD == OnlyBad_nFD) { OnlyGood_nFD = OnlyBad_nFD = false; }

                    double P_upperLim = ConstrainTLmom ? (Ebeam * 0.5) : (Ebeam * 1.1);
                    const std::string OutputDir = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName;
                    system(("rm -rf " + OutputDir).c_str());
                    system(("mkdir -p " + OutputDir).c_str());

                    TFile* outFile = new TFile(("/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/" + OutFileName + ".root").c_str(), "RECREATE");

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
                        // InputFiles = use_ConstPn_samples ? (BaseDir + "/5986MeV_ConstPn_lH2/OutPut_en/reconhipo/*.hipo") : (BaseDir + "/5986MeV/OutPut_en/reconhipo/*.hipo");
                        InputFiles = use_ConstPn_samples ? (BaseDir + "/5986MeV_ConstPn/OutPut_en/reconhipo/*.hipo") : (BaseDir + "/5986MeV/OutPut_en/reconhipo/*.hipo");

                        SampleName = "Uniform_en_sample_5986MeV";
                        Beam_energy_TString = "5986MeV";
                    }

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
                    vector<TH1*> HistoList_electron_cuts;

                    vector<TH1*> HistoList_raw;

                    vector<TH1*> HistoList_clas12reco;

                    vector<TH1*> HistoList_redef;

                    vector<TH1*> HistoList;
                    vector<string> HistSubjects;
                    vector<string> HistSubjects2;
                    vector<bool> FirstPrint;

                    gStyle->SetTitleXSize(0.05);
                    gStyle->SetTitleYSize(0.05);

                    gStyle->SetTitleXOffset(0.8);
                    gStyle->SetTitleYOffset(0.8);

                    char temp_name[100];
                    char temp_title[100];

#pragma region /* RAW */

#pragma region /* Electron pre-selection */
                    TH1D* h_Vz_e_BC_1e_cut = new TH1D("Vz_e_BC_1e_cut", "V_{z}^{e} in 1e cut (before cut);V_{z}^{e} [cm];Counts", 50, -8, 8);
                    HistoList_electron_cuts.push_back(h_Vz_e_BC_1e_cut);
                    TH1D* h_Vz_e_AC_1e_cut = new TH1D("Vz_e_AC_1e_cut", "V_{z}^{e} in 1e cut (after cut);V_{z}^{e} [cm];Counts", 50, -8, 8);
                    HistoList_electron_cuts.push_back(h_Vz_e_AC_1e_cut);

                    TH2D* h_dc_electron_hit_map_BC_1e_cut[4];  // 3 regions
                    TH2D* h_dc_electron_hit_map_AC_1e_cut[4];  // 3 regions

                    // DC hit maps
                    for (int i = 1; i <= 3; i++) {
                        h_dc_electron_hit_map_BC_1e_cut[i] =
                            new TH2D(Form("dc_electron_hit_map_BC_%d", i), Form("DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
                        HistoList_electron_cuts.push_back(h_dc_electron_hit_map_BC_1e_cut[i]);
                        h_dc_electron_hit_map_AC_1e_cut[i] =
                            new TH2D(Form("dc_electron_hit_map_AC_%d", i), Form("DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
                        HistoList_electron_cuts.push_back(h_dc_electron_hit_map_AC_1e_cut[i]);
                    }
#pragma endregion

#pragma region /* Electron PID */
                    TH1D* h_nphe_BC_1e_cut = new TH1D("nphe_BC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut);Number of photo-electrons;Counts", 20, 0, 20);
                    HistoList_electron_cuts.push_back(h_nphe_BC_1e_cut);
                    TH1D* h_nphe_AC_1e_cut = new TH1D("nphe_AC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut);Number of photo-electrons;Counts", 20, 0, 20);
                    HistoList_electron_cuts.push_back(h_nphe_AC_1e_cut);

                    TH2D* h_Edep_PCAL_VS_EC_BC_1e_cut = new TH2D(
                        "Edep_PCAL_VS_EC_BC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100,
                        0, 0.2, 100, 0, 0.3);
                    HistoList_electron_cuts.push_back(h_Edep_PCAL_VS_EC_BC_1e_cut);
                    TH2D* h_Edep_PCAL_VS_EC_AC_1e_cut = new TH2D(
                        "Edep_PCAL_VS_EC_AC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100,
                        0, 0.2, 100, 0, 0.3);
                    HistoList_electron_cuts.push_back(h_Edep_PCAL_VS_EC_AC_1e_cut);

                    TH2D* h_SF_VS_P_e_BC_1e_cut = new TH2D("SF_VS_P_e_BC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut);P_{e} [GeV/c];Electron sampling fraction", 100,
                                                           0, Ebeam * 1.1, 100, 0.16, 0.3);
                    HistoList_electron_cuts.push_back(h_SF_VS_P_e_BC_1e_cut);
                    TH2D* h_SF_VS_P_e_AC_1e_cut = new TH2D("SF_VS_P_e_AC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut);P_{e} [GeV/c];Electron sampling fraction", 100,
                                                           0, Ebeam * 1.1, 100, 0.16, 0.3);
                    HistoList_electron_cuts.push_back(h_SF_VS_P_e_AC_1e_cut);

                    TH2D* h_SF_VS_Lv_BC_1e_cut =
                        new TH2D("SF_VS_Lv_BC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
                    HistoList_electron_cuts.push_back(h_SF_VS_Lv_BC_1e_cut);
                    TH2D* h_SF_VS_Lv_AC_1e_cut =
                        new TH2D("SF_VS_Lv_AC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
                    HistoList_electron_cuts.push_back(h_SF_VS_Lv_AC_1e_cut);

                    TH2D* h_SF_VS_Lw_BC_1e_cut =
                        new TH2D("SF_VS_Lw_BC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
                    HistoList_electron_cuts.push_back(h_SF_VS_Lw_BC_1e_cut);
                    TH2D* h_SF_VS_Lw_AC_1e_cut =
                        new TH2D("SF_VS_Lw_AC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
                    HistoList_electron_cuts.push_back(h_SF_VS_Lw_AC_1e_cut);

                    TH2D* h_SF_VS_Lu_BC_1e_cut =
                        new TH2D("SF_VS_Lu_BC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
                    HistoList_electron_cuts.push_back(h_SF_VS_Lu_BC_1e_cut);
                    TH2D* h_SF_VS_Lu_AC_1e_cut =
                        new TH2D("SF_VS_Lu_AC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
                    HistoList_electron_cuts.push_back(h_SF_VS_Lu_AC_1e_cut);

                    TH2D* h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut =
                        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3,
                                 100, 0, 0.35);
                    HistoList_electron_cuts.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut);
                    TH2D* h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut =
                        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3,
                                 100, 0, 0.35);
                    HistoList_electron_cuts.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut);

#pragma endregion

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
                        new TH2D("reco_theta_e_VS_reco_phi_e_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{e} in 1e cut;#phi^{reco}_{e} [#circ];#theta^{reco}_{e} [#circ]", 100, -180., 180.,
                                 100, 0, 50.);
                    HistoList_raw.push_back(h_reco_theta_e_VS_reco_phi_e_1e_cut);
                    TH2D* h_truth_theta_e_VS_truth_phi_e_1e_cut =
                        new TH2D("truth_theta_e_VS_truth_phi_e_1e_cut", "#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut;#phi^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]", 100, -180.,
                                 180., 100, 0, 50.);
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
                        new TH2D("reco_theta_n_VS_reco_phi_n_1e_cut", "#theta^{reco}_{n} vs. #phi^{reco}_{n} in 1e cut;#phi^{reco}_{n} [#circ];#theta^{reco}_{n} [#circ]", 100, -180., 180.,
                                 100, 0, 50.);
                    HistoList_raw.push_back(h_reco_theta_n_VS_reco_phi_n_1e_cut);
                    TH2D* h_truth_theta_n_VS_truth_phi_n_1e_cut =
                        new TH2D("truth_theta_n_VS_truth_phi_n_1e_cut", "#theta^{truth}_{n} vs. #phi^{truth}_{n} in 1e cut;#phi^{truth}_{n} [#circ];#theta^{truth}_{n} [#circ]", 100, -180.,
                                 180., 100, 0, 50.);
                    HistoList_raw.push_back(h_truth_theta_n_VS_truth_phi_n_1e_cut);
#pragma endregion

#pragma region /* clas12reco */
                    HistSubjects.push_back("clas12reco");
                    HistSubjects2.push_back("#splitline{FD neutron from}{clas12reco}");
                    FirstPrint.push_back(true);
                    TH1D* h_reco_P_nFD_clas12_1e_cut = new TH1D("reco_P_nFD_clas12_1e_cut", "P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
                    HistoList_clas12reco.push_back(h_reco_P_nFD_clas12_1e_cut);
                    TH1D* h_truth_P_nFD_clas12_1e_cut = new TH1D("truth_P_nFD_clas12_1e_cut", "P^{truth}_{nFD} in 1e cut (clas12reco);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
                    HistoList_clas12reco.push_back(h_truth_P_nFD_clas12_1e_cut);
                    TH1D* h_reco_theta_nFD_clas12_1e_cut =
                        new TH1D("reco_theta_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
                    HistoList_clas12reco.push_back(h_reco_theta_nFD_clas12_1e_cut);
                    TH1D* h_truth_theta_nFD_clas12_1e_cut =
                        new TH1D("truth_theta_nFD_clas12_1e_cut", "#theta^{truth}_{nFD} in 1e cut (clas12reco);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
                    HistoList_clas12reco.push_back(h_truth_theta_nFD_clas12_1e_cut);
                    TH1D* h_reco_phi_nFD_clas12_1e_cut = new TH1D("reco_phi_nFD_clas12_1e_cut", "#phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
                    HistoList_clas12reco.push_back(h_reco_phi_nFD_clas12_1e_cut);
                    TH1D* h_truth_phi_nFD_clas12_1e_cut =
                        new TH1D("truth_phi_nFD_clas12_1e_cut", "#phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
                    HistoList_clas12reco.push_back(h_truth_phi_nFD_clas12_1e_cut);
                    TH2D* h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut =
                        new TH2D("reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut",
                                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
                    HistoList_clas12reco.push_back(h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut);
                    TH2D* h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut =
                        new TH2D("truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut",
                                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
                    HistoList_clas12reco.push_back(h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut);
                    TH2D* h_reco_P_e_VS_P_nFD_clas12_1e_cut =
                        new TH2D("reco_P_e_VS_P_nFD_clas12_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2.,
                                 100, 0., Ebeam * 3.);
                    HistoList_clas12reco.push_back(h_reco_P_e_VS_P_nFD_clas12_1e_cut);
                    TH2D* h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut =
                        new TH2D("reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut",
                                 "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0., 50., 100, 0., Ebeam * 3.);
                    HistoList_clas12reco.push_back(h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut);
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
                        new TH2D("reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut",
                                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (redef);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
                    HistoList_redef.push_back(h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut);
                    TH2D* h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut =
                        new TH2D("truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut",
                                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (redef);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
                    HistoList_redef.push_back(h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut);
                    TH2D* h_reco_P_e_VS_P_nFD_redef_1e_cut =
                        new TH2D("reco_P_e_VS_P_nFD_redef_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (redef);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100,
                                 0., Ebeam * 3.);
                    HistoList_redef.push_back(h_reco_P_e_VS_P_nFD_redef_1e_cut);
                    TH2D* h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut =
                        new TH2D("reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (redef);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]",
                                 100, 0., 50., 100, 0., Ebeam * 3.);
                    HistoList_redef.push_back(h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut);
#pragma endregion

#pragma region /* ECALveto */
                    HistSubjects.push_back("ECALveto");
                    HistSubjects2.push_back("#splitline{ECALveto}{and P_{nFD} thresholds}");
                    FirstPrint.push_back(true);

#pragma region /* Base histograms (ECALveto) */
                    TH1D* h_reco_P_nFD_ECALveto_1e_cut = new TH1D("reco_P_nFD_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
                    HistoList.push_back(h_reco_P_nFD_ECALveto_1e_cut);
                    TH1D* h_truth_P_nFD_ECALveto_1e_cut = new TH1D("truth_P_nFD_ECALveto_1e_cut", "P^{truth}_{nFD} in 1e cut (ECALveto);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
                    HistoList.push_back(h_truth_P_nFD_ECALveto_1e_cut);
                    TH1D* h_reco_theta_nFD_ECALveto_1e_cut =
                        new TH1D("reco_theta_nFD_ECALveto_1e_cut", "#theta^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
                    HistoList.push_back(h_reco_theta_nFD_ECALveto_1e_cut);
                    TH1D* h_truth_theta_nFD_ECALveto_1e_cut =
                        new TH1D("truth_theta_nFD_ECALveto_1e_cut", "#theta^{truth}_{nFD} in 1e cut (ECALveto);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
                    HistoList.push_back(h_truth_theta_nFD_ECALveto_1e_cut);
                    TH1D* h_reco_phi_nFD_ECALveto_1e_cut =
                        new TH1D("reco_phi_nFD_ECALveto_1e_cut", "#phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
                    HistoList.push_back(h_reco_phi_nFD_ECALveto_1e_cut);
                    TH1D* h_truth_phi_nFD_ECALveto_1e_cut =
                        new TH1D("truth_phi_nFD_ECALveto_1e_cut", "#phi^{truth}_{nFD} in 1e cut (ECALveto);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
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
                    TH1D* h_reco_theta_LnFD_ECALveto_1e_cut =
                        new TH1D("reco_theta_LnFD_ECALveto_1e_cut", "#theta^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{LnFD} [#circ];Counts", 50, 0, 45.);
                    HistoList.push_back(h_reco_theta_LnFD_ECALveto_1e_cut);
                    TH1D* h_reco_phi_LnFD_ECALveto_1e_cut =
                        new TH1D("reco_phi_LnFD_ECALveto_1e_cut", "#phi^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{LnFD} [#circ];Counts", 50, -180, 180.);
                    HistoList.push_back(h_reco_phi_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut",
                                 "#theta^{reco}_{LnFD} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{LnFD} [#circ];#theta^{reco}_{LnFD} [#circ]", 100, -180., 180., 100, 0, 50.);
                    HistoList.push_back(h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut);

                    TH1D* h_Edep_ECAL_nFD_ECALveto_1e_cut = new TH1D("Edep_ECAL_nFD_ECALveto_1e_cut", "E^{ECAL}_{dep} in 1e cut (ECALveto);E^{ECAL}_{dep} [GeV];Counts", 50, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_nFD_ECALveto_1e_cut);
                    TH1D* h_beta_nFD_ECALveto_1e_cut = new TH1D("beta_nFD_ECALveto_1e_cut", "#beta_{nFD} in 1e cut (ECALveto);#beta_{nFD};Counts", 50, 0.3, 1.1);
                    HistoList.push_back(h_beta_nFD_ECALveto_1e_cut);
                    TH2D* h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut = new TH2D(
                        "Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. #beta_{nFD} in 1e cut (ECALveto);#beta_{nFD};E^{ECAL}_{dep} [GeV]", 100, 0.3, 1.1, 100, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut);

                    TH1D* h_Edep_ECAL_LnFD_ECALveto_1e_cut = new TH1D("Edep_ECAL_LnFD_ECALveto_1e_cut", "E^{ECAL}_{dep} in 1e cut (ECALveto);E^{ECAL}_{dep} [GeV];Counts", 50, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_LnFD_ECALveto_1e_cut);
                    TH1D* h_beta_LnFD_ECALveto_1e_cut = new TH1D("beta_LnFD_ECALveto_1e_cut", "#beta_{LnFD} in 1e cut (ECALveto);#beta_{LnFD};Counts", 50, 0.3, 1.1);
                    HistoList.push_back(h_beta_LnFD_ECALveto_1e_cut);
                    TH2D* h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut = new TH2D(
                        "Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. #beta_{LnFD} in 1e cut (ECALveto);#beta_{LnFD};E^{ECAL}_{dep} [GeV]", 100, 0.3, 1.1, 100, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut);

                    TH2D* h_reco_P_e_VS_P_nFD_ECALveto_1e_cut =
                        new TH2D("reco_P_e_VS_P_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2.,
                                 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_e_VS_P_nFD_ECALveto_1e_cut);
                    TH2D* h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut =
                        new TH2D("reco_theta_e_VS_P_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100,
                                 0, 45., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut);
                    TH2D* h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut =
                        new TH2D("reco_phi_e_VS_P_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, -180.,
                                 180., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut);
                    TH2D* h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut =
                        new TH2D("reco_P_e_VS_theta_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#theta^{reco}_{nFD} [#circ]", 100,
                                 0., P_upperLim, 100, 0, 45.);
                    HistoList.push_back(h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut);
                    TH2D* h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut =
                        new TH2D("reco_theta_e_VS_theta_nFD_ECALveto_1e_cut",
                                 "#theta^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, 0, 45., 100, 0, 45.);
                    HistoList.push_back(h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut);
                    TH2D* h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut =
                        new TH2D("reco_phi_e_VS_theta_nFD_ECALveto_1e_cut",
                                 "#phi^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 45.);
                    HistoList.push_back(h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut);
                    TH2D* h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut =
                        new TH2D("reco_P_e_VS_phi_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#phi^{reco}_{nFD} [#circ]", 100, 0.,
                                 P_upperLim, 100, -180., 180.);
                    HistoList.push_back(h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut);
                    TH2D* h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut =
                        new TH2D("reco_phi_e_VS_phi_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]",
                                 100, 0, 45., 100, -180., 180.);
                    HistoList.push_back(h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut);
                    TH2D* h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut =
                        new TH2D("reco_phi_e_VS_phi_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100,
                                 -180., 180., 100, -180., 180.);
                    HistoList.push_back(h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut);
                    TH2D* h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut =
                        new TH2D("reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut",
                                 "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0., 45., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut);
                    TH1D* h_reco_nFD_multi_ECALveto_1e_cut = new TH1D("reco_nFD_multi_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (ECALveto);nFD multiplicity;Counts", 9, 1, 10);
                    HistoList.push_back(h_reco_nFD_multi_ECALveto_1e_cut);
                    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut =
                        new TH2D("reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut", "reco nFD multiplicity vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100,
                                 0., Ebeam * 3., 9, 1, 10);
                    HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut);

                    TH2D* h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_P_e_VS_P_LnFD_ECALveto_1e_cut", "P^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];P^{reco}_{LnFD} [GeV/c]", 100, 0.,
                                 Ebeam * 2., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_theta_e_VS_P_LnFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];P^{reco}_{LnFD} [GeV/c]",
                                 100, 0, 45., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_phi_e_VS_P_LnFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];P^{reco}_{LnFD} [GeV/c]", 100,
                                 -180., 180., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_P_e_VS_theta_LnFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#theta^{reco}_{LnFD} [#circ]",
                                 100, 0., P_upperLim, 100, 0, 45.);
                    HistoList.push_back(h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut",
                                 "#theta^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#theta^{reco}_{LnFD} [#circ]", 100, 0, 45., 100, 0, 45.);
                    HistoList.push_back(h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut",
                                 "#phi^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#theta^{reco}_{LnFD} [#circ]", 100, -180., 180., 100, 0, 45.);
                    HistoList.push_back(h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_P_e_VS_phi_LnFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#phi^{reco}_{LnFD} [#circ]", 100, 0.,
                                 P_upperLim, 100, -180., 180.);
                    HistoList.push_back(h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut",
                                 "#theta^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#phi^{reco}_{LnFD} [#circ]", 100, 0, 45., 100, -180., 180.);
                    HistoList.push_back(h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#phi^{reco}_{LnFD} [#circ]",
                                 100, -180., 180., 100, -180., 180.);
                    HistoList.push_back(h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut);
                    TH2D* h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut",
                                 "#theta^{reco}_{LnFD} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{LnFD} [#circ];P^{reco}_{LnFD} [GeV/c]", 100, 0., 45., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut);
                    TH1D* h_reco_LnFD_multi_ECALveto_1e_cut = new TH1D("reco_LnFD_multi_ECALveto_1e_cut", "reco LnFD multiplicity in 1e cut (ECALveto);LnFD multiplicity;Counts", 9, 1, 10);
                    HistoList.push_back(h_reco_LnFD_multi_ECALveto_1e_cut);
                    TH2D* h_reco_LnFD_multi_VS_reco_P_LnFD_ECALveto_1e_cut =
                        new TH2D("reco_LnFD_multi_VS_reco_P_LnFD_ECALveto_1e_cut",
                                 "reco LnFD multiplicity vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];LnFD multiplicity", 100, 0., Ebeam * 3., 9, 1, 10);
                    HistoList.push_back(h_reco_LnFD_multi_VS_reco_P_LnFD_ECALveto_1e_cut);
#pragma endregion

                    // TH2D* h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut =
                    //     new TH2D("reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (ECALveto);L^{reco}_{nFD,ECAL} [cm];P^{reco}_{nFD}
                    //     [#circ]", 100,
                    //              700., 850., 100, 0., Ebeam * 3.);
                    // HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut);
                    // TH2D* h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut =
                    //     new TH2D("reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut",
                    //              "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (ECALveto);L^{reco}_{nFD,ECAL} [cm];#theta^{reco}_{nFD} [#circ]", 100, 700., 850., 100, 0., 45.);
                    // HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut);
                    // TH2D* h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut =
                    //     new TH2D("reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (ECALveto);L^{reco}_{nFD,ECAL} [cm];#phi^{reco}_{nFD}
                    //     [#circ]",
                    //              100, 700., 850., 100, -180., 180.);
                    // HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut);
                    // TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut =
                    //     new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut",
                    //              "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECAL} [ns];P^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., Ebeam
                    //              * 3.);
                    // HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut);
                    // TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut =
                    //     new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut",
                    //              "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECAL} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100,
                    //              0., 45.);
                    // HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut);
                    // TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut =
                    //     new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut",
                    //              "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECAL} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180.,
                    //              180.);
                    // HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut);
                    // // TH2D* h_reco_P_nFD_VS_eff_L_ECAL_ECALveto_1e_cut =
                    // //     new TH2D("reco_P_nFD_VS_eff_L_ECAL_ECALveto_1e_cut",
                    // //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (ECALveto);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD}
                    // [#circ]", 100,
                    // //              700., 850., 100, 0., Ebeam * 3.);
                    // // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECAL_ECALveto_1e_cut);
                    // // TH2D* h_reco_theta_nFD_VS_eff_L_ECAL_ECALveto_1e_cut =
                    // //     new TH2D("reco_theta_nFD_VS_eff_L_ECAL_ECALveto_1e_cut",
                    // //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (ECALveto);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF}
                    // [cm];#theta^{reco}_{nFD}
                    // //              [#circ]", 100, 700., 850., 100, 0., 45.);
                    // // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECAL_ECALveto_1e_cut);
                    // // TH2D* h_reco_phi_nFD_VS_eff_L_ECAL_ECALveto_1e_cut =
                    // //     new TH2D("reco_phi_nFD_VS_eff_L_ECAL_ECALveto_1e_cut",
                    // //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (ECALveto);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD}
                    // [#circ]",
                    // //              100, 700., 850., 100, -180., 180.);
                    // // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECAL_ECALveto_1e_cut);

                    // TH2D* h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut =
                    //     new TH2D("reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (ECALveto);L^{reco}_{nFD,ECIN} [cm];P^{reco}_{nFD}
                    //     [#circ]", 100,
                    //              700., 850., 100, 0., Ebeam * 3.);
                    // HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut);
                    // TH2D* h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut =
                    //     new TH2D("reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut",
                    //              "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (ECALveto);L^{reco}_{nFD,ECIN} [cm];#theta^{reco}_{nFD} [#circ]", 100, 700., 850., 100, 0., 45.);
                    // HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut);
                    // TH2D* h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut =
                    //     new TH2D("reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (ECALveto);L^{reco}_{nFD,ECIN} [cm];#phi^{reco}_{nFD}
                    //     [#circ]",
                    //              100, 700., 850., 100, -180., 180.);
                    // HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut);
                    // TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut =
                    //     new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut",
                    //              "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECIN} [ns];P^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., Ebeam
                    //              * 3.);
                    // HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut);
                    // TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut =
                    //     new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut",
                    //              "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECIN} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100,
                    //              0., 45.);
                    // HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut);
                    // TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut =
                    //     new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut",
                    //              "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECIN} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180.,
                    //              180.);
                    // HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut);
                    // // TH2D* h_reco_P_nFD_VS_eff_L_ECIN_ECALveto_1e_cut =
                    // //     new TH2D("reco_P_nFD_VS_eff_L_ECIN_ECALveto_1e_cut",
                    // //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (ECALveto);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD}
                    // [#circ]", 100,
                    // //              700., 850., 100, 0., Ebeam * 3.);
                    // // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECIN_ECALveto_1e_cut);
                    // // TH2D* h_reco_theta_nFD_VS_eff_L_ECIN_ECALveto_1e_cut =
                    // //     new TH2D("reco_theta_nFD_VS_eff_L_ECIN_ECALveto_1e_cut",
                    // //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (ECALveto);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF}
                    // [cm];#theta^{reco}_{nFD}
                    // //              [#circ]", 100, 700., 850., 100, 0., 45.);
                    // // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECIN_ECALveto_1e_cut);
                    // // TH2D* h_reco_phi_nFD_VS_eff_L_ECIN_ECALveto_1e_cut =
                    // //     new TH2D("reco_phi_nFD_VS_eff_L_ECIN_ECALveto_1e_cut",
                    // //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (ECALveto);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD}
                    // [#circ]",
                    // //              100, 700., 850., 100, -180., 180.);
                    // // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECIN_ECALveto_1e_cut);

                    // TH2D* h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut =
                    //     new TH2D("reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (ECALveto);L^{reco}_{nFD,ECOUT} [cm];P^{reco}_{nFD}
                    //     [#circ]", 100,
                    //              700., 850., 100, 0., Ebeam * 3.);
                    // HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut);
                    // TH2D* h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut =
                    //     new TH2D("reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut",
                    //              "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (ECALveto);L^{reco}_{nFD,ECOUT} [cm];#theta^{reco}_{nFD} [#circ]", 100, 700., 850., 100, 0., 45.);
                    // HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut);
                    // TH2D* h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut =
                    //     new TH2D("reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut",
                    //              "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (ECALveto);L^{reco}_{nFD,ECOUT} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700., 850., 100, -180., 180.);
                    // HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut);
                    // TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut =
                    //     new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut",
                    //              "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECOUT} [ns];P^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., Ebeam
                    //              * 3.);
                    // HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut);
                    // TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut =
                    //     new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut",
                    //              "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECOUT} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100,
                    //              0., 45.);
                    // HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut);
                    // TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut =
                    //     new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut",
                    //              "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECOUT} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180.,
                    //              180.);
                    // HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut);
                    // // TH2D* h_reco_P_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut =
                    // //     new TH2D("reco_P_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut",
                    // //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (ECALveto);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD}
                    // [#circ]", 100,
                    // //              700., 850., 100, 0., Ebeam * 3.);
                    // // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut);
                    // // TH2D* h_reco_theta_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut =
                    // //     new TH2D("reco_theta_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut",
                    // //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (ECALveto);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF}
                    // [cm];#theta^{reco}_{nFD}
                    // //              [#circ]", 100, 700., 850., 100, 0., 45.);
                    // // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut);
                    // // TH2D* h_reco_phi_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut =
                    // //     new TH2D("reco_phi_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut",
                    // //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (ECALveto);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD}
                    // [#circ]",
                    // //              100, 700., 850., 100, -180., 180.);
                    // // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut);

                    TH1D* h_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut =
                        new TH1D("reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut",
                                 "#Delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
                    HistoList.push_back(h_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut);
                    TH1D* h_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH1D("reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "#Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
                    HistoList.push_back(h_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);
                    TH2D* h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH2D("reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "#Delta#theta^{reco}_{nFD,e} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                                 "[#circ];#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                                 100, -180., 180., 100, -25., 10.);
                    HistoList.push_back(h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);
                    TH2D* h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut =
                        new TH2D("reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut",
                                 "P^{reco}_{nFD} vs. #Delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                                 "[#circ];P^{reco}_{nFD} [GeV/c]",
                                 100, -25., 10., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut);
                    TH2D* h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut = new TH2D(
                        "reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                        "P^{reco}_{nFD} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];P^{reco}_{nFD} [GeV/c]",
                        100, -180., 180., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);

#pragma region /* New plots - LnFD */
                    TH1D* h_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut =
                        new TH1D("reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut",
                                 "#Delta#theta^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
                    HistoList.push_back(h_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut);
                    TH1D* h_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH1D("reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "#Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
                    HistoList.push_back(h_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut);
                    TH2D* h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH2D("reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                                 "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                                 100, -180., 180., 100, -25., 10.);
                    HistoList.push_back(h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut);
                    TH2D* h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut =
                        new TH2D("reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut",
                                 "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                                 100, -25., 10., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut);
                    TH2D* h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH2D("reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                                 100, -180., 180., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut);

                    TH2D* h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut =
                        new TH2D("LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut",
                                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];LnFD status",
                                 100, -25., 10., 100, 1990, 3000.);
                    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut);

                    TH2D* h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECALveto_1e_cut =
                        new TH2D("LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECALveto_1e_cut",
                                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];LnFD status",
                                 100, -5., 5., 100, 1990, 2250.);
                    //  100, -25., 10., 100, 1990, 2250.);
                    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECALveto_1e_cut);

                    TH2D* h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut =
                        new TH2D("e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut",
                                 "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];e status",
                                 100, -25., 10., 100, 990, 3000.);
                    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut);

                    TH2D* h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECALveto_1e_cut =
                        new TH2D("e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECALveto_1e_cut",
                                 "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];e status",
                                 100, -5., 5., 100, 990, 2250.);
                    //  100, -25., 10., 100, 1990, 2250.);
                    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECALveto_1e_cut);

                    TH1D* h_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut = new TH1D(
                        "reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut",
                        "#Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
                    HistoList.push_back(h_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut);
                    TH1D* h_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut = new TH1D(
                        "reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut",
                        "#Delta#phi^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
                    HistoList.push_back(h_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut);
                    TH2D* h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut = new TH2D(
                        "reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut",
                        "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                        "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                        100, -180., 180., 100, -25., 10.);
                    HistoList.push_back(h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut);
                    TH2D* h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut =
                        new TH2D("reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut",
                                 "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                                 100, -25., 10., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut);
                    TH2D* h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut =
                        new TH2D("reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut",
                                 "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                                 100, -180., 180., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut);

                    TH2D* h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut =
                        new TH2D("LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut",
                                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];LnFD status",
                                 100, -25., 10., 100, 1990, 3000.);
                    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut);

                    TH2D* h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_PCAL_only_ECALveto_1e_cut = new TH2D(
                        "LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_PCAL_only_ECALveto_1e_cut",
                        "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                        "[#circ];LnFD status",
                        100, -5., 5., 100, 1990, 2250.);
                    //  100, -25., 10., 100, 1990, 2250.);
                    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_PCAL_only_ECALveto_1e_cut);

                    TH2D* h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut =
                        new TH2D("e_status_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut",
                                 "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];e status",
                                 100, -25., 10., 100, 1990, 3000.);
                    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut);

                    TH2D* h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_PCAL_only_ECALveto_1e_cut = new TH2D(
                        "e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_PCAL_only_ECALveto_1e_cut",
                        "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, PCAL only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                        "[#circ];e status",
                        100, -5., 5., 100, 1990, 2250.);
                    //  100, -25., 10., 100, 1990, 2250.);
                    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_PCAL_only_ECALveto_1e_cut);

                    TH1D* h_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut = new TH1D(
                        "reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut",
                        "#Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
                    HistoList.push_back(h_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut);
                    TH1D* h_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut = new TH1D(
                        "reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut",
                        "#Delta#phi^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
                    HistoList.push_back(h_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut);
                    TH2D* h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut = new TH2D(
                        "reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut",
                        "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                        "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                        100, -180., 180., 100, -25., 10.);
                    HistoList.push_back(h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut);
                    TH2D* h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut =
                        new TH2D("reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut",
                                 "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                                 100, -25., 10., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut);
                    TH2D* h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut =
                        new TH2D("reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut",
                                 "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                                 100, -180., 180., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut);

                    TH2D* h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut =
                        new TH2D("LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut",
                                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];LnFD status",
                                 100, -25., 10., 100, 1990, 3000.);
                    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut);

                    TH2D* h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECIN_only_ECALveto_1e_cut = new TH2D(
                        "LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECIN_only_ECALveto_1e_cut",
                        "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                        "[#circ];LnFD status",
                        100, -5., 5., 100, 1990, 2250.);
                    //  100, -25., 10., 100, 1990, 2250.);
                    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECIN_only_ECALveto_1e_cut);

                    TH2D* h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut =
                        new TH2D("e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut",
                                 "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];e status",
                                 100, -25., 10., 100, 1990, 3000.);
                    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut);

                    TH2D* h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECIN_only_ECALveto_1e_cut = new TH2D(
                        "e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECIN_only_ECALveto_1e_cut",
                        "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECIN only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                        "[#circ];e status",
                        100, -5., 5., 100, 1990, 2250.);
                    //  100, -25., 10., 100, 1990, 2250.);
                    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECIN_only_ECALveto_1e_cut);

                    TH1D* h_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut =
                        new TH1D("reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut",
                                 "#Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ];Counts", 50,
                                 -25., 10.);
                    HistoList.push_back(h_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut);
                    TH1D* h_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut = new TH1D(
                        "reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut",
                        "#Delta#phi^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
                    HistoList.push_back(h_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut);
                    TH2D* h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut = new TH2D(
                        "reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut",
                        "#Delta#theta^{reco}_{LnFD,e} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                        "[#circ];#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} [#circ]",
                        100, -180., 180., 100, -25., 10.);
                    HistoList.push_back(h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut);
                    TH2D* h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut =
                        new TH2D("reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut",
                                 "P^{reco}_{LnFD} vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                                 100, -25., 10., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut);
                    TH2D* h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut =
                        new TH2D("reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut",
                                 "P^{reco}_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                                 "[#circ];P^{reco}_{LnFD} [GeV/c]",
                                 100, -180., 180., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut);

                    TH2D* h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut =
                        new TH2D("LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut",
                                 "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];LnFD status",
                                 100, -25., 10., 100, 1990, 3000.);
                    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut);

                    TH2D* h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECOUT_only_ECALveto_1e_cut = new TH2D(
                        "LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECOUT_only_ECALveto_1e_cut",
                        "LnFD status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                        "[#circ];LnFD status",
                        100, -5., 5., 100, 1990, 2250.);
                    //  100, -25., 10., 100, 1990, 2250.);
                    HistoList.push_back(h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECOUT_only_ECALveto_1e_cut);

                    TH2D* h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut =
                        new TH2D("e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut",
                                 "e status vs. #Delta#theta^{reco}_{LnFD,e} in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                                 "[#circ];e status",
                                 100, -25., 10., 100, 1990, 3000.);
                    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut);

                    TH2D* h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECOUT_only_ECALveto_1e_cut = new TH2D(
                        "e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECOUT_only_ECALveto_1e_cut",
                        "e status vs. #Delta#theta^{reco}_{LnFD,e} - zoomin - in 1e cut, ECOUT only (ECALveto);#Delta#theta^{reco}_{LnFD,e} = #theta^{reco}_{LnFD} - #theta^{reco}_{e} "
                        "[#circ];e status",
                        100, -5., 5., 100, 1990, 2250.);
                    //  100, -25., 10., 100, 1990, 2250.);
                    HistoList.push_back(h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECOUT_only_ECALveto_1e_cut);
#pragma endregion

                    TH1D* h_v_dist_nFD_ECALveto_1e_cut = new TH1D("v_dist_nFD_ECALveto_1e_cut", "v_dist in 1e cut (ECALveto);v_dist [cm];Counts", 50, 0., 1000.);
                    HistoList.push_back(h_v_dist_nFD_ECALveto_1e_cut);
                    TH2D* h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut = new TH2D(
                        "v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut", "v_dist vs. P^{reco}_{nFD} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{nFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut);
                    TH2D* h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut =
                        new TH2D("v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
                    HistoList.push_back(h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut);
                    TH2D* h_v_dist_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH2D("v_dist_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "v_dist vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);v_dist [cm];#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0.,
                                 1000., 100, -180., 180.);
                    HistoList.push_back(h_v_dist_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);

                    TH1D* h_v_dist_LnFD_ECALveto_1e_cut = new TH1D("v_dist_LnFD_ECALveto_1e_cut", "v_dist in 1e cut (ECALveto);v_dist [cm];Counts", 50, 0., 1000.);
                    HistoList.push_back(h_v_dist_LnFD_ECALveto_1e_cut);
                    TH2D* h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut =
                        new TH2D("v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut", "v_dist vs. P^{reco}_{LnFD} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{LnFD} [GeV/c]", 100, 0., 1000., 100, 0.,
                                 Ebeam * 3.);
                    HistoList.push_back(h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut);
                    TH2D* h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut =
                        new TH2D("v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
                    HistoList.push_back(h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut);
                    TH2D* h_v_dist_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH2D("v_dist_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "v_dist vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);v_dist [cm];#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]", 100,
                                 0., 1000., 100, -180., 180.);
                    HistoList.push_back(h_v_dist_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut);

                    TH2D* h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut =
                        new TH2D("Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];E^{ECAL}_{dep} [GeV]", 100, 0.,
                                 Ebeam * 3., 100, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut);
                    TH2D* h_Edep_ECAL_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut = new TH2D(
                        "Edep_ECAL_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                        "E^{ECAL}_{dep} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];E^{ECAL}_{dep} [GeV]",
                        100, -180., 180., 100, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);

                    TH2D* h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut = new TH2D(
                        "beta_n_VS_reco_P_nFD_ECALveto_1e_cut", "#beta_{nFD} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];#beta_{nFD}", 100, 0., Ebeam * 3., 100, 0.3, 1.1);
                    HistoList.push_back(h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut);
                    TH2D* h_beta_n_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH2D("beta_n_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "#beta_{nFD} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];#beta_{nFD}", 100,
                                 -180., 180., 100, 0.3, 1.1);
                    HistoList.push_back(h_beta_n_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);

                    TH2D* h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut =
                        new TH2D("Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];E^{ECAL}_{dep} [GeV]", 100, 0.,
                                 Ebeam * 3., 100, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut);
                    TH2D* h_Edep_ECAL_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH2D("Edep_ECAL_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "E^{ECAL}_{dep} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
                                 "[#circ]];E^{ECAL}_{dep} [GeV]",
                                 100, -180., 180., 100, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut);

                    TH2D* h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut =
                        new TH2D("beta_n_VS_reco_P_LnFD_ECALveto_1e_cut", "#beta_{LnFD} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];#beta_{LnFD}", 100, 0., Ebeam * 3.,
                                 100, 0.3, 1.1);
                    HistoList.push_back(h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut);
                    TH2D* h_beta_n_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut =
                        new TH2D("beta_n_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut",
                                 "#beta_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]];#beta_{LnFD}",
                                 100, -180., 180., 100, 0.3, 1.1);
                    HistoList.push_back(h_beta_n_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut);

#pragma endregion

#pragma region /* matched */
                    HistSubjects.push_back("matched");
                    HistSubjects2.push_back("matched");
                    FirstPrint.push_back(true);

#pragma region /* Base histograms (matched) */
                    TH1D* h_reco_P_nFD_matched_1e_cut = new TH1D("reco_P_nFD_matched_1e_cut", "P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
                    HistoList.push_back(h_reco_P_nFD_matched_1e_cut);
                    TH1D* h_truth_P_nFD_matched_1e_cut = new TH1D("truth_P_nFD_matched_1e_cut", "P^{truth}_{nFD} in 1e cut (matched);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
                    HistoList.push_back(h_truth_P_nFD_matched_1e_cut);
                    TH1D* h_reco_theta_nFD_matched_1e_cut =
                        new TH1D("reco_theta_nFD_matched_1e_cut", "#theta^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
                    HistoList.push_back(h_reco_theta_nFD_matched_1e_cut);
                    TH1D* h_truth_theta_nFD_matched_1e_cut =
                        new TH1D("truth_theta_nFD_matched_1e_cut", "#theta^{truth}_{nFD} in 1e cut (matched);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
                    HistoList.push_back(h_truth_theta_nFD_matched_1e_cut);
                    TH1D* h_reco_phi_nFD_matched_1e_cut = new TH1D("reco_phi_nFD_matched_1e_cut", "#phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
                    HistoList.push_back(h_reco_phi_nFD_matched_1e_cut);
                    TH1D* h_truth_phi_nFD_matched_1e_cut =
                        new TH1D("truth_phi_nFD_matched_1e_cut", "#phi^{truth}_{nFD} in 1e cut (matched);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
                    HistoList.push_back(h_truth_phi_nFD_matched_1e_cut);
                    TH2D* h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut =
                        new TH2D("reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut",
                                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
                    HistoList.push_back(h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut);
                    TH2D* h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut =
                        new TH2D("truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut",
                                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (matched);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
                    HistoList.push_back(h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut);

                    TH1D* h_reco_P_LnFD_matched_1e_cut = new TH1D("reco_P_LnFD_matched_1e_cut", "P^{reco}_{LnFD} in 1e cut (matched);P^{reco}_{LnFD} [GeV/c];Counts", 50, 0, P_upperLim);
                    HistoList.push_back(h_reco_P_LnFD_matched_1e_cut);
                    TH1D* h_truth_P_LnFD_matched_1e_cut = new TH1D("truth_P_LnFD_matched_1e_cut", "P^{truth}_{LnFD} in 1e cut (matched);P^{truth}_{LnFD} [GeV/c];Counts", 50, 0, P_upperLim);
                    HistoList.push_back(h_truth_P_LnFD_matched_1e_cut);
                    TH1D* h_reco_theta_LnFD_matched_1e_cut =
                        new TH1D("reco_theta_LnFD_matched_1e_cut", "#theta^{reco}_{LnFD} in 1e cut (matched);#theta^{reco}_{LnFD} [#circ];Counts", 50, 0, 45.);
                    HistoList.push_back(h_reco_theta_LnFD_matched_1e_cut);
                    TH1D* h_truth_theta_LnFD_matched_1e_cut =
                        new TH1D("truth_theta_LnFD_matched_1e_cut", "#theta^{truth}_{LnFD} in 1e cut (matched);#theta^{truth}_{LnFD} [#circ];Counts", 50, 0, 45.);
                    HistoList.push_back(h_truth_theta_LnFD_matched_1e_cut);
                    TH1D* h_reco_phi_LnFD_matched_1e_cut =
                        new TH1D("reco_phi_LnFD_matched_1e_cut", "#phi^{reco}_{LnFD} in 1e cut (matched);#phi^{reco}_{LnFD} [#circ];Counts", 50, -180, 180.);
                    HistoList.push_back(h_reco_phi_LnFD_matched_1e_cut);
                    TH1D* h_truth_phi_LnFD_matched_1e_cut =
                        new TH1D("truth_phi_LnFD_matched_1e_cut", "#phi^{truth}_{LnFD} in 1e cut (matched);#phi^{truth}_{LnFD} [#circ];Counts", 50, -180, 180.);
                    HistoList.push_back(h_truth_phi_LnFD_matched_1e_cut);
                    TH2D* h_reco_theta_LnFD_matched_VS_reco_phi_LnFD_matched_1e_cut =
                        new TH2D("reco_theta_LnFD_matched_VS_reco_phi_LnFD_matched_1e_cut",
                                 "#theta^{reco}_{LnFD} vs. #phi^{reco}_{LnFD} in 1e cut (matched);#phi^{reco}_{LnFD} [#circ];#theta^{reco}_{LnFD} [#circ]", 100, -180., 180., 100, 0, 50.);
                    HistoList.push_back(h_reco_theta_LnFD_matched_VS_reco_phi_LnFD_matched_1e_cut);
                    TH2D* h_truth_theta_LnFD_matched_VS_truth_phi_LnFD_matched_1e_cut = new TH2D(
                        "truth_theta_LnFD_matched_VS_truth_phi_LnFD_matched_1e_cut",
                        "#theta^{truth}_{LnFD} vs. #phi^{truth}_{LnFD} in 1e cut (matched);#phi^{truth}_{LnFD} [#circ];#theta^{truth}_{LnFD} [#circ]", 100, -180., 180., 100, 0, 50.);
                    HistoList.push_back(h_truth_theta_LnFD_matched_VS_truth_phi_LnFD_matched_1e_cut);

                    TH1D* h_Edep_ECAL_matched_1e_cut = new TH1D("Edep_ECAL_matched_1e_cut", "E^{ECAL}_{dep} in 1e cut (matched);E^{ECAL}_{dep} [GeV];Counts", 50, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_matched_1e_cut);
                    TH1D* h_beta_n_matched_1e_cut = new TH1D("beta_n_matched_1e_cut", "#beta_{nFD} in 1e cut (matched);#beta_{nFD};Counts", 50, 0.3, 1.1);
                    HistoList.push_back(h_beta_n_matched_1e_cut);
                    TH2D* h_Edep_ECAL_VS_beta_n_matched_1e_cut =
                        new TH2D("Edep_ECAL_VS_beta_n_matched_1e_cut", "E^{ECAL}_{dep} vs. #beta_{nFD} in 1e cut (matched);#beta_{nFD};E^{ECAL}_{dep} [GeV]", 100, 0.3, 1.1, 100, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_VS_beta_n_matched_1e_cut);

                    TH2D* h_reco_P_e_VS_P_nFD_matched_1e_cut =
                        new TH2D("reco_P_e_VS_P_nFD_matched_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2.,
                                 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_e_VS_P_nFD_matched_1e_cut);
                    TH2D* h_reco_theta_e_VS_P_nFD_matched_1e_cut =
                        new TH2D("reco_theta_e_VS_P_nFD_matched_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0,
                                 45., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_theta_e_VS_P_nFD_matched_1e_cut);
                    TH2D* h_reco_phi_e_VS_P_nFD_matched_1e_cut =
                        new TH2D("reco_phi_e_VS_P_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, -180.,
                                 180., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_phi_e_VS_P_nFD_matched_1e_cut);
                    TH2D* h_reco_P_e_VS_theta_nFD_matched_1e_cut =
                        new TH2D("reco_P_e_VS_theta_nFD_matched_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];#theta^{reco}_{nFD} [#circ]", 100, 0.,
                                 P_upperLim, 100, 0, 45.);
                    HistoList.push_back(h_reco_P_e_VS_theta_nFD_matched_1e_cut);
                    TH2D* h_reco_theta_e_VS_theta_nFD_matched_1e_cut =
                        new TH2D("reco_theta_e_VS_theta_nFD_matched_1e_cut",
                                 "#theta^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, 0, 45., 100, 0, 45.);
                    HistoList.push_back(h_reco_theta_e_VS_theta_nFD_matched_1e_cut);
                    TH2D* h_reco_phi_e_VS_theta_nFD_matched_1e_cut =
                        new TH2D("reco_phi_e_VS_theta_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]",
                                 100, -180., 180., 100, 0, 45.);
                    HistoList.push_back(h_reco_phi_e_VS_theta_nFD_matched_1e_cut);
                    TH2D* h_reco_P_e_VS_phi_nFD_matched_1e_cut =
                        new TH2D("reco_P_e_VS_phi_nFD_matched_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);P^{reco}_{e} [GeV/c];#phi^{reco}_{nFD} [#circ]", 100, 0.,
                                 P_upperLim, 100, -180., 180.);
                    HistoList.push_back(h_reco_P_e_VS_phi_nFD_matched_1e_cut);
                    TH2D* h_reco_theta_e_VS_phi_nFD_matched_1e_cut =
                        new TH2D("reco_phi_e_VS_phi_nFD_matched_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]",
                                 100, 0, 45., 100, -180., 180.);
                    HistoList.push_back(h_reco_theta_e_VS_phi_nFD_matched_1e_cut);
                    TH2D* h_reco_phi_e_VS_phi_nFD_matched_1e_cut =
                        new TH2D("reco_phi_e_VS_phi_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100,
                                 -180., 180., 100, -180., 180.);
                    HistoList.push_back(h_reco_phi_e_VS_phi_nFD_matched_1e_cut);
                    TH2D* h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut =
                        new TH2D("reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut",
                                 "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0., 45., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut);
                    TH1D* h_reco_nFD_multi_matched_1e_cut = new TH1D("reco_nFD_multi_matched_1e_cut", "reco nFD multiplicity in 1e cut (matched);nFD multiplicity;Counts", 9, 1, 10);
                    HistoList.push_back(h_reco_nFD_multi_matched_1e_cut);
                    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut =
                        new TH2D("reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut", "reco nFD multiplicity vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100,
                                 0., Ebeam * 3., 9, 1, 10);
                    HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut);
#pragma endregion

                    TH1D* h_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut =
                        new TH1D("reco_theta_nFD_minus_reco_theta_e_matched_1e_cut",
                                 "#Delta#theta^{reco}_{nFD,e} in 1e cut (matched);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];Counts", 50, -25., 10.);
                    HistoList.push_back(h_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut);
                    TH1D* h_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut =
                        new TH1D("reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                                 "#Delta#phi^{reco}_{nFD,e} in 1e cut (matched);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50, -180., 180.);
                    HistoList.push_back(h_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);
                    TH2D* h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut =
                        new TH2D("reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                                 "#Delta#theta^{reco}_{nFD,e} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (matched);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
                                 "[#circ];#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
                                 100, -180., 180., 100, -25., 10.);
                    HistoList.push_back(h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);
                    TH2D* h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut =
                        new TH2D("reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut",
                                 "P^{reco}_{nFD} vs. #Delta#theta^{reco}_{nFD,e} in 1e cut (matched);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
                                 "[#circ];P^{reco}_{nFD} [GeV/c]",
                                 100, -25., 10., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut);
                    TH2D* h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut = new TH2D(
                        "reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                        "P^{reco}_{nFD} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (matched);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];P^{reco}_{nFD} [GeV/c]",
                        100, -180., 180., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);

                    TH1D* h_v_dist_nFD_matched_1e_cut = new TH1D("v_dist_nFD_matched_1e_cut", "v_dist in 1e cut (matched);v_dist [cm];Counts", 50, 0., 1000.);
                    HistoList.push_back(h_v_dist_nFD_matched_1e_cut);
                    TH2D* h_v_dist_nFD_VS_reco_P_nFD_matched_1e_cut = new TH2D(
                        "v_dist_nFD_VS_reco_P_nFD_matched_1e_cut", "v_dist vs. P^{reco}_{nFD} in 1e cut (matched);v_dist [cm];P^{reco}_{nFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
                    HistoList.push_back(h_v_dist_nFD_VS_reco_P_nFD_matched_1e_cut);
                    TH2D* h_v_dist_nFD_VS_reco_P_e_matched_1e_cut =
                        new TH2D("v_dist_nFD_VS_reco_P_e_matched_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (matched);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
                    HistoList.push_back(h_v_dist_nFD_VS_reco_P_e_matched_1e_cut);
                    TH2D* h_v_dist_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut =
                        new TH2D("v_dist_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                                 "v_dist vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (matched);v_dist [cm];#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0.,
                                 1000., 100, -180., 180.);
                    HistoList.push_back(h_v_dist_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);

                    TH2D* h_Edep_ECAL_VS_reco_P_nFD_matched_1e_cut =
                        new TH2D("Edep_ECAL_VS_reco_P_nFD_matched_1e_cut", "E^{ECAL}_{dep} vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];E^{ECAL}_{dep} [GeV]", 100, 0.,
                                 Ebeam * 3., 100, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_VS_reco_P_nFD_matched_1e_cut);
                    TH2D* h_Edep_ECAL_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut = new TH2D(
                        "Edep_ECAL_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                        "E^{ECAL}_{dep} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (matched);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];E^{ECAL}_{dep} [GeV]",
                        100, -180., 180., 100, 0., 0.5);
                    HistoList.push_back(h_Edep_ECAL_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);

                    TH2D* h_beta_n_VS_reco_P_nFD_matched_1e_cut = new TH2D(
                        "beta_n_VS_reco_P_nFD_matched_1e_cut", "#beta_{nFD} vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];#beta_{nFD}", 100, 0., Ebeam * 3., 100, 0.3, 1.1);
                    HistoList.push_back(h_beta_n_VS_reco_P_nFD_matched_1e_cut);
                    TH2D* h_beta_n_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut =
                        new TH2D("beta_n_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                                 "#beta_{nFD} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (matched);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];#beta_{nFD}", 100,
                                 -180., 180., 100, 0.3, 1.1);
                    HistoList.push_back(h_beta_n_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);

#pragma endregion

#pragma endregion

#pragma region /* Prepare AMaps */

                    const std::string OutputDirAMapsPlots = OutputDir + "/01_AMaps_Plots/";
                    system(("rm -rf " + OutputDirAMapsPlots).c_str());
                    system(("mkdir -p " + OutputDirAMapsPlots).c_str());

                    const std::string OutputDirAMapsMaps = OutputDir + "/02_AMaps_Maps/";
                    system(("rm -rf " + OutputDirAMapsMaps).c_str());
                    system(("mkdir -p " + OutputDirAMapsMaps).c_str());

                    const std::string P_e_bin_profile = "uniform_P_e_bins";      // {reformat_e_bins , varying_P_e_bins , uniform_P_e_bins, equi_inverted_P_e}
                    const std::string P_nuc_bin_profile = "uniform_P_nuc_bins";  // {equi_inverted_P_nuc , varying_P_nuc_bins , uniform_P_nuc_bins}

                    double beamE = Ebeam;

                    int NumberNucOfMomSlices = 15, NumberElecOfMomSlices = 15, HistElectronSliceNumOfXBins = 100, HistNucSliceNumOfXBins = 100;

                    AMaps aMaps_master;

                    aMaps_master = AMaps(SampleName, P_e_bin_profile, P_nuc_bin_profile, beamE, "AMaps", OutputDirAMapsPlots, NumberNucOfMomSlices, NumberElecOfMomSlices,
                                         HistNucSliceNumOfXBins, HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins, HistElectronSliceNumOfXBins);

#pragma endregion

                    int counter = 0;

                    int counter_ECALveto_ECINorECOUT_FDneutrons = 0;
                    int counter_ECALveto_ECINorECOUT_FDphotons = 0;
                    int counter_matched_ECINorECOUT_FDneutrons = 0;
                    int counter_matched_ECINorECOUT_FDphotons = 0;

                    // ParticleID PID;

                    while (chain.Next() == true) {
                        // Display completed
                        ++counter;
                        if ((counter % 1000000) == 0) { std::cerr << "\n" << counter / 1000000 << " million completed"; }
                        if ((counter % 100000) == 0) { std::cerr << "."; }

                        if (counter > Limiter) { break; }

                        // get particles by type
                        auto allParticles = c12->getDetParticles();
                        auto electrons = c12->getByID(11);

                        if (electrons.size() != 1) { continue; }

                        double weight = 1;

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
                        double EoP_e = (electrons[0]->cal(clas12::PCAL)->getEnergy() + electrons[0]->cal(ECIN)->getEnergy() + electrons[0]->cal(ECOUT)->getEnergy()) / reco_P_e.Mag();
                        double E_PCALoP_e = electrons[0]->cal(clas12::PCAL)->getEnergy() / reco_P_e.Mag();
                        double E_ECINoP_e = electrons[0]->cal(clas12::ECIN)->getEnergy() / reco_P_e.Mag();
                        double Edep_PCAL = electrons[0]->cal(clas12::PCAL)->getEnergy();
                        double Edep_EC = electrons[0]->cal(clas12::ECIN)->getEnergy() + electrons[0]->cal(clas12::ECOUT)->getEnergy();

                        bool ElectronInPCAL = (electrons[0]->cal(clas12::PCAL)->getDetector() == 7);                                  // PCAL hit
                        bool ElectronInECIN = (electrons[0]->cal(clas12::ECIN)->getDetector() == 7);                                  // ECIN hit
                        bool ElectronInECOUT = (electrons[0]->cal(clas12::ECOUT)->getDetector() == 7);                                // ECOUT hit
                        auto Electron_ECAL_detlayer = ElectronInPCAL ? clas12::PCAL : ElectronInECIN ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

                        //  =======================================================================================================================================================================
                        //  Get truth electron variables
                        //  =======================================================================================================================================================================

                        double Truth_theta_e, Truth_phi_e, Truth_phi_nFD;

                        auto c12 = chain.GetC12Reader();
                        auto mceve = c12->mcevent();
                        auto mcpbank = c12->mcparts();
                        const Int_t Ngen = mcpbank->getRows();

                        for (Int_t i = 0; i < Ngen; i++) {
                            mcpbank->setEntry(i);

                            auto pid_temp = mcpbank->getPid();

                            auto Truth_theta_temp = mcpbank->getTheta() * 180 / M_PI;
                            auto Truth_phi_temp = mcpbank->getPhi() * 180 / M_PI;

                            // cout << "\n\nTruth_theta_temp = " << Truth_theta_temp << "\n\n";

                            if (pid_temp == 11) {
                                Truth_theta_e = Truth_theta_temp, Truth_phi_e = Truth_phi_temp;
                            } else if (pid_temp == 2112) {
                                Truth_phi_nFD = Truth_phi_temp;
                            }
                        }

                        //  =======================================================================================================================================================================
                        //  1e cut (reco)
                        //  =======================================================================================================================================================================

#pragma region /* 1e cut (reco) */

                        //  Electron PID cuts
                        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Electron PID cuts */

                        h_Vz_e_BC_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
                        bool bad_Vz_e_CutCond = (electrons[0]->par()->getVz() < -4. || electrons[0]->par()->getVz() > -2.);
                        // bool bad_Vz_e_CutCond = (electrons[0]->par()->getVz() < -6. || electrons[0]->par()->getVz() > 0.);
                        if (!bad_Vz_e_CutCond) { h_Vz_e_AC_1e_cut->Fill(electrons[0]->par()->getVz(), weight); }

                        fillDCdebug(electrons[0], h_dc_electron_hit_map_BC_1e_cut, weight);
                        bool bad_DC_edge_CutCond = (!DCEdgeCuts(electrons[0]));
                        if (!bad_DC_edge_CutCond) { fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_1e_cut, weight); }

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
                        bool bad_diag_CutCond = (!checkEcalDiagCuts(electrons[0]));
                        if (!bad_diag_CutCond) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight); }

                        if (bad_Vz_e_CutCond) { continue; }
                        if (bad_DC_edge_CutCond) { continue; }

                        if (bad_nphe_CutCond) { continue; }
                        if (bad_Edep_PCAL_CutCond) { continue; }
                        if (bad_SF_CutCond) { continue; }
                        if (bad_PCAL_edge_CutCond) { continue; }
                        if (bad_diag_CutCond) { continue; }

                        // Force the electron to be in the expected region of phase space:
                        if (ConstrainedE && (fabs(reco_P_e.Mag() - Ebeam) > 0.01 * Ebeam * 2)) { continue; }  // The resolution (sigma/P) is less than 1%, so I take twice that
                        // if (ConstrainedE && (fabs(reco_P_e.Mag() - Ebeam) > 0.2)) { continue; }
                        if (ConstrainedE && (fabs((reco_P_e.Theta() * 180 / M_PI) - Truth_theta_e) > 2.)) { continue; }
                        if (ConstrainedE && (fabs((reco_P_e.Phi() * 180 / M_PI) - Truth_phi_e) > 5.)) { continue; }
                        if (ConstrainedE && (CalcdPhi1(fabs(GetPhi_e(Beam_energy_TString, Truth_phi_nFD) - (reco_P_e.Phi() * 180 / M_PI))) > 5.)) { continue; }

                        // if (fabs(Truth_phi_nFD) < 30.) {
                        //     cout << "\n\n==========================================================\n";
                        //     cout << "Truth_phi_nFD = " << Truth_phi_nFD << "\n";
                        //     cout << "Truth_phi_e = " << Truth_phi_e << "\n";
                        //     cout << "GetPhi_e(Beam_energy_TString, Truth_phi_nFD) = " << GetPhi_e(Beam_energy_TString, Truth_phi_nFD) << "\n";
                        //     cout << "reco_P_e.Phi() * 180 / M_PI = " << reco_P_e.Phi() * 180 / M_PI << "\n";
                        //     cout << "fabs(GetPhi_e(Beam_energy_TString, Truth_phi_nFD) - reco_P_e.Phi() * 180 / M_PI) = "
                        //          << fabs(GetPhi_e(Beam_energy_TString, Truth_phi_nFD) - reco_P_e.Phi() * 180 / M_PI) << "\n\n";
                        //     cout << "CalcdPhi1(fabs(GetPhi_e(Beam_energy_TString, Truth_phi_nFD) - reco_P_e.Phi() * 180 / M_PI)) = "
                        //          << CalcdPhi1(fabs(GetPhi_e(Beam_energy_TString, Truth_phi_nFD) - reco_P_e.Phi() * 180 / M_PI)) << "\n\n";
                        // }

                        h_reco_P_e_1e_cut->Fill(reco_P_e.Mag(), weight);
                        h_reco_theta_e_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
                        h_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
                        h_reco_theta_e_VS_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);
#pragma endregion

                        //  =======================================================================================================================================================================
                        //  1e cut (truth)
                        //  =======================================================================================================================================================================

#pragma region /* 1e cut (truth) */

                        double TL_IDed_Leading_nFD_momentum = -1;  // Leading nFD with momentum thresholds
                        int TL_IDed_Leading_nFD_ind = -1;          // Leading nFD with momentum thresholds

                        double Truth_beta_nFD;
                        double Truth_theta_nFD;

                        bool TLpassCuts = true;

                        vector<int> truth_NeutronsFD;

                        for (Int_t i = 0; i < Ngen; i++) {
                            mcpbank->setEntry(i);

                            auto pid_temp = mcpbank->getPid();

                            auto p = mcpbank->getP();
                            auto px = mcpbank->getPx();
                            auto py = mcpbank->getPy();
                            auto pz = mcpbank->getPz();

                            // bool PassMomTh = true;
                            bool PassMomTh = (p >= 0.4);

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
                            } else if (pid_temp == 2112) {
                                TVector3 truth_P_n;
                                truth_P_n.SetXYZ(px, py, pz);

                                h_truth_P_n_1e_cut->Fill(truth_P_n.Mag(), weight);
                                h_truth_theta_n_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                                h_truth_phi_n_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                                h_truth_theta_n_VS_truth_phi_n_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);

                                Truth_theta_nFD = truth_P_n.Theta() * 180 / M_PI;

                                if ((Truth_theta_nFD >= 5.) && (Truth_theta_nFD <= 35.)) {
                                    if (truth_P_n.Mag() >= TL_IDed_Leading_nFD_momentum) {
                                        TL_IDed_Leading_nFD_momentum = truth_P_n.Mag();
                                        TL_IDed_Leading_nFD_ind = i;
                                    }

                                    double truth_E_nFD = sqrt(m_n * m_n + truth_P_n.Mag2());
                                    Truth_beta_nFD = truth_P_n.Mag() / truth_E_nFD;

                                    h_truth_P_nFD_clas12_1e_cut->Fill(truth_P_n.Mag(), weight);
                                    h_truth_theta_nFD_clas12_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                                    h_truth_phi_nFD_clas12_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                                    h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);

                                    h_truth_P_nFD_redef_1e_cut->Fill(truth_P_n.Mag(), weight);
                                    h_truth_theta_nFD_redef_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                                    h_truth_phi_nFD_redef_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                                    h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);

                                    if (PassMomTh) {
                                        truth_NeutronsFD.push_back(i);

                                        h_truth_P_nFD_ECALveto_1e_cut->Fill(truth_P_n.Mag(), weight);
                                        h_truth_theta_nFD_ECALveto_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                                        h_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                                        h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);
                                    }
                                }
                            }
                        }

                        if (ConstrainTLmom && !TLpassCuts) { continue; }

                        // if (truth_NeutronsFD.size() != 1) {
                        //     cout << "\n\nError! truth_NeutronsFD size is not 1! Aborting...\n";
                        //     cout << "truth_NeutronsFD.size() = " << truth_NeutronsFD.size() << "\n";
                        //     cout << "Truth_theta_nFD = " << Truth_theta_nFD << "\nAborting...\n\n", exit(1);
                        // }
                        if (truth_NeutronsFD.size() != 1) { continue; }

                        // Fill leading FD neutron acceptance maps
                        if ((TL_IDed_Leading_nFD_ind != -1) && (TL_IDed_Leading_nFD_momentum > 0)) {
                            mcpbank->setEntry(TL_IDed_Leading_nFD_ind);

                            int particlePDGtmp = mcpbank->getPid();

                            double Particle_TL_Momentum = RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                            double Particle_TL_Theta = acos((mcpbank->getPz()) / RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
                            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                            // bool PassMomTh = true;
                            bool PassMomTh = (Particle_TL_Momentum >= 0.4);

                            if (PassMomTh) { aMaps_master.hFillMaps("TL", "Neutron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, weight); }  // end of if id. TL leading neutron
                        }
#pragma endregion

                        //  Setting up neutrals
                        //  ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up neutrals (RAW) */
                        vector<region_part_ptr> neutrons;
                        vector<region_part_ptr> photons;

                        for (int i = 0; i < allParticles.size(); i++) {
                            int pid_temp = allParticles[i]->par()->getPid();

                            if (pid_temp == 2112) { neutrons.push_back(allParticles[i]); }
                            if (pid_temp == 22) { photons.push_back(allParticles[i]); }
                        }

                        bool Pass_nFD_Multi_cut_raw = (!apply_nFD_multi_cut || neutrons.size() == 1);
                        if (Pass_nFD_Multi_cut_raw) {
                            for (int i = 0; i < neutrons.size(); i++) {
                                TVector3 reco_P_n;
                                reco_P_n.SetMagThetaPhi(neutrons[i]->getP(), neutrons[i]->getTheta(), neutrons[i]->getPhi());

                                bool Pass_dTheta_status_raw = (!apply_dTheta_cuts || (fabs(reco_P_n.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI) <= 5.));
                                bool Pass_dPhi_status_raw = (!apply_dPhi_cuts || (fabs((reco_P_n.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI) - 15.) <= 15.));

                                h_reco_P_n_1e_cut->Fill(reco_P_n.Mag(), weight);
                                h_reco_theta_n_1e_cut->Fill(reco_P_n.Theta() * 180 / M_PI, weight);
                                h_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, weight);
                                h_reco_theta_n_VS_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, reco_P_n.Theta() * 180 / M_PI, weight);
                            }
                        }
#pragma endregion

                        //  Setting up FD neutrals (clas12reco)
                        //  -----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (clas12reco) */
                        vector<region_part_ptr> neutrons_FD_clas12;
                        vector<region_part_ptr> photons_FD_clas12;

                        for (int i = 0; i < allParticles.size(); i++) {
                            int pid_temp = allParticles[i]->par()->getPid();

                            if (pid_temp == 2112 && allParticles[i]->getRegion() == FD) { neutrons_FD_clas12.push_back(allParticles[i]); }
                            if (pid_temp == 22 && allParticles[i]->getRegion() == FD) { photons_FD_clas12.push_back(allParticles[i]); }
                        }

                        bool Pass_nFD_Multi_cut_clas12 = (!apply_nFD_multi_cut || neutrons_FD_clas12.size() == 1);

                        if (Pass_nFD_Multi_cut_clas12) {
                            for (int i = 0; i < neutrons_FD_clas12.size(); i++) {
                                TVector3 reco_P_nFD;
                                reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_clas12[i], electrons[0], starttime), neutrons_FD_clas12[i]->getTheta(), neutrons_FD_clas12[i]->getPhi());
                                // reco_P_nFD.SetMagThetaPhi(neutrons_FD_clas12[i]->getP(), neutrons_FD_clas12[i]->getTheta(), neutrons_FD_clas12[i]->getPhi());

                                h_reco_P_nFD_clas12_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                                h_reco_theta_nFD_clas12_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                                h_reco_phi_nFD_clas12_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                                h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                h_reco_P_e_VS_P_nFD_clas12_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
                                h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                            }
                        }

#pragma endregion

                        //  Setting up FD neutrals (redef)
                        //  ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (redef) */
                        vector<region_part_ptr> neutrons_FD_redef;
                        vector<region_part_ptr> photons_FD_redef;

                        for (int i = 0; i < allParticles.size(); i++) {
                            int pid_temp = allParticles[i]->par()->getPid();

                            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)) {  // If particle is neutral and in the FD

                                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
                                auto Neutron_ECAL_detlayer = ParticleInECIN ? clas12::ECIN : clas12::ECOUT;        // find first layer of hit

                                if ((pid_temp == 2112) || (pid_temp == 22)) {
                                    if (ParticleInPCAL) {
                                        if (pid_temp == 22) { photons_FD_redef.push_back(allParticles[i]); }
                                    } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                                        if (ParticleInECIN || ParticleInECOUT) { neutrons_FD_redef.push_back(allParticles[i]); }
                                    }
                                }  // end of clas12root neutron or 'photon' if
                            }  // end of neutral and in the FD if
                        }

                        bool Pass_nFD_Multi_cut_redef = (!apply_nFD_multi_cut || neutrons_FD_redef.size() == 1);

                        if (Pass_nFD_Multi_cut_redef) {
                            for (int i = 0; i < neutrons_FD_redef.size(); i++) {
                                bool ParticleInPCAL = (neutrons_FD_redef[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                                bool ParticleInECIN = (neutrons_FD_redef[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                                bool ParticleInECOUT = (neutrons_FD_redef[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
                                if (ParticleInPCAL) { cout << "\n\nError! neutrons_FD_redef is in the PCAL! Aborting...\n\n", exit(1); }

                                double Beta_ph = neutrons_FD_redef[i]->par()->getBeta();
                                double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));
                                double Momentum = m_n * Beta_ph * Gamma_ph;

                                TVector3 reco_P_nFD;
                                reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_redef[i], electrons[0], starttime), neutrons_FD_redef[i]->getTheta(), neutrons_FD_redef[i]->getPhi());

                                h_reco_P_nFD_redef_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                                h_reco_theta_nFD_redef_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                                h_reco_phi_nFD_redef_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                                h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                h_reco_P_e_VS_P_nFD_redef_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
                                h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                            }
                        }

#pragma endregion

                        //  Setting up FD neutrals (ECALveto)
                        //  ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (ECALveto) */
                        double P_max = -1;
                        int NeutronsFD_ind_mom_max = -1;

                        vector<region_part_ptr> neutrons_FD_ECALveto;
                        vector<int> neutrons_FD_ECALveto_ind;

                        for (int i = 0; i < allParticles.size(); i++) {
                            int pid_temp = allParticles[i]->par()->getPid();

                            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)) {  // If particle is neutral and in the FD
                                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);                           // PCAL hit
                                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);                           // ECIN hit
                                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);                         // ECOUT hit
                                auto Neutron_ECAL_detlayer = ParticleInECIN ? clas12::ECIN : clas12::ECOUT;                               // find first layer of hit

                                if (apply_neutFD_redef) {
                                    if ((pid_temp == 2112) || (pid_temp == 22)) {
                                        if (ParticleInPCAL) {
                                            if (pid_temp == 22) { photons_FD_redef.push_back(allParticles[i]); }
                                        } else if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                                            if (ParticleInECIN || ParticleInECOUT) {
                                                double Momentum = CalcPnFD(allParticles[i], electrons[0], starttime);

                                                double Path_nFD = CalcPathnFD(allParticles[i], electrons[0]);
                                                double reco_ToF_nFD = CalcToFnFD(allParticles[i], starttime);

                                                // bool PassMomTh = true;
                                                bool PassMomTh = (Momentum >= 0.4);
                                                bool PassECALeadgeCuts =
                                                    (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                                                bool PassVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);
                                                // bool PassVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, apply_PCAL_neutral_veto, cPart_veto_radius, nPart_veto_radius);

                                                bool PassPhi_nFDCuts = true;
                                                int nFD_nSector = allParticles[i]->cal(Neutron_ECAL_detlayer)->getSector();
                                                int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();
                                                if (OnlyGood_nFD) { PassPhi_nFDCuts = (abs(nFD_nSector - e_nSector) == 3); }
                                                if (OnlyBad_nFD) { PassPhi_nFDCuts = !(abs(nFD_nSector - e_nSector) == 3); }

                                                if (PassMomTh && PassECALeadgeCuts && (!apply_ECAL_veto || PassVeto) && PassPhi_nFDCuts) {
                                                    if (Momentum >= P_max) {
                                                        P_max = Momentum;
                                                        NeutronsFD_ind_mom_max = i;
                                                    }

                                                    neutrons_FD_ECALveto.push_back(allParticles[i]);
                                                    neutrons_FD_ECALveto_ind.push_back(i);
                                                }  // end of clas12root neutron or 'photon' if
                                            }
                                        }
                                    }  // end of clas12root neutron or 'photon' if
                                } else {
                                    if (pid_temp == 2112) {
                                        double Momentum = CalcPnFD(allParticles[i], electrons[0], starttime);

                                        double Path_nFD = CalcPathnFD(allParticles[i], electrons[0]);
                                        double reco_ToF_nFD = CalcToFnFD(allParticles[i], starttime);

                                        // bool PassMomTh = true;
                                        bool PassMomTh = (Momentum >= 0.4);
                                        bool PassECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);

                                        if (PassMomTh && PassECALeadgeCuts) {
                                            if (Momentum >= P_max) {
                                                P_max = Momentum;
                                                NeutronsFD_ind_mom_max = i;
                                            }

                                            neutrons_FD_ECALveto.push_back(allParticles[i]);
                                            neutrons_FD_ECALveto_ind.push_back(i);
                                        }  // end of clas12root neutron or 'photon' if
                                    }
                                }
                            }  // end of neutral and in the FD if
                        }

                        bool Pass_nFD_Multi_cut_ECALveto = (!apply_nFD_multi_cut || neutrons_FD_ECALveto.size() == 1);

                        if (Pass_nFD_Multi_cut_ECALveto) {
                            if (neutrons_FD_ECALveto.size() != neutrons_FD_ECALveto_ind.size()) {
                                cout << "\n\nError! neutrons_FD_ECALveto.size() is different from neutrons_FD_ECALveto_ind.size()! Aborting...\n\n", exit(1);
                            }

                            if (NeutronsFD_ind_mom_max != -1) {
                                bool ParticleInPCAL = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                                bool ParticleInECIN = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                                bool ParticleInECOUT = (allParticles[NeutronsFD_ind_mom_max]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
                                auto Neutron_ECAL_detlayer = ParticleInECIN ? clas12::ECIN : clas12::ECOUT;                             // find first layer of hit

                                double Mom_neut_1e_cut = CalcPnFD(allParticles[NeutronsFD_ind_mom_max], electrons[0], starttime);
                                double Theta_neut_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getTheta() * 180.0 / pi;
                                double Phi_neut_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getPhi() * 180.0 / pi;

                                // bool PassMomTh = true;
                                bool PassMomTh = (Mom_neut_1e_cut >= 0.4);
                                bool PassECALeadgeCuts = (allParticles[NeutronsFD_ind_mom_max]->cal(Neutron_ECAL_detlayer)->getLv() > 14. &&
                                                          allParticles[NeutronsFD_ind_mom_max]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                                bool NeutronPassVeto_1e_cut = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, NeutronsFD_ind_mom_max, cPart_veto_radius);
                                // bool NeutronPassVeto_1e_cut =
                                //     NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, NeutronsFD_ind_mom_max, apply_PCAL_neutral_veto, cPart_veto_radius, nPart_veto_radius);

                                bool PassPhi_nFDCuts = true;
                                int nFD_nSector = allParticles[NeutronsFD_ind_mom_max]->cal(Neutron_ECAL_detlayer)->getSector();
                                int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();
                                if (OnlyGood_nFD) { PassPhi_nFDCuts = (abs(nFD_nSector - e_nSector) == 3); }
                                if (OnlyBad_nFD) { PassPhi_nFDCuts = !(abs(nFD_nSector - e_nSector) == 3); }

                                if (PassMomTh && PassECALeadgeCuts && NeutronPassVeto_1e_cut && PassPhi_nFDCuts)  // FOR nFD eff test!
                                {
                                    aMaps_master.hFillMaps("Reco", "Neutron", Mom_neut_1e_cut, Theta_neut_1e_cut, Phi_neut_1e_cut, weight);
                                }  // end of if id. reco leading neutron
                            }

                            for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
                                bool ParticleInPCAL = (neutrons_FD_ECALveto[i]->cal(clas12::PCAL)->getDetector() == 7);         // PCAL hit
                                bool ParticleInECIN = (neutrons_FD_ECALveto[i]->cal(clas12::ECIN)->getDetector() == 7);         // ECIN hit
                                bool ParticleInECOUT = (neutrons_FD_ECALveto[i]->cal(clas12::ECOUT)->getDetector() == 7);       // ECOUT hit
                                auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit
                                if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_ECALveto is in the PCAL! Aborting...\n\n", exit(1); }

                                double Path_nFD = CalcPathnFD(neutrons_FD_ECALveto[i], electrons[0]);
                                double reco_ToF_nFD = CalcToFnFD(neutrons_FD_ECALveto[i], starttime);
                                double reco_Beta_nFD = Path_nFD / (reco_ToF_nFD * c);
                                double Edep_ECAL_nFD = neutrons_FD_ECALveto[i]->cal(clas12::PCAL)->getEnergy() + neutrons_FD_ECALveto[i]->cal(ECIN)->getEnergy() +
                                                       neutrons_FD_ECALveto[i]->cal(ECOUT)->getEnergy();

                                TVector3 reco_P_nFD;
                                reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_ECALveto[i], electrons[0], starttime), neutrons_FD_ECALveto[i]->getTheta(), neutrons_FD_ECALveto[i]->getPhi());

                                if ((reco_P_nFD.Mag() > P_max) && (neutrons_FD_ECALveto_ind.at(i) != NeutronsFD_ind_mom_max)) {
                                    cout << "\n\nError! P_max is is not of the leading neutron!\n";
                                    cout << "P_max = " << P_max << "\n";
                                    cout << "reco_P_nFD.Mag() = " << reco_P_nFD.Mag() << "\n";
                                    cout << "Aborting...\n\n", exit(1);
                                }

                                h_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                                h_reco_theta_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                                h_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                                h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                                    h_reco_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                                    h_reco_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                                    h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                }

                                h_Edep_ECAL_nFD_ECALveto_1e_cut->Fill(Edep_ECAL_nFD, weight);
                                h_beta_nFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, weight);
                                h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, Edep_ECAL_nFD, weight);

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_Edep_ECAL_LnFD_ECALveto_1e_cut->Fill(Edep_ECAL_nFD, weight);
                                    h_beta_LnFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, weight);
                                    h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, Edep_ECAL_nFD, weight);
                                }

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

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
                                    h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                                    h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                                    h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Theta() * 180 / M_PI, weight);
                                    h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                    h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                    h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Phi() * 180 / M_PI, weight);
                                    h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                    h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                    h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                                    h_reco_LnFD_multi_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_ECALveto.size(), weight);
                                }

                                // if (ParticleInECIN || ParticleInECOUT) {
                                //     h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                                //     h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //     h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //     h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                                //     h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //     h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);

                                //     if (ParticleInECIN) {
                                //         h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //         h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //     }

                                //     if (ParticleInECOUT) {
                                //         h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //         h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //     }
                                // }

                                h_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                h_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(
                                    CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(),
                                                                                                        weight);
                                h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(),
                                                                                                    weight);

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                    h_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                    h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(
                                        CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                    h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                              reco_P_nFD.Mag(), weight);
                                    h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI),
                                                                                                          reco_P_nFD.Mag(), weight);
                                    h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                              neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                                    h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                     neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                                    h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                           electrons[0]->par()->getStatus(), weight);
                                    h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                  electrons[0]->par()->getStatus(), weight);

                                    if (ParticleInPCAL) {
                                        h_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                        h_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                        h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut->Fill(
                                            CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                        h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                            reco_P_nFD.Mag(), weight);
                                        h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_PCAL_only_ECALveto_1e_cut->Fill(
                                            CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(), weight);
                                        h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                            neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                                        h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_PCAL_only_ECALveto_1e_cut->Fill(
                                            reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                                        h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_PCAL_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                         electrons[0]->par()->getStatus(), weight);
                                        h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_PCAL_only_ECALveto_1e_cut->Fill(
                                            reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                                    } else if (ParticleInECIN) {
                                        h_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                        h_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                        h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut->Fill(
                                            CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                        h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                            reco_P_nFD.Mag(), weight);
                                        h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECIN_only_ECALveto_1e_cut->Fill(
                                            CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(), weight);
                                        h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                            neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                                        h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECIN_only_ECALveto_1e_cut->Fill(
                                            reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                                        h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECIN_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                         electrons[0]->par()->getStatus(), weight);
                                        h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECIN_only_ECALveto_1e_cut->Fill(
                                            reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                                    } else if (ParticleInECOUT) {
                                        h_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                        h_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                        h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut->Fill(
                                            CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                        h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                             reco_P_nFD.Mag(), weight);
                                        h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECOUT_only_ECALveto_1e_cut->Fill(
                                            CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(), weight);
                                        h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                             neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                                        h_LnFD_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECOUT_only_ECALveto_1e_cut->Fill(
                                            reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                                        h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_ECOUT_only_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                                          electrons[0]->par()->getStatus(), weight);
                                        h_e_status_VS_reco_theta_LnFD_minus_reco_theta_e_zoomin_ECOUT_only_ECALveto_1e_cut->Fill(
                                            reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                                    }
                                }

                                TVector3 v_nhit(neutrons_FD_ECALveto[i]->cal(detlayer)->getX(), neutrons_FD_ECALveto[i]->cal(detlayer)->getY(),
                                                neutrons_FD_ECALveto[i]->cal(detlayer)->getZ());

                                for (int j = 0; j < allParticles.size(); j++) {
                                    if (allParticles[j]->par()->getCharge() == 0) {
                                        bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                                        TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                                        if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                                            v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(),
                                                                 allParticles[j]->cal(clas12::PCAL)->getZ());
                                            TVector3 v_dist = v_nhit - v_neutral_hit;

                                            h_v_dist_nFD_ECALveto_1e_cut->Fill(v_dist.Mag(), weight);
                                            h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                                            h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                                            h_v_dist_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(
                                                v_dist.Mag(), CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);

                                            if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                                h_v_dist_LnFD_ECALveto_1e_cut->Fill(v_dist.Mag(), weight);
                                                h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                                                h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                                                h_v_dist_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(
                                                    v_dist.Mag(), CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                            }
                                        }
                                    }
                                }

                                h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), Edep_ECAL_nFD, weight);
                                h_Edep_ECAL_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), Edep_ECAL_nFD,
                                                                                                   weight);
                                h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), reco_Beta_nFD, weight);
                                h_beta_n_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_Beta_nFD,
                                                                                                weight);

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), Edep_ECAL_nFD, weight);
                                    h_Edep_ECAL_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), Edep_ECAL_nFD,
                                                                                                        weight);
                                    h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), reco_Beta_nFD, weight);
                                    h_beta_n_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_Beta_nFD,
                                                                                                     weight);
                                }
                            }

                            for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
                                bool ParticleInPCAL = (neutrons_FD_ECALveto[i]->cal(clas12::PCAL)->getDetector() == 7);         // PCAL hit
                                bool ParticleInECIN = (neutrons_FD_ECALveto[i]->cal(clas12::ECIN)->getDetector() == 7);         // ECIN hit
                                bool ParticleInECOUT = (neutrons_FD_ECALveto[i]->cal(clas12::ECOUT)->getDetector() == 7);       // ECOUT hit
                                auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit
                                if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_ECALveto is in the PCAL! Aborting...\n\n", exit(1); }

                                double Path_nFD = CalcPathnFD(neutrons_FD_ECALveto[i], electrons[0]);
                                double reco_ToF_nFD = CalcToFnFD(neutrons_FD_ECALveto[i], starttime);
                                double reco_Beta_nFD = Path_nFD / (reco_ToF_nFD * c);
                                double Edep_ECAL_nFD = neutrons_FD_ECALveto[i]->cal(clas12::PCAL)->getEnergy() + neutrons_FD_ECALveto[i]->cal(ECIN)->getEnergy() +
                                                       neutrons_FD_ECALveto[i]->cal(ECOUT)->getEnergy();

                                TVector3 reco_P_nFD;
                                reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_ECALveto[i], electrons[0], starttime), neutrons_FD_ECALveto[i]->getTheta(), neutrons_FD_ECALveto[i]->getPhi());

                                if ((reco_P_nFD.Mag() > P_max) && (neutrons_FD_ECALveto_ind.at(i) != NeutronsFD_ind_mom_max)) {
                                    cout << "\n\nError! P_max is is not of the leading neutron!\n";
                                    cout << "P_max = " << P_max << "\n";
                                    cout << "reco_P_nFD.Mag() = " << reco_P_nFD.Mag() << "\n";
                                    cout << "Aborting...\n\n", exit(1);
                                }

                                h_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                                h_reco_theta_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                                h_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                                h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                                    h_reco_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                                    h_reco_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                                    h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                }

                                h_Edep_ECAL_nFD_ECALveto_1e_cut->Fill(Edep_ECAL_nFD, weight);
                                h_beta_nFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, weight);
                                h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, Edep_ECAL_nFD, weight);

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_Edep_ECAL_LnFD_ECALveto_1e_cut->Fill(Edep_ECAL_nFD, weight);
                                    h_beta_LnFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, weight);
                                    h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, Edep_ECAL_nFD, weight);
                                }

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

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
                                    h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                                    h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                                    h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Theta() * 180 / M_PI, weight);
                                    h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                    h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                    h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Phi() * 180 / M_PI, weight);
                                    h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                    h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                    h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                                    h_reco_LnFD_multi_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_ECALveto.size(), weight);
                                }

                                // if (ParticleInECIN || ParticleInECOUT) {
                                //     h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                                //     h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //     h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //     h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                                //     h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //     h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);

                                //     if (ParticleInECIN) {
                                //         h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //         h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //     }

                                //     if (ParticleInECOUT) {
                                //         h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //         h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //     }
                                // }

                                h_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                h_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(
                                    CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(),
                                                                                                        weight);
                                h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(),
                                                                                                    weight);

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                    h_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                    h_reco_theta_LnFD_minus_reco_theta_e_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(
                                        CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                    h_reco_P_LnFD_VS_reco_theta_LnFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI,
                                                                                                              reco_P_nFD.Mag(), weight);
                                    h_reco_P_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI),
                                                                                                          reco_P_nFD.Mag(), weight);
                                }

                                TVector3 v_nhit(neutrons_FD_ECALveto[i]->cal(detlayer)->getX(), neutrons_FD_ECALveto[i]->cal(detlayer)->getY(),
                                                neutrons_FD_ECALveto[i]->cal(detlayer)->getZ());

                                for (int j = 0; j < allParticles.size(); j++) {
                                    if (allParticles[j]->par()->getCharge() == 0) {
                                        bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                                        TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                                        if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                                            v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(),
                                                                 allParticles[j]->cal(clas12::PCAL)->getZ());
                                            TVector3 v_dist = v_nhit - v_neutral_hit;

                                            h_v_dist_nFD_ECALveto_1e_cut->Fill(v_dist.Mag(), weight);
                                            h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                                            h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                                            h_v_dist_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(
                                                v_dist.Mag(), CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);

                                            if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                                h_v_dist_LnFD_ECALveto_1e_cut->Fill(v_dist.Mag(), weight);
                                                h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                                                h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                                                h_v_dist_LnFD_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(
                                                    v_dist.Mag(), CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                            }
                                        }
                                    }
                                }

                                h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), Edep_ECAL_nFD, weight);
                                h_Edep_ECAL_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), Edep_ECAL_nFD,
                                                                                                   weight);
                                h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), reco_Beta_nFD, weight);
                                h_beta_n_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_Beta_nFD,
                                                                                                weight);

                                if (neutrons_FD_ECALveto_ind.at(i) == NeutronsFD_ind_mom_max) {
                                    h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), Edep_ECAL_nFD, weight);
                                    h_Edep_ECAL_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), Edep_ECAL_nFD,
                                                                                                        weight);
                                    h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), reco_Beta_nFD, weight);
                                    h_beta_n_VS_reco_phi_LnFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_Beta_nFD,
                                                                                                     weight);
                                }

                                double phi_nFD_minus_reco_phi_e = CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI);
                            }

                            if (neutrons_FD_ECALveto.size() != 0) { h_reco_nFD_multi_ECALveto_1e_cut->Fill(neutrons_FD_ECALveto.size(), weight); }
                        }
#pragma endregion

                        //  Setting up FD neutrals (matched)
                        //  -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (matched) */
                        vector<region_part_ptr> neutrons_FD_matched;

                        double tl_P;

                        for (int i = 0; i < allParticles.size(); i++) {
                            int pid_temp = allParticles[i]->par()->getPid();

                            /*
                            if (pid_temp == 2112 && allParticles[i]->getRegion() == FD) {
                                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
                                auto Neutron_ECAL_detlayer = ParticleInECIN ? clas12::ECIN : clas12::ECOUT;        // find first layer of hit

                                // double Momentum = allParticles[i]->par()->getP();
                                double Momentum = CalcPnFD(allParticles[i], electrons[0], starttime);

                                bool PassMomTh = true;
                                // bool PassMomTh = (Momentum >= 0.4);
                                // bool PassECALeadgeCuts = true;
                                bool PassECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                                // bool PassVeto = true;
                                bool PassVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);
                                // bool PassVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, 100,apply_PCAL_neutral_veto, rc_factor ,nPart_veto_radius);

                                if (PassMomTh && PassECALeadgeCuts && PassVeto) {
                                    for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                                        mcpbank->setEntry(truth_NeutronsFD.at(j));

                                        double tl__P = mcpbank->getP();
                                        double tl_Theta = mcpbank->getTheta() * 180 / M_PI;
                                        double tl_Phi = mcpbank->getPhi() * 180 / M_PI;

                                        double reco_Theta = allParticles[i]->getTheta() * 180 / M_PI;
                                        double reco_Phi = allParticles[i]->getPhi() * 180 / M_PI;

                                        bool thetaCut = (fabs(tl_Theta - reco_Theta) <= 2.);
                                        bool phiCut = (fabs(tl_Phi - reco_Phi) <= 5.);

                                        double tl_E_nFD = sqrt(m_n * m_n + tl_P * tl_P);

                                        if (thetaCut && phiCut) {
                                            tl_P = tl__P;
                                            neutrons_FD_matched.push_back(allParticles[i]);

                                            h_truth_P_nFD_matched_1e_cut->Fill(tl_P, weight);
                                            h_truth_theta_nFD_matched_1e_cut->Fill(tl_Theta, weight);
                                            h_truth_phi_nFD_matched_1e_cut->Fill(tl_Phi, weight);
                                            h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut->Fill(tl_Phi, tl_Theta, weight);
                                        }
                                    }
                                }  // end of clas12root neutron or 'photon' if
                            }  // end of clas12root neutron or 'photon' if
                            */

                            if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (pid_temp != 0)) {  // If particle is neutral and in the FD
                                bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);                           // PCAL hit
                                bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);                           // ECIN hit
                                bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);                         // ECOUT hit
                                auto Neutron_ECAL_detlayer = ParticleInECIN ? clas12::ECIN : clas12::ECOUT;                               // find first layer of hit

                                if (apply_neutFD_redef) {
                                    if ((pid_temp == 2112) || (pid_temp == 22)) {
                                        if (!ParticleInPCAL) {  // if there is a neutron or a 'photon' without a PCAL hit
                                            if (ParticleInECIN || ParticleInECOUT) {
                                                double Momentum = CalcPnFD(allParticles[i], electrons[0], starttime);

                                                // bool PassMomTh = true;
                                                bool PassMomTh = (Momentum >= 0.4);
                                                bool PassECALeadgeCuts =
                                                    (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                                                bool PassVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);
                                                // bool PassVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, apply_PCAL_neutral_veto, cPart_veto_radius, nPart_veto_radius);

                                                bool PassPhi_nFDCuts = true;
                                                int nFD_nSector = allParticles[i]->cal(Neutron_ECAL_detlayer)->getSector();
                                                int e_nSector = electrons[0]->cal(Electron_ECAL_detlayer)->getSector();
                                                if (OnlyGood_nFD) { PassPhi_nFDCuts = (abs(nFD_nSector - e_nSector) == 3); }
                                                if (OnlyBad_nFD) { PassPhi_nFDCuts = !(abs(nFD_nSector - e_nSector) == 3); }

                                                if (PassMomTh && PassECALeadgeCuts && (!apply_ECAL_veto || PassVeto) && PassPhi_nFDCuts) {
                                                    for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                                                        mcpbank->setEntry(truth_NeutronsFD.at(j));

                                                        double tl__P = mcpbank->getP();
                                                        double tl_Theta = mcpbank->getTheta() * 180 / M_PI;
                                                        double tl_Phi = mcpbank->getPhi() * 180 / M_PI;

                                                        double reco_Theta = allParticles[i]->getTheta() * 180 / M_PI;
                                                        double reco_Phi = allParticles[i]->getPhi() * 180 / M_PI;

                                                        bool thetaCut = (fabs(tl_Theta - reco_Theta) <= 2.);
                                                        bool phiCut = (fabs(tl_Phi - reco_Phi) <= 5.);

                                                        double tl_E_nFD = sqrt(m_n * m_n + tl_P * tl_P);

                                                        if (thetaCut && phiCut) {
                                                            tl_P = tl__P;
                                                            neutrons_FD_matched.push_back(allParticles[i]);

                                                            h_truth_P_nFD_matched_1e_cut->Fill(tl_P, weight);
                                                            h_truth_theta_nFD_matched_1e_cut->Fill(tl_Theta, weight);
                                                            h_truth_phi_nFD_matched_1e_cut->Fill(tl_Phi, weight);
                                                            h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut->Fill(tl_Phi, tl_Theta, weight);
                                                        }
                                                    }
                                                }  // end of clas12root neutron or 'photon' if
                                            }
                                        }
                                    }  // end of clas12root neutron or 'photon' if
                                } else {
                                    if (pid_temp == 2112) {
                                        double Momentum = CalcPnFD(allParticles[i], electrons[0], starttime);

                                        // bool PassMomTh = true;
                                        bool PassMomTh = (Momentum >= 0.4);
                                        bool PassECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);

                                        if (PassMomTh && PassECALeadgeCuts) {
                                            for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                                                mcpbank->setEntry(truth_NeutronsFD.at(j));

                                                double tl__P = mcpbank->getP();
                                                double tl_Theta = mcpbank->getTheta() * 180 / M_PI;
                                                double tl_Phi = mcpbank->getPhi() * 180 / M_PI;

                                                double reco_Theta = allParticles[i]->getTheta() * 180 / M_PI;
                                                double reco_Phi = allParticles[i]->getPhi() * 180 / M_PI;

                                                bool thetaCut = (fabs(tl_Theta - reco_Theta) <= 2.);
                                                bool phiCut = (fabs(tl_Phi - reco_Phi) <= 5.);

                                                double tl_E_nFD = sqrt(m_n * m_n + tl_P * tl_P);

                                                if (thetaCut && phiCut) {
                                                    tl_P = tl__P;
                                                    neutrons_FD_matched.push_back(allParticles[i]);

                                                    h_truth_P_nFD_matched_1e_cut->Fill(tl_P, weight);
                                                    h_truth_theta_nFD_matched_1e_cut->Fill(tl_Theta, weight);
                                                    h_truth_phi_nFD_matched_1e_cut->Fill(tl_Phi, weight);
                                                    h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut->Fill(tl_Phi, tl_Theta, weight);
                                                }
                                            }
                                        }  // end of clas12root neutron or 'photon' if
                                    }
                                }
                            }  // end of neutral and in the FD if
                        }

                        bool Pass_nFD_Multi_cut_matched = (!apply_nFD_multi_cut || neutrons_FD_matched.size() == 1);

                        if (Pass_nFD_Multi_cut_matched) {
                            for (int i = 0; i < neutrons_FD_matched.size(); i++) {
                                bool ParticleInPCAL = (neutrons_FD_matched[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
                                bool ParticleInECIN = (neutrons_FD_matched[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
                                bool ParticleInECOUT = (neutrons_FD_matched[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
                                if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_matched is in the PCAL! Aborting...\n\n", exit(1); }
                                auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit

                                double Path_nFD = CalcPathnFD(neutrons_FD_matched[i], electrons[0]);
                                double reco_ToF_nFD = CalcToFnFD(neutrons_FD_matched[i], starttime);
                                double reco_Beta_nFD = Path_nFD / (reco_ToF_nFD * c);
                                double Edep_ECAL_nFD =
                                    neutrons_FD_matched[i]->cal(clas12::PCAL)->getEnergy() + neutrons_FD_matched[i]->cal(ECIN)->getEnergy() + neutrons_FD_matched[i]->cal(ECOUT)->getEnergy();

                                TVector3 reco_P_nFD;
                                reco_P_nFD.SetMagThetaPhi(tl_P, neutrons_FD_matched[i]->getTheta(), neutrons_FD_matched[i]->getPhi());

                                h_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight);
                                h_reco_theta_nFD_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                                h_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                                h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                h_Edep_ECAL_matched_1e_cut->Fill(Edep_ECAL_nFD, weight);
                                h_beta_n_matched_1e_cut->Fill(reco_Beta_nFD, weight);
                                h_Edep_ECAL_VS_beta_n_matched_1e_cut->Fill(reco_Beta_nFD, Edep_ECAL_nFD, weight);
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
                                h_reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_matched.size(), weight);

                                // if (ParticleInECIN || ParticleInECOUT) {
                                //     h_reco_P_nFD_VS_reco_L_ECAL_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                                //     h_reco_theta_nFD_VS_reco_L_ECAL_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //     h_reco_phi_nFD_VS_reco_L_ECAL_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //     h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                                //     h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //     h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);

                                //     if (ParticleInECIN) {
                                //         h_reco_P_nFD_VS_reco_L_ECIN_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_reco_L_ECIN_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_reco_L_ECIN_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //         h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //     }

                                //     if (ParticleInECOUT) {
                                //         h_reco_P_nFD_VS_reco_L_ECOUT_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_reco_L_ECOUT_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_reco_L_ECOUT_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //         h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                                //         h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                                //         h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                                //     }
                                // }

                                h_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                h_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(
                                    CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
                                h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(),
                                                                                                       weight);
                                h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(),
                                                                                                   weight);

                                TVector3 v_nhit(neutrons_FD_matched[i]->cal(detlayer)->getX(), neutrons_FD_matched[i]->cal(detlayer)->getY(), neutrons_FD_matched[i]->cal(detlayer)->getZ());

                                for (int j = 0; j < allParticles.size(); j++) {
                                    if (allParticles[j]->par()->getCharge() == 0) {
                                        bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                                        TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                                        if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                                            v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(),
                                                                 allParticles[j]->cal(clas12::PCAL)->getZ());
                                            TVector3 v_dist = v_nhit - v_neutral_hit;

                                            h_v_dist_nFD_matched_1e_cut->Fill(v_dist.Mag(), weight);
                                            h_v_dist_nFD_VS_reco_P_nFD_matched_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                                            h_v_dist_nFD_VS_reco_P_e_matched_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                                            h_v_dist_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(
                                                v_dist.Mag(), CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                                        }
                                    }
                                }

                                h_Edep_ECAL_VS_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), Edep_ECAL_nFD, weight);
                                h_Edep_ECAL_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), Edep_ECAL_nFD,
                                                                                                  weight);
                                h_beta_n_VS_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), reco_Beta_nFD, weight);
                                h_beta_n_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi1(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_Beta_nFD, weight);
                            }

                            if (neutrons_FD_matched.size() != 0) { h_reco_nFD_multi_matched_1e_cut->Fill(neutrons_FD_matched.size(), weight); }
                        }

#pragma endregion

#pragma endregion
                    }

#pragma region /* Organize histograms */

                    /////////////////////////////////////////////////////
                    // Organize histograms
                    /////////////////////////////////////////////////////
                    cout << counter << endl;

                    for (int i = 0; i < HistoList_electron_cuts.size(); i++) {
                        if (HistoList_electron_cuts[i]->InheritsFrom("TH1D")) {
                            HistoList_electron_cuts[i]->Sumw2();
                            HistoList_electron_cuts[i]->SetMinimum(0);
                            HistoList_electron_cuts[i]->SetLineWidth(2);
                            HistoList_electron_cuts[i]->SetLineColor(kRed);
                        }

                        HistoList_electron_cuts[i]->GetXaxis()->CenterTitle();
                        HistoList_electron_cuts[i]->GetYaxis()->CenterTitle();
                    }

                    for (int i = 0; i < HistoList_raw.size(); i++) {
                        if (HistoList_raw[i]->InheritsFrom("TH1D")) {
                            HistoList_raw[i]->Sumw2();
                            HistoList_raw[i]->SetMinimum(0);
                            HistoList_raw[i]->SetLineWidth(2);
                            HistoList_raw[i]->SetLineColor(kRed);
                        }

                        HistoList_raw[i]->GetXaxis()->CenterTitle();
                        HistoList_raw[i]->GetYaxis()->CenterTitle();
                    }

                    for (int i = 0; i < HistoList_clas12reco.size(); i++) {
                        if (HistoList_clas12reco[i]->InheritsFrom("TH1D")) {
                            HistoList_clas12reco[i]->Sumw2();
                            HistoList_clas12reco[i]->SetMinimum(0);
                            HistoList_clas12reco[i]->SetLineWidth(2);
                            HistoList_clas12reco[i]->SetLineColor(kRed);
                        }

                        HistoList_clas12reco[i]->GetXaxis()->CenterTitle();
                        HistoList_clas12reco[i]->GetYaxis()->CenterTitle();
                    }

                    for (int i = 0; i < HistoList_redef.size(); i++) {
                        if (HistoList_redef[i]->InheritsFrom("TH1D")) {
                            HistoList_redef[i]->Sumw2();
                            HistoList_redef[i]->SetMinimum(0);
                            HistoList_redef[i]->SetLineWidth(2);
                            HistoList_redef[i]->SetLineColor(kRed);
                        }

                        HistoList_redef[i]->GetXaxis()->CenterTitle();
                        HistoList_redef[i]->GetYaxis()->CenterTitle();
                    }

                    for (int i = 0; i < HistoList.size(); i++) {
                        if (HistoList[i]->InheritsFrom("TH1D")) {
                            HistoList[i]->Sumw2();
                            HistoList[i]->SetMinimum(0);
                            HistoList[i]->SetLineWidth(2);
                            HistoList[i]->SetLineColor(kRed);
                        }

                        HistoList[i]->GetXaxis()->CenterTitle();
                        HistoList[i]->GetYaxis()->CenterTitle();
                    }

                    /////////////////////////////////////////////////////
                    // Now create the output PDFs
                    /////////////////////////////////////////////////////
                    int pixelx = 1980;
                    int pixely = 1530;
                    TCanvas* myText = new TCanvas("myText", "myText", pixelx, pixely);
                    TLatex titles;
                    TLatex text;
                    titles.SetTextSize(0.075);
                    text.SetTextSize(0.03);

                    gStyle->SetOptStat("ourmen");

#pragma region /* Acceptance maps */

                    if (plot_AMaps) {
                        TCanvas* myCanvas_aMaps = new TCanvas("myCanvas_aMaps", "myCanvas_aMaps", pixelx, pixely);

                        myCanvas_aMaps->cd();
                        myCanvas_aMaps->SetBottomMargin(0.14), myCanvas_aMaps->SetLeftMargin(0.16), myCanvas_aMaps->SetRightMargin(0.12);

                        aMaps_master.DrawAndSaveMaps(SampleName, myCanvas_aMaps, OutputDirAMapsMaps);
                    }

#pragma endregion

#pragma region /* General histograms */

                    if (plot_tester_histograms) {
#pragma region /* Print electron cuts plots */
                        // TCanvas* myCanvas_electron_cuts = new TCanvas("myPage_electron_cuts", "myPage_electron_cuts", pixelx, pixely);
                        TCanvas* myCanvas_electron_cuts = new TCanvas("myPage_electron_cuts", "myPage_electron_cuts", pixelx, pixely);

                        std::string electron_cuts_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/electron_cuts.pdf";
                        char fileName_electron_cuts[electron_cuts_PDF_fileName.length()];
                        sprintf(fileName_electron_cuts, "%s[", electron_cuts_PDF_fileName.c_str());
                        myText->SaveAs(fileName_electron_cuts);
                        sprintf(fileName_electron_cuts, "%s", electron_cuts_PDF_fileName.c_str());

                        /////////////////////////////////////
                        // CND Neutron Information
                        /////////////////////////////////////
                        myText->cd();

                        titles.DrawLatex(0.05, 0.90, "Uniform sample of (e,e'n) events (truth-level)");

                        text.DrawLatex(0.05, 0.80, "OutFolderName:");
                        text.DrawLatex(0.10, 0.75, (OutFolderName).c_str());

                        text.DrawLatex(0.05, 0.70, ("Beam energy: " + Ebeam_status0).c_str());
                        text.DrawLatex(0.05, 0.65, ("ConstrainedE: " + bt::BoolToString(ConstrainedE)).c_str());

                        text.DrawLatex(0.05, 0.60, ("apply_ECAL_veto = " + BoolToString(apply_ECAL_veto)).c_str());
                        text.DrawLatex(0.05, 0.55, ("cPart_veto_radius = " + ToStringWithPrecision(cPart_veto_radius, 0)).c_str());

                        // text.DrawLatex(0.05, 0.45, ("apply_PCAL_neutral_veto = " + BoolToString(apply_PCAL_neutral_veto)).c_str());
                        // text.DrawLatex(0.05, 0.4, ("nPart_veto_radius = " + ToStringWithPrecision(nPart_veto_radius, 0)).c_str());

                        myText->Print(fileName_electron_cuts, "pdf");
                        myText->Clear();

                        for (int i = 0; i < HistoList_electron_cuts.size(); i++) {
                            for (int j = 0; j < HistSubjects.size(); j++) {
                                if (FirstPrint.at(j) && FindSubstring(HistoList_electron_cuts[i]->GetTitle(), HistSubjects.at(j))) {
                                    myText->cd();
                                    titles.DrawLatex(0.3, 0.5, HistSubjects2.at(j).c_str());
                                    myText->Print(fileName_electron_cuts, "pdf");
                                    myText->Clear();

                                    myCanvas_electron_cuts->cd(1);
                                    FirstPrint.at(j) = false;
                                    break;
                                }
                            }

                            // electron_cuts_PDF_fileName->Divide(2,1);

                            myCanvas_electron_cuts->cd();

                            myCanvas_electron_cuts->cd()->SetGrid();
                            myCanvas_electron_cuts->cd()->SetBottomMargin(0.14), myCanvas_electron_cuts->cd()->SetLeftMargin(0.16), myCanvas_electron_cuts->cd()->SetRightMargin(0.12);

                            HistoList_electron_cuts[i]->GetYaxis()->SetTitleOffset(1.5);
                            HistoList_electron_cuts[i]->GetXaxis()->SetTitleOffset(1.1);

                            gPad->SetRightMargin(0.23);

                            // // Set the PDF title and header for the bookmark
                            // std::string Title = HistoList_electron_cuts[i]->GetTitle();
                            // gStyle->SetTitlePS(Title.c_str());  // This sets the title in metadata
                            //                                     // gStyle->SetHeaderPS(("[ /Title " + Title + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title
                            // gStyle->SetHeaderPS(("[ /Page " + to_string(i + 1) + " /View [/Fit] /Title (myTitle) ] /OUT pdfmark").c_str());

                            if (HistoList_electron_cuts[i]->InheritsFrom("TH1D")) {
                                HistoList_electron_cuts[i]->Draw();
                            } else if (HistoList_electron_cuts[i]->InheritsFrom("TH2D")) {
                                HistoList_electron_cuts[i]->Draw("colz");

                                if (HistoList_electron_cuts[i]->GetEntries() != 0) {
                                    gPad->Update();
                                    TPaletteAxis* palette = (TPaletteAxis*)HistoList_electron_cuts[i]->GetListOfFunctions()->FindObject("palette");
                                    palette->SetY2NDC(0.5);
                                    gPad->Modified();
                                    gPad->Update();
                                }
                            }

                            myCanvas_electron_cuts->Print(fileName_electron_cuts, "pdf");
                            myCanvas_electron_cuts->Clear();
                        }

                        sprintf(fileName_electron_cuts, "%s]", electron_cuts_PDF_fileName.c_str());
                        myCanvas_electron_cuts->Print(fileName_electron_cuts, "pdf");

#pragma endregion

#pragma region /* Print raw plots */
                        // TCanvas* myCanvas_raw = new TCanvas("myPage_raw", "myPage_raw", pixelx, pixely);
                        TCanvas* myCanvas_raw = new TCanvas("myPage_raw", "myPage_raw", pixelx, pixely);

                        std::string raw_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/raw_nFD_plots.pdf";
                        char fileName_raw[raw_PDF_fileName.length()];
                        sprintf(fileName_raw, "%s[", raw_PDF_fileName.c_str());
                        myText->SaveAs(fileName_raw);
                        sprintf(fileName_raw, "%s", raw_PDF_fileName.c_str());

                        /////////////////////////////////////
                        // CND Neutron Information
                        /////////////////////////////////////
                        myText->cd();

                        titles.DrawLatex(0.05, 0.90, "Uniform sample of (e,e'n) events (truth-level)");

                        text.DrawLatex(0.05, 0.80, "OutFolderName:");
                        text.DrawLatex(0.10, 0.75, (OutFolderName).c_str());

                        text.DrawLatex(0.05, 0.70, ("Beam energy: " + Ebeam_status0).c_str());
                        text.DrawLatex(0.05, 0.65, ("ConstrainedE: " + bt::BoolToString(ConstrainedE)).c_str());

                        text.DrawLatex(0.05, 0.60, ("apply_ECAL_veto = " + BoolToString(apply_ECAL_veto)).c_str());
                        text.DrawLatex(0.05, 0.55, ("cPart_veto_radius = " + ToStringWithPrecision(cPart_veto_radius, 0)).c_str());

                        // text.DrawLatex(0.05, 0.45, ("apply_PCAL_neutral_veto = " + BoolToString(apply_PCAL_neutral_veto)).c_str());
                        // text.DrawLatex(0.05, 0.4, ("nPart_veto_radius = " + ToStringWithPrecision(nPart_veto_radius, 0)).c_str());

                        myText->Print(fileName_raw, "pdf");
                        myText->Clear();

                        for (int i = 0; i < HistoList_raw.size(); i++) {
                            for (int j = 0; j < HistSubjects.size(); j++) {
                                if (FirstPrint.at(j) && FindSubstring(HistoList_raw[i]->GetTitle(), HistSubjects.at(j))) {
                                    myText->cd();
                                    titles.DrawLatex(0.3, 0.5, HistSubjects2.at(j).c_str());
                                    myText->Print(fileName_raw, "pdf");
                                    myText->Clear();

                                    myCanvas_raw->cd(1);
                                    FirstPrint.at(j) = false;
                                    break;
                                }
                            }

                            // raw_PDF_fileName->Divide(2,1);

                            myCanvas_raw->cd();

                            myCanvas_raw->cd()->SetGrid();
                            myCanvas_raw->cd()->SetBottomMargin(0.14), myCanvas_raw->cd()->SetLeftMargin(0.16), myCanvas_raw->cd()->SetRightMargin(0.12);

                            HistoList_raw[i]->GetYaxis()->SetTitleOffset(1.5);
                            HistoList_raw[i]->GetXaxis()->SetTitleOffset(1.1);

                            gPad->SetRightMargin(0.23);

                            // // Set the PDF title and header for the bookmark
                            // std::string Title = HistoList_raw[i]->GetTitle();
                            // gStyle->SetTitlePS(Title.c_str());  // This sets the title in metadata
                            //                                     // gStyle->SetHeaderPS(("[ /Title " + Title + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title
                            // gStyle->SetHeaderPS(("[ /Page " + to_string(i + 1) + " /View [/Fit] /Title (myTitle) ] /OUT pdfmark").c_str());

                            if (HistoList_raw[i]->InheritsFrom("TH1D")) {
                                HistoList_raw[i]->Draw();
                            } else if (HistoList_raw[i]->InheritsFrom("TH2D")) {
                                HistoList_raw[i]->Draw("colz");

                                if (HistoList_raw[i]->GetEntries() != 0) {
                                    gPad->Update();
                                    TPaletteAxis* palette = (TPaletteAxis*)HistoList_raw[i]->GetListOfFunctions()->FindObject("palette");
                                    palette->SetY2NDC(0.475);
                                    gPad->Modified();
                                    gPad->Update();
                                }
                            }

                            myCanvas_raw->Print(fileName_raw, "pdf");
                            myCanvas_raw->Clear();
                        }

                        sprintf(fileName_raw, "%s]", raw_PDF_fileName.c_str());
                        myCanvas_raw->Print(fileName_raw, "pdf");

#pragma endregion

#pragma region /* Print clas12reco plots */
                        // TCanvas* myCanvas_clas12reco = new TCanvas("myPage_clas12reco", "myPage_clas12reco", pixelx, pixely);
                        TCanvas* myCanvas_clas12reco = new TCanvas("myPage_clas12reco", "myPage_clas12reco", pixelx, pixely);

                        std::string clas12reco_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/clas12reco_nFD_plots.pdf";
                        char fileName_clas12reco[clas12reco_PDF_fileName.length()];
                        sprintf(fileName_clas12reco, "%s[", clas12reco_PDF_fileName.c_str());
                        myText->SaveAs(fileName_clas12reco);
                        sprintf(fileName_clas12reco, "%s", clas12reco_PDF_fileName.c_str());

                        /////////////////////////////////////
                        // CND Neutron Information
                        /////////////////////////////////////
                        myText->cd();

                        titles.DrawLatex(0.05, 0.90, "Uniform sample of (e,e'n) events (truth-level)");

                        text.DrawLatex(0.05, 0.80, "OutFolderName:");
                        text.DrawLatex(0.10, 0.75, (OutFolderName).c_str());

                        text.DrawLatex(0.05, 0.70, ("Beam energy: " + Ebeam_status0).c_str());
                        text.DrawLatex(0.05, 0.65, ("ConstrainedE: " + bt::BoolToString(ConstrainedE)).c_str());

                        text.DrawLatex(0.05, 0.60, ("apply_ECAL_veto = " + BoolToString(apply_ECAL_veto)).c_str());
                        text.DrawLatex(0.05, 0.55, ("cPart_veto_radius = " + ToStringWithPrecision(cPart_veto_radius, 0)).c_str());

                        // text.DrawLatex(0.05, 0.45, ("apply_PCAL_neutral_veto = " + BoolToString(apply_PCAL_neutral_veto)).c_str());
                        // text.DrawLatex(0.05, 0.4, ("nPart_veto_radius = " + ToStringWithPrecision(nPart_veto_radius, 0)).c_str());

                        myText->Print(fileName_clas12reco, "pdf");
                        myText->Clear();

                        for (int i = 0; i < HistoList_clas12reco.size(); i++) {
                            for (int j = 0; j < HistSubjects.size(); j++) {
                                if (FirstPrint.at(j) && FindSubstring(HistoList_clas12reco[i]->GetTitle(), HistSubjects.at(j))) {
                                    myText->cd();
                                    titles.DrawLatex(0.3, 0.5, HistSubjects2.at(j).c_str());
                                    myText->Print(fileName_clas12reco, "pdf");
                                    myText->Clear();

                                    myCanvas_clas12reco->cd(1);
                                    FirstPrint.at(j) = false;
                                    break;
                                }
                            }

                            // clas12reco_PDF_fileName->Divide(2,1);

                            myCanvas_clas12reco->cd();

                            myCanvas_clas12reco->cd()->SetGrid();
                            myCanvas_clas12reco->cd()->SetBottomMargin(0.14), myCanvas_clas12reco->cd()->SetLeftMargin(0.16), myCanvas_clas12reco->cd()->SetRightMargin(0.12);

                            HistoList_clas12reco[i]->GetYaxis()->SetTitleOffset(1.5);
                            HistoList_clas12reco[i]->GetXaxis()->SetTitleOffset(1.1);

                            gPad->SetRightMargin(0.23);

                            // // Set the PDF title and header for the bookmark
                            // std::string Title = HistoList_clas12reco[i]->GetTitle();
                            // gStyle->SetTitlePS(Title.c_str());  // This sets the title in metadata
                            //                                     // gStyle->SetHeaderPS(("[ /Title " + Title + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title
                            // gStyle->SetHeaderPS(("[ /Page " + to_string(i + 1) + " /View [/Fit] /Title (myTitle) ] /OUT pdfmark").c_str());

                            if (HistoList_clas12reco[i]->InheritsFrom("TH1D")) {
                                HistoList_clas12reco[i]->Draw();
                            } else if (HistoList_clas12reco[i]->InheritsFrom("TH2D")) {
                                HistoList_clas12reco[i]->Draw("colz");

                                if (HistoList_clas12reco[i]->GetEntries() != 0) {
                                    gPad->Update();
                                    TPaletteAxis* palette = (TPaletteAxis*)HistoList_clas12reco[i]->GetListOfFunctions()->FindObject("palette");
                                    palette->SetY2NDC(0.475);
                                    gPad->Modified();
                                    gPad->Update();
                                }
                            }

                            myCanvas_clas12reco->Print(fileName_clas12reco, "pdf");
                            myCanvas_clas12reco->Clear();
                        }

                        sprintf(fileName_clas12reco, "%s]", clas12reco_PDF_fileName.c_str());
                        myCanvas_clas12reco->Print(fileName_clas12reco, "pdf");

#pragma endregion

#pragma region /* Print redef plots */
                        // TCanvas* myCanvas_redef = new TCanvas("myPage_redef", "myPage_redef", pixelx, pixely);
                        TCanvas* myCanvas_redef = new TCanvas("myPage_redef", "myPage_redef", pixelx, pixely);

                        std::string redef_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/redef_nFD_plots.pdf";
                        char fileName_redef[redef_PDF_fileName.length()];
                        sprintf(fileName_redef, "%s[", redef_PDF_fileName.c_str());
                        myText->SaveAs(fileName_redef);
                        sprintf(fileName_redef, "%s", redef_PDF_fileName.c_str());

                        /////////////////////////////////////
                        // CND Neutron Information
                        /////////////////////////////////////
                        myText->cd();

                        titles.DrawLatex(0.05, 0.90, "Uniform sample of (e,e'n) events (truth-level)");

                        text.DrawLatex(0.05, 0.80, "OutFolderName:");
                        text.DrawLatex(0.10, 0.75, (OutFolderName).c_str());

                        text.DrawLatex(0.05, 0.70, ("Beam energy: " + Ebeam_status0).c_str());
                        text.DrawLatex(0.05, 0.65, ("ConstrainedE: " + bt::BoolToString(ConstrainedE)).c_str());

                        text.DrawLatex(0.05, 0.60, ("apply_ECAL_veto = " + BoolToString(apply_ECAL_veto)).c_str());
                        text.DrawLatex(0.05, 0.55, ("cPart_veto_radius = " + ToStringWithPrecision(cPart_veto_radius, 0)).c_str());

                        // text.DrawLatex(0.05, 0.45, ("apply_PCAL_neutral_veto = " + BoolToString(apply_PCAL_neutral_veto)).c_str());
                        // text.DrawLatex(0.05, 0.4, ("nPart_veto_radius = " + ToStringWithPrecision(nPart_veto_radius, 0)).c_str());

                        myText->Print(fileName_redef, "pdf");
                        myText->Clear();

                        for (int i = 0; i < HistoList_redef.size(); i++) {
                            for (int j = 0; j < HistSubjects.size(); j++) {
                                if (FirstPrint.at(j) && FindSubstring(HistoList_redef[i]->GetTitle(), HistSubjects.at(j))) {
                                    myText->cd();
                                    titles.DrawLatex(0.3, 0.5, HistSubjects2.at(j).c_str());
                                    myText->Print(fileName_redef, "pdf");
                                    myText->Clear();

                                    myCanvas_redef->cd(1);
                                    FirstPrint.at(j) = false;
                                    break;
                                }
                            }

                            // redef_PDF_fileName->Divide(2,1);

                            myCanvas_redef->cd();

                            myCanvas_redef->cd()->SetGrid();
                            myCanvas_redef->cd()->SetBottomMargin(0.14), myCanvas_redef->cd()->SetLeftMargin(0.16), myCanvas_redef->cd()->SetRightMargin(0.12);

                            HistoList_redef[i]->GetYaxis()->SetTitleOffset(1.5);
                            HistoList_redef[i]->GetXaxis()->SetTitleOffset(1.1);

                            gPad->SetRightMargin(0.23);

                            // // Set the PDF title and header for the bookmark
                            // std::string Title = HistoList_redef[i]->GetTitle();
                            // gStyle->SetTitlePS(Title.c_str());  // This sets the title in metadata
                            //                                     // gStyle->SetHeaderPS(("[ /Title " + Title + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title
                            // gStyle->SetHeaderPS(("[ /Page " + to_string(i + 1) + " /View [/Fit] /Title (myTitle) ] /OUT pdfmark").c_str());

                            if (HistoList_redef[i]->InheritsFrom("TH1D")) {
                                HistoList_redef[i]->Draw();
                            } else if (HistoList_redef[i]->InheritsFrom("TH2D")) {
                                HistoList_redef[i]->Draw("colz");

                                if (HistoList_redef[i]->GetEntries() != 0) {
                                    gPad->Update();
                                    TPaletteAxis* palette = (TPaletteAxis*)HistoList_redef[i]->GetListOfFunctions()->FindObject("palette");
                                    palette->SetY2NDC(0.475);
                                    gPad->Modified();
                                    gPad->Update();
                                }
                            }

                            myCanvas_redef->Print(fileName_redef, "pdf");
                            myCanvas_redef->Clear();
                        }

                        sprintf(fileName_redef, "%s]", redef_PDF_fileName.c_str());
                        myCanvas_redef->Print(fileName_redef, "pdf");

#pragma endregion

#pragma region /* Print neutron plots */
                        TCanvas* myCanvas = new TCanvas("myPage", "myPage", pixelx, pixely);

                        std::string nFD_eff_test_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/" + OutFileName + ".pdf";
                        char fileName[nFD_eff_test_PDF_fileName.length()];
                        sprintf(fileName, "%s[", nFD_eff_test_PDF_fileName.c_str());
                        myText->SaveAs(fileName);
                        sprintf(fileName, "%s", nFD_eff_test_PDF_fileName.c_str());

                        /////////////////////////////////////
                        // CND Neutron Information
                        /////////////////////////////////////
                        myText->cd();

                        titles.DrawLatex(0.05, 0.90, "Uniform sample of (e,e'n) events (truth-level)");

                        text.DrawLatex(0.05, 0.80, "OutFolderName:");
                        text.DrawLatex(0.10, 0.75, (OutFolderName).c_str());

                        text.DrawLatex(0.05, 0.70, ("Beam energy: " + Ebeam_status0).c_str());
                        text.DrawLatex(0.05, 0.65, ("ConstrainedE: " + bt::BoolToString(ConstrainedE)).c_str());

                        text.DrawLatex(0.05, 0.60, ("apply_ECAL_veto = " + BoolToString(apply_ECAL_veto)).c_str());
                        text.DrawLatex(0.05, 0.55, ("cPart_veto_radius = " + ToStringWithPrecision(cPart_veto_radius, 0)).c_str());

                        // text.DrawLatex(0.05, 0.45, ("apply_PCAL_neutral_veto = " + BoolToString(apply_PCAL_neutral_veto)).c_str());
                        // text.DrawLatex(0.05, 0.4, ("nPart_veto_radius = " + ToStringWithPrecision(nPart_veto_radius, 0)).c_str());

                        myText->Print(fileName, "pdf");
                        myText->Clear();

                        // Structured first flags for particles and sectors
                        std::map<std::string, bool> first_flags_scalar = {{"clas12reco", true}, {"redef", true}, {"ECALveto", true}, {"matched", true}};
                        std::map<std::string, std::array<bool, 6>> first_flags_sector;

                        for (auto& [particle, _] : first_flags_scalar) { first_flags_sector[particle] = {true, true, true, true, true, true}; }

                        std::map<std::string, bool*> first_flags;

                        for (auto& [particle, flag] : first_flags_scalar) { first_flags[particle] = &flag; }

                        std::map<std::string, std::map<int, bool*>> sector_flags;

                        for (auto& [particle, sector_array] : first_flags_sector) {
                            for (int sec = 0; sec < 6; ++sec) { sector_flags[particle][sec + 1] = &sector_array[sec]; }
                        }

                        std::map<std::string, std::string> particle_labels = {{"raw", "RAW plots"},
                                                                              {"PID", "e^{-} PID plots"},
                                                                              {"clas12reco", "CLAS12 reco. n_{FD}"},
                                                                              {"redef", "#splitline{reco. n_{FD} from}{neutron redefinition}"},
                                                                              {"ECALveto", "#splitline{Redef. reco. n_{FD} after}{ECALveto and P_{nFD} th.}"},
                                                                              {"matched", "#splitline{Redef. reco. n_{FD} after}{#splitline{ECAL veto +}{matching to TL neutrons}}"},
                                                                              {"eff", "#splitline{reco. n_{FD}}{efficiency plots}"}};

                        for (int i = 0; i < HistoList.size(); i++) {
                            std::string title = HistoList[i]->GetTitle();
                            std::string name = HistoList[i]->GetName();

                            for (const auto& [particle_key, label] : particle_labels) {
                                if (bt::FindSubstring(title, particle_key)) {
                                    myText->cd();
                                    titles.SetTextAlign(22);  // Center text both horizontally and vertically

                                    if (*first_flags[particle_key]) {
                                        std::string bookmark_title;
                                        // std::string bookmark_title = label;
                                        // std::string bookmark_title = label + " plots";

                                        if (bt::FindSubstring(label, "splitline")) {
                                            std::regex splitline_pattern(R"(#splitline\{(.*?)\}\{(.*?)\})");
                                            std::smatch matches;

                                            if (std::regex_search(label, matches, splitline_pattern)) {
                                                // matches[1] is the first part, matches[2] is the second part
                                                bookmark_title = matches[1].str() + " " + matches[2].str();
                                            } else {
                                                bookmark_title = label;
                                            }
                                        } else {
                                            bookmark_title = label;  // moved inside the else branch
                                        }

                                        std::string sanitized_bookmark_title = hf::SanitizeForBookmark(bookmark_title);
                                        titles.DrawLatex(0.5, 0.5, label.c_str());
                                        // titles.DrawLatex(0.5, 0.5, bookmark_title.c_str());
                                        myText->Print(fileName, ("pdf Title:" + sanitized_bookmark_title).c_str());
                                        myText->Clear();
                                        *first_flags[particle_key] = false;
                                        // ++plot_counter;
                                    }

                                    break;  // Stop checking other particles after match
                                }
                            }

                            // for (int j = 0; j < HistSubjects.size(); j++) {
                            //     if (FirstPrint.at(j) && FindSubstring(HistoList[i]->GetTitle(), HistSubjects.at(j))) {
                            //         myText->cd();
                            //         titles.DrawLatex(0.3, 0.5, HistSubjects2.at(j).c_str());
                            //         myText->Print(fileName, "pdf");
                            //         myText->Clear();

                            //         myCanvas->cd(1);
                            //         FirstPrint.at(j) = false;
                            //         break;
                            //     }
                            // }

                            myCanvas->cd();

                            myCanvas->cd()->SetGrid();
                            myCanvas->cd()->SetBottomMargin(0.14), myCanvas->cd()->SetLeftMargin(0.16), myCanvas->cd()->SetRightMargin(0.12);

                            HistoList[i]->GetYaxis()->SetTitleOffset(1.5);
                            HistoList[i]->GetXaxis()->SetTitleOffset(1.1);

                            gPad->SetRightMargin(0.23);

                            // // Set the PDF title and header for the bookmark
                            // std::string Title = HistoList[i]->GetTitle();
                            // gStyle->SetTitlePS(Title.c_str());  // This sets the title in metadata
                            //                                     // gStyle->SetHeaderPS(("[ /Title " + Title + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title
                            // gStyle->SetHeaderPS(("[ /Page " + to_string(i + 1) + " /View [/Fit] /Title (myTitle) ] /OUT pdfmark").c_str());

                            if (HistoList[i]->InheritsFrom("TH1D")) {
                                HistoList[i]->Draw();
                            } else if (HistoList[i]->InheritsFrom("TH2D")) {
                                myCanvas->SetLogz(0);
                                // if (FindSubstring(HistoList[i]->GetTitle(), "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} in 1e cut")) {
                                //     gPad->SetRightMargin(0.225);
                                // } else {
                                //     gPad->SetRightMargin(0.05);
                                // }
                                if (bt::FindSubstring(name, "multi")) { myCanvas->SetLogz(1); }

                                HistoList[i]->Draw("colz");

                                if (HistoList[i]->GetEntries() != 0) {
                                    gPad->Update();
                                    TPaletteAxis* palette = (TPaletteAxis*)HistoList[i]->GetListOfFunctions()->FindObject("palette");
                                    palette->SetY2NDC(0.475);
                                    gPad->Modified();
                                    gPad->Update();
                                }
                            }

                            myCanvas->Print(fileName, "pdf");
                            myCanvas->Clear();
                        }

                        sprintf(fileName, "%s]", nFD_eff_test_PDF_fileName.c_str());
                        myCanvas->Print(fileName, "pdf");

                        hf::FixPDFOrientation(nFD_eff_test_PDF_fileName);                                                                 // Fix orientation
                        hf::ReassignPDFBookmarks(bt::GetCurrentDirectory() + "/", nFD_eff_test_PDF_fileName, nFD_eff_test_PDF_fileName);  // Reassign clean bookmarks

/*                         auto GeneratePDFOutput = [&ConstrainedE, &apply_ECAL_veto, &cPart_veto_radius, &apply_PCAL_neutral_veto, &nPart_veto_radius, &InputFiles](
                                                     std::string TempOutputDir, std::string TempOutFolderName, std::string TempBaseDir, std::vector<std::string> TempInputFiles,
                                                     std::vector<TObject*> TempHistoList) {
                            int pixelx = 1980;
                            int pixely = 1530;
                            TCanvas* myText = new TCanvas("myText", "myText", pixelx, pixely);
                            TLatex titles;
                            TLatex text;
                            titles.SetTextSize(0.1);
                            text.SetTextSize(0.03);

                            gStyle->SetOptStat("ourmen");

                            TCanvas* myCanvas = new TCanvas("myPage", "myPage", pixelx, pixely);
                            double originalTopMargin = myCanvas->GetTopMargin();

                            std::string PDF_fileName = TempOutputDir + "/" + TempOutFolderName + ".pdf";
                            char fileName[PDF_fileName.length()];
                            sprintf(fileName, "%s[", PDF_fileName.c_str());
                            myText->SaveAs(fileName);
                            sprintf(fileName, "%s", PDF_fileName.c_str());

                            // Helper lambda for drawing title blocks
                            auto draw_title_block = [&](const std::string& title, const std::vector<std::string>& lines, double startY, double stepY, bool OverrideOffset = false) {
                                if (title != "") { titles.DrawLatex(0.05, startY, title.c_str()); }

                                for (size_t i = 0; i < lines.size(); ++i) {
                                    if (OverrideOffset) {
                                        text.DrawLatex(0.05, startY - (i + 1) * stepY, lines[i].c_str());
                                    } else {
                                        text.DrawLatex(0.10, startY - (i + 1) * stepY, lines[i].c_str());
                                    }
                                }
                                // titles.DrawLatex(0.05, startY, title.c_str());
                                // for (size_t i = 0; i < lines.size(); ++i) { text.DrawLatex(0.05 + (i == 0 ? 0.05 : 0.10), startY - (i + 1) * stepY, lines[i].c_str()); }
                            };

                            // First page:
                            myText->cd();
                            titles.DrawLatex(0.05, 0.9, "#gamma-n_{FD} Tester Output");
                            // text.DrawLatex(0.05, 0.80, "This output is for the Ar40 implementation in GEMC");
                            // text.DrawLatex(0.05, 0.70, ("Plots from (e,e'n) events in: #font[42]{" + TempCodeRun_status + "}").c_str());

                            draw_title_block("",
                                             {"TempBaseDir: #font[42]{" + TempBaseDir + "}", "TempInputFiles: #font[42]{TempBaseDir + " + InputFiles.substr(TempBaseDir.length()) + "}",
                                              "TempOutFolderName:", "#font[42]{" + TempOutFolderName + "}"},
                                             0.60, 0.05, true);
                            // if (TempIsData) {
                            //     draw_title_block("", {"TempInputFiles: #font[42]{" + InputFiles + "}", "TempOutFolderName:", "#font[42]{" + TempOutFolderName + "}"}, 0.65, 0.05, true);
                            // } else {
                            //     draw_title_block("",
                            //                      {"TempBaseDir: #font[42]{" + TempBaseDir + "}", "TempInputFiles: #font[42]{TempBaseDir + " + InputFiles.substr(TempBaseDir.length()) +
   "}",
                            //                       "TempOutFolderName:", "#font[42]{" + TempOutFolderName + "}"},
                            //                      0.60, 0.05, true);
                            // }

                            // text.DrawLatex(
                            //     0.05, 0.40,
                            //     ("Event counts (ApplyLimiter = " + bt::BoolToString(ApplyLimiter) + (ApplyLimiter ? ", Limiter = " + bt::ToStringWithPrecision(Limiter, 0) : "") + "):")
                            //         .c_str());
                            // draw_title_block("",
                            //                  {"Total #(events):            #font[42]{" + std::to_string(TempNumOfEvents) + "}",
                            //                   "#(Events) with any e_det:  #font[42]{" + std::to_string(TempNumOfEvents_wAny_e_det) + "}",
                            //                   "#(Events) with one e_det:  #font[42]{" + std::to_string(TempNumOfEvents_wOne_e_det) + " (" +
                            //                       bt::ToStringWithPrecision((100 * TempNumOfEvents_wOne_e_det / TempNumOfEvents_wAny_e_det), 2) + "%)}",
                            //                   "#(Events) with any e:       #font[42]{" + std::to_string(TempNumOfEvents_wAny_e) + "}",
                            //                   "#(Events) with one e:       #font[42]{" + std::to_string(TempNumOfEvents_wOne_e) + " (" +
                            //                       bt::ToStringWithPrecision((100 * TempNumOfEvents_wOne_e / TempNumOfEvents_wAny_e), 2) + "%)}"},
                            //                  0.40, 0.05);

                            myText->Print(fileName, "pdf Title: Cover");
                            myText->Clear();

                            // Second page:
                            titles.SetTextSize(0.05);
                            titles.DrawLatex(0.05, 0.90, "Code run parameters");

                            if (ConstrainedE) {
                                text.DrawLatex(0.05, 0.7, "ConstrainedE = yes");
                            } else {
                                text.DrawLatex(0.05, 0.7, "ConstrainedE = no");
                            }

                            text.DrawLatex(0.05, 0.6, ("apply_ECAL_veto = " + BoolToString(apply_ECAL_veto)).c_str());
                            text.DrawLatex(0.05, 0.55, ("cPart_veto_radius = " + ToStringWithPrecision(cPart_veto_radius, 0)).c_str());

                            text.DrawLatex(0.05, 0.45, ("apply_PCAL_neutral_veto = " + BoolToString(apply_PCAL_neutral_veto)).c_str());
                            text.DrawLatex(0.05, 0.4, ("nPart_veto_radius = " + ToStringWithPrecision(nPart_veto_radius, 0)).c_str());

                            myText->Print(fileName, "pdf Title: Parameters");
                            myText->Clear();

                            // Structured first flags for particles and sectors
                            std::map<std::string, bool> first_flags_scalar = {{"raw", true},      {"PID", true},     {"clas12reco", true}, {"redef", true},
                                                                              {"ECALveto", true}, {"matched", true}, {"eff", true}};
                            std::map<std::string, std::array<bool, 6>> first_flags_sector;

                            for (auto& [particle, _] : first_flags_scalar) { first_flags_sector[particle] = {true, true, true, true, true, true}; }

                            std::map<std::string, bool*> first_flags;

                            for (auto& [particle, flag] : first_flags_scalar) { first_flags[particle] = &flag; }

                            std::map<std::string, std::map<int, bool*>> sector_flags;

                            for (auto& [particle, sector_array] : first_flags_sector) {
                                for (int sec = 0; sec < 6; ++sec) { sector_flags[particle][sec + 1] = &sector_array[sec]; }
                            }

                            std::map<std::string, std::string> particle_labels = {{"raw", "RAW plots"},
                                                                                  {"PID", "e^{-} PID plots"},
                                                                                  {"clas12reco", "CLAS12 reco. n_{FD}"},
                                                                                  {"redef", "#splitline{reco. n_{FD} from}{neutron redefinition}"},
                                                                                  {"ECALveto", "#splitline{Redef. reco. n_{FD} after}{ECAL veto}"},
                                                                                  {"matched", "#splitline{Redef. reco. n_{FD} after}{ECAL veto + matching to TL neutrons}"},
                                                                                  {"eff", "#splitline{reco. n_{FD}}{efficiency plots}"}};

                            // For histogram processing: track current slice ID for resetting first_flags
                            std::string current_slice_id;

                            for (int i = 0; i < TempHistoList.size(); i++) {
                                std::string title = HistoList[i]->GetTitle();
                                std::string name = HistoList[i]->GetName();

                                for (const auto& [particle_key, label] : particle_labels) {
                                    if (bt::FindSubstring(title, particle_key)) {
                                        myText->cd();
                                        titles.SetTextAlign(22);  // Center text both horizontally and vertically

                                        if (*first_flags[particle_key]) {
                                            std::string bookmark_title = label + " plots";
                                            std::string sanitized_bookmark_title = hf::SanitizeForBookmark(bookmark_title);
                                            titles.DrawLatex(0.5, 0.5, bookmark_title.c_str());
                                            myText->Print(fileName, ("pdf Title:" + sanitized_bookmark_title).c_str());
                                            myText->Clear();
                                            *first_flags[particle_key] = false;
                                            // ++plot_counter;
                                        }

                                        break;  // Stop checking other particles after match
                                    }
                                }

                                for (int j = 0; j < HistSubjects.size(); j++) {
                                    if (FirstPrint.at(j) && FindSubstring(HistoList[i]->GetTitle(), HistSubjects.at(j))) {
                                        myText->cd();
                                        titles.DrawLatex(0.3, 0.5, HistSubjects2.at(j).c_str());
                                        myText->Print(fileName, "pdf");
                                        myText->Clear();

                                        myCanvas->cd(1);
                                        FirstPrint.at(j) = false;
                                        break;
                                    }
                                }

                                myCanvas->cd();

                                myCanvas->cd()->SetGrid();
                                myCanvas->cd()->SetBottomMargin(0.14), myCanvas->cd()->SetLeftMargin(0.16), myCanvas->cd()->SetRightMargin(0.12);

                                HistoList[i]->GetYaxis()->SetTitleOffset(1.5);
                                HistoList[i]->GetXaxis()->SetTitleOffset(1.1);

                                gPad->SetRightMargin(0.23);

                                // // Set the PDF title and header for the bookmark
                                // std::string Title = HistoList[i]->GetTitle();
                                // gStyle->SetTitlePS(Title.c_str());  // This sets the title in metadata
                                //                                     // gStyle->SetHeaderPS(("[ /Title " + Title + " /DOCVIEW pdfmark").c_str());  // Adds a PDF title
                                // gStyle->SetHeaderPS(("[ /Page " + to_string(i + 1) + " /View [/Fit] /Title (myTitle) ] /OUT pdfmark").c_str());

                                if (HistoList[i]->InheritsFrom("TH1D")) {
                                    HistoList[i]->Draw();
                                } else if (HistoList[i]->InheritsFrom("TH2D")) {
                                    // if (FindSubstring(HistoList[i]->GetTitle(), "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} in 1e cut")) {
                                    //     gPad->SetRightMargin(0.225);
                                    // } else {
                                    //     gPad->SetRightMargin(0.05);
                                    // }

                                    HistoList[i]->Draw("colz");

                                    if (HistoList[i]->GetEntries() != 0) {
                                        gPad->Update();
                                        TPaletteAxis* palette = (TPaletteAxis*)HistoList[i]->GetListOfFunctions()->FindObject("palette");
                                        palette->SetY2NDC(0.475);
                                        gPad->Modified();
                                        gPad->Update();
                                    }
                                }

                                if (true) {
                                    // if (bt::FindSubstring(TempHistoList[i]->GetTitle(), "V_{z}^{") && !bt::FindSubstring(TempHistoList[i]->GetTitle(), "sector")) {
                                    std::string Individual_PDF_fileName = IndividualPlotsOutputDir + to_string(plot_counter) + "_" + TempHistoList[i]->GetName() + ".pdf";
                                    myCanvas->SaveAs(Individual_PDF_fileName.c_str());
                                    hf::FixPDFOrientation(Individual_PDF_fileName);
                                }

                                myCanvas->Print(fileName, "pdf");
                                myCanvas->Clear();
                                ++plot_counter;
                            }

                            sprintf(fileName, "%s]", PDF_fileName.c_str());
                            myCanvas->Print(fileName, "pdf");

                            hf::FixPDFOrientation(PDF_fileName);                                                    // Fix orientation
                            hf::ReassignPDFBookmarks(bt::GetCurrentDirectory() + "/", PDF_fileName, PDF_fileName);  // Reassign clean bookmarks
                        };

                        GeneratePDFOutput("/lustre24/expphy/volatile/clas12/asportes/Analysis_output/", nFD_eff_test_PDF_fileName, BaseDir, InputFiles, HistoList);
 */
#pragma endregion

#pragma region /* Print eff plots */
                        vector<vector<TH1*>> HistoList_eff_plots;

                        TH1D* h_eff_P_nFD_1e_cut_demominator = (TH1D*)h_truth_P_nFD_ECALveto_1e_cut->Clone((std::string(h_truth_P_nFD_ECALveto_1e_cut->GetName()) + "_demominator").c_str());
                        // TH1D* h_eff_P_nFD_1e_cut_demominator = (TH1D*)h_truth_P_n_1e_cut->Clone((std::string(h_truth_P_n_1e_cut->GetName()) + "_demominator").c_str());

                        TH1D* h_eff_P_nFD_ECALveto_1e_cut_numerator =
                            (TH1D*)h_reco_P_nFD_ECALveto_1e_cut->Clone((std::string(h_reco_P_nFD_ECALveto_1e_cut->GetName()) + "_numerator").c_str());
                        TH1D* h_eff_P_nFD_ECALveto_1e_cut = (TH1D*)h_eff_P_nFD_ECALveto_1e_cut_numerator->Clone("eff_P_nFD_ECALveto_1e_cut");
                        h_eff_P_nFD_ECALveto_1e_cut->Divide(h_eff_P_nFD_1e_cut_demominator);
                        h_eff_P_nFD_ECALveto_1e_cut->SetTitle(h_eff_P_nFD_ECALveto_1e_cut->GetName());
                        HistoList_eff_plots.push_back({h_eff_P_nFD_ECALveto_1e_cut_numerator, h_eff_P_nFD_1e_cut_demominator, h_eff_P_nFD_ECALveto_1e_cut});

                        TH1D* h_eff_P_nFD_matched_1e_cut_numerator = (TH1D*)h_reco_P_nFD_matched_1e_cut->Clone((std::string(h_reco_P_nFD_matched_1e_cut->GetName()) + "_numerator").c_str());
                        TH1D* h_eff_P_nFD_matched_1e_cut = (TH1D*)h_eff_P_nFD_matched_1e_cut_numerator->Clone("eff_P_nFD_matched_1e_cut");
                        h_eff_P_nFD_matched_1e_cut->Divide(h_eff_P_nFD_1e_cut_demominator);
                        h_eff_P_nFD_matched_1e_cut->SetTitle(h_eff_P_nFD_matched_1e_cut->GetName());
                        HistoList_eff_plots.push_back({h_eff_P_nFD_matched_1e_cut_numerator, h_eff_P_nFD_1e_cut_demominator, h_eff_P_nFD_matched_1e_cut});

                        // TCanvas* myCanvas_eff_plots = new TCanvas("myPage_eff_plots", "myPage_eff_plots", pixelx, pixely);
                        TCanvas* myCanvas_eff_plots = new TCanvas("myPage_eff_plots", "myPage_eff_plots", pixelx * 3, pixely);
                        // TCanvas* myCanvas_eff_plots = new TCanvas("myPage_eff_plots", "myPage_eff_plots", pixelx * 3 * 2, pixely * 2);

                        std::string eff_plots_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/eff_plots.pdf";
                        char fileName_eff_plots[eff_plots_PDF_fileName.length()];
                        sprintf(fileName_eff_plots, "%s[", eff_plots_PDF_fileName.c_str());
                        myText->SaveAs(fileName_eff_plots);
                        sprintf(fileName_eff_plots, "%s", eff_plots_PDF_fileName.c_str());

                        myCanvas_eff_plots->cd();
                        gPad->Update();

                        titles.DrawLatex(0.05, 0.90, "Uniform sample of (e,e'n) events (truth-level)");

                        text.DrawLatex(0.05, 0.80, "OutFolderName:");
                        text.DrawLatex(0.10, 0.75, (OutFolderName).c_str());

                        text.DrawLatex(0.05, 0.70, ("Beam energy: " + Ebeam_status0).c_str());
                        text.DrawLatex(0.05, 0.65, ("ConstrainedE: " + bt::BoolToString(ConstrainedE)).c_str());

                        text.DrawLatex(0.05, 0.60, ("apply_ECAL_veto = " + BoolToString(apply_ECAL_veto)).c_str());
                        text.DrawLatex(0.05, 0.55, ("cPart_veto_radius = " + ToStringWithPrecision(cPart_veto_radius, 0)).c_str());

                        // text.DrawLatex(0.05, 0.45, ("apply_PCAL_neutral_veto = " + BoolToString(apply_PCAL_neutral_veto)).c_str());
                        // text.DrawLatex(0.05, 0.4, ("nPart_veto_radius = " + ToStringWithPrecision(nPart_veto_radius, 0)).c_str());

                        myCanvas_eff_plots->Print(fileName_eff_plots, "pdf");
                        myCanvas_eff_plots->Clear();
                        // myText->Print(fileName_eff_plots, "pdf");
                        // myText->Clear();

                        for (int i = 0; i < HistoList_eff_plots.size(); i++) {
                            // for (int j = 0; j < HistSubjects.size(); j++) {
                            //     if (FirstPrint.at(j) && FindSubstring(HistoList_eff_plots[i]->GetTitle(), HistSubjects.at(j))) {
                            //         myText->cd();
                            //         titles.DrawLatex(0.3, 0.5, HistSubjects2.at(j).c_str());
                            //         myText->Print(fileName_eff_plots, "pdf");
                            //         myText->Clear();

                            //         myCanvas_eff_plots->cd(1);
                            //         FirstPrint.at(j) = false;
                            //         break;
                            //     }
                            // }

                            myCanvas_eff_plots->Divide(3, 1);

                            for (int j = 0; j < HistoList_eff_plots[i].size(); j++) {
                                myCanvas_eff_plots->cd(j + 1);

                                myCanvas_eff_plots->cd(j + 1)->SetGrid();
                                myCanvas_eff_plots->cd(j + 1)->SetBottomMargin(0.14), myCanvas_eff_plots->cd(j + 1)->SetLeftMargin(0.16), myCanvas_eff_plots->cd(j + 1)->SetRightMargin(0.12);

                                HistoList_eff_plots[i][j]->GetYaxis()->SetTitleOffset(1.5);
                                HistoList_eff_plots[i][j]->GetXaxis()->SetTitleOffset(1.1);

                                gPad->SetRightMargin(0.23);

                                if (HistoList_eff_plots[i][j]->InheritsFrom("TH1D")) {
                                    HistoList_eff_plots[i][j]->Draw();
                                } else if (HistoList_eff_plots[i][j]->InheritsFrom("TH2D")) {
                                    HistoList_eff_plots[i][j]->Draw("colz");

                                    if (HistoList_eff_plots[i][j]->GetEntries() != 0) {
                                        gPad->Update();
                                        TPaletteAxis* palette = (TPaletteAxis*)HistoList_eff_plots[i][j]->GetListOfFunctions()->FindObject("palette");
                                        palette->SetY2NDC(0.475);
                                        gPad->Modified();
                                        gPad->Update();
                                    }
                                }
                            }

                            myCanvas_eff_plots->Print(fileName_eff_plots, "pdf");
                            myCanvas_eff_plots->Clear();
                        }

                        sprintf(fileName_eff_plots, "%s]", eff_plots_PDF_fileName.c_str());
                        myCanvas_eff_plots->Print(fileName_eff_plots, "pdf");

#pragma endregion

                        outFile->cd();
                        for (int i = 0; i < HistoList.size(); i++) { HistoList[i]->Write(); }
                        outFile->Close();
                    }

#pragma endregion

#pragma endregion

                    // Delete all ROOT objects whose class names start with TH (to prevent a memory leak)
                    if (Ebeam_v.size() > 1) { gDirectory->Clear(); }
                    // if (cPart_veto_radii.size() > 1) { gDirectory->Clear(); }
                }
            }
        }
    }
}
