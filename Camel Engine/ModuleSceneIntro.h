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

	void CreateGameObjectByDragging(const char* path);
	void SetTextureDragging(uint &tex_id);

public:
	CubeP* c;
	SphereP* s;
	PyramidP* pyr;
	CylinderP* cyl;
	GameObject* root;

	//Meshes--
	std::vector<FullMesh*> meshes;
	//--------
	//std::vector<MeshPart*>* a;
};