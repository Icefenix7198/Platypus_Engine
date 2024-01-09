#pragma once
#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__ 

#include "ComponentParticleSystem.h"
#include "Particle.h"
#include <vector>

//Ellos generan y controlan las particulas
class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	std::vector<Particle> listParticles;
	ComponentParticleSystem* owner;

	void Init(ComponentParticleSystem* component);

	void Update(float dt);
	void DrawParticles();
	void Reset();

	void SpawnParticle();

private:
	void KillDeadParticles();
	void KillAllParticles();
	void UpdateModules(float dt);

};
#endif //__PARTICLE_EMITTER_H__