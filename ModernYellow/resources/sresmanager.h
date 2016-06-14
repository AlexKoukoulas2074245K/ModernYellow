/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include <memory>
#include <unordered_map>
#include <string>

// Instance retrieval convenince macro
#define resmanager SResManager::instance()

// Downcasting to individual resource types
#define castResToTex(pRes)   (std::dynamic_pointer_cast<TextureResource>(pRes))
#define castResToData(pRes)  (std::dynamic_pointer_cast<DataResource>(pRes))
#define castResToAudio(pRes) (std::dynamic_pointer_cast<AudioResource>(pRes))

// Resource type public enum
enum ResourceType { RT_TEXTURE, RT_AUDIO, RT_DATA };

using std::string;
using std::weak_ptr;
using std::shared_ptr;
using std::unordered_map;

class Resource;
class SResManager
{    
public:

    static SResManager& instance();

public:
    
    /* Attempts to retrieve and return a resource with the given name. a nullptr will
       be returned in the case of failure. Subsequent attemtps to retrieve the same resource
       will yield faster performance due to resource caching */
    shared_ptr<Resource> loadResource(const string& resourceName,
                                      const ResourceType resourceType);

    /* Returns the number of cached resources with the given name (should be 1 or 0 normally) */
    int isResourceCached(const string& resourceName) const;
    
private:

    SResManager();

private:

    using ResourceTable = unordered_map<string, weak_ptr<Resource>>;

    ResourceTable m_resources;
};