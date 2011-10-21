#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Vector2D.hpp"

class Background: public Entity {
public:
    Background(const std::string& type, float speed);
    ~Background();

    void Update(float time_diff);
    void Draw(sf::RenderTarget* target);

private:
    sf::Sprite mSprite1;
    sf::Sprite mSprite2;
    sf::Sprite mSprite3;
    float mSpeed;
};

#endif
