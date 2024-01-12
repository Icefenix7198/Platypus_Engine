#pragma once
#ifndef __EMITTER_INSTANCE_H__
#define __EMITTER_INSTANCE_H__ 

#include "ComponentParticleSystem.h"
#include "Particle.h"
//#include "ParticleEmitter.h"
#include <vector>

enum EmiterType
{
	SPAWN,
	MAX,
};

class ParticleEmitter;

//Plantillas para los Emitters, que seran los settings de las particulas
class EmitterInstance //En Thor Engine esto seria el particle Module
{
public:
	EmitterInstance();
	~EmitterInstance();

	EmiterType type;

	virtual void Spawn(EmitterInstance* emitter, Particle* particle) = 0;
	virtual void Update(float dt, ParticleEmitter* emitter) /*= 0*/;

private:

};
#endif //__EMITTER_INSTANCE_H__