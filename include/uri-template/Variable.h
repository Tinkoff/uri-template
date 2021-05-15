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

/**
 * Variable value type enumerator.
 * Describes type of the variable in the template.
 */
enum class VarType
{
    UNDEFINED, /**< value is not defined */
    STRING, /**< value is a string */
    LIST, /**< value is a list of strings */
    DICT, /**< value is an associative dictionary of strings */
};

/**
 * URI-template variable value.
 * This class represent type-safe union for value instances.
 * The value can hold either std::string or std::vector<std::string> or
 *  std::unordered_map<std::string, std::string> instance.
 */
class VarValue
{
public:
    /**
     * Parametrized constructor.
     * Creates an empty variable value of desired type.
     *
     * @param[in] var_type Type of value to create. Default is VarType::UNDEFINED.
     */
    VarValue(VarType var_type = VarType::UNDEFINED);

    /**
     * Parametrized constructor.
     * Creates a VarType::STRING variable from a @p str_value.
     *
     * @param[in] str_value std::string to hold as value.
     */
    VarValue(std::string&& str_value);

    /**
     * Parametrized constructor.
     * Creates a VarType::LIST variable from a @p list_value.
     *
     * @param[in] list_value std::vector to hold as value.
     */
    VarValue(std::vector<std::string>&& list_value);

    /**
     * Parametrized constructor.
     * Creates a VarType::DICT variable from a @p dict_value.
     *
     * @param[in] dict_value std::unordered_map to hold as value.
     */
    VarValue(std::unordered_map<std::string, std::string>&& dict_value);

    /// Copy constructor.
    VarValue(const VarValue&) = default;
    /// Copy assignment.
    VarValue& operator=(const VarValue&) = default;
    /// Move constructor.
    VarValue(VarValue&&) noexcept = default;
    /// Move assignment.
    VarValue& operator=(VarValue&&) noexcept = default;

    /// Destructor.
    ~VarValue() = default;

    /**
     * Get specific representation.
     *
     * @tparam T Type of the representation to get. Either:
     * @li `std::string` or
     * @li `std::vector<std::string>` or
     * @li `std::unordered_map<std::string, std::string>`
     *
     * @returns A reference of @p T to stored value.
     * @throws std::bad_variant_access if type mismatch.
     */
    template <class T>
    T& Get()
    {
        return std::get<T>(value_);
    }

    /**
     * Get specific representation.
     *
     * @tparam T Type of the representation to get. Either:
     * @li `std::string` or
     * @li `std::vector<std::string>` or
     * @li `std::unordered_map<std::string, std::string>`
     *
     * @returns A const reference of @p T to stored value.
     * @throws std::bad_variant_access if type mismatch.
     */
    template <class T>
    const T& Get() const
    {
        return std::get<T>(value_);
    }

    /// Get type of the value.
    VarType Type() const;

    /**
     * Produce printable value image.
     * Stored value printed as follows:
     *  @li VarType::UNDEFINED -> undefined
     *  @li VarType::STRING -> string
     *  @li VarType::LIST -> ['string', 'string', ...]
     *  @li VarType::DICT -> {'key': 'value', 'key': 'value', ...}
     * Where 'undefined' is literal constant, and 'string', 'key', 'value'
     *  are contents of vector or unordered_map respectively.
     *
     * @returns A string representing stored value.
     */
    std::string Print() const;

    /// Compares two values.
    bool operator==(const VarValue& rhs) const;
    /// Compares two values.
    bool operator!=(const VarValue& rhs) const;

private:
    VarType type_; ///< Type designator.
    // clang-format off
    std::variant<std::monostate,
                 std::string,
                 std::vector<std::string>,
                 std::unordered_map<std::string, std::string>> value_;  ///< Value internal storage.
    // clang-format on
};

/// VarValue printer via std::ostream's operator<<.
inline std::ostream& operator<<(std::ostream& os, const VarValue& var_value)
{
    os << var_value.Print();
    return os;
}

/**
 * URI-template variable definition.
 * This class represent a variable defined in the template.
 * After the operator (if any), each expression in the template contains a list of one or more
 *  comma-separated variable specifiers which are handled as instances of this class.
 */
class Variable
{
public:
    /**
     * Collection of allowed characters for variable name:
     * ALPHA / DIGIT / "_" / pct-encoded / "."
     */
    static const std::unordered_set<char> kNameChars;
    /**
     * Collection of unreserved characters allowed for variable value:
     * ALPHA / DIGIT / "-" / "." / "_" / "~"/ "%" / ","
     */
    static const std::unordered_set<char> kValueChars;
    /**
     * Collection of reserved characters allowed for variable value:
     * ":" / "/" / "?" / "#" / "[" / "]" / "@" / "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
     */
    static const std::unordered_set<char> kReservedChars;

    /**
     * Parametrized constructor.
     * Creates a variable with known name and modifiers.
     *
     * @param[in] name Name of the variable.
     * @param[in] modifier Modifier for the variable. nullptr is equivalent to Template::NOOP_MODIFIER.
     * @param[in] length Prefix length if @p modifier is ModifierType::LENGTH.
     */
    Variable(std::string&& name, std::shared_ptr<Modifier>&& modifier, unsigned length);

    /// Copy constructor.
    Variable(const Variable&) = default;
    /// Copy assignment.
    Variable& operator=(const Variable&) = default;
    /// Move constructor.
    Variable(Variable&&) noexcept = default;
    /// Move assignment.
    Variable& operator=(Variable&&) noexcept = default;

    /// Destructor.
    ~Variable() = default;

    /// Check if the variable has ModifierType::LENGTH modifier.
    bool IsPrefixed() const;
    /// Check if the variable has ModifierType::EXPLODE modifier.
    bool IsExploded() const;

    /// Get the variable name.
    const std::string& Name() const;
    /// Get the variable modifier.
    const Modifier& Mod() const;
    /// Get the variable prefix length.
    unsigned Length() const;
    /// Compares two variables.
    bool operator==(const Variable& rhs) const;
    /// Compares two variables.
    bool operator!=(const Variable& rhs) const;

private:
    std::string name_; ///< Variable name.
    std::shared_ptr<Modifier> modifier_; ///< Variable modifier.
    unsigned length_; ///< Variable prefix length.
};

} // namespace Template
} // namespace URI
