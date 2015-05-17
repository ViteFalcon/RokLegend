#--------------------------------------------------------------------------------
#  irrKlang settings
#--------------------------------------------------------------------------------
SET(irrKlang_DIR "${ROKLEGEND_DEPENDENCIES_DIR}/irrKlang")
SET(irrKlang_INCLUDE_DIRS "${irrKlang_DIR}/include")
IF (MSVC)
    IF (CMAKE_CL_64)
        SET(irrKlang_PLATFORM_DIR "winx64-visualStudio")
    ELSE ()
        SET(irrKlang_PLATFORM_DIR "winx32-visualStudio")
    ENDIF ()
ENDIF ()
SET(irrKlang_BINARY_DIR "${irrKlang_DIR}/bin/${irrKlang_PLATFORM_DIR}")
IF (WIN32)
    LIST(APPEND ROKLEGEND_BINARY_REL "${irrKlang_BINARY_DIR}/irrKlang.dll")
    LIST(APPEND ROKLEGEND_BINARY_DBG "${ROKLEGEND_BINARY_REL}")
ENDIF ()

SET(irrKlang_LIBRARY_DIRS "${irrKlang_DIR}/lib/${irrKlang_PLATFORM_DIR}")
SET(irrKlang_LIBRARIES "irrKlang.lib")

# FIXME: This needs to be changed. We shouldn't have absolute paths in our code.
GET_FILENAME_COMPONENT(irrKlang_PLUGINS_DIR_REL "${ROKLEGEND_BUILD_DIR}/External/irrKlang" REALPATH)
SET(irrKlang_PLUGINS_DIR_DBG "${irrKlang_PLUGINS_DIR_REL}/Debug")
SET(irrKlang_PLUGINS_BINARY_REL "${irrKlang_BINARY_DIR}/ikpMP3.dll")
SET(irrKlang_PLUGINS_BINARY_DBG "${irrKlang_PLUGINS_BINARY_REL}")

IF (NOT "" STREQUAL "${irrKlang_PLUGINS_BINARY_REL}")
    FILE(COPY ${irrKlang_PLUGINS_BINARY_REL} DESTINATION ${irrKlang_PLUGINS_DIR_REL})
ENDIF ()

IF (NOT "" STREQUAL "${irrKlang_PLUGINS_BINARY_DBG}")
    FILE(COPY ${irrKlang_PLUGINS_BINARY_DBG} DESTINATION ${irrKlang_PLUGINS_DIR_DBG})
ENDIF ()
