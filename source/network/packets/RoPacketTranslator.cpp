#include "RoPacketTranslator.h"
#include "RoPacket.h"
#include <core/RoException.h>
#include <core/RoErrorInfo.h>
#include <core/RoLog.h>

RoPacketTranslator& RoPacketTranslator::Get()
{
    static RoPacketTranslator instance;
    return instance;
}

RoPacketTranslator::RoPacketTranslator()
    : mNextId(0)
{
}

RoPacketPtr RoPacketTranslator::translate(const RoHashString& packetName, const RoPropertyMap& properties) const
{
    std::stringstream propertiesString;
    propertiesString << properties;
    roLOG_INFO << propertiesString.str();
    auto translatorItr = mTranslations.find(packetName);
    if (mTranslations.end() == translatorItr)
    {
        return RoPacketPtr{};
    }
    const RoPacketPtr translator = translatorItr->second;
    try
    {
        return translator->cloneFromProperties(properties);
    }
    catch (std::exception& e)
    {
        roLOG_ERR << "Failed to translate packet '" << packetName << "'. Reason: " << e.what();
    }
    return translator;
}

size_t RoPacketTranslator::add(const RoHashString& packetName, RoPacketPtr translation)
{
    static size_t nextId = 1;
    auto translationItr = mTranslations.find(packetName);
    if (mTranslations.end() != translationItr)
    {
        std::stringstream errorMessage;
        errorMessage
            << "A translator for the packet '"
            << packetName
            << "' already exists and is being translated by class '"
            << translationItr->second->getClassName()
            << "'.";
        roTHROW(RoDuplicateItemFound() << RoErrorInfoDetail(errorMessage.str()));
    }
    mTranslations[packetName] = translation;
    return nextId++;
}

