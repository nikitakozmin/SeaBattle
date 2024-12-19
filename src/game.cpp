#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>
#include "game.hpp"
#include "local_exceptions.hpp"
#include "game_state.hpp"

using namespace std::chrono_literals;

bool Game::loading_latest_game()
{
    char selected;
    std::cout << "Load latest game? [Y/n]:" << std::endl;
    while (true)
    {
        std::cin >> selected;
        if (selected == 'y' || selected == 'Y')
        {
            GameState game_state{};
            try
            {
                game_state.from_json(game_state.load_from_file("last save.json"));
                game_state.update_game(player_field, enemy_field, player_ship_manager, enemy_ship_manager, player_ability_manager);
                std::cout << "Game state loaded to 'last save.json'" << std::endl;
                return true;
            }
            catch (const std::exception& ex)
            {
                std::cerr << "Error: " << ex.what() << std::endl;
                std::cout << "Let's continue new game" << std::endl;
                return false;
            }
        }
        else if (selected == 'n' || selected == 'N')
        {
            return false;
        }
        else
        {
            std::cerr << "Invalid input. Please enter Y or n." << std::endl;
        }
    }
}

bool Game::saving_latest_game()
{
    char selected;
    std::cout << "Save latest game? [Y/n]:" << std::endl;
    while (true)
    {
        std::cin >> selected;
        if (selected == 'y' || selected == 'Y')
        {
        GameState game_state(player_field, enemy_field, player_ability_manager);
        try
        {
            game_state.save_to_file("last save.json");
            std::cout << "Game state saved to 'last save.json'" << std::endl;
            return true;
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Error: " << ex.what() << std::endl;
            std::cout << "Let's continue" << std::endl;
        }
        }
        else if (selected == 'n' || selected == 'N')
        {
            return false;
        }
        else
        {
            std::cerr << "Invalid input. Please enter Y or n." << std::endl;
        }
    }
}

Game::Game()
{
    if (!loading_latest_game())
    {
        bool is_random = false; // Стоит true, так как так быстрее тестировать
        creating_player_objects(is_random);
        creating_enemy_objects();
        player_ability_manager = new AbilityManager{*enemy_field, enemy_ship_manager->get_placed_ships()};
    }
    std::cout << "Your field:" << std::endl;
    std::cout << player_field->open_visualize() << std::endl;
    std::this_thread::sleep_for(2000ms);
}

void Game::placement_of_ships(ShipManager *ship_manager, bool is_random)
{
    while (true)
    {
        try
        {
            ship_manager->placement_of_ships(is_random);
            break;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << "I'm trying again" << std::endl;
            std::this_thread::sleep_for(2000ms);
        }
    }
}

void Game::creating_player_objects(bool is_random)
{
    // TODO: (Приоритет: 2) Предоставление возможности выбора размеров поля (те же размеры передавать противнику?)
    while (true)
    {
        try
        {
            player_field = new Field{};
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            exit(0);
            continue;
        }
        break;
    }
    // TODO: (Приоритет: 3) Предоставление возможности выбора кораблей
    player_ship_manager = new ShipManager{*player_field};
    // TODO: (Приоритет: 1) Предоставление возможности выбора установки кораблей игроком (рандомно | самостоятельно)
    if (!is_random)
    {
        std::cout << "Your field:" << std::endl;
        std::cout << player_field->open_visualize() << std::endl;
    }
    placement_of_ships(player_ship_manager, is_random);
    if (is_random)
    {
        std::cout << "Your new field:" << std::endl;
        std::cout << player_field->open_visualize() << std::endl;
    }
}

void Game::creating_enemy_objects()
{
    std::cout << "Creating a field and placing enemy ships..." << std::endl << std::endl;
    enemy_field = new Field{};
    enemy_ship_manager = new ShipManager{*enemy_field};
    placement_of_ships(enemy_ship_manager, true);
    std::cout << "Successfully!" << std::endl;
}

void Game::take_ability(AbilityManager *ability_manager)
{
    char selected_ability;
    std::cout << "Take ability? [Y/n]:" << std::endl;
    while (true)
    {
        std::cin >> selected_ability;
        if (selected_ability == 'y' || selected_ability == 'Y')
        {
            try
            {
                std::unique_ptr<Ability> ability{ability_manager->get_ability()};
                std::cout << "The ability " << typeid(*ability).name() << " is selected" << std::endl;
                ability->use_ability();
            }
            catch (const AbilityException& e)
            {
                std::cerr << e.what() << std::endl;
                std::cout << "Let's continue" << std::endl;
            }
            break;
        }
        else if (selected_ability == 'n' || selected_ability == 'N')
        {
            break;
        }
        else
        {
            std::cerr << "Invalid input. Please enter Y or n." << std::endl;
        }
    }
}

