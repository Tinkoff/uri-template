#pragma once

#include <memory>
#include <vector>

namespace URI {
namespace Template {

/**
 * Operator type enumerator.
 * Describes type of the operator expression has.
 */
enum class OperatorType
{
    NONE, /**< no operator */
    RESERVED_CHARS, /**< reserved character strings */
    FRAGMENT, /**< fragment identifiers prefixed by "#"; */
    LABEL, /**< name labels or extensions prefixed by "." */
    PATH, /**< path segments prefixed by "/" */
    PATH_PARAMETER, /**< path parameter name or name=value pairs prefixed by ";" */
    QUERY, /**< query component beginning with "?" and consisting of name=value pairs separated by "&" */
    QUERY_CONTINUE /**< continuation of query-style &name=value pairs */
};

/**
 * Abstract base to hold definition of expression operator.
 * Defines interface to represent various operators defined in RFC 6570.
 * This table used to fill definitions (https://tools.ietf.org/html/rfc6570#page-31):
 *       | noop  |   +   |   .   |   /   |   ;  |   ?  |   &  |   #
 * ------|-------|-------|-------|-------|------|------|------|-------
 * first |  ""   |  ""   |  "."  |  "/"  |  ";" |  "?" |  "&" |  "#"
 * sep   |  ","  |  ","  |  "."  |  "/"  |  ";" |  "&" |  "&" |  ","
 * named | false | false | false | false | true | true | true | false
 * ifemp |  ""   |  ""   |  ""   |  ""   |  ""  |  "=" |  "=" |  ""
 * allow |   U   |  U+R  |   U   |   U   |   U  |   U  |   U  |  U+R
 */
class Operator
{
public:
    static const char kNoCharacter; ///< Constant used to represent absent character. Equals '\0'.

    /// Destructor.
    virtual ~Operator() = default;

    /// Get type of the operator.
    virtual OperatorType Type() const = 0;
    /// Get starting character of the operator.
    virtual char Start() const = 0;
    /// Get the first character used for operator expansion.
    virtual char First() const = 0;
    /// Get the separator character used for operator expansion.
    virtual char Separator() const = 0;
    /// Check if the operator using named variables or not.
    virtual bool Named() const = 0;
    /// Check if the operator using '=' sign for empty variables.
    virtual bool EmptyEq() const = 0;
    /// Check if the operator allows reserved characters for variable value.
    virtual bool Reserved() const = 0;
    /// Check if the operator's start character also used in expansion.
    virtual bool StartExpanded() const = 0;
};

/**
 * No operations operator.
 * This class represents absence of operator for an expression.
 * Simple string expansion is the default expression type when no operator is given.
 */
class OpNoop: public Operator
{
public:
    /// Constructor.
    OpNoop() = default;
    /// Destructor.
    virtual ~OpNoop() = default;

    /**
     * Get type of the operator.
     *
     * @returns OperatorType::NONE.
     */
    virtual OperatorType Type() const override;

    /**
     * Get starting character of the operator.
     *
     * @throws std::runtime_error on call.
     */
    virtual char Start() const override;

    /**
     * Get the first character used for operator expansion.
     *
     * @returns Operator::kNoCharacter.
     */
    virtual char First() const override;

    /**
     * Get the separator character used for operator expansion.
     *
     * @returns ','.
     */
    virtual char Separator() const override;

    /**
     * Check if the operator using named variables or not.
     *
     * @returns false.
     */
    virtual bool Named() const override;

    /**
     * Check if the operator using '=' sign for empty variables.
     *
     * @returns false.
     */
    virtual bool EmptyEq() const override;

    /**
     * Check if the operator allows reserved characters for variable value.
     *
     * @returns false.
     */
    virtual bool Reserved() const override;

    /**
     * Check if the operator's start character also used in expansion.
     *
     * @returns false.
     */
    virtual bool StartExpanded() const override;
};

/**
 * Reserved characters expansion operator.
 * Reserved expansion, as indicated by the plus ("+") operator for Level 2 and above templates,
 *  is identical to simple string expansion except that the substituted values may also contain
 *  pct-encoded triplets and characters in the reserved set.
 */
class OpReservedChars: public OpNoop
{
public:
    /// Constructor.
    OpReservedChars() = default;
    /// Destructor.
    virtual ~OpReservedChars() = default;

    /**
     * Get type of the operator.
     *
     * @returns OperatorType::RESERVED_CHARS.
     */
    virtual OperatorType Type() const override;

