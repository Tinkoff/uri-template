#include "uri-template/Modifier.h"

#include <stdexcept>

URI::Template::ModifierType URI::Template::ModNoop::Type() const
{
    return ModifierType::NONE;
}

char URI::Template::ModNoop::Start() const
{
    throw std::runtime_error("NONE modifier has no start");
}

URI::Template::ModifierType URI::Template::ModLength::Type() const
{
    return ModifierType::LENGTH;
}

char URI::Template::ModLength::Start() const
{
    return ':';
}

bool URI::Template::ModLength::IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

unsigned URI::Template::ModLength::ToNumber(const std::string& num_str)
{
    // handle up to 10 digits, assume only positive ASCII digits
    unsigned number = 0;
    unsigned length = num_str.length();
    switch (length) {
    case 10:
        number += (num_str[length - 10] - '0') * 1000000000;
        [[fallthrough]];
    case 9:
        number += (num_str[length - 9] - '0') * 100000000;
        [[fallthrough]];
    case 8:
        number += (num_str[length - 8] - '0') * 10000000;
        [[fallthrough]];
    case 7:
        number += (num_str[length - 7] - '0') * 1000000;
        [[fallthrough]];
    case 6:
        number += (num_str[length - 6] - '0') * 100000;
        [[fallthrough]];
    case 5:
        number += (num_str[length - 5] - '0') * 10000;
        [[fallthrough]];
    case 4:
        number += (num_str[length - 4] - '0') * 1000;
        [[fallthrough]];
    case 3:
        number += (num_str[length - 3] - '0') * 100;
        [[fallthrough]];
    case 2:
        number += (num_str[length - 2] - '0') * 10;
        [[fallthrough]];
    case 1:
        number += (num_str[length - 1] - '0');
        break;
    case 0:
        return 0;
    default:
        throw std::runtime_error("Number " + num_str + " is too big");
    }

    return number;
}

URI::Template::ModifierType URI::Template::ModExplode::Type() const
{
    return ModifierType::EXPLODE;
}

char URI::Template::ModExplode::Start() const
{
    return '*';
}
