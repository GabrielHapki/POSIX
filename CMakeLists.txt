PROJECT(posix)

CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

set (POSIX_DIRECTORY  ${CMAKE_HOME_DIRECTORY})

#------------------------------------------------------------
# Libs
#------------------------------------------------------------
message ("-")
ADD_SUBDIRECTORY(libs)

#------------------------------------------------------------
# Examples
#------------------------------------------------------------
message ("-")
ADD_SUBDIRECTORY(examples)