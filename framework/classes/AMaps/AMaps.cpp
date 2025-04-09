//
// Created by Alon Sportes on 22/06/2023.
//

#ifndef AMAPS_H
#define AMAPS_H

#include "AMaps.h"

// AMaps constructors ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* AMaps generation constructor (G1) */
AMaps::AMaps(const std::string &SampleName, const std::string &P_e_bin_profile, const std::string &P_nuc_bin_profile, double beamE, const std::string &AMapsMode, const std::string &SavePath,
             int nOfNucMomBins, int nOfElecMomBins, int hnsNumOfXBins, int hnsNumOfYBins, int hesNumOfXBins, int hesNumOfYBins) {
    AMaps_Mode = AMapsMode;
    SName = SampleName;
    AMapSavePath = SavePath;
    HistNucSliceNumOfXBins = hnsNumOfXBins;
    HistNucSliceNumOfYBins = hnsNumOfYBins;
    HistElectronSliceNumOfXBins = hesNumOfXBins;
    HistElectronSliceNumOfYBins = hesNumOfYBins;
    NumberNucOfMomSlices = nOfNucMomBins;
    NumberElecOfMomSlices = nOfElecMomBins;

#pragma region /* Setting saving directories */
    std::string SavePathAMapsBC = AMapSavePath + "00b_AMaps_BC_from_class/";
    system(("mkdir -p " + SavePathAMapsBC).c_str());

    std::string AMapSavePathTLElectron = SavePath + "01a_Electron_TL_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathTLElectron).c_str());
    std::string AMapSavePathRecoElectron = SavePath + "01b_Electron_Reco_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathRecoElectron).c_str());
    std::string AMapSavePathRecoToTLElectron = SavePath + "01c_Electron_RecoToTL_Ratio/";
    system(("mkdir -p " + AMapSavePathRecoToTLElectron).c_str());
    std::string AMapSavePathSepAMapsElectron = AMapSavePath + "01d_Electron_Separate_AMaps/";
    system(("mkdir -p " + AMapSavePathSepAMapsElectron).c_str());

    std::string AMapSavePathTLProton = SavePath + "02a_Proton_TL_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathTLProton).c_str());
    std::string AMapSavePathRecoProton = SavePath + "02b_Proton_Reco_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathRecoProton).c_str());
    std::string AMapSavePathRecoToTLProton = SavePath + "02c_Proton_RecoToTL_Ratio/";
    system(("mkdir -p " + AMapSavePathRecoToTLProton).c_str());
    std::string AMapSavePathSepAMapsProton = AMapSavePath + "02d_Proton_Separate_AMaps/";
    system(("mkdir -p " + AMapSavePathSepAMapsProton).c_str());

    std::string AMapSavePathTLNeutron = SavePath + "03a_Neutron_TL_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathTLNeutron).c_str());
    std::string AMapSavePathRecoNeutron = SavePath + "03b_Neutron_Reco_Hit_Maps/";
    system(("mkdir -p " + AMapSavePathRecoNeutron).c_str());
    std::string AMapSavePathRecoToTLNeutron = SavePath + "03c_Neutron_RecoToTL_Ratio/";
    system(("mkdir -p " + AMapSavePathRecoToTLNeutron).c_str());
    std::string AMapSavePathSepAMapsNeutron = AMapSavePath + "03d_Neutron_Separate_AMaps/";
    system(("mkdir -p " + AMapSavePathSepAMapsNeutron).c_str());

    std::string AMapSavePathAMap = AMapSavePath + "04_Finalized_AMaps/";
    system(("mkdir -p " + AMapSavePathAMap).c_str());

    std::string AMapSavePathGeneratedAMapCopy = AMapSavePath + "05_Generated_maps/";
    system(("mkdir -p " + AMapSavePathGeneratedAMapCopy).c_str());
    AMapCopySavePath = AMapSavePathGeneratedAMapCopy;
#pragma endregion

#pragma region /* Setting AMapsMode_TitleAddition */
    std::string AMapsMode_TitleAddition;

    if (AMaps_Mode != "") {
        AMapsMode_TitleAddition = AMaps_Mode;
    } else {
        AMapsMode_TitleAddition = "";
    }

    if (!(AMaps_Mode == "AMaps" || AMaps_Mode == "WMaps")) { std::cout << "\n\nAMaps::SaveHitMaps: AMaps_Mode = " << AMaps_Mode << " is not defined! Exiting...\n", exit(0); }

#pragma endregion

    SetBins(P_nuc_bin_profile, beamE);
    SetElectronBins(P_e_bin_profile, beamE);

#pragma region /* Reco theta VS phi BC */

    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        std::string hStatsTitleAMapBCElectron = "Electron_AMap_BC", hTitleAMapBCElectron = "Electron AMap BC", hSaveNameAMapBCElectron = "01_e_AMap_BC";
        reco_theta_e_VS_phi_e_BC = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapBCElectron, hTitleAMapBCElectron, "#phi_{e} [#circ]", "#theta_{e} [#circ]", SavePathAMapsBC,
                                           hSaveNameAMapBCElectron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
    }

    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
        std::string hStatsTitleAMapBCProton = "Proton_AMap_BC", hTitleAMapBCProton = "Proton AMap BC", hSaveNameAMapBCProton = "02_p_AMap_BC";
        reco_theta_p_VS_phi_p_BC = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapBCProton, hTitleAMapBCProton, "#phi_{p} [#circ]", "#theta_{p} [#circ]", SavePathAMapsBC,
                                           hSaveNameAMapBCProton, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    }

    if (basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
        std::string hStatsTitleAMapBCNeutron = "Neutron_AMap_BC", hTitleAMapBCNeutron = "Neutron AMap BC", hSaveNameAMapBCNeutron = "03_n_AMap_BC";
        reco_theta_n_VS_phi_n_BC = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapBCNeutron, hTitleAMapBCNeutron, "#phi_{n} [#circ]", "#theta_{n} [#circ]", SavePathAMapsBC,
                                           hSaveNameAMapBCNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    }

    // std::string hStatsTitleAMapBCNucleon = "Nucleon_AMap_BC", hTitleAMapBCNucleon = "Nucleon AMap BC", hSaveNameAMapBCNucleon = "04_nuc_AMap_BC";
    // reco_theta_nuc_VS_phi_nuc_BC = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapBCNucleon, hTitleAMapBCNucleon, "#phi_{nuc} [#circ]", "#theta_{nuc} [#circ]", SavePathAMapsBC,
    //                                        hSaveNameAMapBCNucleon, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
#pragma endregion

#pragma region /* Setting electron histograms */
    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
            double BinLowerLim = ElectronMomSliceLimits.at(i).at(0), BinUpperLim = ElectronMomSliceLimits.at(i).at(1);

            int BinUpperLimPrecision;
            if (BinUpperLim == beamE) {
                BinUpperLimPrecision = 3;
            } else {
                BinUpperLimPrecision = 2;
            }

            std::string BinDensity = " (" + to_string(HistElectronSliceNumOfXBins) + "x" + to_string(HistElectronSliceNumOfYBins) + ")";

#pragma region /* Setting electron Acceptance maps */

#pragma region /* Electron TL hit map */
            std::string hStatsTitleTLElectron = "TL P_{e} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                                basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
            std::string hTitleTLElectron = "TL P_{e} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                           basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
            std::string hSaveNameTLElectron = to_string(i + 1) + "_TL_P_bin_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                              basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            hPlot2D hPBinTLElectron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleTLElectron, hTitleTLElectron, "#phi_{e} [#circ]", "#theta_{e} [#circ]", AMapSavePathTLElectron,
                                              hSaveNameTLElectron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
            truth_theta_e_VS_phi_e_BySlice.push_back(hPBinTLElectron);
            truth_e_BySlice.push_back(hPBinTLElectron.GetHistogram2D());
#pragma endregion

#pragma region /* Electron Reco. Acceptance maps */
            std::string hStatsTitleRecoElectron = "Reco P_{e} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                                  basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
            std::string hTitleRecoElectron = "Reco P_{e} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                             basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
            std::string hSaveNameRecoElectron = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            hPlot2D hPBinRecoElectron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoElectron, hTitleRecoElectron, "#phi_{e} [#circ]", "#theta_{e} [#circ]", AMapSavePathRecoElectron,
                                                hSaveNameRecoElectron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
            reco_theta_e_VS_phi_e_BySlice.push_back(hPBinRecoElectron);
            reco_e_BySlice.push_back(hPBinRecoElectron.GetHistogram2D());
#pragma endregion

#pragma region /* Electron Reco./TL Ratio */
            std::string hStatsTitleRecoToTLRatioElectron = "Electron Reco/TL ratio for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                                           basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
            std::string hTitleRecoToTLRatioElectron = "Electron Reco/TL ratio for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                                      basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
            std::string hSaveNameRecoToTLRatioElectron = to_string(i + 1) + "_e_Ratio_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                         basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            hPlot2D hPBinRecoToTLRatioElectron =
                hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoToTLRatioElectron, hTitleRecoToTLRatioElectron, "#phi_{e} [#circ]", "#theta_{e} [#circ]", AMapSavePathRecoToTLElectron,
                        hSaveNameRecoToTLRatioElectron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
            acceptance_eff_e_BySlice.push_back(hPBinRecoToTLRatioElectron);
            acc_eff_e_BySlice.push_back(hPBinRecoToTLRatioElectron.GetHistogram2D());
#pragma endregion

#pragma region /* Electron separate AMaps */
            std::string hStatsTitleSepAMapsElectron = "Electron AMap for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                                      basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                      basic_tools::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
            std::string hTitleSepAMapsElectron = "Electron AMap for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" +
                                                 basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                 basic_tools::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
            std::string hSaveNameSepAMapsElectron = to_string(i + 1) + "_e_SepAMap_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                    basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            hPlot2D hPBinSepAMapsElectron =
                hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleSepAMapsElectron, hTitleSepAMapsElectron, "#phi_{e} [#circ]", "#theta_{e} [#circ]", AMapSavePathSepAMapsElectron,
                        hSaveNameSepAMapsElectron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
            filtered_reco_theta_e_VS_phi_e_BySlice.push_back(hPBinSepAMapsElectron);
            filtered_reco_e_BySlice.push_back(hPBinSepAMapsElectron.GetHistogram2D());
#pragma endregion

#pragma endregion
        }  // End of loop over slices of electron momentum
    }
#pragma endregion

#pragma region /* Setting nucleon histograms */

#pragma region /* Setting nucleon slice histograms */
    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_") || basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
            double BinLowerLim = NucleonMomSliceLimits.at(i).at(0), BinUpperLim = NucleonMomSliceLimits.at(i).at(1);

            int BinUpperLimPrecision;
            if (BinUpperLim == beamE) {
                BinUpperLimPrecision = 3;
            } else {
                BinUpperLimPrecision = 2;
            }

            std::string BinDensity = " (" + to_string(HistNucSliceNumOfXBins) + "x" + to_string(HistNucSliceNumOfYBins) + ")";

#pragma region /* Setting proton Acceptance maps */
            if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
#pragma region /* Proton TL Acceptance maps */
                std::string hStatsTitleTLProton = "TL P_{p} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                  basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleTLProton = "TL P_{p} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                             basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameTLProton = to_string(i + 1) + "_TL_P_bin_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                hPlot2D hPBinTLProton = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleTLProton, hTitleTLProton, "#phi_{p} [#circ]", "#theta_{p} [#circ]", AMapSavePathTLProton,
                                                hSaveNameTLProton, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
                truth_theta_p_VS_phi_p_BySlice.push_back(hPBinTLProton);
                truth_p_BySlice.push_back(hPBinTLProton.GetHistogram2D());
#pragma endregion

#pragma region /* Proton Reco. Acceptance maps */
                std::string hStatsTitleRecoProton = "Reco P_{p} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                    basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleRecoProton = "Reco P_{p} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                               basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameRecoProton = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                  basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                hPlot2D hPBinRecoProton = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoProton, hTitleRecoProton, "#phi_{p} [#circ]", "#theta_{p} [#circ]", AMapSavePathRecoProton,
                                                  hSaveNameRecoProton, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
                reco_theta_p_VS_phi_p_BySlice.push_back(hPBinRecoProton);
                reco_p_BySlice.push_back(hPBinRecoProton.GetHistogram2D());
#pragma endregion

#pragma region /* Proton Reco./TL Ratio */
                std::string hStatsTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                             basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                        basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameRecoToTLRatioProton = to_string(i + 1) + "_p_Ratio_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                           basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                hPlot2D hPBinRecoToTLRatioProton =
                    hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoToTLRatioProton, hTitleRecoToTLRatioProton, "#phi_{p} [#circ]", "#theta_{p} [#circ]", AMapSavePathRecoToTLProton,
                            hSaveNameRecoToTLRatioProton, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
                acceptance_eff_p_BySlice.push_back(hPBinRecoToTLRatioProton);
                acc_eff_p_BySlice.push_back(hPBinRecoToTLRatioProton.GetHistogram2D());
#pragma endregion

#pragma region /* Proton separate AMaps */
                std::string hStatsTitleSepAMapsProton = "Proton AMap for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                        basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                        basic_tools::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
                std::string hTitleSepAMapsProton = "Proton AMap for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                                                   basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                   basic_tools::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
                std::string hSaveNameSepAMapsProton = to_string(i + 1) + "_p_SepAMap_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                      basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                hPlot2D hPBinSepAMapsProton =
                    hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleSepAMapsProton, hTitleSepAMapsProton, "#phi_{p} [#circ]", "#theta_{p} [#circ]", AMapSavePathSepAMapsProton,
                            hSaveNameSepAMapsProton, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
                filtered_reco_theta_p_VS_phi_p_BySlice.push_back(hPBinSepAMapsProton);
                filtered_reco_p_BySlice.push_back(hPBinSepAMapsProton.GetHistogram2D());
#pragma endregion
            }
#pragma endregion

#pragma region /* Setting neutron Acceptance maps */
            if (basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
#pragma region /* Neutron TL Acceptance maps */
                std::string hStatsTitleTLNeutron = "TL P_{n} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                                   basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleTLNeutron = "TL P_{n} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                              basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameTLNeutron = to_string(i + 1) + "_TL_P_bin_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                 basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                hPlot2D hPBinTLNeutron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleTLNeutron, hTitleTLNeutron, "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathTLNeutron,
                                                 hSaveNameTLNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
                truth_theta_n_VS_phi_n_BySlice.push_back(hPBinTLNeutron);
                truth_n_BySlice.push_back(hPBinTLNeutron.GetHistogram2D());
#pragma endregion

#pragma region /* Neutron Reco. Acceptance maps */
                std::string hStatsTitleRecoNeutron = "Reco P_{n} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                                     basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleRecoNeutron = "Reco P_{n} bin for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                                basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameRecoNeutron = to_string(i + 1) + "_Reco_P_bin_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                   basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                hPlot2D hPBinRecoNeutron = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoNeutron, hTitleRecoNeutron, "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathRecoNeutron,
                                                   hSaveNameRecoNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
                reco_theta_n_VS_phi_n_BySlice.push_back(hPBinRecoNeutron);
                reco_n_BySlice.push_back(hPBinRecoNeutron.GetHistogram2D());
#pragma endregion

#pragma region /* Neutron Reco./TL Ratio */
                std::string hStatsTitleRecoToTLRatioNeutron = "Neutron Reco/TL ratio for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                                              basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleRecoToTLRatioNeutron = "Neutron Reco/TL ratio for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                                         basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameRecoToTLRatioNeutron = to_string(i + 1) + "_p_Ratio_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                            basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                hPlot2D hPBinRecoToTLRatioNeutron =
                    hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoToTLRatioNeutron, hTitleRecoToTLRatioNeutron, "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathRecoToTLNeutron,
                            hSaveNameRecoToTLRatioNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
                acceptance_eff_n_BySlice.push_back(hPBinRecoToTLRatioNeutron);
                acc_eff_n_BySlice.push_back(hPBinRecoToTLRatioNeutron.GetHistogram2D());
#pragma endregion

#pragma region /* Neutron separate AMaps */
                std::string hStatsTitleSepAMapsNeutron = "Neutron AMap for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                                         basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                         basic_tools::ToStringWithPrecision(Neutral_particle_min_Ratio, 2);
                std::string hTitleSepAMapsNeutron = "Neutron AMap for " + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                                                    basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                    basic_tools::ToStringWithPrecision(Neutral_particle_min_Ratio, 2);
                std::string hSaveNameSepAMapsNeutron = to_string(i + 1) + "_n_SepAMap_for_P_from_" + basic_tools::ToStringWithPrecision(BinLowerLim, 2) + "_to_" +
                                                       basic_tools::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                hPlot2D hPBinSepAMapsNeutron =
                    hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleSepAMapsNeutron, hTitleSepAMapsNeutron, "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathSepAMapsNeutron,
                            hSaveNameSepAMapsNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
                filtered_reco_theta_n_VS_phi_n_BySlice.push_back(hPBinSepAMapsNeutron);
                filtered_reco_n_BySlice.push_back(hPBinSepAMapsNeutron.GetHistogram2D());
#pragma endregion
            }
#pragma endregion
        }  // End of loop over slices of nucleon momentum
    }
#pragma endregion

#pragma region /* Setting neutron Acceptance maps */
    if (basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
#pragma region /* Neutron TL Acceptance maps */
        std::string hStatsTitleTLNeutron = "TL Neutron Hit Map", hTitleTLNeutron = "TL Neutron Hit Map", hSaveNameTLNeutron = "TL_n_AMap";
        truth_theta_n_VS_phi_n = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleTLNeutron, hTitleTLNeutron, "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathTLNeutron,
                                         hSaveNameTLNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
#pragma endregion

#pragma region /* Neutron Reco. Acceptance maps */
        std::string hStatsTitleRecoNeutron = "Reco Neutron Hit Map", hTitleRecoNeutron = "Reco Neutron Hit Map", hSaveNameRecoNeutron = "Reco_n_AMap";
        reco_theta_n_VS_phi_n = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoNeutron, hTitleRecoNeutron, "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathRecoNeutron,
                                        hSaveNameRecoNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
#pragma endregion

#pragma region /* Neutron Reco./TL Ratio */
        std::string hStatsTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hSaveNameRecoToTLNeutron = "Neutron_Ratio";
        acceptance_eff_n = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleRecoToTLNeutron, hTitleRecoToTLNeutron, "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathRecoToTLNeutron,
                                   hSaveNameRecoToTLNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
#pragma endregion
    }
