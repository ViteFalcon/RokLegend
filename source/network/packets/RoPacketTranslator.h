#pragma once
#include <core/RoHashMap.h>
#include <core/RoPropertyMap.h>
#include <core/RoSharedPtr.h>

struct RoPacket;
roDEFINE_PTR(RoPacket);

class RoPacketTranslator
{
public:
    static RoPacketTranslator& Get();

public:
    RoPacketTranslator();

    template <typename T>
    size_t add(const RoHashString& packetName)
    {
        static_assert(std::is_default_constructible<T>::value, "Packet type should have default constructor.");
        static_assert(std::is_copy_constructible<T>::value, "Packet should be copy-constructible.");
        static_assert(std::is_base_of<RoPacket, T>::value, "Packet should inherit from RoPacket");

        RoPacketPtr translation = std::make_shared<T>();
        return add(packetName, translation);
    }

    RoPacketPtr translate(const RoHashString& packetName, const RoPropertyMap& properties) const;

private:
    size_t add(const RoHashString& packetName, RoPacketPtr translation);

private:
    using Translations = RoHashMap < RoHashString, RoPacketPtr > ;

    size_t mNextId;
    Translations mTranslations;
};