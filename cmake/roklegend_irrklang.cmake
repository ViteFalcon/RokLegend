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
    SET(irrKlang_BINARIES "${irrKlang_BINARY_DIR}/irrKlang.dll" "${irrKlang_BINARY_DIR}/ikpMP3.dll")
    LIST(APPEND ROKLEGEND_BINARY_REL "${irrKlang_BINARIES}")
    LIST(APPEND ROKLEGEND_BINARY_DBG "${irrKlang_BINARIES}")
ENDIF ()

SET(irrKlang_LIBRARY_DIRS "${irrKlang_DIR}/lib/${irrKlang_PLATFORM_DIR}")
SET(irrKlang_LIBRARIES "irrKlang.lib")
