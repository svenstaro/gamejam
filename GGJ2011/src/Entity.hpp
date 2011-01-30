#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionShapes/btConvex2dShape.h>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Vector2D.hpp"
#include "EntityAttachment.hpp"

#include "GameObject.hpp"

// ARRGH Forward declaration
class World;

#define BIT(x) (1<<(x))
enum CollisionTypes {
	COL_NOTHING = 0, //<Collide with nothing
	COL_WALL = BIT(1), //<Collide with ships
	COL_BOX = BIT(2), //<Collide with walls
	COL_ENTITY = BIT(3),
	COL_MOVER = BIT(4) //<Collide with powerups
};


class Entity : public GameObject {
public:

	enum PositionType {
		POSITIONTYPE_SCREENPIXEL,
		POSITIONTYPE_SCREENFLOAT,
		POSITIONTYPE_WORLDPIXEL,
		POSITIONTYPE_WORLDFLOAT,
		POSITIONTYPE_VIEWPIXEL
	};

	Entity();
	~Entity();

	virtual void Initialize(World& world);
	virtual void Initialize(World& world, const std::string& imagefile, int layer=1, bool use_physics = false);
	virtual void Initialize(World& world, boost::property_tree::ptree* pt, std::string uid);
	virtual void InitializePhysics();

	virtual void Update(float time_delta);
	void UpdateAllAttachments(float time_delta);

	virtual void Draw(sf::RenderTarget* target, sf::Shader& shader, bool editor_mode = false) const;
	void DrawAllAttachments(sf::RenderTarget* target, sf::Shader& shader);

	void SetImage(const std::string& filename);
	const std::string& GetImage() const;

	void GenerateUID();
	void SetUID(const std::string& id);
	const std::string& GetUID() const;

	void SetPosition(const Vector2D position);
	const Vector2D GetPosition() const;

	void SetSpeed(const Vector2D speed);
	const Vector2D GetSpeed() const;

	void SetRotation(float rotation);
	float GetRotation() const;

	void SetRotationSpeed(float speed);
	float GetRotationSpeed() const;

	void SetAlpha(float alpha);
	float GetAlpha() const;

	void SetScale(float scale);
	float GetScale() const;

	int GetLayer() const;
	void SetLayer(int layer);

	int GetLocalLayer() const;
	void SetLocalLayer(int layer);

	int GetHighlightMode() const;
	void SetHighlightMode(int mode);

	boost::shared_ptr<sf::Sprite> GetDrawable() const;

	bool UsesPhysics() const;
	void TogglePhysics(World& world);
	void SetUsePhysics(World& world, bool use);

	// virtual void serialize();
    
	void Attach(Entity& entity, const Vector2D& position_offset, const float rotation_offset);

	boost::shared_ptr<btCollisionShape> GetCollisionShape();
	boost::shared_ptr<btRigidBody> GetBody();
	boost::shared_ptr<btDefaultMotionState> GetMotionState();

	bool operator<(const Entity& other);
	void Save(boost::property_tree::ptree* pt);

	static bool SortHelper(const Entity& l, const Entity& r);

	virtual bool OnCollide(GameObject* other);

	virtual std::string ToString();

protected:
	std::string mUID;
	std::string mImageFile;
	int mLayer;
	int mLocalLayer;

	Vector2D mPosition;
	Vector2D mSpeed;
	float mRotation;
	float mRotationSpeed;
	float mScale;
	float mAlpha;
	bool mUsePhysics;

	int mHighlightMode;

	boost::ptr_list<EntityAttachment> mAttachments;

	boost::shared_ptr<sf::Sprite> mDrawable;

	/*
	boost::shared_ptr<btCollisionShape> mPlayerWeightCollisionShape;
	boost::shared_ptr<btDefaultMotionState> mPlayerWeightMotionState;
	boost::shared_ptr<btRigidBody> mPlayerWeightBody;
	*/
};

#endif

