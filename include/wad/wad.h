#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "wadtypes.h"

void loadwad(std::string sWADFilePath); // Function used to load the WAD. Will call other functions
extern std::vector<Directory> m_Directories; // A vector to keep track of the directory entries