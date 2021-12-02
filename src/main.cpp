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
    void* block = loadmap("MAP01", VERTEXES);
    Vertex* vertex = read_vertex(block);
    init_window();
    gameloop();
    glfwTerminate();
    z_malloc_cleanup();
    return 0;
}