#pragma once
#include "RoGameState.h"

class RoCharacterSelectState : public RoGameStateT < RoCharacterSelectState >
{
public:
private:
    enum class Stage
    {
        CHARACTER_LISTING,
        CREATE_CHARACTER,
        CHARACTER_SELECTED,
        MAP_SERVER_CONNECTING,
        MAP_SERVER_CONNECT_FAILED,
        MAP_SERVER_CONNECT_SUCCEEDED,
        MAP_SERVER_DISCONNECTED
    };
    using RoAtomicStage = RoAtomic < Stage > ;

    RoAtomicStage mStage;
};