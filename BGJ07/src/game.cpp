#include "game.hpp"
#include "player.hpp"
#include "map.hpp"
#include <map>

#include <SDL_keycode.h>

int Game::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logsSDLError(std::cout, "SDLInit");
        return 1;
    }

    if (TTF_Init() != 0){
        logsTTFError(std::cout, "TTFInit");
        return 1;
    }

    if (IMG_Init(0) != 0){
        logsIMGError(std::cout, "IMGInit");
        return 1;
    }

    m_MainViewport.x = 0;
    m_MainViewport.y = 0;
    m_MainViewport.w = 800;
    m_MainViewport.h = 600;

    m_Window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_MainViewport.w, m_MainViewport.h, SDL_WINDOW_SHOWN);
    if (m_Window == nullptr) {
        logsSDLError(std::cout, "Create window");
        return 1;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_Renderer == nullptr) {
        logsSDLError(std::cout, "Create renderer");
        return 1; 
    }

    m_DrawTexture = SDL_CreateTexture(m_Renderer, SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET, 1600, 1200);
    SDL_SetRenderTarget(m_Renderer, m_DrawTexture);

    //SDL_Rect viewport;
    //viewport.x = 400;
    //viewport.y = 300;
    //viewport.w = 400;
    //viewport.h = 300;
    //SDL_RenderSetViewport(m_Renderer, &viewport);
    //SDL_RenderSetScale(m_Renderer, 2.f, 2.f);
    //SDL_RenderSetLogicalSize(m_Renderer, 300, 200);

    m_previousTime = std::chrono::high_resolution_clock::now();

    resources.init(m_Renderer);
    resources.loadTexture("submarine", "data/gfx/submarine1.png");
    resources.loadFont("font", 12, "/usr/share/fonts/TTF/DejaVuSans.ttf");

    m_World.init(this);

    return 0;
}

void Game::destroy() {
    SDL_DestroyTexture(m_DrawTexture);

    m_World.destroy();
    resources.destroy();

    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::run() {
    std::chrono::duration<float> difference = std::chrono::high_resolution_clock::now() - m_previousTime;
    float dt = difference.count();
    bool quit = false;
    SDL_Event event;
   
    Map* arena = new Map();
    btBvhTriangleMeshShape* meshShape = arena->createMap();

    while(!quit) {
        // Handle input
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT or (event.type == SDL_KEYDOWN and event.key.keysym.sym  == SDLK_ESCAPE))
                quit = true;
              
            m_World.event(event);
        }

        // Update
        m_World.update(dt);

        // Draw
        int x, y;
        SDL_GetMouseState(&x, &y);
        m_CameraViewport.x = x - m_MainViewport.x/2;
        m_CameraViewport.y = y - m_MainViewport.y/2;
        m_CameraViewport.w = m_MainViewport.w;
        m_CameraViewport.h = m_MainViewport.h;

        SDL_RenderClear(m_Renderer);
        m_World.draw(m_Renderer);
        SDL_SetRenderTarget(m_Renderer, 0);
        SDL_RenderCopy(m_Renderer, m_DrawTexture, &m_CameraViewport, NULL); 
        SDL_SetRenderTarget(m_Renderer, m_DrawTexture);
        SDL_RenderPresent(m_Renderer);
    }
}
