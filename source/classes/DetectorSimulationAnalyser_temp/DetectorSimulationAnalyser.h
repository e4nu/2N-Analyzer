/*

 git pull && clas12root -b -q main.c

 * Copy commend (local -> JLab):
scp -r /home/alon/project/temp/BankHist/Analyser.C -J asportes@ftp.jlab.org:/u/home/asportes/clas12project/

 * Copy commend (JLab -> local):
scp -r asportes@ftp.jlab.org:/u/home/asportes/Analyser/plots -J /home/alon/project/temp/
scp -r asportes@ftp.jlab.org:/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo -J /home/alon/project/temp/

 */

#include "../setup/codeSetup.h"
// #include "TempInclude/DetectorSimulationAnalyser_Histograms_Temp.C" //TODO: move to a class!
#include "../source/classes/AMaps/AMaps.cpp"
#include "../source/classes/DEfficiency/DEfficiency.cpp"
#include "../source/classes/DSCuts/DSCuts.h"
#include "../source/classes/clas12ana/clas12ana.h"
#include "../source/classes/hPlots/hPlot1D.cpp"
#include "../source/classes/hPlots/hPlot2D.cpp"

#if !defined(MOMENTUMRESOLUTION_H)
#include "../source/classes/MomentumResolution/MomentumResolution.cpp"
#endif

#if !defined(PARTICLEID_H)
#include "../source/classes/ParticleID/ParticleID.cpp"
#endif

#include "../source/classes/Settings/Settings.cpp"
#include "../source/classes/TLCuts/TLCuts.cpp"
#include "../source/functions/AngleCalc/CalcdPhi.h"
#include "../source/functions/AngleCalc/GetBinFromAng.h"
#include "../source/functions/DrawAndSaveFunctions/DrawAndSaveACorrPlots.h"
#include "../source/functions/DrawAndSaveFunctions/DrawAndSaveEfficiencyPlots.h"
#include "../source/functions/DrawAndSaveFunctions/DrawAndSaveFSRatio.h"
#include "../source/functions/FillByInt.h"
#include "../source/functions/FitFunctions/BetaFit.h"
#include "../source/functions/FitFunctions/BetaFitApprax.h"
#include "../source/functions/GetPi0MomTh.h"
#include "../source/functions/StackPlot3.h"
#include "../source/functions/StackPlot4.h"
#include "../source/functions/TLKinCutsCheck.h"
#include "../source/namespaces/general_utilities/utilities/utilities.cpp"
#include "HipoChain.h"
#include "clas12reader.h"

using namespace std;
using namespace clas12;
using namespace utilities;

class DetectorSimulationAnalyser {
   public:
    DetectorSimulationAnalyser() = default;

    void EventAnalyser(const std::string &AnalyseFilePath, const std::string &AnalyseFileSample, const std::string &AnalyseFile);

   private:
    std::string LoadedInput;
    std::string filePath;
    std::string fileInput;
    std::string plotsInput;

    ExperimentParameters Experiment;

    std::string SampleName, VaryingSampleName;

    double beamE;

    std::string Target;

    int TargetPDG;
    bool isLocal;
    bool isMC, isData;
    bool is2GeVSample, is4GeVSample, is6GeVSample;

    // Final states to analyze (1N & 2N)
    bool calculate_1p, calculate_1n;
    bool calculate_2p, calculate_pFDpCD, calculate_nFDpCD;

    // Truth level calculation setup
    bool calculate_truth_level;
    bool TL_plots_only_for_NC;
    bool fill_TL_plots;
    bool ZoomIn_On_mom_th_plots;
    bool Eff_calc_with_one_reco_electron;
    bool Calc_inc_eff_with_varying_theta;
    bool Calc_1n_n_eff_with_smaller_theta;
    bool Calc_eff_overlapping_FC;
    bool Rec_wTL_ES;

    bool limless_mom_eff_plots;

    // FD neutrals settings
    bool Enable_FD_photons;
    bool Enable_FD_neutrons;
    bool Count_FD_neurton_and_photon_hits;

    bool ES_by_leading_FDneutron;

    // Acceptance maps setup
    bool Generate_Electron_AMaps;
    bool Generate_Nucleon_AMaps;
    bool Generate_WMaps;
    bool AMaps_calc_with_one_reco_electron;
    std::string P_e_bin_profile;
    std::string P_nuc_bin_profile;
    bool Electron_single_slice_test;
    bool Nucleon_single_slice_test;
    std::vector<int> TestSlices;

