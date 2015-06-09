# Sources
SET(INFECTORPP_SOURCES
	infectorpp/Infector_details.hpp
	infectorpp/Infector_multi.hpp
	infectorpp/Infector_private.hpp
	infectorpp/Infector_single.hpp
	infectorpp/InfectorContainer.hpp
	infectorpp/InfectorExceptions.hpp
	infectorpp/InfectorExport.hpp
	infectorpp/InfectorHelpers.hpp
)
RO_SOURCE_GROUP(infectorpp ${INFECTORPP_SOURCES})

SET(ROKLEGEND_SOURCES
    CMakeLists.txt
    RokLegend.cpp
)

#IF (WIN32)
#    SET(APP_TYPE WIN32)
#ENDIF ()

ADD_EXECUTABLE(${ROKLEGEND_PROJECT_NAME} ${APP_TYPE} ${ROKLEGEND_SOURCES} ${INFECTORPP_SOURCES})
RO_TARGET_LINK_LIBRARIES(${ROKLEGEND_PROJECT_NAME}
    ${RO_CORE_LIBRARY}
    ${RO_MATH_LIBRARY}
    ${RO_AUDIO_LIBRARY}
    ${RO_ARCHIVE_LIBRARY}
	${RO_STORAGE_LIBRARY}
	${RO_XML_LIBRARY}
	${RO_NETWORK_LIBRARY}
)
TARGET_LINK_LIBRARIES(${ROKLEGEND_PROJECT_NAME}
    ${irrKlang_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${GRF_LIBRARIES}
    ${TBB_LIBRARIES}
	${SQLITEPP_LIBRARIES}
)