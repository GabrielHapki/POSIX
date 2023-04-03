project (examples)

# https://matgomes.com/integrate-google-test-into-cmake/

set (EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
  googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  GIT_REPOSITORY      https://github.com/google/googletest.git
  GIT_TAG             v1.13.0  
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main gmock_main)

enable_testing()

include_directories (${CMAKE_SOURCE_DIR}/libs)
include_directories (${CMAKE_SOURCE_DIR}/libs/inc)

add_executable(
  posix_test
  test_fifo.cpp
  test_message.cpp
  test_sock_dgram.cpp
  test_sock_stream.cpp
  test_timer.cpp
  test_turtle.cpp
)
target_link_libraries(
  posix_test
  gposix
  timer
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(posix_test)
