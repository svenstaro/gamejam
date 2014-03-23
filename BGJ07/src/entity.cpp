#include "entity.hpp"

#include "renderHelper.hpp"
#include "game.hpp"

void Entity::init(World* world) {
    m_World = world;
    onInit();
}

void Entity::destroy() {
    delete physicsBody;
    delete physicsShape;
    delete motionState;
    onDestroy();
}

void Entity::event(SDL_Event& event) {
    onEvent(event);
}

void Entity::update(float dt) {
    onUpdate(dt);
}

void Entity::draw(SDL_Renderer* renderer) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    RenderHelper::renderTexture(texture, renderer, position.x() - w/2, position.y() - h/2, rotation, m_Flip);
    onDraw(renderer);
}

