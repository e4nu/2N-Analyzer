#!/bin/tcsh

# Usage:
# start_screen_session mysession "echo 'hello'"

alias start_screen_session "screen -dmS \!:1 bash -c '\!:2; exec bash'"
alias kill_all1 "screen -ls | grep Detached | cut -d. -f1 | awk '{print $1}' | xargs kill"
alias kill_all2 "screen -ls | grep -E '^\s*[0-9]+' | awk '{print $1}' | cut -d. -f1 | xargs -r -I {} screen -S {} -X quit"

echo ""
echo "Screen commands have been updated."
echo ""