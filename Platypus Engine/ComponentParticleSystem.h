#pragma once
#ifndef __COMPONENT_PARTICLE_H__
#define __COMPONENT_PARTICLE_H__

#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "Assimp/include/quaternion.h"
#include "Assimp/include/vector3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float4x4.h"

#include "ParticleEmitter.h"

#include <vector>
#include <string>

class ParticleEmitter;

class ComponentParticleSystem : public Component
{
public:

	bool active;

	ComponentParticleSystem();
	ComponentParticleSystem(GameObject* own);

	~ComponentParticleSystem();

	//Vector de Emmiters, los emmiters son quienes crean las particulas
	ParticleEmitter* CreateEmitter();

	std::vector<ParticleEmitter*> allEmitters;
	
	//ImGui
	void OnEditor();

private:
};

#endif //!__COMPONENT_PARTICLE_H__
