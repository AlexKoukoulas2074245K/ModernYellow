/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "textureresource.h"
#include "../portcommon.h"

#include <SDL_image.h>

extern string g_texPath;
extern uint32 g_overworldTilemapColor;
extern uint32 g_currentLevelColor;
extern pRenderer_t g_pRenderer;

/* Color Masks */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define RMASK 0xff000000
    #define GMASK 0x00ff0000
    #define BMASK 0x0000ff00
    #define AMASK 0x000000ff
#else
    #define RMASK 0x000000ff
    #define GMASK 0x0000ff00
    #define BMASK 0x00ff0000
    #define AMASK 0xff000000
#endif

/* ==============
   Static Methods
   ============== */
std::shared_ptr<TextureResource> TextureResource::createEmptyTexture(
    const uint32 width, 
    const uint32 height)
{
    // Create an empty texture resource
    auto pTexRes = std::shared_ptr<TextureResource>(new TextureResource("empty"));

    // Create an SDL_Surface with the given attributes
    auto pSurface = SDL_CreateRGBSurface(
        0, 
        width, 
        height, 
        BITS_PER_PIXEL, 
        RMASK, 
        GMASK, 
        BMASK, 
        AMASK);

    // Set the surface of the result to the newly created surface
    pTexRes->getSurface() = pSurface_t(pSurface, SDL_FreeSurface);

    return pTexRes;
}

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
    m_pSurface.reset(IMG_Load((g_texPath + m_name).c_str()));

    // Compile texture
    compileTexture();
    
    return true;
}

void TextureResource::compileTexture()
{        
    m_pTexture.reset(SDL_CreateTextureFromSurface(g_pRenderer.get(), m_pSurface.get()));
}

void TextureResource::swapColor(const uint32 src, const uint32 dst)
{
    if (src == dst) return;
    if (SDL_MUSTLOCK(m_pSurface.get()))
    {
        SDL_LockSurface(m_pSurface.get());
    }

    int32 w = m_pSurface.get()->w;
    int32 h = m_pSurface.get()->h;

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

    if (SDL_MUSTLOCK(m_pSurface.get()))
    {
        SDL_UnlockSurface(m_pSurface.get());
    }
}

void TextureResource::getPixelSnapshot(const SDL_Rect& area, pixelSnapshot_t& output) const
{    
    for (auto y = 0; y < area.h; ++y)
    {
        output.push_back(std::vector<const uint32>(area.w));
        for (auto x = 0; x < area.w; ++x)
        {
            output[y][x] = getPixelAt(x + area.x, y + area.y);
        }
    }
}

void TextureResource::wildPokemonAnimation(
    const pixelSnapshot_t& snapshot, 
    const SDL_Rect& rect,
    const uint32 step)
{    
    if (SDL_MUSTLOCK(m_pSurface.get()))
    {
        SDL_LockSurface(m_pSurface.get());
    }
    
    switch (step)
    {
        case 1:
        case 5:
        {
            for (int y = 0; y < rect.h; ++y)
            {
                for (int x = 0; x < rect.w; ++x)
                {
                    const auto currPixel = snapshot[y][x];

                    if      (currPixel == envcolors::EC_WHITE) setPixelAt(g_currentLevelColor, x + rect.x, y + rect.y);
                    else if (currPixel == g_currentLevelColor) setPixelAt(envcolors::EC_CBLUE, x + rect.x, y + rect.y);
                    else if (currPixel == envcolors::EC_CBLUE) setPixelAt(envcolors::EC_BLACK, x + rect.x, y + rect.y);                    
                }
            }
        } break;

        case 2:
        case 4:
        {
            for (int y = 0; y < rect.h; ++y)
            {
                for (int x = 0; x < rect.w; ++x)
                {
                    const auto currPixel = snapshot[y][x];

                    if (currPixel == envcolors::EC_WHITE) setPixelAt(envcolors::EC_CBLUE, x + rect.x, y + rect.y);
                    else setPixelAt(envcolors::EC_BLACK, x + rect.x, y + rect.y);                    
                }
            }
        } break;

        case 3:
        {
            for (int y = 0; y < rect.h; ++y)
            {
                for (int x = 0; x < rect.w; ++x)
                {                                        
                    setPixelAt(envcolors::EC_BLACK, x + rect.x, y + rect.y);
                }
            }
        } break;

        case 6:
        case 12:
        {
            for (int y = 0; y < rect.h; ++y)
            {
                for (int x = 0; x < rect.w; ++x)
                {
                    setPixelAt(snapshot[y][x], x + rect.x, y + rect.y);
                }
            }
        } break;

        case 7:
        case 11:
        {
            for (int y = 0; y < rect.h; ++y)
            {
                for (int x = 0; x < rect.w; ++x)
                {
                    const auto currPixel = snapshot[y][x];

                    if (currPixel == g_currentLevelColor) setPixelAt(envcolors::EC_WHITE, x + rect.x, y + rect.y);
                    else if (currPixel == envcolors::EC_CBLUE) setPixelAt(g_currentLevelColor, x + rect.x, y + rect.y);
                    else if (currPixel == envcolors::EC_BLACK) setPixelAt(envcolors::EC_CBLUE, x + rect.x, y + rect.y);
                }
            }
        } break;
 
        case 8:
        case 10:
        {
            for (int y = 0; y < rect.h; ++y)
            {
                for (int x = 0; x < rect.w; ++x)
                {
                    const auto currPixel = snapshot[y][x];

                    if (currPixel == envcolors::EC_BLACK) setPixelAt(envcolors::EC_CBLUE, x + rect.x, y + rect.y);
                    else setPixelAt(envcolors::EC_WHITE, x + rect.x, y + rect.y);
                }
            }
        } break;

        case 9:
        {
            for (int y = 0; y < rect.h; ++y)
            {
                for (int x = 0; x < rect.w; ++x)
                {
                    setPixelAt(envcolors::EC_WHITE, x + rect.x, y + rect.y);
                }
            }
        } break;
    }

    compileTexture();
    
    if (SDL_MUSTLOCK(m_pSurface.get()))
    {
        SDL_UnlockSurface(m_pSurface.get());
    }
}

