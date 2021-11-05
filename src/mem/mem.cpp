#include "mem/mem.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdlib.h>

#define ZONE_ID 0x1d4a11 // DOOM used this number as the ZONE_ID

static memblock_t* buffer;
static memblock_t* rover;

// NOTE THAT THIS MEMORY ALLOCATOR IS DESIGNED FOR WAD LUMPS

static void _check_valid(memblock_t* pointer)
{
    memblock_t* metadata = (memblock_t*)((char*)pointer - sizeof(memblock_t));
    try // Check for heap corruption
    {
        if (pointer->id != ZONE_ID)
        {
            throw "Invalid zone id in memory block!\n";
        }
        if (metadata->size == (((char*)metadata->next - (char*)pointer)))
        {
            throw "Block does not touch next block!\n";
        }
        if (metadata->next->previous != metadata)
        {
            throw "Next block doesn't have proper back link!\n";
        }
    }
    catch(const char* msg)
    {
        std::cerr << "__check_valid: " << msg;
        exit(EXIT_FAILURE);
    }
    

}

void z_free(void* block)
{
    memblock_t* metadata = (memblock_t*)((char*)block - sizeof(memblock_t)); // Get the struct address from the memory address given. Cast to char is needed for pointer arithmetic
    _check_valid(metadata); // Check to make sure the block's zone id is valid
    metadata->user = NULL; // Set the user to null to indicate it's free
    if (metadata->next->user == NULL) // If the next block is also free, then merge
    {
        metadata->size += (sizeof(memblock_t) + metadata->next->size);
        metadata->next = metadata->next->next;
    }
    else if (metadata->previous->user == NULL) // Merge if previous block is free
    {
        metadata->previous->size += (sizeof(memblock_t) + metadata->size);
        metadata->previous->next = metadata->next;
    }
}

/*
z_malloc_init():
Initially allocate requested amount of memory using standard malloc
Set all of the struct's members
Set next and previous pointer to itself
Size is size of allocated block - struct size
Set rover to buffer
*/

void z_malloc_init(int memory_size) 
{
    buffer = (memblock_t*)(std::malloc(memory_size * 1024 * 1024));
    buffer->size = (memory_size * 1024 * 1024) - sizeof(memblock_t);
    buffer->user = NULL;
    buffer->tag = PU_STATIC;
    buffer->id = ZONE_ID;
    buffer->next = buffer;
    buffer->previous = buffer;
    rover = buffer;
    std::printf("malloc_init(): %d megabytes of memory allocated\n", memory_size);
    std::printf("malloc_init(): base block address %p\n", (void*)(buffer));
    std::printf("malloc_init(): buffer size %d\n", buffer->size);
}

void z_malloc_cleanup()
{
    std::free(buffer);
    std::printf("z_malloc_cleanup: Freed main memory block\n");
}

void z_changetag(void* ptr, int tag)
{
    memblock_t* mtd = (memblock_t*)((char*)ptr - sizeof(memblock_t));
    _check_valid(mtd);
    mtd->tag = tag;
}

/*
z_malloc():
z_malloc has 3 parameters: size, user, and tag
size is requested amount of memory
user is the pointer to the array (basically array of cached lumps) of void pointers (the memory block)
tag is one of the tags listed in mem.h
Finds a free block, or if no free block, tries to find a cache block and free it
Otherwise, throw exception "No free block found"
Once free block found, if block is bigger than size, split
Set all of memblock_t's members
Return void pointer to block
*/

void* z_malloc(int size, int tag, void** user = NULL)
{
    if ((user == NULL) && (tag < 100))
    {
        std::printf("z_malloc(): Error - must specify user\n");
        exit(EXIT_FAILURE);
    }
    try
    {
        memblock_t* original_address = rover;
        if (size > buffer->size)
        {
            throw std::bad_alloc();
        }
        do
        {
            if (rover->user == NULL && rover->size >= size)
            {
                break;
            }
            rover = rover->next;
        } while (!(rover == original_address));
        do
        {
            if (rover->tag >= PU_PURGELEVEL)
            {
                z_free((void*)((char*)rover + sizeof(memblock_t)));
                break;
            }
        } while (!(rover == original_address));
        
        if (!(rover->user == NULL))
        {
            throw std::bad_alloc();
        }
    }
    catch(const std::bad_alloc& e)
    {
        std::cerr << e.what() << ": No free block found" << '\n';
        exit(EXIT_FAILURE);
    }
    
    if (rover->size > size) // If the free block size is greater than requ
    {
        memblock_t* saved = (memblock_t*)(char*)rover + size + sizeof(memblock_t);
        saved->id = ZONE_ID;
        saved->user = NULL;
        saved->next = rover->next;
        saved->previous = rover;
        saved->tag = PU_STATIC;
        saved->size = saved->size - size;
        rover->next = saved;
        rover->size = size;
    }
    if (tag >= PU_PURGELEVEL) // Shamelessly copy DOOM's source code
    {
        rover->user = (void**)2; // the 2 indicates that it is in use, but unowned
    }
    else
    {
        rover->user = user; // Otherwise, point to address of the index of the array to indicate which lump is in use
    }
    rover->id = ZONE_ID;
    rover->tag = tag;
    *user = (void*)((char*)rover + sizeof(memblock_t));
    return *user;
}