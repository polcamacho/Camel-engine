#include "Application.h"
#include "Globals.h"
#include "EngineUI.h"
#include "ModuleWindow.h"

#include "imgui/imgui_internal.h"
#include "Assimp/Assimp/include/version.h"
#include "Devil/include/il.h"

#include <gl/GL.h>

#pragma comment (lib, "Assimp/Assimp/libx86/Assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")

EngineUI::EngineUI(Application* app, bool start_enabled) : Module(app, start_enabled) {
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	is_fullscreen = false, is_resizable = true, is_borderless = false, wireframe = false;
}

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
	ImGui::Begin("Test", (bool*)0);
	if (ImGui::CollapsingHeader("System Status")) {
		TextNames();
		if (ImGui::TreeNode("Graphics view")) {
			App->DrawEngineGraphics();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Screen options")) {
			CheckBoxOptions();
			ScrollBarOptions();
			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Hardware status")) {
			HardwareDisplay();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Object options")) {

			if (ImGui::Checkbox("Wireframe", &wireframe)) {
				if (wireframe) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}
			ImGui::TreePop();
		}
		//
	}
	ImGui::End();
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

void EngineUI::AboutWindow()
{
	ImGui::Begin("About");
	ImGui::End();
}

void EngineUI::ConfigWindow() 
{

}

void EngineUI::SystemWindow()
{

}

void EngineUI::TextNames()
{
	ImGui::InputText("App name", name, 30);
	ImGui::InputText("Organization", name2, 30);
}

void EngineUI::	CheckBoxOptions()
{
	ImGui::Checkbox("Fullscreen", &is_fullscreen);
	ImGui::SameLine(200);
	ImGui::Checkbox("Resizable", &is_resizable);
	ImGui::Checkbox("Borderless", &is_borderless);
	ImGui::SameLine(200);
	if (ImGui::Checkbox("Vsync", &App->is_vsync)) {
		App->SetVsync();
	}
	if (is_fullscreen) {
		SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
	}
	else {
		SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
	}
	if (is_resizable) {
		SDL_SetWindowResizable(App->window->window, SDL_TRUE);
	}
	else {
		SDL_SetWindowResizable(App->window->window, SDL_FALSE);
	}
	if (is_borderless) {
		SDL_SetWindowBordered(App->window->window, SDL_FALSE);
	}
	else {
		SDL_SetWindowBordered(App->window->window, SDL_TRUE);
	}

}

void EngineUI::ScrollBarOptions()
{
	SDL_SetWindowBrightness(App->window->window, App->window->brightness_value);
	SDL_SetWindowSize(App->window->window, width, height);
	ImGui::SliderFloat("Brightness", &App->window->brightness_value, 0.0f, 1.0f);
	ImGui::SliderInt("Width", &width, 360, 1920);
	ImGui::SliderInt("Height", &height, 720, 1080);
}

void EngineUI::HardwareDisplay()
{
	int SDL_major_version = SDL_MAJOR_VERSION, SDL_minor_version = SDL_MINOR_VERSION, SDL_pach_level = SDL_PATCHLEVEL;
	int GL_major_version = GLEW_VERSION_MAJOR, GL_minor_version = GLEW_VERSION_MINOR, GL_pach_level = GLEW_VERSION_MICRO;
	int Assimp_major_version = aiGetVersionMajor(), Assimp_minor_version = aiGetVersionMinor(), Assimp_revision = aiGetVersionRevision();
	int Devil_version = ilGetInteger(IL_VERSION_NUM);

	ImGui::Text("SDL Version: ");
	ImGui::SameLine(200);
	ImGui::TextColored({ 1, 1, 0, 100 }, "%d.%d.%d", SDL_major_version, SDL_minor_version, SDL_pach_level);

	ImGui::Text("OpenGL Version: ");
	ImGui::SameLine(200);
	ImGui::TextColored({ 1, 1, 0, 100 }, "%d.%d.%d", GL_major_version, GL_minor_version, GL_pach_level);

	ImGui::Text("Assimp Version: ");
	ImGui::SameLine(200);
	ImGui::TextColored({ 1, 1, 0, 100 }, "%d.%d.%d", Assimp_major_version, Assimp_minor_version, Assimp_revision);

	ImGui::Text("Devil Version: ");
	ImGui::SameLine(200);
	ImGui::TextColored({ 1, 1, 0, 100 }, "%d.%d.%d", (Devil_version / 100), (Devil_version / 10) - ((Devil_version / 100) * 10), Devil_version- ((Devil_version / 10) * 10));

	ImGui::Text("CPU cores: ");
	ImGui::SameLine(200);
	ImGui::TextColored({ 1, 1, 0, 100 }, "%d", SDL_GetCPUCount());

	ImGui::Text("Cache: ");
	ImGui::SameLine(200);
	ImGui::TextColored({ 1, 1, 0, 100 }, "%d KB", SDL_GetCPUCacheLineSize());

	ImGui::Text("RAM: ");
	ImGui::SameLine(200);
	ImGui::TextColored({ 1, 1, 0, 100 }, "%d GB", SDL_GetSystemRAM()/1000);

	ImGui::Text("Caps:");
	ImGui::SameLine();

	if (SDL_Has3DNow()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "3DNow,");
		ImGui::SameLine();
	}
	if (SDL_HasRDTSC()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "RDTSC,");
		ImGui::SameLine();
	}
	if (SDL_HasMMX()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "MMX,");
		ImGui::SameLine();
	}
	if (SDL_HasSSE()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "SSE,");
		ImGui::SameLine();
	}
	if (SDL_HasSSE2()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "SSE2,");
		ImGui::SameLine();
	}
	if (SDL_HasSSE3()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "SSE3,");
		ImGui::SameLine();
	}
	if (SDL_HasSSE41()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "SSE41,");
	}
	if (SDL_HasSSE42()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "SSE42,");
		ImGui::SameLine();
	}
	if (SDL_HasAVX()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "AVX,");
		ImGui::SameLine();
	}
	if (SDL_HasAVX2()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "AVX2,");
		ImGui::SameLine();
	}
	if (SDL_HasAltiVec()) {
		ImGui::TextColored({ 1, 1, 0, 100 }, "AltiVec,");
		ImGui::SameLine();
	}
}