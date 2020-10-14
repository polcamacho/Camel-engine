#pragma once
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "EngineUI.h"
#include "parson/parson.h"

#include <list>
#include <string>
#include <vector>

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleSceneIntro* scene_intro;
	EngineUI* engine_ui;
	ModuleCamera3D* camera;

	JSON_Value* main_root;

private:

	Timer	ms_timer;
	j1PerfTimer p_timer;
	float	dt;

	float ms_now;
	float fps_now;

	std::list<Module*> list_modules;
	std::vector<float> fps_vec;
	std::vector<float> ms_vec;
	char graph_variable[50];

	bool quit_engine;

public:

	Timer T;

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void DrawEngineGraphics();
	void QuitEngine();

	bool DoesFileExist(const std::string& file);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};