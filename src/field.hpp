#ifndef _FIELD_
#define _FIELD_

#include <iostream>
#include <vector>
#include "ship.hpp"

class Field
{
private:
    class Cell
    {
    private:
        char value = '~';
        Ship::Segment *cell_segment = nullptr;
    public:
        char get_value() const;
        void set_value(char candidate);
        bool has_ship() const;
        void set_segment(Ship::Segment *segment);
        Ship::Segment &get_segment();
    };
    unsigned int field_height;
    unsigned int field_width;
    std::vector<std::vector<Cell>> cells;
    unsigned int damage_value = 1;
public:
    Field(unsigned int height=10, unsigned int width=10);
    Field(Field& other);
    Field(Field&& other);
    Field& operator = (Field& other);
    Field& operator = (Field&& other);
    unsigned int get_height() const;
    unsigned int get_width() const;
    std::vector<std::vector<char>> get_cell_values() const;
    bool place_ship(Ship &ship, unsigned int y, unsigned int x, char ship_orientation);
    void set_damage(unsigned int d);
    void attack(unsigned int y, unsigned int x);
    bool check_ship(unsigned int y, unsigned int x);
    bool has_ship_hp(unsigned int y, unsigned int x);
    Ship::Segment *get_segment(unsigned int y, unsigned int x);
    std::string open_visualize();
    void set_value(char candidate, unsigned int y, unsigned int x);
};

std::ostream& operator << (std::ostream &strm, const Field &field);

#endif
