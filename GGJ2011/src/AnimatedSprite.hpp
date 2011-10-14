#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Resource.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>

using namespace sf;

class AnimatedSprite : public sf::Sprite {
public:
	AnimatedSprite();
	explicit AnimatedSprite(const Texture& Texture,
						 const Vector2f& Position = Vector2f(0, 0),
						 const Vector2f& Scale = Vector2f(1, 1),
						 float Rotation = 0.f,
						 const Color& Col = Color(255, 255, 255, 255));

	void SetFPS(float fps);
	float GetFPS() const;

	void SetNumFrames(sf::Uint16 num_frames);
	sf::Uint16 GetNumFrames() const;

	void SetSubRectSize(sf::Vector2f sub_rect_size);
	const sf::Vector2f GetSubRectSize() const;

	void SetSubRectOffset(sf::Vector2f sub_rect_offset);
	const sf::Vector2f GetSubRectOffset() const;

	void Update(const float time_delta);

protected:
	void Render(RenderTarget&, Renderer& renderer) const;

private:
	float mFPS, mTimeSinceLastFrame;
	sf::Uint16 mNumFrames, mFrameNumber;
	sf::Vector2f mSubRectSize;
	sf::Vector2f mSubRectOffset;
};

#endif // ANIMATEDSPRITE_HPP
