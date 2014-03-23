#include "box.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <btBulletDynamicsCommon.h>

#include "game.hpp"

void Box::onInit() {
    //texture = m_World->game->resources.textures["submarine"];
    position = btVector3(400, 200, 0);
    physicsShape = new btBoxShape(btVector3(20, 20, 1));
    mass = 1;
}

void Box::onAdd() {
    physicsBody->forceActivationState(DISABLE_DEACTIVATION);
}

void Box::onDraw(SDL_Renderer* renderer) {
    SDL_Rect rect;
    rect.x = position.x();
    rect.y = position.y();
    rect.w = 20;
    rect.h = 20;
    SDL_RenderFillRect(renderer, &rect);
}
