#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

// General Error message
void logsSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}
// Texture rendering function 
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
    SDL_Texture *texture = nullptr;
    SDL_Surface *loadedImage = IMG_Load(file.c_str());

    if (loadedImage != nullptr) {
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        if (texture == nullptr){
            logsSDLError(std::cout, "CreateTextureFromeSurface");
        }
    }
    else
        logsSDLError(std::cout, "LoadBMP");
    return texture;
}

int main(int argc, char const *argv[]) {

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

    SDL_Texture *submarine = loadTexture("data/gfx/submarine1.png", renderer);
//    renderTexture(submarine, renderer, 0, 0);

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