    /**
     * Get starting character of the operator.
     *
     * @returns '+'.
     */
    virtual char Start() const override;

    /**
     * Check if the operator allows reserved characters for variable value.
     *
     * @returns true.
     */
    virtual bool Reserved() const override;
};

/**
 * Fragment expansion operator.
 * Fragment expansion, as indicated by the crosshatch ("#") operator for Level 2 and above templates,
 *  is identical to reserved expansion except that a crosshatch character (fragment delimiter) is appended
 *  first to the result string if any of the variables are defined.
 */
class OpFragment: public Operator
{
public:
    /// Constructor.
    OpFragment() = default;
    /// Destructor.
    virtual ~OpFragment() = default;

    /**
     * Get type of the operator.
     *
     * @returns OperatorType::FRAGMENT.
     */
    virtual OperatorType Type() const override;

    /**
     * Get starting character of the operator.
     *
     * @returns '#'.
     */
    virtual char Start() const override;

    /**
     * Get the first character used for operator expansion.
     *
     * @returns '#'.
     */
    virtual char First() const override;

    /**
     * Get the separator character used for operator expansion.
     *
     * @returns ','.
     */
    virtual char Separator() const override;

    /**
     * Check if the operator using named variables or not.
     *
     * @returns false.
     */
    virtual bool Named() const override;

    /**
     * Check if the operator using '=' sign for empty variables.
     *
     * @returns false.
     */
    virtual bool EmptyEq() const override;

    /**
     * Check if the operator allows reserved characters for variable value.
     *
     * @returns true.
     */
    virtual bool Reserved() const override;

    /**
     * Check if the operator's start character also used in expansion.
     *
     * @returns true.
     */
    virtual bool StartExpanded() const override;
};

/**
 * Label expansion operator.
 * Label expansion, as indicated by the dot (".") operator for Level 3 and above templates,
 *  is useful for describing URI spaces with varying domain names or path selectors (e.g., filename extensions).
 */
class OpLabel: public Operator
{
public:
    /// Constructor.
    OpLabel() = default;
    /// Destructor.
    virtual ~OpLabel() = default;

    /**
     * Get type of the operator.
     *
     * @returns OperatorType::LABEL.
     */
    virtual OperatorType Type() const override;

    /**
     * Get starting character of the operator.
     *
     * @returns '.'.
     */
    virtual char Start() const override;

    /**
     * Get the first character used for operator expansion.
     *
     * @returns '.'.
     */
    virtual char First() const override;

    /**
     * Get the separator character used for operator expansion.
     *
     * @returns '.'.
     */
    virtual char Separator() const override;

    /**
     * Check if the operator using named variables or not.
     *
     * @returns false.
     */
    virtual bool Named() const override;

    /**
     * Check if the operator using '=' sign for empty variables.
     *
     * @returns false.
     */
    virtual bool EmptyEq() const override;

    /**
     * Check if the operator allows reserved characters for variable value.
     *
     * @returns false.
     */
    virtual bool Reserved() const override;

    /**
     * Check if the operator's start character also used in expansion.
     *
     * @returns true.
     */
    virtual bool StartExpanded() const override;
};

/**
 * Path segment expansion operator.
 * Path segment expansion, as indicated by the slash ("/") operator in Level 3 and above templates,
 * is useful for describing URI path hierarchies.
 */
class OpPath: public Operator
{
public:
    /// Constructor.
    OpPath() = default;
    /// Destructor.
    virtual ~OpPath() = default;

    /**
     * Get type of the operator.
     *
     * @returns OperatorType::PATH.
     */
    virtual OperatorType Type() const override;

    /**
     * Get starting character of the operator.
     *
     * @returns '/'.
     */
    virtual char Start() const override;

    /**
     * Get the first character used for operator expansion.
     *
     * @returns '/'.
     */
    virtual char First() const override;

    /**
     * Get the separator character used for operator expansion.
     *
     * @returns '/'.
     */
    virtual char Separator() const override;

    /**
     * Check if the operator using named variables or not.
     *
     * @returns false.
     */
    virtual bool Named() const override;

    /**
     * Check if the operator using '=' sign for empty variables.
     *
     * @returns false.
     */
    virtual bool EmptyEq() const override;

    /**
     * Check if the operator allows reserved characters for variable value.
     *
     * @returns false.
     */
    virtual bool Reserved() const override;

