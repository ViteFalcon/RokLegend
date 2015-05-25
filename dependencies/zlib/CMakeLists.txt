#------------------------------------------------------------------------------
# This file is part of the CMake build system for RokLegend
#               (Ragnarok Online Client)
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

################################################################################
# Set up the basic build environment
################################################################################
# Just debug / release since that's all that's all we need
set (CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:precise")
  # Enable intrinsics on MSVC in debug mode
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Oi")
endif ()

set(ZLIB_PROJECT_NAME "zlib")

project(${ZLIB_PROJECT_NAME} C)

set(VERSION "1.2.7")

set(ZLIB_INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(ZLIB_INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")

set(ZLIB_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ZLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")


include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)
enable_testing()

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stddef.h    HAVE_STDDEF_H)

#
# Check to see if we have large file support
#
set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
# We add these other definitions here because CheckTypeSize.cmake
# in CMake 2.4.x does not automatically do so and we want
# compatibility with CMake 2.4.x.
if(HAVE_SYS_TYPES_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
endif()
if(HAVE_STDINT_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
endif()
if(HAVE_STDDEF_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
endif()
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if(MSVC)
    # Create debug libraries with _d postfix
    set(CMAKE_DEBUG_POSTFIX "_d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif()

#if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
#    # If we're doing an out of source build and the user has a zconf.h
#    # in their source tree...
#    if(EXISTS ${ZLIB_INCLUDE_DIR}/zconf.h)
#        message(STATUS "Renaming")
#        message(STATUS "    ${ZLIB_INCLUDE_DIR}/zconf.h")
#        message(STATUS "to 'zconf.h.included' because this file is included with zlib")
#        message(STATUS "but CMake generates it automatically in the build directory.")
#        file(RENAME ${ZLIB_INCLUDE_DIR}/zconf.h ${ZLIB_INCLUDE_DIR}/zconf.h.included)
#  endif()
#endif()

set(ZLIB_PC ${CMAKE_CURRENT_BINARY_DIR}/zlib.pc)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/zlib.pc.cmakein
        ${ZLIB_PC} @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/zconf.h.cmakein
        ${ZLIB_INCLUDE_DIR}/zconf.h @ONLY)
include_directories(${ZLIB_INCLUDE_DIR})


#============================================================================
# zlib
#============================================================================

set(ZLIB_PUBLIC_HDRS
    ${ZLIB_INCLUDE_DIR}/zconf.h
    ${ZLIB_INCLUDE_DIR}/zlib.h
)
set(ZLIB_PRIVATE_HDRS
    ${ZLIB_INCLUDE_DIR}/crc32.h
    ${ZLIB_INCLUDE_DIR}/deflate.h
    ${ZLIB_INCLUDE_DIR}/gzguts.h
    ${ZLIB_INCLUDE_DIR}/inffast.h
    ${ZLIB_INCLUDE_DIR}/inffixed.h
    ${ZLIB_INCLUDE_DIR}/inflate.h
    ${ZLIB_INCLUDE_DIR}/inftrees.h
    ${ZLIB_INCLUDE_DIR}/trees.h
    ${ZLIB_INCLUDE_DIR}/zutil.h
)
set(ZLIB_SRCS
    ${ZLIB_SOURCE_DIR}/adler32.c
    ${ZLIB_SOURCE_DIR}/compress.c
    ${ZLIB_SOURCE_DIR}/crc32.c
    ${ZLIB_SOURCE_DIR}/deflate.c
    ${ZLIB_SOURCE_DIR}/gzclose.c
    ${ZLIB_SOURCE_DIR}/gzlib.c
    ${ZLIB_SOURCE_DIR}/gzread.c
    ${ZLIB_SOURCE_DIR}/gzwrite.c
    ${ZLIB_SOURCE_DIR}/inflate.c
    ${ZLIB_SOURCE_DIR}/infback.c
    ${ZLIB_SOURCE_DIR}/inftrees.c
    ${ZLIB_SOURCE_DIR}/inffast.c
    ${ZLIB_SOURCE_DIR}/trees.c
    ${ZLIB_SOURCE_DIR}/uncompr.c
    ${ZLIB_SOURCE_DIR}/zutil.c
)

add_definitions(-DNO_FSEEKO)
add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)

if(NOT MINGW)
    set(ZLIB_SRCS ${ZLIB_SRCS}
        ${ZLIB_SOURCE_DIR}/win32/zlib1.rc # If present will override custom build rule below.
    )
endif()

# parse the full version number from zlib.h and include in ZLIB_FULL_VERSION
file(READ ${ZLIB_INCLUDE_DIR}/zlib.h _zlib_h_contents)
string(REGEX REPLACE ".*#define[ \t]+ZLIB_VERSION[ \t]+\"([-0-9A-Za-z.]+)\".*"
    "\\1" ZLIB_FULL_VERSION ${_zlib_h_contents})

if(MINGW)
    # This gets us DLL resource information when compiling on MinGW.
    if(NOT CMAKE_RC_COMPILER)
        SET(CMAKE_RC_COMPILER windres.exe)
    endif()

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/zlib1rc.obj
                       COMMAND ${CMAKE_RC_COMPILER}
                            -D GCC_WINDRES
                            -I ${ZLIB_INCLUDE_DIR}
                            -o ${CMAKE_CURRENT_BINARY_DIR}/zlib1rc.obj
                            -i ${ZLIB_INCLUDE_DIR}/win32/zlib1.rc)
    set(ZLIB_SRCS ${ZLIB_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/zlib1rc.obj)
endif(MINGW)

add_library(zlib STATIC ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})

if (MSVC)
    # Copy libs to 'lib' directory
    set(ZLIB_DEST_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    add_custom_command(TARGET zlib
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ZLIB_DEST_LIB_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:zlib> ${ZLIB_DEST_LIB_DIR}
    )
endif(MSVC)

if(UNIX)
    # On unix-like platforms the library is almost always called libz
   set_target_properties(zlib PROPERTIES OUTPUT_NAME z)
endif(UNIX)

COTIRE(zlib)