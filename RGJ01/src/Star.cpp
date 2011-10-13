#include "Star.hpp"

Star::Star(const float radius) {
	m_radius = radius;
}

Star::~Star() {}

void Star::Spawn(const float pos_x, const float pos_y, const float speed) {
	m_shape = sf::Shape::Circle(pos_x, pos_y, m_radius, sf::Color(255, 255, 255, 50));
	m_speed = speed;
}

void Star::Update(const float elapsed) {
	m_shape.Move(-1 * m_speed * elapsed, 0);
}

const sf::Shape& Star::GetDrawable() const {
	return m_shape;
}

void Star::SetPosition(const float pos_x, const float pos_y) {
	m_shape.SetPosition(pos_x, pos_y);
}
