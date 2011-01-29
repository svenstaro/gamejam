#include "Rail.hpp"

#include "GameApp.hpp"

Rail::Rail() {
	mLastPointSet = 0;
	mStartPosition = 0.5f;
}

void Rail::Initialize(World& world) {
	InitializePhysics();
	world.GetDynamicsWorld()->addConstraint(mConstraint.get());
	world.GetDynamicsWorld()->addRigidBody(mBody.get());
}

void Rail::InitializePhysics() {
	btTransform tr;
	tr.setIdentity();
	Coordinates tmp;
	tmp.SetWorldPixel(GetPointFromFloat(mStartPosition));
	Vector2D p = tmp.GetWorldFloat();

	tmp.SetWorldPixel(mPoint2-mPoint1);
	Vector2D o = tmp.GetWorldFloat();
	o.Normalize();
	o.Rotate(-90);
	o *= 0.34;
	tr.setOrigin(btVector3(p.x + o.x, p.y + o.y, 0));
	tr.setRotation(btQuaternion(0,0,(mPoint2 - mPoint1).Rotation()));

	btScalar mass(1.f);
	btVector3 local_inertia(0, 0, 0);
	mCollisionShape = boost::shared_ptr<btCollisionShape>(new btBoxShape(btVector3(.4f,.34f,.4f)));
	mCollisionShape->calculateLocalInertia(mass, local_inertia);

	mMotionState = boost::shared_ptr<btDefaultMotionState>(new btDefaultMotionState(tr));
	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, mMotionState.get(), mCollisionShape.get(), local_inertia);

	mBody = boost::shared_ptr<btRigidBody>(new btRigidBody(rb_info));
	mBody->setDamping(.99f, .99f);
	mBody->setLinearFactor(btVector3(1,1,0));
	mBody->setAngularFactor(btVector3(0,0,1));
	mBody->setUserPointer(this);
	mBody->setActivationState(DISABLE_DEACTIVATION);

	btTransform frameB;
	frameB.setIdentity();
	//frameB.setOrigin(btVector3(p.x, p.y, 0));
	//frameB.setRotation(btQuaternion(0,0,(mPoint2 - mPoint1).Rotation()));
	mConstraint = boost::shared_ptr<btGeneric6DofConstraint>(new btGeneric6DofConstraint( *mBody, frameB, true ));

	mConstraint->setAngularLowerLimit(btVector3(0,0,0));
	mConstraint->setAngularUpperLimit(btVector3(0,0,0));
	float l = Coordinates::ScreenPixelToWorldFloat(mPoint2 - mPoint1).Magnitude();
	mConstraint->setLinearLowerLimit(btVector3(-l/2+0.3, 0,0));
	mConstraint->setLinearUpperLimit(btVector3(l/2,0,0));

	/*mConstraint->getTranslationalLimitMotor()->m_enableMotor[0] = true;
	mConstraint->getTranslationalLimitMotor()->m_targetVelocity[0] = 5.0f;
	mConstraint->getTranslationalLimitMotor()->m_maxMotorForce[0] = 0.1f;*/

	mConstraint->setDbgDrawSize(btScalar(5.f));
}

void Rail::Update(float time_delta) {
	mTiledSprite.SetImage(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetImage("rail"));
	Vector2D diff = mPoint2 - mPoint1;
	mTiledSprite.SetPosition(mPoint1.x, mPoint1.y);
	mTiledSprite.SetScale(diff.Magnitude(), 20);
	mTiledSprite.SetRotation(- diff.Rotation() / PI * 180.f);

	mMover.SetRail(this);
	mMover.Update(time_delta);

	// push / pull box
	bool current_and_down = GameApp::get_mutable_instance().GetInput().IsMouseButtonDown(sf::Mouse::Left) && IsCurrentRail();

	Entity* box = GameApp::get_mutable_instance().GetWorldPtr()->GetBoxEntity();
	if(box != NULL && box->UsesPhysics() && box->GetBody().get() != NULL && mBody.get() != NULL) {
		//btVector3 force = (mPoint2 - mPoint1);
		box->GetBody()->activate();
		box->GetBody()->setFriction(btScalar(100.f));
		btVector3 force = mBody->getWorldTransform().getOrigin() - box->GetBody()->getWorldTransform().getOrigin();
		float d = 1 - force.length() / 5.f;
		if (d > 0.f) {
			//force *= 1 / d*d;
			force *= d*d*10;

			if(current_and_down)
				force *= -1;
			box->GetBody()->applyCentralForce(force);
		}
	}
}

