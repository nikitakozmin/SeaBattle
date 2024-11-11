#include <iostream>
#include <limits>
#include "ship.hpp"
#include "ship_manager.hpp"
#include "field.hpp"
#include "ability_manager.hpp"
#include "local_exceptions.hpp"

#include <typeinfo>

int main()
{
    Field field{5u, 10u};
    std::cout << field << std::endl;
    ShipManager ship_manager
    {
        field,
        {
            {2, 1},
            {1, 2}
        }
    };

    std::vector<Ship> &unplaced_ships = ship_manager.get_unplaced_ships();
    unsigned int y;
    unsigned int x;
    char ship_orientation;
    bool place_flag;
    while (!unplaced_ships.empty())
    {
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
        Ship ship = unplaced_ships.back();
        place_flag = false;
        try
        {
            place_flag = ship_manager.place_ship(ship, y, x, ship_orientation);
        }
        catch(const std::invalid_argument& e)
        {
            std::cerr << e.what() << ", try again" << std::endl;
            continue;
        }
        catch(const ConflictShipPlacementException& e)
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
        }
        std::cout << field << std::endl;
    }
    ship_manager.updating_dependencies_placed_ships();

    std::cout << "Values from the ability queue:" << std::endl;
    AbilityManager ability_manager{field, ship_manager.get_placed_ships()};
    std::unique_ptr<Ability> fst_ability{ability_manager.get_ability()};
    std::cout << typeid(*fst_ability).name() << std::endl;
    std::cout << typeid(*ability_manager.get_ability()).name() << std::endl;
    std::cout << typeid(*ability_manager.get_ability()).name() << std::endl;

    try
    {
        std::cout << typeid(*ability_manager.get_ability()).name() << std::endl;
    }
    catch (const AbilityException& e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "Adding and removing a new random ability:" << std::endl;
    ability_manager.add_random_ability();
    std::cout << typeid(*ability_manager.get_ability()).name() << std::endl;

    std::cout << "Apply ability " << typeid(*fst_ability).name() << "..." << std::endl;
    fst_ability->use_ability();
    
    
    unsigned int attack_y = 1;
    unsigned int attack_x = 0;
    bool was_ship_hp = field.has_ship_hp(attack_y, attack_x);
    std::cout << "Attack..." << std::endl;
    try
    {
        field.attack(attack_y, attack_x);
    }
    catch(const AtackException& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "Attack missed" << std::endl;
    }
    if (was_ship_hp && !field.has_ship_hp(attack_y, attack_x))
    {
        std::cout << "Adding a new random ability for a kill!" << std::endl;
        ability_manager.add_random_ability();
    }

    std::cout << "See ships..." << std::endl;
    std::cout << field << std::endl;

    for (Ship ship: ship_manager.get_placed_ships())
    {
        std::cout << "Ship length: "<< ship.get_length() << std::endl;
        std::cout << "Ship HP: ";
        for (unsigned int hp: ship.get_segments_hp())
        {
            std::cout << hp << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
