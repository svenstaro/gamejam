#include "AsteroidManager.hpp"

#include "GameApp.hpp"

AsteroidManager::AsteroidManager(b2World& world) {
	m_current_asteroids = 0;
	m_max_asteroids = 0;
}

AsteroidManager::~AsteroidManager() {}

void AsteroidManager::SetMaxAsteroids(const uint8_t max_asteroids) {
	m_max_asteroids = max_asteroids;
}

uint8_t AsteroidManager::GetMaxAsteroids() const {
	return m_max_asteroids;
}

const boost::ptr_vector<Asteroid>& AsteroidManager::GetAsteroidVector() const {
	return m_asteroids;
}

void AsteroidManager::Update(const float elapsed, b2World& world) {
	// Create new asteroids if needed.
	while(m_current_asteroids < m_max_asteroids) {
		float random_x = GameApp::Random(850, 800*2);
		float random_y = GameApp::Random(0, 600);
		float random_speed = GameApp::Random(50.f, 50.f * m_max_asteroids);
		// This somehow leads to bugs in the asteroids. :(
		float random_size = GameApp::Random(0.9f, 1.1f);
		m_asteroids.push_back(new Asteroid(random_size, "gfx/asteroid.png"));
		m_asteroids.back().Spawn(random_x, random_y, random_speed, world);
		++m_current_asteroids;
	}

	if(!m_asteroids.empty()) {
		for(boost::ptr_vector<Asteroid>::iterator it = m_asteroids.begin(); it != m_asteroids.end(); ++it) {
			// Check for asteroids leaving the screen.
			sf::Sprite sprite = it->GetDrawable();
			if(sprite.GetPosition().x < 0 - sprite.GetSize().x) {
				it = m_asteroids.erase(it);
			}

			// Update all asteroids.
			it->Update(elapsed);
		}
	}
}

void AsteroidManager::Reset() {
	BOOST_FOREACH(Asteroid& Asteroid, m_asteroids) {
		b2Body* body = Asteroid.GetBody();
		b2World* world = body->GetWorld();
		world->DestroyBody(body);
	}
	m_asteroids.clear();
	m_current_asteroids = 0;
	m_max_asteroids = 0;
}
