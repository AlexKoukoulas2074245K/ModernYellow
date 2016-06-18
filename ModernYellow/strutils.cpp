/* ======================
   Date: 17/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "strutils.h"

void string_utils::split(const std::string& s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
}

std::vector<std::string> string_utils::split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool string_utils::startsWith(const std::string& s, const std::string& pattern)
{
    for (size_t i = 0; i < pattern.length(); ++i)
    {
        if (s[i] != pattern[i]) return false;
    }

    return true;
}