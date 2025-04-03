//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef UNIFORM_SAMPLES_H
#define UNIFORM_SAMPLES_H

#include <vector>

namespace uniform_samples {

#pragma region Uniform electron tester samples
/* Uniform electron tester samples */
void Config_Uniform_tester_e_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                     std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                     std::string &AnalyzeFile) {
    /* Uniform electron tester sample @ 2GeV */
    AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV/OutPut_Tester_e_Tester_e";
    // AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV/OutPut_Tester_e_1e";
    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);

    /* Uniform electron tester sample @ 4GeV */
    AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_Tester_e_Tester_e";
    // AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_Tester_e_1e";
    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);

    /* Uniform electron tester sample @ 6GeV */
    AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/5986MeV/OutPut_Tester_e_Tester_e";
    // AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/5986MeV/OutPut_Tester_e_1e";
    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}
#pragma endregion

#pragma region Uniform samples @ 2GeV
/* Uniform samples @ 2GeV */

/* Uniform 1e @ 2GeV */
void Config_Uniform_1e_2GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                    std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                    std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV/OutPut_1e";
    std::string AnalyzeFileSample0 = "reconhipo";
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
    // AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV/OutPut_1e";
    // AnalyzeFileSample = "reconhipo";
    // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    // AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all files
    // AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    // AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    // AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    // AnalyzeFile_v.push_back(AnalyzeFile);
}

/* Uniform en @ 2GeV */
void Config_Uniform_en_2GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                    std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                    std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV_ConstPn/OutPut_en";
    // std::string AnalyzeFilePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV/OutPut_en";
    std::string AnalyzeFileSample0 = "reconhipo";
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
    // AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV/OutPut_en";
    // AnalyzeFileSample = "reconhipo";
    // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    // AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all files
    // AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    // AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    // AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    // AnalyzeFile_v.push_back(AnalyzeFile);
}

/* Uniform ep @ 2GeV */
void Config_Uniform_ep_2GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                    std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                    std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV/OutPut_ep";
    std::string AnalyzeFileSample0 = "reconhipo";
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
    // AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/2070MeV/OutPut_ep";
    // AnalyzeFileSample = "reconhipo";
    // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    // AnalyzeFile = AnalyzeFileDir + "*.hipo"; // all files
    // AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    // AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    // AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    // AnalyzeFile_v.push_back(AnalyzeFile);
}
#pragma endregion

#pragma region Uniform samples @ 4GeV
/* Uniform samples @ 4GeV */

/* Uniform 1e @ 4GeV */
void Config_Uniform_1e_4GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                    std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                    std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_1e";
    std::string AnalyzeFileSample0 = "reconhipo";
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
    // AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_1e";
    // AnalyzeFileSample = "reconhipo";
    // AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    // AnalyzeFile = AnalyzeFileDir + "*.hipo";                                             // all files
    // AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    // AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    // AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    // AnalyzeFile_v.push_back(AnalyzeFile);
}

/* Uniform en @ 4GeV */
void Config_Uniform_en_4GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                    std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                    std::string &AnalyzeFile) {
    AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV_ConstPn/OutPut_en";
    // AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_en";
    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}

/* Uniform ep @ 4GeV */
void Config_Uniform_ep_4GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                    std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                    std::string &AnalyzeFile) {
    AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/4029MeV/OutPut_ep";
    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}
#pragma endregion

#pragma region Uniform samples @ 6GeV
/* Uniform samples @ 6GeV */

/* Uniform 1e @ 6GeV */
void Config_Uniform_1e_6GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                    std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                    std::string &AnalyzeFile) {
    AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/5986MeV/OutPut_1e";
    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}

/* Uniform en @ 6GeV */
void Config_Uniform_en_6GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                    std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                    std::string &AnalyzeFile) {
    AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/5986MeV_ConstPn/OutPut_en";
    // AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/5986MeV/OutPut_en";
    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}

/* Uniform ep @ 6GeV */
void Config_Uniform_ep_6GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                    std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                    std::string &AnalyzeFile) {
    AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco/Uniform_e-p-n_samples/5986MeV/OutPut_ep";
    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}

#pragma endregion

}  // namespace uniform_samples

#endif  // UNIFORM_SAMPLES_H
