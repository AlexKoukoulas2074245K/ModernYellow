/* ======================
   Date: 21/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "../../mydef.h"

#include <string>

using std::string;

class TextureResource;
class UIComponent
{
public:
    
    UIComponent(
        const string& compName, 
        const int32 x, 
        const int32 y);

    virtual ~UIComponent();

    virtual void update() = 0;

    virtual void render();

    virtual bool isActive() const;

    virtual bool isFinished() const;

protected:

    bool m_active;
    bool m_finished;
    int32 m_x, m_y;
    string m_name;
    std::shared_ptr<TextureResource> m_texture;

};