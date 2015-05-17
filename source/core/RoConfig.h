#pragma once
#ifndef ROKLEGEND_CONFIG_H
#define ROKLEGEND_CONFIG_H

#include "RoPrerequisites.h"
#include "RoPropertyMap.h"

class RoConfig
{
public:
    RoConfig(void);
    explicit RoConfig(const RoString& file);
    ~RoConfig(void);

    void open(const RoString& file);

    roINLINE bool has(const RoString& key)
    {
        auto keyHash = RoHashString::FromString(key);
        return mConfiguration.has(keyHash);
    }

    RoVariant get(const RoString& key) const;

    roINLINE RoVariant operator[](const RoString& key) const
    {
        return get(key);
    }
private:
    RoPropertyMap mConfiguration;
};

#endif // ROKLEGEND_CONFIG_H
