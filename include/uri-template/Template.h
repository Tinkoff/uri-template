#pragma once

#include "Operator.h"
#include "Variable.h"

namespace URI {
namespace Template {

namespace detail { // NOLINT(readability-identifier-naming)
template <typename...>
struct typelist;
}

/**
 * URI-template literal part.
 * This class represent text part of a template. The characters inside this part are intended to be copied literally
 * to the URI reference if the character is allowed.
 */
class Literal
{
public:
    /**
     * Collection of not allowed characters in literal parts:
     * CTL, SP, """, "'", "%" (aside from pct-encoded), "<", ">", "\", "^", "`", "{", "|", "}"
     */
    static const std::unordered_set<char> kNotAllowedChars;

    /**
     * Parametrized constructor.
     * Constructs literal part of the template from @p lit_string.
     *
     * @param[in] lit_string A string to construct from.
     */
    Literal(std::string&& lit_string);

    /// Copy constructor.
    Literal(const Literal&) = default;
    /// Copy assignment.
    Literal& operator=(const Literal&) = default;
    /// Move constructor.
    Literal(Literal&&) noexcept = default;
    /// Move assignment.
    Literal& operator=(Literal&&) noexcept = default;

    /// Destructor.
    ~Literal() = default;

    /**
     * Get size of the literal.
     *
     * @returns The number of elements in the literal.
     */
    std::size_t Size() const;

    /// Get literal string.
    const std::string& String() const noexcept;

    /// Compares the contents of two literals.
    bool operator==(const Literal& rhs) const;
    /// Compares the contents of two literals.
    bool operator!=(const Literal& rhs) const;

private:
    std::string lit_string_; ///< Literal string.
};

/**
 * URI-template expression part.
 * This class represent expression part of a template. Each expression contains an operator,
 *  which defines corresponding expansion process, and list of variable specifiers.
 */
class Expression
{
public:
    /**
     * Parametrized constructor.
     * Constructs expression an operator @p oper and @p variables.
     *
     * @param[in] oper An operator for the expression.
     * @param[in] variables A vector of variable definitions for the expression.
     */
    Expression(std::shared_ptr<Operator>&& oper, std::vector<Variable>&& variables);

    /// Copy constructor.
    Expression(const Expression&) = default;
    /// Copy assignment.
    Expression& operator=(const Expression&) = default;
    /// Move constructor.
    Expression(Expression&&) noexcept = default;
    /// Move assignment.
    Expression& operator=(Expression&&) noexcept = default;

    /// Destructor.
    ~Expression() = default;

    /**
     * Get the operator instance for this expression.
     *
     * @returns A const reference to the operator.
     */
    const Operator& Oper() const;

    /**
     * Get the variables for this expression.
     *
     * @returns A const reference to vector of variables.
     */
    const std::vector<Variable>& Vars() const;

    /// Get the expression string.
    std::string String() const noexcept;

    /// Compares two expressions.
    bool operator==(const Expression& rhs) const;
    /// Compares two expressions.
    bool operator!=(const Expression& rhs) const;

private:
    std::shared_ptr<Operator> oper_; ///< Operator.
    std::vector<Variable> var_list_; ///< Variables.
};

/**
 * Template part type enumerator.
 * Describes type of the part of the template.
 */
enum class PartType
{
    LITERAL, /**< literal part */
    EXPRESSION /**< expression part */
};

/**
 * URI-template part.
 * This class represent type-safe union for template part instances.
 * The part can hold either Literal or Expression instance.
 */
class Part
{
public:
    /**
     * Parametrized constructor.
     * Creates a template part. Type of the part deduced from @p args.
     *
     * @tparam ...Args Types of @p args.
     *
     * @param[in] ...args Arguments to pass to the part constructor.
     */
    template <typename... Args, typename = std::enable_if_t<!std::is_same<
                                    detail::typelist<Part>, detail::typelist<std::decay_t<Args>...>>::value>>
    Part(Args&&... args)
        : part_(std::forward<Args>(args)...)
    {
        if (std::holds_alternative<Literal>(part_)) {
            type_ = PartType::LITERAL;
        } else {
            type_ = PartType::EXPRESSION;
        }
    }

