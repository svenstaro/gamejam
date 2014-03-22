#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <btBulletDynamicsCommon.h>
#include <SDL.h>

#include "entityMotionState.hpp"

class World;
class Entity {
    public:
        void init(World* world);
        void destroy();
        void event(SDL_Event& event);
        void update(float dt);
        void draw(SDL_Renderer* renderer);

        virtual void onInit() {}
        virtual void onDestroy() {}
        virtual void onEvent(SDL_Event& event) {}
        virtual void onUpdate(float dt) {}
        virtual void onDraw(SDL_Renderer* renderer) {}

        btVector3 position = btVector3(0, 0, 0);
        float rotation = 0;
        float mass = 0;

        btCollisionShape* physicsShape = nullptr;
        EntityMotionState* motionState = nullptr;
        btRigidBody* physicsBody = nullptr;

        SDL_Texture* texture = nullptr;

    protected:
        World* m_World = nullptr;
};


#endif 

