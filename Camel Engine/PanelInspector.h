#ifndef _PANEL_INSPECTOR_H_
#define _PANEL_INSPECTOR_H_

#include <vector>

#include "Panel.h"
#include "Application.h"

#include "ImGui/imgui.h"

struct TexData;
class GameObject;
class PanelInspector : public Panel
{
public:

	PanelInspector() {};
	PanelInspector(const char* name) : Panel(name) { App->saveLog("Inspector Panel Created"); };
	~PanelInspector() {};

	update_status Draw();

	void DisplayInspector();
	TexData* texture = nullptr;

public:
	bool GL_Texture_2D = true;

};
#endif // !_PANEL_INSPECTOR_H_
