#include "RoAccountGender.h"

RoAccountGender roGetAccountGender(uint8 genderValue)
{
    switch (genderValue)
    {
    case 0:
        return RoAccountGender::FEMALE;
    case 1:
        return RoAccountGender::MALE;
    case 2:
        return RoAccountGender::SERVER;
    default:
        break;
    }
    return RoAccountGender::UNKNOWN;
}

std::string to_string(const RoAccountGender gender)
{
    switch (gender)
    {
    case RoAccountGender::FEMALE:
        return "Female";
    case RoAccountGender::MALE:
        return "Male";
    case RoAccountGender::SERVER:
        return "Server";
    default:
        break;
    }
    return "Unknown Gender";
}

std::ostream& operator<<(std::ostream& stream, const RoAccountGender gender)
{
    return stream << to_string(gender);
}
