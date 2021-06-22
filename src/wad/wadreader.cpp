#include <string.h>
#include "wad/wadreader.h"

/*
Ok, so for now, we only need to have the read 4 bytes function.
read4bytes - parameters: pointer the m_WADData, and offset from the beginning of m_WADData
You create a 4 byte variable, ReadValue, and memcpy the stuff from the offset to ReadValue's address

You don't need to do anything for the wadreader constructor and deconstructor.
read_header():
Only need address of m_WADData, and reference to the header object created.
Start by lookping through each element in the identification array
given that the offset parameter is 0, you just add 1 to the offset each time
identification[i] = p_WADData[offset + i] and so forth

Then use the read4bytes function to get the numnber of lumps (4 bytes) and offset to directory (4 bytes)
*/

wadreader::wadreader()
{
}

wadreader::~wadreader()
{
}

uint32_t wadreader::read4bytes(const uint8_t *p_WADData, int offset)
{
    uint32_t value;
    memcpy(&value, p_WADData + offset, sizeof(uint32_t));
    return value;
}

void wadreader::read_header(const uint8_t *p_WADData, int offset, header &header)
{
    // 0x00 - 0x03
    header.identification[0] = p_WADData[offset];
    header.identification[1] = p_WADData[offset + 1];
    header.identification[2] = p_WADData[offset + 2];
    header.identification[3] = p_WADData[offset + 3];
    header.identification[4] = '\0';

    // 0x04 - 0x07
    header.numlumps = read4bytes(p_WADData, offset + 4);

    // 0x08 - 0x0B
    header.infotableofs = read4bytes(p_WADData, offset + 8);
}