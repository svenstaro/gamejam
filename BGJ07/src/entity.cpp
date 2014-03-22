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
    RenderHelper::renderTexture(texture, renderer, position.x(), position.y());
    onDraw(renderer);
}

