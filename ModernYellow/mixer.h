/* ======================
   Date: 23/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "mydef.h"
#include <string>
#include <unordered_map>

class AudioResource;
class Mixer final
{
public:

    explicit Mixer();

    ~Mixer();

    void tick();

    void playAudio(
        const std::string& audioName,
        const bool overrideCurrent = false,
        const bool silenceRest = false);

    const std::string& getCurrMusicName() const;

    int32 getMusicVolume() const;

    void setMusicVolume(const int32 volume);

public:

    static const int32 MAX_VOLUME;

private:

    std::shared_ptr<AudioResource> m_activeMusic;
    std::shared_ptr<AudioResource> m_activeEffect;
    bool m_effectPlaying;
    bool m_silenceRest;

};