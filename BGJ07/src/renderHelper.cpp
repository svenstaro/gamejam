#include "renderHelper.hpp"
#include "errorHelper.hpp"

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
void RenderHelper::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void RenderHelper::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, double angle, SDL_RendererFlip flip) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopyEx(ren, tex, NULL, &dst, angle, NULL, flip);
}

SDL_Texture* RenderHelper::renderText(const std::string &message, TTF_Font* font, SDL_Color color, SDL_Renderer *renderer){

    SDL_Surface *surface = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surface == nullptr){
        logsSDLError(std::cout, "TTF_RenderText");
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr){
        logsSDLError(std::cout, "CreateTexture");
    }

    SDL_FreeSurface(surface);
    return texture;
}

