#include "game_state.hpp"
#include <queue>

GameState::GameState(Field* player_field, Field* enemy_field, AbilityManager *player_ability_manager)
    : player_field(player_field), enemy_field(enemy_field), player_ability_manager(player_ability_manager) {}

void GameState::update_game(
    Field*& player_field, Field*& enemy_field, 
    ShipManager*& player_ship_manager, 
    ShipManager*& enemy_ship_manager,
    AbilityManager*& player_ability_manager
)
{
    player_field = this->player_field;
    enemy_field = this->enemy_field;
    player_ship_manager = this->player_ship_manager;
    enemy_ship_manager = this->enemy_ship_manager;
    player_ability_manager = this->player_ability_manager;
}

nlohmann::json GameState::to_json() const
{
    nlohmann::json json_result;
    json_result["player_field"] = player_field->get_cell_values();
    json_result["enemy_field"] = enemy_field->get_cell_values();
    std::unordered_set<Ship*> unique_player_ships;
    std::unordered_set<Ship*> unique_enemy_ships;

    auto process_ships = [](
        Field* field, std::unordered_set<Ship*>& unique_ships, const std::string& key, nlohmann::json& json_result
    )
    {
        for (unsigned int y = 0; y < field->get_height(); ++y)
        {
            for (unsigned int x = 0; x < field->get_width(); ++x)
            {
                if (field->check_ship(y, x))
                {
                    Ship::Segment *segment = field->get_segment(y, x);
                    Ship *ship = segment->get_owned_ship();
                    if (unique_ships.insert(ship).second)
                    {
                        nlohmann::json ship_info;
                        ship_info["y"] = y;
                        ship_info["x"] = x;
                        ship_info["orientation"] = ship->get_orientation();
                        ship_info["hp"] = ship->get_segments_hp();

                        json_result[key].push_back(ship_info);  // Добавляем информацию о корабле
                    }
                }
            }
        }
    };
    process_ships(player_field, unique_player_ships, "player_ships", json_result);
    process_ships(enemy_field, unique_enemy_ships, "enemy_ships", json_result);

    std::queue<std::unique_ptr<Ability>> ability_queue;
    while (true)
    {
        try
        {
            std::unique_ptr<Ability> ability{player_ability_manager->get_ability()};
            ability_queue.push(ability->clone());
            std::string type_info = std::string(typeid(*ability).name());
            json_result["player_abilities"].push_back(type_info);
        }
        catch(const std::exception& e)
        {
            break;
        }
        
    }
    while (!ability_queue.empty())
    {
        std::string ability_str = std::string(typeid(*ability_queue.front()).name());
        player_ability_manager->add_ability(ability_str);
        ability_queue.pop();
    }

    return json_result;
}

void GameState::save_to_file(const std::string& filename) const
{
    nlohmann::json json_data = to_json();
    // Открытие файла с RAII
    std::ofstream file_stream(filename);
    if (!file_stream.is_open())
    {
        throw std::ios_base::failure("Unable to open file: " + filename);
    }
    file_stream << json_data.dump(4);
}

nlohmann::json GameState::load_from_file(const std::string& filename) {
    // Открытие файла с RAII
    std::ifstream file_stream(filename);
    if (!file_stream.is_open()) {
        throw std::ios_base::failure("Unable to open file: " + filename);
    }

    // Парсинг json данных
    nlohmann::json json_data;
    file_stream >> json_data;

    // Восстановление состояния игры из json
    return json_data;
}

void GameState::parse_ships(
    const nlohmann::json& json_data,
    const std::string& name_ships,
    std::vector<std::vector<unsigned int>>& hps,
    std::vector<unsigned int>& ys,
    std::vector<unsigned int>& xs,
    std::vector<char>& orientations
)
{
    if (json_data.contains(name_ships) && json_data[name_ships].is_array())
    {
        for (const auto& ship : json_data[name_ships])
        {
            if (ship.contains("hp") && ship["hp"].is_array())
            {
                std::vector<unsigned int> hp_values;
                for (const auto& hp : ship["hp"])
                {
                    hp_values.push_back(hp.get<unsigned int>());
                }
                hps.push_back(hp_values);
            }

            if (ship.contains("y") && ship["y"].is_number_unsigned())
            {
                ys.push_back(ship["y"].get<unsigned int>());
            }
            if (ship.contains("x") && ship["x"].is_number_unsigned())
            {
                xs.push_back(ship["x"].get<unsigned int>());
            }

            if (ship.contains("orientation") && ship["orientation"].is_number_integer())
            {
                orientations.push_back(static_cast<char>(ship["orientation"].get<int>()));
            }
        }
    }
}

void GameState::from_json(const nlohmann::json& json_data)
{
    player_field = new Field(json_data["player_field"].size(), json_data["player_field"][0].size());
    enemy_field = new Field(json_data["enemy_field"].size(), json_data["enemy_field"][0].size());

    std::vector<std::vector<unsigned int>> hps;
    std::vector<unsigned int> ys;
    std::vector<unsigned int> xs;
    std::vector<char> orientations;

    parse_ships(json_data, "player_ships", hps, ys, xs, orientations);
    player_ship_manager = new ShipManager(*player_field, hps, ys, xs, orientations);

    hps.clear();
    ys.clear();
    xs.clear();
    orientations.clear();

    parse_ships(json_data, "enemy_ships", hps, ys, xs, orientations);
    enemy_ship_manager = new ShipManager(*enemy_field, hps, ys, xs, orientations);    

    for (unsigned int y = 0; y < json_data["player_field"].size(); ++y)
    {
        for (unsigned int x = 0; x < json_data["player_field"][y].size(); ++x)
        {
            player_field->set_value(json_data["player_field"][y][x].get<int>(), y, x);
        }
    }
    for (unsigned int y = 0; y < json_data["enemy_field"].size(); ++y)
    {
        for (unsigned int x = 0; x < json_data["enemy_field"][y].size(); ++x)
        {
            enemy_field->set_value(json_data["enemy_field"][y][x].get<int>(), y, x);
        }
    }
    
    player_ability_manager = new AbilityManager{*enemy_field, enemy_ship_manager->get_placed_ships()};
    while (true)
    {
        try
        {
            player_ability_manager->get_ability();
        }
        catch(const std::exception& e)
        {
            break;
        }
        
    }
    for (const auto& ability : json_data["player_abilities"])
    {
        std::string ability_str = ability.get<std::string>();
        player_ability_manager->add_ability(ability_str);
    }
}

// Оператор вывода
std::ostream& operator<<(std::ostream& os, const GameState& state) {
    nlohmann::json json_data = state.to_json();
    os << json_data.dump(4);
    return os;
}

// Оператор ввода
std::istream& operator>>(std::istream& is, GameState& state) {
    nlohmann::json json_data;
    is >> json_data;
    state.from_json(json_data);
    return is;
}