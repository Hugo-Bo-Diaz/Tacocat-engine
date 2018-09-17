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
	LOG("Initiating UI module");
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	draw_demo = false;

	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	// Test window ------
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		draw_demo = !draw_demo;

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (draw_demo)
		ImGui::ShowDemoWindow(&draw_demo);

	/*for (std::list<UI*>::iterator it = UI_Elements.begin(); it != UI_Elements.end(); ++it)
	{
		if ((it*)->isEnabled())
		{
			(it*)->Render();
		}
	}*/

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