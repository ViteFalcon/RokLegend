#include "RoCharacterSelectState.h"
#include "../RokLegend.h"
#include "../services/RoCharacterServerInterface.h"
#include "../utilities/RoCliMenu.h"

#include <core/RoStringBuilder.h>
#include <core/task/RoTaskManager.h>

#include <network/packets/RoCharacterListing.h>

const RoString RoCharacterSelectState::MAIN_MENU_PROMPT_TASK = { L"CharacterSelect:MainMenuPrompt" };
const RoString RoCharacterSelectState::SELECT_CHARACTER_PROMPT_TASK = { L"CharacterSelect:SelectCharacterPrompt" };
const RoString RoCharacterSelectState::CREATE_CHARACTER_PROMPT_TASK = { L"CharacterSelect:CreateCharacterPrompt" };
const RoString RoCharacterSelectState::DELETE_CHARACTER_PROMPT_TASK = { L"CharacterSelect:DeleteCharacterPrompt" };

namespace {
    static const RoHashString CHARACTER_KEY{ "character" };
    static const RoHashString SLOT_KEY{ "slot" };
}

roDEFINE_TASK_ARGS(RoCharacterSelectArgs)
{
    inline RoCharacterInformationPtr getCharacter() const
    {
        RoCharacterInformationPtr character;
        get(CHARACTER_KEY, character);
        return character;
    }

    inline void setCharacter(RoCharacterInformationPtr character)
    {
        set(CHARACTER_KEY, character);
    }
};

roDEFINE_TASK_ARGS(RoCharacterCreateArgs)
{
    inline size_t getSlot() const
    {
        size_t slot;
        get(SLOT_KEY, slot);
        return slot;
    }

    inline void setSlot(size_t slot)
    {
        set(SLOT_KEY, slot);
    }
};

roDEFINE_TASK_ARGS(RoCharacterDeleteArgs)
{
    inline size_t getSlot() const
    {
        size_t slot;
        get(SLOT_KEY, slot);
        return slot;
    }

    inline void setSlot(size_t slot)
    {
        set(SLOT_KEY, slot);
    }
};

RoCharacterSelectState::RoCharacterSelectState(
    RokLegendPtr game,
    RoBackgroundScorePtr backgroundScore,
    RoButtonSoundPtr buttonSound,
    RoCharacterServerInterfacePtr characterServer)
    : RoGameStateT(game, backgroundScore)
    , mButtonSound{ buttonSound }
    , mCharacterServer{ characterServer }
    , mStage{ Stage::NONE }
{
}

void RoCharacterSelectState::addTaskHandlers()
{
    addTaskHandler(MAIN_MENU_PROMPT_TASK, &RoCharacterSelectState::mainMenuPrompt);
    addTaskHandler<RoCharacterSelectArgs>(SELECT_CHARACTER_PROMPT_TASK, &RoCharacterSelectState::selectCharacterPrompt);
    addTaskHandler<RoCharacterCreateArgs>(CREATE_CHARACTER_PROMPT_TASK, &RoCharacterSelectState::createCharacterPrompt);
    addTaskHandler<RoCharacterDeleteArgs>(DELETE_CHARACTER_PROMPT_TASK, &RoCharacterSelectState::deleteCharacterPrompt);
}

bool RoCharacterSelectState::updateState(float timeSinceLastFrameInSecs)
{
    Stage stage = mStage.load();
    switch (stage)
    {
    case Stage::NONE:
    case Stage::CHARACTER_LISTING_CANCELLED:
    case Stage::CREATE_CHARACTER_CANCELLED:
    case Stage::DELETE_CHARACTER_CANCELLED:
        showMainMenu();
        break;
    case Stage::MAIN_MENU_CANCELLED:
        mCharacterServer->disconnect();
        mGame->setGameState(RoGameStates::LOGIN);
        break;
    case Stage::MAIN_MENU:
    default:
        break;
    }
    return true;
}

RoCharacterSelectState::Stage RoCharacterSelectState::getCurrentStage() const
{
    return mStage.load();
}

bool RoCharacterSelectState::changeStage(Stage& expectedState, const Stage newState)
{
    const auto expected = expectedState;
    if (!mStage.compare_exchange_strong(expectedState, newState))
    {
        std::cerr << "Unexpected game-state found. Current: "
            << as_string(expectedState)
            << ", Transition: " << as_string(expected) << " -> " << as_string(newState)
            << std::endl;
        return false;
    }
    return true;
}

std::string RoCharacterSelectState::as_string(const Stage& stage)
{
    switch (stage)
    {
    case Stage::NONE:
        return "NONE";
    case Stage::MAIN_MENU:
        return "MAIN_MENU";
    case Stage::MAIN_MENU_CANCELLED:
        return "MAIN_MENU_CANCELLED";
    case Stage::CHARACTER_LISTING:
        return "CHARACTER_LISTING";
    case Stage::CHARACTER_LISTING_CANCELLED:
        return "CHARACTER_LISTING_CANCELLED";
    case Stage::CREATE_CHARACTER:
        return "CREATE_CHARACTER";
    case Stage::CREATE_CHARACTER_CANCELLED:
        return "CREATE_CHARACTER_CANCELLED";
    case Stage::DELETE_CHARACTER:
        return "DELETE_CHARACTER";
    case Stage::DELETE_CHARACTER_CANCELLED:
        return "DELETE_CHARACTER_CANCELLED";
    default:
        break;
    }
    RoStringBuilder stageString;
    stageString << "UNKNOWN (" << as_integer(stage) << ")";
    return stageString.toString().asUTF8();
}

void RoCharacterSelectState::showMainMenu()
{
    scheduleStage(Stage::MAIN_MENU, MAIN_MENU_PROMPT_TASK, RoEmptyArgs::INSTANCE);
}

