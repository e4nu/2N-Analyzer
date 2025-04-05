#include <vector>

#ifndef Analyzer_CODESETUP_H
#define Analyzer_CODESETUP_H

#define MomResDebugMacro false
#define FullAnalyzer true
#define IndependentCanvasPDF false

#include "../framework/namespaces/setup/path_definitions.h"
#include "../framework/namespaces/setup/debugging.h"
#include "../framework/namespaces/setup/analysis_samples/analysis_samples.h"
// 
#include "../framework/namespaces/general_utilities/basic_tools.h"
#include "../framework/namespaces/general_utilities/constants.h"
#include "../framework/namespaces/plotters/PDF_canvas/PDF_canvas.h"
//
// #include "../framework/classes/Debugger/Debugger.cpp"
#include "../framework/classes/Directories/Directories.cpp"
#include "../framework/classes/ExperimentParameters/ExperimentParameters.cpp"
#include "../framework/functions/HistogramPlottingFunctions.h"
//
// #include "samples/Q2_testing_samples.h"
// #include "samples/Uniform_samples.h"

using namespace PDF_canvas;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code version                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Ver = "DetSim testings";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                    File & path definitions                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace path_definitions;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        Debugger                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace debugging;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                 Samples in sample chain                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace analysis_samples;

// std::vector<std::string> AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v;
// std::string AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile;

// // TODO: get rid of:
// // std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile

// file_name definition -------------------------------------------------------------------------------------------------------------------------------------------------

// // std::string file_name = "12C_2222GeV_GTEST19_10b_00_000";
// std::string file_name = "general_file";

// HIPO files for Q2 study ----------------------------------------------------------------------------------------------------------------------------------------------

// void ConfigSampleChain() {
//     // ==================================================================================================================================================================
//     // Data runs
//     // ==================================================================================================================================================================

//     // H1 ---------------------------------------------------------------------------------------------------------------------------------------------------------------

//     /* H1 2.07052GeV - run 015634 (ifarm DATA files - H1 DATA files) */
//     AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/2gev/H/dst/recon";
//     AnalyzeFileSample = "015634";
//     AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all data files
//     AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     AnalyzeFile_v.push_back(AnalyzeFile);

//     /* H1 5.98636GeV - run 015028 (ifarm DATA files - H1 DATA files) */
//     AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/6gev/H/dst/recon";
//     AnalyzeFileSample = "015028";
//     AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all run files
//     AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     AnalyzeFile_v.push_back(AnalyzeFile);

//     // D2 ---------------------------------------------------------------------------------------------------------------------------------------------------------------

//     /* D2 2.07052GeV - all runs (ifarm DATA files - D2 DATA files) */
//     AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/2gev/D/dst/recon";
//     AnalyzeFileSample = "";
//     AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all data files
//     AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     AnalyzeFile_v.push_back(AnalyzeFile);

//     /* D2 5.98636GeV - run 015443 (ifarm DATA files - D2 DATA files) */
//     AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/6gev/D/dst/recon";
//     AnalyzeFileSample = "015443";
//     AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all run files
//     AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     AnalyzeFile_v.push_back(AnalyzeFile);

//     // C12 --------------------------------------------------------------------------------------------------------------------------------------------------------------

//     /* C12 2.07052GeV - run 015664 (ifarm DATA files - C12 DATA files) */
//     AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/2gev/C/dst/recon";
//     AnalyzeFileSample = "015664";
//     AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     AnalyzeFile = AnalyzeFileDir + "*.hipo";  // all run files
//     AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     AnalyzeFile_v.push_back(AnalyzeFile);

//     /* C12 4.02962GeV - run 015778 (ifarm DATA files - C12 DATA files) */
//     AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/4gev/C/dst/recon";
//     AnalyzeFileSample = "015778";
//     AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all run files
//     AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     AnalyzeFile_v.push_back(AnalyzeFile);

//     /* C12x4 5.98636GeV - run 015188 (ifarm DATA files - C12x4 DATA files) */
//     AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon";
//     AnalyzeFileSample = "015188";
//     AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     // AnalyzeFile = AnalyzeFileDir + "rec_clas_015188.evio.01685-01689.hipo"; // 1 run file
//     AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all run files
//     AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     AnalyzeFile_v.push_back(AnalyzeFile);

//     // Ar40 -------------------------------------------------------------------------------------------------------------------------------------------------------------

