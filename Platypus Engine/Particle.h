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
	Quat worldRotation; //Para el billboarding
	float3 size;
	float lifetime; //Variable % de su vida 
	float oneOverMaxLifetime; //Vida total en 1.0f/dt que queremos que dure, siendo el dt su duracion (60 dt = 1 segundo aprox?)
	float4 velocity; // (x,y,z son los vectores, normalizados, w es la intensidad de la velocidad)
	Color color;

private:

};
