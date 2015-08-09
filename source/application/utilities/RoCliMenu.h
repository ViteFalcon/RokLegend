#pragma once

#include <core/RoPrerequisites.h>

roFORWARD_DECL_PTR(RoButtonSound);

class RoCliMenu
{
public:
    RoCliMenu(const RoString& title);

    RoCliMenu& withOption(const RoString& option);
    RoCliMenu& withOptions(const RoStringArray& options);

    RoCliMenu& withPrompt(const RoString& prompt);

    RoCliMenu& withAcknowledgementSound(RoButtonSoundPtr acknowledgementSound);

    RoOptionalUInt32 getSelection() const;

private: // fields
    const RoString mTitle;
    RoStringArray mOptions;
    RoOptionalString mPrompt;
    optional<RoButtonSoundPtr> mAcknowledgementSound;
};