//     /* Ar40 2.07052GeV - run 015672 (ifarm DATA files - Ar40 DATA files) */
//     AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon";
//     AnalyzeFileSample = "015672";
//     AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all run files
//     AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     AnalyzeFile_v.push_back(AnalyzeFile);

//     // /* Ar40 4.02962GeV - run 015743 (ifarm DATA files - Ar40 DATA files) */
//     // AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon";
//     // AnalyzeFileSample = "015743";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     // AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all run files
//     // AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     // AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     // AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     // AnalyzeFile_v.push_back(AnalyzeFile);

//     // /* Ar40 5.98636GeV - run 015792 (ifarm DATA files - Ar40 DATA files) */
//     // AnalyzeFilePath = "cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon";
//     // AnalyzeFileSample = "015792";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     // AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all run files
//     // AnalyzeFilePath_v.push_back(AnalyzeFilePath);
//     // AnalyzeFileSample_v.push_back(AnalyzeFileSample);
//     // AnalyzeFileDir_v.push_back(AnalyzeFileDir);
//     // AnalyzeFile_v.push_back(AnalyzeFile);

//     // ==================================================================================================================================================================
//     // Uniform simulation files
//     // ==================================================================================================================================================================
//     // These samples were used to generate the acceptance maps

//     // Electron tester samples ------------------------------------------------------------------------------------------------------------------------------------------

//     // /* Uniform electron tester samples */
//     // Config_Uniform_tester_e_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);

//     // Uniform samples --------------------------------------------------------------------------------------------------------------------------------------------------

//     // // /* Uniform samples @ 2GeV */
//     // // Config_Uniform_1e_2GeV_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);
//     // Config_Uniform_en_2GeV_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);
//     // // Config_Uniform_ep_2GeV_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);

//     // // /* Uniform samples @ 4GeV */
//     // // Config_Uniform_1e_4GeV_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);
//     // Config_Uniform_en_4GeV_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);
//     // // Config_Uniform_ep_4GeV_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);

//     // // /* Uniform samples @ 6GeV */
//     // // Config_Uniform_1e_6GeV_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);
//     // Config_Uniform_en_6GeV_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);
//     // // Config_Uniform_ep_6GeV_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);

//     // Small Q2 test samples --------------------------------------------------------------------------------------------------------------------------------------------

//     // /* C12 G18 varing Q2 4GeV */
//     // Config_C12_G18_4GeV_Q2_test_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);

//     // /* C12 SuSa varing Q2 4GeV */
//     // Config_C12_SuSa_4GeV_Q2_test_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);

//     // /* Ar40 G18 varing Q2 6GeV */
//     // Config_Ar40_G18_6GeV_Q2_test_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);

//     // /* Ar40 SuSa varing Q2 6GeV */
//     // Config_Ar40_SuSa_6GeV_Q2_test_samples(AnalyzeFilePath_v, AnalyzeFileSample_v, AnalyzeFileDir_v, AnalyzeFile_v, AnalyzeFilePath, AnalyzeFileSample, AnalyzeFileDir, AnalyzeFile);

//     // ==================================================================================================================================================================
//     // Old simulation files for local runs
//     // ==================================================================================================================================================================

//     // AnalyzeFilePath = "mnt/d/e4nu/hipo_data_files"; // Storage (D:)
//     // // AnalyzeFilePath = "mnt/h/e4nu/hipo_data_files";                                              // Alon's Passport (F:)
//     // // AnalyzeFilePath = "mnt/g/e4nu/hipo_data_files";                                              // Alon's Portable (G:)
//     // AnalyzeFileSample = "C12x4_G18_02a_00_000_Q204_598636MeV"; // my test with Justin's code; all particle data saved
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     // // AnalyzeFile = AnalyzeFileDir + "*.hipo"; // All files
//     // // AnalyzeFile = AnalyzeFileDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_9_torus-1.0.hipo"; // 1 file
//     // // AnalyzeFile = AnalyzeFileDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_51*.hipo";         // 11 files
//     // // AnalyzeFile = AnalyzeFileDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_5*.hipo";          // 111 files
//     // // AnalyzeFile = AnalyzeFileDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_2*.hipo";          // 512 files

//     // ==================================================================================================================================================================
//     // Other ROOT files
//     // ==================================================================================================================================================================

