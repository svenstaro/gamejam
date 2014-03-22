#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Player : public Entity{
    public:
        void init(SDL_Texture* tex, btVector3 pos);
        void event(SDL_Event& event);
        void update(float dt);
};

#endif 