    /**
     * Check if the operator's start character also used in expansion.
     *
     * @returns true.
     */
    virtual bool StartExpanded() const override;
};

/**
 * Path parameter expansion operator.
 * Path-style parameter expansion, as indicated by the semicolon (";") operator in Level 3 and above templates,
 *  is useful for describing URI path parameters, such as "path;property" or "path;name=value".
 */
class OpPathParam: public Operator
{
public:
    /// Constructor.
    OpPathParam() = default;
    /// Destructor.
    virtual ~OpPathParam() = default;

    /**
     * Get type of the operator.
     *
     * @returns OperatorType::PATH_PARAMETER.
     */
    virtual OperatorType Type() const override;

    /**
     * Get starting character of the operator.
     *
     * @returns ';'.
     */
    virtual char Start() const override;

    /**
     * Get the first character used for operator expansion.
     *
     * @returns ';'.
     */
    virtual char First() const override;

    /**
     * Get the separator character used for operator expansion.
     *
     * @returns ';'.
     */
    virtual char Separator() const override;

    /**
     * Check if the operator using named variables or not.
     *
     * @returns true.
     */
    virtual bool Named() const override;

    /**
     * Check if the operator using '=' sign for empty variables.
     *
     * @returns false.
     */
    virtual bool EmptyEq() const override;

    /**
     * Check if the operator allows reserved characters for variable value.
     *
     * @returns false.
     */
    virtual bool Reserved() const override;

    /**
     * Check if the operator's start character also used in expansion.
     *
     * @returns true.
     */
    virtual bool StartExpanded() const override;
};

/**
 * Query expansion operator.
 * Form-style query expansion, as indicated by the question-mark ("?") operator in Level 3 and above templates,
 *  is useful for describing an entire optional query component.
 */
class OpQuery: public Operator
{
public:
    /// Constructor.
    OpQuery() = default;
    /// Destructor.
    virtual ~OpQuery() = default;

    /**
     * Get type of the operator.
     *
     * @returns OperatorType::QUERY.
     */
    virtual OperatorType Type() const override;

    /**
     * Get starting character of the operator.
     *
     * @returns '?'.
     */
    virtual char Start() const override;

    /**
     * Get the first character used for operator expansion.
     *
     * @returns '?'.
     */
    virtual char First() const override;

    /**
     * Get the separator character used for operator expansion.
     *
     * @returns '&'.
     */
    virtual char Separator() const override;

    /**
     * Check if the operator using named variables or not.
     *
     * @returns true.
     */
    virtual bool Named() const override;

    /**
     * Check if the operator using '=' sign for empty variables.
     *
     * @returns true.
     */
    virtual bool EmptyEq() const override;

    /**
     * Check if the operator allows reserved characters for variable value.
     *
     * @returns false.
     */
    virtual bool Reserved() const override;

    /**
     * Check if the operator's start character also used in expansion.
     *
     * @returns true.
     */
    virtual bool StartExpanded() const override;
};

/**
 * Query continuation expansion operator.
 * Form-style query continuation, as indicated by the ampersand ("&") operator in Level 3 and above templates,
 *  is useful for describing optional &name=value pairs in a template that already contains a
 *   literal query component with fixed parameters.
 */
class OpQueryContinue: public Operator
{
public:
    /// Constructor.
    OpQueryContinue() = default;
    /// Destructor.
    virtual ~OpQueryContinue() = default;

    /**
     * Get type of the operator.
     *
     * @returns OperatorType::QUERY_CONTINUE.
     */
    virtual OperatorType Type() const override;

    /**
     * Get starting character of the operator.
     *
     * @returns '&'.
     */
    virtual char Start() const override;

    /**
     * Get the first character used for operator expansion.
     *
     * @returns '&'.
     */
    virtual char First() const override;

    /**
     * Get the separator character used for operator expansion.
     *
     * @returns '&'.
     */
    virtual char Separator() const override;

    /**
     * Check if the operator using named variables or not.
     *
     * @returns true.
     */
    virtual bool Named() const override;

    /**
     * Check if the operator using '=' sign for empty variables.
     *
     * @returns true.
     */
    virtual bool EmptyEq() const override;

    /**
     * Check if the operator allows reserved characters for variable value.
     *
     * @returns false.
     */
    virtual bool Reserved() const override;

    /**
     * Check if the operator's start character also used in expansion.
     *
     * @returns true.
     */
    virtual bool StartExpanded() const override;
};

/// Noop operator instance to use for expressions.
inline const std::shared_ptr<Operator> kNoopOperator = std::make_shared<OpNoop>();
// clang-format off
/// Collection of different operator instances to use for expressions.
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
