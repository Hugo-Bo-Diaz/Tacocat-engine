#include "UI_Time.h"
#include "Application.h"

UI_Time::UI_Time()
{
	Enable(true);
	setPos(ImVec2(20, 20));
}

UI_Time::~UI_Time()
{

}

void UI_Time::Render()
{
	ImGui::SetNextWindowPos(getPos(), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Time", &enabled);
	if (ImGui::Button(">", ImVec2(20, 20)))
	{
		App->custom_dt = 1.0f;
		Save = App->scene_controller->current_scene->GameObjects;
	}
	ImGui::SameLine();
	if (ImGui::Button("||", ImVec2(20, 20)))
		App->custom_dt = 0.0f;
	ImGui::SameLine();
	if (ImGui::Button("[]", ImVec2(20, 20)))
	{
		App->custom_dt = 1.0f;
		App->scene_controller->current_scene->GameObjects = Save;
	}

	ImGui::End();
}
