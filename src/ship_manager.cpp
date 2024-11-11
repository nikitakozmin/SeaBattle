#include <iostream>
#include "ship_manager.hpp"

ShipManager::ShipManager(Field &field, std::map<unsigned int, unsigned int> ships)
{
    manager_field = &field;
    for (const auto& [length, number]: ships)
    {
        for (int i=0; i<number; i++)
        {
            unplaced_ships.push_back(Ship(length));
        }
    }
}

std::vector<Ship> &ShipManager::get_unplaced_ships()
{
    return unplaced_ships;
}
std::vector<Ship> &ShipManager::get_placed_ships()
{
    return placed_ships;
}

bool ShipManager::place_ship(Ship ship, unsigned int y, unsigned int x, char ship_orientation)
{
    if (ship_orientation != 'h' && ship_orientation != 'v')
    {
        throw std::invalid_argument("The orientation of the ship must be h-horizontal or v-vertical");
    }
    placed_ships.push_back(ship);
    if (!(*manager_field).place_ship(placed_ships.back(), y, x, ship_orientation))
    {
        placed_ships.pop_back();
        return 0;
    }
    return 1;
}

void ShipManager::updating_dependencies_placed_ships()
{
    for (unsigned int i = 0; i<get_placed_ships().size(); ++i)
    {
        for (unsigned int j = 0; j<get_placed_ships()[i].get_length(); ++j)
        {
            get_placed_ships()[i].get_segment(j).update_owned_ship(&get_placed_ships()[i]);
        }
    }
}