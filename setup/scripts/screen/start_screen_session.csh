#!/bin/tcsh

# Usage:
# start_screen_session mysession "echo 'hello' && ls -l" "stay_open"

# Check if the correct number of arguments are passed
if ($#argv < 2) then
    echo 'Usage: $SCREEN_SCRIPT_DIR/start_screen_session.csh <session_name> <command> [stay_open]'
    exit 1
endif

# Assign session name, command, and optional stay_open argument
set session_name = "$1"
set command = "$2"
set stay_open = "$3"

echo "Session name: $session_name"
echo "Command: $command"
echo "Stay open: $stay_open"

# Start a detached screen session with the specified name and command
if ("$stay_open" == "") then
    # If stay_open is empty, leave tcsh running
    echo "Running with tcsh, keeping screen open"
    screen -dmS "$session_name" tcsh -c "source ~/.cshrc; $command; exec tcsh"
    echo "Screen session '$session_name' started with command: $command. Keeping tcsh running."
else
    # If stay_open is not empty, close the screen session after running the command
    echo "Running with tcsh, exiting after command completion"
    screen -dmS "$session_name" tcsh -c "source ~/.cshrc; $command; exit"
    echo "Screen session '$session_name' started with command: $command. Exiting after completion."
endif