#include <iostream>
#include <limits>
#include "scanner.hpp"

Scanner::Scanner(Field **field)
{
    manager_field = field;
}

void Scanner::use_ability()
{
    unsigned int y;
    std::cout << "Input: y (index)" << std::endl;
    std::cin >> y;
    while (std::cin.fail())
    {
        std::cerr << "Invalid_argument for y, try again" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
        std::cin >> y;
    }
    std::cout << "Input: x (index)" << std::endl;
    unsigned int x;
    std::cin >> x;
    if (std::cin.fail())
    {
        std::cerr << "Invalid_argument for x, try again" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
        std::cin >> x;
    }
    if ((*manager_field)->check_ship(y, x)||(*manager_field)->check_ship(y+1, x)||
        (*manager_field)->check_ship(y, x+1)||(*manager_field)->check_ship(y+1, x+1)
    )
    {
        std::cout << "The ship has been found in a two by two field" << std::endl;
    }
    else
    {
        std::cout << "The ship was not found in a two by two field" << std::endl;
    }
}

std::unique_ptr<Ability> Scanner::clone() const 
{
    return std::make_unique<Scanner>(*this);
}
