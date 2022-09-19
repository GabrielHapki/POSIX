project (Events)

# The version number
set (Events_VERSION_MAJOR 1)
set (Events_VERSION_MINOR 0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall")

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message ("##################################################################################################")
message ("# lib Events                                                                                  #")
message ("CMAKE_HOME_DIRECTORY => ${CMAKE_HOME_DIRECTORY}")
message ("CMAKE_INCLUDE_DIR =>    ${CMAKE_INCLUDE_DIR}")
message ("CMAKE_SOURCE_DIR =>     ${CMAKE_SOURCE_DIR}")
message ("CMAKE_BINARY_DIR =>     ${CMAKE_BINARY_DIR}")
message ("CMAKE_LIB_OUTPUT_DIR => ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

include_directories (${CMAKE_SOURCE_DIR}/libs)
link_directories (${CMAKE_BINARY_DIR}/lib)

add_library (${PROJECT_NAME} SHARED Events.cpp)
