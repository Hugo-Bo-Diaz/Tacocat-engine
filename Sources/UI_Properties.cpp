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
	if (ImGui::CollapsingHeader("Model"))
	{

		for (int aux = 0; aux < App->scene_controller->current_scene->GameObjects.size(); aux++)
		{
			if (App->scene_controller->current_scene->GameObjects[aux]->Iselected())
			{
				//if (!App->scene_controller->current_scene->GameObjects[aux]->components.empty()) {
					for (std::list<Component*>::iterator it = App->scene_controller->current_scene->GameObjects[aux]->components.begin(); it != App->scene_controller->current_scene->GameObjects[aux]->components.end(); it++)
					{
						(*it)->Properties();
					}
					//break; //Only first selected object will be displayed
				//}
			}
		}
	}
	if (ImGui::CollapsingHeader("Texture"))
	{
		//App->renderer3D->TexProperties();
	}
	ImGui::End();
}