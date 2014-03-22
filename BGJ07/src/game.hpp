#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "renderHelper.hpp"
#include "errorHelper.hpp"
#include "resources.hpp"

class Game {
public:
    int init();
    void destroy();
    void run();

private:
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
};

#endif /* end of include guard: GAME_HPP */

