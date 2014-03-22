#include "player.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <btBulletDynamicsCommon.h>

#include "game.hpp"

void Player::onInit() {
    texture = m_World->game->resources.textures["submarine"];
    position = btVector3(200, 100, 0);
    physicsShape = new btBoxShape(btVector3(2, 2, 1));
}

void Player::onUpdate(float dt) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_LEFT])
        physicsBody->applyCentralForce(btVector3(-1, 0, 0));
    else if(keystate[SDL_SCANCODE_RIGHT])
        physicsBody->applyCentralForce(btVector3(1, 0, 0));

    if(keystate[SDL_SCANCODE_UP])
        physicsBody->applyCentralForce(btVector3(0, 1, 0));
    else if(keystate[SDL_SCANCODE_DOWN])
        physicsBody->applyCentralForce(btVector3(0, -1, 0));

    std::cout << physicsBody->getWorldTransform().getOrigin().getX() << " " <<
                 physicsBody->getWorldTransform().getOrigin().getY() << " " <<
                 position.x() << " " << position.y() << std::endl;
}

void Player::onEvent(SDL_Event& event) {

}

