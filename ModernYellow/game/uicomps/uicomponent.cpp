/* ======================
   Date: 21/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "uicomponent.h"
#include "../../resources/sresmanager.h"
#include "../../resources/textureresource.h"

extern pRenderer_t g_pRenderer;
extern uint32 g_scale;

UIComponent::UIComponent(
    const string& compName,
    const int32 x,
    const int32 y)

    : m_name(compName)
    , m_active(true)
    , m_finished(false)
    , m_x(x)
    , m_y(y)
	, m_shakeXoffset(0)
	, m_shakeYoffset(0)
{
    m_texture = castResToTex(resmanager.loadResource(compName, RT_TEXTURE));
}

UIComponent::~UIComponent(){}

void UIComponent::render()
{    
    SDLRender(
        g_pRenderer, 
        m_texture->getTexture().get(),
        m_x + m_shakeXoffset * g_scale, 
        m_y + m_shakeYoffset * g_scale,
        m_texture->getSurface()->w * g_scale, 
        m_texture->getSurface()->h * g_scale);    
}

bool UIComponent::isActive() const
{
    return m_active;
}

bool UIComponent::isFinished() const
{
    return m_finished;
}

void UIComponent::setShakeOffset(const int32 shakeXoffset, const int32 shakeYoffset)
{
	m_shakeXoffset = shakeXoffset;
	m_shakeYoffset = shakeYoffset;
}
