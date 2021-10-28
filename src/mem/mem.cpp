#include "mem/mem.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdlib.h>

#define ZONE_ID 0x1d4a11

static memblock_t* buffer;
static memblock_t* rover;

static void _check_valid(memblock_t* pointer)
{
    try
    {
        if (pointer->id != ZONE_ID)
        {
            throw std::bad_alloc();
        }
    }
    catch(const std::bad_alloc& e)
    {
        std::cerr << "Invalid zone id in memory block!\n";
        exit(EXIT_FAILURE);
    }
    
}

void z_free(void* block)
{
    memblock_t* metadata = (memblock_t*)((char*)block - sizeof(memblock_t));
    std::printf("ZONE_ID: %x\n", metadata->id);
    _check_valid(metadata);
    metadata->in_use = false;
    if (metadata->next->in_use == false)
    {
        metadata->size += (sizeof(memblock_t) + metadata->next->size);
        metadata->next = metadata->next->next;
    }
    else if (metadata->previous->in_use == false)
    {
        metadata->previous->size += (sizeof(memblock_t) + metadata->size);
        metadata->previous->next = metadata->next;
    }
}

void z_malloc_init(int memory_size)
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
    std::printf("malloc_init(): base block address %p\n", (void*)(buffer));
    std::printf("base block ZONE_ID: %x\n", buffer->id);
}


void* z_malloc(int size, int tag)
{
    try
    {
        memblock_t* original_address = rover;
        do
        {
            _check_valid(rover);
            if (rover->in_use == false && rover->size >= size)
            {
                break;
            }
            else if (rover->tag >= PU_PURGELEVEL)
            {
                z_free((void*)(rover + sizeof(memblock_t)));
                break;
            }
            rover = rover->next;
        } while (!(rover == original_address));
        if (rover->in_use == true)
        {
            throw std::bad_alloc();
        }
    }
    catch(const std::bad_alloc& e)
    {
        std::cerr << "No free block found: " << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    
    if (rover->size > size) // If the free block size is greater than requ
    {
        memblock_t* saved = rover + size + sizeof(memblock_t);
        saved->id = ZONE_ID;
        saved->in_use = false;
        saved->next = rover->next;
        saved->previous = rover;
        saved->in_use = false;
        saved->tag = PU_STATIC;
        saved->size = saved->size - size;
        rover->next = saved;
        rover->size = size;
    }
    rover->in_use = true;
    rover->id = ZONE_ID;
    rover->tag = tag;
    std::printf("rover ZONE_ID: %x\n", rover->id);
    return (void*)((char*)rover + sizeof(memblock_t));
}