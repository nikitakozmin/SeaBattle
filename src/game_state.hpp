#ifndef _GAMESTATE_
#define _GAMESTATE_

#include <unordered_set>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <memory>
#include "ship.hpp"
#include "field.hpp"
#include "includes/nlohmann/json.hpp"
#include "ship_manager.hpp"
#include "ability.hpp"
#include "ability_manager.hpp"


class GameState
{
private:
    Field *player_field;
    Field *enemy_field;
    ShipManager *player_ship_manager;
    ShipManager *enemy_ship_manager;
    AbilityManager *player_ability_manager;
    friend std::ostream& operator<<(std::ostream& os, const GameState& state);
    friend std::istream& operator>>(std::istream& is, GameState& state);
public:
    GameState(
        Field* player_field=nullptr,
        Field* enemy_field=nullptr,
        AbilityManager *player_ability_manager=nullptr 
    );
    void update_game(
        Field*& player_field, Field*& enemy_field,
        ShipManager*& player_ship_manager, 
        ShipManager*& enemy_ship_manager, 
        AbilityManager*& player_ability_manager
    );
    nlohmann::json to_json() const;
    void save_to_file(const std::string& filename) const;
    nlohmann::json load_from_file(const std::string& filename);
    void from_json(const nlohmann::json& json_data);
    void parse_ships(
        const nlohmann::json& json_data,
        const std::string& name_ships,
        std::vector<std::vector<unsigned int>>& hps,
        std::vector<unsigned int>& ys,
        std::vector<unsigned int>& xs,
        std::vector<char>& orientations
    );
};

#endif