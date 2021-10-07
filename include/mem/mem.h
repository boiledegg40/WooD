#pragma once

typedef struct memblock
{
    int size; // Size of block
    bool availability; // If the block is free or not
    int tag; // Purgelevel, check Doom Black Book for info
    memblock_t *next, *previous; // Like OG Doom, linked list with addresses to previous and next lbock
} memblock_t;

void malloc_init();