#pragma endregion

#pragma endregion

#pragma region /* Finalized acceptance maps */
    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        std::string hStatsTitleAMapElectron = "Electron_AMap";
        std::string hTitleAMapElectron = "Electron AMap for (Reco./TL)#geq" + basic_tools::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
        std::string hSaveNameAMapElectron = "01_e_AMap";
        filtered_reco_theta_e_VS_phi_e = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapElectron, hTitleAMapElectron, "#phi_{e} [#circ]", "#theta_{e} [#circ]", AMapSavePathAMap,
                                                 hSaveNameAMapElectron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
    }

    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
        std::string hStatsTitleAMapProton = "Proton_AMap";
        std::string hTitleAMapProton = "Proton AMap for (Reco./TL)#geq" + basic_tools::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
        std::string hSaveNameAMapProton = "02_p_AMap";
        filtered_reco_theta_p_VS_phi_p = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapProton, hTitleAMapProton, "#phi_{p} [#circ]", "#theta_{p} [#circ]", AMapSavePathAMap,
                                                 hSaveNameAMapProton, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    }

    if (basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
        std::string hStatsTitleAMapNeutron = "Neutron_AMap";
        std::string hTitleAMapNeutron = "Neutron AMap for (Reco./TL)#geq" + basic_tools::ToStringWithPrecision(Neutral_particle_min_Ratio, 2);
        std::string hSaveNameAMapNeutron = "03_n_AMap";
        filtered_reco_theta_n_VS_phi_n = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapNeutron, hTitleAMapNeutron, "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathAMap,
                                                 hSaveNameAMapNeutron, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    }

    // std::string hStatsTitleAMapNucleon = "Nucleon_AMap";
    // std::string hTitleAMapNucleon = "Nucleon AMap for (Reco./TL)_{n}#geq" + basic_tools::ToStringWithPrecision(Neutral_particle_min_Ratio, 2) + " and (Reco./TL)_{c}#geq" +
    //                                 basic_tools::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
    // std::string hSaveNameAMapNucleon = "04_nuc_AMap";
    // filtered_reco_theta_nuc_VS_phi_nuc = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapNucleon, hTitleAMapNucleon, "#phi_{nuc} [#circ]", "#theta_{nuc} [#circ]", AMapSavePathAMap,
    //                                              hSaveNameAMapNucleon, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
#pragma endregion
}
#pragma endregion

#pragma region /* AMaps loading constructor */
AMaps::AMaps(const std::string &AcceptanceMapsDirectory, const std::string &SampleName, const double &beamE, const std::string &AMapsMode, const bool &Electron_single_slice_test,
             const bool &Nucleon_single_slice_test, const vector<int> &TestSlices) {
    AMaps_Mode = AMapsMode;

    std::string MapsPrefix = (basic_tools::FindSubstring(AMaps_Mode, "AMaps")) ? "AMap" : "WMap";

    std::cout << "\n\nSetting iso samp names\n";
    std::string BeamE_str = basic_tools::GetBeamEnergyFromDouble(beamE);
    std::string Electron_source_folder = "Uniform_1e_sample_" + BeamE_str;
    std::string Proton_source_folder = "Uniform_ep_sample_" + BeamE_str;
    std::string Neutron_source_folder = "Uniform_en_sample_" + BeamE_str;

    std::cout << "\n\nLoad slices and their limits\n";
    /* Load slices and their limits */
    ReadAMapLimits((AcceptanceMapsDirectory + Electron_source_folder + "/e_" + MapsPrefix + "_by_slice/e_slice_limits.par").c_str(), Loaded_ElectronMomSliceLimits);
    ReadAMapLimits((AcceptanceMapsDirectory + Proton_source_folder + "/p_" + MapsPrefix + "_by_slice/p_slice_limits.par").c_str(), Loaded_NucleonMomSliceLimits);
    // ReadAMapLimits((AcceptanceMapsDirectory + SampleName + "/e_AMap_by_slice/e_slice_limits.par").c_str(), Loaded_ElectronMomSliceLimits);
    // ReadAMapLimits((AcceptanceMapsDirectory + SampleName + "/p_AMap_by_slice/p_slice_limits.par").c_str(), Loaded_NucleonMomSliceLimits);

    std::cout << "\n\nLoad separate maps\n";
    /* Load separate maps */
    if (MapsPrefix == "AMap") {
        ReadAMapSlices(Electron_source_folder, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, Loaded_e_AMap_Slices);
        ReadAMapSlices(Proton_source_folder, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, Loaded_p_AMap_Slices);
        ReadAMapSlices(Neutron_source_folder, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, Loaded_n_AMap_Slices);
        ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Nucleon", Loaded_NucleonMomSliceLimits, Loaded_nuc_AMap_Slices);  // TODO: figure out what to do with these

        // TODO: verify that [0] is x and [0][0] is y
        HistElectronSliceNumOfXBins = Loaded_e_AMap_Slices[0].size();     // 100 by Default
        HistElectronSliceNumOfYBins = Loaded_e_AMap_Slices[0][0].size();  // 100 by Default
        HistNucSliceNumOfXBins = Loaded_p_AMap_Slices[0].size();          // 75 by Default
        HistNucSliceNumOfYBins = Loaded_p_AMap_Slices[0][0].size();       // 75 by Default
    } else {
        ReadWMapSlices(Electron_source_folder, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, Loaded_e_WMap_Slices);  // TODO: figure out what to do with these
        ReadWMapSlices(Proton_source_folder, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, Loaded_p_WMap_Slices);       // TODO: figure out what to do with these
        ReadWMapSlices(Neutron_source_folder, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, Loaded_n_WMap_Slices);     // TODO: figure out what to do with these

        // TODO: verify that [0] is x and [0][0] is y
        HistElectronSliceNumOfXBins = Loaded_e_WMap_Slices[0].size();     // 100 by Default
        HistElectronSliceNumOfYBins = Loaded_e_WMap_Slices[0][0].size();  // 100 by Default
        HistNucSliceNumOfXBins = Loaded_p_WMap_Slices[0].size();          // 75 by Default
        HistNucSliceNumOfYBins = Loaded_p_WMap_Slices[0][0].size();       // 75 by Default
    }
    // ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, Loaded_e_AMap_Slices);
    // ReadWMapSlices(SampleName, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, Loaded_e_WMap_Slices);
    // ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, Loaded_p_AMap_Slices);
    // ReadWMapSlices(SampleName, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, Loaded_p_WMap_Slices);
    // ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, Loaded_n_AMap_Slices);
    // ReadWMapSlices(SampleName, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, Loaded_n_WMap_Slices);
    // ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Nucleon", Loaded_NucleonMomSliceLimits, Loaded_nuc_AMap_Slices);

    // std::cout << "\n\nLoad combined maps\n";
    // /* Load combined maps */
    // // TODO: figure out what to do with these
    // ReadAMap((AcceptanceMapsDirectory + SampleName + "/e_AMap_file.par").c_str(), Loaded_e_AMap);
    // ReadAMap((AcceptanceMapsDirectory + SampleName + "/p_AMap_file.par").c_str(), Loaded_p_AMap);
    // ReadAMap((AcceptanceMapsDirectory + SampleName + "/n_AMap_file.par").c_str(), Loaded_n_AMap);
    // ReadAMap((AcceptanceMapsDirectory + SampleName + "/nuc_AMap_file.par").c_str(), Loaded_nuc_AMap);

    // // TODO: verify that [0] is x and [0][0] is y
    // HistElectronSliceNumOfXBins = Loaded_e_AMap_Slices[0].size();     // 100 by Default
    // HistElectronSliceNumOfYBins = Loaded_e_AMap_Slices[0][0].size();  // 100 by Default
    // HistNucSliceNumOfXBins = Loaded_p_AMap_Slices[0].size();          // 75 by Default
    // HistNucSliceNumOfYBins = Loaded_p_AMap_Slices[0][0].size();       // 75 by Default

    e_single_slice_test = Electron_single_slice_test, nuc_single_slice_test = Nucleon_single_slice_test;
    Slices2Test = TestSlices;
}
#pragma endregion

// SetBins functions ----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetBins function (G1a) */
// This function sets the momentum slices for the nucleons (protons and neutrons).
void AMaps::SetBins(const std::string &P_nuc_bin_profile, double beamE) {
    bool InvertedPrintOut = false;
    bool RegPrintOut = false;

    if (P_nuc_bin_profile == "equi_inverted_P_nuc") {
        double InvertedPLowerLim = (1 / beamE);
        double InvertedPUpper = (1 / Nucleon_Momentum_Slice_Th);
        double Delta = (InvertedPUpper - InvertedPLowerLim) / NumberNucOfMomSlices;

        for (int i = 0; i < NumberNucOfMomSlices; i++) {
            double InvertedBinLower = InvertedPLowerLim + i * Delta;
            double InvertedBinUpper = InvertedBinLower + Delta;

            if (InvertedPrintOut) {
                std::cout << "\n\nP_nuc_bin_profile = " << P_nuc_bin_profile << "\n";
                std::cout << "InvertedBinLower = " << InvertedBinLower << "\n";
                std::cout << "InvertedBinUpper = " << InvertedBinUpper << "\n";
                std::cout << "i = " << i << "\n";
                std::cout << "Delta = " << Delta << "\n\n";
            }

            InvertedNucleonMomSliceLimits.push_back({InvertedBinLower, InvertedBinUpper});
        }

        if (InvertedPrintOut && !RegPrintOut) { exit(0); }

        for (int i = (NumberNucOfMomSlices - 1); i >= 0; i--) {
            double BinLower = 1 / InvertedNucleonMomSliceLimits.at(i).at(1);
            double BinUpper = 1 / InvertedNucleonMomSliceLimits.at(i).at(0);

            if (RegPrintOut) {
                std::cout << "\n\nBinLower = " << BinLower << "\n";
                std::cout << "BinUpper = " << BinUpper << "\n";
                std::cout << "i = " << i << "\n";
            }

            NucleonMomSliceLimits.push_back({BinLower, BinUpper});
        }

        if (RegPrintOut) { exit(0); }
    } else if (P_nuc_bin_profile == "varying_P_nuc_bins") {
        // TODO: separate by SampleName?
        bool RegPrintOut = false;

        NucleonMomSliceLimits = CustomNucleonMomSliceLimits_C12x4_simulation_G18_Q204_6GeV;
        NumberNucOfMomSlices = NucleonMomSliceLimits.size();

        if (RegPrintOut) {
            for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
                std::cout << "\n\nP_nuc_bin_profile = " << P_nuc_bin_profile << "\n";
                std::cout << "SliceLowerLimit = " << NucleonMomSliceLimits.at(i).at(0) << "\n";
                std::cout << "SliceUpperLimit = " << NucleonMomSliceLimits.at(i).at(1) << "\n";
                std::cout << "i = " << i << "\n";
            }

            exit(0);
        }
    } else if (P_nuc_bin_profile == "uniform_P_nuc_bins") {
        double PLowerLim = Nucleon_Momentum_Slice_Th;
        double PUpperLim = beamE;
        double Delta = (PUpperLim - PLowerLim) / NumberNucOfMomSlices;

        int Num_of_bins = 0;  // For monitoring purposes only!

        for (int i = 0; i < NumberNucOfMomSlices; i++) {
            double BinLower = PLowerLim + i * Delta;
            double BinUpper = BinLower + Delta;

            NucleonMomSliceLimits.push_back({BinLower, BinUpper});
            ++Num_of_bins;

            if (RegPrintOut) {
                std::cout << "\n\nP_nuc_bin_profile = " << P_nuc_bin_profile << "\n";
                std::cout << "BinLower = " << BinLower << "\n";
                std::cout << "BinUpper = " << BinUpper << "\n";
                std::cout << "i = " << i << "\n";
                std::cout << "Num_of_bins = " << Num_of_bins << "\n";
                std::cout << "Delta = " << Delta << "\n\n";
            }
        }

        if (RegPrintOut) { exit(0); }
    } else {
        std::cout << "AMaps::SetBins: no valid P_nuc_bin_profile selected! Choose between:\n";
        std::cout << "equi_inverted_P_nuc , varying_P_nuc_bins , uniform_P_nuc_bins\n";
        std::cout << "Exiting...", exit(0);
    }
}
#pragma endregion

#pragma region /* SetElectronBins function (G1b) */
// This function sets the momentum slices for the electron.
void AMaps::SetElectronBins(const std::string &P_e_bin_profile, double beamE) {
    bool InvertedPrintOut = false;
    bool RegPrintOut = false;

    if (P_e_bin_profile == "reformat_e_bins") {
        double InvertedPLowerLim = (1 / beamE);
        double InvertedPUpperLim = (1 / Nucleon_Momentum_Slice_Th);
        double p2 = 1.57819;
        double Delta = (InvertedPUpperLim - InvertedPLowerLim) / NumberNucOfMomSlices;
        double delta = (InvertedPUpperLim - (1 / p2));
        double Ratio = (InvertedPUpperLim - InvertedPLowerLim) / delta;

        int NumOfElectronMomBins;

        bool SliceAndDice = true;
        int iter = 0;

        while (SliceAndDice) {
            double InvBinLower;
            double InvBinUpper;
            double deltaLoop = delta;

            if (iter == 0) {
                InvBinUpper = InvertedPUpperLim;
                InvBinLower = InvBinUpper - deltaLoop;
            } else {
                InvBinUpper = InvBinLower;

                deltaLoop = delta / 12;

                if (InvBinUpper - deltaLoop > 0) {
                    InvBinLower = InvBinUpper - deltaLoop;
                } else {
                    InvBinLower = InvertedPLowerLim;
                }
            }

            if (InvertedPrintOut) {
                std::cout << "\n\nP_e_bin_profile = " << P_e_bin_profile << "\n";
                std::cout << "InvBinLower = " << InvBinLower << "\n";
                std::cout << "InvBinUpper = " << InvBinUpper << "\n";
                std::cout << "iter = " << iter << "\n";
                std::cout << "delta = " << delta << "\n";
                std::cout << "deltaLoop = " << deltaLoop << "\n\n";
            }

            if (InvBinLower >= InvertedPLowerLim) {
                InvertedElectronMomSliceLimits.push_back({InvBinLower, InvBinUpper});
                ++iter;
            } else {
                SliceAndDice = false;
            }
        }

        NumOfElectronMomBins = InvertedElectronMomSliceLimits.size();

        if (InvertedPrintOut && !RegPrintOut) { exit(0); }

        if (RegPrintOut) { std::cout << "\n\n---------------------------------------------------\n"; }

        for (int i = 0; i < NumOfElectronMomBins; i++) {
            double BinLower = 1 / InvertedElectronMomSliceLimits.at(i).at(1);
            double BinUpper = 1 / InvertedElectronMomSliceLimits.at(i).at(0);

            if (RegPrintOut) {
                std::cout << "\n\nP_e_bin_profile = " << P_e_bin_profile << "\n";
                std::cout << "BinLower = " << BinLower << "\n";
                std::cout << "BinUpper = " << BinUpper << "\n";
                std::cout << "i = " << i << "\n";
            }

            ElectronMomSliceLimits.push_back({BinLower, BinUpper});
        }

        if (RegPrintOut) { exit(0); }
    } else if (P_e_bin_profile == "varying_P_e_bins") {
        if (basic_tools::FindSubstring(SName, "C12_simulation_6GeV_T5")) {  // Old sample
            ElectronMomSliceLimits = CustomElectronMomSliceLimits_C12_simulation_6GeV_T5;
        } else {  // New sample (24M; 1-foil & 4-foil)
            ElectronMomSliceLimits = CustomElectronMomSliceLimits_C12x4_simulation_G18_Q204_6GeV;
        }

        int NumOfElectronMomBins = ElectronMomSliceLimits.size();

        if (RegPrintOut) {
            for (int i = 0; i < NumOfElectronMomBins; i++) {
                std::cout << "\n\nP_e_bin_profile = " << P_e_bin_profile << "\n";
                std::cout << "ElectronMomSliceLimits.at(" << i << ").at(" << 0 << ") = " << ElectronMomSliceLimits.at(i).at(0) << "\n";
                std::cout << "ElectronMomSliceLimits.at(" << i << ").at(" << 1 << ") = " << ElectronMomSliceLimits.at(i).at(1) << "\n";
            }
        }

        if (RegPrintOut) { exit(0); }
    } else if (P_e_bin_profile == "uniform_P_e_bins") {
        double PLowerLim = 0;
        double PUpper = beamE;
        double Delta = (PUpper - PLowerLim) / NumberElecOfMomSlices;

        int Num_of_bins = 0;  // For monitoring purposes only!

        for (int i = 0; i < NumberElecOfMomSlices; i++) {
            double BinLower = PLowerLim + i * Delta;
            double BinUpper = BinLower + Delta;

            ElectronMomSliceLimits.push_back({BinLower, BinUpper});
            ++Num_of_bins;

            if (RegPrintOut) {
                std::cout << "\n\nP_e_bin_profile = " << P_e_bin_profile << "\n";
                std::cout << "BinLower = " << BinLower << "\n";
                std::cout << "BinUpper = " << BinUpper << "\n";
                std::cout << "i = " << i << "\n";
                std::cout << "Num_of_bins = " << Num_of_bins << "\n";
                std::cout << "Delta = " << Delta << "\n\n";
            }
        }

        if (RegPrintOut) { exit(0); }
    } else if (P_e_bin_profile == "equi_inverted_P_e") {
        double InvertedPLowerLim = (1 / beamE);
        double InvertedPUpper = (1 / Electron_Momentum_Slice_Th);
        double Delta = (InvertedPUpper - InvertedPLowerLim) / NumberElecOfMomSlices;

        for (int i = 0; i < (NumberElecOfMomSlices - 1); i++) {
            double InvertedBinLower = InvertedPLowerLim + i * Delta;
            double InvertedBinUpper = InvertedBinLower + Delta;

            if (InvertedPrintOut) {
                std::cout << "\n\nP_e_bin_profile = " << P_e_bin_profile << "\n";
                std::cout << "InvertedBinLower = " << InvertedBinLower << "\n";
                std::cout << "InvertedBinUpper = " << InvertedBinUpper << "\n";
                std::cout << "i = " << i << "\n";
                std::cout << "Delta = " << Delta << "\n\n";
            }

            InvertedElectronMomSliceLimits.push_back({InvertedBinLower, InvertedBinUpper});
        }

        if (InvertedPrintOut && !RegPrintOut) { exit(0); }

        if (RegPrintOut) { std::cout << "\n\n---------------------------------------------\n\n"; }

        for (int i = 0; i < (NumberElecOfMomSlices - 1); i++) {
            double BinLower = beamE - (1 / InvertedElectronMomSliceLimits.at(i).at(0));
            double BinUpper = beamE - (1 / InvertedElectronMomSliceLimits.at(i).at(1));

            ElectronMomSliceLimits.push_back({BinLower, BinUpper});
        }

        ElectronMomSliceLimits.push_back({ElectronMomSliceLimits.at(ElectronMomSliceLimits.size() - 1).at(1), beamE});

        if (RegPrintOut) {
            for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
                double BinLower = ElectronMomSliceLimits.at(i).at(0);
                double BinUpper = ElectronMomSliceLimits.at(i).at(1);

                std::cout << "\nP_e_bin_profile = " << P_e_bin_profile << "\n";
                std::cout << "BinLower = " << BinLower << "\n";
                std::cout << "BinUpper = " << BinUpper << "\n";
                std::cout << "i = " << i << "\n\n";
            }
        }

        if (RegPrintOut) { exit(0); }
    } else {
        std::cout << "AMaps::SetElectronBins: no valid P_e_bin_profile selected! Choose between:\n";
        std::cout << "reformat_e_bins , varying_P_e_bins , uniform_P_e_bins, equi_inverted_P_e\n";
        std::cout << "Exiting...", exit(0);
    }
}
#pragma endregion

