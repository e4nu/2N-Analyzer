#
# Created by Alon Sportes on 09/03/2026.
#

# Top-level CMakeLists.txt
# -----------------------------------------------------------------------------
# This is the entry point for the full CMake build.
# It sets the global build rules, discovers external dependencies, defines
# project-wide compiler settings, and then adds the build subdirectories.
# -----------------------------------------------------------------------------

# Require at least CMake 3.29 before configuration starts.
cmake_minimum_required(VERSION 3.29)

# Define the project name and declare that it uses C++.
project(TwoNAnalyzerE4nu LANGUAGES CXX)

# Require the C++17 standard for all targets in this project.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define an option that allows the user to enable or disable MPI support.
# This can be toggled at configure time with -DUSE_MPI=ON or OFF.
option(USE_MPI "Enable MPI support" OFF)

# Read important external package locations from the shell environment.
# These variables must usually be exported before running CMake.
set(CLAS12ROOT $ENV{CLAS12ROOT})
set(HIPO $ENV{HIPO})
set(ROOTSYS_ENV $ENV{ROOTSYS})

# Print the detected external paths to make configuration easier to debug.
message(STATUS "CLAS12ROOT = ${CLAS12ROOT}")
message(STATUS "HIPO       = ${HIPO}")
message(STATUS "ROOTSYS    = ${ROOTSYS_ENV}")

# If ROOTSYS is set, append it to CMAKE_PREFIX_PATH so find_package(ROOT)
# can locate the ROOT installation more easily.
if(ROOTSYS_ENV)
    list(PREPEND CMAKE_PREFIX_PATH "${ROOTSYS_ENV}")
endif()

# When the local ROOT installation changes, stale CMake cache entries can keep
# pointing at libraries and VDT headers from an older Homebrew Cellar path.
# Reset those cached discovery results so find_package(ROOT) resolves the
# current ROOTSYS installation consistently.
if(ROOTSYS_ENV)
    set(ROOT_CMAKE_DIR "${ROOTSYS_ENV}/share/root/cmake")

    get_cmake_property(_cache_variables CACHE_VARIABLES)
    foreach(_cache_var IN LISTS _cache_variables)
        if(_cache_var MATCHES "^(ROOT_|VDT_|Vdt_|FIND_PACKAGE_MESSAGE_DETAILS_Vdt$)")
            unset(${_cache_var} CACHE)
        endif()
    endforeach()

    if(EXISTS "${ROOT_CMAKE_DIR}/ROOTConfig.cmake")
        set(ROOT_DIR "${ROOT_CMAKE_DIR}" CACHE PATH "Path to the ROOT CMake package" FORCE)
    endif()
endif()

# Use the vendored yaml-cpp source tree instead of downloading it during the
# configure step. This makes SSH builds more stable and avoids repeated
# network-dependent setup work.
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/vendor/yaml-cpp/yaml-cpp-src
    ${CMAKE_BINARY_DIR}/vendor/yaml-cpp-build
    EXCLUDE_FROM_ALL
)

# Locate the ROOT installation and load ROOT's CMake helper settings.
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

# When MPI support is requested, locate MPI and define USE_MPI for the code.
if(USE_MPI)
    find_package(MPI REQUIRED)
    add_compile_definitions(USE_MPI)
endif()

# Global compiler flags.
# ROOT_CXX_FLAGS comes from the ROOT package configuration.
# -O0 keeps optimization disabled for easier debugging.
# -ggdb adds debug information suitable for gdb and related debuggers.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS} -O0 -ggdb")

# Fail early if the CLAS12ROOT environment variable was not provided.
if(NOT CLAS12ROOT)
    message(FATAL_ERROR "CLAS12ROOT environment variable is not set.")
endif()

# Fail early if the HIPO environment variable was not provided.
if(NOT HIPO)
    message(FATAL_ERROR "HIPO environment variable is not set.")
endif()

# Add global include directories needed throughout the project.
# include_directories() applies broadly, so these paths are available to the
# targets defined in the subdirectories added later.
include_directories(
    # ROOT headers.
    ${ROOT_INCLUDE_DIRS}

    # HIPO headers.
    ${HIPO}/include
    ${HIPO}/include/hipo4

    # CLAS12ROOT headers.
    ${CLAS12ROOT}/Clas12Root
    ${CLAS12ROOT}/Clas12Banks

    # Project headers.
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add global linker search directories for external libraries.
# This helps the linker resolve library names such as Clas12Root and Hipo4.
link_directories(
    ${HIPO}/lib
    ${HIPO}/lib64
    ${CLAS12ROOT}/lib
    ${CLAS12ROOT}/lib64
)

# Define a project-wide preprocessor macro containing the source directory path.
# The code can use CLAS12ANA_DIR at compile time to refer back to the project
# source tree.
add_definitions(-D_CLAS12ANA_DIR="${CMAKE_SOURCE_DIR}")
add_definitions(-D_CONFIG_DIR="${CMAKE_SOURCE_DIR}/config")

# Add the build subdirectories that define the project's actual targets.
# source/ builds the reusable library code and links against the vendored
# yaml-cpp source tree added above.
# apps/ builds the executable programs.
add_subdirectory(source)
add_subdirectory(apps)
