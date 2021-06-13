#include "wad/wadloader.h"

int main(int argc, char** argv)
{
    wadloader wad("DOOM2.wad");
    wad.loadwad();
}