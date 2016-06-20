/* ======================
   Date: 19/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "mydef.h"
#include <string>
#include <map>

using std::string;

class TextureResource;
class DataResource;
class Font final
{
public:

    static const uint32 FONT_SIZE = 16U;

public:

    Font();

    void renderString(
        const string& text,
        const int32 x,
        const int32 y);

    bool isReady() const;
    
private:

    bool loadFontTexture();
    bool loadFontData();
 
private:

    bool m_ready;
    std::shared_ptr<TextureResource> m_fontTexture;
    std::shared_ptr<DataResource> m_fontData;
    std::map<uint8, std::shared_ptr<TextureResource>> m_glyphs;

};