#ifndef PARTICLEEMITTER_HPP 
#define PARTICLEEMITTER_HPP

#include <boost/ptr_container/ptr_list.hpp>

#include "Entity.hpp"
#include "Particle.hpp"

class ParticleEmitter : public Entity {
public:
	ParticleEmitter(const Vector2D& position_offset = Vector2D(0.f, 0.f),
					const float rotation_offset = 0.f,
					const float speed = 10.f,
					const float spread = 90.f,
					const float time_to_live = 3.f,
					const float rate = 10.f,
					const sf::Blend::Mode = sf::Blend::Add,
					const sf::Color& start_color = sf::Color(255, 255, 255, 255),
					const sf::Color& end_color = sf::Color(170, 200, 255, 255),
					const float start_alpha = 255,
					const float end_alpha = 0,
					const float start_scale = 1.0f,
					const float end_scale = 5.0f,
					const std::string& image_name = "particle");
	
	~ParticleEmitter();
	
	void Initialize(const Vector2D& position_of_partsys,
					const Vector2D& direction_of_partsys);

    void Update(float time_diff);

    void Update(float time_diff,
				const Vector2D& position_of_partsys,
				const Vector2D& direction_of_partsys,
                bool emit = true);

	void Draw(sf::RenderTarget* target);

	void SetPositionOffset(const Vector2D& position_offset);
	void SetRotationOffset(const float position_offset);
	void SetSpeed(const float speed);
	void SetSpread(const float spread);
	void SetTimeToLive(const float ttl);
	void SetRate(const float rate);
	void SetBlendMode(const sf::Blend::Mode mode);
	void SetStartColor(const sf::Color& start_color);
	void SetEndColor(const sf::Color& end_color);
	void SetStartAlpha(const float start_alpha);
	void SetEndAlpha(const float end_alpha);
	void SetStartScale(const float start_scale);
	void SetEndScale(const float end_scale);
	void SetImageName(const std::string image_name);

	const Vector2D& GetPositionOffset() const;
	float GetRotationOffset() const;
	float GetSpeed() const;
	float GetSpread() const;
	float GetTimeToLive() const;
	float GetRate() const;
	sf::Blend::Mode GetBlendMode() const;
	const sf::Color& GetStartColor() const;
	const sf::Color& GetEndColor() const;
	float GetStartAlpha() const;
	float GetEndAlpha() const;
	float GetStartScale() const;
	float GetEndScale() const;

    int GetParticleCount();

private:
	Vector2D mPositionOffset;
	float mRotationOffset;
	float mSpread;
	float mTimeToLive;
	float mRate;
	sf::Blend::Mode mBlendMode;
	sf::Color mStartColor;
	sf::Color mEndColor;
	float mStartAlpha;
	float mEndAlpha;
	float mStartScale;
	float mEndScale;

	float mTimeSinceLastParticle;
	boost::ptr_list<Particle> mParticles;

	std::string mImageName;
};

#endif