//     //////<editor-fold desc="root files">
//     // AnalyzeFilePath = "home/alon/project";
//     // AnalyzeFileSample = "Downloads";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     // AnalyzeFile = AnalyzeFileDir + "/" + "12C_2222GeV_GEM21_11a_00_000_wfsi_mk2.root";
//     ////AnalyzeFile = AnalyzeFileDir + "/" + "e_on_1000060120_2222MeV_em.gst.root";
//     ////AnalyzeFile = "./e_on_1000060120_598636MeV.gst.root";

//     ////<editor-fold desc="SuSAv2 fix for thesis">
//     // AnalyzeFilePath = "home/alon/project";
//     // AnalyzeFileSample = "Downloads";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     ////AnalyzeFile = AnalyzeFileDir + "/" + "12C_2222GeV_GTEST19_10b_00_000.root"; // Original problem
//     // AnalyzeFile = AnalyzeFileDir + "/" + "e_on_1000060120_EMMEC_test_myBranch.root"; // Initial fix with 1M events (myBranch)
//     ////</editor-fold>

//     // AnalyzeFilePath = "mnt/d/e4nu/hipo_data_files";
//     // AnalyzeFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";

//     // AnalyzeFilePath = "home/alon/project";
//     // AnalyzeFileSample = "Downloads";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     // AnalyzeFile = AnalyzeFileDir + "e_on_1000060120_2222MeV_em.gst.root";

//     // AnalyzeFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/" + "first_50_test" + "/";

//     // AnalyzeFilePath = "pnfs/genie/scratch/users/asportes/script_jobs/detector_sim_truth_level";
//     // AnalyzeFileSample = "detector_sim_truth_level/C12_G18_10a_02_11b_2GeV_QEL_test";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     ////AnalyzeFile = AnalyzeFileDir + "Ca48_G18_10a_02_11b_207052MeV.root";
//     // AnalyzeFile = AnalyzeFileDir + "e_on_1000060120_2070MeV.root";

//     // AnalyzeFilePath = "home/alon/project";
//     // AnalyzeFileSample = "Downloads";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     ////AnalyzeFile = AnalyzeFileDir + "Ca48_G18_10a_02_11b_207052MeV.root";
//     // AnalyzeFile = AnalyzeFileDir + "Ca48_G18_10a_02_11b_Q205_598636MeV.root";

//     // AnalyzeFilePath = "lustre19/expphy/volatile/clas12/asportes/truthLevelROOTFiles";
//     // AnalyzeFileSample = "C12_G18_10a_02_11b_207052MeV";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     // AnalyzeFile = AnalyzeFileDir + "C12_G18_10a_02_11b_207052MeV.root";

//     // AnalyzeFilePath = "mnt/d/e4nu/root_data_files/Detector_sim_truth_level_files";
//     // AnalyzeFileSample = "598636MeV_Q2_0_5";
//     // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
//     //
//     ////AnalyzeFile = AnalyzeFileDir + "e_on_1000060120_598636MeV_Q2_0_5.gst.root";
//     ////AnalyzeFile = "./nu_SuSAv2_fix_test_12C_2222GeV_CC_myBranch_10M.root";
//     // AnalyzeFile = "./nu_SuSAv2_fix_test_12C_2222GeV_CCMEC_myBranch_10M.root";
// }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                    Other parameters                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//<editor-fold desc="Other parameters">

// ======================================================================================================================================================================
// BEnergyToNucleus definition
// ======================================================================================================================================================================

double BEnergyToNucleus;

// ======================================================================================================================================================================
// Simulation parameters extraction
// ======================================================================================================================================================================

int Target_pdg, Probe_pdg;
std::string Target_nucleus, Probe;
double BeamEnergy;

// ======================================================================================================================================================================
// Histogram ranges
// ======================================================================================================================================================================

//<editor-fold desc="Histogram range variables">

////<editor-fold desc="Chi2 plots">
// double Chi2_upper_lim, Chi2_lower_lim;
////</editor-fold>

//<editor-fold desc="Vertex plots">
double Vertex_upper_lim, Vertex_lower_lim, dV_upper_lim, dV_lower_lim;
//</editor-fold>

//<editor-fold desc="Theta histograms">

