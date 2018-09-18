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
	ImGui::Text("TacocaT Engine"); // Some help required to write it like this TɑcoɔɒT
	ImGui::Text("It's a palindrome");
	ImGui::Separator();
	ImGui::Text("By Hugo Bo and Daniel Garcia");
	ImGui::Separator();
	ImGui::Text("Made with:");
	//Add the modules used in the engine
	ImGui::Text("Dear ImGui - 1.65");
	ImGui::Text("SDL        - 2.0");
	ImGui::Text("Bullet     - 2.84");
	ImGui::Text("Glut       - 3.7");
	ImGui::Separator();
	ImGui::Text("MIT License");
	if (ImGui::Button("License")) ShellExecute(0, 0, "https://github.com/Hugo-Bo-Diaz/Tacocat-engine/blob/master/LICENSE", 0, 0, SW_SHOW);
	ImGui::Separator();
	if (ImGui::Button("Github")) ShellExecute(0, 0, "https://github.com/Hugo-Bo-Diaz/Tacocat-engine", 0, 0, SW_SHOW);
	ImGui::End();
}