#ifndef SONAR_HPP
#define SONAR_HPP

#include "entity.hpp"

class Sonar : public Entity {
    public:
        void onInit() override;
        void onAdd() override;
        void onDraw(SDL_Renderer* renderer) override;
        const std::string getId() const override;
};

#endif 

