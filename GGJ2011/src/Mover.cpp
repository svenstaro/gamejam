#include "Mover.hpp"

#include "Rail.hpp"
#include "World.hpp"
#include "GameApp.hpp"

Mover::Mover() {
	mRail = NULL;
}

void Mover::Initialize(World& world) {
	InitializePhysics();

}

void Mover::InitializePhysics() {
}

void Mover::Update(float time_delta) {
	if(mRail == NULL && mRail->GetRigidBody() != NULL )
		return;
	if(!(mRail->IsFinished()))
		return;


	btTransform trans;
	mRail->GetRigidBody()->getMotionState()->getWorldTransform(trans);
	btVector3 origin = trans.getOrigin();
	Vector2D p = Coordinates::WorldFloatToWorldPixel(Vector2D(origin.x(), origin.y()));
	mSprite.SetPosition(p.x, p.y);

	if(mRail!=NULL && mRail->IsCurrentRail()) {
		btVector3 body_pos = mRail->GetRigidBody()->getCenterOfMassPosition();
		Vector2D mp = Coordinates::ScreenPixelToWorldFloat(GameApp::get_mutable_instance().GetMousePosition());
		btVector3 to_mouse = btVector3(mp.x, mp.y, 0) - body_pos;
		mRail->GetRigidBody()->applyCentralForce( to_mouse * 4);
	}

	/*btMatrix3x3 rot;
	rot.setIdentity();
	rot = trans.getBasis();
	float fx,fy,fz;
	rot.getEulerZYX(fz,fy,fx);
	std::cout << fz << std::endl;*/
	mSprite.SetRotation(-Vector2D::rad2Deg(mRail->GetRotation()));

	if(GameApp::get_mutable_instance().GetInput().IsMouseButtonDown(sf::Mouse::Left) && mRail != NULL && mRail->IsCurrentRail())
		mSprite.SetImage(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("magnet_push"));
	else
		mSprite.SetImage(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("magnet_pull"));
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
