#include "wad.h"
#include "mem.h"
#include "map.h"
#include "game.h"
#include "error.h"
#include <cstdio>

int main(int argc, char** argv)
{
    z_malloc_init(8);
    loadwad(argc, argv); // Gives the object the path to the WAD specified in the command line argument
    init_window();
    gameloop();
    glfwTerminate();
    z_free(vertex);
    z_malloc_cleanup();
    return 0;
}