#include <algorithm>
#include <random>
#include "ability_manager.hpp"
#include "double_damage.hpp"
#include "scanner.hpp"
#include "bombardment.hpp"
#include "local_exceptions.hpp"

AbilityManager::AbilityManager(Field &field, std::vector<Ship> &placed_ships)
{
    manager_field = &field;
    ships = &placed_ships;
    ability_vector.push_back(std::make_unique<DoubleDamage>(&manager_field));
    ability_vector.push_back(std::make_unique<Scanner>(&manager_field));
    ability_vector.push_back(std::make_unique<Bombardment>(&ships));
    std::shuffle(ability_vector.begin(), ability_vector.end(), std::mt19937(std::random_device{}()));
    for (auto &ptr : ability_vector)
    {
        ability_queue.push(ptr->clone());
    }
}

void AbilityManager::add_random_ability()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, ability_vector.size() - 1);
    size_t random_index = dist(gen);
    ability_queue.push(ability_vector[random_index].get()->clone());
}

std::unique_ptr<Ability> AbilityManager::get_ability()
{
    if (ability_queue.empty())
    {
        throw AbilityException();
    }
    std::unique_ptr<Ability> ptr = std::move(ability_queue.front());
    ability_queue.pop();
    return ptr;
}

void AbilityManager::set_new_parameters(Field &new_manager_field, std::vector<Ship> &new_ships)
{
    manager_field = &new_manager_field;
    ships = &new_ships;
}

void AbilityManager::add_ability(std::string &str)
{
    for (auto &ptr : ability_vector)
    {
        if (str == std::string(typeid(*ptr).name()))
        {
            ability_queue.push(ptr->clone());
            break;
        }
    }
}
