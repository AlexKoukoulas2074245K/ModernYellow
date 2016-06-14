/* ======================
   Date: 13/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "resource.h"
#include <memory>

class DataResource: public Resource
{
    friend class SResManager;
public:    

    bool load();

    const string& getContent() const;
   
private:

    explicit DataResource(const string& resourceName);

private:

    string m_content;

};