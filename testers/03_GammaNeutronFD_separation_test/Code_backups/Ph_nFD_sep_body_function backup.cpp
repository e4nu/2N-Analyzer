#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include <cmath>
#include <tuple>
#include <vector>

// Include histograms:
#include "histograms/CLAS12Reco/CLAS12Reco_histograms.h"
#include "histograms/NeutronSelection_histograms/NeutronSelection_histograms.h"
#include "histograms/RawReco/RawRecoKinematics_histograms.h"
#include "histograms/RawReco/RawRecoPID_histograms.h"
#include "histograms/RawTruth/RawTruth_histograms.h"
#include "histograms/Redef/Redef_histograms.h"
#include "histograms/TruthLevel/TruthLevel_histograms.h"

// Include variables:
#include "helpers/FDNeutralHelpers.h"
#include "variables/NeutronSelection_variables.h"

// Include vetoes:
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
                              bool plot_tester_histograms, std::string General_status, bool plot_AMaps, bool plot_and_fit_MomRes, bool plot_FDNeutralVeto) {
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

    std::string OutFolderName_prefix = bt::ToStringWithPrecision(version, 0) + "_2Nana";
    std::string OutFolderName_ver_status = "";
    // std::string OutFolderName_ver_status = "_v" + bt::ToStringWithPrecision(version, 0);
    std::string samples_status = use_ConstPn_samples ? "CPn" : "FPn";
    std::string ReDef_status = apply_neutFD_redef ? "_Redef" : "_c12def";
    std::string cPart_ECAL_veto_status = apply_cPart_ECAL_veto ? "_wcEV" : "_wocEV";
    std::string nPart_ECAL_veto_status = apply_nPart_ECAL_veto ? "_wnEV" : "_wonEV";
    std::string no_same_sector_cuts_status = "";
    // std::string no_same_sector_cuts_status = apply_no_same_sector_cuts ? "_wSSC" : "_woSSC";
    std::string nFD_multi_status = apply_nFD_multi_cut ? "_wNMC" : "_woNMC";
    std::string FD_photons_status = apply_no_FD_photons_cut ? "_NoPhFDc_0" + bt::ToStringWithPrecision(10 * phFD_mom_th.GetLowerCut(), 0) : "_wPhFD";
    std::string no_other_part_status = apply_no_other_part_multi_cut ? "_woOP" : "";
    // std::string no_other_part_status = apply_no_other_part_multi_cut ? "_woOP" : "_wOP";
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
    std::string OutFolderName = OutFolderName_prefix + "_" + OutFolderName_ver_status + Ebeam_status + "_" + samples_status + ReDef_status + cPart_ECAL_veto_status + nPart_ECAL_veto_status +
                                no_same_sector_cuts_status + nFD_multi_status + FD_photons_status + no_other_part_status + dTheta_status + dPhi_status + neutFD_redef_status +
                                PCAL_neutral_veto_status + rc_factor_status + nPart_veto_radius_status + Good_nFD_status + Bad_nFD_status + ConstrainedE_status + General_status;

    std::string OutFileName = OutFolderName;

    if (OnlyGood_nFD == OnlyBad_nFD) { OnlyGood_nFD = OnlyBad_nFD = false; }

    double P_upperLim = ConstrainTLmom ? (Ebeam * 0.5) : (Ebeam * 1.1);
    const std::string ParentOutputDir =
        "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + ParentOutFolderName + "/" + samples_status + ReDef_status + cPart_ECAL_veto_status + nPart_ECAL_veto_status;
    // const std::string ParentOutputDir = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + ParentOutFolderName;
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

    vector<TH1*> HistoList_RawTruth, HistoList_truth_1e_cut, HistoList_PID_plots, HistoList_raw, HistoList_clas12reco, HistoList_redef, HistoList;

    vector<TObject*> HistoList_nRes_BysliceOf_truth_P_nFD, HistoList_nRes_BysliceOf_reco_P_nFD;

    vector<TObject*> HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n;

    vector<vector<double>> dr_n_SliceLimits = Set_dr_n_Slices(10., 0., 300.);

    vector<std::string> HistSubjects, HistSubjects2;
    vector<bool> FirstPrint;

    gStyle->SetTitleXSize(0.05), gStyle->SetTitleYSize(0.05);
    gStyle->SetTitleXOffset(0.8), gStyle->SetTitleYOffset(0.8);

    // Truth 1e cut histograms: Histograms filled with truth information, but only for events that pass the one reco electron cut. Used to understand the true kinematics of the events that
    // pass the one reco electron cut, and to calculate detection efficiencies.
    TruthLevel_histograms TL_histograms(HistSubjects, HistSubjects2, FirstPrint, HistoList_truth_1e_cut, P_upperLim);
    cout << "\033[33m\n\nFinished setting up Truth 1e cut histograms.\n\033[0m" << std::flush;

    // Raw truth histograms: Histograms filled with truth information, without any cuts. Used as a reference to compare with the truth 1e cut histograms, to understand the effect of the one
    // reco electron cut on the true kinematics of the events.
    RawTruth_histograms RawTL_histograms(HistSubjects, HistSubjects2, FirstPrint, HistoList_RawTruth, P_upperLim);
    cout << "\033[33m\n\nFinished setting up RAW truth histograms.\n\033[0m" << std::flush;

    // Raw reco PID histograms: Histograms filled with reconstructed information, using the raw reconstruction (no nFD redef, no ECAL vetoes, etc.), for events that pass the one reco
    // electron cut. Used to understand the reconstructed kinematics of the events that pass the one reco electron cut, and to compare with the truth 1e cut histograms to understand the
    // effect of the one reco electron cut on the reconstructed kinematics of the events.
    // Raw reco kinematics histograms: Histograms filled with reconstructed information, using the raw reconstruction (no nFD redef, no ECAL vetoes, etc.), for events that pass the one reco
    // electron cut. Used to understand the reconstructed kinematics of the events that pass the one reco electron cut, and to compare with the truth 1e cut histograms to understand the
    // effect of the one reco electron cut on the reconstructed kinematics of the events.
    RawRecoPID_histograms RRPID_histograms(HistoList_PID_plots, Ebeam);
    RawRecoKinematics_histograms RRKin_histograms(HistoList_raw, P_upperLim);
    cout << "\033[33m\n\nFinished setting up RAW histograms.\n\033[0m" << std::flush;

    // Clas12 reco histograms: Histograms filled with reconstructed information, using the clas12reco reconstruction (with nFD redef, ECAL vetoes, etc.), for events that pass the one reco
    // electron cut. Used to understand the reconstructed kinematics of the events that pass the one reco electron cut, and to compare with the truth 1e cut histograms to understand the
    // effect of the one reco electron cut on the reconstructed kinematics of the events, as well as to understand the effect of the nFD redef and ECAL vetoes on the reconstructed kinematics
    // of the events.
    CLAS12Reco_histograms clas12reco_histograms(HistSubjects, HistSubjects2, FirstPrint, HistoList_clas12reco, P_upperLim, Ebeam);
    cout << "\033[33m\n\nFinished setting up clas12reco histograms.\n\033[0m" << std::flush;

    // Redef histograms: Histograms filled with reconstructed information, using the redefined reconstruction (with nFD redef, but no ECAL vetoes), for events that pass the one reco electron
    // cut. Used to understand the reconstructed kinematics of the events that pass the one reco electron cut, and to compare with the truth 1e cut histograms to understand the effect of the
    // one reco electron cut on the reconstructed kinematics of the events, as well as to understand the effect of the nFD redef on the reconstructed kinematics of the events.
    Redef_histograms Redef_histograms(HistSubjects, HistSubjects2, FirstPrint, HistoList_redef, P_upperLim, Ebeam);
    cout << "\033[33m\n\nFinished setting up redef. histograms.\n\033[0m" << std::flush;

#pragma region /* ECALveto */

    NeutronSelection_histograms ECALveto_histograms(sctx{"ECALveto", "#splitline{ECALveto}{and P_{nFD} thresholds}", true}, HistSubjects, HistSubjects2, FirstPrint, HistoList, P_upperLim,
                                                    Ebeam, "ECALveto");

    // HistSubjects.push_back("ECALveto");
    // HistSubjects2.push_back("#splitline{ECALveto}{and P_{nFD} thresholds}");
    // FirstPrint.push_back(true);

#pragma region /* Base histograms (ECALveto) */

    // TH1D* h_reco_P_e_ECALveto_1e_cut = new TH1D("reco_P_e_ECALveto_1e_cut", "P^{reco}_{e} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_reco_P_e_ECALveto_1e_cut);
    // // TH1D* h_truth_P_e_ECALveto_1e_cut = new TH1D("truth_P_e_ECALveto_1e_cut", "P^{truth}_{e} in 1e cut (ECALveto);P^{truth}_{e} [GeV/c];Counts", 50, 0, P_upperLim);
    // // HistoList.push_back(h_truth_P_e_ECALveto_1e_cut);
    // TH1D* h_reco_theta_e_ECALveto_1e_cut = new TH1D("reco_theta_e_ECALveto_1e_cut", "#theta^{reco}_{e} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];Counts", 50, 0, 45.);
    // HistoList.push_back(h_reco_theta_e_ECALveto_1e_cut);
    // // TH1D* h_truth_theta_e_ECALveto_1e_cut = new TH1D("truth_theta_e_ECALveto_1e_cut", "#theta^{truth}_{e} in 1e cut (ECALveto);#theta^{truth}_{e} [#circ];Counts", 50, 0, 45.);
    // // HistoList.push_back(h_truth_theta_e_ECALveto_1e_cut);
    // TH1D* h_reco_phi_e_ECALveto_1e_cut = new TH1D("reco_phi_e_ECALveto_1e_cut", "#phi^{reco}_{e} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];Counts", 50, -180, 180.);
    // HistoList.push_back(h_reco_phi_e_ECALveto_1e_cut);
    // // TH1D* h_truth_phi_e_ECALveto_1e_cut = new TH1D("truth_phi_e_ECALveto_1e_cut", "#phi^{truth}_{e} in 1e cut (ECALveto);#phi^{truth}_{e} [#circ];Counts", 50, -180, 180.);
    // // HistoList.push_back(h_truth_phi_e_ECALveto_1e_cut);
    // TH2D* h_reco_theta_e_ECALveto_VS_reco_phi_e_ECALveto_1e_cut =
    //     new TH2D("reco_theta_e_ECALveto_VS_reco_phi_e_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{e} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#theta^{reco}_{e} [#circ]",
    //     100,
    //              -180., 180., 100, 0, 50.);
    // HistoList.push_back(h_reco_theta_e_ECALveto_VS_reco_phi_e_ECALveto_1e_cut);
    // // TH2D* h_truth_theta_e_ECALveto_VS_truth_phi_e_ECALveto_1e_cut =
    // //     new TH2D("truth_theta_e_ECALveto_VS_truth_phi_e_ECALveto_1e_cut", "#theta^{truth}_{e} vs. #phi^{truth}_{e} in 1e cut (ECALveto);#phi^{truth}_{e} [#circ];#theta^{truth}_{e}
    // [#circ]",
    // //              100, -180., 180., 100, 0, 50.);
    // // HistoList.push_back(h_truth_theta_e_ECALveto_VS_truth_phi_e_ECALveto_1e_cut);

    // TH1D* h_reco_P_nFD_ECALveto_1e_cut = new TH1D("reco_P_nFD_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_reco_P_nFD_ECALveto_1e_cut);

    // // Sub-detector specific histograms according to clas12particle_hit_in_PCAL, clas12particle_hit_in_ECIN, clas12particle_hit_in_ECOUT:
    // TH1D* h_reco_P_nFD_bool_PCAL_test_only_ECALveto_1e_cut =
    //     new TH1D("reco_P_nFD_bool_PCAL_test_only_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut, bool PCAL test only (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_reco_P_nFD_bool_PCAL_test_only_ECALveto_1e_cut);
    // TH1D* h_reco_P_nFD_bool_ECIN_test_only_ECALveto_1e_cut =
    //     new TH1D("reco_P_nFD_bool_ECIN_test_only_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut, bool ECIN test only (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_reco_P_nFD_bool_ECIN_test_only_ECALveto_1e_cut);
    // TH1D* h_reco_P_nFD_bool_ECOUT_test_only_ECALveto_1e_cut =
    //     new TH1D("reco_P_nFD_bool_ECOUT_test_only_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut, bool ECOUT test only (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_reco_P_nFD_bool_ECOUT_test_only_ECALveto_1e_cut);

    // // Sub-detector specific histograms according to clas12detlayer:
    // TH1D* h_reco_P_nFD_PCAL_only_clas12detlayer_ECALveto_1e_cut = new TH1D(
    //     "reco_P_nFD_PCAL_only_clas12detlayer_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut, PCAL only with clas12detlayer (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_reco_P_nFD_PCAL_only_clas12detlayer_ECALveto_1e_cut);
    // TH1D* h_reco_P_nFD_ECIN_only_clas12detlayer_ECALveto_1e_cut = new TH1D(
    //     "reco_P_nFD_ECIN_only_clas12detlayer_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut, ECIN only with clas12detlayer (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_reco_P_nFD_ECIN_only_clas12detlayer_ECALveto_1e_cut);
    // TH1D* h_reco_P_nFD_ECOUT_only_clas12detlayer_ECALveto_1e_cut = new TH1D(
    //     "reco_P_nFD_ECOUT_only_clas12detlayer_ECALveto_1e_cut", "P^{reco}_{nFD} in 1e cut, ECOUT only with clas12detlayer (ECALveto);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_reco_P_nFD_ECOUT_only_clas12detlayer_ECALveto_1e_cut);

    // TH1D* h_truth_P_nFD_ECALveto_1e_cut = new TH1D("truth_P_nFD_ECALveto_1e_cut", "P^{truth}_{nFD} in 1e cut (ECALveto);P^{truth}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_truth_P_nFD_ECALveto_1e_cut);
    // // TH1D* h_reco_theta_nFD_ECALveto_1e_cut = new TH1D("reco_theta_nFD_ECALveto_1e_cut", "#theta^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{nFD} [#circ];Counts", 50, 0, 45.);
    // // HistoList.push_back(h_reco_theta_nFD_ECALveto_1e_cut);
    // TH1D* h_truth_theta_nFD_ECALveto_1e_cut = new TH1D("truth_theta_nFD_ECALveto_1e_cut", "#theta^{truth}_{nFD} in 1e cut (ECALveto);#theta^{truth}_{nFD} [#circ];Counts", 50, 0, 45.);
    // HistoList.push_back(h_truth_theta_nFD_ECALveto_1e_cut);
    // // TH1D* h_reco_phi_nFD_ECALveto_1e_cut = new TH1D("reco_phi_nFD_ECALveto_1e_cut", "#phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{nFD} [#circ];Counts", 50, -180, 180.);
    // // HistoList.push_back(h_reco_phi_nFD_ECALveto_1e_cut);
    // TH1D* h_truth_phi_nFD_ECALveto_1e_cut = new TH1D("truth_phi_nFD_ECALveto_1e_cut", "#phi^{truth}_{nFD} in 1e cut (ECALveto);#phi^{truth}_{nFD} [#circ];Counts", 50, -180, 180.);
    // HistoList.push_back(h_truth_phi_nFD_ECALveto_1e_cut);
    // // TH2D* h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut =
    // //     new TH2D("reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut",
    // //              "#theta^{reco}_{nFD} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{nFD} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    // // HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut);
    // TH2D* h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut =
    //     new TH2D("truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut",
    //              "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} in 1e cut (ECALveto);#phi^{truth}_{nFD} [#circ];#theta^{truth}_{nFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    // HistoList.push_back(h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut);

    // TH1D* h_reco_P_LnFD_ECALveto_1e_cut = new TH1D("reco_P_LnFD_ECALveto_1e_cut", "P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];Counts", 50, 0, P_upperLim);
    // HistoList.push_back(h_reco_P_LnFD_ECALveto_1e_cut);
    // TH1D* h_reco_theta_LnFD_ECALveto_1e_cut = new TH1D("reco_theta_LnFD_ECALveto_1e_cut", "#theta^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{LnFD} [#circ];Counts", 50, 0, 45.);
    // HistoList.push_back(h_reco_theta_LnFD_ECALveto_1e_cut);
    // TH1D* h_reco_phi_LnFD_ECALveto_1e_cut = new TH1D("reco_phi_LnFD_ECALveto_1e_cut", "#phi^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{LnFD} [#circ];Counts", 50, -180, 180.);
    // HistoList.push_back(h_reco_phi_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut",
    //              "#theta^{reco}_{LnFD} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{LnFD} [#circ];#theta^{reco}_{LnFD} [#circ]", 100, -180., 180., 100, 0, 50.);
    // HistoList.push_back(h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut);

    TH1D* h_reco_P_phFD_BPID_ECALveto_1e_cut =
        new TH1D("reco_P_phFD_BPID_ECALveto_1e_cut", "P^{reco}_{#gammaFD} in 1e cut BPID (ECALveto);P^{reco}_{#gammaFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_phFD_BPID_ECALveto_1e_cut);
    TH1D* h_reco_P_phFD_APID_ECALveto_1e_cut =
        new TH1D("reco_P_phFD_APID_ECALveto_1e_cut", "P^{reco}_{#gammaFD} in 1e cut APID (ECALveto);P^{reco}_{#gammaFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_phFD_APID_ECALveto_1e_cut);

    // TH1D* h_Edep_ECAL_nFD_ECALveto_1e_cut = new TH1D("Edep_ECAL_nFD_ECALveto_1e_cut", "E^{ECAL}_{dep} in 1e cut (ECALveto);E^{ECAL}_{dep} [GeV];Counts", 50, 0., 0.5);
    // HistoList.push_back(h_Edep_ECAL_nFD_ECALveto_1e_cut);
    // TH1D* h_beta_nFD_ECALveto_1e_cut = new TH1D("beta_nFD_ECALveto_1e_cut", "#beta_{nFD} in 1e cut (ECALveto);#beta_{nFD};Counts", 50, 0.3, 1.1);
    // HistoList.push_back(h_beta_nFD_ECALveto_1e_cut);
    // TH2D* h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut =
    //     new TH2D("Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. #beta_{nFD} in 1e cut (ECALveto);#beta_{nFD};E^{ECAL}_{dep} [GeV]", 100, 0.3, 1.1, 100, 0., 0.5);
    // HistoList.push_back(h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut);

    // // TH1D* h_Edep_ECAL_LnFD_ECALveto_1e_cut = new TH1D("Edep_ECAL_LnFD_ECALveto_1e_cut", "E^{ECAL}_{dep} in 1e cut (ECALveto);E^{ECAL}_{dep} [GeV];Counts", 50, 0., 0.5);
    // // HistoList.push_back(h_Edep_ECAL_LnFD_ECALveto_1e_cut);
    // // TH1D* h_beta_LnFD_ECALveto_1e_cut = new TH1D("beta_LnFD_ECALveto_1e_cut", "#beta_{LnFD} in 1e cut (ECALveto);#beta_{LnFD};Counts", 50, 0.3, 1.1);
    // // HistoList.push_back(h_beta_LnFD_ECALveto_1e_cut);
    // // TH2D* h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut =
    // //     new TH2D("Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. #beta_{LnFD} in 1e cut (ECALveto);#beta_{LnFD};E^{ECAL}_{dep} [GeV]", 100, 0.3, 1.1, 100, 0., 0.5);
    // // HistoList.push_back(h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut);

    // TH2D* h_nFD_PCAL_hits_VS_nFD_ECIN_hits_ECALveto_1e_cut =
    //     new TH2D("nFD_PCAL_hits_VS_nFD_ECIN_hits_ECALveto_1e_cut", "#(nFD hits in PCAL) vs. #(nFD hits in ECIN) in 1e cut (ECALveto);#(nFD hits in PCAL);#(nFD hits in ECIN)", 2,
    //     -0.5, 1.5,
    //              2, -0.5, 1.5);
    // HistoList.push_back(h_nFD_PCAL_hits_VS_nFD_ECIN_hits_ECALveto_1e_cut);
    // TH2D* h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut =
    //     new TH2D("nFD_PCAL_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut", "#(nFD hits in PCAL) vs. #(nFD hits in ECOUT) in 1e cut (ECALveto);#(nFD hits in PCAL);#(nFD hits in ECOUT)", 2, -0.5,
    //              1.5, 2, -0.5, 1.5);
    // HistoList.push_back(h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut);
    // TH2D* h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut =
    //     new TH2D("nFD_ECIN_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut", "#(nFD hits in ECIN) vs. #(nFD hits in ECOUT) in 1e cut (ECALveto);#(nFD hits in ECIN);#(nFD hits in ECOUT)", 2, -0.5,
    //              1.5, 2, -0.5, 1.5);
    // HistoList.push_back(h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut);

    // TH2D* h_reco_P_e_VS_P_nFD_ECALveto_1e_cut = new TH2D(
    //     "reco_P_e_VS_P_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];P^{reco}_{nFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_e_VS_P_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_theta_e_VS_P_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, 0, 45., 100,
    //     0.,
    //              Ebeam * 3.);
    // HistoList.push_back(h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_phi_e_VS_P_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];P^{reco}_{nFD} [GeV/c]", 100, -180., 180., 100,
    //     0.,
    //              Ebeam * 3.);
    // HistoList.push_back(h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_P_e_VS_theta_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#theta^{reco}_{nFD} [#circ]", 100, 0.,
    //     P_upperLim,
    //              100, 0, 45.);
    // HistoList.push_back(h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_theta_e_VS_theta_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100,
    //     0,
    //              45., 100, 0, 45.);
    // HistoList.push_back(h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_phi_e_VS_theta_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #theta^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#theta^{reco}_{nFD} [#circ]", 100, -180.,
    //              180., 100, 0, 45.);
    // HistoList.push_back(h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_P_e_VS_phi_nFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#phi^{reco}_{nFD} [#circ]", 100, 0., P_upperLim, 100,
    //              -180., 180.);
    // HistoList.push_back(h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_theta_e_VS_phi_nFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100,
    //     0, 45.,
    //              100, -180., 180.);
    // HistoList.push_back(h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_phi_e_VS_phi_nFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{nFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#phi^{reco}_{nFD} [#circ]", 100, -180., 180.,
    //              100, -180., 180.);
    // HistoList.push_back(h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut", "#theta^{reco}_{nFD} vs. P^{reco}_{nFD} in 1e cut (ECALveto);#theta^{reco}_{nFD} [#circ];P^{reco}_{nFD} [GeV/c]", 100,
    //              0., 45., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut);
    // TH2D* h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut =
    //     new TH2D("reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut", "reco nFD multiplicity vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];nFD multiplicity", 100, 0., Ebeam
    //     * 3.,
    //              9, 1, 10);
    // HistoList.push_back(h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut);

    TH1D* h_reco_nFD_multi_AllN_ECALveto_1e_cut = new TH1D("reco_nFD_multi_AllN_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (all nFDs, ECALveto);nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_AllN_ECALveto_1e_cut);
    TH1D* h_reco_nFD_multi_OnlyGood_ECALveto_1e_cut =
        new TH1D("reco_nFD_multi_OnlyGood_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (Only good nFD, ECALveto);Good nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_OnlyGood_ECALveto_1e_cut);
    TH1D* h_reco_nFD_multi_OnlyBad_ECALveto_1e_cut =
        new TH1D("reco_nFD_multi_OnlyBad_ECALveto_1e_cut", "reco nFD multiplicity in 1e cut (Only bad nFD, ECALveto);Bad nFD multiplicity;Counts", 9, 1, 10);
    HistoList.push_back(h_reco_nFD_multi_OnlyBad_ECALveto_1e_cut);

    // TH2D* h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut = new TH2D(
    //     "reco_P_e_VS_P_LnFD_ECALveto_1e_cut", "P^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];P^{reco}_{LnFD} [GeV/c]", 100, 0., Ebeam * 2., 100, 0., Ebeam
    //     * 3.);
    // HistoList.push_back(h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_theta_e_VS_P_LnFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];P^{reco}_{LnFD} [GeV/c]", 100, 0, 45.,
    //     100,
    //              0., Ebeam * 3.);
    // HistoList.push_back(h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_phi_e_VS_P_LnFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];P^{reco}_{LnFD} [GeV/c]", 100, -180., 180., 100,
    //              0., Ebeam * 3.);
    // HistoList.push_back(h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_P_e_VS_theta_LnFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#theta^{reco}_{LnFD} [#circ]", 100, 0.,
    //              P_upperLim, 100, 0, 45.);
    // HistoList.push_back(h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#theta^{reco}_{LnFD} [#circ]",
    //     100,
    //              0, 45., 100, 0, 45.);
    // HistoList.push_back(h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #theta^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#theta^{reco}_{LnFD} [#circ]", 100,
    //     -180.,
    //              180., 100, 0, 45.);
    // HistoList.push_back(h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_P_e_VS_phi_LnFD_ECALveto_1e_cut", "P^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#phi^{reco}_{LnFD} [#circ]", 100, 0., P_upperLim,
    //     100,
    //              -180., 180.);
    // HistoList.push_back(h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut", "#theta^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{e} [#circ];#phi^{reco}_{LnFD} [#circ]", 100, 0,
    //              45., 100, -180., 180.);
    // HistoList.push_back(h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut", "#phi^{reco}_{e} vs. #phi^{reco}_{LnFD} in 1e cut (ECALveto);#phi^{reco}_{e} [#circ];#phi^{reco}_{LnFD} [#circ]", 100, -180.,
    //     180.,
    //              100, -180., 180.);
    // HistoList.push_back(h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut);
    // TH2D* h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut =
    //     new TH2D("reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut", "#theta^{reco}_{LnFD} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);#theta^{reco}_{LnFD} [#circ];P^{reco}_{LnFD}
    //     [GeV/c]",
    //              100, 0., 45., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut);

#pragma endregion

    // TH1D* h_reco_dTheta_nFD_e_ECALveto_1e_cut =
    //     new TH1D("reco_dTheta_nFD_e_ECALveto_1e_cut", "#delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e}
    //     [#circ];Counts",
    //              50, -25., 10.);
    // HistoList.push_back(h_reco_dTheta_nFD_e_ECALveto_1e_cut);
    // TH1D* h_reco_dPhi_nFD_e_ECALveto_1e_cut =
    //     new TH1D("reco_dPhi_nFD_e_ECALveto_1e_cut", "#delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];Counts", 50,
    //              -180., 180.);
    // HistoList.push_back(h_reco_dPhi_nFD_e_ECALveto_1e_cut);
    // TH2D* h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
    //     new TH2D("reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
    //              "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| "
    //              "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
    //              100, -180., 180., 100, -25., 10.);
    // HistoList.push_back(h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);
    // TH2D* h_reco_P_nFD_VS_reco_dTheta_nFD_e_ECALveto_1e_cut =
    //     new TH2D("reco_P_nFD_VS_reco_dTheta_nFD_e_ECALveto_1e_cut",
    //              "P^{reco}_{nFD} vs. #delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
    //              "[#circ];P^{reco}_{nFD} [GeV/c]",
    //              100, -25., 10., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_reco_dTheta_nFD_e_ECALveto_1e_cut);
    // TH2D* h_reco_P_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
    //     new TH2D("reco_P_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
    //              "P^{reco}_{nFD} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ];P^{reco}_{nFD} [GeV/c]", 100,
    //              -180., 180., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_reco_P_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    // TH2D* h_reco_P_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
    //     new TH2D("reco_P_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
    //              "P^{reco}_{e} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);P^{reco}_{e} [GeV/c];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0.,
    //              Ebeam * 1.1, 100, -180., 180.);
    // HistoList.push_back(h_reco_P_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    // TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut =
    //     new TH2D("nFD_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut",
    //              "nFD status vs. #delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
    //              "[#circ];nFD status",
    //              100, -25., 10., 100, 1990, 3000.);
    // HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut);

    // TH2D* h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut =
    //     new TH2D("nFD_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut",
    //              "nFD status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
    //              "[#circ];nFD status",
    //              100, -10., 5., 100, 1990, 2250.);
    // //  100, -25., 10., 100, 1990, 2250.);
    // HistoList.push_back(h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut);

    // TH2D* h_e_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut =
    //     new TH2D("e_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut",
    //              "e status vs. #delta#theta^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
    //              "[#circ];e status",
    //              100, -25., 10., 100, -2250., -1990.);
    // HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut);

    // TH2D* h_e_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut =
    //     new TH2D("e_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut",
    //              "e status vs. #delta#theta^{reco}_{nFD,e} - zoomin - in 1e cut (ECALveto);#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} "
    //              "[#circ];e status",
    //              100, -10., 5., 100, -2250., -0.);
    // //  100, -25., 10., 100, 1990, 2250.);
    // HistoList.push_back(h_e_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut);

    // TH2D* h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_ECALveto_1e_cut =
    //     new TH2D("dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_ECALveto_1e_cut",
    //              "'Neutron Hits' vs. Electron Hits - After Veto in 1e cut (ECALveto);#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [#circ];#Delta#theta_{n,e} = "
    //              "#theta_{n}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
    //              100, -180, 180, 100, -25, 10);
    // //  100, -180, 180, 100, -50, 50);
    // HistoList.push_back(h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_ECALveto_1e_cut);

    // TH1D* h_dr_n_ECALveto_1e_cut =
    //     new TH1D("dr_n_ECALveto_1e_cut", "|#Delta#font[62]{r}_{n}| in 1e cut (ECALveto);|#Delta#font[62]{r}_{n}| = |#font[62]{r}^{hit}_{n} - #font[62]{R}_{e}| [cm];Counts", 50, 0, 400);
    // HistoList.push_back(h_dr_n_ECALveto_1e_cut);
    // TH2D* h_dr_n_VS_dTheta_n_e_ECALveto_1e_cut = new TH2D(
    //     "dr_n_VS_dTheta_n_e_ECALveto_1e_cut",
    //     "|#Delta#font[62]{r}_{n}| vs. #delta#theta^{reco}_{nFD,e} - in 1e cut (ECALveto);|#Delta#font[62]{r}_{n}| = |#font[62]{r}^{hit}_{n} - #font[62]{R}_{e}| [cm];#Delta#theta_{nFD,e} =
    //     "
    //     "#theta_{V}^{ECAL} - #theta_{e}^{ECAL} [#circ]",
    //     100, 0, 400, 100, -25, 10);
    // HistoList.push_back(h_dr_n_VS_dTheta_n_e_ECALveto_1e_cut);
    // TH2D* h_dr_n_VS_dPhi_n_e_ECALveto_1e_cut = new TH2D(
    //     "dr_n_VS_dPhi_n_e_ECALveto_1e_cut",
    //     "|#Delta#font[62]{r}_{n}| vs. #delta#phi^{reco}_{nFD,e} - in 1e cut (ECALveto);|#Delta#font[62]{r}_{n}| = |#font[62]{r}^{hit}_{n} - #font[62]{R}_{e}| [cm];#Delta#phi_{nFD,e} = "
    //     "#phi_{V}^{ECAL} - #phi_{e}^{ECAL} [#circ]",
    //     100, 0, 400, 100, -25, 10);
    // HistoList.push_back(h_dr_n_VS_dPhi_n_e_ECALveto_1e_cut);

    // std::vector<TObject*> Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_ECALveto_1e_cut;
    // hsPlots h_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_ECALveto_1e_cut =
    //     hsPlots(dr_n_SliceLimits, hsPlots::TH2D_TYPE, HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n, &Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList_ECALveto_1e_cut,
    //             "dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_ECALveto_1e_cut",
    //             "#delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} in slices of |#Deltar_{n}| in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} -
    //             #phi^{reco}_{e}| "
    //             "[#circ];#delta#theta^{reco}_{nFD,e} = #theta^{reco}_{nFD} - #theta^{reco}_{e} [#circ]",
    //             100, -180., 180., 100, -25., 10., "|#Deltar_{n}| [cm]");

    // TH2D* h_dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_ECALveto_1e_cut =
    //     new TH2D("dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_ECALveto_1e_cut",
    //              "#Delta#theta_{nFD,V} vs. #Delta#phi_{nFD,V} - After Veto in 1e cut (ECALveto);#Delta#phi_{n,V} = #phi_{n}^{ECAL} - #phi_{V} [#circ];#Delta#theta_{nFD,V} = "
    //              "#theta_{n}^{ECAL} - #theta_{V} [#circ]",
    //              100, -180, 180, 100, -25, 10);
    // //  100, -180, 180, 100, -50, 50);
    // HistoList.push_back(h_dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_ECALveto_1e_cut);

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

    // TH1D* h_v_dist_nFD_ECALveto_1e_cut = new TH1D("v_dist_nFD_ECALveto_1e_cut", "v_dist in 1e cut (ECALveto);v_dist [cm];Counts", 50, 0., 1000.);
    // HistoList.push_back(h_v_dist_nFD_ECALveto_1e_cut);
    // TH2D* h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut =
    //     new TH2D("v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut", "v_dist vs. P^{reco}_{nFD} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{nFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut);
    // TH2D* h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut =
    //     new TH2D("v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
    // HistoList.push_back(h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut);
    // TH2D* h_v_dist_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut = new TH2D(
    //     "v_dist_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
    //     "v_dist vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);v_dist [cm];#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]", 100, 0., 1000., 100, -180.,
    //     180.);
    // HistoList.push_back(h_v_dist_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    // TH1D* h_v_dist_LnFD_ECALveto_1e_cut = new TH1D("v_dist_LnFD_ECALveto_1e_cut", "v_dist in 1e cut (ECALveto);v_dist [cm];Counts", 50, 0., 1000.);
    // HistoList.push_back(h_v_dist_LnFD_ECALveto_1e_cut);
    // TH2D* h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut =
    //     new TH2D("v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut", "v_dist vs. P^{reco}_{LnFD} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{LnFD} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam * 3.);
    // HistoList.push_back(h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut);
    // TH2D* h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut =
    //     new TH2D("v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut", "v_dist vs. P^{reco}_{e} in 1e cut (ECALveto);v_dist [cm];P^{reco}_{e} [GeV/c]", 100, 0., 1000., 100, 0., Ebeam);
    // HistoList.push_back(h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut);
    // TH2D* h_v_dist_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut =
    //     new TH2D("v_dist_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut",
    //              "v_dist vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);v_dist [cm];#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]", 100, 0., 1000., 100,
    //              -180., 180.);
    // HistoList.push_back(h_v_dist_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut);

    // TH2D* h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut = new TH2D(
    //     "Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];E^{ECAL}_{dep} [GeV]", 100, 0., Ebeam * 3., 100, 0.,
    //     0.5);
    // HistoList.push_back(h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut);
    // TH2D* h_Edep_ECAL_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
    //     new TH2D("Edep_ECAL_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
    //              "E^{ECAL}_{dep} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];E^{ECAL}_{dep} [GeV]", 100,
    //              -180., 180., 100, 0., 0.5);
    // HistoList.push_back(h_Edep_ECAL_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    // TH2D* h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut =
    //     new TH2D("beta_n_VS_reco_P_nFD_ECALveto_1e_cut", "#beta_{nFD} vs. P^{reco}_{nFD} in 1e cut (ECALveto);P^{reco}_{nFD} [GeV/c];#beta_{nFD}", 100, 0., Ebeam * 3., 100, 0.3, 1.1);
    // HistoList.push_back(h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut);
    // TH2D* h_beta_n_VS_reco_dPhi_nFD_e_ECALveto_1e_cut =
    //     new TH2D("beta_n_VS_reco_dPhi_nFD_e_ECALveto_1e_cut",
    //              "#beta_{nFD} vs. #delta#phi^{reco}_{nFD,e} in 1e cut (ECALveto);#delta#phi^{reco}_{nFD,e} = |#phi^{reco}_{nFD} - #phi^{reco}_{e}| [#circ]];#beta_{nFD}", 100, -180., 180.,
    //              100, 0.3, 1.1);
    // HistoList.push_back(h_beta_n_VS_reco_dPhi_nFD_e_ECALveto_1e_cut);

    // TH2D* h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut =
    //     new TH2D("Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut", "E^{ECAL}_{dep} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];E^{ECAL}_{dep} [GeV]", 100, 0., Ebeam * 3.,
    //     100,
    //              0., 0.5);
    // HistoList.push_back(h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut);
    // TH2D* h_Edep_ECAL_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut =
    //     new TH2D("Edep_ECAL_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut",
    //              "E^{ECAL}_{dep} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| "
    //              "[#circ]];E^{ECAL}_{dep} [GeV]",
    //              100, -180., 180., 100, 0., 0.5);
    // HistoList.push_back(h_Edep_ECAL_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut);

    // TH2D* h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut =
    //     new TH2D("beta_n_VS_reco_P_LnFD_ECALveto_1e_cut", "#beta_{LnFD} vs. P^{reco}_{LnFD} in 1e cut (ECALveto);P^{reco}_{LnFD} [GeV/c];#beta_{LnFD}", 100, 0., Ebeam * 3., 100,
    //     0.3, 1.1);
    // HistoList.push_back(h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut);
    // TH2D* h_beta_n_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut =
    //     new TH2D("beta_n_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut",
    //              "#beta_{LnFD} vs. #Delta#phi^{reco}_{LnFD,e} in 1e cut (ECALveto);#Delta#phi^{reco}_{LnFD,e} = |#phi^{reco}_{LnFD} - #phi^{reco}_{e}| [#circ]];#beta_{LnFD}", 100, -180.,
    //              180., 100, 0.3, 1.1);
    // HistoList.push_back(h_beta_n_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut);

    cout << "\033[33m\n\nFinished setting up ECALveto histograms.\n\033[0m" << std::flush;

#pragma endregion

#pragma region /* matched */

    HistSubjects.push_back("matched");
    HistSubjects2.push_back("matched");
    FirstPrint.push_back(true);

#pragma region /* Base histograms (matched) */

    TH1D* h_reco_P_nFD_matched_1e_cut = new TH1D("reco_P_nFD_matched_1e_cut", "P^{reco}_{nFD} in 1e cut (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_matched_1e_cut);

    // Sub-detector specific histograms according to clas12particle_hit_in_PCAL, clas12particle_hit_in_ECIN, clas12particle_hit_in_ECOUT:
    TH1D* h_reco_P_nFD_bool_PCAL_test_only_matched_1e_cut =
        new TH1D("reco_P_nFD_bool_PCAL_test_only_matched_1e_cut", "P^{reco}_{nFD} in 1e cut, bool PCAL test only (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_bool_PCAL_test_only_matched_1e_cut);
    TH1D* h_reco_P_nFD_bool_ECIN_test_only_matched_1e_cut =
        new TH1D("reco_P_nFD_bool_ECIN_test_only_matched_1e_cut", "P^{reco}_{nFD} in 1e cut, bool ECIN test only (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_bool_ECIN_test_only_matched_1e_cut);
    TH1D* h_reco_P_nFD_bool_ECOUT_test_only_matched_1e_cut =
        new TH1D("reco_P_nFD_bool_ECOUT_test_only_matched_1e_cut", "P^{reco}_{nFD} in 1e cut, bool ECOUT test only (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_bool_ECOUT_test_only_matched_1e_cut);

    // Sub-detector specific histograms according to clas12detlayer:
    TH1D* h_reco_P_nFD_PCAL_only_clas12detlayer_matched_1e_cut =
        new TH1D("reco_P_nFD_PCAL_only_clas12detlayer_matched_1e_cut", "P^{reco}_{nFD} in 1e cut, PCAL only with clas12detlayer (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_PCAL_only_clas12detlayer_matched_1e_cut);
    TH1D* h_reco_P_nFD_ECIN_only_clas12detlayer_matched_1e_cut =
        new TH1D("reco_P_nFD_ECIN_only_clas12detlayer_matched_1e_cut", "P^{reco}_{nFD} in 1e cut, ECIN only with clas12detlayer (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_ECIN_only_clas12detlayer_matched_1e_cut);
    TH1D* h_reco_P_nFD_ECOUT_only_clas12detlayer_matched_1e_cut = new TH1D(
        "reco_P_nFD_ECOUT_only_clas12detlayer_matched_1e_cut", "P^{reco}_{nFD} in 1e cut, ECOUT only with clas12detlayer (matched);P^{reco}_{nFD} [GeV/c];Counts", 50, 0, P_upperLim);
    HistoList.push_back(h_reco_P_nFD_ECOUT_only_clas12detlayer_matched_1e_cut);

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

#pragma region /* Prepare FDNeutralVeto */

    std::cout << "\033[33m\n\nPrepareing FDNeutralVeto elements...\n\033[0m" << std::flush;

    const std::string OutputDirFDNeutralVetoPlots = OutputDir + "/04_FDNeutralVeto_Plots/";
    system(("rm -rf " + OutputDirFDNeutralVetoPlots).c_str());
    system(("mkdir -p " + OutputDirFDNeutralVetoPlots).c_str());

    TFile* fdVetoFile = new TFile((OutputDirFDNeutralVetoPlots + "FDNeutralVeto.root").c_str(), "RECREATE");

    FDNeutralVeto veto;

    FDNeutralVeto::DebugHistConfig cfg;
    // cfg.enable = false;
    // cfg.baseDir = fdVetoFile;  // TFile* outFile is fine (it is a TDirectory)
    // cfg.book2DStepHists = false;
    // cfg.writeCanvases = false;  // set true only if you really want ROOT canvases
    cfg.enable = true;
    cfg.baseDir = fdVetoFile;  // TFile* outFile is fine (it is a TDirectory)
    cfg.book2DStepHists = true;
    cfg.writeCanvases = false;  // set true only if you really want ROOT canvases

    if (plot_FDNeutralVeto) { veto.EnableDebugHists(cfg); }

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

    int NumberOfnFD_hits_in_event_ECALveto = 0, NumberOfLnFD_hits_in_event_ECALveto = 0;
    int NumberOfnFD_PCAL_hits_in_event_ECALveto = 0, NumberOfLnFD_PCAL_hits_in_event_ECALveto = 0;
    int NumberOfnFD_ECIN_hits_in_event_ECALveto = 0, NumberOfLnFD_ECIN_hits_in_event_ECALveto = 0;
    int NumberOfnFD_ECOUT_hits_in_event_ECALveto = 0, NumberOfLnFD_ECOUT_hits_in_event_ECALveto = 0;

    int NumberOfnFD_hits_in_event_matched = 0, NumberOfLnFD_hits_in_event_matched = 0;
    int NumberOfnFD_PCAL_hits_in_event_matched = 0, NumberOfLnFD_PCAL_hits_in_event_matched = 0;
    int NumberOfnFD_ECIN_hits_in_event_matched = 0, NumberOfLnFD_ECIN_hits_in_event_matched = 0;
    int NumberOfnFD_ECOUT_hits_in_event_matched = 0, NumberOfLnFD_ECOUT_hits_in_event_matched = 0;

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

            RawTL_histograms.Fill(pid_temp, Truth_P_temp, mcpbank->getTheta(), mcpbank->getPhi(), weight);
            // if (pid_temp == 11) {
            //     h_truth_P_e_RawTruth_1e_cut->Fill(Truth_P_temp, weight);
            //     h_truth_theta_e_RawTruth_1e_cut->Fill(Truth_theta_temp, weight);
            //     h_truth_phi_e_RawTruth_1e_cut->Fill(Truth_phi_temp, weight);
            //     h_truth_theta_e_VS_truth_phi_e_RawTruth_1e_cut->Fill(Truth_phi_temp, Truth_theta_temp, weight);
            //     h_truth_theta_e_VS_truth_P_e_RawTruth_1e_cut->Fill(Truth_P_temp, Truth_theta_temp, weight);
            // } else if (pid_temp == 2112) {
            //     h_truth_P_nFD_RawTruth_1e_cut->Fill(Truth_P_temp, weight);
            //     h_truth_theta_nFD_RawTruth_1e_cut->Fill(Truth_theta_temp, weight);
            //     h_truth_phi_nFD_RawTruth_1e_cut->Fill(Truth_phi_temp, weight);
            //     h_truth_theta_nFD_VS_truth_phi_nFD_RawTruth_1e_cut->Fill(Truth_phi_temp, Truth_theta_temp, weight);
            //     h_truth_theta_nFD_VS_truth_P_nFD_RawTruth_1e_cut->Fill(Truth_P_temp, Truth_theta_temp, weight);
            // }
        }

        //  =======================================================================================================================================================================
        //  Setup reco particles and applying 1e cut
        //  =======================================================================================================================================================================

#pragma region /* Setup reco particles and applying 1e cut */

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
        veto.SetStartTime(starttime);

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

#pragma endregion

        //  =======================================================================================================================================================================
        //  Helper code
        //  =======================================================================================================================================================================

#pragma region /* Helper code */

#pragma region /* Helper class: FDNeutralHelpers */

        // Instantiate the helper once per event (captures references to event-scope objects).
        const bool Recalc_Path_nFD = true;
        FDNeutralHelpers fdNeutralHelpers(veto, allParticles, electrons, Ebeam, starttime, nFD_mom_th, apply_cPart_ECAL_veto, apply_nPart_ECAL_veto, cPart_veto_radius, nPart_veto_radius,
                                          Recalc_Path_nFD);

#pragma endregion

#pragma region /* A helper lambda to build the FD photons and the ECAL/matched FD neutrons */

        auto Build_FD_neutrals = [&]() {
            // Delegate to the class implementation so the build logic can be reused outside this lambda.
            return fdNeutralHelpers.BuildFDNeutrals();
        };

#pragma endregion

#pragma region /* A helper lambda to FD neutron momentum */

        auto Calc_nFD_vars = [&](const NeutralHit& Local_reco_nFD, const double& starttime_local, const bool& Recalc_Path_nFD = true) {
            // Delegate to the class implementation for a single, consistent definition.
            return fdNeutralHelpers.Calc_nFD_vars(Local_reco_nFD, starttime_local, Recalc_Path_nFD);
        };

#pragma endregion

#pragma endregion

        //  =======================================================================================================================================================================
        //  Get truth electron variables under 1e cut for ConstrainedE (CE)
        //  =======================================================================================================================================================================

#pragma region /* Get truth electron variables under 1e cut for ConstrainedE (CE) */

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

#pragma endregion

        //  =======================================================================================================================================================================
        //  1e cut (reco)
        //  =======================================================================================================================================================================

#pragma region /* 1e cut (reco) */

        //  Electron PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Electron PID cuts */

        // TODO: ALL of these plots are being filled after cuts are applied. Find a way to fill them before cuts are applied too.

        // Electron PID (BC/AC) histogram filling via RawRecoPID_histograms helper methods

        const double vz_e = electrons[0]->par()->getVz();
        RRPID_histograms.FillElectronVzBC(vz_e, weight);
        bool bad_Vz_e_CutCond = (vz_e < -1.5 || vz_e > 0.5);
        // bool bad_Vz_e_CutCond = (vz_e < -4. || vz_e > -2.);
        if (!bad_Vz_e_CutCond) { RRPID_histograms.FillElectronVzAC(vz_e, weight); }

        // DC debug maps
        RRPID_histograms.FillElectronDCHitBC(electrons[0], weight);
        bool bad_DC_edge_CutCond = (!raf::DCEdgeCuts(electrons[0]));
        if (!bad_DC_edge_CutCond) { RRPID_histograms.FillElectronDCHitAC(electrons[0], weight); }

        const double nphe = electrons[0]->che(clas12::HTCC)->getNphe();
        RRPID_histograms.FillElectronNpheBC(nphe, weight);
        bool bad_nphe_CutCond = (nphe <= 2);
        if (!bad_nphe_CutCond) { RRPID_histograms.FillElectronNpheAC(nphe, weight); }

        RRPID_histograms.FillElectronEdepPCALvsECBC(Edep_PCAL, Edep_EC, weight);
        bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
        if (!bad_Edep_PCAL_CutCond) { RRPID_histograms.FillElectronEdepPCALvsECAC(Edep_PCAL, Edep_EC, weight); }

        const double p_e = reco_P_e.Mag();
        RRPID_histograms.FillElectronSFvsPBC(p_e, EoP_e, weight);
        bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
        if (!bad_SF_CutCond) { RRPID_histograms.FillElectronSFvsPAC(p_e, EoP_e, weight); }

        const double Lv = electrons[0]->cal(clas12::PCAL)->getLv();
        const double Lw = electrons[0]->cal(clas12::PCAL)->getLw();
        const double Lu = electrons[0]->cal(clas12::PCAL)->getLu();
        RRPID_histograms.FillElectronSFvsLvBC(Lv, EoP_e, weight);
        RRPID_histograms.FillElectronSFvsLwBC(Lw, EoP_e, weight);
        RRPID_histograms.FillElectronSFvsLuBC(Lu, EoP_e, weight);
        bool bad_PCAL_edge_CutCond = (Lv < 14. || Lw < 14.);
        if (!bad_PCAL_edge_CutCond) {
            RRPID_histograms.FillElectronSFvsLvAC(Lv, EoP_e, weight);
            RRPID_histograms.FillElectronSFvsLwAC(Lw, EoP_e, weight);
            RRPID_histograms.FillElectronSFvsLuAC(Lu, EoP_e, weight);
        }

        RRPID_histograms.FillElectronEcalDiagBC(E_PCALoP_e, E_ECINoP_e, weight);
        bool bad_diag_CutCond = (!raf::checkEcalDiagCuts(electrons[0]));
        if (!bad_diag_CutCond) { RRPID_histograms.FillElectronEcalDiagAC(E_PCALoP_e, E_ECINoP_e, weight); }

        // Apply the same cut-flow (continues) as before
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

        RRKin_histograms.FillRecoElectroKinematics(reco_P_e, weight);

#pragma endregion

#pragma region /* 1e cut (reco) - charged hadrons */

        //  Proton PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------
        RRPID_histograms.FillProtonAC(IDed_Protons_ind, protons, vtz_e, weight);

        //  Piplus PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------
        RRPID_histograms.FillPiPlusAC(Piplus_ind, piplus, vtz_e, weight);

        //  Piminus PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------
        RRPID_histograms.FillPiMinusAC(Piminus_ind, piminus, vtz_e, weight);

        //  Kaons and deuterons PID
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

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

                RRKin_histograms.FillTruthElectroKinematics(truth_P_e, weight);

                ECALveto_histograms.NSB_nFD_histograms.FillTruthElectronKinematics(truth_P_e, weight);
                // h_truth_P_e_ECALveto_1e_cut->Fill(truth_P_e.Mag(), weight);
                // h_truth_theta_e_ECALveto_1e_cut->Fill(truth_P_e.Theta() * 180 / M_PI, weight);
                // h_truth_phi_e_ECALveto_1e_cut->Fill(truth_P_e.Phi() * 180 / M_PI, weight);
                // h_truth_theta_e_ECALveto_VS_truth_phi_e_ECALveto_1e_cut->Fill(truth_P_e.Phi() * 180 / M_PI, truth_P_e.Theta() * 180 / M_PI, weight);
            } else if (pid_temp == 2112) {
                TVector3 truth_P_n;
                truth_P_n.SetXYZ(px, py, pz);

                bool Truth_InFD =
                    aMaps_FCapplier.IsInFDQuery(/* (!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps || AMapsSettings.Generate_Nucleon_AMaps))= */ false, ThetaFD,
                                                "Neutron", truth_P_n.Mag(), truth_P_n.Theta() * 180.0 / pi, truth_P_n.Phi() * 180.0 / pi, false);

                RRKin_histograms.FillTruthNeutronKinematics(truth_P_n, weight);

                if ((truth_P_n.Theta() * 180 / M_PI >= 5.) && (truth_P_n.Theta() * 180 / M_PI <= 35.)) {
                    if (truth_P_n.Mag() >= Truth_P_LnFD_mag) {
                        Truth_P_LnFD_mag = truth_P_n.Mag();
                        Truth_P_LnFD_ind = i;
                    }

                    clas12reco_histograms.FillTruthNeutronHistograms(truth_P_n, weight);
                    Redef_histograms.FillTruthNeutronHistograms(truth_P_n, weight);

                    if (PassMomTh) {
                        if (Truth_InFD) {
                            truth_NeutronsFD.push_back(i);

                            // Detection efficiency denominator histogram, common to all detection efficiency plots. Filled the truth P_nFD distribution that pass momentum and fiducial cuts
                            // to obtain the (e,e')n events, under the one reco electron cut.
                            TL_histograms.Fill(truth_P_n, weight);

                            ECALveto_histograms.NSB_nFD_histograms.FillTruthNeutronKinematics(truth_P_n, weight);
                            // h_truth_P_nFD_ECALveto_1e_cut->Fill(truth_P_n.Mag(), weight);
                            // h_truth_theta_nFD_ECALveto_1e_cut->Fill(truth_P_n.Theta() * 180 / M_PI, weight);
                            // h_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, weight);
                            // h_truth_theta_nFD_ECALveto_VS_truth_phi_nFD_ECALveto_1e_cut->Fill(truth_P_n.Phi() * 180 / M_PI, truth_P_n.Theta() * 180 / M_PI, weight);
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

                RRKin_histograms.FillRecoNeutronKinematics(reco_P_n, weight);
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

                clas12reco_histograms.FillRecoNeutronHistograms(reco_P_nFD, reco_P_e, weight);
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

                Redef_histograms.FillRecoNeutronHistograms(reco_P_nFD, reco_P_e, weight);
            }
        }

#pragma endregion

        //  ECALveto plots
        //  ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ECALveto plots */

#pragma region /* FD neutrals definitions (ECALveto) */

        double Reco_P_LnFD_mag_ECALveto = -1;  // Reco leading neutron momentum
        int Reco_P_LnFD_ind_ECALveto = -1;     // Reco leading neutron index

        std::vector<NeutralHit> neutrons_FD_ECALveto;
        // std::vector<region_part_ptr> neutrons_FD_ECALveto;
        std::vector<NeutralHit> neutrons_FD_ECALveto_OnlyGood;
        // std::vector<region_part_ptr> neutrons_FD_ECALveto_OnlyGood;
        std::vector<NeutralHit> neutrons_FD_ECALveto_OnlyBad;
        // std::vector<region_part_ptr> neutrons_FD_ECALveto_OnlyBad;
        NeutralHit neutrons_FD_ECALveto_LnFD;
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
#pragma region /* Filling FD leading neutron plots (ECALveto) */

            if (Reco_P_LnFD_ind_ECALveto != -1) {
                NeutronSelection_variables ECALveto_variables_LnFD(allParticles, electrons[0], neutrons_FD_ECALveto_LnFD, fdNeutralHelpers, reco_P_e, NumberOfnFD_hits_in_event_ECALveto,
                                                                   NumberOfnFD_PCAL_hits_in_event_ECALveto, NumberOfnFD_ECIN_hits_in_event_ECALveto,
                                                                   NumberOfnFD_ECOUT_hits_in_event_ECALveto);

                // bool ParticleInPCAL = neutrons_FD_ECALveto_LnFD.clas12particle_hit_in_PCAL;
                // bool ParticleInECIN = neutrons_FD_ECALveto_LnFD.clas12particle_hit_in_ECIN;
                // bool ParticleInECOUT = neutrons_FD_ECALveto_LnFD.clas12particle_hit_in_ECOUT;

                // if (ParticleInPCAL && !(allParticles[neutrons_FD_ECALveto_LnFD.pindex]->cal(clas12::PCAL)->getDetector() == 7)) {
                //     cout << "\n\nError! Leading FD neutron is not really in the PCAL (ECALveto)! Aborting...\n\n", exit(1);
                // }

                // if (ParticleInECIN && !(allParticles[neutrons_FD_ECALveto_LnFD.pindex]->cal(clas12::ECIN)->getDetector() == 7)) {
                //     cout << "\n\nError! Leading FD neutron is not really in the ECIN (ECALveto)! Aborting...\n\n", exit(1);
                // }

                // if (ParticleInECOUT && !(allParticles[neutrons_FD_ECALveto_LnFD.pindex]->cal(clas12::ECOUT)->getDetector() == 7)) {
                //     cout << "\n\nError! Leading FD neutron is not really in the ECOUT (ECALveto)! Aborting...\n\n", exit(1);
                // }

                // NumberOfLnFD_hits_in_event_ECALveto++;
                // if (ParticleInPCAL) { NumberOfLnFD_PCAL_hits_in_event_ECALveto++; }
                // if (ParticleInECIN) { NumberOfLnFD_ECIN_hits_in_event_ECALveto++; }
                // if (ParticleInECOUT) { NumberOfLnFD_ECOUT_hits_in_event_ECALveto++; }

                // bool LnFD_pass_dSector_cuts = true;
                // int LnFD_nSector = neutrons_FD_ECALveto_LnFD.sector;
                // int e_nSector = electrons[0]->cal(neutrons_FD_ECALveto_LnFD.First_ECAL_clas12detlayer)->getSector();  // Get electron sector at the same ECAL layer as FD neutron
                // if (OnlyGood_nFD) { LnFD_pass_dSector_cuts = (abs(LnFD_nSector - e_nSector) == 3); }
                // if (OnlyBad_nFD) { LnFD_pass_dSector_cuts = !(abs(LnFD_nSector - e_nSector) == 3); }
                // bool Same_e_LnFD_Sector_ECALveto = false;

                if (OnlyGood_nFD && !LnFD_pass_dSector_cuts) { continue; }
                if (OnlyBad_nFD && !LnFD_pass_dSector_cuts) { continue; }

                if (apply_no_same_sector_cuts && (neutrons_FD_ECALveto_LnFD->cal(detlayer)->getSector() != electrons[0]->cal(Electron_ECAL_detlayer)->getSector())) { continue; }

                // if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_ECALveto is in the PCAL! Aborting...\n\n", exit(1); }

                // auto [reco_ToF_LnFD, reco_Beta_LnFD, reco_Gamma_LnFD, reco_P_LnFD] = Calc_nFD_vars(neutrons_FD_ECALveto_LnFD, starttime, apply_neutFD_redef);
                // double Path_LnFD = neutrons_FD_ECALveto_LnFD.HitPath.Mag();
                // // double Path_LnFD = am::RadCalc(neutrons_FD_ECALveto_LnFD.xHit, neutrons_FD_ECALveto_LnFD.yHit, neutrons_FD_ECALveto_LnFD.zHit);
                // // double reco_ToF_LnFD = neutrons_FD_ECALveto_LnFD.HitTime;
                // // double reco_Beta_LnFD = Path_LnFD / (reco_ToF_LnFD * c);
                // double Edep_ECAL_LnFD = 0;

                // int PCAL_hit = ParticleInPCAL ? 1 : 0;
                // int ECIN_hit = ParticleInECIN ? 1 : 0;
                // int ECOUT_hit = ParticleInECOUT ? 1 : 0;

                // // TVector3 reco_P_LnFD;
                // // reco_P_LnFD.SetMagThetaPhi(am::RadCalc(neutrons_FD_ECALveto_LnFD.Px, neutrons_FD_ECALveto_LnFD.Py, neutrons_FD_ECALveto_LnFD.Pz),
                // //                            allParticles[neutrons_FD_ECALveto_LnFD.pindex]->getTheta(), allParticles[neutrons_FD_ECALveto_LnFD.pindex]->getPhi());
                // // // am::CalcTheta_rad(neutrons_FD_ECALveto_LnFD.xHit - electrons[0]->par()->getVx(), neutrons_FD_ECALveto_LnFD.yHit - electrons[0]->par()->getVy(),
                // // //                              neutrons_FD_ECALveto_LnFD.zHit - electrons[0]->par()->getVz()),
                // // // am::CalcPhi_rad(neutrons_FD_ECALveto_LnFD.xHit - electrons[0]->par()->getVx(), neutrons_FD_ECALveto_LnFD.yHit - electrons[0]->par()->getVy()));

                // // TODO: make this work somehow!
                // // // Safety check to ensure that Reco_P_LnFD_mag_ECALveto is indeed the largest momentum neutron:
                // // for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
                // //     if (am::RadCalc(neutrons_FD_ECALveto[i].Px, neutrons_FD_ECALveto[i].Py, neutrons_FD_ECALveto[i].Pz) > reco_P_LnFD.Mag() &&
                // //         (neutrons_FD_ECALveto_ind.at(i) != neutrons_FD_ECALveto_LnFD.pindex)) {
                // //         cout << "\n\nError! Reco_P_LnFD_mag_ECALveto is not the largest momentum neutron!\n";
                // //         cout << "Reco_P_LnFD_mag_ECALveto = " << Reco_P_LnFD_mag_ECALveto << "\n";
                // //         cout << "neutrons_FD_ECALveto_ind.at(i) = " << neutrons_FD_ECALveto_ind.at(i) << "\n";
                // //         cout << "reco_P_LnFD.Mag() = " << reco_P_LnFD.Mag() << "\n";
                // //         cout << "Reco_P_LnFD_ind_ECALveto = " << Reco_P_LnFD_ind_ECALveto << "\n";
                // //         cout << "Aborting...\n\n", exit(1);
                // //     }
                // // }

                // double dTheta = (reco_P_LnFD.Theta() - reco_P_e.Theta()) * 180 / M_PI;
                // double dPhi = am::CalcdPhi1((reco_P_LnFD.Phi() - reco_P_e.Phi()) * 180 / M_PI);

                // bool DoesNotPass_theta_nFD_e_vs_dTheta_cuts =
                //     (apply_theta_nFD_e_vs_dTheta_cuts &&
                //      (Same_e_LnFD_Sector_ECALveto && (((dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetLowerCut()) && ((dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetUpperCut()))));

                if (ECALveto_variables_LnFD.DoesNotPass_theta_nFD_e_vs_dTheta_cuts) { continue; }

                ECALveto_histograms.NSB_LnFD_histograms.FillRecoNeutronKinematics(neutrons_FD_ECALveto_LnFD, ECALveto_variables_LnFD, weight);
                // h_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Mag(), weight);
                // h_reco_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Theta() * 180 / M_PI, weight);
                // h_reco_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Phi() * 180 / M_PI, weight);
                // h_reco_theta_LnFD_ECALveto_VS_reco_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Phi() * 180 / M_PI, reco_P_LnFD.Theta() * 180 / M_PI, weight);

                // h_Edep_ECAL_LnFD_ECALveto_1e_cut->Fill(Edep_ECAL_LnFD, weight);
                // h_beta_LnFD_ECALveto_1e_cut->Fill(reco_Beta_LnFD, weight);
                // h_Edep_ECAL_LnFD_VS_beta_LnFD_ECALveto_1e_cut->Fill(reco_Beta_LnFD, Edep_ECAL_LnFD, weight);

                // h_nFD_PCAL_hits_VS_nFD_ECIN_hits_ECALveto_1e_cut->Fill(PCAL_hit, ECIN_hit, weight);
                // h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut->Fill(PCAL_hit, ECOUT_hit, weight);
                // h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut->Fill(ECIN_hit, ECOUT_hit, weight);

                // h_reco_P_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_LnFD.Mag(), weight);
                // h_reco_theta_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_LnFD.Mag(), weight);
                // h_reco_phi_e_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_LnFD.Mag(), weight);
                // h_reco_P_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_LnFD.Theta() * 180 / M_PI, weight);
                // h_reco_theta_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_LnFD.Theta() * 180 / M_PI, weight);
                // h_reco_phi_e_VS_theta_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_LnFD.Theta() * 180 / M_PI, weight);
                // h_reco_P_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_LnFD.Phi() * 180 / M_PI, weight);
                // h_reco_theta_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_LnFD.Phi() * 180 / M_PI, weight);
                // h_reco_phi_e_VS_phi_LnFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_LnFD.Phi() * 180 / M_PI, weight);
                // h_reco_theta_LnFD_ECALveto_VS_P_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Theta() * 180 / M_PI, reco_P_LnFD.Mag(), weight);

                ECALveto_histograms.NSA_LnFD_histograms.Fill(neutrons_FD_ECALveto_LnFD, ECALveto_variables_LnFD, weight);
                // h_reco_dTheta_LnFD_e_ECALveto_1e_cut->Fill(dTheta, weight);
                // h_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, weight);
                // h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                // h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Mag(), weight);
                // h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Mag(), weight);
                // h_reco_P_e_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                // // h_LnFD_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                // // h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                // h_e_status_VS_reco_dTheta_LnFD_e_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                // h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                if (neutrons_FD_ECALveto_LnFD.sector == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
                    bool Pass_dTheta_window = (ECALveto_variables_LnFD.dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetLowerCut() &&
                                              (ECALveto_variables_LnFD.dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetUpperCut();
                    ECALveto_histograms.NSSS_LnFD_histograms.Fill(ECALveto_variables_LnFD, Pass_dTheta_window, weight);
                    // h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, weight);
                    // h_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut->Fill(dTheta, weight);
                    // h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dPhi, weight);
                    // h_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, weight);
                    // h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    // h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);

                    // h_reco_P_e_VS_reco_dPhi_LnFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                    // // h_LnFD_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    // // h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    // h_e_status_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    // h_e_status_VS_reco_dTheta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                    // h_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, weight);

                    // h_reco_theta_LnFD_e_VS_reco_dTheta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, weight);
                    // h_reco_theta_LnFD_e_VS_reco_dPhi_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, weight);
                    // h_reco_theta_LnFD_e_VS_reco_P_LnFD_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Mag(), reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, weight);

                    // // h_LnFD_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, neutrons_FD_ECALveto_LnFD->par()->getStatus(),
                    // //                                                                             weight);
                    // // h_LnFD_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI,
                    // //                                                                                    neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    // h_e_status_VS_reco_theta_LnFD_e_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                    // h_e_status_VS_reco_theta_LnFD_e_zoomin_Same_e_LnFD_Sector_ECALveto_1e_cut->Fill(reco_P_LnFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                }

                if (ParticleInPCAL) {
                    h_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dTheta, weight);
                    h_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dPhi, weight);
                    h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Mag(), weight);
                    h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Mag(), weight);
                    // h_LnFD_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    // h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_PCAL_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_zoomin_PCAL_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                }

                if (ParticleInECIN) {
                    h_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dTheta, weight);
                    h_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dPhi, weight);
                    h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Mag(), weight);
                    h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Mag(), weight);
                    // h_LnFD_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    // h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_ECIN_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECIN_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                }

                if (ParticleInECOUT) {
                    h_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, weight);
                    h_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dPhi, weight);
                    h_reco_dTheta_LnFD_e_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    h_reco_P_LnFD_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, reco_P_LnFD.Mag(), weight);
                    h_reco_P_LnFD_VS_reco_dPhi_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dPhi, reco_P_LnFD.Mag(), weight);
                    // h_LnFD_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    // h_LnFD_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto_LnFD->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_LnFD_e_zoomin_ECOUT_only_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                }

                TVector3 v_nhit_ECALveto(neutrons_FD_ECALveto_LnFD.xHit, neutrons_FD_ECALveto_LnFD.yHit, neutrons_FD_ECALveto_LnFD.zHit);
                TVector3 shifted_e_unit_v_ECALveto = Get_shifted_e_unit_v_NeutVeto(electrons, v_nhit_ECALveto, B, L);
                TVector3 v_dist_ECALveto = v_nhit_ECALveto - shifted_e_unit_v_ECALveto;

                for (int j = 0; j < allParticles.size(); j++) {
                    if (allParticles[j]->par()->getCharge() == 0) {
                        bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                        TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                        if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                            v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
                            TVector3 v_dist = v_nhit_ECALveto - v_neutral_hit;

                            ECALveto_histograms.MSM_LnFD_histograms.Fill(v_dist, ECALveto_variables_LnFD, weight);
                            // h_v_dist_LnFD_ECALveto_1e_cut->Fill(v_dist.Mag(), weight);
                            // h_v_dist_LnFD_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_LnFD.Mag(), weight);
                            // h_v_dist_LnFD_VS_reco_P_e_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                            // h_v_dist_LnFD_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(v_dist.Mag(), dPhi, weight);
                        }
                    }
                }

                // h_Edep_ECAL_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Mag(), Edep_ECAL_LnFD, weight);
                // h_Edep_ECAL_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, Edep_ECAL_LnFD, weight);
                // h_beta_n_VS_reco_P_LnFD_ECALveto_1e_cut->Fill(reco_P_LnFD.Mag(), reco_Beta_LnFD, weight);
                // h_beta_n_VS_reco_dPhi_LnFD_e_ECALveto_1e_cut->Fill(dPhi, reco_Beta_LnFD, weight);

                if (plot_and_fit_MomRes) {
                    int Neutron_match_counter = 0;
                    double RecoNeutronP_Debug = reco_P_LnFD.Mag();
                    double RecoNeutronTheta_Debug = reco_P_LnFD.Theta() * 180.0 / pi;
                    double RecoNeutronPhi_Debug = reco_P_LnFD.Phi() * 180.0 / pi;

                    for (Int_t i = 0; i < Ngen; i++) {
                        mcpbank->setEntry(i);

                        /* TL neutron kinematic variables */
                        double TLNeutronP = am::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
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
                NeutronSelection_variables ECALveto_variables_nFD(allParticles, electrons[0], neutrons_FD_ECALveto[i], fdNeutralHelpers, reco_P_e, NumberOfnFD_hits_in_event_ECALveto,
                                                                  NumberOfnFD_PCAL_hits_in_event_ECALveto, NumberOfnFD_ECIN_hits_in_event_ECALveto, NumberOfnFD_ECOUT_hits_in_event_ECALveto);

                // bool ParticleInPCAL = neutrons_FD_ECALveto[i].clas12particle_hit_in_PCAL;
                // bool ParticleInECIN = neutrons_FD_ECALveto[i].clas12particle_hit_in_ECIN;
                // bool ParticleInECOUT = neutrons_FD_ECALveto[i].clas12particle_hit_in_ECOUT;

                // if (ParticleInPCAL && !(allParticles[neutrons_FD_ECALveto[i].pindex]->cal(clas12::PCAL)->getDetector() == 7)) {
                //     cout << "\n\nError! FD neutron is not really in the PCAL (ECALveto)! Aborting...\n\n", exit(1);
                // }

                // if (ParticleInECIN && !(allParticles[neutrons_FD_ECALveto[i].pindex]->cal(clas12::ECIN)->getDetector() == 7)) {
                //     cout << "\n\nError! FD neutron is not really in the ECIN (ECALveto)! Aborting...\n\n", exit(1);
                // }

                // if (ParticleInECOUT && !(allParticles[neutrons_FD_ECALveto[i].pindex]->cal(clas12::ECOUT)->getDetector() == 7)) {
                //     cout << "\n\nError! FD neutron is not really in the ECOUT (ECALveto)! Aborting...\n\n", exit(1);
                // }

                // NumberOfnFD_hits_in_event_ECALveto++;
                // if (ParticleInPCAL) { NumberOfnFD_PCAL_hits_in_event_ECALveto++; }
                // if (ParticleInECIN) { NumberOfnFD_ECIN_hits_in_event_ECALveto++; }
                // if (ParticleInECOUT) { NumberOfnFD_ECOUT_hits_in_event_ECALveto++; }

                // bool nFD_pass_dSector_cuts = true;
                // int nFD_nSector = neutrons_FD_ECALveto[i].sector;
                // int e_nSector = electrons[0]->cal(neutrons_FD_ECALveto[i].First_ECAL_clas12detlayer)->getSector();  // Get electron sector at the same ECAL layer as FD neutron
                // if (OnlyGood_nFD) { nFD_pass_dSector_cuts = (abs(nFD_nSector - e_nSector) == 3); }
                // if (OnlyBad_nFD) { nFD_pass_dSector_cuts = !(abs(nFD_nSector - e_nSector) == 3); }
                // bool Same_e_nFD_Sector_ECALveto = false;

                if (ECALveto_variables_nFD.OnlyGood_nFD && !ECALveto_variables_nFD.nFD_pass_dSector_cuts) { continue; }
                if (ECALveto_variables_nFD.OnlyBad_nFD && !ECALveto_variables_nFD.nFD_pass_dSector_cuts) { continue; }

                if (apply_no_same_sector_cuts && (neutrons_FD_ECALveto[i]->cal(detlayer)->getSector() != electrons[0]->cal(Electron_ECAL_detlayer)->getSector())) { continue; }

                // // if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_ECALveto is in the PCAL! Aborting...\n\n", exit(1); }

                // auto [reco_ToF_nFD, reco_Beta_nFD, reco_Gamma_nFD, reco_P_nFD] = Calc_nFD_vars(neutrons_FD_ECALveto[i], starttime, apply_neutFD_redef);
                // double Path_nFD = neutrons_FD_ECALveto[i].HitPath.Mag();
                // // double Path_nFD = am::RadCalc(neutrons_FD_ECALveto[i].xHit, neutrons_FD_ECALveto[i].yHit, neutrons_FD_ECALveto[i].zHit);
                // // double reco_ToF_nFD = neutrons_FD_ECALveto[i].HitTime;
                // // double reco_Beta_nFD = Path_nFD / (reco_ToF_nFD * c);
                // double Edep_ECAL_nFD = 0;

                // int PCAL_hit = ParticleInPCAL ? 1 : 0;
                // int ECIN_hit = ParticleInECIN ? 1 : 0;
                // int ECOUT_hit = ParticleInECOUT ? 1 : 0;

                // auto [PCAL_hit_test, ECIN_hit_test, ECOUT_hit_test, ECAL_detlayer_test] = raf::CheckForECALHits(allParticles[neutrons_FD_ECALveto[i].pindex]);
                // if (PCAL_hit_test != PCAL_hit) { cout << "\n\nError: PCAL_hit_test != PCAL_hit, aborting...", exit(1); }
                // if (ECIN_hit_test != ECIN_hit) { cout << "\n\nError: ECIN_hit_test != ECIN_hit, aborting...", exit(1); }
                // if (ECOUT_hit_test != ECOUT_hit) { cout << "\n\nError: ECOUT_hit_test != ECOUT_hit, aborting...", exit(1); }

                // // TVector3 reco_P_nFD;
                // // reco_P_nFD.SetMagThetaPhi(am::RadCalc(neutrons_FD_ECALveto[i].Px, neutrons_FD_ECALveto[i].Py, neutrons_FD_ECALveto[i].Pz),
                // //                           allParticles[neutrons_FD_ECALveto[i].pindex]->getTheta(), allParticles[neutrons_FD_ECALveto[i].pindex]->getPhi());
                // // //   am::CalcTheta_rad(neutrons_FD_ECALveto[i].xHit - electrons[0]->par()->getVx(), neutrons_FD_ECALveto[i].yHit - electrons[0]->par()->getVy(),
                // // //                                neutrons_FD_ECALveto[i].zHit - electrons[0]->par()->getVz()),
                // // //   am::CalcPhi_rad(neutrons_FD_ECALveto[i].xHit - electrons[0]->par()->getVx(), neutrons_FD_ECALveto[i].yHit - electrons[0]->par()->getVy()));

                // double dTheta = (reco_P_nFD.Theta() - reco_P_e.Theta()) * 180 / M_PI;
                // double dPhi = am::CalcdPhi1((reco_P_nFD.Phi() - reco_P_e.Phi()) * 180 / M_PI);

                // bool DoesNotPass_theta_nFD_e_vs_dTheta_cuts =
                //     (apply_theta_nFD_e_vs_dTheta_cuts &&
                //      (Same_e_nFD_Sector_ECALveto && (((dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetLowerCut()) && ((dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetUpperCut()))));

                if (ECALveto_variables_nFD.DoesNotPass_theta_nFD_e_vs_dTheta_cuts) { continue; }

                // bool Pass_dTheta_status_ECALveto = true;
                // bool Pass_dPhi_status_ECALveto = true;

                if (ECALveto_variables_nFD.Pass_dTheta_status_ECALveto && ECALveto_variables_nFD.Pass_dPhi_status_ECALveto) {
                    ECALveto_histograms.NSB_nFD_histograms.FillRecoElectroKinematics(reco_P_e, weight);
                    // h_reco_P_e_ECALveto_1e_cut->Fill(reco_P_e.Mag(), weight);
                    // h_reco_theta_e_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
                    // h_reco_phi_e_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
                    // h_reco_theta_e_ECALveto_VS_reco_phi_e_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);

                    ECALveto_histograms.NSB_nFD_histograms.FillRecoNeutronKinematics(neutrons_FD_ECALveto[i], ECALveto_variables_nFD, weight);
                    // h_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight);

                    // if (ParticleInPCAL) { h_reco_P_nFD_bool_PCAL_test_only_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight); }
                    // if (ParticleInECIN) { h_reco_P_nFD_bool_ECIN_test_only_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight); }
                    // if (ParticleInECOUT) { h_reco_P_nFD_bool_ECOUT_test_only_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight); }

                    // if (neutrons_FD_ECALveto[i].particle_clas12detlayer == clas12::PCAL) { h_reco_P_nFD_PCAL_only_clas12detlayer_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight); }
                    // if (neutrons_FD_ECALveto[i].particle_clas12detlayer == clas12::ECIN) { h_reco_P_nFD_ECIN_only_clas12detlayer_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight); }
                    // if (neutrons_FD_ECALveto[i].particle_clas12detlayer == clas12::ECOUT) { h_reco_P_nFD_ECOUT_only_clas12detlayer_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), weight); }

                    // h_reco_theta_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, weight);
                    // h_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, weight);
                    // h_reco_theta_nFD_ECALveto_VS_reco_phi_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);

                    // h_Edep_ECAL_nFD_ECALveto_1e_cut->Fill(Edep_ECAL_nFD, weight);
                    // h_beta_nFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, weight);
                    // h_Edep_ECAL_nFD_VS_beta_nFD_ECALveto_1e_cut->Fill(reco_Beta_nFD, Edep_ECAL_nFD, weight);

                    // h_nFD_PCAL_hits_VS_nFD_ECIN_hits_ECALveto_1e_cut->Fill(PCAL_hit, ECIN_hit, weight);
                    // h_nFD_PCAL_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut->Fill(PCAL_hit, ECOUT_hit, weight);
                    // h_nFD_ECIN_hits_VS_nFD_ECOUT_hits_ECALveto_1e_cut->Fill(ECIN_hit, ECOUT_hit, weight);

                    // h_reco_P_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Mag(), weight);
                    // h_reco_theta_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                    // h_reco_phi_e_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                    // h_reco_P_e_VS_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Theta() * 180 / M_PI, weight);
                    // h_reco_theta_e_VS_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    // h_reco_phi_e_VS_theta_nFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Theta() * 180 / M_PI, weight);
                    // h_reco_P_e_VS_phi_nFD_ECALveto_1e_cut->Fill(reco_P_e.Mag(), reco_P_nFD.Phi() * 180 / M_PI, weight);
                    // h_reco_theta_e_VS_phi_nFD_ECALveto_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    // h_reco_phi_e_VS_phi_nFD_ECALveto_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_nFD.Phi() * 180 / M_PI, weight);
                    // h_reco_theta_nFD_ECALveto_VS_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Theta() * 180 / M_PI, reco_P_nFD.Mag(), weight);
                    // h_reco_nFD_multi_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), neutrons_FD_ECALveto.size(), weight);

                    ECALveto_histograms.NSA_nFD_histograms.Fill(neutrons_FD_ECALveto[i], ECALveto_variables_nFD, weight);
                    // h_reco_dTheta_nFD_e_ECALveto_1e_cut->Fill(dTheta, weight);
                    // h_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, weight);
                    // h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                    // h_reco_P_nFD_VS_reco_dTheta_nFD_e_ECALveto_1e_cut->Fill(dTheta, reco_P_nFD.Mag(), weight);
                    // h_reco_P_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, reco_P_nFD.Mag(), weight);
                    // h_reco_P_e_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                    // // h_nFD_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                    // // h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                    // h_e_status_VS_reco_dTheta_nFD_e_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    // h_e_status_VS_reco_dTheta_nFD_e_zoomin_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                    if (neutrons_FD_ECALveto[i].sector == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
                        bool Pass_dTheta_window = (ECALveto_variables_nFD.dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetLowerCut() &&
                                                  (ECALveto_variables_nFD.dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetUpperCut();
                        ECALveto_histograms.NSSS_nFD_histograms.Fill(ECALveto_variables_nFD, Pass_dTheta_window, weight);
                        // h_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, weight);
                        // h_reco_dTheta_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(dTheta, weight);
                        // h_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dPhi, weight);
                        // h_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, weight);

                        // if ((dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetLowerCut() && (dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetUpperCut()) {
                        //     h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_ECALveto_1e_cut->Fill(dPhi, weight);
                        //     h_reco_dPhi_nFD_e_Same_e_nFD_Sector_for_dTheta_cuts_zoomin_ECALveto_1e_cut->Fill(dPhi, weight);
                        // }

                        // h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);
                        // h_reco_dTheta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, dTheta, weight);

                        // h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                        // h_reco_P_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(reco_P_e.Mag(), dPhi, weight);
                        // // h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                        // // h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, neutrons_FD_ECALveto[i]->par()->getStatus(), weight);
                        // h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                        // h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                        // h_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);

                        // h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dTheta, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        // h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(dPhi, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        // h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_ECALveto_1e_cut->Fill(dPhi, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        // h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);

                        // // h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, neutrons_FD_ECALveto[i]->par()->getStatus(),
                        // //                                                                          weight);
                        // // h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI,
                        // // neutrons_FD_ECALveto[i]->par()->getStatus(),
                        // //                                                                                 weight);
                        // h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                        // h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_ECALveto_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                    }

                    // TVector3 v_nhit_ECALveto(neutrons_FD_ECALveto[i].xHit, neutrons_FD_ECALveto[i].yHit, neutrons_FD_ECALveto[i].zHit);
                    // TVector3 shifted_e_unit_v_ECALveto = Get_shifted_e_unit_v_NeutVeto(electrons, v_nhit_ECALveto, B, L);
                    // TVector3 v_dist_ECALveto = v_nhit_ECALveto - shifted_e_unit_v_ECALveto;

                    // h_dr_n_ECALveto_1e_cut->Fill(v_dist_ECALveto.Mag(), weight);
                    // h_dr_n_VS_dTheta_n_e_ECALveto_1e_cut->Fill(v_dist_ECALveto.Mag(), dTheta, weight);
                    // h_dr_n_VS_dPhi_n_e_ECALveto_1e_cut->Fill(v_dist_ECALveto.Mag(), dPhi, weight);

                    // h_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n_ECALveto_1e_cut.Fill(v_dist_ECALveto.Mag(), dPhi, dTheta, weight);

                    // h_dTheta_n_V_VS_dPhi_n_V_Electrons_AcEV_ECALveto_1e_cut->Fill(v_dist_ECALveto.Phi() * 180 / M_PI, v_dist_ECALveto.Theta() * 180 / M_PI, weight);

                    for (int j = 0; j < allParticles.size(); j++) {
                        if (allParticles[j]->par()->getCharge() == 0) {
                            bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                            TVector3 v_neutral_hit;  // v_neutral_hit = location of neutral particle hit

                            if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                                v_neutral_hit.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
                                TVector3 v_dist = v_nhit_ECALveto - v_neutral_hit;

                                ECALveto_histograms.MSM_nFD_histograms.Fill(v_dist, ECALveto_variables_nFD, weight);
                                // h_v_dist_nFD_ECALveto_1e_cut->Fill(v_dist.Mag(), weight);
                                // h_v_dist_nFD_VS_reco_P_nFD_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_nFD.Mag(), weight);
                                // h_v_dist_nFD_VS_reco_P_e_ECALveto_1e_cut->Fill(v_dist.Mag(), reco_P_e.Mag(), weight);
                                // h_v_dist_nFD_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(v_dist.Mag(), dPhi, weight);
                            }
                        }
                    }

                    // h_Edep_ECAL_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), Edep_ECAL_nFD, weight);
                    // h_Edep_ECAL_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, Edep_ECAL_nFD, weight);
                    // h_beta_n_VS_reco_P_nFD_ECALveto_1e_cut->Fill(reco_P_nFD.Mag(), reco_Beta_nFD, weight);
                    // h_beta_n_VS_reco_dPhi_nFD_e_ECALveto_1e_cut->Fill(dPhi, reco_Beta_nFD, weight);

                    // // Fake FD neutrons handling (neutron veto)
                    // TVector3 nFD_hit_3V, e_hit_3V;

                    // double nFD_hit_Phi, nFD_hit_Theta, e_hit_Phi, e_hit_Theta, dPhi_hit, dTheta_hit;

                    // // if (!ParticleInPCAL && (ParticleInECIN || ParticleInECOUT)) {
                    // //     // if neutron did not hit PCAL, and hit either ECIN or ECOUT

                    // //     // neutron ECIN/ECAL hit vector and angles:
                    // //     nFD_hit_3V.SetXYZ(neutrons_FD_ECALveto[i].xHit, neutrons_FD_ECALveto[i].yHit, neutrons_FD_ECALveto[i].zHit);
                    // //     nFD_hit_Theta = nFD_hit_3V.Theta() * 180 / pi, nFD_hit_Phi = nFD_hit_3V.Phi() * 180 / pi;

                    // //     if ((detlayer == clas12::ECIN) && (electrons[0]->cal(clas12::ECIN)->getZ() != 0)) {
                    // //         e_hit_3V.SetXYZ(electrons[0]->cal(clas12::ECIN)->getX(), electrons[0]->cal(clas12::ECIN)->getY(), electrons[0]->cal(clas12::ECIN)->getZ());
                    // //         e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                    // //     } else if ((detlayer == clas12::ECOUT) && (electrons[0]->cal(clas12::ECOUT)->getZ() != 0)) {
                    // //         e_hit_3V.SetXYZ(electrons[0]->cal(clas12::ECOUT)->getX(), electrons[0]->cal(clas12::ECOUT)->getY(), electrons[0]->cal(clas12::ECOUT)->getZ());
                    // //         e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                    // //     } else {
                    // //         int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
                    // //         e_hit_3V.SetXYZ(electrons[0]->traj(clas12::ECAL, trajlayer)->getX(), electrons[0]->traj(clas12::ECAL, trajlayer)->getY(),
                    // //                         electrons[0]->traj(clas12::ECAL, trajlayer)->getZ());
                    // //         e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                    // //     }

                    // //     dPhi_hit = am::CalcdPhi1(nFD_hit_Phi - e_hit_Phi), dTheta_hit = nFD_hit_Theta - e_hit_Theta;
                    // // }  // end of if neutron did not hit PCAL & hit either ECIN or ECOUT

                    // // bool NeutronPassVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);

                    // h_dTheta_n_e_VS_dPhi_n_e_Electrons_AcEV_ECALveto_1e_cut->Fill(dPhi_hit, dTheta_hit, weight);
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

        std::vector<NeutralHit> neutrons_FD_matched;
        // std::vector<region_part_ptr> neutrons_FD_matched;
        std::vector<NeutralHit> neutrons_FD_matched_OnlyGood;
        // std::vector<region_part_ptr> neutrons_FD_matched_OnlyGood;
        std::vector<NeutralHit> neutrons_FD_matched_OnlyBad;
        // std::vector<region_part_ptr> neutrons_FD_matched_OnlyBad;
        NeutralHit neutrons_FD_matched_LnFD;
        std::vector<region_part_ptr> photons_FD_matched;
        std::vector<int> neutrons_FD_matched_ind;

        // Vectors for multiplicity plots
        std::vector<NeutralHit> neutrons_FD_PassMatch;
        std::vector<NeutralHit> neutrons_FD_PassMatch_OnlyGood;
        std::vector<NeutralHit> neutrons_FD_PassMatch_OnlyBad;
        std::vector<NeutralHit> neutrons_FD_NotPassMatch;
        std::vector<NeutralHit> neutrons_FD_NotPassMatch_OnlyGood;
        std::vector<NeutralHit> neutrons_FD_NotPassMatch_OnlyBad;

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
            if (Reco_P_LnFD_ind_matched != -1) {
                bool ParticleInPCAL = neutrons_FD_matched_LnFD.clas12particle_hit_in_PCAL;
                bool ParticleInECIN = neutrons_FD_matched_LnFD.clas12particle_hit_in_ECIN;
                bool ParticleInECOUT = neutrons_FD_matched_LnFD.clas12particle_hit_in_ECOUT;

                if (ParticleInPCAL && !(allParticles[neutrons_FD_matched_LnFD.pindex]->cal(clas12::PCAL)->getDetector() == 7)) {
                    cout << "\n\nError! Leading FD neutron is not really in the PCAL (matched)! Aborting...\n\n", exit(1);
                }

                if (ParticleInECIN && !(allParticles[neutrons_FD_matched_LnFD.pindex]->cal(clas12::ECIN)->getDetector() == 7)) {
                    cout << "\n\nError! Leading FD neutron is not really in the ECIN (matched)! Aborting...\n\n", exit(1);
                }

                if (ParticleInECOUT && !(allParticles[neutrons_FD_matched_LnFD.pindex]->cal(clas12::ECOUT)->getDetector() == 7)) {
                    cout << "\n\nError! Leading FD neutron is not really in the ECOUT (matched)! Aborting...\n\n", exit(1);
                }

                NumberOfLnFD_hits_in_event_matched++;
                if (ParticleInPCAL) { NumberOfLnFD_PCAL_hits_in_event_matched++; }
                if (ParticleInECIN) { NumberOfLnFD_ECIN_hits_in_event_matched++; }
                if (ParticleInECOUT) { NumberOfLnFD_ECOUT_hits_in_event_matched++; }

                for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                    mcpbank->setEntry(truth_NeutronsFD.at(j));

                    auto [reco_ToF_LnFD, reco_Beta_LnFD, reco_Gamma_LnFD, reco_P_LnFD] = Calc_nFD_vars(neutrons_FD_matched_LnFD, starttime, apply_neutFD_redef);

                    // TVector3 reco_P_LnFD;
                    // reco_P_LnFD.SetMagThetaPhi(am::RadCalc(neutrons_FD_matched_LnFD.Px, neutrons_FD_matched_LnFD.Py, neutrons_FD_matched_LnFD.Pz),
                    //                            allParticles[neutrons_FD_matched_LnFD.pindex]->getTheta(), allParticles[neutrons_FD_matched_LnFD.pindex]->getPhi());
                    // // am::CalcTheta_rad(neutrons_FD_matched_LnFD.xHit - electrons[0]->par()->getVx(), neutrons_FD_matched_LnFD.yHit - electrons[0]->par()->getVy(),
                    // //                              neutrons_FD_matched_LnFD.zHit - electrons[0]->par()->getVz()),
                    // // am::CalcPhi_rad(neutrons_FD_matched_LnFD.xHit - electrons[0]->par()->getVx(), neutrons_FD_matched_LnFD.yHit - electrons[0]->par()->getVy()));

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
                bool ParticleInPCAL = neutrons_FD_matched[i].clas12particle_hit_in_PCAL;
                bool ParticleInECIN = neutrons_FD_matched[i].clas12particle_hit_in_ECIN;
                bool ParticleInECOUT = neutrons_FD_matched[i].clas12particle_hit_in_ECOUT;

                bool nFD_pass_dSector_cuts = true;
                int nFD_nSector = neutrons_FD_matched[i].sector;
                int e_nSector = electrons[0]->cal(neutrons_FD_matched[i].First_ECAL_clas12detlayer)->getSector();  // Get electron sector at the same ECAL layer as FD neutron
                if (OnlyGood_nFD) { nFD_pass_dSector_cuts = (abs(nFD_nSector - e_nSector) == 3); }
                if (OnlyBad_nFD) { nFD_pass_dSector_cuts = !(abs(nFD_nSector - e_nSector) == 3); }
                bool Same_e_nFD_Sector_matched = false;

                if (OnlyGood_nFD && !nFD_pass_dSector_cuts) { continue; }
                if (OnlyBad_nFD && !nFD_pass_dSector_cuts) { continue; }

                // if (apply_no_same_sector_cuts && (neutrons_FD_matched[i]->cal(detlayer)->getSector() != electrons[0]->cal(Electron_ECAL_detlayer)->getSector())) { continue; }

                // if (apply_neutFD_redef && ParticleInPCAL) { cout << "\n\nError! neutrons_FD_matched is in the PCAL! Aborting...\n\n", exit(1); }

                auto [reco_ToF_nFD, reco_Beta_nFD, reco_Gamma_nFD, reco_P_nFD] = Calc_nFD_vars(neutrons_FD_matched[i], starttime, apply_neutFD_redef);
                double Path_nFD = neutrons_FD_matched[i].HitPath.Mag();
                // double Path_nFD = am::RadCalc(neutrons_FD_matched[i].xHit, neutrons_FD_matched[i].yHit, neutrons_FD_matched[i].zHit);
                // double reco_ToF_nFD = neutrons_FD_matched[i].HitTime;
                // double reco_Beta_nFD = Path_nFD / (reco_ToF_nFD * c);
                double Edep_ECAL_nFD = 0;

                int PCAL_hit = ParticleInPCAL ? 1 : 0;
                int ECIN_hit = ParticleInECIN ? 1 : 0;
                int ECOUT_hit = ParticleInECOUT ? 1 : 0;

                // TVector3 reco_P_nFD;
                // reco_P_nFD.SetMagThetaPhi(am::RadCalc(neutrons_FD_matched[i].Px, neutrons_FD_matched[i].Py, neutrons_FD_matched[i].Pz),
                //                           allParticles[neutrons_FD_matched[i].pindex]->getTheta(), allParticles[neutrons_FD_matched[i].pindex]->getPhi());
                // //   am::CalcTheta_rad(neutrons_FD_matched[i].xHit - electrons[0]->par()->getVx(), neutrons_FD_matched[i].yHit - electrons[0]->par()->getVy(),
                // //                                neutrons_FD_matched[i].zHit - electrons[0]->par()->getVz()),
                // //   am::CalcPhi_rad(neutrons_FD_matched[i].xHit - electrons[0]->par()->getVx(), neutrons_FD_matched[i].yHit - electrons[0]->par()->getVy()));

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
                    if (ParticleInPCAL && !(allParticles[neutrons_FD_matched[i].pindex]->cal(clas12::PCAL)->getDetector() == 7)) {
                        cout << "\n\nError! FD neutron is not really in the PCAL (matched)! Aborting...\n\n", exit(1);
                    }

                    if (ParticleInECIN && !(allParticles[neutrons_FD_matched[i].pindex]->cal(clas12::ECIN)->getDetector() == 7)) {
                        cout << "\n\nError! FD neutron is not really in the ECIN (matched)! Aborting...\n\n", exit(1);
                    }

                    if (ParticleInECOUT && !(allParticles[neutrons_FD_matched[i].pindex]->cal(clas12::ECOUT)->getDetector() == 7)) {
                        cout << "\n\nError! FD neutron is not really in the ECOUT (matched)! Aborting...\n\n", exit(1);
                    }

                    NumberOfnFD_hits_in_event_matched++;
                    if (ParticleInPCAL) { NumberOfnFD_PCAL_hits_in_event_matched++; }
                    if (ParticleInECIN) { NumberOfnFD_ECIN_hits_in_event_matched++; }
                    if (ParticleInECOUT) { NumberOfnFD_ECOUT_hits_in_event_matched++; }

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

                    int nFD_nSector = neutrons_FD_matched[i].sector;
                    int e_nSector = electrons[0]->cal(neutrons_FD_matched[i].First_ECAL_clas12detlayer)->getSector();  // Get electron sector at the same ECAL layer as FD neutron
                    if (abs(nFD_nSector - e_nSector) == 3) {
                        neutrons_FD_PassMatch_OnlyGood.push_back(neutrons_FD_matched[i]);
                    } else {
                        neutrons_FD_PassMatch_OnlyBad.push_back(neutrons_FD_matched[i]);
                    }

                    h_reco_P_nFD_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight);

                    if (ParticleInPCAL) { h_reco_P_nFD_bool_PCAL_test_only_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight); }
                    if (ParticleInECIN) { h_reco_P_nFD_bool_ECIN_test_only_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight); }
                    if (ParticleInECOUT) { h_reco_P_nFD_bool_ECOUT_test_only_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight); }

                    if (neutrons_FD_matched[i].particle_clas12detlayer == clas12::PCAL) { h_reco_P_nFD_PCAL_only_clas12detlayer_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight); }
                    if (neutrons_FD_matched[i].particle_clas12detlayer == clas12::ECIN) { h_reco_P_nFD_ECIN_only_clas12detlayer_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight); }
                    if (neutrons_FD_matched[i].particle_clas12detlayer == clas12::ECOUT) { h_reco_P_nFD_ECOUT_only_clas12detlayer_matched_1e_cut->Fill(reco_P_nFD.Mag(), weight); }

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
                    // h_nFD_status_VS_reco_dTheta_nFD_e_matched_1e_cut->Fill(dTheta, neutrons_FD_matched[i]->par()->getStatus(), weight);
                    // h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut->Fill(dTheta, neutrons_FD_matched[i]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_nFD_e_matched_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                    h_e_status_VS_reco_dTheta_nFD_e_zoomin_matched_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                    if (neutrons_FD_matched[i].sector == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
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
                        // h_nFD_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, neutrons_FD_matched[i]->par()->getStatus(), weight);
                        // h_nFD_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, neutrons_FD_matched[i]->par()->getStatus(), weight);
                        h_e_status_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);
                        h_e_status_VS_reco_dTheta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, electrons[0]->par()->getStatus(), weight);

                        h_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);

                        h_reco_theta_nFD_e_VS_reco_dTheta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dTheta, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(dPhi, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        h_reco_theta_nFD_e_VS_reco_dPhi_nFD_e_Same_e_nFD_Sector_zoomin_matched_1e_cut->Fill(dPhi, reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);
                        h_reco_theta_nFD_e_VS_reco_P_nFD_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Mag(), reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, weight);

                        // h_nFD_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, neutrons_FD_matched[i]->par()->getStatus(),
                        // weight); h_nFD_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI,
                        // neutrons_FD_matched[i]->par()->getStatus(),
                        //                                                                                weight);
                        h_e_status_VS_reco_theta_nFD_e_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                        h_e_status_VS_reco_theta_nFD_e_zoomin_Same_e_nFD_Sector_matched_1e_cut->Fill(reco_P_nFD.Angle(reco_P_e) * 180 / M_PI, electrons[0]->par()->getStatus(), weight);
                    }

                    TVector3 v_nhit_matched(neutrons_FD_matched[i].xHit, neutrons_FD_matched[i].yHit, neutrons_FD_matched[i].zHit);
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

                    // if (!ParticleInPCAL && (ParticleInECIN || ParticleInECOUT)) {
                    //     // if neutron did not hit PCAL, and hit either ECIN or ECOUT

                    //     // neutron ECIN/ECAL hit vector and angles:
                    //     nFD_hit_3V.SetXYZ(neutrons_FD_matched[i]->cal(detlayer)->getX(), neutrons_FD_matched[i]->cal(detlayer)->getY(), neutrons_FD_matched[i]->cal(detlayer)->getZ());
                    //     nFD_hit_Theta = nFD_hit_3V.Theta() * 180 / pi, nFD_hit_Phi = nFD_hit_3V.Phi() * 180 / pi;

                    //     if ((detlayer == clas12::ECIN) && (electrons[0]->cal(clas12::ECIN)->getZ() != 0)) {
                    //         e_hit_3V.SetXYZ(electrons[0]->cal(clas12::ECIN)->getX(), electrons[0]->cal(clas12::ECIN)->getY(), electrons[0]->cal(clas12::ECIN)->getZ());
                    //         e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                    //     } else if ((detlayer == clas12::ECOUT) && (electrons[0]->cal(clas12::ECOUT)->getZ() != 0)) {
                    //         e_hit_3V.SetXYZ(electrons[0]->cal(clas12::ECOUT)->getX(), electrons[0]->cal(clas12::ECOUT)->getY(), electrons[0]->cal(clas12::ECOUT)->getZ());
                    //         e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                    //     } else {
                    //         int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
                    //         e_hit_3V.SetXYZ(electrons[0]->traj(clas12::ECAL, trajlayer)->getX(), electrons[0]->traj(clas12::ECAL, trajlayer)->getY(),
                    //                         electrons[0]->traj(clas12::ECAL, trajlayer)->getZ());
                    //         e_hit_Theta = e_hit_3V.Theta() * 180 / pi, e_hit_Phi = e_hit_3V.Phi() * 180 / pi;
                    //     }

                    //     dPhi_hit = am::CalcdPhi1(nFD_hit_Phi - e_hit_Phi), dTheta_hit = nFD_hit_Theta - e_hit_Theta;
                    // }  // end of if neutron did not hit PCAL & hit either ECIN or ECOUT

                    // // bool NeutronPassVeto = NeutronECAL_Cut_Veto_original(allParticles, electrons, Ebeam, i, cPart_veto_radius);

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
                            double TLNeutronP = am::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
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

                    int nFD_nSector = neutrons_FD_matched[i].sector;
                    int e_nSector = electrons[0]->cal(neutrons_FD_matched[i].First_ECAL_clas12detlayer)->getSector();  // Get electron sector at the same ECAL layer as FD neutron
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
    TH1D* h_eff_P_nFD_1e_cut_denominator =
        (TH1D*)TL_histograms.h_truth_P_nFD_truth_1e_cut->Clone((std::string(TL_histograms.h_truth_P_nFD_truth_1e_cut->GetName()) + "_denominator").c_str());
    // TH1D* h_eff_P_nFD_1e_cut_denominator = (TH1D*)h_truth_P_nFD_truth_1e_cut->Clone((std::string(h_truth_P_nFD_truth_1e_cut->GetName()) + "_denominator").c_str());

    // Get detection efficiency as a function of reco P_nFD by dividing reco P_nFD by the denominator truth P_nFD
    TH1D* h_eff_P_nFD_ECALveto_1e_cut_numerator = (TH1D*)ECALveto_histograms.NSB_nFD_histograms.h_reco_P_nFD_NeutronSelection_1e_cut->Clone(
        (std::string(ECALveto_histograms.NSB_nFD_histograms.h_reco_P_nFD_NeutronSelection_1e_cut->GetName()) + "_numerator").c_str());
    // TH1D* h_eff_P_nFD_ECALveto_1e_cut_numerator = (TH1D*)h_reco_P_nFD_ECALveto_1e_cut->Clone((std::string(h_reco_P_nFD_ECALveto_1e_cut->GetName()) + "_numerator").c_str());
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

            // Maximum characters per line for OutFolderName and max number of lines
            constexpr std::size_t kOutFolderNameMaxLen = 70;  // threshold length
            constexpr int kOutFolderNameMaxLines = 3;

            std::vector<std::string> OutFolderName_lines;

            if (OutFolderName.size() <= kOutFolderNameMaxLen) {
                OutFolderName_lines.push_back(OutFolderName);
            } else {
                for (std::size_t pos = 0; pos < OutFolderName.size() && (int)OutFolderName_lines.size() < kOutFolderNameMaxLines; pos += kOutFolderNameMaxLen) {
                    OutFolderName_lines.push_back(OutFolderName.substr(pos, std::min(kOutFolderNameMaxLen, OutFolderName.size() - pos)));
                }
            }

            int nOutLines = static_cast<int>(OutFolderName_lines.size());
            double firstOutLine_y = 0.65;

            for (int i = 0; i < nOutLines; ++i) {
                double y = firstOutLine_y - 0.05 * i;  // 0.65, 0.60, 0.55
                text.DrawLatex(0.05, y, ("#font[42]{" + OutFolderName_lines[i] + "}").c_str());
            }

            // If we used 3 lines, last OutFolderName line is at 0.55,
            // so push "Sample properties" and below down by 0.05 to avoid overlap.
            double sampleProps_y0 = 0.55;
            if (nOutLines >= 3) { sampleProps_y0 -= 0.05; }  // -> 0.50 if 3 lines

            double beamEnergy_y = sampleProps_y0 - 0.05;  // originally 0.50
            double sampleName_y = sampleProps_y0 - 0.10;  // originally 0.45
            double limiter_y = sampleProps_y0 - 0.15;     // originally 0.40

            text.DrawLatex(0.05, sampleProps_y0, "Sample properties:");
            text.DrawLatex(0.10, beamEnergy_y, ("Beam energy: #font[42]{" + Ebeam_status0 + "}").c_str());
            text.DrawLatex(0.10, sampleName_y, ("Sample name: #font[42]{" + SampleName + "}").c_str());
            text.DrawLatex(0.10, limiter_y, ("Limiter: #font[42]{" + ToStringWithPrecision(Limiter, 0) + "}").c_str());

            // Keep same vertical spacing between "Sample properties" and "Run mode"
            double runMode_y0 = sampleProps_y0 - 0.25;  // originally 0.30 = 0.55 - 0.25
            double constrainedE_y = runMode_y0 - 0.05;  // originally 0.25
            double onlyGood_y = runMode_y0 - 0.10;      // originally 0.20
            double onlyBad_y = runMode_y0 - 0.15;       // originally 0.15

            text.DrawLatex(0.05, runMode_y0, "Run mode:");
            text.DrawLatex(0.10, constrainedE_y, ("ConstrainedE: #font[42]{" + bt::BoolToString(ConstrainedE) + "}").c_str());
            text.DrawLatex(0.10, onlyGood_y, ("OnlyGood_nFD: #font[42]{" + bt::BoolToString(OnlyGood_nFD) + "}").c_str());
            text.DrawLatex(0.10, onlyBad_y, ("OnlyBad_nFD: #font[42]{" + bt::BoolToString(OnlyBad_nFD) + "}").c_str());

            // text.DrawLatex(0.05, 0.70, "OutFolderName:");
            // text.DrawLatex(0.05, 0.65, ("#font[42]{" + OutFolderName + "}").c_str());

            // text.DrawLatex(0.05, 0.55, "Sample properties:");
            // text.DrawLatex(0.10, 0.50, ("Beam energy: #font[42]{" + Ebeam_status0 + "}").c_str());
            // text.DrawLatex(0.10, 0.45, ("Sample name: #font[42]{" + SampleName + "}").c_str());
            // text.DrawLatex(0.10, 0.40, ("Limiter: #font[42]{" + ToStringWithPrecision(Limiter, 0) + "}").c_str());

            // text.DrawLatex(0.05, 0.30, "Run mode:");
            // text.DrawLatex(0.10, 0.25, ("ConstrainedE: #font[42]{" + bt::BoolToString(ConstrainedE) + "}").c_str());
            // text.DrawLatex(0.10, 0.20, ("OnlyGood_nFD: #font[42]{" + bt::BoolToString(OnlyGood_nFD) + "}").c_str());
            // text.DrawLatex(0.10, 0.15, ("OnlyBad_nFD: #font[42]{" + bt::BoolToString(OnlyBad_nFD) + "}").c_str());

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

            subtitles.DrawLatex(0.05, 0.90, "Event counts summary");

            text.DrawLatex(0.05, 0.80, "ECALveto events:");
            text.DrawLatex(0.10, 0.75,
                           ("{Total #(nFD), Total #(LnFD)}}:       #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_hits_in_event_ECALveto, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_hits_in_event_ECALveto, 0) + "}")
                               .c_str());
            text.DrawLatex(0.10, 0.70,
                           ("{#(nFD in PCAL), #(LnFD in PCAL)}}:   #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_PCAL_hits_in_event_ECALveto, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_PCAL_hits_in_event_ECALveto, 0) + "}")
                               .c_str());
            text.DrawLatex(0.10, 0.65,
                           ("{#(nFD in ECIN), #(LnFD in ECIN)}}:   #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_ECIN_hits_in_event_ECALveto, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_ECIN_hits_in_event_ECALveto, 0) + "}")
                               .c_str());
            text.DrawLatex(0.10, 0.60,
                           ("{#(nFD in ECOUT), #(LnFD in ECOUT)}}: #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_ECOUT_hits_in_event_ECALveto, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_ECOUT_hits_in_event_ECALveto, 0) + "}")
                               .c_str());

            text.DrawLatex(0.05, 0.50, "Matched events:");
            text.DrawLatex(0.10, 0.45,
                           ("{Total #(nFD), Total #(LnFD)}}:       #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_hits_in_event_matched, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_hits_in_event_matched, 0) + "}")
                               .c_str());
            text.DrawLatex(0.10, 0.40,
                           ("{#(nFD in PCAL), #(LnFD in PCAL)}}:   #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_PCAL_hits_in_event_matched, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_PCAL_hits_in_event_matched, 0) + "}")
                               .c_str());
            text.DrawLatex(0.10, 0.35,
                           ("{#(nFD in ECIN), #(LnFD in ECIN)}}:   #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_ECIN_hits_in_event_matched, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_ECIN_hits_in_event_matched, 0) + "}")
                               .c_str());
            text.DrawLatex(0.10, 0.30,
                           ("{#(nFD in ECOUT), #(LnFD in ECOUT)}}: #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_ECOUT_hits_in_event_matched, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_ECOUT_hits_in_event_matched, 0) + "}")
                               .c_str());

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
                            palette->SetY2NDC(Is_hsPlot ? 0.40 : 0.50);
                            // palette->SetY2NDC(Is_hsPlot ? 0.45 : 0.55);
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

