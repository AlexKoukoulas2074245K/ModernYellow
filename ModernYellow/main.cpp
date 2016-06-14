/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#include "vld.h"
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <queue>
#include <memory>
#include <string>
#include <fstream>

#include "portcommon.h"
#include "gstates/gsplay.h"
#include "mydef.h"
#include "sinputhandler.h"

using std::string;

// Internal Constants
const string gc_appTitle   = "ModernP";
const string gc_orgName    = "Alex-Productions";
const uint32 gc_origWidth  = 160;
const uint32 gc_origHeight = 144;

// Global Paths
string g_execPath      = "";
string g_savePath      = "";
string g_assetPath     = "../assets/";
string g_configPath    = g_assetPath + "config.ini";
string g_datPath       = g_assetPath + "dat/";
string g_texPath       = g_assetPath + "tex/";

// Global game related variables
uint32 g_fullscreen = 0;
uint32 g_width      = gc_origWidth;
uint32 g_height     = gc_origHeight;
uint32 g_vsync      = 1;
uint32 g_sfxVol     = 1;
uint32 g_musicVol   = 1;
uint32 g_scale      = 1;

// Global Renderer
SDL_Renderer* g_renderer;

/* ============================
   Internal Function Signatures
   ============================ */
static void extractConfigVars();
static void calculateGameVars();

/* ================
   Main Entry Point
   ================ */
int main(int argc, char** argv)
{       
    using std::unique_ptr;
    using std::queue;
    using std::make_unique;
    
    extractConfigVars();
    calculateGameVars();

    // Initialize SDL components that are to be used by the app
    if(SDLR(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))) 
    {
        return -1;
    }
    
    // Create an SDL context window
    auto pWindow = SDL_CreateWindow(
        gc_appTitle.c_str(), 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        g_width,
        g_height,
        g_fullscreen ? SDL_WINDOW_FULLSCREEN : 0);

    // Create the renderer based on the generated window above
    g_renderer = SDL_CreateRenderer(
        pWindow, 
        -1,
        SDL_RENDERER_ACCELERATED | (g_vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
    
    // Initialize PNG Loading
    auto imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        SDL_FORCE_DISPLAY_ERROR("SDL Image coult not be correctly initialized");
        return -1;
    }

    // Acquire paths
    g_savePath = SDL_GetPrefPath(gc_orgName.c_str(), gc_appTitle.c_str());
    g_execPath = SDL_GetBasePath();   

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
        
        // State rendering
        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
        SDL_RenderClear(g_renderer);
        gstates.front()->render();
        SDL_RenderPresent(g_renderer);

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
            secCounter = 0;
            fps = 0;
        }       
    }
    
    // Cleanup   
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}

/* =================================
   Internal Function Implementations
   ================================= */
static void extractConfigVars()
{    
    if (!file_util::exists(g_configPath))
    {
        SDL_FORCE_DISPLAY_ERROR("Missing config file. Starting program with the default configuration");        
        return;
    }

    g_fullscreen = GetPrivateProfileInt("gameconfig", "cfg_fullscreen", g_fullscreen, g_configPath.c_str());    
    g_scale      = GetPrivateProfileInt("gameconfig", "cfg_scale", g_scale, g_configPath.c_str());
    g_vsync      = GetPrivateProfileInt("gameconfig", "cfg_vsync", g_vsync, g_configPath.c_str());
    g_sfxVol     = GetPrivateProfileInt("gameconfig", "cfg_sfxvol", g_sfxVol, g_configPath.c_str());
    g_musicVol   = GetPrivateProfileInt("gameconfig", "cfg_musicvol", g_musicVol, g_configPath.c_str());
}

static void calculateGameVars()
{
    g_width  *= g_scale;
    g_height *= g_scale;
}