#include "EmitterInstance.h"
#include "ComponentTransform.h"
#include "Application.h"

EmitterInstance::EmitterInstance()
{
}

EmitterInstance::~EmitterInstance()
{
}

void EmitterInstance::Update(float dt, ParticleEmitter* emitter)
{

}

EmitterBase::EmitterBase()
{
	emitterOrigin = float3::zero;
	particlesLifeTime = 1.0f;
}

void EmitterBase::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	particle->oneOverMaxLifetime = 1 / particlesLifeTime;
	float4x4 matrix = emitter->owner->owner->objTransform->globalTransform;
	float3 position;
	Quat rotation;
	float3 escale;
	matrix.Decompose(position, rotation, escale);
	particle->position += position + emitterOrigin; //Se inicializan desde 0,0,0 asi que no deberia haber problema en hacer += pero deberia ser lo mismo.
	particle->worldRotation = rotation;
	particle->size = escale;
}

void EmitterBase::Update(float dt, ParticleEmitter* emitter)
{
	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		//Actualizamos el tiempo de vida de cada particula
		emitter->listParticles.at(i)->lifetime += emitter->listParticles.at(i)->oneOverMaxLifetime * dt;
	}
}

EmitterPosition::EmitterPosition()
{
	randomized = false; //Si la direccion es solo la uno o un numero random entre la 1 y la 2
	direction1 = {0,0,0};
	direction2 = {0,0,0};
	acceleration = false;
	particleSpeed1 = 1.0f;
	particleSpeed2 = 0.0f;
	
}

void EmitterPosition::Spawn(ParticleEmitter* emitter, Particle* particle)
{ 
	if (randomized)
	{
		//X
		float x1 = direction1.x;
		float x2 = direction2.x;
		//Y
		float y1 = direction1.y;
		float y2 = direction2.y;
		//Z
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

		float randomX = ((float)rand()) / (float)RAND_MAX;
		float rangeX = maxX - minX;
		float fX = (randomX * rangeX) + minX;

		float randomY = ((float)rand()) / (float)RAND_MAX;
		float rangeY = maxY - minY;
		float fY = (randomY * rangeY) + minY;

		float randomZ = ((float)rand()) / (float)RAND_MAX;
		float rangeZ = maxZ - minZ;
		float fZ = (randomZ * rangeZ) + minZ;

		float newModul = GetModuleVec({fX,fY,fZ});
		particle->velocity.x += fX / newModul;
		particle->velocity.y += fY / newModul;
		particle->velocity.z += fZ / newModul;
	}
	else
	{
		float modul1 = GetModuleVec(direction1);
		if(modul1>0)
		{
			particle->velocity.x += direction1.x / modul1;
			particle->velocity.y += direction1.y / modul1;
			particle->velocity.z += direction1.z / modul1;
		}		
	}

	particle->velocity.w += particleSpeed1;
}

void EmitterPosition::Update(float dt, ParticleEmitter* emitter)
{
	
	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		//Acceleration
		float actualLT = emitter->listParticles.at(i)->lifetime;
		emitter->listParticles.at(i)->velocity.w = particleSpeed1 + ((particleSpeed2 - particleSpeed1) * (actualLT / 1.0f));

		emitter->listParticles.at(i)->position.x += emitter->listParticles.at(i)->velocity.x * emitter->listParticles.at(i)->velocity.w * dt;
		emitter->listParticles.at(i)->position.y += emitter->listParticles.at(i)->velocity.y * emitter->listParticles.at(i)->velocity.w * dt;
		emitter->listParticles.at(i)->position.z += emitter->listParticles.at(i)->velocity.z * emitter->listParticles.at(i)->velocity.w * dt;
	}
}

float EmitterPosition::GetModuleVec(float3 vec)
{
	return sqrt( (vec.x*vec.x) + (vec.y * vec.y) + (vec.z * vec.z) );
}

