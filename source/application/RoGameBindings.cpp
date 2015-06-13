#include "RoGameBindings.h"
#include "infectorpp/InfectorContainer.hpp"
#include <audio/irrklang/RoIrrKlangAudioManager.h>
#include <core/RoFileSystem.h>

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

    ioc.wire<RoConfig>();
    ioc.wire<entityx::EventManager>();
    ioc.wire<entityx::EntityManager, entityx::EventManager>();
    ioc.wire<RoIrrKlangAudioManager, RoConfig>();
    ioc.wire<RoBackgroundScore, RoAudioManager, RoConfig>();

    initConfigs(ioc.buildSingle<RoConfig>());
}

void RoGameBindings::initConfigs(RoConfigPtr configs)
{
    RoString configFile = RoFileSystem::GetPathToGameDirForFile("RokLegend.ini", true);
    configs->open(configFile);
}
