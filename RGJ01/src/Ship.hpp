#ifndef SHIP_HPP_INCLUDED
#define SHIP_HPP_INCLUDED

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

class Ship {
public:
	Ship(const float size, const std::string& image_path);
	~Ship();
	void Spawn(const float pos_x, const float pos_y, b2World* world);
	void Update();
	const sf::Sprite& GetDrawable();
	const sf::Shape GetDebugDrawable();
	const b2ContactEdge* GetContactList();
	void SetHealth(const float health);
	float GetHealth() const;
	void SetMaxHealth(const float health);
	float GetMaxHealth() const;

private:
	float m_scale;
	float m_radius;
	std::string m_image_path;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	float m_health;
	float m_max_health;

	b2Body* m_body;

	void MoveTo(const float pos_x, const float pos_y);
};

#endif
