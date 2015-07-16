#pragma once
#include <core/RoPrerequisites.h>

#include "audio/RoBackgroundScore.h"
#include "audio/RoButtonSound.h"
#include "gamestates/RoGameState.h"

#include <archive/RoGrf.h>
#include <audio/RoAudioManager.h>
#include <core/RoConfig.h>
#include <network/RoNetworkManager.h>
#include <network/packets/RoCharacterServerCollection.h>

#include <entityx/entityx.h>

roFORWARD_DECL_PTR(RokLegend);
roFORWARD_DECL_PTR(RoLoginSuccessful);
roFORWARD_DECL_PTR(RoCharacterListing);
roFORWARD_DECL_PTR(RoLoginServer);

namespace Infector
{
    class Container;
}

class RoGameBindings
{
public:
    static RoConfigPtr getConfigs();
    static RoSharedPtr<entityx::EventManager> getEntityEventManager();
    static RoSharedPtr<entityx::EntityManager> getEntityManager();
    static RoAudioManagerPtr getAudioManager();
    // FIXME
    static RoNetworkManagerPtr getNetworkManager();
    static RoBackgroundScorePtr getBackgroundScore();
    // FIXME
    static RoButtonSoundPtr getButtonSound();

    // FIXME: Remove this. Resource manager should be exposed instead.
    static RoGrf2Ptr getGrf();

    static RokLegendPtr getGame();

    static RoGameStateFactoryPtr getGameStateFactory();
    static RoGameStatePtr getLoginState();
    static RoGameStatePtr getCharacterServerSelectState();

    static RoLoginSuccessfulPtr getAccountInfo();
    static RoCharacterListingPtr getCharacterListing();

    static RoLoginServerPtr getLoginServer();

private:
    static Infector::Container& getIocContainer();
    static void doBindings(Infector::Container& ioc);
    static void initConfigs(RoConfigPtr configs);
};