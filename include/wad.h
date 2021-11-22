#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "wadtypes.h"
#include "map.h"

void loadwad(int argc, char** argv); // Function used to load the WAD. Will call other functions
int findlump(const char* name);
void* loadlump(int index, int tag);
void* loadmap(const char* mapname, MapData lump_type);