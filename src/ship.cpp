#include <iostream>
#include "ship.hpp"

Ship::Segment::Segment(unsigned int hp)
{
    if (hp > 2 || hp < 0)
    {
        throw std::overflow_error("Segment health must be from 0 to 2");
    }
    segment_hp = hp;
}

void Ship::Segment::update_owned_ship(Ship *ship)
{
    owned_ship = ship;
}

Ship *Ship::Segment::get_owned_ship()
{
    return owned_ship;
}

unsigned int Ship::Segment::get_segment_hp() const
{
    return segment_hp;
}

void Ship::Segment::damage(unsigned int amount)
{
    unsigned int value_segment_hp = get_segment_hp();
    if (amount > value_segment_hp)
    {
        amount = value_segment_hp;
    }
    segment_hp -= amount;
}

Ship::Ship(unsigned int length)
{
    if (length < 1 || length > 4)
    {
        throw std::overflow_error("The length of the ship must be from 1 to 4");
    }
    ship_length = length;
    for (unsigned int i = 0; i < length; i++)
    {
        segments.push_back(Segment());
    }
}

unsigned int Ship::get_length() const
{
    return ship_length;
}

std::vector<unsigned int> Ship::get_segments_hp() const
{
    std::vector<unsigned int> segments_hp;
    for (Segment segment: segments)
    {
        segments_hp.push_back(segment.get_segment_hp());
    }
    return segments_hp;
}

Ship::Segment &Ship::get_segment(unsigned int index)
{
    return segments[index];
}

void Ship::set_orientation(char candidate)
{
    orientation = candidate;
}

char Ship::get_orientation()
{
    return orientation;
}
