#pragma once

#include "wad/wadtypes.h"

class Wadreader
{
    public:
        Wadreader();
        ~Wadreader();

        void read_header(const uint8_t *p_WADData, int offset, Header &header);
        void read_directory(const uint8_t *p_WADData, int offset, Directory &directory);

    protected:
        uint16_t read2bytes(const uint8_t *p_WADData, int offset);
        uint32_t read4bytes(const uint8_t *p_WADData, int offset);
};