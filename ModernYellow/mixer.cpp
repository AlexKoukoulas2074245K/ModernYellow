/* ======================
   Date: 23/7/2016
   Author: Alex Koukoulas
   ====================== */

#include "mixer.h"
#include "resources/audioresource.h"
#include "resources/sresmanager.h"

#include <SDL_log.h>

/* ===============
   Class Constants
   =============== */
const int32 Mixer::MAX_VOLUME = MIX_MAX_VOLUME;
static bool i_effectPlaying = false;

/* ==============
   Public Methods
   ============== */
Mixer::Mixer()
{
    setMusicVolume(MAX_VOLUME);
    Mix_ChannelFinished([](int channel){i_effectPlaying = false;});    
}

Mixer::~Mixer(){}

void Mixer::playAudio(const std::string& audioName, const bool overrideCurrent /* false */)
{
    auto audioResource = castResToAudio(resmanager.loadResource(audioName, RT_AUDIO));
    if (audioResource->isMusic())
    {
        m_activeMusic = audioResource;
        m_activeMusic->play(true);
    }
    else
    {
        if (!i_effectPlaying || overrideCurrent)
        {
            m_activeEffect = audioResource;
            i_effectPlaying = true;
            m_activeEffect->play(false);
        }        
    }            
}

const std::string& Mixer::getCurrMusicName() const
{
    return m_activeMusic->getName();
}

int32 Mixer::getMusicVolume() const
{
    return Mix_VolumeMusic(-1);
}

void Mixer::setMusicVolume(const int32 volume)
{
    Mix_VolumeMusic(volume);
}