#include "RoGameState.h"
#include "../RokLegend.h"
#include "../utilities/RoCli.h"

#include <core/RoLog.h>

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
    auto inputKey = RoCli::GetCharacterIfEntered();
    if (RoCli::IsEscapeKey(inputKey.get_value_or(0)))
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
