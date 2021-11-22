#include "error.h"
#include <cstdio>
#include "mem.h"
#include "stdlib.h"

void e_error(const char* msg)
{
    std::printf("Error: %s\n", msg);
    z_malloc_cleanup();
    exit(EXIT_FAILURE);
}