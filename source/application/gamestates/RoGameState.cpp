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

RoGameState::RoGameState(RokLegendPtr game, RoBackgroundScorePtr backgroundScore)
    : mGame(game)
    , mBackgroundScore(backgroundScore)
    , mShouldInitialize(true)
{
}

RoGameState::~RoGameState()
{
    //
}

void RoGameState::initialize()
{
    if (mShouldInitialize)
    {
        addTaskHandlers();
        roLOG_DBG << "Initialized game state: " << typeid(*this).name();
        mShouldInitialize = false;
    }
}

bool RoGameState::update(float timeSinceLastFrameInSecs)
{
    if (mShouldInitialize)
    {
        initialize();
    }
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
    return readRaw(message, isPassword, allowedChars);
}

RoOptionalUInt32 RoGameState::readInteger(std::string message, bool isPassword) const
{
    static RoHashSet<char> allowedChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    RoOptionalUtf8 rawInput = readRaw(message, isPassword, allowedChars);
    if (rawInput.is_initialized())
    {
        return RoOptionalUInt32{ atoi(rawInput->asUTF8().c_str()) };
    }
    return RoOptionalUInt32{};
}

RoOptionalUtf8 RoGameState::readRaw(std::string message, bool isPassword, const RoHashSet<char> allowedChars) const
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
        ch = roGETCH();
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
        else if (ch == roKEY_BACKSPACE)
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
    inputChars.push_back(0);
    std::string inputString(inputChars.begin(), inputChars.end());
    return RoOptionalUtf8{ inputString };
}

#include "../RoGameBindings.h"
#include <core/RoException.h>
#include <core/RoErrorInfo.h>

RoGameStatePtr RoGameStateFactory::getGameState(RoGameStates gameStateType)
{
    switch (gameStateType)
    {
    case RoGameStates::LOGIN:
        return RoGameBindings::getLoginState();
    case RoGameStates::CHARACTER_SERVER_SELECT:
        return RoGameBindings::getCharacterServerSelectState();
    }
    roTHROW(RoInvalidOperation()
        << RoErrorInfoDetail("Unimplemented game state.")
        << RoErrorInfoItemName(to_string(gameStateType)));
}

std::string to_string(const RoGameStates gameStateType)
{
    switch (gameStateType)
    {
    case RoGameStates::LOGIN_SERVER_SELECT:
        return "Login Server Select";
    case RoGameStates::LOGIN:
        return "Login";
    case RoGameStates::CHARACTER_SERVER_SELECT:
        return "Character Server Select";
    case RoGameStates::CHARACTER_SELECT:
        return "Character Select";
    case RoGameStates::MAP:
        return "Map";
    }
    return "UKNOWN state type";
}

std::ostream& operator<<(std::ostream& stream, const RoGameStates gameStateType)
{
    return stream << to_string(gameStateType);
}
