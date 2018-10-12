#include "UI_Configuration.h"
#include "Application.h"

#include "SDL/include/SDL_cpuinfo.h"
#include "mmgr\mmgr.h"

UI_Configuration::UI_Configuration()
{
	Enable(true);
	setPos(ImVec2(20, 20));

	fps_app.resize(GRAPH_SIZE);
	ms_app.resize(GRAPH_SIZE);
	memory.resize(GRAPH_SIZE);
}

UI_Configuration::~UI_Configuration()
{

}

void UI_Configuration::Render()
{

	//int prevwidth = App->window->width;
	//int prevheight = App->window->height;

	ImGui::SetNextWindowPos(getPos(), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Configuration",&enabled);
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
		char title[25];
		sprintf_s(title, 25, "%.1f FPS", avg);
		ImGui::PlotHistogram("", &fps_app[0], fps_app.size(), 0, title, 0.0f, /*float(fps)*/100.0f, ImVec2(200, 100));

		//TODO: Plot histogram milliseconds
		ImGui::Text("Miliseconds");
		//char title[25];
		//sprintf_s(title, 25, "%.1f ms", avg);
		ImGui::PlotHistogram("", &ms_app[0], ms_app.size(), 0, NULL, 0.0f, /*float(fps)*/20.0f, ImVec2(200, 100));

		//TODO: Plot histogram memory consumption
		ImGui::Text("Memory");
		peak = store_memory();
		Calc_avg(memory);
		sprintf_s(title, 25, "%.1f MB", avg);
		ImGui::PlotHistogram("", &memory[0], memory.size(), 0, title, 0.0f, peak, ImVec2(200, 100));
	}
	for (std::list<Module*>::iterator it = App->list_modules.begin(); it != App->list_modules.end(); it++)
	{

		(*it)->Configuration();
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("Total Ram - %d GB", SDL_GetSystemRAM()/1024);
		ImGui::Text("Number of CPU cores - %d", SDL_GetCPUCount());
		if (SDL_HasRDTSC() == SDL_TRUE)
		{ImGui::Text("RDTSC"); ImGui::SameLine();}
		if (SDL_HasSSE() == SDL_TRUE)
		{ImGui::Text("SSE"); ImGui::SameLine();}
		if (SDL_HasSSE2() == SDL_TRUE)
		{ImGui::Text("SSE2"); ImGui::SameLine();}
		if (SDL_HasSSE3() == SDL_TRUE)
		{ImGui::Text("SSE3"); ImGui::SameLine();}
		if (SDL_HasSSE41() == SDL_TRUE)
		{ImGui::Text("SSE41"); ImGui::SameLine();}
		if (SDL_HasSSE42() == SDL_TRUE)
		{ImGui::Text("SSE42"); ImGui::SameLine();}
	}
	ImGui::End();


}

void UI_Configuration::store_app_FPS(float value)
{
	if (fps_app.capacity() != fps_app.size())
	{
		fps_app.push_back(value);
	}
	else
	{
		for (int val = 0; val < (GRAPH_SIZE - 1); val++)
		{
			fps_app[val] = fps_app[val + 1];
		}
		fps_app[fps_app.size() - 1] = value;
	}
}


void UI_Configuration::store_app_ms(float value)
{
	if (ms_app.capacity() != ms_app.size())
	{
		ms_app.push_back(value);
	}
	else
	{
		for (int val = 0; val < (GRAPH_SIZE - 1); val++)
		{
			ms_app[val] = ms_app[val + 1];
		}
		ms_app[ms_app.size() - 1] = value;
	}
}

float UI_Configuration::store_memory()
{
	sMStats stats = m_getMemoryStatistics();

	if (memory.capacity() != memory.size())
	{
		memory.push_back((float)stats.totalReportedMemory / 8192);
	}
	else
	{
		for (int val = 0; val < (GRAPH_SIZE - 1); val++)
		{
			memory[val] = memory[val + 1];
		}
		memory[memory.size() - 1] = (float)stats.totalReportedMemory / 8192;
	}

	return ((float)stats.peakReportedMemory / 8192) * 1.2f;
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