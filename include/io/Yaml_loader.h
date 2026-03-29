//
// Created by Alon Sportes on 25/02/2025.
//

#ifndef YAML_LOADER_H
#define YAML_LOADER_H

#include <stdexcept>
// Standard exception utilities used to convert YAML errors into
// descriptive std::runtime_error messages that propagate through
// the analysis framework.

#include <string>
// std::string is used for YAML keys, file paths, and section names
// provided by configuration files.

#include <type_traits>
// Provides compile‑time type checks (e.g., std::is_default_constructible)
// used to enforce requirements on settings types loaded from YAML.

#include <vector>
// std::vector is used to support multiple YAML files or multiple
// alternative keys when reading configuration parameters.

#define CONFIG_DIR _CONFIG_DIR
// CONFIG_DIR expands to the build‑time configuration directory
// defined by CMake.  This allows the code to reference configuration
// files relative to a known project location.

// yaml-cpp is the external library used to parse YAML configuration
// files.  The project CMake configuration downloads or locates it
// and exposes the headers during compilation.
#include <yaml-cpp/yaml.h>
#define HAS_YAML_CPP 1

// Simple structure describing a YAML input source.
// Path:    absolute or relative path to the YAML file
// Section: optional section within the YAML document to load
struct YamlInputSpec {
    std::string Path;
    std::string Section;
};

