project (examples)

# The version number
set (examples_VERSION_MAJOR 1)
set (examples_VERSION_MINOR 0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall")

set (EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)

# Thirdparty libraries
add_subdirectory(thirdparty/googletest)

message ("############################################################")
message ("# Tests                                                 #")
message ("CMAKE_HOME_DIRECTORY => ${CMAKE_HOME_DIRECTORY}")
message ("CMAKE_INCLUDE_DIR =>    ${CMAKE_INCLUDE_DIR}")
message ("CMAKE_SOURCE_DIR =>     ${CMAKE_SOURCE_DIR}")
message ("CMAKE_BINARY_DIR =>     ${CMAKE_BINARY_DIR}")
message ("EXE_OUTPUT_PATH =>      ${EXECUTABLE_OUTPUT_PATH}")

include_directories (${CMAKE_SOURCE_DIR}/libs)
include_directories (${CMAKE_SOURCE_DIR}/libs/inc)

# add_dependencies(${PROJECT_NAME} getGoogleTest)
