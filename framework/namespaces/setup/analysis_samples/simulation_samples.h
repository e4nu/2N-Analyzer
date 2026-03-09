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

#pragma region /* C12 G18 - corrected geometry */

/* C12 G18 2GeV - corrected geometry */
void Config_C12_G18_2GeV_Corr1Foil_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                                std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                                std::string& InputHipoFiles, const bool& wFC = false) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/G18_10a_00_000/2070MeV_Q2_0_02_devGEMC_rgm_fall2021_C_S";

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

/* C12 G18 4GeV - corrected geometry */
void Config_C12_G18_4GeV_Corr1Foil_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                                std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                                std::string& InputHipoFiles, const bool& wFC = false) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/G18_10a_00_000/4029MeV_Q2_0_25_devGEMC_rgm_fall2021_C_L";

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

#pragma endregion

#pragma region /* C12 SuSa - corrected geometry */

/* C12 SuSa 2GeV - corrected geometry */
void Config_C12_SuSa_2GeV_Corr1Foil_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                                 std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                                 std::string& InputHipoFiles, const bool& wFC = false) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/2070MeV_Q2_0_02_devGEMC_rgm_fall2021_C_S";

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

/* C12 SuSa 4GeV - corrected geometry */
void Config_C12_SuSa_4GeV_Corr1Foil_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                                 std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                                 std::string& InputHipoFiles, const bool& wFC = false) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/4029MeV_Q2_0_25_devGEMC_rgm_fall2021_C_L";

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

#pragma endregion

#pragma region /* C12 G18 */

/* C12 G18 4GeV */
void Config_C12_G18_4GeV_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                 std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                 std::string& InputHipoFiles, const bool& wFC = false) {
    if (wFC) {
        RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/G18_10a_00_000/4029MeV_Q2_0_25_wFC";
    } else {
        RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/G18_10a_00_000/4029MeV_Q2_0_25";
    }

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

#pragma endregion

#pragma region /* C12 SuSa */

/* C12 SuSa 2GeV */
void Config_C12_SuSa_2GeV_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                  std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                  std::string& InputHipoFiles, const bool& wFC = false) {
    if (wFC) {
        RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/2070MeV_Q2_0_02_wFC";
    } else {
        RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/2070MeV_Q2_0_02";
    }

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

/* C12 SuSa 4GeV */
void Config_C12_SuSa_4GeV_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                  std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                  std::string& InputHipoFiles, const bool& wFC = false) {
    if (wFC) {
        RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/4029MeV_Q2_0_25_wFC";
    } else {
        RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/C12/GEM21_11a_00_000/4029MeV_Q2_0_25";
    }

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

#pragma endregion

#pragma endregion

#pragma region /* Ar40 */

#pragma region /* Ar40 G18 - corrected geometry */

/* Ar40 G18 2GeV - corrected geometry */
void Config_Ar40_G18_2GeV_Corr1Foil_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                                 std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                                 std::string& InputHipoFiles, const bool& wFC = false) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/Ar40/G18_10a_00_000/2070MeV_Q2_0_02_devGEMC_rgm_fall2021_Ar";

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

/* Ar40 G18 4GeV - corrected geometry */
void Config_Ar40_G18_4GeV_Corr1Foil_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                                 std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                                 std::string& InputHipoFiles, const bool& wFC = false) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/Ar40/G18_10a_00_000/4029MeV_Q2_0_25_devGEMC_rgm_fall2021_Ar";

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

#pragma endregion

#pragma region /* Ar40 SuSa - corrected geometry */

/* Ar40 SuSa 2GeV - corrected geometry */
void Config_Ar40_SuSa_2GeV_Corr1Foil_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                                  std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                                  std::string& InputHipoFiles, const bool& wFC = false) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/Ar40/GEM21_11a_00_000/2070MeV_Q2_0_02_devGEMC_rgm_fall2021_Ar";

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

/* Ar40 SuSa 4GeV - corrected geometry */
void Config_Ar40_SuSa_4GeV_Corr1Foil_samples(std::vector<std::string>& RecoSamplePath_v, std::vector<std::string>& ReconHipoDir_v, std::vector<std::string>& PathToHipoFilesDir_v,
                                                  std::vector<std::string>& InputHipoFiles_v, std::string& RecoSamplePath, std::string& ReconHipoDir, std::string& PathToHipoFilesDir,
                                                  std::string& InputHipoFiles, const bool& wFC = false) {
    RecoSamplePath = "lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples/Ar40/GEM21_11a_00_000/4029MeV_Q2_0_25_devGEMC_rgm_fall2021_Ar";

    ReconHipoDir = "reconhipo";
    PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    InputHipoFiles = PathToHipoFilesDir + "*.hipo";
    RecoSamplePath_v.push_back(RecoSamplePath);
    ReconHipoDir_v.push_back(ReconHipoDir);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
    InputHipoFiles_v.push_back(InputHipoFiles);
}

#pragma endregion

#pragma endregion

}  // namespace simulation_samples

#endif  // SIMULATION_SAMPLES_H
