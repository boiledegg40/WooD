#include <cstdio>
#include "e_exceptions.h"

alloc_error::alloc_error(std::string msg) : m(msg)
{
}

const char* alloc_error::what()
{
    return m.c_str();
}



