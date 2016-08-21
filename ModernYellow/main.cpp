/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#define SHOW_LEAKS
#if (defined(_DEBUG) || defined(DEBUG)) || defined(SHOW_LEAKS)
#include "vld.h"
#endif

#include "portcommon.h"
#include "gstates/gsplay.h"
#include "mydef.h"
#include "sinputhandler.h"
#include "resources/sresmanager.h"
#include "resources/dataresource.h"
#include "mixer.h"
#include "strutils.h"
#include "font.h"
#include "gameinfo.h"
#include "jsonutil/json.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <fstream>
#include <queue>
#include <iostream>

using std::string;

// Internal Constants
const string gc_appTitle   = "ModernP";
const string gc_orgName    = "Alex-Productions";
const uint32 gc_origWidth  = 160;
const uint32 gc_origHeight = 144;

// Global Paths
string g_execPath   = "";
string g_savePath   = "";
string g_assetPath  = "../assets/";
string g_datPath    = g_assetPath + "dat/";
string g_texPath    = g_assetPath + "tex/";
string g_audioPath  = g_assetPath + "audio/";

// Global game related variables
uint32 g_fullscreen            = 0;
uint32 g_width                 = gc_origWidth;
uint32 g_height                = gc_origHeight;
uint32 g_vsync                 = 1;
uint32 g_sfxVol                = 1;
uint32 g_musicVol              = 1;
uint32 g_scale                 = 1;
uint32 g_tileSize              = 16;
uint32 g_overworldTilemapColor = envcolors::EC_PALET;
uint32 g_currentLevelColor     = envcolors::EC_PALET;
string g_playerName            = "Ash";
string g_rivalName             = "Garry";

// Global Systems
pRenderer_t g_pRenderer;
pFont_t g_pFont;
pGameInfo_t g_pGameInfo;
pMixer_t g_pMixer;

/* ============================
   Internal Function Signatures
   ============================ */
static void extractConfigVars();
static void adjustGameVars();

/* ================
   Main Entry Point
   ================ */
int main(int argc, char** argv)
{               
    using std::unique_ptr;
    using std::queue;
    using std::make_unique;
    
    extractConfigVars();
    adjustGameVars();

    // Initialize SDL components that are to be used by the app
    if (SDLR(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))) 
    {
        return -1;
    } 
    

    // Initialize SDL mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        SDL_FORCE_DISPLAY_ERROR("SDL mixer could not be correctly initialized");       
        return -1;
    }
    
	const auto mixerFlags = MIX_INIT_MP3;
	const auto mixerInitRes = Mix_Init(mixerFlags);
	if ((mixerInitRes & mixerFlags) != mixerFlags)
	{
		SDL_FORCE_DISPLAY_ERROR("SDL Mixer could not initialize MP3 mixing");
		return -1;
	}

    // Create an SDL context window
    auto pWindow = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>(SDL_CreateWindow(
        gc_appTitle.c_str(), 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        g_width,
        g_height,
        g_fullscreen ? SDL_WINDOW_FULLSCREEN : 0),
        SDL_DestroyWindow);

    
    // Create the renderer based on the generated window above
    g_pRenderer.reset(SDL_CreateRenderer(
        pWindow.get(), 
        -1,
        SDL_RENDERER_ACCELERATED | (g_vsync ? SDL_RENDERER_PRESENTVSYNC : 0)),
        SDL_DestroyRenderer);
    
    // Initialize PNG Loading
    auto imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        SDL_FORCE_DISPLAY_ERROR("SDL Image coult not be correctly initialized");
        return -1;
    }

    // Initialize Global Mixer System
    g_pMixer = std::make_shared<Mixer>();

    // Initialize Font
    g_pFont = std::make_shared<Font>();

    // Acquire paths
    g_savePath = SDL_GetPrefPath(gc_orgName.c_str(), gc_appTitle.c_str());
    g_execPath = SDL_GetBasePath();   

    // Create and load Game Info
    g_pGameInfo = std::make_shared<GameInfo>();
    if (!g_pGameInfo->isReady())
    {
        SDL_FORCE_DISPLAY_ERROR("GameInfo could not be correctly initialized");
        return -1;
    }

    // Create game states
    queue<unique_ptr<GState>> gstates;
    gstates.push(make_unique<GSPlay>());
        
    // Game Loop vars
    auto running = true;            
    uint32 fps = 0;
    uint32 secCounter = 0;
    SDL_Event event;

    while (running)
    {
        auto start = SDL_GetTicks();
        
        // Event Dispatching
        while (SDL_PollEvent(&event))
        {
            ihandler.update(event);

            if (ihandler.isKeyDown(K_ESC) || 
                ihandler.shouldExit())
            {
                running = false;
            }
        }                

        // State update       
        gstates.front()->update();        
        
        // Mixer update
        g_pMixer->tick();

        // State rendering
        SDL_SetRenderDrawColor(g_pRenderer.get(), 
                               RED(envcolors::EC_BLACK),
                               GREEN(envcolors::EC_BLACK), 
                               BLUE(envcolors::EC_BLACK),
                               ALPHA(envcolors::EC_BLACK));

        SDL_RenderClear(g_pRenderer.get());
        gstates.front()->render();          
        SDL_RenderPresent(g_pRenderer.get());
        
        // Input frame end
        ihandler.frameEnd();

        // State finish check
        if (gstates.front()->isFinished())
        {
            gstates.pop();
            running = !gstates.empty();
        }

        // Meta-frame profiling
        fps++;
        secCounter += SDL_GetTicks() - start;
        if (secCounter > 1000)
        {
            // Extract per sec profiling info
            SDL_SetWindowTitle(pWindow.get(), std::to_string(fps).c_str());
            secCounter = 0;
            fps = 0;
        }       
    }
    
    // Cleanup   
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}

/* =================================
   Internal Function Implementations
   ================================= */
static void extractConfigVars()
{    
    if (!file_util::exists(g_datPath + "config.ini"))
    {
        SDL_FORCE_DISPLAY_ERROR("Missing config file. Starting program with the default configuration");        
        return;
    }

    auto configData = castResToData(resmanager.loadResource("config.ini", RT_DATA));
    auto content    = configData->getContent();

    for (const auto& line: content)
    {
        const string strVal = string_utils::split(line, '=')[1];

        if (string_utils::startsWith(line, "cfg_fullscreen"))    g_fullscreen = std::atoi(strVal.c_str());
        else if (string_utils::startsWith(line, "cfg_scale"))    g_scale = std::atoi(strVal.c_str());
        else if (string_utils::startsWith(line, "cfg_vsync"))    g_vsync = std::atoi(strVal.c_str());
        else if (string_utils::startsWith(line, "cfg_sfxvol"))   g_sfxVol = std::atoi(strVal.c_str());
        else if (string_utils::startsWith(line, "cfg_musicvol")) g_musicVol = std::atoi(strVal.c_str());
    }
}

static void adjustGameVars()
{
    if (g_scale > 8) g_scale = 8;
    if (g_fullscreen) g_scale = 8;
    g_width  *= g_scale;
    g_height *= g_scale;
    g_tileSize *= g_scale;
}