    // Neutron resolution setup
    bool plot_and_fit_MomRes;
    bool Calculate_momResS2;
    double DeltaSlices;
    bool VaryingDelta;
    bool ForceSmallpResLimits;
    std::string SmearMode;
    std::string CorrMode;
    bool Run_with_momResS2;
    bool momRes_test;

    // clas12ana cuts
    bool apply_cuts;
    bool clas12ana_particles;
    bool only_preselection_cuts;
    bool only_electron_quality_cuts;

    // Preselection cuts (event cuts)
    bool apply_preselection_cuts;
    bool apply_Vz_e_cuts;
    bool apply_Vz_cuts, apply_dVz_cuts;
    bool apply_DC_e_fiducial_cuts;
    bool apply_DC_fiducial_cuts;

    // Electron quality cuts
    bool apply_electron_quality_cuts;
    bool apply_Nphe_cut;
    bool apply_ECAL_SF_cuts;
    bool apply_ECAL_P_cuts;
    bool apply_ECAL_fiducial_cuts;
    bool apply_Electron_beta_cut;

    // Chi2 cuts (= PID cuts)
    bool apply_chi2_cuts_1e_cut;

    // My analysis cuts
    // Nucleon cuts
    bool apply_nucleon_cuts;

    // Physical cuts
    bool apply_nucleon_physical_cuts;
    bool apply_nBeta_fit_cuts;
    bool apply_fiducial_cuts;
    bool apply_kinematical_cuts;
    bool apply_kinematical_weights;
    bool apply_nucleon_SmearAndCorr;

    const bool custom_cuts_naming;
    std::string run_plots_path;
    std::string run_plots_log_save_Directory;

    // Add any necessary member variables here
    Settings settings;

    // clas12ana cuts
    DSCuts Nphe_cuts_FD;
    DSCuts Chi2_Electron_cuts_CD, Chi2_Electron_cuts_FD;
    DSCuts Chi2_Proton_cuts_CD, Chi2_Proton_cuts_FD;
    DSCuts Chi2_piplus_cuts_CD, Chi2_piplus_cuts_FD;
    DSCuts Chi2_piminus_cuts_CD, Chi2_piminus_cuts_FD;
    DSCuts Chi2_Kplus_cuts_CD, Chi2_Kplus_cuts_FD;
    DSCuts Chi2_Kminus_cuts_CD, Chi2_Kminus_cuts_FD;
    DSCuts Chi2_deuteron_cuts_CD, Chi2_deuteron_cuts_FD;
    DSCuts Vz_cut, Vz_cut_FD, Vz_cut_CD;
    DSCuts dVz_cuts, dVz_cuts_FD, dVz_cuts_CD;
    DSCuts SF_cuts;
    DSCuts PCAL_edge_cuts;
    DSCuts DC_e_edge_cuts, DC_edge_cuts;
    DSCuts e_mom_th, no_p_mom_th, pip_mom_th, pim_mom_th, ph_mom_th;
    DSCuts p_mom_th, n_mom_th;

    // Other cuts

    /* Neutron beta cuts (1n & nFDpCD, FD only) */
    DSCuts Beta_max_cut_ABF_FD_n_from_ph;
    DSCuts Beta_max_cut_ABF_FD_n_from_ph_apprax;

    /* Neutron momentum cuts (1n & nFDpCD, FD only) */
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph;
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_apprax;

    /* Truth-level momentum cuts */
    DSCuts TL_e_mom_cuts;
    DSCuts TL_n_mom_cuts;
    DSCuts TL_p_mom_cuts;
    DSCuts TL_pFD_mom_cuts;
    DSCuts TL_pCD_mom_cuts;
    DSCuts TL_pip_mom_cuts;
    DSCuts TL_pipFD_mom_cuts;
    DSCuts TL_pipCD_mom_cuts;
    DSCuts TL_pim_mom_cuts;
    DSCuts TL_pimFD_mom_cuts;
    DSCuts TL_pimCD_mom_cuts;
    DSCuts TL_pi0_mom_cuts;
    DSCuts TL_ph_mom_cuts;

    /* FD & CD theta range */
    DSCuts ThetaFD;
    DSCuts ThetaCD;

    /* FD & CD theta range (for AMaps) */
    DSCuts ThetaFD_AMaps;

