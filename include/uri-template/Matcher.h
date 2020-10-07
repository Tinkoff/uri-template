#pragma once

#include "Template.h"

namespace URI {
namespace Template {

class Match
{
public:
    Match(std::size_t start, std::size_t end);
    ~Match() = default;

    std::size_t Start() const;
    std::size_t End() const;

private:
    std::size_t start_;
    std::size_t end_;
};

std::optional<Match> MatchLiteral(const Literal& literal, const std::string& where, std::size_t start,
                                  bool exact_start);
std::optional<VarValue> MatchVarValue(const Variable& var, const Operator& oper,
                                      std::optional<std::string>&& raw_value);
std::optional<Match> MatchExpression(const Expression& expression, const std::string& where, std::size_t start,
                                     std::size_t end, char terminator,
                                     std::unordered_map<std::string, VarValue>* values = nullptr);
bool MatchURI(const Template& uri_template, const std::string& uri,
              std::unordered_map<std::string, VarValue>* values = nullptr);

} // namespace Template
} // namespace URI
