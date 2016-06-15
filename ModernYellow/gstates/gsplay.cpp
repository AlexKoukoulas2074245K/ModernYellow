/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#include "gsplay.h"
#include "../mydef.h"
#include "../resources/sresmanager.h"
#include "../resources/textureresource.h"
#include "../sinputhandler.h"
#include "../portcommon.h"
#include <SDL_render.h>
#include <SDL_log.h>

extern SDL_Renderer* g_renderer;
extern string g_texPath;
std::shared_ptr<TextureResource> s_res;
std::shared_ptr<TextureResource> s_res2;

/* ==============
   Public Methods
   ============== */
GSPlay::GSPlay():
    GState()
{    
    s_res = castResToTex(resmanager.loadResource(std::string("tilemaps/asd.png"), RT_TEXTURE));        
}

void GSPlay::update()
{
    if (ihandler.isKeyTapped(K_SPACE))
    {        
        s_res->darken();
    }
}

void GSPlay::render()
{        
    SDL_Rect r; r.x = 0; r.y = 0; r.w = 32; r.h = 32;    
    SDL_RenderCopy(g_renderer, s_res->getTexture().get(), nullptr, nullptr);    
}

GSPlay::~GSPlay()
{    
}
