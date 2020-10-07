#pragma once

#include "Template.h"

namespace URI {
namespace Template {

Expression ParseExpression(const std::string& expr_string);
Template ParseTemplate(const std::string& expr_string);

} // namespace Template
} // namespace URI
