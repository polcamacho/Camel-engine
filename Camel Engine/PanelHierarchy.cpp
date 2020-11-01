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

	RecurisveTreeNode(App->scene_intro->root);

	ImGui::End();

}

void PanelHierarchy::RecurisveTreeNode(GameObject* parent)
{
	ImGuiTreeNodeFlags base_flags = /*ImGuiTreeNodeFlags_OpenOnArrow |*/ ImGuiTreeNodeFlags_DefaultOpen /*| ImGuiTreeNodeFlags_SpanAllAvailWidth // this flags ins't on*/;


	if (parent == App->scene_intro->selected_game_obj)
		base_flags = ImGuiTreeNodeFlags_Selected;

	if (parent->name.c_str() != nullptr && ImGui::TreeNodeEx(parent, base_flags, parent->name.c_str()))
	{
		if (ImGui::IsItemClicked())
			App->scene_intro->selected_game_obj = parent;

		std::vector<GameObject*>::const_iterator iter = parent->childs.begin(); //const_iterator just for reading
		while (iter != parent->childs.end())
		{
			
			RecurisveTreeNode((*iter));
			++iter;
		}
		ImGui::TreePop();


	}
}