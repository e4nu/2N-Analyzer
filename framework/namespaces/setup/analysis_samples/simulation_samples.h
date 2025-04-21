//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef SIMULATION_SAMPLES_H
#define SIMULATION_SAMPLES_H

#include <iostream>
#include <string>
#include <vector>

namespace simulation_samples {

#pragma region /* C12 */

#pragma region /* C12 G18 */

/* C12 G18 2GeV */
void Config_C12_G18_2GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                 std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile,
                                 const bool &wFC = false) {
    if (wFC) {
        AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/C12/G18_10a_00_000/2070MeV_Q2_0_02_wFC";
    } else {
        AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/C12/G18_10a_00_000/2070MeV_Q2_0_02";
    }

    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}

/* C12 G18 4GeV */
void Config_C12_G18_4GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                 std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile,
                                 const bool &wFC = false) {
    if (wFC) {
        AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/C12/G18_10a_00_000/4029MeV_Q2_0_25_wFC";
    } else {
        AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/C12/G18_10a_00_000/4029MeV_Q2_0_25";
    }

    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}

#pragma endregion

#pragma region /* C12 SuSa */

/* C12 SuSa 2GeV */
void Config_C12_SuSa_2GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                  std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                  std::string &AnalyzeFile, const bool &wFC = false) {
    if (wFC) {
        AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/C12/GEM21_11a_00_000/2070MeV_Q2_0_02_wFC";
    } else {
        AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/C12/GEM21_11a_00_000/2070MeV_Q2_0_02";
    }

    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}

/* C12 SuSa 4GeV */
void Config_C12_SuSa_4GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                                  std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir,
                                  std::string &AnalyzeFile, const bool &wFC = false) {
    if (wFC) {
        AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/C12/GEM21_11a_00_000/4029MeV_Q2_0_25_wFC";
    } else {
        AnalyzeFilePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/C12/GEM21_11a_00_000/4029MeV_Q2_0_25";
    }

    AnalyzeFileSample = "reconhipo";
    AnalyzeFileDir = "/" + AnalyzeFilePath + "/" + AnalyzeFileSample + "/";
    AnalyzeFile = AnalyzeFileDir + "*.hipo";
    AnalyzeFilePath_v.push_back(AnalyzeFilePath);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir);
    AnalyzeFile_v.push_back(AnalyzeFile);
}

#pragma endregion

#pragma endregion

}  // namespace simulation_samples

#endif  // SIMULATION_SAMPLES_H
