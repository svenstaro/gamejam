#ifndef ASTEROIDMANAGER_HPP_INCLUDED
#define ASTEROIDMANAGER_HPP_INCLUDED

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include <stdint.h>

#include "Asteroid.hpp"

class AsteroidManager {
public:
	AsteroidManager(b2World& world);
	~AsteroidManager();
	void SetMaxAsteroids(const uint8_t max_asteroids);
	const uint8_t GetMaxAsteroids() const;
	const boost::ptr_vector<Asteroid>& GetAsteroidVector() const;
	void Update(const float elapsed, b2World& world);
	void Reset();

private:
	uint8_t m_current_asteroids;
	uint8_t m_max_asteroids;

	b2World* m_world;

	boost::ptr_vector<Asteroid> m_asteroids;
};

#endif
