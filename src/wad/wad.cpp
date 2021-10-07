#include <iostream>
#include "wad/wad.h"

static Header header; // Keep the file in memory.
std::vector<Directory> m_Directories; // A vector to keep track of the directory entries

static void read_header(std::ifstream& m_WADFile);
static void read_directory(std::ifstream& m_WADFile);


void loadwad(std::string sWADFilePath)
{
    std::ifstream m_WADFile; // Class to open the file
    std::cout << "Opening WAD file..." << std::endl;
    m_WADFile.open(sWADFilePath, std::ifstream::binary);
    if (!m_WADFile.is_open())
    {
        std::cout << "Error: Failed to open WAD file " << sWADFilePath << std::endl;
        return;
    }
    read_header(m_WADFile);
    read_directory(m_WADFile);

    m_WADFile.close();
}

/*
For now, open_wad() loads the entire wad file to memory
In the future, just read the header and directories to memory, 
Then go back to load lumps as needed.
Need to implement in the future
*/

static void read_header(std::ifstream& m_WADFile)
{
    m_WADFile.seekg(m_WADFile.beg);
    m_WADFile.read((char*)&header.identification, 4);
    header.identification[4] = '\0';
    m_WADFile.read((char*)&header.numlumps, 4);
    m_WADFile.read((char*)&header.infotableofs, 4);

    std::cout << "WAD header loaded into memory." << std::endl;

    std::cout << "WAD type: " << header.identification << std::endl;
    std::cout << "Number of lumps: " << header.numlumps << std::endl;
    std::cout << "Offset to the directory: " << header.infotableofs << std::endl;
    std::cout << std::endl;
}

static void read_directory(std::ifstream& m_WADFile)
{
    std::cout << "PLaceholder :)" << std::endl;
    // Directory directory;

    // for (unsigned int i = 0; i < header.numlumps; i++)
    // {
    //     wadreader.read_directory(m_Header, header.infotableofs + i * 16, directory);
    //     m_directories.push_back(directory);
    // }
}