#pragma region /* SetBins function (old) */
void AMaps::SetBins(double beamE, double nOfMomBins) {
    double BinUpperLim = beamE;

    bool SliceAndDice = true;
    int BinNumber = 1;

    while (SliceAndDice) {
        double UpperLim, LowerLim;

        if (BinNumber == 1) {
            UpperLim = BinUpperLim;
            LowerLim = (BinUpperLim / nOfMomBins);
        } else {
            UpperLim = LowerLim;
            LowerLim = (UpperLim / nOfMomBins);
        }

        NucleonMomSliceLimits.push_back({LowerLim, UpperLim});

        if (LowerLim <= Nucleon_Momentum_Slice_Th) {
            SliceAndDice = false;
        } else {
            ++BinNumber;
        }
    }
}
#pragma endregion

// isElectron function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isElectron function */
bool AMaps::isElectron(const std::string &SampleType) {
    if (SampleType == "Electron" || SampleType == "electron") {
        return true;
    } else {
        return false;
    }
}
#pragma endregion

// isProton function ----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isProton function */
bool AMaps::isProton(const std::string &SampleType) {
    if (SampleType == "Proton" || SampleType == "proton") {
        return true;
    } else {
        return false;
    }
}
#pragma endregion

// isNeutron function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isNeutron function */
bool AMaps::isNeutron(const std::string &SampleType) {
    if (SampleType == "Neutron" || SampleType == "neutron") {
        return true;
    } else {
        return false;
    }
}
#pragma endregion

// isTL function --------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isTL function */
bool AMaps::isTL(const std::string &SampleType) {
    if (SampleType == "Truth" || SampleType == "truth" || SampleType == "TL" || SampleType == "truth level" || SampleType == "truth-level" || SampleType == "Truth-Level") {
        return true;
    } else {
        return false;
    }
}
#pragma endregion

// isReco function --------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isReco function */
bool AMaps::isReco(const std::string &SampleType) {
    if (SampleType == "reco" || SampleType == "Reco" || SampleType == "Reconstruction") {
        return true;
    } else {
        return false;
    }
}
#pragma endregion

// hFillMaps function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* hFillMaps function (G2) */
void AMaps::hFillMaps(const std::string &SampleType, const std::string &particle, double Momentum, double Theta, double Phi, double Weight) {
    bool is_e = isElectron(particle), is_p = isProton(particle), is_n = isNeutron(particle);
    bool is_TL = isTL(SampleType), is_Reco = isReco(SampleType);

    bool TL_e_PrintOut = false, TL_p_PrintOut = false, TL_n_PrintOut = false;
    bool Reco_e_PrintOut = false, Reco_p_PrintOut = false, Reco_n_PrintOut = false;

#pragma region /* Safety checks (AMaps::hFillMaps) */
    if (is_e && is_p && is_n) { std::cout << "\n\nAMaps::hFillMaps: particle can't all particles! Exiting...\n", exit(0); }
    if (!is_e && !is_p && !is_n) { std::cout << "\n\nAMaps::hFillMaps: particle must be an electron, proton or neutron! Exiting...\n", exit(0); }
    if (is_e && is_p) { std::cout << "\n\nAMaps::hFillMaps: particle can't be both electrons and protons! Exiting...\n", exit(0); }
    if (is_e && is_n) { std::cout << "\n\nAMaps::hFillMaps: particle can't be both electrons and neutrons! Exiting...\n", exit(0); }
    if (is_p && is_n) { std::cout << "\n\nAMaps::hFillMaps: particle can't be both protons and neutrons! Exiting...\n", exit(0); }

    if (is_TL && is_Reco) { std::cout << "\n\nAMaps::hFillMaps: particle can't be both TL and Reco! Exiting...\n", exit(0); }
    if (!is_TL && !is_Reco) { std::cout << "\n\nAMaps::hFillMaps: particle must be either TL and Reco! Exiting...\n", exit(0); }
#pragma endregion

    if (is_TL) {
        if (is_e && basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {  // electrons are charged -> look for correct momentum slice!
            for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
                if ((Momentum >= ElectronMomSliceLimits.at(i).at(0)) && (Momentum < ElectronMomSliceLimits.at(i).at(1))) {
                    truth_theta_e_VS_phi_e_BySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && TL_e_PrintOut) {
                        std::cout << "\n";
                        std::cout << "ElectronMomSliceLimits.at(" << i << ").at(0) = " << ElectronMomSliceLimits.at(i).at(0) << "\n";
                        std::cout << "ElectronMomSliceLimits.at(" << i << ").at(1) = " << ElectronMomSliceLimits.at(i).at(1) << "\n";
                        std::cout << "particle = " << particle << "\n";
                        std::cout << "is_e = " << is_e << "\n";
                        std::cout << "is_TL = " << is_TL << "\n";
                        std::cout << "is_Reco = " << is_Reco << "\n";
                        std::cout << "Momentum = " << Momentum << "\n";
                        std::cout << "Theta = " << Theta << "\n";
                        std::cout << "Phi = " << Phi << "\n";
                    }

#pragma region /* Safety checks (AMaps::hFillMaps) */
                    if (ElectronMomSliceLimits.at(i).at(0) > ElectronMomSliceLimits.at(i).at(1)) {
                        std::cout << "\n\nAMaps::hFillMaps: electron momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
#pragma endregion

                    break;  // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_p && basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {  // protons are charged -> look for correct momentum slice!
            for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
                if ((Momentum >= NucleonMomSliceLimits.at(i).at(0)) && (Momentum < NucleonMomSliceLimits.at(i).at(1))) {
                    truth_theta_p_VS_phi_p_BySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && TL_p_PrintOut) {
                        std::cout << "\n";
                        std::cout << "NucleonMomSliceLimits.at(" << i << ").at(0) = " << NucleonMomSliceLimits.at(i).at(0) << "\n";
                        std::cout << "NucleonMomSliceLimits.at(" << i << ").at(1) = " << NucleonMomSliceLimits.at(i).at(1) << "\n";
                        std::cout << "particle = " << particle << "\n";
                        std::cout << "is_p = " << is_p << "\n";
                        std::cout << "is_TL = " << is_TL << "\n";
                        std::cout << "is_Reco = " << is_Reco << "\n";
                        std::cout << "Momentum = " << Momentum << "\n";
                        std::cout << "Theta = " << Theta << "\n";
                        std::cout << "Phi = " << Phi << "\n";
                    }

#pragma region /* Safety checks (AMaps::hFillMaps) */
                    if (NucleonMomSliceLimits.at(i).at(0) > NucleonMomSliceLimits.at(i).at(1)) {
                        std::cout << "\n\nAMaps::hFillMaps: nucleon momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
#pragma endregion

                    break;  // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_n && basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {  // neutrons are neutral -> same fill all slices!
            truth_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);

            for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
                if ((Momentum >= NucleonMomSliceLimits.at(i).at(0)) && (Momentum < NucleonMomSliceLimits.at(i).at(1))) {
                    truth_theta_n_VS_phi_n_BySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && TL_n_PrintOut) {
                        std::cout << "\n";
                        std::cout << "NucleonMomSliceLimits.at(" << i << ").at(0) = " << NucleonMomSliceLimits.at(i).at(0) << "\n";
                        std::cout << "NucleonMomSliceLimits.at(" << i << ").at(1) = " << NucleonMomSliceLimits.at(i).at(1) << "\n";
                        std::cout << "particle = " << particle << "\n";
                        std::cout << "is_n = " << is_n << "\n";
                        std::cout << "is_TL = " << is_TL << "\n";
                        std::cout << "is_Reco = " << is_Reco << "\n";
                        std::cout << "Momentum = " << Momentum << "\n";
                        std::cout << "Theta = " << Theta << "\n";
                        std::cout << "Phi = " << Phi << "\n";
                    }

#pragma region /* Safety checks (AMaps::hFillMaps) */
                    if (NucleonMomSliceLimits.at(i).at(0) > NucleonMomSliceLimits.at(i).at(1)) {
                        std::cout << "\n\nAMaps::hFillMaps: nucleon momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
#pragma endregion

                    break;  // no need to keep the loop going after filling histogram
                }
            }

            /*
            // truth_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);

            // for (int Bin = 0; Bin < NucleonMomSliceLimits.size(); Bin++) { truth_theta_n_VS_phi_n_BySlice.at(Bin).hFill(Phi, Theta, Weight); }
            */
        }
    } else if (is_Reco) {
        if (is_e && basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {  // electrons are charged -> look for correct momentum slice!
            reco_theta_e_VS_phi_e_BC.hFill(Phi, Theta, Weight);

            for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
                if ((Momentum >= ElectronMomSliceLimits.at(i).at(0)) && (Momentum < ElectronMomSliceLimits.at(i).at(1))) {
                    reco_theta_e_VS_phi_e_BySlice.at(i).hFill(Phi, Theta, Weight);
                    acceptance_eff_e_BySlice.at(i).hFill(Phi, Theta, Weight);
                    filtered_reco_theta_e_VS_phi_e_BySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && Reco_e_PrintOut) {
                        std::cout << "\n";
                        std::cout << "ElectronMomSliceLimits.at(" << i << ").at(0) = " << ElectronMomSliceLimits.at(i).at(0) << "\n";
                        std::cout << "ElectronMomSliceLimits.at(" << i << ").at(1) = " << ElectronMomSliceLimits.at(i).at(1) << "\n";
                        std::cout << "particle = " << particle << "\n";
                        std::cout << "is_p = " << is_p << "\n";
                        std::cout << "is_TL = " << is_TL << "\n";
                        std::cout << "is_Reco = " << is_Reco << "\n";
                        std::cout << "Momentum = " << Momentum << "\n";
                        std::cout << "Theta = " << Theta << "\n";
                        std::cout << "Phi = " << Phi << "\n";
                    }

#pragma region /* Safety checks (AMaps::hFillMaps) */
                    if (ElectronMomSliceLimits.at(i).at(0) > ElectronMomSliceLimits.at(i).at(1)) {
                        std::cout << "\n\nAMaps::hFillMaps: electron momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
#pragma endregion

                    break;  // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_p && basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {  // protons are charged -> look for correct momentum slice!
            reco_theta_p_VS_phi_p_BC.hFill(Phi, Theta, Weight);

            for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
                if ((Momentum >= NucleonMomSliceLimits.at(i).at(0)) && (Momentum < NucleonMomSliceLimits.at(i).at(1))) {
                    reco_theta_p_VS_phi_p_BySlice.at(i).hFill(Phi, Theta, Weight);
                    acceptance_eff_p_BySlice.at(i).hFill(Phi, Theta, Weight);
                    filtered_reco_theta_p_VS_phi_p_BySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && Reco_p_PrintOut) {
                        std::cout << "\n";
                        std::cout << "NucleonMomSliceLimits.at(" << i << ").at(0) = " << NucleonMomSliceLimits.at(i).at(0) << "\n";
                        std::cout << "NucleonMomSliceLimits.at(" << i << ").at(1) = " << NucleonMomSliceLimits.at(i).at(1) << "\n";
                        std::cout << "particle = " << particle << "\n";
                        std::cout << "is_p = " << is_p << "\n";
                        std::cout << "is_TL = " << is_TL << "\n";
                        std::cout << "is_Reco = " << is_Reco << "\n";
                        std::cout << "Momentum = " << Momentum << "\n";
                        std::cout << "Theta = " << Theta << "\n";
                        std::cout << "Phi = " << Phi << "\n";
                    }

#pragma region /* Safety checks (AMaps::hFillMaps) */
                    if (NucleonMomSliceLimits.at(i).at(0) > NucleonMomSliceLimits.at(i).at(1)) {
                        std::cout << "\n\nAMaps::hFillMaps: nucleon momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
#pragma endregion

                    break;  // no need to keep the loop going after filling histogram
                }
            }
        } else if (is_n && basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {  // neutrons are neutral -> same fill all slices!
            reco_theta_n_VS_phi_n_BC.hFill(Phi, Theta, Weight);

            reco_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);
            acceptance_eff_n.hFill(Phi, Theta, Weight);
            filtered_reco_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);

            for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
                if ((Momentum >= NucleonMomSliceLimits.at(i).at(0)) && (Momentum < NucleonMomSliceLimits.at(i).at(1))) {
                    reco_theta_n_VS_phi_n_BySlice.at(i).hFill(Phi, Theta, Weight);
                    acceptance_eff_n_BySlice.at(i).hFill(Phi, Theta, Weight);
                    filtered_reco_theta_n_VS_phi_n_BySlice.at(i).hFill(Phi, Theta, Weight);

                    if (AMaps_Mode == "AMaps" && Reco_n_PrintOut) {
                        std::cout << "\n";
                        std::cout << "NucleonMomSliceLimits.at(" << i << ").at(0) = " << NucleonMomSliceLimits.at(i).at(0) << "\n";
                        std::cout << "NucleonMomSliceLimits.at(" << i << ").at(1) = " << NucleonMomSliceLimits.at(i).at(1) << "\n";
                        std::cout << "particle = " << particle << "\n";
                        std::cout << "is_n = " << is_n << "\n";
                        std::cout << "is_TL = " << is_TL << "\n";
                        std::cout << "is_Reco = " << is_Reco << "\n";
                        std::cout << "Momentum = " << Momentum << "\n";
                        std::cout << "Theta = " << Theta << "\n";
                        std::cout << "Phi = " << Phi << "\n";
                    }

#pragma region /* Safety checks (AMaps::hFillMaps) */
                    if (NucleonMomSliceLimits.at(i).at(0) > NucleonMomSliceLimits.at(i).at(1)) {
                        std::cout << "\n\nAMaps::hFillMaps: nucleon momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                    }
#pragma endregion

                    break;  // no need to keep the loop going after filling histogram
                }
            }

            /*
            for (int Bin = 0; Bin < NucleonMomSliceLimits.size(); Bin++) {
                reco_theta_n_VS_phi_n_BySlice.at(Bin).hFill(Phi, Theta, Weight);
                acceptance_eff_n_BySlice.at(Bin).hFill(Phi, Theta, Weight);
                filtered_reco_theta_n_VS_phi_n_BySlice.at(Bin).hFill(Phi, Theta, Weight);
            }
            */
        }
    }
}
#pragma endregion

// CalcAcceptanceEfficiency function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcAcceptanceEfficiency function (G3a) */
void AMaps::CalcAcceptanceEfficiency() {
    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {  // Old sample
        std::cout << "\n\nCalculating electron acceptance efficiency...";
        for (int i = 0; i < ElectronMomSliceLimits.size(); i++) { acceptance_eff_e_BySlice.at(i).hDivision(truth_theta_e_VS_phi_e_BySlice.at(i).GetHistogram2D()); }
        std::cout << " done!\n";
    }

    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
        std::cout << "\n\nCalculating proton acceptance efficiency...";
        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) { acceptance_eff_p_BySlice.at(i).hDivision(truth_theta_p_VS_phi_p_BySlice.at(i).GetHistogram2D()); }
        std::cout << " done!\n";
    }

    if (basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
        std::cout << "\n\nCalculating neutron acceptance efficiency...";
        acceptance_eff_n.hDivision(truth_theta_n_VS_phi_n.GetHistogram2D());

        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) { acceptance_eff_n_BySlice.at(i).hDivision(truth_theta_n_VS_phi_n_BySlice.at(i).GetHistogram2D()); }
        std::cout << " done!\n";
    }
}
#pragma endregion

// GenerateFilteredRecoMaps function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateFilteredRecoMaps function (G3b) */
void AMaps::GenerateFilteredRecoMaps(double cP_minR, double nP_minR) {
    // Generate filtered reco. electron maps
    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        for (int bin = 0; bin < ElectronMomSliceLimits.size(); bin++) {
            for (int i = 0; i < (HistElectronSliceNumOfXBins + 1); i++) {
                for (int j = 0; j < (HistElectronSliceNumOfYBins + 1); j++) {
                    if (acceptance_eff_e_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { filtered_reco_theta_e_VS_phi_e_BySlice.at(bin).hFillByBin(i, j, 0); }
                }
            }
        }
    }


    std::cout << "\n\nNucleonMomSliceLimits.size() = " << NucleonMomSliceLimits.size() << "\n";


    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_") || basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
        for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
            // Generate filtered reco. proton maps
            if (basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
                for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
                    for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
                        if (acceptance_eff_p_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { filtered_reco_theta_p_VS_phi_p_BySlice.at(bin).hFillByBin(i, j, 0); }
                    }
                }
            }

            // Generate filtered reco. neutron maps
            if (basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
                for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
                    for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
                        if (acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < nP_minR) { filtered_reco_theta_n_VS_phi_n_BySlice.at(bin).hFillByBin(i, j, 0); }
                    }
                }
            }
        }
    }
}
#pragma endregion

