#include "Application.h"
#include "JSONDataController.h"




#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

Application::Application()
{
	window = new ModuleWindow(true);
	input = new ModuleInput(true);
	scene_intro = new ModuleSceneIntro(true);
	renderer3D = new ModuleRenderer3D(true);
	camera = new ModuleCamera3D(true);
	engine_ui = new EngineUI(true);
	load_object = new ModuleLoadObject(true);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	// Load Object
	AddModule(load_object);

	//Scene
	AddModule(scene_intro);

	//Engine UI
	AddModule(engine_ui);


	//Hardware

	// Renderer last!
	AddModule(renderer3D);


	is_vsync = true;
	caps_fps_ideal = 30;
	caps_fps_real = (1 / caps_fps_ideal) * 1000;
	quit_engine = false;
	Uint32 flags = SDL_RENDERER_ACCELERATED;
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		delete(*item);
		++item;
	}
	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	if(DoesJsonFileExist("json_files/paths.json"))
		main_root = json_parse_file((std::string("json_files/paths.json")).data());
	else
	{
		LOG("Failed to load main json file...\nAborting...");
		ret = false;
	}

	// Call Init() in all modules
	std::list<Module*>::iterator item= list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}
	p_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
	p_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	ms_now = p_timer.ReadMs();
	caps_fps_real = (1 / caps_fps_ideal) * 1000;
	if (is_vsync) {
		if (ms_now < caps_fps_real) {
			SDL_Delay(caps_fps_real - ms_now);
			
		}
	}
	//LOG("MS NOW: %.2f", ms_now);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		++item;
	}

	if (quit_engine)return update_status::UPDATE_STOP;

	FinishUpdate();
	return ret;
}

void Application::saveLog(const char* fmt, ...)
{
	log_saves.push_back(fmt);
}

bool Application::CleanUp()
{
	json_value_free(main_root);

	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::DrawEngineGraphics()
{
	fps_now = 1000 / ms_now;
	
	if (ms_vec.size() <= 80) {
		ms_vec.push_back(ms_now);
		fps_vec.push_back(fps_now);
	}
	else {
		ms_vec.erase(ms_vec.begin());
		ms_vec.push_back(ms_now);
		fps_vec.erase(fps_vec.begin());
		fps_vec.push_back(fps_now);
	}
	sprintf_s(graph_variable, 25, "Milliseconds %.1f", ms_vec[ms_vec.size() - 1]);
	ImGui::PlotHistogram("##milliseconds", &ms_vec[0], ms_vec.size(), 0, graph_variable, 0.0f, 100.0f, ImVec2(310, 100));
	sprintf_s(graph_variable, 25, "Framerate %.1f", fps_vec[fps_vec.size() - 1]);
	ImGui::PlotHistogram("##framerate", &fps_vec[0], fps_vec.size(), 0, graph_variable, 0.0f, 100.0f, ImVec2(310, 100));
}

void Application::Log(const char* fmt, ...)
{
	engine_ui->Log(fmt);
}

void Application::SetVsync()
{
	caps_fps_real = (1 / caps_fps_ideal) * 1000;
}

void Application::QuitEngine()
{
	quit_engine = true;
}


