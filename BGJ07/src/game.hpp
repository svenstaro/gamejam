#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>
#include <iostream>
#include <stack>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "errorHelper.hpp"
#include "renderHelper.hpp"
#include "resources.hpp"
#include "world.hpp"

class Game {
public:
    int init();
    void destroy();
    void run();
    Resources resources;

private:
    World m_World;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    std::chrono::high_resolution_clock::time_point m_previousTime;
};

#endif /* end of include guard: GAME_HPP */

