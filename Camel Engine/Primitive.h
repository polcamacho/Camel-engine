
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
	Primitive_Pyramid,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis;
	uint id_for_buffer;
	uint id_for_vertex;

protected:
	PrimitiveTypes type;
	vec3 pos, sizes;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(vec3 position, vec3 size);
	void InnerRender(vec4 rotation) const;
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(vec3 position, float radius, int num_sectors, int num_stacks);
	void InnerRender(vec4 rotation) const;
public:
	float rad;
	int sectorCount, stackCount;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<int> indices;

	int numvertex;
	int numindices;
};

// ============================================
class Pyramid : public Primitive
{
public:
	Pyramid();
	Pyramid(vec3 position, vec3 size);
	void InnerRender(vec4 rotation) const;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(vec3 position, float radius, float height, int num_sectors);
	void InnerRender(vec4 rotation) const;
public:
	float rad;
	float height;
	int sectorCount;

	std::vector<float> vertices;
	std::vector<int>	indices;

	int numvertex;
	int numindices;
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