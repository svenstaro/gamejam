#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "ParticleEmitter.hpp"
#include "GameApp.hpp"

ParticleEmitter::ParticleEmitter(const Vector2D& position_offset,
								 const float rotation_offset,
								 const float speed,
								 const float spread,
								 const float time_to_live,
								 const float rate,
								 const sf::Blend::Mode mode,
								 const sf::Color& start_color,
								 const sf::Color& end_color,
								 const float start_alpha,
								 const float end_alpha,
								 const float start_scale,
								 const float end_scale,
								 const std::string& image_name) {
    mMarkedForRemoval = false;
    mType = "particleemitter";

	mPositionOffset = position_offset;
	mRotationOffset = rotation_offset;
	mSpeed = speed;
	mSpread = spread;
	mTimeToLive = time_to_live;
	mRate = rate;
    mBlendMode = mode;

	mStartColor = start_color;
	mEndColor = end_color;

	mStartAlpha = start_alpha;
	mEndAlpha = end_alpha;

	mStartScale = start_scale;
	mEndScale = end_scale;

	mImageName = image_name;
}

ParticleEmitter::~ParticleEmitter() {}

void ParticleEmitter::Initialize(const Vector2D& position_of_partsys,
								 const Vector2D& direction_of_partsys) {
	mPosition = mPositionOffset + position_of_partsys;
	mDirection = direction_of_partsys;
	mDirection.Rotate(mRotationOffset);
	mTimeSinceLastParticle = 0.f;
}

void ParticleEmitter::Update(float time_diff) {}

void ParticleEmitter::Update(float time_diff,
							 const Vector2D& position_of_partsys,
							 const Vector2D& direction_of_partsys,
                             bool emit) {
	mPosition = mPositionOffset + position_of_partsys;
	mDirection = direction_of_partsys;
	mDirection.Rotate(mRotationOffset);

	// Delete particles that have outlived their life time.
	mParticles.erase_if(boost::bind(&Particle::GetLifeTime, _1) >= mTimeToLive);

	mTimeSinceLastParticle += time_diff;
	// Rate is amount of particles sent per second and time_diff is secs.

    if(emit) {
        while(mTimeSinceLastParticle >= 1.f / mRate) {
            float spread = GameApp::Random(-mSpread/2, mSpread/2);
            Vector2D spread_vector = mDirection;
            spread_vector.Normalize();
            spread_vector.Rotate(Vector2D::deg2Rad(spread));
            Particle* particle = new Particle(mPosition, spread_vector, mSpeed, mStartColor, mEndColor, mStartAlpha, mEndAlpha, mStartScale, mEndScale, mTimeToLive, mBlendMode, mImageName);
            particle->Initialize();
            mParticles.push_back(particle);
            mTimeSinceLastParticle -= 1.f / mRate;
        }
    }

	BOOST_FOREACH(Particle& particle, mParticles)
		particle.Update(time_diff);
}

void ParticleEmitter::Draw(sf::RenderTarget* target) {
	BOOST_FOREACH(Particle& particle, mParticles)
		particle.Draw(target);
}

void ParticleEmitter::SetPositionOffset(const Vector2D& position_offset) {
	mPositionOffset = position_offset;
}

void ParticleEmitter::SetRotationOffset(const float position_offset) {
	mRotationOffset = position_offset;
}

void ParticleEmitter::SetSpeed(const float speed) {
	mSpeed = speed;
}

void ParticleEmitter::SetSpread(const float spread) {
	mSpread = spread;
}

void ParticleEmitter::SetTimeToLive(const float ttl) {
	mTimeToLive = ttl;
}

void ParticleEmitter::SetRate(const float rate) {
	mRate = rate;
}

void ParticleEmitter::SetBlendMode(const sf::Blend::Mode mode) {
	mBlendMode = mode;

}

void ParticleEmitter::SetStartColor(const sf::Color& start_color) {
	mStartColor = start_color;
}

void ParticleEmitter::SetEndColor(const sf::Color& end_color) {
	mEndColor = end_color;
}

void ParticleEmitter::SetStartAlpha(const float start_alpha) {
	mStartAlpha = start_alpha;
}

void ParticleEmitter::SetEndAlpha(const float end_alpha) {
	mEndAlpha = end_alpha;
}

void ParticleEmitter::SetStartScale(const float start_scale) {
	mStartScale = start_scale;
}

void ParticleEmitter::SetEndScale(const float end_scale) {
	mEndScale = end_scale;
}

void ParticleEmitter::SetImageName(const std::string image_name) {
	mImageName = image_name;
}

const Vector2D& ParticleEmitter::GetPositionOffset() const {
	return mPositionOffset;
}

float ParticleEmitter::GetRotationOffset() const {
	return mRotationOffset;
}

float ParticleEmitter::GetSpeed() const {
	return mSpeed;
}

float ParticleEmitter::GetSpread() const {
	return mSpread;
}

float ParticleEmitter::GetTimeToLive() const {
	return mTimeToLive;
}

float ParticleEmitter::GetRate() const {
	return mRate;
}

sf::Blend::Mode ParticleEmitter::GetBlendMode() const {
	return mBlendMode;
}

const sf::Color& ParticleEmitter::GetStartColor() const {
	return mStartColor;
}

const sf::Color& ParticleEmitter::GetEndColor() const {
	return mEndColor;
}

float ParticleEmitter::GetStartAlpha() const {
	return mStartAlpha;
}

float ParticleEmitter::GetEndAlpha() const {
	return mEndAlpha;
}

float ParticleEmitter::GetStartScale() const {
	return mStartScale;
}

float ParticleEmitter::GetEndScale() const {
	return mEndScale;
}

int ParticleEmitter::GetParticleCount() {
    return mParticles.size();
}
