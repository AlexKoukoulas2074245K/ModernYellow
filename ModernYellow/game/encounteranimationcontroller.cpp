/* ======================
   Date: 28/7/2016
   Author: Alex Koukoulas
   ====================== */

#include "encounteranimationcontroller.h"
#include "../resources/textureresource.h"
#include "../resources/sresmanager.h"

extern pRenderer_t g_pRenderer;

static const std::unordered_map<EncounterAnimationController::EncounterAnimationType, int32> i_encounterAnimationFrames = 
{
    {EncounterAnimationController::EAT_WILD_OUTDOORS, 15},
    {EncounterAnimationController::EAT_TRAINER_OUTDOORS, 47}
};

static const std::unordered_map<EncounterAnimationController::EncounterAnimationType, int32> i_encounterAnimationFrameDelay = 
{
    {EncounterAnimationController::EAT_WILD_OUTDOORS, 2},
    {EncounterAnimationController::EAT_TRAINER_OUTDOORS, 4}
};

static const std::unordered_map<EncounterAnimationController::EncounterAnimationType, std::string> i_encounterAnimationNames = 
{
    {EncounterAnimationController::EAT_WILD_OUTDOORS, "wildgrass"},
    {EncounterAnimationController::EAT_TRAINER_OUTDOORS, "normaltrainer"}
};

/* ==============
   Public Methods
   ============== */
EncounterAnimationController::EncounterAnimationController():
    m_playingAnimation(false)
{
    loadAnimations();
}

bool EncounterAnimationController::isPlayingAnimation() const
{
    return m_playingAnimation;
}

void EncounterAnimationController::playEncounterAnimation(
    const EncounterAnimationType& animationType,
    std::function<void()> onCompleteCallback)        
{    
    m_currentPlayingAnimation = m_animationCollection[animationType];
    m_currentAnimationFrameDelay = i_encounterAnimationFrameDelay.at(animationType);
    m_currentAnimationType = animationType;
    m_currentCallback = onCompleteCallback;
    m_playingAnimation = true;
}

void EncounterAnimationController::update()
{
    if (m_playingAnimation && !--m_currentAnimationFrameDelay)
    {
        m_currentAnimationFrameDelay = i_encounterAnimationFrameDelay.at(m_currentAnimationType);

        if (m_currentPlayingAnimation.size() > 1)
        {
            m_currentPlayingAnimation.pop_front();
        }        
        else
        {
            m_currentCallback();
        }        
    }    
}

void EncounterAnimationController::render()
{
    if (m_playingAnimation)
    {
        SDL_RenderCopy(g_pRenderer.get(), m_currentPlayingAnimation.front()->getTexture().get(), nullptr, nullptr);
    }
}

/* ===============
   Private Methods
   =============== */
void EncounterAnimationController::loadAnimations()
{
    for (auto i = static_cast<int32>(EAT_WILD_OUTDOORS); i < static_cast<int32>(EAT_NONE); ++i)
    {
        const auto animationPath = 
            "encounteranims/" 
            + i_encounterAnimationNames.at(static_cast<EncounterAnimationType>(i)) +
            + "/" 
            + i_encounterAnimationNames.at(static_cast<EncounterAnimationType>(i));

        for (auto j = 1; j < i_encounterAnimationFrames.at(static_cast<EncounterAnimationType>(i)); ++j)
        {
            m_animationCollection[static_cast<EncounterAnimationType>(i)].push_back( 
                castResToTex(resmanager.loadResource(animationPath + std::to_string(j) + ".png", RT_TEXTURE)));
        }
    }  
}