#include <iostream>
#include <cstdlib>
#include <cstring>
#include "wad/wad.h"

static lumpinfo_t* lumpinfo;

static void read_header(std::ifstream& m_WADFile, wadinfo_t& header);
static void read_directory(std::ifstream& m_WADFile);


void loadwad(int argc, char** argv)
{
    std::ifstream m_WADFile; // Class to open the file
    int num_lumps = 0;
    std::string IWAD_filepath = "";
    std::string PWAD_filepath = "";
    if (argc <= 2) // If no valid flags, then give tip
    {
        std::printf("Usage is -i <IWAD file> -p <PWAD file (if applicable)>\n");
    }
    else
    {
        wadinfo_t header;
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
                    PWAD_filepath = argv[i + 1];
                }
            }
        }
        std::printf("Opening IWAD file...\n"); // Load and read IWAD
        m_WADFile.open(IWAD_filepath, std::ifstream::binary);
        if (!m_WADFile.is_open())
        {
            std::printf("Error: failed to open IWAD file %s\n", PWAD_filepath.c_str());
            return;
        }
        read_header(m_WADFile, header);
        num_lumps += header.numlumps;
        read_directory(m_WADFile);

        m_WADFile.close();
        m_WADFile.clear();
        if (!(PWAD_filepath == "")) // If PWAD is specified, then open and read
        {
            std::printf("Opening PWAD file...\n");
            m_WADFile.open(PWAD_filepath, std::ifstream::binary);
            if (!m_WADFile.is_open())
            {
                std::printf("Error: failed to open PWAD file %s\n", PWAD_filepath.c_str());
                return;
            }
            read_header(m_WADFile, header);
            num_lumps += header.numlumps;
            read_directory(m_WADFile);

            m_WADFile.close();
            m_WADFile.clear();
        }
        std::printf("Total num of lumps: %d\n", num_lumps);
//      lumpinfo = malloc(num_lumps * sizeof(lumpinfo_t));

    }

}

/*
For now, open_wad() loads the entire wad file to memory
In the future, just read the header and directories to memory, 
Then go back to load lumps as needed.
Need to implement in the future
*/

static void read_header(std::ifstream& m_WADFile, wadinfo_t& header)
{
    m_WADFile.seekg(m_WADFile.beg);
    m_WADFile.read((char*)&header.identification, 4);
    header.identification[4] = '\0';
    m_WADFile.read((char*)&header.numlumps, 4);
    m_WADFile.read((char*)&header.infotableofs, 4);

    std::cout << "WAD info loaded into memory." << std::endl;

    std::cout << "WAD type: " << header.identification << std::endl;
    std::cout << "Number of lumps: " << header.numlumps << std::endl;
    std::cout << "Offset to the directory: " << header.infotableofs << std::endl;
    std::cout << std::endl;
}

static void read_directory(std::ifstream& m_WADFile)
{
    std::printf("read_directory(): Placeholder\n");
}
