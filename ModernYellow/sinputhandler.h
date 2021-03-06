/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "mydef.h"

#include <SDL_events.h>
#include <unordered_map>

// Instance retrieval convenience macro
#define ihandler SInputHandler::instance()

// Public Key constants
const uint16 K_ESC    = 0x0001;
const uint16 K_UP     = 0x0002;
const uint16 K_DOWN   = 0x0004;
const uint16 K_LEFT   = 0x0008;
const uint16 K_RIGHT  = 0x0010;
const uint16 K_A      = 0x0020;
const uint16 K_B      = 0x0040;
const uint16 K_SELECT = 0x0080;
const uint16 K_START  = 0x0100;

class SInputHandler final
{
public:

    static SInputHandler& instance();
   
public:

    static const uint8 IH_STATUS_OK   = 0x01;
    static const uint8 IH_STATUS_QUIT = 0x02;

public:
    
    void update(const SDL_Event& event);

    void frameEnd();

    int shouldExit() const;

    int isKeyDown(const uint16 key) const;

    int isKeyTapped(const uint16 key) const;    

private:

    SInputHandler();

private:

    uint8  m_status;
    uint16 m_currState;
    uint16 m_prevState;
    std::unordered_map<SDL_Keycode, uint16> m_sdlkToCustom;
};