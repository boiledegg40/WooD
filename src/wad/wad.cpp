#include <iostream>
#include "wad/wad.h"

Wadloader::Wadloader(std::string sWADFilePath) : m_sWADFilePath(sWADFilePath)
{

}

Wadloader::~Wadloader()
{
    
}

bool Wadloader::loadwad()
{
    std::cout << "Opening WAD file..." << std::endl;
    m_WADFile.open(m_sWADFilePath, std::ifstream::binary);
    if (!m_WADFile.is_open())
    {
        std::cout << "Error: Failed to open WAD file " << m_sWADFilePath << std::endl;
        return false;
    }
    read_header();
    read_directory();

    m_WADFile.close();
    return true;
}

/*
For now, open_wad() loads the entire wad file to memory
In the future, just read the header and directories to memory, 
Then go back to load lumps as needed.
Need to implement in the future
*/

bool Wadloader::read_header()
{
    m_WADFile.seekg(m_WADFile.beg);
    m_WADFile.read((char*)&header.identification, 4);
    header.identification[4] = '\0';
    m_WADFile.read((char*)&header.numlumps, 4);
    m_WADFile.read((char*)&header.infotableofs, 4);

    std::cout << "WAD header loaded into memory." << std::endl;
}

bool Wadloader::read_directory()
{

    std::cout << "WAD type: " << header.identification << std::endl;
    std::cout << "Number of lumps: " << header.numlumps << std::endl;
    std::cout << "Offset to the directory: " << header.infotableofs << std::endl;
    std::cout << std::endl;
    
    // Directory directory;

    // for (unsigned int i = 0; i < header.numlumps; i++)
    // {
    //     wadreader.read_directory(m_Header, header.infotableofs + i * 16, directory);
    //     m_directories.push_back(directory);
    // }
    
    return true;
}
