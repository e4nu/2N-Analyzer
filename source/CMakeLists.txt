#
# Created by Alon Sportes on 09/03/2026.
#

# source/CMakeLists.txt
# -----------------------------------------------------------------------------
# This file builds the main internal library of the project.
# The library gathers the reusable analysis implementation files so they are
# compiled once and then linked into one or more executables.
# -----------------------------------------------------------------------------

# Build the core analysis library from all implementation (.cpp) files that are
# part of the reusable framework.
add_library(TwoNAnalysisCore
    # Analysis tools.
    AnalysisTools/AMaps.cpp
    AnalysisTools/DEfficiency.cpp
    AnalysisTools/DataAnalyzer.cpp
    AnalysisTools/Debugger.cpp
    AnalysisTools/Directories.cpp
    AnalysisTools/ExperimentParameters.cpp
    AnalysisTools/HipoChainLoader.cpp
    AnalysisTools/MomentumResolution.cpp
    AnalysisTools/ParticleID.cpp
    AnalysisTools/TLCuts.cpp
    AnalysisTools/clas12ana.cpp
    AnalysisTools/clas12debug.cpp
    AnalysisTools/hPlot1D.cpp
    AnalysisTools/hPlot2D.cpp
    AnalysisTools/hsPlots.cpp

    # Settings and runtime configuration helpers.
    Settings/CustomPlotsDirectory.cpp
    Settings/Settings.cpp

    # Reconstructed-analysis implementation.
    TwoNAnalyzer/RecoAnalyzer/RecoAnalyzer.cpp
    TwoNAnalyzer/RecoAnalyzer/RecoAnalyzerPrinter.cpp
    TwoNAnalyzer/RecoAnalyzer/RecoApp.cpp

    # Legacy plotting helpers.
    functions/HistogramPlottingFunctions.cpp

    # Input/output helpers.
    io/Yaml_loader.cpp
    io/reco_cli.cpp

    # General math utility implementation.
    libraries/fitters/fitter_functions.cpp
    libraries/general_utilities/basic_tools.cpp
    libraries/general_utilities/data_processor.cpp
    libraries/general_utilities/histogram_functions.cpp
    libraries/general_utilities/reco_analysis_functions.cpp
    libraries/general_utilities/variable_correctors/variable_correctors.cpp
    libraries/general_utilities/analysis_math/analysis_math.cpp
    libraries/general_utilities/analysis_math/poly_solver.cpp
    libraries/plotters/draw_and_save_functions/draw_and_save_acorr_plots.cpp
    libraries/plotters/draw_and_save_functions/draw_and_save_efficiency_plots.cpp
    libraries/plotters/draw_and_save_functions/draw_and_save_fs_ratio.cpp
)

# Enable CMake unity builds for this target.
# UNITY_BUILD ON asks CMake to batch multiple source files together during
# compilation, which can reduce compile time in large projects.
# UNITY_BUILD_BATCH_SIZE controls how many source files may be grouped into a
# single unity translation unit.
set_target_properties(TwoNAnalysisCore PROPERTIES
    UNITY_BUILD ON
    UNITY_BUILD_BATCH_SIZE 1000
)

# Declare the include directories needed when compiling this library.
# PUBLIC means these include paths are also propagated to targets that link
# against this library, such as the executable in apps/CMakeLists.txt.
target_include_directories(TwoNAnalysisCore
    PUBLIC
    # Main public include root.
    ${CMAKE_SOURCE_DIR}/include

    # yaml-cpp headers provided through the vendored source tree.
    ${CMAKE_SOURCE_DIR}/vendor/yaml-cpp/yaml-cpp-src/include

    # Additional include roots for the project submodules.
    ${CMAKE_SOURCE_DIR}/include/AnalysisTools
    ${CMAKE_SOURCE_DIR}/include/Settings
    ${CMAKE_SOURCE_DIR}/include/io
    ${CMAKE_SOURCE_DIR}/include/libraries
    ${CMAKE_SOURCE_DIR}/include/TwoNAnalyzer/RecoAnalyzer
    ${CMAKE_SOURCE_DIR}/include/TwoNAnalyzer/TruthAnalyzer
    ${CMAKE_SOURCE_DIR}/include/libraries/general_utilities/analysis_math
)

# Try to locate the HIPO4 library in a robust way.
# Some systems name the library Hipo4 while others use hipo4, and some install
# it under lib while others use lib64.
find_library(HIPO4_LIBRARY
    NAMES Hipo4 hipo4
    PATHS
        ${HIPO}/lib
        ${HIPO}/lib64
)

# Stop configuration immediately if the HIPO4 library cannot be found.
if(NOT HIPO4_LIBRARY)
    message(FATAL_ERROR "Could not find Hipo4/hipo4 library under HIPO=${HIPO} (checked lib and lib64).")
endif()

# Link the core library against all required external libraries.
# ROOT_LIBRARIES comes from find_package(ROOT) in the top-level CMake file.
# HistPainter and EG are specific ROOT components required by this project.
# Clas12Banks and Clas12Root are CLAS12-specific external libraries.
target_link_libraries(TwoNAnalysisCore
    PUBLIC
    ${ROOT_LIBRARIES}
    yaml-cpp
    -lHistPainter
    -lEG
    ${HIPO4_LIBRARY}
    -lClas12Banks
    -lClas12Root
)

# Link MPI only when the project was configured with USE_MPI=ON.
if(USE_MPI)
    target_link_libraries(TwoNAnalysisCore PUBLIC MPI::MPI_CXX)
endif()
