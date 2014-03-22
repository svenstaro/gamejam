#include "game.hpp"
#include "player.hpp"

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

    m_Resources.init(m_Renderer);
    m_Resources.loadTexture("submarine", "data/gfx/submarine1.png");
    m_Resources.loadFont("font", 12, "/usr/share/fonts/TTF/DejaVuSans.ttf");

    m_World.init();

    return 0;
}

void Game::destroy() {

    m_World.destroy();
    m_Resources.destroy();

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

    Player* player = new Player();
    player->init(m_Resources.m_Textures["submarine"], btVector3(200, 100, 0));
    m_World.addEntity(player);


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
