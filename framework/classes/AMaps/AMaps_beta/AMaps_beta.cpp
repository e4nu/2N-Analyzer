//
// Created by Alon Sportes on 22/06/2023.
//

#ifndef AMAPS_H
#define AMAPS_H

#include "AMaps_beta.h"

// AMaps constructors ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* AMaps generation constructor */
AMaps::AMaps(const std::string &SampleName, const std::string &P_e_bin_profile, const std::string &P_nuc_bin_profile, double beamE, const std::string &AMapsMode, const std::string &SavePath,
             int nOfNucMomBins, int nOfElecMomBins, int hnsNumOfXBins, int hnsNumOfYBins, int hesNumOfXBins, int hesNumOfYBins) {
    AcceptanceMapsBC_OutFile0 = SavePath + "/" + "AcceptanceMapsBC.pdf";
    TLAMaps_OutFile0 = SavePath + "/" + "TLAMaps.pdf";
    RecoAMaps_OutFile0 = SavePath + "/" + "RecoAMaps.pdf";
    AMapsRatio_OutFile0 = SavePath + "/" + "AMapsRatio.pdf";
    Charged_particle_Sep_AMaps_OutFile0 = SavePath + "/" + "Charged_particle_Sep_AMaps.pdf";
    AcceptanceMaps_OutFile0 = SavePath + "/" + "AcceptanceMaps.pdf";

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
    vector<string> directories = {"00b_AMaps_BC_from_class/",   "01a_Electron_TL_Hit_Maps/",   "01b_Electron_Reco_Hit_Maps/", "01c_Electron_RecoToTL_Ratio/", "01d_Electron_Separate_AMaps/",
                                  "02a_Proton_TL_Hit_Maps/",    "02b_Proton_Reco_Hit_Maps/",   "02c_Proton_RecoToTL_Ratio/",  "02d_Proton_Separate_AMaps/",   "03a_Neutron_TL_Hit_Maps/",
                                  "03b_Neutron_Reco_Hit_Maps/", "03c_Neutron_RecoToTL_Ratio/", "03d_Neutron_Separate_AMaps/", "04_Finalized_AMaps/",          "05_Generated_maps/"};

    for (const auto &dir : directories) { system(("mkdir -p " + SavePath + dir).c_str()); }

    std::string SavePathAMapsBC = SavePath + "00b_AMaps_BC_from_class/";
    std::string AMapSavePathTLElectron = SavePath + "01a_Electron_TL_Hit_Maps/";
    std::string AMapSavePathRecoElectron = SavePath + "01b_Electron_Reco_Hit_Maps/";
    std::string AMapSavePathRecoToTLElectron = SavePath + "01c_Electron_RecoToTL_Ratio/";
    std::string AMapSavePathSepAMapsElectron = SavePath + "01d_Electron_Separate_AMaps/";
    std::string AMapSavePathTLProton = SavePath + "02a_Proton_TL_Hit_Maps/";
    std::string AMapSavePathRecoProton = SavePath + "02b_Proton_Reco_Hit_Maps/";
    std::string AMapSavePathRecoToTLProton = SavePath + "02c_Proton_RecoToTL_Ratio/";
    std::string AMapSavePathSepAMapsProton = SavePath + "02d_Proton_Separate_AMaps/";
    std::string AMapSavePathTLNeutron = SavePath + "03a_Neutron_TL_Hit_Maps/";
    std::string AMapSavePathRecoNeutron = SavePath + "03b_Neutron_Reco_Hit_Maps/";
    std::string AMapSavePathRecoToTLNeutron = SavePath + "03c_Neutron_RecoToTL_Ratio/";
    std::string AMapSavePathSepAMapsNeutron = SavePath + "03d_Neutron_Separate_AMaps/";
    std::string AMapSavePathAMap = SavePath + "04_Finalized_AMaps/";
    AMapCopySavePath = SavePath + "05_Generated_maps/";
    #pragma endregion

    #pragma region /* Setting AMapsMode_TitleAddition */
    std::string AMapsMode_TitleAddition = AMaps_Mode.empty() ? "" : AMaps_Mode;
    #pragma endregion

    SetBins(P_nuc_bin_profile, beamE);
    SetElectronBins(P_e_bin_profile, beamE);

    #pragma region /* Reco theta VS phi BC */
    vector<tuple<string, string, string, hPlot2D *>> particlesBC = {{"Electron_AMap_BC", "Electron AMap BC", "01_e_AMap_BC", &reco_theta_e_VS_phi_e_BC},
                                                                    {"Proton_AMap_BC", "Proton AMap BC", "02_p_AMap_BC", &reco_theta_p_VS_phi_p_BC},
                                                                    {"Neutron_AMap_BC", "Neutron AMap BC", "03_n_AMap_BC", &reco_theta_n_VS_phi_n_BC},
                                                                    {"Nucleon_AMap_BC", "Nucleon AMap BC", "04_nuc_AMap_BC", &reco_theta_nuc_VS_phi_nuc_BC}};

    for (auto &[statsTitle, title, saveName, plot] : particlesBC) {
        *plot = hPlot2D(AMapsMode_TitleAddition, "", statsTitle, title, "#phi [#circ]", "#theta [#circ]", SavePathAMapsBC, saveName, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim,
                        hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    }
    #pragma endregion

    #pragma region /* Setting electron histograms */
    for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
        double BinLowerLim = ElectronMomSliceLimits.at(i).at(0), BinUpperLim = ElectronMomSliceLimits.at(i).at(1);
        int BinUpperLimPrecision = (BinUpperLim == beamE) ? 3 : 2;
        std::string BinDensity = " (" + to_string(HistElectronSliceNumOfXBins) + "x" + to_string(HistElectronSliceNumOfYBins) + ")";

        vector<tuple<string, string, string, vector<hPlot2D> &, vector<TH2D *> &>> electronHistograms = {
            {"TL P_{e} bin for ", "TL P_{e} bin for ", "_TL_P_bin_for_P_from_", truth_theta_e_VS_phi_e_BySlice, truth_e_BySlice},
            {"Reco P_{e} bin for ", "Reco P_{e} bin for ", "_Reco_P_bin_for_P_from_", reco_theta_e_VS_phi_e_BySlice, reco_e_BySlice},
            {"Electron Reco/TL ratio for ", "Electron Reco/TL ratio for ", "_e_Ratio_for_P_from_", acceptance_eff_e_BySlice, acc_eff_e_BySlice},
            {"Electron AMap for ", "Electron AMap for ", "_e_SepAMap_for_P_from_", filtered_reco_theta_e_VS_phi_e_BySlice, filtered_reco_e_BySlice}};

        for (auto &[statsTitlePrefix, titlePrefix, saveNamePrefix, histVec, hist2DVec] : electronHistograms) {
            std::string hStatsTitle =
                statsTitlePrefix + ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" + ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
            std::string hTitle =
                titlePrefix + ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{e}#leq" + ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
            std::string hSaveName = to_string(i + 1) + saveNamePrefix + ToStringWithPrecision(BinLowerLim, 2) + "_to_" + ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            hPlot2D hPBin = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitle, hTitle, "#phi_{e} [#circ]", "#theta_{e} [#circ]", AMapSavePathTLElectron, hSaveName, hBinLowerXLim,
                                    hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
            histVec.push_back(hPBin);
            hist2DVec.push_back(hPBin.GetHistogram2D());
        }
    }
    #pragma endregion

    #pragma region /* Setting nucleon histograms */

    #pragma region /* Setting nucleon slice histograms */
    for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
        double BinLowerLim = NucleonMomSliceLimits.at(i).at(0), BinUpperLim = NucleonMomSliceLimits.at(i).at(1);
        int BinUpperLimPrecision = (BinUpperLim == beamE) ? 3 : 2;
        std::string BinDensity = " (" + to_string(HistNucSliceNumOfXBins) + "x" + to_string(HistNucSliceNumOfYBins) + ")";

        vector<tuple<string, string, string, vector<hPlot2D> &, vector<TH2D *> &>> nucleonHistograms = {
            {"TL P_{p} bin for ", "TL P_{p} bin for ", "_TL_P_bin_for_P_from_", truth_theta_p_VS_phi_p_BySlice, truth_p_BySlice},
            {"Reco P_{p} bin for ", "Reco P_{p} bin for ", "_Reco_P_bin_for_P_from_", reco_theta_p_VS_phi_p_BySlice, reco_p_BySlice},
            {"Proton Reco/TL ratio for ", "Proton Reco/TL ratio for ", "_p_Ratio_for_P_from_", acceptance_eff_p_BySlice, acc_eff_p_BySlice},
            {"Proton AMap for ", "Proton AMap for ", "_p_SepAMap_for_P_from_", filtered_reco_theta_p_VS_phi_p_BySlice, filtered_reco_p_BySlice},
            {"TL P_{n} bin for ", "TL P_{n} bin for ", "_TL_P_bin_for_P_from_", truth_theta_n_VS_phi_n_BySlice, truth_n_BySlice},
            {"Reco P_{n} bin for ", "Reco P_{n} bin for ", "_Reco_P_bin_for_P_from_", reco_theta_n_VS_phi_n_BySlice, reco_n_BySlice},
            {"Neutron Reco/TL ratio for ", "Neutron Reco/TL ratio for ", "_n_Ratio_for_P_from_", acceptance_eff_n_BySlice, acc_eff_n_BySlice},
            {"Neutron AMap for ", "Neutron AMap for ", "_n_SepAMap_for_P_from_", filtered_reco_theta_n_VS_phi_n_BySlice, filtered_reco_n_BySlice}};

        for (auto &[statsTitlePrefix, titlePrefix, saveNamePrefix, histVec, hist2DVec] : nucleonHistograms) {
            std::string hStatsTitle =
                statsTitlePrefix + ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" + ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
            std::string hTitle =
                titlePrefix + ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{p}#leq" + ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
            std::string hSaveName = to_string(i + 1) + saveNamePrefix + ToStringWithPrecision(BinLowerLim, 2) + "_to_" + ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            hPlot2D hPBin = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitle, hTitle, "#phi_{p} [#circ]", "#theta_{p} [#circ]", AMapSavePathTLProton, hSaveName, hBinLowerXLim, hBinUpperXLim,
                                    hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
            histVec.push_back(hPBin);
            hist2DVec.push_back(hPBin.GetHistogram2D());
        }
    }
    #pragma endregion

    #pragma region /* Setting neutron Acceptance maps */

    #pragma region /* Neutron TL Acceptance maps */
    truth_theta_n_VS_phi_n = hPlot2D(AMapsMode_TitleAddition, "", "TL Neutron Hit Map", "TL Neutron Hit Map", "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathTLNeutron, "TL_n_AMap",
                                     hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    #pragma endregion

    #pragma region /* Neutron Reco. Acceptance maps */
    reco_theta_n_VS_phi_n = hPlot2D(AMapsMode_TitleAddition, "", "Reco Neutron Hit Map", "Reco Neutron Hit Map", "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathRecoNeutron,
                                    "Reco_n_AMap", hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    #pragma endregion

    #pragma region /* Neutron Reco./TL Ratio */
    acceptance_eff_n = hPlot2D(AMapsMode_TitleAddition, "", "Neutron Reco/TL ratio", "Neutron Reco/TL ratio", "#phi_{n} [#circ]", "#theta_{n} [#circ]", AMapSavePathRecoToTLNeutron,
                               "Neutron_Ratio", hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    #pragma endregion

    #pragma endregion

    #pragma endregion

    #pragma region /* Finalized acceptance maps */
    filtered_reco_theta_e_VS_phi_e =
        hPlot2D(AMapsMode_TitleAddition, "", "Electron_AMap", "Electron AMap for (Reco./TL)#geq" + ToStringWithPrecision(Charged_particle_min_Ratio, 2), "#phi_{e} [#circ]",
                "#theta_{e} [#circ]", AMapSavePathAMap, "01_e_AMap", hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);

    filtered_reco_theta_p_VS_phi_p =
        hPlot2D(AMapsMode_TitleAddition, "", "Proton_AMap", "Proton AMap for (Reco./TL)#geq" + ToStringWithPrecision(Charged_particle_min_Ratio, 2), "#phi_{p} [#circ]", "#theta_{p} [#circ]",
                AMapSavePathAMap, "02_p_AMap", hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    filtered_reco_theta_n_VS_phi_n =
        hPlot2D(AMapsMode_TitleAddition, "", "Neutron_AMap", "Neutron AMap for (Reco./TL)#geq" + ToStringWithPrecision(Neutral_particle_min_Ratio, 2), "#phi_{n} [#circ]",
                "#theta_{n} [#circ]", AMapSavePathAMap, "03_n_AMap", hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    filtered_reco_theta_nuc_VS_phi_nuc = hPlot2D(
        AMapsMode_TitleAddition, "", "Nucleon_AMap",
        "Nucleon AMap for (Reco./TL)_{n}#geq" + ToStringWithPrecision(Neutral_particle_min_Ratio, 2) + " and (Reco./TL)_{c}#geq" + ToStringWithPrecision(Charged_particle_min_Ratio, 2),
        "#phi_{nuc} [#circ]", "#theta_{nuc} [#circ]", AMapSavePathAMap, "04_nuc_AMap", hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins,
        HistNucSliceNumOfYBins);
    #pragma endregion
}
#pragma endregion

#pragma region /* AMaps loading constructor */
AMaps::AMaps(const std::string &AcceptanceMapsDirectory, const std::string &SampleName, const bool &Electron_single_slice_test, const bool &Nucleon_single_slice_test,
             const vector<int> &TestSlices) {
    /* Load slices and their limits */
    ReadAMapLimits((AcceptanceMapsDirectory + SampleName + "/e_AMap_by_slice/e_slice_limits.par").c_str(), Loaded_ElectronMomSliceLimits);
    ReadAMapLimits((AcceptanceMapsDirectory + SampleName + "/p_AMap_by_slice/p_slice_limits.par").c_str(), Loaded_NucleonMomSliceLimits);

    /* Load separate maps */
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, Loaded_e_AMap_Slices);
    ReadWMapSlices(SampleName, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, Loaded_e_WMap_Slices);
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, Loaded_p_AMap_Slices);
    ReadWMapSlices(SampleName, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, Loaded_p_WMap_Slices);
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, Loaded_n_AMap_Slices);
    ReadWMapSlices(SampleName, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, Loaded_n_WMap_Slices);
    ReadAMapSlices(SampleName, AcceptanceMapsDirectory, "Nucleon", Loaded_NucleonMomSliceLimits, Loaded_nuc_AMap_Slices);

    /* Load combined maps */
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/e_AMap_file.par").c_str(), Loaded_e_AMap);
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/p_AMap_file.par").c_str(), Loaded_p_AMap);
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/n_AMap_file.par").c_str(), Loaded_n_AMap);
    ReadAMap((AcceptanceMapsDirectory + SampleName + "/nuc_AMap_file.par").c_str(), Loaded_nuc_AMap);

    HistElectronSliceNumOfXBins = 100;  // 100 by Default
    HistElectronSliceNumOfYBins = 100;  // 100 by Default
    HistNucSliceNumOfXBins = 75;        // 75 by Default
    HistNucSliceNumOfYBins = 75;        // 75 by Default

    e_single_slice_test = Electron_single_slice_test, nuc_single_slice_test = Nucleon_single_slice_test;
    Slices2Test = TestSlices;
}
#pragma endregion

// SetBins functions ----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetBins function */
void AMaps::SetBins(const std::string &P_nuc_bin_profile, double beamE) {
    if (P_nuc_bin_profile == "equi_inverted_P_nuc") {
        double InvertedPLowerLim = 1 / beamE;
        double InvertedPUpper = 1 / Nucleon_Momentum_Slice_Th;
        double Delta = (InvertedPUpper - InvertedPLowerLim) / NumberNucOfMomSlices;

        for (int i = 0; i < NumberNucOfMomSlices; i++) {
            double InvertedBinLower = InvertedPLowerLim + i * Delta;
            double InvertedBinUpper = InvertedBinLower + Delta;
            InvertedNucleonMomSliceLimits.push_back({InvertedBinLower, InvertedBinUpper});
        }

        for (int i = NumberNucOfMomSlices - 1; i >= 0; i--) {
            double BinLower = 1 / InvertedNucleonMomSliceLimits[i][1];
            double BinUpper = 1 / InvertedNucleonMomSliceLimits[i][0];
            NucleonMomSliceLimits.push_back({BinLower, BinUpper});
        }
    } else if (P_nuc_bin_profile == "varying_P_nuc_bins") {
        NucleonMomSliceLimits = CustomNucleonMomSliceLimits_C12x4_simulation_G18_Q204_6GeV;
        NumberNucOfMomSlices = NucleonMomSliceLimits.size();
    } else if (P_nuc_bin_profile == "uniform_P_nuc_bins") {
        double PLowerLim = Nucleon_Momentum_Slice_Th;
        double PUpperLim = beamE;
        double Delta = (PUpperLim - PLowerLim) / NumberNucOfMomSlices;

        for (int i = 0; i < NumberNucOfMomSlices; i++) {
            double BinLower = PLowerLim + i * Delta;
            double BinUpper = BinLower + Delta;
            NucleonMomSliceLimits.push_back({BinLower, BinUpper});
        }
    } else {
        cout << "AMaps::SetBins: no valid P_nuc_bin_profile selected! Choose between:\n";
        cout << "equi_inverted_P_nuc , varying_P_nuc_bins , uniform_P_nuc_bins\n";
        cout << "Exiting...", exit(0);
    }
}
#pragma endregion

#pragma region /* SetElectronBins function */
void AMaps::SetElectronBins(const std::string &P_e_bin_profile, double beamE) {
    if (P_e_bin_profile == "reformat_e_bins") {
        double InvertedPLowerLim = 1 / beamE;
        double InvertedPUpperLim = 1 / Nucleon_Momentum_Slice_Th;
        double delta = InvertedPUpperLim - (1 / 1.57819);
        int iter = 0;

        while (true) {
            double InvBinUpper = (iter == 0) ? InvertedPUpperLim : InvertedElectronMomSliceLimits.back().at(0);
            double InvBinLower = InvBinUpper - delta / ((iter == 0) ? 1 : 12);

            if (InvBinLower < InvertedPLowerLim) break;

            InvertedElectronMomSliceLimits.push_back({InvBinLower, InvBinUpper});
            iter++;
        }

        for (const auto &limits : InvertedElectronMomSliceLimits) { ElectronMomSliceLimits.push_back({1 / limits.at(1), 1 / limits.at(0)}); }
    } else if (P_e_bin_profile == "varying_P_e_bins") {
        ElectronMomSliceLimits =
            (FindSubstring(SName, "C12_simulation_6GeV_T5")) ? CustomElectronMomSliceLimits_C12_simulation_6GeV_T5 : CustomElectronMomSliceLimits_C12x4_simulation_G18_Q204_6GeV;
    } else if (P_e_bin_profile == "uniform_P_e_bins") {
        double Delta = beamE / NumberElecOfMomSlices;
        for (int i = 0; i < NumberElecOfMomSlices; i++) { ElectronMomSliceLimits.push_back({i * Delta, (i + 1) * Delta}); }
    } else if (P_e_bin_profile == "equi_inverted_P_e") {
        double InvertedPLowerLim = 1 / beamE;
        double InvertedPUpper = 1 / Electron_Momentum_Slice_Th;
        double Delta = (InvertedPUpper - InvertedPLowerLim) / NumberElecOfMomSlices;

        for (int i = 0; i < NumberElecOfMomSlices - 1; i++) { InvertedElectronMomSliceLimits.push_back({InvertedPLowerLim + i * Delta, InvertedPLowerLim + (i + 1) * Delta}); }

        for (const auto &limits : InvertedElectronMomSliceLimits) { ElectronMomSliceLimits.push_back({beamE - 1 / limits.at(0), beamE - 1 / limits.at(1)}); }

        ElectronMomSliceLimits.push_back({ElectronMomSliceLimits.back().at(1), beamE});
    } else {
        cout << "AMaps::SetElectronBins: no valid P_e_bin_profile selected! Choose between:\n";
        cout << "reformat_e_bins , varying_P_e_bins , uniform_P_e_bins, equi_inverted_P_e\n";
        cout << "Exiting...", exit(0);
    }
}
#pragma endregion

#pragma region /* SetBins function (old) */
void AMaps::SetBins(double beamE, int nOfMomBins) {
    double PLowerLim = Nucleon_Momentum_Slice_Th;
    double PUpperLim = beamE;
    double Delta = (PUpperLim - PLowerLim) / nOfMomBins;

    for (int i = 0; i < nOfMomBins; i++) {
        double BinLower = PLowerLim + i * Delta;
        double BinUpper = BinLower + Delta;
        NucleonMomSliceLimits.push_back({BinLower, BinUpper});
    }
}
#pragma endregion

// isElectron function --------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isElectron function */
bool AMaps::isElectron(const std::string &SampleType) {
    static const unordered_set<string> validTypes = {"Electron", "electron"};
    return validTypes.find(SampleType) != validTypes.end();
}
#pragma endregion

// isProton function ----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isProton function */
bool AMaps::isProton(const std::string &SampleType) {
    static const unordered_set<string> validTypes = {"Proton", "proton"};
    return validTypes.find(SampleType) != validTypes.end();
}
#pragma endregion

// isNeutron function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isNeutron function */
bool AMaps::isNeutron(const std::string &SampleType) {
    static const unordered_set<string> validTypes = {"Neutron", "neutron"};
    return validTypes.find(SampleType) != validTypes.end();
}
#pragma endregion

// isTL function --------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isTL function */
bool AMaps::isTL(const std::string &SampleType) {
    static const unordered_set<string> validTypes = {"Truth", "truth", "TL", "truth level", "truth-level", "Truth-Level"};
    return validTypes.find(SampleType) != validTypes.end();
}
#pragma endregion

// isReco function --------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* isReco function */
bool AMaps::isReco(const std::string &SampleType) {
    static const unordered_set<string> validTypes = {"reco", "Reco", "Reconstruction"};
    return validTypes.find(SampleType) != validTypes.end();
}
#pragma endregion

// hFillHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* hFillHitMaps function */
void AMaps::hFillHitMaps(const std::string &SampleType, const std::string &particle, double Momentum, double Theta, double Phi, double Weight) {
    bool is_e = isElectron(particle), is_p = isProton(particle), is_n = isNeutron(particle);
    bool is_TL = isTL(SampleType), is_Reco = isReco(SampleType);

    if ((is_e && (is_p || is_n)) || (is_p && is_n) || (!is_e && !is_p && !is_n)) {
        cout << "\n\nAMaps::hFillHitMaps: particle must be either an electron, proton, or neutron! Exiting...\n", exit(0);
    }

    if ((is_TL && is_Reco) || (!is_TL && !is_Reco)) { cout << "\n\nAMaps::hFillHitMaps: SampleType must be either TL or Reco! Exiting...\n", exit(0); }

    auto fillHistograms = [&](auto &histVec, auto &sliceLimits, const std::string &particleType) {
        for (int i = 0; i < sliceLimits.size(); i++) {
            if ((Momentum >= sliceLimits.at(i).at(0)) && (Momentum < sliceLimits.at(i).at(1))) {
                histVec.at(i).hFill(Phi, Theta, Weight);
                if (sliceLimits.at(i).at(0) > sliceLimits.at(i).at(1)) {
                    cout << "\n\nAMaps::hFillHitMaps: " << particleType << " momentum slice limits were set incorrectly! Exiting...\n", exit(0);
                }
                break;
            }
        }
    };

    if (is_TL) {
        if (is_e) {
            fillHistograms(truth_theta_e_VS_phi_e_BySlice, ElectronMomSliceLimits, "Electron");
        } else if (is_p) {
            fillHistograms(truth_theta_p_VS_phi_p_BySlice, NucleonMomSliceLimits, "Proton");
        } else if (is_n) {
            truth_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);
            fillHistograms(truth_theta_n_VS_phi_n_BySlice, NucleonMomSliceLimits, "Neutron");
        }
    } else if (is_Reco) {
        if (is_e) {
            reco_theta_e_VS_phi_e_BC.hFill(Phi, Theta, Weight);
            fillHistograms(reco_theta_e_VS_phi_e_BySlice, ElectronMomSliceLimits, "Electron");
            fillHistograms(acceptance_eff_e_BySlice, ElectronMomSliceLimits, "Electron");
            fillHistograms(filtered_reco_theta_e_VS_phi_e_BySlice, ElectronMomSliceLimits, "Electron");
        } else if (is_p) {
            reco_theta_p_VS_phi_p_BC.hFill(Phi, Theta, Weight);
            fillHistograms(reco_theta_p_VS_phi_p_BySlice, NucleonMomSliceLimits, "Proton");
            fillHistograms(acceptance_eff_p_BySlice, NucleonMomSliceLimits, "Proton");
            fillHistograms(filtered_reco_theta_p_VS_phi_p_BySlice, NucleonMomSliceLimits, "Proton");
        } else if (is_n) {
            reco_theta_n_VS_phi_n_BC.hFill(Phi, Theta, Weight);
            reco_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);
            acceptance_eff_n.hFill(Phi, Theta, Weight);
            filtered_reco_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);
            fillHistograms(reco_theta_n_VS_phi_n_BySlice, NucleonMomSliceLimits, "Neutron");
            fillHistograms(acceptance_eff_n_BySlice, NucleonMomSliceLimits, "Neutron");
            fillHistograms(filtered_reco_theta_n_VS_phi_n_BySlice, NucleonMomSliceLimits, "Neutron");
        }
    }
}
#pragma endregion

