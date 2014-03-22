#include "entity.hpp"

#include "renderHelper.hpp"

void Entity::event(SDL_Event& event) {
}

void Entity::update(float dt) {
}

void Entity::draw(SDL_Renderer* renderer) {
    RenderHelper::renderTexture(m_Texture, renderer, position.x, position.y);
}
