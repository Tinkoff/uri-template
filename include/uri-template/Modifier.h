#pragma once

#include <memory>
#include <string>
#include <vector>

namespace URI {
namespace Template {

enum class ModifierType
{
    NONE, // no modifier
    LENGTH, // variable expansion is limited in length
    EXPLODE // expansion process is applied to each member of a composite value
};

class Modifier
{
public:
    virtual ~Modifier() = default;

    virtual ModifierType Type() const = 0;
    virtual char Start() const = 0;
};

class ModNoop: public Modifier
{
public:
    ModNoop() = default;
    virtual ~ModNoop() = default;

    virtual ModifierType Type() const override;
    virtual char Start() const override;
};

class ModLength: public Modifier
{
public:
    ModLength() = default;
    virtual ~ModLength() = default;

    virtual ModifierType Type() const override;
    virtual char Start() const override;

    // Fast alternative to std::isdigit
    static bool IsDigit(char c);
    // Fast alternative to std::atoi
    static unsigned ToNumber(const std::string& num_str);
};

class ModExplode: public Modifier
{
public:
    ModExplode() = default;
    virtual ~ModExplode() = default;

    virtual ModifierType Type() const override;
    virtual char Start() const override;
};

inline const std::shared_ptr<Modifier> kNoopModifier = std::make_shared<ModNoop>();
inline const std::vector<std::shared_ptr<Modifier>> kKnownModifiers = {
    std::make_shared<ModLength>(),
    std::make_shared<ModExplode>(),
};

} // namespace Template
} // namespace URI