// GenerateMapMatrices function ----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateMapMatrices function (G3b) */
void AMaps::GenerateMapMatrices(double cP_minR, double nP_minR) {
    // Generate electron map matrices
    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        for (int bin = 0; bin < ElectronMomSliceLimits.size(); bin++) {
            if (AMaps_Mode == "AMaps") { acceptance_eff_e_BySlice.at(bin).ApplyZMaxLim(1.2); }  // TODO: move from here

            /* Fill e_AMap_Slices */
            vector<vector<int>> e_AMap_slice;
            vector<vector<double>> e_WMap_slice;

            for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
                vector<int> e_AMap_slice_col;
                vector<double> e_WMap_slice_col;

                for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                    if (acceptance_eff_e_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                        e_AMap_slice_col.push_back(1);
                        e_WMap_slice_col.push_back(acceptance_eff_e_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1));
                    } else {
                        e_AMap_slice_col.push_back(0);
                        e_WMap_slice_col.push_back(0);
                    }
                }

                e_AMap_slice.push_back(e_AMap_slice_col);
                e_WMap_slice.push_back(e_WMap_slice_col);
            }

            e_AMap_Slices.push_back(e_AMap_slice);
            e_WMap_Slices.push_back(e_WMap_slice);
        }
    }

    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_") || basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
        for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
            // Generate proton map matrices
            if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
                if (AMaps_Mode == "AMaps") { acceptance_eff_p_BySlice.at(bin).ApplyZMaxLim(1.2); }  // TODO: move from here

                /* Fill p_AMap_Slices */
                vector<vector<int>> p_AMap_slice;
                vector<vector<double>> p_WMap_slice;

                for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                    vector<int> p_AMap_slice_col;
                    vector<double> p_WMap_slice_col;

                    for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                        if (acceptance_eff_p_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                            p_AMap_slice_col.push_back(1);
                            p_WMap_slice_col.push_back(acceptance_eff_p_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1));
                        } else {
                            p_AMap_slice_col.push_back(0);
                            p_WMap_slice_col.push_back(0);
                        }
                    }

                    p_AMap_slice.push_back(p_AMap_slice_col);
                    p_WMap_slice.push_back(p_WMap_slice_col);
                }

                p_AMap_Slices.push_back(p_AMap_slice);
                p_WMap_Slices.push_back(p_WMap_slice);
            }

            // Generate neutron map matrices
            if (basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
                for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
                    for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
                        if (acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < nP_minR) { filtered_reco_theta_n_VS_phi_n_BySlice.at(bin).hFillByBin(i, j, 0); }
                    }
                }

                if (AMaps_Mode == "AMaps") { acceptance_eff_n_BySlice.at(bin).ApplyZMaxLim(1.2); }  // TODO: move from here

                /* Fill n_AMap_Slices */
                vector<vector<int>> n_AMap_slice;
                vector<vector<double>> n_WMap_slice;

                for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                    vector<int> n_AMap_slice_col;
                    vector<double> n_WMap_slice_col;

                    for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                        if (acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= nP_minR) {
                            n_AMap_slice_col.push_back(1);
                            n_WMap_slice_col.push_back(acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1));
                        } else {
                            n_AMap_slice_col.push_back(0);
                            n_WMap_slice_col.push_back(0);
                        }
                    }

                    n_AMap_slice.push_back(n_AMap_slice_col);
                    n_WMap_slice.push_back(n_WMap_slice_col);
                }

                n_AMap_Slices.push_back(n_AMap_slice);
                n_WMap_Slices.push_back(n_WMap_slice);
            }
        }
    }
}
#pragma endregion

// GenerateFilteredRecoCPartMaps function ----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateFilteredRecoCPartMaps function - old (G3ba) */
void AMaps::GenerateFilteredRecoCPartMaps(double cP_minR) {
    // Generate electron map matrices
    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        for (int bin = 0; bin < ElectronMomSliceLimits.size(); bin++) {
            for (int i = 0; i < (HistElectronSliceNumOfXBins + 1); i++) {
                for (int j = 0; j < (HistElectronSliceNumOfYBins + 1); j++) {
                    if (acceptance_eff_e_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { filtered_reco_theta_e_VS_phi_e_BySlice.at(bin).hFillByBin(i, j, 0); }
                }
            }

            // TODO: move from here
            if (AMaps_Mode == "AMaps") { acceptance_eff_e_BySlice.at(bin).ApplyZMaxLim(1.2); }

#pragma region /* Fill e_AMap_Slices */
            vector<vector<int>> e_AMap_slice;
            vector<vector<double>> e_WMap_slice;

            for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
                vector<int> e_AMap_slice_col;
                vector<double> e_WMap_slice_col;

                for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                    if (acceptance_eff_e_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                        e_AMap_slice_col.push_back(1);
                        e_WMap_slice_col.push_back(acceptance_eff_e_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1));
                    } else {
                        e_AMap_slice_col.push_back(0);
                        e_WMap_slice_col.push_back(0);
                    }
                }

                e_AMap_slice.push_back(e_AMap_slice_col);
                e_WMap_slice.push_back(e_WMap_slice_col);
            }

            e_AMap_Slices.push_back(e_AMap_slice);
            e_WMap_Slices.push_back(e_WMap_slice);
#pragma endregion
        }
    }

    // Generate proton map matrices
    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
        for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
            for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
                for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
                    if (acceptance_eff_p_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { filtered_reco_theta_p_VS_phi_p_BySlice.at(bin).hFillByBin(i, j, 0); }
                }
            }

            // TODO: move from here
            if (AMaps_Mode == "AMaps") { acceptance_eff_p_BySlice.at(bin).ApplyZMaxLim(1.2); }

#pragma region /* Fill p_AMap_Slices */
            vector<vector<int>> p_AMap_slice;
            vector<vector<double>> p_WMap_slice;

            for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                vector<int> p_AMap_slice_col;
                vector<double> p_WMap_slice_col;

                for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                    if (acceptance_eff_p_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                        p_AMap_slice_col.push_back(1);
                        p_WMap_slice_col.push_back(acceptance_eff_p_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1));
                    } else {
                        p_AMap_slice_col.push_back(0);
                        p_WMap_slice_col.push_back(0);
                    }
                }

                p_AMap_slice.push_back(p_AMap_slice_col);
                p_WMap_slice.push_back(p_WMap_slice_col);
            }

            p_AMap_Slices.push_back(p_AMap_slice);
            p_WMap_Slices.push_back(p_WMap_slice);
#pragma endregion
        }
    }
}
#pragma endregion

// GenerateCPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateCPartAMaps function (G3b) */
void AMaps::GenerateCPartAMaps(double cP_minR) {
    GenerateFilteredRecoCPartMaps(cP_minR);

    // Fill electron finalized maps
    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        for (int bin = 0; bin < ElectronMomSliceLimits.size(); bin++) { filtered_reco_theta_e_VS_phi_e.hAdd(filtered_reco_theta_e_VS_phi_e_BySlice.at(bin).GetHistogram2D()); }
    }

    // Fill proton finalized maps
    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
        for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) { filtered_reco_theta_p_VS_phi_p.hAdd(filtered_reco_theta_p_VS_phi_p_BySlice.at(bin).GetHistogram2D()); }
    }

    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
            vector<int> e_AMap_col;

            for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                if (filtered_reco_theta_e_VS_phi_e.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                    e_AMap_col.push_back(1);
                } else {
                    e_AMap_col.push_back(0);
                }
            }

            e_AMap.push_back(e_AMap_col);
        }
    }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        vector<int> p_AMap_col;

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (filtered_reco_theta_p_VS_phi_p.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                p_AMap_col.push_back(1);
            } else {
                p_AMap_col.push_back(0);
            }
        }

        p_AMap.push_back(p_AMap_col);
    }
}
#pragma endregion

// GenerateNPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateNPartAMaps function */
void AMaps::GenerateNPartAMaps(double nP_minR) {
    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
        for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
            for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
                if (acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < nP_minR) { filtered_reco_theta_n_VS_phi_n_BySlice.at(bin).hFillByBin(i, j, 0); }
            }
        }

        if (AMaps_Mode == "AMaps") {
            // TODO: move from here
            acceptance_eff_n_BySlice.at(bin).ApplyZMaxLim(1.2);
        }

#pragma region /* Fill p_AMap_Slices */
        vector<vector<int>> n_AMap_slice;
        vector<vector<double>> n_WMap_slice;

        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            vector<int> n_AMap_slice_col;
            vector<double> n_WMap_slice_col;

            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if (acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= nP_minR) {
                    n_AMap_slice_col.push_back(1);
                    n_WMap_slice_col.push_back(acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1));
                } else {
                    n_AMap_slice_col.push_back(0);
                    n_WMap_slice_col.push_back(0);
                }
            }

            n_AMap_slice.push_back(n_AMap_slice_col);
            n_WMap_slice.push_back(n_WMap_slice_col);
        }

        n_AMap_Slices.push_back(n_AMap_slice);
        n_WMap_Slices.push_back(n_WMap_slice);
#pragma endregion
    }

    // TODO: recheck if need n_AMap and n_WMap if we're moving to neutron maps by momentum slices
    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        vector<int> n_AMap_col;
        vector<double> n_WMap_col;

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (acceptance_eff_n.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= nP_minR) {
                n_AMap_col.push_back(1);
                n_WMap_col.push_back(acceptance_eff_n.GetHistogram2D()->GetBinContent(j + 1, i + 1));
            } else {
                n_AMap_col.push_back(0);
                n_WMap_col.push_back(0);
            }
        }

        n_AMap.push_back(n_AMap_col);
        n_WMap.push_back(n_WMap_col);
    }

    // Fill finalized neutron maps
    // TODO: recheck if filtered_reco_theta_n_VS_phi_n should be here
    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) { filtered_reco_theta_n_VS_phi_n_BySlice.push_back(filtered_reco_theta_n_VS_phi_n); }

    /*
    for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
        for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
            if (acceptance_eff_n.GetHistogram2D()->GetBinContent(i, j) < nP_minR) {
                filtered_reco_theta_n_VS_phi_n.hFillByBin(i, j, 0);

                for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) { filtered_reco_theta_n_VS_phi_n_BySlice.at(bin).hFillByBin(i, j, 0); }
            }
        }
    }

    if (AMaps_Mode == "AMaps") {
        for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) { acceptance_eff_n_BySlice.at(bin).ApplyZMaxLim(1.2); }

        acceptance_eff_n.ApplyZMaxLim(1.2);
    }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        vector<int> n_AMap_col;
        vector<double> n_WMap_col;

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (acceptance_eff_n.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= nP_minR) {
                n_AMap_col.push_back(1);
                n_WMap_col.push_back(acceptance_eff_n.GetHistogram2D()->GetBinContent(j + 1, i + 1));
            } else {
                n_AMap_col.push_back(0);
                n_WMap_col.push_back(0);
            }
        }

        n_AMap.push_back(n_AMap_col);
        n_WMap.push_back(n_WMap_col);
    }

    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
        filtered_reco_theta_n_VS_phi_n_BySlice.push_back(filtered_reco_theta_n_VS_phi_n);
        n_AMap_Slices.push_back(n_AMap);
        n_WMap_Slices.push_back(n_WMap);
    }
    */
}
#pragma endregion

// GenerateNucleonAMap function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateNucleonAMap function - old (G3c) */
void AMaps::GenerateNucleonAMap() {
    for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
        for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
            if ((reco_theta_n_VS_phi_n_BC.GetHistogram2D()->GetBinContent(i, j) != 0) && (reco_theta_p_VS_phi_p_BC.GetHistogram2D()->GetBinContent(i, j) != 0)) {
                reco_theta_nuc_VS_phi_nuc_BC.hFillByBin(i, j, reco_theta_n_VS_phi_n.GetHistogram2D()->GetBinContent(i, j));
            }

            if ((filtered_reco_theta_n_VS_phi_n.GetHistogram2D()->GetBinContent(i, j) != 0) && (filtered_reco_theta_p_VS_phi_p.GetHistogram2D()->GetBinContent(i, j) != 0)) {
                filtered_reco_theta_nuc_VS_phi_nuc.hFillByBin(i, j, reco_theta_n_VS_phi_n.GetHistogram2D()->GetBinContent(i, j));
            }
        }
    }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        vector<int> nuc_col;

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (filtered_reco_theta_nuc_VS_phi_nuc.GetHistogram2D()->GetBinContent(j + 1, i + 1) != 0) {
                nuc_col.push_back(1);
            } else {
                nuc_col.push_back(0);
            }
        }

        nuc_AMap.push_back(nuc_col);
    }

    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
        vector<vector<int>> nuc_slice;

        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            vector<int> nuc_col;

            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if ((p_AMap_Slices.at(bin).at(i).at(j) == 1) && (n_AMap_Slices.at(bin).at(i).at(j) == 1)) {
                    nuc_col.push_back(1);
                } else {
                    nuc_col.push_back(0);
                }
            }

            nuc_slice.push_back(nuc_col);
        }

        nuc_AMap_Slices.push_back(nuc_slice);
        nuc_WMap_Slices.push_back(nuc_slice);  // TODO: figure out if really need these!
    }
}
#pragma endregion

// SaveMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SaveMaps function (G3d) */

// TODO: separate into AMaps and WMaps

void AMaps::SaveMaps(const std::string &SampleName, const std::string &AcceptanceMapsDirectory) {
    bool PrintOut = false;

    int testNumber = 0;

    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        std::string AMapSliceElectronSavePath = AcceptanceMapsDirectory + SampleName + "/e_" + AMaps_Mode + "_by_slice/";
        system(("mkdir -p " + AMapSliceElectronSavePath).c_str());
        std::string WMapSliceElectronSavePath = AcceptanceMapsDirectory + SampleName + "/e_WMap_by_slice/";
        system(("mkdir -p " + WMapSliceElectronSavePath).c_str());
        std::string AMapSliceElectronSavePathCopy = AMapCopySavePath + "/e_" + AMaps_Mode + "_by_slice/";
        system(("mkdir -p " + AMapSliceElectronSavePathCopy).c_str());
        std::string WMapSliceElectronSavePathCopy = AMapCopySavePath + "/e_WMap_by_slice/";
        system(("mkdir -p " + WMapSliceElectronSavePathCopy).c_str());

        if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

        /* Save electron slices */
        for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
            std::ofstream e_AMap_TempFile, e_WMap_TempFile;

            std::string AMapTempFileName = "e_" + AMaps_Mode + "_file_from_" + basic_tools::ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                           basic_tools::ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(1), 2) + ".par";
            std::string WMapTempFileName = "e_WMap_file_from_" + basic_tools::ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                           basic_tools::ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(1), 2) + ".par";

            // e_AMap_TempFile.open(AMapSliceElectronSavePath + AMapTempFileName);
            // e_WMap_TempFile.open(WMapSliceElectronSavePath + WMapTempFileName);
            e_AMap_TempFile = std::ofstream(AMapSliceElectronSavePath + AMapTempFileName);  // Opens in overwrite mode
            e_WMap_TempFile = std::ofstream(WMapSliceElectronSavePath + WMapTempFileName);

            e_AMap_TempFile << "Lower_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(0) << "\n";
            e_AMap_TempFile << "Upper_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
            e_AMap_TempFile << "\n";

            e_WMap_TempFile << "Lower_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(0) << "\n";
            e_WMap_TempFile << "Upper_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
            e_WMap_TempFile << "\n";

            for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
                e_AMap_TempFile << "Line\t";
                e_WMap_TempFile << "Weight\t";

                for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                    if (j != HistElectronSliceNumOfXBins - 1) {
                        e_AMap_TempFile << e_AMap_Slices.at(Slice).at(i).at(j) << ":";
                        e_WMap_TempFile << e_WMap_Slices.at(Slice).at(i).at(j) << ":";
                    } else {
                        e_AMap_TempFile << e_AMap_Slices.at(Slice).at(i).at(j);
                        e_WMap_TempFile << e_WMap_Slices.at(Slice).at(i).at(j);
                    }
                }

                e_AMap_TempFile << "\n";
                e_WMap_TempFile << "\n";
            }

            e_AMap_TempFile.close();
            e_WMap_TempFile.close();

            system(("cp " + AMapSliceElectronSavePath + AMapTempFileName + " " + AMapSliceElectronSavePathCopy + AMapTempFileName).c_str());
            system(("cp " + WMapSliceElectronSavePath + WMapTempFileName + " " + WMapSliceElectronSavePathCopy + WMapTempFileName).c_str());
        }

        if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

#pragma region /* Slice limits */
        std::ofstream e_slice_limits;

        e_slice_limits = std::ofstream(AMapSliceElectronSavePath + "e_slice_limits.par");  // Opens in overwrite mode

        if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

        for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
            e_slice_limits << "e_slice_" << (Slice + 1) << "\t" << ElectronMomSliceLimits.at(Slice).at(0) << ":" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
        }

        e_slice_limits << "\n";

        e_slice_limits.close();

        system(("cp " + AMapSliceElectronSavePath + "e_slice_limits.par " + AMapSliceElectronSavePathCopy).c_str());

        system(("cp " + WMapSliceElectronSavePath + "e_slice_limits.par " + WMapSliceElectronSavePathCopy).c_str());
