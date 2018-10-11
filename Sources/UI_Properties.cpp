#include "UI_Properties.h"

UI_Properties::UI_Properties()
{

}

UI_Properties::~UI_Properties()
{

}

void UI_Properties::Render()
{
	ImGui::SetNextWindowPos(getPos(), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Properties", &enabled);

	//HUGO --> to put your things

	ImGui::End();
}