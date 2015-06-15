#pragma once
#include <core/RoPrerequisites.h>

enum class RoAccountGender
{
    UNKNOWN = -1,
    FEMALE,
    MALE,
    SERVER
};

RoAccountGender roGetAccountGender(uint8 genderValue);

std::string to_string(const RoAccountGender gender);
std::ostream& operator << (std::ostream& stream, const RoAccountGender gender);
