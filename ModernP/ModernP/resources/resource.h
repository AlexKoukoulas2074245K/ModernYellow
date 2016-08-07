/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include <string>

using std::string;

class Resource
{
public:

    Resource(const string& resourceName);
    
    virtual ~Resource();

    virtual bool load() = 0;

    virtual string getName() const;

protected:

    string m_name;

};