#include "mem.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include "error.h"

#define ZONE_ID 0x1d4a11 // DOOM used this number as the ZONE_ID

static Memblock* buffer;
static Memblock* rover;

// NOTE THAT THIS MEMORY ALLOCATOR IS DESIGNED FOR WAD LUMPS

static void _check_valid(Memblock* pointer)
{
    for (Memblock* block = rover; ;block = rover->next)
    {
        if (block->next == rover)
        {
            break;
        }
        if ((char*)block + sizeof(Memblock) + block->size != (char*)block->next)
        {
            e_error("Block does not touch next block!\n");
        }
        if (block->id != ZONE_ID)
        {
            e_error("Invalid zone id in memory block!\n");
        }
        if (block->next->previous != block)
        {
            
            e_error("Next block doesn't have proper back link!\n");
        }
    
    }

}

void z_free(void* block)
{
    Memblock* metadata = (Memblock*)((char*)block - sizeof(Memblock)); // Get the struct address from the memory address given. Cast to char is needed for pointer arithmetic
    _check_valid(metadata); // Check to make sure the block's zone id is valid
    metadata->user = NULL; // Set the user to null to indicate it's free
    metadata->tag = PU_STATIC;
    if (metadata->next->user == NULL) // If the next block is also free, then merge
    {
        metadata->size += (sizeof(Memblock) + metadata->next->size);
        metadata->next = metadata->next->next;
    }
    else if (metadata->previous->user == NULL) // Merge if previous block is free
    {
        metadata->previous->size += (sizeof(Memblock) + metadata->size);
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

void z_malloc_init(int memorySize) 
{
    buffer = (Memblock*)(std::malloc(memorySize * 1024 * 1024));
    buffer->size = (memorySize * 1024 * 1024) - sizeof(Memblock);
    buffer->user = NULL;
    buffer->tag = PU_STATIC;
    buffer->id = ZONE_ID;
    buffer->next = buffer;
    buffer->previous = buffer;
    rover = buffer;
    std::printf("malloc_init(): %d megabytes of memory allocated\n", memorySize);
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
    Memblock* mtd = (Memblock*)((char*)ptr - sizeof(Memblock));
    _check_valid(mtd);
    if (mtd->tag >= PU_PURGELEVEL && mtd->user == NULL)
    {
        e_error("Owner required for purgable blocks");
    }
    mtd->tag = tag;
    if (tag >= PU_PURGELEVEL)
    {
        mtd->user = (void**)2;
    } 
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
Set all of Memblock's members
Return void pointer to block
*/

void* z_malloc(int size, int tag, void** user)
{
    if ((user == NULL) && (tag < 100))
    {
        std::printf("z_malloc(): Error - must specify user\n");
        exit(EXIT_FAILURE);
    }
    Memblock* original_address = rover;
    if (size > buffer->size)
    {
         e_error("No free block found");
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
            z_free((void*)((char*)rover + sizeof(Memblock)));
            break;
        }
    } while (!(rover == original_address));
    
    if (!(rover->user == NULL))
    {
        e_error("No free block found");
    }
    _check_valid(rover);
    if (rover->size > size) // If the free block size is greater than requ
    {
        Memblock* saved = (Memblock*)((char*)rover + sizeof(Memblock) + size);
        saved->id = ZONE_ID;
        saved->user = NULL;
        saved->next = rover->next;
        saved->previous = rover;
        saved->tag = PU_STATIC;
        saved->size = rover->size - size;
        rover->next = saved;
        rover->size = size;
    }
    _check_valid(rover);
    if (tag >= PU_PURGELEVEL) // Cache block, so should be unowned
    {
        rover->user = (void**)2; // the 2 indicates that it is in use, but unowned
    }
    else
    {
        rover->user = user; // Otherwise, point to address of the index of the array to indicate which lump is in use
    }
    rover->id = ZONE_ID;
    rover->tag = tag;
    *user = (void*)((char*)rover + sizeof(Memblock));
    return *user;
}

int get_blocksize(void* ptr)
{
    Memblock* metadata = (Memblock*)((char*)ptr - sizeof(Memblock));
    return metadata->size;
}