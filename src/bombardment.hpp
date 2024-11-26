#ifndef _BOMBARDMENT_
#define _BOMBARDMENT_

#include <vector>
#include "ability.hpp"

class Bombardment : public Ability
{
private:
    std::vector<Ship> **ships = nullptr;
public:
    Bombardment(std::vector<Ship> **placed_ships);
    void use_ability() override;
    std::unique_ptr<Ability> clone() const override;
};

#endif