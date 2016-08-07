/* ======================
   Date: 13/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "dataresource.h"
#include "../portcommon.h"
#include <sstream>

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
    std::stringstream dataSS;
    
    // Copy file contents to stringstream
    dataSS << dataFile.rdbuf();

    // Construct content from data stream
    m_content = dataSS.str();

    return true;
}

const string& DataResource::getContent() const
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

