#include "local_exceptions.hpp"

BaseException::BaseException(const std::string &msg) : message(msg) {};
const char* BaseException::what() const 
{
        return message.c_str();
}

AbilityException::AbilityException() : BaseException("Error: No remaining abilities") {}

ConflictShipPlacementException::ConflictShipPlacementException()
    : BaseException("Error: Placing a ship close to or at an intersection with another ship") {}

AtackException::AtackException() : BaseException("Error: Attack beyond the borders") {}
