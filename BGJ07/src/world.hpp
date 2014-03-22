#ifndef WORLD_HPP
#define WORLD_HPP

#include <memory>
#include <vector>

#include <SDL.h>
#include <btBulletDynamicsCommon.h>

#include "entity.hpp"
#include "debugDraw.hpp"

class Game;
class World {
public:
    void physicsTickCallback(btScalar timestep);
    void init(Game* g);
    void destroy();
    void addEntity(Entity* entity);
    void event(SDL_Event& event);
    void update(float dt);
    void draw(SDL_Renderer* renderer);
    std::vector<std::unique_ptr<Entity>> entities;

    Game* game = nullptr;

    // physics stuff
    btBroadphaseInterface* m_Broadphase = nullptr;
    btDefaultCollisionConfiguration* m_CollisionConfiguration = nullptr;
    btCollisionDispatcher* m_CollisionDispatcher = nullptr;
    btSequentialImpulseConstraintSolver* m_Solver = nullptr;
    btDiscreteDynamicsWorld* m_DynamicsWorld = nullptr;
    DebugDraw* m_DebugDrawer = nullptr;
};

#endif /* end of include guard: WORLD_HPP */

