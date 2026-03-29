#!/bin/tcsh

# -------------------------------------------------------------------------------------------------
# build_and_run.csh
# -------------------------------------------------------------------------------------------------
# This script performs the main runtime workflow after the outer wrapper script (`run.csh`) has
# already:
#   1) resolved the repository path,
#   2) parsed the high-level control flags,
#   3) set terminal colors, and
#   4) optionally run the repository update helper.
#
# Responsibilities of this script:
#   - print the current git commit and branch,
#   - optionally pull the latest changes,
#   - decide whether a rebuild is actually required,
#   - configure and build the C++ project only when needed,
#   - run the TwoNAnalyzerReco executable with the selected YAML configuration files.
# -------------------------------------------------------------------------------------------------

# Register a Ctrl-C handler for the whole script.
# If the user interrupts during git pull, CMake configure/build, or the C++ executable run,
# execution jumps to the RUN_INTERRUPTED label below.
onintr RUN_INTERRUPTED

# Print the current HEAD commit so it is easy to verify which revision is being used.
echo "${COLOR_START}HEAD:${COLOR_END}"
git log -1 --oneline
echo ""

# Print the currently checked out branch for sanity checks on remote machines.
echo "${COLOR_START}Branch:${COLOR_END}"
git branch --show-current
echo ""

# Echo the workflow control flags that were parsed earlier by run.csh.
# These determine whether git pull, build, and run are enabled for this invocation.
echo "${COLOR_START}- Run controls -------------------------------------------------------------------------------------${COLOR_END}"
echo "${COLOR_START}git pull:${COLOR_END} ${DO_GIT_PULL}"
echo "${COLOR_START}build:${COLOR_END}    ${DO_BUILD}"
echo "${COLOR_START}run:${COLOR_END}      ${DO_RUN}"
echo ""

# Record the commit before pulling so we can later compare file changes and decide
# whether a rebuild is required.
set old_head = `git rev-parse HEAD`

# Pull latest remote changes using fast-forward only.
# This avoids creating merge commits from the script and fails if history diverged.
if ($DO_GIT_PULL) then
    echo "${COLOR_START}- Pulling latest changes ---------------------------------------------------------------------------${COLOR_END}"
    git pull --ff-only
    if ($status != 0) then
        echo "${COLOR_ERR}git pull failed. Resolve git state and retry.${COLOR_END}"
        exit 1
    endif
    echo ""
else
    echo "${COLOR_START}- Skipping git pull --------------------------------------------------------------------------------${COLOR_END}"
    echo ""
endif


# Record the commit after the optional pull step.
# The diff between the last built commit and this commit is used to decide whether
# source or CMake changes require a rebuild.
set new_head = `git rev-parse HEAD`

# Path to the built executable and to the file that records the commit hash of the
# last successful build.  The build stamp lets us skip rebuilding when only runtime
# configuration files changed.
set exe_path = "build/apps/TwoNAnalyzerReco"
set build_stamp = "build/.last_built_commit"

# Initialize the automatic build-decision state.
# SHOULD_BUILD is set to 1 only when one of the checks below shows that the existing
# executable cannot be safely reused.
set SHOULD_BUILD = 0
set BUILD_REASON = "No build needed; runtime-only changes or no relevant changes detected."

if ($DO_BUILD) then
    # No build directory exists at all, so configuration/build must run.
    if (! -d build) then
        set SHOULD_BUILD = 1
        set BUILD_REASON = "Build directory does not exist."
    # The build directory exists, but the expected executable is missing.
    else if (! -e "$exe_path") then
        set SHOULD_BUILD = 1
        set BUILD_REASON = "Executable does not exist."
    # The executable exists, but there is no record of which commit produced it.
    else if (! -e "$build_stamp") then
        set SHOULD_BUILD = 1
        set BUILD_REASON = "No previous successful build stamp was found."
    else
        # Load the commit hash from the previous successful build.
        set last_built_commit = `cat "$build_stamp"`
        # Verify that the recorded commit still exists in the local repository history.
        git cat-file -e ${last_built_commit}^{commit} >& /dev/null
        if ($status != 0) then
            set SHOULD_BUILD = 1
            set BUILD_REASON = "Previous build commit is not available in the current repository history."
        else
            # Rebuild only if build-relevant files changed since the last successful build.
            # This includes CMake files, C++ source/header files, and vendored yaml-cpp files.
            git diff --name-only $last_built_commit $new_head | egrep -q '(^|/)(CMakeLists\.txt|.*\.cmake|.*\.(cpp|cc|cxx|c\+\+|h|hh|hpp|hxx|ipp|tpp)$|vendor/yaml-cpp/)'
            if ($status == 0) then
                set SHOULD_BUILD = 1
                set BUILD_REASON = "Build-relevant source or CMake files changed since the last successful build."
            endif
        endif
    endif
endif

# Skip over the interrupt-handler label during normal execution.
# The RUN_INTERRUPTED label should only be reached through the Ctrl-C handler.
goto RUN_FINISHED

# Ctrl-C handler.
# Print the stop banner and exit with code 130, which is the conventional shell
# exit code for termination by SIGINT.
RUN_INTERRUPTED:
source scripts/printers/print_stop.csh
exit 130

# Resume normal execution after skipping past the interrupt-handler block.
RUN_FINISHED:

# Print the automatic build decision and the reason behind it.
# This makes it easier to debug why the script decided to build or skip.
if ($DO_BUILD) then
    echo "${COLOR_START}Build decision:${COLOR_END}"
    if ($SHOULD_BUILD) then
        echo "${COLOR_START}  action:${COLOR_END} build"
    else
        echo "${COLOR_START}  action:${COLOR_END} skip build"
    endif
    echo "${COLOR_START}  reason:${COLOR_END} ${BUILD_REASON}"
    echo ""
