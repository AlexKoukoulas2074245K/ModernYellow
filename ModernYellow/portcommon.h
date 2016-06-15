/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include <SDL_messagebox.h>
#include <string>
#include <fstream>

/* Custom SDL core error checking function */
inline int SDLR(int sdlres)
{
    if (sdlres)
    {
        std::string err = SDL_GetError();
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_INFORMATION,
            "Notice:",
            err.c_str(),
            nullptr);
    }
    return sdlres;
}

/* Force a messge box displaying the given error */
inline void SDL_FORCE_DISPLAY_ERROR(const std::string& error)
{
    SDL_SetError(error.c_str());
    SDLR(-1);
}

namespace file_util
{
    using std::string;
    using std::ifstream;

    inline bool exists(const std::string& filePath)
    {
        std::ifstream file(filePath);        
        return file.good();
    }    
}