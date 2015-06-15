#pragma once
#include <core/RoPrerequisites.h>
#include <core/task/RoTaskHandler.h>
#include <core/RoHashSet.h>

roFORWARD_DECL_PTR(RokLegend);
roFORWARD_DECL_PTR(RoBackgroundScore);

enum class RoGameStates
{
    NONE,
    LOGIN_SERVER_SELECT,
    LOGIN,
    CHARACTER_SERVER_SELECT,
    CHARACTER_SELECT,
    MAP
};

std::string to_string(const RoGameStates gameStateType);
std::ostream& operator << (std::ostream& stream, const RoGameStates gameStateType);

class RoGameState
{
public:
    RoGameState(RokLegendPtr game, RoBackgroundScorePtr backgroundScore);
    virtual ~RoGameState();

    void initialize();

    bool update(float timeSinceLastFrameInSecs);

protected: // methods
    void exitGame() const;

    RoOptionalUtf8 readInput(std::string message, bool isPassword) const;
    RoOptionalUInt32 readInteger(std::string message, bool isPassword) const;
    RoOptionalUtf8 readRaw(std::string message, bool isPassword, const RoHashSet<char> allowedChars) const;

    virtual void addTaskHandlers() = 0;
    virtual bool updateState(float timeSinceLastFrameInSecs) = 0;

protected:
    RokLegendPtr mGame;
    RoBackgroundScorePtr mBackgroundScore;

private:
    bool mShouldInitialize;
};

roDEFINE_PTR(RoGameState);

template <typename DerivedType>
class RoGameStateT : public RoGameState, public RoTaskHandler<DerivedType>
{
public:
    RoGameStateT(RokLegendPtr game, RoBackgroundScorePtr backgroundScore)
        : RoGameState(game, backgroundScore) {}
    virtual ~RoGameStateT()
    {
        clearTaskHandlers();
    }
};

class RoGameStateFactory
{
public:
    RoGameStatePtr getGameState(RoGameStates gameStateType);
};

roDEFINE_PTR(RoGameStateFactory);