#pragma endregion
    }

    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
        std::string AMapSliceProtonSavePath = AcceptanceMapsDirectory + SampleName + "/p_" + AMaps_Mode + "_by_slice/";
        system(("mkdir -p " + AMapSliceProtonSavePath).c_str());
        std::string WMapSliceProtonSavePath = AcceptanceMapsDirectory + SampleName + "/p_WMap_by_slice/";
        system(("mkdir -p " + WMapSliceProtonSavePath).c_str());
        std::string AMapSliceProtonSavePathCopy = AMapCopySavePath + "/p_" + AMaps_Mode + "_by_slice/";
        system(("mkdir -p " + AMapSliceProtonSavePathCopy).c_str());
        std::string WMapSliceProtonSavePathCopy = AMapCopySavePath + "/p_WMap_by_slice/";
        system(("mkdir -p " + WMapSliceProtonSavePathCopy).c_str());

        if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

        /* Save proton slices */
        for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
            std::ofstream p_AMap_TempFile, p_WMap_TempFile;

            std::string ProtonAMapTempFileName = "p_" + AMaps_Mode + "_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                                 basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
            std::string ProtonWMapTempFileName = "p_WMap_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                                 basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";

            p_AMap_TempFile = std::ofstream(AMapSliceProtonSavePath + ProtonAMapTempFileName);  // Opens in overwrite mode
            p_WMap_TempFile = std::ofstream(WMapSliceProtonSavePath + ProtonWMapTempFileName);

            p_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
            p_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
            p_AMap_TempFile << "\n";

            p_WMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
            p_WMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
            p_WMap_TempFile << "\n";

            for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                p_AMap_TempFile << "Line\t";
                p_WMap_TempFile << "Weight\t";

                for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                    if (j != HistNucSliceNumOfXBins - 1) {
                        p_AMap_TempFile << p_AMap_Slices.at(Slice).at(i).at(j) << ":";
                        p_WMap_TempFile << p_WMap_Slices.at(Slice).at(i).at(j) << ":";
                    } else {
                        p_AMap_TempFile << p_AMap_Slices.at(Slice).at(i).at(j);
                        p_WMap_TempFile << p_WMap_Slices.at(Slice).at(i).at(j);
                    }
                }

                p_AMap_TempFile << "\n";
                p_WMap_TempFile << "\n";
            }

            p_AMap_TempFile.close();
            p_WMap_TempFile.close();

            system(("cp " + AMapSliceProtonSavePath + ProtonAMapTempFileName + " " + AMapSliceProtonSavePathCopy + ProtonAMapTempFileName).c_str());
            system(("cp " + WMapSliceProtonSavePath + ProtonWMapTempFileName + " " + WMapSliceProtonSavePathCopy + ProtonWMapTempFileName).c_str());
        }

        if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

#pragma region /* Slice limits */
        std::ofstream p_slice_limits;

        p_slice_limits = std::ofstream(AMapSliceProtonSavePath + "p_slice_limits.par");

        if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

        for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
            p_slice_limits << "p_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        }

        p_slice_limits << "\n";

        p_slice_limits.close();

        system(("cp " + AMapSliceProtonSavePath + "p_slice_limits.par " + AMapSliceProtonSavePathCopy).c_str());

        system(("cp " + WMapSliceProtonSavePath + "p_slice_limits.par " + WMapSliceProtonSavePathCopy).c_str());
#pragma endregion
    }

    if (basic_tools::FindSubstring(SName, "Uniform_en_sample_")) {
        std::string AMapSliceNeutronSavePath = AcceptanceMapsDirectory + SampleName + "/n_" + AMaps_Mode + "_by_slice/";
        system(("mkdir -p " + AMapSliceNeutronSavePath).c_str());
        std::string WMapSliceNeutronSavePath = AcceptanceMapsDirectory + SampleName + "/n_WMap_by_slice/";
        system(("mkdir -p " + WMapSliceNeutronSavePath).c_str());
        std::string AMapSliceNeutronSavePathCopy = AMapCopySavePath + "/n_" + AMaps_Mode + "_by_slice/";
        system(("mkdir -p " + AMapSliceNeutronSavePathCopy).c_str());
        std::string WMapSliceNeutronSavePathCopy = AMapCopySavePath + "/n_WMap_by_slice/";
        system(("mkdir -p " + WMapSliceNeutronSavePathCopy).c_str());

        if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

        /* Save neutron slices */
        for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
            std::ofstream n_AMap_TempFile, n_WMap_TempFile;

            std::string NeutronAMapTempFileName = "n_" + AMaps_Mode + "_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                                  basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
            std::string NeutronWMapTempFileName = "n_WMap_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                                  basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";

            n_AMap_TempFile = std::ofstream(AMapSliceNeutronSavePath + NeutronAMapTempFileName);  // Opens in overwrite mode
            n_WMap_TempFile = std::ofstream(WMapSliceNeutronSavePath + NeutronWMapTempFileName);

            n_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
            n_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
            n_AMap_TempFile << "\n";

            n_WMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
            n_WMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
            n_WMap_TempFile << "\n";

            for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                n_AMap_TempFile << "Line\t";
                n_WMap_TempFile << "Weight\t";

                for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                    if (j != HistNucSliceNumOfXBins - 1) {
                        n_AMap_TempFile << n_AMap_Slices.at(Slice).at(i).at(j) << ":";
                        n_WMap_TempFile << n_WMap_Slices.at(Slice).at(i).at(j) << ":";
                    } else {
                        n_AMap_TempFile << n_AMap_Slices.at(Slice).at(i).at(j);
                        n_WMap_TempFile << n_WMap_Slices.at(Slice).at(i).at(j);
                    }
                }

                n_AMap_TempFile << "\n";
                n_WMap_TempFile << "\n";
            }

            n_AMap_TempFile.close();
            n_WMap_TempFile.close();

            system(("cp " + AMapSliceNeutronSavePath + NeutronAMapTempFileName + " " + AMapSliceNeutronSavePathCopy + NeutronAMapTempFileName).c_str());
            system(("cp " + WMapSliceNeutronSavePath + NeutronWMapTempFileName + " " + WMapSliceNeutronSavePathCopy + NeutronWMapTempFileName).c_str());
        }

        if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

#pragma region /* Slice limits */
        std::ofstream n_slice_limits;

        n_slice_limits = std::ofstream(AMapSliceNeutronSavePath + "n_slice_limits.par");

        if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

        for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
            n_slice_limits << "n_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        }

        n_slice_limits << "\n";

        n_slice_limits.close();

        system(("cp " + AMapSliceNeutronSavePath + "n_slice_limits.par " + AMapSliceNeutronSavePathCopy).c_str());

        system(("cp " + WMapSliceNeutronSavePath + "n_slice_limits.par " + WMapSliceNeutronSavePathCopy).c_str());
#pragma endregion
    }
}
#pragma endregion

// SaveHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SaveHitMaps function - old (G3d) */

// TODO: separate into AMaps and WMaps

void AMaps::SaveHitMaps(const std::string &SampleName, const std::string &AcceptanceMapsDirectory) {
    bool PrintOut = false;

    int testNumber = 0;

    std::string AMapSliceElectronSavePath = AcceptanceMapsDirectory + SampleName + "/e_" + AMaps_Mode + "_by_slice/";
    system(("mkdir -p " + AMapSliceElectronSavePath).c_str());
    std::string WMapSliceElectronSavePath = AcceptanceMapsDirectory + SampleName + "/e_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceElectronSavePath).c_str());
    std::string AMapSliceElectronSavePathCopy = AMapCopySavePath + "/e_" + AMaps_Mode + "_by_slice/";
    system(("mkdir -p " + AMapSliceElectronSavePathCopy).c_str());
    std::string WMapSliceElectronSavePathCopy = AMapCopySavePath + "/e_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceElectronSavePathCopy).c_str());

    std::string AMapSliceProtonSavePath = AcceptanceMapsDirectory + SampleName + "/p_" + AMaps_Mode + "_by_slice/";
    system(("mkdir -p " + AMapSliceProtonSavePath).c_str());
    std::string WMapSliceProtonSavePath = AcceptanceMapsDirectory + SampleName + "/p_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceProtonSavePath).c_str());
    std::string AMapSliceProtonSavePathCopy = AMapCopySavePath + "/p_" + AMaps_Mode + "_by_slice/";
    system(("mkdir -p " + AMapSliceProtonSavePathCopy).c_str());
    std::string WMapSliceProtonSavePathCopy = AMapCopySavePath + "/p_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceProtonSavePathCopy).c_str());

    std::string AMapSliceNeutronSavePath = AcceptanceMapsDirectory + SampleName + "/n_" + AMaps_Mode + "_by_slice/";
    system(("mkdir -p " + AMapSliceNeutronSavePath).c_str());
    std::string WMapSliceNeutronSavePath = AcceptanceMapsDirectory + SampleName + "/n_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNeutronSavePath).c_str());
    std::string AMapSliceNeutronSavePathCopy = AMapCopySavePath + "/n_" + AMaps_Mode + "_by_slice/";
    system(("mkdir -p " + AMapSliceNeutronSavePathCopy).c_str());
    std::string WMapSliceNeutronSavePathCopy = AMapCopySavePath + "/n_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNeutronSavePathCopy).c_str());

    std::string AMapSliceNucleonSavePath = AcceptanceMapsDirectory + SampleName + "/nuc_" + AMaps_Mode + "_by_slice/";
    system(("mkdir -p " + AMapSliceNucleonSavePath).c_str());
    std::string WMapSliceNucleonSavePath = AcceptanceMapsDirectory + SampleName + "/nuc_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNucleonSavePath).c_str());
    std::string AMapSliceNucleonSavePathCopy = AMapCopySavePath + "/nuc_" + AMaps_Mode + "_by_slice/";
    system(("mkdir -p " + AMapSliceNucleonSavePathCopy).c_str());
    std::string WMapSliceNucleonSavePathCopy = AMapCopySavePath + "/nuc_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNucleonSavePathCopy).c_str());

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

#pragma region /* Save electron slices */fileExists
    for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
        std::ofstream e_AMap_TempFile, e_WMap_TempFile;

        std::string AMapTempFileName = "e_" + AMaps_Mode + "_file_from_" + basic_tools::ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                       basic_tools::ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string WMapTempFileName = "e_WMap_file_from_" + basic_tools::ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                       basic_tools::ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(1), 2) + ".par";

        // e_AMap_TempFile.open(AMapSliceElectronSavePath + AMapTempFileName);
        // e_WMap_TempFile.open(WMapSliceElectronSavePath + WMapTempFileName);
        e_AMap_TempFile = std::ofstream(AMapSliceElectronSavePath + AMapTempFileName);  // Opens in overwrite mode
        e_WMap_TempFile = std::ofstream(WMapSliceElectronSavePath + WMapTempFileName);

        e_AMap_TempFile << "Lower_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(0) << "\n";
        e_AMap_TempFile << "Upper_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
        e_AMap_TempFile << "\n";

        e_WMap_TempFile << "Lower_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(0) << "\n";
        e_WMap_TempFile << "Upper_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
        e_WMap_TempFile << "\n";

        for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
            e_AMap_TempFile << "Line\t";
            e_WMap_TempFile << "Weight\t";

            for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                if (j != HistElectronSliceNumOfXBins - 1) {
                    e_AMap_TempFile << e_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    e_WMap_TempFile << e_WMap_Slices.at(Slice).at(i).at(j) << ":";
                } else {
                    e_AMap_TempFile << e_AMap_Slices.at(Slice).at(i).at(j);
                    e_WMap_TempFile << e_WMap_Slices.at(Slice).at(i).at(j);
                }
            }

            e_AMap_TempFile << "\n";
            e_WMap_TempFile << "\n";
        }

        e_AMap_TempFile.close();
        e_WMap_TempFile.close();

        system(("cp " + AMapSliceElectronSavePath + AMapTempFileName + " " + AMapSliceElectronSavePathCopy + AMapTempFileName).c_str());
        system(("cp " + WMapSliceElectronSavePath + WMapTempFileName + " " + WMapSliceElectronSavePathCopy + WMapTempFileName).c_str());
    }
#pragma endregion

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

#pragma region /* Save proton, neutron & nucleon slices */
    for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
        std::ofstream p_AMap_TempFile, p_WMap_TempFile, n_AMap_TempFile, n_WMap_TempFile, nuc_AMap_TempFile, nuc_WMap_TempFile;

        std::string ProtonAMapTempFileName = "p_" + AMaps_Mode + "_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                             basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string ProtonWMapTempFileName = "p_WMap_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                             basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string NeutronAMapTempFileName = "n_" + AMaps_Mode + "_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                              basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string NeutronWMapTempFileName = "n_WMap_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                              basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string NucleonAMapTempFileName = "nuc_" + AMaps_Mode + "_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                              basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string NucleonWMapTempFileName = "nuc_WMap_file_from_" + basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" +
                                              basic_tools::ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";

        // p_AMap_TempFile.open(AMapSliceProtonSavePath + ProtonAMapTempFileName);
        // p_WMap_TempFile.open(WMapSliceProtonSavePath + ProtonWMapTempFileName);
        // n_AMap_TempFile.open(AMapSliceNeutronSavePath + NeutronAMapTempFileName);
        // n_WMap_TempFile.open(WMapSliceNeutronSavePath + NeutronWMapTempFileName);
        // nuc_AMap_TempFile.open(AMapSliceNucleonSavePath + NucleonAMapTempFileName);
        // nuc_WMap_TempFile.open(WMapSliceNucleonSavePath + NucleonWMapTempFileName);
        p_AMap_TempFile = std::ofstream(AMapSliceProtonSavePath + ProtonAMapTempFileName);  // Opens in overwrite mode
        p_WMap_TempFile = std::ofstream(WMapSliceProtonSavePath + ProtonWMapTempFileName);
        n_AMap_TempFile = std::ofstream(AMapSliceNeutronSavePath + NeutronAMapTempFileName);
        n_WMap_TempFile = std::ofstream(WMapSliceNeutronSavePath + NeutronWMapTempFileName);
        nuc_AMap_TempFile = std::ofstream(AMapSliceNucleonSavePath + NucleonAMapTempFileName);
        nuc_WMap_TempFile = std::ofstream(WMapSliceNucleonSavePath + NucleonWMapTempFileName);

        p_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        p_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        p_AMap_TempFile << "\n";
        n_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        n_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        n_AMap_TempFile << "\n";
        nuc_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        nuc_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        nuc_AMap_TempFile << "\n";

        p_WMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        p_WMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        p_WMap_TempFile << "\n";
        n_WMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        n_WMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        n_WMap_TempFile << "\n";
        nuc_WMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        nuc_WMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        nuc_WMap_TempFile << "\n";

        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            p_AMap_TempFile << "Line\t";
            p_WMap_TempFile << "Weight\t";
            n_AMap_TempFile << "Line\t";
            n_WMap_TempFile << "Weight\t";
            nuc_AMap_TempFile << "Line\t";
            nuc_WMap_TempFile << "Weight\t";

            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if (j != HistNucSliceNumOfXBins - 1) {
                    p_AMap_TempFile << p_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    p_WMap_TempFile << p_WMap_Slices.at(Slice).at(i).at(j) << ":";
                    n_AMap_TempFile << n_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    n_WMap_TempFile << n_WMap_Slices.at(Slice).at(i).at(j) << ":";
                    nuc_AMap_TempFile << nuc_AMap_Slices.at(Slice).at(i).at(j) << ":";
                    nuc_WMap_TempFile << nuc_WMap_Slices.at(Slice).at(i).at(j) << ":";
                } else {
                    p_AMap_TempFile << p_AMap_Slices.at(Slice).at(i).at(j);
                    p_WMap_TempFile << p_WMap_Slices.at(Slice).at(i).at(j);
                    n_AMap_TempFile << n_AMap_Slices.at(Slice).at(i).at(j);
                    n_WMap_TempFile << n_WMap_Slices.at(Slice).at(i).at(j);
                    nuc_AMap_TempFile << nuc_AMap_Slices.at(Slice).at(i).at(j);
                    nuc_WMap_TempFile << nuc_WMap_Slices.at(Slice).at(i).at(j);
                }
            }

            p_AMap_TempFile << "\n";
            p_WMap_TempFile << "\n";
            n_AMap_TempFile << "\n";
            n_WMap_TempFile << "\n";
            nuc_AMap_TempFile << "\n";
            nuc_WMap_TempFile << "\n";
        }

        p_AMap_TempFile.close();
        p_WMap_TempFile.close();
        n_AMap_TempFile.close();
        n_WMap_TempFile.close();
        nuc_AMap_TempFile.close();
        nuc_WMap_TempFile.close();

        system(("cp " + AMapSliceProtonSavePath + ProtonAMapTempFileName + " " + AMapSliceProtonSavePathCopy + ProtonAMapTempFileName).c_str());
        system(("cp " + WMapSliceProtonSavePath + ProtonWMapTempFileName + " " + WMapSliceProtonSavePathCopy + ProtonWMapTempFileName).c_str());
        system(("cp " + AMapSliceNeutronSavePath + NeutronAMapTempFileName + " " + AMapSliceNeutronSavePathCopy + NeutronAMapTempFileName).c_str());
        system(("cp " + WMapSliceNeutronSavePath + NeutronWMapTempFileName + " " + WMapSliceNeutronSavePathCopy + NeutronWMapTempFileName).c_str());
        system(("cp " + AMapSliceNucleonSavePath + NucleonAMapTempFileName + " " + AMapSliceNucleonSavePathCopy + NucleonAMapTempFileName).c_str());
        system(("cp " + WMapSliceNucleonSavePath + NucleonWMapTempFileName + " " + WMapSliceNucleonSavePathCopy + NucleonWMapTempFileName).c_str());
    }
#pragma endregion

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