void TextureResource::darken()
{
    SDL_Rect r = {};
    r.x = 0;
    r.y = 0;
    r.w = m_pSurface->w;
    r.h = m_pSurface->h;

    darken(r);
}

void TextureResource::darken(const SDL_Rect& rect)
{    
    if (SDL_MUSTLOCK(m_pSurface.get()))
    {
        SDL_LockSurface(m_pSurface.get());
    }

    for (int32 y = rect.y; y < rect.y + rect.h; ++y)
    {
        for (int32 x = rect.x; x < rect.x + rect.w; ++x)
        {            
            // Grab curr pixel
            uint32 currPixel = getPixelAt(x, y);                     
            if (currPixel == envcolors::EC_WHITE) setPixelAt(g_currentLevelColor, x, y);
            else if (currPixel == g_currentLevelColor) setPixelAt(envcolors::EC_CBLUE, x, y);
            else if (currPixel == envcolors::EC_CBLUE) setPixelAt(envcolors::EC_BLACK, x, y);    
            else if (currPixel == 16777215) setPixelAt(0, x, y);            
        }
    }

    compileTexture();

    if (SDL_MUSTLOCK(m_pSurface.get()))
    {
        SDL_UnlockSurface(m_pSurface.get());
    }
}

std::shared_ptr<TextureResource> TextureResource::getSubTexture(
    const uint32 tu,
    const uint32 tv,
    const uint32 width,
    const uint32 height) const
{
    auto pTexRes = TextureResource::createEmptyTexture(
        DEFAULT_TILE_SIZE,
        DEFAULT_TILE_SIZE);

    SDL_Rect texcoords{tu, tv, width, height};
    SDL_BlitSurface(getSurface().get(), &texcoords, pTexRes->getSurface().get(), nullptr);

    pTexRes->compileTexture();

    return pTexRes;
}

std::shared_ptr<TextureResource> TextureResource::getHorFlippedTexture() const
{
    auto pTexRes = TextureResource::createEmptyTexture(m_pSurface->w, m_pSurface->h);

    if (SDL_MUSTLOCK(pTexRes->getSurface().get())) SDL_LockSurface(pTexRes->getSurface().get());
    if (SDL_MUSTLOCK(m_pSurface.get())) SDL_LockSurface(m_pSurface.get());

    for (int y = 0; y < m_pSurface->h; ++y)
    {
        for (int x = 0; x < m_pSurface->w; ++x)
        {
            pTexRes->setPixelAt(getPixelAt(x, y), m_pSurface->w - 1 - x, y);
        }
    }

    pTexRes->compileTexture();

    if (SDL_MUSTLOCK(pTexRes->getSurface().get())) SDL_UnlockSurface(pTexRes->getSurface().get());
    if (SDL_MUSTLOCK(m_pSurface.get())) SDL_UnlockSurface(m_pSurface.get());

    return pTexRes;
}

const TextureResource::pTexture_t& TextureResource::getTexture() const
{
    return m_pTexture;
}

const TextureResource::pSurface_t& TextureResource::getSurface() const
{
    return m_pSurface;
}

TextureResource::pSurface_t& TextureResource::getSurface()
{
    return m_pSurface;
}

/* ===============
   Private Methods
   =============== */
TextureResource::TextureResource(const string& resourceName):    
    Resource(resourceName),
    m_pTexture(nullptr, SDL_DestroyTexture),
    m_pSurface(nullptr, SDL_FreeSurface)
{
}

uint32 TextureResource::getPixelAt(const uint32 x, const uint32 y) const
{    
    int bpp = m_pSurface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *) m_pSurface->pixels + y * m_pSurface->pitch + x * bpp;
    
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

void TextureResource::setPixelAt(
    const uint32 pixel, 
    const uint32 x,
    const uint32 y)
{ 
    int bpp = m_pSurface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *) m_pSurface->pixels + y * m_pSurface->pitch + x * bpp;

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