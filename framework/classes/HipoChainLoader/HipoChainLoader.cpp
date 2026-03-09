//
// Created by Alon Sportes on 03/03/2026.
//

#ifndef HIPOCHAINLOADER_H
#define HIPOCHAINLOADER_H

#include "HipoChainLoader.h"

#ifdef USE_MPI
    #include <numeric>
#endif

// Implementation notes:
//   - This .cpp must not be wrapped in header guards.
//   - The fork-guard probe is the safety mechanism that prevents crashes from corrupted files.
/**
 * @brief Construct a HipoChainLoader with a copy of the given options.
 */
HipoChainLoader::HipoChainLoader(Options opt) : opt_(std::move(opt)) {}

/**
 * @brief Access the loader options.
 */
const HipoChainLoader::Options& HipoChainLoader::GetOptions() const { return opt_; }

/**
 * @brief Build a chain from a single glob pattern.
 *
 * Steps:
 *   1) Expand the glob to an explicit file list.
 *   2) Delegate to BuildFromFiles for validation and insertion.
 */
HipoChainLoader::Result HipoChainLoader::Build(clas12root::HipoChain& chain, const std::string& glob_pattern, const std::string& SampleName) const {
    // Expand the glob pattern into a list of files.
    const std::vector<std::string> files = ExpandGlobFiles(glob_pattern);
    // Manual override: opt_.skip_probe can force trust-mode for direct glob builds.
    return BuildFromFiles(chain, files, SampleName, false);
}

/**
 * @brief Validate a list of files and add the good ones to the chain.
 *
 * Line-by-line flow:
 *   - Reset counters.
 *   - Resolve effective probe policy:
 *       effective_skip_probe = opt_.skip_probe || skip_probe.
 *   - If effective_skip_probe is true, mark all files as good (no fork-probe).
 *   - Otherwise, probe files (MPI or serial fork-guard) and split into good/bad.
 *   - Add accepted files to the chain.
 *   - Configure reader tags and optionally disable QADB.
 *   - Log and optionally print skipped files.
 */