    /* Beta cut (1n, FD) */
    DSCuts Beta_cut;

    /* Neutron veto cut (1n & nFDpCD, FD) */
    DSCuts Neutron_veto_cut;

    /* Ghost tracks handling (2p & pFDpCD, CD & FD) */
    DSCuts Theta_p1_cuts_2p;
    DSCuts Theta_p2_cuts_2p;
    DSCuts dphi_p1_p2_2p;

    DSCuts Theta_pFD_cuts_2p;
    DSCuts Theta_pCD_cuts_2p;
    DSCuts dphi_pFD_pCD_2p;

    DSCuts Theta_pFD_cuts_pFDpCD;
    DSCuts Theta_pCD_cuts_pFDpCD;
    DSCuts dphi_pFD_pCD_pFDpCD;

    DSCuts Theta_L_cuts_nFDpCD;
    DSCuts Theta_R_cuts_nFDpCD;
    DSCuts dphi_L_R_nFDpCD;

    /* reco. kinematic cuts (based on nucleons' efficiency) */
    DSCuts FD_nucleon_theta_cut;
    DSCuts FD_nucleon_momentum_cut;

    DSCuts MomRes_mu_cuts;
    DSCuts MomRes_sigma_cuts;

    std::string Plots_Folder;
    const bool Clear_Old_Directories;

    Directories directories;

    bool Save_Plots_folder_to_zip_files;

    TList *plots;
    const char *TListName;

    const char *Histogram_OutPDF;

    bool Plot_selector_master;

    bool Cut_plots_master;
    bool Nphe_plots, Chi2_plots, Vertex_plots, SF_plots, fiducial_plots, Momentum_plots;

    bool W_plots;

    bool Beta_plots;
    bool Beta_vs_P_plots;

    bool Angle_plots_master;
    bool Theta_e_plots, Phi_e_plots;

    bool Momentum_transfer_plots;

    bool E_e_plots;

    bool ETrans_plots_master;
    bool ETrans_all_plots, ETrans_All_Int_plots, ETrans_QEL_plots, ETrans_MEC_plots, ETrans_RES_plots, ETrans_DIS_plots;

    bool Ecal_plots;

    bool TKI_plots;

    bool ToF_plots;

    bool Efficiency_plots;
    bool TL_after_Acceptance_Maps_plots;

    bool AMaps_plots;
    bool WMaps_plots;

    bool Resolution_plots;

    bool Multiplicity_plots;

    bool FSR_1D_plots, FSR_2D_plots;

    bool TestRun;
    bool ApplyLimiter;
    int Limiter;

    bool wider_margin;
    bool debug_plots;

    bool Log_scale_Vx_plots, Log_scale_Vy_plots, Log_scale_Vz_plots;
    bool Log_scale_dVx_plots, Log_scale_dVy_plots, Log_scale_dVz_plots;

    bool normalize_master;

    bool norm_Nphe_plots, norm_Chi2_plots, norm_Vertex_plots, norm_SF_plots, norm_Fiducial_plots, norm_Momentum_plots;

    bool norm_W_plots, norm_Beta_plots, norm_Angle_plots_master, norm_Momentum_transfer_plots, norm_E_e_plots, norm_ET_plots;
    bool norm_Ecal_plots, norm_TKI_plots, norm_MomRes_plots, norm_Multi_plots;

    const bool delete_png_files = true, delete_root_files = true, delete_txt_files = true;

    // Histogram setup
    int numTH1Dbins;
    int numTH2Dbins;
    int numTH2Dbins_Mom_Plots;
    int numTH1Dbins_Beta_Plots;
    int numTH2Dbins_Beta_Plots;
    int numTH1Dbins_W_Plots;
    int numTH2Dbins_W_Plots;
    int numTH1Dbins_Ang_Plots;
    int numTH2Dbins_Ang_Plots;
    int numTH2Dbins_Electron_Ang_Plots;
    int numTH2Dbins_Nucleon_Ang_Plots;
    int numTH1Dbins_Momentum_transfer_plots;
    int numTH2Dbins_Momentum_transfer_plots;
    int numTH1Dbins_E_e_Plots;
    int numTH2Dbins_E_e_Plots;
    int numTH1Dbins_ET_Plots;
    int numTH1Dbins_E_cal_Plots;
    int numTH2Dbins_E_cal_Plots;
    int numTH1Dbins_TKI_dP_T_Plots;
    int numTH1Dbins_TKI_dAlpha_T_Plots;
    int numTH2Dbins_TKI_Plots;
    int numTH1Dbins_Mom_eff_Plots;
    int numTH1Dbins_Ang_eff_Plots;
    int numTH2Dbins_Ang_eff_Plots;
    int numTH2Dbins_Electron_Ang_eff_Plots;
    int numTH2Dbins_Nucleon_Ang_eff_Plots;
    int numTH1Dbins_AMaps_Plots;
    int numTH2Dbins_AMaps_Plots;
    int numTH2Dbins_Electron_AMaps_Plots;
    int numTH2Dbins_Nucleon_AMaps_Plots;
    int numTH1Dbins_nRes_Plots;
    int numTH2Dbins_nRes_Plots;
    int numTH1Dbins_multi_Plots;

