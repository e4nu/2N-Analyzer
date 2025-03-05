#!/bin/tcsh

# Usage:
# start_screen_session mysession "echo 'hello' && ls -l"

# Check if the correct number of arguments are passed
if ($#argv < 2) then
    echo "Usage: start_screen_session <session_name> <command>"
    exit 1
endif

# Assign session name and command from arguments
set session_name = "$1"
set command = "$2"

# Start a detached screen session with the specified name and command
screen -dmS "$session_name" bash -c "$command; exec bash"

echo "Screen session '$session_name' started with command: $command"