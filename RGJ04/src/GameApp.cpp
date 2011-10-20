#include "GameApp.hpp"

GameApp::GameApp() {}

GameApp::~GameApp() {}

void GameApp::Init() {
	mRenderWin = boost::shared_ptr<sf::RenderWindow> (new sf::RenderWindow);
	mRenderWin->Create(sf::VideoMode(WIDTH, HEIGHT, 32), "Polymorphosis");
	mRenderWin->EnableVerticalSync(true);

	mView = boost::shared_ptr<sf::View> (new sf::View(sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(WIDTH, HEIGHT)));

	mRenderWin->SetView(*mView.get());

	mCollisionConfiguration = boost::shared_ptr<btDefaultCollisionConfiguration> (new btDefaultCollisionConfiguration);

	mCollisionDispatcher = boost::shared_ptr<btCollisionDispatcher> (new btCollisionDispatcher(mCollisionConfiguration.get()));
	
	mBroadphase = boost::shared_ptr<btDbvtBroadphase> (new btDbvtBroadphase);

	mSimplex = boost::shared_ptr<btVoronoiSimplexSolver> (new btVoronoiSimplexSolver);
	mPdSolver = boost::shared_ptr<btMinkowskiPenetrationDepthSolver> (new btMinkowskiPenetrationDepthSolver);
	mSolver = boost::shared_ptr<btSequentialImpulseConstraintSolver> (new btSequentialImpulseConstraintSolver);

	mDynamicsWorld = boost::shared_ptr<btDiscreteDynamicsWorld> (new btDiscreteDynamicsWorld(mCollisionDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfiguration.get()));

	mConvexAlgo2d = boost::shared_ptr<btConvex2dConvex2dAlgorithm::CreateFunc> (new btConvex2dConvex2dAlgorithm::CreateFunc(mSimplex.get(), mPdSolver.get()));

	mCollisionDispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE, mConvexAlgo2d.get());
	mCollisionDispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE, mConvexAlgo2d.get());
	mCollisionDispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, mConvexAlgo2d.get());
	//mCollisiDispatcheron.registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, new btBox2dBox2dCollisionAlgorithm::CreateFunc());
	mCollisionDispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE, mBox2dAlgo2d.get());

	// Set gravity to 9.8m/s² along y-axis.
	mDynamicsWorld->setGravity(btVector3(0, 1, 0));

	mDebugDraw = boost::shared_ptr<DebugDraw> (new DebugDraw(mRenderWin));
	//mDebugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mDebugDraw->setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);

	mDynamicsWorld->setDebugDrawer(mDebugDraw.get());

	// Create a wall body.
	btScalar thickness(0.2);
	btScalar extrusion(10); // We don't really use this in 2D but we need it set.
	mWallShape = boost::shared_ptr<btBoxShape> (new btBoxShape(btBoxShape(btVector3(btScalar(WIDTH / 2 * METERS_PER_PIXEL), thickness, extrusion))));
	//mCollisionShapes.push_back(mWallShape);

	// Level "design" below
	// Legend:	S	-	player start
	//			O	-	level block opened connected level blocks
	//			D	-	locked door
	//			K	-	key needed for door
	//			G	-	goal

	/*    000000000011111111
	 *    012345678901234567
	 *    
	 * 00 SOOOO OOOO   OOO O
	 * 01     OOO  O   O O O
	 * 02      D   O   O O O
	 * 03      O OOOOOOO OOO
	 * 04    OOO           O
	 * 05    O   OOOOOOOOO O
	 * 06 ODOOOOOO   O   O O
	 * 07 O              O O
	 * 08 O OOO OOO  KOOOO O
	 * 09 OOO OOO OG       K
	 */

	// Create level blocks.
	// AREA A (first key and first door)
	mLevelBlocks.push_back(new LevelBlock(true, false, true, true, 0, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.back().Attach(new LevelAttachment(sf::Vector2i(100, 100), sf::Vector2i(50, 50), mDynamicsWorld));
	mLevelBlocks.back().AttachText("Hey there!");
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 1, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.back().AttachText("You can FLY!");
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 2, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.back().AttachText("Oh... you alredy knew...");
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 3, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.back().AttachText("Well...");
	mLevelBlocks.push_back(new LevelBlock(true, true, false, false, 4, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.back().AttachText("You can also die.");
	mLevelBlocks.push_back(new LevelBlock(false, false, true, true, 4, 1, mWallShape, mDynamicsWorld));
	mLevelBlocks.back().AttachText("But you probably already \nknew that as well.");
	mLevelBlocks.push_back(new LevelBlock(true, false, false, false, 5, 1, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 5, 2, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, true, false, 6, 1, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, false, true, 6, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 7, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 8, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, true, false, false, 9, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 9, 1, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 9, 2, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, false, true, false, 9, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, true, 8, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 10, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 11, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 12, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, true, false, 13, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 13, 2, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 13, 1, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, false, true, 13, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 14, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, true, false, false, 15, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 15, 1, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 15, 2, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, false, true, true, 15, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 16, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, false, 17, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 17, 2, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 17, 1, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, true, false, true, 17, 0, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 17, 4, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 17, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 17, 6, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 17, 7, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 17, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, true, true, 17, 9, mWallShape, mDynamicsWorld));

	// AREA B (second key and second door)
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 5, 3, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, true, false, 5, 4, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 4, 4, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, false, true, 3, 4, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 3, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, false, true, false, 3, 6, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 4, 6, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 5, 6, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 6, 6, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, true, false, 7, 6, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, false, true, 7, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 8, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 9, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 10, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, false, false, 11, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 12, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 13, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 14, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, true, false, false, 15, 5, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 15, 6, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 15, 7, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, true, false, 15, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 14, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 13, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 12, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, true, 11, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 2, 6, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 1, 6, mWallShape, mDynamicsWorld));

	// AREA C (up to goal)
	mLevelBlocks.push_back(new LevelBlock(true, false, false, true, 0, 6, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 0, 7, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, false, true, 0, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, false, true, true, 0, 9, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 1, 9, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, true, false, 2, 9, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, false, true, 2, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 3, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, true, false, false, 4, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, false, true, true, 4, 9, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 5, 9, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, true, true, false, 6, 9, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, false, true, 6, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, false, true, false, 7, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, true, false, false, 8, 8, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(false, false, true, true, 8, 9, mWallShape, mDynamicsWorld));
	mLevelBlocks.push_back(new LevelBlock(true, true, true, false, 9, 9, mWallShape, mDynamicsWorld));

	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	mPlayerShape = boost::shared_ptr<btConeShape> (new btConeShape(btScalar(0.6), btScalar(1.0)));
	//mCollisionShapes.push_back(&mPlayerShape);

	/// Create Dynamic Objects
	btTransform start_transform;
	start_transform.setIdentity();

	btScalar mass(1.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 local_inertia(0, 0, 0);
	if(isDynamic)
		mPlayerShape->calculateLocalInertia(mass, local_inertia);

	start_transform.setOrigin(btVector3(2, 5, 0));
	start_transform.setRotation(btQuaternion(0, 0, 1, 0));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	mPlayerMotionState = boost::shared_ptr<btDefaultMotionState> (new btDefaultMotionState(start_transform));
	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, mPlayerMotionState.get(), mPlayerShape.get(), local_inertia);
	mPlayerBody = boost::shared_ptr<btRigidBody> (new btRigidBody(rb_info));
	mPlayerBody->setDamping(0.2f, 0.2f);
	mPlayerBody->setActivationState(DISABLE_DEACTIVATION);

	mPlayerBody->setLinearFactor(btVector3(1,1,0));
	mPlayerBody->setAngularFactor(btVector3(0,0,1));

	mDynamicsWorld->addRigidBody(mPlayerBody.get(), COL_PLAYER, PlayerCollidesWith);
}

void GameApp::Run() {
	while(mRenderWin->IsOpened()) {
		while(mRenderWin->PollEvent(mEvent)) {
			if(mEvent.Type == sf::Event::Closed) 
				mRenderWin->Close();
			if(mEvent.Type == sf::Event::KeyPressed) {
				if(mEvent.Key.Code == sf::Keyboard::Escape) 
					mRenderWin->Close();
			}
		}

		//float time_delta = mClock.GetElapsedTime();
		mClock.Reset();

		// get the current "frame time" (seconds elapsed since the previous frame, hopefully close to 1/60 since vsync is enabled)
		//float frameTime = mRenderWin->GetFrameTime();

		//step the Box2D physics world, with a constant time step
		//note that this is kind of a bad way to do time steps, as it can get out of sync if the framerate drops (see http://gafferongames.wordpress.com/game-physics/fix-your-timestep/ for more information)
		mDynamicsWorld->stepSimulation(1 / 60.f, 10);
		mDynamicsWorld->clearForces();

		//check for user keyboard input to control Bullet forces/torques/etc
		//float mag = 50.0f;
		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::Left)) {
			btVector3 relative_force = btVector3(0, 0, -2);
			mPlayerBody->applyTorque(relative_force);
		}
		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::Right)) {
			btVector3 relative_force = btVector3(0, 0, 2);
			mPlayerBody->applyTorque(relative_force);
		}
		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::Up)) {
			btVector3 relativeForce = btVector3(0, 5, 0);
			btMatrix3x3& boxRot = mPlayerBody->getWorldTransform().getBasis();
			btVector3 correctedForce = boxRot * relativeForce;
			mPlayerBody->applyCentralForce(correctedForce);
		}
		if(sf::Keyboard::IsKeyPressed(sf::Keyboard::Down)) {
			btVector3 relativeForce = btVector3(0, -2, 0);
			btMatrix3x3& boxRot = mPlayerBody->getWorldTransform().getBasis();
			btVector3 correctedForce = boxRot * relativeForce;
			mPlayerBody->applyCentralForce(correctedForce);
		}

		mRenderWin->Clear(sf::Color(50,50,50));

		// Mode 1
		int new_width = WIDTH / 2 + WIDTH * int((mPlayerBody->getCenterOfMassPosition().x() * PIXELS_PER_METER) / WIDTH);
		int new_height = HEIGHT / 2 + HEIGHT * int((mPlayerBody->getCenterOfMassPosition().y() * PIXELS_PER_METER) / HEIGHT);
		mView->SetCenter(new_width, new_height);
		std::cout << "x: " << new_width / WIDTH << " y: " << new_height / HEIGHT << std::endl;

		// Mode 2
		/*int new_width = mPlayerBody->getCenterOfMassPosition().x() * PIXELS_PER_METER;
		int new_height = mPlayerBody->getCenterOfMassPosition().y() * PIXELS_PER_METER;
		mView->SetCenter(new_width, new_height);
		std::cout << "x: " << new_width << " y: " << new_height << std::endl;
		*/

		mRenderWin->SetView(*mView);

		/*
		//print positions of all objects
		for(int j=dynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--) {
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState()) {
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);
				//printf("world pos = %f,%f,%f\n", float(trans.getOrigin().getX()),float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
			}
		}*/

		//have Bullet use our debug drawing class to render the world
		
		// TODO: Enable this for debug!
		mDynamicsWorld->debugDrawWorld();

		BOOST_FOREACH(LevelBlock& block, mLevelBlocks) {
			block.Draw(mRenderWin);
		}

		//mText = sf::Text(boost::lexical_cast<std::string>(1.f / time_delta));
		//mText.SetPosition(5, 5);
		//mRenderWin->Draw(mText);

		mRenderWin->Display();
	}
}
