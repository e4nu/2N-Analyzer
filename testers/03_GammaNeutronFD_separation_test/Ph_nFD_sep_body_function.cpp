#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include <cmath>
#include <limits>
#include <tuple>
#include <vector>

// Include classes:
#include "../../framework/classes/NeutronVetoRon/nfdveto.h"

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

// // Include vetoes:
// #include "ECAL_vetoes.cpp"

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
    vector<TObject*> HistoList_dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_HistoList;

    vector<vector<double>> dr_n_SliceLimits = nfdveto::Set_dr_n_Slices(10., 0., 300.);
    vector<vector<double>> truth_P_nFD_SliceLimits = nfdveto::Set_dr_n_Slices(0.2, nFD_mom_th.GetLowerCut(), Ebeam);

    vector<std::string> HistSubjects, HistSubjects2;
    vector<bool> FirstPrint;

    gStyle->SetTitleXSize(0.05), gStyle->SetTitleYSize(0.05);
    gStyle->SetTitleXOffset(0.8), gStyle->SetTitleYOffset(0.8);

    // Raw truth histograms: Histograms filled with truth information from the hipo files, without any cuts.
    RawTruth_histograms RawTL_histograms(HistSubjects, HistSubjects2, FirstPrint, HistoList_RawTruth, P_upperLim);
    cout << "\033[33m\n\nFinished setting up RAW truth histograms.\n\033[0m" << std::flush;

    // Truth 1e cut histograms: Histograms filled with truth information, but only for events that pass the one reco electron cut. Used to understand the true kinematics of the events that
    // pass the one reco electron cut, and to calculate detection efficiencies.
    TruthLevel_histograms TL_histograms(HistSubjects, HistSubjects2, FirstPrint, HistoList_truth_1e_cut, P_upperLim);
    cout << "\033[33m\n\nFinished setting up Truth 1e cut histograms.\n\033[0m" << std::flush;

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

    // ECALveto histograms: Histograms filled with reconstructed information, using the clas12reco reconstruction (with nFD redef and ECAL vetoes), for events that pass the one reco electron
    // cut. Used to understand the reconstructed kinematics of the events that pass the one reco electron cut, and to compare with the truth 1e cut histograms to understand the effect of the
    // one reco electron cut on the reconstructed kinematics of the events, as well as to understand the effect of the nFD redef and ECAL vetoes on the reconstructed kinematics of the
    // events.
    const HistContext ECALveto_ctx("ECALveto", "#splitline{ECALveto}{and P_{nFD} thresholds}", true);
    NeutronSelection_histograms ECALveto_histograms(ECALveto_ctx, HistSubjects, HistSubjects2, FirstPrint, HistoList, P_upperLim, Ebeam, dr_n_SliceLimits, dTheta_cuts,
                                                    HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n, nFD_Res_SliceLimits, HistoList_nRes_BysliceOf_truth_P_nFD,
                                                    HistoList_nRes_BysliceOf_reco_P_nFD);
    cout << "\033[33m\n\nFinished setting up ECALveto histograms.\n\033[0m" << std::flush;

    // Matched histograms:
    const HistContext Matched_ctx("Matched", "#splitline{Matched}{reco and truth nFDs}", true);
    NeutronSelection_histograms Matched_histograms(Matched_ctx, HistSubjects, HistSubjects2, FirstPrint, HistoList, P_upperLim, Ebeam, dr_n_SliceLimits, dTheta_cuts,
                                                   HistoList_dTheta_n_e_VS_dPhi_n_e_BysliceOf_dr_n, nFD_Res_SliceLimits, HistoList_nRes_BysliceOf_truth_P_nFD,
                                                   HistoList_nRes_BysliceOf_reco_P_nFD);
    cout << "\033[33m\n\nFinished setting up Matched histograms.\n\033[0m" << std::flush;

    // ===============================
    // Diagnostics: PCAL neutral presence and photon/dropped classification vs P_truth
    // ===============================

    TH1D* h_diag_den_truthPnFD = new TH1D("diag_den_truthPnFD", "Truth nFD denominator;P^{truth}_{nFD} [GeV/c];Counts", 80, 0.0, 8.0);
    HistoList.push_back(h_diag_den_truthPnFD);

    // Fraction 1: "any neutral PCAL hit exists" (event-level)
    TH1D* h_diag_num_anyNeutralPCAL_event = new TH1D("diag_num_anyNeutralPCAL_event", "Truth nFD with ANY neutral PCAL hit in event;P^{truth}_{nFD} [GeV/c];Counts", 80, 0.0, 8.0);
    HistoList.push_back(h_diag_num_anyNeutralPCAL_event);

    // Fraction 1 (recommended): "neutral PCAL hit matched to truth direction"
    TH1D* h_diag_num_anyNeutralPCAL_matched =
        new TH1D("diag_num_anyNeutralPCAL_matched", "Truth nFD with neutral PCAL hit matched to truth dir;P^{truth}_{nFD} [GeV/c];Counts", 80, 0.0, 8.0);
    HistoList.push_back(h_diag_num_anyNeutralPCAL_matched);

    // Fraction 2: classified as photon OR dropped (mutually exclusive categories if you do it right)
    TH1D* h_diag_num_truthClassifiedPhoton = new TH1D("diag_num_truthClassifiedPhoton", "Truth nFD classified as photon (matched);P^{truth}_{nFD} [GeV/c];Counts", 80, 0.0, 8.0);
    HistoList.push_back(h_diag_num_truthClassifiedPhoton);

    TH1D* h_diag_num_truthDropped = new TH1D("diag_num_truthDropped", "Truth nFD dropped (no matched neutron/photon cand);P^{truth}_{nFD} [GeV/c];Counts", 80, 0.0, 8.0);
    HistoList.push_back(h_diag_num_truthDropped);

    auto AngMatchTruthReco = [&](double truth_theta_deg, double truth_phi_deg, double reco_theta_deg, double reco_phi_deg, double dThetaMax_deg = 2.0, double dPhiMax_deg = 5.0) -> bool {
        const bool thetaCut = (std::fabs(truth_theta_deg - reco_theta_deg) <= dThetaMax_deg);
        const bool phiCut = (std::fabs(am::CalcdPhi1(truth_phi_deg - reco_phi_deg)) <= dPhiMax_deg);
        return (thetaCut && phiCut);
    };

    // "Has ANY neutral PCAL hit in the event" (coarse)
    auto HasAnyNeutralPCALHit_EventLevel = [&](const auto& detParticles) -> bool {
        for (size_t i = 0; i < detParticles.size(); ++i) {
            const auto& p = detParticles[i];
            if (!p) continue;

            if (p->par()->getCharge() != 0) continue;

            // PCAL detector id is 7 in your code usage
            const bool hasPCAL = (p->cal(clas12::PCAL)->getDetector() == 7) && (p->cal(clas12::PCAL)->getZ() != 0);
            if (hasPCAL) return true;
        }
        return false;
    };

    // "Has a neutral PCAL hit matched to truth direction" (recommended)
    auto HasNeutralPCALHit_MatchedToTruth = [&](const auto& detParticles, double truth_theta_deg, double truth_phi_deg) -> bool {
        for (size_t i = 0; i < detParticles.size(); ++i) {
            const auto& p = detParticles[i];
            if (!p) continue;

            if (p->par()->getCharge() != 0) continue;

            const bool hasPCAL = (p->cal(clas12::PCAL)->getDetector() == 7) && (p->cal(clas12::PCAL)->getZ() != 0);
            if (!hasPCAL) continue;

            const double reco_theta_deg = p->getTheta() * 180.0 / M_PI;
            const double reco_phi_deg = p->getPhi() * 180.0 / M_PI;

            if (AngMatchTruthReco(truth_theta_deg, truth_phi_deg, reco_theta_deg, reco_phi_deg)) return true;
        }
        return false;
    };

    // Does the truth neutron direction match at least one NeutralHit candidate you kept
    auto TruthMatchesAnyNeutralHitCandidate = [&](const std::vector<NeutralHit>& neutCands, double truth_theta_deg, double truth_phi_deg) -> bool {
        for (size_t i = 0; i < neutCands.size(); ++i) {
            const double reco_theta_deg = neutCands[i].HitPath.Theta() * 180.0 / M_PI;  // direction proxy: hit path
            const double reco_phi_deg = neutCands[i].HitPath.Phi() * 180.0 / M_PI;
            if (AngMatchTruthReco(truth_theta_deg, truth_phi_deg, reco_theta_deg, reco_phi_deg)) return true;
        }
        return false;
    };

    // Does the truth neutron direction match at least one photon candidate
    auto TruthMatchesAnyPhotonCandidate = [&](const std::vector<region_part_ptr>& phCands, double truth_theta_deg, double truth_phi_deg) -> bool {
        for (size_t i = 0; i < phCands.size(); ++i) {
            const auto& p = phCands[i];
            if (!p) continue;

            const double reco_theta_deg = p->getTheta() * 180.0 / M_PI;
            const double reco_phi_deg = p->getPhi() * 180.0 / M_PI;

            if (AngMatchTruthReco(truth_theta_deg, truth_phi_deg, reco_theta_deg, reco_phi_deg)) return true;
        }
        return false;
    };

    std::vector<TObject*> Sliced_dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_HistoList_Matched_1e_cut;
    hsPlots h_dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_Matched_1e_cut =
        hsPlots(truth_P_nFD_SliceLimits, hsPlots::TH2D_TYPE, HistoList_dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_HistoList,
                &Sliced_dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_HistoList_Matched_1e_cut, "dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_Matched_1e_cut",
                "#delta#theta^{truth,reco} vs. #delta#phi^{truth,reco} in slices of P^{truth}_{nFD} in 1e cut (Matched);#delta#phi^{truth,reco} = |#phi^{truth}_{nFD} - #phi^{reco}_{nFD}| "
                "[#circ];#delta#theta^{truth,reco} = #theta^{truth}_{nFD} - #theta^{reco}_{nFD} [#circ]",
                100, -180., 180., 100, -25., 10., "|#Deltar_{n}| [cm]");

    // ===============================
    // 2D efficiency diagnostics: (P_truth, theta_truth)
    // ===============================

    TH2D* h_den_truthPnFD_vsP_vsTheta = new TH2D("den_truthPnFD_vsP_vsTheta", "Denominator: truth nFD;P^{truth}_{nFD} [GeV/c];#theta^{truth}_{nFD} [deg]", 80, 0.0, 8.0, 60, 5.0, 35.0);
    HistoList.push_back(h_den_truthPnFD_vsP_vsTheta);

    TH2D* h_num_truthPnFD_matched_vsP_vsTheta =
        new TH2D("num_truthPnFD_matched_vsP_vsTheta", "Numerator: truth nFD matched;P^{truth}_{nFD} [GeV/c];#theta^{truth}_{nFD} [deg]", 80, 0.0, 8.0, 60, 5.0, 35.0);
    HistoList.push_back(h_num_truthPnFD_matched_vsP_vsTheta);

    // =======================================================
    // Create output ROOT file for FD veto diagnostics
    // =======================================================

    TFile* fdVetoFile_eff = new TFile((OutputDir + "/FDVetoDiagnostics.root").c_str(), "RECREATE");

    if (!fdVetoFile_eff || fdVetoFile_eff->IsZombie()) {
        std::cerr << "ERROR: Could not create FDVetoDiagnostics.root" << std::endl;
        exit(1);
    }

    // Create structure inside file
    TDirectory* dirEfficiency = fdVetoFile_eff->mkdir("EfficiencyDiagnostics");

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

    AMaps aMaps_FCapplier = AMaps(path_definitions::PathDefinitions.AcceptanceMapsDirectory, Ebeam, "AMaps", /* AMapsSettings.Electron_single_slice_test= */ false,
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
    cfg.enable = false;
    cfg.baseDir = fdVetoFile;  // TFile* outFile is fine (it is a TDirectory)
    cfg.book2DStepHists = false;
    cfg.writeCanvases = false;  // set true only if you really want ROOT canvases
    // cfg.enable = true;
    // cfg.baseDir = fdVetoFile;  // TFile* outFile is fine (it is a TDirectory)
    // cfg.book2DStepHists = true;
    // cfg.writeCanvases = false;  // set true only if you really want ROOT canvases

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

    clasAna.readInputParam((path_definitions::PathDefinitions.ParameterCutFilesDirectory + "ana_c_single.par").c_str());

    // clasAna.readEcalSFPar((path_definitions::PathDefinitions.ParameterCutFilesDirectory + "paramsSF_40Ca_x2.dat").c_str());
    clasAna.setEcalSFCuts();

    // clasAna.readEcalPPar((path_definitions::PathDefinitions.ParameterCutFilesDirectory + "paramsPI_40Ca_x2.dat").c_str());
    clasAna.setEcalPCuts();
    clasAna.setEcalEdgeCuts();

    clasAna.setNpheCuts();
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

    // clasAna.readAdditionalInputParam(
    //     (path_definitions::PathDefinitions.NucleonCutsDirectory + "Nucleon_Cuts_-_" + SampleName + ".par").c_str());  // load sample-appropreate cuts file from CutsDirectory

    std::cout << "\033[33m\nFinal clas12ana parameters are:\033[0m" << std::flush;
    clasAna.printParams();

    // exit(0);

#pragma endregion

#pragma region /* Loop over events */

    std::cout << "\033[33m\n\nLooping over events...\n\033[0m" << std::flush;

    int counter = 0;

    int NumberOfnFD_hits_in_event_ECALveto = 0, NumberOfLnFD_hits_in_event_ECALveto = 0;
    int NumberOfnFD_PCAL_hits_in_event_ECALveto = 0, NumberOfLnFD_PCAL_hits_in_event_ECALveto = 0;
    int NumberOfnFD_ECIN_hits_in_event_ECALveto = 0, NumberOfLnFD_ECIN_hits_in_event_ECALveto = 0;
    int NumberOfnFD_ECOUT_hits_in_event_ECALveto = 0, NumberOfLnFD_ECOUT_hits_in_event_ECALveto = 0;

    int NumberOfnFD_hits_in_event_Matched = 0, NumberOfLnFD_hits_in_event_Matched = 0;
    int NumberOfnFD_PCAL_hits_in_event_Matched = 0, NumberOfLnFD_PCAL_hits_in_event_Matched = 0;
    int NumberOfnFD_ECIN_hits_in_event_Matched = 0, NumberOfLnFD_ECIN_hits_in_event_Matched = 0;
    int NumberOfnFD_ECOUT_hits_in_event_Matched = 0, NumberOfLnFD_ECOUT_hits_in_event_Matched = 0;

    while (chain.Next() == true) {
        // Display completed
        ++counter;
        if ((counter % 1000000) == 0) { std::cerr << "\n" << counter / 1000000 << " million completed"; }
        if ((counter % 100000) == 0) { std::cerr << "."; }

        if (counter > Limiter) { break; }

        clasAna.Run(c12);

        const double weight = 1;

        //  =======================================================================================================================================================================
        //  Setup truth particles bank
        //  =======================================================================================================================================================================

#pragma region /* Setup MC bank */

        auto c12 = chain.GetC12Reader();
        auto mceve = c12->mcevent();
        auto mcpbank = c12->mcparts();
        const Int_t Ngen = mcpbank->getRows();

#pragma endregion

        //  =======================================================================================================================================================================
        //  Fill RawTruth histograms
        //  =======================================================================================================================================================================

#pragma region /* Fill RawTruth histograms */

        // Raw truth histograms: Histograms filled with truth information from the hipo files, without any cuts.
        // These histograms are filled here because below we apply the 1e cut on reco electrons.
        for (Int_t i = 0; i < Ngen; i++) {
            mcpbank->setEntry(i);

            auto pid_temp = mcpbank->getPid();
            double Truth_P_temp = mcpbank->getP(), Truth_theta_temp = mcpbank->getTheta() * 180 / M_PI, Truth_phi_temp = mcpbank->getPhi() * 180 / M_PI;

            RawTL_histograms.Fill(pid_temp, Truth_P_temp, mcpbank->getTheta(), mcpbank->getPhi(), weight);
        }

#pragma endregion

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

        RRPID_histograms.FillElectronCandidatesBC(electrons_det, weight);

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

        const double Lv = electrons[0]->cal(clas12::PCAL)->getLv();
        const double Lw = electrons[0]->cal(clas12::PCAL)->getLw();
        const double Lu = electrons[0]->cal(clas12::PCAL)->getLu();

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

#pragma region /* A helper lambda to build the FD photons and the ECAL/Matched FD neutrons */

        auto Build_FD_neutrals = [&]() {
            // Delegate to the class implementation so the build logic can be reused outside this lambda.
            return fdNeutralHelpers.BuildFDNeutrals();
        };

#pragma endregion

#pragma region /* Helper lambdas for ConstrainedE (CE) truth variables and cut application */

        // Extract truth-level reference angles used by the ConstrainedE (CE) cuts.
        // Returns: (Truth_theta_e_deg, Truth_phi_e_deg, Truth_phi_nFD_deg).
        // Notes:
        //   - The scan is over the MC particles bank (mcpbank) for the current event.
        //   - Values remain at their sentinel defaults if the relevant pid is not found.
        auto GetTruthCEAnglesDeg = [&]() {
            double Truth_theta_e_deg = std::numeric_limits<double>::quiet_NaN();
            double Truth_phi_e_deg = std::numeric_limits<double>::quiet_NaN();
            double Truth_phi_nFD_deg = std::numeric_limits<double>::quiet_NaN();

            for (Int_t i = 0; i < Ngen; i++) {
                mcpbank->setEntry(i);

                const auto pid_temp = mcpbank->getPid();

                const double Truth_theta_temp_deg = mcpbank->getTheta() * 180.0 / M_PI;
                const double Truth_phi_temp_deg = mcpbank->getPhi() * 180.0 / M_PI;

                if (pid_temp == 11) {
                    Truth_theta_e_deg = Truth_theta_temp_deg;
                    Truth_phi_e_deg = Truth_phi_temp_deg;
                } else if (pid_temp == 2112) {
                    Truth_phi_nFD_deg = Truth_phi_temp_deg;
                }
            }

            return std::make_tuple(Truth_theta_e_deg, Truth_phi_e_deg, Truth_phi_nFD_deg);
        };

        // Apply ConstrainedE (CE) kinematic consistency cuts.
        // Returns true if the event should be kept, false if it should be rejected.
        // Notes:
        //   - When ConstrainedE is false, this always returns true.
        //   - The fourth cut compares the reco electron phi to the expected electron phi derived
        //     from the truth neutron phi (via GetPhi_e).
        auto PassConstrainedECuts = [&](const TVector3& reco_P_e_local, const double Ebeam_local, const TString& Beam_energy_TString_local) {
            if (!ConstrainedE) { return true; }

            const auto [Truth_theta_e_deg, Truth_phi_e_deg, Truth_phi_nFD_deg] = GetTruthCEAnglesDeg();

            // If any of the truth references are missing, do not apply CE cuts.
            if (std::isnan(Truth_theta_e_deg) || std::isnan(Truth_phi_e_deg) || std::isnan(Truth_phi_nFD_deg)) { return true; }

            // The resolution (sigma/P) is less than 1%, so use twice that as a window.
            if (fabs(reco_P_e_local.Mag() - Ebeam_local) > 0.01 * Ebeam_local * 2.0) { return false; }
            if (fabs((reco_P_e_local.Theta() * 180.0 / M_PI) - Truth_theta_e_deg) > 2.0) { return false; }
            if (fabs((reco_P_e_local.Phi() * 180.0 / M_PI) - Truth_phi_e_deg) > 5.0) { return false; }

            const double expected_reco_phi_e_deg = GetPhi_e(Beam_energy_TString_local, Truth_phi_nFD_deg);
            const double reco_phi_e_deg = reco_P_e_local.Phi() * 180.0 / M_PI;
            if (am::CalcdPhi1(fabs(expected_reco_phi_e_deg - reco_phi_e_deg)) > 5.0) { return false; }

            return true;
        };

#pragma endregion

#pragma endregion

        //  =======================================================================================================================================================================
        //  1e cut (reco)
        //  =======================================================================================================================================================================

#pragma region /* 1e cut (reco) */

        //  Electron PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Electron PID cuts */

        // RRPID_histograms.FillElectronCandidatesBC(electrons_det, weight);

        // AC: fill once, for the selected electron (electrons[0]) and keep your cut definitions.
        const double vz_e = electrons[0]->par()->getVz();
        const bool bad_Vz_e_CutCond = (vz_e < -1.5 || vz_e > 0.5);
        if (!bad_Vz_e_CutCond) { RRPID_histograms.FillElectronVzAC(vz_e, weight); }

        // DC hit maps (AC)
        RRPID_histograms.FillElectronDCHitAC(electrons[0], weight);

        // nphe (AC)
        const double nphe = electrons[0]->che(clas12::HTCC)->getNphe();
        RRPID_histograms.FillElectronNpheAC(nphe, weight);

        // Edep PCAL vs EC (AC) using your already-computed values
        RRPID_histograms.FillElectronEdepPCALvsECAC(Edep_PCAL, Edep_EC, weight);

        // SF vs P (AC) using your already-computed reco_P_e and EoP_e
        RRPID_histograms.FillElectronSFvsPAC(reco_P_e.Mag(), EoP_e, electrons[0]->getSector(), weight);
        RRPID_histograms.FillElectronSFvsEdepPCALAC(Edep_PCAL, EoP_e, electrons[0]->getSector(), weight);

        // SF vs Lv/Lw/Lu (AC) using your already-computed Lv/Lw/Lu
        RRPID_histograms.FillElectronSFvsLvAC(Lv, EoP_e, weight);
        RRPID_histograms.FillElectronSFvsLwAC(Lw, EoP_e, weight);
        RRPID_histograms.FillElectronSFvsLuAC(Lu, EoP_e, weight);

        // ECAL diag (AC) using your already-computed E_PCALoP_e and E_ECINoP_e
        RRPID_histograms.FillElectronEcalDiagAC(E_PCALoP_e, E_ECINoP_e, weight);

        // Apply the same cut flow:
        if (bad_Vz_e_CutCond) { continue; }

        auto [Truth_theta_e_CE_1e_cut, Truth_phi_e_CE_1e_cut, Truth_phi_nFD_CE_1e_cut] = GetTruthCEAnglesDeg();
        if (!PassConstrainedECuts(reco_P_e, Ebeam, Beam_energy_TString)) { continue; }

        RRKin_histograms.FillRecoElectronKinematics(reco_P_e, weight);

#pragma endregion

#pragma region /* 1e cut (reco) - charged hadrons */

        RRPID_histograms.FillProtonAC(IDed_Protons_ind, protons, vtz_e, weight);  //  Proton PID cuts
        RRPID_histograms.FillPiPlusAC(Piplus_ind, piplus, vtz_e, weight);         //  Piplus PID cuts
        RRPID_histograms.FillPiMinusAC(Piminus_ind, piminus, vtz_e, weight);      //  Piminus PID cuts

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

                RRKin_histograms.FillTruthElectronKinematics(truth_P_e, weight);

                ECALveto_histograms.NSB_nFD_histograms.FillTruthElectronKinematics(truth_P_e, weight);
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

                            // Detection efficiency denominator histogram, common to all detection efficiency plots. Filled the truth P_nFD distribution that pass momentum and fiducial
                            // cuts to obtain the (e,e')n events, under the one reco electron cut.
                            TL_histograms.Fill(truth_P_n, weight);
                            h_den_truthPnFD_vsP_vsTheta->Fill(truth_P_n.Mag(), truth_P_n.Theta() * 180.0 / M_PI, weight);

                            ECALveto_histograms.NSB_nFD_histograms.FillTruthNeutronKinematics(truth_P_n, weight);
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

        // ===============================
        // Fill truth-based diagnostics vs P_truth
        // Requires exactly one truth neutron in FD (you already enforce that)
        // ===============================
        mcpbank->setEntry(truth_NeutronsFD[0]);

        const double P_truth = mcpbank->getP();
        const double theta_truth_deg = mcpbank->getTheta() * 180.0 / M_PI;
        const double phi_truth_deg = mcpbank->getPhi() * 180.0 / M_PI;

        // Denominator: the same population you use for efficiency denominator
        h_diag_den_truthPnFD->Fill(P_truth, weight);

        // Fraction 1: PCAL neutral hit presence
        const bool anyNeutralPCAL_event = HasAnyNeutralPCALHit_EventLevel(allParticles_det);
        const bool anyNeutralPCAL_matched = HasNeutralPCALHit_MatchedToTruth(allParticles_det, theta_truth_deg, phi_truth_deg);

        if (anyNeutralPCAL_event) h_diag_num_anyNeutralPCAL_event->Fill(P_truth, weight);
        if (anyNeutralPCAL_matched) h_diag_num_anyNeutralPCAL_matched->Fill(P_truth, weight);

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
        std::vector<NeutralHit> neutrons_FD_ECALveto_OnlyGood;
        std::vector<NeutralHit> neutrons_FD_ECALveto_OnlyBad;
        NeutralHit neutrons_FD_ECALveto_LnFD;
        std::vector<region_part_ptr> photons_FD_ECALveto;
        std::vector<int> neutrons_FD_ECALveto_ind;

        std::tie(Reco_P_LnFD_mag_ECALveto, Reco_P_LnFD_ind_ECALveto, neutrons_FD_ECALveto, neutrons_FD_ECALveto_OnlyGood, neutrons_FD_ECALveto_OnlyBad, neutrons_FD_ECALveto_LnFD,
                 photons_FD_ECALveto, neutrons_FD_ECALveto_ind) = Build_FD_neutrals();

#pragma endregion

#pragma region /* Fill FD neutrals histograms (ECALveto) */

        // Filling phFD momentum BPID (ECALveto):
        for (int i = 0; i < photons_FD_ECALveto.size(); i++) { ECALveto_histograms.NSB_nFD_histograms.FillRecoPhotonKinematicsBPID(photons_FD_ECALveto[i]->par()->getP(), weight); }

        if ((!apply_nFD_multi_cut || neutrons_FD_ECALveto.size() == 1)        // Apply nFD multiplicity cut
            && (!apply_no_FD_photons_cut || photons_FD_ECALveto.size() == 0)  // Apply no FD photons above threshold cut
        ) {
#pragma region /* Filling FD leading neutron plots (ECALveto) */

            if (Reco_P_LnFD_ind_ECALveto != -1) {
                NeutronSelection_variables ECALveto_variables_LnFD(OnlyGood_nFD, OnlyBad_nFD, apply_neutFD_redef, apply_theta_nFD_e_vs_dTheta_cuts, dTheta_cuts, allParticles, electrons[0],
                                                                   starttime, neutrons_FD_ECALveto_LnFD, fdNeutralHelpers, reco_P_e);

                if (OnlyGood_nFD && !ECALveto_variables_LnFD.nFD_pass_dSector_cuts) { continue; }
                if (OnlyBad_nFD && !ECALveto_variables_LnFD.nFD_pass_dSector_cuts) { continue; }
                if (ECALveto_variables_LnFD.DoesNotPass_theta_nFD_e_vs_dTheta_cuts) { continue; }

                NumberOfLnFD_hits_in_event_ECALveto++;
                if (ECALveto_variables_LnFD.ParticleInPCAL) { NumberOfLnFD_PCAL_hits_in_event_ECALveto++; }
                if (ECALveto_variables_LnFD.ParticleInECIN) { NumberOfLnFD_ECIN_hits_in_event_ECALveto++; }
                if (ECALveto_variables_LnFD.ParticleInECOUT) { NumberOfLnFD_ECOUT_hits_in_event_ECALveto++; }

                ECALveto_histograms.NSB_LnFD_histograms.FillRecoElectronKinematics(reco_P_e, weight);
                ECALveto_histograms.NSB_LnFD_histograms.FillRecoNeutronKinematics(neutrons_FD_ECALveto_LnFD, ECALveto_variables_LnFD, 1, weight);
                ECALveto_histograms.NSA_LnFD_histograms.Fill(neutrons_FD_ECALveto_LnFD, ECALveto_variables_LnFD, electrons, B, L, weight);

                if (neutrons_FD_ECALveto_LnFD.sector == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
                    bool Pass_dTheta_window = (ECALveto_variables_LnFD.dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetLowerCut() &&
                                              (ECALveto_variables_LnFD.dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetUpperCut();
                    ECALveto_histograms.NSSS_LnFD_histograms.Fill(ECALveto_variables_LnFD, Pass_dTheta_window, weight);
                }

                for (int j = 0; j < allParticles.size(); j++) {
                    if (allParticles[j]->par()->getCharge() == 0) {
                        bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                        if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                            ECALveto_histograms.MSM_LnFD_histograms.Fill(ECALveto_variables_LnFD.v_dist, ECALveto_variables_LnFD, weight);
                        }
                    }
                }
            }  // end of if neutrons_FD_ECALveto_LnFD

#pragma endregion

#pragma region /* Filling FD neutron plots (ECALveto) */

            for (int i = 0; i < neutrons_FD_ECALveto.size(); i++) {
                NeutronSelection_variables ECALveto_variables_nFD(OnlyGood_nFD, OnlyBad_nFD, apply_neutFD_redef, apply_theta_nFD_e_vs_dTheta_cuts, dTheta_cuts, allParticles, electrons[0],
                                                                  starttime, neutrons_FD_ECALveto[i], fdNeutralHelpers, reco_P_e);

                if (OnlyGood_nFD && !ECALveto_variables_nFD.nFD_pass_dSector_cuts) { continue; }
                if (OnlyBad_nFD && !ECALveto_variables_nFD.nFD_pass_dSector_cuts) { continue; }
                if (ECALveto_variables_nFD.DoesNotPass_theta_nFD_e_vs_dTheta_cuts) { continue; }

                if (ECALveto_variables_nFD.Pass_dTheta_status && ECALveto_variables_nFD.Pass_dPhi_status) {
                    NumberOfnFD_hits_in_event_ECALveto++;
                    if (ECALveto_variables_nFD.ParticleInPCAL) { NumberOfnFD_PCAL_hits_in_event_ECALveto++; }
                    if (ECALveto_variables_nFD.ParticleInECIN) { NumberOfnFD_ECIN_hits_in_event_ECALveto++; }
                    if (ECALveto_variables_nFD.ParticleInECOUT) { NumberOfnFD_ECOUT_hits_in_event_ECALveto++; }

                    ECALveto_histograms.NSB_nFD_histograms.FillRecoElectronKinematics(reco_P_e, weight);
                    ECALveto_histograms.NSB_nFD_histograms.FillRecoNeutronKinematics(neutrons_FD_ECALveto[i], ECALveto_variables_nFD, neutrons_FD_ECALveto.size(), weight);
                    ECALveto_histograms.NSA_nFD_histograms.Fill(neutrons_FD_ECALveto[i], ECALveto_variables_nFD, electrons, B, L, weight);

                    if (neutrons_FD_ECALveto[i].sector == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
                        bool Pass_dTheta_window = (ECALveto_variables_nFD.dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetLowerCut() &&
                                                  (ECALveto_variables_nFD.dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetUpperCut();
                        ECALveto_histograms.NSSS_nFD_histograms.Fill(ECALveto_variables_nFD, Pass_dTheta_window, weight);
                    }

                    for (int j = 0; j < allParticles.size(); j++) {
                        if (allParticles[j]->par()->getCharge() == 0) {
                            bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                            if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                                ECALveto_histograms.MSM_nFD_histograms.Fill(ECALveto_variables_nFD.v_dist, ECALveto_variables_nFD, weight);
                            }
                        }
                    }
                }  // end of if pass Pass_dTheta_status_ECALveto && Pass_dPhi_status_ECALveto
            }  // end of for loop over neutrons_FD_ECALveto

            for (int i = 0; i < photons_FD_ECALveto.size(); i++) { ECALveto_histograms.NSB_nFD_histograms.FillRecoPhotonKinematicsAPID(photons_FD_ECALveto[i]->par()->getP(), weight); }

            ECALveto_histograms.NSB_nFD_histograms.FillNeutronMultiplicity(neutrons_FD_ECALveto, neutrons_FD_ECALveto_OnlyGood, neutrons_FD_ECALveto_OnlyBad, weight);

#pragma endregion

        }  // end of if pass Pass_nFD_Multi_cut_ECALveto && Pass_no_FD_photon_cut_ECALveto

#pragma endregion

#pragma endregion

        //  Matched plots
        //  -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Matched plots */

#pragma region /* FD neutrals definitions (Matched) */

        double Reco_P_LnFD_mag_Matched = -1;  // Reco leading neutron momentum
        int Reco_P_LnFD_ind_Matched = -1;     // Reco leading neutron index

        std::vector<NeutralHit> neutrons_FD_Matched;
        std::vector<NeutralHit> neutrons_FD_Matched_OnlyGood;
        std::vector<NeutralHit> neutrons_FD_Matched_OnlyBad;
        NeutralHit neutrons_FD_Matched_LnFD;
        std::vector<region_part_ptr> photons_FD_Matched;
        std::vector<int> neutrons_FD_Matched_ind;

        // Vectors for multiplicity plots
        std::vector<NeutralHit> neutrons_FD_PMC;            // neutrons_FD_PassMatchCuts
        std::vector<NeutralHit> neutrons_FD_PMC_OnlyGood;   // neutrons_FD_PassMatchCuts_OnlyGood
        std::vector<NeutralHit> neutrons_FD_PMC_OnlyBad;    // neutrons_FD_PassMatchCuts_OnlyBad
        std::vector<NeutralHit> neutrons_FD_NPMC;           // neutrons_FD_NotPassMatchCuts
        std::vector<NeutralHit> neutrons_FD_NPMC_OnlyGood;  // neutrons_FD_NotPassMatchCuts_OnlyGood
        std::vector<NeutralHit> neutrons_FD_NPMC_OnlyBad;   // neutrons_FD_NotPassMatchCuts_OnlyBad

        std::tie(Reco_P_LnFD_mag_Matched, Reco_P_LnFD_ind_Matched, neutrons_FD_Matched, neutrons_FD_Matched_OnlyGood, neutrons_FD_Matched_OnlyBad, neutrons_FD_Matched_LnFD,
                 photons_FD_Matched, neutrons_FD_Matched_ind) = Build_FD_neutrals();

        // ===============================
        // Truth diagnostics vs P_truth (part B): classified photon vs dropped
        // Requires neutrons_FD_ECALveto and photons_FD_ECALveto to already be built.
        // ===============================

        // Fraction 2: truth neutron ends up "classified as photon" OR "dropped"
        // Define these by matching the truth direction to your candidate lists
        const bool matchedToKeptNeutron = TruthMatchesAnyNeutralHitCandidate(neutrons_FD_ECALveto, theta_truth_deg, phi_truth_deg);
        const bool matchedToPhotonCand = TruthMatchesAnyPhotonCandidate(photons_FD_ECALveto, theta_truth_deg, phi_truth_deg);

        // Mutually exclusive logic: prefer neutron if both match (should be rare, but guard it)
        if (!matchedToKeptNeutron && matchedToPhotonCand) {
            h_diag_num_truthClassifiedPhoton->Fill(P_truth, weight);
        } else if (!matchedToKeptNeutron && !matchedToPhotonCand) {
            h_diag_num_truthDropped->Fill(P_truth, weight);
        }

#pragma endregion

#pragma region /* Fill FD neutrals histograms (Matched) */

        // Filling phFD momentum BPID (Matched):
        for (int i = 0; i < photons_FD_ECALveto.size(); i++) { ECALveto_histograms.NSB_nFD_histograms.FillRecoPhotonKinematicsBPID(photons_FD_ECALveto[i]->par()->getP(), weight); }

        if ((!apply_nFD_multi_cut || neutrons_FD_Matched.size() == 1)        // Apply nFD multiplicity cut
            && (!apply_no_FD_photons_cut || photons_FD_Matched.size() == 0)  // Apply no FD photons above threshold cut
        ) {
#pragma region /* Fill leading FD neutron plots and matching (Matched) */

            bool PassMatchingCuts_LnFD = false;

            bool truthMatchedToAnyRecoNFD_thisEvent = false;

            // Check if leading neutron passes matching cuts:
            if (Reco_P_LnFD_ind_Matched != -1) {
                NeutronSelection_variables Matched_variables_LnFD(OnlyGood_nFD, OnlyBad_nFD, apply_neutFD_redef, apply_theta_nFD_e_vs_dTheta_cuts, dTheta_cuts, allParticles, electrons[0],
                                                                  starttime, neutrons_FD_Matched_LnFD, fdNeutralHelpers, reco_P_e);

                for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                    mcpbank->setEntry(truth_NeutronsFD.at(j));

                    double Temp_truth_P_nFD = mcpbank->getP(), Temp_truth_theta_nFD = mcpbank->getTheta() * 180 / M_PI, Temp_truth_phi_nFD = mcpbank->getPhi() * 180 / M_PI;
                    double reco_Theta = Matched_variables_LnFD.reco_P_nFD.Theta() * 180 / M_PI, reco_Phi = Matched_variables_LnFD.reco_P_nFD.Phi() * 180 / M_PI;
                    bool thetaCut = (fabs(Temp_truth_theta_nFD - reco_Theta) <= 2.), phiCut = (fabs(am::CalcdPhi1(Temp_truth_phi_nFD - reco_Phi)) <= 5.);

                    // Check if reco leading neutron is in FD fiducial volume:
                    bool Reco_InFD = aMaps_FCapplier.IsInFDQuery(
                        /* (!CutSettings.apply_fiducial_cuts || (AMapsSettings.Generate_Electron_AMaps || AMapsSettings.Generate_Nucleon_AMaps))= */ false, ThetaFD, "Neutron",
                        Matched_variables_LnFD.reco_P_nFD.Mag(), Matched_variables_LnFD.reco_P_nFD.Theta() * 180.0 / pi, Matched_variables_LnFD.reco_P_nFD.Phi() * 180.0 / pi, false);

                    TVector3 truth_P_n;
                    truth_P_n.SetMagThetaPhi(mcpbank->getP(), mcpbank->getTheta(), mcpbank->getPhi());

                    h_dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_Matched_1e_cut.Fill(truth_P_n.Mag(), phiCut, thetaCut);

                    // if ((thetaCut && phiCut) && Reco_InFD) {  // Leading neutron passes matching cuts and is in FD fiducial volume
                    if ((thetaCut && phiCut)) {  // Leading neutron passes matching cuts and is in FD fiducial volume
                        PassMatchingCuts_LnFD = true;

                        if (OnlyGood_nFD && !Matched_variables_LnFD.nFD_pass_dSector_cuts) { continue; }
                        if (OnlyBad_nFD && !Matched_variables_LnFD.nFD_pass_dSector_cuts) { continue; }
                        if (Matched_variables_LnFD.DoesNotPass_theta_nFD_e_vs_dTheta_cuts) { continue; }

                        NumberOfLnFD_hits_in_event_Matched++;
                        if (Matched_variables_LnFD.ParticleInPCAL) { NumberOfLnFD_PCAL_hits_in_event_Matched++; }
                        if (Matched_variables_LnFD.ParticleInECIN) { NumberOfLnFD_ECIN_hits_in_event_Matched++; }
                        if (Matched_variables_LnFD.ParticleInECOUT) { NumberOfLnFD_ECOUT_hits_in_event_Matched++; }

                        Matched_histograms.NSB_LnFD_histograms.FillTruthNeutronKinematics(truth_P_n, weight);
                        Matched_histograms.NSB_LnFD_histograms.FillTruthNeutronKinematics2(truth_P_n, Matched_variables_LnFD.reco_P_nFD, weight);
                        Matched_histograms.NSB_LnFD_histograms.FillRecoElectronKinematics(reco_P_e, weight);
                        Matched_histograms.NSB_LnFD_histograms.FillRecoNeutronKinematics(neutrons_FD_Matched_LnFD, Matched_variables_LnFD, 1, weight);
                        Matched_histograms.NSA_LnFD_histograms.Fill(neutrons_FD_Matched_LnFD, Matched_variables_LnFD, electrons, B, L, weight);

                        if (neutrons_FD_Matched_LnFD.sector == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
                            bool Pass_dTheta_window = (Matched_variables_LnFD.dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetLowerCut() &&
                                                      (Matched_variables_LnFD.dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetUpperCut();
                            Matched_histograms.NSSS_LnFD_histograms.Fill(Matched_variables_LnFD, Pass_dTheta_window, weight);
                        }
                    }
                }  // end of if id. reco leading neutron
            }

#pragma endregion

#pragma region /* Filling FD neutron plots (Matched) */

            double truth_P_n_mag;
            double truth_P_n_theta_deg;

            for (int i = 0; i < neutrons_FD_Matched.size(); i++) {
                NeutronSelection_variables Matched_variables_nFD(OnlyGood_nFD, OnlyBad_nFD, apply_neutFD_redef, apply_theta_nFD_e_vs_dTheta_cuts, dTheta_cuts, allParticles, electrons[0],
                                                                 starttime, neutrons_FD_Matched[i], fdNeutralHelpers, reco_P_e);

                if (OnlyGood_nFD && !Matched_variables_nFD.nFD_pass_dSector_cuts) { continue; }
                if (OnlyBad_nFD && !Matched_variables_nFD.nFD_pass_dSector_cuts) { continue; }
                if (Matched_variables_nFD.DoesNotPass_theta_nFD_e_vs_dTheta_cuts) { continue; }

                bool PassMatchingCuts = false;

                // For every reco nFD, loop over all truth nFD in the event and find a match
                for (int j = 0; j < truth_NeutronsFD.size(); j++) {
                    mcpbank->setEntry(truth_NeutronsFD.at(j));

                    double Temp_truth_P_nFD = mcpbank->getP(), Temp_truth_theta_nFD = mcpbank->getTheta() * 180 / M_PI, Temp_truth_phi_nFD = mcpbank->getPhi() * 180 / M_PI;
                    double reco_Theta = Matched_variables_nFD.reco_P_nFD.Theta() * 180 / M_PI, reco_Phi = Matched_variables_nFD.reco_P_nFD.Phi() * 180 / M_PI;
                    bool thetaCut = (fabs(Temp_truth_theta_nFD - reco_Theta) <= 2.), phiCut = (fabs(am::CalcdPhi1(Temp_truth_phi_nFD - reco_Phi)) <= 5.);

                    if (thetaCut && phiCut) {
                        PassMatchingCuts = true;
                        truthMatchedToAnyRecoNFD_thisEvent = true;

                        TVector3 truth_P_n;
                        truth_P_n.SetMagThetaPhi(mcpbank->getP(), mcpbank->getTheta(), mcpbank->getPhi());
                        truth_P_n_mag = mcpbank->getP();
                        truth_P_n_theta_deg = mcpbank->getTheta() * 180 / M_PI;

                        // TODO: move to a differant numerator with the leading neutron, after talking with Adi
                        // Detection efficiency numerator histogram. Filled the truth P_nFD distribution of the truth nFD that passes matching cuts with the i-th reco nFD. This gives us
                        // (e,e'n) events.
                        Matched_histograms.NSB_nFD_histograms.FillTruthNeutronKinematics(truth_P_n, weight);
                        Matched_histograms.NSB_nFD_histograms.FillTruthNeutronKinematics2(truth_P_n, Matched_variables_nFD.reco_P_nFD, weight);

                        // TODO: Should this be here? this means that for every reco nFD that matches to a truth nFD, we fill the numerator, meaning that if 2 reco nFD match to the same
                        // truth nFD Acceptance efficiency numerator histograms filled with the truth P_nFD distribution of the truth nFD that passes matching cuts with the i-th reco
                        // nFD. This gives us (e,e'n) events.
                        aMaps_generator.hFillMaps("Reco", "Neutron", Temp_truth_P_nFD, Temp_truth_theta_nFD, Temp_truth_phi_nFD, weight);
                    }
                }

                // if (true) {
                // if (PassMatchingCuts_nFD) {
                // if (PassMatchingCuts_nFD && Reco_InFD) {
                // if (PassMatchingCuts && Reco_InFD) {
                if (PassMatchingCuts) {
                    NumberOfnFD_hits_in_event_Matched++;
                    if (Matched_variables_nFD.ParticleInPCAL) { NumberOfnFD_PCAL_hits_in_event_Matched++; }
                    if (Matched_variables_nFD.ParticleInECIN) { NumberOfnFD_ECIN_hits_in_event_Matched++; }
                    if (Matched_variables_nFD.ParticleInECOUT) { NumberOfnFD_ECOUT_hits_in_event_Matched++; }

                    neutrons_FD_PMC.push_back(neutrons_FD_Matched[i]);

                    Matched_histograms.NSB_nFD_histograms.FillRecoElectronKinematics(reco_P_e, weight);
                    Matched_histograms.NSB_nFD_histograms.FillRecoNeutronKinematics(neutrons_FD_Matched[i], Matched_variables_nFD, neutrons_FD_Matched.size(), weight);
                    Matched_histograms.NSA_nFD_histograms.Fill(neutrons_FD_Matched[i], Matched_variables_nFD, electrons, B, L, weight);

                    if (neutrons_FD_Matched[i].sector == electrons[0]->cal(Electron_ECAL_detlayer)->getSector()) {
                        bool Pass_dTheta_window = (Matched_variables_nFD.dTheta - dTheta_cuts.GetMean()) < dTheta_cuts.GetLowerCut() &&
                                                  (Matched_variables_nFD.dTheta - dTheta_cuts.GetMean()) > dTheta_cuts.GetUpperCut();
                        Matched_histograms.NSSS_nFD_histograms.Fill(Matched_variables_nFD, Pass_dTheta_window, weight);
                    }

                    for (int j = 0; j < allParticles.size(); j++) {
                        if (allParticles[j]->par()->getCharge() == 0) {
                            bool neutral_hit_PCAL = (allParticles[j]->cal(clas12::PCAL)->getDetector() == 7);

                            if (neutral_hit_PCAL && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
                                Matched_histograms.MSM_nFD_histograms.Fill(Matched_variables_nFD.v_dist, Matched_variables_nFD, weight);
                            }
                        }
                    }

                    Matched_histograms.NSB_nFD_histograms.h_reco_nFD_multi_VS_reco_P_nFD_PassMatch->Fill(Matched_variables_nFD.reco_P_nFD.Mag(), neutrons_FD_Matched.size(), weight);
                } else {
                    neutrons_FD_NPMC.push_back(neutrons_FD_Matched[i]);

                    if (abs(Matched_variables_nFD.nFD_nSector - Matched_variables_nFD.e_nSector) == 3) {
                        neutrons_FD_NPMC_OnlyGood.push_back(neutrons_FD_Matched[i]);
                    } else {
                        neutrons_FD_NPMC_OnlyBad.push_back(neutrons_FD_Matched[i]);
                    }

                    Matched_histograms.NSB_nFD_histograms.h_reco_nFD_multi_VS_reco_P_nFD_NotPassMatch->Fill(Matched_variables_nFD.reco_P_nFD.Mag(), neutrons_FD_Matched.size(), weight);
                }  // end of if else pass PassMatchingCuts
            }  // end of for loop over neutrons_FD_Matched

            if (truthMatchedToAnyRecoNFD_thisEvent) { h_num_truthPnFD_matched_vsP_vsTheta->Fill(truth_P_n_mag, truth_P_n_theta_deg, weight); }

            // Filling phFD momentum APID (Matched):
            for (int i = 0; i < photons_FD_Matched.size(); i++) { Matched_histograms.NSB_nFD_histograms.FillRecoPhotonKinematicsAPID(photons_FD_Matched[i]->par()->getP(), weight); }

            Matched_histograms.NSB_nFD_histograms.FillNeutronMultiplicity(neutrons_FD_PMC, neutrons_FD_PMC_OnlyGood, neutrons_FD_PMC_OnlyBad, weight, "PassMatch");
            Matched_histograms.NSB_nFD_histograms.FillNeutronMultiplicity(neutrons_FD_NPMC, neutrons_FD_NPMC_OnlyGood, neutrons_FD_NPMC_OnlyBad, weight, "NotPassMatch");

#pragma endregion

        }  // end of if pass Pass_nFD_Multi_cut_Matched && Pass_no_FD_photon_cut_Matched

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

    SetHistogramListStyle_hsPlots(ECALveto_histograms.NSA_nFD_histograms.Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList);
    SetHistogramListStyle_hsPlots(ECALveto_histograms.NSA_LnFD_histograms.Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList);
    SetHistogramListStyle_hsPlots(ECALveto_histograms.NSR_nFD_histograms.Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList);
    SetHistogramListStyle_hsPlots(ECALveto_histograms.NSR_nFD_histograms.Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList);

    SetHistogramListStyle_hsPlots(Matched_histograms.NSA_nFD_histograms.Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList);
    SetHistogramListStyle_hsPlots(Matched_histograms.NSA_LnFD_histograms.Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList);
    SetHistogramListStyle_hsPlots(Matched_histograms.NSR_nFD_histograms.Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList);
    SetHistogramListStyle_hsPlots(Matched_histograms.NSR_nFD_histograms.Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList);
    SetHistogramListStyle_hsPlots(Sliced_dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_HistoList_Matched_1e_cut);

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
    TH1D* h_eff_P_nFD_ECALveto_1e_cut_numerator =
        (TH1D*)ECALveto_histograms.NSB_nFD_histograms.h_reco_P_nFD->Clone((std::string(ECALveto_histograms.NSB_nFD_histograms.h_reco_P_nFD->GetName()) + "_numerator").c_str());
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

    // Get detection efficiency as a function of truth P_nFD (Matched to reco any nFD) by dividing truth P_nFD of Matched neutrons by the denominator truth P_nFD from above
    // TODO: move to a differant numerator with the leading neutron, after talking with Adi
    // h_truth_P_nFD_Matched_1e_cut is the detection efficiency numerator histogram. Filled the truth P_nFD distribution of the truth nFD that passes matching cuts with the i-th reco
    // nFD. This gives us (e,e'n) events.
    TH1D* h_eff_P_nFD_Matched_1e_cut_numerator =
        (TH1D*)Matched_histograms.NSB_nFD_histograms.h_truth_P_nFD->Clone((std::string(Matched_histograms.NSB_nFD_histograms.h_truth_P_nFD->GetName()) + "_numerator").c_str());
    TH1D* h_eff_P_nFD_Matched_1e_cut = (TH1D*)h_eff_P_nFD_Matched_1e_cut_numerator->Clone("eff_P_nFD_Matched_1e_cut");
    h_eff_P_nFD_Matched_1e_cut->Divide(h_eff_P_nFD_1e_cut_denominator);
    h_eff_P_nFD_Matched_1e_cut->SetName("nFD_det_eff_truth_P_nFD_Matched_1e_cut");
    h_eff_P_nFD_Matched_1e_cut->SetTitle("nFD det. eff. as a function of P^{truth}_{nFD} in 1e cut");
    h_eff_P_nFD_Matched_1e_cut->GetXaxis()->SetTitle("P^{truth}_{nFD} [GeV/c]");
    h_eff_P_nFD_Matched_1e_cut->GetYaxis()->SetTitle("Detection Efficiency");
    if (bt::FindSubstring(h_eff_P_nFD_Matched_1e_cut->GetTitle(), "det. eff.") && (h_eff_P_nFD_Matched_1e_cut->GetMaximum() > 1.)) { h_eff_P_nFD_Matched_1e_cut->SetMaximum(1.); }
    if (bt::FindSubstring(h_eff_P_nFD_Matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_Matched_1e_cut->GetXaxis()->SetRangeUser(0., 8.); }
    if (bt::FindSubstring(h_eff_P_nFD_Matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_Matched_1e_cut->GetYaxis()->SetRangeUser(0., 1.); }
    if (bt::FindSubstring(h_eff_P_nFD_Matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_Matched_1e_cut->SetStats(0); }
    HistoList_eff_plots_3x1.push_back({h_eff_P_nFD_Matched_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_Matched_1e_cut});
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_Matched_1e_cut_numerator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_1e_cut_denominator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_Matched_1e_cut);

    // Get detection efficiency as a function of truth P_nFD (Matched to reco LnFD) by dividing truth P_nFD of Matched neutrons by the denominator truth P_nFD from above
    TH1D* h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut_numerator =
        (TH1D*)Matched_histograms.NSB_LnFD_histograms.h_truth_P_nFD->Clone((std::string(Matched_histograms.NSB_LnFD_histograms.h_truth_P_nFD->GetName()) + "_numerator").c_str());
    TH1D* h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut = (TH1D*)h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut_numerator->Clone("eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut");
    h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->Sumw2();
    h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->Divide(h_eff_P_nFD_1e_cut_denominator);
    h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->SetName("LnFD_det_eff_truth_P_LnFD_Matched_to_reco_LnFD_Matched_1e_cut");
    h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->SetTitle("LnFD det. eff. as a function of P^{truth}_{LnFD} in 1e cut Matched to reco LnFD");
    h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->GetXaxis()->SetTitle("P^{truth}_{LnFD} [GeV/c]");
    h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->GetYaxis()->SetTitle("Detection Efficiency");
    if (bt::FindSubstring(h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->GetTitle(), "det. eff.") && (h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->GetMaximum() > 1.)) {
        h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->SetMaximum(1.);
    }
    if (bt::FindSubstring(h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->GetXaxis()->SetRangeUser(0., 8.); }
    if (bt::FindSubstring(h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->GetYaxis()->SetRangeUser(0., 1.); }
    if (bt::FindSubstring(h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->GetTitle(), "det. eff.")) { h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut->SetStats(0); }
    HistoList_eff_plots_3x1.push_back({h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut});
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut_numerator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_1e_cut_denominator);
    HistoList_eff_plots_1x1.push_back(h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut);

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
                           ("{Total #(nFD), Total #(LnFD)}}:       #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_hits_in_event_Matched, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_hits_in_event_Matched, 0) + "}")
                               .c_str());
            text.DrawLatex(0.10, 0.40,
                           ("{#(nFD in PCAL), #(LnFD in PCAL)}}:   #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_PCAL_hits_in_event_Matched, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_PCAL_hits_in_event_Matched, 0) + "}")
                               .c_str());
            text.DrawLatex(0.10, 0.35,
                           ("{#(nFD in ECIN), #(LnFD in ECIN)}}:   #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_ECIN_hits_in_event_Matched, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_ECIN_hits_in_event_Matched, 0) + "}")
                               .c_str());
            text.DrawLatex(0.10, 0.30,
                           ("{#(nFD in ECOUT), #(LnFD in ECOUT)}}: #font[42]{" + bt::ToStringWithPrecision(NumberOfnFD_ECOUT_hits_in_event_Matched, 0) + ", " +
                            bt::ToStringWithPrecision(NumberOfLnFD_ECOUT_hits_in_event_Matched, 0) + "}")
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
        GeneratePDFOutput(PlotsBySliceOutputDir, "dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_ECALveto",
                          ECALveto_histograms.NSA_nFD_histograms.Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList,
                          "Uniform (e,e'n) - #delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} plots in slices of |#Deltar_{nFD}|");
        GeneratePDFOutput(PlotsBySliceOutputDir, "dTheta_LnFD_e_VS_dPhi_LnFD_e_BysliceOf_dr_LnFD_ECALveto",
                          ECALveto_histograms.NSA_LnFD_histograms.Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList,
                          "Uniform (e,e'n) - #delta#theta^{reco}_{LnFD,e} vs. #delta#phi^{reco}_{LnFD,e} plots in slices of |#Deltar_{LnFD}|");
        GeneratePDFOutput(PlotsBySliceOutputDir, "dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_ECALveto",
                          Matched_histograms.NSA_nFD_histograms.Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList,
                          "Uniform (e,e'n) - #delta#theta^{reco}_{nFD,e} vs. #delta#phi^{reco}_{nFD,e} plots in slices of |#Deltar_{nFD}|");
        GeneratePDFOutput(PlotsBySliceOutputDir, "dTheta_LnFD_e_VS_dPhi_LnFD_e_BysliceOf_dr_LnFD_Matched",
                          Matched_histograms.NSA_LnFD_histograms.Sliced_dTheta_nFD_e_VS_dPhi_nFD_e_BysliceOf_dr_nFD_HistoList,
                          "Uniform (e,e'n) - #delta#theta^{reco}_{LnFD,e} vs. #delta#phi^{reco}_{LnFD,e} plots in slices of |#Deltar_{LnFD}|");

        GeneratePDFOutput(PlotsBySliceOutputDir, "dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_Matched", Sliced_dTheta_tr_VS_dPhi_tr_BysliceOf_truth_P_nFD_HistoList_Matched_1e_cut,
                          "#delta#theta^{truth,reco} vs. #delta#phi^{truth,reco} in slices of P^{truth}_{nFD} in 1e cut");

        // Sliced P_nFD resolution plots
        GeneratePDFOutput(PlotsBySliceOutputDir, "Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_ECALveto_1e_cut",
                          ECALveto_histograms.NSR_nFD_histograms.Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList,
                          "Uniform (e,e'n) - Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_ECALveto_1e_cut");
        GeneratePDFOutput(PlotsBySliceOutputDir, "Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_ECALveto_1e_cut",
                          ECALveto_histograms.NSR_nFD_histograms.Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList,
                          "Uniform (e,e'n) - Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_ECALveto_1e_cut");
        GeneratePDFOutput(PlotsBySliceOutputDir, "Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_Matched_1e_cut",
                          Matched_histograms.NSR_nFD_histograms.Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList,
                          "Uniform (e,e'n) - Sliced_P_nFD_Res_BysliceOf_truth_P_nFD_HistoList_Matched_1e_cut");
        GeneratePDFOutput(PlotsBySliceOutputDir, "Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_Matched_1e_cut",
                          Matched_histograms.NSR_nFD_histograms.Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList,
                          "Uniform (e,e'n) - Sliced_P_nFD_Res_BysliceOf_reco_P_nFD_HistoList_Matched_1e_cut");

#pragma endregion

#pragma region /* Compare histograms */

        ECALveto_histograms.CompareHistograms(ECALveto_ctx, OutputDir, "Histogram_Comparisons");
        Matched_histograms.CompareHistograms(Matched_ctx, OutputDir, "Histogram_Comparisons");

#if ENABLE_EFF_PLOT_OUTPUT
        hf::CompareHistograms({h_eff_P_nFD_ECALveto_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_ECALveto_1e_cut}, OutputDir, "Histogram_Comparisons",
                              "nFD_det_eff_by_reco_P_nFD_ECALveto_1e_cut");
        hf::CompareHistograms({h_eff_P_nFD_Matched_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_Matched_1e_cut}, OutputDir, "Histogram_Comparisons",
                              "nFD_det_eff_by_truth_P_nFD_Matched_1e_cut");
        hf::CompareHistograms({h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut_numerator, h_eff_P_nFD_1e_cut_denominator, h_eff_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut}, OutputDir,
                              "Histogram_Comparisons", "nFD_det_eff_by_truth_P_nFD_Matched_to_reco_LnFD_Matched_1e_cut");
#endif

        dirEfficiency->cd();

        auto MakeEfficiencySlices = [&](TH2D* h_den, TH2D* h_num) {
            // --- Efficiency vs P in slices of theta ---
            const int nThetaBins = h_den->GetYaxis()->GetNbins();

            for (int ybin = 1; ybin <= nThetaBins; ++ybin) {
                double th_lo = h_den->GetYaxis()->GetBinLowEdge(ybin);
                double th_hi = h_den->GetYaxis()->GetBinUpEdge(ybin);

                TH1D* den_px = h_den->ProjectionX(Form("den_P_thetaBin%d", ybin), ybin, ybin);
                TH1D* num_px = h_num->ProjectionX(Form("num_P_thetaBin%d", ybin), ybin, ybin);

                if (den_px->GetEntries() == 0) {
                    delete den_px;
                    delete num_px;
                    continue;
                }

                TEfficiency* eff = new TEfficiency(*num_px, *den_px);
                eff->SetName(Form("eff_vs_P_theta_%0.1f_%0.1f_deg", th_lo, th_hi));
                eff->SetTitle(Form("Efficiency vs P^{truth} for #theta in [%0.1f,%0.1f] deg;P^{truth}_{nFD} [GeV/c];Efficiency", th_lo, th_hi));
                eff->Write();

                delete den_px;
                delete num_px;
            }

            // --- Efficiency vs theta in slices of P ---
            const int nPBins = h_den->GetXaxis()->GetNbins();

            for (int xbin = 1; xbin <= nPBins; ++xbin) {
                double p_lo = h_den->GetXaxis()->GetBinLowEdge(xbin);
                double p_hi = h_den->GetXaxis()->GetBinUpEdge(xbin);

                TH1D* den_py = h_den->ProjectionY(Form("den_theta_pBin%d", xbin), xbin, xbin);
                TH1D* num_py = h_num->ProjectionY(Form("num_theta_pBin%d", xbin), xbin, xbin);

                if (den_py->GetEntries() == 0) {
                    delete den_py;
                    delete num_py;
                    continue;
                }

                TEfficiency* eff = new TEfficiency(*num_py, *den_py);
                eff->SetName(Form("eff_vs_theta_P_%0.2f_%0.2f_GeV", p_lo, p_hi));
                eff->SetTitle(Form("Efficiency vs #theta^{truth} for P in [%0.2f,%0.2f] GeV;#theta^{truth}_{nFD} [deg];Efficiency", p_lo, p_hi));
                eff->Write();

                delete den_py;
                delete num_py;
            }
        };

        MakeEfficiencySlices(h_den_truthPnFD_vsP_vsTheta, h_num_truthPnFD_matched_vsP_vsTheta);
        fdVetoFile_eff->cd();
        fdVetoFile_eff->Write();
        fdVetoFile_eff->Close();
        delete fdVetoFile_eff;

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

    std::cout << "Total {#(nFD), #(LnFD)} in Matched events:                    " << NumberOfnFD_hits_in_event_Matched << ", " << NumberOfLnFD_hits_in_event_Matched << "\n";
    std::cout << "Total {#(nFD in PCAL), #(LnFD in PCAL)} in Matched events:    " << NumberOfnFD_PCAL_hits_in_event_Matched << ", " << NumberOfLnFD_PCAL_hits_in_event_Matched << "\n";
    std::cout << "Total {#(nFD in ECIN), #(LnFD in ECIN)} in Matched events:    " << NumberOfnFD_ECIN_hits_in_event_Matched << ", " << NumberOfLnFD_ECIN_hits_in_event_Matched << "\n";
    std::cout << "Total {#(nFD in ECOUT), #(LnFD in ECOUT)} in Matched events:  " << NumberOfnFD_ECOUT_hits_in_event_Matched << ", " << NumberOfLnFD_ECOUT_hits_in_event_Matched << "\n\n";

    std::cout << "\033[0m" << std::flush;

#pragma endregion

    gDirectory->Clear();

    ++LoopNumber;
}  // end of GammaNeutronFD_separation_test function
