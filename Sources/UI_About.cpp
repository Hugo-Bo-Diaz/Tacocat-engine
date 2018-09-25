#include "UI_About.h"

#include "SDL\include\SDL_version.h"
#include <stdlib.h>
#include <Windows.h>
#include "Bullet/include/LinearMath/btScalar.h"

UI_About::UI_About() 
{
	Enable(false);
	setPos(ImVec2(200, 100));
}

UI_About::~UI_About()
{

}

void UI_About::Render()
{
	ImGui::SetNextWindowPos(getPos() , ImGuiSetCond_FirstUseEver);
	ImGui::Begin("About",&enabled);
	ImGui::Text("TacocaT Engine"); // Some help required to write it like this TɑcoɔɒT
	ImGui::Text("It's a palindrome");
	ImGui::Separator();
	ImGui::Text("By Hugo Bo and Daniel Garcia");
	ImGui::Separator();
	ImGui::Text("Made with:");
	//Add the resources used in the engine

	ImGui::Text("Dear ImGui - %s",ImGui::GetVersion());
	SDL_version version;
	SDL_GetVersion(&version);
	ImGui::Text("SDL        - %d.%d.%d",version.major,version.minor,version.patch);
	ImGui::Text("Bullet     - %d.%d", btGetVersion()/100, btGetVersion()- (btGetVersion()/100)*100);
	ImGui::Text("PCG        - help");
	
	ImGui::Separator();
	ImGui::Text("MIT License");
	if (ImGui::Button("License")) ShellExecute(0, 0, "https://github.com/Hugo-Bo-Diaz/Tacocat-engine/blob/master/LICENSE", 0, 0, SW_SHOW);
	ImGui::Separator();
	if (ImGui::Button("Github")) ShellExecute(0, 0, "https://github.com/Hugo-Bo-Diaz/Tacocat-engine", 0, 0, SW_SHOW);
	ImGui::End();
}