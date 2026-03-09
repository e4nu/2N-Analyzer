//
// Created by Alon Sportes on 25/02/2025.
//

// #pragma once

#include <string>

#include "../../../structures/DataSample/DataSample.h"

/**
 * @namespace Yaml_loader
 * @brief A namespace for loading data samples from YAML files, including a function to load a DataSample struct from a YAML file path.
 * @details This namespace contains a function called LoadFromYamlFile, which takes a YAML file path as input and returns a DataSample struct populated with the information from the YAML
 * file. The function uses the YAML-CPP library to parse the YAML file and populate the DataSample struct. The LoadFromYamlFile function throws a std::runtime_error if there are issues with
 * opening the YAML file or parsing it, providing informative error messages. The DataSample struct is defined in the framework/structures/DataSample/DataSample.h file and contains various
 * fields related to the data sample, such as sample file directories, sample name, flags for whether the sample is a simulation or data sample, beam energy flags, and a list of sample runs.
 * The LoadFromYamlFile function is used in the main code to load the data sample information from a YAML file, allowing for flexibility in configuring the analysis based on different
 * samples by simply changing the YAML file path.
 * @note The LoadFromYamlFile function relies on the YAML-CPP library for parsing the YAML file, so it is important to ensure that the library is properly included and linked in the project.
 * Additionally, the DataSample struct should be defined appropriately in the framework/structures/DataSample/DataSample.h file, with the fields that are expected to be populated from the
 * YAML file. It is also important to handle any exceptions thrown by the LoadFromYamlFile function in the main code to ensure that any issues with loading the YAML file are properly
 * addressed and do not cause the program to crash unexpectedly.
 */
namespace Yaml_loader {

// Load from a YAML file path (throws std::runtime_error on errors).
DataSample LoadFromYamlFile(const std::string& yaml_path);

}  // namespace Yaml_loader
