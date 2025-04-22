#!/bin/tcsh

# Usage:
# start_screen_session mysession "echo 'hello' && ls -l" "stay_open"

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
    echo "${COLOR_START}Screen session ${COLOR_END}'${session_name}'${COLOR_START} started with command:${COLOR_END} ${command}${COLOR_START}. Aborting after completion.${COLOR_END}"
endif