//
// Created by Alon Sportes on 22/06/2023.
//

#include "AMaps.h"

// AMaps constructors ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* AMaps generation constructor (G1) */
/**
 * @brief Generation-mode constructor for the AMaps class.
 *
 * This constructor initializes the full acceptance-map generation pipeline. It prepares
 * all histogram containers, momentum slices, and output directories required to build
 * detector acceptance maps from uniform Monte Carlo samples.
 *
 * Responsibilities:
 *   1. Store configuration parameters (sample name, particle labels, beam energy,
 *      map mode, binning configuration, and output paths).
 *   2. Determine which particle samples exist (electron, proton, neutron) based on
 *      the provided sample name.
 *   3. Create the directory structure where intermediate and final maps will be saved.
 *   4. Configure momentum slice limits for electrons and nucleons using the selected
 *      binning profiles.
 *   5. Allocate all ROOT histograms used during map generation:
 *        - Truth-level hit maps
 *        - Reconstructed hit maps
 *        - Reco/TL efficiency ratio maps
 *        - Thresholded acceptance maps (AMaps)
 *   6. Prepare containers that store the slice-by-slice maps used later to build
 *      matrix-based AMaps and WMaps.
 *
 * Workflow context:
 *   This constructor is used only in the "generation" stage of the analysis:
 *      - histograms are filled via hFillMaps(...)
 *      - efficiencies are computed via CalcAcceptanceEfficiency()
 *      - filtered acceptance maps are produced
 *      - matrices are generated and written to disk
 *
 * Map modes:
 *   - "AMaps" : binary acceptance maps used for fiducial cuts
 *   - "WMaps" : continuous acceptance maps used for event weights
 *
 * @param SampleName             Name of the uniform MC sample being processed.
 * @param P_e_bin_profile        Electron momentum binning profile.
 * @param P_nuc_bin_profile      Nucleon momentum binning profile.
 * @param beamE                  Beam energy used for slice definitions.
 * @param AMapsMode              Operation mode ("AMaps" or "WMaps").
 * @param SavePath               Base directory where output maps will be written.
 * @param nOfNucMomBins          Number of nucleon momentum slices.
 * @param nOfElecMomBins         Number of electron momentum slices.
 * @param hnsNumOfXBins          Number of φ bins for nucleon histograms.
 * @param hnsNumOfYBins          Number of θ bins for nucleon histograms.
 * @param hesNumOfXBins          Number of φ bins for electron histograms.
 * @param hesNumOfYBins          Number of θ bins for electron histograms.
 * @param ElectronLabel          Label used for electron histogram axes.
 * @param ProtonLabel            Label used for proton histogram axes.
 * @param NeutronLabel           Label used for neutron histogram axes.
 * @param type                   ParticleType specifying which particle maps are active.
 */
AMaps::AMaps(const std::string& SampleName, const std::string& P_e_bin_profile, const std::string& P_nuc_bin_profile, double beamE, const std::string& AMapsMode, const std::string& SavePath,
             int nOfNucMomBins, int nOfElecMomBins, int hnsNumOfXBins, int hnsNumOfYBins, int hesNumOfXBins, int hesNumOfYBins, const std::string& ElectronLabel,
             const std::string& ProtonLabel, const std::string& NeutronLabel, ParticleType type)
    : particleType(type) {
    eLabel = ElectronLabel, pLabel = ProtonLabel, nLabel = NeutronLabel;
    Maps_Mode = AMapsMode;
    SName = SampleName;
    AMapSavePath = SavePath;
    HistNucSliceNumOfXBins = hnsNumOfXBins;
    HistNucSliceNumOfYBins = hnsNumOfYBins;
    HistElectronSliceNumOfXBins = hesNumOfXBins;
    HistElectronSliceNumOfYBins = hesNumOfYBins;
    NumberNucOfMomSlices = nOfNucMomBins;
    NumberElecOfMomSlices = nOfElecMomBins;

    bt::PrintMessage("[AMaps::AMaps (generator)] Setting uniform sample names...", false);

    const bool hasElectronSample = bt::FindSubstring(SName, "Uniform_1e_sample_");
    const bool hasProtonSample = bt::FindSubstring(SName, "Uniform_ep_sample_");
    const bool hasNeutronSample = bt::FindSubstring(SName, "Uniform_en_sample_");

    bt::PrintMessage("[AMaps::AMaps (generator)] done.", false);

#pragma region /* Setting saving directories */
    bt::PrintMessage("[AMaps::AMaps (generator)] Setting " + Maps_Mode + " saving directories...", false);

    auto EnsureDirectory = [](const std::string& path) { system(("mkdir -p " + path).c_str()); };

    std::string SavePathAMapsBC = AMapSavePath + "00b_AMaps_BC_from_class/";
    EnsureDirectory(SavePathAMapsBC);

    std::string AMapSavePathTLElectron, AMapSavePathRecoElectron, AMapSavePathRecoToTLElectron, AMapSavePathSepAMapsElectron;

    if (hasElectronSample) {
        AMapSavePathTLElectron = SavePath + "01a_Electron_TL_Hit_Maps/";
        EnsureDirectory(AMapSavePathTLElectron);
        AMapSavePathRecoElectron = SavePath + "01b_Electron_Reco_Hit_Maps/";
        EnsureDirectory(AMapSavePathRecoElectron);
        AMapSavePathRecoToTLElectron = SavePath + "01c_Electron_RecoToTL_Ratio/";
        EnsureDirectory(AMapSavePathRecoToTLElectron);
        AMapSavePathSepAMapsElectron = AMapSavePath + "01d_Electron_Separate_AMaps/";
        EnsureDirectory(AMapSavePathSepAMapsElectron);
    }

    std::string AMapSavePathTLProton, AMapSavePathRecoProton, AMapSavePathRecoToTLProton, AMapSavePathSepAMapsProton;

    if (hasProtonSample) {
        AMapSavePathTLProton = SavePath + "02a_Proton_TL_Hit_Maps/";
        EnsureDirectory(AMapSavePathTLProton);
        AMapSavePathRecoProton = SavePath + "02b_Proton_Reco_Hit_Maps/";
        EnsureDirectory(AMapSavePathRecoProton);
        AMapSavePathRecoToTLProton = SavePath + "02c_Proton_RecoToTL_Ratio/";
        EnsureDirectory(AMapSavePathRecoToTLProton);
        AMapSavePathSepAMapsProton = AMapSavePath + "02d_Proton_Separate_AMaps/";
        EnsureDirectory(AMapSavePathSepAMapsProton);
    }

    std::string AMapSavePathTLNeutron, AMapSavePathRecoNeutron, AMapSavePathRecoToTLNeutron, AMapSavePathSepAMapsNeutron;
    std::string AMapConstSavePathTLNeutron, AMapConstSavePathRecoNeutron, AMapConstSavePathRecoToTLNeutron, AMapConstSavePathSepAMapsNeutron;

    if (hasNeutronSample) {
        AMapSavePathTLNeutron = SavePath + "03aa_Neutron_TL_Hit_Maps/";
        EnsureDirectory(AMapSavePathTLNeutron);
        AMapSavePathRecoNeutron = SavePath + "03ab_Neutron_Reco_Hit_Maps/";
        EnsureDirectory(AMapSavePathRecoNeutron);
        AMapSavePathRecoToTLNeutron = SavePath + "03ac_Neutron_RecoToTL_Ratio/";
        EnsureDirectory(AMapSavePathRecoToTLNeutron);
        AMapSavePathSepAMapsNeutron = AMapSavePath + "03ad_Neutron_Separate_AMaps/";
        EnsureDirectory(AMapSavePathSepAMapsNeutron);

        AMapConstSavePathTLNeutron = SavePath + "03ba_Neutron_TL_Hit_Maps_Const/";
        EnsureDirectory(AMapConstSavePathTLNeutron);
        AMapConstSavePathRecoNeutron = SavePath + "03bb_Neutron_Reco_Hit_Maps_Const/";
        EnsureDirectory(AMapConstSavePathRecoNeutron);
        AMapConstSavePathRecoToTLNeutron = SavePath + "03bc_Neutron_RecoToTL_Ratio_Const/";
        EnsureDirectory(AMapConstSavePathRecoToTLNeutron);
        AMapConstSavePathSepAMapsNeutron = AMapSavePath + "03bd_Neutron_Separate_AMaps_Const/";
        EnsureDirectory(AMapConstSavePathSepAMapsNeutron);
    }

    std::string AMapSavePathAMap = AMapSavePath + "04_Finalized_AMaps/";
    EnsureDirectory(AMapSavePathAMap);

    std::string AMapSavePathGeneratedAMapCopy = AMapSavePath + "05_Generated_maps/";
    EnsureDirectory(AMapSavePathGeneratedAMapCopy);
    AMapCopySavePath = AMapSavePathGeneratedAMapCopy;

    bt::PrintMessage("[AMaps::AMaps (generator)] done.", false);
#pragma endregion

#pragma region /* Setting AMapsMode_TitleAddition */
    std::string AMapsMode_TitleAddition = Maps_Mode.empty() ? "" : Maps_Mode;

    if (!(Maps_Mode == "AMaps" || Maps_Mode == "WMaps")) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Maps_Mode = " + Maps_Mode + " is not defined!"); }

#pragma endregion

    SetBins(P_nuc_bin_profile, beamE);
    SetElectronBins(P_e_bin_profile, beamE);

    auto CreateMap = [&](const std::string& statsTitle, const std::string& title, const std::string& particleLabel, const std::string& savePath, const std::string& saveName, int xBins,
                         int yBins) {
        return hPlot2D(AMapsMode_TitleAddition, "", statsTitle, title, "#phi_{" + particleLabel + "} [#circ]", "#theta_{" + particleLabel + "} [#circ]", savePath, saveName, hBinLowerXLim,
                       hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, xBins, yBins);
    };

    auto CreateAndStoreMap = [&](const std::string& statsTitle, const std::string& title, const std::string& particleLabel, const std::string& savePath, const std::string& saveName,
                                 int xBins, int yBins, std::vector<hPlot2D>& destination, std::vector<TObject*>& rootHistograms) {
        hPlot2D generatedMap = CreateMap(statsTitle, title, particleLabel, savePath, saveName, xBins, yBins);
        destination.push_back(generatedMap);
        rootHistograms.push_back(generatedMap.GetHistogram2D());
    };

#pragma region /* Reco theta VS phi BC */
    bt::PrintMessage("[AMaps::AMaps (generator)] Setting reco. theta VS phi BC histograms...", false);

    if (hasElectronSample) {
        std::string hStatsTitleAMapBCElectron = "Electron_AMap_BC", hTitleAMapBCElectron = "Electron AMap BC", hSaveNameAMapBCElectron = "01_e_AMap_BC";
        reco_theta_e_VS_phi_e_BC =
            CreateMap(hStatsTitleAMapBCElectron, hTitleAMapBCElectron, eLabel, SavePathAMapsBC, hSaveNameAMapBCElectron, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
    }

    if (hasProtonSample) {
        std::string hStatsTitleAMapBCProton = "Proton_AMap_BC", hTitleAMapBCProton = "Proton AMap BC", hSaveNameAMapBCProton = "02_p_AMap_BC";
        reco_theta_p_VS_phi_p_BC = CreateMap(hStatsTitleAMapBCProton, hTitleAMapBCProton, pLabel, SavePathAMapsBC, hSaveNameAMapBCProton, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    }

    if (hasNeutronSample) {
        std::string hStatsTitleAMapBCNeutron = "Neutron_AMap_BC", hTitleAMapBCNeutron = "Neutron AMap BC", hSaveNameAMapBCNeutron = "03_n_AMap_BC";
        reco_theta_n_VS_phi_n_BC = CreateMap(hStatsTitleAMapBCNeutron, hTitleAMapBCNeutron, nLabel, SavePathAMapsBC, hSaveNameAMapBCNeutron, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    }

    // std::string hStatsTitleAMapBCNucleon = "Nucleon_AMap_BC", hTitleAMapBCNucleon = "Nucleon AMap BC", hSaveNameAMapBCNucleon = "04_nuc_AMap_BC";
    // reco_theta_nuc_VS_phi_nuc_BC = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapBCNucleon, hTitleAMapBCNucleon, "#phi_{nuc} [#circ]", "#theta_{nuc} [#circ]", SavePathAMapsBC,
    //                                        hSaveNameAMapBCNucleon, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    bt::PrintMessage("[AMaps::AMaps (generator)] done.", false);
#pragma endregion

#pragma region /* Setting electron histograms */
    bt::PrintMessage("[AMaps::AMaps (generator)] Setting electron histograms...", false);

    if (hasElectronSample) {
        for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
            double BinLowerLim = ElectronMomSliceLimits.at(i).at(0), BinUpperLim = ElectronMomSliceLimits.at(i).at(1);

            int BinUpperLimPrecision;
            if (BinUpperLim == beamE) {
                BinUpperLimPrecision = 3;
            } else {
                BinUpperLimPrecision = 2;
            }

            std::string BinDensity = " (" + std::to_string(HistElectronSliceNumOfXBins) + "x" + std::to_string(HistElectronSliceNumOfYBins) + ")";

#pragma region /* Setting electron Acceptance maps */

#pragma region /* Electron TL hit map */
            std::string hStatsTitleTLElectron = "TL P_{" + eLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + eLabel + "}#leq" +
                                                bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
            std::string hTitleTLElectron = "TL P_{" + eLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + eLabel + "}#leq" +
                                           bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
            std::string hSaveNameTLElectron =
                std::to_string(i + 1) + "_TL_P_bin_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            CreateAndStoreMap(hStatsTitleTLElectron, hTitleTLElectron, eLabel, AMapSavePathTLElectron, hSaveNameTLElectron, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins,
                              truth_theta_e_VS_phi_e_BySlice, truth_e_BySlice);
#pragma endregion

#pragma region /* Electron Reco. Acceptance maps */
            std::string hStatsTitleRecoElectron = "Reco P_{" + eLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + eLabel + "}#leq" +
                                                  bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
            std::string hTitleRecoElectron = "Reco P_{" + eLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + eLabel + "}#leq" +
                                             bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
            std::string hSaveNameRecoElectron =
                std::to_string(i + 1) + "_Reco_P_bin_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            CreateAndStoreMap(hStatsTitleRecoElectron, hTitleRecoElectron, eLabel, AMapSavePathRecoElectron, hSaveNameRecoElectron, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins,
                              reco_theta_e_VS_phi_e_BySlice, reco_e_BySlice);
#pragma endregion

#pragma region /* Electron Reco./TL Ratio */
            std::string hStatsTitleRecoToTLRatioElectron = "Electron Reco/TL ratio for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + eLabel + "}#leq" +
                                                           bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
            std::string hTitleRecoToTLRatioElectron = "Electron Reco/TL ratio for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + eLabel + "}#leq" +
                                                      bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
            std::string hSaveNameRecoToTLRatioElectron =
                std::to_string(i + 1) + "_e_Ratio_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            CreateAndStoreMap(hStatsTitleRecoToTLRatioElectron, hTitleRecoToTLRatioElectron, eLabel, AMapSavePathRecoToTLElectron, hSaveNameRecoToTLRatioElectron,
                              HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins, acceptance_eff_e_BySlice, acc_eff_e_BySlice);
#pragma endregion

#pragma region /* Electron separate AMaps */
            std::string hStatsTitleSepAMapsElectron = "Electron AMap for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + eLabel + "}#leq" +
                                                      bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                      bt::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
            std::string hTitleSepAMapsElectron = "Electron AMap for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + eLabel + "}#leq" +
                                                 bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                 bt::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
            std::string hSaveNameSepAMapsElectron =
                std::to_string(i + 1) + "_e_SepAMap_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
            CreateAndStoreMap(hStatsTitleSepAMapsElectron, hTitleSepAMapsElectron, eLabel, AMapSavePathSepAMapsElectron, hSaveNameSepAMapsElectron, HistElectronSliceNumOfXBins,
                              HistElectronSliceNumOfYBins, filtered_reco_theta_e_VS_phi_e_BySlice, filtered_reco_e_BySlice);
#pragma endregion

#pragma endregion
        }  // End of loop over slices of electron momentum
    }

    bt::PrintMessage("[AMaps::AMaps (generator)] done.", false);
#pragma endregion

#pragma region /* Setting nucleon histograms */
    bt::PrintMessage("[AMaps::AMaps (generator)] Setting nucleon histograms...", false);

#pragma region /* Setting nucleon slice histograms */
    if (hasProtonSample || hasNeutronSample) {
        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
            double BinLowerLim = NucleonMomSliceLimits.at(i).at(0), BinUpperLim = NucleonMomSliceLimits.at(i).at(1);

            int BinUpperLimPrecision;
            if (BinUpperLim == beamE) {
                BinUpperLimPrecision = 3;
            } else {
                BinUpperLimPrecision = 2;
            }

            std::string BinDensity = " (" + std::to_string(HistNucSliceNumOfXBins) + "x" + std::to_string(HistNucSliceNumOfYBins) + ")";

#pragma region /* Setting proton Acceptance maps */
            if (hasProtonSample) {
#pragma region /* Proton TL Acceptance maps */
                std::string hStatsTitleTLProton = "TL P_{" + pLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + pLabel + "}#leq" +
                                                  bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleTLProton = "TL P_{" + pLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + pLabel + "}#leq" +
                                             bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameTLProton =
                    std::to_string(i + 1) + "_TL_P_bin_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                CreateAndStoreMap(hStatsTitleTLProton, hTitleTLProton, pLabel, AMapSavePathTLProton, hSaveNameTLProton, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins,
                                  truth_theta_p_VS_phi_p_BySlice, truth_p_BySlice);
#pragma endregion

#pragma region /* Proton Reco. Acceptance maps */
                std::string hStatsTitleRecoProton = "Reco P_{" + pLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + pLabel + "}#leq" +
                                                    bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleRecoProton = "Reco P_{" + pLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + pLabel + "}#leq" +
                                               bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameRecoProton =
                    std::to_string(i + 1) + "_Reco_P_bin_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                CreateAndStoreMap(hStatsTitleRecoProton, hTitleRecoProton, pLabel, AMapSavePathRecoProton, hSaveNameRecoProton, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins,
                                  reco_theta_p_VS_phi_p_BySlice, reco_p_BySlice);
#pragma endregion

#pragma region /* Proton Reco./TL Ratio */
                std::string hStatsTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + pLabel + "}#leq" +
                                                             bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleRecoToTLRatioProton = "Proton Reco/TL ratio for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + pLabel + "}#leq" +
                                                        bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameRecoToTLRatioProton =
                    std::to_string(i + 1) + "_p_Ratio_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                CreateAndStoreMap(hStatsTitleRecoToTLRatioProton, hTitleRecoToTLRatioProton, pLabel, AMapSavePathRecoToTLProton, hSaveNameRecoToTLRatioProton, HistNucSliceNumOfXBins,
                                  HistNucSliceNumOfYBins, acceptance_eff_p_BySlice, acc_eff_p_BySlice);
#pragma endregion

#pragma region /* Proton separate AMaps */
                std::string hStatsTitleSepAMapsProton = "Proton AMap for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + pLabel + "}#leq" +
                                                        bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                        bt::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
                std::string hTitleSepAMapsProton = "Proton AMap for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + pLabel + "}#leq" +
                                                   bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                   bt::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
                std::string hSaveNameSepAMapsProton =
                    std::to_string(i + 1) + "_p_SepAMap_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                CreateAndStoreMap(hStatsTitleSepAMapsProton, hTitleSepAMapsProton, pLabel, AMapSavePathSepAMapsProton, hSaveNameSepAMapsProton, HistNucSliceNumOfXBins,
                                  HistNucSliceNumOfYBins, filtered_reco_theta_p_VS_phi_p_BySlice, filtered_reco_p_BySlice);
#pragma endregion
            }
#pragma endregion

#pragma region /* Setting neutron Acceptance maps */
            if (hasNeutronSample) {
#pragma region /* Neutron TL Acceptance maps */
                std::string hStatsTitleTLNeutron = "TL P_{" + nLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + nLabel + "}#leq" +
                                                   bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleTLNeutron = "TL P_{" + nLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + nLabel + "}#leq" +
                                              bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameTLNeutron =
                    std::to_string(i + 1) + "_TL_P_bin_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                CreateAndStoreMap(hStatsTitleTLNeutron, hTitleTLNeutron, nLabel, AMapSavePathTLNeutron, hSaveNameTLNeutron, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins,
                                  truth_theta_n_VS_phi_n_BySlice, truth_n_BySlice);
#pragma endregion

#pragma region /* Neutron TL Acceptance maps - const */
                CreateAndStoreMap(hStatsTitleTLNeutron + " const", hTitleTLNeutron, nLabel, AMapConstSavePathTLNeutron, hSaveNameTLNeutron, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins,
                                  truth_theta_n_VS_phi_n_ByConstSlice, truth_n_ByConstSlice);
#pragma endregion

#pragma region /* Neutron Reco. Acceptance maps */
                std::string hStatsTitleRecoNeutron = "Reco P_{" + nLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + nLabel + "}#leq" +
                                                     bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleRecoNeutron = "Reco P_{" + nLabel + "} bin for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + nLabel + "}#leq" +
                                                bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameRecoNeutron =
                    std::to_string(i + 1) + "_Reco_P_bin_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                CreateAndStoreMap(hStatsTitleRecoNeutron, hTitleRecoNeutron, nLabel, AMapSavePathRecoNeutron, hSaveNameRecoNeutron, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins,
                                  reco_theta_n_VS_phi_n_BySlice, reco_n_BySlice);
#pragma endregion

#pragma region /* Neutron Reco. Acceptance maps - const */
                CreateAndStoreMap(hStatsTitleRecoNeutron + " const", hTitleRecoNeutron, nLabel, AMapConstSavePathRecoNeutron, hSaveNameRecoNeutron, HistNucSliceNumOfXBins,
                                  HistNucSliceNumOfYBins, reco_theta_n_VS_phi_n_ByConstSlice, reco_n_ByConstSlice);
#pragma endregion

#pragma region /* Neutron Reco./TL Ratio */
                std::string hStatsTitleRecoToTLRatioNeutron = "Neutron Reco/TL ratio for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + nLabel + "}#leq" +
                                                              bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]";
                std::string hTitleRecoToTLRatioNeutron = "Neutron Reco/TL ratio for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + nLabel + "}#leq" +
                                                         bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c]" + BinDensity;
                std::string hSaveNameRecoToTLRatioNeutron =
                    std::to_string(i + 1) + "_p_Ratio_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                CreateAndStoreMap(hStatsTitleRecoToTLRatioNeutron, hTitleRecoToTLRatioNeutron, nLabel, AMapSavePathRecoToTLNeutron, hSaveNameRecoToTLRatioNeutron, HistNucSliceNumOfXBins,
                                  HistNucSliceNumOfYBins, acceptance_eff_n_BySlice, acc_eff_n_BySlice);
#pragma endregion

#pragma region /* Neutron Reco./TL Ratio - const */
                CreateAndStoreMap(hStatsTitleRecoToTLRatioNeutron + " const", hTitleRecoToTLRatioNeutron, nLabel, AMapConstSavePathRecoToTLNeutron, hSaveNameRecoToTLRatioNeutron,
                                  HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, acceptance_eff_n_ByConstSlice, acc_eff_n_ByConstSlice);
#pragma endregion

#pragma region /* Neutron separate AMaps */
                std::string hStatsTitleSepAMapsNeutron = "Neutron AMap for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + nLabel + "}#leq" +
                                                         bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                         bt::ToStringWithPrecision(Neutral_particle_min_Ratio, 2);
                std::string hTitleSepAMapsNeutron = "Neutron AMap for " + bt::ToStringWithPrecision(BinLowerLim, 2) + "#leqP^{truth}_{" + nLabel + "}#leq" +
                                                    bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision) + " [GeV/c] and (Reco./TL)#geq" +
                                                    bt::ToStringWithPrecision(Neutral_particle_min_Ratio, 2);
                std::string hSaveNameSepAMapsNeutron =
                    std::to_string(i + 1) + "_n_SepAMap_for_P_from_" + bt::ToStringWithPrecision(BinLowerLim, 2) + "_to_" + bt::ToStringWithPrecision(BinUpperLim, BinUpperLimPrecision);
                CreateAndStoreMap(hStatsTitleSepAMapsNeutron, hTitleSepAMapsNeutron, nLabel, AMapSavePathSepAMapsNeutron, hSaveNameSepAMapsNeutron, HistNucSliceNumOfXBins,
                                  HistNucSliceNumOfYBins, filtered_reco_theta_n_VS_phi_n_BySlice, filtered_reco_n_BySlice);
#pragma endregion

#pragma region /* Neutron separate AMaps - const */
                CreateAndStoreMap(hStatsTitleSepAMapsNeutron + " const", hTitleSepAMapsNeutron, nLabel, AMapConstSavePathSepAMapsNeutron, hSaveNameSepAMapsNeutron, HistNucSliceNumOfXBins,
                                  HistNucSliceNumOfYBins, filtered_reco_theta_n_VS_phi_n_ByConstSlice, filtered_reco_n_ByConstSlice);
#pragma endregion
            }
#pragma endregion
        }  // End of loop over slices of nucleon momentum
    }
#pragma endregion

