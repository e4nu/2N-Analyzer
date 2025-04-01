//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef ACCEPTANCEMAPSSETTINGS_H
#define ACCEPTANCEMAPSSETTINGS_H

#include <iostream>

#include "../../../namespaces/general_utilities/basic_tools.h"

struct AMapsSettings {
    bool Generate_Electron_AMaps;
    bool Generate_Nucleon_AMaps;
    bool Generate_WMaps;
    bool AMaps_calc_with_one_reco_electron;
    
    const std::string P_e_bin_profile;
    const std::string P_nuc_bin_profile;
    
    bool Electron_single_slice_test;
    bool Nucleon_single_slice_test;
    std::vector<int> TestSlices;

    AMapsSettings()
        : Generate_Electron_AMaps(false),
          Generate_Nucleon_AMaps(true),
          Generate_WMaps(true),
          AMaps_calc_with_one_reco_electron(true),
          
          P_e_bin_profile("uniform_P_e_bins"),
          P_nuc_bin_profile("uniform_P_nuc_bins"),
          
          Electron_single_slice_test(false),
          Nucleon_single_slice_test(false),
          TestSlices({1, 1, 1}) {}

    void RefreshSettings(const RunParameters& parameters, const std::string& SampleName) {
        if (parameters.isData) {
            Generate_Electron_AMaps = Generate_Nucleon_AMaps = Generate_WMaps = false;
        }

        if (Generate_Electron_AMaps && Generate_Nucleon_AMaps) {
            std::cout << "\n\nGenerate AMaps: Generate_Electron_AMaps and Generate_Nucleon_AMaps can't be true at the same time! Exiting...";
            exit(0);
        }

        if (Generate_Electron_AMaps && !basic_tools::FindSubstring(SampleName, "Uniform_1e")) {
            Generate_Electron_AMaps = false;
        }

        if (Generate_Nucleon_AMaps && (!basic_tools::FindSubstring(SampleName, "Uniform_ep")) && !basic_tools::FindSubstring(SampleName, "Uniform_en")) {
            Generate_Nucleon_AMaps = false;
        }
    }
};

#endif  // ACCEPTANCEMAPSSETTINGS_H