EmitterSpawner::EmitterSpawner()
{
	basedTimeSpawn = false;
	spawnRatio = 0.2f; //Dividir en current time por cuantas se spawnean 
	currentTimer = 0.0f;
	numParticlesToSpawn = 5;
}

void EmitterSpawner::Spawn(ParticleEmitter* emitter, Particle* particle)
{
}

void EmitterSpawner::Update(float dt, ParticleEmitter* emitter)
{
	if(!basedTimeSpawn)
	{
		int remainingParticlesToSpawn = numParticlesToSpawn - emitter->listParticles.size();
		if (remainingParticlesToSpawn>0)
		{
			emitter->SpawnParticle(remainingParticlesToSpawn);
		}
	}
	else
	{
		currentTimer += dt;
		int numToSpawn = 0;
		if (currentTimer>spawnRatio)
		{
			numToSpawn = currentTimer / spawnRatio;
			emitter->SpawnParticle(numToSpawn);
		}
		currentTimer -= (spawnRatio * numToSpawn);
	}
}

void EmitterRotation::Spawn(ParticleEmitter* emitter, Particle* particle)
{
}

void EmitterRotation::Update(float dt, ParticleEmitter* emitter)
{
	float4x4* camaraMatrix = (float4x4*)App->camera->GetViewMatrix();
	float3 tempPos;
	Quat tempRot;
	float3 tempSca;
	camaraMatrix->Decompose(tempPos, tempRot, tempSca);

	for (int i = 0; i < emitter->listParticles.size(); i++)
	{
		emitter->listParticles.at(i)->worldRotation = tempRot;
	}
}

EmitterSize::EmitterSize()
{
	progresive = false;
	startChange = 0.0f; //Range from 0 to 1 as lifetime
	stopChange = 1.0f; //Range from 0 to 1 as lifetime
	sizeMultiplier1 = 1.0f;
	sizeMultiplier2 = 1.0f;
}

void EmitterSize::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	particle->size *= sizeMultiplier1;
}

void EmitterSize::Update(float dt, ParticleEmitter* emitter)
{
	if (progresive)
	{
		float3 base = float3::one;
		float timeForLerp = stopChange - startChange;
		for (int i = 0; i < emitter->listParticles.size(); i++)
		{
			float actualLT = emitter->listParticles.at(i)->lifetime;

			if (startChange <= actualLT && actualLT <= stopChange)
			{
				emitter->listParticles.at(i)->size = base * (sizeMultiplier1 + ((sizeMultiplier2 - sizeMultiplier1) * (actualLT / timeForLerp))); //Lerp size multiplication

			}
		}
	}
}

void EmitterColor::Spawn(ParticleEmitter* emitter, Particle* particle)
{
	particle->color = color1;
}

EmitterColor::EmitterColor()
{
	progresive = false;
	startChange = 0.0f; //Range from 0 to 1 as lifetime
	stopChange = 1.0f; //Range from 0 to 1 as lifetime
	color1 = { 0,0,0,1 };
	color2 = { 1,1,1,1 };
}

void EmitterColor::Update(float dt, ParticleEmitter* emitter)
{
	if(progresive)
	{
		float timeForLerp = stopChange - startChange;
		for (int i = 0; i < emitter->listParticles.size(); i++)
		{
			float actualLT = emitter->listParticles.at(i)->lifetime;

			if (startChange<= actualLT && actualLT  <= stopChange)
			{
				emitter->listParticles.at(i)->color.r = color1.r + ((color2.r - color1.r) * (actualLT / timeForLerp)); //Lerp red
				emitter->listParticles.at(i)->color.g = color1.g + ((color2.g - color1.g) * (actualLT / timeForLerp)); //Lerp green
				emitter->listParticles.at(i)->color.b = color1.b + ((color2.b - color1.b) * (actualLT / timeForLerp)); //Lerp blue
				emitter->listParticles.at(i)->color.a = color1.a + ((color2.a - color1.a) * (actualLT / timeForLerp)); //Lerp alpha

			}			
		}
	}
}
