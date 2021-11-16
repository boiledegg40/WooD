#pragma once

#include <exception>
#include <string>

class alloc_error : public std::exception
{
    public:
        alloc_error(std::string msg);
        const char* what();

    private:
        std::string m;
};

