#pragma once
#ifndef __EMITTER_H__
#define __EMITTER_H__

#include <vector>
#include "Particle.h"
#include "ParticleModule.h"

class R_Material;

class Emitter
{
public:
	//Any override constructor we might need?
	Emitter();

	//Loop through all modules, loop through particles, update them
	void Update(float dt);

	//void SaveAsset(Config& config) const;
	//void SaveResource(char* buffer);

	//void Load(Config& config);
	bool AddModuleFromType(ParticleModule::particleType type);

public:
	std::string name = "Particle Emitter";
	std::vector<ParticleModule*> modules;

	int maxParticleCount = 100;
};

#endif // !__EMITTER_H__