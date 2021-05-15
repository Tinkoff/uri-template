#include "uri-template/Parser.h"
#include "uri-template/Modifier.h"

namespace {

enum class ExprParts
{
    OPERATOR,
    VARIABLE,
    MODIFIER,
    LENGTH
};

enum class TemplParts
{
    LITERAL,
    EXPRESSION,
};

} // namespace

URI::Template::Expression URI::Template::ParseExpression(const std::string& expr_string)
{
    std::string var_len;
    std::string var_name;
    std::vector<Variable> variables;
    std::shared_ptr<Modifier> var_mod;
    std::shared_ptr<Operator> expr_oper;

    auto cursor = expr_string.begin();
    ExprParts parsing = ExprParts::OPERATOR;
    while (cursor != expr_string.end()) {
        char cur_char = *cursor;

        switch (parsing) {
        case ExprParts::OPERATOR:
            for (const auto& known_oper : URI::Template::KNOWN_OPERATORS) {
                if (cur_char == known_oper->Start()) {
                    expr_oper = known_oper;
                    break;
                }
            }
            parsing = ExprParts::VARIABLE;
            if (expr_oper) {
                break;
            }
            [[fallthrough]];

        case ExprParts::VARIABLE:
            if (Variable::kNameChars.count(cur_char)) {
                var_name += cur_char;
                break;
            } else {
                if (var_name.empty()) {
                    throw std::runtime_error("no variable name found");
                }
                parsing = ExprParts::MODIFIER;
            }
            [[fallthrough]];

        case ExprParts::MODIFIER:
            for (const auto& known_mod : URI::Template::KNOWN_MODIFIERS) {
                if (cur_char == known_mod->Start()) {
                    var_mod = known_mod;
                    break; // out of for loop
                }
            }
            parsing = ExprParts::LENGTH;
            if (var_mod) {
                break;
            }
            [[fallthrough]];

        case ExprParts::LENGTH:
            if (var_mod && var_mod->Type() == ModifierType::LENGTH && ModLength::IsDigit(cur_char)) {
                var_len += cur_char;
            } else if (cur_char == ',') {
                variables.emplace_back(std::move(var_name), std::move(var_mod), ModLength::ToNumber(var_len));
                var_name.clear();
                var_len.clear();
                var_mod = nullptr;
                parsing = ExprParts::VARIABLE;
            } else {
                throw std::runtime_error(std::string("character '") + cur_char + "' is not allowed");
            }
            break;
        }

        ++cursor;
    }
    // push the last one
    if (!var_name.empty()) {
        variables.emplace_back(std::move(var_name), std::move(var_mod), ModLength::ToNumber(var_len));
    }
    return Expression(std::move(expr_oper), std::move(variables));
}

URI::Template::Template URI::Template::ParseTemplate(const std::string& tmpl_string)
{
    Template result;
    std::string cur_string;

    auto cursor = tmpl_string.begin();
    TemplParts parsing = TemplParts::LITERAL;
    while (cursor != tmpl_string.end()) {
        char cur_char = *cursor;

        switch (parsing) {
        case TemplParts::LITERAL:
            if (Literal::kNotAllowedChars.count(cur_char) == 0) {
                cur_string += cur_char;
            } else if (cur_char == '{') {
                if (!cur_string.empty()) {
                    result.EmplaceBack(std::move(cur_string));
                    cur_string.clear();
                }
                parsing = TemplParts::EXPRESSION;
            } else {
                throw std::runtime_error(std::string("character '") + cur_char + "' is not allowed");
            }
            break;

        case TemplParts::EXPRESSION:
            if (cur_char == '}') {
                if (cur_string.empty()) {
                    throw std::runtime_error("expression is empty");
                }

                result.EmplaceBack(ParseExpression(cur_string));
                cur_string.clear();
                parsing = TemplParts::LITERAL;
            } else {
                cur_string += cur_char;
            }
            break;
        }

        ++cursor;
    }
    // push the last one
    if (!cur_string.empty()) {
        switch (parsing) {
        case TemplParts::LITERAL:
            result.EmplaceBack(std::move(cur_string));
            break;

        case TemplParts::EXPRESSION:
            throw std::runtime_error("closing template parenthesis is missing");
            break;
        }
    }
    return result;
}
