
# The `HipoChainLoader` class

**Author:** Alon Sportes  
**Project:** `2N-Analyzer-e4nu`

## Overview

`HipoChainLoader` is a utility that builds a `clas12root::HipoChain` from one or more file globs.

It supports two build modes:

- **Safe probe mode**: probe each file in a forked child process and skip bad files.
- **Trust mode**: skip probing and add all files directly.

The key problem it solves in safe mode is that some bad HIPO files can trigger `abort()`, assertions, or other hard failures inside the reader stack. Those failures cannot be handled with C++ exceptions. `HipoChainLoader` avoids crashing your analysis by probing each file in a separate child process using `fork()`.

## How it works

Shared flow:

1. Expand input glob(s) into an explicit list of files.
2. Choose policy:
   - if `Options::skip_probe == true`, mark all files as accepted.
   - else if call-site `skip_probe == true` (for example, explicit `DATA_TYPE` in `BuildFromList`), mark all files as accepted.
   - otherwise run probe flow below.
3. Add accepted files to the chain.

Probe flow (`skip_probe == false`):

1. For each file, `fork()` a child process.
2. In the child process, attempt to open the file with `hipo::reader` and read `nRecords`.
3. Accept only files that pass the probe.

### Probe rule

The probe uses:

- `hipo::reader r; r.open(file.c_str());`
- `long nrec = r.getNRecords();`

If `Options::require_positive_records` is `true`, the file is accepted only if `nrec > 0`.

If the child exits with a nonzero status (including exceptions, aborts, crashes, or `nrec <= 0` when required), the file is treated as bad and skipped.

## Public API

### `Build`

Build into an existing chain from a single glob pattern.

- Input: `glob_pattern` such as `/path/to/reconhipo/*.hipo`
- Output: `Result` summary containing accepted and rejected files

### `BuildPtr`

Convenience wrapper that allocates the chain and returns `{chainPtr, result}`.

### `BuildFromList`

Build into an existing chain using `ExperimentParameters`.

This reproduces the analysis-level behavior for choosing input globs:

- Simulation-like samples (`GENIE_SIMULATION_TYPE` or `UNIFORM_TYPE`): use `Experiment.GetInputFiles()` and probe files.
- Data samples (`DATA_TYPE`):
  - If `Experiment.GetReconHipoDir()` is empty and the sample name matches a known dataset, expand into per-run globs using the predefined run lists in `lists.h`.
  - Otherwise, use `Experiment.GetInputFiles()`.
- For explicit `DATA_TYPE`, probing is skipped and all expanded files are added directly.
- If `Options::skip_probe == true`, probing is skipped regardless of sample type.
- Unknown type: fall back to `Experiment.GetInputFiles()`.

Internally this path uses `MakeInputGlobsFromList()` to produce one or more glob patterns, expands them, sorts and de-duplicates the resulting file list, then decides whether to probe based on `Options::skip_probe` and explicit sample type.

### `BuildPtrFromList`

Convenience wrapper that allocates the chain and builds it from `ExperimentParameters`.

## Options

`HipoChainLoader::Options` controls logging, printing, reader tags, and the probe condition.

Fields (with current defaults):

- `print_progress = true`  
  Print periodic progress while probing (prints every 200 files and on the last file).

- `print_skipped = true`  
  Print the list of skipped files at the end (only if there are any).

- `log_skipped = true`  
  If `true`, write skipped files to disk.

- `log_path = path_definitions::skipped_files_list_prefix`  
  Path to the skipped-files log.

- `append_log = true`  
  If `true`, append to `log_path`; otherwise overwrite.

- `reader_tags = {0L}`  
  Tags passed to `clas12root::HipoChain::SetReaderTags` (must be `std::vector<long>`).

- `turn_off_qadb = true`  
  If `true`, calls `chain.db()->turnOffQADB()`.

- `require_positive_records = true`  
  If `true`, reject files with `nRecords <= 0` in the fork-guard probe.

- `skip_probe = false`  
  Manual override. If `true`, bypass probing and trust all expanded files.

## Result

`HipoChainLoader::Result` summarizes a build operation:

- `n_globbed`: number of files produced by glob expansion
- `n_added`: number of files added to the chain
- `n_skipped`: number of files rejected (always `0` in trust mode)
- `added_files`: list of accepted paths
- `skipped_files`: list of rejected paths

The class also maintains per-build counters that can be accessed via:

- `GetNumOfGoodHipoFiles()`
- `GetNumOfBadHipoFiles()`
- `GetTotalNumOfHipoFiles()`

These counters reflect the most recent build call.

## Exceptions and guards

The builder throws `std::runtime_error` in two cases:

- No files matched the input glob(s).
- After filtering/policy application, zero files were added to the chain.

## Example usage

### Build from a single glob

```cpp
HipoChainLoader::Options opt{};
opt.log_skipped = true;
opt.log_path = OutputDir + pd::skipped_files_list_prefix;
opt.reader_tags = {0L};
opt.skip_probe = true; // Manual override: disable probing.

HipoChainLoader loader(opt);

auto [chainPtr, res] = loader.BuildPtr("/path/to/reconhipo/*.hipo", "MySample");
auto& chain = *chainPtr;
```

### Build using `ExperimentParameters`

```cpp
ExperimentParameters exp(RecoSamplePath, ReconHipoDir);

HipoChainLoader loader;
auto [chainPtr, res] = loader.BuildPtrFromList(exp);
auto& chain = *chainPtr;
```

## Legacy functions

The class still contains two older helpers:

- `AddToHipoChain(...)`
- `AddToHipoChainFromList(...)`

These functions directly add glob patterns to the chain (including hardcoded run lists in the older version) and do not use the fork-guard probe. Prefer the `Build*` API for consistent policy handling and result reporting.

## Notes and caveats

- Probing uses `fork()`, so it is intended for Linux and other POSIX systems.
- If you run on many files, fork-probing is safer but slower than blindly adding all files.
- The skipped-files log includes a timestamp and (when provided) the sample name.