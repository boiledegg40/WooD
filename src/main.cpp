#include "wad/wad.h"
#include "mem/mem.h"

int main(int argc, char** argv)
{
    loadwad(argv[1]); // Gives the object the path to the WAD specified in the command line argument
    Z_Malloc_init(8);
    return 0;
}