#pragma region /* Setting neutron Acceptance maps */
    if (hasNeutronSample) {
#pragma region /* Neutron TL Acceptance maps */
        std::string hStatsTitleTLNeutron = "TL Neutron Hit Map", hTitleTLNeutron = "TL Neutron Hit Map", hSaveNameTLNeutron = "TL_n_AMap";
        truth_theta_n_VS_phi_n = CreateMap(hStatsTitleTLNeutron, hTitleTLNeutron, nLabel, AMapSavePathTLNeutron, hSaveNameTLNeutron, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
#pragma endregion

#pragma region /* Neutron Reco. Acceptance maps */
        std::string hStatsTitleRecoNeutron = "Reco Neutron Hit Map", hTitleRecoNeutron = "Reco Neutron Hit Map", hSaveNameRecoNeutron = "Reco_n_AMap";
        reco_theta_n_VS_phi_n = CreateMap(hStatsTitleRecoNeutron, hTitleRecoNeutron, nLabel, AMapSavePathRecoNeutron, hSaveNameRecoNeutron, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
#pragma endregion

#pragma region /* Neutron Reco./TL Ratio */
        std::string hStatsTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hTitleRecoToTLNeutron = "Neutron Reco/TL ratio", hSaveNameRecoToTLNeutron = "Neutron_Ratio";
        acceptance_eff_n =
            CreateMap(hStatsTitleRecoToTLNeutron, hTitleRecoToTLNeutron, nLabel, AMapSavePathRecoToTLNeutron, hSaveNameRecoToTLNeutron, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
#pragma endregion
    }
#pragma endregion

    bt::PrintMessage("[AMaps::AMaps (generator)] done.", false);
#pragma endregion

#pragma region /* Finalized acceptance maps */
    bt::PrintMessage("[AMaps::AMaps (generator)] Finalized acceptance maps...", false);

    if (hasElectronSample) {
        std::string hStatsTitleAMapElectron = "Electron_AMap";
        std::string hTitleAMapElectron = "Electron AMap for (Reco./TL)#geq" + bt::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
        std::string hSaveNameAMapElectron = "01_e_AMap";
        filtered_reco_theta_e_VS_phi_e =
            CreateMap(hStatsTitleAMapElectron, hTitleAMapElectron, eLabel, AMapSavePathAMap, hSaveNameAMapElectron, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins);
    }

    if (hasProtonSample) {
        std::string hStatsTitleAMapProton = "Proton_AMap";
        std::string hTitleAMapProton = "Proton AMap for (Reco./TL)#geq" + bt::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
        std::string hSaveNameAMapProton = "02_p_AMap";
        filtered_reco_theta_p_VS_phi_p = CreateMap(hStatsTitleAMapProton, hTitleAMapProton, pLabel, AMapSavePathAMap, hSaveNameAMapProton, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    }

    if (hasNeutronSample) {
        std::string hStatsTitleAMapNeutron = "Neutron_AMap";
        std::string hTitleAMapNeutron = "Neutron AMap for (Reco./TL)#geq" + bt::ToStringWithPrecision(Neutral_particle_min_Ratio, 2);
        std::string hSaveNameAMapNeutron = "03_n_AMap";
        filtered_reco_theta_n_VS_phi_n = CreateMap(hStatsTitleAMapNeutron, hTitleAMapNeutron, nLabel, AMapSavePathAMap, hSaveNameAMapNeutron, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);
    }

    // std::string hStatsTitleAMapNucleon = "Nucleon_AMap";
    // std::string hTitleAMapNucleon = "Nucleon AMap for (Reco./TL)_{"+nLabel+"}#geq" + bt::ToStringWithPrecision(Neutral_particle_min_Ratio, 2) + " and (Reco./TL)_{c}#geq" +
    //                                 bt::ToStringWithPrecision(Charged_particle_min_Ratio, 2);
    // std::string hSaveNameAMapNucleon = "04_nuc_AMap";
    // filtered_reco_theta_nuc_VS_phi_nuc = hPlot2D(AMapsMode_TitleAddition, "", hStatsTitleAMapNucleon, hTitleAMapNucleon, "#phi_{nuc} [#circ]", "#theta_{nuc} [#circ]", AMapSavePathAMap,
    //                                              hSaveNameAMapNucleon, hBinLowerXLim, hBinUpperXLim, hBinLowerYLim, hBinUpperYLim, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins);

    bt::PrintMessage("[AMaps::AMaps (generator)] done.");
#pragma endregion
}
#pragma endregion

#pragma region /* AMaps loading constructor */
/**
 * @brief Loading-mode constructor for the AMaps class.
 *
 * This constructor initializes the AMaps object by loading previously generated
 * acceptance maps (AMaps) or weight maps (WMaps) from disk. It reconstructs the
 * internal data structures needed to perform map matching and weight lookups
 * during analysis.
 *
 * Main responsibilities:
 *   1. Determine the correct source folders for the electron, proton, and neutron
 *      uniform Monte Carlo samples based on the provided beam energy.
 *   2. Load the momentum slice limits used during map generation for each particle
 *      type (electron and nucleon). These limits define the momentum ranges that
 *      correspond to each acceptance-map slice.
 *   3. Load the map matrices (either AMaps or WMaps) for each particle type
 *      depending on the selected operation mode.
 *   4. Store the loaded maps internally as 3D containers:
 *         slice -> theta bin -> phi bin
 *      which allows fast lookup during event matching.
 *   5. Determine the histogram binning (number of φ and θ bins) from the loaded
 *      maps to ensure consistency with the generated acceptance maps.
 *   6. Optionally configure single-slice testing mode, which allows debugging
 *      of map matching by restricting the lookup to specific slices.
 *
 * Operation modes:
 *   - "AMaps" : binary acceptance maps used for fiducial acceptance checks.
 *   - "WMaps" : weighted maps used to compute detector acceptance weights.
 *
 * Loaded data structures:
 *   - Electron slices: Loaded_e_AMap_Slices / Loaded_e_WMap_Slices
 *   - Proton slices:   Loaded_p_AMap_Slices / Loaded_p_WMap_Slices
 *   - Neutron slices:  Loaded_n_AMap_Slices / Loaded_n_WMap_Slices
 *   - Neutron constant slices (for neutral particle treatment)
 *
 * @param AcceptanceMapsDirectory Base directory containing the generated maps.
 * @param beamE                   Beam energy used during map generation.
 * @param AMapsMode               Mode of operation ("AMaps" or "WMaps").
 * @param Electron_single_slice_test  Enable debugging using a single electron slice.
 * @param Nucleon_single_slice_test   Enable debugging using a single nucleon slice.
 * @param TestSlices              Indices of slices used when test mode is enabled.
 * @param type                    ParticleType specifying which particle maps
 *                                should be loaded (electron, proton, neutron,
 *                                or undefined to load all).
 */
AMaps::AMaps(const std::string& AcceptanceMapsDirectory, const double& beamE, const std::string& AMapsMode, const bool& Electron_single_slice_test, const bool& Nucleon_single_slice_test,
             const std::vector<int>& TestSlices, ParticleType type)
    : particleType(type) {
    bool PrintOut = false;

    Maps_Mode = AMapsMode;

    bt::PrintMessage("[AMaps::AMaps (loader)] Setting uniform sample names...", false);

    std::string BeamE_str = bt::GetBeamEnergyFromDouble(beamE);
    std::string Electron_source_folder = "Uniform_1e_sample_" + BeamE_str;
    std::string Proton_source_folder = "Uniform_ep_sample_" + BeamE_str;
    std::string Neutron_source_folder = "Uniform_en_sample_" + BeamE_str;

    bt::PrintMessage("[AMaps::AMaps (loader)] done.", false);

    bt::PrintMessage("[AMaps::AMaps (loader)] Loading slices and their limits...", false);
    /* Load slices and their limits */
    const auto readSliceLimitsWithFallback = [&](const std::string& sampleFolder, const std::string& particleTag, std::vector<std::vector<double>>& destination) {
        const std::string basePath = AcceptanceMapsDirectory + sampleFolder + "/";
        std::vector<std::string> candidatePaths;

        if (Maps_Mode == "AMaps") {
            candidatePaths.push_back(basePath + particleTag + "_AMaps_by_slice/" + particleTag + "_slice_limits.par");
            candidatePaths.push_back(basePath + particleTag + "_AMap_by_slice/" + particleTag + "_slice_limits.par");
        } else if (Maps_Mode == "WMaps") {
            candidatePaths.push_back(basePath + particleTag + "_WMaps_by_slice/" + particleTag + "_slice_limits.par");
            candidatePaths.push_back(basePath + particleTag + "_WMap_by_slice/" + particleTag + "_slice_limits.par");
        }

        for (const auto& path : candidatePaths) {
            if (!bt::fileExists(path.c_str())) { continue; }
            ReadAMapLimits(path.c_str(), destination);
            return;
        }

        if (db::AMapsDebuggerMode) {
            std::cout << "\n\033[33mAMaps::AMaps warning: could not find slice limits for particle tag '" << particleTag << "' in:\033[0m\n";
            for (const auto& path : candidatePaths) { std::cout << "  - " << path << "\n"; }
        }
    };

    readSliceLimitsWithFallback(Electron_source_folder, "e", Loaded_ElectronMomSliceLimits);
    readSliceLimitsWithFallback(Proton_source_folder, "p", Loaded_NucleonMomSliceLimits);
    if (Loaded_NucleonMomSliceLimits.empty()) {
        // Some runs provide nucleon limits only under the neutron sample directory.
        readSliceLimitsWithFallback(Neutron_source_folder, "n", Loaded_NucleonMomSliceLimits);
    }

    // Safety check - debugging outputs for loaded slice limits (electrons):
    if (Loaded_ElectronMomSliceLimits.size() == 0) {
        bt::PrintWarning(__func__, __FILE__, __LINE__, "no electron slice limits were loaded from file!");
    } else if (db::AMapsDebuggerMode) {
        std::cout << "\n\n\033[35mLoaded Electron Slice Limits (" << Maps_Mode << "):\n\033[0m";
        for (const auto& limit_pair : Loaded_ElectronMomSliceLimits) { std::cout << "Lower: " << limit_pair.at(0) << " , Upper: " << limit_pair.at(1) << "\n"; }
    }

    // Safety check - debugging outputs for loaded slice limits (electrons):
    if (Loaded_NucleonMomSliceLimits.size() == 0) {
        bt::PrintWarning(__func__, __FILE__, __LINE__, "no nucleon slice limits were loaded from file!");
    } else if (db::AMapsDebuggerMode) {
        std::cout << "\n\n\033[35mLoaded Nucleon Slice Limits (" << Maps_Mode << "):\n\033[0m";
        for (const auto& limit_pair : Loaded_NucleonMomSliceLimits) { std::cout << "Lower: " << limit_pair.at(0) << " , Upper: " << limit_pair.at(1) << "\n"; }
    }

    bt::PrintMessage("[AMaps::AMaps (loader)] done.", false);

    bt::PrintMessage("[AMaps::AMaps (loader)] Loading separate maps...", false);
    /* Load separate maps */
    if (Maps_Mode == "AMaps") {
        if (particleType == ELECTRON_TYPE) {
            ReadMapSlices(Electron_source_folder, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, &Loaded_e_AMap_Slices);
            ReadMapSlices(Electron_source_folder, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, &Loaded_e_AMap_Slices_extended, nullptr, true);
        } else if (particleType == PROTON_TYPE) {
            ReadMapSlices(Proton_source_folder, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, &Loaded_p_AMap_Slices);
        } else if (particleType == NEUTRON_TYPE) {
            ReadMapSlices(Neutron_source_folder, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, &Loaded_n_AMap_Slices);
            ReadMapSlices(Neutron_source_folder, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, &Loaded_n_AMap_ConstSlices, nullptr, true);
        } else {
            // Load everything if particleType is UNDEFINED_TYPE
            // TODO: rethink this case!
            ReadMapSlices(Electron_source_folder, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, &Loaded_e_AMap_Slices);
            ReadMapSlices(Electron_source_folder, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, &Loaded_e_AMap_Slices_extended, nullptr, true);
            ReadMapSlices(Proton_source_folder, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, &Loaded_p_AMap_Slices);
            ReadMapSlices(Neutron_source_folder, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, &Loaded_n_AMap_Slices);
            ReadMapSlices(Neutron_source_folder, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, &Loaded_n_AMap_ConstSlices, nullptr, true);
        }
    } else {
        if (particleType == ELECTRON_TYPE) {
            ReadMapSlices(Electron_source_folder, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, nullptr, &Loaded_e_WMap_Slices);
        } else if (particleType == PROTON_TYPE) {
            ReadMapSlices(Proton_source_folder, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, nullptr, &Loaded_p_WMap_Slices);
        } else if (particleType == NEUTRON_TYPE) {
            ReadMapSlices(Neutron_source_folder, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, nullptr, &Loaded_n_WMap_Slices);
        } else {
            // Load everything if particleType is UNDEFINED_TYPE
            // TODO: rethink this case!
            ReadMapSlices(Electron_source_folder, AcceptanceMapsDirectory, "Electron", Loaded_ElectronMomSliceLimits, nullptr, &Loaded_e_WMap_Slices);
            ReadMapSlices(Proton_source_folder, AcceptanceMapsDirectory, "Proton", Loaded_NucleonMomSliceLimits, nullptr, &Loaded_p_WMap_Slices);
            ReadMapSlices(Neutron_source_folder, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, nullptr, &Loaded_n_WMap_Slices);
            ReadMapSlices(Neutron_source_folder, AcceptanceMapsDirectory, "Neutron", Loaded_NucleonMomSliceLimits, nullptr, &Loaded_n_WMap_ConstSlices, true);
        }
    }

    if (PrintOut) {
        if (Maps_Mode == "AMaps") {
            // Safety check - debugging outputs for loaded maps (electrons):
            if (Loaded_e_AMap_Slices.size() == 0) {
                bt::PrintWarning(__func__, __FILE__, __LINE__, "no electron maps were loaded from file!");
            } else if (db::AMapsDebuggerMode) {
                std::cout << "\n\n\033[35mLoaded electron map size (" << Maps_Mode << "):\n\033[0m";
                for (const auto& slice : Loaded_e_AMap_Slices) { std::cout << "BinX: " << slice.at(0).size() << " , BinY: " << slice.size() << "\n"; }
            }

            // Safety check - debugging outputs for loaded extended maps (electrons):
            if (Loaded_e_AMap_Slices_extended.size() == 0) {
                bt::PrintWarning(__func__, __FILE__, __LINE__, "no extended electron maps were loaded from file!");
            } else if (db::AMapsDebuggerMode) {
                std::cout << "\n\n\033[35mLoaded extended electron map size (" << Maps_Mode << "):\n\033[0m";
                for (const auto& slice : Loaded_e_AMap_Slices_extended) { std::cout << "BinX: " << slice.at(0).size() << " , BinY: " << slice.size() << "\n"; }
            }

            // Safety check - debugging outputs for loaded maps (protons):
            if (Loaded_p_AMap_Slices.size() == 0) {
                bt::PrintWarning(__func__, __FILE__, __LINE__, "no proton maps were loaded from file!");
            } else if (db::AMapsDebuggerMode) {
                std::cout << "\n\n\033[35mLoaded proton map size (" << Maps_Mode << "):\n\033[0m";
                for (const auto& slice : Loaded_p_AMap_Slices) { std::cout << "BinX: " << slice.at(0).size() << " , BinY: " << slice.size() << "\n"; }
            }

            // Safety check - debugging outputs for loaded maps (neutron):
            if (Loaded_n_AMap_Slices.size() == 0) {
                bt::PrintWarning(__func__, __FILE__, __LINE__, "no neutron maps were loaded from file!");
            } else if (db::AMapsDebuggerMode) {
                std::cout << "\n\n\033[35mLoaded neutron map size (" << Maps_Mode << "):\n\033[0m";
                for (const auto& slice : Loaded_n_AMap_Slices) { std::cout << "BinX: " << slice.at(0).size() << " , BinY: " << slice.size() << "\n"; }
            }

            // Safety check - debugging outputs for loaded ConstSlices maps (neutron):
            if (Loaded_n_AMap_ConstSlices.size() == 0) {
                bt::PrintWarning(__func__, __FILE__, __LINE__, "no ConstSlices neutron maps were loaded from file!");
            } else if (db::AMapsDebuggerMode) {
                std::cout << "\n\n\033[35mLoaded ConstSlices neutron map size (" << Maps_Mode << "):\n\033[0m";
                for (const auto& slice : Loaded_n_AMap_ConstSlices) { std::cout << "BinX: " << slice.at(0).size() << " , BinY: " << slice.size() << "\n"; }
            }
        } else if (Maps_Mode == "WMaps") {
            // Safety check - debugging outputs for loaded maps (electrons):
            if (Loaded_e_WMap_Slices.size() == 0) {
                bt::PrintWarning(__func__, __FILE__, __LINE__, "no electron maps were loaded from file!");
            } else if (db::AMapsDebuggerMode) {
                std::cout << "\n\n\033[35mLoaded electron map size (" << Maps_Mode << "):\n\033[0m";
                for (const auto& slice : Loaded_e_WMap_Slices) { std::cout << "BinX: " << slice.at(0).size() << " , BinY: " << slice.size() << "\n"; }
            }

            // Safety check - debugging outputs for loaded maps (protons):
            if (Loaded_p_WMap_Slices.size() == 0) {
                bt::PrintWarning(__func__, __FILE__, __LINE__, "no proton maps were loaded from file!");
            } else if (db::AMapsDebuggerMode) {
                std::cout << "\n\n\033[35mLoaded proton map size (" << Maps_Mode << "):\n\033[0m";
                for (const auto& slice : Loaded_p_WMap_Slices) { std::cout << "BinX: " << slice.at(0).size() << " , BinY: " << slice.size() << "\n"; }
            }

            // Safety check - debugging outputs for loaded maps (neutron):
            if (Loaded_n_WMap_Slices.size() == 0) {
                bt::PrintWarning(__func__, __FILE__, __LINE__, "no neutron maps were loaded from file!");
            } else if (db::AMapsDebuggerMode) {
                std::cout << "\n\n\033[35mLoaded neutron map size (" << Maps_Mode << "):\n\033[0m";
                for (const auto& slice : Loaded_n_WMap_Slices) { std::cout << "BinX: " << slice.at(0).size() << " , BinY: " << slice.size() << "\n"; }
            }

            // Safety check - debugging outputs for loaded ConstSlices maps (neutron):
            if (Loaded_n_WMap_ConstSlices.size() == 0) {
                bt::PrintWarning(__func__, __FILE__, __LINE__, "no ConstSlices neutron maps were loaded from file!");
            } else if (db::AMapsDebuggerMode) {
                std::cout << "\n\n\033[35mLoaded ConstSlices neutron map size (" << Maps_Mode << "):\n\033[0m";
                for (const auto& slice : Loaded_n_WMap_ConstSlices) { std::cout << "BinX: " << slice.at(0).size() << " , BinY: " << slice.size() << "\n"; }
            }
        }
    }

    bt::PrintMessage("[AMaps::AMaps (loader)] done.", false);

    if (Maps_Mode == "AMaps" && !Loaded_e_AMap_Slices.empty() && !Loaded_e_AMap_Slices.at(0).empty() && !Loaded_e_AMap_Slices.at(0).at(0).empty()) {
        HistElectronSliceNumOfXBins = Loaded_e_AMap_Slices.at(0).at(0).size();
        HistElectronSliceNumOfYBins = Loaded_e_AMap_Slices.at(0).size();
    } else if (Maps_Mode == "WMaps" && !Loaded_e_WMap_Slices.empty() && !Loaded_e_WMap_Slices.at(0).empty() && !Loaded_e_WMap_Slices.at(0).at(0).empty()) {
        HistElectronSliceNumOfXBins = Loaded_e_WMap_Slices.at(0).at(0).size();
        HistElectronSliceNumOfYBins = Loaded_e_WMap_Slices.at(0).size();
    } else {
        bt::PrintWarning(__func__, __FILE__, __LINE__, "electron map slices were not loaded (or are empty). Using defaults.");
        HistElectronSliceNumOfXBins = 100;
        HistElectronSliceNumOfYBins = 100;
    }

    // Nucleon binning: only read nucleon map dimensions if those maps were actually loaded.
    // In electron-only mode, neutron/proton slices are not loaded, and indexing would segfault.
    if (Maps_Mode == "AMaps" && !Loaded_n_AMap_ConstSlices.empty() && !Loaded_n_AMap_ConstSlices.at(0).empty() && !Loaded_n_AMap_ConstSlices.at(0).at(0).empty()) {
        HistNucSliceNumOfXBins = Loaded_n_AMap_ConstSlices.at(0).at(0).size();
        HistNucSliceNumOfYBins = Loaded_n_AMap_ConstSlices.at(0).size();
    } else if (Maps_Mode == "AMaps" && !Loaded_p_AMap_Slices.empty() && !Loaded_p_AMap_Slices.at(0).empty() && !Loaded_p_AMap_Slices.at(0).at(0).empty()) {
        HistNucSliceNumOfXBins = Loaded_p_AMap_Slices.at(0).at(0).size();
        HistNucSliceNumOfYBins = Loaded_p_AMap_Slices.at(0).size();
    } else if (Maps_Mode == "AMaps" && !Loaded_n_AMap_Slices.empty() && !Loaded_n_AMap_Slices.at(0).empty() && !Loaded_n_AMap_Slices.at(0).at(0).empty()) {
        HistNucSliceNumOfXBins = Loaded_n_AMap_Slices.at(0).at(0).size();
        HistNucSliceNumOfYBins = Loaded_n_AMap_Slices.at(0).size();
    } else if (Maps_Mode == "WMaps" && !Loaded_p_WMap_Slices.empty() && !Loaded_p_WMap_Slices.at(0).empty() && !Loaded_p_WMap_Slices.at(0).at(0).empty()) {
        HistNucSliceNumOfXBins = Loaded_p_WMap_Slices.at(0).at(0).size();
        HistNucSliceNumOfYBins = Loaded_p_WMap_Slices.at(0).size();
    } else if (Maps_Mode == "WMaps" && !Loaded_n_WMap_Slices.empty() && !Loaded_n_WMap_Slices.at(0).empty() && !Loaded_n_WMap_Slices.at(0).at(0).empty()) {
        HistNucSliceNumOfXBins = Loaded_n_WMap_Slices.at(0).at(0).size();
        HistNucSliceNumOfYBins = Loaded_n_WMap_Slices.at(0).size();
    } else {
        // Fall back to the historic defaults used throughout the project.
        HistNucSliceNumOfXBins = 75;
        HistNucSliceNumOfYBins = 75;
    }

    e_single_slice_test = Electron_single_slice_test, nuc_single_slice_test = Nucleon_single_slice_test;
    Slices2Test = TestSlices;

    if (particleType == ELECTRON_TYPE && Loaded_ElectronMomSliceLimits.size() == 0) { bt::PrintWarning(__func__, __FILE__, __LINE__, "no electron slice limits were loaded!"); }

    if ((particleType == PROTON_TYPE || particleType == NEUTRON_TYPE) && Loaded_NucleonMomSliceLimits.size() == 0) {
        bt::PrintWarning(__func__, __FILE__, __LINE__, "no nucleon slice limits were loaded!");
    }
}
#pragma endregion

// SetBins functions ----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetBins function (G1a) */
/**
 * @brief Configure momentum slice limits for nucleons (protons and neutrons).
 *
 * This function defines the momentum binning used to generate nucleon
 * acceptance maps. The binning determines how the nucleon momentum phase
 * space is divided into slices, where each slice corresponds to a separate
 * θ–φ acceptance map.
 *
 * The binning strategy is selected using the `P_nuc_bin_profile` argument:
 *
 *  - "equi_inverted_P_nuc"
 *      Creates bins that are uniform in inverse momentum (1/P). The limits
 *      are first generated in the inverted space and then transformed back
 *      to regular momentum space. This produces finer resolution at low
 *      momenta and wider bins at higher momenta.
 *
 *  - "varying_P_nuc_bins"
 *      Uses a predefined set of custom momentum slice limits optimized for
 *      specific simulations (e.g., C12x4_simulation_G18_Q204_6GeV).
 *
 *  - "uniform_P_nuc_bins"
 *      Generates evenly spaced momentum bins between the nucleon momentum
 *      threshold (`Nucleon_Momentum_Slice_Th`) and the beam energy.
 *
 * The resulting slice limits are stored in `NucleonMomSliceLimits`, which is
 * later used when filling truth and reconstructed histograms to determine
 * which momentum slice a nucleon belongs to.
 *
 * @param P_nuc_bin_profile  String selecting the binning strategy.
 * @param beamE              Beam energy used to define the upper momentum limit.
 */
void AMaps::SetBins(const std::string& P_nuc_bin_profile, double beamE) {
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

        if (InvertedPrintOut && !RegPrintOut) { exit(1); }

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

        if (RegPrintOut) { exit(1); }
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

            exit(1);
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

        if (RegPrintOut) { exit(1); }
    } else {
        bt::ExitWithError(__func__, __FILE__, __LINE__,
                          "No valid P_nuc_bin_profile selected! Choose between:\n"
                          "equi_inverted_P_nuc , varying_P_nuc_bins , uniform_P_nuc_bins");
    }
}
#pragma endregion

#pragma region /* SetElectronBins function (G1b) */
/**
 * @brief Configure momentum slice limits for electrons.
 *
 * This function defines how the electron momentum phase space is divided
 * into slices. Each slice corresponds to a separate θ–φ acceptance map
 * used later when filling truth-level and reconstructed histograms.
 *
 * The binning strategy is selected using the `P_e_bin_profile` argument.
 * Different profiles are provided to support various simulation setups
 * and analysis needs.
 *
 * Available profiles:
 *
 *  - "reformat_e_bins"
 *      Generates bins in inverse momentum space (1/P) and then converts
 *      them back to regular momentum space. The algorithm first creates
 *      a large bin near the upper momentum region and then iteratively
 *      subdivides the remaining region with smaller steps. This produces
 *      finer resolution where it is most needed for the analysis.
 *
 *  - "varying_P_e_bins"
 *      Uses predefined custom bin limits optimized for specific
 *      simulations. The exact set of limits depends on the sample name
 *      (`SName`). Older and newer simulation campaigns use different
 *      predefined arrays.
 *
 *  - "uniform_P_e_bins"
 *      Creates evenly spaced momentum bins between 0 and the beam
 *      energy. This is the simplest binning scheme and produces
 *      constant-width momentum slices.
 *
 *  - "equi_inverted_P_e"
 *      Creates bins uniformly in inverse momentum (1/P). The limits are
 *      generated in inverted space and then transformed back to regular
 *      momentum coordinates. This provides finer binning at high
 *      electron energies where detector acceptance may vary rapidly.
 *
 * The resulting slice limits are stored in `ElectronMomSliceLimits`,
 * which is later used by the map-filling routines to determine which
 * acceptance-map slice a given electron belongs to.
 *
 * @param P_e_bin_profile  String selecting the electron momentum
 *                         binning strategy.
 * @param beamE            Beam energy used to define the upper
 *                         momentum boundary.
 */
void AMaps::SetElectronBins(const std::string& P_e_bin_profile, double beamE) {
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

        if (InvertedPrintOut && !RegPrintOut) { exit(1); }

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

        if (RegPrintOut) { exit(1); }
    } else if (P_e_bin_profile == "varying_P_e_bins") {
        if (bt::FindSubstring(SName, "C12_simulation_6GeV_T5")) {  // Old sample
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

        if (RegPrintOut) { exit(1); }
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

        if (RegPrintOut) { exit(1); }
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

        if (InvertedPrintOut && !RegPrintOut) { exit(1); }

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

        if (RegPrintOut) { exit(1); }
    } else {
        bt::ExitWithError(__func__, __FILE__, __LINE__,
                          "No valid P_e_bin_profile selected! Choose between:\n"
                          "reformat_e_bins , varying_P_e_bins , uniform_P_e_bins, equi_inverted_P_e");
    }
}
#pragma endregion

// Functions to check particle species  ---------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Functions to check particle species */

/**
 * @brief Helper functions for identifying particle species from string labels.
 *
 * These utility functions provide a consistent way to determine the particle
 * type associated with a given string label (usually coming from configuration
 * files, sample names, or analysis inputs).
 *
 * The core helper `IsOneOf` checks whether a string matches any value from a
 * provided list. The particle‑specific wrappers (`isElectron`, `isProton`,
 * `isNeutron`) use this helper to test common naming variations for each
 * particle type.
 *
 * Purpose:
 *   - Avoid repeated string comparison logic throughout the code.
 *   - Centralize particle-type detection in one place.
 *   - Allow multiple naming conventions (e.g. "Electron" vs "electron").
 *
 * Functions:
 *   - IsOneOf(...)   : Generic string matcher against a list of allowed values.
 *   - isElectron(...) : Returns true if the sample corresponds to an electron.
 *   - isProton(...)   : Returns true if the sample corresponds to a proton.
 *   - isNeutron(...)  : Returns true if the sample corresponds to a neutron.
 */

bool AMaps::IsOneOf(const std::string& value, std::initializer_list<const char*> allowedValues) {
    return std::any_of(allowedValues.begin(), allowedValues.end(), [&value](const char* allowedValue) { return value == allowedValue; });
}

bool AMaps::isElectron(const std::string& SampleType) { return IsOneOf(SampleType, {"Electron", "electron"}); }

bool AMaps::isProton(const std::string& SampleType) { return IsOneOf(SampleType, {"Proton", "proton"}); }

bool AMaps::isNeutron(const std::string& SampleType) { return IsOneOf(SampleType, {"Neutron", "neutron"}); }
#pragma endregion

// Functions to check particle level  -----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Functions to check particle level */
/**
 * @brief Helper functions for identifying the reconstruction level of an event sample.
 *
 * These utilities determine whether a given sample label corresponds to
 * truth‑level (generator level) information or reconstructed detector-level
 * information.
 *
 * The functions rely on the generic `IsOneOf` helper, which compares the
 * provided string against a list of accepted naming variations. This allows
 * the analysis code to remain robust against different naming conventions
 * that may appear in configuration files, sample labels, or external inputs.
 *
 * Functions:
 *   - isTL(...)
 *       Returns true if the sample corresponds to truth-level information
 *       (generator-level particles before detector reconstruction).
 *
 *   - isReco(...)
 *       Returns true if the sample corresponds to reconstructed detector
 *       quantities obtained after the reconstruction chain.
 *
 * Typical usage:
 *   These helpers are used in routines such as `hFillMaps()` to decide
 *   whether a particle should be filled into truth-level histograms
 *   (used to compute detector acceptance) or reconstructed histograms
 *   (used to evaluate reconstruction efficiency).
 */
bool AMaps::isTL(const std::string& SampleType) { return IsOneOf(SampleType, {"Truth", "truth", "TL", "truth level", "truth-level", "Truth-Level"}); }
bool AMaps::isReco(const std::string& SampleType) { return IsOneOf(SampleType, {"reco", "Reco", "Reconstruction"}); }
#pragma endregion

// hFillMaps function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* hFillMaps function (G2) */
/**
 * @brief Fill θ–φ acceptance histograms for a given particle and sample level.
 *
 * This function is responsible for filling the internal histograms used to
 * construct detector acceptance maps (AMaps) and weighted maps (WMaps).
 * Each call processes a single particle entry and determines:
 *
 *   1. The particle species (electron, proton, or neutron).
 *   2. The data level (truth-level or reconstructed).
 *   3. The correct momentum slice associated with the particle momentum.
 *
 * Based on these properties, the function fills the corresponding θ–φ
 * histograms that are later used to compute acceptance efficiencies.
 *
 * Workflow:
 *   - Identify the particle type using helper functions
 *     (`isElectron`, `isProton`, `isNeutron`).
 *   - Determine whether the entry corresponds to truth-level (TL)
 *     or reconstructed (Reco) information using `isTL` and `isReco`.
 *   - Validate that the particle type and level are mutually exclusive
 *     using safety checks.
 *   - Find the correct momentum slice using the pre-defined slice limits
 *     (`ElectronMomSliceLimits` or `NucleonMomSliceLimits`).
 *   - Fill the corresponding θ–φ histograms for that slice.
 *
 * Particle-specific behavior:
 *
 *   Electrons / Protons (charged particles):
 *     - The particle momentum determines a unique slice.
 *     - Only the histogram corresponding to that slice is filled.
 *
 *   Neutrons (neutral particles):
 *     - Since neutron acceptance does not depend strongly on momentum
 *       slicing in the same way as charged particles, the event is also
 *       filled into "constant slices" used for neutron map generation.
 *
 * Histogram categories filled:
 *
 *   Truth-level histograms:
 *     - Used as denominators when computing detector acceptance.
 *
 *   Reconstructed histograms:
 *     - Used as numerators for acceptance efficiency calculations.
 *
 *   Filtered reconstructed histograms:
 *     - Used later to generate filtered maps that suppress regions with
 *       poor reconstruction efficiency.
 *
 * @param SampleType  Indicates whether the particle belongs to
 *                    truth-level or reconstructed data.
 * @param particle    Particle species label ("Electron", "Proton", "Neutron").
 * @param Momentum    Particle momentum used to determine the slice.
 * @param Theta       Polar angle used for θ–φ map filling.
 * @param Phi         Azimuthal angle used for θ–φ map filling.
 * @param Weight      Event weight applied during histogram filling.
 */
void AMaps::hFillMaps(const std::string& SampleType, const std::string& particle, double Momentum, double Theta, double Phi, double Weight) {
    bool is_e = isElectron(particle), is_p = isProton(particle), is_n = isNeutron(particle);
    bool is_TL = isTL(SampleType), is_Reco = isReco(SampleType);
    const bool hasElectronSample = bt::FindSubstring(SName, "Uniform_1e_sample_");
    const bool hasProtonSample = bt::FindSubstring(SName, "Uniform_ep_sample_");
    const bool hasNeutronSample = bt::FindSubstring(SName, "Uniform_en_sample_");

    bool TL_e_PrintOut = false, TL_p_PrintOut = false, TL_n_PrintOut = false;
    bool Reco_e_PrintOut = false, Reco_p_PrintOut = false, Reco_n_PrintOut = false;

#pragma region /* Safety checks (AMaps::hFillMaps) */
    if (is_e && is_p && is_n) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Particle can't be all particles!"); }
    if (!is_e && !is_p && !is_n) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Particle must be an electron, proton or neutron!"); }
    if (is_e && is_p) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Particle can't be both electrons and protons!"); }
    if (is_e && is_n) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Particle can't be both electrons and neutrons!"); }
    if (is_p && is_n) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Particle can't be both protons and neutrons!"); }

    if (is_TL && is_Reco) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Particle can't be both TL and Reco!"); }
    if (!is_TL && !is_Reco) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Particle must be either TL or Reco!"); }
#pragma endregion

    auto PrintSliceDebug = [&](bool enabled, const char* limitsName, int sliceIndex, double lower, double upper, const char* particleFlagName, bool particleFlagValue) {
        if (Maps_Mode == "AMaps" && enabled) {
            std::cout << "\n";
            std::cout << limitsName << ".at(" << sliceIndex << ").at(0) = " << lower << "\n";
            std::cout << limitsName << ".at(" << sliceIndex << ").at(1) = " << upper << "\n";
            std::cout << "particle = " << particle << "\n";
            std::cout << particleFlagName << " = " << particleFlagValue << "\n";
            std::cout << "is_TL = " << is_TL << "\n";
            std::cout << "is_Reco = " << is_Reco << "\n";
            std::cout << "Momentum = " << Momentum << "\n";
            std::cout << "Theta = " << Theta << "\n";
            std::cout << "Phi = " << Phi << "\n";
        }
    };

    auto FillFirstMatchingSlice = [&](const std::vector<std::vector<double>>& limits, const char* invalidLimitsErrorMessage, auto&& fillOnMatch, auto&& debugOnMatch) {
        for (int i = 0; i < limits.size(); ++i) {
            const double lower = limits.at(i).at(0);
            const double upper = limits.at(i).at(1);

            if ((Momentum >= lower) && (Momentum < upper)) {
                fillOnMatch(i);
                debugOnMatch(i, lower, upper);

                if (lower > upper) { bt::ExitWithError(__func__, __FILE__, __LINE__, invalidLimitsErrorMessage); }

                break;  // no need to keep the loop going after filling histogram
            }
        }
    };

    if (is_TL) {
        if (is_e && hasElectronSample) {  // electrons are charged -> look for correct momentum slice!
            FillFirstMatchingSlice(
                ElectronMomSliceLimits, "Electron momentum slice limits were set incorrectly!", [&](int i) { truth_theta_e_VS_phi_e_BySlice.at(i).hFill(Phi, Theta, Weight); },
                [&](int i, double lower, double upper) { PrintSliceDebug(TL_e_PrintOut, "ElectronMomSliceLimits", i, lower, upper, "is_e", is_e); });
        } else if (is_p && hasProtonSample) {  // protons are charged -> look for correct momentum slice!
            FillFirstMatchingSlice(
                NucleonMomSliceLimits, "Nucleon momentum slice limits were set incorrectly!", [&](int i) { truth_theta_p_VS_phi_p_BySlice.at(i).hFill(Phi, Theta, Weight); },
                [&](int i, double lower, double upper) { PrintSliceDebug(TL_p_PrintOut, "NucleonMomSliceLimits", i, lower, upper, "is_p", is_p); });
        } else if (is_n && hasNeutronSample) {  // neutrons are neutral -> same fill all slices!
            truth_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);

            for (int Bin = 0; Bin < NucleonMomSliceLimits.size(); Bin++) { truth_theta_n_VS_phi_n_ByConstSlice.at(Bin).hFill(Phi, Theta, Weight); }

            FillFirstMatchingSlice(
                NucleonMomSliceLimits, "Nucleon momentum slice limits were set incorrectly!", [&](int i) { truth_theta_n_VS_phi_n_BySlice.at(i).hFill(Phi, Theta, Weight); },
                [&](int i, double lower, double upper) { PrintSliceDebug(TL_n_PrintOut, "NucleonMomSliceLimits", i, lower, upper, "is_n", is_n); });

            /*
            // truth_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);

            // for (int Bin = 0; Bin < NucleonMomSliceLimits.size(); Bin++) { truth_theta_n_VS_phi_n_BySlice.at(Bin).hFill(Phi, Theta, Weight); }
            */
        }
    } else if (is_Reco) {
        if (is_e && hasElectronSample) {  // electrons are charged -> look for correct momentum slice!
            reco_theta_e_VS_phi_e_BC.hFill(Phi, Theta, Weight);

            FillFirstMatchingSlice(
                ElectronMomSliceLimits, "Electron momentum slice limits were set incorrectly!",
                [&](int i) {
                    reco_theta_e_VS_phi_e_BySlice.at(i).hFill(Phi, Theta, Weight);
                    acceptance_eff_e_BySlice.at(i).hFill(Phi, Theta, Weight);
                    filtered_reco_theta_e_VS_phi_e_BySlice.at(i).hFill(Phi, Theta, Weight);
                },
                [&](int i, double lower, double upper) {
                    // Keep historic debug output as-is ("is_p" in the electron reco block).
                    PrintSliceDebug(Reco_e_PrintOut, "ElectronMomSliceLimits", i, lower, upper, "is_p", is_p);
                });
        } else if (is_p && hasProtonSample) {  // protons are charged -> look for correct momentum slice!
            reco_theta_p_VS_phi_p_BC.hFill(Phi, Theta, Weight);

            FillFirstMatchingSlice(
                NucleonMomSliceLimits, "Nucleon momentum slice limits were set incorrectly!",
                [&](int i) {
                    reco_theta_p_VS_phi_p_BySlice.at(i).hFill(Phi, Theta, Weight);
                    acceptance_eff_p_BySlice.at(i).hFill(Phi, Theta, Weight);
                    filtered_reco_theta_p_VS_phi_p_BySlice.at(i).hFill(Phi, Theta, Weight);
                },
                [&](int i, double lower, double upper) { PrintSliceDebug(Reco_p_PrintOut, "NucleonMomSliceLimits", i, lower, upper, "is_p", is_p); });
        } else if (is_n && hasNeutronSample) {  // neutrons are neutral -> same fill all slices!
            reco_theta_n_VS_phi_n_BC.hFill(Phi, Theta, Weight);

            reco_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);
            acceptance_eff_n.hFill(Phi, Theta, Weight);
            filtered_reco_theta_n_VS_phi_n.hFill(Phi, Theta, Weight);

            for (int Bin = 0; Bin < NucleonMomSliceLimits.size(); Bin++) {
                reco_theta_n_VS_phi_n_ByConstSlice.at(Bin).hFill(Phi, Theta, Weight);
                acceptance_eff_n_ByConstSlice.at(Bin).hFill(Phi, Theta, Weight);
                filtered_reco_theta_n_VS_phi_n_ByConstSlice.at(Bin).hFill(Phi, Theta, Weight);
            }

            FillFirstMatchingSlice(
                NucleonMomSliceLimits, "Nucleon momentum slice limits were set incorrectly!",
                [&](int i) {
                    reco_theta_n_VS_phi_n_BySlice.at(i).hFill(Phi, Theta, Weight);
                    acceptance_eff_n_BySlice.at(i).hFill(Phi, Theta, Weight);
                    filtered_reco_theta_n_VS_phi_n_BySlice.at(i).hFill(Phi, Theta, Weight);
                },
                [&](int i, double lower, double upper) { PrintSliceDebug(Reco_n_PrintOut, "NucleonMomSliceLimits", i, lower, upper, "is_n", is_n); });

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

// CalcAcceptanceEfficiency function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* CalcAcceptanceEfficiency function (G3a) */
/**
 * @brief Compute detector acceptance efficiency maps from truth and reconstructed histograms.
 *
 * This function calculates the acceptance efficiency for each particle type
 * (electrons, protons, and neutrons) by dividing reconstructed θ–φ histograms
 * by their corresponding truth-level histograms.
 *
 * Conceptually, the acceptance efficiency in each bin is defined as:
 *
 *      Efficiency(θ, φ) = Reco(θ, φ) / Truth(θ, φ)
 *
 * where:
 *   - Truth histograms represent the generated particle distribution
 *     (denominator).
 *   - Reconstructed histograms represent the particles successfully
 *     reconstructed by the detector (numerator).
 *
 * The calculation is performed independently for each momentum slice
 * defined in:
 *   - `ElectronMomSliceLimits` for electrons
 *   - `NucleonMomSliceLimits` for protons and neutrons
 *
 * Particle handling:
 *   - Electrons: efficiency computed for every electron momentum slice.
 *   - Protons:  efficiency computed for each nucleon momentum slice.
 *   - Neutrons: efficiency computed for:
 *        • a global map (all neutrons)
 *        • momentum-sliced maps
 *        • constant-slice maps used for neutron studies.
 *
 * Additional processing:
 *   When running in "AMaps" mode, the efficiency values are capped using
 *   `ApplyZMaxLim(1.2)` to suppress unphysical fluctuations above unity
 *   that may arise from statistical effects.
 *
 * Execution is controlled by the sample type encoded in `SName`:
 *   - "Uniform_1e_sample_" → electron maps
 *   - "Uniform_ep_sample_" → proton maps
 *   - "Uniform_en_sample_" → neutron maps
 *
 * The resulting efficiency histograms are later used to generate
 * acceptance maps (AMaps) and weighted maps (WMaps).
 */
void AMaps::CalcAcceptanceEfficiency() {
    // Calculate electron acceptance efficiency
    if (bt::FindSubstring(SName, "Uniform_1e_sample_")) {  // Old sample
        std::cout << "\n\nCalculating electron acceptance efficiency...";

        for (int i = 0; i < ElectronMomSliceLimits.size(); i++) {
            acceptance_eff_e_BySlice.at(i).hDivision(truth_theta_e_VS_phi_e_BySlice.at(i).GetHistogram2D());

            if (Maps_Mode == "AMaps") { acceptance_eff_e_BySlice.at(i).ApplyZMaxLim(1.2); }
        }

        std::cout << " done!\n";
    }

    // Calculate proton acceptance efficiency
    if (bt::FindSubstring(SName, "Uniform_ep_sample_")) {
        std::cout << "\n\nCalculating proton acceptance efficiency...";

        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
            acceptance_eff_p_BySlice.at(i).hDivision(truth_theta_p_VS_phi_p_BySlice.at(i).GetHistogram2D());

            if (Maps_Mode == "AMaps") { acceptance_eff_p_BySlice.at(i).ApplyZMaxLim(1.2); }
        }

        std::cout << " done!\n";
    }

    // Calculate neutron acceptance efficiency
    if (bt::FindSubstring(SName, "Uniform_en_sample_")) {
        std::cout << "\n\nCalculating neutron acceptance efficiency...";
        acceptance_eff_n.hDivision(truth_theta_n_VS_phi_n.GetHistogram2D());

        // Regular slices
        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
            acceptance_eff_n_BySlice.at(i).hDivision(truth_theta_n_VS_phi_n_BySlice.at(i).GetHistogram2D());

            if (Maps_Mode == "AMaps") { acceptance_eff_n_BySlice.at(i).ApplyZMaxLim(1.2); }
        }

        // Constant slices
        for (int i = 0; i < NucleonMomSliceLimits.size(); i++) {
            acceptance_eff_n_ByConstSlice.at(i).hDivision(truth_theta_n_VS_phi_n_ByConstSlice.at(i).GetHistogram2D());

            if (Maps_Mode == "AMaps") { acceptance_eff_n_ByConstSlice.at(i).ApplyZMaxLim(1.2); }
        }

        std::cout << " done!\n";
    }
}
#pragma endregion

// GenerateFilteredRecoMaps function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GenerateFilteredRecoMaps function (G3b) */
/**
 * @brief Apply acceptance-efficiency filtering to reconstructed θ–φ maps.
 *
 * This function generates filtered reconstructed histograms by removing
 * detector regions with low acceptance efficiency. For each particle type
 * and momentum slice, the function scans the corresponding acceptance
 * efficiency histogram and suppresses bins whose efficiency falls below
 * a specified minimum ratio.
 *
 * Conceptually:
 *      if Efficiency(θ, φ) < threshold  →  remove the bin
 *
 * The filtering is performed by setting the corresponding bin in the
 * reconstructed histogram to zero using `hFillByBin(i, j, 0)`.
 *
 * Particle handling:
 *
 *   Electrons and Protons (charged particles):
 *     - The function loops over all momentum slices.
 *     - Each θ–φ bin is checked against the charged-particle threshold
 *       `cP_minR`.
 *
 *   Neutrons (neutral particles):
 *     - Both regular momentum slices and constant-slice maps are filtered.
 *     - The neutron-specific threshold `nP_minR` is used.
 *
 * Histogram dimensions:
 *   The loops iterate over the stored histogram bin counts:
 *     - `HistElectronSliceNumOfXBins`, `HistElectronSliceNumOfYBins`
 *     - `HistNucSliceNumOfXBins`, `HistNucSliceNumOfYBins`
 *
 * Purpose:
 *   These filtered maps are later used to construct acceptance maps that
 *   exclude detector regions with poor reconstruction performance, which
 *   helps reduce systematic effects in the final analysis.
 *
 * @param cP_minR  Minimum allowed acceptance ratio for charged particles
 *                 (electrons and protons).
 * @param nP_minR  Minimum allowed acceptance ratio for neutrons.
 */
void AMaps::GenerateFilteredRecoMaps(double cP_minR, double nP_minR) {
    // TODO: generate filtered acceptance efficiency maps instead of filtered theta VS phi plots

    // Generate filtered reco. electron maps
    if (bt::FindSubstring(SName, "Uniform_1e_sample_")) {
        for (int bin = 0; bin < ElectronMomSliceLimits.size(); bin++) {
            for (int i = 0; i < (HistElectronSliceNumOfXBins + 1); i++) {
                for (int j = 0; j < (HistElectronSliceNumOfYBins + 1); j++) {
                    if (acceptance_eff_e_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { filtered_reco_theta_e_VS_phi_e_BySlice.at(bin).hFillByBin(i, j, 0); }
                }
            }
        }
    }

    // Generate filtered reco. proton maps
    if (bt::FindSubstring(SName, "Uniform_ep_sample_")) {
        for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
            for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
                for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
                    if (acceptance_eff_p_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < cP_minR) { filtered_reco_theta_p_VS_phi_p_BySlice.at(bin).hFillByBin(i, j, 0); }
                }
            }
        }
    }

    // Generate filtered reco. neutron maps
    if (bt::FindSubstring(SName, "Uniform_en_sample_")) {
        for (int bin = 0; bin < NucleonMomSliceLimits.size(); bin++) {
            for (int i = 0; i < (HistNucSliceNumOfXBins + 1); i++) {
                for (int j = 0; j < (HistNucSliceNumOfYBins + 1); j++) {
                    // Regular slices
                    if (acceptance_eff_n_BySlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < nP_minR) { filtered_reco_theta_n_VS_phi_n_BySlice.at(bin).hFillByBin(i, j, 0); }

                    // Constant slices
                    if (acceptance_eff_n_ByConstSlice.at(bin).GetHistogram2D()->GetBinContent(i, j) < nP_minR) { filtered_reco_theta_n_VS_phi_n_ByConstSlice.at(bin).hFillByBin(i, j, 0); }
                }
            }
        }
    }
}
#pragma endregion

