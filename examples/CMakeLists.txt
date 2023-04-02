project (examples)

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
target_link_libraries(events event timer)

add_executable (eg_inotify Inotify.cpp)
target_link_libraries(eg_inotify gposix)