#pragma region /* Save combined maps */
    // TODO: figure out rather or not to keep these combind maps!
    std::ofstream e_AMap_file, p_AMap_file, n_AMap_file, nuc_AMap_file;

    // e_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/e_" + AMaps_Mode + "_file.par");
    // p_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/p_" + AMaps_Mode + "_file.par");
    // n_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/n_" + AMaps_Mode + "_file.par");
    // nuc_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/nuc_" + AMaps_Mode + "_file.par");
    e_AMap_file = std::ofstream(AcceptanceMapsDirectory + SampleName + "/e_" + AMaps_Mode + "_file.par");  // Opens in overwrite mode
    p_AMap_file = std::ofstream(AcceptanceMapsDirectory + SampleName + "/p_" + AMaps_Mode + "_file.par");
    n_AMap_file = std::ofstream(AcceptanceMapsDirectory + SampleName + "/n_" + AMaps_Mode + "_file.par");
    nuc_AMap_file = std::ofstream(AcceptanceMapsDirectory + SampleName + "/nuc_" + AMaps_Mode + "_file.par");

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
        e_AMap_file << "e_slice_" << (Slice + 1) << "\t" << ElectronMomSliceLimits.at(Slice).at(0) << ":" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
    }

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
        p_AMap_file << "p_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        n_AMap_file << "n_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        nuc_AMap_file << "nuc_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
    }

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
        e_AMap_file << "Line\t";

        for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
            if (j != HistElectronSliceNumOfXBins - 1) {
                e_AMap_file << e_AMap.at(i).at(j) << ":";
            } else {
                e_AMap_file << e_AMap.at(i).at(j);
            }
        }

        e_AMap_file << "\n";
    }

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        p_AMap_file << "Line\t";
        n_AMap_file << "Line\t";
        nuc_AMap_file << "Line\t";

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (j != HistNucSliceNumOfXBins - 1) {
                if (PrintOut) { std::cout << "\n\np_" + AMaps_Mode + "_file << p_AMap.at(" << i << ").at(" << j << ") <<\n"; }
                p_AMap_file << p_AMap.at(i).at(j) << ":";
                if (PrintOut) { std::cout << "\n\nn_" + AMaps_Mode + "_file << n_AMap.at(" << i << ").at(" << j << ") <<\n"; }
                n_AMap_file << n_AMap.at(i).at(j) << ":";
                if (PrintOut) { std::cout << "\n\nnuc_" + AMaps_Mode + "_file << nuc_AMap.at(" << i << ").at(" << j << ") <<\n"; }
                nuc_AMap_file << nuc_AMap.at(i).at(j) << ":";
            } else {
                if (PrintOut) { std::cout << "\n\np_" + AMaps_Mode + "_file << p_AMap.at(" << i << ").at(" << j << ") <<\n"; }
                p_AMap_file << p_AMap.at(i).at(j);
                if (PrintOut) { std::cout << "\n\nn_" + AMaps_Mode + "_file << n_AMap.at(" << i << ").at(" << j << ") <<\n"; }
                n_AMap_file << n_AMap.at(i).at(j);
                if (PrintOut) { std::cout << "\n\nnuc_" + AMaps_Mode + "_file << nuc_AMap.at(" << i << ").at(" << j << ") <<\n"; }
                nuc_AMap_file << nuc_AMap.at(i).at(j);
            }
        }

        p_AMap_file << "\n";
        n_AMap_file << "\n";
        nuc_AMap_file << "\n";
    }

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

    e_AMap_file.close();
    p_AMap_file.close();
    n_AMap_file.close();
    nuc_AMap_file.close();

    system(("cp " + AcceptanceMapsDirectory + SampleName + "/e_" + AMaps_Mode + "_file.par " + AMapCopySavePath).c_str());
    system(("cp " + AcceptanceMapsDirectory + SampleName + "/p_" + AMaps_Mode + "_file.par " + AMapCopySavePath).c_str());
    system(("cp " + AcceptanceMapsDirectory + SampleName + "/n_" + AMaps_Mode + "_file.par " + AMapCopySavePath).c_str());
    system(("cp " + AcceptanceMapsDirectory + SampleName + "/nuc_" + AMaps_Mode + "_file.par " + AMapCopySavePath).c_str());
#pragma endregion

#pragma region /* Slice limits */
    std::ofstream e_slice_limits, p_slice_limits, n_slice_limits, nuc_slice_limits;

    // e_slice_limits.open(AMapSliceElectronSavePath + "e_slice_limits.par");
    // p_slice_limits.open(AMapSliceProtonSavePath + "p_slice_limits.par");
    // n_slice_limits.open(AMapSliceNeutronSavePath + "n_slice_limits.par");
    // nuc_slice_limits.open(AMapSliceNucleonSavePath + "nuc_slice_limits.par");
    e_slice_limits = std::ofstream(AMapSliceElectronSavePath + "e_slice_limits.par");  // Opens in overwrite mode
    p_slice_limits = std::ofstream(AMapSliceProtonSavePath + "p_slice_limits.par");
    n_slice_limits = std::ofstream(AMapSliceNeutronSavePath + "n_slice_limits.par");
    nuc_slice_limits = std::ofstream(AMapSliceNucleonSavePath + "nuc_slice_limits.par");

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
        e_slice_limits << "e_slice_" << (Slice + 1) << "\t" << ElectronMomSliceLimits.at(Slice).at(0) << ":" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
    }

    if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
        p_slice_limits << "p_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        n_slice_limits << "n_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        nuc_slice_limits << "nuc_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
    }

    e_slice_limits << "\n";
    p_slice_limits << "\n";
    n_slice_limits << "\n";
    nuc_slice_limits << "\n";

    e_slice_limits.close();
    p_slice_limits.close();
    n_slice_limits.close();
    nuc_slice_limits.close();

    system(("cp " + AMapSliceElectronSavePath + "e_slice_limits.par " + AMapSliceElectronSavePathCopy).c_str());
    system(("cp " + AMapSliceProtonSavePath + "p_slice_limits.par " + AMapSliceProtonSavePathCopy).c_str());
    system(("cp " + AMapSliceNeutronSavePath + "n_slice_limits.par " + AMapSliceNeutronSavePathCopy).c_str());
    system(("cp " + AMapSliceNucleonSavePath + "nuc_slice_limits.par " + AMapSliceNucleonSavePathCopy).c_str());

    system(("cp " + WMapSliceElectronSavePath + "e_slice_limits.par " + WMapSliceElectronSavePathCopy).c_str());
    system(("cp " + WMapSliceProtonSavePath + "p_slice_limits.par " + WMapSliceProtonSavePathCopy).c_str());
    system(("cp " + WMapSliceNeutronSavePath + "n_slice_limits.par " + WMapSliceNeutronSavePathCopy).c_str());
    system(("cp " + WMapSliceNucleonSavePath + "nuc_slice_limits.par " + WMapSliceNucleonSavePathCopy).c_str());
#pragma endregion
}
#pragma endregion

// DrawAndSaveMapsPDFs function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveMapsPDFs function */
void AMaps::DrawAndSaveMapsPDFs(vector<TObject *> HistoList, const std::string &PDFfileName) {
    TCanvas *myText_temp = new TCanvas("myText_temp", "myText_temp", pixelx, pixely);
    TCanvas *myCanvas_temp = new TCanvas("myCanvas_temp", "myCanvas_temp", pixelx, pixely);

    char fileName[PDFfileName.length()];
    sprintf(fileName, "%s[", PDFfileName.c_str());
    myText_temp->SaveAs(fileName);
    sprintf(fileName, "%s", PDFfileName.c_str());

    for (int i = 0; i < HistoList.size(); i++) {
        myCanvas_temp->cd();

        myCanvas_temp->cd()->SetGrid();
        myCanvas_temp->cd()->SetBottomMargin(0.14), myCanvas_temp->cd()->SetLeftMargin(0.16), myCanvas_temp->cd()->SetRightMargin(0.12);

        if (TH1 *hist = dynamic_cast<TH1 *>(HistoList[i])) {
            HistoList[i]->Draw("colz");

            hist->SetStats(1);
            // HistoList[i]->SetStats(1);
            gStyle->SetOptStat("ourmen");
            gStyle->SetStatX(0.98);   // gStyle->SetStatX(0.87);
            gStyle->SetStatY(0.935);  // gStyle->SetStatY(0.875);
            gPad->Modified();
            gPad->Update();

            gPad->Update();
            TPaletteAxis *palette = (TPaletteAxis *)hist->GetListOfFunctions()->FindObject("palette");
            // TPaletteAxis *palette = (TPaletteAxis *)HistoList[i]->GetListOfFunctions()->FindObject("palette");
            if (palette) { palette->SetY2NDC(0.55), gPad->Modified(), gPad->Update(); }
        }

        myCanvas_temp->Print(fileName, "pdf");
        myCanvas_temp->Clear();
    }

    sprintf(fileName, "%s]", PDFfileName.c_str());
    myCanvas_temp->Print(fileName, "pdf");

    delete myText_temp;
    delete myCanvas_temp;
}
#pragma endregion

// DrawAndSaveMaps function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveMaps function (G3) */
void AMaps::DrawAndSaveMaps(const std::string &SampleName, TCanvas *h1DCanvas, const std::string &AcceptanceMapsDirectory) {
    std::cout << "\n\nProcessing maps...\n";

    std::cout << "\n\nGenerating maps directories...\n";
    system(("mkdir -p " + AcceptanceMapsDirectory + SampleName).c_str());

    std::cout << "\n\nCalculating efficiencies...\n";

    CalcAcceptanceEfficiency();

    std::cout << "\n\nGenerating filtered maps...\n";
    GenerateFilteredRecoMaps(Charged_particle_min_Ratio, Neutral_particle_min_Ratio);

    std::cout << "\n\nGenerating map matrices...\n";
    GenerateMapMatrices(Charged_particle_min_Ratio, Neutral_particle_min_Ratio);
    // GenerateCPartAMaps(Charged_particle_min_Ratio);
    // GenerateNPartAMaps(Neutral_particle_min_Ratio);
    // GenerateNucleonAMap();

    std::cout << "\n\nSaving maps...\n";
    SaveMaps(SampleName, AcceptanceMapsDirectory);

    if (basic_tools::FindSubstring(SName, "Uniform_1e_sample_")) {
        /* Acceptance maps BC */
        reco_theta_e_VS_phi_e_BC.hDrawAndSave(SName, h1DCanvas, AcceptanceMapsBC, AMapsBC_HistoList, true);

        /* Electron maps */
        for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
            truth_theta_e_VS_phi_e_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, TLAMaps, AMap_TL_HistoList, true);
            reco_theta_e_VS_phi_e_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, RecoAMaps, AMap_Reco_HistoList, true);
            acceptance_eff_e_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, AMapsRatio, AMap_Ratio_HistoList, true);
            filtered_reco_theta_e_VS_phi_e_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, Charged_particle_Sep_AMaps, cPart_Sep_AMaps_HistoList, true);
        }

        /* Finalized acceptance maps */
        filtered_reco_theta_e_VS_phi_e.hDrawAndSave(SName, h1DCanvas, AcceptanceMaps, AMaps_HistoList, true);

        /* Saving maps in PDFs */
        DrawAndSaveMapsPDFs(truth_e_BySlice, truth_theta_e_VS_phi_e_BySlice[0].GetHistogram2DSaveNamePath() + "truth_theta_e_VS_phi_e_BySlice.pdf");
        DrawAndSaveMapsPDFs(reco_e_BySlice, reco_theta_e_VS_phi_e_BySlice[0].GetHistogram2DSaveNamePath() + "reco_theta_e_VS_phi_e_BySlice.pdf");
        DrawAndSaveMapsPDFs(acc_eff_e_BySlice, acceptance_eff_e_BySlice[0].GetHistogram2DSaveNamePath() + "acceptance_eff_e_BySlice.pdf");
        DrawAndSaveMapsPDFs(filtered_reco_e_BySlice, filtered_reco_theta_e_VS_phi_e_BySlice[0].GetHistogram2DSaveNamePath() + "filtered_reco_theta_e_VS_phi_e_BySlice.pdf");
    }

    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
        /* Acceptance maps BC */
        reco_theta_p_VS_phi_p_BC.hDrawAndSave(SName, h1DCanvas, AcceptanceMapsBC, AMapsBC_HistoList, true);

        /* Proton maps */
        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
            truth_theta_p_VS_phi_p_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, TLAMaps, AMap_TL_HistoList, true);
            reco_theta_p_VS_phi_p_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, RecoAMaps, AMap_Reco_HistoList, true);
            acceptance_eff_p_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, AMapsRatio, AMap_Ratio_HistoList, true);
            filtered_reco_theta_p_VS_phi_p_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, Charged_particle_Sep_AMaps, cPart_Sep_AMaps_HistoList, true);
        }

        /* Finalized acceptance maps */
        filtered_reco_theta_p_VS_phi_p.hDrawAndSave(SName, h1DCanvas, AcceptanceMaps, AMaps_HistoList, true);

        /* Saving maps in PDFs */
        DrawAndSaveMapsPDFs(truth_p_BySlice, truth_theta_p_VS_phi_p_BySlice[0].GetHistogram2DSaveNamePath() + "truth_theta_p_VS_phi_p_BySlice.pdf");
        DrawAndSaveMapsPDFs(reco_p_BySlice, reco_theta_p_VS_phi_p_BySlice[0].GetHistogram2DSaveNamePath() + "reco_theta_p_VS_phi_p_BySlice.pdf");
        DrawAndSaveMapsPDFs(acc_eff_p_BySlice, acceptance_eff_p_BySlice[0].GetHistogram2DSaveNamePath() + "acceptance_eff_p_BySlice.pdf");
        DrawAndSaveMapsPDFs(filtered_reco_p_BySlice, filtered_reco_theta_p_VS_phi_p_BySlice[0].GetHistogram2DSaveNamePath() + "filtered_reco_theta_p_VS_phi_p_BySlice.pdf");
    }

    if (basic_tools::FindSubstring(SName, "Uniform_ep_sample_")) {
        /* Acceptance maps BC */
        reco_theta_n_VS_phi_n_BC.hDrawAndSave(SName, h1DCanvas, AcceptanceMapsBC, AMapsBC_HistoList, true);

        /* Neutron maps (by momentum slice) */
        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
            truth_theta_n_VS_phi_n_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, TLAMaps, AMap_TL_HistoList, true);
            reco_theta_n_VS_phi_n_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, RecoAMaps, AMap_Reco_HistoList, true);
            acceptance_eff_n_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, AMapsRatio, AMap_Ratio_HistoList, true);
            filtered_reco_theta_n_VS_phi_n_BySlice.at(i).hDrawAndSave(SName, h1DCanvas, Charged_particle_Sep_AMaps, cPart_Sep_AMaps_HistoList, true);
        }

        /* Neutron maps */
        truth_theta_n_VS_phi_n.hDrawAndSave(SName, h1DCanvas, TLAMaps, AMap_TL_HistoList, true);
        reco_theta_n_VS_phi_n.hDrawAndSave(SName, h1DCanvas, RecoAMaps, AMap_Reco_HistoList, true);
        acceptance_eff_n.hDrawAndSave(SName, h1DCanvas, AMapsRatio, AMap_Ratio_HistoList, true);

        /* Finalized acceptance maps */
        filtered_reco_theta_n_VS_phi_n.hDrawAndSave(SName, h1DCanvas, AcceptanceMaps, AMaps_HistoList, true);

        /* Saving maps in PDFs */
        DrawAndSaveMapsPDFs(truth_n_BySlice, truth_theta_n_VS_phi_n_BySlice[0].GetHistogram2DSaveNamePath() + "truth_theta_n_VS_phi_n_BySlice.pdf");
        DrawAndSaveMapsPDFs(reco_n_BySlice, reco_theta_n_VS_phi_n_BySlice[0].GetHistogram2DSaveNamePath() + "reco_theta_n_VS_phi_n_BySlice.pdf");
        DrawAndSaveMapsPDFs(acc_eff_n_BySlice, acceptance_eff_n_BySlice[0].GetHistogram2DSaveNamePath() + "acceptance_eff_n_BySlice.pdf");
        DrawAndSaveMapsPDFs(filtered_reco_n_BySlice, filtered_reco_theta_n_VS_phi_n_BySlice[0].GetHistogram2DSaveNamePath() + "filtered_reco_theta_n_VS_phi_n_BySlice.pdf");
    }

#pragma region /* Save TL Acceptance maps to plots directory */
    std::cout << "\n\nSaving acceptance maps to plots directory...";
    /* Acceptance maps BC */
    TFile *AMapsBC_plots_path_fout = new TFile((AMapSavePath + "/" + AMapsBC_prefix + SampleName + ".root").c_str(), "recreate");
    AMapsBC_plots_path_fout->cd();
    AcceptanceMapsBC->Write();
    AMapsBC_plots_path_fout->Write();
    AMapsBC_plots_path_fout->Close();

    /* TL Acceptance maps */
    TFile *TLAMaps_plots_path_fout = new TFile((AMapSavePath + "/" + AMap_TL_prefix + SampleName + ".root").c_str(), "recreate");
    TLAMaps_plots_path_fout->cd();
    TLAMaps->Write();
    TLAMaps_plots_path_fout->Write();
    TLAMaps_plots_path_fout->Close();

    /* Reco Acceptance maps */
    TFile *RecoAMaps_plots_path_fout = new TFile((AMapSavePath + "/" + AMap_Reco_prefix + SampleName + ".root").c_str(), "recreate");
    RecoAMaps_plots_path_fout->cd();
    RecoAMaps->Write();
    RecoAMaps_plots_path_fout->Write();
    RecoAMaps_plots_path_fout->Close();

    /* Ratio Acceptance maps */
    TFile *RatioHitMaps_plots_path_fout = new TFile((AMapSavePath + "/" + AMap_Ratio_prefix + SampleName + ".root").c_str(), "recreate");
    RatioHitMaps_plots_path_fout->cd();
    AMapsRatio->Write();
    RatioHitMaps_plots_path_fout->Write();
    RatioHitMaps_plots_path_fout->Close();

    /* Charged particle separate AMaps */
    TFile *cPartAMaps_plots_path_fout = new TFile((AMapSavePath + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    cPartAMaps_plots_path_fout->cd();
    Charged_particle_Sep_AMaps->Write();
    cPartAMaps_plots_path_fout->Write();
    cPartAMaps_plots_path_fout->Close();

    /* Acceptance maps */
    TFile *AMaps_plots_path_fout = new TFile((AMapSavePath + "/" + AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    AMaps_plots_path_fout->cd();
    AcceptanceMaps->Write();
    AMaps_plots_path_fout->Write();
    AMaps_plots_path_fout->Close();
#pragma endregion

    std::cout << "done!\n";

#pragma region /* Save TL Acceptance maps to reference Acceptance maps directory */
    std::cout << "\n\nSaving acceptance maps to reference Acceptance maps directory...";

    /* Acceptance maps BC */
    TFile *AMapsBC_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMapsBC_prefix + SampleName + ".root").c_str(), "recreate");
    AMapsBC_ref_AMaps_fout->cd();
    AcceptanceMapsBC->Write();
    AMapsBC_ref_AMaps_fout->Write();
    AMapsBC_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMapsBC_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* TL Acceptance maps */
    TFile *TLAMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMap_TL_prefix + SampleName + ".root").c_str(), "recreate");
    TLAMaps_ref_AMaps_fout->cd();
    TLAMaps->Write();
    TLAMaps_ref_AMaps_fout->Write();
    TLAMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMap_TL_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* Reco Acceptance maps */
    TFile *RecoAMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMap_Reco_prefix + SampleName + ".root").c_str(), "recreate");
    RecoAMaps_ref_AMaps_fout->cd();
    RecoAMaps->Write();
    RecoAMaps_ref_AMaps_fout->Write();
    RecoAMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMap_Reco_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* Ratio Acceptance maps */
    TFile *RatioHitMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMap_Ratio_prefix + SampleName + ".root").c_str(), "recreate");
    RatioHitMaps_ref_AMaps_fout->cd();
    AMapsRatio->Write();
    RatioHitMaps_ref_AMaps_fout->Write();
    RatioHitMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMap_Ratio_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* Charged particle separate AMaps */
    TFile *cPartAMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    cPartAMaps_ref_AMaps_fout->cd();
    Charged_particle_Sep_AMaps->Write();
    cPartAMaps_ref_AMaps_fout->Write();
    cPartAMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());

    /* Acceptance maps */
    TFile *AMaps_ref_AMaps_fout = new TFile((AcceptanceMapsDirectory + SampleName + "/" + AMaps_prefix + SampleName + ".root").c_str(), "recreate");
    AMaps_ref_AMaps_fout->cd();
    AcceptanceMaps->Write();
    AMaps_ref_AMaps_fout->Write();
    AMaps_ref_AMaps_fout->Close();
    system(("cp " + AMapSavePath + "/" + AMaps_prefix + SampleName + ".root" + " " + AMapCopySavePath).c_str());
