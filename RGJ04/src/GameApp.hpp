#ifndef GAMEAPP_HPP
#define GAMEAPP_HPP

#include <iostream>

#include <SFML/Graphics.hpp>

#include <bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h>
#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include <BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h>

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/foreach.hpp>

#include "DebugDraw.hpp"
#include "Definitions.hpp"
#include "LevelBlock.hpp"

class GameApp {
public:
	GameApp();
	~GameApp();

	void Init();
	void Run();
private:
	boost::shared_ptr<sf::RenderWindow> mRenderWin;
	boost::shared_ptr<sf::View> mView;

	sf::Event mEvent;
	sf::Clock mClock;
	sf::Text mText;

	// Collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	boost::shared_ptr<btDefaultCollisionConfiguration> mCollisionConfiguration;

	// Use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded).
	boost::shared_ptr<btCollisionDispatcher> mCollisionDispatcher;

	// btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	boost::shared_ptr<btDbvtBroadphase> mBroadphase;

	// The default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded).
	boost::shared_ptr<btVoronoiSimplexSolver> mSimplex;
	boost::shared_ptr<btMinkowskiPenetrationDepthSolver> mPdSolver;
	boost::shared_ptr<btSequentialImpulseConstraintSolver> mSolver;
	
	// The world contains all dynamic objects.
	boost::shared_ptr<btDiscreteDynamicsWorld> mDynamicsWorld;

	boost::shared_ptr<btConvex2dConvex2dAlgorithm::CreateFunc> mConvexAlgo2d;
	boost::shared_ptr<btBox2dBox2dCollisionAlgorithm::CreateFunc> mBox2dAlgo2d;

	// Get us some debug output. Without this, we'd see nothing at all.
	boost::shared_ptr<DebugDraw> mDebugDraw;

	// TODO: Not used right now!
	// Keep track of the shapes, we release memory at exit.
	// Make sure to re-use collision shapes among rigid bodies whenever possible!
	//btAlignedObjectArray<btCollisionShape> mCollisionShapes;

	// Generic wall shape used for constructing everything.
	boost::shared_ptr<btBoxShape> mWallShape;	

	// Level
	boost::ptr_list<LevelBlock> mLevelBlocks;

	// Player
	boost::shared_ptr<btConeShape> mPlayerShape;
	boost::shared_ptr<btDefaultMotionState> mPlayerMotionState;
	boost::shared_ptr<btRigidBody> mPlayerBody;
};

#endif
