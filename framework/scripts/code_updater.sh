#!/bin/bash

unset COLOR_START
setenv COLOR_START '\033[35m'

unset COLOR_END
setenv COLOR_END '\033[0m'

echo ""
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Running update script                                               =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo ""

echo "${COLOR_START}- Pulling repository --------------------------------------------------${COLOR_END}"
echo ""
git reset --hard
git pull
echo ""
echo "${COLOR_START}HEAD:${COLOR_END}"
git log -1 --oneline
echo ""
echo "${COLOR_START}- Cleaning excessive file ---------------------------------------------${COLOR_END}"
echo ""
git clean -f
echo ""

source ./framework/scripts/environment/set_environment.csh
source ./framework/scripts/screen/setup_screen_commands.csh

echo ""
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Update review                                                       =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo ""
echo "${COLOR_START}HEAD:${COLOR_END}"
git log -1 --oneline
echo ""