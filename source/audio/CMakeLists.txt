ADD_DEFINITIONS("-DroIRRKLANG_PLUGIN_DIR_DBG=\"${irrKlang_PLUGINS_DIR_DBG}\"")
ADD_DEFINITIONS("-DroIRRKLANG_PLUGIN_DIR_REL=\"${irrKlang_PLUGINS_DIR_REL}\"")
INCLUDE_DIRECTORIES(
    ${irrKlang_INCLUDE_DIRS}
)

SET(RO_AUDIO_IRRKLANG_SOURCE_FILES
    irrklang/RoIrrKlangAudio.cpp
    irrklang/RoIrrKlangAudio.h
    irrklang/RoIrrKlangAudioManager.cpp
    irrklang/RoIrrKlangAudioManager.h
    irrklang/RoNullIrrKlangSound.cpp
    irrklang/RoNullIrrKlangSound.h
)
RO_SOURCE_GROUP(irrKlang FILES ${RO_AUDIO_IRRKLANG_SOURCE_FILES})

SET(RO_AUDIO_SOURCES
    CMakeLists.txt
    RoAudio.h
    RoAudioManager.cpp
    RoAudioManager.h
    ${RO_AUDIO_IRRKLANG_SOURCE_FILES}
)

ADD_LIBRARY(${RO_AUDIO_LIBRARY} STATIC ${RO_AUDIO_SOURCES})