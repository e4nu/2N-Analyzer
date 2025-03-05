#!/bin/bash

source ./source/scripts/code_updater.sh

echo "\033[35m- Lunching nFD efficiency test code -----------------------------------\033[0m"
echo ""
clas12root -b -q testers/nFD_eff_test/nFD_eff_test.cpp
echo ""
echo "\033[35m- Operation finished --------------------------------------------------\033[0m"
echo ""

source start_screen_session.sh
