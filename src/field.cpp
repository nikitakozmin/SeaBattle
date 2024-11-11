#include <string>
#include "field.hpp"
#include "local_exceptions.hpp"

char Field::Cell::get_value() const
{
    return value;
}
void Field::Cell::set_value(char candidate)
{
    value = candidate;
}

bool Field::Cell::has_ship() const
{
    return cell_segment != nullptr;
}

void Field::Cell::set_segment(Ship::Segment *segment)
{
    cell_segment = segment;
}
Ship::Segment &Field::Cell::get_segment()
{
    return *(cell_segment);
}

Field::Field(unsigned int height, unsigned int width)
{
    field_height = height;
    field_width = width;
    for (unsigned int i=0; i<height; i++)
    {
        cells.push_back(std::vector<Cell>());
        for (unsigned int j=0; j<width; j++)
        {
            cells[i].push_back(Cell());
        }
    }
}

Field::Field(Field& other)
    : field_height(other.field_height), field_width(other.field_width),
      cells(other.field_height, std::vector<Cell>(other.field_width))
{
    for (unsigned int i = 0; i < field_height; ++i)
    {
        for (unsigned int j = 0; j < field_width; ++j)
        {
            cells[i][j].set_value(other.cells[i][j].get_value());
            if (other.cells[i][j].has_ship())
            {
                cells[i][j].set_segment(&other.cells[i][j].get_segment());
            }
        }
    }
}

Field& Field::operator = (Field& other)
{
    if (this != &other)
    {
        field_height = other.field_height;
        field_width = other.field_width;
        cells.resize(field_height, std::vector<Cell>(field_width));
        for (unsigned int i = 0; i < field_height; ++i)
        {
            for (unsigned int j = 0; j < field_width; ++j)
            {
                cells[i][j].set_value(other.cells[i][j].get_value());
                if (other.cells[i][j].has_ship()) {
                    cells[i][j].set_segment(&other.cells[i][j].get_segment());
                } else {
                    cells[i][j].set_segment(nullptr);
                }
            }
        }
    }
    return *this;
}

Field::Field(Field&& other)
    : field_height(other.field_height), field_width(other.field_width), cells(std::move(other.cells))
{
    other.field_height = 0;
    other.field_width = 0;
}

Field& Field::operator = (Field&& other)
{
    if (this != &other)
    {
        field_height = other.field_height;
        field_width = other.field_width;
        cells = std::move(other.cells);
        other.field_height = 0;
        other.field_width = 0;
    }
    return *this;
}

unsigned int Field::get_height() const
{
    return field_height;
}
unsigned int Field::get_width() const
{
    return field_width;
}

std::vector<std::vector<char>> Field::get_cell_values() const
{
    std::vector<std::vector<char>> cell_values;
    for (unsigned int i=0; i<get_height(); i++)
    {
        cell_values.push_back(std::vector<char>());
        for (unsigned int j=0; j<get_width(); j++)
        {
            cell_values[i].push_back(cells[i][j].get_value());
        }
    }
    return cell_values;
}

bool Field::place_ship(Ship &ship, unsigned int y, unsigned int x, char ship_orientation)
{   
    unsigned int add_y;
    unsigned int add_x;
    if (ship_orientation == 'h')
    {
        add_y = 2;
        add_x = ship.get_length()+1;
    }
    else
    {
        add_y = ship.get_length()+1;
        add_x = 2;
    }
    for (long int i = (long int)y-1; i<(long int)y+(long int)add_y; i++)
    {
        for (long int j = (long int)x-1; j<(long int)x+(long int)add_x; j++)
        {
            if (j >= 0 && i >= 0 && i < field_height && j < field_width)
            {
                if (cells[i][j].has_ship())
                {
                    throw ConflictShipPlacementException();
                }
            }
        }
    }
    if (x >= field_width || y >= field_height)
    {
        return 0;
    }
    if (ship_orientation == 'h')
    {
        if (x+ship.get_length()-1 >= field_width)
        {
            return 0;
        }
        for (unsigned int i = x; i<x+ship.get_length(); i++)
        {
            cells[y][i].set_segment(&ship.get_segment(i-x));
            cells[y][i].set_value(ship.get_segment(i-x).get_segment_hp() + '0');
        }
    }
    else
    {
        if (y+ship.get_length()-1 >= field_height)
        {
            return 0;
        }
        for (unsigned int i = y; i<y+ship.get_length(); i++)
        {
            cells[i][x].set_segment(&ship.get_segment(i-y));
            cells[i][x].set_value(ship.get_segment(i-y).get_segment_hp() + '0');
        }
    }
    return 1;
}

void Field::set_damage(unsigned int d)
{
    damage_value = d;
}

void Field::attack(unsigned int y, unsigned int x)
{
    if (x < field_width && y < field_height)
    {
        if (!cells[y][x].has_ship())
        {
            cells[y][x].set_value('_');
            return;
        }
        cells[y][x].get_segment().damage(damage_value);
        cells[y][x].set_value((char)(cells[y][x].get_segment().get_segment_hp()) + '0');
        set_damage(1);
    }
    else
    {
        throw AtackException();
    }
}

bool Field::check_ship(unsigned int y, unsigned int x)
{
    bool flag = false;
    if (x < field_width && y < field_height)
    {
        if (cells[y][x].has_ship())
        {
            flag = true;
        }
    }
    return flag;
}

bool Field::has_ship_hp(unsigned int y, unsigned int x)
{
    if (x >= field_width || y >= field_height)
    {
        return false;
    }
    if (!cells[y][x].has_ship())
    {
        return false;
    }
    bool flag = false;
    Ship *ship = cells[y][x].get_segment().get_owned_ship();
    std::vector<unsigned int> hps = ship->get_segments_hp();
    for (unsigned int i = 0; i < hps.size(); ++i)
    {
        if (hps[i] > 0)
        {
            flag = true;
            break;
        }
    }
    return flag;
}

std::ostream& operator << (std::ostream &strm, const Field &field)
{
    std::string rslt{""};
    for (std::vector<char> line: field.get_cell_values())
    {
        for (char value: line)
        {
            rslt += std::string{value} + ' ';
        }
        rslt += '\n';
    }
    return strm << rslt;
}