// Functions to build binary matrices -----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Functions to build binary matrices (G3c+G3d) */
/**
 * @brief Generate acceptance-map (AMap) and weighted-map (WMap) matrices from efficiency histograms.
 *
 * This function converts the acceptance efficiency histograms calculated earlier
 * into discrete matrix representations used for saving and later analysis.
 * Each matrix corresponds to a specific momentum slice and represents the
 * detector acceptance in the θ–φ phase space.
 *
 * Two types of matrices are produced:
 *
 *   - **AMap (Acceptance Map)**
 *       A binary matrix indicating whether a given θ–φ bin passes the
 *       minimum acceptance threshold.
 *
 *           AMap(i,j) = 1  if Efficiency(i,j) ≥ threshold
 *           AMap(i,j) = 0  otherwise
 *
 *   - **WMap (Weighted Map)**
 *       A floating-point matrix storing the actual efficiency value for
 *       bins that pass the threshold. Bins below the threshold are set
 *       to zero.
 *
 *           WMap(i,j) = Efficiency(i,j)  if Efficiency(i,j) ≥ threshold
 *           WMap(i,j) = 0                otherwise
 *
 * Workflow:
 *   - Iterate over all momentum slices of a particle.
 *   - For each slice, loop over the θ–φ histogram bins.
 *   - Compare the efficiency value to the minimum allowed ratio.
 *   - Fill the corresponding AMap and WMap matrices.
 *
 * Particle handling:
 *   - **Electrons**
 *       Matrices are generated from `acceptance_eff_e_BySlice` using the
 *       charged-particle threshold `cP_minR`. An additional copy of the
 *       AMap slices is stored in `e_AMap_Slices_extended` for later
 *       extension operations.
 *
 *   - **Protons**
 *       Matrices are generated from `acceptance_eff_p_BySlice` using
 *       the charged-particle threshold `cP_minR`.
 *
 *   - **Neutrons**
 *       Two matrix sets are generated:
 *         • Momentum-dependent slices (`n_AMap_Slices`, `n_WMap_Slices`)
 *         • Constant slices (`n_AMap_ConstSlices`, `n_WMap_ConstSlices`)
 *       using the neutron threshold `nP_minR`.
 *
 * Histogram binning:
 *   The matrix dimensions correspond directly to the underlying histogram
 *   bin counts:
 *
 *       - Electrons: `HistElectronSliceNumOfXBins × HistElectronSliceNumOfYBins`
 *       - Nucleons:  `HistNucSliceNumOfXBins × HistNucSliceNumOfYBins`
 *
 * These matrices are later written to disk by `SaveMaps()` and used by
 * external analysis tools or reconstruction pipelines.
 *
 * @param cP_minR  Minimum acceptance ratio for charged particles
 *                 (electrons and protons).
 * @param nP_minR  Minimum acceptance ratio for neutrons.
 */
void AMaps::GenerateMapMatrices(double cP_minR, double nP_minR) {
    // A helper lambda to generate AMap and WMap matrices for a given particle type
    auto GenerateParticleMapMatrices = [&](auto& acceptance_eff_BySlice, int numXBins, int numYBins, double minRatio, std::vector<std::vector<std::vector<int>>>& AMap_Slices,
                                           std::vector<std::vector<std::vector<double>>>& WMap_Slices) {
        for (size_t bin = 0; bin < acceptance_eff_BySlice.size(); bin++) {
            std::vector<std::vector<int>> AMap_Slice;
            std::vector<std::vector<double>> WMap_Slice;

            for (int i = 0; i < numYBins; i++) {
                std::vector<int> AMap_Slice_col;
                std::vector<double> WMap_Slice_col;

                for (int j = 0; j < numXBins; j++) {
                    double binContent = acceptance_eff_BySlice.at(bin).GetHistogram2D()->GetBinContent(j + 1, i + 1);
                    if (binContent >= minRatio) {
                        AMap_Slice_col.push_back(1);
                        WMap_Slice_col.push_back(binContent);
                    } else {
                        AMap_Slice_col.push_back(0);
                        WMap_Slice_col.push_back(0.0);
                    }
                }

                AMap_Slice.push_back(AMap_Slice_col);
                WMap_Slice.push_back(WMap_Slice_col);
            }

            AMap_Slices.push_back(AMap_Slice);
            WMap_Slices.push_back(WMap_Slice);
        }
    };

    // Generate electron map matrices
    if (bt::FindSubstring(SName, "Uniform_1e_sample_")) {
        GenerateParticleMapMatrices(acceptance_eff_e_BySlice, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins, cP_minR, e_AMap_Slices, e_WMap_Slices);

        e_AMap_Slices_extended = e_AMap_Slices;
    }

    // Generate proton map matrices
    if (bt::FindSubstring(SName, "Uniform_ep_sample_")) {
        GenerateParticleMapMatrices(acceptance_eff_p_BySlice, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, cP_minR, p_AMap_Slices, p_WMap_Slices);
    }

    // Generate neutron map matrices
    if (bt::FindSubstring(SName, "Uniform_en_sample_")) {
        GenerateParticleMapMatrices(acceptance_eff_n_BySlice, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, nP_minR, n_AMap_Slices, n_WMap_Slices);
        GenerateParticleMapMatrices(acceptance_eff_n_ByConstSlice, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, nP_minR, n_AMap_ConstSlices, n_WMap_ConstSlices);
    }
}

