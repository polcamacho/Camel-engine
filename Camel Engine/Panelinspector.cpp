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

	if (App->scene_intro->selected_game_obj != nullptr)
	{
		static char go_name[20];
		memcpy(go_name, App->scene_intro->selected_game_obj->name.data(), 20);
		if (ImGui::InputText("Name", go_name, 20, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
		{
			App->scene_intro->selected_game_obj->name = go_name;
		}

		if (ImGui::Checkbox("hide", &App->scene_intro->selected_game_obj->active))
		{

			LOG("hide");
		}

		/*ImGui::SameLine();
		if (ImGui::Button("delete", ImVec2(90, 20)))
		{
			if (App->scene_intro->selected_game_obj->parent->type == OBJECT_TYPE::SUBPARENT)
			{
				App->scene_intro->root->RemoveSubChildGameObject(App->scene_intro->selected_game_obj);
			}

			else App->scene_intro->root->RemoveGameObject(App->scene_intro->selected_game_obj);

		}*/



	}




	ImGui::End();
}

