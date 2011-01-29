#include "Entity.hpp"
#include "GameApp.hpp"
#include "World.hpp"

Entity::Entity() {
	mAlpha = 1.f;
	mRotation = 0.f;
	mRotationSpeed = 0.f;
	mScale = 1.f;
	mUID = "";
}
Entity::~Entity() {}

void Entity::Initialize(World& world) {
	SetUsePhysics(world, false);
}

void Entity::Initialize(World& world, const std::string& imagefile, int layer, bool use_physics) {
	mImageFile = imagefile;
	mLayer = layer;
	mUsePhysics = false;
	SetUsePhysics(world, use_physics);

	if (mUID=="")
		GenerateUID();
}

void Entity::Initialize(World& world, boost::property_tree::ptree* pt, std::string uid) {
	float x = pt->get<float>("entities."+uid+".x");
	float y = pt->get<float>("entities."+uid+".y");
	std::string img = pt->get<std::string>("entities."+uid+".img");
	float rot = pt->get<float>("entities."+uid+".rot");
	float a = pt->get<float>("entities."+uid+".a");
	float s = pt->get<float>("entities."+uid+".s");
	int l = pt->get<int>("entities."+uid+".l");
	int ll = pt->get<int>("entities."+uid+".ll");
	bool phy = pt->get<bool>("entities."+uid+".phy");
	if(uid=="player") {
		rot = 0;
		x = 3;
		y = -2;
	}
	SetScale(s);
	SetLocalLayer(ll);
	SetPosition(Vector2D(x,y));
	SetRotation(rot);
	SetAlpha(a);
	SetUID(uid);
	Initialize(world, img, l, phy);
}

void Entity::InitializePhysics() {
	btTransform transform(btQuaternion(0, 0, 0, 1), btVector3(mPosition.x, mPosition.y, 0));
	transform.setIdentity();
	//btTransform transform;
	transform.setIdentity();
	btScalar mass(1.f);
	btVector3 local_inertia(0, 0, 0);

	if(mUID == "player") {
		mCollisionShape = boost::shared_ptr<btCollisionShape>(new btCapsuleShape(1*mScale, 0.3*mScale));
		mass = 1.f;
	} else {
		mCollisionShape = boost::shared_ptr<btCollisionShape>(new btBoxShape(btVector3(1*mScale, 1*mScale, 1)));
	}
	mCollisionShape->calculateLocalInertia(mass, local_inertia);
	transform.setOrigin(btVector3(mPosition.x, mPosition.y, 0));
	transform.setRotation(btQuaternion(0, 0, 1, mRotation));

	mMotionState = boost::shared_ptr<btDefaultMotionState>(new btDefaultMotionState(transform));
	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, mMotionState.get(), mCollisionShape.get(), local_inertia);
	mBody = boost::shared_ptr<btRigidBody>(new btRigidBody(rb_info));
	mBody->setDamping(0.2f, 0.2f);
	//mBody->setActivationState(DISABLE_DEACTIVATION);
	mBody->setLinearFactor(btVector3(1,1,0));
	mBody->setAngularFactor(btVector3(0,0,1));
	//mBody->setAngularFactor(btVector3(0,0,1));
	mBody->setUserPointer(this);

	/*
	if(mUID == "player") {
		btTransform t;
		t.setIdentity();
		btScalar playerweight_mass(10.f);
		btVector3 playerweight_local_inertia(0, 0, 0);
		mPlayerWeightCollisionShape = boost::shared_ptr<btCollisionShape>(new btSphereShape(1));
		mPlayerWeightCollisionShape->calculateLocalInertia(playerweight_mass, playerweight_local_inertia);
		t.setOrigin(btVector3(mPosition.x - 2, mPosition.y - 1, 0));
		t.setRotation(btQuaternion(0, 0, 1, mRotation));

		mPlayerWeightMotionState = boost::shared_ptr<btDefaultMotionState>(new btDefaultMotionState(t));
		btRigidBody::btRigidBodyConstructionInfo player_weight_rb_info(playerweight_mass, mPlayerWeightMotionState.get(), mPlayerWeightCollisionShape.get(), playerweight_local_inertia);
		mPlayerWeightBody = boost::shared_ptr<btRigidBody>(new btRigidBody(player_weight_rb_info));
		mPlayerWeightBody->setDamping(0.2f, 0.2f);
		mPlayerWeightBody->setLinearFactor(btVector3(1,1,0));
		mPlayerWeightBody->setAngularFactor(btVector3(0,0,1));
		mPlayerWeightBody->setUserPointer(this);
		GameApp::get_mutable_instance().GetWorldPtr()->AddRigidBody(mPlayerWeightBody.get());
	}*/
}

void Entity::Update(float time_delta) {
	UpdateAllAttachments(time_delta);

	if (mUsePhysics) {
		// Move
		btTransform trans;
		mBody->getMotionState()->getWorldTransform(trans);
		btVector3 origin = trans.getOrigin();
		SetPosition(Vector2D(origin.x(), origin.y()));

		btMatrix3x3 rot;
		rot.setIdentity();
		rot = trans.getBasis();
		float fx,fy,fz;
		rot.getEulerZYX(fz,fy,fx);
		SetRotation(PI-fz);
	}
}

void Entity::UpdateAllAttachments(float time_delta) {
	BOOST_FOREACH (EntityAttachment& attachment, mAttachments) {
		attachment.Update(time_delta, mPosition, mRotation);
	}
}

