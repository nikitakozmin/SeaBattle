#ifndef _GAME_
#define _GAME_

#include "field.hpp"
#include "ship_manager.hpp"
#include "ability_manager.hpp"

class Game
{
private:
    Field *player_field;
    ShipManager *player_ship_manager;
    Field *enemy_field;
    ShipManager *enemy_ship_manager;
    AbilityManager *player_ability_manager;
    AbilityManager *enemy_ability_manager;

public:
    Game();
    void placement_of_ships(ShipManager *ship_manager, bool is_random);
    void creating_player_objects(bool is_random=false);
    void creating_enemy_objects();
    void take_ability(AbilityManager *ability_manager);
    void attack_with_ability_charge(Field *opponent_field, AbilityManager *ability_manager);
    void start_game();
    bool check_win(ShipManager *opponent_ship_manager);
};

#endif
