#include <iostream>
#include <string>

#include "helper/renderHelper.hpp"
#include "helper/errorHelper.hpp"

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logsSDLError(std::cout, "SDLInit");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        logsSDLError(std::cout, "Create window");
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logsSDLError(std::cout, "Create renderer");
        return 1; 
    }
    
    SDL_Texture *submarine = IMG_LoadTexture(renderer, "data/gfx/submarine1.png");

    bool quit = false;
    SDL_Event event;

    while(!quit){
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT or event.type == SDL_KEYDOWN or event.type == SDL_MOUSEBUTTONDOWN)
                quit = true;
        }
        SDL_RenderClear(renderer);
        RenderHelper::renderTexture(submarine, renderer, 0, 0);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(submarine);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

