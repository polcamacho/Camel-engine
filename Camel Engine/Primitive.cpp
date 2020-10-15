
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

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
Cube::Cube() :Primitive()
{
	type = PrimitiveTypes::Primitive_Cube;
}

Cube::Cube(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ) : Primitive(), size(sizeX, sizeY, sizeZ), pos(posX, posY, posZ)
{
	type = PrimitiveTypes::Primitive_Cube;
}


void Cube::InnerRender() const
{	
	/*float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sx, -sy, sz);
	glVertex3f( sx, -sy, sz);
	glVertex3f( sx,  sy, sz);
	glVertex3f(-sx,  sy, sz);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f( sx, -sy, -sz);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx,  sy, -sz);
	glVertex3f( sx,  sy, -sz);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(sx, -sy,  sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx,  sy, -sz);
	glVertex3f(sx,  sy,  sz);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, -sy,  sz);
	glVertex3f(-sx,  sy,  sz);
	glVertex3f(-sx,  sy, -sz);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sx, sy,  sz);
	glVertex3f( sx, sy,  sz);
	glVertex3f( sx, sy, -sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f( sx, -sy, -sz);
	glVertex3f( sx, -sy,  sz);
	glVertex3f(-sx, -sy,  sz);

	glEnd();*/
	
	float vertex[24]{

			(size.x * 0.0f) + pos.x, (size.y * 0.0f) + pos.y, (size.z * 0.0f) + pos.z,
			(size.x * 0.0f) + pos.x, (size.y * 0.0f) + pos.y, (size.z * 1.0f) + pos.z,
			(size.x * 0.0f) + pos.x, (size.y * 1.0f) + pos.y, (size.z * 1.0f) + pos.z,
			(size.x * 0.0f) + pos.x, (size.y * 1.0f) + pos.y, (size.z * 0.0f) + pos.z,
			(size.x * 1.0f) + pos.x, (size.y * 0.0f) + pos.y, (size.z * 0.0f) + pos.z,
			(size.x * 1.0f) + pos.x, (size.y * 0.0f) + pos.y, (size.z * 1.0f) + pos.z,
			(size.x * 1.0f) + pos.x, (size.y * 1.0f) + pos.y, (size.z * 0.0f) + pos.z,
			(size.x * 1.0f) + pos.x, (size.y * 1.0f) + pos.y, (size.z * 1.0f) + pos.z,
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

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_for_buffer);
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void Cube::Size(float x, float y, float z)
{
	size.x = x;
	size.y = y;
	size.z = z;
}

// SPHERE ============================================
Sphere::Sphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

Sphere::Sphere(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

// PYRAMID ==================================

Pyramid::Pyramid(): Primitive()
{
	type = Primitive_Pyramid;
}

Pyramid::Pyramid(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ) :Primitive(), size(sizeX, sizeY, sizeZ), pos(posX, posY, posZ)
{
	type = Primitive_Pyramid;
}

void Pyramid::InnerRender() const 
{
	float vertex2[15]{
		
		// base
		(size.x * 0.0f) + pos.x, (size.y * 0.0f) + pos.y, (size.z * 0.0f) + pos.z,
		(size.x * 1.0f) + pos.x, (size.y * 0.0f) + pos.y, (size.z * 0.0f) + pos.z,
		(size.x * 0.0f) + pos.x, (size.y * 0.0f) + pos.y, (size.z * -1.0f) + pos.z,
		(size.x * 1.0f) + pos.x, (size.y * 0.0f) + pos.y, (size.z * -1.0f) + pos.z,

		// up vertex
		(size.x * 0.5f) + pos.x, (size.y * 1.0f) + pos.y, (size.z * -0.5f) + pos.z,
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

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_for_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_for_buffer);
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

}

// CYLINDER ============================================
Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

Cylinder::Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void Cylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();
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