
#include "Globals.h"
#include "glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{
	id_for_buffer = 0;
	id_for_vertex = 0;
}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if(axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// CUBE ============================================
Cube::Cube() :Primitive()
{
	type = PrimitiveTypes::Primitive_Cube;
}

Cube::Cube(vec3 position, vec3 size) : Primitive()
{
	type = PrimitiveTypes::Primitive_Cube;
	
	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	sizes.x = size.x;
	sizes.y = size.y;
	sizes.z = size.z;

	float vertex[24]{

			(sizes.x * 0.0f) + pos.x, (sizes.y * 0.0f) + pos.y, (sizes.z * 0.0f) + pos.z,
			(sizes.x * 0.0f) + pos.x, (sizes.y * 0.0f) + pos.y, (sizes.z * 1.0f) + pos.z,
			(sizes.x * 0.0f) + pos.x, (sizes.y * 1.0f) + pos.y, (sizes.z * 1.0f) + pos.z,
			(sizes.x * 0.0f) + pos.x, (sizes.y * 1.0f) + pos.y, (sizes.z * 0.0f) + pos.z,
			(sizes.x * 1.0f) + pos.x, (sizes.y * 0.0f) + pos.y, (sizes.z * 0.0f) + pos.z,
			(sizes.x * 1.0f) + pos.x, (sizes.y * 0.0f) + pos.y, (sizes.z * 1.0f) + pos.z,
			(sizes.x * 1.0f) + pos.x, (sizes.y * 1.0f) + pos.y, (sizes.z * 0.0f) + pos.z,
			(sizes.x * 1.0f) + pos.x, (sizes.y * 1.0f) + pos.y, (sizes.z * 1.0f) + pos.z,
	};

	int indices[36]{

		//front
		0, 1, 2,
		2, 3, 0,

		//right
		1, 5, 7,
		7, 2, 1,

		//left
		4, 0, 3,
		3, 6, 4,

		//back
		5, 4, 6,
		6, 7, 5,

		//up
		3, 2, 7,
		7, 6, 3,

		//down
		1, 0, 4,
		4, 5, 1
	};

	glGenBuffers(1, (GLuint*)&(id_for_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertex, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(id_for_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, id_for_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 36, indices, GL_STATIC_DRAW);
}


void Cube::InnerRender(vec4 rotation) const
{	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_for_buffer);
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	//glRotatef(rotation.w, rotation.x, rotation.y, rotation.z);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// SPHERE ============================================
Sphere::Sphere() : Primitive()
{
	type = PrimitiveTypes::Primitive_Sphere;
}

Sphere::Sphere(vec3 position, float radius, int num_sectors, int num_stacks) : Primitive(), rad(radius), sectorCount (num_sectors), stackCount (num_stacks)
{
	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	sectorCount = num_sectors;
	stackCount = num_stacks;
	rad = radius;

	type = PrimitiveTypes::Primitive_Sphere;
	// CREATING VERTICES =============================================

	std::vector<float>().swap(vertices);
	std::vector<float>().swap(normals);
	std::vector<float>().swap(texCoords);
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / rad;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - (i * stackStep);        // starting from pi/2 to -pi/2
		xy = rad * cosf(stackAngle);             // r * cos(u)
		z = rad * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(x + pos.x);
			vertices.push_back(y + pos.y);
			vertices.push_back(z + pos.z);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	// CREATING INDICES =============================================

	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	// Transform indices and vertices to float,
	// so that buffers can manage them

	numvertex = vertices.size();
	float* new_vertices = new float[numvertex];
	numindices = indices.size();
	int* new_indices = new int[numindices];

	for (size_t i = 0; i < numvertex; i++)
	{
		new_vertices[i] = vertices[i];
	}

	for (size_t i = 0; i < numindices; i++)
	{
		new_indices[i] = indices[i];
	}

	glGenBuffers(1, (GLuint*)&(id_for_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numvertex, new_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(id_for_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_for_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numindices, new_indices, GL_STATIC_DRAW);
}

void Sphere::InnerRender(vec4 rotation) const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_for_buffer);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	//glRotatef(rotation.w, rotation.x, rotation.y, rotation.z);
	glDrawElements(GL_TRIANGLES, numindices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// PYRAMID ==================================

Pyramid::Pyramid(): Primitive()
{
	type = Primitive_Pyramid;
}

Pyramid::Pyramid(vec3 position, vec3 size) :Primitive()
{
	type = Primitive_Pyramid;
	sizes.x = size.x;
	sizes.y = size.y;
	sizes.z = size.z;

	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	float vertex2[15]{

		// base
		(sizes.x * 0.0f) + pos.x, (sizes.y * 0.0f) + pos.y, (sizes.z * 0.0f) + pos.z,
		(sizes.x * 1.0f) + pos.x, (sizes.y * 0.0f) + pos.y, (sizes.z * 0.0f) + pos.z,
		(sizes.x * 0.0f) + pos.x, (sizes.y * 0.0f) + pos.y, (sizes.z * -1.0f) + pos.z,
		(sizes.x * 1.0f) + pos.x, (sizes.y * 0.0f) + pos.y, (sizes.z * -1.0f) + pos.z,

		// up vertex
		(sizes.x * 0.5f) + pos.x, (sizes.y * 1.0f) + pos.y, (sizes.z * -0.5f) + pos.z,
	};

	uint indices2[18]{

		//Front
		0,1,4,

		//Right
		2,0,4,

		//Left
		1,3,4,

		//Back
		3,2,4,

		//Down
		0,3,1,
		0,2,3,
	};
	glGenBuffers(1, (GLuint*)&(id_for_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertex2, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(id_for_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, id_for_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 36, indices2, GL_STATIC_DRAW);
}

void Pyramid::InnerRender(vec4 rotation) const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_for_buffer);
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	//glRotatef(rotation.w, rotation.x, rotation.y, rotation.z);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

}

// CYLINDER ============================================
Cylinder::Cylinder() : Primitive()
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

Cylinder::Cylinder(vec3 position, float radius, float height, int num_sectors) : Primitive(), rad(radius), height(height), sectorCount(num_sectors)
{
	type = PrimitiveTypes::Primitive_Cylinder;

	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	// GENERATE CIRCLE FROM XY PLANE
	
	float sectorStep = 2 * PI / sectorCount;
	float sectorAngle;  // radian

	std::vector<float> unitCircleVertices;
	for (int i = 0; i <= sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(cos(sectorAngle)); // x
		unitCircleVertices.push_back(sin(sectorAngle)); // y
		unitCircleVertices.push_back(0);                // z
	}

	// clear memory of prev arrays
	std::vector<float>().swap(vertices);

	// get unit circle vectors on XY-plane
	std::vector<float> unitVertices = unitCircleVertices;

	// put side vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float t = 1.0f - i;                              // vertical tex coord; 1 to 0

		for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			float uz = unitVertices[k + 2];
			// position vector
			vertices.push_back((ux * rad)+pos.x);             // vx
			vertices.push_back((uy * rad)+pos.y);             // vy
			vertices.push_back(h + pos.z);                       // vz

		}
	}

	// the starting index for the base/top surface
	//NOTE: it is used for generating indices later
	int baseCenterIndex = (int)vertices.size() / 3;
	int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

	// put base and top vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
		float nz = -1 + i * 2;                           // z value of normal; -1 to 1

		// center point
		vertices.push_back(0 + pos.x);     vertices.push_back(0 + pos.y);     vertices.push_back(h + pos.z);


		for (int j = 0, k = 0; j < sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			// position vector
			vertices.push_back((ux * rad) + pos.x);             // vx
			vertices.push_back((uy * rad) + pos.y);             // vy
			vertices.push_back(h + pos.z);                       // vz

		}
	}

	int k1 = 0;                         // 1st vertex index at base
	int k2 = sectorCount + 1;           // 1st vertex index at top

	// indices for the side surface
	for (int i = 0; i < sectorCount; ++i, ++k1, ++k2)
	{
		// 2 triangles per sector
		// k1 => k1+1 => k2
		indices.push_back(k1);
		indices.push_back(k1 + 1);
		indices.push_back(k2);

		// k2 => k1+1 => k2+1
		indices.push_back(k2);
		indices.push_back(k1 + 1);
		indices.push_back(k2 + 1);
	}

	// indices for the base surface
	//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
	//      please see the previous code snippet
	for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(k + 1);
			indices.push_back(k);
		}
		else // last triangle
		{
			indices.push_back(baseCenterIndex);
			indices.push_back(baseCenterIndex + 1);
			indices.push_back(k);
		}
	}

	// indices for the top surface
	for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
	{
		if (i < sectorCount - 1)
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(k + 1);
		}
		else // last triangle
		{
			indices.push_back(topCenterIndex);
			indices.push_back(k);
			indices.push_back(topCenterIndex + 1);
		}
	}

	numvertex = vertices.size();
	float* new_vertices = new float[numvertex];
	numindices = indices.size();
	int* new_indices = new int[numindices];

	for (size_t i = 0; i < numvertex; i++)
	{
		new_vertices[i] = vertices[i];
	}

	for (size_t i = 0; i < numindices; i++)
	{
		new_indices[i] = indices[i];
	}


	glGenBuffers(1, (GLuint*)&(id_for_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numvertex, new_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(id_for_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_for_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * numindices, new_indices, GL_STATIC_DRAW);
}

void Cylinder::InnerRender(vec4 rotation) const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_for_buffer);
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	//glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(pos.x, pos.y, pos.z);
	glDrawElements(GL_TRIANGLES, numindices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

// LINE ==================================================
Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void Line::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
Plane::Plane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void Plane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}