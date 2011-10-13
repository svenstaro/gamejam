#ifndef PHOTON_HPP
#define PHOTON_HPP

#include <Entity.hpp>

class Photon : public Entity {
public:
    Photon(Vector2D pos = Vector2D(400,300));

    void SetTarget(Entity* entity);
    Entity* GetTarget();

    void Update(float time_diff);
    void Draw(sf::RenderTarget* target);
    
    bool TargetIsPlayer();
    void CheckOpponentCollision();

private:
    Entity* mTarget;

    sf::Sprite mSprite;
};

#endif
