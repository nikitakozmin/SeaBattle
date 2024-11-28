#ifndef _SHIP_MANAGER_
#define _SHIP_MANAGER_

#include <map>
#include <vector>
#include "ship.hpp"
#include "field.hpp"

class ShipManager
{
private:
    std::vector<Ship> unplaced_ships;
    std::vector<Ship> placed_ships;
    Field *manager_field;
public:
    //                  Field. Ship length, number
    ShipManager(Field &field, std::map<unsigned int, unsigned int> ships = {{4, 1}, {3, 2}, {2, 3}, {1, 4}});
    ShipManager(
        Field &field,
        std::vector<std::vector<unsigned int>> &hps, 
        std::vector<unsigned int> &ys, std::vector<unsigned int> &xs, 
        std::vector<char> &orientations
    );
    std::vector<Ship> &get_placed_ships();
    //              Start from the top left corner. Ship_orientation = 'h' || 'v'
    bool place_ship(Ship ship, unsigned int y, unsigned int x, char ship_orientation='h');
    void updating_dependencies_placed_ships();
    void placement_of_ships(bool is_random = false);
};

#endif
