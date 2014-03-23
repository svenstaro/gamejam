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
        btVector3 cast(btVector3 rayStart, btVector3 rayEnd);
        const std::string getId() const override;

    private:
        float m_SonarRotation;
        btVector3 m_SonarTarget;;
};

#endif 