// CalcAMapsRatio function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcAMapsRatio function */
void AMaps::CalcAMapsRatio() {
    auto calculateEfficiency = [](bool calcRatio, auto &acceptanceEff, const auto &truthHist, const std::string &particleType) {
        if (calcRatio) {
            cout << "\n\nCalculating " << particleType << " acceptance efficiency...";
            for (size_t i = 0; i < truthHist.size(); ++i) { acceptanceEff.at(i).hDivision(truthHist.at(i).GetHistogram2D()); }
            cout << " done!\n";
        }
    };

    calculateEfficiency(calc_Electron_RecoToTL_Ratio, acceptance_eff_e_BySlice, truth_theta_e_VS_phi_e_BySlice, "electron");
    calculateEfficiency(calc_Proton_RecoToTL_Ratio, acceptance_eff_p_BySlice, truth_theta_p_VS_phi_p_BySlice, "proton");
    calculateEfficiency(calc_Neutron_RecoToTL_Ratio, acceptance_eff_n_BySlice, truth_theta_n_VS_phi_n_BySlice, "neutron");
}

// GenerateSeparateCPartAMaps function ----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateSeparateCPartAMaps function */
void AMaps::GenerateSeparateCPartAMaps(double cP_minR) {
    auto GenerateMapMatrices = [&](const vector<vector<double>> &sliceLimits, const vector<hPlot2D> &acceptanceEffBySlice, vector<vector<vector<int>>> &aMapSlices,
                                   vector<vector<vector<double>>> &wMapSlices, vector<hPlot2D> &filteredRecoThetaVsPhiBySlice, int histNumOfXBins, int histNumOfYBins) {
        for (int bin = 0; bin < sliceLimits.size(); bin++) {
            for (int i = 0; i < (histNumOfXBins + 1); i++) {
                for (int j = 0; j < (histNumOfYBins + 1); j++) {
                    if (acceptanceEffBySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { filteredRecoThetaVsPhiBySlice.at(bin).hFillByBin(i, j, 0); }
                }
            }

            if (AMaps_Mode == "AMaps") { acceptanceEffBySlice.at(bin).ApplyZMaxLim(1.2); }

            vector<vector<int>> aMapSlice(histNumOfYBins, vector<int>(histNumOfXBins, 0));
            vector<vector<double>> wMapSlice(histNumOfYBins, vector<double>(histNumOfXBins, 0));

            for (int i = 0; i < histNumOfYBins; i++) {
                for (int j = 0; j < histNumOfXBins; j++) {
                    if (acceptanceEffBySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR) {
                        aMapSlice[i][j] = 1;
                        wMapSlice[i][j] = acceptanceEffBySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1);
                    }
                }
            }

            aMapSlices.push_back(aMapSlice);
            wMapSlices.push_back(wMapSlice);
        }
    };

    GenerateMapMatrices(ElectronMomSliceLimits, acceptance_eff_e_BySlice, e_AMap_Slices, e_WMap_Slices, filtered_reco_theta_e_VS_phi_e_BySlice, HistElectronSliceNumOfXBins,
                        HistElectronSliceNumOfYBins);
    GenerateMapMatrices(NucleonMomSliceLimits, acceptance_eff_p_BySlice, p_AMap_Slices, p_WMap_Slices, filtered_reco_theta_p_VS_phi_p_BySlice, HistNucSliceNumOfXBins,
                        HistNucSliceNumOfYBins);
}

// GenerateCPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateCPartAMaps function */
void AMaps::GenerateCPartAMaps(double cP_minR) {
    GenerateSeparateCPartAMaps(cP_minR);

    auto fillFinalizedMaps = [&](auto &filteredRecoThetaVsPhi, const auto &filteredRecoThetaVsPhiBySlice, auto &aMap, int histNumOfXBins, int histNumOfYBins) {
        for (const auto &slice : filteredRecoThetaVsPhiBySlice) { filteredRecoThetaVsPhi.hAdd(slice.GetHistogram2D()); }

        for (int i = 0; i < histNumOfYBins; i++) {
            vector<int> aMapCol;
            for (int j = 0; j < histNumOfXBins; j++) { aMapCol.push_back(filteredRecoThetaVsPhi.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= cP_minR ? 1 : 0); }
            aMap.push_back(aMapCol);
        }
    };

    fillFinalizedMaps(filtered_reco_theta_e_VS_phi_e, filtered_reco_theta_e_VS_phi_e_BySlice, e_AMap, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
    fillFinalizedMaps(filtered_reco_theta_p_VS_phi_p, filtered_reco_theta_p_VS_phi_p_BySlice, p_AMap, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
}

// GenerateNPartAMaps function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateNPartAMaps function */
void AMaps::GenerateNPartAMaps(double nP_minR) {
    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if (acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) < nP_minR) { filtered_reco_theta_n_VS_phi_n_BySlice.at(bin).hFillByBin(j + 1, i + 1, 0); }
            }
        }

        if (AMaps_Mode == "AMaps") { acceptance_eff_n_BySlice.at(bin).ApplyZMaxLim(1.2); }

        vector<vector<int>> n_AMap_slice(HistNucSliceNumOfYBins, vector<int>(HistNucSliceNumOfXBins, 0));
        vector<vector<double>> n_WMap_slice(HistNucSliceNumOfYBins, vector<double>(HistNucSliceNumOfXBins, 0.0));

        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if (acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1) >= nP_minR) {
                    n_AMap_slice[i][j] = 1;
                    n_WMap_slice[i][j] = acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1);
                }
            }
        }

        n_AMap_Slices.push_back(n_AMap_slice);
        n_WMap_Slices.push_back(n_WMap_slice);
    }

    vector<vector<int>> n_AMap(HistNucSliceNumOfYBins, vector<int>(HistNucSliceNumOfXBins, 0));
    vector<vector<double>> n_WMap(HistNucSliceNumOfYBins, vector<double>(HistNucSliceNumOfXBins, 0.0));

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (acceptance_eff_n.GetHistogram2D()->GetBinContent(j + 1, i + 1) >= nP_minR) {
                n_AMap[i][j] = 1;
                n_WMap[i][j] = acceptance_eff_n.GetHistogram2D()->GetBinContent(j + 1, i + 1);
            }
        }
    }

    n_AMap_Slices.push_back(n_AMap);
    n_WMap_Slices.push_back(n_WMap);

    for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) { filtered_reco_theta_n_VS_phi_n_BySlice.push_back(filtered_reco_theta_n_VS_phi_n); }
}

