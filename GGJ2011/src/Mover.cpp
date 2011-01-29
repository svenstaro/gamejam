#include "Mover.hpp"

#include "Rail.hpp"
#include "World.hpp"
#include "GameApp.hpp"

Mover::Mover() {}

void Mover::Initialize(World& world) {
	InitializePhysics();
	world.AddRigidBody(mBody.get());
	mRail = NULL;
}

void Mover::InitializePhysics() {
/*	btTransform transform(btQuaternion(0, 0, 0, 1), btVector3(mPosition.x, mPosition.y, 0));
	transform.setIdentity();
	btScalar mass(1.f);
	btVector3 local_inertia(0, 0, 0);

	mCollisionShape = boost::shared_ptr<btCollisionShape>(new btBoxShape(btVector3(1, 1, 1)));

	mCollisionShape->calculateLocalInertia(mass, local_inertia);
	transform.setOrigin(btVector3(mPosition.x, mPosition.y, 0));
	transform.setRotation(btQuaternion(0, 0, 1, mRotation));

	mMotionState = boost::shared_ptr<btDefaultMotionState>(new btDefaultMotionState(transform));
	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, mMotionState.get(), mCollisionShape.get(), local_inertia);
	mBody = boost::shared_ptr<btRigidBody>(new btRigidBody(rb_info));
	mBody->setDamping(0.2f, 0.2f);
	//mBody->setActivationState(DISABLE_DEACTIVATION);
	mBody->setLinearFactor(btVector3(1,1,0));
	mBody->setAngularFactor(btVector3(0,0,0));
	//mBody->setAngularFactor(btVector3(0,0,1));
	mBody->setUserPointer(this);*/
}

void Mover::Update(float time_delta) {
	if(mRail == NULL)
		return;
	if(!(mRail->IsFinished()))
		return;

	btTransform trans;
	mRail->GetRigidBody()->getMotionState()->getWorldTransform(trans);
	btVector3 origin = trans.getOrigin();
	Vector2D p = Coordinates::WorldFloatToWorldPixel(Vector2D(origin.x(), origin.z()));
	mSprite.SetPosition(p.x, p.y);

	btMatrix3x3 rot;
	rot.setIdentity();
	rot = trans.getBasis();
	float fx,fy,fz;
	rot.getEulerZYX(fz,fy,fx);
	mSprite.SetRotation(Vector2D::rad2Deg(PI-fz));

	if(GameApp::get_mutable_instance().GetInput().IsMouseButtonDown(sf::Mouse::Left))
		mSprite.SetImage(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("magnet_push"));
	else
		mSprite.SetImage(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("magnet_pull"));
}

void Mover::Draw(sf::RenderTarget* target, sf::Shader& shader, bool editor_mode) const {
	target->Draw(mSprite);
}

void Mover::SetRail(Rail* rail) {
	mRail = rail;
}
