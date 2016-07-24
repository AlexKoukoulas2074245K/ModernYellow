/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "resource.h"

/* ==============
   Public Methods
   ============== */
Resource::Resource(const string& resourceName):
    m_name(resourceName)
{
}

Resource::~Resource(){}

const string& Resource::getName() const
{
    return m_name;
}