// GenerateNucleonAMap function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateNucleonAMap function */
void AMaps::GenerateNucleonAMap() {
    for (int i = 0; i < HistNucSliceNumOfXBins + 1; i++) {
        for (int j = 0; j < HistNucSliceNumOfYBins + 1; j++) {
            if (reco_theta_n_VS_phi_n_BC.GetHistogram2D()->GetBinContent(i, j) != 0 && reco_theta_p_VS_phi_p_BC.GetHistogram2D()->GetBinContent(i, j) != 0) {
                reco_theta_nuc_VS_phi_nuc_BC.hFillByBin(i, j, reco_theta_n_VS_phi_n.GetHistogram2D()->GetBinContent(i, j));
            }

            if (filtered_reco_theta_n_VS_phi_n.GetHistogram2D()->GetBinContent(i, j) != 0 && filtered_reco_theta_p_VS_phi_p.GetHistogram2D()->GetBinContent(i, j) != 0) {
                filtered_reco_theta_nuc_VS_phi_nuc.hFillByBin(i, j, reco_theta_n_VS_phi_n.GetHistogram2D()->GetBinContent(i, j));
            }
        }
    }

    nuc_AMap.resize(HistNucSliceNumOfYBins, vector<int>(HistNucSliceNumOfXBins, 0));
    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (filtered_reco_theta_nuc_VS_phi_nuc.GetHistogram2D()->GetBinContent(j + 1, i + 1) != 0) { nuc_AMap[i][j] = 1; }
        }
    }

    for (const auto &bin : NucleonMomSliceLimits) {
        vector<vector<int>> nuc_slice(HistNucSliceNumOfYBins, vector<int>(HistNucSliceNumOfXBins, 0));
        for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
            for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
                if (p_AMap_Slices[bin][i][j] == 1 && n_AMap_Slices[bin][i][j] == 1) { nuc_slice[i][j] = 1; }
            }
        }
        nuc_AMap_Slices.push_back(nuc_slice);
        nuc_WMap_Slices.push_back(nuc_slice);  // TODO: figure out if really need these!
    }
}

// SaveToTXTFiles function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SaveToTXTFiles function */

// TODO: separate into AMaps and WMaps

