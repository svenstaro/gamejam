#ifndef LEVELATTACHMENT_HPP
#define LEVELATTACHMENT_HPP

#include <SFML/Graphics.hpp>

#include <bullet/btBulletDynamicsCommon.h>

#include <boost/shared_ptr.hpp>

#include "Definitions.hpp"

class LevelAttachment {
public:
	LevelAttachment(const sf::Vector2i pos, const sf::Vector2i size,
					boost::shared_ptr<btDiscreteDynamicsWorld> dynamics_world);
	~LevelAttachment();

	void Draw(boost::shared_ptr<sf::RenderWindow> window);

private:
	sf::Vector2f mPos;
	sf::Vector2f mSize;

	sf::Sprite mSprite;

	boost::shared_ptr<btBoxShape> mBodyShape;
	boost::shared_ptr<btDiscreteDynamicsWorld> mDynamicsWorld;
	boost::shared_ptr<btDefaultMotionState> mBodyMotionState;
	boost::shared_ptr<btRigidBody> mBody;
};

#endif
