#include "entityMotionState.hpp"

#include "entity.hpp"

EntityMotionState::EntityMotionState(const btTransform &initialpos, Entity* entity) :
    m_Entity(entity),
    m_Transform(initialpos)
{}

EntityMotionState::~EntityMotionState() {}

void EntityMotionState::getWorldTransform(btTransform &worldTrans) const {
    worldTrans = m_Transform;
}

void EntityMotionState::setWorldTransform(const btTransform &worldTrans) {
    btQuaternion rot = worldTrans.getRotation();
    m_Entity->rotation = rot.getAngle() * rot.getAxis().z();
    btVector3 pos = worldTrans.getOrigin();
    m_Entity->position = pos;
    m_Transform = worldTrans;
}
