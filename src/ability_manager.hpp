#ifndef _ABILITY_MANAGER_
#define _ABILITY_MANAGER_

#include <vector>
#include <queue>
#include <memory>
#include "field.hpp"
#include "ship_manager.hpp"
#include "ability.hpp"

class AbilityManager
{
private:
    Field *manager_field = nullptr;
    std::vector<Ship> *ships = nullptr;
    std::vector<std::unique_ptr<Ability>> ability_vector;
    std::queue<std::unique_ptr<Ability>> ability_queue;
public:
    AbilityManager(Field &field, std::vector<Ship> &placed_ships);
    void add_random_ability();
    std::unique_ptr<Ability> get_ability();
    void set_new_parameters(Field &new_manager_field, std::vector<Ship> &new_ships);
};

#endif
