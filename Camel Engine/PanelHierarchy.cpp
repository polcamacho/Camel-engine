#include "PanelHierarchy.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "EngineUI.h"



update_status PanelHierarchy::Draw()
{



	if (App->engine_ui->hierarchy_window)
		DisplayHierarchy();
	return UPDATE_CONTINUE;
}

void PanelHierarchy::DisplayHierarchy()
{
	ImGui::Begin("Hierarchy", &App->engine_ui->hierarchy_window);

	

	ImGui::End();

}