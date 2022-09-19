project (gInotify)

# The version number
set (gEvents_VERSION_MAJOR 1)
set (gEvents_VERSION_MINOR 0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall")

set (EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)

message ("############################################################")
message ("# Example gEvents                                          #")
message ("CMAKE_HOME_DIRECTORY => ${CMAKE_HOME_DIRECTORY}")
message ("CMAKE_INCLUDE_DIR =>    ${CMAKE_INCLUDE_DIR}")
message ("CMAKE_SOURCE_DIR =>     ${CMAKE_SOURCE_DIR}")
message ("CMAKE_BINARY_DIR =>     ${CMAKE_BINARY_DIR}")
message ("EXE_OUTPUT_PATH =>      ${EXECUTABLE_OUTPUT_PATH}")

include_directories (${CMAKE_SOURCE_DIR}/libs)

add_executable (${PROJECT_NAME} main.cpp)