#pragma endregion

    std::cout << "done!\n\n\n";
}
#pragma endregion

// HistCounter function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* HistCounter function */
int AMaps::HistCounter(const char *fname) {
    bool PrintHistInfo = false;

    TKey *key;
    TFile *f = TFile::Open(fname, "READ");

    Int_t total = 0;
    TIter next((TList *)f->GetListOfKeys());

    while ((key = (TKey *)next())) {
        TClass *cl = gROOT->GetClass(key->GetClassName());

        if (cl->InheritsFrom("TH1")) {
            // the following line is not needed if you only want
            // to count the histograms
            TH1 *h = (TH1 *)key->ReadObj();

            if (PrintHistInfo) { std::cout << "Histo found: " << h->GetName() << " - " << h->GetTitle() << endl; }

            total++;
        }
    }

    if (PrintHistInfo) { std::cout << "\n\nFound " << total << " Histograms\n" << endl; }

    return total;
}
#pragma endregion

// SetHistBinsFromHistTitle function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetHistBinsFromHistTitle function */
void AMaps::SetHistBinsFromHistTitle(TH2D *Histogram2D) {
    bool PrintOut = false;

    std::string Title = Histogram2D->GetTitle();
    std::string NumOfXBinsStr = Title.substr((Title.find_last_of('(')) + 1, 3);
    std::string NumOfYBinsStr = Title.substr(Title.find_last_of('x') + 1, 3);

    HistNucSliceNumOfXBins = stoi(NumOfXBinsStr);
    HistNucSliceNumOfYBins = stoi(NumOfYBinsStr);

    if (PrintOut) {
        std::cout << "\n\nTitle = " << Title << "\n\n";
        std::cout << "NumOfXBinsStr = " << NumOfXBinsStr << "\n";
        std::cout << "NumOfYBinsStr = " << NumOfYBinsStr << "\n\n";

        std::cout << "HistNucSliceNumOfXBins = " << HistNucSliceNumOfXBins << "\n";
        std::cout << "HistNucSliceNumOfYBins = " << HistNucSliceNumOfYBins << "\n\n";
    }
}
#pragma endregion

// SetSlicesFromHistTitle function --------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetSlicesFromHistTitle function */
void AMaps::SetSlicesFromHistTitle(TH2D *Histogram2D, const std::string &Particle) {
    bool PrintOut = false;

    std::string Title = Histogram2D->GetTitle();
    std::string SliceLowerLimStr = Title.substr((Title.find_first_of('#')) - 4, 4);
    std::string SliceUpperLimStr = Title.substr(Title.find_last_of('#') + 4, 4);

    double SliceLowerLim = stod(SliceLowerLimStr);
    double SliceUpperLim = stod(SliceUpperLimStr);

    vector<double> MomBinsLimitsTemp = {SliceLowerLim, SliceUpperLim};

    if (isElectron(Particle)) {
        ElectronMomSliceLimits.push_back(MomBinsLimitsTemp);
    } else if (isProton(Particle) || isNeutron(Particle)) {
        NucleonMomSliceLimits.push_back(MomBinsLimitsTemp);
    }

    if (PrintOut) {
        std::cout << "\n\nTitle = " << Title << "\n\n";
        std::cout << "SliceLowerLimStr = " << SliceLowerLimStr << "\n";
        std::cout << "SliceUpperLimStr = " << SliceUpperLimStr << "\n\n";

        std::cout << "SliceLowerLim = " << SliceLowerLim << "\n";
        std::cout << "SliceUpperLim = " << SliceUpperLim << "\n\n";
    }
}
#pragma endregion

#pragma region /* SetSlicesFromHistTitle function (original) */
void AMaps::SetSlicesFromHistTitle(TH2D *Histogram2D, vector<vector<double>> MomBinsLimits) {
    bool PrintOut = false;

    std::string Title = Histogram2D->GetTitle();
    std::string SliceLowerLimStr = Title.substr((Title.find_first_of('#')) - 4, 4);
    std::string SliceUpperLimStr = Title.substr(Title.find_last_of('#') + 4, 4);

    double SliceLowerLim = stod(SliceLowerLimStr);
    double SliceUpperLim = stod(SliceUpperLimStr);

    vector<double> MomBinsLimitsTemp = {SliceLowerLim, SliceUpperLim};
    NucleonMomSliceLimits.push_back(MomBinsLimitsTemp);

    if (PrintOut) {
        std::cout << "\n\nTitle = " << Title << "\n\n";
        std::cout << "SliceLowerLimStr = " << SliceLowerLimStr << "\n";
        std::cout << "SliceUpperLimStr = " << SliceUpperLimStr << "\n\n";

        std::cout << "SliceLowerLim = " << SliceLowerLim << "\n";
        std::cout << "SliceUpperLim = " << SliceUpperLim << "\n\n";

        std::cout << "MomBinsLimitsTemp.size() = " << MomBinsLimitsTemp.size() << "\n\n";
    }
}
#pragma endregion

// ReadHitMaps function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadHitMaps function */
void AMaps::ReadHitMaps(const std::string &AcceptanceMapsDirectory, const std::string &SampleName) {
    bool PrintKeys = false;

#pragma region /* Load AMapsBC */
    std::string AMapsBC_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMapsBC_prefix + SampleName + ".root";
    TFile *AMapsBC_RootFile = new TFile(AMapsBC_RootFile_FileName.c_str());
    if (!AMapsBC_RootFile) { std::cout << "\n\nAMaps::ReadHitMaps: could not load AMapsBC root file! Exiting...\n", exit(0); }

    for (TObject *keyAsObj : *AMapsBC_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;

        if (basic_tools::FindSubstring(TempHist->GetTitle(), "Electron") || basic_tools::FindSubstring(TempHist->GetTitle(), "electron")) {
            reco_theta_e_VS_phi_e_BC.SetHistogram2D(TempHist);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "Proton") || basic_tools::FindSubstring(TempHist->GetTitle(), "proton")) {
            reco_theta_p_VS_phi_p_BC.SetHistogram2D(TempHist);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "Neutron") || basic_tools::FindSubstring(TempHist->GetTitle(), "neutron")) {
            reco_theta_n_VS_phi_n_BC.SetHistogram2D(TempHist);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "Nucleon") || basic_tools::FindSubstring(TempHist->GetTitle(), "nucleon")) {
            reco_theta_nuc_VS_phi_nuc_BC.SetHistogram2D(TempHist);
        }
    }
#pragma endregion

#pragma region /* Load Hit_Maps_TL */
    std::string Hit_Maps_TL_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_TL_prefix + SampleName + ".root";
    TFile *Hit_Maps_TL_RootFile = new TFile(Hit_Maps_TL_RootFile_FileName.c_str());
    if (!Hit_Maps_TL_RootFile) { std::cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    int counter = 0;

    for (TObject *keyAsObj : *Hit_Maps_TL_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (counter == 0) { SetHistBinsFromHistTitle(TempHist); }

        if (basic_tools::FindSubstring(TempHist->GetTitle(), "{e}")) {
            truth_theta_e_VS_phi_e_BySlice.push_back(Temp2DHist);
            SetSlicesFromHistTitle(TempHist, NucleonMomSliceLimits);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "{p}")) {
            truth_theta_p_VS_phi_p_BySlice.push_back(Temp2DHist);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "{n}")) {
            truth_theta_n_VS_phi_n.SetHistogram2D(TempHist);
        }

        ++counter;
    }
#pragma endregion

#pragma region /* Load Hit_Maps_Reco */
    std::string Hit_Maps_Reco_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_Reco_prefix + SampleName + ".root";
    TFile *Hit_Maps_Reco_RootFile = new TFile(Hit_Maps_Reco_RootFile_FileName.c_str());
    if (!Hit_Maps_Reco_RootFile) { std::cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_Reco root file! Exiting...\n", exit(0); }

    for (TObject *keyAsObj : *Hit_Maps_Reco_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (basic_tools::FindSubstring(TempHist->GetTitle(), "{e}")) {
            reco_theta_e_VS_phi_e_BySlice.push_back(Temp2DHist);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "{p}")) {
            reco_theta_p_VS_phi_p_BySlice.push_back(Temp2DHist);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "{n}")) {
            reco_theta_n_VS_phi_n.SetHistogram2D(TempHist);
        }
    }
#pragma endregion

#pragma region /* Load Hit_Maps_Ratio */
    std::string Hit_Maps_Ratio_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_Ratio_prefix + SampleName + ".root";
    TFile *Hit_Maps_Ratio_RootFile = new TFile(Hit_Maps_Ratio_RootFile_FileName.c_str());
    if (!Hit_Maps_Ratio_RootFile) { std::cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_Ratio root file! Exiting...\n", exit(0); }

    for (TObject *keyAsObj : *Hit_Maps_Ratio_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (basic_tools::FindSubstring(TempHist->GetTitle(), "{e}")) {
            acceptance_eff_e_BySlice.push_back(Temp2DHist);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "{p}")) {
            acceptance_eff_p_BySlice.push_back(Temp2DHist);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "{n}")) {
            acceptance_eff_n.SetHistogram2D(TempHist);
        }
    }
#pragma endregion

#pragma region /* Load cPart_Sep_AMaps */
    std::string cPart_Sep_AMaps_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root";
    TFile *cPart_Sep_AMaps_RootFile = new TFile(cPart_Sep_AMaps_RootFile_FileName.c_str());
    if (!cPart_Sep_AMaps_RootFile) { std::cout << "\n\nAMaps::ReadHitMaps: could not load cPart_Sep_AMaps root file! Exiting...\n", exit(0); }

    for (TObject *keyAsObj : *cPart_Sep_AMaps_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (basic_tools::FindSubstring(TempHist->GetTitle(), "Electron") || basic_tools::FindSubstring(TempHist->GetTitle(), "electron")) {
            filtered_reco_theta_e_VS_phi_e_BySlice.push_back(Temp2DHist);
        } else if (basic_tools::FindSubstring(TempHist->GetTitle(), "Proton") || basic_tools::FindSubstring(TempHist->GetTitle(), "proton")) {
            filtered_reco_theta_p_VS_phi_p_BySlice.push_back(Temp2DHist);
        }
    }
#pragma endregion

#pragma region /* Load AMaps */
    std::string AMaps_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMaps_prefix + SampleName + ".root";
    TFile *AMaps_RootFile = new TFile(AMaps_RootFile_FileName.c_str());
    if (!AMaps_RootFile) { std::cout << "\n\nAMaps::ReadHitMaps: could not load AMaps root file! Exiting...\n", exit(0); }

    LoadedElectronAMaps0 = (TH2D *)AMaps_RootFile->Get("Electron_AMap");
    if (!LoadedElectronAMaps0) { std::cout << "\n\nAMaps::ReadHitMaps: could not load Electron_AMap from root file! Exiting...\n", exit(0); }

    LoadedProtonAMap = (TH2D *)AMaps_RootFile->Get("Proton_AMap");
    if (!LoadedProtonAMap) { std::cout << "\n\nAMaps::ReadHitMaps: could not load Proton_AMap from root file! Exiting...\n", exit(0); }

    LoadedNeutronAMap = (TH2D *)AMaps_RootFile->Get("Neutron_AMap");
    if (!LoadedNeutronAMap) { std::cout << "\n\nAMaps::ReadHitMaps: could not load Neutron_AMap from root file! Exiting...\n", exit(0); }

    LoadedNucleonAMap = (TH2D *)AMaps_RootFile->Get("Nucleon_AMap");
    if (!LoadedNucleonAMap) { std::cout << "\n\nAMaps::ReadHitMaps: could not load Nucleon_AMap from root file! Exiting...\n", exit(0); }
#pragma endregion

    std::cout << "\n\nAcceptance maps loaded!\n\n";
}
#pragma endregion

// ReadAMapLimits function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadAMapLimits function */
void AMaps::ReadAMapLimits(const char *filename, vector<vector<double>> &Loaded_particle_limits) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        std::string tp;

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            std::string parameter, parameter2;
            ss >> parameter;  // get cut identifier

            if (basic_tools::FindSubstring(parameter, "_slice_")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);

                std::string LineEntry;
                vector<double> particle_limits;

                while (getline(ss2, LineEntry, ':')) { particle_limits.push_back(stod(LineEntry)); }

                Loaded_particle_limits.push_back(particle_limits);
            }
        }
    } else {
        std::cout << "\n\nAMaps::ReadAMap: file not found! Target file was set to:\n" << filename << "\nExiting...\n\n", exit(0);
    }
}
#pragma endregion

// ReadAMapSlices function (AMaps) --------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadAMapSlices function (AMaps) */
void AMaps::ReadAMapSlices(const std::string &SampleName, const std::string &AcceptanceMapsDirectory, const std::string &Particle, const vector<vector<double>> &Loaded_particle_limits,
                           vector<vector<vector<int>>> &Loaded_Particle_AMap_Slices) {
    std::string ParticleShort;

    if (isElectron(Particle)) {
        ParticleShort = "e";
    } else if (isProton(Particle)) {
        ParticleShort = "p";
    } else if (isNeutron(Particle)) {
        ParticleShort = "n";
    } else {
        ParticleShort = "nuc";
    }

    for (int Slice = 0; Slice < Loaded_particle_limits.size(); Slice++) {
        vector<vector<int>> Loaded_Particle_AMap_TempSlice;

        std::string TempFileName = ParticleShort + "_AMap_by_slice/" + ParticleShort + "_AMap_file_from_" + basic_tools::ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(0), 2) +
                                   "_to_" + basic_tools::ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(1), 2) + ".par";

        std::cout << "\n\nReading " << Particle << " map: " << TempFileName << "\n";

        ReadAMap((AcceptanceMapsDirectory + SampleName + "/" + TempFileName).c_str(), Loaded_Particle_AMap_TempSlice);

        Loaded_Particle_AMap_Slices.push_back(Loaded_Particle_AMap_TempSlice);
    }
}
#pragma endregion

// ReadWMapSlices function (WMaps) --------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadWMapSlices function (WMaps) */
void AMaps::ReadWMapSlices(const std::string &SampleName, const std::string &AcceptanceMapsDirectory, const std::string &Particle, const vector<vector<double>> &Loaded_particle_limits,
                           vector<vector<vector<double>>> &Loaded_Particle_WMap_Slices) {
    std::string ParticleShort;

    if (isElectron(Particle)) {
        ParticleShort = "e";
    } else if (isProton(Particle)) {
        ParticleShort = "p";
    } else if (isNeutron(Particle)) {
        ParticleShort = "n";
    } else {
        ParticleShort = "nuc";
    }

    for (int Slice = 0; Slice < Loaded_particle_limits.size(); Slice++) {
        vector<vector<double>> Loaded_Particle_WMap_TempSlice;

        std::string TempFileName = ParticleShort + "_WMap_by_slice/" + ParticleShort + "_WMap_file_from_" + basic_tools::ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(0), 2) +
                                   "_to_" + basic_tools::ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(1), 2) + ".par";

        ReadWMap((AcceptanceMapsDirectory + SampleName + "/" + TempFileName).c_str(), Loaded_Particle_WMap_TempSlice);

        Loaded_Particle_WMap_Slices.push_back(Loaded_Particle_WMap_TempSlice);
    }
}
#pragma endregion

// ReadAMap function (AMaps) --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadAMap function (AMaps) */
/* A function that reads AMaps */
void AMaps::ReadAMap(const char *filename, vector<vector<int>> &Loaded_particle_AMap) {
    bool PrintOut = false;

    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        std::string tp;
        int lineNumber = 0;  // Track line number

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            lineNumber++;  // Increment line number for each line read
            stringstream ss(tp);
            std::string parameter, parameter2;
            ss >> parameter;  // get cut identifier

            if (basic_tools::FindSubstring(parameter, "Line")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);

                std::string LineEntry;
                vector<int> col;

                if (PrintOut) {
                    std::cout << "\n\nfilename = " << filename << "\n";
                    std::cout << "parameter = " << parameter << "\n";
                    std::cout << "parameter2 = " << parameter2 << "\n";
                    std::cout << "LineEntry = " << LineEntry << "\n\n";
                }

                while (getline(ss2, LineEntry, ':')) {
                    if (LineEntry.empty()) {
                        cerr << "AMaps::ReadAMap: Error! Empty entry at line " << lineNumber << " in file:\n"
                             << filename << ":\n"
                             << "   -> " << tp << "\nAborting...",
                            exit(0);  // Abort if there are empty entries from "::"
                    }

                    try {
                        col.push_back(stoi(LineEntry));  // Convert string to int safely
                    } catch (const std::invalid_argument &e) { cerr << "Invalid integer found in file " << filename << ": " << LineEntry << "\n"; } catch (const std::out_of_range &e) {
                        cerr << "Integer out of range in file " << filename << ": " << LineEntry << "\n";
                    }
                }

                Loaded_particle_AMap.push_back(col);
            }
        }
    }
    // else {
    //     std::cout << "\n\nAMaps::ReadAMap: file:\n" << filename << "\nwas not found! Exiting...\n\n", exit(0);
    // }

    std::cout << "\033[35m\n\nAMaps::ReadAMap:\033[36m Warning!\033[0m file:\n" << filename << "\nwas not found!\n\n";
}
#pragma endregion

