#include "io/reco_cli.h"

// Implementation of the reconstructed-analysis command-line interface.
// This file is responsible for:
//   1) parsing raw argc/argv input,
//   2) validating option values,
//   3) printing user-facing help text, and
//   4) printing the final parsed CLI state for debugging.

// Standard library headers used by the helper functions below.
#include <algorithm>
// std::transform is used to lowercase boolean-like strings such as "TRUE" or "Yes".
#include <cctype>
// std::tolower is used for case-insensitive parsing of boolean command-line values.
#include <cstdlib>
// std::exit is used when CLI parsing fails and the program must terminate immediately.
#include <iostream>
// std::cout and std::cerr are used for help text, diagnostics, and debug prints.
#include <stdexcept>
// std::runtime_error is used to report invalid command-line inputs with descriptive messages.
#include <string>
// std::string is the main type used to store options, paths, and raw argument values.


// reco_cli namespace
// ------------------
// Groups together all logic related to runtime command-line configuration for
// the reconstructed-analysis executable.
namespace reco_cli {


// Anonymous namespace
// -------------------
// Contains internal helper functions that are private to this translation unit.
// These helpers are not part of the public CLI interface exposed in reco_cli.h.
namespace {


// ToLowerCopy
// -----------
// Returns a lowercase copy of the input string so that boolean-like CLI values
// can be parsed in a case-insensitive way.
std::string ToLowerCopy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}


// ParseBoolOrThrow
// ----------------
// Converts strings such as "true", "false", "yes", "no", "1", and "0"
// into a C++ bool. Throws an exception if the value is not recognized.
bool ParseBoolOrThrow(const std::string& option_name, const std::string& value) {
    const std::string lowered = ToLowerCopy(value);

    if (lowered == "true" || lowered == "1" || lowered == "yes" || lowered == "on") { return true; }

    if (lowered == "false" || lowered == "0" || lowered == "no" || lowered == "off") { return false; }

    throw std::runtime_error("Invalid boolean value for " + option_name + ": '" + value + "'. Expected true/false, 1/0, yes/no, or on/off.");
}


// RequireValueOrThrow
// -------------------
// Reads the next argv entry as the value for an option such as
// `--sample-chain <path>`. Throws if the option was provided without a value.
std::string RequireValueOrThrow(int argc, char** argv, int& i, const std::string& option_name) {
    if (i + 1 >= argc) { throw std::runtime_error("Missing value for option " + option_name + "."); }

    return argv[++i];
}


// ParseIntOrThrow
// ---------------
// Parses a string into an int and verifies that the entire string was consumed.
// This prevents values like "12abc" from being accepted silently.
int ParseIntOrThrow(const std::string& option_name, const std::string& value) {
    std::size_t parsed_chars = 0;
    const int parsed_value = std::stoi(value, &parsed_chars);

    if (parsed_chars != value.size()) {
        throw std::runtime_error("Invalid integer value for " + option_name + ": '" + value + "'.");
    }

    return parsed_value;
}


// ParseLongLongOrThrow
// --------------------
// Same idea as ParseIntOrThrow, but for long long values such as event counts.
long long ParseLongLongOrThrow(const std::string& option_name, const std::string& value) {
    std::size_t parsed_chars = 0;
    const long long parsed_value = std::stoll(value, &parsed_chars);

    if (parsed_chars != value.size()) {
        throw std::runtime_error("Invalid integer value for " + option_name + ": '" + value + "'.");
    }

    return parsed_value;
}


// ValidateRecoCliOptionsOrThrow
// -----------------------------
// Applies cross-option validation after all arguments were parsed. This catches
// values that are syntactically valid but semantically invalid for the program.
void ValidateRecoCliOptionsOrThrow(const reco_cli::RecoCliOptions& options) {
    if (options.sample_index < -1) {
        throw std::runtime_error("Invalid value for --sample-index: " + std::to_string(options.sample_index) + ". Use -1 for all samples, or a non-negative integer.");
    }

    if (options.n_events == 0 || options.n_events < -1) {
        throw std::runtime_error("Invalid value for --n-events: " + std::to_string(options.n_events) + ". Use -1 for all events, or a positive integer.");
    }

    if (options.Skip_prob_limit <= 0) {
        throw std::runtime_error("Invalid value for --skip-prob-limit: " + std::to_string(options.Skip_prob_limit) + ". It must be a positive integer.");
    }
}

}  // namespace


