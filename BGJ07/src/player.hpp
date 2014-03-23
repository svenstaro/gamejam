#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <vector>

struct ClosestNonPlayerRayResultCallback : public btCollisionWorld::ClosestRayResultCallback {
    explicit ClosestNonPlayerRayResultCallback(const btVector3 &rayFromWorld, const btVector3& rayToWorld) :
        ClosestRayResultCallback(rayFromWorld, rayToWorld)
    {}
    virtual ~ClosestNonPlayerRayResultCallback() {}

    virtual bool needsCollision(btBroadphaseProxy* proxy0) const {
        const btCollisionObject* obj = static_cast<const btCollisionObject*>(proxy0->m_clientObject);
        const Entity* ent = static_cast<const Entity*>(obj->getUserPointer());
        if(ent != 0 && ent->getId() != "Player")
            return true;
        else
            return false;
    }
};


class Player : public Entity{
    public:
        void onInit() override;
        void onDestroy() override;
        void onAdd() override;
        void onEvent(SDL_Event& event) override;
        void onUpdate(float dt) override;
        void onDraw(SDL_Renderer* renderer) override;
        ClosestNonPlayerRayResultCallback cast(btVector3 rayStart, btVector3 rayEnd);
        const std::string getId() const override;

    private:
        float m_SonarRotation;
        btVector3 m_SonarLength = btVector3(500, 0, 0);
        btVector3 m_SonarSpeed = btVector3(2, 0, 0);
        btVector3 m_SonarTarget;
        btGhostObject* m_SonarGhost = nullptr;
        bool m_SonarGhostTraveling = false;
        //std::vector<btVector3> m_RayHits;
};

#endif 

