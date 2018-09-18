#include "UI_Configuration.h"
#include "Globals.h"
#include "Application.h"

UI_Configuration::UI_Configuration()
{
	Enable(false);
	setPos(ImVec2(200, 100));
}

UI_Configuration::~UI_Configuration()
{

}

void UI_Configuration::Render()
{
	ImGui::SetNextWindowPos(getPos(), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Configuration");
	ImGui::Text("Options");
	if (ImGui::CollapsingHeader("Application"))
	{
		ImGui::Text(TITLE); ImGui::SameLine();
		ImGui::Text("App name");
		ImGui::Text(ORGANIZATION); ImGui::SameLine();
		ImGui::Text("Organization");
		const int fps = 0;
		ImGui::SliderInt("MaxFPS", fps, 0, 120);
		//TODO: Make function to set the max fps

	}
	ImGui::End();
}