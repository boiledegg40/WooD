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

    return true;
}

bool wadloader::open_wad()
{
    std::cout << "Opening WAD file..." << std::endl;
    m_WADFile.open(m_sWADFilePath);
    if (!m_WADFile.is_open())
    {
        std::cout << "Error: Failed to open WAD file " << m_sWADFilePath << std::endl;
        return false;
    }
    
}
