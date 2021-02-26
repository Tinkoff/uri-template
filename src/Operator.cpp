#include "uri-template/Operator.h"

const char URI::Template::Operator::kNoCharacter = '\0';

URI::Template::OperatorType URI::Template::OpNoop::Type() const
{
    return OperatorType::NONE;
}

char URI::Template::OpNoop::Start() const
{
    throw std::runtime_error("NONE operator has no start");
}

char URI::Template::OpNoop::First() const
{
    return Operator::kNoCharacter;
}

char URI::Template::OpNoop::Separator() const
{
    return ',';
}

bool URI::Template::OpNoop::Named() const
{
    return false;
}

bool URI::Template::OpNoop::EmptyEq() const
{
    return false;
}

bool URI::Template::OpNoop::Reserved() const
{
    return false;
}

bool URI::Template::OpNoop::StartExpanded() const
{
    return false;
}

URI::Template::OperatorType URI::Template::OpReservedChars::Type() const
{
    return OperatorType::RESERVED_CHARS;
}

char URI::Template::OpReservedChars::Start() const
{
    return '+';
}

bool URI::Template::OpReservedChars::Reserved() const
{
    return true;
}

URI::Template::OperatorType URI::Template::OpFragment::Type() const
{
    return OperatorType::FRAGMENT;
}

char URI::Template::OpFragment::Start() const
{
    return '#';
}

char URI::Template::OpFragment::First() const
{
    return '#';
}

char URI::Template::OpFragment::Separator() const
{
    return ',';
}

bool URI::Template::OpFragment::Named() const
{
    return false;
}

bool URI::Template::OpFragment::EmptyEq() const
{
    return false;
}

bool URI::Template::OpFragment::Reserved() const
{
    return true;
}

bool URI::Template::OpFragment::StartExpanded() const
{
    return true;
}

URI::Template::OperatorType URI::Template::OpLabel::Type() const
{
    return OperatorType::LABEL;
}

char URI::Template::OpLabel::Start() const
{
    return '.';
}

char URI::Template::OpLabel::First() const
{
    return '.';
}

char URI::Template::OpLabel::Separator() const
{
    return '.';
}

bool URI::Template::OpLabel::Named() const
{
    return false;
}

bool URI::Template::OpLabel::EmptyEq() const
{
    return false;
}

bool URI::Template::OpLabel::Reserved() const
{
    return false;
}

bool URI::Template::OpLabel::StartExpanded() const
{
    return true;
}

URI::Template::OperatorType URI::Template::OpPath::Type() const
{
    return OperatorType::PATH;
}

char URI::Template::OpPath::Start() const
{
    return '/';
}

char URI::Template::OpPath::First() const
{
    return '/';
}

char URI::Template::OpPath::Separator() const
{
    return '/';
}

bool URI::Template::OpPath::Named() const
{
    return false;
}

bool URI::Template::OpPath::EmptyEq() const
{
    return false;
}

bool URI::Template::OpPath::Reserved() const
{
    return false;
}

bool URI::Template::OpPath::StartExpanded() const
{
    return true;
}

URI::Template::OperatorType URI::Template::OpPathParam::Type() const
{
    return OperatorType::PATH_PARAMETER;
}

char URI::Template::OpPathParam::Start() const
{
    return ';';
}

char URI::Template::OpPathParam::First() const
{
    return ';';
}

char URI::Template::OpPathParam::Separator() const
{
    return ';';
}

bool URI::Template::OpPathParam::Named() const
{
    return true;
}

bool URI::Template::OpPathParam::EmptyEq() const
{
    return false;
}

bool URI::Template::OpPathParam::Reserved() const
{
    return false;
}

bool URI::Template::OpPathParam::StartExpanded() const
{
    return true;
}

URI::Template::OperatorType URI::Template::OpQuery::Type() const
{
    return OperatorType::QUERY;
}

char URI::Template::OpQuery::Start() const
{
    return '?';
}

char URI::Template::OpQuery::First() const
{
    return '?';
}

char URI::Template::OpQuery::Separator() const
{
    return '&';
}

bool URI::Template::OpQuery::Named() const
{
    return true;
}

bool URI::Template::OpQuery::EmptyEq() const
{
    return true;
}

bool URI::Template::OpQuery::Reserved() const
{
    return false;
}

bool URI::Template::OpQuery::StartExpanded() const
{
    return true;
}

URI::Template::OperatorType URI::Template::OpQueryContinue::Type() const
{
    return OperatorType::QUERY_CONTINUE;
}

char URI::Template::OpQueryContinue::Start() const
{
    return '&';
}

char URI::Template::OpQueryContinue::First() const
{
    return '&';
}

char URI::Template::OpQueryContinue::Separator() const
{
    return '&';
}

bool URI::Template::OpQueryContinue::Named() const
{
    return true;
}

bool URI::Template::OpQueryContinue::EmptyEq() const
{
    return true;
}

bool URI::Template::OpQueryContinue::Reserved() const
{
    return false;
}

bool URI::Template::OpQueryContinue::StartExpanded() const
{
    return true;
}
