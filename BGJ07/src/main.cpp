#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "helper/renderHelper.hpp"
#include "helper/errorHelper.hpp"

int main(int argc, char *argv[]) {

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
    
    bool quit = false;
    SDL_Event event;
    SDL_Color color = {255, 255, 0, 0};
    
    SDL_Texture *submarine = IMG_LoadTexture(renderer, "data/gfx/submarine1.png");
    SDL_Texture *text = RenderHelper::renderText("PENIS", "/usr/share/fonts/TTF/DejaVuSans.ttf", color, 64, renderer); 

    while(!quit){
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT or event.type == SDL_KEYDOWN or event.type == SDL_MOUSEBUTTONDOWN)
                quit = true;
        }
        SDL_RenderClear(renderer);
        RenderHelper::renderTexture(text, renderer, 200, 200);
        RenderHelper::renderTexture(submarine, renderer, 0, 0);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(submarine);
    SDL_DestroyTexture(text);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

