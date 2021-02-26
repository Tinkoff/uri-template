#include "uri-template/Matcher.h"

namespace {

enum class ExprParts
{
    OPERATOR,
    VARIABLE,
};

enum class VarParts
{
    NAME,
    VALUE,
};

bool StartsWith(const std::string& str, const std::string& prefix)
{
    // in c++17 we don't have starts_with()
    const auto str_start = std::string_view(str).substr(0, prefix.size());
    if (str_start == prefix) {
        return true;
    }
    return false;
}

} // namespace

URI::Template::Match::Match(std::size_t start, std::size_t end)
    : start_(start)
    , end_(end)
{
}

std::size_t URI::Template::Match::Start() const
{
    return start_;
}

std::size_t URI::Template::Match::End() const
{
    return end_;
}

std::optional<URI::Template::Match> URI::Template::MatchLiteral(const Literal& literal, const std::string& where,
                                                                std::size_t start, bool exact_start)
{
    if (start + literal.Size() > where.size()) {
        // literal doesn't fit in where
        return std::nullopt;
    }

    if (exact_start) {
        if (!StartsWith(where, literal.String())) {
            return std::nullopt;
        }
        return Match(start, start + literal.Size());
    }

    std::size_t m_start = where.find(literal.String(), start);
    if (m_start == std::string::npos) {
        return std::nullopt;
    }
    return Match(m_start, m_start + literal.Size());
}

std::optional<URI::Template::VarValue> URI::Template::MatchVarValue(const Variable& var, const Operator& oper,
                                                                    std::optional<std::string>&& where)
{
    if (!where) {
        // treat undefined exploded as an empty list
        return var.IsExploded() ? VarValue(VarType::LIST) : VarValue(VarType::UNDEFINED);
    }

    if (oper.Reserved()) {
        // reserved operator can contain all symbols in a string
        auto value = VarValue(VarType::STRING);
        value.Get<std::string>() = std::move(*where);
        // TODO: exploded reserved
        return value;
    }

    std::size_t pos = 0;
    std::string parsed_part;
    std::vector<std::string> names;
    std::vector<std::string> values;
    std::unordered_set<std::string> names_set;

    VarType var_type = VarType::STRING;
    VarParts parsing = oper.Named() ? VarParts::NAME : VarParts::VALUE;
    while (pos < where->size()) {
        char cur_char = where->at(pos);

        switch (parsing) {
        case VarParts::NAME:
            if (cur_char == '=') {
                if (var.IsExploded()) {
                    var_type = VarType::DICT;
                } else if (!oper.Named()) {
                    // cannot contain '=' if not exloded or named
                    return std::nullopt;
                }

                names_set.emplace(names.emplace_back(std::move(parsed_part)));
                parsed_part.clear();
                parsing = VarParts::VALUE;
            } else {
                parsed_part += cur_char;
            }
            break;

        case VarParts::VALUE:
            if (cur_char == '=') {
                parsing = VarParts::NAME;
                continue;
            } else if (cur_char == ',') {
                var_type = VarType::LIST;
                values.emplace_back(std::move(parsed_part));
                parsed_part.clear();
            } else if (cur_char == oper.Separator()) {
                if (cur_char == '.' && pos < where->size() - 1 &&
                    (where->at(pos + 1) == cur_char || where->at(pos + 1) == ',')) {
                    // if current and next either '.' or ',' then greedy
                    parsed_part += cur_char;
                } else {
                    if (var.IsExploded() && var_type == VarType::STRING) {
                        var_type = VarType::LIST;
                    }
                    values.emplace_back(std::move(parsed_part));
                    parsed_part.clear();
                }
            } else {
                parsed_part += cur_char;
            }
            break;
        }

        ++pos;
    }
    switch (parsing) {
    case VarParts::NAME:
        names.emplace_back(std::move(parsed_part));
        if (!oper.EmptyEq()) {
            values.emplace_back("");
        }
        break;
    case VarParts::VALUE:
        values.emplace_back(std::move(parsed_part));
        break;
    }

    VarValue var_value(var_type);
    switch (var_type) {
    case VarType::UNDEFINED:
        break;

    case VarType::STRING:
        if (oper.Named()) {
            if (names.size() != 1) {
                // named string should have exactly one name
                return std::nullopt;
            }
            if (names[0] != var.Name()) {
                // named string start with its' name
                return std::nullopt;
            }
        }
        if (values.size() != 1) {
            // string should have exactly one value
            return std::nullopt;
        } else {
            var_value.Get<std::string>() = std::move(values[0]);
        }
        break;

    case VarType::LIST:
        var_value.Get<std::vector<std::string>>() = std::move(values);
        break;

    case VarType::DICT: {
        if (names.size() != values.size()) {
            // dict has unequal number of names and values
            return std::nullopt;
        }
        if (names_set.size() == 1 && names[0] == var.Name()) {
            // vars with same name as this variable
            var_value = VarValue(VarType::LIST);
            var_value.Get<std::vector<std::string>>() = std::move(values);
        } else if (names_set.size() == values.size()) {
            // vars with unique names
            auto& dict = var_value.Get<std::unordered_map<std::string, std::string>>();
            for (std::size_t i = 0; i < names.size(); ++i) {
                dict.emplace(std::move(names[i]), std::move(values[i]));
            }
        } else {
            // names/values pairs neiter unique nor the same
            return std::nullopt;
        }

    } break;
    }

    return var_value;
}