/**
 * @brief Generate extended electron acceptance-map slices by expanding active bins.
 *
 * This function takes a reference set of electron acceptance-map (AMap) slices
 * and produces an extended version where each active bin (value = 1) is expanded
 * to include its neighboring bins. The extension is applied in all directions
 * around the original bin, including both orthogonal neighbors (up, down,
 * left, right) and diagonal neighbors.
 *
 * Conceptually, this performs a one-bin morphological dilation of the
 * acceptance map in the θ–φ grid. If a bin in the reference map is marked
 * as accepted (value = 1), the corresponding bin and all its immediate
 * neighbors are also marked as accepted in the extended map.
 *
 * The algorithm proceeds as follows:
 *   1. Create a temporary copy of the current extended electron AMap slices.
 *   2. Loop over all momentum slices defined by `ElectronMomSliceLimits`.
 *   3. For each slice, scan every θ–φ bin in the reference map.
 *   4. When a bin with value 1 is found, set its neighboring bins in the
 *      temporary map to 1, provided they lie within the histogram bounds.
 *   5. After processing all bins, replace `e_AMap_Slices_extended` with
 *      the updated temporary matrix.
 *
 * Boundary checks ensure that neighbor indices remain inside the
 * histogram dimensions defined by:
 *   - `HistElectronSliceNumOfXBins`
 *   - `HistElectronSliceNumOfYBins`
 *
 * This extension helps smooth the acceptance map by slightly enlarging
 * accepted regions, which can reduce edge artifacts caused by limited
 * detector statistics or sharp bin boundaries.
 *
 * @param ref_e_AMap_Slices Reference electron AMap slices used to
 *                          determine which bins should be extended.
 */
void AMaps::GenerateExtendedElectronMapMatrices(const std::vector<std::vector<std::vector<int>>>& ref_e_AMap_Slices) {
    // Generate extended electron map matrices
    if (bt::FindSubstring(SName, "Uniform_1e_sample_")) {
        std::vector<std::vector<std::vector<int>>> e_AMap_Slices_extended_temp = e_AMap_Slices_extended;

        for (int bin = 0; bin < ElectronMomSliceLimits.size(); bin++) {  // Loop over the slices
            for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
                for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                    // Check if the ref. matrix entry is 1:
                    if (ref_e_AMap_Slices.at(bin).at(i).at(j) == 1) {
                        // Check if the bin is already extended on the sides:
                        if ((i - 1) >= 0) { e_AMap_Slices_extended_temp.at(bin).at(i - 1).at(j) = 1; }

                        if ((i + 1) < HistElectronSliceNumOfYBins) { e_AMap_Slices_extended_temp.at(bin).at(i + 1).at(j) = 1; }

                        if ((j - 1) >= 0) { e_AMap_Slices_extended_temp.at(bin).at(i).at(j - 1) = 1; }

                        if ((j + 1) < HistElectronSliceNumOfXBins) { e_AMap_Slices_extended_temp.at(bin).at(i).at(j + 1) = 1; }

                        // Check if the bin is already extended on the diagonals:
                        if ((i - 1) >= 0 && (j - 1) >= 0) { e_AMap_Slices_extended_temp.at(bin).at(i - 1).at(j - 1) = 1; }

                        if ((i - 1) >= 0 && (j + 1) < HistElectronSliceNumOfXBins) { e_AMap_Slices_extended_temp.at(bin).at(i - 1).at(j + 1) = 1; }

                        if ((i + 1) < HistElectronSliceNumOfYBins && (j - 1) >= 0) { e_AMap_Slices_extended_temp.at(bin).at(i + 1).at(j - 1) = 1; }

                        if ((i + 1) < HistElectronSliceNumOfYBins && (j + 1) < HistElectronSliceNumOfXBins) { e_AMap_Slices_extended_temp.at(bin).at(i + 1).at(j + 1) = 1; }
                    }
                }
            }
        }

        e_AMap_Slices_extended = e_AMap_Slices_extended_temp;
    }
}
#pragma endregion

// SaveMaps function ----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SaveMaps function (G3e) */
/**
 * @brief Save acceptance-map (AMap) or weighted-map (WMap) matrices to disk.
 *
 * This function writes the generated map matrices for each particle type
 * (electron, proton, neutron) and each momentum slice to text files. The
 * matrices originate from the internal containers produced by
 * `GenerateMapMatrices()` and represent detector acceptance in θ–φ space.
 *
 * Depending on the current `Maps_Mode`, the function saves either:
 *
 *   - **AMaps (Acceptance Maps)**
 *       Binary matrices where each bin contains either 1 (accepted region)
 *       or 0 (rejected region).
 *
 *   - **WMaps (Weighted Maps)**
 *       Floating‑point matrices containing the acceptance efficiency values
 *       for each θ–φ bin.
 *
 * Workflow:
 *   1. Determine the map type from `Maps_Mode` (AMaps or WMaps).
 *   2. Create output directories for each particle species.
 *   3. Loop over all momentum slices defined in
 *      `ElectronMomSliceLimits` or `NucleonMomSliceLimits`.
 *   4. For each slice, write the corresponding matrix to a `.par` file
 *      containing:
 *        - the momentum slice limits
 *        - the histogram binning
 *        - the matrix rows representing θ–φ bins.
 *   5. Copy the produced files to a secondary backup directory
 *      (`AMapCopySavePath`).
 *
 * Special handling:
 *   - When running in AMaps mode for electrons, the function also saves
 *     the extended acceptance maps stored in `e_AMap_Slices_extended`.
 *   - Neutron maps include both momentum‑dependent slices and
 *     constant-slice maps.
 *
 * File format:
 *   Each output file stores the slice momentum limits followed by the
 *   matrix values for the θ–φ histogram bins. Rows correspond to θ bins
 *   and columns correspond to φ bins.
 *
 * @param SampleName              Name of the processed sample. Used to
 *                                construct the output directory structure.
 * @param AcceptanceMapsDirectory Base directory where map files are saved.
 */
void AMaps::SaveMaps(const std::string& SampleName, const std::string& AcceptanceMapsDirectory) {
    bool PrintOut = false;

    int testNumber = 0;
    const bool saveAMaps = (Maps_Mode == "AMaps");
    const bool saveWMaps = (Maps_Mode == "WMaps");
    const std::string modeLabel = Maps_Mode;

    if (!saveAMaps && !saveWMaps) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Unsupported Maps_Mode: " + Maps_Mode); }

    auto ensureDir = [](const std::string& path) { system(("mkdir -p " + path).c_str()); };
    auto copyToDir = [](const std::string& sourceFile, const std::string& targetDir) { system(("cp " + sourceFile + " " + targetDir).c_str()); };

    auto sliceFileName = [&](const std::string& particleTag, const std::vector<std::vector<double>>& limits, int slice) {
        return particleTag + "_" + modeLabel + "_file_from_" + bt::ToStringWithPrecision(limits.at(slice).at(0), 2) + "_to_" + bt::ToStringWithPrecision(limits.at(slice).at(1), 2) + ".par";
    };

    auto writeSliceMatrixFile = [&](const std::string& filePath, const std::vector<std::vector<double>>& limits, int slice, int xBins, int yBins, const char* xBinsLabel,
                                    const char* yBinsLabel, const char* rowPrefix, const auto& slicesContainer) {
        std::ofstream tempFile(filePath);
        tempFile << "Lower_P_lim:\t" << limits.at(slice).at(0) << "\n";
        tempFile << "Upper_P_lim:\t" << limits.at(slice).at(1) << "\n";
        tempFile << "\n";
        tempFile << xBinsLabel << ":\t" << xBins << "\n";
        tempFile << yBinsLabel << ":\t" << yBins << "\n";
        tempFile << "\n";

        for (int i = 0; i < yBins; i++) {
            tempFile << rowPrefix << "\t";
            for (int j = 0; j < xBins; j++) {
                tempFile << slicesContainer.at(slice).at(i).at(j);
                if (j != xBins - 1) { tempFile << ":"; }
            }
            tempFile << "\n";
        }
    };

    auto writeElectronExtendedSlice = [&](const std::string& filePath, const std::vector<std::vector<double>>& limits, int slice) {
        std::ofstream tempFile(filePath);
        tempFile << "Lower_P_lim:\t" << limits.at(slice).at(0) << "\n";
        tempFile << "Upper_P_lim:\t" << limits.at(slice).at(1) << "\n";
        tempFile << "\n";

        for (int i = 0; i < HistElectronSliceNumOfYBins; i++) {
            tempFile << "Line\t";
            for (int j = 0; j < HistElectronSliceNumOfXBins; j++) {
                tempFile << e_AMap_Slices_extended.at(slice).at(i).at(j);
                if (j != HistElectronSliceNumOfXBins - 1) { tempFile << ":"; }
            }
            tempFile << "\n";
        }
    };

    auto writeSliceLimits = [&](const std::string& particleTag, const std::vector<std::vector<double>>& limits, const std::string& savePath, const std::string& savePathCopy) {
        const std::string limitsFileName = particleTag + "_slice_limits.par";
        const std::string fullPath = savePath + limitsFileName;
        std::ofstream limitsFile(fullPath);

        for (int slice = 0; slice < limits.size(); slice++) {
            limitsFile << particleTag << "_slice_" << (slice + 1) << "\t" << limits.at(slice).at(0) << ":" << limits.at(slice).at(1) << "\n";
        }
        limitsFile << "\n";
        limitsFile.close();
        copyToDir(fullPath, savePathCopy);
    };

    auto makeModePaths = [&](const std::string& particleTag, const std::string& suffix) {
        const std::string savePath = AcceptanceMapsDirectory + SampleName + "/" + particleTag + "_" + modeLabel + suffix;
        const std::string copyPath = AMapCopySavePath + "/" + particleTag + "_" + modeLabel + suffix;
        ensureDir(savePath);
        ensureDir(copyPath);
        return std::pair<std::string, std::string>{savePath, copyPath};
    };

    if (bt::FindSubstring(SName, "Uniform_1e_sample_")) {
        auto [electronSavePath, electronCopyPath] = makeModePaths("e", "_by_slice/");

        for (int slice = 0; slice < ElectronMomSliceLimits.size(); slice++) {
            const std::string fileName = sliceFileName("e", ElectronMomSliceLimits, slice);
            const std::string fullPath = electronSavePath + fileName;

            if (saveAMaps) {
                writeSliceMatrixFile(fullPath, ElectronMomSliceLimits, slice, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins, "HistElectronSliceNumOfXBins",
                                     "HistElectronSliceNumOfYBins", "Line", e_AMap_Slices);
            } else {
                writeSliceMatrixFile(fullPath, ElectronMomSliceLimits, slice, HistElectronSliceNumOfXBins, HistElectronSliceNumOfYBins, "HistElectronSliceNumOfXBins",
                                     "HistElectronSliceNumOfYBins", "Weight", e_WMap_Slices);
            }

            copyToDir(fullPath, electronCopyPath);
        }

        writeSliceLimits("e", ElectronMomSliceLimits, electronSavePath, electronCopyPath);

        if (saveAMaps) {
            const std::string extendedSavePath = AcceptanceMapsDirectory + SampleName + "/e_extended_" + Maps_Mode + "_by_slice/";
            const std::string extendedCopyPath = AMapCopySavePath + "/e_extended_" + Maps_Mode + "_by_slice/";
            ensureDir(extendedSavePath);
            ensureDir(extendedCopyPath);

            for (int slice = 0; slice < ElectronMomSliceLimits.size(); slice++) {
                const std::string fileName = "e_" + Maps_Mode + "_file_from_" + bt::ToStringWithPrecision(ElectronMomSliceLimits.at(slice).at(0), 2) + "_to_" +
                                             bt::ToStringWithPrecision(ElectronMomSliceLimits.at(slice).at(1), 2) + ".par";
                const std::string fullPath = extendedSavePath + fileName;
                writeElectronExtendedSlice(fullPath, ElectronMomSliceLimits, slice);
                copyToDir(fullPath, extendedCopyPath);
            }
        }
    }

    if (bt::FindSubstring(SName, "Uniform_ep_sample_")) {
        auto [protonSavePath, protonCopyPath] = makeModePaths("p", "_by_slice/");

        for (int slice = 0; slice < NucleonMomSliceLimits.size(); slice++) {
            const std::string fileName = sliceFileName("p", NucleonMomSliceLimits, slice);
            const std::string fullPath = protonSavePath + fileName;

            if (saveAMaps) {
                writeSliceMatrixFile(fullPath, NucleonMomSliceLimits, slice, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, "HistNucSliceNumOfXBins", "HistNucSliceNumOfYBins", "Line",
                                     p_AMap_Slices);
            } else {
                writeSliceMatrixFile(fullPath, NucleonMomSliceLimits, slice, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, "HistNucSliceNumOfXBins", "HistNucSliceNumOfYBins", "Weight",
                                     p_WMap_Slices);
            }

            copyToDir(fullPath, protonCopyPath);
        }

        writeSliceLimits("p", NucleonMomSliceLimits, protonSavePath, protonCopyPath);
    }

    if (bt::FindSubstring(SName, "Uniform_en_sample_")) {
        auto [neutronSavePath, neutronCopyPath] = makeModePaths("n", "_by_slice/");
        auto [neutronConstSavePath, neutronConstCopyPath] = makeModePaths("n", "_by_ConstSlice/");

        for (int slice = 0; slice < NucleonMomSliceLimits.size(); slice++) {
            if (PrintOut) { ++testNumber, std::cout << "\n\n\nTEST " << testNumber << "\n"; }

            const std::string fileName = sliceFileName("n", NucleonMomSliceLimits, slice);
            const std::string fullPath = neutronSavePath + fileName;
            const std::string fullPathConst = neutronConstSavePath + fileName;

            if (saveAMaps) {
                writeSliceMatrixFile(fullPath, NucleonMomSliceLimits, slice, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, "HistNucSliceNumOfXBins", "HistNucSliceNumOfYBins", "Line",
                                     n_AMap_Slices);
                writeSliceMatrixFile(fullPathConst, NucleonMomSliceLimits, slice, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, "HistNucSliceNumOfXBins", "HistNucSliceNumOfYBins", "Line",
                                     n_AMap_ConstSlices);
            } else {
                writeSliceMatrixFile(fullPath, NucleonMomSliceLimits, slice, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, "HistNucSliceNumOfXBins", "HistNucSliceNumOfYBins", "Weight",
                                     n_WMap_Slices);
                writeSliceMatrixFile(fullPathConst, NucleonMomSliceLimits, slice, HistNucSliceNumOfXBins, HistNucSliceNumOfYBins, "HistNucSliceNumOfXBins", "HistNucSliceNumOfYBins",
                                     "Weight", n_WMap_ConstSlices);
            }

            copyToDir(fullPath, neutronCopyPath);
            copyToDir(fullPathConst, neutronConstCopyPath);
        }

        writeSliceLimits("n", NucleonMomSliceLimits, neutronSavePath, neutronCopyPath);
        writeSliceLimits("n", NucleonMomSliceLimits, neutronConstSavePath, neutronConstCopyPath);
    }
}
#pragma endregion

// DrawAndSaveMapsBySliceToPDFs function --------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveMapsBySliceToPDFs function */
/**
 * @brief Draw a list of histograms and save them as a multi-page PDF.
 *
 * This function takes a list of ROOT histogram objects and exports them into
 * a single PDF file, with one histogram drawn per page. It supports both
 * one-dimensional (`TH1`) and two-dimensional (`TH2`) histograms.
 *
 * Workflow:
 *   1. Create temporary canvases used to initialize and render the PDF.
 *   2. Open the output PDF in multi-page mode using the `[` syntax.
 *   3. Loop over all objects in `HistoList`.
 *   4. For each object:
 *        - detect whether it is a `TH1` or `TH2`
 *        - configure margins, grids, and statistics display
 *        - draw the histogram
 *        - adjust the title, stats box, and color palette placement
 *        - optionally mark empty histograms with a notice
 *        - print the current canvas page into the PDF
 *   5. Close the PDF in multi-page mode using the `]` syntax.
 *   6. Run a post-processing step to fix the PDF orientation.
 *   7. Delete the temporary canvases.
 *
 * Histogram handling:
 *   - `TH2` histograms are drawn with the `COLZ` option so the bin contents
 *     are displayed as a colored map with a palette.
 *   - `TH1` histograms are drawn with the default 1D draw option.
 *   - For 2D histograms, the function preserves axis labels when rewriting
 *     titles using `convert_title_to_splitline(...)`.
 *
 * Layout adjustments:
 *   - The statistics box is repositioned so it does not overlap the palette
 *     or title region.
 *   - The color palette is moved to a fixed right-hand position for cleaner
 *     presentation.
 *   - Empty histograms are explicitly labeled using
 *     `hf::DrawEmptyHistogramNotice(...)`.
 *
 * Notes:
 *   - The function assumes that all entries in `HistoList` are ROOT histogram
 *     objects compatible with `TH1` or `TH2`.
 *   - The generated PDF is later corrected by `hf::FixPDFOrientation(...)`
 *     to ensure consistent page orientation.
 *
 * @param HistoList    List of histogram objects to draw and export.
 * @param PDFfileName  Full path and filename of the output PDF.
 */
void AMaps::DrawAndSaveMapsBySliceToPDFs(std::vector<TObject*> HistoList, const std::string& PDFfileName) {
    TCanvas* myText_temp = new TCanvas("myText_temp", "myText_temp", pixelx, pixely);
    TCanvas* myCanvas_temp = new TCanvas("myCanvas_temp", "myCanvas_temp", pixelx, pixely);

    gStyle->SetOptStat("ourmen");

    myText_temp->SaveAs((PDFfileName + "[").c_str());
    const char* fileName = PDFfileName.c_str();

    for (int i = 0; i < HistoList.size(); i++) {
        myCanvas_temp->cd();

        myCanvas_temp->SetGrid();
        myCanvas_temp->SetBottomMargin(0.14), myCanvas_temp->SetLeftMargin(0.16), myCanvas_temp->SetRightMargin(0.23);
        myCanvas_temp->SetTopMargin(0.16);

        if (TH2* h2 = dynamic_cast<TH2*>(HistoList[i])) {
            h2->SetStats(kTRUE);

            h2->Draw("COLZ"), gPad->Modified(), gPad->Update();

            // Save current axis labels
            std::string xlab = h2->GetXaxis() ? h2->GetXaxis()->GetTitle() : "";
            std::string ylab = h2->GetYaxis() ? h2->GetYaxis()->GetTitle() : "";
            std::string zlab;

            if (auto* h = dynamic_cast<TH2*>(h2)) {
                if (h->GetZaxis()) { zlab = h->GetZaxis()->GetTitle(); }
            }

            // Convert title (and sanitize)
            const char* ct = h2->GetTitle();
            const std::string curr = ct ? std::string(ct) : std::string();
            const std::string converted = convert_title_to_splitline(curr);

            if (converted != curr) {
                h2->SetTitle(converted.c_str());  // Set only the title (no semicolons inside!)

                // Restore axis labels explicitly
                if (h2->GetXaxis()) h2->GetXaxis()->SetTitle(xlab.c_str());
                if (h2->GetYaxis()) h2->GetYaxis()->SetTitle(ylab.c_str());
                if (auto* h = dynamic_cast<TH2*>(h2)) {
                    if (h->GetZaxis()) { h->GetZaxis()->SetTitle(zlab.c_str()); }
                }

                if (auto* t = (TPaveText*)gPad->GetPrimitive("title")) { t->SetY1NDC(t->GetY1NDC() - 0.04), t->SetY2NDC(t->GetY2NDC() - 0.04), gPad->Modified(), gPad->Update(); }
            }

            // Stats box (guard for nullptr)
            TPaveStats* stats = dynamic_cast<TPaveStats*>(h2->FindObject("stats"));
            if (stats) { stats->SetX1NDC(0.78), stats->SetY1NDC(0.595), stats->SetX2NDC(0.98), stats->SetY2NDC(0.835); }

            // Palette (guard for nullptr)
            TPaletteAxis* palette = dynamic_cast<TPaletteAxis*>(h2->GetListOfFunctions()->FindObject("palette"));
            if (palette) { palette->SetX1NDC(0.775), palette->SetY1NDC(0.14), palette->SetX2NDC(0.82), palette->SetY2NDC(0.55); }

            if (hf::IsHistogramEmpty(h2)) { hf::DrawEmptyHistogramNotice(0.165, 0.4, 0.785, 0.6, 0.08); }

            gPad->Modified(), gPad->Update();
        } else if (TH1* h1 = dynamic_cast<TH1*>(HistoList[i])) {
            h1->SetStats(kTRUE);

            h1->Sumw2(), h1->Draw(), gPad->Modified(), gPad->Update();

            // Stats box (guard for nullptr)
            if (TPaveStats* stats = dynamic_cast<TPaveStats*>(h2->FindObject("stats"))) {
                stats->SetX1NDC(0.78), stats->SetY1NDC(0.595);
                stats->SetX2NDC(0.98), stats->SetY2NDC(0.835);
            }

            if (hf::IsHistogramEmpty(h1)) { hf::DrawEmptyHistogramNotice(0.165, 0.4, 0.785, 0.6, 0.08); }

            gPad->Modified(), gPad->Update();
        }

        myCanvas_temp->Print(fileName, "pdf");
        myCanvas_temp->Clear();
    }

    myCanvas_temp->Print((PDFfileName + "]").c_str(), "pdf");

    hf::FixPDFOrientation(PDFfileName);  // Fix orientation

    delete myText_temp;
    delete myCanvas_temp;
}
#pragma endregion

// DrawAndSaveMaps function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* DrawAndSaveMaps function (G3) */
/**
 * @brief Main driver routine for generating, saving, and visualizing acceptance maps.
 *
 * This function orchestrates the full workflow for constructing detector
 * acceptance maps (AMaps) or weighted maps (WMaps) for a given simulation
 * sample. It coordinates efficiency calculations, filtering of poorly
 * reconstructed regions, matrix generation, file export, and visualization
 * of the resulting maps.
 *
 * Workflow:
 *   1. Create the output directory for the current sample.
 *   2. Compute acceptance efficiencies using `CalcAcceptanceEfficiency()`.
 *   3. Remove low-efficiency bins from reconstructed maps using
 *      `GenerateFilteredRecoMaps()`.
 *   4. Convert efficiency histograms into discrete AMap/WMap matrices via
 *      `GenerateMapMatrices()`.
 *   5. Extend electron acceptance maps using
 *      `GenerateExtendedElectronMapMatrices()` to smooth boundaries.
 *   6. Save the generated matrices to disk using `SaveMaps()`.
 *   7. Draw all intermediate and final histograms and export them to
 *      multi‑page PDF files using `DrawAndSaveMapsBySliceToPDFs()`.
 *   8. Store acceptance-map histograms into ROOT files for later reuse.
 *
 * Particle handling:
 *   - **Electrons**: momentum-sliced maps are produced and extended maps
 *     are generated to enlarge accepted regions.
 *   - **Protons**: momentum-sliced acceptance maps are produced.
 *   - **Neutrons**: both momentum-sliced and constant-slice maps are
 *     generated and saved.
 *
 * Visualization:
 *   For each slice, the following histogram categories are drawn and
 *   exported:
 *     - truth (generated particle distribution)
 *     - reconstructed distribution
 *     - acceptance-efficiency ratio
 *     - filtered reconstructed maps
 *
 * Output:
 *   - PDF files containing per-slice θ–φ maps
 *   - `.par` files containing AMap/WMap matrices
 *   - ROOT files storing histogram collections for later analysis
 *
 * @param SampleName Name of the processed Monte Carlo sample.
 * @param h1DCanvas  Canvas used for drawing histograms.
 * @param AcceptanceMapsDirectory Base directory where maps and plots are saved.
 */
