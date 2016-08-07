/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "resource.h"
#include <memory>

struct SDL_Texture;
struct SDL_Deleter;

class TextureResource: public Resource
{    
    friend class SResManager;
public:    
    using ptexture_t = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;
    
    bool load();

    const ptexture_t& getTexture() const;

private:

    explicit TextureResource(const string& resourceName);

private:
    
    ptexture_t m_texture;

};