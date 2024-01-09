#pragma once

#include "ComponentParticleSystem.h"
#include <vector>

#ifndef __PARTICLE_MODULE_H__
#define __PARTICLE_MODULE_H__ 

class ParticleModule
{
public:
	ParticleModule();
	~ParticleModule();

enum particleType
{
	MAX,
};

void Spawn(/*EmitterInstance**/);
void Update(/*EmitterInstance**/);
void Save();
void Load();

private:

};
#endif //__PARTICLE_MODULE_H__