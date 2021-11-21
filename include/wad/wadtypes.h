#pragma once
#include <cstdint>
/* All info taken from the DOOM Wiki */

/*
Header struct:
A WAD File always starts with a 12 byte header, containing 3 values.

+----------+--------+----------------+--------------------------------------------------------------------------------------+
| Position | Length |      Name      |                                     Description                                      |
+----------+--------+----------------+--------------------------------------------------------------------------------------+
| 0x00     |      4 | identification | The ASCII characters "IWAD" or "PWAD". Defines whether the WAD is an IWAD or a PWAD. |
| 0x04     |      4 | numlumps       | An integer specifying the number of lumps in the WAD.                                |
| 0x08     |      4 | infotableofs   | An integer holding the offset of the location of the directory.                      |
+----------+--------+----------------+--------------------------------------------------------------------------------------+
For the identification array, we give one more space for the null character ("\0")
The rest of the variables are uint32_t's
*/

typedef struct wadinfo
{
    char identification[5];
    uint32_t numlumps;
    uint32_t infotableofs;
} wadinfo_t;

/*
Directory struct:
The directory contains entries that help locate the lumps in the wad file. Each entry is 16 bytes in length.

+----------+--------+---------+---------------------------------------------------------------------------+
| Position | Length |  Name   |                                Description                                |
+----------+--------+---------+---------------------------------------------------------------------------+
| 0x00     |      4 | filepos | An integer holding a pointer to the start of the lump's data in the file. |
| 0x04     |      4 | size    | An integer representing the size of the lump in bytes.                    |
| 0x08     |      8 | name    | An ASCII string defining the lump's name.                                 |
+----------+--------+---------+---------------------------------------------------------------------------+


*/

typedef struct filelump
{
    uint32_t filepos; // Offset to start of lump's data
    uint32_t size; // Size of of lump in bytes
    char name[8]; // ASCII string defining lump's name
} filelump_t;

/*
lumpinfo struct:
Handle is the offset of this directory, so once we actually load in lumps, we know the location of directory
name, is of course, the lump name
pos is same as filepos (listed above): the offset to the actual lump
size is the size of lump in bytes
*/

typedef struct lumpinfo
{
    char name[9];
    char* filename; // Don't know size of filename
    uint32_t pos; // Offset from directory to lump
    uint32_t size; // Size of lump
} lumpinfo_t;