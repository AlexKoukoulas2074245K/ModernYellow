/* ======================
   Date: 19/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "font.h"
#include "resources/sresmanager.h"
#include "resources/dataresource.h"
#include "resources/textureresource.h"
#include "strutils.h"

#include <SDL_render.h>

extern pRenderer_t g_pRenderer;
extern uint32 g_scale;

/* ==============
   Public Methods
   ============== */
Font::Font():
    m_ready(false)
{
    if (!loadFontTexture()) return;
    if (!loadFontData()) return;

    m_ready = true;
}

void Font::renderString(
    const string& text,
    const int32 x,
    const int32 y)
{    
    SDL_Rect glyphRect;
    glyphRect.w = FONT_SIZE * g_scale/2;
    glyphRect.h = FONT_SIZE * g_scale/2;
    glyphRect.x = x;
    glyphRect.y = y;

    for (const auto& letter: text)
    {   
        if (m_glyphs.count(letter))
        {
            SDL_RenderCopy(
                g_pRenderer.get(),
                m_glyphs[letter]->getTexture().get(),
                nullptr,
                &glyphRect);
        }        

        glyphRect.x += letter == '\'' ? FONT_SIZE : FONT_SIZE * g_scale/2;
        
    }
}

bool Font::isReady() const
{
    return m_ready;
}

/* ===============
   Private Methods
   =============== */
bool Font::loadFontTexture()
{
    m_fontTexture = castResToTex(resmanager.loadResource("tilemaps/fontmap.png", RT_TEXTURE));
    if (!m_fontTexture) return false;

    return true;
}

bool Font::loadFontData()
{
    m_fontData = castResToData(resmanager.loadResource("fontmeta.dat", RT_DATA));
    if (!m_fontData) return false;

    auto content = m_fontData->getContent();

    uint32 texU = 0;
    uint32 texV = 0;

    for (size_t i = 0; i < content.size(); ++i)
    {
        const auto& line = content[i];

        auto vecComps = string_utils::split(line, ' ');

        for (const auto& letter: vecComps)
        {
            char chr = letter[0];
            if (chr != '_')
            {
                m_glyphs[chr] = m_fontTexture->getSubTexture(texU, texV, FONT_SIZE, FONT_SIZE);
            }            
            texU += FONT_SIZE;    
        }

        texU = 0;
        texV += FONT_SIZE;           
    }

    return true;
}