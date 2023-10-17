#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "MathGeoLib\MathGeoLib.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const math::vec &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	math::float4x4 transform;			// With glmath.h was mat4x4
	bool axis, wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public:
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	math::vec size;
};

// ============================================
//class Sphere : public Primitive
//{
//public:
//	Sphere();
//	Sphere(float radius);
//	void InnerRender() const;
//public:
//	float radius;
//};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);
	void InnerRender() const;
public:
	math::vec origin;
	math::vec destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane();
	Plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	math::vec normal;
	float constant;
};

#endif //!__PRIMITIVE_H__