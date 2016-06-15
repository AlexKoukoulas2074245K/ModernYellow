/* ======================
   Date: 13/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "resource.h"
#include <memory>
#include <vector>

class DataResource: public Resource
{
    friend class SResManager;
public:    

    bool load();

    const std::vector<string>& getContent() const;
   
private:

    explicit DataResource(const string& resourceName);

private:

    std::vector<string> m_content;

};