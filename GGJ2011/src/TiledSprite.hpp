#ifndef TILEDSPRITE_HPP
#define TILEDSPRITE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Resource.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>

using namespace sf;

class TiledSprite : public sf::Sprite {
public:
	TiledSprite();
    explicit TiledSprite(const Image& Img,
                         const Vector2f& Position = Vector2f(0, 0),
                         const Vector2f& Scale = Vector2f(1, 1),
                         const float Rotation = 0.f,
						 const Color& Col = Color(255, 255, 255, 255));
protected:
	void Render(RenderTarget&, Renderer& renderer) const;
};

#endif
