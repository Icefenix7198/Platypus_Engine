#pragma once

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "Color.h"

class Particle
{
public:
	Particle();
	~Particle();

	float3 position;
	Quat worldRotation;
	float lifetime;
	float4 velocity;
	Color color;

private:

};
