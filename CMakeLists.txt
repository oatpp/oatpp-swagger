cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(OATPP_THIS_MODULE_NAME oatpp-swagger)

project(${OATPP_THIS_MODULE_NAME})

set(CMAKE_CXX_STANDARD 11)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(OATPP_DIR_SRC "Path to oatpp module directory (sources)")
option(OATPP_DIR_LIB "Path to directory with liboatpp (directory containing ex: liboatpp.so or liboatpp.dynlib)")
option(OATPP_BUILD_TESTS "Build tests for this module" ON)

message("OATPP_MODULE='${OATPP_THIS_MODULE_NAME}'")

#####################################################################################################
##  if OATPP_BUILD_TESTS=ON and both options OATPP_DIR_SRC and  OATPP_DIR_LIB are empty
##  oatpp lib will be git-cloned.
##  Otherwise options OATPP_DIR_SRC and OATPP_DIR_LIB are expected to point to valid directories
#####################################################################################################

if(OATPP_BUILD_TESTS AND (NOT OATPP_DIR_SRC) AND (NOT OATPP_DIR_LIB))

    include(ExternalProject)

    set(MODULE_WAIT_DEPS ON)

    set(LIB_oatpp "lib_oatpp")
    ExternalProject_Add(${LIB_oatpp}
            GIT_REPOSITORY "https://github.com/oatpp/oatpp.git"
            GIT_TAG origin/new_module_structure
            CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} -DOATPP_INSTALL=OFF -DOATPP_BUILD_TESTS=OFF
            INSTALL_COMMAND cmake -E echo "SKIP INSTALL '${LIB_oatpp}'"
    )

    ExternalProject_Get_Property(${LIB_oatpp} BINARY_DIR)
    set(OATPP_DIR_LIB ${BINARY_DIR})

    ExternalProject_Get_Property(${LIB_oatpp} SOURCE_DIR)
    set(OATPP_DIR_SRC ${SOURCE_DIR})

    message("OATPP_DIR_SRC --> '${OATPP_DIR_SRC}'")
    message("OATPP_DIR_LIB --> '${OATPP_DIR_LIB}'")

else()

    if(OATPP_DIR_SRC)
        message("OATPP_DIR_SRC='${OATPP_DIR_SRC}'")
    else()
        message(FATAL_ERROR "'OATPP_DIR_SRC' parameter is required. It should point to main oatpp module directory.")
    endif()

    if(BUILD_SHARED_LIBS)
        if(OATPP_DIR_LIB)
            message("OATPP_DIR_LIB='${OATPP_DIR_LIB}'")
        else()
            message(FATAL_ERROR "'OATPP_DIR_LIB' parameter is required. It should point to directory with liboatpp (directory containing ex: liboatpp.so or liboatpp.dynlib).")
        endif()
    endif()

endif()

add_subdirectory("src")

if(OATPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory("test")
endif()