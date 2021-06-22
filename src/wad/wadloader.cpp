#include <iostream>
#include "wad/wadloader.h"
#include "wad/wadreader.h"

wadloader::wadloader(std::string sWADFilePath) : m_WADData(NULL), m_sWADFilePath(sWADFilePath)
{

}

wadloader::~wadloader()
{
    delete[] m_WADData;
}

bool wadloader::loadwad()
{
    if (!open_wad())
    {
        return false;
    }

    if (!read_directory())
    {
        return false;
    }
    return true;
}

bool wadloader::open_wad()
{
    std::cout << "Opening WAD file..." << std::endl;
    m_WADFile.open(m_sWADFilePath, std::ifstream::binary);
    if (!m_WADFile.is_open())
    {
        std::cout << "Error: Failed to open WAD file " << m_sWADFilePath << std::endl;
        return false;
    }
    
    m_WADFile.seekg(0, m_WADFile.end);
    size_t length = m_WADFile.tellg();

    if (m_WADData)
    {
        delete[] m_WADData;
        m_WADData = NULL;
    }

    m_WADData = new uint8_t[length];
    if (m_WADData == NULL)
    {
        std::cout << "Error: failed to allocate memory for WAD file of size " << length << std::endl;
        return false;
    }

    m_WADFile.seekg(std::ifstream::beg);
    m_WADFile.read((char *)m_WADData, length);

    m_WADFile.close();

    std::cout << "WAD loaded into memory." << std::endl;
    return true;
}

bool wadloader::read_directory()
{
    header header;
    wadreader wadreader;
    wadreader.read_header(m_WADData, 0, header);

    std::cout << "WAD type: " << header.identification << std::endl;
    std::cout << "Number of lumps: " << header.numlumps << std::endl;
    std::cout << "Offset to the directory: " << header.infotableofs << std::endl;
    std::cout << std::endl;

    directory directory;

    for (unsigned int i = 0; i < header.numlumps; i++)
    {
        wadreader.read_directory(m_WADData, header.infotableofs + i * 16, directory);
        m_directories.push_back(directory);

        std::cout << "Lump offset: " << directory.filepos << std::endl;
        std::cout << "Lump size: " << directory.size << std::endl;
        std::cout << "Lump name: " << directory.name << std::endl;
        
    }
    
    return true;
}
