#include "PanelInspector.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "Component.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "EngineUI.h"


update_status PanelInspector::Draw()
{
	if (App->engine_ui->inspector_window)
		DisplayInspector();


	return UPDATE_CONTINUE;
}

void PanelInspector::DisplayInspector()
{
	ImGui::Begin("Inspector", &App->engine_ui->inspector_window);

	




	ImGui::End();
}

