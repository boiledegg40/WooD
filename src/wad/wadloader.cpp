#include <iostream>
#include "wad/wadloader.h"

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

    if (!read_directory)
    {

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
    size_t = m_WADFile.tellg();

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

    m_WADFile.seekg(ifstream::beg);
    m_WADFile.read((char *)m_WADData, length);

    m_WADFile.close();

    std::cout << "WAD loaded into memory." << std::endl;
    return true;
}

wadloader::read_directory()
{
    
}
