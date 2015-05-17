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

#-------------------------------------------------------------------------------
# Use relative paths
# This is mostly to reduce path size for command-line limits on windows
#-------------------------------------------------------------------------------
IF (WIN32)
    # This seems to break Xcode projects so definitely don't enable on Apple builds
    SET(CMAKE_USE_RELATIVE_PATHS false)
    SET(CMAKE_SUPPRESS_REGENERATION false)
    SET(ROKLEGEND_SHAREDEXT ".dll")
ELSE ()
    SET(ROKLEGEND_SHAREDEXT ".so")
ENDIF ()

# Constants
SET(ROKLEGEND_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
SET(ROKLEGEND_SOURCE_DIR "${ROKLEGEND_ROOT_DIR}/source")
SET(ROKLEGEND_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
SET(ROKLEGEND_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
SET(ROKLEGEND_BINARY_DIR_REL "${ROKLEGEND_BUILD_DIR}/bin/Release")
SET(ROKLEGEND_BINARY_DIR_DBG "${ROKLEGEND_BUILD_DIR}/bin/Debug")
SET(ROKLEGEND_LIB_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
SET(ROKLEGEND_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
SET(ROKLEGEND_ARCH_64 "x64")
SET(ROKLEGEND_ARCH_32 "x86")

IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(ROKLEGEND_ARCH ${ROKLEGEND_ARCH_64})
    SET(RO_IS_32BIT FALSE)
    SET(RO_IS_64BIT TRUE)
ELSE ()
    SET(ROKLEGEND_ARCH ${ROKLEGEND_ARCH_32})
    SET(RO_IS_32BIT TRUE)
    SET(RO_IS_64BIT FALSE)
ENDIF ()

IF (WIN32)
    SET(ROKLEGEND_LIB_DIR_DBG ${ROKLEGEND_LIB_DIR}/Debug)
    SET(ROKLEGEND_LIB_DIR_REL ${ROKLEGEND_LIB_DIR}/Release)
ELSE ()
    SET(ROKLEGEND_LIB_DIR_DBG ${ROKLEGEND_LIB_DIR})
    SET(ROKLEGEND_LIB_DIR_REL ${ROKLEGEND_LIB_DIR})
ENDIF ()

IF (MSVC)
    # Create debug libraries with _d postfix
    SET(CMAKE_DEBUG_POSTFIX "_d")
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE)
    ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
    # Enable intrinsics on MSVC in debug mode
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Oi")
    IF (CMAKE_CL_64)
        # Visual Studio bails out on debug builds in 64bit mode unless
        # this flag is set...
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
        SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
        SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /bigobj")
    ENDIF()
ENDIF ()

FIND_PACKAGE(Boost)

# Set common stuff here
SET(RO_COMMON_INCLUDES
    ${Boost_INCLUDE_DIRS}
)
SET(RO_COMMON_LINK_DIRECTORIES
    ${Boost_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES(${RO_COMMON_INCLUDES})
LINK_DIRECTORIES(${RO_COMMON_LINK_DIRECTORIES})

ADD_SUBDIRECTORY(source)