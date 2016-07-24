/* ======================
   Date: 11/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "resource.h"
#include "../mydef.h"
#include <SDL_mixer.h>

class AudioResource: public Resource
{
    friend class SResManager;
public:   

    ~AudioResource();

    bool load();        

    void play(const bool loop);

    bool isMusic() const;

private:

    explicit AudioResource(const string& resourceName);

private:
    Mix_Music* m_music;
    Mix_Chunk* m_chunk;
};