#include "EmitterInstance.h"

EmitterInstance::EmitterInstance()
{
}

EmitterInstance::~EmitterInstance()
{
}

void EmitterInstance::Update(float dt, ParticleEmitter* emitter)
{

}

void EmitterBase::Spawn(EmitterInstance* emitter, Particle* particle)
{

}

void EmitterBase::Update(float dt, ParticleEmitter* emitter)
{

}

void EmitterDestructor::Spawn(EmitterInstance* emitter, Particle* particle)
{
	particle->lifetime = 0;
}

void EmitterDestructor::Update(float dt, ParticleEmitter* emitter,float maxTime)
{

}