    /// Copy constructor.
    Part(const Part&) = default;
    /// Copy assignment.
    Part& operator=(const Part&) = default;
    /// Move constructor.
    Part(Part&&) noexcept = default;
    /// Move assignment.
    Part& operator=(Part&&) noexcept = default;

    /// Destructor.
    ~Part() = default;

    /**
     * Get specific representation.
     *
     * @tparam T Type of the representation to get. Either:
     * @li `Literal` or
     * @li `Expression`
     *
     * @returns A reference of @p T to stored instance.
     * @throws std::bad_variant_access if type mismatch.
     */
    template <class T>
    T& Get()
    {
        return std::get<T>(part_);
    }

    /**
     * Get specific representation.
     *
     * @tparam T Type of the representation to get. Either Literal or Expression.
     *
     * @returns A const reference of @p T to stored instance.
     * @throws std::bad_variant_access if type mismatch.
     */
    template <class T>
    const T& Get() const
    {
        return std::get<T>(part_);
    }

    /// Get type of the part.
    inline PartType Type() const
    {
        return type_;
    }

    /// Get the part string.
    inline std::string String() const noexcept
    {
        if (std::holds_alternative<Literal>(part_)) {
            return Get<Literal>().String();
        }
        return Get<Expression>().String();
    }

    /// Compares two parts.
    inline bool operator==(const Part& rhs) const
    {
        return type_ == rhs.type_ && part_ == rhs.part_;
    }

    /// Compares two parts.
    inline bool operator!=(const Part& rhs) const
    {
        return !(*this == rhs);
    }

private:
    PartType type_; ///< Type designator.
    std::variant<Literal, Expression> part_; ///< Part internal storage.
};

class Template
{
public:
    /**
     * Constructor.
     * Creates empty URI-template. Template should be populated later with EmplaceBack().
     */
    Template() = default;

    /// Copy constructor.
    Template(const Template&) = default;
    /// Copy assignment.
    Template& operator=(const Template&) = default;
    /// Move constructor.
    Template(Template&&) noexcept = default;
    /// Move assignment.
    Template& operator=(Template&&) noexcept = default;

    /// Destructor.
    ~Template() = default;

    /**
     * Appends a new part to the end of the template.
     *
     * @tparam ...Args Types of @p args.
     *
     * @param[in] ...args Arguments to pass to the part constructor.
     */
    template <class... Args>
    Part& EmplaceBack(Args&&... args)
    {
        return parts_.emplace_back(std::forward<Args>(args)...);
    }

    /**
     * Check if the template has any expressions in it.
     *
     * @returns true if template has at least one expression, false otherwise.
     */
    bool IsTemplated() const;

    /**
     * Get size of the template.
     *
     * @returns The number of parts in the template.
     */
    std::size_t Size() const;

    /**
     * Get a vector of parts in the template.
     *
     * @returns A reference to vector of parts.
     */
    std::vector<Part>& Parts();

    /**
     * Get a vector of parts in the template.
     *
     * @returns A const reference to vector of parts.
     */
    const std::vector<Part>& Parts() const;

    /**
     * Get specific part of the template by its index.
     * @note Accessing a nonexistent element through this operator is undefined behavior.
     *
     * @param[in] pos Index of the part to return.
     *
     * @returns A reference to the part at specified location @p pos.
     */
    Part& operator[](std::size_t pos);

    /**
     * Get specific part of the template by its index.
     * @note Accessing a nonexistent element through this operator is undefined behavior.
     *
     * @param[in] pos Index of the part to return.
     *
     * @returns A const reference to the part at specified location @p pos.
     */
    const Part& operator[](std::size_t pos) const;

    /// Get the template string.
    std::string String() const noexcept;

private:
    std::vector<Part> parts_; ///< Collection of parts.
};

} // namespace Template
} // namespace URI
