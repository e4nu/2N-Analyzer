//
// Created by Alon Sportes on 25/02/2025.
//

#ifndef YAML_LOADER
#define YAML_LOADER

#include "Yaml_loader.h"

#include <stdexcept>
#include <string>
#include <vector>

#if __has_include(<yaml-cpp/yaml.h>)
    #include <yaml-cpp/yaml.h>
    #define HAS_YAML_CPP 1
#else
    #define HAS_YAML_CPP 0
#endif

#if HAS_YAML_CPP
namespace {

template <typename T>
bool try_read_first_matching_key(const YAML::Node& node, const std::vector<std::string>& keys, T& out_value, std::string* matched_key = nullptr) {
    if (!node || !node.IsMap()) return false;

    for (const auto& k : keys) {
        const YAML::Node v = node[k];
        if (v && !v.IsNull()) {
            try {
                out_value = v.as<T>();
            } catch (const YAML::Exception& e) { throw std::runtime_error("YAML type conversion error for key '" + k + "': " + std::string(e.what())); }
            if (matched_key) *matched_key = k;
            return true;
        }
    }
    return false;
}

static void validate_sample(const DataSample& s) {
    // Basic consistency checks. Adjust to your conventions.
    if (s.IsSimulationSample && s.IsDataSample) { throw std::runtime_error("Config error: IsSimulationSample and IsDataSample cannot both be true."); }
    if (!s.IsSimulationSample && !s.IsDataSample) { throw std::runtime_error("Config error: IsSimulationSample and IsDataSample cannot both be false."); }

    const int n_beams = static_cast<int>(s.BeamAt2GeV) + static_cast<int>(s.BeamAt4GeV) + static_cast<int>(s.BeamAt6GeV);

    if (n_beams > 1) { throw std::runtime_error("Config error: only one of BeamAt2GeV, BeamAt4GeV, BeamAt6GeV may be true."); }
}

static void load_into_struct(const YAML::Node& root, DataSample& out) {
    if (!root) { throw std::runtime_error("YAML root node is empty."); }
    if (!root.IsMap()) { throw std::runtime_error("YAML root must be a map (key:value pairs)."); }

    // Strings
    (void)try_read_first_matching_key<std::string>(root, {"SampleFilesTopDir", "sample_file_path"}, out.SampleFilesTopDir);
    (void)try_read_first_matching_key<std::string>(root, {"SampleFilesRecHipoDir", "sample_file_name"}, out.SampleFilesRecHipoDir);
    (void)try_read_first_matching_key<std::string>(root, {"SampleFileDir", "sample_file_dir"}, out.SampleFileDir);
    (void)try_read_first_matching_key<std::string>(root, {"SampleFile", "sample_file"}, out.SampleFile);
    (void)try_read_first_matching_key<std::string>(root, {"SampleName", "sample_name"}, out.SampleName);

    // Booleans
    (void)try_read_first_matching_key<bool>(root, {"IsSimulationSample", "is_simulation_sample"}, out.IsSimulationSample);
    (void)try_read_first_matching_key<bool>(root, {"IsDataSample", "is_data_sample"}, out.IsDataSample);

    (void)try_read_first_matching_key<bool>(root, {"BeamAt2GeV", "beam_at_2gev"}, out.BeamAt2GeV);
    (void)try_read_first_matching_key<bool>(root, {"BeamAt4GeV", "beam_at_4gev"}, out.BeamAt4GeV);
    (void)try_read_first_matching_key<bool>(root, {"BeamAt6GeV", "beam_at_6gev"}, out.BeamAt6GeV);

    // std::vector<string>
    (void)try_read_first_matching_key<std::vector<std::string>>(root, {"SampleRuns", "sample_runs"}, out.SampleRuns);

    validate_sample(out);
}

}  // namespace

namespace Yaml_loader {

DataSample LoadFromYamlFile(const std::string& yaml_path) {
    DataSample sample;  // start from your defaults

    YAML::Node root;
    try {
        root = YAML::LoadFile(yaml_path);
    } catch (const YAML::BadFile& e) { throw std::runtime_error("Cannot open YAML file '" + yaml_path + "': " + std::string(e.what())); } catch (const YAML::Exception& e) {
        throw std::runtime_error("YAML parse error in '" + yaml_path + "': " + std::string(e.what()));
    }

    load_into_struct(root, sample);
    return sample;
}

}  // namespace Yaml_loader
#else
namespace Yaml_loader {

DataSample LoadFromYamlFile(const std::string& yaml_path) {
    throw std::runtime_error("yaml-cpp headers not found at compile time. Cannot load YAML file: " + yaml_path);
}

}  // namespace Yaml_loader
#endif

#endif  // YAML_LOADER