//<editor-fold desc="Theta histograms (2p)">
double theta_lp_upper_lim_2p, theta_lp_lower_lim_2p, theta_p1_upper_lim_2p, theta_p1_lower_lim_2p, theta_p2_upper_lim_2p, theta_p2_lower_lim_2p;
double dtheta_upper_lim_2p, dtheta_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Theta histograms (1n1p)">
double theta_lp_upper_lim_1n1p, theta_lp_lower_lim_1n1p, theta_p_upper_lim_1n1p, theta_p_lower_lim_1n1p, theta_n_upper_lim_1n1p, theta_n_lower_lim_1n1p;
double dtheta_upper_lim_1n1p, dtheta_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Phi histograms">

//<editor-fold desc="Phi histograms (2p)">
double phi_lp_upper_lim_2p, phi_lp_lower_lim_2p, phi_p1_upper_lim_2p, phi_p1_lower_lim_2p, phi_p2_upper_lim_2p, phi_p2_lower_lim_2p;
double dphi_upper_lim_2p, dphi_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Phi histograms (1n1p)">
double phi_lp_upper_lim_1n1p, phi_lp_lower_lim_1n1p, phi_p_upper_lim_1n1p, phi_p_lower_lim_1n1p, phi_n_upper_lim_1n1p, phi_n_lower_lim_1n1p;
double dphi_upper_lim_1n1p, dphi_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy histograms">

//<editor-fold desc="Energy histograms (2p)">
double fsEl_upper_lim_2p, fsEl_lower_lim_2p;
double fsEl_QEL_upper_lim_2p, fsEl_QEL_lower_lim_2p, fsEl_MEC_upper_lim_2p, fsEl_MEC_lower_lim_2p;
double fsEl_RES_upper_lim_2p, fsEl_RES_lower_lim_2p, fsEl_DIS_upper_lim_2p, fsEl_DIS_lower_lim_2p;
double fsEl_VS_theta_lp_upper_lim_2p_x, fsEl_VS_theta_lp_lower_lim_2p_x, fsEl_VS_theta_lp_upper_lim_2p_y, fsEl_VS_theta_lp_lower_lim_2p_y;
//</editor-fold>

//<editor-fold desc="Energy histograms (1n1p)">
double fsEl_upper_lim_1n1p, fsEl_lower_lim_1n1p;
double fsEl_QEL_upper_lim_1n1p, fsEl_QEL_lower_lim_1n1p, fsEl_MEC_upper_lim_1n1p, fsEl_MEC_lower_lim_1n1p;
double fsEl_RES_upper_lim_1n1p, fsEl_RES_lower_lim_1n1p, fsEl_DIS_upper_lim_1n1p, fsEl_DIS_lower_lim_1n1p;
double fsEl_VS_theta_lp_upper_lim_1n1p_x, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_y, fsEl_VS_theta_lp_lower_lim_1n1p_y;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms">

//<editor-fold desc="Energy Transfer histograms (all interactions)">

//<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
double E_Trans_all_ang_all_int_upper_lim_2p, E_Trans_all_ang_all_int_lower_lim_2p;
double E_Trans15_all_upper_lim_2p, E_Trans15_all_lower_lim_2p, E_Trans45_all_upper_lim_2p, E_Trans45_all_lower_lim_2p, E_Trans90_all_upper_lim_2p, E_Trans90_all_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
double E_Trans_all_ang_all_int_upper_lim_1n1p, E_Trans_all_ang_all_int_lower_lim_1n1p;
double E_Trans15_all_upper_lim_1n1p, E_Trans15_all_lower_lim_1n1p, E_Trans45_all_upper_lim_1n1p, E_Trans45_all_lower_lim_1n1p, E_Trans90_all_upper_lim_1n1p, E_Trans90_all_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only)">

//<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
double E_Trans15_QEL_upper_lim_2p, E_Trans15_QEL_lower_lim_2p, E_Trans45_QEL_upper_lim_2p, E_Trans45_QEL_lower_lim_2p, E_Trans90_QEL_upper_lim_2p, E_Trans90_QEL_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
double E_Trans15_QEL_upper_lim_1n1p, E_Trans15_QEL_lower_lim_1n1p, E_Trans45_QEL_upper_lim_1n1p, E_Trans45_QEL_lower_lim_1n1p, E_Trans90_QEL_upper_lim_1n1p, E_Trans90_QEL_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only)">

