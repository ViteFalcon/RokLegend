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
    addTaskHandler(SELECT_CHARACTER_PROMPT_TASK, &RoCharacterSelectState::selectCharacterPrompt);
    addTaskHandler(CREATE_CHARACTER_PROMPT_TASK, &RoCharacterSelectState::createCharacterPrompt);
    addTaskHandler(DELETE_CHARACTER_PROMPT_TASK, &RoCharacterSelectState::deleteCharacterPrompt);
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
        changeStage(stage, Stage::MAIN_MENU);
        roSCHEDULE_TASK_NAMED(MAIN_MENU_PROMPT_TASK, RoEmptyArgs::INSTANCE);
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

void RoCharacterSelectState::mainMenuPrompt(const RoTaskArgs & args)
{
    auto stage = Stage::MAIN_MENU;
    auto selection = RoCliMenu("Character Listing and Options")
        .withAcknowledgementSound(mButtonSound)
        .withOption("Select character")
        .withOption("Create character")
        .withOption("Delete character")
        .getSelection();
    if (!selection.is_initialized())
    {
        changeStage(stage, Stage::MAIN_MENU_CANCELLED);
        return;
    }
    std::cout << "-- Selected option: " << selection.get() << std::endl;
    switch (selection.get())
    {
    case 0:
        changeStage(stage, Stage::CHARACTER_LISTING);
        roSCHEDULE_TASK_NAMED(SELECT_CHARACTER_PROMPT_TASK, RoEmptyArgs::INSTANCE);
        break;
    case 1:
        changeStage(stage, Stage::CREATE_CHARACTER);
        roSCHEDULE_TASK_NAMED(CREATE_CHARACTER_PROMPT_TASK, RoEmptyArgs::INSTANCE);
        break;
    case 2:
        changeStage(stage, Stage::DELETE_CHARACTER);
        roSCHEDULE_TASK_NAMED(DELETE_CHARACTER_PROMPT_TASK, RoEmptyArgs::INSTANCE);
        break;
    default:
        // Loop back to the main-menu
        changeStage(stage, Stage::NONE);
        break;
    }
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

void RoCharacterSelectState::selectCharacterPrompt(const RoTaskArgs& args)
{
    auto stage = Stage::CHARACTER_LISTING;
    auto characters = mCharacterServer->getCharacterListing()->getCharacters();
    if (characters.empty())
    {
        std::cout << "There are no characters to select!" << std::endl;
        changeStage(stage, Stage::CHARACTER_LISTING_CANCELLED);
        return;
    }
    RoStringArray characterNames;
    characterNames.reserve(characters.size());
    for each (auto character in characters)
    {
        characterNames.push_back(toShortString(*character));
    }
    auto selection = RoCliMenu("Select Character")
        .withOptions(characterNames)
        .withAcknowledgementSound(mButtonSound)
        .getSelection();
    if (!selection.is_initialized())
    {
        changeStage(stage, Stage::CHARACTER_LISTING_CANCELLED);
        return;
    }
    auto selectedCharacterIndex = selection.get();
    std::cout
        << "Selected the following character:" << std::endl
        << toString(*characters[selectedCharacterIndex]) << std::endl;
    // FIXME: Canceling character listing for now
    changeStage(stage, Stage::CHARACTER_LISTING_CANCELLED);
}

void RoCharacterSelectState::createCharacterPrompt(const RoTaskArgs & args)
{
    // FIXME: Canceling character creation for now
    auto stage = Stage::CREATE_CHARACTER;
    changeStage(stage, Stage::CREATE_CHARACTER_CANCELLED);
}

void RoCharacterSelectState::deleteCharacterPrompt(const RoTaskArgs & args)
{
    // FIXME: Canceling character deletion for now
    auto stage = Stage::DELETE_CHARACTER;
    changeStage(stage, Stage::DELETE_CHARACTER_CANCELLED);
}