    // Histogram limits
    const double Nphe_boundary;
    double Chi2_boundary;
    double Vertex_boundary, Vertex_uboundary, Vertex_lboundary;
    double Vertex_boundary_FD, Vertex_uboundary_FD, Vertex_lboundary_FD;
    double Vertex_boundary_CD, Vertex_uboundary_CD, Vertex_lboundary_CD;
    double dV_boundary, dV_uboundary, dV_lboundary;
    double dV_boundary_FD, dV_uboundary_FD, dV_lboundary_FD;
    double dV_boundary_CD, dV_uboundary_CD, dV_lboundary_CD;
    double SF_uboundary, SF_lboundary;
    const double Momentum_lboundary, Momentum_uboundary;
    double FDMomentum_lboundary, FDMomentum_uboundary;
    const double CDMomentum_lboundary, CDMomentum_uboundary;
    double P_nucFD_lboundary, P_nucFD_uboundary;
    double P_nucCD_lboundary, P_nucCD_uboundary;
    const double W_lboundary;
    const double W_uboundary;
    const double dBeta_sigma_boundary;
    const double Beta_dist_uboundary, Beta_dist_lboundary;
    const double dBeta_sigma_ZOOMOUT_boundary;
    const double Beta_dist_ZOOMOUT_uboundary, Beta_dist_ZOOMOUT_lboundary;
    double Beta_boundary_const, Beta_boundary, P_boundary;
    const double Theta_lboundary_FD, Theta_uboundary_FD;
    const double Theta_lboundary_CD, Theta_uboundary_CD;
    const double Opening_Ang_wide_lboundary, Opening_Ang_wide_uboundary;
    const double Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary;
    const double Phi_lboundary, Phi_uboundary;
    double Q2_lboundary_FD, Q2_uboundary_FD;
    const double dP_T_boundary;

    AMaps aMaps_master, wMaps_master;
    // TODO: UPDATE AMaps loading constructor electron histogram's number of bins

    // Flags and efficiency object
    bool save_ACorr_data;

    // Efficiency object
    DEfficiency eff;

    // List for ACorr data
    TList *ACorr_data;

    // Directory and file name strings
    std::string ACorr_data_Dir;
    std::string ACorr_data_listName;

    // File name as a C-string
    const char *ACorr_DataName;

    // Neutron resolution class declaration & definition
    MomentumResolution nRes("Neutron"), pRes("Proton");

    const bool GoodProtonsMonitorPlots;

    const bool PrintEvents;
    const int Ne_in_event, Nf_in_event, nEvents2print;

    ofstream EventPrint;
    std::string EventPrint_save_Directory;

    ParticleID pid;
    std::string AnalyserVersion = "Version 1.10";
    // ...existing code...

    TCanvas *c1;
    
    void CodeSetup(const std::string &AnalyseFilePath, const std::string &AnalyseFileSample, const std::string &AnalyseFile);

    void InputProcessing(const std::string &AnalyseFilePath, const std::string &AnalyseFileSample, const std::string &AnalyseFile);

    void EventSelectionSetup();

    void CutSetup();

    void PrintCuts();

    void DefineCuts();

    void DirectorySetup();

    void HistogramSetup();

    void RefreshNumOfBins();

    void RefresHistogramLimits();

    void PlotSelectorSetup();

    void NormalizationSetup();

    void DeleteSetup();

    void AcceptanceMapsSetup();

    void AcceptanceCorrectionSetup();

    void NeutronResolutionSetup();

    void DebuggingSetup();

    void ProcessEvents();

    void SavePlots();

    void SaveLog();
};