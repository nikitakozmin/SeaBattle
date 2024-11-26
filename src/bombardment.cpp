#include <random>
#include "bombardment.hpp"
#include "ship.hpp"

Bombardment::Bombardment(std::vector<Ship> **placed_ships)
{
    ships = placed_ships;
}

void Bombardment::use_ability()
{
    if ((*ships)->empty())
    {
        throw std::overflow_error("No ship available");
    }
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<size_t> dist_ships(0, (*ships)->size() - 1);
    size_t random_ship_index = dist_ships(gen);
    Ship &random_ship = (**ships)[random_ship_index];

    std::uniform_int_distribution<size_t> dist_segments(0, random_ship.get_length() - 1);
    size_t random_segment_index = dist_segments(gen);
    Ship::Segment &random_segment = random_ship.get_segment(random_segment_index);

    random_segment.damage(1);
}

std::unique_ptr<Ability> Bombardment::clone() const 
{
    return std::make_unique<Bombardment>(*this);
}
