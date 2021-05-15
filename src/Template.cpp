#include "uri-template/Template.h"

/*
 * RFC6570:
 *      literals = any Unicode character except:
 *                  CTL, SP, """, "'", "%" (aside from pct-encoded),
 *                  "<", ">", "\", "^", "`", "{", "|", "}"
 */
// clang-format off
const std::unordered_set<char> URI::Template::Literal::kNotAllowedChars = {
    // CTL
    '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A',
    '\x0B', '\x0C', '\x0D', '\x0E', '\x0F', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15',
    '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F', '\x7F',
    // SP, """, "'", "<", ">", "\", "^", "`", "{", "|", "}"
    ' ', '\"', '\'', '<', '>', '\\', '^', '`', '{', '|', '}'
};
// clang-format on

URI::Template::Literal::Literal(std::string&& lit_string)
    : lit_string_(std::move(lit_string))
{
}

std::size_t URI::Template::Literal::Size() const
{
    return lit_string_.size();
}

const std::string& URI::Template::Literal::String() const
{
    return lit_string_;
}

bool URI::Template::Literal::operator==(const Literal& rhs) const
{
    return lit_string_ == rhs.lit_string_;
}

bool URI::Template::Literal::operator!=(const Literal& rhs) const
{
    return !(*this == rhs);
}

URI::Template::Expression::Expression(std::shared_ptr<Operator>&& oper, std::vector<Variable>&& variables)
    : oper_(std::move(oper))
    , var_list_(std::move(variables))
{
    if (!oper_) {
        oper_ = NOOP_OPERATOR;
    }
}

const URI::Template::Operator& URI::Template::Expression::Oper() const
{
    return *oper_;
}

const std::vector<URI::Template::Variable>& URI::Template::Expression::Vars() const
{
    return var_list_;
}

bool URI::Template::Expression::operator==(const Expression& rhs) const
{
    return oper_ == rhs.oper_ && var_list_ == rhs.var_list_;
}

bool URI::Template::Expression::operator!=(const Expression& rhs) const
{
    return !(*this == rhs);
}

bool URI::Template::Template::IsTemplated() const
{
    if (parts_.empty() || (parts_.size() == 1 && parts_.at(0).Type() == PartType::LITERAL)) {
        return false;
    }
    return true;
}

std::size_t URI::Template::Template::Size() const
{
    return parts_.size();
}

std::vector<URI::Template::Part>& URI::Template::Template::Parts()
{
    return parts_;
}

const std::vector<URI::Template::Part>& URI::Template::Template::Parts() const
{
    return parts_;
}

URI::Template::Part& URI::Template::Template::operator[](std::size_t pos)
{
    return parts_[pos];
}

const URI::Template::Part& URI::Template::Template::operator[](std::size_t pos) const
{
    return parts_.at(pos);
}
