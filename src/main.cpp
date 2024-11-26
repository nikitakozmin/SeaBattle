#include <iostream>
#include "ship.hpp"
#include "ship_manager.hpp"
#include "field.hpp"
#include "ability_manager.hpp"
#include "local_exceptions.hpp"
#include "game.hpp"

#include <typeinfo>

int main()
{
    Game game;
    game.start_game();
    // for (Ship ship: ship_manager.get_placed_ships())
    // {
    //     std::cout << "Ship length: "<< ship.get_length() << std::endl;
    //     std::cout << "Ship HP: ";
    //     for (unsigned int hp: ship.get_segments_hp())
    //     {
    //         std::cout << hp << " ";
    //     }
    //     std::cout << std::endl;
    // }
    return 0;
}
