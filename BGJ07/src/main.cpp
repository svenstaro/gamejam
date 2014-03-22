#include "game.hpp"

int main(int argc, char *argv[]) {
    Game game;
    if(game.init() != 0)
        return 1;
    game.run();
    game.destroy();

    return 0;
}

