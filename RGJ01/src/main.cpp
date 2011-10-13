#include <iostream>

#include "GameApp.hpp"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "Asteroid Jam"
#define SPEED 10.f

#define DEBUG true

int main(int argc, char* argv[]) {
	GameApp Game(WIDTH, HEIGHT, TITLE, SPEED);

	// Try to init game, else fail.
	if(Game.Init())
		Game.Run();
	else
		std::cerr << "Something went wrong during init :(" << std::endl;

	return 0;
}
