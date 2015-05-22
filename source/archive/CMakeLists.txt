INCLUDE_DIRECTORIES(
    ${GRF_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
)

SET(RO_ARCHIVE_SOURCES
    CMakeLists.txt
    RoCacheStore.cpp
    RoCacheStore.h
    RoDataFolder.cpp
    RoDataFolder.h
    RoGrf.cpp
    RoGrf.h
    RoGrfErrorInfo.h
    RoGrfFactory.cpp
    RoGrfFactory.h
    RoGrfFile.cpp
    RoGrfFile.h
    RoGrfFileStats.cpp
    RoGrfFileStats.h
    RoGrfHashMapBackend.cpp
    RoGrfHashMapBackend.h
    RoGrfHeader.cpp
    RoGrfHeader.h
    RoGrfSqliteBackend.cpp
    RoGrfSqliteBackend.h
    RoGrfStorageBackend.cpp
    RoGrfStorageBackend.h
    RoGrfVersion1.cpp
    RoGrfVersion1.h
    RoGrfVersion2.cpp
    RoGrfVersion2.h
    RoZlib.cpp
    RoZlib.h
)

ADD_LIBRARY(${RO_ARCHIVE_LIBRARY} STATIC ${RO_ARCHIVE_SOURCES})