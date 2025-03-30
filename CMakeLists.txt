cmake_minimum_required(VERSION 3.29)
project(2N_Analyser_e4nu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CLAS12ROOT $ENV{CLAS12ROOT})
# set(HIPO $ENV{HIPO})

MESSAGE( CHECK_START "Checking for installed hipo..."  )

if(DEFINED ENV{HIPO})
    set(HIPOEXT $ENV{HIPO})
    set(HIPO_INCLUDE_DIR ${HIPOEXT}/include/)
    set(HIPO_LIBS "-L${HIPOEXT}/lib -lhipo4")

    MESSAGE(STATUS "HIPO INCLUDE_DIR : " ${HIPO_INCLUDE_DIR} )
    MESSAGE(STATUS "HIPO LIBRARIES : " ${HIPO_LIBS} )
else()
    MESSAGE(CHECK_FAIL "$HIPO not set" )
    MESSAGE(FATAL_ERROR , "\n ... $HIPO must be set to local install of the hipo library\n ... see clas12root/README.md\n ... cmake exiting")
endif()


find_package(ROOT REQUIRED)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})  # Ensure ROOTSYS is set before finding ROOT
include(${ROOT_USE_FILE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

find_package(PkgConfig REQUIRED)
# pkg_check_modules(hipo4 REQUIRED IMPORTED_TARGET hipo4) # makes the target 'PkgConfig::hipo4`

include_directories(${CLAS12ROOT}/Clas12Banks)
include_directories(${CLAS12ROOT}/Clas12Root)
include_directories(${HIPOEXT}/include)
# include_directories(${HIPO}/include)

set(Analyser myclas12root)

set(FRAMEWORK_DIR framework)
include_directories(${FRAMEWORK_DIR})

set(CLASSES_DIR ${FRAMEWORK_DIR}/classes)
include_directories(${CLASSES_DIR})
include_directories(${CLASSES_DIR}/AMaps)
include_directories(${CLASSES_DIR}/clas12ana)
include_directories(${CLASSES_DIR}/DataAnalyser)
include_directories(${CLASSES_DIR}/Debugger)
include_directories(${CLASSES_DIR}/DEfficiency)
include_directories(${CLASSES_DIR}/Directories)
include_directories(${CLASSES_DIR}/DSCuts)
include_directories(${CLASSES_DIR}/ExperimentParameters)
include_directories(${CLASSES_DIR}/gst)
include_directories(${CLASSES_DIR}/hPlots)
include_directories(${CLASSES_DIR}/MomentumResolution)
include_directories(${CLASSES_DIR}/ParticleID)
include_directories(${CLASSES_DIR}/Settings)
include_directories(${CLASSES_DIR}/TargetParameters)
include_directories(${CLASSES_DIR}/TLCuts)

set(NAMESPACES_DIR ${FRAMEWORK_DIR}/namespaces)
include_directories(${NAMESPACES_DIR}/fitters/fitter_functions)
include_directories(${NAMESPACES_DIR}/general_utilities/analysis_math)
include_directories(${NAMESPACES_DIR}/general_utilities/basic_tools)
include_directories(${NAMESPACES_DIR}/general_utilities/constants)
include_directories(${NAMESPACES_DIR}/general_utilities/data_processor)
include_directories(${NAMESPACES_DIR}/general_utilities/histogram_functions)
include_directories(${NAMESPACES_DIR}/general_utilities/lists)
include_directories(${NAMESPACES_DIR}/general_utilities/reco_analysis_functions)
include_directories(${NAMESPACES_DIR}/general_utilities/utilities)
include_directories(${NAMESPACES_DIR}/plotters/draw_and_save_functions)
include_directories(${NAMESPACES_DIR}/plotters/PDF_canvas)
include_directories(${NAMESPACES_DIR}/settings/debugging_settings)
include_directories(${NAMESPACES_DIR}/settings/PDF_canvas_settings)
include_directories(${NAMESPACES_DIR}/setup/analysis_samples)
include_directories(${NAMESPACES_DIR}/setup/code_setup)
include_directories(${NAMESPACES_DIR}/setup/path_definitions)

set(STRUCTURES_DIR ${FRAMEWORK_DIR}/structures)
include_directories(${STRUCTURES_DIR}/CodeDirectories)
include_directories(${STRUCTURES_DIR}/DataSample)

set(FUNCTIONS_DIR ${FRAMEWORK_DIR}/functions)

set(APPS_DIR framework/applications)
include_directories(${APPS_DIR})

include_directories(setup)

# Ensure subdirectories exist and contain a CMakeLists.txt
add_subdirectory(${STRUCTURES_DIR})
add_subdirectory(${NAMESPACES_DIR})
add_subdirectory(${CLASSES_DIR})
add_subdirectory(${APPS_DIR})

add_executable(${Analyser} main.cpp)

# Link external libraries first, then project libraries
# target_link_libraries(${Analyser} ${ROOT_LIBRARIES} Hipo4 Clas12Banks Clas12Root)

target_link_libraries(${Analyser} PUBLIC ${ROOT_LIBRARIES} ${HIPO_LIBS} -L${CLAS12ROOT}/lib -lClas12Banks -lClas12Root -L${CLAS12ROOT}/ccdb/lib)
# target_link_libraries(${Analyser} ${ROOT_LIBRARIES} PkgConfig::hipo4 -L${CLAS12ROOT}/lib -lClas12Banks -lClas12Root -L${CLAS12ROOT}/ccdb/lib AnalysisDataStructures AnalysisLibraries AnalysisTools AnalysisApps)

target_link_libraries(${Analyser} AnalysisDataStructures)
target_link_libraries(${Analyser} AnalysisLibraries)
target_link_libraries(${Analyser} AnalysisTools)
target_link_libraries(${Analyser} AnalysisApps)
