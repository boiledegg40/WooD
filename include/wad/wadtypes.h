#pragma once
#include <cstdint>

struct header
{
    char identification[5];
    uint32_t numlumps;
    uint32_t infotableofs;
};

struct directory
{
    uint32_t lump_offset;
    uint32_t lump_size;
    char lump_name[9];
};
