#include <iostream>
#include <limits>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include "ship_manager.hpp"
#include "local_exceptions.hpp"

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

std::vector<Ship> &ShipManager::get_placed_ships()
{
    return placed_ships;
}

bool ShipManager::place_ship(Ship ship, unsigned int y, unsigned int x, char ship_orientation)
{
    placed_ships.push_back(ship);
    bool place_flag;
    try
    {
        place_flag = (*manager_field).place_ship(placed_ships.back(), y, x, ship_orientation);
    }
    catch(const std::exception& e){
        placed_ships.pop_back();
        throw;
    }
    if (!place_flag)
    {
        placed_ships.pop_back();
        return 0;
    }
    return 1;
}

void ShipManager::updating_dependencies_placed_ships()
{
    for (unsigned int i = 0; i<placed_ships.size(); ++i)
    {
        for (unsigned int j = 0; j<placed_ships[i].get_length(); ++j)
        {
            placed_ships[i].get_segment(j).update_owned_ship(&placed_ships[i]);
        }
    }
}

void ShipManager::placement_of_ships(bool is_random)
{
    unsigned int y;
    unsigned int x;
    char ship_orientation;
    bool place_flag;
    if (is_random)
    {
        unsigned int num_cells = manager_field->get_height() * manager_field->get_width();
        std::vector<int> one_d_coords(num_cells);
        std::iota(one_d_coords.begin(), one_d_coords.end(), 0); // Заполнение вектора последовательностью 0, 1, 2, ...
        std::shuffle(one_d_coords.begin(), one_d_coords.end(), std::mt19937(std::random_device{}()));
        std::vector<char> ship_orientations{'h', 'v'};
        unsigned long int i = 0;
        unsigned long int p;
        unsigned int index_orientation;
        while (!unplaced_ships.empty())
        {
            if (i == num_cells)
            {
                throw std::range_error("There is not enough space to install a new ship");
            }
            p = one_d_coords[i];
            y = p / manager_field->get_width();
            x = p % manager_field->get_width();
            std::shuffle(ship_orientations.begin(), ship_orientations.end(), std::mt19937(std::random_device{}()));
            Ship ship = unplaced_ships.back();
            place_flag = false;
            index_orientation = 0;
            while (index_orientation < 2){
                ship_orientation = ship_orientations[index_orientation];
                try
                {
                    place_flag = place_ship(ship, y, x, ship_orientation);
                }
                catch(const ConflictShipPlacementException& e)
                {
                    ++index_orientation;
                    continue;
                }
                if (!place_flag)
                {
                    ++index_orientation;
                }
                else
                {
                    unplaced_ships.pop_back();
                    break;
                }
            }
            ++i;
        }
    }
    else
    {
        while (!unplaced_ships.empty())
        {
            Ship ship = unplaced_ships.back();
            std::cout << "Set the ship to length " << ship.get_length() << std::endl;
            std::cout << "Input: y (index), x (index), ship_orientation (h or v)" << std::endl;
            std::cin >> y;
            if (std::cin.fail())
            {
                std::cerr << "Invalid_argument for y, try again" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
                continue;
            }
            std::cin >> x;
            if (std::cin.fail())
            {
                std::cerr << "Invalid_argument for x, try again" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
                continue;
            }
            std::cin >> ship_orientation;
            place_flag = false;
            try
            {
                place_flag = place_ship(ship, y, x, ship_orientation);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << ", try again" << std::endl;
                continue;
            }
            if (!place_flag)
            {
                std::cout << "Ship not placed, try again" << std::endl;
                continue;
            }
            else
            {
                unplaced_ships.pop_back();
                std::cout << "Your new field:" << std::endl;
                std::cout << manager_field->open_visualize() << std::endl;
            }
        }
    }
    updating_dependencies_placed_ships();
}
