/* ======================
   Date: 21/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "uicomponent.h"
#include <vector>

class UIBoolean;
class UITextbox: public UIComponent
{
public:

    UITextbox(const string& text);

    void update() override;

    void render() override;

private:

    using sentences_t = std::vector<std::string>;
    using dialogue_t = std::vector<sentences_t>;

    using dialogue_iter_t = dialogue_t::iterator;
    using sentence_iter_t = sentences_t::iterator;

private:

    void parseText(const string& text);

    void parseDialogue(const string& text, dialogue_t& outDialogue);

    void feedSentence();
    
    void advanceIters();

private:
    
    static const int32 UITB_REND_X = 0;
    static const int32 UITB_REND_Y = 96;

    static const int32 UITB_TOP_SENTENCE_X = UITB_REND_X + 8;
    static const int32 UITB_TOP_SENTENCE_Y = UITB_REND_Y + 16;

    static const int32 UITB_BOT_SENTENCE_X = UITB_REND_X + 8;
    static const int32 UITB_BOT_SENTENCE_Y = UITB_REND_Y + 32;

    static const int32 UITB_PROMPT_ARROW_X = UITB_REND_X + 144;
    static const int32 UITB_PROMPT_ARROW_Y = UITB_REND_X + 128;

    static const uint32 UITB_CHAR_DELAY = 3U;
    static const uint32 UITB_ARROW_TTL = 44U;
    
    static const uint8 UITB_PARAGRAPH_DELIM = '@';
    static const uint8 UITB_SENTENCE_DELIM = '#';
    static const uint8 UITB_BOOLEAN_DELIM = ';';

private:

    dialogue_t m_mainDialogue;
    dialogue_t m_yesdialogue;
    dialogue_t m_nodialogue;

    dialogue_iter_t m_dialogueIndex;
    sentence_iter_t m_sentenceIndex;
    
    std::unique_ptr<UIBoolean> m_bool;

    string m_topSentence;
    string m_botSentence;
    
    string m_rendTopSentence;
    string m_rendBotSentence;

    int32 m_showCharDelay;
    int32 m_arrowttl;

    bool m_promptNext;
    bool m_firstSentence;
    bool m_lastSentence;
    bool m_arrowPrompt;
};
