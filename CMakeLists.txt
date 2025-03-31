cmake_minimum_required(VERSION 3.29)
project(2N_Analysis_e4nu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CLAS12ROOT $ENV{CLAS12ROOT})
message(STATUS "CLAS12ROOT: " ${CLAS12ROOT})
set(HIPO $ENV{HIPO})
message(STATUS "HIPO: " ${HIPO})

find_package(ROOT REQUIRED)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})  # Ensure ROOTSYS is set before finding ROOT
include(${ROOT_USE_FILE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS} -O0 -ggdb")

# Include directories
include_directories(
    ${ROOT_INCLUDE_DIRS}
    $ENV{HIPO}/include
    $ENV{CLAS12ROOT}/Clas12Root
    $ENV{CLAS12ROOT}/Clas12Banks
)

# Link directories
link_directories(
    $ENV{HIPO}/lib
    $ENV{CLAS12ROOT}/lib
)

set(Analyser 2N_Analyser_e4nu)

# message(STATUS "Including analysis directories...")
set(FRAMEWORK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/framework)
include_directories(${FRAMEWORK_DIR})
# message(STATUS "Included directory: ${FRAMEWORK_DIR}")

set(STRUCTURES_DIR ${FRAMEWORK_DIR}/structures)
include_directories(${STRUCTURES_DIR}/CodeDirectories)
# message(STATUS "Included directory: ${STRUCTURES_DIR}/CodeDirectories")
include_directories(${STRUCTURES_DIR}/DataSample)
# message(STATUS "Included directory: ${STRUCTURES_DIR}/DataSample")

set(NAMESPACES_DIR ${FRAMEWORK_DIR}/namespaces)

set(NAMESPACES_SUBDIRS 
    fitters/fitter_functions
    general_utilities/analysis_math
    general_utilities/basic_tools
    general_utilities/constants
    general_utilities/data_processor
    general_utilities/histogram_functions
    general_utilities/lists
    general_utilities/reco_analysis_functions
    general_utilities/utilities
    plotters/draw_and_save_functions
    plotters/PDF_canvas
    settings/debugging_settings
    settings/PDF_canvas_settings
    setup/analysis_samples
    setup/code_setup
    setup/path_definitions
)

foreach(subdir ${NAMESPACES_SUBDIRS})
    include_directories(${NAMESPACES_DIR}/${subdir})
    # message(STATUS "Included directory: ${NAMESPACES_DIR}/${subdir}")
endforeach()

set(CLASSES_DIR ${FRAMEWORK_DIR}/classes)
set(CLASSES_SUBDIRS 
    AMaps 
    clas12ana 
    DataAnalyser 
    Debugger 
    DEfficiency 
    Directories 
    DSCuts 
    ExperimentParameters 
    gst 
    hPlots 
    MomentumResolution 
    ParticleID 
    Settings 
    TargetParameters 
    TLCuts
)

foreach(subdir ${CLASSES_SUBDIRS})
    include_directories(${CLASSES_DIR}/${subdir})
    # message(STATUS "Included directory: ${CLASSES_DIR}/${subdir}")
endforeach()

# set(FUNCTIONS_DIR ${FRAMEWORK_DIR}/functions)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/setup)
# message(STATUS "Included directory: ${CMAKE_CURRENT_SOURCE_DIR}/setup")

set(APPS_DIR ${FRAMEWORK_DIR}/applications)
include_directories(${APPS_DIR})
# message(STATUS "Included directory: ${APPS_DIR}")

# Add subdirectories
# Ensure subdirectories exist and contain a CMakeLists.txt
# message(STATUS "Adding analysis subdirectories...")
foreach(dir ${STRUCTURES_DIR} ${NAMESPACES_DIR} ${CLASSES_DIR} ${APPS_DIR})
    if(EXISTS ${dir})
        add_subdirectory(${dir})
        message(STATUS "Added subdirectory: ${dir}")
    else()
        message(WARNING "Directory ${dir} does not exist.")
    endif()
endforeach()

add_executable(${Analyser} main.cpp)

# link libraries (external libraries first, then project libraries)
target_link_libraries(${Analyser}
    ${ROOT_LIBRARIES} -lEG 
    -lHipo4
    -lClas12Banks -lClas12Root
    -L${CLAS12ROOT}/lib
)

target_link_libraries(${Analyser}
    AnalysisDataStructures    
    AnalysisLibraries
    AnalysisTools
    AnalysisApps
)