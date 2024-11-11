#ifndef _ABILITIY_
#define _ABILITIY_

#include <memory>
#include "field.hpp"

class Ability
{
public:
    virtual ~Ability() = default;
    virtual void use_ability() = 0;
    virtual std::unique_ptr<Ability> clone() const = 0;
};

#endif
