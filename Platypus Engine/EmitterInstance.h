#pragma once
#ifndef __EMITTER_INSTANCE_H__
#define __EMITTER_INSTANCE_H__ 

#include "ComponentParticleSystem.h"
#include "Particle.h"
//#include "ParticleEmitter.h"
#include <vector>

enum EmiterType
{
	BASE,
	SPAWN,
	POSITION,
	MAX,
};

class ParticleEmitter;

//Plantillas para los Emitters, que seran los settings de las particulas
struct EmitterInstance //En Thor Engine esto seria el particle Module
{
public:
	EmitterInstance();
	~EmitterInstance();

	EmiterType type;

	virtual void Spawn(ParticleEmitter* emitter, Particle* particle) = 0;
	virtual void Update(float dt, ParticleEmitter* emitter) /*= 0*/;
	
	bool unique = true; //Si es unique solo puede haber uno de ese tipo
private:

};

struct EmitterBase : EmitterInstance
{
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);

	//Variable unica, posicion donde spawnean
	float3 emitterOrigin = float3::zero;
};

struct EmitterSpawner : EmitterInstance
{
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);

	//Variable unica, ritmo de spawn
	float spawnRatio = 0.1f; //Dividir en current time por cuantas se spawnean (?)
	float currentTimer = 0.0f;
	int numParticlesToSpawn = 1;
};

struct EmitterPosition : EmitterInstance
{
	void Spawn(ParticleEmitter* emitter, Particle* particle);
	void Update(float dt, ParticleEmitter* emitter);

	float GetModuleVec(float3 vec);

	bool randomized = false; //Si la direccion es solo la uno o un numero random entre la 1 y la 2
	float3 direction1 = {0,0,0};
	float3 direction2 = { 0,0,0 };
	float particleSpeed = 1.0f;
};


#endif //__EMITTER_INSTANCE_H__