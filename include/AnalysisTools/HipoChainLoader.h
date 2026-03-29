//
// Created by Alon Sportes on 03/03/2026.
//

#ifndef HIPOCHAINLOADER_H
#define HIPOCHAINLOADER_H

#include <glob.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Include libraries:
#include "../libraries/general_utilities/lists.h"
#include "../libraries/general_utilities/utilities.h"

// Include Settings:
#include "../Settings/setup/debugging.h"
#include "../Settings/setup/path_definitions.h"

// Include AnalysisTools:
#include "ExperimentParameters.h"

// MultiThread:
#include "../MultiThread/MPI_include.h"

// Include CLAS12 libraries:
#include "../CLAS12/clas12_include.h"

namespace env = environment;
namespace pd = path_definitions;
using HipoChain = clas12root::HipoChain;

// TODO: debug class - kills too many files.

/**
 * @class HipoChainLoader
 *
 * Build a clas12root::HipoChain from a glob pattern, while safely excluding bad HIPO files.
 *
 * Rationale:
 *   Some corrupted HIPO files can hard-abort (assert/abort/segfault) inside the reader stack.
 *   C++ try/catch cannot catch abort/assert. This loader probes files in a forked child process
 *   so bad files cannot crash the parent process.
 */
class HipoChainLoader {
   public:
    /**
     * @struct Options
     *
     * Runtime configuration for HipoChainLoader.
     *
     * Notes:
     *   - `reader_tags` must be `std::vector<long>` because clas12root::HipoChain::SetReaderTags expects long.
     *   - `require_positive_records` rejects empty files (nRecords <= 0).
     */
    struct Options {
        bool print_progress;  ///< Print periodic progress while probing files.
        bool print_skipped;   ///< Print the list of skipped files at the end.

        bool log_skipped;      ///< If true, append skipped files to `log_path`.
        std::string log_path;  ///< Path to the skipped-files log.
        bool append_log;       ///< If true, append to `log_path`; otherwise overwrite.

        std::vector<long> reader_tags;  ///< Reader tags passed to clas12root::HipoChain.
        bool turn_off_qadb;             ///< If true, disables QADB for the chain.

        bool require_positive_records;  ///< If true, require nRecords > 0 in the fork-guard probe.
        bool use_mpi_probe;             ///< If true and MPI is available, distribute probe work across MPI ranks.
        bool mpi_auto_init_finalize;    ///< If true, call MPI_Init/MPI_Finalize when needed.
        bool skip_probe;                ///< If true, bypass probing and trust all expanded files.

        Options()
            : print_progress(true),
              print_skipped(true),
              log_skipped(true),
              log_path(pd::skipped_files_list_prefix),
              append_log(true),
              reader_tags({0L}),
              turn_off_qadb(true),
              require_positive_records(true),
              use_mpi_probe(false),
              mpi_auto_init_finalize(true),
              skip_probe(false) {}
    };

    /**
     * @struct Result
     *
     * Summary of a build operation.
     */
    struct Result {
        int n_globbed;  ///< Number of files produced by glob expansion.
        int n_added;    ///< Number of files added to the chain.
        int n_skipped;  ///< Number of files rejected by fork-guard checks.

        std::vector<std::string> added_files;    ///< Full paths of files accepted and added.
        std::vector<std::string> skipped_files;  ///< Full paths of files rejected.

        Result() : n_globbed(0), n_added(0), n_skipped(0) {}
    };

    /**
     * @brief Get the loader configuration.
     */
    const Options& GetOptions() const;

    /**
     * @brief Construct a loader with the given options.
     */
    explicit HipoChainLoader(Options opt = Options());

    /**
     * @brief Build a chain from a single glob pattern.
     *
     * @param chain Output chain to populate.
     * @param glob_pattern Glob of input files.
     * @param SampleName Optional label used in skipped-files log.
     */
    Result Build(clas12root::HipoChain& chain, const std::string& glob_pattern, const std::string& SampleName = "") const;

    /**
     * @brief Convenience wrapper that allocates the chain and returns it with the build result.
     */
    std::pair<std::unique_ptr<clas12root::HipoChain>, Result> BuildPtr(const std::string& glob_pattern, const std::string& SampleName = "") const;

