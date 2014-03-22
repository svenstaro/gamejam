#include "renderHelper.hpp"
#include "errorHelper.cpp"

// Texture rendering function 
void RenderHelper::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;

    SDL_RenderCopy(ren, tex, NULL, &dst);
}

// Texture rendering function 
void RenderHelper::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

