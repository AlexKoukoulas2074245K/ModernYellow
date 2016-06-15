/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "textureresource.h"
#include <SDL_image.h>
#include "../portcommon.h"

extern string g_texPath;
extern SDL_Renderer* g_renderer;

/* ==============
   Public Methods
   ============== */
bool TextureResource::load()
{
    if (!file_util::exists(g_texPath + m_name))
    {
        SDL_FORCE_DISPLAY_ERROR("Resource: " + m_name + " could not be found.");
        return false;
    }

    // Load surface
    m_surface.reset(IMG_Load((g_texPath + m_name).c_str()));

    // Compile texture
    compileTexture();
    
    return true;
}

void TextureResource::compileTexture()
{    
    gatherSurfacePixelData();
    m_texture.reset(SDL_CreateTextureFromSurface(g_renderer, m_surface.get()));
}

void TextureResource::swapColor(const uint32 src, const uint32 dst)
{
    if (SDL_MUSTLOCK(m_surface.get()))
    {
        SDL_LockSurface(m_surface.get());
    }

    int32 w = m_surface.get()->w;
    int32 h = m_surface.get()->h;

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            if (getPixelAt(x, y) == src)
            {
                setPixelAt(dst, x, y);
            }
        }
    }

    compileTexture();

    if (SDL_MUSTLOCK(m_surface.get()))
    {
        SDL_UnlockSurface(m_surface.get());
    }
}

void TextureResource::darken()
{
    if (SDL_MUSTLOCK(m_surface.get()))
    {
        SDL_LockSurface(m_surface.get());
    }

    for (int y = 0; y < m_surface->h; ++y)
    {
        for (int x = 0; x < m_surface->w; ++x)
        {
            // Grab curr pixel
            uint32 currPixel = getPixelAt(x, y);

            // Get the position of the pixel in the color set
            auto posIter = m_colors.find(currPixel);

            // if the pixel is not the darkest (std::set is ordered so
            // darkest color will reside at set.begin()), make it one step darker
            if (posIter != m_colors.begin())
            {
                uint32 nextPixel = *(--posIter);
                setPixelAt(nextPixel, x, y);
            }            
        }
    }

    compileTexture();

    if (SDL_MUSTLOCK(m_surface.get()))
    {
        SDL_UnlockSurface(m_surface.get());
    }
}

const TextureResource::pTexture_t& TextureResource::getTexture() const
{
    return m_texture;
}

const TextureResource::pSurface_t& TextureResource::getSurface() const
{
    return m_surface;
}

TextureResource::pSurface_t& TextureResource::getSurface()
{
    return m_surface;
}

/* ===============
   Private Methods
   =============== */
TextureResource::TextureResource(const string& resourceName):    
    Resource(resourceName),
    m_texture(nullptr, SDL_DestroyTexture),
    m_surface(nullptr, SDL_FreeSurface)
{
}

void TextureResource::gatherSurfacePixelData()
{
    m_colors.clear();
    for (int y = 0; y < m_surface->h; ++y)
    {
        for (int x = 0; x < m_surface->w; ++x)
        {
            m_colors.insert(getPixelAt(x, y));
        }
    }
}

uint32 TextureResource::getPixelAt(const uint32 x, const uint32 y) const
{    
    int bpp = m_surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *) m_surface->pixels + y * m_surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *) p;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *) p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void TextureResource::setPixelAt(const uint32 pixel, const uint32 x, const uint32 y)
{ 
    int bpp = m_surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *) m_surface->pixels + y * m_surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *) p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *) p = pixel;
        break;
    }
}