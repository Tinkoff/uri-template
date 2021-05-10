#pragma once

#include "Template.h"

#include <optional>

namespace URI {
namespace Template {

/**
 * Container to hold match borders.
 */
class Match
{
public:
    /**
     * Parametrized constructor.
     * Creates a match with known start and end positions. Match considered to be in [start, end)
     */
    Match(std::size_t start, std::size_t end);

    /// Destructor.
    ~Match() = default;

    /// Get the starting position of the match.
    std::size_t Start() const;
    /// Get the ending position of the match.
    std::size_t End() const;

private:
    std::size_t start_; ///< Start of the match.
    std::size_t end_; ///< End of the match.
};

/**
 * Lookup for a literal.
 * Returns result of searching for a match of @p literal in @p where, starting from position @p start.
 *
 * @param[in] literal A literal to match.
 * @param[in] where A string where to lookup for a match.
 * @param[in] start Position to start lookup at.
 * @param[in] exact_start If true then .
 *
 * @returns Match instance wrapped in std::optional or std::nullopt.
 */
std::optional<Match> MatchLiteral(const Literal& literal, const std::string& where, std::size_t start,
                                  bool exact_start);

/**
 * Lookup for variable value.
 * Returns result of searching for a value of @p var in @p where consistent with the operator @p oper.
 *
 * @param[in] var Variable definition to lookup value for.
 * @param[in] oper Template operator for @p var.
 * @param[in] where A string where to lookup for a match.
 *
 * @returns VarValue instance wrapped in std::optional or std::nullopt.
 */
std::optional<VarValue> MatchVarValue(const Variable& var, const Operator& oper, std::optional<std::string>&& where);

/**
 * Lookup for template expression.
 * Returns result of searching for a expansion of an @p expression in @p where.
 * Expression considered matched if its' expansion does not contradict definition
 *
 * @param[in] expression Template expression to lookup for.
 * @param[in] where A string where to lookup for a match.
 * @param[in] start Position to start lookup at.
 * @param[in] end Position to end lookup at.
 * @param[in] terminator If this character met matching stops.
 * @param[out] values Map of template variables values found in the string.
 *  Not expanded variables will be filled with VarType::UNDEFINED.
 *
 * @returns Match instance wrapped in std::optional or std::nullopt.
 */
std::optional<Match> MatchExpression(const Expression& expression, const std::string& where, std::size_t start,
                                     std::size_t end, char terminator,
                                     std::unordered_map<std::string, VarValue>* values = nullptr);

/**
 * Lookup for URI-template.
 * Returns result of searching for a expansion of an @p uri_template in @p uri.
 * Template considered matched if its' expansion does not contradict definition,
 *  i.e. all literals are in place and all expressions doesn't have any violation in their expansions.
 *
 * @param[in] uri_template Template to lookup for.
 * @param[in] uri An URI where to lookup for a match.
 * @param[out] values Map of template variables values found in the string.
 *  Not expanded variables will be filled with VarType::UNDEFINED.
 *
 * @returns true if template matched, false – if not.
 */
bool MatchURI(const Template& uri_template, const std::string& uri,
              std::unordered_map<std::string, VarValue>* values = nullptr);

} // namespace Template
} // namespace URI
