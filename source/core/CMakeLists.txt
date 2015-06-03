# Sources
SET(RO_TASK_SOURCES
    task/RoTask.h
	task/RoTaskArgs.cpp
    task/RoTaskArgs.h
    task/RoTaskCollection.cpp
    task/RoTaskCollection.h
    task/RoTaskManager.cpp
    task/RoTaskManager.h
)
RO_SOURCE_GROUP(task FILES ${RO_TASK_SOURCES})

SET(RO_MESSAGE_QUEUE_SOURCES
	message_queue/RoMessage.cpp
	message_queue/RoMessage.h
	message_queue/RoMessageQueue.cpp
	message_queue/RoMessageQueue.h
)
RO_SOURCE_GROUP(message_queue FILES ${RO_MESSAGE_QUEUE_SOURCES})

SET(RO_CORE_SOURCES
    CMakeLists.txt
	RoAllocator.h
	RoConfig.cpp
	RoConfig.h
	RoCpuInfo.cpp
	RoCpuInfo.h
	RoDataStream.h
	RoDefines.h
	RoDict.cpp
	RoDict.h
	RoErrorInfo.h
	RoException.cpp
	RoException.h
	RoFileSystem.cpp
	RoFileSystem.h
	RoGlobPattern.cpp
	RoGlobPattern.h
	RoHashMap.h
	RoHashSet.h
	RoHashString.cpp
	RoHashString.h
	RoIndexedMap.h
	RoLog.cpp
	RoLog.h
	RoLogOptions.cpp
	RoLogOptions.h
	RoMemoryDataStream.cpp
	RoMemoryDataStream.h
	RoPlatform.cpp
	RoPlatform.h
	RoPrerequisites.h
	RoProperty.h
	RoPropertyMap.cpp
	RoPropertyMap.h
	RoScopedPtr.h
	RoSerialization.h
	RoSharedPtr.h
	RoString.cpp
	RoString.h
	RoStringBuilder.cpp
	RoStringBuilder.h
	RoStringUtil.cpp
	RoStringUtil.h
	RoTimer.cpp
	RoTimer.h
	RoTypes.h
	RoUtilities.cpp
	RoUtilities.h
	RoVariant.cpp
	RoVariant.h
	RoVector.h
	RoZipIterator.h
    ${RO_TASK_SOURCES}
	${RO_MESSAGE_QUEUE_SOURCES}
)

ADD_LIBRARY(${RO_CORE_LIBRARY} STATIC ${RO_CORE_SOURCES})