cmake_minimum_required(VERSION 3.29)
project(2N_Analysis_e4nu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CLAS12ROOT $ENV{CLAS12ROOT})
MESSAGE(STATUS "CLAS12ROOT: " ${CLAS12ROOT})
set(HIPO $ENV{HIPO})
MESSAGE(STATUS "HIPO: " ${HIPO})

find_package(ROOT REQUIRED)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})  # Ensure ROOTSYS is set before finding ROOT
include(${ROOT_USE_FILE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS} -O0 -ggdb")

find_package(PkgConfig REQUIRED)
pkg_check_modules(hipo4 REQUIRED IMPORTED_TARGET hipo4) # makes the target 'PkgConfig::hipo4`

set(INCLUDES ${ROOT_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/libraries/simulation_reweighting ${HIPO}/include)
set(LIBRARIES ${CMAKE_SOURCE_DIR}/libraries ${HIPO}/lib)
include_directories(${CLAS12ROOT}/Clas12Banks ${CLAS12ROOT}/Clas12Root ${INCLUDES} ${LIBRARIES})

set(Analyser 2N_Analyser_e4nu)

set(FRAMEWORK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/framework)
include_directories(${FRAMEWORK_DIR})

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
endforeach()

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
endforeach()

set(STRUCTURES_DIR ${FRAMEWORK_DIR}/structures)
include_directories(${STRUCTURES_DIR}/CodeDirectories)
include_directories(${STRUCTURES_DIR}/DataSample)

set(FUNCTIONS_DIR ${FRAMEWORK_DIR}/functions)

set(APPS_DIR ${FRAMEWORK_DIR}/applications)
include_directories(${APPS_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/setup)

# Add subdirectories
# Ensure subdirectories exist and contain a CMakeLists.txt
foreach(dir ${STRUCTURES_DIR} ${NAMESPACES_DIR} ${CLASSES_DIR} ${APPS_DIR})
    if(EXISTS ${dir})
        add_subdirectory(${dir})
        message(STATUS "Added subdirectory: ${dir}")
    else()
        message(WARNING "Directory ${dir} does not exist.")
    endif()
endforeach()

add_executable(${Analyser} main.cpp)

# Link external libraries first, then project libraries
# target_link_libraries(${Analyser} ${ROOT_LIBRARIES} Hipo4 Clas12Banks Clas12Root)

target_link_libraries(${Analyser} PUBLIC ${ROOT_LIBRARIES} Clas12Banks hipo4 -L${CLAS12ROOT}/lib -L${CLAS12ROOT}/ccdb/lib AnalysisDataStructures AnalysisLibraries AnalysisTools AnalysisApps)
# target_link_libraries(${Analyser} PUBLIC ${ROOT_LIBRARIES} -L${HIPO}/lib Clas12Banks hipo4 -L${CLAS12ROOT}/lib -L${CLAS12ROOT}/ccdb/lib AnalysisDataStructures AnalysisLibraries AnalysisTools AnalysisApps)
# target_link_libraries(${Analyser} PUBLIC ${ROOT_LIBRARIES} Clas12Banks PkgConfig::hipo4 -L${CLAS12ROOT}/lib -L${CLAS12ROOT}/ccdb/lib AnalysisDataStructures AnalysisLibraries AnalysisTools AnalysisApps)
# target_link_libraries(${Analyser} PUBLIC ${ROOT_LIBRARIES} Clas12Banks Hipo4 -L${CLAS12ROOT}/lib -L${CLAS12ROOT}/ccdb/lib AnalysisDataStructures AnalysisLibraries AnalysisTools AnalysisApps) # best one!