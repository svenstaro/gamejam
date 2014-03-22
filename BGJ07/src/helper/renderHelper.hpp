#ifndef RENDERHELPER_HPP
#define RENDERHELPER_HPP 

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class RenderHelper{
    public:
        static void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
        static void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
};
        static SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer);

#endif

