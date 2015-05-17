////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#include "RoConfig.h"
#include "RoException.h"
#include "RoErrorInfo.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <fstream>
#include <set>

RoConfig::RoConfig(void)
{
}

RoConfig::RoConfig(const RoString& file)
{
    open(file);
}

RoConfig::~RoConfig(void)
{
}

#ifndef _DEBUG
#   pragma warning(disable:4800) // Silencing a weird boost warning!
#endif
void RoConfig::open(const RoString& filename)
{
    try
    {
        std::ifstream file;
#if roPLATFORM_IS_WIN32
        file.open(filename.asWStr_c_str(), std::ios::in | std::ios::binary);
#else
        file.open(filename.asUTF8_c_str(), std::ios::in | std::ios::binary);
#endif
        if (!file.is_open())
            roTHROW(RoFileNotFound() << RoErrorInfoFileName(filename));

        RoString fileContent = RoString::FromStream(file);
        std::wstringstream textStream;
        textStream.write(fileContent.asWStr_c_str(), fileContent.size());

        using namespace boost::property_tree;
        wptree propertyTree;
        ini_parser::read_ini(textStream, propertyTree);
        wptree::iterator
            sectionItr = propertyTree.begin(),
            sectionEnd = propertyTree.end();
        RoPropertyMap configuration;
        while (sectionItr != sectionEnd)
        {
            const RoString sectionName = sectionItr->first;
            wptree::iterator
                settingItr = sectionItr->second.begin(),
                settingEnd = sectionItr->second.end();
            while (settingItr != settingEnd)
            {
                auto settingName = sectionName + L"." + settingItr->first;
                auto settingKey = RoHashString::FromString(settingName);
                const RoString settingValue = settingItr->second.get_value<std::wstring>();

                configuration.set(settingKey, settingValue);

                ++settingItr;
            }
            ++sectionItr;
        }
        mConfiguration.swap(configuration);
    }
    catch(boost::exception& ex)
    {
        ex << RoErrorInfoFileName(filename);
        throw;
    }
    catch(std::exception& ex)
    {
        roTHROW(RoException()
            << RoErrorInfoDetail(ex.what())
            << RoErrorInfoFileName(filename)
            << RoErrorInfoInnerException(ex));
    }

}

#ifndef _DEBUG
#   pragma warning(pop)
#endif

RoVariant RoConfig::get(const RoString& key) const
{
    auto keyHash = RoHashString::FromString(key);
    return mConfiguration.get(keyHash);
}
