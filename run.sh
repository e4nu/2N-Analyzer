#!/bin/bash

source update_only.sh

echo "\033[35m- Lunching data-analysis code -----------------------------------------\033[0m"
echo ""
clas12root -b -q ./main.cpp
# clas12root -b -q ./main.c
echo ""
echo "\033[35m- Operation finished --------------------------------------------------\033[0m"
echo ""
