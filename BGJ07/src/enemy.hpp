#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"

class Enemy : public Entity{
    public:
        void init(SDL_Texture* tex, btVector3 pos);
        void event(SDL_Event& event);
        void update(float dt);
        std::string getId();
};

#endif 

