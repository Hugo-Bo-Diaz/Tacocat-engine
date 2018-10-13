#include "UI_Properties.h"
#include "Application.h"

UI_Properties::UI_Properties()
{
	Enable(false);
	setPos(ImVec2(20, 20));
}

UI_Properties::~UI_Properties()
{

}

void UI_Properties::Render()
{
	ImGui::SetNextWindowPos(getPos(), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Properties", &enabled);
	if (ImGui::CollapsingHeader("Info"))
	{
		App->renderer3D->Properties();
	}
	ImGui::End();
}