// Public CLI interface
// --------------------
// The functions below form the user-facing CLI layer used by the rest of the
// reconstructed-analysis application.
// ======================================================================================================================================================================


// PrintRecoUsage
// --------------
// Prints the supported command-line flags and a short description of each one.
// This function is called when the user passes `--help` or when parsing fails.
void PrintRecoUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options]\n\n";

    // General help and top-level configuration-file options.
    std::cout << "Options:\n";
    std::cout << "  --help                              Show this help message\n";
    std::cout << "  --directories <path>                YAML file with core code directory settings\n";
    std::cout << "  --event-selection <path>            YAML file with event selection settings\n";
    std::cout << "  --sample-chain <path>               YAML file describing the sample chain\n";
    std::cout << "  --cut-values <path>                 YAML file with cut values\n";
    std::cout << "  --analysis-cuts <path>              YAML file with analysis cut settings\n";
    std::cout << "  --acceptance-maps-config <path>     YAML file with acceptance and weight maps settings\n";
    std::cout << "  --momentum-resolution-config <path> YAML file with momentum resolution settings\n";

    // Runtime control over which sample to run and how output cleanup behaves.
    std::cout << "  --sample-index <int>                Run only a single sample from the chain\n";
    std::cout << "  --delete-png-files <bool>           Remove PNG files from output directory\n";
    std::cout << "  --delete-root-files <bool>          Remove ROOT files from output directory\n";
    std::cout << "  --delete-txt-files <bool>           Remove TXT files from output directory\n";
    std::cout << "  --clear-old-directories <bool>      Clear old run directories\n";

    // Performance / debugging controls for probing, test mode, and event limits.
    std::cout << "  --skip-hipo-files-prob <bool>       Skip hipo file prober\n";
    std::cout << "  --skip-prob-limit <int>             Number of events before displaying skip prob warning\n";
    std::cout << "  --test-run <bool>                   Run analysis code in test mode (only selected plots)\n";
    std::cout << "  --n-events <int>                    Maximum events to process per sample\n";
    std::cout << "  --save-acceptance-corr-data <bool>  Save acceptance correction data\n";

    // Diagnostic output.
    std::cout << "  --verbose                           Enable verbose output\n";
    std::cout << std::endl;
}


// PrintRecoCliOptions
// -------------------
// Dumps the fully parsed CLI state to stdout. This is useful when debugging
// how command-line arguments were interpreted by the parser.
void PrintRecoCliOptions(const RecoCliOptions& options) {
    // Paths to YAML configuration files selected for this run.
    std::cout << "[CLI] directories-file: " << options.directories_file << std::endl;
    std::cout << "[CLI] event-selection-file: " << options.event_selection_file << std::endl;
    std::cout << "[CLI] sample-chain-file: " << options.sample_chain_file << std::endl;
    std::cout << "[CLI] cut-values-file: " << options.cut_values_file << std::endl;
    std::cout << "[CLI] analysis-cuts-file: " << options.analysis_cuts_file << std::endl;
    std::cout << "[CLI] acceptance-maps-file: " << options.acceptance_maps_file << std::endl;
    std::cout << "[CLI] momentum-resolution-file: " << options.momentum_resolution_file << std::endl;

    // Runtime execution controls and cleanup behavior.
    std::cout << "[CLI] sample-index: " << options.sample_index << std::endl;
    std::cout << "[CLI] delete-png-files: " << (options.delete_png_files ? "true" : "false") << std::endl;
    std::cout << "[CLI] delete-root-files: " << (options.delete_root_files ? "true" : "false") << std::endl;
    std::cout << "[CLI] delete-txt-files: " << (options.delete_txt_files ? "true" : "false") << std::endl;
    std::cout << "[CLI] clear-old-directories: " << (options.Clear_Old_Directories ? "true" : "false") << std::endl;

    // Performance / debugging toggles.
    std::cout << "[CLI] skip-hipo-files-prob: " << (options.Skip_prob ? "true" : "false") << std::endl;
    std::cout << "[CLI] skip-prob-limit: " << options.Skip_prob_limit << std::endl;
    std::cout << "[CLI] test-run: " << (options.TestRun ? "true" : "false") << std::endl;
    std::cout << "[CLI] n-events: " << options.n_events << std::endl;
    std::cout << "[CLI] save-acceptance-corr-data: " << (options.save_ACorr_data ? "true" : "false") << std::endl;

    // Whether verbose CLI debug printing is enabled.
    std::cout << "[CLI] verbose: " << (options.verbose ? "true" : "false") << std::endl;
}


