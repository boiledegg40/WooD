#pragma once
#include <cstdint>

struct Header
{
    char identification[5];
    uint32_t numlumps;
    uint32_t infotableofs;
};

struct Directory
{
    uint32_t filepos; // Offset to start of lump's data
    uint32_t size; // Size of of lump in bytes
    char name[9]; // ASCII string defining lump's name
};
