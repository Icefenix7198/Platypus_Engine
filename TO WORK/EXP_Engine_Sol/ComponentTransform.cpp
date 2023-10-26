#include "ComponentTransform.h"

#include "MathGeoLib/include/Math/Quat.h"

ComponentTransform::ComponentTransform()
{
	translation.x = 0;
	translation.y = 0;
	translation.z = 0;
	pos = {translation.x,translation.y,translation.z};

	scaling.x = 1;
	scaling.y = 1;
	scaling.z = 1;
	scale = { scaling.x, scaling.y, scaling.z };

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	rotation.w = 0;
	rot = {rotation.x, rotation.y, rotation.z, rotation.w};
}

ComponentTransform::ComponentTransform(aiVector3D vecPos)
{
	translation.x = vecPos.x;
	translation.y = vecPos.y;
	translation.z = vecPos.z;
	pos = { translation.x,translation.y,translation.z };

	scaling.x = 1;
	scaling.y = 1;
	scaling.z = 1;
	scale = { scaling.x, scaling.y, scaling.z };

	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	rotation.w = 0;
	rot = { rotation.x, rotation.y, rotation.z, rotation.w };
}

ComponentTransform::ComponentTransform(aiVector3D vecPos, aiVector3D vecScale, aiQuaternion quatRot)
{
	translation = vecPos;
	pos = { translation.x,translation.y,translation.z };

	scaling = vecScale;
	scale = { scaling.x, scaling.y, scaling.z };

	rotation = quatRot;
	rot = { rotation.x, rotation.y, rotation.z, rotation.w };
}

ComponentTransform::~ComponentTransform()
{
}

Component::OnEditor(){}