std::optional<URI::Template::Match> URI::Template::MatchExpression(const Expression& expression,
                                                                   const std::string& where, std::size_t start,
                                                                   std::size_t end, char terminator,
                                                                   std::unordered_map<std::string, VarValue>* values)
{
    if (start > end || start > where.size()) {
        // range is incorrect
        return std::nullopt;
    }

    std::size_t matched_vars = 0;
    std::optional<std::string> raw_value;
    const auto& exp_oper = expression.Oper();
    const auto& exp_vars = expression.Vars();

    bool terminate = false;
    std::size_t pos = start;
    ExprParts matching = ExprParts::OPERATOR;

    auto match_and_store = [&exp_vars, &exp_oper, &values](std::optional<std::string>&& raw_value,
                                                           std::size_t& pos) -> bool {
        if (exp_oper.Named() && raw_value && pos != exp_vars.size() - 1) {
            // if it is named, lookup for closest same-name variable,
            // variables in-between will be undefined
            bool found = false;
            std::size_t new_pos = pos;
            while (new_pos < exp_vars.size()) {
                const auto& var = exp_vars[new_pos];

                if (var.IsExploded() || StartsWith(*raw_value, var.Name())) {
                    found = true;
                    break;
                }
                if (values != nullptr) {
                    // fill skipped with 'undefined'
                    values->emplace(var.Name(), VarValue(VarType::UNDEFINED));
                }

                ++new_pos;
            }
            if (found) {
                pos = new_pos;
            }
        }

        const auto& variable = exp_vars[pos];
        auto var_value = MatchVarValue(variable, exp_oper, std::move(raw_value));
        if (!var_value) {
            return false;
        }
        if (values != nullptr) {
            values->insert_or_assign(variable.Name(), std::move(*var_value));
        }
        ++pos;
        return true;
    };

    while (pos < where.size() && pos < end) {
        char cur_char = where[pos];

        switch (matching) {
        case ExprParts::OPERATOR:
            if (exp_oper.StartExpanded() && cur_char != exp_oper.Start()) {
                // operator start not matched
                // possible if all variables are undefined
                terminate = true; // stop right now
                break;
            } else {
                matching = ExprParts::VARIABLE;
                // variables start from next char if operator expanded
                if (exp_oper.StartExpanded()) {
                    // can't be undefined after after operator symbol
                    raw_value = std::string();
                    break;
                }
            }
            [[fallthrough]];

        case ExprParts::VARIABLE: {
            // clang-format off
            bool char_allowed = exp_oper.Reserved() ||
                                Variable::kValueChars.count(cur_char) ||
                                (cur_char == '=' && (exp_oper.Named() || exp_vars[matched_vars].IsExploded()));
            // clang-format on
            if (cur_char == terminator) {
                // it was last variable before terminator
                if (!match_and_store(std::move(raw_value), matched_vars)) {
                    return std::nullopt;
                }
                terminate = true; // stop right now
            } else if (char_allowed && matched_vars == exp_vars.size() - 1) {
                // greedy for the last variable
                if (!raw_value) {
                    raw_value = std::string();
                }
                *raw_value += cur_char;
            } else if (cur_char == exp_oper.Separator()) {
                if (exp_vars[matched_vars].IsExploded()) {
                    // separator is a part of composite
                    *raw_value += cur_char;
                } else {
                    // can't be undefined before or after separator
                    if (!raw_value) {
                        raw_value = std::string();
                    }
                    if (!match_and_store(std::move(raw_value), matched_vars)) {
                        return std::nullopt;
                    }
                    // note that a moved-from std::optional still contains a value:
                    // https://en.cppreference.com/w/cpp/utility/optional/operator%3D
                    raw_value->clear();
                }
            } else if (char_allowed) {
                if (!raw_value) {
                    raw_value = std::string();
                }
                *raw_value += cur_char;
            } else {
                // character is not allowed
                terminate = true; // stop right now
            }
        } break;
        }

        if (terminate || matched_vars == exp_vars.size()) {
            break;
        }
        ++pos;
    }
    // fill the last one parsed
    if (raw_value && matched_vars < exp_vars.size()) {
        if (!match_and_store(std::move(raw_value), matched_vars)) {
            return std::nullopt;
        }
    }
    // following variables will be undefined
    for (; values && matched_vars < exp_vars.size(); ++matched_vars) {
        values->emplace(exp_vars[matched_vars].Name(), VarValue(VarType::UNDEFINED));
    }
    return Match(start, pos);
}