//<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
double E_Trans15_MEC_upper_lim_2p, E_Trans15_MEC_lower_lim_2p, E_Trans45_MEC_upper_lim_2p, E_Trans45_MEC_lower_lim_2p, E_Trans90_MEC_upper_lim_2p, E_Trans90_MEC_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
double E_Trans15_MEC_upper_lim_1n1p, E_Trans15_MEC_lower_lim_1n1p, E_Trans45_MEC_upper_lim_1n1p, E_Trans45_MEC_lower_lim_1n1p, E_Trans90_MEC_upper_lim_1n1p, E_Trans90_MEC_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only)">

//<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
double E_Trans15_RES_upper_lim_2p, E_Trans15_RES_lower_lim_2p, E_Trans45_RES_upper_lim_2p, E_Trans45_RES_lower_lim_2p, E_Trans90_RES_upper_lim_2p, E_Trans90_RES_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
double E_Trans15_RES_upper_lim_1n1p, E_Trans15_RES_lower_lim_1n1p, E_Trans45_RES_upper_lim_1n1p, E_Trans45_RES_lower_lim_1n1p, E_Trans90_RES_upper_lim_1n1p, E_Trans90_RES_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only)">

//<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
double E_Trans15_DIS_upper_lim_2p, E_Trans15_DIS_lower_lim_2p, E_Trans45_DIS_upper_lim_2p, E_Trans45_DIS_lower_lim_2p, E_Trans90_DIS_upper_lim_2p, E_Trans90_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
double E_Trans15_DIS_upper_lim_1n1p, E_Trans15_DIS_lower_lim_1n1p, E_Trans45_DIS_upper_lim_1n1p, E_Trans45_DIS_lower_lim_1n1p, E_Trans90_DIS_upper_lim_1n1p, E_Trans90_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//</editor-fold>

//<editor-fold desc="Inclusive Energy Transfer histograms">
double E_Trans15_all_inclusive_upper_lim, E_Trans15_all_inclusive_lower_lim;
double E_Trans15_QEL_inclusive_upper_lim, E_Trans15_QEL_inclusive_lower_lim, E_Trans15_MEC_inclusive_upper_lim, E_Trans15_MEC_inclusive_lower_lim;
double E_Trans15_RES_inclusive_upper_lim, E_Trans15_RES_inclusive_lower_lim, E_Trans15_DIS_inclusive_upper_lim, E_Trans15_DIS_inclusive_lower_lim;
double E_Trans_VS_q_all_inclusive_upper_lim_x, E_Trans_VS_q_all_inclusive_lower_lim_x, E_Trans_VS_q_all_inclusive_upper_lim_y, E_Trans_VS_q_all_inclusive_lower_lim_y;
double E_Trans_VS_q_QEL_inclusive_upper_lim_x, E_Trans_VS_q_QEL_inclusive_lower_lim_x, E_Trans_VS_q_QEL_inclusive_upper_lim_y, E_Trans_VS_q_QEL_inclusive_lower_lim_y;
double E_Trans_VS_q_MEC_inclusive_upper_lim_x, E_Trans_VS_q_MEC_inclusive_lower_lim_x, E_Trans_VS_q_MEC_inclusive_upper_lim_y, E_Trans_VS_q_MEC_inclusive_lower_lim_y;

double E_Trans_VS_q3_all_upper_lim_x_2p, E_Trans_VS_q3_all_lower_lim_x_2p, E_Trans_VS_q3_all_upper_lim_y_2p, E_Trans_VS_q3_all_lower_lim_y_2p;
double E_Trans_VS_q3_QEL_upper_lim_x_2p, E_Trans_VS_q3_QEL_lower_lim_x_2p, E_Trans_VS_q3_QEL_upper_lim_y_2p, E_Trans_VS_q3_QEL_lower_lim_y_2p;
double E_Trans_VS_q3_MEC_upper_lim_x_2p, E_Trans_VS_q3_MEC_lower_lim_x_2p, E_Trans_VS_q3_MEC_upper_lim_y_2p, E_Trans_VS_q3_MEC_lower_lim_y_2p;

double E_Trans_VS_q3_all_upper_lim_x_1n1p, E_Trans_VS_q3_all_lower_lim_x_1n1p, E_Trans_VS_q3_all_upper_lim_y_1n1p, E_Trans_VS_q3_all_lower_lim_y_1n1p;
double E_Trans_VS_q3_QEL_upper_lim_x_1n1p, E_Trans_VS_q3_QEL_lower_lim_x_1n1p, E_Trans_VS_q3_QEL_upper_lim_y_1n1p, E_Trans_VS_q3_QEL_lower_lim_y_1n1p;
double E_Trans_VS_q3_MEC_upper_lim_x_1n1p, E_Trans_VS_q3_MEC_lower_lim_x_1n1p, E_Trans_VS_q3_MEC_upper_lim_y_1n1p, E_Trans_VS_q3_MEC_lower_lim_y_1n1p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms">

