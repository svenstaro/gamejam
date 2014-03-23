#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Player : public Entity{
    public:
        void onInit() override;
        void onAdd() override;
        void onEvent(SDL_Event& event) override;
        void onUpdate(float dt) override;
        void onDraw(SDL_Renderer* renderer) override;
        std::string getId();

    private:
        float m_SonarRotation;
};

#endif 