void AMaps::SaveToTXTFiles(const std::string &SampleName, const std::string &AcceptanceMapsDirectory) {
    bool PrintOut = false;

    int testNumber = 0;

    std::string AMapSliceElectronSavePath = AcceptanceMapsDirectory + SampleName + "/e_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceElectronSavePath).c_str());
    std::string WMapSliceElectronSavePath = AcceptanceMapsDirectory + SampleName + "/e_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceElectronSavePath).c_str());
    std::string AMapSliceElectronSavePathCopy = AMapCopySavePath + "/e_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceElectronSavePathCopy).c_str());
    std::string WMapSliceElectronSavePathCopy = AMapCopySavePath + "/e_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceElectronSavePathCopy).c_str());

    std::string AMapSliceProtonSavePath = AcceptanceMapsDirectory + SampleName + "/p_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceProtonSavePath).c_str());
    std::string WMapSliceProtonSavePath = AcceptanceMapsDirectory + SampleName + "/p_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceProtonSavePath).c_str());
    std::string AMapSliceProtonSavePathCopy = AMapCopySavePath + "/p_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceProtonSavePathCopy).c_str());
    std::string WMapSliceProtonSavePathCopy = AMapCopySavePath + "/p_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceProtonSavePathCopy).c_str());

    std::string AMapSliceNeutronSavePath = AcceptanceMapsDirectory + SampleName + "/n_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceNeutronSavePath).c_str());
    std::string WMapSliceNeutronSavePath = AcceptanceMapsDirectory + SampleName + "/n_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNeutronSavePath).c_str());
    std::string AMapSliceNeutronSavePathCopy = AMapCopySavePath + "/n_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceNeutronSavePathCopy).c_str());
    std::string WMapSliceNeutronSavePathCopy = AMapCopySavePath + "/n_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNeutronSavePathCopy).c_str());

    std::string AMapSliceNucleonSavePath = AcceptanceMapsDirectory + SampleName + "/nuc_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceNucleonSavePath).c_str());
    std::string WMapSliceNucleonSavePath = AcceptanceMapsDirectory + SampleName + "/nuc_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNucleonSavePath).c_str());
    std::string AMapSliceNucleonSavePathCopy = AMapCopySavePath + "/nuc_AMap_by_slice/";
    system(("mkdir -p " + AMapSliceNucleonSavePathCopy).c_str());
    std::string WMapSliceNucleonSavePathCopy = AMapCopySavePath + "/nuc_WMap_by_slice/";
    system(("mkdir -p " + WMapSliceNucleonSavePathCopy).c_str());

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

    #pragma region /* Save electron slices */
    for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
        ofstream e_AMap_TempFile, e_WMap_TempFile;

        std::string AMapTempFileName =
            "e_AMap_file_from_" + ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(0), 2) + "_to_" + ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string WMapTempFileName =
            "e_WMap_file_from_" + ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(0), 2) + "_to_" + ToStringWithPrecision(ElectronMomSliceLimits.at(Slice).at(1), 2) + ".par";

        e_AMap_TempFile.open(AMapSliceElectronSavePath + AMapTempFileName);
        e_WMap_TempFile.open(WMapSliceElectronSavePath + WMapTempFileName);

        e_AMap_TempFile << "Lower_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(0) << "\n";
        e_AMap_TempFile << "Upper_P_lim:\t" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
        e_AMap_TempFile << "\n";

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

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

    #pragma region /* Save proton, neutron & nucleon slices */
    for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
        ofstream p_AMap_TempFile, p_WMap_TempFile, n_AMap_TempFile, n_WMap_TempFile, nuc_AMap_TempFile, nuc_WMap_TempFile;

        std::string ProtonAMapTempFileName =
            "p_AMap_file_from_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string ProtonWMapTempFileName =
            "p_WMap_file_from_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string NeutronAMapTempFileName =
            "n_AMap_file_from_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string NeutronWMapTempFileName =
            "n_WMap_file_from_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string NucleonAMapTempFileName =
            "nuc_AMap_file_from_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";
        std::string NucleonWMapTempFileName =
            "nuc_WMap_file_from_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(0), 2) + "_to_" + ToStringWithPrecision(NucleonMomSliceLimits.at(Slice).at(1), 2) + ".par";

        p_AMap_TempFile.open(AMapSliceProtonSavePath + ProtonAMapTempFileName);
        p_WMap_TempFile.open(WMapSliceProtonSavePath + ProtonWMapTempFileName);
        n_AMap_TempFile.open(AMapSliceNeutronSavePath + NeutronAMapTempFileName);
        n_WMap_TempFile.open(WMapSliceNeutronSavePath + NeutronWMapTempFileName);
        nuc_AMap_TempFile.open(AMapSliceNucleonSavePath + NucleonAMapTempFileName);
        nuc_WMap_TempFile.open(WMapSliceNucleonSavePath + NucleonWMapTempFileName);

        p_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        p_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        p_AMap_TempFile << "\n";
        n_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        n_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        n_AMap_TempFile << "\n";
        nuc_AMap_TempFile << "Lower_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(0) << "\n";
        nuc_AMap_TempFile << "Upper_P_lim:\t" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        nuc_AMap_TempFile << "\n";

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

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

    #pragma region /* Save combined maps */
    // TODO: figure out rather or not to keep these combind maps!
    ofstream e_AMap_file, p_AMap_file, n_AMap_file, nuc_AMap_file;

    e_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/e_AMap_file.par");
    p_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/p_AMap_file.par");
    n_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/n_AMap_file.par");
    nuc_AMap_file.open(AcceptanceMapsDirectory + SampleName + "/nuc_AMap_file.par");

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
        e_AMap_file << "e_slice_" << (Slice + 1) << "\t" << ElectronMomSliceLimits.at(Slice).at(0) << ":" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
    }

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int Slice = 0; Slice < NucleonMomSliceLimits.size(); Slice++) {
        p_AMap_file << "p_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        n_AMap_file << "n_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
        nuc_AMap_file << "nuc_slice_" << (Slice + 1) << "\t" << NucleonMomSliceLimits.at(Slice).at(0) << ":" << NucleonMomSliceLimits.at(Slice).at(1) << "\n";
    }

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

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

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
        p_AMap_file << "Line\t";
        n_AMap_file << "Line\t";
        nuc_AMap_file << "Line\t";

        for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
            if (j != HistNucSliceNumOfXBins - 1) {
                if (PrintOut) { cout << "\n\np_AMap_file << p_AMap.at(i).at(j) <<\n"; }
                p_AMap_file << p_AMap.at(i).at(j) << ":";
                if (PrintOut) { cout << "\n\nn_AMap_file << n_AMap.at(i).at(j) <<\n"; }
                n_AMap_file << n_AMap.at(i).at(j) << ":";
                if (PrintOut) { cout << "\n\nnuc_AMap_file << nuc_AMap.at(i).at(j) <<\n"; }
                nuc_AMap_file << nuc_AMap.at(i).at(j) << ":";
            } else {
                if (PrintOut) { cout << "\n\np_AMap_file << p_AMap.at(i).at(j) <<\n"; }
                p_AMap_file << p_AMap.at(i).at(j);
                if (PrintOut) { cout << "\n\nn_AMap_file << n_AMap.at(i).at(j) <<\n"; }
                n_AMap_file << n_AMap.at(i).at(j);
                if (PrintOut) { cout << "\n\nnuc_AMap_file << nuc_AMap.at(i).at(j) <<\n"; }
                nuc_AMap_file << nuc_AMap.at(i).at(j);
            }
        }

        p_AMap_file << "\n";
        n_AMap_file << "\n";
        nuc_AMap_file << "\n";
    }

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

    e_AMap_file.close();
    p_AMap_file.close();
    n_AMap_file.close();
    nuc_AMap_file.close();

    system(("cp " + AcceptanceMapsDirectory + SampleName + "/e_AMap_file.par " + AMapCopySavePath).c_str());
    system(("cp " + AcceptanceMapsDirectory + SampleName + "/p_AMap_file.par " + AMapCopySavePath).c_str());
    system(("cp " + AcceptanceMapsDirectory + SampleName + "/n_AMap_file.par " + AMapCopySavePath).c_str());
    system(("cp " + AcceptanceMapsDirectory + SampleName + "/nuc_AMap_file.par " + AMapCopySavePath).c_str());
    #pragma endregion

    #pragma region /* Slice limits */
    ofstream e_slice_limits, p_slice_limits, n_slice_limits, nuc_slice_limits;

    e_slice_limits.open(AMapSliceElectronSavePath + "e_slice_limits.par");
    p_slice_limits.open(AMapSliceProtonSavePath + "p_slice_limits.par");
    n_slice_limits.open(AMapSliceNeutronSavePath + "n_slice_limits.par");
    nuc_slice_limits.open(AMapSliceNucleonSavePath + "nuc_slice_limits.par");

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

    for (int Slice = 0; Slice < ElectronMomSliceLimits.size(); Slice++) {
        e_slice_limits << "e_slice_" << (Slice + 1) << "\t" << ElectronMomSliceLimits.at(Slice).at(0) << ":" << ElectronMomSliceLimits.at(Slice).at(1) << "\n";
    }

    if (PrintOut) { ++testNumber, cout << "\n\n\nTEST " << testNumber << "\n"; }

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
    #pragma endregion
}
#pragma endregion

// DrawAndSaveHitMapsPDFs function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveHitMapsPDFs function */
void AMaps::DrawAndSaveHitMapsPDFs(vector<TObject *> HistoList, const std::string &PDFfileName) {
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

        HistoList[i]->Draw("colz");

        HistoList[i]->SetStats(1);
        gStyle->SetOptStat("ourmen");
        gStyle->SetStatX(0.98);   // gStyle->SetStatX(0.87);
        gStyle->SetStatY(0.935);  // gStyle->SetStatY(0.875);
        gPad->Modified();
        gPad->Update();

        gPad->Update();
        TPaletteAxis *palette = (TPaletteAxis *)HistoList[i]->GetListOfFunctions()->FindObject("palette");
        if (palette) { palette->SetY2NDC(0.55), gPad->Modified(), gPad->Update(); }

        myCanvas_temp->Print(fileName, "pdf");
        myCanvas_temp->Clear();
    }

    sprintf(fileName, "%s]", PDFfileName.c_str());
    myCanvas_temp->Print(fileName, "pdf");

    delete myText_temp;
    delete myCanvas_temp;
}
#pragma endregion