// Yaml_loader namespace
// ---------------------
// Provides generic utilities for loading strongly‑typed C++ settings
// structures from YAML configuration files.  The design separates
// parsing from mapping so that each settings type defines its own
// adapter.
namespace Yaml_loader {

// Internal helper utilities that are not intended to be used directly
// by analysis code.  They handle low‑level YAML file loading and
// section selection logic.
namespace detail {

// Load a YAML file from disk and convert any yaml-cpp exceptions
// into std::runtime_error with clearer error messages.
YAML::Node LoadYamlFileOrThrow(const std::string& yaml_path);
// Given a YAML root node, return either the root itself or a
// specified subsection.  Errors are thrown if the section does
// not exist or is not a map.
YAML::Node SelectRootOrSectionOrThrow(const YAML::Node& root, const std::string& section, const std::string& yaml_path);

}  // namespace detail

/**
 * User-defined adapter for a settings type.
 *
 * For every settings struct/class that should be loadable from YAML, define:
 *
 * template <>
 * struct Yaml_loader::SettingsAdapter<MySettings> {
 *     static void Apply(const YAML::Node& node, MySettings& settings);
 *     static void Validate(const MySettings& settings);
 * };
 */

// SettingsAdapter
// ---------------
// Template specialization point used to map YAML nodes to
// concrete settings objects.  Each settings type defines
// its own specialization implementing Apply() and Validate().
template <typename SettingsT>
struct SettingsAdapter;

// --------------------------------------------------------------------------------------
// Generic field-reading helpers to be used inside SettingsAdapter specializations
// --------------------------------------------------------------------------------------

// TryReadKey
// ----------
// Attempts to read a single key from a YAML node. If the key exists
// and the value can be converted to the requested type T, the value
// is written into out_value and the function returns true.
// If the key does not exist the function returns false.
template <typename T>
bool TryReadKey(const YAML::Node& node, const std::string& key, T& out_value) {
    if (!node || !node.IsMap()) { return false; }

    const YAML::Node value_node = node[key];
    if (!value_node || value_node.IsNull()) { return false; }

    try {
        out_value = value_node.as<T>();
    } catch (const YAML::Exception& e) { throw std::runtime_error("YAML type conversion error for key '" + key + "': " + std::string(e.what())); }

    return true;
}

// TryReadFirstMatchingKey
// -----------------------
// Attempts to read the first key from a list of possible keys.
// This is useful when supporting legacy configuration names or
// aliases for the same parameter.
template <typename T>
bool TryReadFirstMatchingKey(const YAML::Node& node, const std::vector<std::string>& keys, T& out_value, std::string* matched_key = nullptr) {
    for (const auto& key : keys) {
        if (TryReadKey(node, key, out_value)) {
            if (matched_key != nullptr) { *matched_key = key; }
            return true;
        }
    }
    return false;
}

// ReadRequiredKey
// ---------------
// Reads a YAML key that must exist. If the key is missing or
// cannot be converted to the requested type, an exception is
// thrown immediately.
template <typename T>
T ReadRequiredKey(const YAML::Node& node, const std::string& key) {
    if (!node || !node.IsMap()) { throw std::runtime_error("Internal error: attempted to read required key '" + key + "' from a non-map YAML node."); }

    const YAML::Node value_node = node[key];
    if (!value_node || value_node.IsNull()) { throw std::runtime_error("Missing required YAML key: '" + key + "'."); }

    try {
        return value_node.as<T>();
    } catch (const YAML::Exception& e) { throw std::runtime_error("YAML type conversion error for required key '" + key + "': " + std::string(e.what())); }
}

// HasKey
// ------
// Utility function that checks whether a YAML map contains
// a given key and that the value is not null.
inline bool HasKey(const YAML::Node& node, const std::string& key) {
    if (!node || !node.IsMap()) { return false; }
    const YAML::Node value_node = node[key];
    return value_node && !value_node.IsNull();
}

// --------------------------------------------------------------------------------------
// Generic loader entry points
// --------------------------------------------------------------------------------------

// ApplyYamlNodeToSettings
// -----------------------
// Delegates the actual YAML-to-structure mapping to the
// SettingsAdapter specialization for the given settings type.
template <typename SettingsT>
void ApplyYamlNodeToSettings(const YAML::Node& node, SettingsT& settings) {
    SettingsAdapter<SettingsT>::Apply(node, settings);
}

// ValidateSettings
// ----------------
// Runs validation logic defined in the SettingsAdapter for
// the given settings type after loading.
template <typename SettingsT>
void ValidateSettings(const SettingsT& settings) {
    SettingsAdapter<SettingsT>::Validate(settings);
}

// ApplyYamlFileToSettings
// -----------------------
// Loads a YAML file, optionally selects a subsection, and
// applies its contents to an existing settings object.
template <typename SettingsT>
void ApplyYamlFileToSettings(const std::string& yaml_path, SettingsT& settings, const std::string& section = "") {
    const YAML::Node root = detail::LoadYamlFileOrThrow(yaml_path);
    const YAML::Node selected_root = detail::SelectRootOrSectionOrThrow(root, section, yaml_path);
    ApplyYamlNodeToSettings(selected_root, settings);
}

// ApplyYamlFilesToSettings
// ------------------------
// Sequentially applies multiple YAML files to the same
// settings object. Later files override earlier values.
template <typename SettingsT>
void ApplyYamlFilesToSettings(const std::vector<std::string>& yaml_paths, SettingsT& settings, const std::string& section = "") {
    for (const auto& yaml_path : yaml_paths) { ApplyYamlFileToSettings(yaml_path, settings, section); }
}

// ApplyYamlSpecsToSettings
// ------------------------
// Same idea as ApplyYamlFilesToSettings, but each file may
// specify a different section inside the YAML document.
template <typename SettingsT>
void ApplyYamlSpecsToSettings(const std::vector<YamlInputSpec>& yaml_specs, SettingsT& settings) {
    for (const auto& yaml_spec : yaml_specs) { ApplyYamlFileToSettings(yaml_spec.Path, settings, yaml_spec.Section); }
}

// LoadFromYamlFile
// ----------------
// High-level helper that creates a settings object, loads
// values from a YAML file, validates them, and returns the
// fully initialized object.
template <typename SettingsT>
SettingsT LoadFromYamlFile(const std::string& yaml_path, const std::string& section = "") {
    static_assert(std::is_default_constructible_v<SettingsT>,
                  "Yaml_loader::LoadFromYamlFile requires a default-constructible settings type. "
                  "For runtime-dependent objects, construct the object first and then call ApplyYamlFileToSettings.");

    SettingsT settings{};
    ApplyYamlFileToSettings(yaml_path, settings, section);
    ValidateSettings(settings);
    return settings;
}

// LoadFromYamlFiles
// -----------------
// Same as LoadFromYamlFile but supports multiple YAML files
// that are applied sequentially.
template <typename SettingsT>
SettingsT LoadFromYamlFiles(const std::vector<std::string>& yaml_paths, const std::string& section = "") {
    static_assert(std::is_default_constructible_v<SettingsT>,
                  "Yaml_loader::LoadFromYamlFiles requires a default-constructible settings type. "
                  "For runtime-dependent objects, construct the object first and then call ApplyYamlFilesToSettings.");

    SettingsT settings{};
    ApplyYamlFilesToSettings(yaml_paths, settings, section);
    ValidateSettings(settings);
    return settings;
}

// LoadFromYamlSpecs
// -----------------
// Highest-level loader that supports both multiple files and
// per-file section selection.
template <typename SettingsT>
SettingsT LoadFromYamlSpecs(const std::vector<YamlInputSpec>& yaml_specs) {
    static_assert(std::is_default_constructible_v<SettingsT>,
                  "Yaml_loader::LoadFromYamlSpecs requires a default-constructible settings type. "
                  "For runtime-dependent objects, construct the object first and then call ApplyYamlSpecsToSettings.");

    SettingsT settings{};
    ApplyYamlSpecsToSettings(yaml_specs, settings);
    ValidateSettings(settings);
    return settings;
}

}  // namespace Yaml_loader

#endif  // YAML_LOADER_H
