#ifndef BOX_HPP
#define BOX_HPP

#include "entity.hpp"

class Box : public Entity {
    public:
        Box() = default;
        Box(btVector3 vec);
        void onInit() override;
        void onAdd() override;
        void onDraw(SDL_Renderer* renderer) override;
        const std::string getId() const override;
};

#endif 

