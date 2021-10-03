#include <string.h>
#include "wad/wadreader.h"

/*
Ok, so for now, we only need to have the read 4 bytes function.
read4bytes - parameters: pointer the m_Header, and offset from the beginning of m_Header
You create a 4 byte variable, ReadValue, and memcpy the stuff from the offset to ReadValue's address

You don't need to do anything for the wadreader constructor and deconstructor.
read_header():
Only need address of m_Header, and reference to the header object created.
Start by lookping through each element in the identification array
given that the offset parameter is 0, you just add 1 to the offset each time
identification[i] = p_WADData[offset + i] and so forth

Then use the read4bytes function to get the numnber of lumps (4 bytes) and offset to directory (4 bytes)


*/

Wadreader::Wadreader()
{
}

Wadreader::~Wadreader()
{
}

uint32_t Wadreader::read4bytes(const uint8_t *p_WADData, int offset)
{
    uint32_t value;
    memcpy(&value, p_WADData + offset, sizeof(uint32_t));
    return value;
}

uint16_t Wadreader::read2bytes(const uint8_t *p_WADData, int offset)
{
    uint16_t value;
    memcpy(&value, p_WADData + offset, sizeof(uint16_t));
    return value;
}

void Wadreader::read_header(const uint8_t *p_WADData, int offset, Header &header)
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

void Wadreader::read_directory(const uint8_t *p_WADData, int offset, Directory &directory)
{
    // 0x00 - 0x03
    directory.filepos = read4bytes(p_WADData, offset);

    // 0x04 - 0x07
    directory.size = read4bytes(p_WADData, offset + 4);

    // 0x08 - 0x0F
    directory.name[0] = p_WADData[offset + 8];
    directory.name[1] = p_WADData[offset + 9];
    directory.name[2] = p_WADData[offset + 10];
    directory.name[3] = p_WADData[offset + 11];
    directory.name[4] = p_WADData[offset + 12];
    directory.name[5] = p_WADData[offset + 13];
    directory.name[6] = p_WADData[offset + 14];
    directory.name[7] = p_WADData[offset + 15];
    directory.name[8] = '\0';
}