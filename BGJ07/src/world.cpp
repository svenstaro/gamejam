#include "world.hpp"

void World::addEntity(Entity* entity) {
    entities.emplace_back(entity);
}

void World::event(SDL_Event& event) {
    for(auto& entity : entities)
        entity->event(event);
}

void World::update(float dt) {
    for(auto& entity : entities)
        entity->update(dt);
}

void World::draw(SDL_Renderer* renderer) {
    for(auto& entity : entities)
        entity->draw(renderer);
}
