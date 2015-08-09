#pragma once

#include <core/RoPrerequisites.h>

roFORWARD_DECL_PTR(RoCharacterCreateRequest);

class RoCharacterCreateRequest
{
public:
    roFORWARD_DECL_PTR(Builder);

    static BuilderPtr ForSlot(uint8 slot);

    ~RoCharacterCreateRequest() = default;

    const RoString getName() const
    {
        return mName;
    }

    const uint8 getSlot() const
    {
        return mSlot;
    }

    const uint16 getHairStyle() const
    {
        return mHairStyle;
    }

    const uint16 getHairColor() const
    {
        return mHairColor;
    }
public:
    class Builder
    {
    public:
        Builder() = default;
        virtual ~Builder() = default;

        virtual Builder& withName(const RoString& name) = 0;
        virtual Builder& withHairStyle(const uint16 hairStyle) = 0;
        virtual Builder& withHairColor(const uint16 hairColor) = 0;

        virtual RoCharacterCreateRequestPtr build() = 0;
    };

private:
    friend class RoCreateCharacterRequestBuilder;

    RoCharacterCreateRequest(RoString name, uint8 slot, uint16 hairStyle, uint16 hairColor);

private:
    const RoString mName;
    const uint8 mSlot;
    const uint16 mHairStyle;
    const uint16 mHairColor;
};