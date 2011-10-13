#include "LevelBlock.hpp"

LevelBlock::LevelBlock(bool topwall, bool rightwall, bool bottomwall, bool leftwall,
					   const sf::Uint16 tile_x, const sf::Uint16 tile_y, 
					   const boost::shared_ptr<btBoxShape> wall_shape, 
					   const boost::shared_ptr<btDiscreteDynamicsWorld> dynamics_world) {
	mTileX = tile_x;
	mTileY = tile_y;
	mWallShape = wall_shape;
	mDynamicsWorld = dynamics_world;

	if(topwall) {
		const float posx = (WIDTH / 2) * METERS_PER_PIXEL + tile_x * (WIDTH * METERS_PER_PIXEL);
		const float posy = tile_y * (HEIGHT * METERS_PER_PIXEL);
		btTransform mTopWallTransform(btQuaternion(0, 0, 0, 1), btVector3(posx, posy, 0));
		mTopWallMotionState = boost::shared_ptr<btDefaultMotionState> (new btDefaultMotionState(mTopWallTransform));
		btRigidBody::btRigidBodyConstructionInfo mTopWallRbInfo(0, mTopWallMotionState.get(), mWallShape.get(), btVector3(0, 0, 0));
		mTopWallBody = boost::shared_ptr<btRigidBody> (new btRigidBody(mTopWallRbInfo));
		mTopWallBody->setLinearFactor(btVector3(1,1,0));
		mTopWallBody->setAngularFactor(btVector3(0,0,1));

		mDynamicsWorld->addRigidBody(mTopWallBody.get(), COL_WALL, WallCollidesWith);
	}
	if(rightwall) {
		const float posx = WIDTH * METERS_PER_PIXEL + tile_x * (WIDTH * METERS_PER_PIXEL);
		const float posy = (HEIGHT / 2) * METERS_PER_PIXEL + tile_y * (HEIGHT * METERS_PER_PIXEL);
		btTransform mRightWallTransform(btQuaternion(0, 0, 1, 1), btVector3(posx, posy, 0));
		mRightWallMotionState = boost::shared_ptr<btDefaultMotionState> (new btDefaultMotionState(mRightWallTransform));
		btRigidBody::btRigidBodyConstructionInfo mRightWallRbInfo(0, mRightWallMotionState.get(), mWallShape.get(), btVector3(0, 0, 0));
		mRightWallBody = boost::shared_ptr<btRigidBody> (new btRigidBody(mRightWallRbInfo));
		mRightWallBody->setLinearFactor(btVector3(1,1,0));
		mRightWallBody->setAngularFactor(btVector3(0,0,1));

		mDynamicsWorld->addRigidBody(mRightWallBody.get(), COL_WALL, WallCollidesWith);
	}
	if(bottomwall) {
		const float posx = (WIDTH / 2) * METERS_PER_PIXEL + tile_x * (WIDTH * METERS_PER_PIXEL);
		const float posy = HEIGHT * METERS_PER_PIXEL + tile_y * (HEIGHT * METERS_PER_PIXEL);
		btTransform mBottomWallTransform(btQuaternion(0, 0, 0, 1), btVector3(posx, posy, 0));
		mBottomWallMotionState = boost::shared_ptr<btDefaultMotionState> (new btDefaultMotionState(mBottomWallTransform));
		btRigidBody::btRigidBodyConstructionInfo mBottomWallRbInfo(0, mBottomWallMotionState.get(), mWallShape.get(), btVector3(0, 0, 0));
		mBottomWallBody = boost::shared_ptr<btRigidBody> (new btRigidBody(mBottomWallRbInfo));
		mBottomWallBody->setLinearFactor(btVector3(1,1,0));
		mBottomWallBody->setAngularFactor(btVector3(0,0,1));

		mDynamicsWorld->addRigidBody(mBottomWallBody.get(), COL_WALL, WallCollidesWith);
	}
	if(leftwall) {
		const float posx = tile_x * (WIDTH * METERS_PER_PIXEL);
		const float posy = (HEIGHT / 2) * METERS_PER_PIXEL + tile_y * (HEIGHT * METERS_PER_PIXEL);
		btTransform mLeftWallTransform(btQuaternion(0, 0, 1, 1), btVector3(posx, posy, 0));
		mLeftWallMotionState = boost::shared_ptr<btDefaultMotionState> (new btDefaultMotionState(mLeftWallTransform));
		btRigidBody::btRigidBodyConstructionInfo mLeftWallRbInfo(0, mLeftWallMotionState.get(), mWallShape.get(), btVector3(0, 0, 0));
		mLeftWallBody = boost::shared_ptr<btRigidBody> (new btRigidBody(mLeftWallRbInfo));
		mLeftWallBody->setLinearFactor(btVector3(1,1,0));
		mLeftWallBody->setAngularFactor(btVector3(0,0,1));

		mDynamicsWorld->addRigidBody(mLeftWallBody.get(), COL_WALL, WallCollidesWith);
	}
}

LevelBlock::~LevelBlock() {}

void LevelBlock::Attach(LevelAttachment* attachment) {
	mLevelAttachments.push_back(attachment);
}

void LevelBlock::AttachText(const std::string& text) {
	mTextAttachments.push_back(new sf::Text(text, sf::Font::GetDefaultFont(), 50));
	mTextAttachments.back().SetOrigin(mTextAttachments.back().GetRect().Width / 2,
									  mTextAttachments.back().GetRect().Height / 2);
	mTextAttachments.back().SetPosition(WIDTH / 2 + mTileX * WIDTH,
										(HEIGHT / 2 + mTileY * HEIGHT) - 200);
}

void LevelBlock::Draw(boost::shared_ptr<sf::RenderWindow> window) {
	BOOST_FOREACH(LevelAttachment& attachment, mLevelAttachments) {
		attachment.Draw(window);
	}
	
	BOOST_FOREACH(const sf::Text& text, mTextAttachments) {
		window->Draw(text);
	}
}
