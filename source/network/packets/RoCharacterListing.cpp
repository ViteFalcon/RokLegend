#include "RoCharacterListing.h"

roREGISTER_PACKET("CharactersReceived", RoCharacterListing);

RoCharacterListing::RoCharacterListing()
    : mMaxSlots{ 9 }
    , mAvailableSlots{ 9 }
    , mPremiumSlots{ 0 }
{
    addOptional<uint8>("max_slots", &RoCharacterListing::mMaxSlots);
    addOptional<uint8>("available_slots", &RoCharacterListing::mAvailableSlots);
    addOptional<uint8>("premium_slots", &RoCharacterListing::mPremiumSlots);
    addList<RoCharacterInformation>("characters", &RoCharacterListing::mCharacters);
}

