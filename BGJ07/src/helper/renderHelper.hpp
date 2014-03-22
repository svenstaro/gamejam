#ifndef RENDERHELPER_HPP
#define RENDERHELPER_HPP 

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class RenderHelper{
    public:
        static void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
        static void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
};

#endif

