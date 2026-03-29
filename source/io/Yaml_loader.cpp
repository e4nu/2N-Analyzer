//
// Created by Alon Sportes on 25/02/2025.
//

#include "Yaml_loader.h"

// Implementation of the low-level YAML loading helpers declared
// in Yaml_loader.h. These functions wrap yaml-cpp functionality
// and provide clearer error handling.

#include <stdexcept>
#include <string>

namespace Yaml_loader {

// Implementation namespace for the YAML loader utilities.

namespace detail {

// Internal helper functions used by the higher-level template
// loading utilities defined in the header.

// LoadYamlFileOrThrow
// -------------------
// Attempts to read a YAML file from disk. yaml-cpp exceptions are
// caught and rethrown as std::runtime_error with clearer messages
// for the analysis framework.
YAML::Node LoadYamlFileOrThrow(const std::string& yaml_path) {
    try {
        return YAML::LoadFile(yaml_path);
    } catch (const YAML::BadFile& e) { throw std::runtime_error("Cannot open YAML file '" + yaml_path + "': " + std::string(e.what())); } catch (const YAML::Exception& e) {
        throw std::runtime_error("YAML parse error in '" + yaml_path + "': " + std::string(e.what()));
    }
}

// SelectRootOrSectionOrThrow
// --------------------------
// Validates the YAML document structure and returns either the
// entire root map or a specific subsection requested by the user.
// Errors are thrown if the structure is invalid or the section
// does not exist.
YAML::Node SelectRootOrSectionOrThrow(const YAML::Node& root, const std::string& section, const std::string& yaml_path) {
    if (!root) { throw std::runtime_error("YAML root node is empty in file '" + yaml_path + "'."); }

    if (!root.IsMap()) { throw std::runtime_error("YAML root must be a map (key:value pairs) in file '" + yaml_path + "'."); }

    if (section.empty()) { return root; }

    const YAML::Node selected = root[section];
    if (!selected) { throw std::runtime_error("Requested YAML section '" + section + "' was not found in file '" + yaml_path + "'."); }

    if (!selected.IsMap()) { throw std::runtime_error("Requested YAML section '" + section + "' in file '" + yaml_path + "' is not a map."); }

    return selected;
}

}  // namespace detail

}  // namespace Yaml_loader
