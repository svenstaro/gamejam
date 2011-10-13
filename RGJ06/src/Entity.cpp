#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include "Entity.hpp"
#include "EntityAttachment.hpp"
#include "Photon.hpp"

Entity::Entity() {}

Entity::~Entity() {}

void Entity::UpdateAllAttachments(float time_diff) {
    BOOST_FOREACH(EntityAttachment attachment, mAttachments)
        attachment.Update(time_diff, mPosition, mDirection);
}

void Entity::DrawAllAttachments(sf::RenderTarget* target) {
    BOOST_FOREACH(EntityAttachment attachment, mAttachments)
        attachment.Draw(target);
}

void Entity::Attach(boost::shared_ptr<Entity> entity,
                    Vector2D position_offset,
                    float rotation_offset,
                    RestraintSettings restraint_settings) {
    EntityAttachment* attachment = new EntityAttachment(entity,
                                                        mPosition,
                                                        mDirection,
                                                        position_offset,
                                                        rotation_offset,
                                                        restraint_settings);
    mAttachments.push_back(attachment);
}

void Entity::DetachAndDrop(Entity* entity) {
    mAttachments.erase_if(boost::bind(&EntityAttachment::GetEntity, _1) == entity);
}

Vector2D Entity::GetPosition() {
    return mPosition;
}

void Entity::SetPosition(Vector2D position) {
    mPosition = position;
}

float Entity::GetRotation() {
    return mRotation;
}

void Entity::SetRotation(float rotation) {
    mRotation = rotation;
}

float Entity::GetScale() {
    return mScale;
}

void Entity::SetScale(float scale) {
    mScale = scale;
}

float Entity::GetSpeed() {
    return mSpeed;
}

void Entity::SetSpeed(float speed) {
    mSpeed = speed;
}

Vector2D Entity::GetDirection() {
    return mDirection;
}

void Entity::SetDirection(Vector2D direction) {
    mDirection = direction;
    mDirection.Normalize();
}

const std::string& Entity::GetType() {
    return mType;
}

const std::string& Entity::GetName() {
    return mName;
}

void Entity::SetName(const std::string& name) {
    mName = name;
}

float Entity::GetLifeTime() {
    return mLifeTime.GetElapsedTime();
}

sf::Color Entity::GetPhotonColor() {
    return sf::Color::White;
}

void Entity::MarkForRemoval() {
    mMarkedForRemoval = true;
}

bool Entity::IsMarkedForRemoval() {
    return mMarkedForRemoval;
}
