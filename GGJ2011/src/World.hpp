#ifndef WORLD_HPP
#define WORLD_HPP

#include <string>
#include <cstdio>

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>
#include <boost/signals2.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h>
#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include <BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h>

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "CollisionPolygon.hpp"
#include "Coordinates.hpp"
#include "DebugDraw.hpp"
#include "Rail.hpp"

enum EditorMouseAction {
	EMA_GRAB,
	EMA_SCALE,
	EMA_ROTATE,
	EMA_ALPHA,
	EMA_NONE,
	EMA_ACCEPT
};

static void myTickCallback(btDynamicsWorld *world, btScalar timeStep);

class World {
public:
	World();
	~World();

	void Initialize();
	void Update(const float time_delta);
	void Draw(sf::RenderTarget* const target, sf::Shader& shader);
	void AddEntity(Entity* entity);

	void HandleEvent(const sf::Event& event);
	
	int GetEntityCount() const;
	std::vector<Entity*> GetEntitiesByLayer(int layer);
	Entity* GetClosestEntityOnLayer(const Vector2D pos, int layer=0);
	CollisionPolygon* GetClosestCollisionPolygon();
	Rail* GetClosestRail();
	
	Entity* GetEntityByUID(const std::string& uid);

	void DeleteEntityByUID(const std::string& uid);

	void ToggleSetMouseAction(EditorMouseAction action);

	static std::string GetImageKeyRelativeTo(const std::string& original, int offset);

	int GetEditorLayer() const;
	void SetEditorLayer(int layer);

	void Save();
	void Load();

	void SetSelectedEntity(Entity* e);
	void SetRenameMode(bool mode);

	void TickCallback(btScalar timestep);

	void AddRigidBody(btRigidBody* body);
	void RemoveRigidBody(btRigidBody* body);

	btDiscreteDynamicsWorld* GetDynamicsWorld();

	void ReloadTriMeshBody();
	Entity* GetEntityByLocalLayerId(int ll);

	Entity* GetBoxEntity();
	Rail* GetCurrentRail();
	void SetCurrentRail(Rail* rail);

private:
	boost::ptr_vector<Entity> mEntities;
	boost::ptr_list<CollisionPolygon> mCollisionPolygons;
	boost::ptr_list<Rail> mRails;

	boost::shared_ptr<btDefaultCollisionConfiguration> mCollisionConfiguration;
	boost::shared_ptr<btCollisionDispatcher> mCollisionDispatcher;
	boost::shared_ptr<btDbvtBroadphase> mBroadphase;
	boost::shared_ptr<btVoronoiSimplexSolver> mSimplex;
	boost::shared_ptr<btMinkowskiPenetrationDepthSolver> mPdSolver;
	boost::shared_ptr<btSequentialImpulseConstraintSolver> mSolver;
	boost::shared_ptr<btDiscreteDynamicsWorld> mDynamicsWorld;
	boost::shared_ptr<btConvex2dConvex2dAlgorithm::CreateFunc> mConvexAlgo2d;
	boost::shared_ptr<btBox2dBox2dCollisionAlgorithm::CreateFunc> mBox2dAlgo2d;
	boost::shared_ptr<btTriangleMesh> mTriMesh;
	boost::shared_ptr<btBvhTriangleMeshShape> mTriMeshShape;
	boost::shared_ptr<btDefaultMotionState> mTriMeshBodyMotionState;
	boost::shared_ptr<btRigidBody> mTriMeshBody;

	boost::shared_ptr<DebugDraw> mDebugDraw;

	bool mEntityListNeedsSorting;

	int mEditorLayer;
	EditorMouseAction mEditorMouseAction;
	Coordinates mEditorMouseActionStartMousePosition;
	Coordinates mEditorMouseActionStartEntityPosition;
	float mEditorMouseActionStartEntityRotation;
	float mEditorMouseActionStartEntityScale;
	float mEditorMouseActionStartEntityAlpha;
	Entity* mEditorMouseActionEntity;

	bool mEditorRenameMode;
	std::string mEditorRenameString;
	bool mEditorEnabledRenameThisFrame;

	bool mEditorPolygonFinished;
	bool mEditorRailFinished;

	Entity* mEditorSelectedEntity;
	int mLocalLayerCount;

	Rail* mClosestRail;
	Vector2D mClosestRailPoint;

	Rail* mCurrentRail;
};

#endif
