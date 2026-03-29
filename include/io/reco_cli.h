//
// Created by Alon Sportes on 11/03/2026.
//

#ifndef RECOCLI_H
// Header guard start.
// Prevents this header from being included multiple times in the same
// compilation unit, which would otherwise cause redefinition errors.

#define RECOCLI_H

// Standard library header providing std::string, which is used to
// store file paths and command‑line option values.
#include <string>

// reco_cli namespace
// ------------------
// Contains all structures and functions related to parsing and
// managing command‑line arguments for the reconstructed analysis
// executable. Keeping CLI logic inside its own namespace prevents
// name collisions with the rest of the framework.
namespace reco_cli {
// ======================================================================================================================================================================
// CLI struct
// ======================================================================================================================================================================

// RecoCliOptions
// --------------
// Structure that stores all command‑line configuration options for
// the reconstructed analysis executable. Each field corresponds to
// one CLI flag that can be passed when running the program.
struct RecoCliOptions {
    // If true, the program will print the help/usage message and exit
    // without executing the analysis.
    bool help = false;

    // Path to the YAML configuration file defining core code directories
    // used by the analysis framework.
    std::string directories_file = std::string(CONFIG_DIR) + "/io/CodeDirectories_Default.yaml";
    // YAML file describing event‑selection configuration parameters.
    std::string event_selection_file = std::string(CONFIG_DIR) + "/EventSelection/EventSelectionSettings_Default.yaml";
    // YAML configuration describing the chain of samples that the
    // reconstructed analysis will process.
    std::string sample_chain_file = std::string(CONFIG_DIR) + "/samples/RecoSampleChain_Default.yaml";
    // YAML file defining numeric values used by the CutValueManager
    // for physics selection cuts.
    std::string cut_values_file = std::string(CONFIG_DIR) + "/cuts/CutValueManager_Default.yaml";
    // YAML configuration that specifies which analysis cuts are
    // enabled and how they are applied.
    std::string analysis_cuts_file = std::string(CONFIG_DIR) + "/cuts/AnalysisCutSettings_Default.yaml";
    // YAML configuration describing acceptance and weight maps
    // used for detector efficiency corrections.
    std::string acceptance_maps_file = std::string(CONFIG_DIR) + "/tools/AcceptanceMapsSettings_Default.yaml";
    // YAML file defining momentum resolution parameters used
    // in reconstruction studies.
    std::string momentum_resolution_file = std::string(CONFIG_DIR) + "/tools/MomentumResolutionSettings_Default.yaml";

    // Selects which sample from the sample chain should be processed.
    // Value meanings:
    //   -1  → process all samples
    //   >=0 → run only the sample at this index.
    int sample_index = -1;

    // Controls whether PNG plot files should be deleted from the
    // output directory before starting a new run.
    bool delete_png_files = true;
    // Controls whether ROOT output files from previous runs
    // should be removed before executing the analysis again.
    bool delete_root_files = true;
    // Controls whether TXT output files should be cleaned
    // from previous runs.
    bool delete_txt_files = true;

    // If true, directories produced by earlier runs will be cleared
    // before the analysis begins.
    bool Clear_Old_Directories = true;

    /*
    For full runs - use
            Skip_prob = false
    */
    // When enabled, the code skips the HIPO probing stage and
    // assumes that all expanded files are valid. This speeds up
    // startup but disables safety checks.
    bool Skip_prob = false;  // Set true to bypass probing and trust all expanded files.

    // Maximum number of events to probe before triggering a
    // warning related to skip‑probing logic.
    int Skip_prob_limit = 1000000;

    /*
    For full runs - use
            TestRun = false
            ApplyLimiter = false
    */
    // Enables a reduced "test run" mode that executes only
    // a subset of plots or analysis logic for debugging.
    bool TestRun = false;
    // Maximum number of events to process per sample.
    // Value meanings:
    //   -1 → process all available events
    //   >0 → limit the run to this many events.
    long long n_events = -1;

    // If true, acceptance‑correction data will be saved
    // to output files during the analysis.
    bool save_ACorr_data = false;

    // Enables verbose diagnostic printing during execution
    // and CLI parsing.
    bool verbose = false;
};

// ======================================================================================================================================================================
// CLI functions
// ======================================================================================================================================================================

// CLI function declarations
// -------------------------
// These functions implement the command‑line interface used
// by the reconstructed analysis executable.

// Prints a help/usage message listing all supported CLI
// options and their descriptions.
void PrintRecoUsage(const char* program_name);

// Prints the parsed CLI configuration so the user can verify
// which options were applied for the current run.
void PrintRecoCliOptions(const RecoCliOptions& options);

// Parses the command‑line arguments provided to main().
// Converts raw argv values into a fully populated
// RecoCliOptions structure and validates the result.
RecoCliOptions ParseRecoCliOptions(int argc, char** argv);

}  // namespace reco_cli

// End of header guard.
// Ensures the contents of this header are included only once.
#endif  // RECOCLI_H
