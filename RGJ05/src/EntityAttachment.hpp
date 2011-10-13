#ifndef ENTITYATTACHMENT_HPP
#define ENTITYATTACHMENT_HPP

#include <SFML/Graphics.hpp>

#include "Vector2D.hpp"

// Forward declare Entity :(
class Entity;

class EntityAttachment {
public:
	EntityAttachment(Entity* entity,
					 const Vector2D& position_offset,
					 float rotation_offset);

	~EntityAttachment();

	void Update(const float time_delta,
				const Vector2D& position,
				float rotation);

	void Draw(sf::RenderTarget* target, sf::Shader& shader);

private:
	Vector2D mPosition;
	Vector2D mPositionOffset;
	float mRotation;
	float mRotationOffset;

	Entity* mEntity;
};

#endif
