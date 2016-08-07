/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#include "sresmanager.h"
#include "resource.h"
#include "audioresource.h"
#include "textureresource.h"
#include "dataresource.h"

/* =====================
   Public Static Methods
   ===================== */
SResManager& SResManager::instance()
{
    static SResManager instance;
    return instance;
}

/* ==============
   Public Methods
   ============== */

shared_ptr<Resource> SResManager::loadResource(const string& resourceName,
                                               const ResourceType resourceType)
{    
    if (isResourceCached(resourceName))
    {
        // Check that the cached resource has not expired (there are still
        // shared pointers pointing to it) before returning it
        if (!m_resources[resourceName].expired())
        {
            return m_resources[resourceName].lock();
        }
        // Else remove entry and start resource loading normally
        else
        {
            m_resources.erase(resourceName);
        }
    }
       
    shared_ptr<Resource> pResource;    
    
    // Create the appropriate type of resource
    switch (resourceType)
    {        
        case RT_TEXTURE: pResource.reset(new TextureResource(resourceName)); break;
        case RT_AUDIO:   pResource.reset(new AudioResource(resourceName)); break;
        case RT_DATA:    pResource.reset(new DataResource(resourceName)); break;
    }
    
    // Attempt to load it
    if (!pResource->load()) return nullptr;

    // Cache it
    m_resources[resourceName] = weak_ptr<Resource>(pResource);
    
    return pResource;
}

int SResManager::isResourceCached(const string& resourceName) const
{
    return m_resources.count(resourceName);
}

/* ===============
   Private Methods
   =============== */
SResManager::SResManager()
{
}
