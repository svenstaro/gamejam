#include "EntityAttachment.hpp"
#include "Entity.hpp"

EntityAttachment::EntityAttachment(boost::shared_ptr<Entity> entity,
								   const Vector2D& position,
								   const Vector2D& direction,
								   const Vector2D& position_offset,
								   const float rotation_offset,
								   const RestraintSettings restraint_settings) {
	mEntity = entity;
	mPosition = position + position_offset;
	mPositionOffset = position_offset;
	mDirection = direction;
	mDirection.Rotate(rotation_offset);
	mRotationOffset = rotation_offset;
	mRestraintSettings = restraint_settings;
}

EntityAttachment::~EntityAttachment() {}

void EntityAttachment::Update(const float time_delta, 
							  const Vector2D& position, 
							  const Vector2D& direction) {
	mEntity->Update(time_delta);

	if (!mRestraintSettings.RestrainX)
		 mPosition.x = position.x + mPositionOffset.x;
	if (!mRestraintSettings.RestrainY)
		 mPosition.y = position.y + mPositionOffset.y;
	mEntity->SetPosition(mPosition);

	if (!mRestraintSettings.RestrainRotation) {
		mDirection = direction;
		mDirection.Rotate(mRotationOffset);
		mEntity->SetDirection(mDirection);
	}
}

void EntityAttachment::Draw(sf::RenderTarget* target) {
	mEntity->Draw(target);
}


Entity* EntityAttachment::GetEntity() {
    return mEntity.get();
}
