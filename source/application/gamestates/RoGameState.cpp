#include "RoGameState.h"
#include "../RokLegend.h"

#include <core/RoHashSet.h>

#if roCOMPILER_IS_MSVC
#   include <conio.h>
#   define roKBHIT _kbhit
#   define roGETCH _getch
#endif // roCOMPILER_IS_MSVC

#define roKEY_ESC 27
#define roKEY_RETURN 13

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
    do
    {
        ch = roGETCH();
        if (isalnum(ch) || allowedChars.count(ch))
        {
            inputChars.push_back(ch);
            if (isPassword)
            {
                std::cout << "*";
            }
            else
            {
                std::cout << ch;
            }
        }
        else if (ch == roKEY_ESC)
        {
            return RoOptionalUtf8{};
        }
    } while (ch != roKEY_RETURN);
    std::cout << std::endl;
    inputChars.push_back(0);
    std::string inputString(inputChars.begin(), inputChars.end());
    return RoOptionalUtf8{ inputString };
}
