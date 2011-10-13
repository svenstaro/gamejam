#include "LevelAttachment.hpp"

LevelAttachment::LevelAttachment(const sf::Vector2i pos, const sf::Vector2i size,
								 boost::shared_ptr<btDiscreteDynamicsWorld> dynamics_world) {
	mDynamicsWorld = dynamics_world;

	btScalar thickness(0.2);
	btScalar extrusion(10); // We don't really use this in 2D but we need it set.
	mBodyShape = boost::shared_ptr<btBoxShape> (new btBoxShape(btBoxShape(btVector3(btScalar(WIDTH / 2 * METERS_PER_PIXEL), thickness, extrusion))));

	btTransform body_transform(btQuaternion(0, 0, 1, 1), btVector3(pos.x, pos.y, 0));
	mBodyMotionState = boost::shared_ptr<btDefaultMotionState> (new btDefaultMotionState(body_transform));
	btRigidBody::btRigidBodyConstructionInfo mBodyRbInfo(0, mBodyMotionState.get(), mBodyShape.get(), btVector3(0, 0, 0));
	mBody = boost::shared_ptr<btRigidBody> (new btRigidBody(mBodyRbInfo));

	mDynamicsWorld->addRigidBody(mBody.get(), COL_ENEMY, EnemyCollidesWith);
}

LevelAttachment::~LevelAttachment() {}

void LevelAttachment::Draw(boost::shared_ptr<sf::RenderWindow> window) {
	window->Draw(mSprite);
}