void AMaps::DrawAndSaveMaps(const std::string& SampleName, TCanvas* h1DCanvas, const std::string& AcceptanceMapsDirectory) {
    bool PrintOut = false;
    const bool hasElectronSample = bt::FindSubstring(SName, "Uniform_1e_sample_");
    const bool hasProtonSample = bt::FindSubstring(SName, "Uniform_ep_sample_");
    const bool hasNeutronSample = bt::FindSubstring(SName, "Uniform_en_sample_");

    if (PrintOut) { std::cout << "\n\nProcessing maps...\n"; }

    if (PrintOut) { std::cout << "\n\nGenerating maps directories...\n"; }
    system(("mkdir -p " + AcceptanceMapsDirectory + SampleName).c_str());

    if (PrintOut) { std::cout << "\n\nCalculating efficiencies...\n"; }
    CalcAcceptanceEfficiency();

    if (PrintOut) { std::cout << "\n\nGenerating filtered maps...\n"; }
    GenerateFilteredRecoMaps(Charged_particle_min_Ratio, Neutral_particle_min_Ratio);

    if (PrintOut) { std::cout << "\n\nGenerating map matrices...\n"; }
    GenerateMapMatrices(Charged_particle_min_Ratio, Neutral_particle_min_Ratio);
    GenerateExtendedElectronMapMatrices(e_AMap_Slices);           // First extension by e_AMap_Slices
    GenerateExtendedElectronMapMatrices(e_AMap_Slices_extended);  // Second extension by e_AMap_Slices_extended

    if (PrintOut) { std::cout << "\n\nSaving maps...\n"; }
    SaveMaps(SampleName, AcceptanceMapsDirectory);

    auto DrawSlices = [&](int numSlices, auto& truthBySlice, auto& recoBySlice, auto& ratioBySlice, auto& filteredBySlice) {
        for (int i = 0; i < numSlices; i++) {
            truthBySlice.at(i).hDrawAndSave(SName, h1DCanvas, TLAMaps, AMap_TL_HistoList, true);
            recoBySlice.at(i).hDrawAndSave(SName, h1DCanvas, RecoAMaps, AMap_Reco_HistoList, true);
            ratioBySlice.at(i).hDrawAndSave(SName, h1DCanvas, AMapsRatio, AMap_Ratio_HistoList, true);
            filteredBySlice.at(i).hDrawAndSave(SName, h1DCanvas, Charged_particle_Sep_AMaps, cPart_Sep_AMaps_HistoList, true);
        }
    };

    auto DrawConstNeutronSlices = [&](int numSlices) {
        for (int i = 0; i < numSlices; i++) {
            truth_theta_n_VS_phi_n_ByConstSlice.at(i).hDrawAndSave(SName, h1DCanvas, ConstTLAMaps, AMap_ConstTL_HistoList, true);
            reco_theta_n_VS_phi_n_ByConstSlice.at(i).hDrawAndSave(SName, h1DCanvas, ConstRecoAMaps, AMap_ConstReco_HistoList, true);
            acceptance_eff_n_ByConstSlice.at(i).hDrawAndSave(SName, h1DCanvas, ConstAMapsRatio, AMap_ConstRatio_HistoList, true);
            filtered_reco_theta_n_VS_phi_n_ByConstSlice.at(i).hDrawAndSave(SName, h1DCanvas, Const_neutral_particle_Sep_AMaps, nPart_Sep_AMaps_HistoList_Const, true);
        }
    };

    auto SaveSlicePDFs = [&](const std::vector<TObject*>& objects, const std::string& basePath, const std::string& fileName) { DrawAndSaveMapsBySliceToPDFs(objects, basePath + fileName); };

    if (hasElectronSample) {
        if (PrintOut) { std::cout << "\n\nPlotting electron maps...\n"; }

        /* Acceptance maps BC */
        reco_theta_e_VS_phi_e_BC.hDrawAndSave(SName, h1DCanvas, AcceptanceMapsBC, AMapsBC_HistoList, true);

        /* Electron maps */
        DrawSlices(ElectronMomSliceLimits.size(), truth_theta_e_VS_phi_e_BySlice, reco_theta_e_VS_phi_e_BySlice, acceptance_eff_e_BySlice, filtered_reco_theta_e_VS_phi_e_BySlice);

        /* Finalized acceptance maps */
        filtered_reco_theta_e_VS_phi_e.hDrawAndSave(SName, h1DCanvas, AcceptanceMaps, AMaps_HistoList, true);

        /* Saving maps in PDFs */
        SaveSlicePDFs(truth_e_BySlice, truth_theta_e_VS_phi_e_BySlice[0].GetHistogram2DSaveNamePath(), "truth_theta_e_VS_phi_e_BySlice.pdf");
        SaveSlicePDFs(reco_e_BySlice, reco_theta_e_VS_phi_e_BySlice[0].GetHistogram2DSaveNamePath(), "reco_theta_e_VS_phi_e_BySlice.pdf");
        SaveSlicePDFs(acc_eff_e_BySlice, acceptance_eff_e_BySlice[0].GetHistogram2DSaveNamePath(), "acceptance_eff_e_BySlice.pdf");
        SaveSlicePDFs(filtered_reco_e_BySlice, filtered_reco_theta_e_VS_phi_e_BySlice[0].GetHistogram2DSaveNamePath(), "filtered_reco_theta_e_VS_phi_e_BySlice.pdf");
    }

    if (hasProtonSample) {
        if (PrintOut) { std::cout << "\n\nPlotting proton maps...\n"; }

        /* Acceptance maps BC */
        reco_theta_p_VS_phi_p_BC.hDrawAndSave(SName, h1DCanvas, AcceptanceMapsBC, AMapsBC_HistoList, true);

        /* Proton maps */
        DrawSlices(NucleonMomSliceLimits.size(), truth_theta_p_VS_phi_p_BySlice, reco_theta_p_VS_phi_p_BySlice, acceptance_eff_p_BySlice, filtered_reco_theta_p_VS_phi_p_BySlice);

        /* Finalized acceptance maps */
        filtered_reco_theta_p_VS_phi_p.hDrawAndSave(SName, h1DCanvas, AcceptanceMaps, AMaps_HistoList, true);

        /* Saving maps in PDFs */
        SaveSlicePDFs(truth_p_BySlice, truth_theta_p_VS_phi_p_BySlice[0].GetHistogram2DSaveNamePath(), "truth_theta_p_VS_phi_p_BySlice.pdf");
        SaveSlicePDFs(reco_p_BySlice, reco_theta_p_VS_phi_p_BySlice[0].GetHistogram2DSaveNamePath(), "reco_theta_p_VS_phi_p_BySlice.pdf");
        SaveSlicePDFs(acc_eff_p_BySlice, acceptance_eff_p_BySlice[0].GetHistogram2DSaveNamePath(), "acceptance_eff_p_BySlice.pdf");
        SaveSlicePDFs(filtered_reco_p_BySlice, filtered_reco_theta_p_VS_phi_p_BySlice[0].GetHistogram2DSaveNamePath(), "filtered_reco_theta_p_VS_phi_p_BySlice.pdf");
    }

    if (hasNeutronSample) {
        if (PrintOut) { std::cout << "\n\nPlotting proton maps...\n"; }

        /* Acceptance maps BC */
        reco_theta_n_VS_phi_n_BC.hDrawAndSave(SName, h1DCanvas, AcceptanceMapsBC, AMapsBC_HistoList, true);

        /* Neutron maps (by momentum slice) */
        DrawSlices(NucleonMomSliceLimits.size(), truth_theta_n_VS_phi_n_BySlice, reco_theta_n_VS_phi_n_BySlice, acceptance_eff_n_BySlice, filtered_reco_theta_n_VS_phi_n_BySlice);
        DrawConstNeutronSlices(NucleonMomSliceLimits.size());

        /* Neutron maps */
        truth_theta_n_VS_phi_n.hDrawAndSave(SName, h1DCanvas, TLAMaps, AMap_TL_HistoList, true);
        reco_theta_n_VS_phi_n.hDrawAndSave(SName, h1DCanvas, RecoAMaps, AMap_Reco_HistoList, true);
        acceptance_eff_n.hDrawAndSave(SName, h1DCanvas, AMapsRatio, AMap_Ratio_HistoList, true);

        /* Finalized acceptance maps */
        filtered_reco_theta_n_VS_phi_n.hDrawAndSave(SName, h1DCanvas, AcceptanceMaps, AMaps_HistoList, true);

        /* Saving maps in PDFs */
        SaveSlicePDFs(truth_n_BySlice, truth_theta_n_VS_phi_n_BySlice[0].GetHistogram2DSaveNamePath(), "truth_theta_n_VS_phi_n_BySlice.pdf");
        SaveSlicePDFs(reco_n_BySlice, reco_theta_n_VS_phi_n_BySlice[0].GetHistogram2DSaveNamePath(), "reco_theta_n_VS_phi_n_BySlice.pdf");
        SaveSlicePDFs(acc_eff_n_BySlice, acceptance_eff_n_BySlice[0].GetHistogram2DSaveNamePath(), "acceptance_eff_n_BySlice.pdf");
        SaveSlicePDFs(filtered_reco_n_BySlice, filtered_reco_theta_n_VS_phi_n_BySlice[0].GetHistogram2DSaveNamePath(), "filtered_reco_theta_n_VS_phi_n_BySlice.pdf");

        /* Saving maps in PDFs (const) */
        SaveSlicePDFs(truth_n_ByConstSlice, truth_theta_n_VS_phi_n_ByConstSlice[0].GetHistogram2DSaveNamePath(), "truth_theta_n_VS_phi_n_ByConstSlice.pdf");
        SaveSlicePDFs(reco_n_ByConstSlice, reco_theta_n_VS_phi_n_ByConstSlice[0].GetHistogram2DSaveNamePath(), "reco_theta_n_VS_phi_n_ByConstSlice.pdf");
        SaveSlicePDFs(acc_eff_n_ByConstSlice, acceptance_eff_n_ByConstSlice[0].GetHistogram2DSaveNamePath(), "acceptance_eff_n_ByConstSlice.pdf");
        SaveSlicePDFs(filtered_reco_n_ByConstSlice, filtered_reco_theta_n_VS_phi_n_ByConstSlice[0].GetHistogram2DSaveNamePath(), "filtered_reco_theta_n_VS_phi_n_ByConstSlice.pdf");
    }

    // A helper lambda that saves acceptance maps into TFiles
    auto SaveAMapsToROOTFile = [&SampleName](const std::string SavePath, const std::string AMaps_prefix, TList* AcceptanceMapsList, const bool& CopyAMapsToCopyPath,
                                             const std::string AMapsCopySavePath) {
        std::string ROOTFileName = AMaps_prefix + SampleName + ".root";
        std::string ROOTFile = SavePath + "/" + ROOTFileName;

        // if (gSystem->mkdir(refDir.c_str(), kTRUE) != 0) {
        //     // mkdir returns 0 on success; allow "already exists". Verify access:
        void* dirp = gSystem->OpenDirectory(SavePath.c_str());

        if (!dirp) {
            std::cerr << "AMaps::DrawAndSaveMaps: ERROR: cannot access '" << SavePath << "'\n";
            return;  // or throw
        }
        gSystem->FreeDirectory(dirp);
        // }

        TFile* AMapsROOTFile = new TFile(ROOTFile.c_str(), "recreate");
        if (!AMapsROOTFile || AMapsROOTFile->IsZombie()) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "ERROR opening '" + ROOTFile + "'");
            // return nullptr;  // or throw
        }

        AMapsROOTFile->cd();
        AcceptanceMapsList->Write();
        AMapsROOTFile->Write();
        AMapsROOTFile->Close();

        if (CopyAMapsToCopyPath) { system(("cp " + ROOTFile + " " + AMapsCopySavePath).c_str()); }
    };

#pragma region /* Saving acceptance maps to ROOT files */
    if (PrintOut) { std::cout << "\n\nSaving acceptance maps to ROOT files...\n\n"; }

    SaveAMapsToROOTFile(AMapSavePath, AMapsBC_prefix, AcceptanceMapsBC, true, AMapCopySavePath);                    // Acceptance maps BC
    SaveAMapsToROOTFile(AMapSavePath, AMap_TL_prefix, TLAMaps, true, AMapCopySavePath);                             // TL Acceptance maps
    SaveAMapsToROOTFile(AMapSavePath, AMap_Reco_prefix, RecoAMaps, true, AMapCopySavePath);                         // Reco Acceptance maps
    SaveAMapsToROOTFile(AMapSavePath, AMap_Ratio_prefix, AMapsRatio, true, AMapCopySavePath);                       // Ratio Acceptance maps
    SaveAMapsToROOTFile(AMapSavePath, cPart_Sep_AMaps_prefix, Charged_particle_Sep_AMaps, true, AMapCopySavePath);  // Charged particle separate AMaps
    SaveAMapsToROOTFile(AMapSavePath, AMaps_prefix, AcceptanceMaps, true, AMapCopySavePath);                        // Acceptance maps

    SaveAMapsToROOTFile(AMapSavePath, AMap_TL_prefix + "_const_", ConstTLAMaps, true, AMapCopySavePath);                              // TL Acceptance maps
    SaveAMapsToROOTFile(AMapSavePath, AMap_Reco_prefix + "_const_", ConstRecoAMaps, true, AMapCopySavePath);                          // Reco Acceptance maps
    SaveAMapsToROOTFile(AMapSavePath, AMap_Ratio_prefix + "_const_", ConstAMapsRatio, true, AMapCopySavePath);                        // Ratio Acceptance maps
    SaveAMapsToROOTFile(AMapSavePath, nPart_Sep_AMaps_prefix + "_const_", Const_neutral_particle_Sep_AMaps, true, AMapCopySavePath);  // Charged particle separate AMaps
#pragma endregion

    if (PrintOut) { std::cout << "done!\n\n\n"; }
}
#pragma endregion

// SetHistBinsFromHistTitle function ------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* SetHistBinsFromHistTitle function */
/**
 * @brief Extract the histogram binning from the histogram title.
 *
 * This function reads the number of θ–φ bins used in a 2D histogram
 * directly from the histogram title string. The title is assumed to
 * contain the bin counts encoded in the format:
 *
 *     "... (XXX x YYY)"
 *
 * where:
 *   - `XXX` represents the number of bins along the X axis (φ bins)
 *   - `YYY` represents the number of bins along the Y axis (θ bins)
 *
 * The function parses the title string to locate the numeric substrings
 * corresponding to the bin counts and converts them to integers using
 * `std::stoi`. The extracted values are stored in the class members:
 *
 *   - `HistNucSliceNumOfXBins`
 *   - `HistNucSliceNumOfYBins`
 *
 * These variables define the matrix dimensions used later when building
 * acceptance-map (AMap) and weighted-map (WMap) matrices.
 *
 * Notes:
 *   - The parsing assumes fixed-width numeric fields (three characters)
 *     in the title. If the title format changes, the extraction logic
 *     may need to be updated.
 *   - Debug output can be enabled via the `PrintOut` flag to display
 *     the parsed substrings and resulting bin values.
 *
 * @param Histogram2D Pointer to the ROOT `TH2D` histogram whose title
 *                    encodes the binning information.
 */
void AMaps::SetHistBinsFromHistTitle(TH2D* Histogram2D) {
    bool PrintOut = false;

    std::string Title = Histogram2D->GetTitle();
    std::string NumOfXBinsStr = Title.substr((Title.find_last_of('(')) + 1, 3);
    std::string NumOfYBinsStr = Title.substr(Title.find_last_of('x') + 1, 3);

    HistNucSliceNumOfXBins = std::stoi(NumOfXBinsStr);
    HistNucSliceNumOfYBins = std::stoi(NumOfYBinsStr);

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
/**
 * @brief Extract momentum-slice limits from a histogram title and store them.
 *
 * This function parses the title of a 2D ROOT histogram to recover the
 * momentum-slice boundaries used when the histogram was originally created.
 * The slice limits are encoded in the title string and are extracted by
 * locating specific markers (`#`) that surround the numeric values.
 *
 * Expected title structure:
 *   The title contains two numeric values representing the lower and upper
 *   momentum limits of the slice. These values appear near the `#` symbols
 *   and are extracted using fixed-length substrings.
 *
 * Workflow:
 *   1. Read the histogram title string.
 *   2. Locate the first and last `#` characters that mark the slice limits.
 *   3. Extract the numeric substrings corresponding to the lower and upper
 *      momentum bounds.
 *   4. Convert the extracted substrings to `double` using `std::stod`.
 *   5. Store the resulting `{lower, upper}` limits in a temporary vector.
 *   6. Append the slice limits to the appropriate container depending on
 *      the particle type:
 *        - `ElectronMomSliceLimits` for electrons
 *        - `NucleonMomSliceLimits` for protons and neutrons
 *
 * Notes:
 *   - The parsing assumes a fixed-width format for the numeric substrings.
 *     If the histogram title format changes, the extraction logic may need
 *     to be updated.
 *   - Debug output can be enabled via the `PrintOut` flag to display the
 *     extracted strings and converted values.
 *
 * @param Histogram2D Pointer to the ROOT `TH2D` histogram whose title
 *                    encodes the slice limits.
 * @param Particle String identifier used to determine which particle
 *                 container should receive the extracted slice limits.
 */
void AMaps::SetSlicesFromHistTitle(TH2D* Histogram2D, const std::string& Particle) {
    bool PrintOut = false;

    std::string Title = Histogram2D->GetTitle();
    std::string SliceLowerLimStr = Title.substr((Title.find_first_of('#')) - 4, 4);
    std::string SliceUpperLimStr = Title.substr(Title.find_last_of('#') + 4, 4);

    double SliceLowerLim = std::stod(SliceLowerLimStr);
    double SliceUpperLim = std::stod(SliceUpperLimStr);

    std::vector<double> MomBinsLimitsTemp = {SliceLowerLim, SliceUpperLim};

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
/**
 * @brief Extract momentum-slice limits from a histogram title (legacy implementation).
 *
 * This original helper function parses the title of a ROOT `TH2D` histogram
 * to recover the lower and upper momentum limits of a slice. The slice
 * boundaries are encoded in the histogram title and are extracted using
 * fixed substring positions around `#` markers embedded in the title.
 *
 * Workflow:
 *   1. Retrieve the histogram title string.
 *   2. Locate the first and last `#` characters that delimit the slice
 *      limits inside the title.
 *   3. Extract two fixed-length substrings representing the lower and
 *      upper momentum limits.
 *   4. Convert the extracted strings to `double` values using `std::stod`.
 *   5. Store the resulting `{lower, upper}` interval inside a temporary
 *      vector and append it to `NucleonMomSliceLimits`.
 *
 * Notes:
 *   - This function represents the original implementation that always
 *     stores the extracted limits in `NucleonMomSliceLimits`, regardless
 *     of the particle type.
 *   - The parsing assumes a fixed-width numeric format in the title. If
 *     the title format changes, the substring offsets may no longer be
 *     valid.
 *   - The `MomBinsLimits` argument is unused and kept only for backward
 *     compatibility with older call sites.
 *
 * @param Histogram2D Pointer to the histogram whose title encodes the
 *                    momentum slice limits.
 * @param MomBinsLimits Unused legacy parameter originally intended to
 *                      receive the extracted slice limits.
 */
void AMaps::SetSlicesFromHistTitle(TH2D* Histogram2D, std::vector<std::vector<double>> MomBinsLimits) {
    bool PrintOut = false;

    std::string Title = Histogram2D->GetTitle();
    std::string SliceLowerLimStr = Title.substr((Title.find_first_of('#')) - 4, 4);
    std::string SliceUpperLimStr = Title.substr(Title.find_last_of('#') + 4, 4);

    double SliceLowerLim = std::stod(SliceLowerLimStr);
    double SliceUpperLim = std::stod(SliceUpperLimStr);

    std::vector<double> MomBinsLimitsTemp = {SliceLowerLim, SliceUpperLim};
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
/**
 * @brief Load previously generated acceptance‑map histograms from ROOT files.
 *
 * This function reads several ROOT files produced earlier by
 * `DrawAndSaveMaps()` and restores the stored histograms into the
 * corresponding internal containers of the `AMaps` class. These histograms
 * represent different stages of the acceptance‑map construction pipeline
 * and are used later for analysis, visualization, or map matching.
 *
 * Workflow:
 *   1. Open the ROOT file containing the **baseline reconstructed maps**
 *      (`AMapsBC_prefix`).
 *   2. Iterate over all keys in the file and assign the histograms to
 *      the appropriate particle container based on their title
 *      (electron, proton, neutron, or nucleon).
 *   3. Load the **truth-level hit maps** (`AMap_TL_prefix`) and convert
 *      each histogram into an `hPlot2D` wrapper object.
 *      - The first histogram is also used to extract the binning
 *        information via `SetHistBinsFromHistTitle()`.
 *      - Slice limits are reconstructed using `SetSlicesFromHistTitle()`.
 *   4. Load the **reconstructed hit maps** (`AMap_Reco_prefix`) for
 *      each particle and momentum slice.
 *   5. Load the **acceptance-efficiency ratio maps** (`AMap_Ratio_prefix`),
 *      which represent the ratio between reconstructed and truth
 *      distributions.
 *   6. Load the **filtered reconstructed maps** (`cPart_Sep_AMaps_prefix`)
 *      where low-efficiency bins have already been removed.
 *   7. Finally, load the **combined acceptance maps** (`AMaps_prefix`)
 *      for electrons, protons, neutrons, and nucleons that are used
 *      directly for map matching.
 *
 * File structure:
 *   Each ROOT file contains either individual histograms or histogram
 *   collections stored as keys. The function loops over the keys,
 *   identifies each histogram by inspecting its title, and assigns
 *   it to the corresponding internal member variables.
 *
 * Notes:
 *   - The function performs strict error checking and terminates if
 *     any required ROOT file or histogram is missing.
 *   - Histogram ownership remains with the ROOT file unless explicitly
 *     cloned elsewhere.
 *
 * @param AcceptanceMapsDirectory Base directory containing the stored
 *                                ROOT files with acceptance maps.
 * @param SampleName Name of the sample whose acceptance maps should
 *                   be loaded.
 */
void AMaps::ReadHitMaps(const std::string& AcceptanceMapsDirectory, const std::string& SampleName) {
    bool PrintKeys = false;

#pragma region /* Load AMapsBC */
    std::string AMapsBC_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMapsBC_prefix + SampleName + ".root";
    TFile* AMapsBC_RootFile = new TFile(AMapsBC_RootFile_FileName.c_str());
    if (!AMapsBC_RootFile) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load AMapsBC root file!"); }

    for (TObject* keyAsObj : *AMapsBC_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey*>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << std::endl; }

        TH2D* TempHist = (TH2D*)keyAsObj;

        if (bt::FindSubstring(TempHist->GetTitle(), "Electron") || bt::FindSubstring(TempHist->GetTitle(), "electron")) {
            reco_theta_e_VS_phi_e_BC.SetHistogram2D(TempHist);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "Proton") || bt::FindSubstring(TempHist->GetTitle(), "proton")) {
            reco_theta_p_VS_phi_p_BC.SetHistogram2D(TempHist);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "Neutron") || bt::FindSubstring(TempHist->GetTitle(), "neutron")) {
            reco_theta_n_VS_phi_n_BC.SetHistogram2D(TempHist);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "Nucleon") || bt::FindSubstring(TempHist->GetTitle(), "nucleon")) {
            reco_theta_nuc_VS_phi_nuc_BC.SetHistogram2D(TempHist);
        }
    }
#pragma endregion

#pragma region /* Load Hit_Maps_TL */
    std::string Hit_Maps_TL_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_TL_prefix + SampleName + ".root";
    TFile* Hit_Maps_TL_RootFile = new TFile(Hit_Maps_TL_RootFile_FileName.c_str());
    if (!Hit_Maps_TL_RootFile) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load Hit_Maps_TL root file!"); }

    int counter = 0;

    for (TObject* keyAsObj : *Hit_Maps_TL_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey*>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << std::endl; }

        TH2D* TempHist = (TH2D*)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (counter == 0) { SetHistBinsFromHistTitle(TempHist); }

        if (bt::FindSubstring(TempHist->GetTitle(), "{" + eLabel + "}")) {
            truth_theta_e_VS_phi_e_BySlice.push_back(Temp2DHist);
            SetSlicesFromHistTitle(TempHist, NucleonMomSliceLimits);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "{" + pLabel + "}")) {
            truth_theta_p_VS_phi_p_BySlice.push_back(Temp2DHist);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "{" + nLabel + "}")) {
            truth_theta_n_VS_phi_n.SetHistogram2D(TempHist);
        }

        ++counter;
    }
#pragma endregion

#pragma region /* Load Hit_Maps_Reco */
    std::string Hit_Maps_Reco_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_Reco_prefix + SampleName + ".root";
    TFile* Hit_Maps_Reco_RootFile = new TFile(Hit_Maps_Reco_RootFile_FileName.c_str());
    if (!Hit_Maps_Reco_RootFile) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load Hit_Maps_Reco root file!"); }

    for (TObject* keyAsObj : *Hit_Maps_Reco_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey*>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << std::endl; }

        TH2D* TempHist = (TH2D*)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (bt::FindSubstring(TempHist->GetTitle(), "{" + eLabel + "}")) {
            reco_theta_e_VS_phi_e_BySlice.push_back(Temp2DHist);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "{" + pLabel + "}")) {
            reco_theta_p_VS_phi_p_BySlice.push_back(Temp2DHist);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "{" + nLabel + "}")) {
            reco_theta_n_VS_phi_n.SetHistogram2D(TempHist);
        }
    }
#pragma endregion

#pragma region /* Load Hit_Maps_Ratio */
    std::string Hit_Maps_Ratio_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMap_Ratio_prefix + SampleName + ".root";
    TFile* Hit_Maps_Ratio_RootFile = new TFile(Hit_Maps_Ratio_RootFile_FileName.c_str());
    if (!Hit_Maps_Ratio_RootFile) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load Hit_Maps_Ratio root file!"); }

    for (TObject* keyAsObj : *Hit_Maps_Ratio_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey*>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << std::endl; }

        TH2D* TempHist = (TH2D*)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (bt::FindSubstring(TempHist->GetTitle(), "{" + eLabel + "}")) {
            acceptance_eff_e_BySlice.push_back(Temp2DHist);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "{" + pLabel + "}")) {
            acceptance_eff_p_BySlice.push_back(Temp2DHist);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "{" + nLabel + "}")) {
            acceptance_eff_n.SetHistogram2D(TempHist);
        }
    }
#pragma endregion

#pragma region /* Load cPart_Sep_AMaps */
    std::string cPart_Sep_AMaps_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + cPart_Sep_AMaps_prefix + SampleName + ".root";
    TFile* cPart_Sep_AMaps_RootFile = new TFile(cPart_Sep_AMaps_RootFile_FileName.c_str());
    if (!cPart_Sep_AMaps_RootFile) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load cPart_Sep_AMaps root file!"); }

    for (TObject* keyAsObj : *cPart_Sep_AMaps_RootFile->GetListOfKeys()) {
        auto key = dynamic_cast<TKey*>(keyAsObj);

        if (PrintKeys) { std::cout << "Key name: " << key->GetName() << " Type: " << key->GetClassName() << std::endl; }

        TH2D* TempHist = (TH2D*)keyAsObj;
        hPlot2D Temp2DHist;
        Temp2DHist.SetHistogram2D(TempHist);

        if (bt::FindSubstring(TempHist->GetTitle(), "Electron") || bt::FindSubstring(TempHist->GetTitle(), "electron")) {
            filtered_reco_theta_e_VS_phi_e_BySlice.push_back(Temp2DHist);
        } else if (bt::FindSubstring(TempHist->GetTitle(), "Proton") || bt::FindSubstring(TempHist->GetTitle(), "proton")) {
            filtered_reco_theta_p_VS_phi_p_BySlice.push_back(Temp2DHist);
        }
    }
#pragma endregion

#pragma region /* Load AMaps */
    std::string AMaps_RootFile_FileName = AcceptanceMapsDirectory + "/" + SampleName + "/" + AMaps_prefix + SampleName + ".root";
    TFile* AMaps_RootFile = new TFile(AMaps_RootFile_FileName.c_str());
    if (!AMaps_RootFile) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load AMaps root file!"); }

    LoadedElectronAMaps0 = (TH2D*)AMaps_RootFile->Get("Electron_AMap");
    if (!LoadedElectronAMaps0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load Electron_AMap from root file!"); }

    LoadedProtonAMap = (TH2D*)AMaps_RootFile->Get("Proton_AMap");
    if (!LoadedProtonAMap) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load Proton_AMap from root file!"); }

    LoadedNeutronAMap = (TH2D*)AMaps_RootFile->Get("Neutron_AMap");
    if (!LoadedNeutronAMap) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load Neutron_AMap from root file!"); }

    LoadedNucleonAMap = (TH2D*)AMaps_RootFile->Get("Nucleon_AMap");
    if (!LoadedNucleonAMap) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load Nucleon_AMap from root file!"); }
#pragma endregion

    std::cout << "\n\nAcceptance maps loaded!\n\n";
}
#pragma endregion

// LoadWMapsFromRatioROOTFiles function ---------------------------------------------------------------------------------------------------------------------------------

