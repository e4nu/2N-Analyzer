// source nFD_eff_tester_run.sh
// git pull && clas12root -b -q testers/nFD_eff_test.cpp

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include "nFD_eff_test_functions.cpp"

void nFD_eff_test() {
    cout << "\n\nInitiating nFD_eff_test.cpp\n";

    bool Is2GeV = false, Is4GeV = false, Is6GeV = false;

    // double Ebeam = 2.07052;
    // Is2GeV = true;
    // double Ebeam = 4.02962;
    // Is4GeV = true;
    double Ebeam = 5.98636;
    Is6GeV = true;

    //

    bool ConstrainTLmom = false;

    //

    bool use_ConstPn_samples = true;

    bool apply_neutFD_redef = true;
    bool apply_ECAL_veto = true;
    bool apply_PCAL_neutral_veto = true;

    double ECALvetoCut = 100.;
    double rc_factor = 1.;
    double rn_factor = 1.;

    bool ConstrainedE = true;

    //

    double P_upperLim;

    if (ConstrainTLmom) {
        P_upperLim = Ebeam * 0.5;
    } else {
        P_upperLim = Ebeam * 1.1;
    }

    // int Limiter = 25000000;  // 2500 files
    // int Limiter = 10000000; // 1000 files
    // int Limiter = 1000000;  // 100 files
    int Limiter = 100000;  // 10 files
    // int Limiter = 10000; // 1 file

    string OutFolderName_prefix = "nFD_eff_test";
    string OutFolderName_ver_status = "_v6";
    string samples_status = use_ConstPn_samples ? "_ConstPn" : "";
    string neutFD_redef_status = apply_neutFD_redef ? "_ReDefed" : "_clas12neut";
    string ECAL_veto_status = apply_ECAL_veto ? "_wECALveto" : "_NoECALveto";
    string PCAL_neutral_veto_status = apply_PCAL_neutral_veto ? "_wPCALneutVeto" : "_NoPCALneutVeto";
    string rc_factor_status = apply_ECAL_veto ? "_rc" + ToStringWithPrecision(rc_factor * ECALvetoCut, 0) : "";
    string rn_factor_status = apply_PCAL_neutral_veto ? "_rn" + ToStringWithPrecision(rn_factor * ECALvetoCut, 0) : "";
    string ConstrainedE_status = ConstrainedE ? "_ConstrainedE" : "";

    // string OutFolderName = "nFD_eff_test_v4_wPCALnVeto_rc100_rn200";
    string OutFolderName = OutFolderName_prefix + OutFolderName_ver_status + samples_status + neutFD_redef_status + ECAL_veto_status + PCAL_neutral_veto_status + rc_factor_status +
                           rn_factor_status + ConstrainedE_status;
    // string OutFolderName = "nFD_eff_test_v5_AMaps_ConstPn_wPCALnVeto_rn100_6GeV_3";

    const string OutputDir = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName;
    system(("rm -rf " + OutputDir).c_str());
    system(("mkdir -p " + OutputDir).c_str());

    TFile* outFile = new TFile(("/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/nFD_eff_test.root").c_str(), "RECREATE");

    clas12root::HipoChain chain;
    string InputFiles, SampleName;

    if (Is2GeV) {
        if (use_ConstPn_samples) {
            InputFiles = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV_ConstPn/OutPut_en/reconhipo/*.hipo";
        } else {
            InputFiles = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV/OutPut_en/reconhipo/*.hipo";
        }

        SampleName = "Uniform_en_sample_2070MeV";
    } else if (Is4GeV) {
        if (use_ConstPn_samples) {
            InputFiles = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV_ConstPn/OutPut_en/reconhipo/*.hipo";
        } else {
            InputFiles = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_en/reconhipo/*.hipo";
        }

        SampleName = "Uniform_en_sample_4029MeV";
    } else if (Is6GeV) {
        if (use_ConstPn_samples) {
            InputFiles = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/5986MeV_ConstPn/OutPut_en/reconhipo/*.hipo";
        } else {
            InputFiles = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/5986MeV/OutPut_en/reconhipo/*.hipo";
        }

        SampleName = "Uniform_en_sample_5986MeV";
    }

    chain.Add(InputFiles);
    chain.SetReaderTags({0});
    chain.db()->turnOffQADB();
    auto config_c12 = chain.GetC12Reader();
    const std::unique_ptr<clas12::clas12reader>& c12 = chain.C12ref();

#pragma region /* Prepare histograms */
    /////////////////////////////////////
    // Prepare histograms
    /////////////////////////////////////
    vector<TH1*> HistoList_electron_cuts;

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
        h_dc_electron_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_BC_%d", i), Form("DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_electron_cuts.push_back(h_dc_electron_hit_map_BC_1e_cut[i]);
        h_dc_electron_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_AC_%d", i), Form("DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_electron_cuts.push_back(h_dc_electron_hit_map_AC_1e_cut[i]);
    }

    // TH1D* h_Vz_e_AallC_1e_cut = new TH1D("Vz_e_AallC_1e_cut", "V_{z}^{e} in 1e cut (after cut);V_{z}^{e} [cm];Counts", 50, -8, 8);
    // HistoList.push_back(h_Vz_e_AallC_1e_cut);

    // TH2D* h_dc_electron_hit_map_AallC_1e_cut[4];  // 3 regions

    // // DC hit maps
    // for (int i = 1; i <= 3; i++) {
    //     h_dc_electron_hit_map_AallC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_AallC%d", i), Form("DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300,
    //     300);
    // }

#pragma endregion

#pragma region /* Electron PID */
    TH1D* h_nphe_BC_1e_cut = new TH1D("nphe_BC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList_electron_cuts.push_back(h_nphe_BC_1e_cut);
    TH1D* h_nphe_AC_1e_cut = new TH1D("nphe_AC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList_electron_cuts.push_back(h_nphe_AC_1e_cut);

    TH2D* h_Edep_PCAL_VS_EC_BC_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
                 0.2, 100, 0, 0.3);
    HistoList_electron_cuts.push_back(h_Edep_PCAL_VS_EC_BC_1e_cut);
    TH2D* h_Edep_PCAL_VS_EC_AC_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2,
                 100, 0, 0.3);
    HistoList_electron_cuts.push_back(h_Edep_PCAL_VS_EC_AC_1e_cut);

    TH2D* h_SF_VS_P_e_BC_1e_cut =
        new TH2D("SF_VS_P_e_BC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_P_e_BC_1e_cut);
    TH2D* h_SF_VS_P_e_AC_1e_cut =
        new TH2D("SF_VS_P_e_AC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_P_e_AC_1e_cut);

    TH2D* h_SF_VS_Lv_BC_1e_cut = new TH2D("SF_VS_Lv_BC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lv_BC_1e_cut);
    TH2D* h_SF_VS_Lv_AC_1e_cut = new TH2D("SF_VS_Lv_AC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lv_AC_1e_cut);

    TH2D* h_SF_VS_Lw_BC_1e_cut = new TH2D("SF_VS_Lw_BC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lw_BC_1e_cut);
    TH2D* h_SF_VS_Lw_AC_1e_cut = new TH2D("SF_VS_Lw_AC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lw_AC_1e_cut);

    TH2D* h_SF_VS_Lu_BC_1e_cut = new TH2D("SF_VS_Lu_BC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lu_BC_1e_cut);
    TH2D* h_SF_VS_Lu_AC_1e_cut = new TH2D("SF_VS_Lu_AC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lu_AC_1e_cut);

    TH2D* h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
    HistoList_electron_cuts.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut);
    TH2D* h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
    HistoList_electron_cuts.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut);

#pragma endregion

    TH1D* h_reco_P_e_1e_cut = new TH1D("reco_P_e_1e_cut", "P^{reco}_{e} in 1e cut;P^{reco}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_e_1e_cut);
    TH1D* h_truth_P_e_1e_cut = new TH1D("truth_P_e_1e_cut", "P^{truth}_{e} in 1e cut;P^{truth}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_e_1e_cut);
    TH1D* h_reco_theta_e_1e_cut = new TH1D("reco_theta_e_1e_cut", "#theta^{reco}_{e} in 1e cut;#theta^{reco}_{e} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_e_1e_cut);
    TH1D* h_truth_theta_e_1e_cut = new TH1D("truth_theta_e_1e_cut", "#theta^{truth}_{e} in 1e cut;#theta^{truth}_{e} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_e_1e_cut);
    TH1D* h_reco_phi_e_1e_cut = new TH1D("reco_phi_e_1e_cut", "#phi^{reco}_{e} in 1e cut;#phi^{reco}_{e} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_e_1e_cut);
    TH1D* h_truth_phi_e_1e_cut = new TH1D("truth_phi_e_1e_cut", "#phi^{truth}_{e} in 1e cut;#phi^{truth}_{e} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_e_1e_cut);
    TH2D* h_reco_theta_e_VS_reco_phi_e_1e_cut =
        new TH2D("reco_theta_e_VS_reco_phi_e_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{e} in 1e cut;#phi^{reco}_{e} [#circ];#theta^{reco}_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_e_VS_reco_phi_e_1e_cut);
    TH2D* h_truth_theta_e_VS_truth_phi_e_1e_cut = new TH2D(
        "truth_theta_e_VS_truth_phi_e_1e_cut", "#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut;#phi^{truth}_{e} [#circ];#theta^{truth}_{e} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_e_VS_truth_phi_e_1e_cut);

    TH1D* h_reco_P_n_1e_cut = new TH1D("reco_P_n_1e_cut", "P^{reco}_{n} in 1e cut;P^{reco}_{n} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_n_1e_cut);
    TH1D* h_truth_P_n_1e_cut = new TH1D("truth_P_n_1e_cut", "P^{truth}_{n} in 1e cut;P^{truth}_{n} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_n_1e_cut);
    TH1D* h_reco_theta_n_1e_cut = new TH1D("reco_theta_n_1e_cut", "#theta^{reco}_{n} in 1e cut;#theta^{reco}_{n} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_n_1e_cut);
    TH1D* h_truth_theta_n_1e_cut = new TH1D("truth_theta_n_1e_cut", "#theta^{truth}_{n} in 1e cut;#theta^{truth}_{n} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_n_1e_cut);
    TH1D* h_reco_phi_n_1e_cut = new TH1D("reco_phi_n_1e_cut", "#phi^{reco}_{n} in 1e cut;#phi^{reco}_{n} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_n_1e_cut);
    TH1D* h_truth_phi_n_1e_cut = new TH1D("truth_phi_n_1e_cut", "#phi^{truth}_{n} in 1e cut;#phi^{truth}_{n} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_n_1e_cut);
    TH2D* h_reco_theta_n_VS_reco_phi_n_1e_cut =
        new TH2D("reco_theta_n_VS_reco_phi_n_1e_cut", "#theta^{reco}_{n} vs. #phi^{reco}_{n} in 1e cut;#phi^{reco}_{n} [#circ];#theta^{reco}_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_n_VS_reco_phi_n_1e_cut);
    TH2D* h_truth_theta_n_VS_truth_phi_n_1e_cut = new TH2D(
        "truth_theta_n_VS_truth_phi_n_1e_cut", "#theta^{truth}_{n} vs. #phi^{truth}_{n} in 1e cut;#phi^{truth}_{n} [#circ];#theta^{truth}_{n} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_n_VS_truth_phi_n_1e_cut);
#pragma endregion

#pragma region /* clas12reco */
    HistSubjects.push_back("clas12reco");
    HistSubjects2.push_back("#splitline{FD neutron from}{clas12reco}");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_clas12_1e_cut = new TH1D("reco_P_nFD_clas12_1e_cut", "P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_clas12_1e_cut);
    TH1D* h_truth_P_nFD_clas12_1e_cut = new TH1D("truth_P_nFD_clas12_1e_cut", "P^{truth}_{nFD} in 1e cut (clas12reco);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_nFD_clas12_1e_cut);
    TH1D* h_reco_theta_nFD_clas12_1e_cut = new TH1D("reco_theta_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_clas12_1e_cut);
    TH1D* h_truth_theta_nFD_clas12_1e_cut = new TH1D("truth_theta_nFD_clas12_1e_cut", "#theta^{truth}_{nFD} in 1e cut (clas12reco);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_clas12_1e_cut);
    TH1D* h_reco_phi_nFD_clas12_1e_cut = new TH1D("reco_phi_nFD_clas12_1e_cut", "#phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_clas12_1e_cut);
    TH1D* h_truth_phi_nFD_clas12_1e_cut = new TH1D("truth_phi_nFD_clas12_1e_cut", "#phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_clas12_1e_cut);
    TH2D* h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut =
        new TH2D("reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut",
                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (clas12reco);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_clas12_VS_reco_phi_nFD_clas12_1e_cut);
    TH2D* h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut =
        new TH2D("truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (clas12reco);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_clas12_VS_truth_phi_nFD_clas12_1e_cut);
    TH2D* h_reco_P_e_VS_P_nFD_clas12_1e_cut = new TH2D(
        "reco_P_e_VS_P_nFD_clas12_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (clas12reco);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_clas12_1e_cut);
    TH2D* h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut =
        new TH2D("reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (clas12reco);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0.,
                 50., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_clas12_VS_P_nFD_clas12_1e_cut);
#pragma endregion

#pragma region /* redef */
    HistSubjects.push_back("redef");
    HistSubjects2.push_back("redef");
    FirstPrint.push_back(true);
    TH1D* h_reco_P_nFD_redef_1e_cut = new TH1D("reco_P_nFD_redef_1e_cut", "P^{reco}_{nFD} in 1e cut (redef);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_redef_1e_cut);
    TH1D* h_truth_P_nFD_redef_1e_cut = new TH1D("truth_P_nFD_redef_1e_cut", "P^{truth}_{nFD} in 1e cut (redef);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_truth_P_nFD_redef_1e_cut);
    TH1D* h_reco_theta_nFD_redef_1e_cut = new TH1D("reco_theta_nFD_redef_1e_cut", "#theta^{reco}_{nFD} in 1e cut (redef);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_redef_1e_cut);
    TH1D* h_truth_theta_nFD_redef_1e_cut = new TH1D("truth_theta_nFD_redef_1e_cut", "#theta^{truth}_{nFD} in 1e cut (redef);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_redef_1e_cut);
    TH1D* h_reco_phi_nFD_redef_1e_cut = new TH1D("reco_phi_nFD_redef_1e_cut", "#phi^{reco}_{nFD} in 1e cut (redef);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_redef_1e_cut);
    TH1D* h_truth_phi_nFD_redef_1e_cut = new TH1D("truth_phi_nFD_redef_1e_cut", "#phi^{truth}_{nFD} in 1e cut (redef);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_redef_1e_cut);
    TH2D* h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut =
        new TH2D("reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut", "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (redef);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]",
                 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_redef_VS_reco_phi_nFD_redef_1e_cut);
    TH2D* h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut =
        new TH2D("truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (redef);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_redef_VS_truth_phi_nFD_redef_1e_cut);
    TH2D* h_reco_P_e_VS_P_nFD_redef_1e_cut =
        new TH2D("reco_P_e_VS_P_nFD_redef_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (redef);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_e_VS_P_nFD_redef_1e_cut);
    TH2D* h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut =
        new TH2D("reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (redef);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0., 50.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_redef_VS_P_nFD_redef_1e_cut);
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
        new TH2D("reco_phi_e_VS_phi_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, 0, 45.,
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
    TH1D* h_reco_nFD_multi_ECALveto_1e_cut = new TH1D("reco_nFD_multi_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (ECALveto);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_ECALveto_1e_cut);
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut =
        new TH2D("reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut", "reco nFD multiplicity vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam * 3.,
                 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut);
#pragma endregion

    TH2D* h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (ECALveto);L^{reco}_{nFD,ECAL} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (ECALveto);L^{reco}_{nFD,ECAL} [cm];#theta^{reco}_{nFD} [#circ]",
                 100, 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (ECALveto);L^{reco}_{nFD,ECAL} [cm];#phi^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut", "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECAL} [ns];P^{reco}_{nFD} [#circ]",
                 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECAL} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECAL} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECAL_ECALveto_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECAL_ECALveto_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (ECALveto);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
    //              100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECAL_ECALveto_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECAL_ECALveto_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECAL_ECALveto_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (ECALveto);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECAL_ECALveto_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECAL_ECALveto_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECAL_ECALveto_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (ECALveto);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECAL_ECALveto_1e_cut);

    TH2D* h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (ECALveto);L^{reco}_{nFD,ECIN} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (ECALveto);L^{reco}_{nFD,ECIN} [cm];#theta^{reco}_{nFD} [#circ]",
                 100, 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (ECALveto);L^{reco}_{nFD,ECIN} [cm];#phi^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut", "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECIN} [ns];P^{reco}_{nFD} [#circ]",
                 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECIN} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECIN} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECIN_ECALveto_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECIN_ECALveto_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (ECALveto);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
    //              100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECIN_ECALveto_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECIN_ECALveto_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECIN_ECALveto_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (ECALveto);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECIN_ECALveto_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECIN_ECALveto_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECIN_ECALveto_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (ECALveto);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECIN_ECALveto_1e_cut);

    TH2D* h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (ECALveto);L^{reco}_{nFD,ECOUT} [cm];P^{reco}_{nFD} [#circ]", 100, 700.,
                 850., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (ECALveto);L^{reco}_{nFD,ECOUT} [cm];#theta^{reco}_{nFD} [#circ]",
                 100, 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (ECALveto);L^{reco}_{nFD,ECOUT} [cm];#phi^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut",
                 "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECOUT} [ns];P^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECOUT} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (ECALveto);t^{reco}_{ToF,nFD,ECOUT} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (ECALveto);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (ECALveto);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (ECALveto);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECOUT_ECALveto_1e_cut);

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
                 "P^{reco}_{nFD} vs. #Delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
                 "P^{reco}_{nFD} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];P^{reco}_{nFD} [GeV/c]", 100,
                 -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);

    TH1D* h_v_dist_ECALveto_1e_cut = new TH1D("v_dist_ECALveto_1e_cut", "v_dist in 1e cut (ECALveto);v_dist [cm];Counts", 50, 0., 1000.);
    HistoList.push_back(h_v_dist_ECALveto_1e_cut);
    TH2D* h_v_dist_VS_reco_P_nFD_ECALveto_1e_cut =
        new TH2D("v_dist_VS_reco_P_nFD_ECALveto_1e_cut", "v_dist vs. P^{reco}_{nFD} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{nFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_v_dist_VS_reco_P_nFD_ECALveto_1e_cut);
    TH2D* h_v_dist_VS_reco_P_e_ECALveto_1e_cut =
        new TH2D("v_dist_VS_reco_P_e_ECALveto_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
    HistoList.push_back(h_v_dist_VS_reco_P_e_ECALveto_1e_cut);
    TH2D* h_v_dist_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut = new TH2D(
        "v_dist_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut",
        "v_dist vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);v_dist [cm];#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0., 1000., 100, -180., 180.);
    HistoList.push_back(h_v_dist_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut);

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
    TH1D* h_reco_theta_nFD_matched_1e_cut = new TH1D("reco_theta_nFD_matched_1e_cut", "#theta^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_reco_theta_nFD_matched_1e_cut);
    TH1D* h_truth_theta_nFD_matched_1e_cut = new TH1D("truth_theta_nFD_matched_1e_cut", "#theta^{truth}_{nFD} in 1e cut (matched);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    HistoList.push_back(h_truth_theta_nFD_matched_1e_cut);
    TH1D* h_reco_phi_nFD_matched_1e_cut = new TH1D("reco_phi_nFD_matched_1e_cut", "#phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_reco_phi_nFD_matched_1e_cut);
    TH1D* h_truth_phi_nFD_matched_1e_cut = new TH1D("truth_phi_nFD_matched_1e_cut", "#phi^{truth}_{nFD} in 1e cut (matched);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    HistoList.push_back(h_truth_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut =
        new TH2D("reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut",
                 "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut);
    TH2D* h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut =
        new TH2D("truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut",
                 "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (matched);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    HistoList.push_back(h_truth_theta_nFD_matched_VS_truth_phi_nFD_matched_1e_cut);

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
        new TH2D("reco_phi_e_VS_phi_nFD_matched_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, 0, 45., 100,
                 -180., 180.);
    HistoList.push_back(h_reco_theta_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_phi_e_VS_phi_nFD_matched_1e_cut =
        new TH2D("reco_phi_e_VS_phi_nFD_matched_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (matched);#phi^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, -180., 180., 100,
                 -180., 180.);
    HistoList.push_back(h_reco_phi_e_VS_phi_nFD_matched_1e_cut);
    TH2D* h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut =
        new TH2D("reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (matched);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0.,
                 45., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_theta_nFD_matched_VS_P_nFD_matched_1e_cut);
    TH1D* h_reco_nFD_multi_matched_1e_cut = new TH1D("reco_nFD_multi_matched_1e_cut", "reco nFD multiplicity in 1e cut (matched);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_matched_1e_cut);
    TH2D* h_reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut = new TH2D(
        "reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut", "reco nFD multiplicity vs. P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam * 3., 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_matched_1e_cut);
#pragma endregion

    TH2D* h_reco_P_nFD_VS_reco_L_ECAL_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECAL_matched_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (matched);L^{reco}_{nFD,ECAL} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECAL_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECAL_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECAL_matched_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (matched);L^{reco}_{nFD,ECAL} [cm];#theta^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECAL_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECAL_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECAL_matched_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECAL} in 1e cut (matched);L^{reco}_{nFD,ECAL} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
                 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECAL_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut", "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (matched);t^{reco}_{ToF,nFD,ECAL} [ns];P^{reco}_{nFD} [#circ]",
                 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (matched);t^{reco}_{ToF,nFD,ECAL} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECAL} in 1e cut (matched);t^{reco}_{ToF,nFD,ECAL} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECAL_matched_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECAL_matched_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (matched);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
    //              100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECAL_matched_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECAL_matched_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECAL_matched_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (matched);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECAL_matched_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECAL_matched_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECAL_matched_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECAL} in 1e cut (matched);L^{eff}_{nFD,ECAL} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECAL_matched_1e_cut);

    TH2D* h_reco_P_nFD_VS_reco_L_ECIN_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECIN_matched_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (matched);L^{reco}_{nFD,ECIN} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECIN_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECIN_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECIN_matched_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (matched);L^{reco}_{nFD,ECIN} [cm];#theta^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECIN_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECIN_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECIN_matched_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECIN} in 1e cut (matched);L^{reco}_{nFD,ECIN} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
                 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECIN_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut", "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (matched);t^{reco}_{ToF,nFD,ECIN} [ns];P^{reco}_{nFD} [#circ]",
                 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (matched);t^{reco}_{ToF,nFD,ECIN} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECIN} in 1e cut (matched);t^{reco}_{ToF,nFD,ECIN} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECIN_matched_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECIN_matched_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (matched);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
    //              100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECIN_matched_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECIN_matched_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECIN_matched_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (matched);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECIN_matched_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECIN_matched_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECIN_matched_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECIN} in 1e cut (matched);L^{eff}_{nFD,ECIN} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECIN_matched_1e_cut);

    TH2D* h_reco_P_nFD_VS_reco_L_ECOUT_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_L_ECOUT_matched_1e_cut", "P^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (matched);L^{reco}_{nFD,ECOUT} [cm];P^{reco}_{nFD} [#circ]", 100, 700., 850.,
                 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_L_ECOUT_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_reco_L_ECOUT_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_reco_L_ECOUT_matched_1e_cut", "#theta^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (matched);L^{reco}_{nFD,ECOUT} [cm];#theta^{reco}_{nFD} [#circ]",
                 100, 700., 850., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_reco_L_ECOUT_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_reco_L_ECOUT_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_reco_L_ECOUT_matched_1e_cut", "#phi^{reco}_{nFD} vs. L^{reco}_{nFD,ECOUT} in 1e cut (matched);L^{reco}_{nFD,ECOUT} [cm];#phi^{reco}_{nFD} [#circ]", 100,
                 700., 850., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_reco_L_ECOUT_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut",
                 "P^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (matched);t^{reco}_{ToF,nFD,ECOUT} [ns];P^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut);
    TH2D* h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut =
        new TH2D("reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut",
                 "#theta^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (matched);t^{reco}_{ToF,nFD,ECOUT} [ns];#theta^{reco}_{nFD} [#circ]", 100, 15., 75., 100, 0., 45.);
    HistoList.push_back(h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut);
    TH2D* h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut =
        new TH2D("reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut",
                 "#phi^{reco}_{nFD} vs. t^{reco}_{ToF,nFD,ECOUT} in 1e cut (matched);t^{reco}_{ToF,nFD,ECOUT} [ns];#phi^{reco}_{nFD} [#circ]", 100, 15., 75., 100, -180., 180.);
    HistoList.push_back(h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut);
    // TH2D* h_reco_P_nFD_VS_eff_L_ECOUT_matched_1e_cut =
    //     new TH2D("reco_P_nFD_VS_eff_L_ECOUT_matched_1e_cut",
    //              "P^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (matched);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];P^{reco}_{nFD} [#circ]", 100, 700.,
    //              850., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_eff_L_ECOUT_matched_1e_cut);
    // TH2D* h_reco_theta_nFD_VS_eff_L_ECOUT_matched_1e_cut =
    //     new TH2D("reco_theta_nFD_VS_eff_L_ECOUT_matched_1e_cut",
    //              "#theta^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (matched);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#theta^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, 0., 45.);
    // HistoList.push_back(h_reco_theta_nFD_VS_eff_L_ECOUT_matched_1e_cut);
    // TH2D* h_reco_phi_nFD_VS_eff_L_ECOUT_matched_1e_cut =
    //     new TH2D("reco_phi_nFD_VS_eff_L_ECOUT_matched_1e_cut",
    //              "#phi^{reco}_{nFD} vs. L^{eff}_{nFD,ECOUT} in 1e cut (matched);L^{eff}_{nFD,ECOUT} = c#beta^{truth}t^{#beta^{reco}_{nFD}}_{ToF} [cm];#phi^{reco}_{nFD} [#circ]", 100,
    //              700., 850., 100, -180., 180.);
    // HistoList.push_back(h_reco_phi_nFD_VS_eff_L_ECOUT_matched_1e_cut);

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
                 "P^{reco}_{nFD} vs. #Delta#theta^{reco}_{nFD,e} in 1e cut (matched);#Delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]",
                 100, -25., 10., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut);
    TH2D* h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut =
        new TH2D("reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
                 "P^{reco}_{nFD} vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (matched);#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];P^{reco}_{nFD} [GeV/c]", 100,
                 -180., 180., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);

    TH1D* h_v_dist_matched_1e_cut = new TH1D("v_dist_matched_1e_cut", "v_dist in 1e cut (matched);v_dist [cm];Counts", 50, 0., 1000.);
    HistoList.push_back(h_v_dist_matched_1e_cut);
    TH2D* h_v_dist_VS_reco_P_nFD_matched_1e_cut =
        new TH2D("v_dist_VS_reco_P_nFD_matched_1e_cut", "v_dist vs. P^{reco}_{nFD} in 1e cut (matched);v_dist [cm];P^{reco}_{nFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
    HistoList.push_back(h_v_dist_VS_reco_P_nFD_matched_1e_cut);
    TH2D* h_v_dist_VS_reco_P_e_matched_1e_cut =
        new TH2D("v_dist_VS_reco_P_e_matched_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (matched);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
    HistoList.push_back(h_v_dist_VS_reco_P_e_matched_1e_cut);
    TH2D* h_v_dist_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut = new TH2D(
        "v_dist_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut",
        "v_dist vs. #Delta#phi^{reco}_{nFD,e} in 1e cut (matched);v_dist [cm];#Delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0., 1000., 100, -180., 180.);
    HistoList.push_back(h_v_dist_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut);

#pragma endregion

#pragma endregion

#pragma region /* Prepare AMaps */

    const string OutputDirAMapsPlots = OutputDir + "/01_AMaps_Plots/";
    system(("rm -rf " + OutputDirAMapsPlots).c_str());
    system(("mkdir -p " + OutputDirAMapsPlots).c_str());

    const string OutputDirAMapsMaps = OutputDir + "/02_AMaps_Maps/";
    system(("rm -rf " + OutputDirAMapsMaps).c_str());
    system(("mkdir -p " + OutputDirAMapsMaps).c_str());

    const string P_e_bin_profile = "uniform_P_e_bins";      // {reformat_e_bins , varying_P_e_bins , uniform_P_e_bins, equi_inverted_P_e}
    const string P_nuc_bin_profile = "uniform_P_nuc_bins";  // {equi_inverted_P_nuc , varying_P_nuc_bins , uniform_P_nuc_bins}

    double beamE = Ebeam;

    int NumberNucOfMomSlices = 15, NumberElecOfMomSlices = 15, HistElectronSliceNumOfXBins = 100, HistNucSliceNumOfXBins = 100;

    AMaps aMaps_master;

    aMaps_master = AMaps(SampleName, P_e_bin_profile, P_nuc_bin_profile, beamE, "AMaps", OutputDirAMapsPlots, NumberNucOfMomSlices, NumberElecOfMomSlices, HistNucSliceNumOfXBins,
                         HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins, HistElectronSliceNumOfXBins);

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
        if ((counter % 1000000) == 0) { cerr << "\n" << counter / 1000000 << " million completed"; }
        if ((counter % 100000) == 0) { cerr << "."; }

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

            auto Truth_theta_temp = mcpbank->getTheta();
            auto Truth_phi_temp = mcpbank->getPhi();

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

        //  Electron PID cuts -----------------------------------------------------------------------------------------------------------------------------------------------------

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

        if (ConstrainedE && (reco_P_e.Mag() < Ebeam - 0.2 || reco_P_e.Mag() > Ebeam + 0.2)) { continue; }
        if (ConstrainedE && (fabs((reco_P_e.Theta() * 180 / M_PI) - Truth_theta_e) > 2.)) { continue; }
        if (ConstrainedE && (fabs((reco_P_e.Phi() * 180 / M_PI) - Truth_phi_e) > 5.)) { continue; }
        // if (ConstrainedE && (fabs((reco_P_e.Theta() * 180 / M_PI) - 25.) > 2.)) { continue; }
        if (ConstrainedE && (fabs(getPhi_e(TString(InputFiles), Truth_phi_nFD) - reco_P_e.Phi() * 180 / M_PI) > 5.)) { continue; }

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

            // bool PassMomth = true;
            bool PassMomth = (p >= 0.4);

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

                    if (PassMomth) {
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
        //     cout << "Truth_theta_nFD = " << Truth_theta_nFD << "\nAborting...\n\n", exit(0);
        // }
        if (truth_NeutronsFD.size() != 1) { continue; }

        // Fill leading FD neutron acceptance maps
        if ((TL_IDed_Leading_nFD_ind != -1) && (TL_IDed_Leading_nFD_momentum > 0)) {
            mcpbank->setEntry(TL_IDed_Leading_nFD_ind);

            int particlePDGtmp = mcpbank->getPid();

            double Particle_TL_Momentum = RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
            double Particle_TL_Theta = acos((mcpbank->getPz()) / RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

            // bool PassMomth = true;
            bool PassMomth = (Particle_TL_Momentum >= 0.4);

            if (PassMomth) { aMaps_master.hFillHitMaps("TL", "Neutron", Particle_TL_Momentum, Particle_TL_Theta, Particle_TL_Phi, weight); }  // end of if id. TL leading neutron
        }
#pragma endregion

        //  Setting up neutrals ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up neutrals (RAW) */
        vector<region_part_ptr> neutrons;
        vector<region_part_ptr> photons;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();

            if (pid_temp == 2112) { neutrons.push_back(allParticles[i]); }
            if (pid_temp == 22) { photons.push_back(allParticles[i]); }
        }

        for (int i = 0; i < neutrons.size(); i++) {
            TVector3 reco_P_n;
            reco_P_n.SetMagThetaPhi(neutrons[i]->getP(), neutrons[i]->getTheta(), neutrons[i]->getPhi());

            h_reco_P_n_1e_cut->Fill(reco_P_n.Mag(), weight);
            h_reco_theta_n_1e_cut->Fill(reco_P_n.Theta() * 180 / M_PI, weight);
            h_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, weight);
            h_reco_theta_n_VS_reco_phi_n_1e_cut->Fill(reco_P_n.Phi() * 180 / M_PI, reco_P_n.Theta() * 180 / M_PI, weight);
        }
#pragma endregion

        //  Setting up FD neutrals (clas12reco) -----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (clas12reco) */
        vector<region_part_ptr> neutrons_FD_clas12;
        vector<region_part_ptr> photons_FD_clas12;

        for (int i = 0; i < allParticles.size(); i++) {
            int pid_temp = allParticles[i]->par()->getPid();

            if (pid_temp == 2112 && allParticles[i]->getRegion() == FD) { neutrons_FD_clas12.push_back(allParticles[i]); }
            if (pid_temp == 22 && allParticles[i]->getRegion() == FD) { photons_FD_clas12.push_back(allParticles[i]); }
        }

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

#pragma endregion

        //  Setting up FD neutrals (redef) ----------------------------------------------------------------------------------------------------------------------------------------

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

        for (int i = 0; i < neutrons_FD_redef.size(); i++) {
            bool ParticleInPCAL = (neutrons_FD_redef[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool ParticleInECIN = (neutrons_FD_redef[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool ParticleInECOUT = (neutrons_FD_redef[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
            if (ParticleInPCAL) { cout << "\n\nError! neutrons_FD_redef is in the PCAL! Aborting...\n\n", exit(0); }

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
#pragma endregion

        //  Setting up FD neutrals (ECALveto) ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Setting up FD neutrals (ECALveto) */
        double P_max = -1;
        int NeutronsFD_ind_mom_max = -1;

        vector<region_part_ptr> neutrons_FD_ECALveto;

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

                                // bool PassMomth = true;
                                bool PassMomth = (Momentum >= 0.4);
                                bool passECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                                bool passVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, ECALvetoCut, apply_PCAL_neutral_veto, rc_factor, rn_factor);

                                if (PassMomth && passECALeadgeCuts && (!apply_ECAL_veto || passVeto) && true) {
                                    if (Momentum >= P_max) {
                                        P_max = Momentum;
                                        NeutronsFD_ind_mom_max = i;
                                    }

                                    neutrons_FD_ECALveto.push_back(allParticles[i]);
                                }  // end of clas12root neutron or 'photon' if
                            }
                        }
                    }  // end of clas12root neutron or 'photon' if
                } else {
                    if (pid_temp == 2112) {
                        double Momentum = CalcPnFD(allParticles[i], electrons[0], starttime);

                        double Path_nFD = CalcPathnFD(allParticles[i], electrons[0]);
                        double reco_ToF_nFD = CalcToFnFD(allParticles[i], starttime);

                        // bool PassMomth = true;
                        bool PassMomth = (Momentum >= 0.4);
                        bool passECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);

                        if (PassMomth && passECALeadgeCuts) {
                            if (Momentum >= P_max) {
                                P_max = Momentum;
                                NeutronsFD_ind_mom_max = i;
                            }

                            neutrons_FD_ECALveto.push_back(allParticles[i]);
                        }  // end of clas12root neutron or 'photon' if
                    }
                }
            }  // end of neutral and in the FD if
        }

        if (NeutronsFD_ind_mom_max != -1) {
            bool NeutronPassVeto_1e_cut = NeutronECAL_Cut_Veto(allParticles, electrons, beamE, NeutronsFD_ind_mom_max, ECALvetoCut, apply_PCAL_neutral_veto, rc_factor, rn_factor);

            double Mom_neut_1e_cut = CalcPnFD(allParticles[NeutronsFD_ind_mom_max], electrons[0], starttime);
            double Theta_neut_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getTheta() * 180.0 / pi;
            double Phi_neut_1e_cut = allParticles[NeutronsFD_ind_mom_max]->getPhi() * 180.0 / pi;

            // bool PassMomth = true;
            bool PassMomth = (Mom_neut_1e_cut >= 0.4);

            if (PassMomth && NeutronPassVeto_1e_cut)  // FOR nFD eff test!
            {
                aMaps_master.hFillHitMaps("Reco", "Neutron", Mom_neut_1e_cut, Theta_neut_1e_cut, Phi_neut_1e_cut, weight);
            }  // end of if id. reco leading neutron
        }

        for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
            bool ParticleInPCAL = (neutrons_FD_ECALveto[i]->cal(clas12::PCAL)->getDetector() == 7);         // PCAL hit
            bool ParticleInECIN = (neutrons_FD_ECALveto[i]->cal(clas12::ECIN)->getDetector() == 7);         // ECIN hit
            bool ParticleInECOUT = (neutrons_FD_ECALveto[i]->cal(clas12::ECOUT)->getDetector() == 7);       // ECOUT hit
            auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit
            if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_ECALveto is in the PCAL! Aborting...\n\n", exit(0); }

            double Path_nFD = CalcPathnFD(neutrons_FD_ECALveto[i], electrons[0]);
            double reco_ToF_nFD = CalcToFnFD(neutrons_FD_ECALveto[i], starttime);

            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(CalcPnFD(neutrons_FD_ECALveto[i], electrons[0], starttime), neutrons_FD_ECALveto[i]->getTheta(), neutrons_FD_ECALveto[i]->getPhi());

            h_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
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

            if (ParticleInECIN || ParticleInECOUT) {
                h_reco_P_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_VS_reco_L_ECAL_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);

                if (ParticleInECIN) {
                    h_reco_P_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_reco_L_ECIN_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                }

                if (ParticleInECOUT) {
                    h_reco_P_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_reco_L_ECOUT_ECALveto_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_ECALveto_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                }
            }

            h_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
            h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI),
                                                                                                       reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
            h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(), weight);

            TVector3 v_nhit(neutrons_FD_ECALveto[i]->cal(detlayer)->getX(), neutrons_FD_ECALveto[i]->cal(detlayer)->getY(), neutrons_FD_ECALveto[i]->cal(detlayer)->getZ());

            for (int j = 0; j < allParticles.size(); j++) {
                if (allParticles[j]->par()->getCharge() == 0) {
                    bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                    TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                    if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                        v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
                        TVector3 v_dist = v_nhit - v_neutral_hit;

                        h_v_dist_ECALveto_1e_cut->Fill(v_dist.Mag(), weight);
                        h_v_dist_VS_reco_P_nFD_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                        h_v_dist_VS_reco_P_e_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                        h_v_dist_VS_reco_phi_nFD_minus_reco_phi_e_ECALveto_1e_cut->Fill(v_dist.Mag(), CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                    }
                }
            }
        }

        if (neutrons_FD_ECALveto.size() != 0) { h_reco_nFD_multi_ECALveto_1e_cut->Fill(neutrons_FD_ECALveto.size(), weight); }
#pragma endregion

        //  Setting up FD neutrals (matched) -----------------------------------------------------------------------------------------------------------------------------------------

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

                bool PassMomth = true;
                // bool PassMomth = (Momentum >= 0.4);
                // bool passECALeadgeCuts = true;
                bool passECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                // bool passVeto = true;
                bool passVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, 100,apply_PCAL_neutral_veto, rc_factor ,rn_factor);

                if (PassMomth && passECALeadgeCuts && passVeto) {
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

                                // bool PassMomth = true;
                                bool PassMomth = (Momentum >= 0.4);
                                bool passECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);
                                bool passVeto = NeutronECAL_Cut_Veto(allParticles, electrons, Ebeam, i, ECALvetoCut, apply_PCAL_neutral_veto, rc_factor, rn_factor);

                                if (PassMomth && passECALeadgeCuts && (!apply_ECAL_veto || passVeto)) {
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

                        // bool PassMomth = true;
                        bool PassMomth = (Momentum >= 0.4);
                        bool passECALeadgeCuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > 14. && allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > 14.);

                        if (PassMomth && passECALeadgeCuts) {
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

        for (int i = 0; i < neutrons_FD_matched.size(); i++) {
            bool ParticleInPCAL = (neutrons_FD_matched[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool ParticleInECIN = (neutrons_FD_matched[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool ParticleInECOUT = (neutrons_FD_matched[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit
            if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_matched is in the PCAL! Aborting...\n\n", exit(0); }
            auto detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // determine the earliest layer of the neutral hit

            double Path_nFD = CalcPathnFD(neutrons_FD_matched[i], electrons[0]);
            double reco_ToF_nFD = CalcToFnFD(neutrons_FD_matched[i], starttime);

            TVector3 reco_P_nFD;
            reco_P_nFD.SetMagThetaPhi(tl_P, neutrons_FD_matched[i]->getTheta(), neutrons_FD_matched[i]->getPhi());

            h_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight);
            h_reco_theta_nFD_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
            h_reco_theta_nFD_matched_VS_reco_phi_nFD_matched_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
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

            if (ParticleInECIN || ParticleInECOUT) {
                h_reco_P_nFD_VS_reco_L_ECAL_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_VS_reco_L_ECAL_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_VS_reco_L_ECAL_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                h_reco_P_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECAL_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);

                if (ParticleInECIN) {
                    h_reco_P_nFD_VS_reco_L_ECIN_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_reco_L_ECIN_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_reco_L_ECIN_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_P_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECIN_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                }

                if (ParticleInECOUT) {
                    h_reco_P_nFD_VS_reco_L_ECOUT_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_reco_L_ECOUT_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_reco_L_ECOUT_matched_1e_cut->Fill(Path_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    h_reco_P_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Mag(), weight);
                    h_reco_theta_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    h_reco_phi_nFD_VS_ToF_nFD_fromFile_ECOUT_matched_1e_cut->Fill(reco_ToF_nFD, reco_P_nFD.Phi() * 180 / M_PI, weight);
                }
            }

            h_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
            h_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
            h_reco_theta_nFD_minus_reco_theta_e_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI),
                                                                                                      reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, weight);
            h_reco_P_nFD_VS_reco_theta_nFD_minus_reco_theta_e_matched_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI - reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
            h_reco_P_nFD_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), reco_P_nFD.Mag(), weight);

            TVector3 v_nhit(neutrons_FD_matched[i]->cal(detlayer)->getX(), neutrons_FD_matched[i]->cal(detlayer)->getY(), neutrons_FD_matched[i]->cal(detlayer)->getZ());

            for (int j = 0; j < allParticles.size(); j++) {
                if (allParticles[j]->par()->getCharge() == 0) {
                    bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                    TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                    if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                        v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
                        TVector3 v_dist = v_nhit - v_neutral_hit;

                        h_v_dist_matched_1e_cut->Fill(v_dist.Mag(), weight);
                        h_v_dist_VS_reco_P_nFD_matched_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                        h_v_dist_VS_reco_P_e_matched_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                        h_v_dist_VS_reco_phi_nFD_minus_reco_phi_e_matched_1e_cut->Fill(v_dist.Mag(), CalcdPhi(reco_P_nFD.Phi() * 180 / M_PI - reco_P_e.Phi() * 180 / M_PI), weight);
                    }
                }
            }
        }

        if (neutrons_FD_matched.size() != 0) { h_reco_nFD_multi_matched_1e_cut->Fill(neutrons_FD_matched.size(), weight); }
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
    titles.SetTextSize(0.1);
    text.SetTextSize(0.05);

    gStyle->SetOptStat("ourmen");

#pragma region /* Acceptance maps */
    TCanvas* myCanvas_aMaps = new TCanvas("myCanvas_aMaps", "myCanvas_aMaps", pixelx, pixely);

    myCanvas_aMaps->cd();
    myCanvas_aMaps->SetBottomMargin(0.14), myCanvas_aMaps->SetLeftMargin(0.16), myCanvas_aMaps->SetRightMargin(0.12);

    aMaps_master.DrawAndSaveHitMaps(SampleName, myCanvas_aMaps, OutputDirAMapsMaps);
#pragma endregion

#pragma region /* General histograms */

#pragma region /* Print electron cuts plots */
    // TCanvas* myCanvas_electron_cuts = new TCanvas("myPage_electron_cuts", "myPage_electron_cuts", pixelx, pixely);
    TCanvas* myCanvas_electron_cuts = new TCanvas("myPage_electron_cuts", "myPage_electron_cuts", pixelx, pixely);

    string electron_cuts_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/electron_cuts.pdf";
    char fileName_electron_cuts[electron_cuts_PDF_fileName.length()];
    sprintf(fileName_electron_cuts, "%s[", electron_cuts_PDF_fileName.c_str());
    myText->SaveAs(fileName_electron_cuts);
    sprintf(fileName_electron_cuts, "%s", electron_cuts_PDF_fileName.c_str());

    /////////////////////////////////////
    // CND Neutron Information
    /////////////////////////////////////
    myText->cd();
    if (ConstrainedE) {
        text.DrawLatex(0.2, 0.9, "Uniform sample of (e,e'n) events (truth-level) - ConstrainedE");
    } else {
        text.DrawLatex(0.2, 0.9, "Uniform sample of (e,e'n) events (truth-level)");
    }

    if (FindSubstring(InputFiles, "2070MeV")) {
        text.DrawLatex(0.2, 0.7, "Beam energy: 2070MeV");
    } else if (FindSubstring(InputFiles, "4029MeV")) {
        text.DrawLatex(0.2, 0.7, "Beam energy: 4029MeV");
    } else if (FindSubstring(InputFiles, "5986MeV")) {
        text.DrawLatex(0.2, 0.7, "Beam energy: 5986MeV");
    }

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
        // string Title = HistoList_electron_cuts[i]->GetTitle();
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
                palette->SetY2NDC(0.55);
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

#pragma region /* Print neutron plots */
    TCanvas* myCanvas = new TCanvas("myPage", "myPage", pixelx, pixely);

    string nFD_eff_test_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/nFD_eff_test.pdf";
    char fileName[nFD_eff_test_PDF_fileName.length()];
    sprintf(fileName, "%s[", nFD_eff_test_PDF_fileName.c_str());
    myText->SaveAs(fileName);
    sprintf(fileName, "%s", nFD_eff_test_PDF_fileName.c_str());

    /////////////////////////////////////
    // CND Neutron Information
    /////////////////////////////////////
    myText->cd();
    text.DrawLatex(0.2, 0.9, "Uniform sample of (e,e'n) events (truth-level)");
    if (Is2GeV) {
        text.DrawLatex(0.2, 0.7, "Beam energy: 2070MeV");
    } else if (Is4GeV) {
        text.DrawLatex(0.2, 0.7, "Beam energy: 4029MeV");
    } else if (Is6GeV) {
        text.DrawLatex(0.2, 0.7, "Beam energy: 5986MeV");
    }
    myText->Print(fileName, "pdf");
    myText->Clear();

    for (int i = 0; i < HistoList.size(); i++) {
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
        // string Title = HistoList[i]->GetTitle();
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
                palette->SetY2NDC(0.55);
                gPad->Modified();
                gPad->Update();
            }
        }

        myCanvas->Print(fileName, "pdf");
        myCanvas->Clear();
    }

    sprintf(fileName, "%s]", nFD_eff_test_PDF_fileName.c_str());
    myCanvas->Print(fileName, "pdf");
#pragma endregion

#pragma region /* Print eff plots */
    vector<vector<TH1*>> HistoList_eff_plots;

    TH1D* h_eff_P_nFD_1e_cut_demominator = (TH1D*)h_truth_P_nFD_ECALveto_1e_cut->Clone((std::string(h_truth_P_nFD_ECALveto_1e_cut->GetName()) + "_demominator").c_str());
    // TH1D* h_eff_P_nFD_1e_cut_demominator = (TH1D*)h_truth_P_n_1e_cut->Clone((std::string(h_truth_P_n_1e_cut->GetName()) + "_demominator").c_str());

    TH1D* h_eff_P_nFD_ECALveto_1e_cut_numerator = (TH1D*)h_reco_P_nFD_ECALveto_1e_cut->Clone((std::string(h_reco_P_nFD_ECALveto_1e_cut->GetName()) + "_numerator").c_str());
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

    string eff_plots_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/eff_plots.pdf";
    char fileName_eff_plots[eff_plots_PDF_fileName.length()];
    sprintf(fileName_eff_plots, "%s[", eff_plots_PDF_fileName.c_str());
    myText->SaveAs(fileName_eff_plots);
    sprintf(fileName_eff_plots, "%s", eff_plots_PDF_fileName.c_str());

    /////////////////////////////////////
    // CND Neutron Information
    /////////////////////////////////////
    myCanvas_eff_plots->cd();
    gPad->Update();
    // myText->cd();
    text.DrawLatex(0.2, 0.9, "Uniform sample of (e,e'n) events (truth-level)");
    if (FindSubstring(InputFiles, "2070MeV")) {
        text.DrawLatex(0.2, 0.7, "Beam energy: 2070MeV");
    } else if (FindSubstring(InputFiles, "4029MeV")) {
        text.DrawLatex(0.2, 0.7, "Beam energy: 4029MeV");
    } else if (FindSubstring(InputFiles, "5986MeV")) {
        text.DrawLatex(0.2, 0.7, "Beam energy: 5986MeV");
    }
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
                    palette->SetY2NDC(0.55);
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

#pragma endregion

#pragma endregion
}
