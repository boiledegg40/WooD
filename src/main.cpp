#include "wad/wad.h"
#include "mem/mem.h"
#include <cstdio>

int main(int argc, char** argv)
{
    loadwad(argv[1]); // Gives the object the path to the WAD specified in the command line argument
    z_malloc_init(8);
    int* test = (int*)(z_malloc(sizeof(int), PU_STATIC));
    *test = 2;
    printf("test value: %d\n", *test);
    printf("test address: %p\n", (void*)test);
    z_free(test);
    return 0;
}