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

#include <list>
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

	bool is_vsync;
	int caps_fps_ideal;
	int caps_fps_real;
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
	Uint32 flags;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void DrawEngineGraphics();
	void QuitEngine();
	void SetVsync();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};