#include "ParticleEmitter.h"
//#include "ModuleRenderer3D.h"
#include "Application.h"

ParticleEmitter::ParticleEmitter()
{
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::KillDeadParticles()
{
	//A�adimos en una lista todas las posiciones de particulas que queremos eliminar
	std::vector<int> particlesToDelete;

	//Buscamos en toda la lista que particulas estan muertas
	for (int i = 0; i < listParticles.size(); i++)
	{
		//Si la particula esta muerta eliminarla del vector
		if (true)
		{
			particlesToDelete.push_back(i);
		}
	}

	//Leemos de final a principio la lista de particulas para eliminarlas y que no haya problemas de cambio de tama�o
	for (size_t i = 0; i < 10; i++)
	{
		for (auto it = listParticles.rbegin(); it != listParticles.rend(); it++)
		{
			//listParticles.erase(it);
		}
	}

}

void ParticleEmitter::UpdateModules(float dt)
{
	for (int i = 0; i < modules.size() ; ++i)
	{
		modules[i]->Update(2.0f, this);
	}
}

void ParticleEmitter::Update(float dt)
{
	//emmiterTime += dt;

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