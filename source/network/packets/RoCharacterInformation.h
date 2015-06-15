#pragma once
#include "RoPacket.h"

roDEFINE_PACKET(RoCharacterInformation)
{
public:
    RoCharacterInformation();

    uint16 getSlot() const
    {
        return (uint16) mSlot;
    }

    uint32 getId() const
    {
        return mId;
    }

    RoString getName() const
    {
        return mName;
    }

    uint16 getBaseLevel() const
    {
        return mBaseLevel;
    }

    uint32 getBaseExperience() const
    {
        return mBaseExperience;
    }

    uint16 getJobId() const
    {
        return mJobId;
    }

    uint32 getJobLevel() const
    {
        return mJobLevel;
    }

    uint32 getJobExperience() const
    {
        return mJobExperience;
    }

    uint32 getHealth() const
    {
        return mHealth;
    }

    uint32 getMaxHealth() const
    {
        return mMaxHealth;
    }

    float getHealthPercent() const
    {
        return float(getHealth()) / float(getMaxHealth());
    }

    uint32 getMana() const
    {
        return mMana;
    }

    uint32 getMaxMana() const
    {
        return mMaxMana;
    }

    float getManaPercent() const
    {
        return float(getMana()) / float(getMaxMana());
    }

    uint16 getWalkSpeed() const
    {
        return mWalkSpeed;
    }

    uint32 getBodyState() const
    {
        return mBodyState;
    }

    uint32 getHealthState() const
    {
        return mHealthState;
    }

    uint32 getEffectState() const
    {
        return mEffectState;
    }

    uint16 getStatusPoints() const
    {
        return mStatusPoints;
    }

    uint16 getSkillPoints() const
    {
        return mSkillPoints;
    }

    uint32 getMoney() const
    {
        return mMoney;
    }

    uint32 getKarma() const
    {
        return mKarma;
    }

    uint32 getManner() const
    {
        return mManner;
    }

    uint16 getHair() const
    {
        return mHair;
    }

    uint16 getHairColor() const
    {
        return mHairColor;
    }

    uint16 getHeadTop() const
    {
        return mHeadTop;
    }

    uint16 getHeadMid() const
    {
        return mHeadMid;
    }

    uint16 getHeadBottom() const
    {
        return mHeadBottom;
    }

    uint16 getWeapon() const
    {
        return mWeapon;
    }

    uint16 getShield() const
    {
        return mShield;
    }

    uint16 getClothColor() const
    {
        return mClothColor;
    }

    uint16 getRobe() const
    {
        return mRobe;
    }

    uint8 getStrength() const
    {
        return mStrength;
    }

    uint8 getAgility() const
    {
        return mAgility;
    }

    uint8 getVitality() const
    {
        return mVitality;
    }

    uint8 getIntelligence() const
    {
        return mIntelligence;
    }

    uint8 getDexterity() const
    {
        return mDexterity;
    }

    uint8 getLuck() const
    {
        return mLuck;
    }

    bool isRenamingPossible() const
    {
        return mCanRename;
    }

    RoOptionalString getMapName() const
    {
        return mMapName;
    }

    RoOptionalUInt32 getDeleteDate() const
    {
        return mDeleteDate;
    }
private:
    uint8 mSlot;
    uint32 mId;
    RoString mName;
    uint16 mBaseLevel;
    uint32 mBaseExperience;
    uint16 mJobId;
    uint32 mJobLevel;
    uint32 mJobExperience;
    uint32 mHealth;
    uint32 mMaxHealth;
    uint32 mMana;
    uint32 mMaxMana;
    uint16 mWalkSpeed;
    uint32 mBodyState;
    uint32 mHealthState;
    uint32 mEffectState;
    uint16 mStatusPoints;
    uint16 mSkillPoints;
    uint32 mMoney;
    uint32 mKarma;
    uint32 mManner;
    uint16 mHair;
    uint16 mHairColor;
    uint16 mHeadTop;
    uint16 mHeadMid;
    uint16 mHeadBottom;
    uint16 mWeapon;
    uint16 mShield;
    uint16 mClothColor;
    uint16 mRobe;
    uint8 mStrength;
    uint8 mAgility;
    uint8 mVitality;
    uint8 mIntelligence;
    uint8 mDexterity;
    uint8 mLuck;
    bool mCanRename;
    RoOptionalString mMapName;
    RoOptionalUInt32 mDeleteDate;
};