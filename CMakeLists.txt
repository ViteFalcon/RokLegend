#------------------------------------------------------------------------------
# This file is part of the CMake build system for RokLegend
#               (Ragnarok Online Client)
#------------------------------------------------------------------------------
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
CMAKE_POLICY(SET CMP0003 NEW)

################################################################################
# Set up the basic build environment
################################################################################
# Just debug / release since that's all that's all we need
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

IF (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:precise /vmg /Zm1000" CACHE STRING "" FORCE)
    # Enable intrinsics on MSVC in debug mode
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Oi /vmg /Zm1000" CACHE STRING "" FORCE)
ENDIF ()

SET(ROKLEGEND_PROJECT_NAME "RokLegend")

PROJECT(${ROKLEGEND_PROJECT_NAME})

# Constants
SET(ROKLEGEND_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
SET(ROKLEGEND_DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")
SET(ROKLEGEND_SOURCE_DIR "${ROKLEGEND_ROOT_DIR}/source")
SET(ROKLEGEND_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
SET(ROKLEGEND_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
SET(ROKLEGEND_BINARY_DIR_REL "${ROKLEGEND_BUILD_DIR}/bin/Release")
SET(ROKLEGEND_BINARY_DIR_DBG "${ROKLEGEND_BUILD_DIR}/bin/Debug")
SET(ROKLEGEND_LIB_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
SET(ROKLEGEND_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
SET(ROKLEGEND_HXX_SOURCE_GROUP "Header Files")
SET(ROKLEGEND_CXX_SOURCE_GROUP "Source Files")
SET(ROKLEGEND_ARCH_64 "x64")
SET(ROKLEGEND_ARCH_32 "x86")

LIST(APPEND CMAKE_MODULE_PATH
    ${ROKLEGEND_CMAKE_DIR}
)

INCLUDE(cotire)
INCLUDE(roklegend_macros)
INCLUDE(roklegend_platform)
# Dependencies
INCLUDE(roklegend_irrklang)
INCLUDE(roklegend_zlib)
INCLUDE(roklegend_grf)
INCLUDE(roklegend_tbb)
INCLUDE(roklegend_sqlitepp)
INCLUDE(roklegend_entityx)

FIND_PACKAGE(Boost)
IF ("${Boost_LIBRARY_DIRS}" STREQUAL "")
    IF (MSVC12 AND CMAKE_CL_64)
        SET(Boost_LIBRARY_DIRS "${Boost_INCLUDE_DIRS}/lib64-msvc-12.0")
    ENDIF ()
ENDIF ()
IF (MSVC12)
	ADD_DEFINITIONS(-DBOOST_EXCLUDE_MSVC12_BUG)
ENDIF ()

# Set common stuff here
INCLUDE_DIRECTORIES(
    ${ROKLEGEND_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${TBB_INCLUDE_DIRS}
)
LINK_DIRECTORIES(
    ${Boost_LIBRARY_DIRS}
    ${irrKlang_LIBRARY_DIRS}
    ${TBB_LIBRARY_DIRS}
)

ADD_SUBDIRECTORY(source)

IF (NOT "" STREQUAL "${ROKLEGEND_BINARY_DBG}")
    FILE(COPY ${ROKLEGEND_BINARY_DBG} DESTINATION ${ROKLEGEND_BINARY_DIR_DBG})
ENDIF ()

IF (NOT "" STREQUAL "${ROKLEGEND_BINARY_REL}")
    FILE(COPY ${ROKLEGEND_BINARY_REL} DESTINATION ${ROKLEGEND_BINARY_DIR_REL})
ENDIF ()

SET(ROKLEGEND_ROOT_FILE "${ROKLEGEND_BUILD_DIR}/source/application/ROOT")
FILE(WRITE  ${ROKLEGEND_ROOT_FILE} "${ROKLEGEND_ROOT_DIR}")

SET(RO_CMAKE_INCLUDED_FILES
	cmake/cotire.cmake
	cmake/roklegend_grf.cmake
	cmake/roklegend_irrklang.cmake
	cmake/roklegend_macros.cmake
	cmake/roklegend_platform.cmake
	cmake/roklegend_sqlitepp.cmake
	cmake/roklegend_tbb.cmake
	cmake/roklegend_zlib.cmake
)
SOURCE_GROUP(cmake FILES ${RO_CMAKE_INCLUDED_FILES})

SET(RO_CMAKE_SOURCE_FILES
	source/CMakeLists.txt
)
SOURCE_GROUP(source FILES ${RO_CMAKE_SOURCE_FILES})

SET(RO_CMAKE_FILES
	${RO_CMAKE_INCLUDED_FILES}
	${RO_CMAKE_SOURCE_FILES}
	CMakeLists.txt
)

ADD_CUSTOM_TARGET(CMakeFiles SOURCES ${RO_CMAKE_FILES})