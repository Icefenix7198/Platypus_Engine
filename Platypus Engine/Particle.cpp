#include "Particle.h"

Particle::Particle()
{
	position = { 0,0,0 };
	worldRotation = {0,0,0,0};
	lifetime = 1.0f;
	velocity = {0,0,0,0};
	color = {0,0,0,1};
}

Particle::~Particle()
{
}
