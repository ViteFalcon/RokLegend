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

SET(ROKLEGEND_AUDIO_SOURCES
	audio/RoBackgroundScore.cpp
	audio/RoBackgroundScore.h
	audio/RoButtonSound.cpp
	audio/RoButtonSound.h
)
RO_SOURCE_GROUP(audio ${ROKLEGEND_AUDIO_SOURCES})

SET(ROKLEGEND_GAMESTATE_SOURCES
	gamestates/RoGameState.cpp
	gamestates/RoGameState.h
	gamestates/RoLoginState.cpp
	gamestates/RoLoginState.h
)
RO_SOURCE_GROUP(gamestates ${ROKLEGEND_GAMESTATE_SOURCES})

SET(ROKLEGEND_SOURCES
    CMakeLists.txt
	RoGame.h
	RoGameBindings.cpp
	RoGameBindings.h
    RokLegend.cpp
)

INCLUDE_DIRECTORIES(
	${ENTITYX_INCLUDE_DIR}
	${irrKlang_INCLUDE_DIRS}
)

#IF (WIN32)
#    SET(APP_TYPE WIN32)
#ENDIF ()

ADD_EXECUTABLE(${ROKLEGEND_PROJECT_NAME} ${APP_TYPE}
	${ROKLEGEND_SOURCES}
	${ROKLEGEND_AUDIO_SOURCES}
	${ROKLEGEND_GAMESTATE_SOURCES}
	${INFECTORPP_SOURCES}
)
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
	${ENTITYX_LIBRARIES}
)