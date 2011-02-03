#include "Mover.hpp"

#include "Rail.hpp"
#include "World.hpp"
#include "GameApp.hpp"

Mover::Mover() {
	mRail = NULL;
	mType = MT_MAGNET;
}

void Mover::Initialize(World& world) {
	InitializePhysics();

}

void Mover::InitializePhysics() {
}

void Mover::Update(float time_delta) {
	if(mRail == NULL && mRail->GetRigidBody() != NULL)
		return;
	if(!(mRail->IsFinished()))
		return;


	btTransform trans;
	mRail->GetRigidBody()->getMotionState()->getWorldTransform(trans);
	btVector3 origin = trans.getOrigin();
	Vector2D p = Coordinates::WorldFloatToWorldPixel(Vector2D(origin.x(), origin.y()));
	mSprite.SetPosition(p.x, p.y);

	if(mRail!=NULL && mRail->IsCurrentRail() && GameApp::get_mutable_instance().GetAppMode() == AM_PLAY && mType == MT_MAGNET) {
		btVector3 body_pos = mRail->GetRigidBody()->getCenterOfMassPosition();
		Vector2D mp = Coordinates::ScreenPixelToWorldFloat(GameApp::get_mutable_instance().GetMousePosition());
		btVector3 to_mouse = btVector3(mp.x, mp.y, 0) - body_pos;
		mRail->GetRigidBody()->applyCentralForce( to_mouse * 4 * 1000);
	}

	if(mRail!=NULL && mRail->IsCurrentRail() && GameApp::get_mutable_instance().GetAppMode() == AM_PLAY && mType != MT_MAGNET) {
		btVector3 body_pos = mRail->GetRigidBody()->getCenterOfMassPosition();
		Vector2D tp = Coordinates::ScreenPixelToWorldFloat(mRail->GetPointFromFloat(mRail->GetStartPosition()));
		btVector3 to_mouse = btVector3(tp.x, tp.y, 0) - body_pos;
		mRail->GetRigidBody()->applyCentralForce( to_mouse * 5* 1000);
	}

	mSprite.SetRotation(-Vector2D::rad2Deg(mRail->GetRotation()));

	std::string img = "";

	if(mType == MT_MAGNET) img = "magnet";
	else if(mType == MT_SPRING) img = "spring";
	else if(mType == MT_STATIC) img = "static";
	img += "_";

	if(!mRail->IsCurrentRail() && GameApp::get_mutable_instance().GetAppMode() == AM_PLAY)
		img += "off";
	else if(GameApp::get_mutable_instance().GetInput().IsMouseButtonDown(sf::Mouse::Left) && mRail != NULL && mRail->IsCurrentRail() && GameApp::get_mutable_instance().GetAppMode() == AM_PLAY)
		img += "push";
	else
		img += "pull";

	mSprite.SetImage(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage(img));
	mSprite.SetOrigin(mSprite.GetImage()->GetWidth() / 2,mSprite.GetImage()->GetHeight() / 2);
}

void Mover::Draw(sf::RenderTarget* target, sf::Shader& shader, bool editor_mode) const {
	target->Draw(mSprite);
}

void Mover::SetRail(Rail* rail) {
	mRail = rail;
}

bool Mover::OnCollide(GameObject* other) {
	return true;
}

std::string Mover::ToString() {
	return "mover";
}


void Mover::SetMoverType(MoverType type) {
	mType = type;
}

MoverType Mover::GetMoverType() const {
	return mType;
}

void Mover::ToggleMoverType() {
	if(mType == MT_MAGNET)
		mType = MT_SPRING;
	else if(mType == MT_SPRING)
		mType = MT_STATIC;
	else if(mType == MT_STATIC)
		mType = MT_MAGNET;
}
