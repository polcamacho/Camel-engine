#include "PanelAbout.h"
#include "EngineUI.h"

#include "ImGui/imgui.h"

update_status PanelAbout::Draw()
{
	if (App->engine_ui->about_window)
		DisplayAbout();
	return UPDATE_CONTINUE;
}

void PanelAbout::DisplayAbout()
{
	ImGui::Begin("About", &App->engine_ui->about_window);
	ImGui::BeginChild("scrolling");

	ImGui::Separator();

	ImGui::Text("Camel Engine %s", App->GetAppVersion());
	ImGui::Text("The old-new smoking engine");
	if (ImGui::Button("By Alexandru Cercel, Pol Camacho & Marc Rosell"))
		App->OpenWebsite("https://github.com/polcamacho/Camel-engine");

	ImGui::NewLine();

	ImGui::Text("3rd Party Libraries used");
	ImGui::BulletText("Glew 2.1.0");
	ImGui::BulletText("PCG Random Number Generation 0.98");
	ImGui::BulletText("MathGeoLib");
	ImGui::BulletText("ImGui 1.73");
	ImGui::BulletText("mmgr");
	ImGui::BulletText("gpudetect");
	ImGui::NewLine();

	ImGui::Text("%s", App->GetLicense());


	ImGui::EndChild();
	ImGui::End();
}
