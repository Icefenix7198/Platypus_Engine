#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "MathGeoLib\MathGeoLib.h"

#include <list>

class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetLinearVelocity(float x, float y, float z);
	void SetAngularVelocity(float x, float y, float z);
	math::vec GetLinearVelocity() const;

	// CRZ
	const math::vec GetPos() const;

private:
	btRigidBody* body = nullptr;

public:
	std::list<Module*>  collision_listeners;
	bool				is_sensor;
	float			    rotation;
};

#endif // __PhysBody3D_H__