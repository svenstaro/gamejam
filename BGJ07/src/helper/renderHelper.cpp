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

SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer){

    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr){
        logsSDLError(std::cout, "TTF_OpenFont");
        return nullptr;
    }

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
    TTF_CloseFont(font);
    return texture;
}


