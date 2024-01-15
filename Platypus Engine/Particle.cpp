#include "Particle.h"

Particle::Particle()
{
	position = { 0,0,0 };
	worldRotation = {0,0,0,0};
	size = { 1,1,1 };
	lifetime = 0.0f;
	oneOverMaxLifetime = 1.0f / 60;
	velocity = {0,0,0,0};
	color = {1,0,0,1};
}

Particle::~Particle()
{
}