    /**
     * @brief Build a chain using ExperimentParameters, including run-list expansion for data.
     *
     * The function expands one or more glob patterns to file paths, then:
     *   - if `opt_.skip_probe` is true, probing is skipped for all sample types;
     *   - else if `Experiment.GetSampleType() == ExperimentParameters::SampleType::DATA_TYPE`, it skips probing
     *     and assumes all expanded files are valid;
     *   - otherwise, it validates files via the fork-guard probe.
     */
    Result BuildFromList(clas12root::HipoChain& chain, const ExperimentParameters& Experiment) const;

    /**
     * @brief Convenience wrapper for BuildFromList that returns a heap-allocated chain.
     */
    std::pair<std::unique_ptr<clas12root::HipoChain>, Result> BuildPtrFromList(const ExperimentParameters& Experiment) const;

    // AddToHipoChain function ------------------------------------------------------------------------------------------------------------------------------------------

    // This is the old function used to add runs to the HipoChain
    void AddToHipoChain(const ExperimentParameters& Experiment, HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                        const std::string& InputHipoFiles);

    // AddToHipoChainFromList function ----------------------------------------------------------------------------------------------------------------------------------

    void AddToHipoChainFromList(const ExperimentParameters& Experiment, HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                                const std::string& InputHipoFiles);

    // Getters for file counts ------------------------------------------------------------------------------------------------------------------------------------------

    int GetNumOfGoodHipoFiles() const { return Num_of_good_hipo_files; }
    int GetNumOfBadHipoFiles() const { return Num_of_bad_hipo_files; }
    int GetTotalNumOfHipoFiles() const { return Total_num_of_hipo_files; }

   private:
    Options opt_;

    // Per-build counters. Marked mutable so they can be updated from const BuildFromFiles().
    mutable int Num_of_good_hipo_files = 0;   ///< Files that passed the fork-guard probe.
    mutable int Num_of_bad_hipo_files = 0;    ///< Files that failed the probe.
    mutable int Total_num_of_hipo_files = 0;  ///< Total files checked in the last build.

    /** @brief Expand a glob pattern into a sorted list of file paths. */
    static std::vector<std::string> ExpandGlobFiles(const std::string& pattern);

    /**
     * @brief Probe a HIPO file in a forked child process.
     *
     * This guards the parent process from aborts/asserts triggered by corrupted files.
     */
    static bool IsGoodHipoFile_ForkGuard(const std::string& file, bool require_positive_records);

#ifdef USE_MPI
    /**
     * @brief Probe files in parallel using MPI ranks and reconstruct ordered result vectors.
     *
     * @return true if MPI probing was used; false if caller should fall back to serial probing.
     */
    bool ProbeFilesMPI(const std::vector<std::string>& files, std::vector<std::string>& added_files, std::vector<std::string>& skipped_files, bool& is_root_rank) const;
#endif

    /** @brief Get a local timestamp string for logs. */
    static std::string NowString();

    /** @brief Append skipped files to the configured log file. */
    void WriteSkippedLog(const std::vector<std::string>& skipped, const std::string& SampleName) const;

    /**
     * @brief Shared implementation used by all build entry points.
     *
     * Takes an already expanded list of file paths and adds them to the chain.
     *
     * Behavior:
     *   - If `(opt_.skip_probe || skip_probe)` is false: validate each file through the fork-guard probe and skip bad files.
     *   - If `(opt_.skip_probe || skip_probe)` is true: bypass probing and treat all files as good.
     */
    Result BuildFromFiles(clas12root::HipoChain& chain, const std::vector<std::string>& files, const std::string& SampleName, bool skip_probe = false) const;

    /**
     * @brief Derive one or more glob patterns based on ExperimentParameters.
     *
     * For data samples, this may expand into per-run globs when no explicit recon subdir was requested.
     */
    static std::vector<std::string> MakeInputGlobsFromList(const ExperimentParameters& Experiment);
};

/**
 * @class HipoChainLoader
 *
 * Build a clas12root::HipoChain from a glob pattern, while safely excluding bad HIPO files.
 *
 * Rationale:
 *   Some corrupted HIPO files can hard-abort (assert/abort/segfault) inside the reader stack.
 *   C++ try/catch cannot catch abort/assert. This loader probes files in a forked child process
 *   so bad files cannot crash the parent process.
 *
 * Typical usage:
 *   - For a single glob: `BuildPtr("<path>*.hipo")`
 *   - For data run lists: `BuildPtrFromList(ExperimentParameters)`
 */

#endif  // HIPOCHAINLOADER_H
