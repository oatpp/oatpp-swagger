cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(oatpp-swagger)

set(CMAKE_CXX_STANDARD 11)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(DIR_SRC_OATPP "Path to oatpp module directory (sources)")
option(DIR_LIB_OATPP "Path to directory with liboatpp (directory containing ex: liboatpp.so or liboatpp.dynlib)")

message("oatpp-swagger module")

if(DIR_SRC_OATPP)
    message("DIR_SRC_OATPP='${DIR_SRC_OATPP}'")
else()
    message(FATAL_ERROR "'DIR_SRC_OATPP' parameter is required. It should point to main oatpp module directory.")
endif()

if(BUILD_SHARED_LIBS)
    if(DIR_LIB_OATPP)
        message("DIR_LIB_OATPP='${DIR_LIB_OATPP}'")
    else()
        message(FATAL_ERROR "'DIR_LIB_OATPP' parameter is required. It should point to directory with liboatpp (directory containing ex: liboatpp.so or liboatpp.dynlib).")
    endif()
endif()


add_subdirectory("src")