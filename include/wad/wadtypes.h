#include <cstdint>

typedef struct
{
    char idenfication[4];
    uint32_t numlumps;
    uint32_t infotableofs;
} header;

typedef struct
{
    uint32_t lump_offset;
    uint32_t lump_size;
    char lump_name[9];
} directory;
