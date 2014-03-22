#include <iostream>
#include "resources.hpp"
#include "errorHelper.hpp"

void Resources::init(SDL_Renderer* renderer) {
    m_Renderer = renderer; 
}

void Resources::destroy() {
    for (auto texture : m_Textures ) { 
        std::cerr << "Destroying Texture " << texture.first << std::endl;
        SDL_DestroyTexture(texture.second);
        texture.second = nullptr;
    }
    for (auto font : m_Fonts) {
        std::cerr << "Closing Font " << font.first << std::endl;
        //TTF_CloseFont(font.second);
        font.second = nullptr;
    }
}

bool Resources::loadTexture(std::string name, const std::string &path) {
    SDL_Texture* picture = IMG_LoadTexture(m_Renderer, path.c_str());
    if (picture == nullptr) {
        logsSDLError(std::cout, "Texture loading"); 
        return false;
    }
    m_Textures[name] = picture; 
    return true;
}

bool Resources::loadFont(std::string name, int fontSize, const std::string &path) {
    TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize); 
    if (font == nullptr) {
        logsSDLError(std::cout, "Font loading"); 
        return false;
    }
    m_Fonts[name] = font;
    return true;
}

