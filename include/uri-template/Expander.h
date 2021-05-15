#pragma once

#include "Template.h"

#include <limits>

namespace URI {
namespace Template {

/**
 * Performs percent-encoding of the string.
 * Will percent-encode incoming @p value. If @p allow_reserved is true then the characters from reserved
 * (as per https://tools.ietf.org/html/rfc6570#section-1.5) are not encoded.
 * This function is idempotent, means that already encoded triplets in @p value will not be encoded twice.
 *
 * @param[in] value A value to encode.
 * @param[in] allow_reserved If reserved characters are allowed in the result.
 * @param[in] max_len Maximum length of the result. Encoded triplets are counted as single character.
 *
 * @returns A percent-encoded representation of the @p value.
 */
std::string PctEncode(const std::string& value, bool allow_reserved = false,
                      std::size_t max_len = std::numeric_limits<size_t>::max());

/**
 * Expands a single template expression.
 * Expands an @p expression into a string according to the rules from https://tools.ietf.org/html/rfc6570#section-3.2.
 * Uses @p values to locate variables values that are expanded. Variables which are not in the map treated as undefined.
 *
 * @param[in] expression A template expression to expand.
 * @param[in] values Variables values to use for expansion.
 *
 * @returns Expansion result.
 */
std::string ExpandExpression(const Expression& expression, const std::unordered_map<std::string, VarValue>& values);

/**
 * Expands uri-template into a string.
 * Expands an @p uri_template into a string according to the rules from https://tools.ietf.org/html/rfc6570#section-3
 * Uses @p values to locate variables values that are expanded. Variables which are not in the map treated as undefined.
 *
 * @param[in] uri_template A template expression to expand.
 * @param[in] values Variables values to use for expansion.
 *
 * @returns Expansion result.
 */
std::string ExpandTemplate(const Template& uri_template, const std::unordered_map<std::string, VarValue>& values);

} // namespace Template
} // namespace URI
