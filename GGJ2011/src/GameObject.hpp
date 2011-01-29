#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP


#include <string>

#include <boost/shared_ptr.hpp>

#include <bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionShapes/btConvex2dShape.h>

#include "Vector2D.hpp"
#include <SFML/Graphics.hpp>

class World;

class GameObject
{
public:
	virtual ~GameObject() = 0;

	virtual void Initialize(World& world) = 0;
	virtual void InitializePhysics() = 0;

	virtual void Update(float time_delta) = 0;
	virtual void Draw(sf::RenderTarget* target, sf::Shader& shader, bool editor_mode = false) const = 0;

protected:
	boost::shared_ptr<btCollisionShape> mCollisionShape;
	boost::shared_ptr<btDefaultMotionState> mMotionState;
	boost::shared_ptr<btRigidBody> mBody;
};

#endif // GAMEOBJECT_HPP
