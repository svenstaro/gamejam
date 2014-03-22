#include "world.hpp"

#include "game.hpp"

void bulletTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    auto w = static_cast<World*>(world->getWorldUserInfo());
    w->physicsTickCallback(timeStep);
}

void World::physicsTickCallback(btScalar timestep) {
    int numManifolds = m_DynamicsWorld->getDispatcher()->getNumManifolds();
    for(int i=0; i<numManifolds; i++) {
        btPersistentManifold* contactManifold =  m_DynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
        const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

        int numContacts = contactManifold->getNumContacts();
        for (int j=0; j<numContacts; j++) {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if(pt.getLifeTime() == 1) {
                if (obA->getUserPointer() && obB->getUserPointer()) {
                    auto a = static_cast<Entity*>(obA->getUserPointer());
                    auto b = static_cast<Entity*>(obB->getUserPointer());

                    //EntityCollision c;
                    //c.distance = pt.getDistance();
                    //c.other = b;
                    //c.position = pt.getPositionWorldOnA();
                    //c.otherPosition = pt.getPositionWorldOnB();
                    //c.collisionObject = obA;
                    //c.otherCollisionObject = obB;

                    //if(!a->onCollide(b, c)) {
                    //    std::swap(c.position, c.otherPosition);
                    //    std::swap(c.collisionObject, c.otherCollisionObject);
                    //    c.other = a;
                    //    b->onCollide(a, c);
                    //}
                }
            }
        }
    }
}

void World::init() {
    m_Broadphase = new btDbvtBroadphase();
    m_CollisionConfiguration = new btDefaultCollisionConfiguration();
    m_CollisionDispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
    m_Solver = new btSequentialImpulseConstraintSolver;
    m_DynamicsWorld = new btDiscreteDynamicsWorld(m_CollisionDispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);

    m_DynamicsWorld->setWorldUserInfo(this);
    m_DynamicsWorld->setInternalTickCallback(bulletTickCallback, static_cast<void *>(this));
    m_DynamicsWorld->setGravity(btVector3(0, 9.81, 0));
}

void World::destroy() {
    delete m_DebugDrawer;
    delete m_DynamicsWorld;
    delete m_Solver;
    delete m_CollisionDispatcher;
    delete m_CollisionConfiguration;
    delete m_Broadphase;
}

void World::addEntity(Entity* entity) {
    entities.emplace_back(entity);
}

void World::event(SDL_Event& event) {
    for(auto& entity : entities)
        entity->event(event);
}

void World::update(float dt) {
    m_DynamicsWorld->stepSimulation(dt, 10);

    for(auto& entity : entities)
        entity->update(dt);
}

void World::draw(SDL_Renderer* renderer) {
    if(!m_DebugDrawer) {
        m_DebugDrawer = new DebugDraw(renderer);
        m_DebugDrawer->setDebugMode(DebugDraw::DBG_DrawWireframe | DebugDraw::DBG_DrawContactPoints | DebugDraw::DBG_DrawConstraints | DebugDraw::DBG_DrawNormals);
        m_DynamicsWorld->setDebugDrawer(m_DebugDrawer);
    }
    
    m_DynamicsWorld->debugDrawWorld();

    for(auto& entity : entities)
        entity->draw(renderer);
}

