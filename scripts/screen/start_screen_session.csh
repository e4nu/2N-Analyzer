#!/bin/tcsh

# ------------------------------------------------------------------------------------------
# start_screen_session.csh
# ------------------------------------------------------------------------------------------
# Purpose
# -------
# Utility script to launch commands inside detached GNU Screen sessions.
# This allows long-running analysis jobs to continue executing even after
# the SSH session is closed.
#
# Usage
# -----
# start_screen_session.csh <session_name> <command> [stay_open]
#
# Arguments
# ---------
# session_name : name of the screen session
# command      : command to execute inside the screen
# stay_open    : optional flag; if present, the session exits after command
# ------------------------------------------------------------------------------------------

# Print section header

echo ""
echo "${COLOR_START}- Setting screen commands --------------------------------------------------------------------------${COLOR_END}"
echo ""

# Validate argument count
# At least session name and command must be provided

if ($#argv < 2) then
    echo 'Usage: $SCREEN_SCRIPT_DIR/start_screen_session.csh <session_name> <command> [stay_open]'
    exit 1
endif

# Parse arguments
set session_name = "$1"
set command = "$2"
set stay_open = "$3"

# ------------------------------------------------------------------------------------------
# Generate default session name if none provided
# ------------------------------------------------------------------------------------------

if ("${session_name}" == "") then

    # Use timestamp to create unique session name
    set session_name = "session_`date +%Y%m%d%H%M%S`"

    echo "${COLOR_START}No session name provided. Using default session name:${COLOR_END} ${session_name}"

endif

# ------------------------------------------------------------------------------------------
# Check for duplicate screen sessions
# ------------------------------------------------------------------------------------------

set existing_sessions = `screen -list | grep -w "${session_name}"`

if ("${existing_sessions}" != "") then

    echo "${COLOR_START}Error: A screen session with the name ${COLOR_END}'${session_name}'${COLOR_START} already exists.${COLOR_END}"
    exit 1

endif

# ------------------------------------------------------------------------------------------
# Start detached screen session
# ------------------------------------------------------------------------------------------

if ("$stay_open" == "") then

    # Run command and keep tcsh alive afterwards
    screen -dmS "${session_name}" tcsh -c "source ~/.cshrc;${command}; exec tcsh"

    echo "${COLOR_START}Screen session ${COLOR_END}'${session_name}'${COLOR_START} started with command:${COLOR_END} ${command}${COLOR_START}. Keeping tcsh running.${COLOR_END}"

else

    # Run command then exit session automatically
    screen -dmS "${session_name}" tcsh -c "source ~/.cshrc;${command}; exit"

    echo "${COLOR_START}Screen session ${COLOR_END}'${session_name}'${COLOR_START} started with command:${COLOR_END} ${command}${COLOR_START}. Aborting after completion.${COLOR_END}"

endif

# Print spacing for readability

echo ""