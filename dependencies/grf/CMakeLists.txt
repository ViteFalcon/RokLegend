ADD_DEFINITIONS("-DGRF_STATIC")

SET(GRF_SOURCE_FILES
    CMakeLists.txt
    includes/grf.h
    includes/hash_tables.h
    includes/libgrf.h
    src/euc_kr.c
    src/grf.c
    src/hash_tables.c
    src/main.c
    src/unicode_table_uhc.h
    src/zlib.c
)

INCLUDE_DIRECTORIES(includes ${ZLIB_INCLUDE_DIRS})

ADD_LIBRARY(${GRF_PROJECT} STATIC ${GRF_SOURCE_FILES})