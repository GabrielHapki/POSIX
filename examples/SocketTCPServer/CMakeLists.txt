project (SocketTCPServer)

# The version number
set (SocketTCPServer_VERSION_MAJOR 1)
set (SocketTCPServer_VERSION_MINOR 0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall")

set (EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)
#set (CMAKE_INCLUDE_DIR ${CMAKE_HOME_DIRECTORY}/include)

message ("############################################################")
message ("# Example SocketTCPServer                                  #")
message ("CMAKE_HOME_DIRECTORY => ${CMAKE_HOME_DIRECTORY}")
message ("CMAKE_INCLUDE_DIR =>    ${CMAKE_INCLUDE_DIR}")
message ("CMAKE_SOURCE_DIR =>     ${CMAKE_SOURCE_DIR}")
message ("CMAKE_BINARY_DIR =>     ${CMAKE_BINARY_DIR}")
message ("EXE_OUTPUT_PATH =>      ${EXECUTABLE_OUTPUT_PATH}")

include_directories (${CMAKE_SOURCE_DIR}/libs/libSocketTCP)
link_directories (${CMAKE_BINARY_DIR}/lib)

add_executable (SocketTCPServer main.cpp)

target_link_libraries (SocketTCPServer SocketTCP)