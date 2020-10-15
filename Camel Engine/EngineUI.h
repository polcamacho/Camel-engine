#pragma once
#include "Module.h"
#include "Globals.h"


#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_sdl.h"
#include "imgui/examples/imgui_impl_opengl3.h"

class EngineUI : public Module
{
private:
	bool is_fullscreen = false, is_resizable = true, is_borderless = false;
	char name[30] = "Camel Engine";
	char name2[30] = "UPC CITM";

	int width, height;
public: 
	EngineUI(Application* app, bool start_enabled = true);
	~EngineUI();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void MainMenu();
	void CheckBoxOptions();
	void TextNames();
	void ScrollBarOptions();
	void HardwareDisplay();

};