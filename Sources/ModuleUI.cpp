#include "ModuleUI.h"
#include "Application.h"

#include "imgui-1.65/imgui.h"
#include "imgui-1.65/imgui_impl_sdl.h"
#include "imgui-1.65/imgui_impl_opengl2.h"

ModuleUI::ModuleUI(bool start_enabled) : Module( start_enabled)
{

}

ModuleUI::~ModuleUI()
{

}

bool ModuleUI::Init()
{
	CONSOLE_LOG("Initiating UI module");
	bool ret = true;

	CONSOLE_LOG("Initiating dear ImGui");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	CONSOLE_LOG("Creating UI components")
	draw_demo = false;
	about = new UI_About();
	config = new UI_Configuration();

	UI_Elements.push_back(about);
	UI_Elements.push_back(config);

	return ret;
}

update_status ModuleUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// Test window ------
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		draw_demo = !draw_demo;

	// Draw menu bar ----
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File", &draw_menu))
		{
			if (ImGui::MenuItem("Save"))
			{
			}
			if (ImGui::MenuItem("Load"))
			{
			}
			if (ImGui::MenuItem("Options"))
			{
			}
			if (ImGui::MenuItem("Exit"))
			{
				App->Close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration"))
				config->Enable(!config->isEnabled());
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
				about->Enable(!about->isEnabled());

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (draw_demo)
		ImGui::ShowDemoWindow(&draw_demo);
	
	for (std::list<UI*>::iterator it = UI_Elements.begin(); it != UI_Elements.end(); ++it)
	{
		if ((*it)->isEnabled())
		{
			(*it)->Render();
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}