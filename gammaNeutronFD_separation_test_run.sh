#!/bin/bash

source update_only.sh

echo "\033[35m- Lunching gamma-nFD separation test code -----------------------------------\033[0m"
echo ""
clas12root -b -q testers/GammaNeutronFD_separation_test/GammaNeutronFD_separation_test.cpp
echo ""
echo "\033[35m- Operation finished --------------------------------------------------------\033[0m"
echo ""