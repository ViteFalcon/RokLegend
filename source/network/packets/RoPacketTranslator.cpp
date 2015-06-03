#include "RoPacketTranslator.h"
#include "RoPacket.h"
#include <core/RoException.h>
#include <core/RoErrorInfo.h>

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
    auto translatorItr = mTranslations.find(packetName);
    if (mTranslations.end() == translatorItr)
    {
        return RoPacketPtr{};
    }
    const RoPacketPtr translator = translatorItr->second;
    return translator->cloneFromProperties(properties);
}

size_t RoPacketTranslator::add(const RoHashString& packetName, RoPacketPtr translation)
{
    static size_t nextId = 1;
    auto translationItr = mTranslations.find(packetName);
    if (mTranslations.end() == translationItr)
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

