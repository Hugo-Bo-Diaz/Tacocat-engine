#include "UI_About.h"

#include <stdlib.h>
#include <Windows.h>

UI_About::UI_About() 
{
	this->Enable(false);
	this->SetPos(ImVec2(200, 100));
}

UI_About::~UI_About()
{

}

void UI_About::Render()
{
	ImGui::SetNextWindowPos(getPos() , ImGuiSetCond_FirstUseEver);
	ImGui::Begin("About");
	ImGui::Text("TacocaT Engine");
	ImGui::Text("It's a palindrome");
	ImGui::Separator();
	ImGui::Text("By Hugo Bo and Daniel Garcia");
	ImGui::Separator();
	ImGui::Text("Made with:");
	//Add the modules used in the engine
	ImGui::Separator();
	if (ImGui::Button("Github")) ShellExecute(0, 0, "https://github.com/Hugo-Bo-Diaz/Tacocat-engine", 0, 0, SW_SHOW);
	ImGui::End();
}