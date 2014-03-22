#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <btBulletDynamicsCommon.h>

class Entity {
    public:
        void event(SDL_Event& event);
        void update(float dt);
        void draw(SDL_Renderer* renderer);

        btVector3 position;
        btCollisionShape* shape;
        SDL_Texture* texture;
};


#endif 

