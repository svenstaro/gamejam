#ifndef ENTITYMOTIONSTATE_HPP
#define ENTITYMOTIONSTATE_HPP

class Entity;

#include <memory>

#include <btBulletDynamicsCommon.h>

class EntityMotionState : public btMotionState
{

public:
    EntityMotionState(const btTransform &initialpos, Entity* entity);
    virtual ~EntityMotionState();

    virtual void getWorldTransform(btTransform &worldTrans) const override;
    virtual void setWorldTransform(const btTransform &worldTrans) override;

protected:
    Entity* m_Entity;
    btTransform m_Transform;
};

#endif // ENTITYMOTIONSTATE_HPP
