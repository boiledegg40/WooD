#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "wad/wad.h"

static lumpinfo_t* lumpinfo; // Holds directory entries so we could use this to access lumps in file and load to memory
static void** lumpcache; // This array will hold pointers to memory blocks that will hold lumps. Mirrors the lumpinfo array
static int num_lumps = 0;

static void read_header(std::ifstream& m_WADFile, wadinfo_t& header, const char* wadtype);
static void read_directory(std::ifstream& m_WADFile, int ofs, int numlumps, const char* filename);

static std::ifstream i_WADFile; // Class to open the file
static std::ifstream p_WADFile;


void loadwad(int argc, char** argv)
{
    std::string IWAD_filepath = "";
    // Can only support one PWAD for now
    std::vector<std::string> PWAD_filepath;
    if (argc <= 2) // If no valid flags, then give tip
    {
        std::printf("Usage is -i <IWAD file> -p <PWAD file (if applicable)>\n");
    }
    else
    {
        wadinfo_t i_header;
        wadinfo_t p_header;
        for (int i = 1; i < argc; i++) // Check for and supplied arguments (The wad file)
        {
            if ((i + 1) != argc)
            {
                if ((std::strcmp(argv[i], "-i")) == 0)
                {
                    IWAD_filepath = argv[i + 1];
                }
                else if ((std::strcmp(argv[i], "-p")) == 0)
                {
                    PWAD_filepath.push_back(argv[i + 1]);
                }
            }
        }
        std::printf("loadwad(): Opening IWAD file %s\n", IWAD_filepath.c_str()); // Load and read IWAD
        i_WADFile.open(IWAD_filepath, std::ifstream::binary);
        if (!i_WADFile.is_open())
        {
            std::printf("loadwad(): Error - failed to open IWAD file %s\n", IWAD_filepath.c_str());
            return;
        }
        read_header(i_WADFile, i_header, "IWAD");
        if (strcmp(i_header.identification, "IWAD") != 0)
        {
            std::printf("loadwad(): %s is not a valid IWAD!\n", IWAD_filepath.c_str());
            return;
        }
        num_lumps += i_header.numlumps;
        lumpinfo = (lumpinfo_t*)(malloc(num_lumps * sizeof(lumpinfo_t)));
        lumpcache = (void**)(malloc(num_lumps * sizeof(void*))); // Allocate size for the lumpcache array
        read_directory(i_WADFile, i_header.infotableofs, i_header.numlumps, IWAD_filepath.c_str());
        
        if (!(PWAD_filepath.empty())) // If PWAD is specified, then open and read
        {
            for(std::string& pwadfile : PWAD_filepath) // Loop through each pwad
            {
                std::printf("Opening PWAD file %s\n", pwadfile.c_str());
                p_WADFile.open(pwadfile, std::ifstream::binary);
                if (!p_WADFile.is_open())
                {
                    std::printf("Error: failed to open PWAD file %s\n", pwadfile.c_str());
                    return;
                }
                read_header(p_WADFile, p_header, "PWAD");
                if (strcmp(p_header.identification, "PWAD") != 0)
                {
                    std::printf("loadwad(): %s is not a valid PWAD!\n", IWAD_filepath.c_str());
                    return;
                }
                num_lumps += p_header.numlumps;
                lumpinfo = (lumpinfo_t*)(realloc(lumpinfo, num_lumps * sizeof(lumpinfo_t)));
                lumpcache = (void**)(realloc(lumpcache, num_lumps * sizeof(void*)));
                read_directory(p_WADFile, p_header.infotableofs, p_header.numlumps, pwadfile.c_str());
                p_WADFile.close();
                p_WADFile.clear();
            }
        }
        std::printf("Total num of lumps: %d\n", num_lumps);
        i_WADFile.close();
    }

}

/*
The wad reader used to load the entire wad into memory, which would not be very efficient
Now, it only reads the header to get to the directory, and puts the directory into an array
Doesn't load the lumps themselves
After the custom malloc was implemented, we can now specific lumps as needed
*/

static void read_header(std::ifstream& m_WADFile, wadinfo_t& header, const char* wadtype)
{
    m_WADFile.seekg(m_WADFile.beg);
    try
    {
        m_WADFile.read((char*)&header.identification, 4);
        header.identification[4] = '\0';
        if (strcmp(wadtype, header.identification) != 0)
        {
        std::printf("loadwad(): Loaded wad is not valid %s", wadtype);
        throw std::exception();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    
    
    m_WADFile.read((char*)&header.numlumps, 4);
    m_WADFile.read((char*)&header.infotableofs, 4);
    m_WADFile.seekg(header.infotableofs);

    std::cout << "WAD info loaded into memory." << std::endl;

    std::cout << "WAD type: " << header.identification << std::endl;
    std::cout << "Number of lumps: " << header.numlumps << std::endl;
    std::cout << "Offset to the directory: " << header.infotableofs << std::endl;
    std::cout << std::endl;
}

/*
read_directory takes in 3 arguments: m_WADFile, which is a reference to the ifstream object, 
ofs which is offset to directory, 
numlumps, the number of lumps in the wad

It creates an static int lumpinfo_index, which is meant to last so we can read from different files and add to array without doing some janky offset
Loop through each directory entry, and add it to a struct (wadtypes.h) in lumpinfo array
Set the mirrored array lumpcache to NULL 
Increment lumpinfo_index
*/

static void read_directory(std::ifstream& m_WADFile, int ofs, int numlumps, const char* filename)
{
    static int lumpinfo_index = 0;
    for (int i = 0; i < numlumps; i++)
    {
        lumpinfo[lumpinfo_index].handle = m_WADFile.tellg();
        m_WADFile.read((char*)&lumpinfo[lumpinfo_index].pos, 4);
        m_WADFile.read((char*)&lumpinfo[lumpinfo_index].size, 4);
        m_WADFile.read((char*)&lumpinfo[lumpinfo_index].name, 8);
        lumpinfo[lumpinfo_index].name[8] = '\0';
        lumpcache[lumpinfo_index] = NULL;
        lumpinfo[lumpinfo_index].filename = strdup(filename);
        lumpinfo_index++;
    }

}


int findlump(const char* name)
{
    for (int i = num_lumps - 1; i >= 0; i--)
    {
        if (strcmp(lumpinfo[i].name, name) == 0)
        {
            return i;
        }
    }
    std::printf("findlump(): No lump with name %s found\n", name);
    return -1;
}