HipoChainLoader::Result HipoChainLoader::BuildFromFiles(clas12root::HipoChain& chain, const std::vector<std::string>& files, const std::string& SampleName, bool skip_probe) const {
    Result res;
    res.n_globbed = static_cast<int>(files.size());
    // Reset counters for this build.
    Num_of_good_hipo_files = 0;
    Num_of_bad_hipo_files = 0;
    Total_num_of_hipo_files = 0;

    // Guard: no files found.
    if (files.empty()) {
        std::ostringstream oss;
        oss << "HipoChainLoader: no files matched";
        throw std::runtime_error(oss.str());
    }

    const bool effective_skip_probe = (opt_.skip_probe || skip_probe);

    // Print progress header if requested.
    if (opt_.print_progress) {
        if (effective_skip_probe) {
            std::cout << env::SYSTEM_COLOR << "\nHipoChainLoader:" << env::INFO_COLOR << " probing disabled, trusting " << res.n_globbed << " file(s)" << env::RESET_COLOR << "\n"
                      << std::flush;
        } else {
            std::cout << env::SYSTEM_COLOR << "\nHipoChainLoader:" << env::INFO_COLOR << " probing " << res.n_globbed << " file(s)" << env::RESET_COLOR << "\n" << std::flush;
        }
    }

    res.added_files.reserve(files.size());
    res.skipped_files.reserve(files.size());

#ifdef USE_MPI
    bool used_mpi_probe = false;
    bool is_root_rank = true;
#endif

    if (effective_skip_probe) {
        // Trust-mode: add all expanded files without probing.
        res.added_files = files;
    } else {
#ifdef USE_MPI
        if (opt_.use_mpi_probe) { used_mpi_probe = ProbeFilesMPI(files, res.added_files, res.skipped_files, is_root_rank); }
#else
        if (opt_.use_mpi_probe) {
            std::cout << env::SYSTEM_COLOR << "HipoChainLoader: " << env::INFO_COLOR << "MPI probe requested, but this build was compiled without USE_MPI. Falling back to serial probe.\n"
                      << env::RESET_COLOR << "\n"
                      << std::flush;
        }
#endif

#ifdef USE_MPI
        if (!used_mpi_probe)
#endif
        {
            int idx = 0;
            // Probe each file using the fork-guard.
            for (const auto& f : files) {
                ++idx;
                const bool good = IsGoodHipoFile_ForkGuard(f, opt_.require_positive_records);
                if (good) {
                    // Good file: add to chain and record.
                    res.added_files.push_back(f);
                } else {
                    // Bad file: record as skipped.
                    res.skipped_files.push_back(f);
                }
                // Print progress every 200 files or at the end.
                if (opt_.print_progress && (idx % 200 == 0 || idx == res.n_globbed)) {
                    std::cout << env::SYSTEM_COLOR << "\tchecked " << env::RESET_COLOR << idx << env::SYSTEM_COLOR << "/" << env::RESET_COLOR << res.n_globbed << env::SYSTEM_COLOR
                              << " | added " << env::RESET_COLOR << res.added_files.size() << env::SYSTEM_COLOR << " | skipped " << env::RESET_COLOR << res.skipped_files.size() << "\n"
                              << std::flush;
                }
            }
        }
    }

    for (const auto& f : res.added_files) { chain.Add(f); }
    Num_of_good_hipo_files = static_cast<int>(res.added_files.size());
    Num_of_bad_hipo_files = static_cast<int>(res.skipped_files.size());
    Total_num_of_hipo_files = res.n_globbed;

    res.n_added = static_cast<int>(res.added_files.size());
    res.n_skipped = static_cast<int>(res.skipped_files.size());

    // Set reader tags for the chain.
    chain.SetReaderTags(opt_.reader_tags);
    // Optionally disable QADB for this chain.
    if (opt_.turn_off_qadb) { chain.db()->turnOffQADB(); }

    bool should_print_and_log = true;
#ifdef USE_MPI
    if (!effective_skip_probe && used_mpi_probe) { should_print_and_log = is_root_rank; }
#endif

    // Write skipped files to log if requested.
    if (should_print_and_log && opt_.log_skipped && !res.skipped_files.empty()) { WriteSkippedLog(res.skipped_files, SampleName); }

    // Optionally print skipped files to stdout.
    if (should_print_and_log && opt_.print_skipped && !res.skipped_files.empty()) {
        std::cout << env::SYSTEM_COLOR << "\nHipoChainLoader: skipped " << env::RESET_COLOR << res.n_skipped << env::SYSTEM_COLOR << " bad file(s)\n";
        for (const auto& f : res.skipped_files) { std::cout << env::SYSTEM_COLOR << "  skipped: " << env::RESET_COLOR << f << "\n" << std::flush; }
    }

    if (should_print_and_log) { std::cout << std::endl; }

    // Guard: if no files were added, throw.
    if (res.n_added == 0) {
        std::ostringstream oss;
        oss << "HipoChainLoader: after filtering, chain has 0 files.";
        throw std::runtime_error(oss.str());
    }

    return res;
}

/**
 * @brief Derive glob patterns from ExperimentParameters.
 *
 * Behavior:
 *   - For GENIE and uniform: return the experiment-provided input glob.
 *   - For data: if ReconHipoDir is empty, expand to per-run globs; otherwise use the input glob.
 *   - For unknown: use the input glob.
 */
