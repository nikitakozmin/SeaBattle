#ifndef _SHIP_
#define _SHIP_

#include <vector>

class Ship
{
public:
    Ship(unsigned int length = 1);
    class Segment
    {
    private:
        unsigned int segment_hp;
        Ship *owned_ship = nullptr;
    public:
        Segment(unsigned int hp = 2);
        void update_owned_ship(Ship *ship);
        Ship *get_owned_ship();
        unsigned int get_segment_hp() const;
        void damage(unsigned int amount);
    };
    unsigned int get_length() const;
    std::vector<unsigned int> get_segments_hp() const;
    Segment &get_segment(unsigned int index);
    void set_orientation(char candidate);
    char get_orientation();
private:
    unsigned int ship_length;
    std::vector<Segment> segments;
    char orientation;
};

#endif
