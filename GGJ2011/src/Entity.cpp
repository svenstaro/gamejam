#include "Entity.hpp"
#include "GameApp.hpp"
#include "World.hpp"

Entity::Entity() {
	mAlpha = 1.f;
	mRotation = 0.f;
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

	mInitialPosition = mPosition;
	mInitialRotation = mRotation;
	mInitialScale = mScale;
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

	if(mUID.substr(0,5) == "empty") {
		mCollisionShape = boost::shared_ptr<btCollisionShape>(new btBoxShape(btVector3(15*mScale, 1*mScale, 1)));
	} else if(mImageFile == "evil1") {
		mCollisionShape = boost::shared_ptr<btCollisionShape>(new btBoxShape(btVector3(0.5*mScale, 0.22*mScale, 1)));
	} else if(mImageFile == "evil2") {
		mCollisionShape = boost::shared_ptr<btCollisionShape>(new btBoxShape(btVector3(0.5*mScale, 0.05*mScale, 1)));
	} else {
		mCollisionShape = boost::shared_ptr<btCollisionShape>(new btBoxShape(btVector3(1.2*mScale, 1.2*mScale, 1)));
	}

	mCollisionShape->calculateLocalInertia(mass, local_inertia);
	transform.setOrigin(btVector3(mPosition.x, mPosition.y, 0));
	transform.setRotation(btQuaternion(0, 0, PI - mRotation));

	mMotionState = boost::shared_ptr<btDefaultMotionState>(new btDefaultMotionState(transform));
	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, mMotionState.get(), mCollisionShape.get(), local_inertia);
	mBody = boost::shared_ptr<btRigidBody>(new btRigidBody(rb_info));
	mBody->setDamping(0.2f, 0.2f);
	//mBody->setActivationState(DISABLE_DEACTIVATION);
	mBody->setLinearFactor(btVector3(1,1,0));
	mBody->setAngularFactor(btVector3(0,0,1));
	//mBody->setAngularFactor(btVector3(0,0,1));
	mBody->setUserPointer(this);
}

void Entity::Reset(World& world) {
	world.GetDynamicsWorld()->removeRigidBody(mBody.get());

	SetPosition(mInitialPosition);
	SetScale(mInitialScale);
	SetRotation(mInitialRotation);
	bool p = mUsePhysics;
	SetUsePhysics(world, false);
	SetUsePhysics(world, p);
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
	mWasCollidedLastTick -= 1;
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

void Entity::SetRotation(float rotation) {
	mRotation = rotation;
}

float Entity::GetRotation() const {
	return mRotation;
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
			world.GetDynamicsWorld()->addRigidBody(mBody.get(), COL_BOX, COL_WALL | COL_MOVER | COL_BOX);
		} else {
			world.RemoveRigidBody(mBody.get());
		}
		mUsePhysics = use;
	}
}


bool Entity::OnCollide(GameObject* other) {
	if(mUID == "box") {
		if(mWasCollidedLastTick <= 0) {
			GameApp::get_mutable_instance().GetResourceManagerPtr()->PlaySound("collide");

			if(other != NULL && other->ToString() == "entity") {
				std::string uid = ((Entity*)other)->GetUID();
				if(uid == "target" || uid == "empty-target") {
					std::cout << "Level complete!" << std::endl;
					GameApp::get_mutable_instance().GetWorldPtr()->LoadNextLevel();
					return false;
				} else if(uid == "empty-quit") {
					exit(0);
				} else if(uid == "empty-credits") {
					GameApp::get_mutable_instance().ShowCredits();
					return false;
				} else if(uid.substr(0,4) == "evil") {
					GameApp::get_mutable_instance().GetWorldPtr()->ResetLevel();
				}
			}
		}
	}
	mWasCollidedLastTick = 2;
	return true;
}

std::string Entity::ToString() {
	return "entity";
}
