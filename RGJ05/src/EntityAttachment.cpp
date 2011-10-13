#include "EntityAttachment.hpp"
#include "Entity.hpp"

EntityAttachment::EntityAttachment(Entity* entity,
								   const Vector2D& position_offset,
								   float rotation_offset) {
	mEntity = entity;
	mPositionOffset = position_offset;
	mRotationOffset = rotation_offset;
}

EntityAttachment::~EntityAttachment() {}

void EntityAttachment::Update(const float time_delta,
							  const Vector2D& position,
							  float rotation) {
	mEntity->Update(time_delta);

	mPosition = position + mPositionOffset;
	mRotation = rotation + mRotationOffset;

	mEntity->SetPosition(mPosition);
	mEntity->SetRotation(mRotation);
}

void EntityAttachment::Draw(sf::RenderTarget* target, sf::Shader& shader) {
	mEntity->Draw(target, shader);
}
