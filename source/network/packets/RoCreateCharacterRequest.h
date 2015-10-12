#pragma once
#include "RoPacket.h"

roDEFINE_PACKET(RoCreateCharacterRequest)
{
public:
    RoCreateCharacterRequest();
    ~RoCreateCharacterRequest() = default;

    inline RoString getName() const
    {
        return mName;
    }

    inline void setName(const RoString& name)
    {
        mName = name;
    }

    inline uint8 getSlot() const
    {
        return mSlot;
    }

    inline void setSlot(uint8 slot)
    {
        mSlot = slot;
    }

    inline uint16 getHairColor() const
    {
        return mHairColor;
    }

    inline void setHairColor(uint16 hairColor)
    {
        mHairColor = hairColor;
    }

    inline uint16 getHairStyle() const
    {
        return mHairStyle;
    }

    inline void setHairStyle(uint16 hairStyle)
    {
        mHairStyle = hairStyle;
    }
private:
    RoString mName;
    uint8 mSlot;
    uint16 mHairColor;
    uint16 mHairStyle;
};