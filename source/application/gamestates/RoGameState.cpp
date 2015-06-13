#include "RoGameState.h"
#include "../RokLegend.h"

#include <core/RoHashSet.h>
#include <core/RoLog.h>

#if roCOMPILER_IS_MSVC
#   include <conio.h>
#   define roKBHIT _kbhit
#   define roGETCH _getch
#endif // roCOMPILER_IS_MSVC

#define roKEY_ESC 27
#define roKEY_RETURN 13
#define roKEY_BACKSPACE 8

void RoGameState::initialize()
{
    static bool shouldInitialize = true;
    if (shouldInitialize)
    {
        addTaskHandlers();
        shouldInitialize = false;
    }
}

bool RoGameState::update(float timeSinceLastFrameInSecs)
{
    int ch = roKBHIT() ? roGETCH() : 0;
    if (ch == roKEY_ESC)
    {
        mGame->exit();
        return false;
    }
    return updateState(timeSinceLastFrameInSecs);
}

void RoGameState::exitGame() const
{
    mGame->exit();
}

RoOptionalUtf8 RoGameState::readInput(std::string message, bool isPassword) const
{
    static RoHashSet<char> allowedChars = { '.', ',', '!', '@', '$', '%', '^', '&', '*', '+', '-', '_', '=' };
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
        ch = roGETCH();
        if (isalnum(ch) || allowedChars.count(ch))
        {
            inputChars.push_back(ch);
            echo_func(ch);
        }
        else if (ch == roKEY_ESC)
        {
            return RoOptionalUtf8{};
        }
        else if (ch == roKEY_BACKSPACE)
        {
            int clearCharCount = message.length() + inputChars.size();
            std::cout << "\r";
            for (int i = 0; i < clearCharCount; ++i)
            {
                std::cout << ' ';
            }
            inputChars.pop_back();
            std::cout << "\r" << message;
            std::for_each(inputChars.begin(), inputChars.end(), echo_func);
        }
        else
        {
            roLOG_DBG << (int)ch;
        }
    } while (ch != roKEY_RETURN);
    std::cout << std::endl;
    inputChars.push_back(0);
    std::string inputString(inputChars.begin(), inputChars.end());
    return RoOptionalUtf8{ inputString };
}
