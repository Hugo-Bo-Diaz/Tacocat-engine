#include "UI_Configuration.h"
#include "Globals.h"
#include "Application.h"

UI_Configuration::UI_Configuration()
{
	Enable(false);
	setPos(ImVec2(200, 100));

	fps_app.resize(GRAPH_SIZE);
}

UI_Configuration::~UI_Configuration()
{

}

void UI_Configuration::Render()
{
	ImGui::SetNextWindowPos(getPos(), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300,300));
	ImGui::Begin("Configuration");
	ImGui::Text("Options");
	if (ImGui::CollapsingHeader("Application"))
	{
		//Info from the application
		ImGui::Text(TITLE); ImGui::SameLine();
		ImGui::Text("App name");
		ImGui::Text(ORGANIZATION); ImGui::SameLine();
		ImGui::Text("Organization");
		const int fps = 0;
		//ImGui::SliderInt("MaxFPS", fps, 0, 120);
		//TODO: Make function to set the max fps
		ImGui::Text("Frames Per Second");
		Calc_avg(fps_app);
		fps_text = std::to_string(avg) + " FPS";
		char *c_app = new char[fps_text.size()];
		ImGui::PlotHistogram("", &fps_app[0], fps_app.size(), 0, c_app, 0.0f, /*float(fps)*/100.0f, ImVec2(200, 100));
		delete[] c_app;

		//TODO: Plot histogram milliseconds

		//TODO: Plot histogram memory consumption
	}
	if (ImGui::CollapsingHeader("Window"))
	{
		//Window options
	}
	if (ImGui::CollapsingHeader("File System"))
	{
		//File systems options
	}
	if (ImGui::CollapsingHeader("Input"))
	{
		//Input options
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		//Hardware info
	}
	ImGui::End();
}

void UI_Configuration::store_app_FPS(float value)
{
	fps_app.push_back(value);
}

void UI_Configuration::Calc_avg(std::vector<float> aux) {
	float num_values = 0;
	avg = 0;

	for (int counter = 0; counter < aux.size(); counter++) {
		avg += aux[counter];
		num_values++;
	}

	avg = avg / num_values;
}