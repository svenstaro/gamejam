#include <Config.hpp>

#include <iostream>

#include "GameState.hpp"
#include <Core/Root.hpp>
#include <Scene/Game.hpp>

int main(int argc, char** argv) {
    dt::Game game;
    game.Run(new GameState, argc, argv);
    return 0;
}
