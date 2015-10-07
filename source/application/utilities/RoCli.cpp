#include "RoCli.h"
#include "../audio/RoButtonSound.h"

#include <cstdlib>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#if roCOMPILER_IS_MSVC
#   include <conio.h>
#   define roKBHIT _kbhit
#   define roGETCH _getch
#endif // roCOMPILER_IS_MSVC

#define roKEY_ESC 27
#define roKEY_RETURN 13
#define roKEY_BACKSPACE 8

const RoHashSet<char> RoCli::INTEGER_CHARS = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const RoHashSet<char> RoCli::TEXT_SPECIAL_CHARS = { '.', ',', '!', '@', '$', '%', '^', '&', '*', '+', '-', '_', '=' };

bool RoCli::HasKeyboardHit()
{
    return roKBHIT() != 0;
}

char RoCli::GetCharacter()
{
    return roGETCH();
}

optional<char> RoCli::GetCharacterIfEntered()
{
    if (!HasKeyboardHit())
    {
        return optional<char>{};
    }
    return optional<char>{GetCharacter()};
}

bool RoCli::IsEscapeKey(const char key)
{
    return key == roKEY_ESC;
}

RoOptionalUtf8 RoCli::ReadString(std::string message, bool isPassword, optional<RoButtonSoundPtr> buttonSound)
{
    return ReadRaw(message, isPassword, TEXT_SPECIAL_CHARS, buttonSound);
}

RoOptionalUInt32 RoCli::ReadInteger(std::string message, bool isPassword, optional<RoButtonSoundPtr> buttonSound)
{
    RoOptionalUtf8 rawInput = ReadRaw(message, isPassword, INTEGER_CHARS, buttonSound);
    if (rawInput.is_initialized())
    {
        char* endChar;
        const std::string inputString = rawInput->asUTF8();
        uint32 result = strtol(&inputString[0], &endChar, 0);
        if (*endChar == 0)
        {
            return RoOptionalUInt32{ result };
        }
    }
    return RoOptionalUInt32{};
}

RoOptionalUtf8 RoCli::ReadRaw(std::string message, bool isPassword, const RoHashSet<char> allowedChars, optional<RoButtonSoundPtr> buttonSound)
{
    RoVector<char> inputChars;
    std::cout << message;
    char ch = 0;
    auto masked_chars = [](const char ch) {
        std::cout << "*";
    };
    auto echo_chars = [](const char ch) {
        std::cout << ch;
    };
    std::function<void(const char)> echo_func = echo_chars;
    if (isPassword)
    {
        echo_func = masked_chars;
    }
    do
    {
        ch = GetCharacter();
        if (isalnum(ch) || allowedChars.empty() || allowedChars.count(ch))
        {
            inputChars.push_back(ch);
            echo_func(ch);
        }
        else if (ch == roKEY_ESC)
        {
            std::cout << std::endl;
            return RoOptionalUtf8{};
        }
        else if (ch == roKEY_BACKSPACE && !inputChars.empty())
        {
            size_t clearCharCount = message.length() + inputChars.size();
            std::cout << "\r";
            for (size_t i = 0; i < clearCharCount; ++i)
            {
                std::cout << ' ';
            }
            inputChars.pop_back();
            std::cout << "\r" << message;
            std::for_each(inputChars.begin(), inputChars.end(), echo_func);
        }
    } while (ch != roKEY_RETURN);
    std::cout << std::endl;
    if (inputChars.empty())
    {
        return RoOptionalUtf8{};
    }
    inputChars.push_back(0);
    std::string inputString(inputChars.begin(), inputChars.end());
    if (buttonSound)
    {
        buttonSound.get()->play();
    }
    return RoOptionalUtf8{ inputString };
}

