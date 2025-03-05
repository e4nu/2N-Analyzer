#!/bin/tcsh

# Usage:
# start_screen_session mysession "echo 'hello' && ls -l" "stay_open"

echo ""
echo "${COLOR_START}- Setting screen commands ---------------------------------------------${COLOR_END}"
echo ""

# Check if the correct number of arguments are passed
if ($#argv < 2) then
    echo 'Usage: $SCREEN_SCRIPT_DIR/start_screen_session.csh <session_name> <command> [stay_open]'
    # echo "Usage: start_screen_session <session_name> <command> [stay_open]"
    exit 1
endif

# Assign session name, command, and optional stay_open argument
set session_name = "$1"
set command = "$2"
set stay_open = "$3"

# If session_name is empty, generate a default session name (e.g., using timestamp)
if ("${session_name}" == "") then
    set session_name = "session_`date +%Y%m%d%H%M%S`"
    echo "${COLOR_START}No session name provided. Using default session name:${COLOR_END} ${session_name}"
endif

# Check if a screen session with the same name already exists
set existing_sessions = `screen -list | grep -w "${session_name}"`
if ("${existing_sessions}" != "") then
    echo "${COLOR_START}Error: A screen session with the name ${COLOR_END}'${session_name}'${COLOR_START} already exists.${COLOR_END}"
    exit 1
endif

# Start a detached screen session with the specified name and command
if ("$stay_open" == "") then
    # If stay_open is empty, leave tcsh running
    screen -dmS "${session_name}" tcsh -c "source ~/.cshrc;${command}; exec tcsh"
    # screen -dmS "${session_name}" tcsh -c "${command}; exec tcsh"
    # screen -dmS "$session_name" bash -c "$command; exec bash"
    echo "${COLOR_START}Screen session ${COLOR_END}'${session_name}'${COLOR_START} started with command:${COLOR_END} ${command}${COLOR_START}. Keeping tcsh running.${COLOR_END}"
else
    # If stay_open is not empty, close the screen session after running the command
    screen -dmS "${session_name}" tcsh -c "source ~/.cshrc;${command}; exit"
    # screen -dmS "${session_name}" tcsh -c "${command}; exit"
    echo "${COLOR_START}Screen session ${COLOR_END}'${session_name}'${COLOR_START} started with command:${COLOR_END} ${command}${COLOR_START}. Exiting after completion.${COLOR_END}"
endif

echo ""