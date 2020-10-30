project(GSTATS C CXX)
cmake_minimum_required(VERSION 3.6)

set( CMAKE_VERBOSE_MAKEFILE off )
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )
set( CMAKE_C_COMPILER "/usr/local/Cellar/gcc/9.3.0_1/bin/gcc-9" )
set( CMAKE_C_FLAGS "-O3" )
set( CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/9.3.0_1/bin/g++-9" )
set( CMAKE_CXX_FLAGS "-O3 -ftree-vectorize -std=c++11 -fopenmp -DOPENMP -DTHREADS=4" )
set( GSTATS_INCLUDES )
set( GSTATS_LIBRARIES )

set (CMAKE_CXX_STANDARD 11)
cmake_policy(SET CMP0048 NEW)

# include
include_directories( "${CMAKE_SOURCE_DIR}" )

# GSL (external)
find_package(GSL)
if(GSL_FOUND)
    set(HAVE_LIBGSL 1)
    list(APPEND GSTATS_INCLUDES ${GSL_INCLUDE_DIR})
    list(APPEND GSTATS_LIBRARIES ${GSL_LIBRARIES})
else(GSL_FOUND)
    message(FATAL_ERROR "Require GSL: set GSL_HOME")
endif(GSL_FOUND)

# add source files
set(GSTATS_SRCS
    src/TAnisotropyCombo.cpp
    src/TAverageFlux.cpp
    src/TData.cpp
    src/TElectrons.cpp
    src/TFitElectrons.cpp
    src/TFitPositrons.cpp
    src/TFitProtons.cpp
    src/TPositrons.cpp
    src/TProtons.cpp
    src/TSecondary.cpp
    src/Utilities.cpp
    )	

# make library
include_directories (include ${GSTATS_INCLUDES})
add_library( GSTATS_LIB ${GSTATS_SRCS} ${GSTATS_INCLUDES} )

# make executables
add_executable(combine_outputs projects/combine_outputs.cpp)
target_link_libraries (combine_outputs GSTATS_LIB ${GSTATS_LIBRARIES})

add_executable(nuclei_outputs projects/nuclei_outputs.cpp)
target_link_libraries (nuclei_outputs GSTATS_LIB ${GSTATS_LIBRARIES})

# show summary
message (STATUS "CMAKE_SYSTEM .......... = ${CMAKE_SYSTEM}")
message (STATUS "BUILD_SHARED_LIBS ..... = ${BUILD_SHARED_LIBS}")
message (STATUS "CMAKE_C_COMPILER ...... = ${CMAKE_C_COMPILER}")
message (STATUS "CMAKE_C_FLAGS ......... = ${CMAKE_C_FLAGS}")
message (STATUS "CMAKE_CXX_COMPILER .... = ${CMAKE_CXX_COMPILER}")
message (STATUS "CMAKE_CXX_FLAGS ....... = ${CMAKE_CXX_FLAGS}")
message (STATUS "CMAKE_INCLUDE_PATH .... = ${CMAKE_INCLUDE_PATH}")
message (STATUS "GSTATS_INCLUDES ...... = ${GSTATS_INCLUDES}")
message (STATUS "GSTATS_LIBRARIES ..... = ${GSTATS_LIBRARIES}")