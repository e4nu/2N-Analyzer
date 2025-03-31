#!/bin/tcsh

source update_only.sh

echo "\033[35m- Launching data-analysis code -------------------------------------\033[0m"
echo ""
clas12root -b -q ./main.cpp
echo ""
echo "\033[35m- Operation finished --------------------------------------------------\033[0m"
echo ""
