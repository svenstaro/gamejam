#ifndef STAR_HPP_INCLUDED
#define STAR_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include <stdint.h>

class Star {
public:
	Star(const float radius);
	~Star();
	void Spawn(const float pos_x, const float pos_y, const float speed);
	void Update(const float elapsed);
	const sf::Shape& GetDrawable() const;
	void SetPosition(const float pos_x, const float pos_y);

private:
	float m_radius;
	float m_speed;
	sf::Shape m_shape;
};

#endif
