# Requires TBB_ROOT to exist that points to the root directory of a TBB SDK installation
# Article of interest: http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.71.8289
SET(_TBB_INCLUDE_HINT_FILE "tbb/task_scheduler_init.h")
SET(_TBB_LIB_NAME "tbb")
SET(_TBB_DEBUG_LIB_NAME "tbb_debug")
SET(_TBB_MALLOC_LIB_NAME "tbbmalloc")
SET(_TBB_MALLOC_DEBUG_LIB_NAME "tbbmalloc_debug")
IF (WIN32)
    SET(TBB_ROOT "$ENV{TBB_ROOT}")
    IF ("${TBB_ROOT}" STREQUAL "")
        MESSAGE(FATAL_ERROR "Failed to find environment variable 'TBB_ROOT'.")
    ELSEIF (NOT EXISTS "${TBB_ROOT}/")
        MESSAGE(FATAL_ERROR "${TBB_ROOT} is not a directory that exists!")
    ELSEIF (NOT EXISTS "${TBB_ROOT}/include/${_TBB_INCLUDE_HINT_FILE}")
        MESSAGE(FATAL_ERROR "${TBB_ROOT} is pointing to an invalid TBB SDK location.")
    ENDIF ()
    SET(_TBB_BIN_DIR ${TBB_ROOT}/bin)
    SET(CMAKE_FIND_LIBRARY_PREFIXES "")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
ELSEIF (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lrt")
ENDIF ()

# Cross-platform concerns:
# Variables to be set:
#   _TBB_ARCH: Architecture of TBB
#   _TBB_LIB_DIR: Directory to which we'll be able to find TBB library
#   _TBB_BIN_DIR: DIrectory where we could find the shared object for TBB library (DLL's in Windows).
#   _TBB_LIB: Lib for tbb (core)
#   _TBB_DEBUG_LIB: Debug lib for tbb (core)
#   _TBB_MALLOC_LIB: Lib for tbb malloc
#   _TBB_MALLOC_DEBUG_LIB: Debug lib for tbb malloc

# We need MSVC >= MSVC12
IF (MSVC)
    IF (${RO_IS_64BIT})
        SET(_TBB_ARCH "intel64")
    ELSE ()
        SET(_TBB_ARCH "ia32")
    ENDIF ()

    IF (MSVC12)
        SET(_TBB_MSVC_LIB_DIR "vc12")
    ELSEIF (MSVC14)
        SET(_TBB_MSVC_LIB_DIR "vc14")
    ELSE ()
        MESSAGE(FATAL_ERROR "Unsupported Visual Studio version: '${ROKLEGEND_MSVC}'!")
    ENDIF ()

    SET(_TBB_LIB_DIR "${TBB_ROOT}/lib/${_TBB_ARCH}/${_TBB_MSVC_LIB_DIR}")
    SET(_TBB_BIN_DIR "${_TBB_BIN_DIR}/${_TBB_ARCH}/${_TBB_MSVC_LIB_DIR}")
ENDIF ()

IF ("${TBB_INCLUDE_DIRS}" STREQUAL "" OR "${TBB_INCLUDE_DIRS}" STREQUAL "TBB_INCLUDE_DIRS-NOTFOUND")
    FIND_PATH(TBB_INCLUDE_DIRS "${_TBB_INCLUDE_HINT_FILE}" PATHS ${TBB_ROOT}/include ENV CPATH)
    MARK_AS_ADVANCED(TBB_INCLUDE_DIRS)
ENDIF ()

FIND_LIBRARY(_TBB_LIBRARY ${_TBB_LIB_NAME} HINTS ${_TBB_LIB_DIR})
FIND_LIBRARY(_TBB_DEBUG_LIBRARY ${_TBB_DEBUG_LIB_NAME} HINTS ${_TBB_LIB_DIR})
FIND_LIBRARY(_TBB_MALLOC_LIBRARY ${_TBB_MALLOC_LIB_NAME} HINTS ${_TBB_LIB_DIR})
FIND_LIBRARY(_TBB_MALLOC_DEBUG_LIBRARY ${_TBB_MALLOC_DEBUG_LIB_NAME} HINTS ${_TBB_LIB_DIR})
IF ("${_TBB_LIBRARY}" STREQUAL "_TBB_LIBRARY-NOTFOUND")
    MESSAGE(FATAL_ERROR "Failed to locate TBB library directory, when trying to find library named '${_TBB_LIB_NAME}'.")
ENDIF ()
GET_FILENAME_COMPONENT(TBB_LIBRARY_DIRS ${_TBB_LIBRARY} PATH)
MARK_AS_ADVANCED(TBB_LIBRARY_DIRS)

FIND_LIBRARY(TBB_BINARY ${_TBB_LIB_NAME} HINTS ${_TBB_BIN_DIR})
IF ("${TBB_BINARY}" STREQUAL "TBB_BINARY-NOTFOUND")
    MESSAGE(FATAL_ERROR "Failed to find TBB binary file.")
ENDIF ()
FIND_LIBRARY(TBB_DEBUG_BINARY ${_TBB_DEBUG_LIB_NAME} HINTS ${_TBB_BIN_DIR})
IF ("${TBB_DEBUG_BINARY}" STREQUAL "TBB_DEBUG_BINARY-NOTFOUND")
    MESSAGE(FATAL_ERROR "Failed to find TBB (debug) binary file.")
ENDIF ()

FIND_LIBRARY(TBB_MALLOC_BINARY ${_TBB_MALLOC_LIB_NAME} HINTS ${_TBB_BIN_DIR})
IF ("${TBB_MALLOC_BINARY}" STREQUAL "TBB_MALLOC_BINARY-NOTFOUND")
    MESSAGE(FATAL_ERROR "Failed to find TBB malloc binary file.")
ENDIF ()
FIND_LIBRARY(TBB_MALLOC_DEBUG_BINARY ${_TBB_MALLOC_DEBUG_LIB_NAME} HINTS ${_TBB_BIN_DIR})
IF ("${TBB_MALLOC_DEBUG_BINARY}" STREQUAL "TBB_MALLOC_DEBUG_BINARY-NOTFOUND")
    MESSAGE(FATAL_ERROR "Failed to find TBB malloc (debug) binary file.")
ENDIF ()

SET(TBB_LIBRARIES debug ${_TBB_DEBUG_LIBRARY} optimized ${_TBB_LIBRARY})
SET(TBB_MALLOC_LIBRARIES debug ${_TBB_MALLOC_DEBUG_LIBRARY} optimized ${_TBB_MALLOC_LIBRARY})

LIST(APPEND ROKLEGEND_BINARY_DBG
    ${TBB_DEBUG_BINARY}
    ${TBB_MALLOC_DEBUG_BINARY}
)
LIST(APPEND ROKLEGEND_BINARY_REL
    ${TBB_BINARY}
    ${TBB_MALLOC_BINARY}
)