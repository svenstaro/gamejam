#ifndef LEVELBLOCK_HPP
#define LEVELBLOCK_HPP

#include <string>

#include <SFML/Graphics.hpp>

#include <bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h>
#include <BulletCollision/CollisionShapes/btConvex2dShape.h>

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include "Definitions.hpp"
#include "LevelAttachment.hpp"

class LevelBlock {
public:
	LevelBlock(bool topwall, bool rightwall, bool bottomwall, bool leftwall,
			   const sf::Uint16 tile_x, const sf::Uint16 tile_y, 
			   const boost::shared_ptr<btBoxShape> wall_shape, 
			   const boost::shared_ptr<btDiscreteDynamicsWorld> dynamics_world);
	~LevelBlock();

	void Attach(LevelAttachment* attachment);
	void AttachText(const std::string& text);

	void Draw(boost::shared_ptr<sf::RenderWindow> window);

private:
	sf::Uint16 mTileX;
	sf::Uint16 mTileY;

	boost::ptr_list<LevelAttachment> mLevelAttachments;
	boost::ptr_list<sf::Text> mTextAttachments;

	boost::shared_ptr<btBoxShape> mWallShape;
	boost::shared_ptr<btDiscreteDynamicsWorld> mDynamicsWorld;

	boost::shared_ptr<btDefaultMotionState> mTopWallMotionState;
	boost::shared_ptr<btRigidBody> mTopWallBody;

	boost::shared_ptr<btDefaultMotionState> mRightWallMotionState;
	boost::shared_ptr<btRigidBody> mRightWallBody;

	boost::shared_ptr<btDefaultMotionState> mBottomWallMotionState;
	boost::shared_ptr<btRigidBody> mBottomWallBody;

	boost::shared_ptr<btDefaultMotionState> mLeftWallMotionState;
	boost::shared_ptr<btRigidBody> mLeftWallBody;
};

#endif
