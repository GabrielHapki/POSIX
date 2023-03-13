set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message ("##################################################################################################")
message ("# libs                                                                                        #")
message ("CMAKE_HOME_DIRECTORY => ${CMAKE_HOME_DIRECTORY}")
message ("CMAKE_INCLUDE_DIR =>    ${CMAKE_INCLUDE_DIR}")
message ("CMAKE_SOURCE_DIR =>     ${CMAKE_SOURCE_DIR}")
message ("CMAKE_BINARY_DIR =>     ${CMAKE_BINARY_DIR}")
message ("CMAKE_LIB_OUTPUT_DIR => ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

include_directories(${CMAKE_SOURCE_DIR}/libs/inc)

add_library(fifo SHARED src/Fifo.cpp)
add_library(inotify SHARED src/Inotify.cpp)

find_library(LIBRT rt)
add_library(msgqueues SHARED src/MsgQueues.cpp)
target_link_libraries(msgqueues ${LIBRT})

add_library(pid SHARED src/Pid.cpp)
add_library(semaphore SHARED src/Semaphore.cpp)
add_library(sockdgram SHARED src/SockDgram.cpp)
add_library(timer SHARED src/Timer.cpp)

