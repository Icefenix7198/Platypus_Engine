#pragma once
#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__ 

#include "ComponentParticleSystem.h"
#include "EmitterInstance.h"
#include "Particle.h"
#include <vector>

#define MAXPARTICLES 200

class ComponentParticleSystem;
class EmitterInstance;

//Ellos generan y controlan las particulas
class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	std::vector<Particle> listParticles;
	std::vector<EmitterInstance*> modules; //Son las Particle module del power, son las settings
	ComponentParticleSystem* owner;

	float emitterTime = 0; //Tiempo de las particulas

	void Init(ComponentParticleSystem* component);

	void Update(float dt);
	void DrawParticles();
	void Reset();

	void SpawnParticle(uint particlesToAdd = 1);

	EmitterInstance* CreateEmitterByType(uint type);

private:
	void KillDeadParticles();
	void KillAllParticles();
	void UpdateModules(float dt);

};
#endif //__PARTICLE_EMITTER_H__