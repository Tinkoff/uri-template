#pragma once

#include <memory>
#include <vector>

namespace URI {
namespace Template {

enum class OperatorType
{
    NONE, // no operator
    RESERVED_CHARS, // Reserved character strings
    FRAGMENT, // Fragment identifiers prefixed by "#";
    LABEL, // name labels or extensions prefixed by "."
    PATH, // path segments prefixed by "/"
    PATH_PARAMETER, // path parameter name or name=value pairs prefixed by ";"
    QUERY, // query component beginning with "?" and consisting of name=value pairs separated by "&"
    QUERY_CONTINUE // continuation of query-style &name=value pairs
};

// RFC 6570
// .------------------------------------------------------------------.
// |         noop     +      .       /       ;      ?      &      #   |
// |------------------------------------------------------------------|
// | first |  ""     ""     "."     "/"     ";"    "?"    "&"    "#"  |
// | sep   |  ","    ","    "."     "/"     ";"    "&"    "&"    ","  |
// | named | false  false  false   false   true   true   true   false |
// | ifemp |  ""     ""     ""      ""      ""     "="    "="    ""   |
// | allow |   U     U+R     U       U       U      U      U     U+R  |
// `------------------------------------------------------------------'

class Operator
{
public:
    virtual ~Operator() = default;

    virtual OperatorType Type() const = 0;
    virtual char Start() const = 0;
    virtual char Separator() const = 0;
    virtual bool Named() const = 0;
    virtual bool EmptyEq() const = 0;
    virtual bool Reserved() const = 0;
    virtual bool StartExpanded() const = 0;
};

class OpNoop: public Operator
{
public:
    OpNoop() = default;
    virtual ~OpNoop() = default;

    virtual OperatorType Type() const override;
    virtual char Start() const override;
    virtual char Separator() const override;
    virtual bool Named() const override;
    virtual bool EmptyEq() const override;
    virtual bool Reserved() const override;
    virtual bool StartExpanded() const override;
};

class OpReservedChars: public OpNoop
{
public:
    OpReservedChars() = default;
    virtual ~OpReservedChars() = default;

    virtual OperatorType Type() const override;
    virtual char Start() const override;
    virtual bool Reserved() const override;
};

class OpFragment: public Operator
{
public:
    OpFragment() = default;
    virtual ~OpFragment() = default;

    virtual OperatorType Type() const override;
    virtual char Start() const override;
    virtual char Separator() const override;
    virtual bool Named() const override;
    virtual bool EmptyEq() const override;
    virtual bool Reserved() const override;
    virtual bool StartExpanded() const override;
};

class OpLabel: public Operator
{
public:
    OpLabel() = default;
    virtual ~OpLabel() = default;

    virtual OperatorType Type() const override;
    virtual char Start() const override;
    virtual char Separator() const override;
    virtual bool Named() const override;
    virtual bool EmptyEq() const override;
    virtual bool Reserved() const override;
    virtual bool StartExpanded() const override;
};

class OpPath: public Operator
{
public:
    OpPath() = default;
    virtual ~OpPath() = default;

    virtual OperatorType Type() const override;
    virtual char Start() const override;
    virtual char Separator() const override;
    virtual bool Named() const override;
    virtual bool EmptyEq() const override;
    virtual bool Reserved() const override;
    virtual bool StartExpanded() const override;
};

class OpPathParam: public Operator
{
public:
    OpPathParam() = default;
    virtual ~OpPathParam() = default;

    virtual OperatorType Type() const override;
    virtual char Start() const override;
    virtual char Separator() const override;
    virtual bool Named() const override;
    virtual bool EmptyEq() const override;
    virtual bool Reserved() const override;
    virtual bool StartExpanded() const override;
};

class OpQuery: public Operator
{
public:
    OpQuery() = default;
    virtual ~OpQuery() = default;

    virtual OperatorType Type() const override;
    virtual char Start() const override;
    virtual char Separator() const override;
    virtual bool Named() const override;
    virtual bool EmptyEq() const override;
    virtual bool Reserved() const override;
    virtual bool StartExpanded() const override;
};

class OpQueryContinue: public Operator
{
public:
    OpQueryContinue() = default;
    virtual ~OpQueryContinue() = default;

    virtual OperatorType Type() const override;
    virtual char Start() const override;
    virtual char Separator() const override;
    virtual bool Named() const override;
    virtual bool EmptyEq() const override;
    virtual bool Reserved() const override;
    virtual bool StartExpanded() const override;
};

inline const std::shared_ptr<Operator> kNoopOperator = std::make_shared<OpNoop>();
// clang-format off
inline const std::vector<std::shared_ptr<Operator>> kKnownOperators = {
    std::make_shared<OpReservedChars>(),
    std::make_shared<OpFragment>(),
    std::make_shared<OpLabel>(),
    std::make_shared<OpPath>(),
    std::make_shared<OpPathParam>(),
    std::make_shared<OpQuery>(),
    std::make_shared<OpQueryContinue>(),
};
// clang-format on

} // namespace Template
} // namespace URI
