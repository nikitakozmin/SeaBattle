#ifndef _LOCAL_EXCEPTIONS_
#define _LOCAL_EXCEPTIONS_

#include <iostream>
#include <string>

class BaseException {
protected:
    std::string message;

public:
    explicit BaseException(const std::string &msg);
    virtual const char* what() const;
};


class AbilityException : public BaseException {
public:
    AbilityException();
};

class ConflictShipPlacementException : public BaseException {
public:
    ConflictShipPlacementException();
};

class AtackException : public BaseException {
public:
    AtackException();
};

#endif