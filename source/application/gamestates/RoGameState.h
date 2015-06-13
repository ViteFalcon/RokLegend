#pragma once
#include <core/RoPrerequisites.h>
#include <core/task/RoTaskHandler.h>

roFORWARD_DECL_PTR(RokLegend);
roFORWARD_DECL_PTR(RoBackgroundScore);

enum class RoGameStates
{
    LOGIN_SERVER_SELECT,
    LOGIN,
    CHARACTER_SERVER_SELECT,
    CHARACTER_SELECT,
    MAP
};

class RoGameState
{
public:
    RoGameState(RokLegendPtr game, RoBackgroundScorePtr backgroundScore)
        : mGame(game)
        , mBackgroundScore(backgroundScore) {}
    virtual ~RoGameState() = default;

    void initialize();

    bool update(float timeSinceLastFrameInSecs);

protected: // methods
    void exitGame() const;

    RoOptionalUtf8 readInput(std::string message, bool isPassword) const;

    virtual void addTaskHandlers() = 0;
    virtual bool updateState(float timeSinceLastFrameInSecs) = 0;

protected:
    RokLegendPtr mGame;
    RoBackgroundScorePtr mBackgroundScore;
};

roDEFINE_PTR(RoGameState);

template <typename DerivedType>
class RoGameStateT : public RoGameState, public RoTaskHandler<DerivedType>
{
public:
    RoGameStateT(RokLegendPtr game, RoBackgroundScorePtr backgroundScore)
        : RoGameState(game, backgroundScore) {}
    virtual ~RoGameStateT() = default;
};
