#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>
#include <vector>

#include <SDL.h>

#include "entity.hpp"

class World {
public:
    void addEntity(Entity* entity);
    void event(SDL_Event& event);
    void update(float dt);
    void draw(SDL_Renderer* renderer);
    std::vector<std::unique_ptr<Entity>> entities;
};

#endif /* end of include guard: WORLD_HPP */

