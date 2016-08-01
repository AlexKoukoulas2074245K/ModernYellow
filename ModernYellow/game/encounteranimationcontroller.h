/* ======================
   Date: 28/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../mydef.h"

#include <string>
#include <list>
#include <unordered_map>
#include <functional>

class TextureResource;
class EncounterAnimationController
{
public:

    enum EncounterAnimationType
    {
        EAT_WILD_OUTDOORS = 0,
        EAT_TRAINER_OUTDOORS = 1,
        EAT_WILD_FOREST_OR_CAVE = 2,
        EAT_TRAINER_FOREST_OR_CAVE = 3,
        EAT_NONE = 2
    };

public:

    using OnCompleteCallback = std::function<void()>;

    EncounterAnimationController();

    bool isPlayingAnimation() const;

    void playEncounterAnimation(
        const EncounterAnimationType& animationType,
        OnCompleteCallback onCompleteCallback);

    void update();

    void render();

private:

    void loadAnimations();


private:

    using animation_t = std::list<std::shared_ptr<TextureResource>>;
    using animation_collection_t = std::unordered_map<EncounterAnimationType, animation_t>;
    
    bool m_playingAnimation;
    animation_collection_t m_animationCollection;
    animation_t m_currentPlayingAnimation;
    EncounterAnimationType m_currentAnimationType;
    int32 m_currentAnimationFrameDelay;
    OnCompleteCallback m_currentCallback;

};