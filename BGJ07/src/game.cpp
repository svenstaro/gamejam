#include "game.hpp"

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

    m_Window = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (m_Window == nullptr) {
        logsSDLError(std::cout, "Create window");
        return 1;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_Renderer == nullptr) {
        logsSDLError(std::cout, "Create renderer");
        return 1; 
    }

    m_previousTime = std::chrono::high_resolution_clock::now();

    return 0;
}

void Game::destroy() {
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
    Resources resources(m_Renderer);

    SDL_Event event;

    resources.loadTexture("submarine", "data/gfx/submarine1.png");
    resources.loadFont("font", 64, "/usr/share/fonts/TTF/DejaVuSans.ttf");

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
        SDL_RenderClear(m_Renderer);
        m_World.draw(m_Renderer);
        SDL_RenderPresent(m_Renderer);
    }
}
