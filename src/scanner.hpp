#ifndef _SCANNER_
#define _SCANNER_

#include "ability.hpp"

class Scanner : public Ability
{
private:
    Field *manager_field = nullptr;
public:
    Scanner(Field &field);
    void use_ability() override;
    std::unique_ptr<Ability> clone() const override;
};

#endif