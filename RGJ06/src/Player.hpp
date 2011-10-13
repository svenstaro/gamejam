#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Player : public Entity {
public:
    Player();
    ~Player();

    void Update(float time_diff);
    void Draw(sf::RenderTarget* target);

    float GetRadius();
    sf::Color GetPhotonColor();
private:
    sf::Sprite mSprite;
};

#endif
