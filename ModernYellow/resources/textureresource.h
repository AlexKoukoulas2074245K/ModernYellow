/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "resource.h"
#include "../mydef.h"
#include <memory>
#include <set>

struct SDL_Texture;
struct SDL_Surface;
class TextureResource: public Resource
{    
    friend class SResManager;
public:    

    using pTexture_t = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;
    using pSurface_t = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;

    bool load();
    
    /* After manual pixel manipulation on the surface, the texture needs to
       be recompiled to display the changes */
    void compileTexture();

    /* Changes all pixels with the src color (if any) to the dst color. Manual recompilation 
       of the texture is not needed as it is done automatically after the swap */
    void swapColor(const uint32 src, const uint32 dst);

    /* Darkens the surface (and as the result the texture) one step */
    void darken();

    const pTexture_t& getTexture() const;

    const pSurface_t& getSurface() const;

          pSurface_t& getSurface();

private:

    explicit TextureResource(const string& resourceName);

    void gatherSurfacePixelData();

    uint32 getPixelAt(const uint32 x, const uint32 y) const;

    void setPixelAt(const uint32 rgb, const uint32 x, const uint32 y);    

private:
    
    pTexture_t m_texture;
    pSurface_t m_surface;
    std::set<uint32> m_colors;

};