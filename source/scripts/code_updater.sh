#!/bin/bash

echo ""
git pull
echo ""
echo "\033[35m- Re-pulling repository -----------------------------------------------\033[0m"
echo ""
git reset --hard
git pull
echo ""
# echo "\033[35m- Cleaning excessive file ---------------------------------------------\033[0m"
# echo ""
# git clean -f
# echo ""

source ./source/scripts/start_screen_session.sh