/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "audioresource.h"
#include "../portcommon.h"
#include "../strutils.h"

#include <SDL_log.h>
#include <SDL_mixer.h>

extern string g_audioPath;

/* ==============
   Public Methods
   ============== */
AudioResource::~AudioResource()
{
    Mix_FreeMusic(m_music);
    Mix_FreeChunk(m_chunk);

}

bool AudioResource::load()
{
    if (!file_util::exists(g_audioPath + m_name))
    {
        SDL_FORCE_DISPLAY_ERROR("Resource: " + m_name + " could not be found.");
        return false;
    }
    
    if (string_utils::startsWith(m_name, "sfx"))
    {
        m_chunk = Mix_LoadWAV((g_audioPath + m_name).c_str());        
        m_music = nullptr;
    }
    else
    {
        m_music = Mix_LoadMUS((g_audioPath + m_name).c_str());        
        m_chunk = nullptr;
    }

    if (!m_music && !m_chunk)
    {
        SDL_FORCE_DISPLAY_ERROR("Resource: " + m_name + " could not be loaded.");
        return false;
    }

    return true;
}

void AudioResource::play(const bool loop)
{
    if (m_chunk)
    {
        Mix_PlayChannel(-1, m_chunk, loop ? -1 : 0);
    }
    else
    {
        Mix_PlayMusic(m_music, loop ? -1 : 1);
    }
}

bool AudioResource::isMusic() const
{
    return m_music != nullptr;
}

/* ===============
   Private Methods
   =============== */
AudioResource::AudioResource(const string& resourceName):    
    Resource(resourceName)
{
}
