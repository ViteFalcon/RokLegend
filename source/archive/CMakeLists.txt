INCLUDE_DIRECTORIES(
    ${GRF_INCLUDE_DIRS}
	${LIBGRF_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
)

SET(RO_ARCHIVE_GRF2_IMPL_SOURCES
	grf/RoGrf2Impl.cpp
	grf/RoGrf2Impl.h
)
RO_SOURCE_GROUP(grf ${RO_ARCHIVE_GRF2_IMPL_SOURCES})

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

# Setting the following to 1/0 will enable/disable concurrent caching of filenames when loading GRF.
# Vite Falcon: On a Core i5, I saw consistent timings with the non-concurrent version.
ADD_DEFINITIONS(-DroGRF_USE_CONCURRENT_LOADING=0)

ADD_LIBRARY(${RO_ARCHIVE_LIBRARY} STATIC
	${RO_ARCHIVE_SOURCES}
	${RO_ARCHIVE_GRF2_IMPL_SOURCES}
)