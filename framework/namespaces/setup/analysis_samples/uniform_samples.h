//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef UNIFORM_SAMPLES_H
#define UNIFORM_SAMPLES_H

#include <iostream>
#include <string>
#include <vector>

namespace uniform_samples {

#pragma region Uniform electron tester samples
/* Uniform electron tester samples */
void Config_Uniform_tester_e_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                     std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                     std::string &InputHipoFiles) {
    /* Uniform electron tester sample @ 2GeV */
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_Tester_e_Tester_e";
    // RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_Tester_e_1e";
    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);

    /* Uniform electron tester sample @ 4GeV */
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_Tester_e_Tester_e";
    // RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_Tester_e_1e";
    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);

    /* Uniform electron tester sample @ 6GeV */
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_Tester_e_Tester_e";
    // RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_Tester_e_1e";
    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}
#pragma endregion

#pragma region Uniform samples @ 2GeV
/* Uniform samples @ 2GeV */

/* Uniform 1e @ 2GeV */
void Config_Uniform_1e_2GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                    std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                    std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_1e";
    std::string ReconHipoDir0 = "reconhipo";
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
    // RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_1e";
    // ReconHipoDir = "reconhipo";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    // InputHipoFiles = PathToHipoFilesDir + "*.hipo"; // all files
    // RecoSamplePath_v.push_back(RecoSamplePath);
    // ReconHipoDir_v.push_back(ReconHipoDir);
    // PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    // InputHipoFiles_v.push_back(InputHipoFiles);
}

/* Uniform en @ 2GeV */
void Config_Uniform_en_2GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                    std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                    std::string &InputHipoFiles) {
    // std::string RecoSamplePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV_ConstPn/OutPut_en";
    std::string RecoSamplePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_en";
    std::string ReconHipoDir0 = "reconhipo";
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
    // RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_en";
    // ReconHipoDir = "reconhipo";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    // InputHipoFiles = PathToHipoFilesDir + "*.hipo"; // all files
    // RecoSamplePath_v.push_back(RecoSamplePath);
    // ReconHipoDir_v.push_back(ReconHipoDir);
    // PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    // InputHipoFiles_v.push_back(InputHipoFiles);
}

/* Uniform ep @ 2GeV */
void Config_Uniform_ep_2GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                    std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                    std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_ep";
    std::string ReconHipoDir0 = "reconhipo";
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
    // RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/2070MeV/OutPut_ep";
    // ReconHipoDir = "reconhipo";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    // InputHipoFiles = PathToHipoFilesDir + "*.hipo"; // all files
    // RecoSamplePath_v.push_back(RecoSamplePath);
    // ReconHipoDir_v.push_back(ReconHipoDir);
    // PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    // InputHipoFiles_v.push_back(InputHipoFiles);
}
#pragma endregion

#pragma region Uniform samples @ 4GeV
/* Uniform samples @ 4GeV */

/* Uniform 1e @ 4GeV */
void Config_Uniform_1e_4GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                    std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                    std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV_2/OutPut_1e";
    // std::string RecoSamplePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_1e";
    std::string ReconHipoDir0 = "reconhipo";
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
    // RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_1e";
    // ReconHipoDir = "reconhipo";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    // InputHipoFiles = PathToHipoFilesDir + "*.hipo";                                             // all files
    // RecoSamplePath_v.push_back(RecoSamplePath);
    // ReconHipoDir_v.push_back(ReconHipoDir);
    // PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    // InputHipoFiles_v.push_back(InputHipoFiles);
}

/* Uniform en @ 4GeV */
void Config_Uniform_en_4GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                    std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                    std::string &InputHipoFiles) {
    // RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV_ConstPn/OutPut_en";
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_en";
    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

/* Uniform ep @ 4GeV */
void Config_Uniform_ep_4GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                    std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                    std::string &InputHipoFiles) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/4029MeV/OutPut_ep";
    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}
#pragma endregion

#pragma region Uniform samples @ 6GeV
/* Uniform samples @ 6GeV */

/* Uniform 1e @ 6GeV */
void Config_Uniform_1e_6GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                    std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                    std::string &InputHipoFiles) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_1e";
    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

/* Uniform en @ 6GeV */
void Config_Uniform_en_6GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                    std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                    std::string &InputHipoFiles) {
    // RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV_ConstPn/OutPut_en";
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_en";
    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

/* Uniform ep @ 6GeV */
void Config_Uniform_ep_6GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                                    std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir,
                                    std::string &InputHipoFiles) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/Uniform_e-p-n_samples/5986MeV/OutPut_ep";
    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

#pragma endregion

}  // namespace uniform_samples

#endif  // UNIFORM_SAMPLES_H
