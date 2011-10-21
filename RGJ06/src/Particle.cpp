#include "GameApp.hpp"
#include "Particle.hpp"

Particle::Particle() {}

Particle::Particle(const Vector2D& position,
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
				   const std::string& image_name) {
    mType = "particle";
    mMarkedForRemoval = false;

	mPosition = position;
	mDirection = direction;
	mSpeed = speed;

	mColor = start_color;
	mStartColor = start_color;
	mEndColor = end_color;

	mAlpha = start_alpha;
	mStartAlpha = start_alpha;
	mEndAlpha = end_alpha;

	mScale = start_scale;
	mStartScale = start_scale;
	mEndScale = end_scale;

	mTimeToLive = time_to_live;
	mBlendMode = mode;

	mImageName = image_name;
}

Particle::~Particle() {}

void Particle::Initialize() {
	mSprite.SetTexture(GameApp::get_mutable_instance().GetResourceManagerPtr()->GetTexture(mImageName));
	mSprite.SetOrigin(mSprite.GetSize().x / 2, mSprite.GetSize().y / 2);
	mSprite.SetBlendMode(mBlendMode);
}

void Particle::Update(float time_diff) {
	if(mTimeToLive >= 0) {
		mPosition += mDirection * mSpeed * time_diff;

		float f = GetLifeTime() / mTimeToLive;
		// fix last frame, when lifetime > time to live
		if (f > 1)
			f = 1.f;

		mScale = (mEndScale - mStartScale) * f + mStartScale;

		int r = (mEndColor.r - mStartColor.r) * f + mStartColor.r;
		int g = (mEndColor.g - mStartColor.g) * f + mStartColor.g;
		int b = (mEndColor.b - mStartColor.b) * f + mStartColor.b;
		int a = (mEndAlpha - mStartAlpha) * f + mStartAlpha;

		mColor = sf::Color(r, g, b, a);

		mSprite.SetScale(mScale, mScale);
		mSprite.SetColor(mColor);
	}
    mSprite.SetPosition(mPosition.x, mPosition.y);
    //std::cout << mPosition << std::endl;
}

void Particle::Draw(sf::RenderTarget* target) {
	target->Draw(mSprite);
}
