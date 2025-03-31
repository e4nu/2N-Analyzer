#!/bin/tcsh

# To run script:
# source ./scripts/cmake/cmake_rebuild.csh

unset COLOR_START
setenv COLOR_START '\033[35m'

unset COLOR_END
setenv COLOR_END '\033[0m'

unset DIR_2N_ANALYSIS_CODE
setenv DIR_2N_ANALYSIS_CODE /w/hallb-scshelf2102/clas12/asportes/2N-Analyser/macros/CMakeTest

# echo ""
# echo "${COLOR_START}=======================================================================${COLOR_END}"
# echo "${COLOR_START}= Running update script                                               =${COLOR_END}"
# echo "${COLOR_START}=======================================================================${COLOR_END}"
# echo ""

# echo "${COLOR_START}- Pulling repository --------------------------------------------------${COLOR_END}"
# echo ""
# git reset --hard
# git pull
# echo ""
# echo "${COLOR_START}HEAD:${COLOR_END}"
# git log -1 --oneline
# echo ""
# echo "${COLOR_START}- Cleaning excessive file ---------------------------------------------${COLOR_END}"
# echo ""
# git clean -f
# echo ""
# echo "${COLOR_START}=======================================================================${COLOR_END}"
# echo "${COLOR_START}= Update review                                                       =${COLOR_END}"
# echo "${COLOR_START}=======================================================================${COLOR_END}"
# echo ""
# echo "${COLOR_START}HEAD:${COLOR_END}"
# git log -1 --oneline
# echo ""

cd ${DIR_2N_ANALYSIS_CODE}

echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Rebuilding analysis code                                            =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo ""

echo "\033[35m- Running initial Setup -----------------------------------------------\033[0m"
echo ""
# rm -rf build
# echo "rm -rf build"
cd build
# echo "mkdir build && cd build"
# cmake ..
# echo "cmake .."
cmake --build .
cd ..
# echo "cmake --build ."
echo ""
echo "\033[35m- Building code -------------------------------------------------------\033[0m"
echo ""
# cmake --build build
# echo ""
# echo "\033[35m- Launching data-analysis code ----------------------------------------\033[0m"
# echo ""

cd ${DIR_2N_ANALYSIS_CODE}
# # To run script:
# # source ./scripts/cmake/cmake_rebuild.csh

# unset COLOR_START
# setenv COLOR_START '\033[35m'

# unset COLOR_END
# setenv COLOR_END '\033[0m'

# echo ""
# source ./scripts/environment/set_environment.csh

# cd ${DIR_2N_ANALYSIS_CODE}

# echo "${COLOR_START}=======================================================================${COLOR_END}"
# echo "${COLOR_START}= Rebuilding analysis code                                            =${COLOR_END}"
# echo "${COLOR_START}=======================================================================${COLOR_END}"
# echo ""

# echo "\033[35m- Running initial Setup -----------------------------------------------\033[0m"
# echo ""
# rm -rf build
# echo "rm -rf build"
# mkdir build && cd build
# echo "mkdir build && cd build"
# cmake ..
# echo "cmake .."
# cmake --build .
# echo "cmake --build ."
# echo ""
# echo "\033[35m- Building code -------------------------------------------------------\033[0m"
# echo ""
# # cmake --build build
# # echo ""
# # echo "\033[35m- Launching data-analysis code ----------------------------------------\033[0m"
# # echo ""

# cd ${DIR_2N_ANALYSIS_CODE}
