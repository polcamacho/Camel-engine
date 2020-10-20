
#pragma once
#include "glmath.h"
#include "Color.h"

#include <vector>

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Triangle_pyramid,
	Primitive_Pyramid,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire;
	uint id_for_buffer;
	uint id_for_vertex;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(int posX, int posY, int posZ, int sizeX, int sizeY, int sizeZ);
	void InnerRender() const;
	void Size(float x, float y, float z);
public:
	vec3 size, pos;
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(int posX, int posY, int posZ, float radius, int num_sectors, int num_stacks);
	void InnerRender() const;
public:
	float radius;
	int sectorCount, stackCount;
	vec3 pos;
};

// ============================================
class Pyramid : public Primitive
{
public:
	Pyramid();
	Pyramid(int posX, int posY, int posZ, int sizeX, int sizeY, int sizeZ);
	void InnerRender() const;
public:
	vec3 size, pos;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float posX, float posY, float posZ, float radius, float height, int num_sectors);
	void InnerRender() const;
public:
	float radius;
	float height;
	int sectorCount;
	vec3 pos;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane();
	Plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};