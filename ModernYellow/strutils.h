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
    void split(const std::string& s, char delim, std::vector<std::string>& elems);

    std::vector<std::string> split(const std::string& s, char delim);

    bool replace(std::string& s, const std::string& oldVal, const std::string& newVal);

    bool startsWith(const std::string& s, const std::string& pattern);
}

