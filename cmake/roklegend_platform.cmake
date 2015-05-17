#-------------------------------------------------------------------------------
# Use relative paths
# This is mostly to reduce path size for command-line limits on windows
#-------------------------------------------------------------------------------
IF (WIN32)
    # This seems to break Xcode projects so definitely don't enable on Apple builds
    SET(CMAKE_USE_RELATIVE_PATHS true)
    SET(CMAKE_SUPPRESS_REGENERATION false)
    SET(ROKLEGEND_SHAREDEXT ".dll")
ELSE ()
    SET(ROKLEGEND_SHAREDEXT ".so")
ENDIF ()

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