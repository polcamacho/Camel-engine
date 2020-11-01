#pragma once
#include "Module.h"
#include "Globals.h"
#include <vector>





#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"


class Panel;
class PanelConsole;
class PanelAbout;
class PanelHierarchy;
class PanelInspector;

class EngineUI : public Module
{
private:
	bool is_fullscreen, is_resizable, is_borderless, wireframe;
	char name[30] = "Camel Engine";
	char name2[30] = "UPC CITM";

	int width, height;
public: 
	EngineUI(bool start_enabled = true);
	~EngineUI();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void MainMenu();

	void SystemWindow();
	void ConfigWindow();
	void Log(const char* fmt, ...);

	void CheckBoxOptions();
	void TextNames();
	void ScrollBarOptions();
	void HardwareDisplay();
	
public:

	PanelConsole* console_p;
	PanelAbout* about_p;
	PanelHierarchy* hierarchy_p;
	PanelInspector* inspector_p;
	

	bool console_window = false;
	bool about_window = false;
	bool hierarchy_window = false;
	bool inspector_window = false;
	

	std::vector<Panel*> panel_list;


};