bool URI::Template::MatchURI(const Template& uri_template, const std::string& uri,
                             std::unordered_map<std::string, VarValue>* values)
{
    if (uri_template.Size() == 0) {
        return uri.empty() ? true : false;
    }

    std::vector<std::optional<Match>> matches(uri_template.Size(), std::nullopt);

    // find all literals first
    std::size_t pos = 0;
    for (std::size_t i = 0; i < uri_template.Size(); ++i) {
        const auto& part = uri_template[i];
        if (part.Type() == PartType::EXPRESSION) {
            // skip expressions
            continue;
        }

        auto match = MatchLiteral(part.Get<Literal>(), uri, pos, i == 0);
        if (!match) {
            return false;
        }

        pos = match->End();
        matches[i] = std::move(match);
    }

    // match expressions
    pos = 0;
    for (std::size_t i = 0; i < uri_template.Size(); ++i) {
        const auto& part = uri_template[i];

        // take the start as previous match end
        if (matches[i].has_value()) {
            pos = matches[i]->End();
            continue;
        }
        // lookup for nearest match to take the end or terminator
        char terminator = '\0';
        bool next_matched = false;
        std::size_t end = std::string::npos;
        const auto& cur_expr = part.Get<Expression>();
        for (std::size_t j = i + 1; j < uri_template.Size(); ++j) {
            if (matches[j].has_value()) {
                end = matches[j]->Start();
                if (j == i + 1) {
                    // next part is a match
                    next_matched = true;
                }
                break;
            } else {
                // next not matched can be only expression (all literals have been matched)
                const auto& next_expr_oper = uri_template[j].Get<Expression>().Oper();
                // terminator is next operator start character if it differs from current separator
                if (!next_expr_oper.StartExpanded()) {
                    continue;
                }
                if (next_expr_oper.Start() == cur_expr.Oper().Separator()) {
                    continue;
                }
                terminator = next_expr_oper.Start();
                break;
            }
        }

        auto match = MatchExpression(cur_expr, uri, pos, end, terminator, values);
        if (!match) {
            return false;
        }
        if (next_matched && match->End() != matches[i + 1]->Start()) {
            // doesn't fill whole space till next match
            return false;
        }
        if (i == uri_template.Size() - 1 && match->End() != uri.size()) {
            // last match doesn't fill till the end
            return false;
        }

        pos = match->End();
        matches[i] = std::move(match);
    }

    return true;
}
