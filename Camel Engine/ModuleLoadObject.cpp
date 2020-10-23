#include "Globals.h"
#include "Application.h"
#include "ModuleLoadObject.h"
#include "glmath.h"
#include "glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")

ModuleLoadObject::ModuleLoadObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleLoadObject::~ModuleLoadObject()
{}
