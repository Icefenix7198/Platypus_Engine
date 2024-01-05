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


#include <vector>
#include <string>


class ComponentParticleSystem : public Component
{
public:

	bool active;

	ComponentParticleSystem();
	ComponentParticleSystem(GameObject* own);

	~ComponentParticleSystem();

	//Matrix Values
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	void SetValues(aiVector3D translation, aiVector3D scaling, aiQuaternion rotation);
	
	float3 pos;
	float3 scale;
	math::Quat rot;
	float3 angles; //Only for display

	float4x4 localTransform;
	float4x4 globalTransform;
	
	//ImGui
	void OnEditor();

private:
};

#endif //!__COMPONENT_PARTICLE_H__
