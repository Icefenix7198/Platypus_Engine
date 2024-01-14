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
		emitter->listParticles.at(i)->lifetime += emitter->listParticles.at(i)->oneOverMaxLifetime * dt;
		emitter->listParticles.at(i)->position;
	}
}

void EmitterPosition::Spawn(ParticleEmitter* emitter, Particle* particle)
{ 
	if (randomized)
	{
		float x1 = direction1.x;
		float x2 = direction2.x;
		float y1 = direction1.y;
		float y2 = direction2.y;
		float z1 = direction1.z;
		float z2 = direction2.z;

		float maxX;
		float minX;
		if(x1 > x2)
		{
			maxX = x1;
			minX = x2;
		}
		else
		{
			maxX = x2;
			minX = x1;
		}

		float maxY;
		float minY;
		if (y1 > y2)
		{
			maxY = y1;
			minY = y2;
		}
		else
		{
			maxY = y2;
			minY = y1;
		}

		float maxZ;
		float minZ;
		if (z1 > z2)
		{
			maxZ = z1;
			minZ = z2;
		}
		else
		{
			maxZ = z2;
			minZ = z1;
		}

		float random = ((float)rand()) / (float)RAND_MAX;
		float rangeX = maxX - minX;
		float fX = (random * rangeX) + minX;
		float rangeY = maxY - minY;
		float fY = (random * rangeY) + minY;
		float rangeZ = maxZ - minZ;
		float fZ = (random * rangeZ) + minZ;

		float newModul = GetModuleVec({fX,fY,fZ});
		particle->velocity.x += fX / newModul;
		particle->velocity.y += fY / newModul;
		particle->velocity.z += fZ / newModul;
	}
	else
	{
		float modul1 = GetModuleVec(direction1);
		particle->velocity.x += direction1.x / modul1;
		particle->velocity.y += direction1.x / modul1;
		particle->velocity.z += direction1.x / modul1;
		
	}

	particle->velocity.w += particleSpeed;
}

void EmitterPosition::Update(float dt, ParticleEmitter* emitter)
{
	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
	}
}

float EmitterPosition::GetModuleVec(float3 vec)
{
	return sqrt( (vec.x*vec.x) + (vec.y * vec.y) + (vec.z * vec.z) );
}

void EmitterSpawner::Spawn(ParticleEmitter* emitter, Particle* particle)
{
}

void EmitterSpawner::Update(float dt, ParticleEmitter* emitter)
{
	int remainingParticlesToSpawn = numParticlesToSpawn - emitter->listParticles.size();
	if (remainingParticlesToSpawn>0)
	{
		emitter->SpawnParticle(remainingParticlesToSpawn);
	}
	
}