#pragma region /* LoadWMapsFromRatioROOTFiles function */
/**
 * @brief Load weighted maps (WMaps) from acceptance‑ratio ROOT files and export them to matrix files.
 *
 * This function reconstructs WMap matrices from previously generated
 * acceptance‑ratio histograms stored in ROOT files. The ratio maps
 * represent reconstruction efficiency (Reco / Truth) and are converted
 * into numeric weight matrices used later during map‑matching or event
 * weighting.
 *
 * Workflow:
 *   1. Normalize input and output directory paths.
 *   2. Read ratio ROOT files produced during the acceptance‑map workflow
 *      (`AMap_Ratio_prefix`).
 *   3. Iterate over all stored histograms (either directly or inside
 *      `TList` containers).
 *   4. Identify particle type and slice type using histogram names
 *      and titles:
 *        - electron momentum slices
 *        - proton momentum slices
 *        - neutron momentum slices
 *        - neutron constant slices
 *        - combined neutron map
 *   5. Extract the lower momentum bound of each slice from the histogram
 *      name and sort slices in ascending order.
 *   6. Convert each `TH2D` histogram into a 2‑D numeric matrix
 *      (`std::vector<std::vector<double>>`) representing the WMap.
 *   7. Load slice‑limit files (`*_slice_limits.par`) when needed to
 *      reconstruct the momentum‑slice boundaries.
 *   8. Infer histogram binning from the loaded matrices and update
 *      internal bin‑count members.
 *   9. Temporarily switch the map mode to `WMaps` and call `SaveMaps()`
 *      so the matrices are exported into `.par` files using the same
 *      infrastructure as acceptance maps.
 *
 * Data produced:
 *   - `e_WMap_Slices`            Electron slice weight matrices
 *   - `p_WMap_Slices`            Proton slice weight matrices
 *   - `n_WMap_Slices`            Neutron slice weight matrices
 *   - `n_WMap_ConstSlices`       Neutron constant‑slice matrices
 *   - `n_WMap`                   Combined neutron map
 *
 * Notes:
 *   - Histograms are detached from the ROOT file using `SetDirectory(nullptr)`
 *     so they remain valid after the file is closed.
 *   - Slice ordering is reconstructed using the momentum limits encoded
 *     in histogram names ("_from_..._to_...").
 *   - The function supports both direct histogram storage and ROOT
 *     `TList` containers.
 *
 * @param SampleName Name of the Monte Carlo sample whose maps are loaded.
 * @param InputAcceptanceMapsDirectory Directory containing the ratio ROOT files.
 * @param OutputAcceptanceMapsDirectory Optional directory where generated
 *                                      WMap `.par` files will be written.
 *                                      If empty, the input directory is used.
 */
void AMaps::LoadWMapsFromRatioROOTFiles(const std::string& SampleName, const std::string& InputAcceptanceMapsDirectory, const std::string& OutputAcceptanceMapsDirectory) {
    auto ensureTrailingSlash = [](std::string path) {
        if (!path.empty() && path.back() != '/') { path.push_back('/'); }
        return path;
    };

    const std::string inputDir = ensureTrailingSlash(InputAcceptanceMapsDirectory);
    const std::string outputDir = ensureTrailingSlash(OutputAcceptanceMapsDirectory.empty() ? InputAcceptanceMapsDirectory : OutputAcceptanceMapsDirectory);

    struct SliceHistogram {
        double lowerLimit = 0.0;
        TH2D* histogram = nullptr;
    };

    auto histogramToWMap = [](TH2D* hist) {
        std::vector<std::vector<double>> wmap;
        if (!hist) { return wmap; }

        const int xBins = hist->GetNbinsX();
        const int yBins = hist->GetNbinsY();
        wmap.reserve(yBins);

        for (int y = 0; y < yBins; y++) {
            std::vector<double> row;
            row.reserve(xBins);

            for (int x = 0; x < xBins; x++) { row.push_back(hist->GetBinContent(x + 1, y + 1)); }

            wmap.push_back(row);
        }

        return wmap;
    };

    auto extractSliceLowerLimit = [](const std::string& histName) {
        const std::string fromTag = "_from_";
        const std::string toTag = "_to_";
        const size_t fromPos = histName.find(fromTag);
        const size_t toPos = histName.find(toTag);

        if (fromPos == std::string::npos || toPos == std::string::npos || toPos <= (fromPos + fromTag.size())) { return -1e9; }

        const std::string value = histName.substr(fromPos + fromTag.size(), toPos - (fromPos + fromTag.size()));
        try {
            return std::stod(value);
        } catch (...) { return -1e9; }
    };

    auto readRatioFile = [&](const std::string& rootFilePath, bool isConstFile, std::vector<SliceHistogram>& electronSlices, std::vector<SliceHistogram>& protonSlices,
                             std::vector<SliceHistogram>& neutronSlices, std::vector<SliceHistogram>& neutronConstSlices, TH2D*& neutronCombined) {
        auto consumeHistogram = [&](TH2D* hist) {
            if (!hist) { return; }

            hist->SetDirectory(nullptr);
            const std::string title = hist->GetTitle();
            const std::string name = hist->GetName();
            const bool isSlice = (name.find("_for_P_from_") != std::string::npos) || (title.find("P^{truth}_{") != std::string::npos);

            if (title.find("{" + eLabel + "}") != std::string::npos) {
                electronSlices.push_back({extractSliceLowerLimit(name), hist});
            } else if (title.find("{" + pLabel + "}") != std::string::npos) {
                protonSlices.push_back({extractSliceLowerLimit(name), hist});
            } else if (title.find("{" + nLabel + "}") != std::string::npos || title.find("Neutron") != std::string::npos || title.find("neutron") != std::string::npos) {
                if (!isSlice) {
                    neutronCombined = hist;
                } else if (isConstFile) {
                    neutronConstSlices.push_back({extractSliceLowerLimit(name), hist});
                } else {
                    neutronSlices.push_back({extractSliceLowerLimit(name), hist});
                }
            }
        };

        TFile* rootFile = TFile::Open(rootFilePath.c_str(), "READ");
        if (!rootFile || rootFile->IsZombie()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Could not load ratio root file: " + rootFilePath); }

        TIter nextKey(rootFile->GetListOfKeys());
        TKey* key = nullptr;

        while ((key = (TKey*)nextKey())) {
            TObject* obj = key->ReadObj();
            if (!obj) { continue; }

            if (obj->InheritsFrom(TH2D::Class())) {
                consumeHistogram(dynamic_cast<TH2D*>(obj));
            } else if (obj->InheritsFrom(TList::Class())) {
                auto* list = dynamic_cast<TList*>(obj);
                if (!list) { continue; }

                TIter nextObj(list);
                TObject* listObj = nullptr;
                while ((listObj = nextObj())) {
                    if (!listObj->InheritsFrom(TH2D::Class())) { continue; }
                    TH2D* hist = dynamic_cast<TH2D*>(listObj);
                    if (!hist) { continue; }
                    TH2D* clonedHist = dynamic_cast<TH2D*>(hist->Clone(hist->GetName()));
                    consumeHistogram(clonedHist);
                }
            }
        }

        rootFile->Close();
    };

    auto sortBySliceLowerLimit = [](std::vector<SliceHistogram>& slices) {
        std::sort(slices.begin(), slices.end(), [](const SliceHistogram& a, const SliceHistogram& b) { return a.lowerLimit < b.lowerLimit; });
    };

    e_WMap_Slices.clear();
    p_WMap_Slices.clear();
    n_WMap.clear();
    n_WMap_Slices.clear();
    n_WMap_ConstSlices.clear();

    const std::string ratioRootFile = inputDir + SampleName + "/" + AMap_Ratio_prefix + SampleName + ".root";
    const std::string constRatioRootFile = inputDir + SampleName + "/" + AMap_Ratio_prefix + "_const_" + SampleName + ".root";

    std::vector<SliceHistogram> electronSlices;
    std::vector<SliceHistogram> protonSlices;
    std::vector<SliceHistogram> neutronSlices;
    std::vector<SliceHistogram> neutronConstSlices;
    TH2D* neutronCombined = nullptr;

    readRatioFile(ratioRootFile, false, electronSlices, protonSlices, neutronSlices, neutronConstSlices, neutronCombined);

    TFile* constRatioFileProbe = TFile::Open(constRatioRootFile.c_str(), "READ");
    if (constRatioFileProbe && !constRatioFileProbe->IsZombie()) {
        constRatioFileProbe->Close();
        readRatioFile(constRatioRootFile, true, electronSlices, protonSlices, neutronSlices, neutronConstSlices, neutronCombined);
    } else if (constRatioFileProbe) {
        constRatioFileProbe->Close();
    }

    sortBySliceLowerLimit(electronSlices);
    sortBySliceLowerLimit(protonSlices);
    sortBySliceLowerLimit(neutronSlices);
    sortBySliceLowerLimit(neutronConstSlices);

    for (const auto& slice : electronSlices) { e_WMap_Slices.push_back(histogramToWMap(slice.histogram)); }
    for (const auto& slice : protonSlices) { p_WMap_Slices.push_back(histogramToWMap(slice.histogram)); }
    for (const auto& slice : neutronSlices) { n_WMap_Slices.push_back(histogramToWMap(slice.histogram)); }
    for (const auto& slice : neutronConstSlices) { n_WMap_ConstSlices.push_back(histogramToWMap(slice.histogram)); }
    n_WMap = histogramToWMap(neutronCombined);

    auto readSliceLimitsIfExists = [&](const std::string& limitsFilePath, std::vector<std::vector<double>>& destination) {
        std::ifstream probe(limitsFilePath);
        if (!probe.good()) { return; }
        probe.close();

        destination.clear();
        ReadAMapLimits(limitsFilePath.c_str(), destination);
    };

    if (ElectronMomSliceLimits.empty() && !e_WMap_Slices.empty()) {
        const std::string eSliceLimitsFile = inputDir + SampleName + "/e_AMaps_by_slice/e_slice_limits.par";
        readSliceLimitsIfExists(eSliceLimitsFile, ElectronMomSliceLimits);
    }

    if (NucleonMomSliceLimits.empty() && (!p_WMap_Slices.empty() || !n_WMap_Slices.empty() || !n_WMap_ConstSlices.empty())) {
        const std::string pSliceLimitsFile = inputDir + SampleName + "/p_AMaps_by_slice/p_slice_limits.par";
        const std::string nSliceLimitsFile = inputDir + SampleName + "/n_AMaps_by_slice/n_slice_limits.par";
        readSliceLimitsIfExists(pSliceLimitsFile, NucleonMomSliceLimits);
        if (NucleonMomSliceLimits.empty()) { readSliceLimitsIfExists(nSliceLimitsFile, NucleonMomSliceLimits); }
    }

    if (!e_WMap_Slices.empty() && !e_WMap_Slices.at(0).empty() && !e_WMap_Slices.at(0).at(0).empty()) {
        HistElectronSliceNumOfYBins = e_WMap_Slices.at(0).size();
        HistElectronSliceNumOfXBins = e_WMap_Slices.at(0).at(0).size();
    }

    if (!n_WMap_Slices.empty() && !n_WMap_Slices.at(0).empty() && !n_WMap_Slices.at(0).at(0).empty()) {
        HistNucSliceNumOfYBins = n_WMap_Slices.at(0).size();
        HistNucSliceNumOfXBins = n_WMap_Slices.at(0).at(0).size();
    } else if (!p_WMap_Slices.empty() && !p_WMap_Slices.at(0).empty() && !p_WMap_Slices.at(0).at(0).empty()) {
        HistNucSliceNumOfYBins = p_WMap_Slices.at(0).size();
        HistNucSliceNumOfXBins = p_WMap_Slices.at(0).at(0).size();
    } else if (!n_WMap_ConstSlices.empty() && !n_WMap_ConstSlices.at(0).empty() && !n_WMap_ConstSlices.at(0).at(0).empty()) {
        HistNucSliceNumOfYBins = n_WMap_ConstSlices.at(0).size();
        HistNucSliceNumOfXBins = n_WMap_ConstSlices.at(0).at(0).size();
    }

    SName = SampleName;
    AMapCopySavePath = outputDir + SampleName;
    system(("mkdir -p " + AMapCopySavePath).c_str());

    const std::string modeBackup = Maps_Mode;
    Maps_Mode = "WMaps";
    SaveMaps(SampleName, outputDir);
    Maps_Mode = modeBackup;
}
#pragma endregion

// ReadAMapLimits function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadAMapLimits function */
/**
 * @brief Read momentum-slice limits from a parameter file.
 *
 * This function loads slice boundary definitions from a `.par` file
 * produced during the acceptance-map generation stage. Each line
 * describing a slice is expected to contain an identifier with
 * `_slice_` followed by two numeric limits separated by `:`.
 *
 * File format example:
 *   e_slice_0 0.50:0.75
 *   e_slice_1 0.75:1.00
 *
 * Workflow:
 *   1. Open the provided parameter file.
 *   2. Read the file line by line.
 *   3. Identify lines containing `_slice_`, which correspond to
 *      momentum-slice definitions.
 *   4. Extract the slice limits string ("lower:upper").
 *   5. Split the string using `:` as a delimiter.
 *   6. Convert the resulting values to `double`.
 *   7. Store the limits as `{lower, upper}` inside
 *      `Loaded_particle_limits`.
 *
 * Output:
 *   The vector `Loaded_particle_limits` becomes a list of
 *   momentum-slice intervals where each element is:
 *
 *       { slice_lower_limit , slice_upper_limit }
 *
 * Error handling:
 *   If the input file cannot be opened, the function aborts
 *   execution via `bt::ExitWithError()` and prints the missing
 *   file path.
 *
 * @param filename Path to the slice-limit parameter file.
 * @param Loaded_particle_limits Destination container where the
 *        parsed slice boundaries will be stored.
 */
void AMaps::ReadAMapLimits(const char* filename, std::vector<std::vector<double>>& Loaded_particle_limits) {
    std::ifstream infile;
    infile.open(filename);

    if (db::AMapsDebuggerMode) { cout << "\nLoading electron slice limits from file:\n" << filename << "\n"; }

    if (infile.is_open()) {
        std::string tp;

        // getline(infile, tp) = read data from file object and put it into string.
        while (std::getline(infile, tp)) {
            std::stringstream ss(tp);
            std::string parameter, parameter2;
            ss >> parameter;  // get cut identifier

            if (bt::FindSubstring(parameter, "_slice_")) {
                // get cut values
                ss >> parameter2;
                std::stringstream ss2(parameter2);

                std::string LineEntry;
                std::vector<double> particle_limits;

                while (std::getline(ss2, LineEntry, ':')) { particle_limits.push_back(std::stod(LineEntry)); }

                Loaded_particle_limits.push_back(particle_limits);
            }
        }
    } else {
        bt::ExitWithError(__func__, __FILE__, __LINE__, std::string("File not found! Target file was set to:\n") + filename);
    }
}
#pragma endregion

// ReadMapSlices function (AMaps/WMaps) ---------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadMapSlices function (AMaps/WMaps) */
/**
 * @brief Load momentum‑slice maps (AMaps or WMaps) from disk.
 *
 * This function reads map files corresponding to individual momentum
 * slices for a given particle type and loads them into the provided
 * containers. The behavior depends on the current `Maps_Mode`:
 *
 *   - **AMaps mode** → loads integer acceptance maps (`std::vector<std::vector<int>>`)
 *   - **WMaps mode** → loads floating‑point weight maps (`std::vector<std::vector<double>>`)
 *
 * For each momentum slice defined in `Loaded_particle_limits`, the
 * function constructs the expected file name based on:
 *
 *   - particle type (electron, proton, neutron, nucleon)
 *   - slice limits (lower and upper momentum bounds)
 *   - map mode (`AMaps` or `WMaps`)
 *   - whether special maps are requested (e.g. extended electron maps
 *     or neutron constant‑slice maps)
 *
 * The function then checks multiple candidate file paths to support
 * both the current naming convention and legacy naming formats.
 *
 * Workflow:
 *   1. Determine the particle short label (`e`, `p`, `n`, `nuc`).
 *   2. Iterate over all provided momentum slices.
 *   3. Build candidate file paths for the slice map.
 *   4. Select the first existing file among the candidates.
 *   5. Load the map using `ReadMap()`.
 *   6. Append the loaded slice matrix to the destination container.
 *
 * Notes:
 *   - Only one of `Loaded_Particle_AMap_Slices` or
 *     `Loaded_Particle_WMap_Slices` is used depending on `Maps_Mode`.
 *   - The function supports special map variants such as:
 *       • extended electron maps
 *       • neutron constant‑slice maps
 *   - Debug messages print the slice limits and selected file path
 *     when `PrintOut` is enabled.
 *
 * @param SampleName Name of the Monte Carlo sample.
 * @param AcceptanceMapsDirectory Base directory containing stored maps.
 * @param Particle Particle identifier used to determine which map set to load.
 * @param Loaded_particle_limits List of slice boundaries `{lower, upper}`.
 * @param Loaded_Particle_AMap_Slices Destination container for acceptance maps.
 * @param Loaded_Particle_WMap_Slices Destination container for weight maps.
 * @param Special_maps Enables special map variants (extended or const‑slice).
 */
void AMaps::ReadMapSlices(const std::string& SampleName, const std::string& AcceptanceMapsDirectory, const std::string& Particle,
                          const std::vector<std::vector<double>>& Loaded_particle_limits, std::vector<std::vector<std::vector<int>>>* Loaded_Particle_AMap_Slices,
                          std::vector<std::vector<std::vector<double>>>* Loaded_Particle_WMap_Slices, const bool Special_maps) {
    bool PrintOut = false;

    const bool isAMapsMode = (Maps_Mode == "AMaps");
    const bool isWMapsMode = (Maps_Mode == "WMaps");

    if (!isAMapsMode && !isWMapsMode) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Unsupported Maps_Mode: " + Maps_Mode); }
    if (isAMapsMode && !Loaded_Particle_AMap_Slices) { bt::ExitWithError(__func__, __FILE__, __LINE__, "AMaps destination container is null"); }
    if (isWMapsMode && !Loaded_Particle_WMap_Slices) { bt::ExitWithError(__func__, __FILE__, __LINE__, "WMaps destination container is null"); }

    const std::string ParticleShort = isElectron(Particle) ? "e" : (isProton(Particle) ? "p" : (isNeutron(Particle) ? "n" : "nuc"));
    const std::string ExtendedMaps = (isElectron(Particle) && Special_maps) ? "_extended" : "";

    for (int Slice = 0; Slice < Loaded_particle_limits.size(); Slice++) {
        const std::string lowLim = bt::ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(0), 2);
        const std::string highLim = bt::ToStringWithPrecision(Loaded_particle_limits.at(Slice).at(1), 2);
        const std::string basePath = AcceptanceMapsDirectory + SampleName + "/";
        std::string fileToRead;
        std::vector<std::string> candidateFiles;

        if (isAMapsMode) {
            if (isNeutron(Particle) && Special_maps) {
                candidateFiles.push_back(ParticleShort + "_" + Maps_Mode + "_by_ConstSlice/" + ParticleShort + "_" + Maps_Mode + "_file_from_" + lowLim + "_to_" + highLim + ".par");
                candidateFiles.push_back(ParticleShort + "_AMap_by_ConstSlice/" + ParticleShort + "_AMap_file_from_" + lowLim + "_to_" + highLim + ".par");
            } else {
                candidateFiles.push_back(ParticleShort + ExtendedMaps + "_" + Maps_Mode + "_by_slice/" + ParticleShort + "_" + Maps_Mode + "_file_from_" + lowLim + "_to_" + highLim +
                                         ".par");
                candidateFiles.push_back(ParticleShort + ExtendedMaps + "_AMap_by_slice/" + ParticleShort + "_AMap_file_from_" + lowLim + "_to_" + highLim + ".par");
            }
        } else {
            const std::string primaryFileName = ParticleShort + "_" + Maps_Mode + "_by_slice/" + ParticleShort + "_" + Maps_Mode + "_file_from_" + lowLim + "_to_" + highLim + ".par";
            const std::string legacyFileName = ParticleShort + "_WMap_by_slice/" + ParticleShort + "_WMap_file_from_" + lowLim + "_to_" + highLim + ".par";
            candidateFiles.push_back(primaryFileName);
            candidateFiles.push_back(legacyFileName);
        }

        fileToRead = candidateFiles.front();
        for (const auto& candidate : candidateFiles) {
            if (!bt::fileExists((basePath + candidate).c_str())) { continue; }
            fileToRead = candidate;
            break;
        }

        if (PrintOut) {
            std::cout << "\033[32m" << "Reading " << Particle << ((Special_maps && particleType == ELECTRON_TYPE) ? " extended " : " ") << Maps_Mode << " from " << "\033[0m" << lowLim
                      << "\033[32m" << " to " << "\033[0m" << highLim << "\033[32m" << " [GeV/c]:" << "\033[0m" << " " << fileToRead << "\n";
        }

        if (isAMapsMode) {
            std::vector<std::vector<int>> Loaded_Particle_AMap_TempSlice;
            ReadMap((basePath + fileToRead).c_str(), &Loaded_Particle_AMap_TempSlice);
            Loaded_Particle_AMap_Slices->push_back(Loaded_Particle_AMap_TempSlice);
        } else {
            std::vector<std::vector<double>> Loaded_Particle_WMap_TempSlice;
            ReadMap((basePath + fileToRead).c_str(), nullptr, &Loaded_Particle_WMap_TempSlice);
            Loaded_Particle_WMap_Slices->push_back(Loaded_Particle_WMap_TempSlice);
        }
    }
}
#pragma endregion

// ReadMap function (AMaps/WMaps) ---------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ReadMap function (AMaps/WMaps) */
/**
 * @brief Read a map file from disk and load its contents into memory.
 *
 * This function parses a `.par` file containing either an acceptance map
 * (AMap) or a weight map (WMap), depending on the current value of
 * `Maps_Mode`. Each line in the file represents one row of the map and
 * contains colon-separated values describing the bin contents.
 *
 * File format:
 *   - In **AMaps mode**, each relevant line begins with `Line` and contains
 *     integer values (0 or 1) describing whether a bin is accepted.
 *
 *       Example:
 *         Line0 1:1:0:0:1
 *         Line1 0:1:1:0:0
 *
 *   - In **WMaps mode**, each relevant line begins with `Weight` and
 *     contains floating-point values describing bin weights.
 *
 *       Example:
 *         Weight0 0.85:0.92:0.00:1.00
 *
 * Workflow:
 *   1. Validate that `Maps_Mode` is either `AMaps` or `WMaps`.
 *   2. Verify that the appropriate destination container pointer
 *      was provided.
 *   3. Open the target file and read it line by line.
 *   4. Identify map rows using the expected prefix (`Line` or `Weight`).
 *   5. Split the row entries using `:` as the delimiter.
 *   6. Convert each entry to the correct numeric type:
 *        - `int` for AMaps
 *        - `double` for WMaps
 *   7. Append the parsed row to the destination matrix container.
 *
 * Error handling:
 *   - If `Maps_Mode` is invalid or the destination container pointer is
 *     null, execution stops via `bt::ExitWithError()`.
 *   - Empty entries inside a row are treated as malformed input and
 *     trigger a fatal error.
 *   - Numeric conversion errors are reported to `stderr`.
 *
 * @param filename Path to the map `.par` file to read.
 * @param Loaded_particle_AMap Destination container for integer
 *        acceptance maps (used only in `AMaps` mode).
 * @param Loaded_particle_WMap Destination container for floating-point
 *        weight maps (used only in `WMaps` mode).
 */
void AMaps::ReadMap(const char* filename, std::vector<std::vector<int>>* Loaded_particle_AMap, std::vector<std::vector<double>>* Loaded_particle_WMap) {
    const bool PrintOut = false;
    const bool isAMapsMode = (Maps_Mode == "AMaps");
    const bool isWMapsMode = (Maps_Mode == "WMaps");

    if (!isAMapsMode && !isWMapsMode) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Unsupported Maps_Mode: " + Maps_Mode); }
    if (isAMapsMode && !Loaded_particle_AMap) { bt::ExitWithError(__func__, __FILE__, __LINE__, "AMaps destination container is null"); }
    if (isWMapsMode && !Loaded_particle_WMap) { bt::ExitWithError(__func__, __FILE__, __LINE__, "WMaps destination container is null"); }

    std::ifstream infile(filename);

    if (infile.is_open()) {
        const char* expectedPrefix = isAMapsMode ? "Line" : "Weight";
        std::string tp;
        int lineNumber = 0;

        while (std::getline(infile, tp)) {
            lineNumber++;
            std::stringstream ss(tp);
            std::string parameter, parameter2;
            ss >> parameter;

            if (bt::FindSubstring(parameter, expectedPrefix)) {
                ss >> parameter2;
                std::stringstream ss2(parameter2);

                std::string LineEntry;

                if (isAMapsMode) {
                    std::vector<int> col;

                    if (PrintOut) {
                        std::cout << "\n\nfilename = " << filename << "\n";
                        std::cout << "parameter = " << parameter << "\n";
                        std::cout << "parameter2 = " << parameter2 << "\n";
                        std::cout << "LineEntry = " << LineEntry << "\n\n";
                    }

                    while (std::getline(ss2, LineEntry, ':')) {
                        if (LineEntry.empty()) {
                            bt::ExitWithError(__func__, __FILE__, __LINE__, "Empty entry at line " + std::to_string(lineNumber) + " in file:\n" + filename + ":\n   -> " + tp);
                        }

                        try {
                            col.push_back(std::stoi(LineEntry));
                        } catch (const std::invalid_argument& e) {
                            std::cerr << "Invalid integer found in file " << filename << ": " << LineEntry << "\n";
                        } catch (const std::out_of_range& e) { std::cerr << "Integer out of range in file " << filename << ": " << LineEntry << "\n"; }
                    }

                    Loaded_particle_AMap->push_back(col);
                } else {
                    std::vector<double> col;

                    if (PrintOut) {
                        std::cout << "\n\nfilename = " << filename << "\n";
                        std::cout << "parameter = " << parameter << "\n";
                        std::cout << "parameter2 = " << parameter2 << "\n";
                        std::cout << "LineEntry = " << LineEntry << "\n\n";
                    }

                    while (std::getline(ss2, LineEntry, ':')) {
                        if (LineEntry.empty()) {
                            bt::ExitWithError(__func__, __FILE__, __LINE__, "Empty entry at line " + std::to_string(lineNumber) + " in file:\n" + filename + ":\n   -> " + tp);
                        }

                        try {
                            col.push_back(std::stoi(LineEntry));
                        } catch (const std::invalid_argument& e) {
                            std::cerr << "Invalid double found in file " << filename << ": " << LineEntry << "\n";
                        } catch (const std::out_of_range& e) { std::cerr << "Double out of range in file " << filename << ": " << LineEntry << "\n"; }
                    }

                    Loaded_particle_WMap->push_back(col);
                }
            }
        }
    } else {
        bt::PrintWarning(__func__, __FILE__, __LINE__, std::string("file:\n") + filename + "\nwas not found!");
    }
}
#pragma endregion

// FindSliceAndBinIndices function --------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* FindSliceAndBinIndices function */
/**
 * @brief Find the momentum-slice index and 2D angular-bin indices for a kinematical query.
 *
 * Purpose:
 *   Convert `(Momentum, Theta, Phi)` into a matched momentum slice and `(YBinInd, XBinInd)`
 *   matrix coordinates using the same fast lookup algorithm used by map matching and weighting.
 *
 * Algorithm:
 *   1. Validate that momentum slices and angular bin counts are well defined.
 *   2. Clamp the requested slice range to the loaded slice container.
 *   3. Reject points outside the global `(Theta, Phi)` map limits.
 *   4. Convert `Theta` and `Phi` directly into bin indices using bin arithmetic.
 *   5. Use a binary search over sorted momentum-slice limits to find the slice that contains `Momentum`.
 *   6. Return `true` only if all three indices were found successfully.
 *
 * @param LoadedMomSliceLimits Loaded momentum-slice boundaries to search.
 * @param HistSliceNumOfYBins Number of theta bins in the queried map.
 * @param HistSliceNumOfXBins Number of phi bins in the queried map.
 * @param Momentum Particle momentum used to choose the correct momentum slice.
 * @param Theta Particle polar angle used to choose the correct map row.
 * @param Phi Particle azimuthal angle used to choose the correct map column.
 * @param InitialSlice First slice index to consider, inclusive.
 * @param FinalSlice Last slice boundary to consider, exclusive.
 * @param matchedSlice Output index of the matched momentum slice.
 * @param YBinInd Output row index in the 2D map.
 * @param XBinInd Output column index in the 2D map.
 * @param ContextLabel Label used in error messages to identify the caller.
 *
 * @return True if a valid slice and bin indices were found, otherwise false.
 */
