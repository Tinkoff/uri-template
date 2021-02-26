#include "uri-template/Expander.h"

#include <iomanip>
#include <sstream>

std::string URI::Template::PctEncode(const std::string& value, bool allow_reserved, std::size_t max_len)
{
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;
    if (max_len > value.size()) {
        max_len = value.size();
    }

    for (std::size_t i = 0; i < max_len; ++i) {
        const auto& c = value[i];
        if (c == '%' && i + 2 < max_len && std::isxdigit(value[i + 1]) && std::isxdigit(value[i + 2])) {
            encoded << c;
            // already encoded triplets count as one character
            if (max_len + 2 <= value.size()) {
                max_len += 2;
            }
            continue;
        }
        if (c != '%' && c != ',' && URI::Template::Variable::kValueChars.count(c)) {
            encoded << c;
            continue;
        }
        if (allow_reserved && URI::Template::Variable::kReservedChars.count(c)) {
            encoded << c;
            continue;
        }
        // Any other characters are percent-encoded
        encoded << std::uppercase;
        encoded << '%' << std::setw(2) << int((unsigned char)c);
        encoded << std::nouppercase;
    }

    return encoded.str();
}

std::string URI::Template::ExpandExpression(const URI::Template::Expression& expression,
                                            const std::unordered_map<std::string, URI::Template::VarValue>& values)
{
    std::string result;
    const Operator& oper = expression.Oper();
    const std::vector<Variable>& variables = expression.Vars();

    if (variables.empty()) {
        throw std::runtime_error("expression is empty");
    }

    bool first = true;
    auto expand_variable = [&first, &oper](const std::string& name, const std::string& value, int length, bool named,
                                           bool encode) {
        std::string result;

        if (first) {
            first = false;
            if (oper.First() != Operator::kNoCharacter) {
                result += oper.First();
            }
        } else {
            result += oper.Separator();
        }

        if (named) {
            result += name;

            if (!value.empty() || oper.EmptyEq()) {
                result += '=';
            }
        }

        if (length >= 0) {
            result += encode ? PctEncode(value, oper.Reserved(), length) : value.substr(0, length);
        } else {
            result += encode ? PctEncode(value, oper.Reserved()) : value;
        }

        return result;
    };

    const auto undefined_value = VarValue(VarType::UNDEFINED);
    for (const Variable& var : variables) {
        const std::string& var_name = var.Name();
        const Modifier& var_modifier = var.Mod();

        const VarValue* var_value = &undefined_value;
        const auto value_lookup = values.find(var_name);
        if (value_lookup != values.end()) {
            var_value = &value_lookup->second;
        }

        switch (var_value->Type()) {
        case VarType::UNDEFINED:
            break;
        case VarType::STRING:
            result += expand_variable(var_name, var_value->Get<std::string>(),
                                      var_modifier.Type() == ModifierType::LENGTH ? var.Length() : -1, oper.Named(),
                                      true);
            break;
        case VarType::LIST:
            if (var_modifier.Type() == ModifierType::EXPLODE) {
                for (const auto& list_item : var_value->Get<std::vector<std::string>>()) {
                    result += expand_variable(var_name, list_item, -1, oper.Named(), true);
                }
            } else {
                bool first_item = true;
                std::string join_value;
                for (const auto& list_item : var_value->Get<std::vector<std::string>>()) {
                    if (!first_item) {
                        join_value += ',';
                    }
                    join_value += PctEncode(list_item, oper.Reserved());
                    first_item = false;
                }
                result += expand_variable(var_name, join_value, -1, oper.Named(), false);
            }
            break;
        case VarType::DICT:
            if (var_modifier.Type() == ModifierType::EXPLODE) {
                for (const auto& [name, val] : var_value->Get<std::unordered_map<std::string, std::string>>()) {
                    result += expand_variable(PctEncode(name, oper.Reserved()), val, -1, true, true);
                }
            } else {
                bool first_item = true;
                std::string join_value;
                for (const auto& [name, val] : var_value->Get<std::unordered_map<std::string, std::string>>()) {
                    if (!first_item) {
                        join_value += ',';
                    }
                    join_value += PctEncode(name, oper.Reserved()) + "," + PctEncode(val, oper.Reserved());
                    first_item = false;
                }
                result += expand_variable(var_name, join_value, -1, oper.Named(), false);
            }
            break;
        }
    }

    return result;
}

std::string URI::Template::ExpandTemplate(const URI::Template::Template& uri_template,
                                          const std::unordered_map<std::string, URI::Template::VarValue>& values)
{
    std::string result;

    for (const auto& part : uri_template.Parts()) {
        switch (part.Type()) {
        case PartType::LITERAL:
            result += part.Get<Literal>().String();
            break;
        case PartType::EXPRESSION:
            result += ExpandExpression(part.Get<Expression>(), values);
        }
    }

    return result;
}
