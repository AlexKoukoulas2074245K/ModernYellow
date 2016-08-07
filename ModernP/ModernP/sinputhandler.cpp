/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#include "sinputhandler.h"

/* ============================
   Internal Function Signatures
   ============================ */
static uint16 translateSDLKey(const SDL_Keycode sdlKey);


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
            uint16 transKey = translateSDLKey(event.key.keysym.sym);
                        
            if (transKey)
            {
                m_currState |= transKey;
            }
        } break;

        case SDL_KEYUP:
        {
            uint16 transKey = translateSDLKey(event.key.keysym.sym);
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
{}


/* =================================
   Internal Function Implementations
   ================================= */
static uint16 translateSDLKey(const SDL_Keycode sdlKey)
{
    switch (sdlKey)
    {
    case SDLK_ESCAPE: return K_ESC;
    case SDLK_UP:     return K_UP;
    case SDLK_DOWN:   return K_DOWN;
    case SDLK_LEFT:   return K_LEFT;
    case SDLK_RIGHT:  return K_RIGHT;
    case SDLK_SPACE:  return K_SPACE;
    }

    return 0;
}