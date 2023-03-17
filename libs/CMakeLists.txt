set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message ("##################################################################################################")
message ("# libs                                                                                        #")
message ("CMAKE_HOME_DIRECTORY => ${CMAKE_HOME_DIRECTORY}")
message ("CMAKE_INCLUDE_DIR =>    ${CMAKE_INCLUDE_DIR}")
message ("CMAKE_SOURCE_DIR =>     ${CMAKE_SOURCE_DIR}")
message ("CMAKE_BINARY_DIR =>     ${CMAKE_BINARY_DIR}")
message ("CMAKE_LIB_OUTPUT_DIR => ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

include_directories(${CMAKE_SOURCE_DIR}/libs/inc)

find_library(LIBRT rt)
add_library(gposix SHARED
    src/Fifo.cpp
    src/Inotify.cpp
    src/MsgQueues.cpp
    src/Pid.cpp
    src/Semaphore.cpp
    src/SockDgram.cpp
    src/SockStreamClient.cpp
    src/SockStreamServer.cpp
)
target_link_libraries(gposix ${LIBRT})

add_library(timer SHARED src/Timer.cpp)

add_library(event SHARED src/Event.cpp)
target_link_libraries(event timer)