// DrawAndSaveHitMaps function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveHitMaps function */
void AMaps::DrawAndSaveHitMaps(const std::string &SampleName, TCanvas *h1DCanvas, const std::string &AcceptanceMapsDirectory) {
    cout << "\n\nProcessing maps...\n";

    std::string SampleNameTemp = SampleName;
    const char *AcceptanceMapsBC_OutFile = AcceptanceMapsBC_OutFile0.c_str();
    const char *TLAMaps_OutFile = TLAMaps_OutFile0.c_str();
    const char *RecoAMaps_OutFile = RecoAMaps_OutFile0.c_str();
    const char *AMapsRatio_OutFile = AMapsRatio_OutFile0.c_str();
    const char *Charged_particle_Sep_AMaps_OutFile = Charged_particle_Sep_AMaps_OutFile0.c_str();
    const char *AcceptanceMaps_OutFile = AcceptanceMaps_OutFile0.c_str();

    cout << "\n\nGenerating maps directories...\n";
    system(("mkdir -p " + AcceptanceMapsDirectory + SampleName).c_str());

    cout << "\n\nCalculating efficiencies...\n";
    // if (FindSubstring(SampleName, "1e")) {
    //     calc_Electron_RecoToTL_Ratio = true;
    //     calc_Neutron_RecoToTL_Ratio = false;
    //     calc_Proton_RecoToTL_Ratio = false;
    // } else if (FindSubstring(SampleName, "en")) {
    //     calc_Electron_RecoToTL_Ratio = false;
    //     calc_Neutron_RecoToTL_Ratio = true;
    //     calc_Proton_RecoToTL_Ratio = false;
    // } else if (FindSubstring(SampleName, "ep")) {
    //     calc_Electron_RecoToTL_Ratio = false;
    //     calc_Neutron_RecoToTL_Ratio = false;
    //     calc_Proton_RecoToTL_Ratio = true;
    // }

    CalcAMapsRatio();

    cout << "\n\nGenerating filtered maps...\n";
    GenerateCPartAMaps(Charged_particle_min_Ratio);
    GenerateNPartAMaps(Neutral_particle_min_Ratio);
    GenerateNucleonAMap();

    cout << "\n\nSaving maps...\n";
    SaveToTXTFiles(SampleName, AcceptanceMapsDirectory);

    SaveToPNGFiles(SampleNameTemp, h1DCanvas, AcceptanceMapsBC_OutFile, TLAMaps_OutFile, RecoAMaps_OutFile, AMapsRatio_OutFile, Charged_particle_Sep_AMaps_OutFile, AcceptanceMaps_OutFile);

    SaveToPDFFiles();

    SaveToRootFiles(SampleName, AcceptanceMapsDirectory);
}

// SaveToPNGFiles function ------------------------------------------------------------------------------------------------------------------------------------------

void AMaps::SaveToPNGFiles(std::__1::string &SampleNameTemp, TCanvas *h1DCanvas, const char *AcceptanceMapsBC_OutFile, const char *TLAMaps_OutFile, const char *RecoAMaps_OutFile,
                           const char *AMapsRatio_OutFile, const char *Charged_particle_Sep_AMaps_OutFile, const char *AcceptanceMaps_OutFile) {
    /* Acceptance maps BC */
    reco_theta_e_VS_phi_e_BC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, AcceptanceMapsBC_OutFile, true);
    reco_theta_p_VS_phi_p_BC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, AcceptanceMapsBC_OutFile, true);
    reco_theta_n_VS_phi_n_BC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, AcceptanceMapsBC_OutFile, true);
    reco_theta_nuc_VS_phi_nuc_BC.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMapsBC, AcceptanceMapsBC_OutFile, true);

    /* Electron maps */
    for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
        truth_theta_e_VS_phi_e_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLAMaps, TLAMaps_OutFile, true);
        reco_theta_e_VS_phi_e_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoAMaps, RecoAMaps_OutFile, true);
        acceptance_eff_e_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, AMapsRatio, AMapsRatio_OutFile, true);
        filtered_reco_theta_e_VS_phi_e_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, Charged_particle_Sep_AMaps, Charged_particle_Sep_AMaps_OutFile, true);
    }

    /* Proton maps */
    for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
        truth_theta_p_VS_phi_p_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLAMaps, TLAMaps_OutFile, true);
        reco_theta_p_VS_phi_p_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoAMaps, RecoAMaps_OutFile, true);
        acceptance_eff_p_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, AMapsRatio, AMapsRatio_OutFile, true);
        filtered_reco_theta_p_VS_phi_p_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, Charged_particle_Sep_AMaps, Charged_particle_Sep_AMaps_OutFile, true);
    }

    /* Neutron maps (by momentum slice) */
    for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
        truth_theta_n_VS_phi_n_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, TLAMaps, TLAMaps_OutFile, true);
        reco_theta_n_VS_phi_n_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, RecoAMaps, RecoAMaps_OutFile, true);
        acceptance_eff_n_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, AMapsRatio, AMapsRatio_OutFile, true);
        filtered_reco_theta_n_VS_phi_n_BySlice.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, Charged_particle_Sep_AMaps, Charged_particle_Sep_AMaps_OutFile, true);
    }

    /* Neutron maps */
    truth_theta_n_VS_phi_n.hDrawAndSave(SampleNameTemp, h1DCanvas, TLAMaps, TLAMaps_OutFile, true);
    reco_theta_n_VS_phi_n.hDrawAndSave(SampleNameTemp, h1DCanvas, RecoAMaps, RecoAMaps_OutFile, true);
    acceptance_eff_n.hDrawAndSave(SampleNameTemp, h1DCanvas, AMapsRatio, AMapsRatio_OutFile, true);

    /* Finalized acceptance maps */
    filtered_reco_theta_e_VS_phi_e.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, AcceptanceMaps_OutFile, true);
    filtered_reco_theta_p_VS_phi_p.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, AcceptanceMaps_OutFile, true);
    filtered_reco_theta_n_VS_phi_n.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, AcceptanceMaps_OutFile, true);
    filtered_reco_theta_nuc_VS_phi_nuc.hDrawAndSave(SampleNameTemp, h1DCanvas, AcceptanceMaps, AcceptanceMaps_OutFile, true);
}
#pragma endregion

// SaveToPDFFiles function ------------------------------------------------------------------------------------------------------------------------------------------

void AMaps::SaveToPDFFiles() {
    /* Saving maps in PDFs */
    DrawAndSaveHitMapsPDFs(truth_e_BySlice, truth_theta_e_VS_phi_e_BySlice[0].GetHistogram2DSaveNamePath() + "truth_theta_e_VS_phi_e_BySlice.pdf");
    DrawAndSaveHitMapsPDFs(reco_e_BySlice, reco_theta_e_VS_phi_e_BySlice[0].GetHistogram2DSaveNamePath() + "reco_theta_e_VS_phi_e_BySlice.pdf");
    DrawAndSaveHitMapsPDFs(acc_eff_e_BySlice, acceptance_eff_e_BySlice[0].GetHistogram2DSaveNamePath() + "acceptance_eff_e_BySlice.pdf");
    DrawAndSaveHitMapsPDFs(filtered_reco_e_BySlice, filtered_reco_theta_e_VS_phi_e_BySlice[0].GetHistogram2DSaveNamePath() + "filtered_reco_theta_e_VS_phi_e_BySlice.pdf");

    DrawAndSaveHitMapsPDFs(truth_p_BySlice, truth_theta_p_VS_phi_p_BySlice[0].GetHistogram2DSaveNamePath() + "truth_theta_p_VS_phi_p_BySlice.pdf");
    DrawAndSaveHitMapsPDFs(reco_p_BySlice, reco_theta_p_VS_phi_p_BySlice[0].GetHistogram2DSaveNamePath() + "reco_theta_p_VS_phi_p_BySlice.pdf");
    DrawAndSaveHitMapsPDFs(acc_eff_p_BySlice, acceptance_eff_p_BySlice[0].GetHistogram2DSaveNamePath() + "acceptance_eff_p_BySlice.pdf");
    DrawAndSaveHitMapsPDFs(filtered_reco_p_BySlice, filtered_reco_theta_p_VS_phi_p_BySlice[0].GetHistogram2DSaveNamePath() + "filtered_reco_theta_p_VS_phi_p_BySlice.pdf");

    DrawAndSaveHitMapsPDFs(truth_n_BySlice, truth_theta_n_VS_phi_n_BySlice[0].GetHistogram2DSaveNamePath() + "truth_theta_n_VS_phi_n_BySlice.pdf");
    DrawAndSaveHitMapsPDFs(reco_n_BySlice, reco_theta_n_VS_phi_n_BySlice[0].GetHistogram2DSaveNamePath() + "reco_theta_n_VS_phi_n_BySlice.pdf");
    DrawAndSaveHitMapsPDFs(acc_eff_n_BySlice, acceptance_eff_n_BySlice[0].GetHistogram2DSaveNamePath() + "acceptance_eff_n_BySlice.pdf");
    DrawAndSaveHitMapsPDFs(filtered_reco_n_BySlice, filtered_reco_theta_n_VS_phi_n_BySlice[0].GetHistogram2DSaveNamePath() + "filtered_reco_theta_n_VS_phi_n_BySlice.pdf");
}

// SaveToRootFiles function ------------------------------------------------------------------------------------------------------------------------------------------

void AMaps::SaveToRootFiles(const std::__1::string &SampleName, const std::__1::string &AcceptanceMapsDirectory) {
    #pragma region /* Save TL Acceptance maps to plots directory */
    SaveToPlotDir(SampleName);

    #pragma region /* Save TL Acceptance maps to reference Acceptance maps directory */
    SaveToRefDir(AcceptanceMapsDirectory, SampleName);
}

// SaveToRefDir function ------------------------------------------------------------------------------------------------------------------------------------------

void AMaps::SaveToRefDir(const std::__1::string &AcceptanceMapsDirectory, const std::__1::string &SampleName) {
    cout << "\n\nSaving acceptance maps to reference Acceptance maps directory...";

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

    cout << "done!\n\n\n";
}

// SaveToPlotDir function ------------------------------------------------------------------------------------------------------------------------------------------

