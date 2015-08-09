#include "RoCliMenu.h"

#include <core/RoStringBuilder.h>

#include "RoCli.h"

namespace
{
    RoString ToHyphens(const RoString& str)
    {
        RoStringBuilder hyphensBuilder;
        for (int i = 0; i < str.length(); ++i)
        {
            hyphensBuilder.append(L"-");
        }
        return hyphensBuilder.toString();
    }
}

RoCliMenu::RoCliMenu(const RoString& title)
    : mTitle(title)
{
}

RoCliMenu& RoCliMenu::withOption(const RoString& option)
{
    mOptions.push_back(option);
    return *this;
}

RoCliMenu& RoCliMenu::withOptions(const RoStringArray& options)
{
    mOptions.insert(mOptions.end(), options.begin(), options.end());
    return *this;
}

RoCliMenu& RoCliMenu::withPrompt(const RoString& prompt)
{
    mPrompt = prompt;
    return *this;
}

RoCliMenu& RoCliMenu::withAcknowledgementSound(RoButtonSoundPtr acknowledgementSound)
{
    if (acknowledgementSound)
    {
        mAcknowledgementSound = acknowledgementSound;
    }
    return *this;
}

RoOptionalUInt32 RoCliMenu::getSelection() const
{
    if (mOptions.empty())
    {
        return RoOptionalUInt32{};
    }

    const RoString titleUnderline = ToHyphens(mTitle);

    while (true)
    {
        std::cout << std::endl;
        std::cout << mTitle << std::endl;
        std::cout << titleUnderline << std::endl;
        for (int i = 0; i < mOptions.size(); ++i)
        {
            std::cout << "\t" << i << ". " << mOptions[i] << std::endl;
        }
        RoOptionalUInt32 selectedOption = RoCli::ReadInteger("> Select server: ", false, mAcknowledgementSound);
        if (!selectedOption)
        {
            return selectedOption;
        }
        const uint32 selectedIndex = selectedOption.get();
        if (selectedIndex >= 0 && selectedIndex < mOptions.size())
        {
            return selectedOption;
        }
        std::cout << "Invalid selection. Please try again!" << std::endl;
    }
}
