PROJECT(posix)

CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

# The version number
set (examples_VERSION_MAJOR 1)
set (examples_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")

set (POSIX_DIRECTORY  ${CMAKE_HOME_DIRECTORY})

#------------------------------------------------------------
# Libraries
#------------------------------------------------------------
message ("-")
ADD_SUBDIRECTORY(libs)

#------------------------------------------------------------
# Examples
#------------------------------------------------------------
message ("-")
ADD_SUBDIRECTORY(examples)

#------------------------------------------------------------
# Tests
#------------------------------------------------------------
message ("-")
ADD_SUBDIRECTORY(tests)
