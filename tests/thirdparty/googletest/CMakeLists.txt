include(ExternalProject)
ExternalProject_Add(getGoogleTest
    PREFIX getGoogleTest

    # --- Download step ---
    GIT_REPOSITORY      https://github.com/google/googletest.git
    GIT_TAG             v1.13.0
    LOG_DOWNLOAD        1

    # --- Patch step ---
    # PATCH_COMMAND       cp -v ${CMAKE_CURRENT_SOURCE_DIR}/patch/CMakeLists.txt <SOURCE_DIR>/
    LOG_PATCH           ""

    CMAKE_ARGS          -DBUILD_SHARED_LIBS=YES
                        -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_OUTPUT_DIRECTORY}
                        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_OUTPUT_DIRECTORY}
                        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
                        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}

    UPDATE_COMMAND      ""

    # --- Configure step ---
    # CONFIGURE_COMMAND   ""
    LOG_CONFIGURE       1

    # --- Build step ---
    # BUILD_IN_SOURCE     1
    # BUILD_COMMAND       gcc -c <SOURCE_DIR>/gtest.c -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Os -g0 -D_FORTIFY_SOURCE=1 -DMG_ENABLE_OPENSSL=1 -o <SOURCE_DIR>/gtest.o
    # COMMAND             ar rcs <SOURCE_DIR>/libgtest.a <SOURCE_DIR>/gtest.o
    LOG_BUILD           1

    # --- Install step ---
    # UPDATE_COMMAND      ""
    # INSTALL_COMMAND     ""
    LOG_INSTALL         1

    # --- Test step ---
    # TEST_COMMAND        ""
)

# ExternalProject_Get_Property(x264 SOURCE_DIR)
# add_library(x264 STATIC IMPORTED)
# set_target_properties(x264 PROPERTIES IMPORTED_LOCATION ${SOURCE_DIR}/libx264.a)

set(GTEST_INCLUDE_DIRS ${CMAKE_INSTALL_OUTPUT_DIRECTORY}/include CACHE PATH "gtest header directory" FORCE)
message(STATUS "gtest Include directory: ${GTEST_INCLUDE_DIRS}")

set(GTEST_LIBRARIES ${CMAKE_INSTALL_OUTPUT_DIRECTORY}/lib/libgtest.so CACHE PATH "gtest library directory" FORCE)
message(STATUS "gtest Library directory: ${GTEST_LIBRARIES}")
