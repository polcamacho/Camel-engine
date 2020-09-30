#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "EngineUI.h"

#include <list>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleSceneIntro* scene_intro;
	EngineUI* engine_ui;
	ModuleCamera3D* camera;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Timer T;

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};