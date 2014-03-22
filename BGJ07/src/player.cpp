#include "player.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <btBulletDynamicsCommon.h>

#include "game.hpp"

void Player::onInit() {
    texture = m_World->game->resources.textures["submarine"];
    position = btVector3(200, 100, 0);
}

void Player::onUpdate(float dt) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDLK_LEFT])
        physicsBody->applyCentralForce(btVector3(-1, 0, 0));
    else if (keystate[SDLK_RIGHT])
        physicsBody->applyCentralForce(btVector3(1, 0, 0));

    if (keystate[SDLK_UP])
        physicsBody->applyCentralForce(btVector3(0, 1, 0));
    else if (keystate[SDLK_DOWN])
        physicsBody->applyCentralForce(btVector3(0, -1, 0));

}

void Player::onEvent(SDL_Event& event) {

}

