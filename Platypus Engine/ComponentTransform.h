#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Globals.h"
#include "GameObject.h"
#include "Component.h"
#include "MathGeoLib/include/Math/float3.h"
#include "aasimp.h"
#include "Assimp/include/quaternion.h"
#include "Assimp/include/vector3.h"
#include "MathGeoLib/include/Math/Quat.h"

#include <vector>
#include <string>


class ComponentTransform : public Component
{
public:

	bool active;

	ComponentTransform();
	ComponentTransform(GameObject* own);
	ComponentTransform(aiVector3D vecPos); //Only position
	ComponentTransform(aiVector3D vecPos, aiVector3D vecScale, aiQuaternion quatRot); //All parameters

	~ComponentTransform();

	//Matrix Values
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	void SetValues(aiVector3D translation, aiVector3D scaling, aiQuaternion rotation);
	
	float3 pos;
	float3 scale;
	math::Quat rot;

	float4x4 localTransform;
	float4x4 globalTransform;
	
	//Sets the local transform in function of the values of the Component
	void GenerateLocalMatrix();
	float4x4 GenerateMatrix(float3 translation, float3 scaling, Quat rotation);
	
	//ImGui
	void OnEditor();

private:

};

#endif //!__COMPONENT_TRANSFORM_H__
