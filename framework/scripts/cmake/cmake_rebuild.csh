#!/bin/tcsh

# Set the project directory (replace with your actual project directory)
set PROJECT_DIR = "${DIR_2N_ANALYSIS_CODE}"

# Print the start message
echo "\033[35m- Cleaning up old build files --------------------------------------\033[0m"
echo ""

# Go to the project directory
cd ${PROJECT_DIR}

# Remove the old build directory if it exists
if (-d "build") then
    echo "Removing old build directory..."
    rm -rf build
endif

# Create a new build directory
echo "Creating a fresh build directory..."
mkdir build
cd build

# Run cmake to configure the project
echo "\033[35m- Configuring the project using CMake ------------------------------\033[0m"
cmake ..

# Build the project
echo "\033[35m- Building the project --------------------------------------------\033[0m"
cmake --build .

# Final message
echo "\033[35m- Rebuild complete ------------------------------------------------\033[0m"







# # To run script:
# # source ./framework/scripts/cmake/cmake_rebuild.csh

# unset COLOR_START
# setenv COLOR_START '\033[35m'

# unset COLOR_END
# setenv COLOR_END '\033[0m'

# echo ""
# source ./framework/scripts/environment/set_environment.csh

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
