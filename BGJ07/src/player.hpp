#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Player : public Entity{
    public:
        void onInit() override;
        void onAdd() override;
        void onEvent(SDL_Event& event) override;
        void onUpdate(float dt) override;
};

#endif 

