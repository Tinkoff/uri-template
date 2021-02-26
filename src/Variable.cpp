#include "uri-template/Variable.h"

/*
 * RFC6570:
 *      varname     = varchar *( ["."] varchar )
 *      varchar     = ALPHA / DIGIT / "_" / pct-encoded
 *      ALPHA       = A-Z / a-z
 *      DIGIT       = 0-9
 *      pct-encoded = "%" HEXDIG HEXDIG
 *      HEXDIG      = DIGIT / "A" / "B" / "C" / "D" / "E" / "F"
 */
// clang-format off
const std::unordered_set<char> URI::Template::Variable::kNameChars = {
    // ALPHA
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    // DIGIT
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    // "_" / pct-encoded / "."
    '_', '%', '.'
};
/*
 * RFC6570:
 *      unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
 */
const std::unordered_set<char> URI::Template::Variable::kValueChars = {
    // ALPHA
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    // DIGIT
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    // "-" / "." / "_" / "~"
    '-', '.', '_', '~',
    // pct-encoded / list-delim
    '%', ','
};
/*
 * RFC6570:
 *      reserved   = gen-delims / sub-delims
 *      gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
 *      sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
 */
const std::unordered_set<char> URI::Template::Variable::kReservedChars = {
    // gen-delims
    ':', '/', '?', '#', '[', ']', '@',
    // sub-delims
    '!', '$', '&', '\'', '(', ')', '*', '+', ',', ';', '='
};
// clang-format on

URI::Template::VarValue::VarValue(VarType var_type)
    : type_(var_type)
{
    switch (type_) {
    case VarType::UNDEFINED:
        break;
    case VarType::STRING:
        value_ = std::string();
        break;
    case VarType::LIST:
        value_ = std::vector<std::string>();
        break;
    case VarType::DICT:
        value_ = std::unordered_map<std::string, std::string>();
        break;
    }
}

URI::Template::VarValue::VarValue(std::string&& str_value)
    : type_(VarType::STRING)
    , value_(std::move(str_value))
{
}

URI::Template::VarValue::VarValue(std::vector<std::string>&& list_value)
    : type_(VarType::LIST)
    , value_(std::move(list_value))
{
}

URI::Template::VarValue::VarValue(std::unordered_map<std::string, std::string>&& dict_value)
    : type_(VarType::DICT)
    , value_(std::move(dict_value))
{
}

URI::Template::VarType URI::Template::VarValue::Type() const
{
    return type_;
}

std::string URI::Template::VarValue::Print() const
{
    std::string result;

    switch (type_) {
    case VarType::UNDEFINED:
        result = "undefined";
        break;

    case VarType::STRING:
        result = Get<std::string>();
        break;

    case VarType::LIST:
        result = "[";
        for (const auto& part : Get<std::vector<std::string>>()) {
            result += "'" + part + "', ";
        }
        if (result.size() > 1) {
            // remove trailing ", "
            result.pop_back();
            result.pop_back();
        }
        result += "]";
        break;
    case VarType::DICT:
        result = "{";
        for (const auto& [key, value] : Get<std::unordered_map<std::string, std::string>>()) {
            result += "'" + key + "': '" + value + "', ";
        }
        if (result.size() > 1) {
            // remove trailing ", "
            result.pop_back();
            result.pop_back();
        }
        result += "}";
        break;
    }

    return result;
}

bool URI::Template::VarValue::operator==(const VarValue& rhs) const
{
    return type_ == rhs.type_ && value_ == rhs.value_;
}

bool URI::Template::VarValue::operator!=(const VarValue& rhs) const
{
    return !(*this == rhs);
}

URI::Template::Variable::Variable(std::string&& name, std::shared_ptr<Modifier>&& modifier, unsigned length)
    : name_(std::move(name))
    , modifier_(std::move(modifier))
    , length_(length)
{
    if (!modifier_) {
        modifier_ = kNoopModifier;
    }
}

bool URI::Template::Variable::IsPrefixed() const
{
    return modifier_->Type() == ModifierType::LENGTH;
}

bool URI::Template::Variable::IsExploded() const
{
    return modifier_->Type() == ModifierType::EXPLODE;
}

const std::string& URI::Template::Variable::Name() const
{
    return name_;
}

const URI::Template::Modifier& URI::Template::Variable::Mod() const
{
    return *modifier_;
}

unsigned URI::Template::Variable::Length() const
{
    return length_;
}

bool URI::Template::Variable::operator==(const Variable& rhs) const
{
    return name_ == rhs.name_ && modifier_ == rhs.modifier_ && length_ == rhs.length_;
}

bool URI::Template::Variable::operator!=(const Variable& rhs) const
{
    return !(*this == rhs);
}
