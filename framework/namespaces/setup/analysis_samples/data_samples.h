//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef DATA_SAMPLES_H
#define DATA_SAMPLES_H

#include <iostream>
#include <string>
#include <vector>

namespace data_samples {

#pragma region /* H1 data */
/* H1 2.07052GeV - run 015634 (ifarm DATA files - H1 DATA files) */
void Config_H1_2GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                            std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/2gev/H/dst/recon";
    std::string ReconHipoDir0 = "015634";  // only run 015634
    // std::string ReconHipoDir0 = ""; // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}

/* H1 5.98636GeV - run 015028 (ifarm DATA files - H1 DATA files) */
void Config_H1_6GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                            std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/6gev/H/dst/recon";
    std::string ReconHipoDir0 = "015028";  // only run 015028
    // std::string ReconHipoDir0 = ""; // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}
#pragma endregion

#pragma region /* D2 data */
/* D2 2.07052GeV - all runs (ifarm DATA files - D2 DATA files) */
void Config_D2_2GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                            std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/2gev/D/dst/recon";
    std::string ReconHipoDir0 = "";  // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}

/* D2 5.98636GeV - run 015443 (ifarm DATA files - D2 DATA files) */
void Config_D2_6GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                            std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/6gev/D/dst/recon";
    std::string ReconHipoDir0 = "015443";  // only run 015443
    // std::string ReconHipoDir0 = ""; // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}
#pragma endregion

#pragma region /* C12 data */
/* C12 2.07052GeV - run 015664 (ifarm DATA files - C12 DATA files) */
void Config_C12_2GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                             std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/2gev/C/dst/recon";
    std::string ReconHipoDir0 = "015664";  // only run 015664
    // std::string ReconHipoDir0 = "";  // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}

/* C12 4.02962GeV - run 015778 (ifarm DATA files - C12 DATA files) */
void Config_C12_4GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                             std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/4gev/C/dst/recon";
    std::string ReconHipoDir0 = "015778";  // only run 015778
    // std::string ReconHipoDir0 = ""; // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}

/* C12x4 5.98636GeV - run 015188 (ifarm DATA files - C12x4 DATA files) */
void Config_C12_6GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                             std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon";
    // InputHipoFiles = PathToHipoFilesDir + "rec_clas_015188.evio.01685-01689.hipo"; // 1 run file
    std::string ReconHipoDir0 = "015188";  // only run 015188
    // std::string ReconHipoDir0 = ""; // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}
#pragma endregion

#pragma region /* Ar40 data */
/* Ar40 2.07052GeV - run 015672 (ifarm DATA files - Ar40 DATA files) */
void Config_Ar40_2GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                              std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon";
    std::string ReconHipoDir0 = "015672";  // only run 015672
    // std::string ReconHipoDir0 = "";  // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}

/* Ar40 4.02962GeV - run 015743 (ifarm DATA files - Ar40 DATA files) */
void Config_Ar40_4GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                              std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon";
    std::string ReconHipoDir0 = "015743";  // only run 015743
    // std::string ReconHipoDir0 = ""; // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}

/* Ar40 5.98636GeV - run 015792 (ifarm DATA files - Ar40 DATA files) */
void Config_Ar40_6GeV_samples(std::vector<std::string> &RecoSamplePath_v, std::vector<std::string> &ReconHipoDir_v, std::vector<std::string> &PathToHipoFilesDir_v,
                              std::vector<std::string> &InputHipoFiles_v, std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles) {
    std::string RecoSamplePath0 = "cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon";
    std::string ReconHipoDir0 = "015792";  // only run 015792
    // std::string ReconHipoDir0 = ""; // all runs
    std::string PathToHipoFilesDir0 = "/" + RecoSamplePath0 + "/" + ReconHipoDir0 + "/";
    std::string InputHipoFiles0 = PathToHipoFilesDir0 + "*.hipo";  // all files
    RecoSamplePath_v.push_back(RecoSamplePath0);
    ReconHipoDir_v.push_back(ReconHipoDir0);
    PathToHipoFilesDir_v.push_back(PathToHipoFilesDir0);
    InputHipoFiles_v.push_back(InputHipoFiles0);
}
#pragma endregion

}  // namespace data_samples

#endif  // DATA_SAMPLES_H
