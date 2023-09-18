#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
	rotating = false;
	current_angle = 0;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Grey;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}


	CreateCarPart(vec3( info.chassis_size.x, info.chassis_size.y, info.chassis_size.z ), { info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z }, Red).Render();
	CreateCarPart(vec3( 1.8f,0.6,1.5f ), { 0, 1.25f,2.1f }, Red).Render();
	CreateCarPart(vec3( 0.5, 0.8,0.8 ), { 0, 2, 1.2 }, Grey).Render();
	CreateCarPart(vec3( 0.25f, 0.4f, 0.4f ), { 0.5f, 1.4f, 2.4f }, White).Render();
	CreateCarPart(vec3( 0.25f, 0.4f, 0.4f ), { -0.5f, 1.4f, 2.4f }, White).Render();
	CreateCarPart(vec3( 1.4, 1,1.5 ), { 0, 2, -0.2 }, White).Render();
	CreateCarPart(vec3( 0.1, 0.3,.3 ), { .6, 2.2, -1.5 }, Yellow).Render();
	CreateCarPart(vec3( 0.1, 0.3,.3 ), { -.6, 2.2, -1.5 }, Yellow).Render();
	CreateCarPart(vec3( 2, 0.07,.5 ), { 0, 2.35, -1.5 }, Yellow).Render();

}

Cube PhysVehicle3D::CreateCarPart(const vec3 size, const vec3 position, const Color color)
{
	Cube part(size.x, size.y, size.z);

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&part.transform);

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 m_offset(position.x, position.y, position.z);

	m_offset = m_offset.rotate(q.getAxis(), q.getAngle());

	part.transform.M[12] += m_offset.getX();
	part.transform.M[13] += m_offset.getY();
	part.transform.M[14] += m_offset.getZ();

	part.color = color;

	return part;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

void PhysVehicle3D::SmoothRotation(float target, vec3 s_axis) 
{
	rotating = true;
	target_angle = target;
	axis = s_axis;
}