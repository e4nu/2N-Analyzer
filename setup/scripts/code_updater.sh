#!/bin/bash

source ./setup/scripts/environment/set_environment.csh

echo ""
git pull
echo ""
echo "$COLOR_START- Re-pulling repository -----------------------------------------------COLOR_END"
echo ""
git reset --hard
git pull
echo ""
# echo "$COLOR_START- Cleaning excessive file ---------------------------------------------COLOR_END"
# echo ""
# git clean -f
# echo ""

source ./setup/scripts/screen/setup_screen_commands.csh