#include <cstdio>
#include "map.h"
#include "mem.h"

Vertex* read_vertex(void* data)
{
    Vertex* block = (Vertex*)data;
    int num_vertex = get_blocksize(data) / sizeof(Vertex);
    for (int i = 0; i < num_vertex; i++)
    {
        std::printf("(%d, %d)\n", block[i].x, block[i].y);
    }
    return block;
}