#include "player.hpp"

#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <btBulletDynamicsCommon.h>

#include "game.hpp"
#include "box.hpp"

void Player::onInit() {
    texture = m_World->game->resources.textures["submarine"];
    position = btVector3(400, 300, 1);
    physicsShape = new btBoxShape(btVector3(30, 10, 1));
    mass = 1;
}

void Player::onDestroy() {
    delete m_SonarGhost;
}

const std::string Player::getId() const {
    return "Player";
}

void Player::onAdd() {
    physicsBody->forceActivationState(DISABLE_DEACTIVATION);
    physicsBody->setDamping(0.5, 0.5);

    m_SonarGhost = new btGhostObject();
    m_SonarGhost->setCollisionShape(new btSphereShape(10));
    m_SonarGhost->setCollisionFlags(m_SonarGhost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    m_SonarGhost->setUserPointer(static_cast<void*>(this));
    m_World->m_DynamicsWorld->addCollisionObject(m_SonarGhost, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
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

    btVector3 rotated_direction = m_SonarLength.rotate(btVector3(0, 0, 1), m_SonarRotation);
    m_SonarTarget = position + rotated_direction;

    // Check ghost collisions
    if(m_SonarGhostTraveling == false) {
        m_SonarGhost->setWorldTransform(physicsBody->getWorldTransform());
    } else {
        for(int i = 0; i <m_SonarGhost->getNumOverlappingObjects(); i++) {
            // Dynamic cast to make sure its a rigid body
            Entity* entity = static_cast<Entity*>(m_SonarGhost->getOverlappingObject(i)->getUserPointer());
        }
        auto transform = m_SonarGhost->getWorldTransform();
        transform.setOrigin(transform.getOrigin() + m_SonarSpeed.rotate(btVector3(0, 0, 1), m_SonarRotation));
        m_SonarGhost->setWorldTransform(transform);
    }

    //auto m = m_World->getBodyContacts(m_SonarGhost);
    //btVector3 total(0, 0, 0);
    //for(auto pair : m) {
    //if(pair.first == this) {
    //for(auto c : pair.second) {
    //if(c.other->getTypeName() == "CollisionShape" || c.other->getTypeName() == "Toy" || c.other->getTypeName() == "Egg") {
    //auto d = c.position - m_SonarGhost->getWorldTransform().getOrigin();
    //if(d.y() > 0 || m_ability >= WALLS) {
    //total += d;
    //}
    //}
    //}
    //}
    //}
}

void Player::onDraw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, position.x(), position.y(), m_SonarTarget.x(), m_SonarTarget.y());

    SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
    //for(int i = 0; i < (int)m_RayHits.size() - 1; i++) {
    //SDL_RenderDrawLine(renderer, m_RayHits[i].x(), m_RayHits[i].y(),
    //m_RayHits[i+1].x(), m_RayHits[i+1].y());
    //}

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void Player::onEvent(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_SPACE) {
        m_SonarGhostTraveling = true;

        //auto origin = physicsBody->getWorldTransform().getOrigin();
        //origin += 
        //m_SonarGhost->
        // Go go power ray
        /*
           m_RayHits.clear();

           m_RayHits.push_back(position);
           btVector3 lastOut = m_SonarTarget;
           for(size_t i = 0; i < 5; i++) {
           auto rayCallback = cast(m_RayHits[i], lastOut);

           if(rayCallback.hasHit()) {
           btVector3 hitPoint = rayCallback.m_hitPointWorld;
           btVector3 incoming = hitPoint - m_RayHits[i];
           btVector3 normal = rayCallback.m_hitNormalWorld;
           btVector3 reflected = -2*(incoming.dot(normal))*normal + incoming;
           btVector3 sonar = reflected.normalize();// * btVector3(500, 500, 0);
           m_RayHits.push_back(hitPoint);
           lastOut = sonar;
           }
           }
           */
    }
}

ClosestNonPlayerRayResultCallback Player::cast(btVector3 rayStart, btVector3 rayEnd) {
    ClosestNonPlayerRayResultCallback rayCallback(rayStart, rayEnd);
    m_World->m_DynamicsWorld->rayTest(rayStart, rayEnd, rayCallback);
    return rayCallback;
}