void RoCharacterSelectState::showSelectedCharacterMenu(RoCharacterInformationPtr character)
{
    RoCharacterSelectArgs args;
    args.setCharacter(character);
    scheduleStage(Stage::CHARACTER_LISTING, SELECT_CHARACTER_PROMPT_TASK, args);
}

void RoCharacterSelectState::showCreateCharacterMenu(size_t slot)
{
    RoCharacterCreateArgs args;
    args.setSlot(slot);
    scheduleStage(Stage::CREATE_CHARACTER, CREATE_CHARACTER_PROMPT_TASK, args);
}

void RoCharacterSelectState::showDeleteCharacterPrompt(size_t slot)
{
    RoCharacterDeleteArgs args;
    args.setSlot(slot);
    scheduleStage(Stage::DELETE_CHARACTER, DELETE_CHARACTER_PROMPT_TASK, args);
}

inline void RoCharacterSelectState::scheduleStage(Stage stage, RoString taskName, const RoTaskArgs& args)
{
    Stage previousStage = mStage.load();
    changeStage(previousStage, stage);
    roSCHEDULE_TASK_NAMED(taskName, args);
}

namespace {
    RoString toShortString(const RoCharacterInformation& character)
    {
        RoStringBuilder characterString;
        characterString
            << character.getName()
            << " [" << character.getBaseLevel() << "/" << character.getJobLevel() << "]";
        return characterString.toString();
    }

    RoString toString(const RoCharacterInformation& character)
    {
        RoStringBuilder characterString;
        characterString
            << "Name: " << character.getName() << std::endl
            << "Base Level: " << character.getBaseLevel() << std::endl
            << "Job Level: " << character.getJobLevel() << std::endl
            << "Health: " << int(character.getHealthPercent() * 100) << std::endl
            << "Stamina: " << int(character.getManaPercent() * 100) << std::endl
            << "Stats" << std::endl
            << "-----" << std::endl
            << "\tStr: " << character.getStrength() << std::endl
            << "\tAgi: " << character.getAgility() << std::endl
            << "\tDex: " << character.getDexterity() << std::endl
            << "\tVit: " << character.getVitality() << std::endl
            << "\tInt: " << character.getIntelligence() << std::endl
            << "\tLuk: " << character.getLuck() << std::endl;
        return characterString.toString();
    }
}

void RoCharacterSelectState::mainMenuPrompt(const RoTaskArgs & args)
{
    static const RoString EMPTY_CHARACTER_OPTION{ L"Empty - Create character" };
    auto characterListing = mCharacterServer->getCharacterListing();
    auto characters = characterListing->getCharacters();
    size_t maxSlots = static_cast<size_t>(characterListing->getMaxSlots().get_value_or(9));
    RoStringArray characterNames(maxSlots, EMPTY_CHARACTER_OPTION);
    for each (auto character in characters)
    {
        characterNames[character->getSlot()] = toShortString(*character);
    }

    auto stage = Stage::MAIN_MENU;
    auto selection = RoCliMenu("Character Listing and Options")
        .withAcknowledgementSound(mButtonSound)
        .withOptions(characterNames)
        .getSelection();
    if (!selection.is_initialized())
    {
        changeStage(stage, Stage::MAIN_MENU_CANCELLED);
        return;
    }

    uint32 selectedSlot = selection.get();
    optional<RoCharacterInformationPtr> selectedCharacter;
    for each (auto character in characters)
    {
        if (character->getSlot() == selectedSlot)
        {
            selectedCharacter = character;
            break;
        }
    }
    if (selectedCharacter)
    {
        showSelectedCharacterMenu(selectedCharacter.get());
    }
    else
    {
        showCreateCharacterMenu(selectedSlot);
    }
}

void RoCharacterSelectState::selectCharacterPrompt(const RoCharacterSelectArgs& args)
{
    RoCharacterInformationPtr character = args.getCharacter();
    std::cout << "Selected character:" << std::endl << toString(*character);
    // FIXME: Canceling character listing for now
    auto stage = mStage.load();
    changeStage(stage, Stage::CHARACTER_LISTING_CANCELLED);
}

void RoCharacterSelectState::createCharacterPrompt(const RoCharacterCreateArgs & args)
{
    // FIXME: Canceling character creation for now
    auto stage = Stage::CREATE_CHARACTER;
    std::string name;
    std::cout << "Name: ";
    std::cin >> name;
    uint16 hairColor = 1;
    uint16 hairStyle = 1;
    RoCreateCharacterCallbacks callbacks;
    callbacks.failureCallback = std::bind(&RoCharacterSelectState::onFailedCharacterCreation, this, std::placeholders::_1);
    callbacks.successCallback = std::bind(&RoCharacterSelectState::onSuccessfulCharacterCreation, this, std::placeholders::_1);
    mCharacterServer->createCharacter(callbacks, args.getSlot(), name, hairColor, hairStyle);
}

void RoCharacterSelectState::deleteCharacterPrompt(const RoCharacterDeleteArgs & args)
{
    // FIXME: Canceling character deletion for now
    auto stage = Stage::DELETE_CHARACTER;
    changeStage(stage, Stage::DELETE_CHARACTER_CANCELLED);
}

void RoCharacterSelectState::onSuccessfulCharacterCreation(const RoCharacterInformation& character)
{
    std::cout << "Successfully created character:" << std::endl << toString(character) << std::endl;
    showMainMenu();
}

void RoCharacterSelectState::onFailedCharacterCreation(const RoString& reason)
{
    std::cout << "Failed to create character. Reason: " << reason << std::endl;
    showMainMenu();
}
