/* ======================
   Date: 21/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "uitextbox.h"
#include "uiboolean.h"
#include "../../sinputhandler.h"
#include "../../strutils.h"
#include "../../font.h"
#include "../../mixer.h"

#include <SDL_log.h>

extern uint32 g_scale;
extern pFont_t g_pFont;
extern pMixer_t g_pMixer;
extern string g_playerName;
extern string g_rivalName;

/* ==============
   Public Methods
   ============== */
UITextbox::UITextbox(const string& text):
    UIComponent("misctex/textbox.png", UITB_REND_X * g_scale, UITB_REND_Y * g_scale),        
    m_firstSentence(true),
    m_lastSentence(false),
    m_promptNext(false),
    m_arrowPrompt(false),  
    m_forcedArrow(false),
    m_forceFinish(false),
    m_arrowttl(0),
    m_showCharDelay(UITB_CHAR_DELAY),
    m_waitingForSfx(false),
    m_itemSfxPause(0)
{
    if (text[0] == UITB_NO_DIALOGUE)
    {
        m_finished = true;
        return;
    }        

    parseText(text);    
    feedSentence();
}

void UITextbox::update()
{    
    // Check whether the bool child component is active
    // and if so update it
    if (m_bool && m_bool->isActive())
    {
        m_bool->update();
        if (m_bool->isFinished())
        {
            // After retreiving the boolean component answer
            // move to the respective dialogue and reset the textboxe's 
            // attributes
            if (m_bool->getOption()) m_mainDialogue = m_yesdialogue;
            else m_mainDialogue = m_nodialogue;

            m_bool = nullptr;
            m_active = true;

            m_firstSentence = true;
            m_lastSentence = false;            

            m_topSentence.clear();
            m_botSentence.clear();
            m_rendTopSentence.clear();
            m_rendBotSentence.clear();

            m_dialogueIndex = m_mainDialogue.begin();
            m_sentenceIndex = m_dialogueIndex->begin();

            m_promptNext = false;
            m_arrowPrompt = false;

            feedSentence();
        }
    }

    // If child components are active the parent
    // component remains inactive
    if (!m_active) return;

    // Waiting for sfx to finish
    if (m_waitingForSfx)
    {
        if (!--m_itemSfxPause) m_waitingForSfx = false;        
        return;
    }
    
    if (m_promptNext && m_topSentence == g_playerName + " got" &&
        m_lastItemReceivedSentence != m_botSentence)
    {
        m_lastItemReceivedSentence = m_botSentence;
        g_pMixer->playAudio("sfx/gotitem.wav", true, true);
        m_waitingForSfx = true;
        m_itemSfxPause = UITB_GOT_ITEM_DELAY;
    }

    if ((
        ihandler.isKeyTapped(K_A) ||         
        ihandler.isKeyTapped(K_B))
        && m_promptNext)
    {          
        m_arrowPrompt = false;        
        m_arrowttl = 0;
        m_promptNext = false;
        if (m_lastSentence)
        {
            if (m_forcedArrow)
            {
                m_forcedArrow = false;
                g_pMixer->playAudio("sfx/click.wav", true);
            }
            m_finished = true;
            m_active = false;
        }
        else
        {   
            // Play sfx
            g_pMixer->playAudio("sfx/click.wav", true);

            if (m_firstSentence)
            {
                m_topSentence.clear();
                m_botSentence.clear();
                m_rendTopSentence.clear();
                m_rendBotSentence.clear();
            }
            else
            {
                m_rendTopSentence = m_rendBotSentence;
                m_rendBotSentence.clear();
            }

            feedSentence();            
        }
    }
    else if (m_promptNext && (!m_lastSentence || m_forcedArrow))
    {                
        if (--m_arrowttl <= 0)
        {
            m_arrowPrompt = !m_arrowPrompt;
            m_arrowttl = UITB_ARROW_TTL;
        }        
    }
    else if (!m_promptNext)
    {
        uint32 decr = 1;

        if (
            ihandler.isKeyTapped(K_A) ||
            ihandler.isKeyDown(K_A) ||
            ihandler.isKeyTapped(K_B) ||
            ihandler.isKeyDown(K_B))
        {
            decr = 2;
        }

        m_showCharDelay -= decr;

        if (m_showCharDelay <= 0 && !m_promptNext)
        {
            m_showCharDelay = UITB_CHAR_DELAY;

            if (m_rendTopSentence.size() < m_topSentence.size())
            {
                if (m_topSentence[m_rendTopSentence.size()] == ';')
                {
                    m_bool = std::make_unique<UIBoolean>(false);
                    m_active = false;
                }
                else
                {
                    m_rendTopSentence += m_topSentence[m_rendTopSentence.size()];
                }
                
            }
            else if (m_rendBotSentence.size() < m_botSentence.size())
            {
                if (m_botSentence[m_rendBotSentence.size()] == ';')
                {
                    m_bool = std::make_unique<UIBoolean>(false);
                    m_active = false;
                }
                else
                {
                    if (m_botSentence[m_rendBotSentence.size()] == UITB_FORCE_ARROW) 
                    {
                        m_forcedArrow = true;
                        m_promptNext = true;
                    }                        
                    else                    
                        m_rendBotSentence += m_botSentence[m_rendBotSentence.size()];
                }                
            }
            else
            {      
                if (m_forceFinish)
                {
                    m_finished = true;
                    m_active = false;
                }
                else
                    m_promptNext = true;
            }
        }
    }    
}

