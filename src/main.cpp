#include "wad/wad.h"
#include "mem/mem.h"
#include <cstdio>

int main(int argc, char** argv)
{
    z_malloc_init(8);
    loadwad(argc, argv); // Gives the object the path to the WAD specified in the command line argument
    
    return 0;
}