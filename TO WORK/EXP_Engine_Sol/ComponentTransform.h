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
	ComponentTransform();
	ComponentTransform(aiVector3D vecPos); //Only position
	ComponentTransform(aiVector3D vecPos, aiVector3D vecScale, aiQuaternion quatRot); //All parameters


	~ComponentTransform();

	virtual void Component::OnEditor()

	aiVector3D translation, scaling;
	aiQuaternion rotation;
	
	//node->mTransformation.Decompose(scaling, rotation, translation); //Esto es conseguir es scenes creo?
	
	float3 pos;
	float3 scale;
	math::Quat rot;

private:

};

#endif //!__COMPONENT_TRANSFORM_H__
