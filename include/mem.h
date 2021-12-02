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

struct Memblock
{
    int size; // Size of block
    void** user; // If the block is free or not
    int tag; // Purgelevel, check Doom Black Book for info
    int id;
    Memblock *next, *previous; // Like OG Doom, linked list with addresses to previous and next lbock
};


void z_malloc_init(int memorySize);
void z_free(void* block);
void* z_malloc(int size, int tag, void** user);
void z_changetag(void* ptr, int tag);
void z_malloc_cleanup();
int get_blocksize(void* ptr);