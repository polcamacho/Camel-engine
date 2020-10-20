#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include <gl/GL.h>
#include "glew/include/glew.h"
#include <gl/GLU.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");


	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	Cube c(0, 0, 0, 2, 2, 2);
	c.InnerRender();
	Pyramid pyr(5, 0, 0, 2, 4, 2);
	pyr.InnerRender();

	Sphere s(10, 0, 0, 2.0f, 10, 10);
	s.InnerRender();

	Cylinder cy(10, 0, 0, 1.0f, 5.0f, 10);
	cy.InnerRender();

	return UPDATE_CONTINUE;
}