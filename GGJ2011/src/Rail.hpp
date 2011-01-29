#ifndef RAIL_HPP
#define RAIL_HPP

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include "Vector2D.hpp"
#include "SFML/Graphics.hpp"
#include "Coordinates.hpp"

#include "TiledSprite.hpp"
#include "GameObject.hpp"

#include "Mover.hpp"

class Rail : public GameObject {
public:
	Rail();

	virtual void Reinitialize(World& world);
	virtual void Deinitialize(World& world);

	virtual void Initialize(World& world);
	virtual void InitializePhysics();

	virtual void Update(float time_delta);
	virtual void Draw(sf::RenderTarget* target, sf::Shader& shader, bool editor_mode = false) const;

	void SetNextPoint(Vector2D point);

	void Save(boost::property_tree::ptree* pt, int id);
	void Load(boost::property_tree::ptree* pt, int id);

	Vector2D GetCenter();
	float GetRotation();

	bool IsFinished() const;

	float ClosestPositionOnLine(Vector2D pixel_pos);
	Vector2D GetPointFromFloat(float f) const;

	btRigidBody* GetRigidBody();
	btTypedConstraint* GetConstraint();

	virtual bool OnCollide(GameObject* other);

	bool IsCurrentRail() const;

	std::string ToString();

	void ToggleInitialState();

private:
	Vector2D mPoint1, mPoint2;
	float mStartPosition;
	int mLastPointSet;
	TiledSprite mTiledSprite;

	boost::shared_ptr<btGeneric6DofConstraint> mConstraint;
	Mover mMover;

	bool mInitialMoverMounted;

	bool mMounted;
};

#endif // RAIL_HPP

