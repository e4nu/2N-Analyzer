#!/bin/tcsh

# Usage:
# start_screen_session mysession "echo 'hello'"

echo ""
echo "Setting start_screen_session command..."
echo ""

alias start_screen_session 'screen -dmS \!:1 bash -c "\!:2; exec bash"'

echo "Function start_screen_session is now available."