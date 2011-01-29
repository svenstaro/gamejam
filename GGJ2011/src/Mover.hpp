#ifndef MOVER_HPP
#define MOVER_HPP

#include "GameObject.hpp"
#include "World.hpp"

class Mover : public GameObject {
public:
	Mover();

	virtual void Initialize(World& world);
	virtual void InitializePhysics();

	virtual void Update(float time_delta);
	virtual void Draw(sf::RenderTarget* target, sf::Shader& shader, bool editor_mode = false) const;
};

#endif // MOVER_HPP
