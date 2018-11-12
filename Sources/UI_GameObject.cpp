#include "UI_GameObject.h"
#include "Application.h"

UI_GameObject::UI_GameObject()
{
	Enable(false);
	setPos(ImVec2(20, 20));
}

UI_GameObject::~UI_GameObject()
{
}

void UI_GameObject::Render()
{
	ImGui::SetNextWindowPos(getPos(), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Hierarchy", &enabled);
	if (ImGui::CollapsingHeader("Scene"))
	{
		for (std::vector<GameObject*>::iterator it = App->scene_controller->current_scene->GameObjects.begin(); it != App->scene_controller->current_scene->GameObjects.end(); it++)
		{
			(*it)->Hierarchy();
		}
	}
	ImGui::End();
}
