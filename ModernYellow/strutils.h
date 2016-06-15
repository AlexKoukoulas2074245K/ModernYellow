/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace string_utils
{
    void split(const std::string& s, char delim, std::vector<std::string>& elems)
    {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            elems.push_back(item);
        }
    }

    std::vector<std::string> split(const std::string& s, char delim)
    {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }
}

