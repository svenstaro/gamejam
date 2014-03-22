#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char const *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, 1920, 1080, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindowError: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 1; 
    }

    SDL_Texture *submarine = IMG_LoadTexture(renderer, "data/gfx/submarine1.png"); 
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, submarine, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyTexture(submarine);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

