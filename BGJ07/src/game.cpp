#include "game.hpp"

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
   bool quit = false;
   Resources resources(m_Renderer);
   
   SDL_Event event;
   SDL_Color color = {255, 255, 0, 0};

   resources.loadTexture("submarine", "data/gfx/submarine1.png");
   resources.loadFont("font", 64, "/usr/share/fonts/TTF/DejaVuSans.ttf");
   SDL_Texture* text = RenderHelper::renderText("PENIS", resources.m_Fonts["font"], color, m_Renderer); 

   while(!quit) {
       while(SDL_PollEvent(&event)){
           if (event.type == SDL_QUIT or event.type == SDL_KEYDOWN or event.type == SDL_MOUSEBUTTONDOWN)
               quit = true;
       }
       SDL_RenderClear(m_Renderer);
       RenderHelper::renderTexture(text, m_Renderer, 200, 200);
       RenderHelper::renderTexture(resources.m_Textures["submarine"], m_Renderer, 0, 0);
       SDL_RenderPresent(m_Renderer);
   }

   SDL_DestroyTexture(text);
}
