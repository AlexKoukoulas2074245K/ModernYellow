/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "audioresource.h"

/* ==============
   Public Methods
   ============== */
bool AudioResource::load()
{
    return true;
}

/* ===============
   Private Methods
   =============== */
AudioResource::AudioResource(const string& resourceName):    
    Resource(resourceName)
{
}