// ReadWMap function (WMaps) --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadWMap function (WMaps) */
/* A function that reads WMaps */
void AMaps::ReadWMap(const char *filename, vector<vector<double>> &Loaded_particle_WMap) {
    bool PrintOut = false;

    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        std::string tp;
        int lineNumber = 0;  // Track line number

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            lineNumber++;  // Increment line number for each line read
            stringstream ss(tp);
            std::string parameter, parameter2;
            ss >> parameter;  // get cut identifier

            if (basic_tools::FindSubstring(parameter, "Weight")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);

                std::string LineEntry;
                vector<double> col;

                if (PrintOut) {
                    std::cout << "\n\nfilename = " << filename << "\n";
                    std::cout << "parameter = " << parameter << "\n";
                    std::cout << "parameter2 = " << parameter2 << "\n";
                    std::cout << "LineEntry = " << LineEntry << "\n\n";
                }

                while (getline(ss2, LineEntry, ':')) {
                    if (LineEntry.empty()) {
                        cerr << "AMaps::ReadWMap: Error! Empty entry at line " << lineNumber << " in file:\n"
                             << filename << ":\n"
                             << "   -> " << tp << "\nAborting...",
                            exit(0);  // Abort if there are empty entries from "::"
                    }

                    try {
                        col.push_back(stoi(LineEntry));  // Convert string to int safely
                    } catch (const std::invalid_argument &e) { cerr << "Invalid double found in file " << filename << ": " << LineEntry << "\n"; } catch (const std::out_of_range &e) {
                        cerr << "Double out of range in file " << filename << ": " << LineEntry << "\n";
                    }
                }

                Loaded_particle_WMap.push_back(col);
            }
        }
    }
    // else {
    //     std::cout << "\n\nWMaps::ReadWMap: file:\n" << filename << "\nwas not found! Exiting...\n\n", exit(0);
    // }

    std::cout << "\033[35m\n\nAMaps::ReadWMap:\033[36m Warning!\033[0m file:\n" << filename << "\nwas not found!\n\n";
}
// void AMaps::ReadWMap(const char *filename, vector<vector<double>> &Loaded_particle_WMaps) {
//     ifstream infile;
//     infile.open(filename);

//     if (infile.is_open()) {
//         std::string tp;

//         // getline(infile, tp) = read data from file object and put it into string.
//         while (getline(infile, tp)) {
//             stringstream ss(tp);
//             std::string parameter, parameter2;
//             ss >> parameter;  // get cut identifier

//             if (basic_tools::FindSubstring(parameter, "Weight")) {
//                 // get cut values
//                 ss >> parameter2;
//                 stringstream ss2(parameter2);

//                 std::string LineEntry;
//                 vector<double> col;

//                 while (getline(ss2, LineEntry, ':')) { col.push_back(stod(LineEntry)); }

//                 Loaded_particle_WMaps.push_back(col);
//             }
//         }
//     } else {
//         std::cout << "\n\nAMaps::ReadWMap: file:\n" << filename << "\nwas not found! Exiting...\n\n", exit(0);
//     }
// }
#pragma endregion

// MatchAngToHitMap function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* MatchAngToHitMap function (original with neutron FC 'bug') */
bool AMaps::MatchAngToHitMap(const std::string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC) {
    int e_InitialSlice = 0, e_FinalSlice = Loaded_ElectronMomSliceLimits.size();
    int p_InitialSlice = 0, p_FinalSlice = Loaded_NucleonMomSliceLimits.size(), n_InitialSlice = 0, n_FinalSlice = Loaded_NucleonMomSliceLimits.size();

    int e_TestSlice = Slices2Test.at(0), p_TestSlice = Slices2Test.at(1), n_TestSlice = Slices2Test.at(2);

    if (e_single_slice_test) { e_InitialSlice = e_TestSlice - 1, e_FinalSlice = e_TestSlice; }

    if (nuc_single_slice_test) { p_InitialSlice = p_TestSlice - 1, p_FinalSlice = p_TestSlice, n_InitialSlice = n_TestSlice - 1, n_FinalSlice = n_TestSlice; }

    if (isElectron(Particle)) {
        for (int Slice = e_InitialSlice; Slice < e_FinalSlice; Slice++) {
            if (Momentum >= Loaded_ElectronMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_ElectronMomSliceLimits.at(Slice).at(1)) {
                for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
                    double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / HistElectronSliceNumOfYBins;
                    double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                    double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                    if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                        for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                            double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / HistElectronSliceNumOfXBins;
                            double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                            double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                            if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                if (debugging_settings::AMapsDebuggerMode) {
                                    std::cout << "\n\nLoaded_e_AMap_Slices.at(Slice).size() = " << Loaded_e_AMap_Slices.at(Slice).size() << "\n";
                                    std::cout << "Loaded_e_AMap_Slices.at(Slice).at(" << i << ").size() = " << Loaded_e_AMap_Slices.at(Slice).at(i).size() << "\n";
                                    std::cout << "Loaded_ElectronMomSliceLimits.at(Slice).at(0) = " << Loaded_ElectronMomSliceLimits.at(Slice).at(0) << "\n";
                                    std::cout << "Loaded_ElectronMomSliceLimits.at(Slice).at(1) = " << Loaded_ElectronMomSliceLimits.at(Slice).at(1) << "\n\n";
                                }

                                if (Loaded_e_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                    return true;
                                } else {
                                    return false;
                                }
                            }  // end of find right phi if
                        }
                    }  // end of find right theta if
                }
            }  // end of if the right momentum
        }
    } else if (isProton(Particle)) {
        for (int Slice = p_InitialSlice; Slice < p_FinalSlice; Slice++) {
            if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
                for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                    double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
                    double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                    double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                    if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                            double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
                            double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                            double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                            if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                if (NucleonOverlappingFC) {
                                    if (debugging_settings::AMapsDebuggerMode) {
                                        std::cout << "\n\nLoaded_nuc_AMap_Slices.at(Slice).size() = " << Loaded_nuc_AMap_Slices.at(Slice).size() << "\n";
                                        std::cout << "Loaded_nuc_AMap_Slices.at(Slice).at(" << i << ").size() = " << Loaded_nuc_AMap_Slices.at(Slice).at(i).size() << "\n";
                                        std::cout << "Loaded_NucleonMomSliceLimits.at(Slice).at(0) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                        std::cout << "Loaded_NucleonMomSliceLimits.at(Slice).at(1) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";
                                    }

                                    if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                } else {
                                    if (debugging_settings::AMapsDebuggerMode) {
                                        std::cout << "\n\nLoaded_p_AMap_Slices.at(Slice).size() = " << Loaded_p_AMap_Slices.at(Slice).size() << "\n";
                                        std::cout << "Loaded_p_AMap_Slices.at(Slice).at(" << i << ").size() = " << Loaded_p_AMap_Slices.at(Slice).at(i).size() << "\n";
                                        std::cout << "Loaded_NucleonMomSliceLimits.at(Slice).at(0) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                        std::cout << "Loaded_NucleonMomSliceLimits.at(Slice).at(1) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";
                                    }

                                    if (Loaded_p_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                }
                            }  // end of find right phi if
                        }
                    }  // end of find right theta if
                }
            }  // end of if the right momentum
        }
    } else if (isNeutron(Particle)) {
        for (int Slice = n_InitialSlice; Slice < n_FinalSlice; Slice++) {
            if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
                for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                    double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
                    double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                    double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                    if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                            double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
                            double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                            double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                            if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                if (NucleonOverlappingFC) {
                                    if (debugging_settings::AMapsDebuggerMode) {
                                        std::cout << "\n\nLoaded_nuc_AMap_Slices.at(Slice).size() = " << Loaded_nuc_AMap_Slices.at(Slice).size() << "\n";
                                        std::cout << "Loaded_nuc_AMap_Slices.at(Slice).at(" << i << ").size() = " << Loaded_nuc_AMap_Slices.at(Slice).at(i).size() << "\n";
                                        std::cout << "Loaded_NucleonMomSliceLimits.at(Slice).at(0) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                        std::cout << "Loaded_NucleonMomSliceLimits.at(Slice).at(1) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";
                                    }

                                    if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                } else {
                                    if (debugging_settings::AMapsDebuggerMode) {
                                        std::cout << "\n\nLoaded_n_AMap_Slices.at(Slice).size() = " << Loaded_n_AMap_Slices.at(Slice).size() << "\n";
                                        std::cout << "Loaded_n_AMap_Slices.at(Slice).at(" << i << ").size() = " << Loaded_n_AMap_Slices.at(Slice).at(i).size() << "\n";
                                        std::cout << "Loaded_NucleonMomSliceLimits.at(Slice).at(0) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                        std::cout << "Loaded_NucleonMomSliceLimits.at(Slice).at(1) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";
                                    }

                                    if (Loaded_n_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                }
                            }  // end of find right phi if
                        }
                    }  // end of find right theta if
                }
            }  // end of if the right momentum
        }
    }  // end of if Particle

    return false;
}
#pragma endregion

// #pragma region /* MatchAngToHitMap function (original after neutron FC 'bug' fix) */
//  bool AMaps::MatchAngToHitMap(const std::string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC) {
//      int e_InitialSlice = 0, e_FinalSlice = Loaded_ElectronMomSliceLimits.size();
//      int p_InitialSlice = 0, p_FinalSlice = Loaded_NucleonMomSliceLimits.size(), n_InitialSlice = 0, n_FinalSlice = Loaded_NucleonMomSliceLimits.size();
//
//      int e_TestSlice = Slices2Test.at(0), p_TestSlice = Slices2Test.at(1), n_TestSlice = Slices2Test.at(2);
//
//      if (e_single_slice_test) { e_InitialSlice = e_TestSlice - 1, e_FinalSlice = e_TestSlice; }
//
//      if (nuc_single_slice_test) { p_InitialSlice = p_TestSlice - 1, p_FinalSlice = p_TestSlice, n_InitialSlice = n_TestSlice - 1, n_FinalSlice = n_TestSlice; }
//
//      if (isElectron(Particle)) {
//          for (int Slice = e_InitialSlice; Slice < e_FinalSlice; Slice++) {
//              if (Momentum >= Loaded_ElectronMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_ElectronMomSliceLimits.at(Slice).at(1)) {
//                  for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
//                      double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / HistElectronSliceNumOfYBins;
//                      double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                      double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                      if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                          for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
//                              double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / HistElectronSliceNumOfXBins;
//                              double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                              double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                              if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                                  if (Loaded_e_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                      return true;
//                                  } else {
//                                      return false;
//                                  }
//                              } // end of find right phi if
//                          }
//                      } // end of find right theta if
//                  }
//              } // end of if the right momentum
//          }
//      } else if (isProton(Particle)) {
//          for (int Slice = p_InitialSlice; Slice < p_FinalSlice; Slice++) {
//              if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
//                  for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
//                      double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
//                      double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                      double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                      if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                          for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
//                              double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
//                              double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                              double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                              if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                                  if (NucleonOverlappingFC) {
//                                      if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                          return true;
//                                      } else {
//                                          return false;
//                                      }
//                                  } else {
//                                      if (Loaded_p_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                          return true;
//                                      } else {
//                                          return false;
//                                      }
//                                  }
//                              } // end of find right phi if
//                          }
//                      } // end of find right theta if
//                  }
//              } // end of if the right momentum
//          }
//      } else if (isNeutron(Particle)) {
//          if (NucleonOverlappingFC) {
//              for (int Slice = n_InitialSlice; Slice < n_FinalSlice; Slice++) {
//                  if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
//                      for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
//                          double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
//                          double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                          double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                          if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                              for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
//                                  double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
//                                  double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                                  double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                                  if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                                      if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                          return true;
//                                      } else {
//                                          return false;
//                                      }
//                                  } // end of find right phi if
//                              }
//                          } // end of find right theta if
//                      }
//                  } // end of if the right momentum
//              }
//          } else {
//              /* If we're not applying overlapping fiducial cuts (in momRes calculations, for example),
//                 then don't look for momentum based cuts, since neutron momentum can be greater than beamE (bad
//                 neutrons with P_n > beamE will be cut later!) */
//
//              for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
//                  double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
//                  double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                  double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                  if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                      for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
//                          double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
//                          double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                          double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                          if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                              if (Loaded_n_AMap.at(i).at(j) != 0) {
//                                  return true;
//                              } else {
//                                  return false;
//                              }
//                          } // end of find right phi if
//                      }
//                  } // end of find right theta if
//              }
//          }
//      } // end of if Particle
//
//      return false;
//  }
// #pragma endregion

// GetWeight function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetWeight function */
double AMaps::GetWeight(bool apply_kinematical_weights, const std::string &Particle, double Momentum, double Theta, double Phi) {
    bool PrintOut = false;

    if (apply_kinematical_weights) {
        if (isElectron(Particle)) {
            return 1;
        } else if (isProton(Particle)) {
            for (int Slice = 0; Slice < Loaded_NucleonMomSliceLimits.size(); Slice++) {
                if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
                    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                        double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
                        double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                        double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                        if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                                double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
                                double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                                double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                                if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                    double Weight;

                                    if (Loaded_p_WMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        Weight = (1 / (Loaded_p_WMap_Slices.at(Slice).at(i).at(j)));

                                        if (PrintOut) {
                                            std::cout << "MomentumLowerLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                            std::cout << "Momentum = " << Momentum << "\n";
                                            std::cout << "MomentumUpperLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";

                                            std::cout << "ThetaLowerLimTemp = " << ThetaLowerLimTemp << "\n";
                                            std::cout << "Theta = " << Theta << "\n";
                                            std::cout << "ThetaUpperLimTemp = " << ThetaUpperLimTemp << "\n\n";

                                            std::cout << "PhiLowerLimTemp = " << PhiLowerLimTemp << "\n";
                                            std::cout << "Phi = " << Phi << "\n";
                                            std::cout << "PhiUpperLimTemp = " << PhiUpperLimTemp << "\n\n";

                                            std::cout << "Weight = " << Weight << "\n\n";

                                            std::cout << "1/Weight = " << (1 / Weight) << "\n\n\n\n";
                                        }
                                    } else {
                                        Weight = 0;
                                    }

                                    return Weight;
                                }  // end of find right phi if
                            }
                        }  // end of find right theta if
                    }
                }  // end of if the right momentum
            }
        } else if (isNeutron(Particle)) {
            for (int Slice = 0; Slice < Loaded_NucleonMomSliceLimits.size(); Slice++) {
                if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
                    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
                        double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
                        double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
                        double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;

                        if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
                            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                                double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
                                double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
                                double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;

                                if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
                                    double Weight;

                                    if (Loaded_n_WMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        Weight = (1 / (Loaded_n_WMap_Slices.at(Slice).at(i).at(j)));

                                        if (PrintOut) {
                                            std::cout << "MomentumLowerLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                            std::cout << "Momentum = " << Momentum << "\n";
                                            std::cout << "MomentumUpperLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";

                                            std::cout << "ThetaLowerLimTemp = " << ThetaLowerLimTemp << "\n";
                                            std::cout << "Theta = " << Theta << "\n";
                                            std::cout << "ThetaUpperLimTemp = " << ThetaUpperLimTemp << "\n\n";

                                            std::cout << "PhiLowerLimTemp = " << PhiLowerLimTemp << "\n";
                                            std::cout << "Phi = " << Phi << "\n";
                                            std::cout << "PhiUpperLimTemp = " << PhiUpperLimTemp << "\n\n";

                                            std::cout << "Weight = " << Weight << "\n\n";

                                            std::cout << "1/Weight = " << (1 / Weight) << "\n\n\n\n";
                                        }
                                    } else {
                                        Weight = 0;
                                    }

                                    return Weight;
                                }  // end of find right phi if
                            }
                        }  // end of find right theta if
                    }
                }  // end of if the right momentum
            }
        }  // end of if Particle

        return 0;
    } else {
        return 1;
    }
}
#pragma endregion

// IsInFDQuery function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* IsInFDQuery function */
bool AMaps::IsInFDQuery(bool Generate_AMaps, const DSCuts &ThetaFD, const std::string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC) {
    std::string DebuggingBaseString = "AMaps::IsInFDQuery: " + Particle + " with momentum: " + basic_tools::ToStringWithPrecision(Momentum, 2) +
                                      " GeV/c, theta: " + basic_tools::ToStringWithPrecision(Theta, 2) + " rad, phi: " + basic_tools::ToStringWithPrecision(Phi, 2) + " deg";
    debugging::CodeDebugger.PrintStepTester(__FILE__, __LINE__, DebuggerMode, OnlyPrintNamedTesterSteps, (DebuggingBaseString + " - Start"));

    bool inFDQuery, part_inSomeSector;

    if (!Generate_AMaps) {
        part_inSomeSector = MatchAngToHitMap(Particle, Momentum, Theta, Phi, NucleonOverlappingFC);
    } else {
        part_inSomeSector = true;
    }

    inFDQuery = (part_inSomeSector && (Theta >= ThetaFD.GetLowerCutConst()) && (Theta <= ThetaFD.GetUpperCutConst()));

    debugging::CodeDebugger.PrintStepTester(__FILE__, __LINE__, DebuggerMode, OnlyPrintNamedTesterSteps, (DebuggingBaseString + " - End"));

    return inFDQuery;
}
#pragma endregion

#endif  // AMAPS_H
