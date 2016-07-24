/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "resource.h"
#include "../mydef.h"

#include <set>
#include <vector>

struct SDL_Texture;
struct SDL_Surface;
struct SDL_Rect;
class TextureResource: public Resource
{        
    friend class SResManager;

public:

    static std::shared_ptr<TextureResource> createEmptyTexture(
        const uint32 width, 
        const uint32 height);

public:    

    using pTexture_t      = std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)>;
    using pSurface_t      = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>;
    using pixelSnapshot_t = std::vector<std::vector<const uint32>>;

    bool load() override;
    
    /* After manual pixel manipulation on the surface, the texture needs to
       be recompiled to display the changes */
    void compileTexture();

    /* Changes all pixels with the src color (if any) to the dst color. Manual recompilation 
       of the texture is not needed as it is done automatically after the swap */
    void swapColor(const uint32 src, const uint32 dst);

    /* Darkens the surface (and as the result the texture) one step */
    void darken();

    void darken(const SDL_Rect& rect);        

    /* Constructs and returns a snapshot of the pixels in the original texture
       before starting the wild pokemon encounter animation */
    void getPixelSnapshot(const SDL_Rect& area, pixelSnapshot_t& output) const;

    /* Swaps pixels in the given area to produce the wild pokemon encounter animation 
       for the step given */
    void wildPokemonAnimation(
        const pixelSnapshot_t& snapshot, 
        const SDL_Rect& rect, 
        const uint32 step);

    std::shared_ptr<TextureResource> getSubTexture(
        const uint32 tu,
        const uint32 tv,
        const uint32 width, 
        const uint32 height) const;

    std::shared_ptr<TextureResource> getHorFlippedTexture() const;

    const pTexture_t& getTexture() const;

    const pSurface_t& getSurface() const;

          pSurface_t& getSurface();

private:

    explicit TextureResource(const string& resourceName);

    uint32 getPixelAt(const uint32 x, const uint32 y) const;

    void setPixelAt(
        const uint32 rgb, 
        const uint32 x,
        const uint32 y);    

private:
    
    pTexture_t m_pTexture;
    pSurface_t m_pSurface;    
    
};