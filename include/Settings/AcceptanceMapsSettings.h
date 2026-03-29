//
// Created by Alon Sportes on 01/04/2025.
//

#ifndef ACCEPTANCEMAPSSETTINGS_H
#define ACCEPTANCEMAPSSETTINGS_H

#include <iostream>

// Include libraries:
#include "../libraries/general_utilities/basic_tools.h"

// Include settings:
#include "EventSelectionSettings.h"
#include "RunParameters.h"

// Include YAML loader:
#include "../io/Yaml_loader.h"

namespace bt = basic_tools;

/**
 * @struct AcceptanceMapsSettings
 * @brief Settings related to acceptance maps, including whether to generate electron and nucleon acceptance maps, the binning profiles for electron and nucleon momentum, and settings for
 * single slice tests.
 * @details These settings are used in the main code to control the generation of acceptance maps for electrons and nucleons, as well as the generation of efficiency maps (WMaps). The
 * settings also include options for single slice tests, which can be used to test the acceptance maps in specific momentum slices. The UpdateAMapsSettingsByParameters function is used to
 * update the settings based on the run parameters, while the RefreshSettingsByEventSelection function is used to update the settings based on the event selection settings.
 * @note The Generate_Electron_AMaps and Generate_Nucleon_AMaps settings are mutually exclusive, meaning that both cannot be true at the same time. The binning profiles for electron and
 * nucleon momentum can be set to different options, such as uniform or varying bins. The single slice test settings should be set to false for normal runs, and can be used for testing
 * purposes.
 */
struct AcceptanceMapsSettings {
    bool Generate_Electron_AMaps;  // Generate electron acceptance maps
    bool Generate_Nucleon_AMaps;   // Generate nucleon acceptance maps
    bool Generate_WMaps;           // Generate efficiency maps
    bool AMaps_calc_with_one_reco_electron;

    std::string P_e_bin_profile;    // {reformat_e_bins , varying_P_e_bins , uniform_P_e_bins, equi_inverted_P_e}
    std::string P_nuc_bin_profile;  // {equi_inverted_P_nuc , varying_P_nuc_bins , uniform_P_nuc_bins}

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

    void UpdateAMapsSettingsByParameters(const RunParameters& parameters) {
        if (parameters.isData) { Generate_Electron_AMaps = Generate_Nucleon_AMaps = Generate_WMaps = false; }

        // Don't generate electron AMaps if the samples is not Uniform_1e_[...]
        if (Generate_Electron_AMaps && !bt::FindSubstring(parameters.SampleName, "Uniform_1e")) { Generate_Electron_AMaps = false; }

        // Don't generate nucleon AMaps if the samples is not Uniform_ep/en_[...]
        if (Generate_Nucleon_AMaps && (!bt::FindSubstring(parameters.SampleName, "Uniform_ep")) && !basic_tools::FindSubstring(parameters.SampleName, "Uniform_en")) {
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

// --------------------------------------------------------------------------------------
// YAML adapter specialization for AcceptanceMapsSettings
// --------------------------------------------------------------------------------------

template <>
struct Yaml_loader::SettingsAdapter<AcceptanceMapsSettings> {
    static void Apply(const YAML::Node& node, AcceptanceMapsSettings& s) {
        using namespace Yaml_loader;

        TryReadKey(node, "Generate_Electron_AMaps", s.Generate_Electron_AMaps);
        TryReadKey(node, "Generate_Nucleon_AMaps", s.Generate_Nucleon_AMaps);
        TryReadKey(node, "Generate_WMaps", s.Generate_WMaps);
        TryReadKey(node, "AMaps_calc_with_one_reco_electron", s.AMaps_calc_with_one_reco_electron);

        // Allow overriding binning profiles from YAML
        TryReadKey(node, "P_e_bin_profile", s.P_e_bin_profile);
        TryReadKey(node, "P_nuc_bin_profile", s.P_nuc_bin_profile);

        TryReadKey(node, "Electron_single_slice_test", s.Electron_single_slice_test);
        TryReadKey(node, "Nucleon_single_slice_test", s.Nucleon_single_slice_test);

        TryReadKey(node, "TestSlices", s.TestSlices);

        TryReadKey(node, "NumberNucOfMomSlices", s.NumberNucOfMomSlices);
        TryReadKey(node, "NumberElecOfMomSlices", s.NumberElecOfMomSlices);
        TryReadKey(node, "HistElectronSliceNumOfXBins", s.HistElectronSliceNumOfXBins);
        TryReadKey(node, "HistNucSliceNumOfXBins", s.HistNucSliceNumOfXBins);
    }

    static void Validate(const AcceptanceMapsSettings& s) {
        if (s.Generate_Electron_AMaps && s.Generate_Nucleon_AMaps) {
            bt::ExitWithError(__func__, __FILE__, __LINE__, "Generate_Electron_AMaps and Generate_Nucleon_AMaps can't both be true.");
        }

        if (s.TestSlices.size() != 3) { bt::ExitWithError(__func__, __FILE__, __LINE__, "TestSlices must contain exactly 3 values: {Electron, Proton, Neutron}."); }
    }
};

#endif  // ACCEPTANCEMAPSSETTINGS_H
