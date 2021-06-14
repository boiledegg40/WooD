#include "wad/wadloader.h"

int main(int argc, char** argv)
{
    wadloader wad(argv[1]); // Gives the object the path to the WAD specified in the command line argument
    wad.loadwad();

    return 0;
}