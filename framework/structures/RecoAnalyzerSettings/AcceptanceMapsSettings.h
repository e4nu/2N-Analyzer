//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef ACCEPTANCEMAPSSETTINGS_H
#define ACCEPTANCEMAPSSETTINGS_H

#include <iostream>

// Include libraries:
#include "../../namespaces/general_utilities/basic_tools.h"

// Include settings:
#include "EventSelectionSettings.h"
#include "RunParameters.h"

namespace bt = basic_tools;

/**
 * @struct AcceptanceMapsSettings
 * @brief Settings related to acceptance maps, including whether to generate electron and nucleon acceptance maps, the binning profiles for electron and nucleon momentum, and settings for
 * single slice tests.
 * @details These settings are used in the main code to control the generation of acceptance maps for electrons and nucleons, as well as the generation of efficiency maps (WMaps). The
 * settings also include options for single slice tests, which can be used to test the acceptance maps in specific momentum slices. The RefreshSettingsByParameters function is used to update
 * the settings based on the run parameters, while the RefreshSettingsByEventSelection function is used to update the settings based on the event selection settings.
 * @note The Generate_Electron_AMaps and Generate_Nucleon_AMaps settings are mutually exclusive, meaning that both cannot be true at the same time. The binning profiles for electron and
 * nucleon momentum can be set to different options, such as uniform or varying bins. The single slice test settings should be set to false for normal runs, and can be used for testing
 * purposes.
 */
struct AcceptanceMapsSettings {
    bool Generate_Electron_AMaps;  // Generate electron acceptance maps
    bool Generate_Nucleon_AMaps;   // Generate nucleon acceptance maps
    bool Generate_WMaps;           // Generate efficiency maps
    bool AMaps_calc_with_one_reco_electron;

    const std::string P_e_bin_profile;    // {reformat_e_bins , varying_P_e_bins , uniform_P_e_bins, equi_inverted_P_e}
    const std::string P_nuc_bin_profile;  // {equi_inverted_P_nuc , varying_P_nuc_bins , uniform_P_nuc_bins}

    bool Electron_single_slice_test;  // keep as false for normal runs!
    bool Nucleon_single_slice_test;   // keep as false for normal runs!
    std::vector<int> TestSlices;      // {ElectronTestSlice, ProtonTestSlice, NeutronTestSlice}

    /* Set Bins by case */
    // int NumberNucOfMomSlices = 25, NumberElecOfMomSlices = 60, HistElectronSliceNumOfXBins = 120, HistNucSliceNumOfXBins = 150;
    // int NumberNucOfMomSlices = 20, NumberElecOfMomSlices = 60, HistElectronSliceNumOfXBins = 120, HistNucSliceNumOfXBins = 100; // 2 GeV nucleons
    int NumberNucOfMomSlices = 25, NumberElecOfMomSlices = 60, HistElectronSliceNumOfXBins = 120, HistNucSliceNumOfXBins = 100;  // 4 and 6 GeV
    // int NumberNucOfMomSlices = 15, NumberElecOfMomSlices = 15, HistElectronSliceNumOfXBins = 100, HistNucSliceNumOfXBins = 100;

    AcceptanceMapsSettings()
        : Generate_Electron_AMaps(false),  // Will remain true if the sample is Uniform_1e_[...]
          Generate_Nucleon_AMaps(false),   // Will remain true if the sample is Uniform_ep/en_[...]
          Generate_WMaps(false),           // Generated with both Uniform_1e_[...] and Uniform_ep/en_[...], for now

          AMaps_calc_with_one_reco_electron(true),

          P_e_bin_profile("uniform_P_e_bins"),
          P_nuc_bin_profile("uniform_P_nuc_bins"),

          Electron_single_slice_test(false),
          Nucleon_single_slice_test(false),
          TestSlices({1, 1, 1}) {}

    void RefreshSettingsByParameters(const RunParameters& parameters) {
        if (parameters.isData) { Generate_Electron_AMaps = Generate_Nucleon_AMaps = Generate_WMaps = false; }

        // Don't generate electron AMaps if the samples is not Uniform_1e_[...]
        if (Generate_Electron_AMaps && !basic_tools::FindSubstring(parameters.SampleName, "Uniform_1e")) { Generate_Electron_AMaps = false; }

        // Don't generate nucleon AMaps if the samples is not Uniform_ep/en_[...]
        if (Generate_Nucleon_AMaps && (!basic_tools::FindSubstring(parameters.SampleName, "Uniform_ep")) && !basic_tools::FindSubstring(parameters.SampleName, "Uniform_en")) {
            Generate_Nucleon_AMaps = false;
        }

        if (Generate_Electron_AMaps && Generate_Nucleon_AMaps) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "Generate_Electron_AMaps and Generate_Nucleon_AMaps can't be true at the same time!");
        }
    }

    void RefreshSettingsByEventSelection(const EventSelectionSettings& ESSettings, bool& AMaps_plots, bool& WMaps_plots) {
        if (!ESSettings.calculate_truth_level) { Generate_WMaps = false; }

        if (!Generate_Electron_AMaps && !Generate_Nucleon_AMaps) { AMaps_plots = false; }

        if (!Generate_WMaps) { WMaps_plots = false; }
    }
};

#endif  // ACCEPTANCEMAPSSETTINGS_H