#pragma region /* FDNeutralVeto plots */

    if (plot_FDNeutralVeto) {
        veto.FinalizeDebugHists();
        fdVetoFile->Write();
        fdVetoFile->Close();
        // delete fdVetoFile;
        // fdVetoFile = nullptr;
    }

#pragma endregion

#pragma endregion

#pragma region /* Summary printout */

    std::cout << "\033[33m\n\n===========================================================================\n\033[0m";
    std::cout << "\033[33m\t\t\tSummary of GammaNeutronFD_separation_test\n\033[0m";
    std::cout << "\033[33m===========================================================================\n\n\033[0m";

    std::cout << "Inputfiles: " << InputFiles << "\n\n";
    std::cout << "Processed " << LoopNumber + 1 << " files.\n\n";

    std::cout << "Total {#(nFD), #(LnFD)} in ECALveto events:                   " << NumberOfnFD_hits_in_event_ECALveto << ", " << NumberOfLnFD_hits_in_event_ECALveto << "\n";
    std::cout << "Total {#(nFD in PCAL), #(LnFD in PCAL)} in ECALveto events:   " << NumberOfnFD_PCAL_hits_in_event_ECALveto << ", " << NumberOfLnFD_PCAL_hits_in_event_ECALveto << "\n";
    std::cout << "Total {#(nFD in ECIN), #(LnFD in ECIN)} in ECALveto events:   " << NumberOfnFD_ECIN_hits_in_event_ECALveto << ", " << NumberOfLnFD_ECIN_hits_in_event_ECALveto << "\n";
    std::cout << "Total {#(nFD in ECOUT), #(LnFD in ECOUT)} in ECALveto events: " << NumberOfnFD_ECOUT_hits_in_event_ECALveto << ", " << NumberOfLnFD_ECOUT_hits_in_event_ECALveto << "\n\n";

    std::cout << "Total {#(nFD), #(LnFD)} in matched events:                    " << NumberOfnFD_hits_in_event_matched << ", " << NumberOfLnFD_hits_in_event_matched << "\n";
    std::cout << "Total {#(nFD in PCAL), #(LnFD in PCAL)} in matched events:    " << NumberOfnFD_PCAL_hits_in_event_matched << ", " << NumberOfLnFD_PCAL_hits_in_event_matched << "\n";
    std::cout << "Total {#(nFD in ECIN), #(LnFD in ECIN)} in matched events:    " << NumberOfnFD_ECIN_hits_in_event_matched << ", " << NumberOfLnFD_ECIN_hits_in_event_matched << "\n";
    std::cout << "Total {#(nFD in ECOUT), #(LnFD in ECOUT)} in matched events:  " << NumberOfnFD_ECOUT_hits_in_event_matched << ", " << NumberOfLnFD_ECOUT_hits_in_event_matched << "\n\n";

    std::cout << "\033[0m" << std::flush;

#pragma endregion

    gDirectory->Clear();

    ++LoopNumber;
}  // end of GammaNeutronFD_separation_test function
