#include "Application.h"
#include "JSONDataController.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	engine_ui = new EngineUI(this, true);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	//Scene
	AddModule(scene_intro);

	//Engine UI
	AddModule(engine_ui);
	
	// Renderer last!
	AddModule(renderer3D);

	quit_engine = false;
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

	if(DoesFileExist("json_files/paths.json"))
		main_root = json_parse_file((std::string("json_files/paths.json")).data());
	else
	{
		LOG("Failed to load main json file...\nAborting...");
		ret = false;
	}

	T.d = true;
	T.Start();

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
void Application::QuitEngine()
{
	quit_engine = true;
}

inline bool Application::DoesFileExist(const std::string& file) {
	struct stat buffer;
	return (stat(file.c_str(), &buffer) == 0);
}