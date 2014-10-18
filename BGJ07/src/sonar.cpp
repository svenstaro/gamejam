#include "sonar.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <btBulletDynamicsCommon.h>

#include "game.hpp"

void Sonar::onInit() {
    //texture = m_World->game->resources.textures["submarine"];
    physicsShape = new btSphereShape(10);
    mass = 1;
}

void Sonar::onAdd() {
    physicsBody->forceActivationState(DISABLE_DEACTIVATION);
}

const std::string Sonar::getId() const {
    return "Sonar";
}

void Sonar::onDraw(SDL_Renderer* renderer) {
    SDL_Rect rect;
    rect.x = position.x();
    rect.y = position.y();
    rect.w = 10;
    rect.h = 10;
    SDL_RenderFillRect(renderer, &rect);
}