std::vector<std::string> HipoChainLoader::MakeInputGlobsFromList(const ExperimentParameters& Experiment) {
    // Use the new ExperimentParameters-owned strings
    const std::string& RecoSamplePath = Experiment.GetRecoSamplePath();
    const std::string& ReconHipoDir = Experiment.GetReconHipoDir();
    const std::string InputHipoFiles = Experiment.GetInputFiles();

    // Infer sample type from Experiment parameters or name/path.
    auto infer_sample_type = [&]() -> int {
        if (Experiment.GetSampleType() == ExperimentParameters::SampleType::DATA_TYPE || Experiment.GetSampleType() == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE ||
            Experiment.GetSampleType() == ExperimentParameters::SampleType::UNIFORM_TYPE) {
            return Experiment.GetSampleType();
        }
        // Fallback: infer from sample name.
        if (bt::FindSubstring(Experiment.GetSampleName(), "_data_") || bt::FindSubstring(Experiment.GetSampleName(), "_data")) return ExperimentParameters::SampleType::DATA_TYPE;
        if (bt::FindSubstring(Experiment.GetSampleName(), "_simulation_") || bt::FindSubstring(Experiment.GetSampleName(), "GENIE") || bt::FindSubstring(Experiment.GetSampleName(), "G18") ||
            bt::FindSubstring(Experiment.GetSampleName(), "GEM21") || bt::FindSubstring(Experiment.GetSampleName(), "SuSa") || bt::FindSubstring(Experiment.GetSampleName(), "Uniform")) {
            // "Uniform" is a proxy for uniform samples.
            return (bt::FindSubstring(Experiment.GetSampleName(), "Uniform")) ? ExperimentParameters::SampleType::UNIFORM_TYPE : ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE;
        }
        // Fallback: infer from RecoSamplePath.
        if (bt::FindSubstring(RecoSamplePath, "clas12/rg-m/production") || (bt::FindSubstring(RecoSamplePath, "rg-m") && bt::FindSubstring(RecoSamplePath, "dst")))
            return ExperimentParameters::SampleType::DATA_TYPE;
        if (bt::FindSubstring(RecoSamplePath, "GENIE_Reco_Samples")) return ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE;
        if (bt::FindSubstring(RecoSamplePath, "Uniform")) return ExperimentParameters::SampleType::UNIFORM_TYPE;
        return ExperimentParameters::SampleType::UNKNOWN_TYPE;
    };
    const int effectiveType = infer_sample_type();

    // Helper for building run globs
    auto make_abs_run_glob = [&](const std::string& base, const std::string& run) -> std::string {
        const bool base_has_leading_slash = (!base.empty() && base.front() == '/');
        return std::string(base_has_leading_slash ? "" : "/") + base + "/" + run + "/*.hipo";
    };

    // Simulation-like samples: just use the input glob.
    if (effectiveType == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE || effectiveType == ExperimentParameters::SampleType::UNIFORM_TYPE) { return {InputHipoFiles}; }

    // Data samples: expand to per-run globs if ReconHipoDir is empty.
    if (effectiveType == ExperimentParameters::SampleType::DATA_TYPE) {
        const std::vector<std::string>* runs = nullptr;
        if (bt::FindSubstring(Experiment.GetSampleName(), "H1_data_2GeV") || bt::FindSubstring(Experiment.GetSampleName(), "H1_data_2070MeV")) {
            runs = &lists::H1_data_2GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "D2_data_2GeV") || bt::FindSubstring(Experiment.GetSampleName(), "D2_data_2070MeV")) {
            runs = &lists::D2_data_2GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "C12_data_2GeV") || bt::FindSubstring(Experiment.GetSampleName(), "C12_data_2070MeV")) {
            runs = &lists::C12_data_2GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_2GeV") || bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_2070MeV")) {
            runs = &lists::Ar40_data_2GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "C12_data_4GeV") || bt::FindSubstring(Experiment.GetSampleName(), "C12_data_4029MeV")) {
            runs = &lists::C12_data_4GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_4GeV") || bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_4029MeV")) {
            runs = &lists::Ar40_data_4GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "H1_data_6GeV") || bt::FindSubstring(Experiment.GetSampleName(), "H1_data_5986MeV")) {
            runs = &lists::H1_data_6GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "D2_data_6GeV") || bt::FindSubstring(Experiment.GetSampleName(), "D2_data_5986MeV")) {
            runs = &lists::D2_data_6GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "C12x4_data_6GeV") || bt::FindSubstring(Experiment.GetSampleName(), "C12x4_data_5986MeV")) {
            runs = &lists::C12x4_data_6GeV_runs;
        } else if (bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_6GeV") || bt::FindSubstring(Experiment.GetSampleName(), "Ar40_data_5986MeV")) {
            runs = &lists::Ar40_data_6GeV_runs;
        }
        // Expand run list only if ReconHipoDir is empty.
        if (runs != nullptr && ReconHipoDir == "") {
            std::vector<std::string> globs;
            for (const auto& run : *runs) { globs.push_back(make_abs_run_glob(RecoSamplePath, run)); }
            return globs;
        }
        // Otherwise, just use the input glob.
        return {InputHipoFiles};
    }

    // Unknown/unsupported sample type: fall back to caller's input glob.
    return {InputHipoFiles};
}

/**
 * @brief Build a chain using ExperimentParameters-derived glob patterns.
 *
 * Steps:
 *   1) Produce one or more glob patterns.
 *   2) Expand all patterns to a single file list.
 *   3) Sort and de-duplicate the list.
 *   4) Select probe policy:
 *      - if opt_.skip_probe is true, probing is always disabled;
 *      - else if sample type is explicit DATA_TYPE, probing is disabled;
 *      - else probing is enabled.
 *   5) Delegate to BuildFromFiles with the computed policy.
 */