void AMaps::SaveToPlotDir(const std::__1::string &SampleName) {
    cout << "\n\nSaving acceptance maps to plots directory...";
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

    cout << "done!\n";
}

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

            if (PrintHistInfo) { cout << "Histo found: " << h->GetName() << " - " << h->GetTitle() << endl; }

            total++;
        }
    }

    if (PrintHistInfo) { cout << "\n\nFound " << total << " Histograms\n" << endl; }

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
        cout << "\n\nTitle = " << Title << "\n\n";
        cout << "NumOfXBinsStr = " << NumOfXBinsStr << "\n";
        cout << "NumOfYBinsStr = " << NumOfYBinsStr << "\n\n";

        cout << "HistNucSliceNumOfXBins = " << HistNucSliceNumOfXBins << "\n";
        cout << "HistNucSliceNumOfYBins = " << HistNucSliceNumOfYBins << "\n\n";
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
        cout << "\n\nTitle = " << Title << "\n\n";
        cout << "SliceLowerLimStr = " << SliceLowerLimStr << "\n";
        cout << "SliceUpperLimStr = " << SliceUpperLimStr << "\n\n";

        cout << "SliceLowerLim = " << SliceLowerLim << "\n";
        cout << "SliceUpperLim = " << SliceUpperLim << "\n\n";
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
        cout << "\n\nTitle = " << Title << "\n\n";
        cout << "SliceLowerLimStr = " << SliceLowerLimStr << "\n";
        cout << "SliceUpperLimStr = " << SliceUpperLimStr << "\n\n";

        cout << "SliceLowerLim = " << SliceLowerLim << "\n";
        cout << "SliceUpperLim = " << SliceUpperLim << "\n\n";

        cout << "MomBinsLimitsTemp.size() = " << MomBinsLimitsTemp.size() << "\n\n";
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
    if (!AMapsBC_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load AMapsBC root file! Exiting...\n", exit(0); }

    for (TObject *keyAsObj : *AMapsBC_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;

        if (FindSubstring(TempHist->GetTitle(), "Electron") || FindSubstring(TempHist->GetTitle(), "electron")) {
            reco_theta_e_VS_phi_e_BC.SetHistogram2D(TempHist);
        } else if (FindSubstring(TempHist->GetTitle(), "Proton") || FindSubstring(TempHist->GetTitle(), "proton")) {
            reco_theta_p_VS_phi_p_BC.SetHistogram2D(TempHist);
        } else if (FindSubstring(TempHist->GetTitle(), "Neutron") || FindSubstring(TempHist->GetTitle(), "neutron")) {
            reco_theta_n_VS_phi_n_BC.SetHistogram2D(TempHist);
        } else if (FindSubstring(TempHist->GetTitle(), "Nucleon") || FindSubstring(TempHist->GetTitle(), "nucleon")) {
            reco_theta_nuc_VS_phi_nuc_BC.SetHistogram2D(TempHist);
        }
    }
    #pragma endregion

    #pragma region /* Load Hit_Maps_TL */
    std::string Hit_Maps_TL_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_TL_prefix + SampleName + ".root";
    TFile *Hit_Maps_TL_RootFile = new TFile(Hit_Maps_TL_RootFile_FileName.c_str());
    if (!Hit_Maps_TL_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    int counter = 0;

    for (TObject *keyAsObj : *Hit_Maps_TL_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (counter == 0) { SetHistBinsFromHistTitle(TempHist); }

        if (FindSubstring(TempHist->GetTitle(), "{e}")) {
            truth_theta_e_VS_phi_e_BySlice.push_back(Temp2DHist);
            SetSlicesFromHistTitle(TempHist, NucleonMomSliceLimits);
        } else if (FindSubstring(TempHist->GetTitle(), "{p}")) {
            truth_theta_p_VS_phi_p_BySlice.push_back(Temp2DHist);
        } else if (FindSubstring(TempHist->GetTitle(), "{n}")) {
            truth_theta_n_VS_phi_n.SetHistogram2D(TempHist);
        }

        ++counter;
    }
    #pragma endregion

    #pragma region /* Load Hit_Maps_Reco */
    std::string Hit_Maps_Reco_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_Reco_prefix + SampleName + ".root";
    TFile *Hit_Maps_Reco_RootFile = new TFile(Hit_Maps_Reco_RootFile_FileName.c_str());
    if (!Hit_Maps_Reco_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_Reco root file! Exiting...\n", exit(0); }

    for (TObject *keyAsObj : *Hit_Maps_Reco_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (FindSubstring(TempHist->GetTitle(), "{e}")) {
            reco_theta_e_VS_phi_e_BySlice.push_back(Temp2DHist);
        } else if (FindSubstring(TempHist->GetTitle(), "{p}")) {
            reco_theta_p_VS_phi_p_BySlice.push_back(Temp2DHist);
        } else if (FindSubstring(TempHist->GetTitle(), "{n}")) {
            reco_theta_n_VS_phi_n.SetHistogram2D(TempHist);
        }
    }
    #pragma endregion

    #pragma region /* Load Hit_Maps_Ratio */
    std::string Hit_Maps_Ratio_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_Ratio_prefix + SampleName + ".root";
    TFile *Hit_Maps_Ratio_RootFile = new TFile(Hit_Maps_Ratio_RootFile_FileName.c_str());
    if (!Hit_Maps_Ratio_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load Hit_Maps_Ratio root file! Exiting...\n", exit(0); }

    for (TObject *keyAsObj : *Hit_Maps_Ratio_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (FindSubstring(TempHist->GetTitle(), "{e}")) {
            acceptance_eff_e_BySlice.push_back(Temp2DHist);
        } else if (FindSubstring(TempHist->GetTitle(), "{p}")) {
            acceptance_eff_p_BySlice.push_back(Temp2DHist);
        } else if (FindSubstring(TempHist->GetTitle(), "{n}")) {
            acceptance_eff_n.SetHistogram2D(TempHist);
        }
    }
    #pragma endregion

    #pragma region /* Load cPart_Sep_AMaps */
    std::string cPart_Sep_AMaps_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root";
    TFile *cPart_Sep_AMaps_RootFile = new TFile(cPart_Sep_AMaps_RootFile_FileName.c_str());
    if (!cPart_Sep_AMaps_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load cPart_Sep_AMaps root file! Exiting...\n", exit(0); }

    for (TObject *keyAsObj : *cPart_Sep_AMaps_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey *>(keyAsObj);

        if (PrintKeys) { cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << endl; }

        TH2D *TempHist = (TH2D *)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (FindSubstring(TempHist->GetTitle(), "Electron") || FindSubstring(TempHist->GetTitle(), "electron")) {
            filtered_reco_theta_e_VS_phi_e_BySlice.push_back(Temp2DHist);
        } else if (FindSubstring(TempHist->GetTitle(), "Proton") || FindSubstring(TempHist->GetTitle(), "proton")) {
            filtered_reco_theta_p_VS_phi_p_BySlice.push_back(Temp2DHist);
        }
    }
    #pragma endregion

    #pragma region /* Load AMaps */
    std::string AMaps_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMaps_prefix + SampleName + ".root";
    TFile *AMaps_RootFile = new TFile(AMaps_RootFile_FileName.c_str());
    if (!AMaps_RootFile) { cout << "\n\nAMaps::ReadHitMaps: could not load AMaps root file! Exiting...\n", exit(0); }

    LoadedElectronAMaps0 = (TH2D *)AMaps_RootFile->Get("Electron_AMap");
    if (!LoadedElectronAMaps0) { cout << "\n\nAMaps::ReadHitMaps: could not load Electron_AMap from root file! Exiting...\n", exit(0); }

    LoadedProtonAMap = (TH2D *)AMaps_RootFile->Get("Proton_AMap");
    if (!LoadedProtonAMap) { cout << "\n\nAMaps::ReadHitMaps: could not load Proton_AMap from root file! Exiting...\n", exit(0); }

    LoadedNeutronAMap = (TH2D *)AMaps_RootFile->Get("Neutron_AMap");
    if (!LoadedNeutronAMap) { cout << "\n\nAMaps::ReadHitMaps: could not load Neutron_AMap from root file! Exiting...\n", exit(0); }

    LoadedNucleonAMap = (TH2D *)AMaps_RootFile->Get("Nucleon_AMap");
    if (!LoadedNucleonAMap) { cout << "\n\nAMaps::ReadHitMaps: could not load Nucleon_AMap from root file! Exiting...\n", exit(0); }
    #pragma endregion

    cout << "\n\nAcceptance maps loaded!\n\n";
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

            if (FindSubstring(parameter, "_slice_")) {
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
        cout << "\n\nAMaps::ReadAMap: file not found! Exiting...\n\n", exit(0);
    }
}
#pragma endregion

// ReadAMapSlices function (AMaps) --------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadAMapSlices function (AMaps) */
void AMaps::ReadAMapSlices(const std::string &SampleName, const std::string &AcceptanceMapsDirectory, const std::string &Particle, const vector<vector<double>> &Loaded_particle_limits,
                           vector<vector<vector<int>>

                                  > &Loaded_Particle_AMap_Slices) {
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

        std::string TempFileName = ParticleShort + "_AMap_by_slice/" + ParticleShort + "_AMap_file_from_" + ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(0), 2) + "_to_" +
                                   ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(1), 2) + ".par";

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

        std::string TempFileName = ParticleShort + "_WMap_by_slice/" + ParticleShort + "_WMap_file_from_" + ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(0), 2) + "_to_" +
                                   ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(1), 2) + ".par";

        ReadWMap((AcceptanceMapsDirectory + SampleName + "/" + TempFileName).c_str(), Loaded_Particle_WMap_TempSlice);

        Loaded_Particle_WMap_Slices.push_back(Loaded_Particle_WMap_TempSlice);
    }
}
#pragma endregion

