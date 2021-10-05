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

        ~Wadloader(); // Deconstructor - does nothing

    protected:
        bool read_header();
        bool read_directory();

        std::string m_sWADFilePath; // Keep path of the file in memory
        std::ifstream m_WADFile; // Class to open the file
        Header header; // Keep the file in memory. 
        std::vector<Directory> m_directories; // A vector to keep track of the directory entries
};