void Game::attack_with_ability_charge(Field *opponent_field, AbilityManager *ability_manager)
{
    unsigned int attack_y;
    unsigned int attack_x;
    bool was_ship_hp;
    std::cout << "Attack:" << std::endl;
    while (true)
    {
        std::cout << "Input: y (index)" << std::endl;
        std::cin >> attack_y;
        while (std::cin.fail())
        {
            std::cerr << "Invalid_argument for y, try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
            std::cin >> attack_y;
        }
        std::cout << "Input: x (index)" << std::endl;
        std::cin >> attack_x;
        if (std::cin.fail())
        {
            std::cerr << "Invalid_argument for x, try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
            std::cin >> attack_x;
        }
        was_ship_hp = opponent_field->has_ship_hp(attack_y, attack_x);
        try
        {
            opponent_field->attack(attack_y, attack_x); // TODO: Подумать над добавлением ограничения по повторному попаданию
            break;
        }
        catch(const AtackException& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << "Try again" << std::endl;
        }
    }
    if (was_ship_hp && !opponent_field->has_ship_hp(attack_y, attack_x))
    {
        std::cout << "Adding a new random ability for a kill!" << std::endl;
        ability_manager->add_random_ability();
    }
    std::cout << "Current enemy field:" << std::endl;
    std::cout << *opponent_field << std::endl;
}

 
void Game::start_game()
{
    // // Пример работы потока вывода
    // GameState game_state(player_field, enemy_field, player_ability_manager);
    // std::ofstream out_file("game_state.json");
    // out_file << game_state;
    // out_file.close();

    // // Пример работы потока ввода
    // GameState game_state{};
    // std::ifstream in_file("game_state.json");
    // in_file >> game_state;
    // std::cout << game_state;
    // in_file.close();

    unsigned int attack_y;
    unsigned int attack_x;
    char cell_value;
    unsigned int num_cells_player_field = player_field->get_height() * player_field->get_width();
    std::vector<int> one_d_coords(num_cells_player_field*2);
    std::iota(one_d_coords.begin(), one_d_coords.end(), 0); // Заполнение вектора последовательностью 0, 1, 2, ...
    std::shuffle(one_d_coords.begin(), one_d_coords.end(), std::mt19937(std::random_device{}()));
    unsigned long int i = 0;
    unsigned long int p;

    while (true)
    {        
        std::cout << "Enemy field:" << std::endl;
        std::cout << *enemy_field << std::endl;
        
        take_ability(player_ability_manager);

        attack_with_ability_charge(enemy_field, player_ability_manager);

        if (check_win(enemy_ship_manager))
        {
            std::cout << "Congratulations on the victory, let's update enemy" << std::endl;
            creating_enemy_objects();
            player_ability_manager->set_new_parameters(*enemy_field, enemy_ship_manager->get_placed_ships());
            continue;
        }

        std::cout << "Enemy attack..." << std::endl;
        do
        {
            p = one_d_coords[i]%num_cells_player_field;
            ++i;
            attack_y = p / player_field->get_width();
            attack_x = p % player_field->get_width();
            cell_value = player_field->get_cell_values()[attack_y][attack_x];
        } while (cell_value == '0' || cell_value == '_');
        std::this_thread::sleep_for(2000ms);
        player_field->attack(attack_y, attack_x);
        std::cout << "Your new field:" << std::endl;
        std::cout << player_field->open_visualize() << std::endl;
        std::this_thread::sleep_for(2000ms);
        
        if (check_win(player_ship_manager))
        {
            std::cout << "Unfortunately you lost, let's update your field" << std::endl;
            Game game;
            game.start_game();
            break;
        }

        saving_latest_game();
    }
}

bool Game::check_win(ShipManager *opponent_ship_manager)
{
    bool flag = true;
    for (Ship ship: opponent_ship_manager->get_placed_ships())
    {
        for (unsigned int hp: ship.get_segments_hp())
        {
            if (hp > 0)
            {
                flag = false;
            }
        }
        if (flag == false)
        {
            break;
        }
    }
    return flag;
}
