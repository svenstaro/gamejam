#include "player.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <btBulletDynamicsCommon.h>

#include "game.hpp"

void Player::onInit() {
    texture = m_World->game->resources.textures["submarine"];
    position = btVector3(400, 300, 0);
    physicsShape = new btBoxShape(btVector3(40, 20, 1));
    mass = 1;
}

void Player::onAdd() {
    physicsBody->forceActivationState(DISABLE_DEACTIVATION);
}

void Player::onUpdate(float dt) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_LEFT]) {
        physicsBody->applyCentralForce(btVector3(-1000, 0, 0));
    } else if(keystate[SDL_SCANCODE_RIGHT]) {
        physicsBody->applyCentralForce(btVector3(1000, 0, 0));
    }

    if(keystate[SDL_SCANCODE_UP]) {
        physicsBody->applyCentralForce(btVector3(0, -1000, 0));
    } else if(keystate[SDL_SCANCODE_DOWN]) {
        physicsBody->applyCentralForce(btVector3(0, 1000, 0));
    }
}

void Player::onEvent(SDL_Event& event) {

}

