project (examples)

# The version number
set (examples_VERSION_MAJOR 1)
set (examples_VERSION_MINOR 0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-Wall")

set (EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)

message ("############################################################")
message ("# Examples                                                 #")
message ("CMAKE_HOME_DIRECTORY => ${CMAKE_HOME_DIRECTORY}")
message ("CMAKE_INCLUDE_DIR =>    ${CMAKE_INCLUDE_DIR}")
message ("CMAKE_SOURCE_DIR =>     ${CMAKE_SOURCE_DIR}")
message ("CMAKE_BINARY_DIR =>     ${CMAKE_BINARY_DIR}")
message ("EXE_OUTPUT_PATH =>      ${EXECUTABLE_OUTPUT_PATH}")

include_directories (${CMAKE_SOURCE_DIR}/libs)

add_executable (events Events.cpp)
add_executable (fifoClient FifoClient.cpp)
add_executable (fifoServer FifoServer.cpp)
add_executable (inotify Inotify.cpp)

find_library(LIBRT rt)
add_executable (msgReceiver MessageReceiver.cpp)
target_link_libraries(msgReceiver ${LIBRT})
add_executable (msgReceiverEvent MessageReceiverEvent.cpp)
target_link_libraries(msgReceiverEvent ${LIBRT})
add_executable (msgSender MessageSender.cpp)
target_link_libraries(msgSender ${LIBRT})

add_executable (sockDgramClient SockDgramClient.cpp)
add_executable (sockDgramServer SockDgramServer.cpp)
add_executable (sockStreamClient SockStreamClient.cpp)
add_executable (sockStreamServer SockStreamServer.cpp)