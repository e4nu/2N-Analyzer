#!/bin/tcsh

# To run script:
# source ./scripts/cmake/cmake_rebuild.csh

unset COLOR_START
setenv COLOR_START '\033[35m'

unset COLOR_END
setenv COLOR_END '\033[0m'

echo ""
source ../../update_only.sh
source ../../scripts/environment/set_environment.csh

cd ${DIR_2N_ANALYSIS_CODE}

echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Rebuilding analysis code                                            =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo ""

echo "\033[35m- Running initial Setup -----------------------------------------------\033[0m"
echo ""
rm -rf build
# echo "rm -rf build"
mkdir build && cd build
# echo "mkdir build && cd build"
cmake ..
# echo "cmake .."
cmake --build .
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
