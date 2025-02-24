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
echo "\033[35m- Lunching nFD efficiency test code -----------------------------------\033[0m"
echo ""
clas12root -b -q testers/nFD_eff_test/nFD_eff_test.cpp
echo ""
echo "\033[35m- Operation finished --------------------------------------------------\033[0m"
echo ""
