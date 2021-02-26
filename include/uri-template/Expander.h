#pragma once

#include "Template.h"

namespace URI {
namespace Template {

std::string PctEncode(const std::string& value, bool allow_reserved = false,
                      std::size_t max_len = std::numeric_limits<size_t>::max());
std::string ExpandExpression(const Expression& expression, const std::unordered_map<std::string, VarValue>& values);
std::string ExpandTemplate(const Template& uri_template, const std::unordered_map<std::string, VarValue>& values);

} // namespace Template
} // namespace URI
