#!/bin/bash

source update_only.sh

echo "\033[35m- Lunching nFD efficiency test code -----------------------------------\033[0m"
echo ""
clas12root -b -q testers/nFD_eff_test/nFD_eff_test.cpp
echo ""
echo "\033[35m- Operation finished --------------------------------------------------\033[0m"
echo ""