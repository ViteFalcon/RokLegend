#include "RoCreateCharacterRequest.h"

#include <core/RoException.h>

class RoCreateCharacterRequestBuilder : public RoCharacterCreateRequest::Builder
{
public:
    RoCreateCharacterRequestBuilder(const uint8 slot)
        : mSlot(slot)
    {
    }

    virtual Builder& withName(const RoString& name) override
    {
        mName = name;
        return *this;
    }

    virtual Builder& withHairStyle(const uint16 hairStyle) override
    {
        mHairStyle = hairStyle;
        return *this;
    }

    virtual Builder& withHairColor(const uint16 hairColor) override
    {
        mHairColor = hairColor;
        return *this;
    }

    virtual RoCharacterCreateRequestPtr build() override
    {
        roTHROW_IF(!mName.is_initialized(), RoInvalidOperation() << RoErrorInfoDetail("Name of the character was not set."));
        roTHROW_IF(!mHairStyle.is_initialized(), RoInvalidOperation() << RoErrorInfoDetail("Hair style of the character was not set"));
        roTHROW_IF(!mHairColor.is_initialized(), RoInvalidOperation() << RoErrorInfoDetail("Hair color of the character was not set"));
        return RoCharacterCreateRequestPtr{
            new RoCharacterCreateRequest{
                mName.get(),
                mSlot,
                mHairStyle.get(),
                mHairColor.get()
            } 
        };
    }

private:
    const uint8 mSlot;
    optional<RoString> mName;
    optional<uint16> mHairStyle;
    optional<uint16> mHairColor;
};

RoCharacterCreateRequest::BuilderPtr RoCharacterCreateRequest::ForSlot(uint8 slot)
{
    return std::make_shared<RoCreateCharacterRequestBuilder>(slot);
}

inline RoCharacterCreateRequest::RoCharacterCreateRequest(RoString name, uint8 slot, uint16 hairStyle, uint16 hairColor)
    : mName(name)
    , mSlot(slot)
    , mHairStyle(hairStyle)
    , mHairColor(hairColor)
{
}
