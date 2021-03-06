SET(RO_CORE_LIBRARY "RoCore")
SET(RO_MATH_LIBRARY "RoMath")
SET(RO_AUDIO_LIBRARY "RoAudio")
SET(RO_ARCHIVE_LIBRARY "RoArchive")
SET(RO_STORAGE_LIBRARY "RoStorage")
SET(RO_XML_LIBRARY "RoXml")
SET(RO_NETWORK_LIBRARY "RoNetwork")

ADD_SUBDIRECTORY(core)
SET(RO_CORE_DIR ${ROKLEGEND_SOURCE_DIR}/core)
RO_TARGET_ENABLE_AVX2(${RO_CORE_LIBRARY})

ADD_SUBDIRECTORY(math)
SET(RO_MATH_DIR ${ROKLEGEND_SOURCE_DIR}/math)
RO_TARGET_ENABLE_AVX2(${RO_MATH_LIBRARY})

ADD_SUBDIRECTORY(archive)
ADD_DEPENDENCIES(${RO_ARCHIVE_LIBRARY}
	${RO_CORE_LIBRARY}
    ${GRF_PROJECT}
    ${ZLIB_LIBRARY_NAME}
)
RO_TARGET_ENABLE_AVX2(${RO_ARCHIVE_LIBRARY})

ADD_SUBDIRECTORY(storage)
ADD_DEPENDENCIES(${RO_STORAGE_LIBRARY}
	${RO_CORE_LIBRARY}
	${SQLITEPP_LIBRARY_NAME}
)
RO_TARGET_ENABLE_AVX2(${RO_STORAGE_LIBRARY})

ADD_SUBDIRECTORY(audio)
ADD_DEPENDENCIES(${RO_AUDIO_LIBRARY}
    ${RO_CORE_LIBRARY}
    ${RO_AUDIO_LIBRARY}
)
RO_TARGET_ENABLE_AVX2(${RO_AUDIO_LIBRARY})

ADD_SUBDIRECTORY(xml)
ADD_DEPENDENCIES(${RO_XML_LIBRARY}
	${RO_CORE_LIBRARY}
)
RO_TARGET_ENABLE_AVX2(${RO_XML_LIBRARY})

ADD_SUBDIRECTORY(network)
ADD_DEPENDENCIES(${RO_NETWORK_LIBRARY}
	${RO_XML_LIBRARY}
)
RO_TARGET_ENABLE_AVX2(${RO_NETWORK_LIBRARY})

ADD_SUBDIRECTORY(application)
ADD_DEPENDENCIES(${ROKLEGEND_PROJECT_NAME}
    ${RO_AUDIO_LIBRARY}
    ${RO_ARCHIVE_LIBRARY}
	${RO_CORE_LIBRARY}
	${RO_MATH_LIBRARY}
	${RO_NETWORK_LIBRARY}
	${RO_STORAGE_LIBRARY}
	${RO_XML_LIBRARY}
)
RO_TARGET_ENABLE_AVX2(${ROKLEGEND_PROJECT_NAME})