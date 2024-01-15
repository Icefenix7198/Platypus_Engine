#include "ParticleEmitter.h"
#include "EmitterInstance.h"
//#include "ModuleRenderer3D.h"
#include "Application.h"

ParticleEmitter::ParticleEmitter()
{
	//Quiza haya que meterle alguna info? IDK
	
}

ParticleEmitter::~ParticleEmitter()
{
}

EmitterInstance* ParticleEmitter::CreateEmitterByType(uint type)
{
	//MOST emittter instances cannot be made twice so have an unique value
	for (int i = 0; i < modules.size(); i++)
	{
		if(modules.at(i)->unique && modules.at(i)->type==type)
		{
			return nullptr;
		}
	}

	EmitterInstance* nuevoEmitter = nullptr;
	switch ((EmiterType)type)
	{
	case BASE:
	{
		nuevoEmitter = new EmitterBase;
		nuevoEmitter->type = EmiterType::BASE;
		nuevoEmitter->unique = true;

		break;
	}
	case SPAWN:
	{	
		nuevoEmitter = new EmitterSpawner;
		nuevoEmitter->type = EmiterType::SPAWN;
		nuevoEmitter->unique = true;
	
		break;
	}
	case POSITION:
	{	
		nuevoEmitter = new EmitterPosition;
		nuevoEmitter->type = EmiterType::POSITION;
		nuevoEmitter->unique = true;

		break;
	}
	case ROTATION:
	{
		nuevoEmitter = new EmitterRotation;
		nuevoEmitter->type = EmiterType::ROTATION;
		nuevoEmitter->unique = true;

		break;
	}
	case SIZEPARTICLE:
	{
		nuevoEmitter = new EmitterSize;
		nuevoEmitter->type = EmiterType::SIZEPARTICLE;
		nuevoEmitter->unique = true;

		break;
	}
	case COLOR:
	{
		nuevoEmitter = new EmitterColor;
		nuevoEmitter->type = EmiterType::COLOR;
		nuevoEmitter->unique = true;

		break;
	}
	case MAX:
		break;
	default:
		break;
	}

	//
	modules.push_back(nuevoEmitter);

	return nuevoEmitter;
}

void ParticleEmitter::KillDeadParticles()
{
	//Añadimos en una lista todas las posiciones de particulas que queremos eliminar
	std::vector<int> particlesToDelete;

	//Buscamos en toda la lista que particulas estan muertas
	for (int i = 0; i < listParticles.size(); i++)
	{
		//Si la particula esta muerta eliminarla del vector
		if (listParticles.at(i)->lifetime >= 1.0f)
		{
			particlesToDelete.push_back(i);
		}
	}

	//Leemos de final a principio la lista de particulas para eliminarlas y que no haya problemas de cambio de tamaño
	for (int j = particlesToDelete.size()-1; j >= 0; --j)
	{
		listParticles.erase(listParticles.begin()+ particlesToDelete.at(j));
	}
}

void ParticleEmitter::KillAllParticles()
{
	listParticles.clear();
}

void ParticleEmitter::UpdateModules(float dt)
{
	for (int i = 0; i < modules.size() ; ++i)
	{
		modules[i]->Update(dt, this);
	}
}

void ParticleEmitter::Init(ComponentParticleSystem* component)
{
	//Si el numero de particles es fijo y lo que hay pues
	this->owner = component;
	//listParticles.resize(MAXPARTICLES);
	//array de indices de particulas (int*)
	//particleIndices = new unsigned int[MAXPARTICLES]
}

void ParticleEmitter::Update(float dt)
{
	emitterTime += dt;

	//Eliminar las particulas de la lista que ya acabaron su tiempo de vida
	KillDeadParticles();

	//Actualizamos modulos que puedan cambiar con el tiempo (cambiar las particulas, moverlas o lo que haga falta)
	UpdateModules(dt);

	//Llamamos a Draw particles para que printe todas las particulas con su info updateada
	DrawParticles();
}

void ParticleEmitter::DrawParticles()
{
	for (int i = 0; i < listParticles.size(); i++)
	{
		auto par = listParticles.at(i);
		App->renderer3D->DrawParticles(par);
		//Draw particles a traves de moduleRenderer
	}
}

void ParticleEmitter::Reset()
{
	emitterTime = 0;

	KillAllParticles();
}

void ParticleEmitter::SpawnParticle(uint particlesToAdd)
{
	if (listParticles.size()<MAXPARTICLES)
	{
		for (int i = 0; i < particlesToAdd; i++)
		{
			Particle* particula = new Particle();
			for (int i = 0; i < modules.size(); i++)
			{
				modules.at(i)->Spawn(this,particula);
			}
			listParticles.push_back(particula);
		}
	}
}
