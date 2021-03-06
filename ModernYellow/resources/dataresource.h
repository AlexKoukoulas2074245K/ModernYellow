/* ======================
   Date: 13/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "resource.h"
#include "../mydef.h"

#include <vector>

class DataResource: public Resource
{
    friend class SResManager;
public:    

    bool load() override;

    const std::vector<string>& getContent() const;
   
private:

    explicit DataResource(const string& resourceName);

private:

    std::vector<string> m_content;

};