void Rail::Draw(sf::RenderTarget* target, sf::Shader& shader, bool editor_mode) const {
	if(IsFinished()) {
		// debug: line
		TiledSprite s(mTiledSprite);
		if(editor_mode)
			s.SetColor(sf::Color(255,255,255,128));
		target->Draw(s);
	} else {
		// draw p1
		sf::Shape shape = sf::Shape::Circle(mPoint1.x, mPoint1.y, 3.f, sf::Color(255,255,255));
		target->Draw(shape);
	}

	mMover.Draw(target, shader, editor_mode);
}

void Rail::SetNextPoint(Vector2D point) {
	if(mLastPointSet == 0) {
		mPoint1 = point;
	} else if(mLastPointSet == 1) {
		mPoint2 = point;
	}
	++mLastPointSet;
}

void Rail::Save(boost::property_tree::ptree* pt, int id) {
	std::string sid = boost::lexical_cast<std::string>(id);
	pt->add("rails."+sid+".1.x", mPoint1.x);
	pt->add("rails."+sid+".1.y", mPoint1.y);
	pt->add("rails."+sid+".2.x", mPoint2.x);
	pt->add("rails."+sid+".2.y", mPoint2.y);
}

void Rail::Load(boost::property_tree::ptree* pt, int id) {
	std::string sid = boost::lexical_cast<std::string>(id);
	mPoint1.x = pt->get<float>("rails."+sid+".1.x");
	mPoint1.y = pt->get<float>("rails."+sid+".1.y");
	mPoint2.x = pt->get<float>("rails."+sid+".2.x");
	mPoint2.y = pt->get<float>("rails."+sid+".2.y");
	mLastPointSet = 2;

}

Vector2D Rail::GetCenter() {
	return (mPoint1 + mPoint2) / 2;
}

float Rail::GetRotation() {
	return (mPoint2 - mPoint1).Rotation();
}

bool Rail::IsFinished() const {
	return mLastPointSet >= 2;
}

float Rail::ClosestPositionOnLine(Vector2D pixel_pos) {
	// don't ask me, ask him ;)
	// http://www.codeguru.com/forum/showpost.php?p=538288&postcount=2
	double cx = pixel_pos.x;
	double cy = pixel_pos.y;
	double ax = mPoint1.x;
	double ay = mPoint1.y;
	double bx = mPoint2.x;
	double by = mPoint2.y;
	double distanceSegment, distanceLine;


	double r_numerator = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
	double r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
	double r = r_numerator / r_denomenator;
	double px = ax + r*(bx-ax);
	double py = ay + r*(by-ay);
	double s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;
	distanceLine = fabs(s)*sqrt(r_denomenator);

	//
	// (xx,yy) is the point on the lineSegment closest to (cx,cy)
	//
	double xx = px;
	double yy = py;

	if(r > 1) r = 1;
	if(r < 0) r = 0;
	return r;

	if ((r >= 0) && (r <= 1)) {
		distanceSegment = distanceLine;
	} else {
		double dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
		double dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
		if (dist1 < dist2) {
			xx = ax;
			yy = ay;
			distanceSegment = sqrt(dist1);
		} else {
			xx = bx;
			yy = by;
			distanceSegment = sqrt(dist2);
		}
	}

	return distanceSegment;
}


Vector2D Rail::GetPointFromFloat(float f) const {
	return mPoint1 + (mPoint2-mPoint1) * f;
}

btRigidBody* Rail::GetRigidBody() {
	return mBody.get();
}

btTypedConstraint* Rail::GetConstraint() {
	return mConstraint.get();
}


void Rail::OnCollide(GameObject* other) {
	if(other == GameApp::get_mutable_instance().GetWorldPtr()->GetBoxEntity()) {
		GameApp::get_mutable_instance().GetWorldPtr()->SetCurrentRail(this);
	}
}

bool Rail::IsCurrentRail() const {
	return (this == GameApp::get_mutable_instance().GetWorldPtr()->GetCurrentRail());
}