HipoChainLoader::Result HipoChainLoader::BuildFromList(clas12root::HipoChain& chain, const ExperimentParameters& Experiment) const {
    // 1) Get the glob patterns.
    std::vector<std::string> globs = MakeInputGlobsFromList(Experiment);
    // 2) Expand all globs and concatenate.
    std::vector<std::string> files;
    for (const auto& pat : globs) {
        std::vector<std::string> expanded = ExpandGlobFiles(pat);
        files.insert(files.end(), expanded.begin(), expanded.end());
    }
    // 3) Sort and de-duplicate.
    std::sort(files.begin(), files.end());
    files.erase(std::unique(files.begin(), files.end()), files.end());
    // 4) Build policy: manual option OR explicit DATA_TYPE skips probing.
    const std::string logName = Experiment.GetSampleName();
    const bool skip_probe = (opt_.skip_probe || Experiment.GetSampleType() == ExperimentParameters::SampleType::DATA_TYPE);
    // 5) Build from the explicit file list using the selected probe policy.
    return BuildFromFiles(chain, files, logName, skip_probe);
}

/**
 * @brief Allocate a chain on the heap and build it from ExperimentParameters.
 */
std::pair<std::unique_ptr<clas12root::HipoChain>, HipoChainLoader::Result> HipoChainLoader::BuildPtrFromList(const ExperimentParameters& Experiment) const {
    auto chain = std::make_unique<clas12root::HipoChain>();
    Result res = BuildFromList(*chain, Experiment);
    return {std::move(chain), std::move(res)};
}

/**
 * @brief Allocate a chain on the heap and build it from a glob pattern.
 */
std::pair<std::unique_ptr<clas12root::HipoChain>, HipoChainLoader::Result> HipoChainLoader::BuildPtr(const std::string& glob_pattern, const std::string& SampleName) const {
    auto chain = std::make_unique<clas12root::HipoChain>();
    Result res = Build(*chain, glob_pattern, SampleName);
    return {std::move(chain), std::move(res)};
}

/**
 * @brief Expand a glob() pattern into a sorted list of paths.
 */
std::vector<std::string> HipoChainLoader::ExpandGlobFiles(const std::string& pattern) {
    std::vector<std::string> files;
    // Use glob_t to expand the pattern.
    glob_t glob_result;
    std::memset(&glob_result, 0, sizeof(glob_result));
    const int ret = ::glob(pattern.c_str(), GLOB_TILDE, nullptr, &glob_result);
    if (ret == 0) {
        files.reserve(glob_result.gl_pathc);
        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
            if (glob_result.gl_pathv[i]) { files.emplace_back(glob_result.gl_pathv[i]); }
        }
    }
    ::globfree(&glob_result);
    // Sort for reproducibility.
    std::sort(files.begin(), files.end());
    return files;
}

/**
 * @brief Determine whether a file is safe to read.
 *
 * Implementation:
 *   - fork()
 *   - In the child: open the file with hipo::reader and read nRecords.
 *   - Exit with code 0 on success; nonzero on failure.
 *   - In the parent: waitpid() and interpret the exit status.
 */
bool HipoChainLoader::IsGoodHipoFile_ForkGuard(const std::string& file, bool require_positive_records) {
    // Fork a child process to probe the file.
    const pid_t pid = ::fork();
    if (pid < 0) { return false; }
    if (pid == 0) {
        // Child: try to open the file and check nRecords.
        int code = 1;
        try {
            hipo::reader r;
            r.open(file.c_str());
            const long nrec = r.getNRecords();
            if (require_positive_records) {
                code = (nrec > 0) ? 0 : 1;
            } else {
                code = 0;
            }
        } catch (...) { code = 1; }
        _exit(code);  // Exit with code: 0 if good, 1 if bad.
    }
    // Parent: wait for child and check exit status.
    int status = 0;
    const pid_t w = ::waitpid(pid, &status, 0);
    if (w < 0) { return false; }
    if (WIFEXITED(status)) { return WEXITSTATUS(status) == 0; }
    // Any other case: treat as bad.
    return false;
}

