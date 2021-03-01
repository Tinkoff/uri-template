#pragma once

#include "Template.h"

namespace URI {
namespace Template {

/**
 * Parse string for a single URI-template expression instance.
 * Expression here considered to be an operator and a list of variables with modifiers, if any.
 * @p expr_string should not contain leading and trailing brackets {}, just a single expression.
 *
 * @param[in] expr_string String to parse.
 *
 * @returns Expression instance parsed from @p expr_string.
 * @throws std::runtime_error if failed to parse.
 */
Expression ParseExpression(const std::string& expr_string);

/**
 * Parse string for an URI-template instance.
 * URI-template can have multiple literals or expressions in it.
 *
 * @param[in] tmpl_string String to parse.
 *
 * @returns Template instance parsed from @p tmpl_string.
 * @throws std::runtime_error if failed to parse.
 */
Template ParseTemplate(const std::string& tmpl_string);

} // namespace Template
} // namespace URI
