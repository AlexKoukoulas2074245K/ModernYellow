/* ======================
   Date: 13/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "dataresource.h"
#include "../portcommon.h"

extern string g_datPath;

/* ==============
   Public Methods
   ============== */
bool DataResource::load()
{
    if (!file_util::exists(g_datPath + m_name))
    {        
        SDL_FORCE_DISPLAY_ERROR("Resource: " + m_name + " could not be found.");
        return false;
    }

    // Create streams
    std::ifstream dataFile(g_datPath + m_name);
    
    string line;    
    while(std::getline(dataFile, line))
    {
        m_content.push_back(line);
    }

    return true;
}

const std::vector<string>& DataResource::getContent() const
{
    return m_content;
}

/* =============== 
   Private Methods
   =============== */
DataResource::DataResource(const string& resourceName):
    Resource(resourceName)
{
}

