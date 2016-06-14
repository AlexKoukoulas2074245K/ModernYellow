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
    auto pSurface = IMG_Load((g_texPath + m_name).c_str());
    
    // Convert and save surface to texture
    m_texture.reset(SDL_CreateTextureFromSurface(g_renderer, pSurface));
    
    return true;
}

const TextureResource::ptexture_t& TextureResource::getTexture() const
{
    return m_texture;
}

/* ===============
   Private Methods
   =============== */
TextureResource::TextureResource(const string& resourceName):    
    Resource(resourceName),
    m_texture(nullptr, SDL_DestroyTexture)
{
}
