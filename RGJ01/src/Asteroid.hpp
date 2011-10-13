#ifndef ASTEROID_HPP_INCLUDED
#define ASTEROID_HPP_INCLUDED

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

class Asteroid {
public:
	Asteroid(const float size, const std::string& image_path);
	~Asteroid();
	void Spawn(const float pos_x, const float pos_y, const float speed, b2World& world);
	void Update(const float elapsed);
	const sf::Sprite& GetDrawable() const;
	const sf::Shape GetDebugDrawable() const;
	b2Body* GetBody();

private:
	float m_scale;
	float m_radius;
	std::string m_image_path;
	sf::Image m_image;
	sf::Sprite m_sprite;
	float m_rotation;
	float m_speed;

	b2Body* m_body;
};

#endif