void UITextbox::render()
{    
    UIComponent::render();

    g_pFont->renderString(m_rendTopSentence, UITB_TOP_SENTENCE_X * g_scale + m_shakeXoffset * g_scale, UITB_TOP_SENTENCE_Y * g_scale + m_shakeYoffset * g_scale);          
    g_pFont->renderString(m_rendBotSentence, UITB_BOT_SENTENCE_X * g_scale + m_shakeXoffset * g_scale, UITB_BOT_SENTENCE_Y * g_scale + m_shakeYoffset * g_scale);      
    if (m_arrowPrompt) g_pFont->renderString("|", UITB_PROMPT_ARROW_X * g_scale + m_shakeXoffset * g_scale, UITB_PROMPT_ARROW_Y * g_scale + m_shakeYoffset * g_scale);
    if (m_bool) m_bool->render();
}

/* ===============
   Private Methods
   =============== */
void UITextbox::parseText(const string& text)
{
    auto pVecDialogs = string_utils::split(text, UITB_BOOLEAN_DELIM);

    parseDialogue(pVecDialogs[0], m_mainDialogue);
    
    // Text has a boolean branch
    if (pVecDialogs.size() > 1)
    {
        parseDialogue(pVecDialogs[1], m_yesdialogue);
        parseDialogue(pVecDialogs[2], m_nodialogue);

        auto& pLastParagraph = m_mainDialogue[m_mainDialogue.size() - 1];
        auto& pLastSentence = pLastParagraph[pLastParagraph.size() - 1];
        pLastSentence += ';';
    }

    m_dialogueIndex = m_mainDialogue.begin();
    m_sentenceIndex = m_dialogueIndex->begin();
}

void UITextbox::parseDialogue(const string& text, dialogue_t& outDialogue)
{
    if (text.find(UITB_FORCE_FINISH) != text.npos) m_forceFinish = true;
    
    auto vecParagraphs = string_utils::split(text, UITB_PARAGRAPH_DELIM);

    for (const auto& paragraph: vecParagraphs)
    {
        outDialogue.push_back(sentences_t());

        auto vecSentences = string_utils::split(paragraph, UITB_SENTENCE_DELIM);

        for (auto& sentence: vecSentences)
        {
            string_utils::replace(sentence, "PLAYERNAME", g_playerName);
            string_utils::replace(sentence, "RIVALNAME", g_rivalName);
            
            const auto forceFinishPos = sentence.find(UITB_FORCE_FINISH);
            if (forceFinishPos != sentence.npos) sentence.erase(forceFinishPos);

            outDialogue.back().push_back(sentence);
        }
    }    
}


void UITextbox::feedSentence()
{
    if (m_firstSentence)
    {
        m_topSentence = *m_sentenceIndex;
        advanceIters();        

        if (!m_lastSentence && !m_firstSentence)
        {
            m_botSentence = *m_sentenceIndex;
            advanceIters();                        
        }                   
    }
    else
    {        
        m_topSentence = m_botSentence;
        m_botSentence = *m_sentenceIndex;
        advanceIters();       
    }
}

void UITextbox::advanceIters()
{
    m_sentenceIndex++;
    if (m_sentenceIndex == m_dialogueIndex->end())
    {

        if (++m_dialogueIndex == m_mainDialogue.end())
        {
            m_lastSentence = true;
        }
        else
        {
            m_firstSentence = true;
            m_sentenceIndex = m_dialogueIndex->begin();
        }        
    }
    else
    {
        m_firstSentence = false;            
    }
}