// ParseRecoCliOptions
// -------------------
// Main argument parser for the reconstructed-analysis executable.
// It walks through argv, matches recognized flags, converts values into the
// correct C++ types, validates the final option set, and aborts with a clear
// message if anything is invalid.
RecoCliOptions ParseRecoCliOptions(int argc, char** argv) {
    // Start from the default option set defined in RecoCliOptions.
    RecoCliOptions options;

    // Catch all parsing/validation errors in one place so the user receives
    // both the error message and the usage summary.
    try {
        // Walk through all user-provided arguments, skipping argv[0]
        // which contains the program name.
        for (int i = 1; i < argc; ++i) {
            // Current raw command-line token being interpreted.
            const std::string arg = argv[i];

            // Help / documentation.
            if (arg == "--help") {
                options.help = true;
            // YAML configuration-file paths.
            } else if (arg == "--directories") {
                options.directories_file = RequireValueOrThrow(argc, argv, i, arg);
            } else if (arg == "--event-selection") {
                options.event_selection_file = RequireValueOrThrow(argc, argv, i, arg);
            } else if (arg == "--sample-chain") {
                options.sample_chain_file = RequireValueOrThrow(argc, argv, i, arg);
            } else if (arg == "--cut-values") {
                options.cut_values_file = RequireValueOrThrow(argc, argv, i, arg);
            } else if (arg == "--analysis-cuts") {
                options.analysis_cuts_file = RequireValueOrThrow(argc, argv, i, arg);
            } else if (arg == "--acceptance-maps-config") {
                options.acceptance_maps_file = RequireValueOrThrow(argc, argv, i, arg);
            } else if (arg == "--momentum-resolution-config") {
                options.momentum_resolution_file = RequireValueOrThrow(argc, argv, i, arg);
            // Sample selection and output-cleanup options.
            } else if (arg == "--sample-index") {
                options.sample_index = ParseIntOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            } else if (arg == "--delete-png-files") {
                options.delete_png_files = ParseBoolOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            } else if (arg == "--delete-root-files") {
                options.delete_root_files = ParseBoolOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            } else if (arg == "--delete-txt-files") {
                options.delete_txt_files = ParseBoolOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            // Backward-compatible parsing for legacy mixed-case option names.
            } else if (arg == "--clear-old-directories" || arg == "--Clear-old-directories") {
                options.Clear_Old_Directories = ParseBoolOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            } else if (arg == "--skip-hipo-files-prob" || arg == "--Skip-hipo-files-prob") {
                options.Skip_prob = ParseBoolOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            } else if (arg == "--skip-prob-limit" || arg == "--Skip-prob-limit") {
                options.Skip_prob_limit = ParseIntOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            // Performance / debugging controls.
            } else if (arg == "--test-run") {
                options.TestRun = ParseBoolOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            } else if (arg == "--n-events") {
                options.n_events = ParseLongLongOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            } else if (arg == "--save-acceptance-corr-data") {
                options.save_ACorr_data = ParseBoolOrThrow(arg, RequireValueOrThrow(argc, argv, i, arg));
            // Diagnostic verbosity.
            } else if (arg == "--verbose") {
                options.verbose = true;
            // Any unrecognized token is treated as an error immediately.
            } else {
                throw std::runtime_error("Unknown option: " + arg);
            }
        }
        // Perform final semantic validation after all individual arguments
        // have been parsed successfully.
        ValidateRecoCliOptionsOrThrow(options);
    }
    // Convert any parsing or validation failure into a user-facing error report.
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        PrintRecoUsage(argv[0]);
        std::exit(1);
    }

    // Return the fully parsed and validated CLI configuration to the caller.
    return options;
}

}  // namespace reco_cli
