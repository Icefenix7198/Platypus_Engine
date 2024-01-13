#include "EmitterInstance.h"
#include "ComponentTransform.h"

EmitterInstance::EmitterInstance()
{
}

EmitterInstance::~EmitterInstance()
{
}

void EmitterInstance::Update(float dt, ParticleEmitter* emitter)
{

}

void EmitterBase::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	float3 position = emitter->owner->owner->objTransform->pos;
	particle->position += position + emitterOrigin; //Se inicializan desde 0,0,0 asi que no deberia haber problema en hacer += pero deberia ser lo mismo.
}

void EmitterBase::Update(float dt, ParticleEmitter* emitter)
{
	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		//Actualizamos el tiempo de vida de cada particula
		emitter->listParticles.at(i).lifetime += emitter->listParticles.at(i).oneOverMaxLifetime * dt;
		emitter->listParticles.at(i).position;
	}
}

void EmitterDestructor::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	particle->lifetime = 0.0f;
}

void EmitterDestructor::Update(float dt, ParticleEmitter* emitter)
{

}

void EmitterSpawner::Spawn(ParticleEmitter* emitter, Particle* particle)
{
}

void EmitterSpawner::Update(float dt, ParticleEmitter* emitter)
{
}
