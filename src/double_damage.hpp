#ifndef _DOUBLE_DAMAGE_
#define _DOUBLE_DAMAGE_

#include "ability.hpp"

class DoubleDamage : public Ability
{
private:
    Field **manager_field = nullptr;
public:
    DoubleDamage(Field **field);
    void use_ability() override;
    std::unique_ptr<Ability> clone() const override;
};

#endif
