#ifndef DATA_SAMPLES_H
#define DATA_SAMPLES_H

#include <vector>

namespace analysis_samples {

#pragma region /* H1 data */
/* H1 2.07052GeV - run 015634 (ifarm DATA files - H1 DATA files) */
void Config_H1_2GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                            std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/2gev/H/dst/recon";
    std::string AnalyseFileSample0 = "015634";  // only run 015634
    // std::string AnalyseFileSample0 = ""; // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}

/* H1 5.98636GeV - run 015028 (ifarm DATA files - H1 DATA files) */
void Config_H1_6GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                            std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/6gev/H/dst/recon";
    std::string AnalyseFileSample0 = "015028";  // only run 015028
    // std::string AnalyseFileSample0 = ""; // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}
#pragma endregion

#pragma region /* D2 data */
/* D2 2.07052GeV - all runs (ifarm DATA files - D2 DATA files) */
void Config_D2_2GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                            std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/2gev/D/dst/recon";
    std::string AnalyseFileSample0 = "";  // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}

/* D2 5.98636GeV - run 015443 (ifarm DATA files - D2 DATA files) */
void Config_D2_6GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                            std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/6gev/D/dst/recon";
    std::string AnalyseFileSample0 = "015443";  // only run 015443
    // std::string AnalyseFileSample0 = ""; // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}
#pragma endregion

#pragma region /* C12 data */
/* C12 2.07052GeV - run 015664 (ifarm DATA files - C12 DATA files) */
void Config_C12_2GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                             std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/2gev/C/dst/recon";
    std::string AnalyseFileSample0 = "015664";  // only run 015664
    // std::string AnalyseFileSample0 = "";  // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}

/* C12 4.02962GeV - run 015778 (ifarm DATA files - C12 DATA files) */
void Config_C12_4GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                             std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/4gev/C/dst/recon";
    std::string AnalyseFileSample0 = "015778";  // only run 015778
    // std::string AnalyseFileSample0 = ""; // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}

/* C12x4 5.98636GeV - run 015188 (ifarm DATA files - C12x4 DATA files) */
void Config_C12_6GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                             std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon";
    // AnalyseFile = AnalyseFileDir + "rec_clas_015188.evio.01685-01689.hipo"; // 1 run file
    std::string AnalyseFileSample0 = "015188";  // only run 015188
    // std::string AnalyseFileSample0 = ""; // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}
#pragma endregion

#pragma region /* Ar40 data */
/* Ar40 2.07052GeV - run 015672 (ifarm DATA files - Ar40 DATA files) */
void Config_Ar40_2GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                              std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon";
    std::string AnalyseFileSample0 = "015672";  // only run 015672
    // std::string AnalyseFileSample0 = "";  // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}

/* Ar40 4.02962GeV - run 015743 (ifarm DATA files - Ar40 DATA files) */
void Config_Ar40_4GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                              std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon";
    std::string AnalyseFileSample0 = "015743";  // only run 015743
    // std::string AnalyseFileSample0 = ""; // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}

/* Ar40 5.98636GeV - run 015792 (ifarm DATA files - Ar40 DATA files) */
void Config_Ar40_6GeV_samples(std::vector<std::string> &AnalyseFilePath_v, std::vector<std::string> &AnalyseFileSample_v, std::vector<std::string> &AnalyseFileDir_v,
                              std::vector<std::string> &AnalyseFile_v, std::string &AnalyseFilePath, std::string &AnalyseFileSample, std::string &AnalyseFileDir, std::string &AnalyseFile) {
    std::string AnalyseFilePath0 = "cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon";
    std::string AnalyseFileSample0 = "015792";  // only run 015792
    // std::string AnalyseFileSample0 = ""; // all runs
    std::string AnalyseFileDir0 = "/" + AnalyseFilePath0 + "/" + AnalyseFileSample0 + "/";
    std::string AnalyseFile0 = AnalyseFileDir0 + "*.hipo";  // all files
    AnalyseFilePath_v.push_back(AnalyseFilePath0);
    AnalyseFileSample_v.push_back(AnalyseFileSample0);
    AnalyseFileDir_v.push_back(AnalyseFileDir0);
    AnalyseFile_v.push_back(AnalyseFile0);
}
#pragma endregion

}  // namespace analysis_samples

#endif  // DATA_SAMPLES_H
