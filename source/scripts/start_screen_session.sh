#!/bin/bash

# usage:
# start_screen_session mysession "source something.sh"

start_screen_session() {
    local session_name="$1"
    local command="$2"

    if [ -z "$session_name" ] || [ -z "$command" ]; then
        echo "Usage: start_screen_session <session_name> <command>"
        return 1
    fi

    # Start a detached screen session with the specified name and command
    screen -dmS "$session_name" bash -c "$command; exec bash"

    echo "Screen session '$session_name' started with command: $command"
}