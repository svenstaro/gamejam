#ifndef BOX_HPP
#define BOX_HPP

#include "entity.hpp"

class Box : public Entity {
    public:
        void onInit() override;
        void onAdd() override;
        void onDraw(SDL_Renderer* renderer) override;
};

#endif 

