#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "wadtypes.h"

class Wadloader
{
    public: 
        Wadloader(std::string sWADFilePath); // Path of the file
        bool loadwad(); // Function used to load the WAD. Will call other functions

        ~Wadloader(); // Deconstructor - Point m_WADData to Null

    protected:
        bool open_wad();
        bool read_directory();

        std::string m_sWADFilePath; // Keep path of the file in memory
        std::ifstream m_WADFile; // Class to open the file
        uint8_t *m_WADData; // Keep the file in memory. 
        std::vector<Directory> m_directories; // A vector to keep track of the directory entries
};