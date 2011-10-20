#include "StarfieldManager.hpp"

#include "GameApp.hpp"

StarfieldManager::StarfieldManager() {
	m_current_stars = 0;
	m_max_stars = 0;
}

StarfieldManager::~StarfieldManager() {}

void StarfieldManager::SetMaxStars(const uint16_t max_stars) {
	m_max_stars = max_stars;
}

uint16_t StarfieldManager::GetMaxStars() const {
	return m_max_stars;
}

const boost::ptr_vector<Star>& StarfieldManager::GetStarVector() const {
	return m_stars;
}

void StarfieldManager::Update(float elapsed) {
	// Create new stars if needed.
	while(m_current_stars < m_max_stars) {
		float random_x = GameApp::Random(10, 800);
		float random_y = GameApp::Random(0, 600);
		float random_radius = GameApp::Random(1.f, 5.f);
		float random_speed = GameApp::Random(200, 1000);
		m_stars.push_back(new Star(random_radius));
		m_stars.back().Spawn(random_x, random_y, random_speed);
		++m_current_stars;
	}

	if(!m_stars.empty()) {
		for(boost::ptr_vector<Star>::iterator it = m_stars.begin(); it != m_stars.end(); ++it) {
			// Check for stars leaving the screen.
			sf::Shape shape = it->GetDrawable();
			float random_x = GameApp::Random(600, 800);
			float random_y = GameApp::Random(0, 600);
			if(shape.GetPosition().x < -800.f) {
				it->SetPosition(random_x, random_y);
			}

			// Update all stars.
			it->Update(elapsed);
		}
	}
}