bool AMaps::FindSliceAndBinIndices(const std::vector<std::vector<double>>& LoadedMomSliceLimits, int HistSliceNumOfYBins, int HistSliceNumOfXBins, double Momentum, double Theta, double Phi,
                                   int InitialSlice, int FinalSlice, int& matchedSlice, int& YBinInd, int& XBinInd, const std::string& ContextLabel) {
    // Initialize all outputs to "not found" in case the function returns early.
    matchedSlice = -1;
    YBinInd = -1;
    XBinInd = -1;

    // Cache the total number of loaded momentum slices.
    const int totalSlices = static_cast<int>(LoadedMomSliceLimits.size());

    // Abort immediately if no momentum slices were loaded.
    if (totalSlices == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, ContextLabel + ": Loaded momentum-slice limits have size 0!"); }

    // Clamp the requested slice range so it always stays inside the loaded slice array.
    const int firstSlice = std::max(0, InitialSlice);
    const int lastSlice = std::min(FinalSlice, totalSlices);

    // If the requested slice interval is empty after clamping, no match is possible.
    if (firstSlice >= lastSlice) { return false; }

    // Verify that the loaded maps have a valid positive number of theta and phi bins.
    if (HistSliceNumOfYBins <= 0 || HistSliceNumOfXBins <= 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, ContextLabel + ": map bin counts must be positive"); }

    // Compute the full theta and phi span covered by the loaded maps.
    const double thetaInterval = hBinUpperYLim - hBinLowerYLim;
    const double phiInterval = hBinUpperXLim - hBinLowerXLim;

    // Abort if the map axis limits are malformed or degenerate.
    if (thetaInterval <= 0.0 || phiInterval <= 0.0) { bt::ExitWithError(__func__, __FILE__, __LINE__, ContextLabel + ": map axis limits are invalid"); }

    // Reject coordinates that lie outside the global angular range of the map.
    if (Theta < hBinLowerYLim || Theta >= hBinUpperYLim || Phi < hBinLowerXLim || Phi >= hBinUpperXLim) { return false; }

    // Precompute 1/bin_width so the angular coordinates can be converted directly into bin indices.
    const double invThetaBinWidth = static_cast<double>(HistSliceNumOfYBins) / thetaInterval;
    const double invPhiBinWidth = static_cast<double>(HistSliceNumOfXBins) / phiInterval;

    // Convert Theta into its row index in the 2D map matrix.
    YBinInd = static_cast<int>((Theta - hBinLowerYLim) * invThetaBinWidth);

    // Convert Phi into its column index in the 2D map matrix.
    XBinInd = static_cast<int>((Phi - hBinLowerXLim) * invPhiBinWidth);

    // Protect against any out-of-range bin index caused by boundary effects.
    if (YBinInd < 0 || YBinInd >= HistSliceNumOfYBins || XBinInd < 0 || XBinInd >= HistSliceNumOfXBins) { return false; }

    // Set the lower edge of the binary-search interval.
    int low = firstSlice;

    // Set the upper edge of the binary-search interval.
    int high = lastSlice - 1;

    // Binary-search the sorted momentum slices to find the one that contains Momentum.
    while (low <= high) {
        // Pick the midpoint of the current search interval.
        const int mid = low + (high - low) / 2;

        // Read the lower boundary of the candidate slice.
        const double lower = LoadedMomSliceLimits.at(mid).at(0);

        // Read the upper boundary of the candidate slice.
        const double upper = LoadedMomSliceLimits.at(mid).at(1);

        // If Momentum is below this slice, continue searching the lower half.
        if (Momentum < lower) {
            high = mid - 1;
        }
        // If Momentum is above this slice, continue searching the upper half.
        else if (Momentum > upper) {
            low = mid + 1;
        }
        // Otherwise Momentum belongs to this slice, so store it and stop the search.
        else {
            matchedSlice = mid;
            break;
        }
    }

    // Return true only if a matching momentum slice was found.
    return (matchedSlice >= 0);
}
#pragma endregion

// MatchToElectronAMap function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* MatchToElectronAMap */
/**
 * @brief Check whether an electron falls inside the loaded electron acceptance map.
 *
 * Purpose:
 *   Convert the electron kinematics `(Momentum, Theta, Phi)` into a momentum-slice index
 *   and a 2D angular-bin index, then query the corresponding loaded acceptance-map cell.
 *
 * Algorithm:
 *   1. Validate that electron momentum slices and angular binning were loaded correctly.
 *   2. Restrict the slice search range to the requested `[InitialSlice, FinalSlice)` interval.
 *   3. Reject points that are outside the global `(Theta, Phi)` map limits.
 *   4. Convert `Theta` and `Phi` directly into matrix indices using bin arithmetic:
 *        bin = floor((value - lower_limit) / bin_width)
 *      This avoids looping over all theta and phi bins.
 *   5. Use a binary search over the sorted momentum-slice limits to find the unique slice
 *      that contains `Momentum`. This avoids scanning all momentum slices linearly.
 *   6. Read the corresponding map cell from either the nominal electron map or the
 *      extended electron map, depending on `UseExtendedElectronMaps`.
 *   7. Return `true` if the selected map cell equals 1, otherwise return `false`.
 *
 * Performance notes:
 *   The original implementation used nested scans over momentum slices, theta bins, and phi bins.
 *   This version reduces the lookup cost by:
 *     - replacing theta-bin and phi-bin scans with O(1) index calculations
 *     - replacing the momentum-slice linear scan with an O(log N) binary search
 *   This is much faster when running over many slices, many angular bins, and millions of events.
 *
 * @param Momentum Electron momentum used to choose the correct momentum slice.
 * @param Theta Electron polar angle used to choose the correct map row.
 * @param Phi Electron azimuthal angle used to choose the correct map column.
 * @param InitialSlice First slice index to consider, inclusive.
 * @param FinalSlice Last slice boundary to consider, exclusive.
 * @param UseExtendedElectronMaps If true, query the extended electron maps instead of the nominal maps.
 *
 * @return True if the electron lies inside the selected acceptance-map bin and that bin is marked as accepted.
 *         False if no matching slice is found, the coordinates are outside the map, or the selected bin is rejected.
 */
