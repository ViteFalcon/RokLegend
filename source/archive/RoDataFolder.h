////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarök Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROGRF_DATA_FOLDER_H
#define ROGRF_DATA_FOLDER_H

#include <core/RoPrerequisites.h>

/**
  * Class containing folder path for different resources in GRF.
 **/
class RoDataFolder
{
public:
    // Folder that contains sprites for male
    static const RoString Male;
    // Folder that contains sprites for female
    static const RoString Female;

    // Folder where contents of book are
    static const RoString Book;
    // Contains files that control the thumbnail views of characters in 
    // character selection screen.
    static const RoString Imf;
    // Contains various models like statues, foliage, street lights, etc.
    static const RoString Model;
    // Interface script files.
    static const RoString LuaFiles;

    // Palette for cloths
    static const RoString ClothPalette;
    // Palette for hairs
    static const RoString HairPalette;

    // Folder with sprites of monsters
    static const RoString MonsterSprites;
    // Contains sprites of drops, items and skills
    static const RoString ItemAndSkillSprites;
    // Item sprites
    static const RoString ItemSprites;
    // Contains sprites of shields
    static const RoString ShieldSprites;
    // Contains sprites of different jobs
    static const RoString JobSprites;
    // Contains sprites of different hair styles
    static const RoString HairSprites;
    // Folder with sprites for status and other effects. E.g. curse, sleep,
    // the time-o-attack that pops up in PvP
    static const RoString EffectSprites;
    // Book sprites
    static const RoString BookSprites;
    // Homunculus sprites
    static const RoString HomunculusSprites;
    // NPC sprites
    static const RoString NpcSprites;

    // Logo textures
    static const RoString LogoTextures;
    // Folder for skills effects, which are always .tga format
    static const RoString EffectTextures;
    // Interface images (buttons and other UI images)
    static const RoString BasicInterfaceTextures;
    // Help buttons (?)
    static const RoString BattleFieldTextures;
    // Card images for when you click view in-game or right click the card.
    // Image has to be .bmp format.
    static const RoString CardTextures;
    // Images for items, the image will show up to the left of the
    // description for the item. Has to be .bmp format.
    static const RoString ItemDescriptionTextures;
    // Images for NPCs, pets, and quest images. Examples are kafra, or the
    // flag in the novice grounds. They have to have the transparent pink
    // background, and be .bmp.
    static const RoString IllustrationTextures;
    // Images for the items when seen in the inventory. Images have to have
    // the transparent pink background and be .bmp
    static const RoString InventoryTextures;
    // The login interface. Images for character selection, and the login
    // screen. Also has some warning images. .bmp format and needs transparency.
    static const RoString LoginInterfaceTextures;
    // Map images, the map is displayed in the upper right hand corner.
    static const RoString MiniMapTextures;

    // All the game sounds for monsters, and attacks.
    static const RoString Sounds;
    // All the game sounds for skills.
    static const RoString EffectSounds;

private:
    RoDataFolder();
    virtual ~RoDataFolder();
};

#endif // ROGRF_DATA_FOLDER_H
