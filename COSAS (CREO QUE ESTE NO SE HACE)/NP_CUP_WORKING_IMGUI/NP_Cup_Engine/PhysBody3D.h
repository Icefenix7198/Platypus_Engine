#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "glmath.h"
#include "Primitive.h"

class btRigidBody;
class Module;

enum SensorType {

	CHECKPOINT,
	GRAVITYMOD,
	FINISHLINE,
	SPEEDBOOST,
	RESPAWN,
};

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
	bool IsSensor();

protected:
	bool is_sensor;

protected:
	btRigidBody* body = nullptr;

public:
	p2List<Module*> collision_listeners;
};

// =================================================
struct PhysSensor3D : public PhysBody3D
{

public:
	PhysSensor3D(btRigidBody* body, SensorType s_type);
	~PhysSensor3D();

	void SetAsSensor(bool is_sensor);

public:
	SensorType type;
	vec3 gravityMod;
	vec4 targetRot;
	bool isEnabled;

	Primitive* lights[2];
};

#endif // __PhysBody3D_H__