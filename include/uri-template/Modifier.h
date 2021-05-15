#pragma once

#include <memory>
#include <string>
#include <vector>

namespace URI {
namespace Template {

/**
 * Variable modifier type enumerator.
 * Describes type of the modifier a variable has in the expression.
 */
enum class ModifierType
{
    NONE, /**< no modifier */
    LENGTH, /**< variable expansion is limited in length */
    EXPLODE /**< expansion process is applied to each member of a composite value */
};

/**
 * Abstract base to hold definition of variable modifier.
 */
class Modifier
{
public:
    /// Destructor.
    virtual ~Modifier() = default;

    /// Get type of the modifier.
    virtual ModifierType Type() const = 0;
    /// Get starting character of the modifier.
    virtual char Start() const = 0;
};

/**
 * No modifications modifier.
 * This class represents absence of modifier for a variable.
 */
class ModNoop: public Modifier
{
public:
    /// Constructor.
    ModNoop() = default;
    /// Destructor.
    virtual ~ModNoop() = default;

    /**
     * Get type of the modifier.
     *
     * @returns ModifierType::NONE.
     */
    virtual ModifierType Type() const override;

    /**
     * Get starting character of the modifier.
     *
     * @throws std::runtime_error on call.
     */
    virtual char Start() const override;
};

/**
 * Length modifier.
 * This class represents length modifier for a variable.
 */
class ModLength: public Modifier
{
public:
    /// Constructor.
    ModLength() = default;
    /// Destructor.
    virtual ~ModLength() = default;

    /**
     * Get type of the modifier.
     *
     * @returns ModifierType::LENGTH.
     */
    virtual ModifierType Type() const override;

    /**
     * Get starting character of the modifier.
     *
     * @returns ':'.
     */
    virtual char Start() const override;

    /**
     * Fast and limited alternative to std::isdigit.
     * This function is used to parse length modifier in URI-template.
     *
     * @param[in] c Character to test.
     *
     * @returns If @p c in [0..9] range.
     */
    static bool IsDigit(char c);

    /**
     * Fast and limited alternative to std::atoi.
     * This function is used to parse length modifier in URI-template.
     *
     * @li @p num_str can contain only [0..9] digits.
     * @li Does not support signed notation.
     *
     * @param[in] num_str Character to test.
     *
     * @returns Number @p num_str represents.
     * @throws std::runtime_error if @p num_str length greater than 10.
     */
    static unsigned ToNumber(const std::string& num_str);
};

/**
 * Explode modifier.
 * This class represents explode modifier for a variable.
 */
class ModExplode: public Modifier
{
public:
    /// Constructor.
    ModExplode() = default;
    /// Destructor.
    virtual ~ModExplode() = default;

    /**
     * Get type of the modifier.
     *
     * @returns ModifierType::EXPLODE.
     */
    virtual ModifierType Type() const override;

    /**
     * Get starting character of the modifier.
     *
     * @returns '*'.
     */
    virtual char Start() const override;
};

/// Noop modifier instance to use for variables.
inline const std::shared_ptr<Modifier> NOOP_MODIFIER = std::make_shared<ModNoop>();
/// Collection of different modifier instances to use for variables.
inline const std::vector<std::shared_ptr<Modifier>> KNOWN_MODIFIERS = {
    std::make_shared<ModLength>(),
    std::make_shared<ModExplode>(),
};

} // namespace Template
} // namespace URI
