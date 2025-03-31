cmake_minimum_required(VERSION 3.29)
project(2N_Analysis_e4nu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CLAS12ROOT $ENV{CLAS12ROOT})
set(HIPO $ENV{HIPO})

set(Analyser 2N_Analysis_e4nu)

find_package(ROOT)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS}) 
include(${ROOT_USE_FILE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

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

add_executable(${Analyser}  main.C)

# Link libraries
# target_link_libraries(${Analyser}
#     hipo4
#     Clas12Banks
#     Clas12Root
#     ${ROOT_LIBRARIES}
# )



target_link_libraries(${Analyser}
    ${ROOT_LIBRARIES} -lEG 
    -lHipo4
    -lClas12Banks -lClas12Root
    -L${CLAS12ROOT}/lib
)



# target_link_libraries(${Analyser} ${ROOT_LIBRARIES} -lEG)
# target_link_libraries(${Analyser} -lClas12Banks -lHipo4 -lClas12Root)
# target_link_libraries(${Analyser} -L${CLAS12ROOT}/lib)