PROJECT(posix)

CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

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
# message ("-")
# ADD_SUBDIRECTORY(tests)
