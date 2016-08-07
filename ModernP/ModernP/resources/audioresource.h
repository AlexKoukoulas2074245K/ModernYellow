/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "resource.h"
#include <memory>

class AudioResource: public Resource
{
    friend class SResManager;
public:   

    bool load();        

private:

    explicit AudioResource(const string& resourceName);

};