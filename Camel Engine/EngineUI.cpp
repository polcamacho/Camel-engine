#include "Application.h"
#include "Globals.h"
#include "EngineUI.h"

#include "imgui/imgui_internal.h"

#include <gl/GL.h>

EngineUI::EngineUI(Application* app, bool start_enabled) : Module(app, start_enabled) {}

EngineUI::~EngineUI() {}

bool EngineUI::Start()
{
	LOG("Loading Engine UI...");
	bool ret = true;

	// Initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls ImGuiWindowFlags_MenuBar

	io.DisplaySize.x = 1280.0f;
	io.DisplaySize.y = 720.0f;
	io.IniFilename = "imgui.ini";

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);

	return ret;
}

bool EngineUI::CleanUp()
{
	LOG("Unloading Engine UI...");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

update_status EngineUI::PreUpdate(float dt)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

// Update
update_status EngineUI::Update(float dt)
{
	bool show_demo_wndow = true;
	ImGui::ShowDemoWindow(&show_demo_wndow);

	MainMenu();

	return UPDATE_CONTINUE;
}

update_status EngineUI::PostUpdate(float dt)
{

	ImGui::Render();

	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.64f);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);

	// If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!), 
	// you may need to backup/reset/restore current shader using the commented lines below.
	//GLint last_program; 
	//glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	//glUseProgram(0);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//glUseProgram(last_program);

	return UPDATE_CONTINUE;
}

void EngineUI::MainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit Engine"))
				App->QuitEngine();

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
			{
				ImGui::Begin("About", (bool*)0);
				ImGui::End();
			}


			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}