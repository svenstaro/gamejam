#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <Box2D/Box2D.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <cstdint>
#include <cstdlib>
#include <string>

#include "Asteroid.hpp"
#include "AsteroidManager.hpp"
#include "Ship.hpp"
#include "StarfieldManager.hpp"
#include "Star.hpp"

class GameApp {
public:
	// Default constructor, no work takes place here.
	GameApp(const uint32_t width, const uint32_t height, const std::string& name, const float speed);

	// Default destructor, no work takes place here.
	~GameApp();

	// Do actual initialization work in here.
	bool Init();

	// Runs the whole game (game loop).
	void Run();

    static float Random(float min, float max);
    static int Random(int min, int max);

private:
	uint32_t m_width;
	uint32_t m_height;
	std::string m_name;
	float m_speed;

	sf::RenderWindow m_RenderWin;
	sf::Clock m_Clock;
	int8_t m_pause_mode;
	uint16_t m_level;

	void GetEvents();
};

#endif