void Entity::Draw(sf::RenderTarget* target, sf::Shader& shader, bool editor_mode) const {

	Vector2D p = Coordinates::WorldFloatToWorldPixel(mPosition);
	// TODO: Actually draw
	sf::Sprite s;
	s.SetImage(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage(mImageFile));
	s.SetOrigin(s.GetImage()->GetWidth()/2, s.GetImage()->GetHeight()/2);
	s.SetPosition(p.x, p.y);
	s.SetRotation(Vector2D::rad2Deg(mRotation));
	s.SetScale(mScale, mScale);
	s.SetColor(sf::Color(255,255,255,mAlpha * 255));
	target->Draw(s, shader);

	if (editor_mode) {
		sf::Color c = sf::Color(80,80,80);
		if (mHighlightMode == 1) c = sf::Color(255,255,255);
		else if (mHighlightMode == 2) c = sf::Color(255,0,180);
		sf::Shape center = sf::Shape::Circle(p.x, p.y, 3, c, 1.f);
		target->Draw(center);
	}
}

void Entity::DrawAllAttachments(sf::RenderTarget* target, sf::Shader& shader) {
	BOOST_FOREACH (EntityAttachment& attachment, mAttachments) {
		attachment.Draw(target, shader);
	}
}

void Entity::SetImage(const std::string& filename) {
	mImageFile = filename;
}

const std::string& Entity::GetImage() const {
	return mImageFile;
}

void Entity::GenerateUID() {
	// GameApp::get_mutable_instance().GetNextId()
	int i = GameApp::get_mutable_instance().GetNextId();
	std::string str = "entity-" + boost::lexical_cast<std::string>(i);
	while(GameApp::get_mutable_instance().GetWorldPtr()->GetEntityByUID(str) != NULL) {
		i = GameApp::get_mutable_instance().GetNextId();
		str = "entity-" + boost::lexical_cast<std::string>(i);
	}
	SetUID(str);
}

void Entity::SetUID(const std::string& id) {
	mUID = id;
}

const std::string& Entity::GetUID() const {
	return mUID;
}

void Entity::SetPosition(const Vector2D position) {
	mPosition = position;
}

const Vector2D Entity::GetPosition() const {
	return mPosition;
}

void Entity::SetSpeed(const Vector2D speed) {
	mSpeed = speed;
}

const Vector2D Entity::GetSpeed() const {
	return mSpeed;
}

void Entity::SetRotation(float rotation) {
	mRotation = rotation;
}

float Entity::GetRotation() const {
	return mRotation;
}

void Entity::SetRotationSpeed(float speed) {
	mRotationSpeed = speed;
}

float Entity::GetRotationSpeed() const {
	return mRotationSpeed;
}

void Entity::SetAlpha(float alpha) {
	mAlpha = alpha;
}

float Entity::GetAlpha() const {
	return mAlpha;
}

void Entity::SetScale(float scale) {
	mScale = scale;
}

float Entity::GetScale() const {
	return mScale;
}

int Entity::GetLayer() const {
	return mLayer;
}

void Entity::SetLayer(int layer) {
	mLayer = layer;
}

int Entity::GetLocalLayer() const {
	return mLocalLayer;
}

void Entity::SetLocalLayer(int layer) {
	mLocalLayer = layer;
}

int Entity::GetHighlightMode() const {
	return mHighlightMode;
}

void Entity::SetHighlightMode(int mode) {
	mHighlightMode = mode;
}

boost::shared_ptr<sf::Sprite> Entity::GetDrawable() const {
	return mDrawable;
}

void Entity::Attach(Entity& entity, const Vector2D& position_offset, const float rotation_offset) {
	EntityAttachment* attachment = new EntityAttachment(&entity, position_offset, rotation_offset);
	mAttachments.push_back(attachment);
}

boost::shared_ptr<btCollisionShape> Entity::GetCollisionShape() {
	return mCollisionShape;
}

boost::shared_ptr<btRigidBody> Entity::GetBody() {
	return mBody;
}

boost::shared_ptr<btDefaultMotionState> Entity::GetMotionState() {
	return mMotionState;
}

bool Entity::SortHelper(const Entity& l, const Entity& r) {
	if(l.GetLayer() == r.GetLayer()) {
		return l.GetLocalLayer() < r.GetLocalLayer();
	}
	return l.GetLayer() < r.GetLayer();
}

void Entity::Save(boost::property_tree::ptree* pt) {
	pt->add("entities."+mUID+".x", mPosition.x);
	pt->add("entities."+mUID+".y", mPosition.y);
	pt->add("entities."+mUID+".img", mImageFile);
	pt->add("entities."+mUID+".rot", mRotation);
	pt->add("entities."+mUID+".a", mAlpha);
	pt->add("entities."+mUID+".s", mScale);
	pt->add("entities."+mUID+".l", mLayer);
	pt->add("entities."+mUID+".ll", mLocalLayer);
	pt->add("entities."+mUID+".phy", mUsePhysics);
}

bool Entity::UsesPhysics() const {
	return mUsePhysics;
}

void Entity::TogglePhysics(World& world) {
	SetUsePhysics(world, !mUsePhysics);
}

void Entity::SetUsePhysics(World& world, bool use) {
	if (use != mUsePhysics) {
		if (use) {
			InitializePhysics();
			world.AddRigidBody(mBody.get());
		} else {
			world.RemoveRigidBody(mBody.get());
		}
		mUsePhysics = use;
	}
}


void Entity::OnCollide(GameObject* other) {}
