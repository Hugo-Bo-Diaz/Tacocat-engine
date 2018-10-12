#include "UI_Properties.h"

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
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Properties", &enabled);

	//HUGO --> to put your things

	ImGui::End();
}