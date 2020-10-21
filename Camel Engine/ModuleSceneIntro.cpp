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

	// Creating primitives
	c = new Cube({ 20,0,0 }, { 2,2,2 });
	s = new Sphere({ 2,0,2 }, 1.0f, 10, 10);

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

	// Primitives render
	c->InnerRender({ 1,1,0,45 });
	/*Pyramid pyr(5, 0, 0, 2, 4, 2);
	pyr.InnerRender();*/

	s->InnerRender({ 0,0,0,0 });

	/*Cylinder cy(10, 0, 0, 1.0f, 5.0f, 10);
	cy.InnerRender();*/

	return UPDATE_CONTINUE;
}