#ifdef USE_MPI
bool HipoChainLoader::ProbeFilesMPI(const std::vector<std::string>& files, std::vector<std::string>& added_files, std::vector<std::string>& skipped_files, bool& is_root_rank) const {
    is_root_rank = true;
    int mpi_finalized = 0;
    MPI_Finalized(&mpi_finalized);
    if (mpi_finalized) { return false; }

    int mpi_initialized = 0;
    MPI_Initialized(&mpi_initialized);

    bool did_mpi_init = false;
    if (!mpi_initialized) {
        if (!opt_.mpi_auto_init_finalize) { return false; }
        int argc = 0;
        char** argv = nullptr;
        if (MPI_Init(&argc, &argv) != MPI_SUCCESS) { return false; }
        did_mpi_init = true;
    }

    int rank = 0;
    int size = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    is_root_rank = (rank == 0);

    // A single rank gives no acceleration; fall back to the regular serial path.
    if (size <= 1) {
        if (did_mpi_init) { MPI_Finalize(); }
        return false;
    }

    if (rank == 0 && opt_.print_progress) {
        std::cout << env::SYSTEM_COLOR << "HipoChainLoader: " << env::INFO_COLOR << "MPI probing with " << env::RESET_COLOR << size << env::INFO_COLOR << " ranks" << env::RESET_COLOR << "\n"
                  << std::flush;
    }

    std::vector<int> local_good_idx;
    std::vector<int> local_bad_idx;
    local_good_idx.reserve(files.size() / static_cast<size_t>(size) + 1);
    local_bad_idx.reserve(files.size() / static_cast<size_t>(size) + 1);

    for (size_t i = static_cast<size_t>(rank); i < files.size(); i += static_cast<size_t>(size)) {
        const bool good = IsGoodHipoFile_ForkGuard(files[i], opt_.require_positive_records);
        if (good) {
            local_good_idx.push_back(static_cast<int>(i));
        } else {
            local_bad_idx.push_back(static_cast<int>(i));
        }
    }

    const int local_good_n = static_cast<int>(local_good_idx.size());
    const int local_bad_n = static_cast<int>(local_bad_idx.size());

    std::vector<int> good_counts;
    std::vector<int> bad_counts;
    if (rank == 0) {
        good_counts.resize(size);
        bad_counts.resize(size);
    }

    MPI_Gather(&local_good_n, 1, MPI_INT, (rank == 0 ? good_counts.data() : nullptr), 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&local_bad_n, 1, MPI_INT, (rank == 0 ? bad_counts.data() : nullptr), 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> all_good_idx;
    std::vector<int> all_bad_idx;
    std::vector<int> good_displs;
    std::vector<int> bad_displs;
    int total_good = 0;
    int total_bad = 0;

    if (rank == 0) {
        good_displs.resize(size, 0);
        bad_displs.resize(size, 0);
        for (int r = 1; r < size; ++r) {
            good_displs[r] = good_displs[r - 1] + good_counts[r - 1];
            bad_displs[r] = bad_displs[r - 1] + bad_counts[r - 1];
        }
        total_good = std::accumulate(good_counts.begin(), good_counts.end(), 0);
        total_bad = std::accumulate(bad_counts.begin(), bad_counts.end(), 0);
        all_good_idx.resize(static_cast<size_t>(total_good));
        all_bad_idx.resize(static_cast<size_t>(total_bad));
    }

    MPI_Gatherv(local_good_idx.data(), local_good_n, MPI_INT, (rank == 0 ? all_good_idx.data() : nullptr), (rank == 0 ? good_counts.data() : nullptr),
                (rank == 0 ? good_displs.data() : nullptr), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gatherv(local_bad_idx.data(), local_bad_n, MPI_INT, (rank == 0 ? all_bad_idx.data() : nullptr), (rank == 0 ? bad_counts.data() : nullptr), (rank == 0 ? bad_displs.data() : nullptr),
                MPI_INT, 0, MPI_COMM_WORLD);

    // Reconstruct deterministic order on rank 0, then broadcast to all ranks.
    if (rank == 0) {
        std::sort(all_good_idx.begin(), all_good_idx.end());
        all_good_idx.erase(std::unique(all_good_idx.begin(), all_good_idx.end()), all_good_idx.end());
        std::sort(all_bad_idx.begin(), all_bad_idx.end());
        all_bad_idx.erase(std::unique(all_bad_idx.begin(), all_bad_idx.end()), all_bad_idx.end());
    }

    int n_good_global = (rank == 0 ? static_cast<int>(all_good_idx.size()) : 0);
    int n_bad_global = (rank == 0 ? static_cast<int>(all_bad_idx.size()) : 0);
    MPI_Bcast(&n_good_global, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n_bad_global, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        all_good_idx.resize(static_cast<size_t>(n_good_global));
        all_bad_idx.resize(static_cast<size_t>(n_bad_global));
    }
    if (n_good_global > 0) { MPI_Bcast(all_good_idx.data(), n_good_global, MPI_INT, 0, MPI_COMM_WORLD); }
    if (n_bad_global > 0) { MPI_Bcast(all_bad_idx.data(), n_bad_global, MPI_INT, 0, MPI_COMM_WORLD); }

    added_files.clear();
    skipped_files.clear();
    added_files.reserve(all_good_idx.size());
    skipped_files.reserve(all_bad_idx.size());

    for (const int idx : all_good_idx) {
        if (idx >= 0 && static_cast<size_t>(idx) < files.size()) { added_files.push_back(files[static_cast<size_t>(idx)]); }
    }
    for (const int idx : all_bad_idx) {
        if (idx >= 0 && static_cast<size_t>(idx) < files.size()) { skipped_files.push_back(files[static_cast<size_t>(idx)]); }
    }

    if (rank == 0 && opt_.print_progress) {
        std::cout << env::SYSTEM_COLOR << "\tchecked " << env::RESET_COLOR << files.size() << env::SYSTEM_COLOR << "/" << env::RESET_COLOR << files.size() << env::SYSTEM_COLOR << " | added "
                  << env::RESET_COLOR << added_files.size() << env::SYSTEM_COLOR << " | skipped " << env::RESET_COLOR << skipped_files.size() << "\n"
                  << std::flush;
    }

    if (did_mpi_init) { MPI_Finalize(); }
    return true;
}
#endif

/**
 * @brief Return a local timestamp string for human-readable logs.
 */
std::string HipoChainLoader::NowString() {
    using clock = std::chrono::system_clock;
    const auto now = clock::now();
    const std::time_t t = clock::to_time_t(now);
    std::tm tm_buf;
    localtime_r(&t, &tm_buf);
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

/**
 * @brief Write the skipped-files list to disk.
 */
void HipoChainLoader::WriteSkippedLog(const std::vector<std::string>& skipped, const std::string& SampleName) const {
    // Choose open mode: append or truncate.
    std::ios_base::openmode mode = std::ios::out;
    mode |= (opt_.append_log ? std::ios::app : std::ios::trunc);
    std::ofstream out(opt_.log_path, mode);
    if (!out) {
        std::cerr << "\nHipoChainLoader: could not open log file: " << opt_.log_path << "\n" << std::flush;
        return;
    }
    // Write header and timestamp.
    out << "============================================================\n";
    out << "Timestamp: " << NowString() << "\n";
    // Optionally include sample name.
    if (!SampleName.empty()) { out << "Sample: " << SampleName << "\n"; }
    out << "Skipped bad HIPO files:\n";
    for (const auto& f : skipped) { out << "  " << f << "\n"; }
    out << "\n";
}

// AddToHipoChain function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* AddToHipoChain function */
// This is the old function used to add runs to the HipoChain
void HipoChainLoader::AddToHipoChain(const ExperimentParameters& Experiment, HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                                     const std::string& InputHipoFiles) {
    bool PrintOut = true;

    if (Experiment.GetSampleType() == ExperimentParameters::SampleType::DATA_TYPE) {
        if (sn == "D2_data_2070MeV") {
            if (ReconHipoDir == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                std::vector<std::string> Runs = {"015567", "015573", "015578", "015583", "015590", "015595", "015602", "015608", "015613", "015618", "015624", "015568", "015574", "015579",
                                                 "015586", "015591", "015598", "015603", "015609", "015614", "015619", "015625", "015569", "015575", "015580", "015587", "015592", "015599",
                                                 "015604", "015610", "015615", "015620", "015626", "015570", "015576", "015581", "015588", "015593", "015600", "015606", "015611", "015616",
                                                 "015622", "015627", "015572", "015577", "015582", "015589", "015594", "015601", "015607", "015612", "015617", "015623"};

                for (int i = 0; i < Runs.size(); i++) {
                    std::string TempInputHipoFiles = "/" + RecoSamplePath + "/" + Runs.at(i) + "/*.hipo";
                    chain.Add(TempInputHipoFiles.c_str());

                    if (PrintOut) { std::cout << env::SYSTEM_COLOR << TempInputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n" << std::flush; }
                }

                if (PrintOut) { std::cout << endl; }
            }
        } else if (sn == "C12x4_data_5986MeV") {
            if (ReconHipoDir == "") {
                /* Data in cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon */
                std::vector<std::string> Runs = {"015186", "015187", "015188", "015189", "015190", "015191", "015192", "015193", "015194", "015196", "015199", "015200", "015202",
                                                 "015203", "015204", "015205", "015206", "015207", "015210", "015212", "015213", "015214", "015215", "015217", "015219", "015220",
                                                 "015221", "015223", "015224", "015225", "015226", "015228", "015234", "015235", "015236", "015238", "015239", "015240", "015241",
                                                 "015242", "015243", "015245", "015246", "015247", "015248", "015249", "015250", "015252", "015253", "015254", "015255", "015257",
                                                 "015258", "015259", "015260", "015261", "015262", "015263", "015264", "015265", "015266", "015269", "015270", "015271", "015272",
                                                 "015273", "015274", "015275", "015278", "015279", "015280", "015282", "015283", "015284", "015286", "015287", "015288", "015289",
                                                 "015290", "015291", "015292", "015293", "015294", "015295", "015296", "015298", "015300", "015301", "015302", "015303", "015304",
                                                 "015305", "015306", "015307", "015308", "015309", "015310", "015311", "015312", "015313", "015314", "015316", "015317"};

                for (int i = 0; i < Runs.size(); i++) {
                    std::string TempInputHipoFiles = "/" + RecoSamplePath + "/" + Runs.at(i) + "/*.hipo";
                    chain.Add(TempInputHipoFiles.c_str());

                    if (PrintOut) { std::cout << env::SYSTEM_COLOR << TempInputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n" << std::flush; }
                }

                if (PrintOut) { std::cout << endl; }
            }
        } else {
            chain.Add(InputHipoFiles.c_str());
        }
    } else if (Experiment.GetSampleType() == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE) {
        chain.Add(InputHipoFiles.c_str());

        if (PrintOut) { std::cout << env::SYSTEM_COLOR << InputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n\n" << std::flush; }
    }
}
#pragma endregion

// AddToHipoChainFromList function --------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* AddToHipoChainFromList function */
void HipoChainLoader::AddToHipoChainFromList(const ExperimentParameters& Experiment, HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath,
                                             const std::string& ReconHipoDir, const std::string& InputHipoFiles) {
    const bool PrintOut = true;

    // Determine the effective sample type as robustly as possible.
    // The legacy implementation keyed off DataSample/SimulationSample, which are not guaranteed to be set in all code paths.
    // Here we prefer the configured `sampleType`, and fall back to inference from `sn` and `RecoSamplePath` if needed.
    auto infer_sample_type = [&](void) -> int {
        if (Experiment.GetSampleType() == ExperimentParameters::SampleType::DATA_TYPE || Experiment.GetSampleType() == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE ||
            Experiment.GetSampleType() == ExperimentParameters::SampleType::UNIFORM_TYPE) {
            return Experiment.GetSampleType();
        }

        // Fallback: infer from `sn`
        if (bt::FindSubstring(sn, "_data_") || bt::FindSubstring(sn, "_data")) { return ExperimentParameters::SampleType::DATA_TYPE; }
        if (bt::FindSubstring(sn, "_simulation_") || bt::FindSubstring(sn, "GENIE") || bt::FindSubstring(sn, "G18") || bt::FindSubstring(sn, "GEM21") || bt::FindSubstring(sn, "SuSa") ||
            bt::FindSubstring(sn, "Uniform")) {
            // Note: "Uniform" here is used as a proxy for uniform samples.
            return (bt::FindSubstring(sn, "Uniform")) ? ExperimentParameters::SampleType::UNIFORM_TYPE : ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE;
        }

        // Fallback: infer from `RecoSamplePath`
        if (bt::FindSubstring(RecoSamplePath, "clas12/rg-m/production") || (bt::FindSubstring(RecoSamplePath, "rg-m") && bt::FindSubstring(RecoSamplePath, "dst"))) {
            return ExperimentParameters::SampleType::DATA_TYPE;
        }
        if (bt::FindSubstring(RecoSamplePath, "GENIE_Reco_Samples")) { return ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE; }
        if (bt::FindSubstring(RecoSamplePath, "Uniform")) { return ExperimentParameters::SampleType::UNIFORM_TYPE; }

        return ExperimentParameters::SampleType::UNKNOWN_TYPE;
    };

    const int effectiveType = infer_sample_type();

    // Ensure we build the same absolute paths as the legacy code:
    // legacy used "/" + AnalyzeFilePath + "/" + run + "/*.hipo" assuming AnalyzeFilePath had no leading '/'.
    auto make_abs_run_glob = [&](const std::string& base, const std::string& run) -> std::string {
        const bool base_has_leading_slash = (!base.empty() && base.front() == '/');
        return std::string(base_has_leading_slash ? "" : "/") + base + "/" + run + "/*.hipo";
    };

    auto add_runs = [&](const std::vector<std::string>& runs) {
        for (size_t i = 0; i < runs.size(); ++i) {
            const std::string TempInputHipoFiles = make_abs_run_glob(RecoSamplePath, runs.at(i));
            chain.Add(TempInputHipoFiles.c_str());
            if (PrintOut) { std::cout << env::SYSTEM_COLOR << TempInputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n" << std::flush; }
        }
        if (PrintOut) { std::cout << endl; }
    };

    // Simulation-like samples: the caller should provide an explicit glob.
    if (effectiveType == ExperimentParameters::SampleType::GENIE_SIMULATION_TYPE || effectiveType == ExperimentParameters::SampleType::UNIFORM_TYPE) {
        chain.Add(InputHipoFiles.c_str());
        if (PrintOut) { std::cout << env::SYSTEM_COLOR << InputHipoFiles << " directory added to HipoChain!" << env::RESET_COLOR << "\n\n" << std::flush; }
        return;
    }

    // Data samples
    if (effectiveType == ExperimentParameters::SampleType::DATA_TYPE) {
        const std::vector<std::string>* runs = nullptr;

        // Allow names that include a run suffix (e.g. "..._run_015186").
        if (bt::FindSubstring(sn, "H1_data_2GeV") || bt::FindSubstring(sn, "H1_data_2070MeV")) {
            runs = &lists::H1_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "D2_data_2GeV") || bt::FindSubstring(sn, "D2_data_2070MeV")) {
            runs = &lists::D2_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "C12_data_2GeV") || bt::FindSubstring(sn, "C12_data_2070MeV")) {
            runs = &lists::C12_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "Ar40_data_2GeV") || bt::FindSubstring(sn, "Ar40_data_2070MeV")) {
            runs = &lists::Ar40_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "C12_data_4GeV") || bt::FindSubstring(sn, "C12_data_4029MeV")) {
            runs = &lists::C12_data_4GeV_runs;
        } else if (bt::FindSubstring(sn, "Ar40_data_4GeV") || bt::FindSubstring(sn, "Ar40_data_4029MeV")) {
            runs = &lists::Ar40_data_4GeV_runs;
        } else if (bt::FindSubstring(sn, "H1_data_6GeV") || bt::FindSubstring(sn, "H1_data_5986MeV")) {
            runs = &lists::H1_data_6GeV_runs;
        } else if (bt::FindSubstring(sn, "D2_data_6GeV") || bt::FindSubstring(sn, "D2_data_5986MeV")) {
            runs = &lists::D2_data_6GeV_runs;
        } else if (bt::FindSubstring(sn, "C12x4_data_6GeV") || bt::FindSubstring(sn, "C12x4_data_5986MeV")) {
            runs = &lists::C12x4_data_6GeV_runs;
        } else if (bt::FindSubstring(sn, "Ar40_data_6GeV") || bt::FindSubstring(sn, "Ar40_data_5986MeV")) {
            runs = &lists::Ar40_data_6GeV_runs;
        }

        // Match the legacy behavior exactly:
        // Expand the run list only when the user did not request a specific run directory.
        if (runs != nullptr && ReconHipoDir == "") {
            add_runs(*runs);
            return;
        }

        // Otherwise, fall back to the explicit file/glob passed by the caller.
        chain.Add(InputHipoFiles.c_str());
        return;
    }

    // Unknown/unsupported sample type: fall back to whatever the caller provided.
    chain.Add(InputHipoFiles.c_str());
}
#pragma endregion

#endif  // HIPOCHAINLOADER_H