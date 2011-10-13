#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/shared_ptr.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "RestraintSettings.hpp"
#include "Vector2D.hpp"

// OMG FORWARD DECLARATION
class World;
class EntityAttachment;
class Entity {
public:
    Entity();
    virtual ~Entity() = 0; 

    virtual void Update(float time_diff) = 0;
    virtual void Draw(sf::RenderTarget* target) = 0;

    void UpdateAllAttachments(float time_diff);
    void DrawAllAttachments(sf::RenderTarget* target);

    void Attach(boost::shared_ptr<Entity> entity,
                Vector2D position_offset,
                float rotation_offset,
                RestraintSettings restraint_settings = RestraintSettings());

    void DetachAndDrop(Entity* entity);

    Vector2D GetPosition();
    void SetPosition(Vector2D position);

    float GetRotation();
    void SetRotation(float rotation);

    float GetScale();
    void SetScale(float scale);

    float GetSpeed();
    void SetSpeed(float speed);

    Vector2D GetDirection();
    void SetDirection(Vector2D direction);

    const std::string& GetType();

    const std::string& GetName();
    void SetName(const std::string& name);

    float GetLifeTime();

    virtual sf::Color GetPhotonColor();

    void MarkForRemoval();
    bool IsMarkedForRemoval();

protected:
    Vector2D mPosition; // Position as integer pixel coordinates
    float mRotation; // Rotation in degrees
    float mScale; // Scale where 1.f is default scale
    float mSpeed;
    Vector2D mDirection;
    std::string mType; // Type of the entity
    std::string mName; // Optional name of entity
    sf::Clock mLifeTime; // Seconds the entity has been alive

    bool mMarkedForRemoval;

    boost::ptr_list<EntityAttachment> mAttachments;
};

#endif
