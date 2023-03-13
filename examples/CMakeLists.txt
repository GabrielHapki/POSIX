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
include_directories (${CMAKE_SOURCE_DIR}/libs/inc)

add_executable (events Events.cpp)
target_link_libraries(events timer)

add_executable (fifoClient FifoClient.cpp)
target_link_libraries(fifoClient fifo)
add_executable (fifoServer FifoServer.cpp)
target_link_libraries(fifoServer fifo)
add_executable (eg_inotify Inotify.cpp)
target_link_libraries(eg_inotify inotify)

add_executable (msgReceiver MessageReceiver.cpp)
target_link_libraries(msgReceiver msgqueues)
add_executable (msgReceiverEvent MessageReceiverEvent.cpp)
target_link_libraries(msgReceiverEvent msgqueues)
add_executable (msgSender MessageSender.cpp)
target_link_libraries(msgSender msgqueues)

add_executable (sockDgramClient SockDgramClient.cpp)
target_link_libraries(sockDgramClient sockdgram)
add_executable (sockDgramServer SockDgramServer.cpp)
target_link_libraries(sockDgramServer sockdgram)
add_executable (sockStreamClient SockStreamClient.cpp)
add_executable (sockStreamServer SockStreamServer.cpp)