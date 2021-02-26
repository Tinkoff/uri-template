#pragma once

#include "Modifier.h"

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace URI {
namespace Template {

enum class VarType
{
    UNDEFINED,
    STRING,
    LIST,
    DICT,
};

class VarValue
{
public:
    VarValue(VarType var_type = VarType::UNDEFINED);
    VarValue(std::string&& str_value);
    VarValue(std::vector<std::string>&& list_value);
    VarValue(std::unordered_map<std::string, std::string>&& dict_value);
    ~VarValue() = default;

    template <class T>
    T& Get()
    {
        return std::get<T>(value_);
    }

    template <class T>
    const T& Get() const
    {
        return std::get<T>(value_);
    }

    VarType Type() const;
    std::string Print() const;

    bool operator==(const VarValue& rhs) const;
    bool operator!=(const VarValue& rhs) const;

private:
    VarType type_;
    // clang-format off
    std::variant<std::monostate,
                 std::string,
                 std::vector<std::string>,
                 std::unordered_map<std::string, std::string>> value_;
    // clang-format on
};

inline std::ostream& operator<<(std::ostream& os, const VarValue& var_value)
{
    os << var_value.Print();
    return os;
}

class Variable
{
public:
    static const std::unordered_set<char> kNameChars;
    static const std::unordered_set<char> kValueChars;
    static const std::unordered_set<char> kReservedChars;

    Variable(std::string&& name, std::shared_ptr<Modifier>&& modifier, unsigned length);
    ~Variable() = default;

    bool IsPrefixed() const;
    bool IsExploded() const;
    const std::string& Name() const;
    const Modifier& Mod() const;
    unsigned Length() const;

    bool operator==(const Variable& rhs) const;
    bool operator!=(const Variable& rhs) const;

private:
    std::string name_;
    std::shared_ptr<Modifier> modifier_;
    unsigned length_;
};

} // namespace Template
} // namespace URI