//<editor-fold desc="E_cal range">
double E_cal_QEL_upper_lim_range, E_cal_QEL_lower_lim_range, E_cal_MEC_upper_lim_range, E_cal_MEC_lower_lim_range;
double E_cal_RES_upper_lim_range, E_cal_RES_lower_lim_range, E_cal_DIS_upper_lim_range, E_cal_DIS_lower_lim_range;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (2p)">
double E_cal_QEL_upper_lim_2p, E_cal_QEL_lower_lim_2p, E_cal_MEC_upper_lim_2p, E_cal_MEC_lower_lim_2p;
double E_cal_RES_upper_lim_2p, E_cal_RES_lower_lim_2p, E_cal_DIS_upper_lim_2p, E_cal_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (1n1p)">
double E_cal_QEL_upper_lim_1n1p, E_cal_QEL_lower_lim_1n1p, E_cal_MEC_upper_lim_1n1p, E_cal_MEC_lower_lim_1n1p;
double E_cal_RES_upper_lim_1n1p, E_cal_RES_lower_lim_1n1p, E_cal_DIS_upper_lim_1n1p, E_cal_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Momentum histograms">

//<editor-fold desc="Momentum histograms (2p)">
double P_L_hist_upper_lim_2p, P_L_hist_lower_lim_2p, P_R_hist_upper_lim_2p, P_R_hist_lower_lim_2p, P_lp_hist_upper_lim_2p, P_lp_hist_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Momentum histograms (1n1p)">
double P_p_hist_upper_lim_1n1p, P_p_hist_lower_lim_1n1p, P_n_hist_upper_lim_1n1p, P_n_hist_lower_lim_1n1p, P_lp_hist_upper_lim_1n1p, P_lp_hist_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="MicroBooNE plots">

//<editor-fold desc="MicroBooNE gamma plots (unweighted)">
double Gamma_Lab_upper_lim, Gamma_Lab_lower_lim, Gamma_mu_p_tot_upper_lim, Gamma_mu_p_tot_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (Q4 weighted)">
double Gamma_Lab_weighted_upper_lim, Gamma_Lab_weighted_lower_lim, Gamma_mu_p_tot_weighted_upper_lim, Gamma_mu_p_tot_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (all int., no pions)">
double Gamma_Lab_noPions_All_Int_upper_lim, Gamma_Lab_noPions_All_Int_lower_lim, Gamma_Lab_noPions_All_Int_weighted_upper_lim, Gamma_Lab_noPions_All_Int_weighted_lower_lim;
double Gamma_Lab_noPions_QEL_upper_lim, Gamma_Lab_noPions_QEL_lower_lim, Gamma_Lab_noPions_QEL_weighted_upper_lim, Gamma_Lab_noPions_QEL_weighted_lower_lim;
double Gamma_Lab_noPions_MEC_upper_lim, Gamma_Lab_noPions_MEC_lower_lim, Gamma_Lab_noPions_MEC_weighted_upper_lim, Gamma_Lab_noPions_MEC_weighted_lower_lim;
double Gamma_Lab_noPions_RES_upper_lim, Gamma_Lab_noPions_RES_lower_lim, Gamma_Lab_noPions_RES_weighted_upper_lim, Gamma_Lab_noPions_RES_weighted_lower_lim;
double Gamma_Lab_noPions_DIS_upper_lim, Gamma_Lab_noPions_DIS_lower_lim, Gamma_Lab_noPions_DIS_weighted_upper_lim, Gamma_Lab_noPions_DIS_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE dP_T plots">
double dP_T_hist_upper_lim, dP_T_hist_lower_lim, dP_T_hist_weighted_upper_lim, dP_T_hist_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
double P_L_hist_upper_lim, P_L_hist_lower_lim, P_R_hist_upper_lim, P_R_hist_lower_lim, P_lp_hist_upper_lim, P_lp_hist_lower_lim, P_pion_hist_upper_lim, P_pion_hist_lower_lim;
//</editor-fold>

//</editor-fold>

//</editor-fold>

//</editor-fold>

#endif  // Analyzer_CODESETUP_H
