#include "UI_Primitives.h"
#include "Application.h"

UI_Primitives::UI_Primitives()
{
	Enable(false);
	setPos(ImVec2(20, 20));

	Init();
}

UI_Primitives::~UI_Primitives()
{

}

void UI_Primitives::Render()
{
	ImGui::SetNextWindowPos(getPos(), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("Primitives", &enabled);
	ImGui::Text("Create Primitives");
	ImGui::DragFloat("Size", &size, 1.0f, 1.0f, 100.0f, "%.2f", 1.0f);
	ImGui::DragFloat2("Polygons", &pol, 1.0f, 3.0f, 20.0f, "%.0f", 1.0f);
	ImGui::DragFloat3("Position", &pos, 0.2f, -100.0f, 100.0f, "%.2f", 1.0f);
	if (ImGui::Button("Cube"))
	{
		App->renderer3D->AddCube(size, size, size, pos.x, pos.y, pos.z);
		Init();
	} ImGui::SameLine(50);
	if (ImGui::Button("Sphere"))
	{
		App->renderer3D->AddSphere(size, pol.x, pol.y, pos.x, pos.y, pos.z);
		Init();
	} ImGui::SameLine(105);
	if (ImGui::Button("Cylinder"))
	{
		App->renderer3D->AddCylinder(size, pol.x, pol.y, pos.x, pos.y, pos.z);
		Init();
	}
	ImGui::End();
}

void UI_Primitives::Init()
{
	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;

	size = 1.0f;

	pol.x = 3.0f;
	pol.y = 3.0f;
}
