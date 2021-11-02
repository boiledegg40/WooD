#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "wad/wad.h"

static lumpinfo_t* lumpinfo;
static void** lumpcache;

static void read_header(std::ifstream& m_WADFile, wadinfo_t& header);
static void read_directory(std::ifstream& m_WADFile, int ofs, int numlumps);


void loadwad(int argc, char** argv)
{
    std::ifstream i_WADFile; // Class to open the file
    std::ifstream p_WADFile;
    int num_lumps = 0;
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
        std::printf("Opening IWAD file %s\n", IWAD_filepath.c_str()); // Load and read IWAD
        i_WADFile.open(IWAD_filepath, std::ifstream::binary);
        if (!i_WADFile.is_open())
        {
            std::printf("Error: failed to open IWAD file %s\n", IWAD_filepath.c_str());
            return;
        }
        read_header(i_WADFile, i_header);
        num_lumps += i_header.numlumps;
        lumpinfo = (lumpinfo_t*)(malloc(num_lumps * sizeof(lumpinfo_t)));
        read_directory(i_WADFile, i_header.infotableofs, i_header.numlumps);
        
        if (!(PWAD_filepath.empty())) // If PWAD is specified, then open and read
        {
            for(std::string& pwadfile : PWAD_filepath)
            {
                std::printf("Opening PWAD file %s\n", pwadfile.c_str());
                p_WADFile.open(pwadfile, std::ifstream::binary);
                if (!p_WADFile.is_open())
                {
                    std::printf("Error: failed to open PWAD file %s\n", pwadfile.c_str());
                    return;
                }
                read_header(p_WADFile, p_header);
                num_lumps += p_header.numlumps;
                lumpinfo = (lumpinfo_t*)(realloc(lumpinfo, num_lumps * sizeof(lumpinfo_t)));
                read_directory(p_WADFile, p_header.infotableofs, p_header.numlumps);
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

static void read_header(std::ifstream& m_WADFile, wadinfo_t& header)
{
    m_WADFile.seekg(m_WADFile.beg);
    m_WADFile.read((char*)&header.identification, 4);
    header.identification[4] = '\0';
    m_WADFile.read((char*)&header.numlumps, 4);
    m_WADFile.read((char*)&header.infotableofs, 4);
    m_WADFile.seekg(header.infotableofs);

    std::cout << "WAD info loaded into memory." << std::endl;

    std::cout << "WAD type: " << header.identification << std::endl;
    std::cout << "Number of lumps: " << header.numlumps << std::endl;
    std::cout << "Offset to the directory: " << header.infotableofs << std::endl;
    std::cout << std::endl;
}

static void read_directory(std::ifstream& m_WADFile, int ofs, int numlumps)
{
    static int lumpinfo_index = 0;
    for (int i = 0; i < numlumps; i++)
    {
        lumpinfo[lumpinfo_index].handle = m_WADFile.tellg();
        m_WADFile.read((char*)&lumpinfo[lumpinfo_index].pos, 4);
        m_WADFile.read((char*)&lumpinfo[lumpinfo_index].size, 4);
        m_WADFile.read((char*)&lumpinfo[lumpinfo_index].name, 8);
        lumpinfo[lumpinfo_index].name[8] = '\0';
        lumpinfo_index++;
    }

}