endif

# Run CMake configure/build only when the decision logic says a rebuild is needed.
if ($DO_BUILD && $SHOULD_BUILD) then
    echo "${COLOR_START}- Configuring build --------------------------------------------------------------------------------${COLOR_END}"
    cmake -S . -B build
    if ($status != 0) then
        echo "${COLOR_ERR}CMake configure failed.${COLOR_END}"
        exit 1
    endif
    echo ""

    # Compile project targets in parallel.
    # Adjust -j4 if your SSH machine has more/less available CPU.
    echo "${COLOR_START}- Building -----------------------------------------------------------------------------------------${COLOR_END}"
    cmake --build build -j4
    if ($status != 0) then
        echo "${COLOR_ERR}Build failed.${COLOR_END}"
        exit 1
    endif
    echo ""

    # Record the current commit as the one that produced the latest successful build.
    git rev-parse HEAD >! "$build_stamp"
else if ($DO_BUILD) then
    echo "${COLOR_START}- Skipping configure/build -------------------------------------------------------------------------${COLOR_END}"
    echo ""
else
    echo "${COLOR_START}- Skipping configure/build -------------------------------------------------------------------------${COLOR_END}"
    echo ""
endif

# Safety check: abort if the executable is still missing after the optional build step.
if (! -x "$exe_path") then
    echo "${COLOR_ERR}Executable not found: $exe_path${COLOR_END}"
    exit 1
endif

# Absolute path to the runtime configuration directory used on the SSH machine.
# The nested loops below select combinations of YAML files and pass them into
# the executable as command-line options.
set CONFIG_DIR = /w/hallb-scshelf2102/clas12/asportes/2N-Analyzer-e4nu/config

# Run the analyzer only when execution is enabled.
# The loops below make it possible to scan over different sample chains, cut settings,
# and momentum-resolution configurations in one invocation.
if ($DO_RUN) then
	
	# Loops for multiple runs with different cuts:
    echo "${COLOR_START}- Running ${exe_path} ------------------------------------------------------------------------${COLOR_END}"
    
    # Loop over sample-chain YAML files.
	# ---------------------------------------------------------------------------------------------
    foreach Samples ( samples/RecoSampleChain_Default.yaml )
    
        # Loop over event-selection YAML files:
		# -----------------------------------------------------------------------------------------
        foreach EventSelection ( EventSelection/EventSelectionSettings_Default.yaml )
        # foreach EventSelection ( EventSelection/EventSelectionSettings_Default.yaml EventSelection/EventSelectionSettings_wFCwO.yaml EventSelection/EventSelectionSettings_wFCnoO.yaml )
        # foreach EventSelection ( EventSelection/EventSelectionSettings_wFCnoO.yaml )
	
            # Loop over analysis-cut YAML files:
			# -------------------------------------------------------------------------------------
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_NO_CUTS.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_woChi2.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_noNC.yaml )
			foreach AnalysisCuts ( cuts/AnalysisCutSettings_Default.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_woChi2.yaml cuts/AnalysisCutSettings_noNC.yaml cuts/AnalysisCutSettings_Default.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_NO_CUTS.yaml cuts/AnalysisCutSettings_woChi2.yaml cuts/AnalysisCutSettings_noNC.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_NO_CUTS.yaml cuts/AnalysisCutSettings_woChi2.yaml cuts/AnalysisCutSettings_noNC.yaml cuts/AnalysisCutSettings_Default.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_Default.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_wFC.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_nECALv.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_wFC_nECALv.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_Default.yaml cuts/AnalysisCutSettings_wFC.yaml )
			# foreach AnalysisCuts ( cuts/AnalysisCutSettings_Default.yaml cuts/AnalysisCutSettings_nECALv.yaml cuts/AnalysisCutSettings_wFC.yaml )
	
    			# Loop over momentum-resolution YAML files:
				# ---------------------------------------------------------------------------------
				foreach CalcMomRes ( tools/MomentumResolutionSettings_Default.yaml )
				# foreach CalcMomRes ( tools/MomentumResolutionSettings_momResS1.yaml )
				# foreach CalcMomRes ( tools/MomentumResolutionSettings_Default.yaml tools/MomentumResolutionSettings_momResS1.yaml )

					# Launch the C++ executable and forward any extra user arguments from run.csh.
					# The selected YAML files are passed explicitly here as command-line flags.
					$exe_path $forwarded_args \
						# --skip-hipo-files-prob true \
						# --n-events 300000 \
						# --n-events 1000000 \
						# --test-run true \
						--sample-chain ${CONFIG_DIR}/${Samples} \
						--event-selection ${CONFIG_DIR}/${EventSelection} \
						--analysis-cuts ${CONFIG_DIR}/${AnalysisCuts} \
						--momentum-resolution-config ${CONFIG_DIR}/${CalcMomRes}

					# Abort immediately if the executable returns a non-zero exit status.
					if ($status != 0) then
						echo "${COLOR_ERR}Run failed.${COLOR_END}"
						exit 1
					endif
					echo ""
				end # End of loop over CalcMomRes
			end # End of loop over AnalysisCuts
        end # End of loop over EventSelection
    end # End of loop over Samples

else
    # If running was disabled by the outer control flags, report that explicitly.
    echo "${COLOR_START}- Skipping executable run --------------------------------------------------------------------------${COLOR_END}"
    echo ""
endif
