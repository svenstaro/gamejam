#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <btBulletDynamicsCommon.h>
#include <SDL.h>

#include "entityMotionState.hpp"

class Entity {
    public:
        void event(SDL_Event& event);
        void update(float dt);
        void draw(SDL_Renderer* renderer);

        btVector3 position;
        float rotation;
        btCollisionShape* shape = nullptr;
        EntityMotionState* motionState = nullptr;
        btRigidBody* physicsBody = nullptr;

        SDL_Texture* texture = nullptr;
};


#endif 

