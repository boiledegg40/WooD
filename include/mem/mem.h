#pragma once

// Tag definitions
// Tags < 100 are not overwritten until free() is explicitly called
#define PU_STATIC 1
#define PU_SOUND 2
#define PU_MUSIC 3
#define PU_LEVEL 50 // for current level, when level exits, search for all blocks with this tag and free

// Tags >= 100 can be overwritten (They are used for cache, but can be freed automatically if required)
#define PU_PURGELEVEL 100
#define PU_CACHE 101

typedef struct memblock
{
    int size; // Size of block
    bool in_use; // If the block is free or not
    int tag; // Purgelevel, check Doom Black Book for info
    int id;
    struct memblock *next, *previous; // Like OG Doom, linked list with addresses to previous and next lbock
} memblock_t;


void Z_Malloc_init(int memory_size);