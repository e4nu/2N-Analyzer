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
    int NumberNucOfMomSlices = 15, NumberElecOfMomSlices = 45, HistElectronSliceNumOfXBins = 120, HistNucSliceNumOfXBins = 120;
    // int NumberNucOfMomSlices = 15, NumberElecOfMomSlices = 15, HistElectronSliceNumOfXBins = 100, HistNucSliceNumOfXBins = 100;

    AcceptanceMapsSettings()
        : Generate_Electron_AMaps(true),
          Generate_Nucleon_AMaps(true),
          Generate_WMaps(true),
          AMaps_calc_with_one_reco_electron(true),

          P_e_bin_profile("uniform_P_e_bins"),
          P_nuc_bin_profile("uniform_P_nuc_bins"),

          Electron_single_slice_test(false),
          Nucleon_single_slice_test(false),
          TestSlices({1, 1, 1}) {}

    void RefreshSettingsByParameters(const RunParameters& parameters) {
        if (parameters.isData) { Generate_Electron_AMaps = Generate_Nucleon_AMaps = Generate_WMaps = false; }

        if (Generate_Electron_AMaps && !basic_tools::FindSubstring(parameters.SampleName, "Uniform_1e")) { Generate_Electron_AMaps = false; }

        if (Generate_Nucleon_AMaps && (!basic_tools::FindSubstring(parameters.SampleName, "Uniform_ep")) && !basic_tools::FindSubstring(parameters.SampleName, "Uniform_en")) {
            Generate_Nucleon_AMaps = false;
        }

        if (Generate_Electron_AMaps && Generate_Nucleon_AMaps) {
            std::cout << "\n\n\033[33mAcceptanceMapsSettings::RefreshSettings:\033[31m ERROR!\033[0m Generate_Electron_AMaps and Generate_Nucleon_AMaps can't be true at the same time! "
                         "Aborting...";
            exit(0);
        }
    }

    void RefreshSettingsByEventSelection(const EventSelectionSettings& ESSettings, bool& AMaps_plots, bool& WMaps_plots) {
        if (!ESSettings.calculate_truth_level) { Generate_WMaps = false; }

        if (!Generate_Electron_AMaps && !Generate_Nucleon_AMaps) { AMaps_plots = false; }

        if (!Generate_WMaps) { WMaps_plots = false; }
    }
};

#endif  // ACCEPTANCEMAPSSETTINGS_H
