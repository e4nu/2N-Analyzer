#!/bin/bash

source update_only.sh

echo "\033[35m- Lunching reaction test code -----------------------------------------\033[0m"
echo ""
clas12root -b -q testers/reaction_cuts_test/reaction_cuts_test.cpp
echo ""
echo "\033[35m- Operation finished --------------------------------------------------\033[0m"
echo ""