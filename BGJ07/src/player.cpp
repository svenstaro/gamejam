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
    int direction = 1;
    if(m_Flip == SDL_FLIP_HORIZONTAL)
        direction = -1;

    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]) {
        physicsBody->applyCentralForce(btVector3(-1000, 0, 0));
    } else if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
        physicsBody->applyCentralForce(btVector3(1000, 0, 0));
    }

    if(keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) {
        physicsBody->applyCentralForce(btVector3(0, -1000, 0));
        physicsBody->applyTorque(btVector3(0, 0, -8000 * direction));
    } else if(keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
        physicsBody->applyCentralForce(btVector3(0, 1000, 0));
        physicsBody->applyTorque(btVector3(0, 0, 8000 * direction));
    }

    if(keystate[SDL_SCANCODE_Q]) {
        m_SonarRotation -= dt * 10;
    } else if(keystate[SDL_SCANCODE_E]) {
        m_SonarRotation += dt * 10;
    }

    if(rotation <= 0.f)
        physicsBody->applyTorque(btVector3(0, 0, 1000));
    if(rotation > 0.f)
        physicsBody->applyTorque(btVector3(0, 0, -1000));

    if(rotation <= -30.f || rotation > 30.f)
        physicsBody->setAngularVelocity(btVector3(0, 0, 0));

    //auto rot = physicsBody->getCenterOfMassTransform().getRotation();
    //auto real_rot = rot.getAngle() * rot.getAxis().z();
    //physicsBody->getWorldTransform().setRotation(btQuaternion(btVector3(0, 0, 1), -real_rot));
    if(physicsBody->getLinearVelocity().x() < 0) {
        m_Flip = SDL_FLIP_HORIZONTAL;
    } else {
        m_Flip = SDL_FLIP_NONE;
    }
}

void Player::onDraw(SDL_Renderer* renderer) {
    btVector3 direction(500, 0, 0);
    btVector3 rotated_direction = direction.rotate(btVector3(0, 0, 1), m_SonarRotation);
    btVector3 absolute_direction = position + rotated_direction;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, position.x(), position.y(), absolute_direction.x(), absolute_direction.y());
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void Player::onEvent(SDL_Event& event) {
}

