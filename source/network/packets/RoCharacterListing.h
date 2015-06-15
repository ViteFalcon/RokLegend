#pragma once
#include "RoPacket.h"
#include "RoCharacterInformationCollection.h"
#include "RoCharacterInformation.h"

roDEFINE_PACKET(RoCharacterListing)
{
public:
    RoCharacterListing();

    RoOptionalUInt8 getMaxSlots() const
    {
        return mMaxSlots;
    }

    RoOptionalUInt8 getAvailableSlots() const
    {
        return mAvailableSlots;
    }

    RoOptionalUInt8 getPremiumSlots() const
    {
        return mPremiumSlots;
    }

    RoCharacterInformationCollection getCharacters() const
    {
        return mCharacters;
    }
private:
    RoOptionalUInt8 mMaxSlots;
    RoOptionalUInt8 mAvailableSlots;
    RoOptionalUInt8 mPremiumSlots;
    RoCharacterInformationCollection mCharacters;
};
