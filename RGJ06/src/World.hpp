#ifndef WORLD_HPP
#define WORLD_HPP

#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

#include "Entity.hpp"
#include "Player.hpp"
#include "Blob.hpp"
#include "Photon.hpp"
#include "ParticleSystem.hpp"

class World {
public:
    World();
    ~World();

    void Initialize();
    void Reset();
    void HandleEvent(const sf::Event& event);
    void Update(float time_diff);
    void Draw(sf::RenderTarget* target);

    void AddEntity(Entity* e);

    template <typename T>
    std::vector<T*> GetEntitiesByType() {
        std::vector<T*> entities_by_type;

        T t;

        BOOST_FOREACH(Entity& entity, mEntities) {
            if(entity.GetType() == t.GetType()) {
                entities_by_type.push_back((T*)&entity);
            }
        }

        return entities_by_type;
    }

    template <typename T>
    T* GetEntityByName(const std::string& name) {
        T* entity_by_name;

        T t;

        bool found = false;
        BOOST_FOREACH(Entity& entity, mEntities) {
            if((entity.GetType() == t.GetType()) && (entity.GetName() == name)) {
                entity_by_name = (T*)&entity;
                found = true;
            }
        }

        if(!found) {
            entity_by_name = NULL;
            std::cout << "WARNING: GetEntityByName returns nullptr as it didn't find a matching Entity!" << std::endl;
        }

        return entity_by_name;
    }

    Player* GetPlayer();
    Photon* GetPhotonWithTarget(Entity* target);
    bool PointWithinPlayerRange(Vector2D point, int radius = 0);

    void AddPhoton(Entity* target);

    float GetEnemyInterval();
    int GetLevel();

    int GetEnergyOfEntity(Entity* e);
private:
    boost::ptr_vector<Entity> mEntities;
    Blob* mCurrentBlob;
    Player* mPlayer;
    float mScore;
    int mLevel;
    bool mDebug;
    float mTimeSinceLastEnemy;
    float mTimeSinceLastPhoton;
    float mTimeSinceLastPhotonTransfered;
};

#endif
