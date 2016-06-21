/* ======================
   Date: 21/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "uicomponent.h"
#include "../../resources/sresmanager.h"
#include "../../resources/textureresource.h"

#include <SDL_render.h>

extern pRenderer_t g_pRenderer;
extern uint32 g_scale;

UIComponent::UIComponent(
    const string& compName,
    const int32 x,
    const int32 y):

    m_name(compName),
    m_active(true),
    m_finished(false),
    m_x(x),
    m_y(y)
{
    m_texture = castResToTex(resmanager.loadResource(compName, RT_TEXTURE));
}

UIComponent::~UIComponent(){}

void UIComponent::render()
{
    SDL_Rect rendArea;
    rendArea.x = m_x;
    rendArea.y = m_y;
    rendArea.w = m_texture->getSurface()->w * g_scale;
    rendArea.h = m_texture->getSurface()->h * g_scale;

    SDL_RenderCopy(g_pRenderer.get(), m_texture->getTexture().get(), nullptr, &rendArea);
}

bool UIComponent::isActive() const
{
    return m_active;
}

bool UIComponent::isFinished() const
{
    return m_finished;
}