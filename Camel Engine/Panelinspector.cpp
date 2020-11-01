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

		ImGui::SameLine();


		if (ImGui::Button("delete", ImVec2(90, 20)))
		{
			std::vector<GameObject*>	childs = App->scene_intro->selected_game_obj->childs;
			if (!childs.empty())
			{
				for (std::vector<GameObject*>::iterator iter = childs.begin(); iter != childs.end(); ++iter)
				{
					App->scene_intro->root->DeleteChild(App->scene_intro->selected_game_obj);

				}
			}

			else
			{ 
				delete App->scene_intro->selected_game_obj;
				App->scene_intro->selected_game_obj = nullptr;
			}
		}
	}


	if (ImGui::CollapsingHeader("Mesh"))
	{
		if (App->scene_intro->selected_game_obj != nullptr)
		{
			for (std::vector<FullMesh*>::iterator iter = App->scene_intro->meshes.begin(); iter != App->scene_intro->meshes.end(); ++iter)
			{

				if ((*iter)->id == App->scene_intro->selected_game_obj->name.c_str())
				{
					ImGui::Text("Mesh information");
					ImGui::Separator();
					ImGui::Text("%i num_vertex", (*iter)->num_vertex);
					ImGui::Text("%i num_index", (*iter)->num_index);
				}
			}
		}
	}

	if (ImGui::CollapsingHeader("Texture"))
	{
		
		if (ImGui::Checkbox("Enable Texture", &GL_Texture_2D)) {}

		
		if (App->scene_intro->selected_game_obj != nullptr)
		{
			for (std::vector<FullMesh*>::iterator iter = App->scene_intro->meshes.begin(); iter != App->scene_intro->meshes.end(); ++iter)
			{
				if ((*iter)->id == App->scene_intro->selected_game_obj->name.c_str())
				{

					ImGui::Image((ImTextureID)(*(*iter)->parts.begin())->texture_id, ImVec2(150,150), ImVec2(0, 1), ImVec2(1, 0));

						
				}
			}
		}
	}



	ImGui::End();
}

