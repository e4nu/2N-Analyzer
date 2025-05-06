//
// Created by Alon Sportes on 30/04/2025.
//

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2.h>
#include <TH2D.h>
#include <THStack.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TObject.h>
#include <TPad.h>
#include <TPaletteAxis.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

// Include libraries:
#include "../framework/namespaces/general_utilities/histogram_functions.h"

// To run: root -q -b /Users/alon/Projects/2N-Analyser_e4nu/macros/CompareHistogramsTester.cpp

using namespace std;

// CompareHistogramsTester ------------------------------------------------------------------

void CompareHistogramsTester() {
    std::vector<bool> MissWnucL_status = {false, true};  // Set to true if you want to use the miss_with_leading_nuc sample

    for (auto i = 0; i < MissWnucL_status.size(); ++i) {
        bool Use_miss_with_leading_nuc = MissWnucL_status[i];

        std::string MissWnucL = Use_miss_with_leading_nuc ? "_miss_with_leading_nuc" : "";
        std::vector<std::string> SamplePaths = {
            "/Users/alon/Downloads/v23/C12_simulation_G18_Q2_0_02_2070MeV_S03ACNC_V23" + MissWnucL + "/C12_simulation_G18_Q2_0_02_2070MeV_S03ACNC_V23" + MissWnucL + "_plots.root",
            "/Users/alon/Downloads/v23/C12_simulation_G18_Q2_0_02_2070MeV_wFC_S03ACNC_V23" + MissWnucL + "/C12_simulation_G18_Q2_0_02_2070MeV_wFC_S03ACNC_V23" + MissWnucL + "_plots.root",
            "/Users/alon/Downloads/v23/C12_simulation_SuSa_Q2_0_02_2070MeV_S03ACNC_V23" + MissWnucL + "/C12_simulation_SuSa_Q2_0_02_2070MeV_S03ACNC_V23" + MissWnucL + "_plots.root",
            "/Users/alon/Downloads/v23/C12_simulation_SuSa_Q2_0_02_2070MeV_wFC_S03ACNC_V23" + MissWnucL + "/C12_simulation_SuSa_Q2_0_02_2070MeV_wFC_S03ACNC_V23" + MissWnucL + "_plots.root",
            "/Users/alon/Downloads/v23/C12_data_2GeV_run_015664_S03ACNC_V23" + MissWnucL + "/C12_data_2GeV_run_015664_S03ACNC_V23" + MissWnucL + "_plots.root",
            "/Users/alon/Downloads/v23/C12_simulation_G18_Q2_0_25_4029MeV_S03ACNC_V23" + MissWnucL + "/C12_simulation_G18_Q2_0_25_4029MeV_S03ACNC_V23" + MissWnucL + "_plots.root",
            "/Users/alon/Downloads/v23/C12_simulation_G18_Q2_0_25_4029MeV_wFC_S03ACNC_V23" + MissWnucL + "/C12_simulation_G18_Q2_0_25_4029MeV_wFC_S03ACNC_V23" + MissWnucL + "_plots.root",
            "/Users/alon/Downloads/v23/C12_simulation_SuSa_Q2_0_25_4029MeV_S03ACNC_V23" + MissWnucL + "/C12_simulation_SuSa_Q2_0_25_4029MeV_S03ACNC_V23" + MissWnucL + "_plots.root",
            "/Users/alon/Downloads/v23/C12_simulation_SuSa_Q2_0_25_4029MeV_wFC_S03ACNC_V23" + MissWnucL + "/C12_simulation_SuSa_Q2_0_25_4029MeV_wFC_S03ACNC_V23" + MissWnucL + "_plots.root",
            "/Users/alon/Downloads/v23/C12_data_4GeV_run_015778_S03ACNC_V23" + MissWnucL + "/C12_data_4GeV_run_015778_S03ACNC_V23" + MissWnucL + "_plots.root"};

        // std::string SaveDirFolder = "'/Users/alon/University/Ph.D. (TAU)/e4nu (PhD)/Presentations (PhD)/17 IP17 - E4nu meeting 05-05-25/Figures'";
        std::string SaveDirFolder = "/Users/alon/Downloads";

        for (auto i = 0; i < SamplePaths.size(); ++i) {
            std::string SampleType = (basic_tools::FindSubstring(SamplePaths[i], "data")) ? "Data" : (basic_tools::FindSubstring(SamplePaths[i], "SuSa")) ? "SuSa" : "G18";
            std::string SampleBeamE = (basic_tools::FindSubstring(SamplePaths[i], "2070MeV") || basic_tools::FindSubstring(SamplePaths[i], "2GeV"))   ? "_2GeV"
                                      : (basic_tools::FindSubstring(SamplePaths[i], "4029MeV") || basic_tools::FindSubstring(SamplePaths[i], "4GeV")) ? "_4GeV"
                                                                                                                                                      : "_6GeV";
            std::string FCStatus = (basic_tools::FindSubstring(SamplePaths[i], "data")) ? "" : (basic_tools::FindSubstring(SamplePaths[i], "_wFC")) ? "_wFC" : "";

            std::string sample = SampleType + SampleBeamE + FCStatus + MissWnucL;
            // std::string sample = Samples[i];
            std::string SaveDirFoldersName = "Histogram_Comparisons_" + sample;
            const char *filename = SamplePaths[i].c_str();

            system(("rm -rf " + SaveDirFolder + "/" + SaveDirFoldersName).c_str());
            system(("mkdir -p " + SaveDirFolder + "/" + SaveDirFoldersName).c_str());

            std::cout << "\033[33m\n\n===========================================================================================\n\n";
            std::cout << "\033[33msample:            \t\033[0m" << sample << "\n";
            std::cout << "\033[33mSaveDirFolder:     \t\033[0m" << SaveDirFolder << "\n";
            std::cout << "\033[33mSaveDirFoldersName:\t\033[0m" << SaveDirFoldersName << "\n";
            std::cout << "\033[33mfilename:          \t\033[0m" << filename << "\n";
            std::cout << "\033[33mUse_miss_with_leading_nuc: \033[0m" << basic_tools::BoolToString(Use_miss_with_leading_nuc) << "\n\n";

            TFile *file = new TFile(filename);
            std::vector<TObject *> histograms_to_delete;

            // Load histograms and register them for deletion
            auto load = [&](const char *name, const std::string &cls) -> TObject * {
                TObject *obj = histogram_functions::FindHistogram(file, name, cls);
                histograms_to_delete.push_back(obj);
                return obj;
            };

            auto hP_e_vs_Theta_e_reco_All_int_pFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_All_int_pFDpCD", "TH2D");
            auto hP_e_vs_Theta_e_reco_QEL_pFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_QEL_pFDpCD", "TH2D");
            auto hP_e_vs_Theta_e_reco_MEC_pFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_MEC_pFDpCD", "TH2D");
            auto hP_e_vs_Theta_e_reco_RES_pFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_RES_pFDpCD", "TH2D");
            auto hP_e_vs_Theta_e_reco_DIS_pFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_DIS_pFDpCD", "TH2D");
            auto hP_e_vs_Theta_e_reco_All_int_nFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_All_int_nFDpCD", "TH2D");
            auto hP_e_vs_Theta_e_reco_QEL_nFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_QEL_nFDpCD", "TH2D");
            auto hP_e_vs_Theta_e_reco_MEC_nFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_MEC_nFDpCD", "TH2D");
            auto hP_e_vs_Theta_e_reco_RES_nFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_RES_nFDpCD", "TH2D");
            auto hP_e_vs_Theta_e_reco_DIS_nFDpCD = (TH2D *)load("P_e_vs_Theta_e_reco_DIS_nFDpCD", "TH2D");
            histogram_functions::CompareHistograms({hP_e_vs_Theta_e_reco_All_int_pFDpCD, hP_e_vs_Theta_e_reco_All_int_nFDpCD}, SaveDirFolder, SaveDirFoldersName,
                                                   "P_e_vs_Theta_e_ReacCompo_All_int");
            histogram_functions::CompareHistograms(
                {hP_e_vs_Theta_e_reco_All_int_pFDpCD, hP_e_vs_Theta_e_reco_QEL_pFDpCD, hP_e_vs_Theta_e_reco_MEC_pFDpCD, hP_e_vs_Theta_e_reco_RES_pFDpCD, hP_e_vs_Theta_e_reco_DIS_pFDpCD},
                SaveDirFolder, SaveDirFoldersName, "P_e_vs_Theta_e_ReacCompo_pFDpCD");
            histogram_functions::CompareHistograms(
                {hP_e_vs_Theta_e_reco_All_int_nFDpCD, hP_e_vs_Theta_e_reco_QEL_nFDpCD, hP_e_vs_Theta_e_reco_MEC_nFDpCD, hP_e_vs_Theta_e_reco_RES_nFDpCD, hP_e_vs_Theta_e_reco_DIS_nFDpCD},
                SaveDirFolder, SaveDirFoldersName, "P_e_vs_Theta_e_ReacCompo_nFDpCD");

            auto sW_1e_cut = (THStack *)load("W distribution (All Int., 1e cut)", "THStack");
            auto sW_1p = (THStack *)load("W distribution (All Int., 1p)", "THStack");
            auto sW_1n = (THStack *)load("W distribution (All Int., 1n)", "THStack");
            auto sW_pFDpCD = (THStack *)load("W distribution (All Int., pFDpCD)", "THStack");
            auto sW_nFDpCD = (THStack *)load("W distribution (All Int., nFDpCD)", "THStack");
            histogram_functions::CompareHistograms({sW_1e_cut, sW_1p, sW_1n, sW_pFDpCD, sW_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "W_stack_ReacComp");

            auto hW_1e_cut = (TH1D *)load("W distribution (All Int., 1e cut)", "TH1D");
            auto hW_1p = (TH1D *)load("W distribution (All Int., 1p)", "TH1D");
            auto hW_1n = (TH1D *)load("W distribution (All Int., 1n)", "TH1D");
            auto hW_pFDpCD = (TH1D *)load("W distribution (All Int., pFDpCD)", "TH1D");
            auto hW_nFDpCD = (TH1D *)load("W distribution (All Int., nFDpCD)", "TH1D");
            histogram_functions::CompareHistograms({hW_1e_cut, hW_1p, hW_1n, hW_pFDpCD, hW_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "W_All_Int_ReacComp");

            auto hP_miss_1N_All_Int_pFDpCD = (TH1D *)load("P^{1N}_{miss} distribution (All Int., pFDpCD)", "TH1D");
            auto sP_miss_1N_pFDpCD = (THStack *)load("P^{1N}_{miss} distribution (All Int., pFDpCD)", "THStack");
            auto hP_miss_1N_All_Int_nFDpCD = (TH1D *)load("P^{1N}_{miss} distribution (All Int., nFDpCD)", "TH1D");
            auto sP_miss_1N_nFDpCD = (THStack *)load("P^{1N}_{miss} distribution (All Int., nFDpCD)", "THStack");
            if (Use_miss_with_leading_nuc) {
                histogram_functions::TitleAligner(hP_miss_1N_All_Int_pFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}|", "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(sP_miss_1N_pFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}|", "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_All_Int_nFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}|", "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(sP_miss_1N_nFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}|", "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
            }
            histogram_functions::CompareHistograms({hP_miss_1N_All_Int_pFDpCD, hP_miss_1N_All_Int_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "P_miss_1N_All_Int_ReacComp");
            histogram_functions::CompareHistograms({sP_miss_1N_pFDpCD, sP_miss_1N_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "P_miss_1N_stack_ReacComp");

            auto hE_miss_1N_All_Int_pFDpCD = (TH1D *)load("E^{1N}_{miss} distribution (All Int., pFDpCD)", "TH1D");
            auto sE_miss_1N_pFDpCD = (THStack *)load("E^{1N}_{miss} distribution (All Int., pFDpCD)", "THStack");
            auto hE_miss_1N_All_Int_nFDpCD = (TH1D *)load("E^{1N}_{miss} distribution (All Int., nFDpCD)", "TH1D");
            auto sE_miss_1N_nFDpCD = (THStack *)load("E^{1N}_{miss} distribution (All Int., nFDpCD)", "THStack");
            if (Use_miss_with_leading_nuc) {
                histogram_functions::TitleAligner(hE_miss_1N_All_Int_pFDpCD, "E^{1N}_{miss} = #omega - T_{pFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(sE_miss_1N_pFDpCD, "E^{1N}_{miss} = #omega - T_{pFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hE_miss_1N_All_Int_nFDpCD, "E^{1N}_{miss} = #omega - T_{nFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(sE_miss_1N_nFDpCD, "E^{1N}_{miss} = #omega - T_{nFD}", "E^{1N}_{miss} = #omega - T_{L}");
            }
            histogram_functions::CompareHistograms({hE_miss_1N_All_Int_pFDpCD, hE_miss_1N_All_Int_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "E_miss_1N_All_Int_ReacComp");
            histogram_functions::CompareHistograms({sE_miss_1N_pFDpCD, sE_miss_1N_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "E_miss_1N_stack_ReacComp");

            auto hP_miss_1N_VS_E_miss_1N_All_Int_pFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (All Int., pFDpCD)", "TH2D");
            auto hP_miss_1N_VS_E_miss_1N_QEL_pFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (QEL only, pFDpCD)", "TH2D");
            auto hP_miss_1N_VS_E_miss_1N_MEC_pFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (MEC only, pFDpCD)", "TH2D");
            auto hP_miss_1N_VS_E_miss_1N_RES_pFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (RES only, pFDpCD)", "TH2D");
            auto hP_miss_1N_VS_E_miss_1N_DIS_pFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (DIS only, pFDpCD)", "TH2D");
            auto hP_miss_1N_VS_E_miss_1N_All_Int_nFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (All Int., nFDpCD)", "TH2D");
            auto hP_miss_1N_VS_E_miss_1N_QEL_nFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (QEL only, nFDpCD)", "TH2D");
            auto hP_miss_1N_VS_E_miss_1N_MEC_nFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (MEC only, nFDpCD)", "TH2D");
            auto hP_miss_1N_VS_E_miss_1N_RES_nFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (RES only, nFDpCD)", "TH2D");
            auto hP_miss_1N_VS_E_miss_1N_DIS_nFDpCD = (TH2D *)load("P^{1N}_{miss} vs. E^{1N}_{miss} (DIS only, nFDpCD)", "TH2D");
            if (Use_miss_with_leading_nuc) {
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_All_Int_pFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_QEL_pFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_MEC_pFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_RES_pFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_DIS_pFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{pFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_All_Int_nFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_QEL_nFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_MEC_nFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_RES_nFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_DIS_nFDpCD, "P^{1N}_{miss} = |#font[62]{P}_{nFD} - #font[62]{q}|",
                                                  "P^{1N}_{miss} = |#font[62]{P}_{L} - #font[62]{q}|");

                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_All_Int_pFDpCD, "E^{1N}_{miss} = #omega - T_{pFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_QEL_pFDpCD, "E^{1N}_{miss} = #omega - T_{pFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_MEC_pFDpCD, "E^{1N}_{miss} = #omega - T_{pFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_RES_pFDpCD, "E^{1N}_{miss} = #omega - T_{pFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_DIS_pFDpCD, "E^{1N}_{miss} = #omega - T_{pFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_All_Int_nFDpCD, "E^{1N}_{miss} = #omega - T_{nFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_QEL_nFDpCD, "E^{1N}_{miss} = #omega - T_{nFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_MEC_nFDpCD, "E^{1N}_{miss} = #omega - T_{nFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_RES_nFDpCD, "E^{1N}_{miss} = #omega - T_{nFD}", "E^{1N}_{miss} = #omega - T_{L}");
                histogram_functions::TitleAligner(hP_miss_1N_VS_E_miss_1N_DIS_nFDpCD, "E^{1N}_{miss} = #omega - T_{nFD}", "E^{1N}_{miss} = #omega - T_{L}");
            }
            histogram_functions::CompareHistograms({hP_miss_1N_VS_E_miss_1N_All_Int_pFDpCD, hP_miss_1N_VS_E_miss_1N_All_Int_nFDpCD}, SaveDirFolder, SaveDirFoldersName,
                                                   "P_miss_1N_VS_E_miss_1N_All_Int_ReacComp");
            histogram_functions::CompareHistograms({hP_miss_1N_VS_E_miss_1N_All_Int_pFDpCD, hP_miss_1N_VS_E_miss_1N_QEL_pFDpCD, hP_miss_1N_VS_E_miss_1N_MEC_pFDpCD,
                                                    hP_miss_1N_VS_E_miss_1N_RES_pFDpCD, hP_miss_1N_VS_E_miss_1N_DIS_pFDpCD},
                                                   SaveDirFolder, SaveDirFoldersName, "P_miss_1N_VS_E_miss_1N_ReacComp_pFDpCD");
            histogram_functions::CompareHistograms({hP_miss_1N_VS_E_miss_1N_All_Int_nFDpCD, hP_miss_1N_VS_E_miss_1N_QEL_nFDpCD, hP_miss_1N_VS_E_miss_1N_MEC_nFDpCD,
                                                    hP_miss_1N_VS_E_miss_1N_RES_nFDpCD, hP_miss_1N_VS_E_miss_1N_DIS_nFDpCD},
                                                   SaveDirFolder, SaveDirFoldersName, "P_miss_1N_VS_E_miss_1N_ReacComp_nFDpCD");

            auto hP_miss_2N_All_Int_pFDpCD = (TH1D *)load("P^{2N}_{miss} distribution (All Int., pFDpCD)", "TH1D");
            auto sP_miss_2N_pFDpCD = (THStack *)load("P^{2N}_{miss} distribution (All Int., pFDpCD)", "THStack");
            auto hP_miss_2N_All_Int_nFDpCD = (TH1D *)load("P^{2N}_{miss} distribution (All Int., nFDpCD)", "TH1D");
            auto sP_miss_2N_nFDpCD = (THStack *)load("P^{2N}_{miss} distribution (All Int., nFDpCD)", "THStack");
            histogram_functions::CompareHistograms({hP_miss_2N_All_Int_pFDpCD, hP_miss_2N_All_Int_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "P_miss_2N_All_Int_ReacComp");
            histogram_functions::CompareHistograms({sP_miss_2N_pFDpCD, sP_miss_2N_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "P_miss_2N_stack_ReacComp");

            auto hE_miss_2N_All_Int_pFDpCD = (TH1D *)load("E^{2N}_{miss} distribution (All Int., pFDpCD)", "TH1D");
            auto sE_miss_2N_pFDpCD = (THStack *)load("E^{2N}_{miss} distribution (All Int., pFDpCD)", "THStack");
            auto hE_miss_2N_All_Int_nFDpCD = (TH1D *)load("E^{2N}_{miss} distribution (All Int., nFDpCD)", "TH1D");
            auto sE_miss_2N_nFDpCD = (THStack *)load("E^{2N}_{miss} distribution (All Int., nFDpCD)", "THStack");
            histogram_functions::CompareHistograms({hE_miss_2N_All_Int_pFDpCD, hE_miss_2N_All_Int_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "E_miss_2N_All_Int_ReacComp");
            histogram_functions::CompareHistograms({sE_miss_2N_pFDpCD, sE_miss_2N_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "E_miss_2N_stack_ReacComp");

            auto hP_miss_2N_VS_E_miss_2N_All_Int_pFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (All Int., pFDpCD)", "TH2D");
            auto hP_miss_2N_VS_E_miss_2N_QEL_pFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (QEL only, pFDpCD)", "TH2D");
            auto hP_miss_2N_VS_E_miss_2N_MEC_pFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (MEC only, pFDpCD)", "TH2D");
            auto hP_miss_2N_VS_E_miss_2N_RES_pFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (RES only, pFDpCD)", "TH2D");
            auto hP_miss_2N_VS_E_miss_2N_DIS_pFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (DIS only, pFDpCD)", "TH2D");
            auto hP_miss_2N_VS_E_miss_2N_All_Int_nFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (All Int., nFDpCD)", "TH2D");
            auto hP_miss_2N_VS_E_miss_2N_QEL_nFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (QEL only, nFDpCD)", "TH2D");
            auto hP_miss_2N_VS_E_miss_2N_MEC_nFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (MEC only, nFDpCD)", "TH2D");
            auto hP_miss_2N_VS_E_miss_2N_RES_nFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (RES only, nFDpCD)", "TH2D");
            auto hP_miss_2N_VS_E_miss_2N_DIS_nFDpCD = (TH2D *)load("P^{2N}_{miss} vs. E^{2N}_{miss} (DIS only, nFDpCD)", "TH2D");
            histogram_functions::CompareHistograms({hP_miss_2N_VS_E_miss_2N_All_Int_pFDpCD, hP_miss_2N_VS_E_miss_2N_All_Int_nFDpCD}, SaveDirFolder, SaveDirFoldersName,
                                                   "P_miss_2N_VS_E_miss_2N_All_Int_ReacComp");
            histogram_functions::CompareHistograms({hP_miss_2N_VS_E_miss_2N_All_Int_pFDpCD, hP_miss_2N_VS_E_miss_2N_QEL_pFDpCD, hP_miss_2N_VS_E_miss_2N_MEC_pFDpCD,
                                                    hP_miss_2N_VS_E_miss_2N_RES_pFDpCD, hP_miss_2N_VS_E_miss_2N_DIS_pFDpCD},
                                                   SaveDirFolder, SaveDirFoldersName, "P_miss_2N_VS_E_miss_2N_ReacComp_pFDpCD");
            histogram_functions::CompareHistograms({hP_miss_2N_VS_E_miss_2N_All_Int_nFDpCD, hP_miss_2N_VS_E_miss_2N_QEL_nFDpCD, hP_miss_2N_VS_E_miss_2N_MEC_nFDpCD,
                                                    hP_miss_2N_VS_E_miss_2N_RES_nFDpCD, hP_miss_2N_VS_E_miss_2N_DIS_nFDpCD},
                                                   SaveDirFolder, SaveDirFoldersName, "P_miss_2N_VS_E_miss_2N_ReacComp_nFDpCD");

            auto hQ2_VS_xB_All_Int_pFDpCD = (TH2D *)load("Q^{2} vs. x_{B} (All Int., pFDpCD)", "TH2D");
            auto hQ2_VS_xB_All_Int_nFDpCD = (TH2D *)load("Q^{2} vs. x_{B} (All Int., nFDpCD)", "TH2D");
            histogram_functions::CompareHistograms({hQ2_VS_xB_All_Int_pFDpCD, hQ2_VS_xB_All_Int_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "Q2_VS_xB_ReacComp_All_Int");

            auto hxB_All_Int_pFDpCD = (TH1D *)load("x_{B} distribution (All Int., pFDpCD)", "TH1D");
            auto hxB_All_Int_nFDpCD = (TH1D *)load("x_{B} distribution (All Int., nFDpCD)", "TH1D");

            histogram_functions::CompareHistograms({hxB_All_Int_pFDpCD, hxB_All_Int_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "xB_All_Int_ReacComp");

            auto sxB_pFDpCD = (THStack *)load("x_{B} distribution (All Int., pFDpCD)", "THStack");
            auto sxB_nFDpCD = (THStack *)load("x_{B} distribution (All Int., nFDpCD)", "THStack");
            histogram_functions::CompareHistograms({sxB_pFDpCD, sxB_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "xB_stack_ReacComp");

            auto hQ2_All_Int_pFDpCD = (TH1D *)load("Q^{2} (pFDpCD, FD)", "TH1D");
            auto hQ2_All_Int_nFDpCD = (TH1D *)load("Q^{2} (nFDpCD, FD)", "TH1D");

            histogram_functions::CompareHistograms({hQ2_All_Int_pFDpCD, hQ2_All_Int_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "Q2_All_Int_ReacComp");

            auto sQ2_pFDpCD = (THStack *)load("Q^{2} (pFDpCD, CD & FD)", "THStack");
            auto sQ2_nFDpCD = (THStack *)load("Q^{2} (nFDpCD, CD & FD)", "THStack");
            histogram_functions::CompareHistograms({sQ2_pFDpCD, sQ2_nFDpCD}, SaveDirFolder, SaveDirFoldersName, "Q2_stack_ReacComp");

            // Clean up
            for (TObject *obj : histograms_to_delete) { delete obj; }

            file->Close();
            delete file;
        }
    }
}