// ReadAMap function (AMaps) --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadAMap function (AMaps) */
/* A function that reads AMaps */

void AMaps::ReadAMap(const char *filename, vector<vector<int>> &Loaded_particle_AMap) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        std::string tp;

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            std::string parameter, parameter2;
            ss >> parameter;  // get cut identifier

            if (FindSubstring(parameter, "Line")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);

                std::string LineEntry;
                vector<int> col;

                while (getline(ss2, LineEntry, ':')) { col.push_back(stoi(LineEntry)); }

                Loaded_particle_AMap.push_back(col);
            }
        }
    } else {
        cout << "\n\nAMaps::ReadAMap: file:\n" << filename << "\nwas not found! Exiting...\n\n", exit(0);
    }
}
#pragma endregion

// ReadWMap function (WMaps) --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadWMap function (WMaps) */
/* A function that reads WMaps */

void AMaps::ReadWMap(const char *filename, vector<vector<double>> &Loaded_particle_WMaps) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        std::string tp;

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            std::string parameter, parameter2;
            ss >> parameter;  // get cut identifier

            if (FindSubstring(parameter, "Weight")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);

                std::string LineEntry;
                vector<double> col;

                while (getline(ss2, LineEntry, ':')) { col.push_back(stod(LineEntry)); }

                Loaded_particle_WMaps.push_back(col);
            }
        }
    } else {
        cout << "\n\nAMaps::ReadWMap: file:\n" << filename << "\nwas not found! Exiting...\n\n", exit(0);
    }
}
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
                                    if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                } else {
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
                                    if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
                                        return true;
                                    } else {
                                        return false;
                                    }
                                } else {
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

//#pragma region /* MatchAngToHitMap function (original after neutron FC 'bug' fix) */
// bool AMaps::MatchAngToHitMap(const std::string &Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC) {
//     int e_InitialSlice = 0, e_FinalSlice = Loaded_ElectronMomSliceLimits.size();
//     int p_InitialSlice = 0, p_FinalSlice = Loaded_NucleonMomSliceLimits.size(), n_InitialSlice = 0, n_FinalSlice = Loaded_NucleonMomSliceLimits.size();
//
//     int e_TestSlice = Slices2Test.at(0), p_TestSlice = Slices2Test.at(1), n_TestSlice = Slices2Test.at(2);
//
//     if (e_single_slice_test) { e_InitialSlice = e_TestSlice - 1, e_FinalSlice = e_TestSlice; }
//
//     if (nuc_single_slice_test) { p_InitialSlice = p_TestSlice - 1, p_FinalSlice = p_TestSlice, n_InitialSlice = n_TestSlice - 1, n_FinalSlice = n_TestSlice; }
//
//     if (isElectron(Particle)) {
//         for (int Slice = e_InitialSlice; Slice < e_FinalSlice; Slice++) {
//             if (Momentum >= Loaded_ElectronMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_ElectronMomSliceLimits.at(Slice).at(1)) {
//                 for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
//                     double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / HistElectronSliceNumOfYBins;
//                     double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                     double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                     if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                         for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
//                             double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / HistElectronSliceNumOfXBins;
//                             double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                             double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                             if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                                 if (Loaded_e_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                     return true;
//                                 } else {
//                                     return false;
//                                 }
//                             } // end of find right phi if
//                         }
//                     } // end of find right theta if
//                 }
//             } // end of if the right momentum
//         }
//     } else if (isProton(Particle)) {
//         for (int Slice = p_InitialSlice; Slice < p_FinalSlice; Slice++) {
//             if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
//                 for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
//                     double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
//                     double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                     double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                     if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                         for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
//                             double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
//                             double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                             double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                             if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                                 if (NucleonOverlappingFC) {
//                                     if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                         return true;
//                                     } else {
//                                         return false;
//                                     }
//                                 } else {
//                                     if (Loaded_p_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                         return true;
//                                     } else {
//                                         return false;
//                                     }
//                                 }
//                             } // end of find right phi if
//                         }
//                     } // end of find right theta if
//                 }
//             } // end of if the right momentum
//         }
//     } else if (isNeutron(Particle)) {
//         if (NucleonOverlappingFC) {
//             for (int Slice = n_InitialSlice; Slice < n_FinalSlice; Slice++) {
//                 if (Momentum >= Loaded_NucleonMomSliceLimits.at(Slice).at(0) && Momentum <= Loaded_NucleonMomSliceLimits.at(Slice).at(1)) {
//                     for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
//                         double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
//                         double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                         double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                         if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                             for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
//                                 double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
//                                 double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                                 double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                                 if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                                     if (Loaded_nuc_AMap_Slices.at(Slice).at(i).at(j) != 0) {
//                                         return true;
//                                     } else {
//                                         return false;
//                                     }
//                                 } // end of find right phi if
//                             }
//                         } // end of find right theta if
//                     }
//                 } // end of if the right momentum
//             }
//         } else {
//             /* If we're not applying overlapping fiducial cuts (in momRes calculations, for example),
//                then don't look for momentum based cuts, since neutron momentum can be greater than beamE (bad
//                neutrons with P_n > beamE will be cut later!) */
//
//             for (int i = 0; i < HistNucSliceNumOfYBins; i++) {
//                 double dThetaTemp = (hBinUpperYLim - hBinLowerYLim) / (HistNucSliceNumOfYBins);
//                 double ThetaLowerLimTemp = hBinLowerYLim + i * dThetaTemp;
//                 double ThetaUpperLimTemp = ThetaLowerLimTemp + dThetaTemp;
//
//                 if ((Theta >= ThetaLowerLimTemp) && (Theta < ThetaUpperLimTemp)) {
//                     for (int j = 0; j < HistNucSliceNumOfXBins; j++) {
//                         double dPhiTemp = (hBinUpperXLim - hBinLowerXLim) / (HistNucSliceNumOfXBins);
//                         double PhiLowerLimTemp = hBinLowerXLim + j * dPhiTemp;
//                         double PhiUpperLimTemp = PhiLowerLimTemp + dPhiTemp;
//
//                         if ((Phi >= PhiLowerLimTemp) && (Phi < PhiUpperLimTemp)) {
//                             if (Loaded_n_AMap.at(i).at(j) != 0) {
//                                 return true;
//                             } else {
//                                 return false;
//                             }
//                         } // end of find right phi if
//                     }
//                 } // end of find right theta if
//             }
//         }
//     } // end of if Particle
//
//     return false;
// }
//#pragma endregion

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
                                            cout << "MomentumLowerLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                            cout << "Momentum = " << Momentum << "\n";
                                            cout << "MomentumUpperLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";

                                            cout << "ThetaLowerLimTemp = " << ThetaLowerLimTemp << "\n";
                                            cout << "Theta = " << Theta << "\n";
                                            cout << "ThetaUpperLimTemp = " << ThetaUpperLimTemp << "\n\n";

                                            cout << "PhiLowerLimTemp = " << PhiLowerLimTemp << "\n";
                                            cout << "Phi = " << Phi << "\n";
                                            cout << "PhiUpperLimTemp = " << PhiUpperLimTemp << "\n\n";

                                            cout << "Weight = " << Weight << "\n\n";

                                            cout << "1/Weight = " << (1 / Weight) << "\n\n\n\n";
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
                                            cout << "MomentumLowerLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                            cout << "Momentum = " << Momentum << "\n";
                                            cout << "MomentumUpperLimTemp = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";

                                            cout << "ThetaLowerLimTemp = " << ThetaLowerLimTemp << "\n";
                                            cout << "Theta = " << Theta << "\n";
                                            cout << "ThetaUpperLimTemp = " << ThetaUpperLimTemp << "\n\n";

                                            cout << "PhiLowerLimTemp = " << PhiLowerLimTemp << "\n";
                                            cout << "Phi = " << Phi << "\n";
                                            cout << "PhiUpperLimTemp = " << PhiUpperLimTemp << "\n\n";

                                            cout << "Weight = " << Weight << "\n\n";

                                            cout << "1/Weight = " << (1 / Weight) << "\n\n\n\n";
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
    bool inFDQuery, part_inSomeSector;

    if (!Generate_AMaps) {
        part_inSomeSector = MatchAngToHitMap(Particle, Momentum, Theta, Phi, NucleonOverlappingFC);
    } else {
        part_inSomeSector = true;
    }

    inFDQuery = (part_inSomeSector && (Theta >= ThetaFD.GetLowerCutConst()) && (Theta <= ThetaFD.GetUpperCutConst()));

    return inFDQuery;
}
#pragma endregion

#endif  // AMAPS_H