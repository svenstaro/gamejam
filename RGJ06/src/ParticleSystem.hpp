#ifndef PARTICLESYSTEM_HPP 
#define PARTICLESYSTEM_HPP

#include <boost/ptr_container/ptr_list.hpp>

#include "Entity.hpp"
#include "ParticleEmitter.hpp"

class ParticleSystem : public Entity {
public:
	ParticleSystem(const Vector2D& position = Vector2D(0.f, 0.f),
				   const Vector2D& direction = Vector2D(0.f, 0.f));
	
	~ParticleSystem();
	
	void Initialize();

    void Update(float time_diff);

	void Draw(sf::RenderTarget* target);

	void AddEmitter(ParticleEmitter* emitter);

	void SetPosition(const Vector2D& position);
	void SetDirection(const Vector2D& direction);

    void StopEmitting();
    bool IsEmitting();

    int GetParticleCount();
private:
	boost::ptr_list<ParticleEmitter> mParticleEmitters;
    bool mStopEmitting;
};

#endif
