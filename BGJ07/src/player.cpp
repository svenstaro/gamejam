#include "player.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <btBulletDynamicsCommon.h>

#include "game.hpp"

void Player::onInit() {
    texture = m_World->game->resources.textures["submarine"];
    position = btVector3(400, 300, 0);
    physicsShape = new btBoxShape(btVector3(30, 10, 1));
    mass = 1;
}

std::string Player::getId(){
    return "Player";
}

void Player::onAdd() {
    physicsBody->forceActivationState(DISABLE_DEACTIVATION);
    physicsBody->setDamping(0.5, 0.5);
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
        physicsBody->applyTorque(btVector3(0, 0, -5000));
    } else if(keystate[SDL_SCANCODE_DOWN]) {
        physicsBody->applyCentralForce(btVector3(0, 1000, 0));
        physicsBody->applyTorque(btVector3(0, 0, 5000));
    }

    if(rotation <= -0.f)
        physicsBody->applyTorque(btVector3(0, 0, 1000));
    if(rotation > 0.f)
        physicsBody->applyTorque(btVector3(0, 0, -1000));

    if(rotation <= -30.f || rotation > 30.f)
        physicsBody->setAngularVelocity(btVector3(0, 0, 0));
}

void Player::onEvent(SDL_Event& event) {

}

