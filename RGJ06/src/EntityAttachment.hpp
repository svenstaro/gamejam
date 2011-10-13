#ifndef ENTITYATTACHMENT_HPP
#define ENTITYATTACHMENT_HPP

#include <boost/shared_ptr.hpp>

#include <SFML/Graphics.hpp>

#include "Vector2D.hpp"
#include "RestraintSettings.hpp"

// Forward declare Entity :(
class Entity;

class EntityAttachment {
public:
	EntityAttachment(boost::shared_ptr<Entity> entity,
					 const Vector2D& position,
					 const Vector2D& direction,
					 const Vector2D& position_offset,
					 const float rotation_offset,
					 const RestraintSettings restraint_settings = RestraintSettings());

	~EntityAttachment();

	void Update(const float time_delta, 
				const Vector2D& position, 
				const Vector2D& direction);

	void Draw(sf::RenderTarget* target);

    Entity* GetEntity();

private:
	Vector2D mPosition;
	Vector2D mPositionOffset;
	Vector2D mDirection;
	float mRotationOffset;
	RestraintSettings mRestraintSettings;

    boost::shared_ptr<Entity> mEntity;
};

#endif
