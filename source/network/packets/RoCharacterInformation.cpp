#include "RoCharacterInformation.h"

roREGISTER_SUB_PACKET(RoCharacterInformation);

RoCharacterInformation::RoCharacterInformation()
{
    add<uint8>("slot", &RoCharacterInformation::mSlot);
    add<uint32>("character_id", &RoCharacterInformation::mId);
    add<RoString>("name", &RoCharacterInformation::mName);
    add<uint16>("base_level", &RoCharacterInformation::mBaseLevel);
    add<uint32>("base_xp", &RoCharacterInformation::mBaseExperience);
    add<uint16>("job_id", &RoCharacterInformation::mJobId);
    add<uint32>("job_lvl", &RoCharacterInformation::mJobLevel);
    add<uint32>("job_xp", &RoCharacterInformation::mJobExperience);
    add<uint32>("hp", &RoCharacterInformation::mHealth);
    add<uint32>("maxhp", &RoCharacterInformation::mMaxHealth);
    add<uint32>("sp", &RoCharacterInformation::mMana);
    add<uint32>("maxsp", &RoCharacterInformation::mMaxMana);
    add<uint16>("walk_speed", &RoCharacterInformation::mWalkSpeed);
    add<uint32>("body_state", &RoCharacterInformation::mBodyState);
    add<uint32>("health_state", &RoCharacterInformation::mHealthState);
    add<uint32>("effect_state", &RoCharacterInformation::mEffectState);
    add<uint16>("status_point", &RoCharacterInformation::mStatusPoints);
    add<uint16>("skill_points", &RoCharacterInformation::mSkillPoints);
    add<uint32>("zeny", &RoCharacterInformation::mMoney);
    add<uint32>("karma", &RoCharacterInformation::mKarma);
    add<uint32>("manner", &RoCharacterInformation::mManner);
    add<uint16>("hair", &RoCharacterInformation::mHair);
    add<uint16>("hair_color", &RoCharacterInformation::mHairColor);
    add<uint16>("head_top", &RoCharacterInformation::mHeadTop);
    add<uint16>("head_mid", &RoCharacterInformation::mHeadMid);
    add<uint16>("head_bottom", &RoCharacterInformation::mHeadBottom);
    add<uint16>("weapon", &RoCharacterInformation::mWeapon);
    add<uint16>("shield", &RoCharacterInformation::mShield);
    add<uint16>("cloth_color", &RoCharacterInformation::mClothColor);
    add<uint16>("robe", &RoCharacterInformation::mRobe);
    add<uint8>("str", &RoCharacterInformation::mStrength);
    add<uint8>("agi", &RoCharacterInformation::mAgility);
    add<uint8>("vit", &RoCharacterInformation::mVitality);
    add<uint8>("int", &RoCharacterInformation::mIntelligence);
    add<uint8>("dex", &RoCharacterInformation::mDexterity);
    add<uint8>("luk", &RoCharacterInformation::mLuck);
    add<bool>("can_rename", &RoCharacterInformation::mCanRename);
    addOptional<RoString>("map_name", &RoCharacterInformation::mMapName);
    addOptional<uint32>("delete_date", &RoCharacterInformation::mDeleteDate);
}
