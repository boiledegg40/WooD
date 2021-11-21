#pragma once

#include <cstdint>

enum MapData
{
    THINGS = 1,
    LINEDEFS,
    SIDEDEFS,
    VERTEXES,
    SEGS,
    SSECTORS,
    NODES,
    SECTORS,
    REJECT,
    BLOCKMAP
};

typedef struct Vertex
{
    uint16_t x;
    uint16_t y;
} Vertex;

Vertex* read_vertex(void* data);