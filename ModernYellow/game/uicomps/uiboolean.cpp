/* ======================
   Date: 21/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "uiboolean.h"
#include "../../font.h"
#include "../../sinputhandler.h"

extern pFont_t g_pFont;
extern uint32 g_scale;

UIBoolean::UIBoolean(const bool left):

    UIComponent(
    "misctex/boolean.png",
    left ? UIBOOL_LEFT_X * g_scale : UIBOOL_RIGHT_X * g_scale, 
    left ? UIBOOL_LEFT_Y * g_scale : UIBOOL_RIGHT_Y * g_scale),
    m_option(true)
{
}

void UIBoolean::update()
{
    if (ihandler.isKeyTapped(K_A))
    {        
        m_active = false;
        m_finished = true;
    }
    else if (ihandler.isKeyTapped(K_B))
    {
        // Always return no on B press
        m_option = false;
        m_active = false;
        m_finished = true;
    }
    if ((m_option && ihandler.isKeyTapped(K_DOWN)) ||
        (!m_option && ihandler.isKeyTapped(K_UP)))
    {
        m_option = !m_option;
    }    
}

void UIBoolean::render()
{
    UIComponent::render();
    g_pFont->renderString(
        "}", 
        m_x + UIBOOL_ARROW_X_OFFSET * g_scale, 
        m_y + UIBOOL_ARROW_Y_OFFSET * g_scale + (m_option ? 0 : UIBOOL_OPTION_DIST * g_scale));
}

bool UIBoolean::getOption() const
{
    return m_option;
}