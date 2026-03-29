#!/bin/tcsh

# Usage:
# start_screen_session mysession "echo 'hello'"

echo ""
echo "Setting start_screen_session command..."
echo ""

start_screen_session() {
    set session_name = "$1"
    set command = "$2"

    if ("$session_name" == "" || "$command" == "") then
        echo "Usage: start_screen_session <session_name> <command>"
        return 1
    endif

    # Start a detached screen session with the specified name and command
    screen -dmS "$session_name" bash -c "$command; exec bash"

    echo "Screen session '$session_name' started with command: $command"
}

echo "Function start_screen_session is now available."