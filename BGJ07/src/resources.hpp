#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <unordered_map>
#include <string>
#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Resources{
    public:
        void init(SDL_Renderer *renderer);
        void destroy();

        bool loadTexture(std::string name,const std::string &path);
        bool loadFont(std::string name, int fontSize, const std::string &path); 

        std::unordered_map<std::string, SDL_Texture*> m_Textures;
        std::unordered_map<std::string, TTF_Font*> m_Fonts;


    private:
        SDL_Renderer *m_Renderer;
};

#endif

