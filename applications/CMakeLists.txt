set(APPS_SOURCES
    DetectorSimulationAnalyser.cpp
)

# Create a library
add_library(AnalysisApps ${APPS_SOURCES})

# Link ROOT properly (without -lEG)
target_link_libraries(AnalysisApps PUBLIC ${ROOT_LIBRARIES})

# Link Clas12 libraries correctly (no -l)
target_link_libraries(AnalysisApps PUBLIC Clas12Banks Hipo4 Clas12Root)

# set(APPS_SOURCES
#     DetectorSimulationAnalyser.cpp
# )

# foreach(fnameAppSrc ${APPS_SOURCES})
    
#     message(STATUS ${fnameAppSrc})
    
#     string(REPLACE ".cpp" "" fnameAppExe ${fnameAppSrc})
    
#     add_executable(${fnameAppExe} ${fnameAppSrc})
#     # Link libraries

#     target_link_libraries(${fnameAppExe} PUBLIC ${ROOT_LIBRARIES})

#     # Link Clas12 libraries correctly (no -l)
#     target_link_libraries(${fnameAppExe} PUBLIC Clas12Banks Hipo4 Clas12Root)

#     # target_link_libraries(${fnameAppExe} ${ROOT_LIBRARIES})
#     # target_link_libraries(${fnameAppExe} -lClas12Banks -lHipo4 -lClas12Root)
#     # target_link_libraries(${fnameAppExe} -L${CLAS12ROOT}/lib)
#     # # target_link_libraries(${fnameAppExe} ${ROOT_LIBRARIES} PkgConfig::hipo4 -lEG -lClas12Banks -lClas12Root -L${CLAS12ROOT}/lib -L${CLAS12ROOT}/ccdb/lib)
# endforeach()