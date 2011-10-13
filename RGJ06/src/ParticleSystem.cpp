#include <boost/foreach.hpp>

#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(const Vector2D& position,
							   const Vector2D& direction) {
    mMarkedForRemoval = false;
    mStopEmitting = false;
    mType = "particlesystem";
	mPosition = position;
	mDirection = direction;
}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::Initialize() {}

void ParticleSystem::Update(float time_diff) {
	BOOST_FOREACH(ParticleEmitter& emitter, mParticleEmitters) {
		emitter.Update(time_diff, mPosition, mDirection, !mStopEmitting);
	}
}

void ParticleSystem::Draw(sf::RenderTarget* target) {
	BOOST_FOREACH(ParticleEmitter& emitter, mParticleEmitters) {
		emitter.Draw(target);
	}
}

void ParticleSystem::AddEmitter(ParticleEmitter* emitter) {
	emitter->Initialize(mPosition, mDirection);
	mParticleEmitters.push_back(emitter);
}

void ParticleSystem::SetPosition(const Vector2D& position) {
	mPosition = position;
}

void ParticleSystem::SetDirection(const Vector2D& direction) {
	mDirection = direction;
}
    
void ParticleSystem::StopEmitting() {
    mStopEmitting = true;
}

bool ParticleSystem::IsEmitting() {
    return mStopEmitting;
}

int ParticleSystem::GetParticleCount() {
    int c = 0;
	BOOST_FOREACH(ParticleEmitter& emitter, mParticleEmitters) {
        c += emitter.GetParticleCount();
    }
    return c;
}