bool AMaps::MatchToElectronAMap(double Momentum, double Theta, double Phi, int InitialSlice, int FinalSlice, bool UseExtendedElectronMaps) {
    // Optionally print how many electron momentum slices are currently loaded.
    if (db::AMapsDebuggerMode) { std::cout << "\n\nLoaded_ElectronMomSliceLimits.size() = " << Loaded_ElectronMomSliceLimits.size() << "\n\n"; }

    // Initialize the matched momentum-slice index and angular-bin indices.
    int matchedSlice = -1, YBinInd = -1, XBinInd = -1;

    // Run the shared fast lookup algorithm to find the matched slice and angular-bin indices.
    if (!FindSliceAndBinIndices(Loaded_ElectronMomSliceLimits, HistElectronSliceNumOfYBins, HistElectronSliceNumOfXBins, Momentum, Theta, Phi, InitialSlice, FinalSlice, matchedSlice,
                                YBinInd, XBinInd, "MatchToElectronAMap")) {
        return false;
    }

    // Branch to the extended-electron-map query if the caller requested it.
    if (UseExtendedElectronMaps) {
        // Make sure the extended electron map container was loaded.
        if (Loaded_e_AMap_Slices_extended.empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_e_AMap_Slices_extended has size 0!"); }

        // Make sure the matched slice index exists in the extended electron map container.
        if (matchedSlice >= static_cast<int>(Loaded_e_AMap_Slices_extended.size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "matchedSlice is out of range for Loaded_e_AMap_Slices_extended");
        }

        // Make sure the selected extended-map slice is not empty.
        if (Loaded_e_AMap_Slices_extended.at(matchedSlice).empty()) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_e_AMap_Slices_extended.at(" + std::to_string(matchedSlice) + ") has size 0!");
        }

        // Make sure the requested theta row exists inside the selected extended-map slice.
        if (YBinInd >= static_cast<int>(Loaded_e_AMap_Slices_extended.at(matchedSlice).size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "YBinInd is out of range for Loaded_e_AMap_Slices_extended.at(" + std::to_string(matchedSlice) + ")");
        }

        // Make sure the requested phi column exists inside the selected extended-map row.
        if (XBinInd >= static_cast<int>(Loaded_e_AMap_Slices_extended.at(matchedSlice).at(YBinInd).size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__,
                              "XBinInd is out of range for Loaded_e_AMap_Slices_extended.at(" + std::to_string(matchedSlice) + ").at(" + std::to_string(YBinInd) + ")");
        }

        // Print detailed debugging information for the selected extended map and bin if debugging is enabled.
        if (db::AMapsDebuggerMode) {
            std::cout << "\n\nLoaded_e_AMap_Slices_extended.size() = " << Loaded_e_AMap_Slices_extended.size() << "\n";
            std::cout << "Loaded_e_AMap_Slices_extended.at(" << matchedSlice << ").size() = " << Loaded_e_AMap_Slices_extended.at(matchedSlice).size() << "\n";
            std::cout << "Loaded_e_AMap_Slices_extended.at(" << matchedSlice << ").at(" << YBinInd << ").size() = " << Loaded_e_AMap_Slices_extended.at(matchedSlice).at(YBinInd).size()
                      << "\n";
            std::cout << "Loaded_ElectronMomSliceLimits.at(" << matchedSlice << ").at(0) = " << Loaded_ElectronMomSliceLimits.at(matchedSlice).at(0) << "\n";
            std::cout << "Loaded_ElectronMomSliceLimits.at(" << matchedSlice << ").at(1) = " << Loaded_ElectronMomSliceLimits.at(matchedSlice).at(1) << "\n\n";
        }

        // Return true only if the selected extended-electron-map bin is marked as accepted.
        return (Loaded_e_AMap_Slices_extended.at(matchedSlice).at(YBinInd).at(XBinInd) == 1);
    }

    // Make sure the nominal electron map container was loaded.
    if (Loaded_e_AMap_Slices.empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_e_AMap_Slices has size 0!"); }

    // Make sure the matched slice index exists in the nominal electron map container.
    if (matchedSlice >= static_cast<int>(Loaded_e_AMap_Slices.size())) { bt::ExitWithError(__func__, __FILE__, __LINE__, "matchedSlice is out of range for Loaded_e_AMap_Slices"); }

    // Make sure the selected nominal-map slice is not empty.
    if (Loaded_e_AMap_Slices.at(matchedSlice).empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_e_AMap_Slices.at(" + std::to_string(matchedSlice) + ") has size 0!"); }

    // Make sure the requested theta row exists inside the selected nominal-map slice.
    if (YBinInd >= static_cast<int>(Loaded_e_AMap_Slices.at(matchedSlice).size())) {
        bt::ExitWithError(__func__, __FILE__, __LINE__, "YBinInd is out of range for Loaded_e_AMap_Slices.at(" + std::to_string(matchedSlice) + ")");
    }

    // Make sure the requested phi column exists inside the selected nominal-map row.
    if (XBinInd >= static_cast<int>(Loaded_e_AMap_Slices.at(matchedSlice).at(YBinInd).size())) {
        bt::ExitWithError(__func__, __FILE__, __LINE__, "XBinInd is out of range for Loaded_e_AMap_Slices.at(" + std::to_string(matchedSlice) + ").at(" + std::to_string(YBinInd) + ")");
    }

    // Print detailed debugging information for the selected nominal map and bin if debugging is enabled.
    if (db::AMapsDebuggerMode) {
        std::cout << "\n\nLoaded_e_AMap_Slices.size() = " << Loaded_e_AMap_Slices.size() << "\n";
        std::cout << "Loaded_e_AMap_Slices.at(" << matchedSlice << ").size() = " << Loaded_e_AMap_Slices.at(matchedSlice).size() << "\n";
        std::cout << "Loaded_e_AMap_Slices.at(" << matchedSlice << ").at(" << YBinInd << ").size() = " << Loaded_e_AMap_Slices.at(matchedSlice).at(YBinInd).size() << "\n";
        std::cout << "Loaded_ElectronMomSliceLimits.at(" << matchedSlice << ").at(0) = " << Loaded_ElectronMomSliceLimits.at(matchedSlice).at(0) << "\n";
        std::cout << "Loaded_ElectronMomSliceLimits.at(" << matchedSlice << ").at(1) = " << Loaded_ElectronMomSliceLimits.at(matchedSlice).at(1) << "\n\n";
    }

    // Return true only if the selected nominal electron-map bin is marked as accepted.
    return (Loaded_e_AMap_Slices.at(matchedSlice).at(YBinInd).at(XBinInd) == 1);
}

/*
bool AMaps::MatchToElectronAMap(double Momentum, double Theta, double Phi, int InitialSlice, int FinalSlice, bool UseExtendedElectronMaps) {
    if (db::AMapsDebuggerMode) { std::cout << "\n\nLoaded_ElectronMomSliceLimits.size() = " << Loaded_ElectronMomSliceLimits.size() << "\n\n"; }

    for (int Slice = InitialSlice; Slice < FinalSlice; Slice++) {
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
                            if (UseExtendedElectronMaps) {
                                if (Loaded_e_AMap_Slices_extended.size() == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_e_AMap_Slices_extended has size 0!"); }
                                if (Loaded_e_AMap_Slices_extended.at(Slice).size() == 0) {
                                    bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_e_AMap_Slices_extended.at(" + std::to_string(Slice) + ") has size 0!");
                                }

                                if (db::AMapsDebuggerMode) {
                                    std::cout << "\n\nLoaded_e_AMap_Slices_extended.size() = " << Loaded_e_AMap_Slices_extended.size() << "\n";
                                    std::cout << "Loaded_e_AMap_Slices_extended.at(" << Slice << ").size() = " << Loaded_e_AMap_Slices_extended.at(Slice).size() << "\n";
                                    std::cout << "Loaded_e_AMap_Slices_extended.at(" << Slice << ").at(" << i << ").size() = " << Loaded_e_AMap_Slices_extended.at(Slice).at(i).size()
                                              << "\n";
                                    std::cout << "Loaded_ElectronMomSliceLimits.at(" << Slice << ").at(0) = " << Loaded_ElectronMomSliceLimits.at(Slice).at(0) << "\n";
                                    std::cout << "Loaded_ElectronMomSliceLimits.at(" << Slice << ").at(1) = " << Loaded_ElectronMomSliceLimits.at(Slice).at(1) << "\n\n";
                                }

                                return (Loaded_e_AMap_Slices_extended.at(Slice).at(i).at(j) == 1);
                            } else {
                                if (Loaded_e_AMap_Slices.size() == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_e_AMap_Slices has size 0!"); }
                                if (Loaded_e_AMap_Slices.at(Slice).size() == 0) {
                                    bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_e_AMap_Slices.at(" + std::to_string(Slice) + ") has size 0!");
                                }

                                if (db::AMapsDebuggerMode) {
                                    std::cout << "\n\nLoaded_e_AMap_Slices.size() = " << Loaded_e_AMap_Slices.size() << "\n";
                                    std::cout << "Loaded_e_AMap_Slices.at(" << Slice << ").size() = " << Loaded_e_AMap_Slices.at(Slice).size() << "\n";
                                    std::cout << "Loaded_e_AMap_Slices.at(" << Slice << ").at(" << i << ").size() = " << Loaded_e_AMap_Slices.at(Slice).at(i).size() << "\n";
                                    std::cout << "Loaded_ElectronMomSliceLimits.at(" << Slice << ").at(0) = " << Loaded_ElectronMomSliceLimits.at(Slice).at(0) << "\n";
                                    std::cout << "Loaded_ElectronMomSliceLimits.at(" << Slice << ").at(1) = " << Loaded_ElectronMomSliceLimits.at(Slice).at(1) << "\n\n";
                                }

                                return (Loaded_e_AMap_Slices.at(Slice).at(i).at(j) == 1);
                            }
                        }  // end of find right phi if
                    }
                }  // end of find right theta if
            }
        }  // end of if the right momentum
    }

    return false;
}
*/
#pragma endregion

// MatchToNucleonAMap function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* MatchToNucleonAMap function */
/**
 * @brief Check whether a proton or neutron falls inside the loaded nucleon acceptance map.
 *
 * Purpose:
 *   Convert the nucleon kinematics `(Momentum, Theta, Phi)` into a momentum-slice index
 *   and a 2D angular-bin index, then query the corresponding loaded proton map,
 *   neutron map, or proton-neutron overlap map.
 *
 * Algorithm:
 *   1. Determine whether the query is for a proton or neutron.
 *   2. Validate that nucleon momentum slices and angular binning were loaded correctly.
 *   3. Restrict the slice search range to the requested `[InitialSlice, FinalSlice)` interval.
 *   4. Reject points that are outside the global `(Theta, Phi)` map limits.
 *   5. Convert `Theta` and `Phi` directly into matrix indices using bin arithmetic:
 *        bin = floor((value - lower_limit) / bin_width)
 *      This avoids looping over all theta and phi bins.
 *   6. Use a binary search over the sorted nucleon momentum-slice limits to find the
 *      unique slice that contains `Momentum`. This avoids scanning all momentum slices linearly.
 *   7. If `NucleonOverlappingFC` is enabled, require both the proton and neutron-const map
 *      bins to be accepted in the matched slice.
 *   8. Otherwise, query only the proton map for proton requests or only the neutron-const
 *      map for neutron requests.
 *   9. Return `true` if the selected map cell is accepted, otherwise return `false`.
 *
 * Performance notes:
 *   The original implementation used nested scans over momentum slices, theta bins, and phi bins.
 *   This version reduces the lookup cost by:
 *     - replacing theta-bin and phi-bin scans with O(1) index calculations
 *     - replacing the momentum-slice linear scan with an O(log N) binary search
 *   This is much faster when running over many slices, many angular bins, and millions of events.
 *
 * @param Particle Particle label used to decide whether the query should use proton or neutron maps.
 * @param Momentum Nucleon momentum used to choose the correct momentum slice.
 * @param Theta Nucleon polar angle used to choose the correct map row.
 * @param Phi Nucleon azimuthal angle used to choose the correct map column.
 * @param InitialSlice First slice index to consider, inclusive.
 * @param FinalSlice Last slice boundary to consider, exclusive.
 * @param NucleonOverlappingFC If true, require the matched bin to be accepted in both the proton map and the neutron const-slice map.
 *
 * @return True if the nucleon lies inside the selected acceptance-map bin and that bin is marked as accepted.
 *         False if no matching slice is found, the coordinates are outside the map, or the selected bin is rejected.
 */
bool AMaps::MatchToNucleonAMap(const std::string& Particle, double Momentum, double Theta, double Phi, int InitialSlice, int FinalSlice, bool NucleonOverlappingFC) {
    // TODO: figure out if neutron acceptance maps should be in momentum slices or not, due to bin migration
    // TODO: decide which neutron maps to use here - const or not

    // Determine whether this query should use proton maps or neutron maps.
    const bool protonQuery = isProton(Particle);

    // Optionally print how many nucleon momentum slices are currently loaded.
    if (db::AMapsDebuggerMode) { std::cout << "\n\nLoaded_NucleonMomSliceLimits.size() = " << Loaded_NucleonMomSliceLimits.size() << "\n\n"; }

    // Initialize the matched momentum-slice index and angular-bin indices.
    int matchedSlice = -1, YBinInd = -1, XBinInd = -1;

    // Run the shared fast lookup algorithm to find the matched slice and angular-bin indices.
    if (!FindSliceAndBinIndices(Loaded_NucleonMomSliceLimits, HistNucSliceNumOfYBins, HistNucSliceNumOfXBins, Momentum, Theta, Phi, InitialSlice, FinalSlice, matchedSlice, YBinInd, XBinInd,
                                "MatchToNucleonAMap")) {
        return false;
    }

    // If overlap mode is enabled, require both the proton and neutron const-slice bins to be accepted.
    if (NucleonOverlappingFC) {
        // Make sure the proton map container was loaded.
        if (Loaded_p_AMap_Slices.empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_p_AMap_Slices has size 0!"); }

        // Make sure the neutron const-slice map container was loaded.
        if (Loaded_n_AMap_ConstSlices.empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_n_AMap_ConstSlices has size 0!"); }

        // Make sure the matched slice exists in the proton map container.
        if (matchedSlice >= static_cast<int>(Loaded_p_AMap_Slices.size())) { bt::ExitWithError(__func__, __FILE__, __LINE__, "matchedSlice is out of range for Loaded_p_AMap_Slices"); }

        // Make sure the matched slice exists in the neutron const-slice map container.
        if (matchedSlice >= static_cast<int>(Loaded_n_AMap_ConstSlices.size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "matchedSlice is out of range for Loaded_n_AMap_ConstSlices");
        }

        // Make sure the selected proton-map slice is not empty.
        if (Loaded_p_AMap_Slices.at(matchedSlice).empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_p_AMap_Slices.at(" + std::to_string(matchedSlice) + ") has size 0!"); }

        // Make sure the selected neutron const-slice map is not empty.
        if (Loaded_n_AMap_ConstSlices.at(matchedSlice).empty()) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_n_AMap_ConstSlices.at(" + std::to_string(matchedSlice) + ") has size 0!");
        }

        // Make sure the requested theta row exists inside the selected proton-map slice.
        if (YBinInd >= static_cast<int>(Loaded_p_AMap_Slices.at(matchedSlice).size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "YBinInd is out of range for Loaded_p_AMap_Slices.at(" + std::to_string(matchedSlice) + ")");
        }

        // Make sure the requested theta row exists inside the selected neutron const-slice map.
        if (YBinInd >= static_cast<int>(Loaded_n_AMap_ConstSlices.at(matchedSlice).size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "YBinInd is out of range for Loaded_n_AMap_ConstSlices.at(" + std::to_string(matchedSlice) + ")");
        }

        // Make sure the requested phi column exists inside the selected proton-map row.
        if (XBinInd >= static_cast<int>(Loaded_p_AMap_Slices.at(matchedSlice).at(YBinInd).size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "XBinInd is out of range for Loaded_p_AMap_Slices.at(" + std::to_string(matchedSlice) + ").at(" + std::to_string(YBinInd) + ")");
        }

        // Make sure the requested phi column exists inside the selected neutron const-slice row.
        if (XBinInd >= static_cast<int>(Loaded_n_AMap_ConstSlices.at(matchedSlice).at(YBinInd).size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__,
                              "XBinInd is out of range for Loaded_n_AMap_ConstSlices.at(" + std::to_string(matchedSlice) + ").at(" + std::to_string(YBinInd) + ")");
        }

        // Print detailed debugging information for the selected overlap maps and bin if debugging is enabled.
        if (db::AMapsDebuggerMode) {
            std::cout << "\n\nLoaded_n_AMap_ConstSlices.size() = " << Loaded_n_AMap_ConstSlices.size() << "\n";
            std::cout << "Loaded_n_AMap_ConstSlices.at(" << matchedSlice << ").size() = " << Loaded_n_AMap_ConstSlices.at(matchedSlice).size() << "\n";
            std::cout << "Loaded_n_AMap_ConstSlices.at(" << matchedSlice << ").at(" << YBinInd << ").size() = " << Loaded_n_AMap_ConstSlices.at(matchedSlice).at(YBinInd).size() << "\n";
            std::cout << "Loaded_p_AMap_Slices.size() = " << Loaded_p_AMap_Slices.size() << "\n";
            std::cout << "Loaded_p_AMap_Slices.at(" << matchedSlice << ").size() = " << Loaded_p_AMap_Slices.at(matchedSlice).size() << "\n";
            std::cout << "Loaded_p_AMap_Slices.at(" << matchedSlice << ").at(" << YBinInd << ").size() = " << Loaded_p_AMap_Slices.at(matchedSlice).at(YBinInd).size() << "\n";
            std::cout << "Loaded_NucleonMomSliceLimits.at(" << matchedSlice << ").at(0) = " << Loaded_NucleonMomSliceLimits.at(matchedSlice).at(0) << "\n";
            std::cout << "Loaded_NucleonMomSliceLimits.at(" << matchedSlice << ").at(1) = " << Loaded_NucleonMomSliceLimits.at(matchedSlice).at(1) << "\n\n";
        }

        // Return true only if both overlap-map bins are marked as accepted.
        return ((Loaded_p_AMap_Slices.at(matchedSlice).at(YBinInd).at(XBinInd) == 1) && (Loaded_n_AMap_ConstSlices.at(matchedSlice).at(YBinInd).at(XBinInd) == 1));
    }

    // Branch to the proton-map query for proton requests.
    if (protonQuery) {
        // Make sure the proton map container was loaded.
        if (Loaded_p_AMap_Slices.empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_p_AMap_Slices has size 0!"); }

        // Make sure the matched slice exists in the proton map container.
        if (matchedSlice >= static_cast<int>(Loaded_p_AMap_Slices.size())) { bt::ExitWithError(__func__, __FILE__, __LINE__, "matchedSlice is out of range for Loaded_p_AMap_Slices"); }

        // Make sure the selected proton-map slice is not empty.
        if (Loaded_p_AMap_Slices.at(matchedSlice).empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_p_AMap_Slices.at(" + std::to_string(matchedSlice) + ") has size 0!"); }

        // Make sure the requested theta row exists inside the selected proton-map slice.
        if (YBinInd >= static_cast<int>(Loaded_p_AMap_Slices.at(matchedSlice).size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "YBinInd is out of range for Loaded_p_AMap_Slices.at(" + std::to_string(matchedSlice) + ")");
        }

        // Make sure the requested phi column exists inside the selected proton-map row.
        if (XBinInd >= static_cast<int>(Loaded_p_AMap_Slices.at(matchedSlice).at(YBinInd).size())) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "XBinInd is out of range for Loaded_p_AMap_Slices.at(" + std::to_string(matchedSlice) + ").at(" + std::to_string(YBinInd) + ")");
        }

        // Print detailed debugging information for the selected proton map and bin if debugging is enabled.
        if (db::AMapsDebuggerMode) {
            std::cout << "\nLoaded_p_AMap_Slices.at(" << matchedSlice << ").size() = " << Loaded_p_AMap_Slices.at(matchedSlice).size() << "\n";
            std::cout << "Loaded_p_AMap_Slices.at(" << matchedSlice << ").at(" << YBinInd << ").size() = " << Loaded_p_AMap_Slices.at(matchedSlice).at(YBinInd).size() << "\n";
            std::cout << "Loaded_NucleonMomSliceLimits.at(" << matchedSlice << ").at(0) = " << Loaded_NucleonMomSliceLimits.at(matchedSlice).at(0) << "\n";
            std::cout << "Loaded_NucleonMomSliceLimits.at(" << matchedSlice << ").at(1) = " << Loaded_NucleonMomSliceLimits.at(matchedSlice).at(1) << "\n\n";
        }

        // Return true only if the selected proton-map bin is marked as accepted.
        return (Loaded_p_AMap_Slices.at(matchedSlice).at(YBinInd).at(XBinInd) == 1);
    }

    // Make sure the neutron const-slice map container was loaded.
    if (Loaded_n_AMap_ConstSlices.empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_n_AMap_ConstSlices has size 0!"); }

    // Make sure the matched slice exists in the neutron const-slice map container.
    if (matchedSlice >= static_cast<int>(Loaded_n_AMap_ConstSlices.size())) { bt::ExitWithError(__func__, __FILE__, __LINE__, "matchedSlice is out of range for Loaded_n_AMap_ConstSlices"); }

    // Make sure the selected neutron const-slice map is not empty.
    if (Loaded_n_AMap_ConstSlices.at(matchedSlice).empty()) {
        bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_n_AMap_ConstSlices.at(" + std::to_string(matchedSlice) + ") has size 0!");
    }

    // Make sure the requested theta row exists inside the selected neutron const-slice map.
    if (YBinInd >= static_cast<int>(Loaded_n_AMap_ConstSlices.at(matchedSlice).size())) {
        bt::ExitWithError(__func__, __FILE__, __LINE__, "YBinInd is out of range for Loaded_n_AMap_ConstSlices.at(" + std::to_string(matchedSlice) + ")");
    }

    // Make sure the requested phi column exists inside the selected neutron const-slice row.
    if (XBinInd >= static_cast<int>(Loaded_n_AMap_ConstSlices.at(matchedSlice).at(YBinInd).size())) {
        bt::ExitWithError(__func__, __FILE__, __LINE__, "XBinInd is out of range for Loaded_n_AMap_ConstSlices.at(" + std::to_string(matchedSlice) + ").at(" + std::to_string(YBinInd) + ")");
    }

    // Print detailed debugging information for the selected neutron const-slice map and bin if debugging is enabled.
    if (db::AMapsDebuggerMode) {
        std::cout << "\n\nLoaded_n_AMap_ConstSlices.size() = " << Loaded_n_AMap_ConstSlices.size() << "\n";
        std::cout << "Loaded_n_AMap_ConstSlices.at(" << matchedSlice << ").size() = " << Loaded_n_AMap_ConstSlices.at(matchedSlice).size() << "\n";
        std::cout << "Loaded_n_AMap_ConstSlices.at(" << matchedSlice << ").at(" << YBinInd << ").size() = " << Loaded_n_AMap_ConstSlices.at(matchedSlice).at(YBinInd).size() << "\n";
        std::cout << "Loaded_NucleonMomSliceLimits.at(" << matchedSlice << ").at(0) = " << Loaded_NucleonMomSliceLimits.at(matchedSlice).at(0) << "\n";
        std::cout << "Loaded_NucleonMomSliceLimits.at(" << matchedSlice << ").at(1) = " << Loaded_NucleonMomSliceLimits.at(matchedSlice).at(1) << "\n\n";
    }

    // Return true only if the selected neutron const-slice bin is marked as accepted.
    return (Loaded_n_AMap_ConstSlices.at(matchedSlice).at(YBinInd).at(XBinInd) == 1);
}

/*
bool AMaps::MatchToNucleonAMap(const std::string& Particle, double Momentum, double Theta, double Phi, int InitialSlice, int FinalSlice, bool NucleonOverlappingFC) {
    const bool protonQuery = isProton(Particle);

    // TODO: figure out if neutron acceptance maps should be in momentum slices or not, due to bin migration
    // TODO: decide which neutron maps to use here - const or not

    if (db::AMapsDebuggerMode) { std::cout << "\n\nLoaded_NucleonMomSliceLimits.size() = " << Loaded_NucleonMomSliceLimits.size() << "\n\n"; }

    for (int Slice = InitialSlice; Slice < FinalSlice; Slice++) {
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
                                if (Loaded_p_AMap_Slices.size() == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_p_AMap_Slices has size 0!"); }
                                if (Loaded_n_AMap_ConstSlices.size() == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_n_AMap_ConstSlices has size 0!"); }

                                if (Loaded_p_AMap_Slices.at(Slice).size() == 0) {
                                    bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_p_AMap_Slices.at(" + std::to_string(Slice) + ") has size 0!");
                                }
                                if (Loaded_n_AMap_ConstSlices.at(Slice).size() == 0) {
                                    bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_n_AMap_ConstSlices.at(" + std::to_string(Slice) + ") has size 0!");
                                }

                                if (db::AMapsDebuggerMode) {
                                    std::cout << "\n\nLoaded_n_AMap_ConstSlices.size() = " << Loaded_n_AMap_ConstSlices.size() << "\n";
                                    std::cout << "Loaded_n_AMap_ConstSlices.at(" << Slice << ").size() = " << Loaded_n_AMap_ConstSlices.at(Slice).size() << "\n";
                                    std::cout << "Loaded_n_AMap_ConstSlices.at(" << Slice << ").at(" << i << ").size() = " << Loaded_n_AMap_ConstSlices.at(Slice).at(i).size() << "\n";
                                    std::cout << "Loaded_p_AMap_Slices.size() = " << Loaded_p_AMap_Slices.size() << "\n";
                                    std::cout << "Loaded_p_AMap_Slices.at(" << Slice << ").size() = " << Loaded_p_AMap_Slices.at(Slice).size() << "\n";
                                    std::cout << "Loaded_p_AMap_Slices.at(" << Slice << ").at(" << i << ").size() = " << Loaded_p_AMap_Slices.at(Slice).at(i).size() << "\n";
                                    std::cout << "Loaded_NucleonMomSliceLimits.at(" << Slice << ").at(0) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                    std::cout << "Loaded_NucleonMomSliceLimits.at(" << Slice << ").at(1) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";
                                }

                                return ((Loaded_p_AMap_Slices.at(Slice).at(i).at(j) == 1) && (Loaded_n_AMap_ConstSlices.at(Slice).at(i).at(j) == 1));
                            }

                            if (protonQuery) {
                                if (db::AMapsDebuggerMode) {
                                    std::cout << "\nLoaded_p_AMap_Slices.at(" << Slice << ").size() = " << Loaded_p_AMap_Slices.at(Slice).size() << "\n";
                                    std::cout << "Loaded_p_AMap_Slices.at(" << Slice << ").at(" << i << ").size() = " << Loaded_p_AMap_Slices.at(Slice).at(i).size() << "\n";
                                    std::cout << "Loaded_NucleonMomSliceLimits.at(" << Slice << ").at(0) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                    std::cout << "Loaded_NucleonMomSliceLimits.at(" << Slice << ").at(1) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";
                                }

                                return (Loaded_p_AMap_Slices.at(Slice).at(i).at(j) == 1);
                            }

                            if (Loaded_n_AMap_ConstSlices.size() == 0) { bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_n_AMap_ConstSlices has size 0!"); }

                            if (Loaded_n_AMap_ConstSlices.at(Slice).size() == 0) {
                                bt::ExitWithError(__func__, __FILE__, __LINE__, "Loaded_n_AMap_ConstSlices.at(" + std::to_string(Slice) + ") has size 0!");
                            }

                            if (db::AMapsDebuggerMode) {
                                std::cout << "\n\nLoaded_n_AMap_ConstSlices.size() = " << Loaded_n_AMap_ConstSlices.size() << "\n";
                                std::cout << "Loaded_n_AMap_ConstSlices.at(" << Slice << ").size() = " << Loaded_n_AMap_ConstSlices.at(Slice).size() << "\n";
                                std::cout << "Loaded_n_AMap_ConstSlices.at(" << Slice << ").at(" << i << ").size() = " << Loaded_n_AMap_ConstSlices.at(Slice).at(i).size() << "\n";
                                std::cout << "Loaded_NucleonMomSliceLimits.at(" << Slice << ").at(0) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(0) << "\n";
                                std::cout << "Loaded_NucleonMomSliceLimits.at(" << Slice << ").at(1) = " << Loaded_NucleonMomSliceLimits.at(Slice).at(1) << "\n\n";
                            }

                            return (Loaded_n_AMap_ConstSlices.at(Slice).at(i).at(j) == 1);
                        }  // end of find right phi if
                    }
                }  // end of find right theta if
            }
        }  // end of if the right momentum
    }

    return false;
}
*/
#pragma endregion

// MatchToAMap function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* MatchToAMap function (original with neutron FC 'bug') */
/**
 * @brief Dispatch function that routes a particle to the correct acceptance-map matcher.
 *
 * Purpose:
 *   This function acts as a central interface that decides which specific
 *   acceptance-map matching routine should be called based on the particle type.
 *   It determines the slice range to test and then forwards the kinematics
 *   (Momentum, Theta, Phi) to the appropriate matching function.
 *
 * High‑level algorithm:
 *   1. Initialize default slice ranges for electrons, protons, and neutrons.
 *   2. Optionally override these ranges if "single slice test" debugging modes
 *      are enabled.
 *   3. If slice-testing configuration is invalid, disable the test mode safely.
 *   4. If slice-testing is enabled, restrict the slice range to the selected slice.
 *   5. Determine the particle type.
 *   6. Dispatch the kinematics to the corresponding matcher:
 *        - electrons → MatchToElectronAMap
 *        - protons   → MatchToNucleonAMap (proton branch)
 *        - neutrons  → MatchToNucleonAMap (neutron branch)
 *   7. Return the result of the selected matcher.
 *
 * Notes:
 *   - This function itself does not perform any acceptance lookup.
 *   - It only prepares slice ranges and delegates the heavy computation.
 *   - The neutron "bug" mentioned in the region label refers to historical
 *     behavior where neutron maps were treated differently from proton maps.
 */
bool AMaps::MatchToAMap(const std::string& Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC, bool UseExtendedElectronMaps) {
    // Define the default slice range for electrons. By default we search all slices.
    int e_InitialSlice = 0, e_FinalSlice = Loaded_ElectronMomSliceLimits.size();

    // Define the default slice ranges for protons and neutrons (also search all slices).
    int p_InitialSlice = 0, p_FinalSlice = Loaded_NucleonMomSliceLimits.size(), n_InitialSlice = 0, n_FinalSlice = Loaded_NucleonMomSliceLimits.size();

    // These variables hold the specific slice indices used when "single slice test" debugging mode is enabled.
    int e_TestSlice = 1, p_TestSlice = 1, n_TestSlice = 1;

    // Validate that slice-testing configuration is correct.
    // If slice-testing is enabled but the slice vector does not contain the required {e,p,n} entries,
    // disable the test mode to avoid out-of-range errors.
    if ((e_single_slice_test || nuc_single_slice_test) && Slices2Test.size() < 3) {
        bt::PrintWarning(__func__, __FILE__, __LINE__, "single-slice test mode requires TestSlices to contain 3 entries {e,p,n}; disabling single-slice tests for this run.");

        // Disable electron single-slice testing if configuration is invalid.
        e_single_slice_test = false;
        // Disable nucleon single-slice testing if configuration is invalid.
        nuc_single_slice_test = false;
    }

    // If valid slice indices were provided, extract the slice numbers for
    // electrons, protons, and neutrons from the configuration vector.
    if (Slices2Test.size() >= 3) {
        e_TestSlice = Slices2Test.at(0);
        p_TestSlice = Slices2Test.at(1);
        n_TestSlice = Slices2Test.at(2);
    }

    // Restrict the electron slice search to a single slice when debugging mode is enabled.
    if (e_single_slice_test) { e_InitialSlice = e_TestSlice - 1, e_FinalSlice = e_TestSlice; }

    // Restrict proton and neutron slice searches to a single slice when debugging mode is enabled.
    if (nuc_single_slice_test) { p_InitialSlice = p_TestSlice - 1, p_FinalSlice = p_TestSlice, n_InitialSlice = n_TestSlice - 1, n_FinalSlice = n_TestSlice; }

    // Dispatch electron queries to the electron acceptance-map matcher.
    if (isElectron(Particle)) {
        return MatchToElectronAMap(Momentum, Theta, Phi, e_InitialSlice, e_FinalSlice, UseExtendedElectronMaps);
    }
    // Dispatch proton queries to the nucleon matcher using the proton slice range.
    else if (isProton(Particle)) {
        return MatchToNucleonAMap(Particle, Momentum, Theta, Phi, p_InitialSlice, p_FinalSlice, NucleonOverlappingFC);
    }
    // Dispatch neutron queries to the nucleon matcher using the neutron slice range.
    else if (isNeutron(Particle)) {
        return MatchToNucleonAMap(Particle, Momentum, Theta, Phi, n_InitialSlice, n_FinalSlice, NucleonOverlappingFC);
    }  // end of if Particle

    // If the particle type was not recognized, return false (no map match).
    return false;
}
#pragma endregion

// GetNucleonWeight function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetNucleonWeight */
/**
 * @brief Return the kinematical acceptance weight for a proton or neutron.
 *
 * Purpose:
 *   Convert the nucleon kinematics `(Momentum, Theta, Phi)` into a momentum-slice index
 *   and a 2D angular-bin index, then read the corresponding weight-map cell and return
 *   its inverse, `1 / epsilon`, as the event weight.
 *
 * Algorithm:
 *   1. Determine whether the query is for a proton or neutron.
 *   2. Validate that nucleon momentum slices, angular binning, and weight-map containers
 *      were loaded correctly.
 *   3. Reject points that are outside the global `(Theta, Phi)` map limits.
 *   4. Convert `Theta` and `Phi` directly into matrix indices using bin arithmetic:
 *        bin = floor((value - lower_limit) / bin_width)
 *      This avoids looping over all theta and phi bins.
 *   5. Use a binary search over the sorted nucleon momentum-slice limits to find the
 *      unique slice that contains `Momentum`. This avoids scanning all momentum slices linearly.
 *   6. Read the corresponding proton or neutron weight-map value from the matched cell.
 *   7. If the stored map value is nonzero, return its inverse as the correction weight.
 *   8. If the map value is zero or no valid slice/bin is found, return 0.
 *
 * Performance notes:
 *   The original implementation used nested scans over momentum slices, theta bins, and phi bins.
 *   This version reduces the lookup cost by:
 *     - replacing theta-bin and phi-bin scans with O(1) index calculations
 *     - replacing the momentum-slice linear scan with an O(log N) binary search
 *   This is much faster when running over many slices, many angular bins, and millions of events.
 *
 * Notes:
 *   - The returned quantity is `1 / mapWeight`, not the raw map value itself.
 *   - A return value of 0 means that no usable correction weight was found.
 *   - The current implementation keeps the same proton vs neutron behavior as before:
 *     proton queries use `Loaded_p_WMap_Slices`, while neutron queries use `Loaded_n_WMap_Slices`.
 *
 * @param Particle Particle label used to decide whether the query should use proton or neutron weight maps.
 * @param Momentum Nucleon momentum used to choose the correct momentum slice.
 * @param Theta Nucleon polar angle used to choose the correct map row.
 * @param Phi Nucleon azimuthal angle used to choose the correct map column.
 * @param PrintOut If true, print detailed debugging information for the matched slice, bin, and weight.
 *
 * @return The inverse map weight `1 / epsilon` for the matched nucleon bin, or 0 if no valid weight exists.
 */
double AMaps::GetNucleonWeight(const std::string& Particle, double Momentum, double Theta, double Phi, bool PrintOut) {
    // Determine whether this query should use proton weight maps or neutron weight maps.
    const bool protonQuery = isProton(Particle);

    // TODO: figure out if neutron weight maps should be in momentum slices or not, due to bin migration
    // TODO: decide which neutron maps to use here - const or not

    // Select the appropriate weight-map container based on the particle type.
    const auto& weightSlices = protonQuery ? Loaded_p_WMap_Slices : Loaded_n_WMap_Slices;

    // Make sure the selected weight-map container was loaded.
    if (weightSlices.empty()) { bt::ExitWithError(__func__, __FILE__, __LINE__, protonQuery ? "Loaded_p_WMap_Slices has size 0!" : "Loaded_n_WMap_Slices has size 0!"); }

    // Initialize the matched momentum-slice index and angular-bin indices.
    int matchedSlice = -1, YBinInd = -1, XBinInd = -1;

    // Run the shared fast lookup algorithm to find the matched slice and angular-bin indices.
    if (!FindSliceAndBinIndices(Loaded_NucleonMomSliceLimits, HistNucSliceNumOfYBins, HistNucSliceNumOfXBins, Momentum, Theta, Phi, 0, static_cast<int>(Loaded_NucleonMomSliceLimits.size()),
                                matchedSlice, YBinInd, XBinInd, "GetNucleonWeight")) {
        return 0;
    }

    // Make sure the matched slice exists in the selected weight-map container.
    if (matchedSlice >= static_cast<int>(weightSlices.size())) {
        bt::ExitWithError(__func__, __FILE__, __LINE__, protonQuery ? "matchedSlice is out of range for Loaded_p_WMap_Slices" : "matchedSlice is out of range for Loaded_n_WMap_Slices");
    }

    // Make sure the selected weight-map slice is not empty.
    if (weightSlices.at(matchedSlice).empty()) {
        bt::ExitWithError(__func__, __FILE__, __LINE__,
                          (protonQuery ? std::string("Loaded_p_WMap_Slices.at(") : std::string("Loaded_n_WMap_Slices.at(")) + std::to_string(matchedSlice) + ") has size 0!");
    }

    // Make sure the requested theta row exists inside the selected weight-map slice.
    if (YBinInd >= static_cast<int>(weightSlices.at(matchedSlice).size())) {
        bt::ExitWithError(__func__, __FILE__, __LINE__,
                          (protonQuery ? std::string("YBinInd is out of range for Loaded_p_WMap_Slices.at(") : std::string("YBinInd is out of range for Loaded_n_WMap_Slices.at(")) +
                              std::to_string(matchedSlice) + ")");
    }

    // Make sure the requested phi column exists inside the selected weight-map row.
    if (XBinInd >= static_cast<int>(weightSlices.at(matchedSlice).at(YBinInd).size())) {
        bt::ExitWithError(__func__, __FILE__, __LINE__,
                          (protonQuery ? std::string("XBinInd is out of range for Loaded_p_WMap_Slices.at(") : std::string("XBinInd is out of range for Loaded_n_WMap_Slices.at(")) +
                              std::to_string(matchedSlice) + ").at(" + std::to_string(YBinInd) + ")");
    }

    // Read the raw map value from the matched slice and angular bin.
    const double mapWeight = weightSlices.at(matchedSlice).at(YBinInd).at(XBinInd);

    // Initialize the return value to zero, which means "no usable weight".
    double Weight = 0;

    // If the stored map value is nonzero, return its inverse as the correction weight.
    if (mapWeight != 0) {
        Weight = 1 / mapWeight;

        // Optionally print detailed debugging information for the matched slice, bin, and weight.
        if (PrintOut) {
            const double thetaInterval = hBinUpperYLim - hBinLowerYLim;
            const double phiInterval = hBinUpperXLim - hBinLowerXLim;
            const double thetaBinWidth = thetaInterval / HistNucSliceNumOfYBins;
            const double phiBinWidth = phiInterval / HistNucSliceNumOfXBins;
            const double ThetaLowerLimTemp = hBinLowerYLim + YBinInd * thetaBinWidth;
            const double ThetaUpperLimTemp = ThetaLowerLimTemp + thetaBinWidth;
            const double PhiLowerLimTemp = hBinLowerXLim + XBinInd * phiBinWidth;
            const double PhiUpperLimTemp = PhiLowerLimTemp + phiBinWidth;

            std::cout << "MomentumLowerLimTemp = " << Loaded_NucleonMomSliceLimits.at(matchedSlice).at(0) << "\n";
            std::cout << "Momentum = " << Momentum << "\n";
            std::cout << "MomentumUpperLimTemp = " << Loaded_NucleonMomSliceLimits.at(matchedSlice).at(1) << "\n\n";

            std::cout << "ThetaLowerLimTemp = " << ThetaLowerLimTemp << "\n";
            std::cout << "Theta = " << Theta << "\n";
            std::cout << "ThetaUpperLimTemp = " << ThetaUpperLimTemp << "\n\n";

            std::cout << "PhiLowerLimTemp = " << PhiLowerLimTemp << "\n";
            std::cout << "Phi = " << Phi << "\n";
            std::cout << "PhiUpperLimTemp = " << PhiUpperLimTemp << "\n\n";

            std::cout << "Weight = " << Weight << "\n\n";
            std::cout << "1/Weight = " << (1 / Weight) << "\n\n\n\n";
        }
    }

    // Return the final correction weight, or 0 if no valid map value existed.
    return Weight;
}
#pragma endregion

// GetWeight function ---------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* GetWeight function */
/**
 * @brief Return the kinematical weight for a reconstructed particle.
 *
 * Purpose:
 *   Provide a single public interface for event-by-event kinematical weighting.
 *   Depending on the particle type and on whether kinematical weighting is enabled,
 *   this function either returns a particle-specific correction weight or returns 1.
 *
 * Algorithm:
 *   1. Check whether kinematical weighting is enabled.
 *   2. If weighting is disabled, return 1 immediately.
 *   3. If weighting is enabled:
 *        - electrons return 1
 *        - protons  call `GetNucleonWeight(...)`
 *        - neutrons call `GetNucleonWeight(...)`
 *   4. If the particle type is not recognized, return 0.
 *
 * Notes:
 *   - In the current implementation, electrons do not receive a kinematical map weight.
 *   - A return value of 1 means "no correction applied".
 *   - A return value of 0 means the particle type was not recognized or no valid nucleon
 *     weight could be produced downstream.
 *
 * @param apply_kinematical_weights If true, apply particle-dependent kinematical weights.
 * @param Particle Particle label used to decide which weighting path to use.
 * @param Momentum Particle momentum passed to the downstream weighting routine.
 * @param Theta Particle polar angle passed to the downstream weighting routine.
 * @param Phi Particle azimuthal angle passed to the downstream weighting routine.
 *
 * @return The particle weight to apply in the analysis.
 */
double AMaps::GetWeight(bool apply_kinematical_weights, const std::string& Particle, double Momentum, double Theta, double Phi) {
    // TODO: figure out if neutron weight maps should be in momentum slices or not, due to bin migration
    // TODO: decide which neutron maps to use here - const or not

    // Control whether detailed weight-printing should be enabled in downstream routines.
    bool PrintOut = false;

    // If kinematical weighting is disabled, return the neutral multiplicative weight.
    if (!apply_kinematical_weights) { return 1; }

    // Electrons currently receive no kinematical acceptance weight, so return 1.
    if (isElectron(Particle)) {
        return 1;
    }
    // Route proton queries to the nucleon-weight routine.
    else if (isProton(Particle)) {
        return GetNucleonWeight(Particle, Momentum, Theta, Phi, PrintOut);
    }
    // Route neutron queries to the nucleon-weight routine.
    else if (isNeutron(Particle)) {
        return GetNucleonWeight(Particle, Momentum, Theta, Phi, PrintOut);
    }

    // If the particle type was not recognized, return 0 to signal that no valid weight was found.
    return 0;
}
#pragma endregion

// IsInFDQuery function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* IsInFDQuery function */
/**
 * @brief Determine whether a particle satisfies the Forward Detector (FD) query.
 *
 * Purpose:
 *   This function evaluates whether a particle should be considered inside the
 *   Forward Detector region according to both geometrical limits and acceptance
 *   map conditions.
 *
 * Algorithm:
 *   1. Optionally test whether the particle falls inside any acceptance-map bin
 *      using `MatchToAMap(...)`. This step is skipped when acceptance maps are
 *      currently being generated (`Generate_AMaps == true`).
 *   2. Check whether the particle polar angle `Theta` lies within the FD angular
 *      limits defined by the `ThetaFD` cut object.
 *   3. Return `true` only if both conditions are satisfied:
 *        - the particle belongs to an accepted sector (or map generation mode is active)
 *        - the particle angle lies within the FD angular limits
 *
 * Notes:
 *   - When `Generate_AMaps` is enabled, the acceptance-map test is bypassed so
 *     that all particles passing the angular FD cut can contribute to the map
 *     generation procedure.
 *   - Detailed debugging messages can be printed through the `CodeDebugger`
 *     interface when debugging modes are enabled.
 *
 * @param Generate_AMaps If true, bypass acceptance-map matching while maps are being generated.
 * @param ThetaFD Angular cut object defining the FD theta limits.
 * @param Particle Particle label used for map matching.
 * @param Momentum Particle momentum used in map matching.
 * @param Theta Particle polar angle used for FD selection and map matching.
 * @param Phi Particle azimuthal angle used in map matching.
 * @param NucleonOverlappingFC If true, require proton and neutron maps to overlap for nucleon matching.
 * @param UseExtendedElectronMaps If true, use the extended electron acceptance maps.
 *
 * @return True if the particle satisfies the FD query conditions, otherwise false.
 */
bool AMaps::IsInFDQuery(bool Generate_AMaps, const DSCuts& ThetaFD, const std::string& Particle, double Momentum, double Theta, double Phi, bool NucleonOverlappingFC,
                        bool UseExtendedElectronMaps) {
    std::string DebuggingBaseString = "[AMaps::IsInFDQuery]: " + Particle + "\nMomentum: " + bt::ToStringWithPrecision(Momentum, 2) +
                                      " [GeV/c]\nTheta: " + bt::ToStringWithPrecision(Theta, 2) + " [rad]\nPhi: " + bt::ToStringWithPrecision(Phi, 2) + " deg\nMode = " + Maps_Mode;
    db::CodeDebugger.PrintStepTester(__FILE__, __LINE__, db::DebuggerMode, db::OnlyPrintNamedTesterSteps, (DebuggingBaseString + " - Start"));

    bool inFDQuery, part_inSomeSector;

    if (!Generate_AMaps) {
        part_inSomeSector = MatchToAMap(Particle, Momentum, Theta, Phi, NucleonOverlappingFC, UseExtendedElectronMaps);
    } else {
        part_inSomeSector = true;
    }

    inFDQuery = (part_inSomeSector && (Theta >= ThetaFD.GetLowerCutConst()) && (Theta <= ThetaFD.GetUpperCutConst()));

    db::CodeDebugger.PrintStepTester(__FILE__, __LINE__, db::DebuggerMode, db::OnlyPrintNamedTesterSteps, (DebuggingBaseString + " - End"));

    return inFDQuery;
}
#pragma endregion
