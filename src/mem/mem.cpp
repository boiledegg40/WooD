#include "mem/mem.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

#define ZONE_ID 0x1d4a11

static memblock_t* buffer;
static memblock_t* rover;


void Z_Malloc_init(int memory_size)
{
    buffer = (memblock_t*)(malloc(memory_size * 1024 * 1024));
    buffer->size = (memory_size * 1024 * 1024) - sizeof(memblock_t);
    buffer->in_use = false;
    buffer->tag = PU_STATIC;
    buffer->id = ZONE_ID;
    buffer->next = buffer;
    buffer->previous = buffer;
    rover = buffer;
    std::printf("malloc_init(): %d megabytes of memory allocated\n", memory_size);
}


void* Z_Malloc(int size)
{
    
}