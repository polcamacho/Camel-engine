#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "ModuleLoadObject.h"
#include "MeshHandler.h"
#include "GameObject.h"
#include <vector>

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	std::vector<MeshPart*>* AddMesh(const char* path);
	void CreateCheckersImage();
	void CreateCheckerBuffer(uint& id);

	void CreateGameObjectByDragging(const char* path);

public:
	Cube* c;
	Sphere* s;
	Pyramid* pyr;
	Cylinder* cyl;
	GameObject* root;
	GameObject* selected_game_obj;

	GLubyte checkerImage[64][64][4];
	//Meshes--
	std::vector<FullMesh*> meshes;
	//--------
	//std::vector<MeshPart*>* a;
};