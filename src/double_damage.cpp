#include "double_damage.hpp"

DoubleDamage::DoubleDamage(Field &field)
{
    manager_field = &field;
}

void DoubleDamage::use_ability()
{
    manager_field->set_damage(2);
}

std::unique_ptr<Ability> DoubleDamage::clone() const 
{
    return std::make_unique<DoubleDamage>(*this);
}
