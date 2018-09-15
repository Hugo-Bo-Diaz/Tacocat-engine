#include "ModuleUI.h"
#include "Application.h"

#include "imgui-1.65/imgui.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleUI::~ModuleUI()
{

}

bool ModuleUI::Init()
{
	LOG("Initiating UI module");
	bool ret = true;

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
	if (draw_demo)
		ImGui::ShowDemoWindow(&draw_demo);

	/*for (std::list<UI*>::iterator it = UI_Elements.begin(); it != UI_Elements.end(); ++it)
	{
		if ((it*)->isEnabled())
		{
			(it*)->Render();
		}
	}*/

	return UPDATE_CONTINUE;
}