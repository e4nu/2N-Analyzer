//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef ACCEPTANCEMAPSSETTINGS_H
#define ACCEPTANCEMAPSSETTINGS_H

#include <iostream>

// Include settings:
#include "../RunParameters.h"

// Include libraries:
#include "../../namespaces/general_utilities/basic_tools.h"

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

    AcceptanceMapsSettings()
        : Generate_Electron_AMaps(false),
          Generate_Nucleon_AMaps(false),
          Generate_WMaps(false),
          AMaps_calc_with_one_reco_electron(true),

          P_e_bin_profile("uniform_P_e_bins"),
          P_nuc_bin_profile("uniform_P_nuc_bins"),

          Electron_single_slice_test(false),
          Nucleon_single_slice_test(false),
          TestSlices({1, 1, 1}) {}

    void RefreshSettings(const RunParameters& parameters) {
        if (parameters.isData) { Generate_Electron_AMaps = Generate_Nucleon_AMaps = Generate_WMaps = false; }

        if (Generate_Electron_AMaps && Generate_Nucleon_AMaps) {
            std::cout << "\n\nAcceptanceMapsSettings::RefreshSettings: Generate_Electron_AMaps and Generate_Nucleon_AMaps can't be true at the same time! Exiting...";
            exit(0);
        }

        if (Generate_Electron_AMaps && !basic_tools::FindSubstring(parameters.SampleName, "Uniform_1e")) { Generate_Electron_AMaps = false; }

        if (Generate_Nucleon_AMaps && (!basic_tools::FindSubstring(parameters.SampleName, "Uniform_ep")) && !basic_tools::FindSubstring(parameters.SampleName, "Uniform_en")) {
            Generate_Nucleon_AMaps = false;
        }
    }
};

#endif  // ACCEPTANCEMAPSSETTINGS_H
