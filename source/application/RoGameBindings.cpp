#include "RoGameBindings.h"

#include "RokLegend.h"

#include "gamestates/RoCharacterSelectState.h"
#include "gamestates/RoCharacterServerSelectState.h"
#include "gamestates/RoLoginState.h"

#include "infectorpp/InfectorContainer.hpp"

#include "services/RoCharacterServerInterface.h"
#include "services/RoLoginServerInterface.h"

#include <audio/irrklang/RoIrrKlangAudioManager.h>

#include <core/RoFileSystem.h>

#include <network/packets/RoCharacterListing.h>
#include <network/packets/RoLoginSuccessful.h>

RoConfigPtr RoGameBindings::getConfigs()
{
    return getIocContainer().buildSingle<RoConfig>();
}

RoSharedPtr<entityx::EventManager> RoGameBindings::getEntityEventManager()
{
    return getIocContainer().buildSingle<entityx::EventManager>();
}

RoSharedPtr<entityx::EntityManager> RoGameBindings::getEntityManager()
{
    //static auto eventManager = getEntityEventManager();
    //static auto entityManager = std::make_shared<entityx::EntityManager>(*eventManager);
    //return entityManager;
    return getIocContainer().buildSingle<entityx::EntityManager>();
}

RoAudioManagerPtr RoGameBindings::getAudioManager()
{
    return getIocContainer().buildSingle<RoAudioManager>();
}

RoNetworkManagerPtr RoGameBindings::getNetworkManager()
{
    static auto networkManager = std::make_shared<RoNetworkManager>("data/packets.xml");
    return networkManager;
}

RoBackgroundScorePtr RoGameBindings::getBackgroundScore()
{
    return getIocContainer().buildSingle<RoBackgroundScore>();
}

RoButtonSoundPtr RoGameBindings::getButtonSound()
{
    static auto audioManager = getAudioManager();
    static auto grf = getGrf();
    static auto buttonSound = std::make_shared<RoButtonSound>(audioManager, grf);
    return buttonSound;
}

#define roGRF_TEST_FILE_KEY L"Game.Grf.TestFile"

RoGrf2Ptr RoGameBindings::getGrf()
{
    static auto config = getConfigs();
    static auto grf = RoGrf2::FromFile(config->get(roGRF_TEST_FILE_KEY).as<RoString>());
    return grf;
}

RokLegendPtr RoGameBindings::getGame()
{
    static auto audioManager = getAudioManager();
    static auto networkManager = getNetworkManager();
    static auto gameStateFactory = getGameStateFactory();
    static auto game = std::make_shared<RokLegend>(audioManager, networkManager, gameStateFactory);
    return game;
}

RoGameStateFactoryPtr RoGameBindings::getGameStateFactory()
{
    return getIocContainer().buildSingle<RoGameStateFactory>();
}

RoGameStatePtr RoGameBindings::getLoginState()
{
    auto game = getGame();
    auto backgroundScore = getBackgroundScore();
    auto buttonSound = getButtonSound();
    auto loginServer = getLoginServer();
    auto gameState = std::make_shared<RoLoginState>(game, backgroundScore, buttonSound, loginServer);
    return gameState;
}

RoGameStatePtr RoGameBindings::getCharacterServerSelectState()
{
    auto game = getGame();
    auto backgroundScore = getBackgroundScore();
    auto buttonSound = getButtonSound();
    auto characterServer = getCharacterServer();
    auto accountInfo = getAccountInfo();
    auto characterListing = getCharacterListing();
    auto gameState = std::make_shared<RoCharacterServerSelectState>(
        game,
        backgroundScore,
        buttonSound,
        characterServer,
        accountInfo,
        characterListing);
    return gameState;
}

RoGameStatePtr RoGameBindings::getCharacterSelectionState()
{
    auto game = getGame();
    auto backgroundScore = getBackgroundScore();
    auto buttonSound = getButtonSound();
    auto characterServer = getCharacterServer();
    return std::make_shared<RoCharacterSelectState>(
        game,
        backgroundScore,
        buttonSound,
        characterServer);
}

RoLoginSuccessfulPtr RoGameBindings::getAccountInfo()
{
    return getIocContainer().buildSingle<RoLoginSuccessful>();
}

RoCharacterListingPtr RoGameBindings::getCharacterListing()
{
    return getIocContainer().buildSingle<RoCharacterListing>();
}

RoLoginServerInterfacePtr RoGameBindings::getLoginServer()
{
    auto server = getIocContainer().buildSingle<RoLoginServerInterface>();
    server->initialize();
    return server;
}

RoCharacterServerInterfacePtr RoGameBindings::getCharacterServer()
{
    auto server = getIocContainer().buildSingle<RoCharacterServerInterface>();
    server->initialize();
    return server;
}

Infector::Container& RoGameBindings::getIocContainer()
{
    static Infector::Container ioc;
    static bool shouldDoBindings = true;
    if (shouldDoBindings)
    {
        doBindings(ioc);
        shouldDoBindings = false;
    }
    return ioc;
}

void RoGameBindings::doBindings(Infector::Container& ioc)
{
    ioc.bindSingleAsNothing<RoConfig>();
    ioc.bindSingleAsNothing<entityx::EventManager>();
    ioc.bindSingleAsNothing<entityx::EntityManager>();
    ioc.bindSingleAs<RoIrrKlangAudioManager, RoAudioManager>();
    ioc.bindSingleAsNothing<RoBackgroundScore>();
    ioc.bindSingleAsNothing<RoLoginSuccessful>();
    ioc.bindSingleAsNothing<RoCharacterListing>();
    ioc.bindSingleAsNothing<RoGameStateFactory>();
    ioc.bindSingleAsNothing<RoLoginServerInterface>();
    ioc.bindSingleAsNothing<RoCharacterServerInterface>();

    ioc.wire<RoConfig>();
    ioc.wire<entityx::EventManager>();
    ioc.wire<entityx::EntityManager, entityx::EventManager>();
    ioc.wire<RoIrrKlangAudioManager, RoConfig>();
    ioc.wire<RoBackgroundScore, RoAudioManager, RoConfig>();
    ioc.wire<RoLoginSuccessful>();
    ioc.wire<RoCharacterListing>();
    ioc.wire<RoGameStateFactory>();
    ioc.wire<RoLoginServerInterface, RoLoginSuccessful>();
    ioc.wire<RoCharacterServerInterface, RoLoginSuccessful, RoCharacterListing>();

    initConfigs(ioc.buildSingle<RoConfig>());
}

void RoGameBindings::initConfigs(RoConfigPtr configs)
{
    RoString configFile = RoFileSystem::GetPathToGameDirForFile("RokLegend.ini", true);
    configs->open(configFile);
}
