#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Enemy : public Entity {
public:
    Enemy();
    ~Enemy();

    void GeneratePhotons();

    void Update(float time_diff);
    void Draw(sf::RenderTarget* target);

    sf::Color GetPhotonColor();

private:
    sf::Sprite mSprite;
};

#endif
