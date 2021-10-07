#include "wad/wad.h"

int main(int argc, char** argv)
{
    loadwad(argv[1]); // Gives the object the path to the WAD specified in the command line argument

    return 0;
}