//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef ANALYSIS_SAMPLES_H
#define ANALYSIS_SAMPLES_H

#include <iostream>
#include <string>
#include <vector>

// Include YAML loader:
#include "Yaml_loader.h"

namespace analysis_samples {
// ======================================================================================================================================================================
// analysis_samples structs
// ======================================================================================================================================================================

// SampleChainEntry struct ----------------------------------------------------------------------------------------------------------------------------------------------

struct SampleChainEntry {
    std::string Label;
    std::string RecoSamplePath;
    std::string ReconHipoDir = "reconhipo";
    std::string InputPattern = "*.hipo";

    std::string PathToHipoFilesDir;
    std::string InputHipoFiles;

    void RefreshDerivedFields() {
        PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
        InputHipoFiles = PathToHipoFilesDir + InputPattern;
    }
};

// SampleChainConfig struct ---------------------------------------------------------------------------------------------------------------------------------------------

struct SampleChainConfig {
    std::vector<SampleChainEntry> Samples;
};

// ======================================================================================================================================================================
// analysis_samples global variables
// ======================================================================================================================================================================

inline std::vector<std::string> RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v;
inline std::string RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles;

// Keep the old global for compatibility with the rest of the code.
inline std::string file_name = "general_file";

// ======================================================================================================================================================================
// analysis_samples functions
// ======================================================================================================================================================================

// ClearSampleChain function --------------------------------------------------------------------------------------------------------------------------------------------

inline void ClearSampleChain() {
    RecoSamplePath_v.clear();
    ReconHipoDir_v.clear();
    PathToHipoFilesDir_v.clear();
    InputHipoFiles_v.clear();

    RecoSamplePath.clear();
    ReconHipoDir.clear();
    PathToHipoFilesDir.clear();
    InputHipoFiles.clear();
}

// PopulateLegacySampleVectors function ---------------------------------------------------------------------------------------------------------------------------------

inline void PopulateLegacySampleVectors(const SampleChainConfig& config) {
    ClearSampleChain();

    for (const auto& sample : config.Samples) {
        RecoSamplePath = sample.RecoSamplePath;
        ReconHipoDir = sample.ReconHipoDir;
        PathToHipoFilesDir = sample.PathToHipoFilesDir;
        InputHipoFiles = sample.InputHipoFiles;

        RecoSamplePath_v.push_back(RecoSamplePath);
        ReconHipoDir_v.push_back(ReconHipoDir);
        PathToHipoFilesDir_v.push_back(PathToHipoFilesDir);
        InputHipoFiles_v.push_back(InputHipoFiles);
    }
}

// LoadSampleChainConfigFromYaml function -------------------------------------------------------------------------------------------------------------------------------

inline SampleChainConfig LoadSampleChainConfigFromYaml(const std::string& yaml_path, const std::string& section = "") {
    SampleChainConfig config = Yaml_loader::LoadFromYamlFile<SampleChainConfig>(yaml_path, section);
    PopulateLegacySampleVectors(config);
    return config;
}

// LoadSampleChainFromYaml function -------------------------------------------------------------------------------------------------------------------------------------

inline void LoadSampleChainFromYaml(const std::string& yaml_path, const std::string& section = "") {
    const SampleChainConfig config = Yaml_loader::LoadFromYamlFile<SampleChainConfig>(yaml_path, section);
    PopulateLegacySampleVectors(config);
}

}  // namespace analysis_samples

template <>
struct Yaml_loader::SettingsAdapter<analysis_samples::SampleChainEntry> {
    static void Apply(const YAML::Node& node, analysis_samples::SampleChainEntry& s) {
        using namespace Yaml_loader;

        TryReadKey(node, "label", s.Label);
        TryReadKey(node, "Label", s.Label);

        TryReadKey(node, "reco_sample_path", s.RecoSamplePath);
        TryReadKey(node, "RecoSamplePath", s.RecoSamplePath);

        TryReadKey(node, "recon_hipo_dir", s.ReconHipoDir);
        TryReadKey(node, "ReconHipoDir", s.ReconHipoDir);

        TryReadKey(node, "input_pattern", s.InputPattern);
        TryReadKey(node, "InputPattern", s.InputPattern);

        s.RefreshDerivedFields();

        // Optional explicit overrides for derived fields.
        TryReadKey(node, "path_to_hipo_files_dir", s.PathToHipoFilesDir);
        TryReadKey(node, "PathToHipoFilesDir", s.PathToHipoFilesDir);

        TryReadKey(node, "input_hipo_files", s.InputHipoFiles);
        TryReadKey(node, "InputHipoFiles", s.InputHipoFiles);
    }

    static void Validate(const analysis_samples::SampleChainEntry& s) {
        if (s.RecoSamplePath.empty()) { throw std::runtime_error("SampleChainEntry validation error: RecoSamplePath cannot be empty."); }

        if (s.ReconHipoDir.empty()) { throw std::runtime_error("SampleChainEntry validation error: ReconHipoDir cannot be empty."); }

        if (s.PathToHipoFilesDir.empty()) { throw std::runtime_error("SampleChainEntry validation error: PathToHipoFilesDir cannot be empty."); }

        if (s.InputHipoFiles.empty()) { throw std::runtime_error("SampleChainEntry validation error: InputHipoFiles cannot be empty."); }
    }
};

template <>
struct Yaml_loader::SettingsAdapter<analysis_samples::SampleChainConfig> {
    static void Apply(const YAML::Node& node, analysis_samples::SampleChainConfig& s) {
#if !HAS_YAML_CPP
        (void)node;
        (void)s;
        throw std::runtime_error("yaml-cpp headers not found at compile time. Cannot populate SampleChainConfig.");
#else
        if (!node || !node.IsMap()) { throw std::runtime_error("SampleChainConfig YAML error: root node must be a map."); }

        const YAML::Node samples_node = node["samples"] ? node["samples"] : node["Samples"];
        if (!samples_node) { throw std::runtime_error("SampleChainConfig YAML error: missing required key 'samples'."); }

        if (!samples_node.IsSequence()) { throw std::runtime_error("SampleChainConfig YAML error: 'samples' must be a sequence."); }

        s.Samples.clear();
        s.Samples.reserve(samples_node.size());

        for (std::size_t i = 0; i < samples_node.size(); ++i) {
            analysis_samples::SampleChainEntry entry;
            Yaml_loader::ApplyYamlNodeToSettings(samples_node[i], entry);
            Yaml_loader::ValidateSettings(entry);
            s.Samples.push_back(entry);
        }
#endif
    }

    static void Validate(const analysis_samples::SampleChainConfig& s) {
        if (s.Samples.empty()) { throw std::runtime_error("SampleChainConfig validation error: the sample chain cannot be empty."); }

        for (const auto& sample : s.Samples) { Yaml_loader::ValidateSettings(sample); }
    }
};

#endif  // ANALYSIS_SAMPLES_H