#ifndef PARTICLE_HPP 
#define PARTICLE_HPP

#include <string>

#include "Entity.hpp"
#include "Vector2D.hpp"

class Particle : public Entity {
public:
	Particle();
	Particle(const Vector2D& position,
			 const Vector2D& direction,
			 const float speed,
			 const sf::Color& start_color,
			 const sf::Color& end_color,
			 const float start_alpha,
			 const float end_alpha,
			 const float start_scale,
			 const float end_scale,
			 const float time_to_live,
			 const sf::Blend::Mode mode,
			 const std::string& image_name);
	
	~Particle();

	void Initialize();

	void Update(float time_diff);

	void Draw(sf::RenderTarget* target);

private:
	sf::Sprite mSprite;

	sf::Color mColor;
	sf::Color mStartColor;
	sf::Color mEndColor;
	float mAlpha;			// 100% = visible
	float mStartAlpha;
	float mEndAlpha;
	float mScale;
	float mStartScale;
	float mEndScale;

	sf::Blend::Mode mBlendMode;

	float mTimeToLive;
	std::string mImageName;
};

#endif
