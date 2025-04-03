//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef ANALYSIS_SAMPLES_H
#define ANALYSIS_SAMPLES_H

#include <iostream>

#include "data_samples.h"
#include "uniform_samples.h"

namespace analysis_samples {
using namespace data_samples;
using namespace uniform_samples;

std::vector<std::string> AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v;
std::string AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile;

// TODO: get rid of:
// std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile

// file_name definition -------------------------------------------------------------------------------------------------------------------------------------------------

std::string file_name = "general_file";

// HIPO files for Q2 study ----------------------------------------------------------------------------------------------------------------------------------------------

void ConfigSampleChain() {
    // ==================================================================================================================================================================
    // Data runs
    // ==================================================================================================================================================================

    // H1 ---------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* H1 2.07052GeV - run 015634 (ifarm DATA files - H1 DATA files) */
    // Config_H1_2GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // // /* H1 5.98636GeV - run 015028 (ifarm DATA files - H1 DATA files) */
    // Config_H1_6GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // D2 ---------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* D2 2.07052GeV - all runs (ifarm DATA files - D2 DATA files) */
    // Config_D2_2GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // // /* D2 5.98636GeV - run 015443 (ifarm DATA files - D2 DATA files) */
    // Config_D2_6GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // C12 --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* C12 2.07052GeV - run 015664 (ifarm DATA files - C12 DATA files) */
    // Config_C12_2GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // /* C12 4.02962GeV - run 015778 (ifarm DATA files - C12 DATA files) */
    // Config_C12_4GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // // /* C12x4 5.98636GeV - run 015188 (ifarm DATA files - C12x4 DATA files) */
    // Config_C12_6GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // Ar40 -------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* Ar40 2.07052GeV - run 015672 (ifarm DATA files - Ar40 DATA files) */
    // Config_Ar40_2GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // // /* Ar40 4.02962GeV - run 015743 (ifarm DATA files - Ar40 DATA files) */
    // Config_Ar40_4GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // // /* Ar40 5.98636GeV - run 015792 (ifarm DATA files - Ar40 DATA files) */
    // Config_Ar40_6GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // ==================================================================================================================================================================
    // Uniform simulation files
    // ==================================================================================================================================================================
    // These samples were used to generate the acceptance maps

    // Electron tester samples ------------------------------------------------------------------------------------------------------------------------------------------

    // /* Uniform electron tester samples */
    // Config_Uniform_tester_e_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // Uniform samples --------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* Uniform samples @ 2GeV */
    // // Config_Uniform_1e_2GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);
    Config_Uniform_en_2GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);
    // // Config_Uniform_ep_2GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // // /* Uniform samples @ 4GeV */
    // // Config_Uniform_1e_4GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);
    Config_Uniform_en_4GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);
    // // Config_Uniform_ep_4GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // // /* Uniform samples @ 6GeV */
    // // Config_Uniform_1e_6GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);
    Config_Uniform_en_6GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);
    // // Config_Uniform_ep_6GeV_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // Small Q2 test samples --------------------------------------------------------------------------------------------------------------------------------------------

    // /* C12 G18 varing Q2 4GeV */
    // Config_C12_G18_4GeV_Q2_test_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // /* C12 SuSa varing Q2 4GeV */
    // Config_C12_SuSa_4GeV_Q2_test_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // /* Ar40 G18 varing Q2 6GeV */
    // Config_Ar40_G18_6GeV_Q2_test_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // /* Ar40 SuSa varing Q2 6GeV */
    // Config_Ar40_SuSa_6GeV_Q2_test_samples(AnalyseFilePath_v, AnalyseFileSample_v, AnalyseFileDir_v, AnalyseFile_v, AnalyseFilePath, AnalyseFileSample, AnalyseFileDir, AnalyseFile);

    // ==================================================================================================================================================================
    // Old simulation files for local runs
    // ==================================================================================================================================================================

    // AnalyseFilePath = "mnt/d/e4nu/hipo_data_files"; // Storage (D:)
    // // AnalyseFilePath = "mnt/h/e4nu/hipo_data_files";                                              // Alon's Passport (F:)
    // // AnalyseFilePath = "mnt/g/e4nu/hipo_data_files";                                              // Alon's Portable (G:)
    // AnalyseFileSample = "C12x4_G18_02a_00_000_Q204_598636MeV"; // my test with Justin's code; all particle data saved
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
    // // AnalyseFile = AnalyseFileDir + "*.hipo"; // All files
    // // AnalyseFile = AnalyseFileDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_9_torus-1.0.hipo"; // 1 file
    // // AnalyseFile = AnalyseFileDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_51*.hipo";         // 11 files
    // // AnalyseFile = AnalyseFileDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_5*.hipo";          // 111 files
    // // AnalyseFile = AnalyseFileDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_2*.hipo";          // 512 files

    // ==================================================================================================================================================================
    // Other ROOT files
    // ==================================================================================================================================================================

    //////<editor-fold desc="root files">
    // AnalyseFilePath = "home/alon/project";
    // AnalyseFileSample = "Downloads";
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
    // AnalyseFile = AnalyseFileDir + "/" + "12C_2222GeV_GEM21_11a_00_000_wfsi_mk2.root";
    ////AnalyseFile = AnalyseFileDir + "/" + "e_on_1000060120_2222MeV_em.gst.root";
    ////AnalyseFile = "./e_on_1000060120_598636MeV.gst.root";

    ////<editor-fold desc="SuSAv2 fix for thesis">
    // AnalyseFilePath = "home/alon/project";
    // AnalyseFileSample = "Downloads";
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
    ////AnalyseFile = AnalyseFileDir + "/" + "12C_2222GeV_GTEST19_10b_00_000.root"; // Original problem
    // AnalyseFile = AnalyseFileDir + "/" + "e_on_1000060120_EMMEC_test_myBranch.root"; // Initial fix with 1M events (myBranch)
    ////</editor-fold>

    // AnalyseFilePath = "mnt/d/e4nu/hipo_data_files";
    // AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";

    // AnalyseFilePath = "home/alon/project";
    // AnalyseFileSample = "Downloads";
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
    // AnalyseFile = AnalyseFileDir + "e_on_1000060120_2222MeV_em.gst.root";

    // AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/" + "first_50_test" + "/";

    // AnalyseFilePath = "pnfs/genie/scratch/users/asportes/script_jobs/detector_sim_truth_level";
    // AnalyseFileSample = "detector_sim_truth_level/C12_G18_10a_02_11b_2GeV_QEL_test";
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
    ////AnalyseFile = AnalyseFileDir + "Ca48_G18_10a_02_11b_207052MeV.root";
    // AnalyseFile = AnalyseFileDir + "e_on_1000060120_2070MeV.root";

    // AnalyseFilePath = "home/alon/project";
    // AnalyseFileSample = "Downloads";
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
    ////AnalyseFile = AnalyseFileDir + "Ca48_G18_10a_02_11b_207052MeV.root";
    // AnalyseFile = AnalyseFileDir + "Ca48_G18_10a_02_11b_Q205_598636MeV.root";

    // AnalyseFilePath = "lustre19/expphy/volatile/clas12/asportes/truthLevelROOTFiles";
    // AnalyseFileSample = "C12_G18_10a_02_11b_207052MeV";
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
    // AnalyseFile = AnalyseFileDir + "C12_G18_10a_02_11b_207052MeV.root";

    // AnalyseFilePath = "mnt/d/e4nu/root_data_files/Detector_sim_truth_level_files";
    // AnalyseFileSample = "598636MeV_Q2_0_5";
    // AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
    //
    ////AnalyseFile = AnalyseFileDir + "e_on_1000060120_598636MeV_Q2_0_5.gst.root";
    ////AnalyseFile = "./nu_SuSAv2_fix_test_12C_2222GeV_CC_myBranch_10M.root";
    // AnalyseFile = "./nu_SuSAv2_fix_test_12C_2222GeV_CCMEC_myBranch_10M.root";
}

}  // namespace analysis_samples

#endif  // ANALYSIS_SAMPLES_H
