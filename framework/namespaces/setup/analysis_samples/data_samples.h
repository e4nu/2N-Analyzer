//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef DATA_SAMPLES_H
#define DATA_SAMPLES_H

#include <vector>

namespace data_samples {

#pragma region /* H1 data */
/* H1 2.07052GeV - run 015634 (ifarm DATA files - H1 DATA files) */
void Config_H1_2GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                            std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/2gev/H/dst/recon";
    std::string AnalyzeFileSample0 = "015634";  // only run 015634
    // std::string AnalyzeFileSample0 = ""; // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}

/* H1 5.98636GeV - run 015028 (ifarm DATA files - H1 DATA files) */
void Config_H1_6GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                            std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/6gev/H/dst/recon";
    std::string AnalyzeFileSample0 = "015028";  // only run 015028
    // std::string AnalyzeFileSample0 = ""; // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}
#pragma endregion

#pragma region /* D2 data */
/* D2 2.07052GeV - all runs (ifarm DATA files - D2 DATA files) */
void Config_D2_2GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                            std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/2gev/D/dst/recon";
    std::string AnalyzeFileSample0 = "";  // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}

/* D2 5.98636GeV - run 015443 (ifarm DATA files - D2 DATA files) */
void Config_D2_6GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                            std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/6gev/D/dst/recon";
    std::string AnalyzeFileSample0 = "015443";  // only run 015443
    // std::string AnalyzeFileSample0 = ""; // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}
#pragma endregion

#pragma region /* C12 data */
/* C12 2.07052GeV - run 015664 (ifarm DATA files - C12 DATA files) */
void Config_C12_2GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                             std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/2gev/C/dst/recon";
    std::string AnalyzeFileSample0 = "015664";  // only run 015664
    // std::string AnalyzeFileSample0 = "";  // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}

/* C12 4.02962GeV - run 015778 (ifarm DATA files - C12 DATA files) */
void Config_C12_4GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                             std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/4gev/C/dst/recon";
    std::string AnalyzeFileSample0 = "015778";  // only run 015778
    // std::string AnalyzeFileSample0 = ""; // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}

/* C12x4 5.98636GeV - run 015188 (ifarm DATA files - C12x4 DATA files) */
void Config_C12_6GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                             std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon";
    // AnalyzeFile = AnalyzeFileDir + "rec_clas_015188.evio.01685-01689.hipo"; // 1 run file
    std::string AnalyzeFileSample0 = "015188";  // only run 015188
    // std::string AnalyzeFileSample0 = ""; // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}
#pragma endregion

#pragma region /* Ar40 data */
/* Ar40 2.07052GeV - run 015672 (ifarm DATA files - Ar40 DATA files) */
void Config_Ar40_2GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                              std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon";
    std::string AnalyzeFileSample0 = "015672";  // only run 015672
    // std::string AnalyzeFileSample0 = "";  // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}

/* Ar40 4.02962GeV - run 015743 (ifarm DATA files - Ar40 DATA files) */
void Config_Ar40_4GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                              std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon";
    std::string AnalyzeFileSample0 = "015743";  // only run 015743
    // std::string AnalyzeFileSample0 = ""; // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}

/* Ar40 5.98636GeV - run 015792 (ifarm DATA files - Ar40 DATA files) */
void Config_Ar40_6GeV_samples(std::vector<std::string> &AnalyzeFilePath_v, std::vector<std::string> &AnalyzeFileSample_v, std::vector<std::string> &AnalyzeFileDir_v,
                              std::vector<std::string> &AnalyzeFile_v, std::string &AnalyzeFilePath, std::string &AnalyzeFileSample, std::string &AnalyzeFileDir, std::string &AnalyzeFile) {
    std::string AnalyzeFilePath0 = "cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon";
    std::string AnalyzeFileSample0 = "015792";  // only run 015792
    // std::string AnalyzeFileSample0 = ""; // all runs
    std::string AnalyzeFileDir0 = "/" + AnalyzeFilePath0 + "/" + AnalyzeFileSample0 + "/";
    std::string AnalyzeFile0 = AnalyzeFileDir0 + "*.hipo";  // all files
    AnalyzeFilePath_v.push_back(AnalyzeFilePath0);
    AnalyzeFileSample_v.push_back(AnalyzeFileSample0);
    AnalyzeFileDir_v.push_back(AnalyzeFileDir0);
    AnalyzeFile_v.push_back(AnalyzeFile0);
}
#pragma endregion

}  // namespace data_samples

#endif  // DATA_SAMPLES_H
