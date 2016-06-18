/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#include "sinputhandler.h"

/* =====================
   Public Static Methods
   ===================== */
SInputHandler& SInputHandler::instance()
{
    static SInputHandler instance;
    return instance;
}

/* ==============
   Public Methods
   ============== */
void SInputHandler::update(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_QUIT: m_status = IH_STATUS_QUIT; break;
        
        case SDL_KEYDOWN: 
        {
            uint16 transKey = m_sdlkToCustom[event.key.keysym.sym];
                        
            if (transKey)
            {
                m_currState |= transKey;
            }
        } break;

        case SDL_KEYUP:
        {
            uint16 transKey = m_sdlkToCustom[event.key.keysym.sym];
            if (transKey) 
            {
                m_currState ^= transKey;
            }
        } break;
    }
}

void SInputHandler::frameEnd()
{
    m_prevState = m_currState;
}

int SInputHandler::shouldExit() const
{
    return m_status & IH_STATUS_QUIT;
}

int SInputHandler::isKeyDown(const uint16 key) const
{
    return m_currState & key;
}

int SInputHandler::isKeyTapped(const uint16 key) const
{
    return (m_currState & key) && !(m_prevState & key);
}

/* ===============
   Private Methods
   =============== */
SInputHandler::SInputHandler():
    m_currState(0U),
    m_prevState(0U),
    m_status(IH_STATUS_OK)
{
    m_sdlkToCustom[SDLK_ESCAPE]    = K_ESC;
    m_sdlkToCustom[SDLK_UP]        = K_UP;
    m_sdlkToCustom[SDLK_DOWN]      = K_DOWN;
    m_sdlkToCustom[SDLK_LEFT]      = K_LEFT;
    m_sdlkToCustom[SDLK_RIGHT]     = K_RIGHT;
    m_sdlkToCustom[SDLK_z]         = K_A;
    m_sdlkToCustom[SDLK_x]         = K_B;
    m_sdlkToCustom[SDLK_RETURN]    = K_START;
    m_sdlkToCustom[SDLK_BACKSPACE] = K_SELECT;    
}
