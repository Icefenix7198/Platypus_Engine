#pragma once

#include "ComponentParticleSystem.h"
#include "ParticleModule.h"
#include <vector>

#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__ 

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

//std::vector<Particle>
//Emitter *
	ComponentParticleSystem* owner;

	void Init();
	void UpdateModules();
	void DrawParticles();

	std::vector<ParticleModule> vecModules;

private:

};
#endif //__PARTICLE_EMITTER_H__