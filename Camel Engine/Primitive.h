
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
class CubeP : public Primitive
{
public :
	CubeP();
	CubeP(vec3 position, vec3 size);
	void InnerRender(vec4 rotation) const;
};

// ============================================
class SphereP : public Primitive
{
public:
	SphereP();
	SphereP(vec3 position, float radius, int num_sectors, int num_stacks);
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
class PyramidP : public Primitive
{
public:
	PyramidP();
	PyramidP(vec3 position, vec3 size);
	void InnerRender(vec4 rotation) const;
};

// ============================================
class CylinderP : public Primitive
{
public:
	CylinderP();
	CylinderP(vec3 position, float radius, float height, int num_sectors);
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
class LineP : public Primitive
{
public:
	LineP();
	LineP(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class PlaneP : public Primitive
{
public:
	PlaneP();
	PlaneP(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};