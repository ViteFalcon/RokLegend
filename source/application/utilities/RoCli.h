#pragma once

#include <core/RoPrerequisites.h>
#include <core/RoHashSet.h>

roFORWARD_DECL_PTR(RoButtonSound);

class RoCli
{
public:
    static const RoHashSet<char> INTEGER_CHARS;
    static const RoHashSet<char> TEXT_SPECIAL_CHARS;

    static bool HasKeyboardHit();

    static char GetCharacter();

    static optional<char> GetCharacterIfEntered();

    static bool IsEscapeKey(const char key);

    static RoOptionalUtf8 ReadString(std::string message, bool isPassword, optional<RoButtonSoundPtr> buttonSound = optional<RoButtonSoundPtr>{});

    static RoOptionalUInt32 ReadInteger(std::string message, bool isPassword, optional<RoButtonSoundPtr> buttonSound = optional<RoButtonSoundPtr>{});

    static RoOptionalUtf8 ReadRaw(std::string message, bool isPassword, const RoHashSet<char> allowedChars, optional<RoButtonSoundPtr> buttonSound = optional<RoButtonSoundPtr>{});
};